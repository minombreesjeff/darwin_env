/* $Id: qrpsb.c,v 1.6 2001/11/13 19:45:47 bauer Exp $
*===========================================================================
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
* File Name:  qrpsb.c
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 11/27/2000
*
* $Revision: 1.6 $
*
* File Description:
*         WWW-RPS BLAST using the BLAST queue
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: qrpsb.c,v $
* Revision 1.6  2001/11/13 19:45:47  bauer
* restricted number of automated queue re-checks
*
* Revision 1.5  2001/05/31 22:04:46  bauer
* changes to accomodate new type of Smart accessions
*
* Revision 1.4  2001/03/21 17:00:48  bauer
* fixes for changes in LOAD accessions
*
* Revision 1.3  2001/03/08 21:33:14  bauer
* updated graphics display
*
* Revision 1.2  2001/01/11 21:55:36  bauer
* disabled option for search heuristics
*
* Revision 1.1  2000/12/08 20:32:48  bauer
* initial version of the CD-search server using the BLAST queue
*
*
* ==========================================================================
*/

#define  BLASTCLI_BUF_SIZE 255
#include <sequtil.h>
#include <prtutil.h>
#include <tofasta.h>
#include <objblst3.h>
#include <netblap3.h>
#include <blastpri.h>
#include <dust.h>
#include <txalign.h>
#include <accentr.h>
#include <sqnutils.h>
#include <wrpsb.h>
#include <accid1.h>
#include <ddvcreate.h>
#include <wrpsbtool.h>
#include <www.h>

/*---------------------------------------------------------------------------*/
/* USE_PNG is the switch determining whether graphics are stored as PNG-file */
/* or as GIFs (the latter may be more compatible across different browsers(  */
/*---------------------------------------------------------------------------*/
#ifndef USE_PNG
#include <gifgen.h>
#endif

#ifdef USE_PNG
#include <gd.h>
#include <gdfontmb.h>
#include <gdfontt.h>
#endif

/*---------------------------------------------------------------------------*/
/* defines to specify runtime mode                                           */
/*---------------------------------------------------------------------------*/
#define FRONTPAGE 0
#define SUBMITJOB 1
#define FORMATJOB 2

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* read parameters from configuration file                                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean CddGetParams()
{
  URLBase[0] = URLcgi[0] = ENTREZurl[0] = DOCSUMurl[0] = MAILto[0] = '\0';
  MMDBpath[0] = gunzip[0] = '\0';

  GetAppParam("cdd", "CDDSRV", "URLBase", "", URLBase, PATH_MAX);
  if (URLBase[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no URLBase...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "URLcgi", "", URLcgi, PATH_MAX);
  if (URLcgi[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no URLcgi...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "ENTREZurl", "", ENTREZurl, PATH_MAX);
  if (ENTREZurl[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no ENTREZurl...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "DOCSUMurl", "", DOCSUMurl, PATH_MAX);
  if (DOCSUMurl[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no DOCSUMurl...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "Gunzip", "", gunzip, (size_t) 256*(sizeof(Char)));
  if (gunzip[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no Gunzip...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "Database", "", MMDBpath, PATH_MAX);
  if (MMDBpath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"MMDB config file\nMMDBSRV section has no Database...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "MAILto", "", MAILto, PATH_MAX);
  if (MAILto[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no MAILto...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "DATApath", "", DATApath, PATH_MAX);
  if (DATApath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no VAST Data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDatabase", "", CDDdpath, PATH_MAX);
  if (CDDdpath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDD data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CVDatabase", "", CDDvpath, PATH_MAX);
  if (CDDvpath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDD/VAST data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDextens", "", CDDextens, PATH_MAX);
  if (CDDextens[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDD file name extension...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "RAWextens", "", RAWextens, PATH_MAX);
  if (RAWextens[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no RAW file name extension...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDdescr", "", CDDdescr, PATH_MAX);
  if (CDDdescr[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no description file name extension...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "Database", "", database, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no VAST data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDProper", "", CDDProper, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDProper...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDOasis", "", CDDOasis, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDOasis...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "TREextens", "", TREextens, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no TREextens...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "SEQextens", "", SEQextens, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no SEQextens...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "OASIScgi", "", OASIScgi, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no OASIScgi...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDcgi", "", CDDcgi, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDcgi...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDPrefix", "", CDDPrefix, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDPrefix...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDPost_O", "", CDDPost_O, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDPost_O...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDPost_C", "", CDDPost_C, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDPost_C...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDefault", "", CDDefault, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDefault...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSearch1", "", CDDSearch1, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSearch1...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSearch2", "", CDDSearch2, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSearch2...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSearch3", "", CDDSearch3, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSearch3...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSearch4", "", CDDSearch4, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSearch4...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSname1", "", CDDSname1, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSname1...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSname2", "", CDDSname2, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSname2...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSname3", "", CDDSname3, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSname3...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSname4", "", CDDSname4, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSname4...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDlocat", "", CDDlocat, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDlocat...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDhuman", "", CDDhuman, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDhuman...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDhumsq", "", CDDhumsq, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDhumsq...\n");
    return FALSE;
  }
  return TRUE;
}                                                       /* end GetVastParams */

/*---------------------------------------------------------------------------*/
/* Montior hook to print to stderr for UNIX clients.                         */
/*---------------------------------------------------------------------------*/
static int LIBCALLBACK UNIXMontiorHook(Nlm_MonitorPtr mon, MonCode code)
{
  switch (code)  {
#ifdef OS_UNIX
    case MonCode_Create :
      fprintf(stderr, "%s\n", (Nlm_CharPtr) mon->strTitle);
      break;
    case MonCode_StrValue :
      fprintf(stderr, "%s\n", (Nlm_CharPtr) mon->strValue);
      break;
#endif
    default :
      break;
  }
  return 0;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void PrintMotd(CharPtr string, FILE *fp, Boolean html_format)
{
    Char buffer[100];
    CharPtr ptr;
    
    if (string == NULL)
        return;
    
    buffer[0] = NULLB;
    ptr = buffer;
    
    if (html_format) {
        fprintf(fp, "<PRE>\n");
    }
    
    while (*string != NULLB) {
        if (*string == '~') {
            *ptr = NULLB;
            fprintf(fp, "%s\n", buffer);
            buffer[0] = NULLB;
            ptr = buffer;
            string++;
            if (*string == NULLB)
                break;
        } else {
            *ptr=*string;
            ptr++;  string++;
        }
    }
    *ptr = NULLB;
    fprintf(fp, "%s\n", buffer);
    
    if (html_format) {
        fprintf(fp, "</PRE>\n");
    }
    
    fflush(fp);
}


#define NUMARGS (sizeof(myargs)/sizeof(myargs[0]))

static Args myargs [] = {
    { "Program Name",                                                  /* 0 */
      "blastp",    NULL, NULL, FALSE, 'p', ARG_STRING,   0.0, 0, NULL},
    { "Database",                                                      /* 1 */
      "oasis_sap", NULL, NULL, FALSE, 'd', ARG_STRING,   0.0, 0, NULL},
    { "Query File",                                                    /* 2 */
      "stdin",     NULL, NULL, TRUE , 'i', ARG_FILE_IN,  0.0, 0, NULL},
    { "Expectation value (E)",                                         /* 3 */
      "0.01",      NULL, NULL, FALSE, 'e', ARG_FLOAT,    0.0, 0, NULL},
    { "alignment view options:\n0 = pairwise,\n1 = master-slave showing identities,\n2 = master-slave no identities,\n3 = flat master-slave, show identities,\n4 = flat master-slave, no identities,\n5 = master-slave no identities and blunt ends,\n6 = flat master-slave, no identities and blunt ends", /* 4 */
      "0",         NULL, NULL, FALSE, 'm', ARG_INT,      0.0, 0, NULL},
    { "BLAST report Output File",                                      /* 5 */
      "stdout",    NULL, NULL, TRUE,  'o', ARG_FILE_OUT, 0.0, 0, NULL},
    { "Filter query sequence (DUST with blastn, SEG with others)",     /* 6 */
      "F",         NULL, NULL, FALSE, 'F', ARG_STRING,   0.0, 0, NULL},
    { "Cost to open a gap (zero invokes default behavior)",            /* 7 */
      "0",         NULL, NULL, FALSE, 'G', ARG_INT,      0.0, 0, NULL},
    { "Cost to extend a gap (zero invokes default behavior)",          /* 8 */
      "0",         NULL, NULL, FALSE, 'E', ARG_INT,      0.0, 0, NULL},
    { "X dropoff value for gapped alignment (in bits)\n(zero invokes default behavior)", /* 9 */
      "0",         NULL, NULL, FALSE, 'X', ARG_INT,      0.0, 0, NULL},
    { "Show GI's in deflines",                                        /* 10 */
      "F",         NULL, NULL, FALSE, 'I', ARG_BOOLEAN,  0.0, 0, NULL},
    { "Penalty for a nucleotide mismatch (blastn only)",              /* 11 */
      "-3",        NULL, NULL, FALSE, 'q', ARG_INT,      0.0, 0, NULL},
    { "Reward for a nucleotide match (blastn only)",                  /* 12 */
  "1",       NULL, NULL, FALSE, 'r', ARG_INT,      0.0, 0, NULL},
    { "Number of one-line descriptions (V)",                          /* 13 */
      "50",        NULL, NULL, FALSE, 'v', ARG_INT,      0.0, 0, NULL},
    { "Number of alignments to show (B)",                             /* 14 */
      "50",        NULL, NULL, FALSE, 'b', ARG_INT,      0.0, 0, NULL},
    { "Threshold for extending hits, default if zero",                /* 15 */
      "0",         NULL, NULL, FALSE, 'f', ARG_INT,      0.0, 0, NULL},
    { "Perfom gapped alignment (not available with tblastx)",         /* 16 */
      "T",         NULL, NULL, FALSE, 'g', ARG_BOOLEAN,  0.0, 0, NULL},
    { "Query Genetic code to use",                                    /* 17 */
      "1",         NULL, NULL, FALSE, 'Q', ARG_INT,      0.0, 0, NULL},
    { "DB Genetic code (for tblast[nx] only)",                        /* 18 */
      "1",         NULL, NULL, FALSE, 'D', ARG_INT,      0.0, 0, NULL},
    { "Number of processors to use",                                  /* 19 */
      "1",         NULL, NULL, FALSE, 'a', ARG_INT,      0.0, 0, NULL},
    { "SeqAlign file",                                                /* 20 */
      NULL,        NULL, NULL, TRUE,  'O', ARG_FILE_OUT, 0.0, 0, NULL},
    { "Believe the query defline",                                    /* 21 */
      "F",         NULL, NULL, FALSE, 'J', ARG_BOOLEAN,  0.0, 0, NULL},
    { "Word size, default if zero",                                   /* 22 */
      "0",         NULL, NULL, FALSE, 'W', ARG_INT,      0.0, 0, NULL},
    { "Start of the sequence",                                        /* 23 */
      "1",         NULL, NULL, FALSE, 'A', ARG_INT,      0.0, 0, NULL},
    { "End of the  sequence (-1 is entire sequence)",                 /* 24 */
      "-1",        NULL, NULL, FALSE, 'B', ARG_INT,      0.0, 0, NULL},
    { "Matrix",                                                       /* 25 */
      "BLOSUM62",  NULL, NULL, FALSE, 'M', ARG_STRING,   0.0, 0, NULL},
    { "Produce HTML output",                                          /* 26 */
      "T",         NULL, NULL, FALSE, 'T', ARG_BOOLEAN,  0.0, 0, NULL},
    { "Restrict search of database to list of GI's",                  /* 27 */
      NULL,        NULL, NULL, TRUE,  'l', ARG_STRING,   0.0, 0, NULL},
    { "Number of best hits from a region to keep",                    /* 28 */
      "100",       NULL, NULL, FALSE, 'K', ARG_INT,      0.0, 0, NULL},
    { "Length of region used to judge hits",                          /* 29 */
      "20",        NULL, NULL, FALSE, 'L', ARG_INT,      0.0, 0, NULL},
    { "Effective length of the search space (use 0 for real size)",   /* 30 */
      "0",         NULL, NULL, FALSE, 'Y', ARG_FLOAT,    0.0, 0, NULL},
    { "Query strands to search against database (for blast[nx], and tblastx).  3 is both, 1 is top, 2 is bottom", /* 31 */
      "3",         NULL, NULL, FALSE, 'S', ARG_INT,      0.0, 0, NULL},
    { "RPS Blast search",                                             /* 32 */
      "T",         NULL, NULL, FALSE, 'R', ARG_BOOLEAN,  0.0, 0, NULL},
    { "Restrict search of database to results of Entrez2 lookup",     /* 33 */
      NULL,        NULL, NULL, TRUE, 'u', ARG_STRING,    0.0, 0, NULL},
};


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* A special version of GetArgs that doesn't actually check the program args */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean WRPSBGetArgs(CharPtr progname, Int2 numargs, ArgPtr ap)
{
  static Nlm_CharPtr types[] = {
    NULL,
    "T/F",
    "Integer",
    "Real",
    "String",
    "File In",
    "File Out",
    "Data In",
    "Data Out"
  };
  Nlm_Boolean all_default = TRUE, range;
  Nlm_Int2    i, j;
  Nlm_ArgPtr  curarg;
  Nlm_Boolean *resolved;

  if (!ap  ||  numargs <= 0) return FALSE;

  resolved = (Nlm_Boolean*) Nlm_MemNew(numargs * sizeof(Nlm_Boolean));
  if ( !resolved ) return FALSE;

/*---------------------------------------------------------------------------*/
/* reset all args                                                            */
/*---------------------------------------------------------------------------*/
  curarg = ap;
  for (i = 0;  i < numargs;  i++, curarg++) {
    curarg->intvalue   = 0;
    curarg->floatvalue = 0.0;
    curarg->strvalue   = NULL;
  }

/*---------------------------------------------------------------------------*/
/* set defaults                                                              */
/*---------------------------------------------------------------------------*/
  curarg = ap;
  for (i = 0;  i < numargs;  i++, curarg++) {
    if ((curarg->type < ARG_BOOLEAN) || (curarg->type > ARG_DATA_OUT)) {
      ErrPostEx(SEV_ERROR, 0,0, "Invalid Arg->type in %s", curarg->prompt);
      Nlm_MemFree( resolved );
      Nlm_FreeArgs(i, ap);
      return FALSE;
    }
    curarg->intvalue = 0;
    curarg->floatvalue = 0.0;
    curarg->strvalue = NULL;
    if (curarg->defaultvalue != NULL) {
      resolved[i] = TRUE;
      switch (curarg->type) {
        case ARG_BOOLEAN:
          if (TO_UPPER(*curarg->defaultvalue) == 'T') curarg->intvalue = 1;
          else curarg->intvalue = 0;
          break;
        case ARG_INT: {
          long val;
          sscanf(curarg->defaultvalue, "%ld", &val);
          curarg->intvalue = val;
          break;
        }
        case ARG_FLOAT: {
          double val;
          sscanf(curarg->defaultvalue, "%lf", &val);
          curarg->floatvalue = val;
          break;
        }
        case ARG_STRING:
        case ARG_FILE_IN:
        case ARG_FILE_OUT:
        case ARG_DATA_IN:
        case ARG_DATA_OUT:
          curarg->strvalue = StringSave (curarg->defaultvalue);
          break;
      }
    } else if (curarg->optional == FALSE)
      all_default = FALSE;    /* must have some arguments */
  }
  if (all_default) {
    Nlm_MemFree(resolved);
    return TRUE;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Prints an acknowledgement of the Blast Query in the standard BLAST format.*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean QRPSBAcknowledgeBlastQuery(BioseqPtr bsp, Int4 line_length, FILE *outfp, Boolean believe_query, Boolean html)

{
  Char buffer[BUFFER_LENGTH];

  if (bsp == NULL || outfp == NULL) return FALSE;
  asn2ff_set_output(outfp, NULL);

  ff_StartPrint(0, 0, line_length, NULL);
  if (html) ff_AddString("<b>Query=</b> ");
  else ff_AddString("Query= ");
  ff_AddString(BioseqGetTitle(bsp));
  NewContLine();
  TabToColumn(10);
  ff_AddChar('(');
  ff_AddString(Ltostr((long) BioseqGetLen(bsp), 1));
  ff_AddString(" letters)");
  ff_EndPrint();
  return TRUE;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* This function draws the search page                                       */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean WRPSBDrawPage()
{
  CharPtr databases[5] = {NULL,NULL,NULL,NULL,NULL};
  CharPtr datab_nam[5] = {NULL,NULL,NULL,NULL,NULL};
  Char    *expect[] = { "0.000001","0.0001", "0.01", 
                        "1", "10", "100", NULL};
  Char    *nhits[] = {"10","25","50","100","250",NULL};
  Int4    i;

  databases[0] = CDDSearch1;
  databases[1] = CDDSearch2;
  databases[2] = CDDSearch3;
  datab_nam[0] = CDDSname1;
  datab_nam[1] = CDDSname2;
  datab_nam[2] = CDDSname3;
  if (strcmp(CDDlocat,"inhouse")==0) {
    databases[3] = CDDSearch4;
    datab_nam[3] = CDDSname4; 
  }

  printf("Content-type: text/html\n\n");
  printf("<HTML>\n");
  printf("<TITLE>NCBI CD-Search</TITLE>\n");
  printf("<BODY BGCOLOR=#FFFFFF>\n");
  printf("<A HREF=\"blast_form.map\">\n");
  printf("<IMG SRC=\"%scdsearch.gif\" BORDER=0 ISMAP></A>\n",URLcgi);
  printf("<H4>Search the <A HREF=\"cdd.shtml\">Conserved Domain Database</A> with Reverse Position Specific BLAST</H4>\n");

  printf("<b><a href=\"http://www.ncbi.nlm.nih.gov/htbin-post/Entrez/query?uid=9254694&form=6&db=m&Dopt=r\">Reference</a>:</b>\n");
  printf("Altschul, Stephen F., Thomas L. Madden, Alejandro A. Sch&auml;ffer, \n");
  printf("Jinghui Zhang, Zheng Zhang, Webb Miller, and David J. Lipman (1997), \n");
  printf("\"Gapped BLAST and PSI-BLAST: a new generation of protein database search\n");
  printf("programs\",  Nucleic Acids Res. 25:3389-3402.\n");
  
/*---------------------------------------------------------------------------*/
/* name of the rps-blast cgi-bin program encoded here!                       */
/*---------------------------------------------------------------------------*/
  printf("<FORM ACTION=\"%s\" METHOD=POST" ">\n", QRPSBNAME);


  printf("Search Database: <SELECT NAME=\"DATALIB\">\n");
  for (i=0; databases[i] != NULL; i++) {        
    if (strcmp(databases[i],DEFAULT_DATALIB)==0) {
      printf("<OPTION VALUE=\"%s\" SELECTED> %s\n",databases[i],datab_nam[i]);
    } else {
      printf("<OPTION VALUE=\"%s\"> %s\n",databases[i],datab_nam[i]);
    }
  }
  printf("</SELECT>\n");

  printf("<BR>Enter query as <strong>Protein</strong>\n"
         "<SELECT NAME=\"INPUT_TYPE\"> \n" 
         " <OPTION SELECTED VALUE=\"fasta\"> Sequence in FASTA format \n"
         " <OPTION VALUE=\"access\"> Accession or GI \n"
         "</SELECT>\n");
/*
  printf("<BR>Enter query as \n<SELECT NAME=\"IS_PROT\"> \n"
         " <OPTION SELECTED VALUE=1>Protein \n <OPTION VALUE=0>DNA/RNA "
         "</SELECT>\n"
         "<SELECT NAME=\"INPUT_TYPE\"> \n" 
         " <OPTION SELECTED VALUE=\"fasta\"> Sequence in FASTA format \n"
         " <OPTION VALUE=\"access\"> Accession or GI \n"
         "</SELECT>\n");
*/
  printf("<INPUT TYPE=\"submit\">\n");
  printf("<INPUT TYPE=\"reset\">\n");
  printf("<BR><TEXTAREA NAME=\"SEQUENCE\" rows=6 cols=60></TEXTAREA>\n");
  printf("<BR><BR>Please read about <A HREF=\"http://www.ncbi.nlm.nih.gov/BLAST/fasta.html\">"
         "FASTA</A> format description<BR>\n");
  printf("<HR>\n");

  printf("<BR><strong>Advanced options for the BLAST server:</strong><BR><BR>\n");
  
  printf("<a href=\"cdd_help.shtml#WRPSBExpect\">Expect&nbsp;</a>");

  printf("<SELECT name = \"EXPECT\">\n");
  for (i=0; expect[i] != NULL; i++) {   
    if (strcmp(expect[i],DEFAULT_EVALUE)==0) {
      printf("<OPTION SELECTED VALUE=\"%s\">&nbsp;%s\n",expect[i],expect[i]);
    } else {
      printf("<OPTION VALUE=\"%s\">&nbsp;%s\n",expect[i],expect[i]);
    }
  }
  printf("</SELECT>\n");
  printf("&nbsp;&nbsp;<a href=\"cdd_help.shtml#WRPSBFilter\">Filter</a>\n");
  printf("<INPUT TYPE=\"checkbox\" VALUE=\"T\" NAME=\"FILTER\" CHECKED> Low complexity.&nbsp;&nbsp;\n");

  printf("Search <A HREF=\"cdd_help.shtml#WRPSBMode\">mode</A>: <SELECT name = \"SMODE\">\n");
  printf("<OPTION SELECTED VALUE=\"0\"> Multiple hits 1-pass\n");
/*  printf("<OPTION VALUE=\"1\"> Single hit 1-pass\n");
  printf("<OPTION VALUE=\"2\"> 2-pass\n"); */
  printf("</SELECT><BR>\n");
  
  printf("<BR><BR><strong>Output formatting options:</strong><BR><BR>\n");

  printf("Display <A HREF=\"cdd_help.shtml#WRPSBHits\">up to</A> <SELECT name = \"NHITS\">\n");
  for (i=0; nhits[i] != NULL; i++) {    
    if (strcmp(nhits[i],DEFAULT_NHITS)==0) {
      printf("<OPTION SELECTED VALUE=\"%s\">&nbsp;%s hits\n",nhits[i],nhits[i]);
    } else {
      printf("<OPTION VALUE=\"%s\">&nbsp;%s hits\n",nhits[i],nhits[i]);
    }
  }
  printf("</SELECT>\n");
  printf("<A HREF=\"cdd_help.shtml#WRPSBGraph\">with</A>&nbsp;\n");
  printf("<SELECT NAME=\"GRAPH\">\n");
  printf("<OPTION VALUE=\"0\">No Graphic Overview\n");
  printf("<OPTION VALUE=\"1\">Condensed Graphic Overview\n");
  printf("<OPTION SELECTED VALUE=\"2\">Extended Graphic Overview\n");
  printf("</SELECT>\n");
  printf("<A HREF=\"cdd_help.shtml#WRPSBColor\">in</A>&nbsp;\n");
  printf("<SELECT NAME=\"PAIR\">\n");
  printf("<OPTION VALUE=\"0\">Color Scheme 1\n");
  printf("<OPTION VALUE=\"1\">Color Scheme 2\n");
  printf("<OPTION SELECTED VALUE=\"2\">Color Scheme 3\n");
  printf("</SELECT>\n");
  printf("<hr>\n");
  printf("Retrieve CD-Search request #");
  printf("<INPUT type=\"TEXT\" name=\"RID\" maxlength=40 size=25>\n");
  printf("<INPUT TYPE=\"submit\">\n");
  printf("</FORM>\n");
  printf("<hr>\n");

  printf("</BODY>\n");
  printf("</HTML>\n");
  fflush(NULL);
  exit(0);
}


/*---------------------------------------------------------------------------*/
/* use this function yanked from batch.c                                     */
/*---------------------------------------------------------------------------*/
static Int4 AccessionToGi(CharPtr string,  Int4Ptr PNTR giptr, Int2 seqtype) 
{
    Int4 i, gi = 0;
    CharPtr str;
    Char    teststring[32];
    ByteStorePtr bsp;
    Int4 GiNum;
    
    if(string == NULL)
        return 0;
    
    if((gi = atol(string)) > 0) { /* He-he this is gi... */
/*---------------------------------------------------------------------------*/
/* need to insert code to make sure that PDB-Id's are not interpreted as gi's*/
/*---------------------------------------------------------------------------*/
        sprintf(teststring,"%d",gi);
        if (strcmp(string,teststring)==0 || gi > 999) {   
          *giptr = (Int4Ptr) MemNew(sizeof(Int4));
          (*giptr)[0] = gi;
          return 1;
        }
    }
    str = (CharPtr) MemNew(StringLen(string)+30);
    sprintf(str, "\"%s\"[ACCN]", string);
    
    if((bsp = (ByteStorePtr) EntrezTLEvalXString(str, seqtype, -1, NULL, NULL)) == NULL) {
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

/*---------------------------------------------------------------------------*/
/* Old AccessionToGi function, taken out of service for good ..              */
/*---------------------------------------------------------------------------*/
static Int4 OldAccessionToGi (CharPtr string) 
{
  Int4         gi;
  SeqIdPtr     sip;
  TextSeqIdPtr tsip;

  if((gi = atol(string)) > 0) return gi;

  gi = 0;
  sip = ValNodeNew (NULL);
  if (sip != NULL) {
    tsip = TextSeqIdNew ();
    if (tsip != NULL) {
      tsip->accession = StringSave(string);
/* GenBank, EMBL, and DDBJ. */
      sip->choice = SEQID_GENBANK;
      sip->data.ptrvalue = (Pointer) tsip;
      gi = EntrezFindSeqId (sip);
      if (gi == 0) {
/* SwissProt. */
        sip->choice = SEQID_SWISSPROT;
        gi = EntrezFindSeqId (sip);
      }
      if (gi == 0) {
/* PIR */
        sip->choice = SEQID_PIR;
        gi = EntrezFindSeqId (sip);
      }
      if (gi == 0) {
/* PRF */
        sip->choice = SEQID_PRF;
        gi = EntrezFindSeqId (sip);
      }
      if (gi == 0) {
/* OTHER, probably 'ref' */
        sip->choice = SEQID_OTHER;
        gi = ID1FindSeqId (sip);
      }
    }
  }
  return gi;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* fixes a bug noted by the first critical user of CDD-Search :)             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CharPtr FixStringForWWW(CharPtr oldstring)
{
  Int4    length;
  Int4    i, j = 0;
  CharPtr newstring;
  
  length = strlen (oldstring);
  newstring = MemNew((length+1)*sizeof(Char));
  for (i=0;i<length;i++) {
    if (oldstring[i]!=39)           /* take care of "'" by simply removing it*/
      newstring[j++]=oldstring[i];
  }
  newstring[j]='\0';
  return(newstring);
}


#ifdef USE_PNG

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Print the wrapper for the graphics page plus the PNG-file used in display */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void WRPSBCl3PrintGraphics(AlignmentAbstractPtr aap, FILE *table, Int4 maxrow,
                                  BioseqPtr query_bsp, ValNodePtr mask,
                                  Int4 iGraphMode)
{
  gdImagePtr           im;
  gdPoint              points[3];
  FILE                *pngout;
  Int4                 gystep = WRPSB_GRAPH_HEIGHT+WRPSB_GRAPH_SPACER;
  Int4                 colidx, qlen, white, black, lblue;
  Int4                 ulx, uly, lrx, lry, swidth, i;
  Int4                 tick, ntick, dont, sky, missg;
  Char                 cTmp[16];
  Int4                 right = WRPSB_GRAPH_WIDTH - 1;
  Char                 path[PATH_MAX], dstring[PATH_MAX];
  AlignmentAbstractPtr aapThis;
  SeqLocPtr            slp, slpThis;
  SeqIntPtr            sintp;
  ValNodePtr           vnp, vnpThis;
  static Int4          ticksteps[14] = {5,10,20,25,50,100,200,250,500,1000,
                                        2000,2500,5000,10000};

  if (aap)
    srandom((unsigned int) (query_bsp->length + aap->gstart + (Int4)getpid()));
  else 
    srandom((unsigned int) (query_bsp->length + (Int4)getpid()));
  sprintf(path,"/tmp/%ld%ld.png",(long)random(),(long)getpid());

  fprintf(table,"<CENTER>\n");
  fprintf(table,"<FORM NAME=BLASTFORM>\n");
  fprintf(table,"<input type=text name=defline size=66 value=\"Mouse-over boxes to display more information\">");
  fprintf(table,"</FORM>\n");
  fprintf(table,"<map name=\"img_map\">\n");

/*---------------------------------------------------------------------------*/
/* create and save the png file                                              */
/*---------------------------------------------------------------------------*/
  im = gdImageCreate(WRPSB_GRAPH_WIDTH,gystep*(min(maxrow,WRPSB_GRAPH_MAXROW))+30);
  white = gdImageColorAllocate(im, 255, 255, 255); 
  black = gdImageColorAllocate(im,   0,   0,   0);
  lblue = gdImageColorAllocate(im,  51, 102, 255);     
  sky   = gdImageColorAllocate(im,   0, 255, 255);
  gdImageFilledRectangle(im,  5, 20,right-5, 25,black);
  qlen = query_bsp->length - 1;
  vnp = mask;
  while (vnp) {
    slp = vnp->data.ptrvalue;
    while (slp) {
      if (slp->choice == 4) {      
        sintp = slp->data.ptrvalue;
        ulx = 5 + ((WRPSB_GRAPH_WIDTH-10)*sintp->from) / qlen;
        lrx = 5 + ((WRPSB_GRAPH_WIDTH-10)*sintp->to)   / qlen;
        gdImageFilledRectangle(im,ulx,20,lrx,25,sky);
        fprintf(table,"<area shape=rect coords=%d,%d,%d,%d href=\"http://www.ncbi.nlm.nih.gov/BLAST/filtered.html\" ONMOUSEOVER=\'document.BLASTFORM.defline.value=\"Masked-out region, low complexity\"\' ONMOUSEOUT=\'document.BLASTFORM.defline.value=\"Mouse-over boxes to display more information\"\'>\n",
                ulx+1,21,lrx+1,26);
      } else if (slp->choice == 5) {
        slpThis = slp->data.ptrvalue;
        while (slpThis) {
          if (slpThis->choice == 4) {
            sintp = slpThis->data.ptrvalue;
            ulx = 5 + ((WRPSB_GRAPH_WIDTH-10)*sintp->from) / qlen;
            lrx = 5 + ((WRPSB_GRAPH_WIDTH-10)*sintp->to)   / qlen;
            gdImageFilledRectangle(im,ulx,20,lrx,25,sky);
            fprintf(table,"<area shape=rect coords=%d,%d,%d,%d href=\"http://www.ncbi.nlm.nih.gov/BLAST/filtered.html\" ONMOUSEOVER=\'document.BLASTFORM.defline.value=\"Masked-out region, low complexity\"\' ONMOUSEOUT=\'document.BLASTFORM.defline.value=\"Mouse-over boxes to display more information\"\'>\n",
                    ulx+1,21,lrx+1,26);
          }
          slpThis = slpThis->next;
        }
      }
      slp = slp->next;
    }

    vnp = vnp->next;
  }

  gdImageLine(im,       5,20,      5,15,lblue);
  gdImageLine(im, right-5,20,right-5,15,lblue);
  uly = 15-gdFontTiny->h;
  strcpy(cTmp,"1");
  gdImageString(im,gdFontTiny,   5,uly,(unsigned char *) cTmp,lblue);
  cTmp[0] = '\0'; sprintf(cTmp,"%d",query_bsp->length);
  dont = right-5-(strlen(cTmp)*gdFontTiny->w);
  gdImageString(im,gdFontTiny,dont,uly,(unsigned char *) cTmp,lblue);
  for (i=0;i<14;i++) {
    ntick = qlen / ticksteps[i]; if (ntick < 10) {
      tick = ticksteps[i]; break;
    }
  }
  for (i=1;i<=ntick;i++) {
    ulx = 5 + ((i*tick-1)*(WRPSB_GRAPH_WIDTH-10))/qlen;
    if (ulx > (right-5)) ulx=right-5;
    gdImageLine(im,ulx,20,ulx,15,lblue);
    cTmp[0] = '\0'; sprintf(cTmp,"%d",i*tick);
    if (ulx+(strlen(cTmp)*gdFontTiny->w/2) < (dont-1)) {
      gdImageString(im,gdFontTiny,ulx-(strlen(cTmp)*gdFontTiny->w/2),uly,(unsigned char *) cTmp,lblue);
    }  
  }
  aapThis = aap;
  while (aapThis) {
    if (aapThis->row <= WRPSB_GRAPH_MAXROW &&
      (iGraphMode > 1 || aapThis->bDrawThisOne)) {
      ulx = aapThis->gstart + 5;
      lrx = aapThis->gstop  + 5;
      uly = WRPSB_GRAPH_SPACER + WRPSB_GRAPH_SPACER + gystep*aapThis->row;
      lry = WRPSB_GRAPH_SPACER + gystep             + gystep*aapThis->row;
      gdImageRectangle(im, ulx,uly,lrx,lry,black);
      colidx = gdImageColorExact(im,aapThis->red,aapThis->green,aapThis->blue);
      if (colidx == -1) {
        colidx = gdImageColorAllocate(im,aapThis->red,aapThis->green,aapThis->blue);
      }
      dstring[0] = '\0'; missg = min(50,strlen(aapThis->long_defline));
      strncpy(dstring,aapThis->long_defline,(size_t) missg);
      if (missg == 50) { dstring[47]='.'; dstring[48]='.'; dstring[49]='.'; };
      dstring[missg]='\0';
      missg = 50 - strlen(dstring);
      for (i=0;i<missg;i++) strcat(dstring," ");
      dstring[strlen(dstring)]='\0';
      strcat(dstring," ");
      dstring[strlen(dstring)]='\0';
      cTmp[0] = '\0'; 
      print_score_evalue(aapThis->defline->evalue,aapThis->defline->bit_score,cTmp);
      strcat(dstring,cTmp);
      dstring[strlen(dstring)]='\0';
      fprintf(table,"<area shape=rect coords=%d,%d,%d,%d href=\"#%s\" ONMOUSEOVER=\'document.BLASTFORM.defline.value=\"%s\"\' ONMOUSEOUT=\'document.BLASTFORM.defline.value=\"Mouse-over boxes to display more information\"\'>\n",
              ulx+1,uly+1,lrx+1,lry+1,aapThis->name,FixStringForWWW(dstring));
      ulx++; uly++; lrx--; lry--;
      if (ulx <= lrx) {
        gdImageFilledRectangle(im,ulx,uly,lrx,lry,colidx);
        if (lrx-ulx > 8) {
  	  if (aapThis->nmissg>=0.2) {
	    points[0].x = ulx - 1;
	    points[0].y = uly - 1;
	    points[1].x = ulx - 1;
	    points[1].y = uly - 1 + WRPSB_GRAPH_HEIGHT / 2;
	    points[2].x = ulx + 3;
	    points[2].y = uly - 1 + WRPSB_GRAPH_HEIGHT / 4;
	    gdImageFilledPolygon(im,points,3,white);
	    points[0].x = ulx - 1;
	    points[0].y = uly - 1 + WRPSB_GRAPH_HEIGHT / 2;
	    points[1].x = ulx - 1;
	    points[1].y = lry;
	    points[2].x = ulx + 4;
	    points[2].y = lry + 1 - WRPSB_GRAPH_HEIGHT / 4;
	    gdImageFilledPolygon(im,points,3,white);
	  }
	  if (aapThis->cmissg>=0.2) {
	    points[0].x = lrx + 1;
	    points[0].y = uly - 1;
	    points[1].x = lrx + 1;
	    points[1].y = uly - 1 + WRPSB_GRAPH_HEIGHT / 2;
	    points[2].x = lrx - 4;
	    points[2].y = uly - 1 + WRPSB_GRAPH_HEIGHT / 4;
	    gdImageFilledPolygon(im,points,3,white);
	    points[0].x = lrx + 1;
	    points[0].y = uly - 1 + WRPSB_GRAPH_HEIGHT / 2;
	    points[1].x = lrx + 1;
	    points[1].y = lry + 1;
	    points[2].x = lrx - 3;
	    points[2].y = lry + 1 - WRPSB_GRAPH_HEIGHT / 4;
	    gdImageFilledPolygon(im,points,3,white);
	  }
	}
        swidth = lrx-ulx+1;
        ulx = 5+(aapThis->gstart+aapThis->gstop)/2;
        uly = WRPSB_GRAPH_SPACER+(2*WRPSB_GRAPH_SPACER+WRPSB_GRAPH_HEIGHT)/2+gystep*aapThis->row;
        strcpy(cTmp,aapThis->cGraphId);
        if (strlen(cTmp)*gdFontMediumBold->w < swidth) {
          gdImageString(im,
                        gdFontMediumBold,
                        ulx-strlen(cTmp)*gdFontMediumBold->w / 2,
                        uly-gdFontMediumBold->h / 2, (unsigned char *) cTmp, white);
        } else if (strlen(cTmp)*gdFontTiny->w < swidth) {
          gdImageString(im,
                        gdFontTiny,
                        ulx-strlen(cTmp)*gdFontTiny->w / 2,
                        uly-gdFontTiny->h / 2, (unsigned char *) cTmp, white);
        }
      }
    }
    aapThis = aapThis->next;
  }
  pngout = (FILE *) fopen(path, "wb");
  gdImagePng(im, pngout);
  fflush(pngout);
  fclose(pngout);
  gdImageDestroy(im);
  fprintf(table,"</map>\n");
  fprintf(table,"<img src=\"%snph-viewpng.cgi?%s\" usemap=\"#img_map\" border=0 ISMAP>\n",
          URLcgi,path);
  fprintf(table,"</CENTER>\n");
}

#endif
#ifndef USE_PNG

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Print the wrapper for the graphics page plus the GIF-file used in display */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void WRPSBCl3PrintGraphics(AlignmentAbstractPtr aap, FILE *table, Int4 maxrow,
                                  BioseqPtr query_bsp, ValNodePtr mask,
                                  Int4 iGraphMode)
{
  gdImagePtr           im;
  FILE                 *gifout;
  Int4                 gystep = WRPSB_GRAPH_HEIGHT+WRPSB_GRAPH_SPACER;
  Int4                 colidx, qlen, white, black, lblue;
  Int4                 ulx, uly, lrx, lry, swidth, i;
  Int4                 tick, ntick, dont, sky, missg;
  Char                 cTmp[16];
  Int4                 right = WRPSB_GRAPH_WIDTH - 1;
  Char                 path[PATH_MAX], dstring[PATH_MAX];
  AlignmentAbstractPtr aapThis;
  SeqLocPtr            slp, slpThis;
  SeqIntPtr            sintp;
  ValNodePtr           vnp, vnpThis;
  static Int4          ticksteps[14] = {5,10,20,25,50,100,200,250,500,1000,
                                        2000,2500,5000,10000};

  if (aap)
    srandom((unsigned int) (query_bsp->length + aap->gstart + (Int4)getpid()));
  else 
    srandom((unsigned int) (query_bsp->length + (Int4)getpid()));
  sprintf(path,"/tmp/%ld%ld.gif",(long)random(),(long)getpid());

  fprintf(table,"<CENTER>\n");
  fprintf(table,"<FORM NAME=BLASTFORM>\n");
  fprintf(table,"<input type=text name=defline size=66 value=\"Mouse-over boxes to display more information\">");
  fprintf(table,"</FORM>\n");
  fprintf(table,"<map name=\"img_map\">\n");

/*---------------------------------------------------------------------------*/
/* create and save the gif file                                              */
/*---------------------------------------------------------------------------*/
  im = gdImageCreate(WRPSB_GRAPH_WIDTH,gystep*(min(maxrow,WRPSB_GRAPH_MAXROW))+30);
  white = gdImageColorAllocate(im, 255, 255, 255); 
  black = gdImageColorAllocate(im,   0,   0,   0);
  lblue = gdImageColorAllocate(im,  51, 102, 255);     
  sky   = gdImageColorAllocate(im,   0, 255, 255);
  gdImageFilledRectangle(im,  5, 20,right-5, 25,black);
  qlen = query_bsp->length;
  vnp = mask;
  while (vnp) {
    slp = vnp->data.ptrvalue;
    while (slp) {
      if (slp->choice == 4) {      
        sintp = slp->data.ptrvalue;
        ulx = 5 + ((WRPSB_GRAPH_WIDTH-10)*sintp->from) / qlen;
        lrx = 5 + ((WRPSB_GRAPH_WIDTH-10)*sintp->to)   / qlen;
        gdImageFilledRectangle(im,ulx,20,lrx,25,sky);
        fprintf(table,"<area shape=rect coords=%d,%d,%d,%d href=\"http://www.ncbi.nlm.nih.gov/BLAST/filtered.html\" ONMOUSEOVER=\'document.BLASTFORM.defline.value=\"Masked-out region, low complexity\"\' ONMOUSEOUT=\'document.BLASTFORM.defline.value=\"Mouse-over boxes to display more information\"\'>\n",
                ulx+1,21,lrx+1,26);
      } else if (slp->choice == 5) {
        slpThis = slp->data.ptrvalue;
        while (slpThis) {
          if (slpThis->choice == 4) {
            sintp = slpThis->data.ptrvalue;
            ulx = 5 + ((WRPSB_GRAPH_WIDTH-10)*sintp->from) / qlen;
            lrx = 5 + ((WRPSB_GRAPH_WIDTH-10)*sintp->to)   / qlen;
            gdImageFilledRectangle(im,ulx,20,lrx,25,sky);
            fprintf(table,"<area shape=rect coords=%d,%d,%d,%d href=\"http://www.ncbi.nlm.nih.gov/BLAST/filtered.html\" ONMOUSEOVER=\'document.BLASTFORM.defline.value=\"Masked-out region, low complexity\"\' ONMOUSEOUT=\'document.BLASTFORM.defline.value=\"Mouse-over boxes to display more information\"\'>\n",
                    ulx+1,21,lrx+1,26);
          }
          slpThis = slpThis->next;
        }
      }
      slp = slp->next;
    }

    vnp = vnp->next;
  }

  gdImageLine(im,       5,20,      5,15,lblue);
  gdImageLine(im, right-5,20,right-5,15,lblue);
  uly = 15-gdFont5X8->h;
  strcpy(cTmp,"1");
  gdImageString(im,gdFont5X8,   5,uly,(char *) cTmp,lblue);
  cTmp[0] = '\0'; sprintf(cTmp,"%d",qlen);
  dont = right-5-(strlen(cTmp)*gdFont5X8->w);
  gdImageString(im,gdFont5X8,dont,uly,(char *) cTmp,lblue);
  for (i=0;i<14;i++) {
    ntick = qlen / ticksteps[i]; if (ntick < 10) {
      tick = ticksteps[i]; break;
    }
  }
  for (i=1;i<=ntick;i++) {
    ulx = (Int4) (5.0 + (((float)(i*tick)-1.0)*(WRPSB_GRAPH_WIDTH-10))/(float)qlen);
    if (ulx > (right-5)) ulx=right-5;
    gdImageLine(im,ulx,20,ulx,15,lblue);
    cTmp[0] = '\0'; sprintf(cTmp,"%d",i*tick);
    if (ulx+(strlen(cTmp)*gdFont5X8->w/2) < (dont-1)) {
      gdImageString(im,gdFont5X8,ulx-(strlen(cTmp)*gdFont5X8->w/2),uly,(char *) cTmp,lblue);
    }  
  }
  aapThis = aap;
  while (aapThis) {
    if (aapThis->row <= WRPSB_GRAPH_MAXROW &&
      (iGraphMode > 1 || aapThis->bDrawThisOne)) {
      ulx = aapThis->gstart + 5;
      lrx = aapThis->gstop  + 5;
      uly = WRPSB_GRAPH_SPACER + WRPSB_GRAPH_SPACER + gystep*aapThis->row;
      lry = WRPSB_GRAPH_SPACER + gystep             + gystep*aapThis->row;
      gdImageRectangle(im, ulx,uly,lrx,lry,black);
      colidx = gdImageColorExact(im,aapThis->red,aapThis->green,aapThis->blue);
      if (colidx == -1) {
        colidx = gdImageColorAllocate(im,aapThis->red,aapThis->green,aapThis->blue);
      }
      dstring[0] = '\0'; missg = min(50,strlen(aapThis->long_defline));
      strncpy(dstring,aapThis->long_defline,(size_t) missg);
      if (missg == 50) { dstring[47]='.'; dstring[48]='.'; dstring[49]='.'; };
      dstring[missg]='\0';
      missg = 50 - strlen(dstring);
      for (i=0;i<missg;i++) strcat(dstring," ");
      dstring[strlen(dstring)]='\0';
      strcat(dstring," ");
      dstring[strlen(dstring)]='\0';
      cTmp[0] = '\0'; 
      print_score_evalue(aapThis->defline->evalue,aapThis->defline->bit_score,cTmp);
      strcat(dstring,cTmp);
      dstring[strlen(dstring)]='\0';
      fprintf(table,"<area shape=rect coords=%d,%d,%d,%d href=\"#%s\" ONMOUSEOVER=\'document.BLASTFORM.defline.value=\"%s\"\' ONMOUSEOUT=\'document.BLASTFORM.defline.value=\"Mouse-over boxes to display more information\"\'>\n",
              ulx+1,uly+1,lrx+1,lry+1,aapThis->name,FixStringForWWW(dstring));
      ulx++; uly++; lrx--; lry--;
      if (ulx <= lrx) {
        gdImageFilledRectangle(im,ulx,uly,lrx,lry,colidx);
        swidth = lrx-ulx+1;
        ulx = 5+(aapThis->gstart+aapThis->gstop)/2;
        uly = WRPSB_GRAPH_SPACER+(2*WRPSB_GRAPH_SPACER+WRPSB_GRAPH_HEIGHT)/2+gystep*aapThis->row;
        strcpy(cTmp,aapThis->cGraphId);
        if (strlen(cTmp)*gdFont7X13b->w < swidth) {
          gdImageString(im,
                        gdFont7X13b,
                        ulx-strlen(cTmp)*gdFont7X13b->w / 2,
                        uly-gdFont7X13b->h / 2, (char *) cTmp, white);
        } else if (strlen(cTmp)*gdFont5X8->w < swidth) {
          gdImageString(im,
                        gdFont5X8,
                        ulx-strlen(cTmp)*gdFont5X8->w / 2,
                        uly-gdFont5X8->h / 2, (char *) cTmp, white);
        }
      }
    }
    aapThis = aapThis->next;
  }
  gifout = fopen(path, "wb");
  gdImageGif(im, gifout);
  fflush(gifout);
  fclose(gifout);
  gdImageDestroy(im);
  fprintf(table,"</map>\n");
  fprintf(table,"<img src=\"snph-viewgif.cgi?%s\" usemap=\"#img_map\" border=0 ISMAP>\n",
          URLcgi,path);
  fprintf(table,"</CENTER>\n");
}

#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* fill in the CDD hit data structure                                        */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------
CddHitPtr RPSBgetCddHits(SeqAlignPtr sap)
{
  CddHitPtr         cdhThis, cdhHead = NULL, cdhTail = NULL;
  DenseSegPtr       dsp;
  Int4              i, number;
  SeqIdPtr          sip = NULL;
  BioseqPtr         bsp = NULL;
  Boolean           found_score = FALSE;
  SeqAlignPtr       sapThis;
  Char              buffer[BUFFER_LENGTH+1];
  CharPtr           dbname;
  CharPtr           title, ctmp, tbuffer;

  if (!sap) return NULL;
  sapThis = sap;
  while (sapThis) {
    cdhThis = (CddHitPtr)MemNew(sizeof(CddHit));
    cdhThis->start = -1; cdhThis->stop = -1;
    dsp = sapThis->segs; 
    if (dsp) {
      for (i=0;i<dsp->numseg;i++) {
        if (dsp->starts[2*i] > -1) {
          cdhThis->start = dsp->starts[2*i];
          break;
        }
      }
      for (i=0;i<dsp->numseg;i++) {
        if (dsp->starts[2*i] > -1) {
          cdhThis->stop = dsp->starts[2*i]+dsp->lens[i]-1;
        }
      }
      sip = dsp->ids->next;
    }
    found_score = GetScoreAndEvalue(sapThis, &cdhThis->score,
                                    &cdhThis->bit_score,
                                    &cdhThis->evalue, &number);
    if (sip) {
      bsp = BioseqLockById(sip);
      if(bsp) {
        SeqIdWrite(bsp->id, buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
        title = StringSave(BioseqGetTitle(bsp));
        BioseqUnlock(bsp);
        tbuffer = StringSave(buffer);
        ctmp = StrTok(tbuffer,"|");
        if (ctmp) {
          dbname = StringSave(StrTok(NULL,"|"));
          cdhThis->CDDid = StringSave(StrTok(NULL,"|"));
        } else {
          cdhThis->CDDid = StringSave(buffer);
        }
        if (StrCmp(dbname,"Pfam") == 0) {
          cdhThis->ShortName = StringSave(StrTok(title,","));
          cdhThis->Definition = StringSave(StrTok(NULL,",")+1);
        } else if (StrCmp(dbname,"Smart") == 0) {
          cdhThis->ShortName = StringSave(cdhThis->CDDid);
          cdhThis->Definition = StringSave(StrTok(title,";"));
        } else if (StrCmp(dbname,"Load") == 0) {
          cdhThis->ShortName = StringSave(StrTok(title,","));
          cdhThis->Definition = StringSave(StrTok(NULL,",")+1);
        } else {
          cdhThis->ShortName = StringSave(cdhThis->CDDid);
          cdhThis->Definition = StringSave(title);
        }
        title = MemFree(title);
        tbuffer = MemFree(tbuffer);
        dbname = MemFree(dbname);
      }
    }
    if (cdhHead) {
      cdhTail->next = cdhThis;
      cdhTail = cdhThis;
    } else {
      cdhHead = cdhThis;
      cdhTail = cdhThis;
    }
    sapThis = sapThis->next;
  }
  return(cdhHead);
}
 ---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* fill in the Alignment Abstract Data Structure                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static AlignmentAbstractPtr WRPSBCl3AbstractAlignment(BlastPruneSapStructPtr prune,
                                                      BioseqPtr query_bsp,
                                                      Int4 iGraphWidth,
                                                      Int4 *mxr,
                                                      Int4 iGraphMode,
                                                      CharPtr dbversion,
                                                      Boolean *bAnyPdb)
{
  Boolean              *bConflict;
  Boolean              bDbIsOasis = TRUE, found_score = FALSE;
  Int4                 maxrow = 1, i;
  Int4                 iCount = 0;
  Int4                 iColValue, number, score;
  Int4                 *iOvrlap, *iMutual;
  AlignmentAbstractPtr aap, aapThis, aapTmp, aapHead = NULL;
  BioseqPtr            bsp;
  CddTreePtr           pcddt;
  CddDescrPtr          description;
  CharPtr              cTemp;
  CharPtr              cCurrDesc;
  DbtagPtr             dbtp;
  DenseSegPtr          dsp;
  ObjectIdPtr          oidp;
  ScorePtr             thisScorePtr;
  SeqAlignPtr          sap;
  SeqIdPtr             sip, gi_list;
  TxDfLineStructPtr    txsp;
  Nlm_FloatHi          evalue, bit_score;
  Char                 path[PATH_MAX], hpath[PATH_MAX];
  Char                 buffer[BUFFER_LENGTH+1];
  Char                 cDatabase[16];
  Char                 cCDDid[16];

  if (strcmp(myargs[1].strvalue,"cdd")==0) bDbIsOasis = FALSE;
  
  sap = prune->sap;
  *bAnyPdb = FALSE;
  while (sap) {
    iCount++;
    aapThis = (AlignmentAbstractPtr)MemNew(sizeof(AlignmentAbstract));
    aapThis->salp = sap;
    dsp = sap->segs;
    for (i=0;i<dsp->numseg;i++) {
      if (dsp->starts[2*i] > -1) {
        aapThis->mstart = dsp->starts[2*i];
	aapThis->nmissg = (Nlm_FloatHi) dsp->starts[2*i+1];
        break;
      }
    }
    for (i=0;i<dsp->numseg;i++) {
      if (dsp->starts[2*i] > -1) {
        aapThis->mstop = dsp->starts[2*i]+dsp->lens[i]-1;
	aapThis->cmissg = (Nlm_FloatHi) (dsp->starts[2*i+1]+dsp->lens[i]-1);
      }
    }
    aapThis->gstart = (aapThis->mstart * iGraphWidth) / (query_bsp->length-1);
    aapThis->gstop =  (aapThis->mstop  * iGraphWidth) / (query_bsp->length-1);
    aapThis->score = sap->score;
    sip = dsp->ids->next;
    thisScorePtr = sap->score;
    found_score = GetScoreAndEvalue(sap, &score, &bit_score, &evalue, &number);
    if      (evalue >  100.0   ) iColValue = 204;
    else if (evalue >    1.0   ) iColValue = 153;
    else if (evalue >    0.01  ) iColValue = 102;
    else if (evalue >    0.0001) iColValue =  51;
    else                         iColValue =   0;
    bsp = BioseqLockById(sip);
    aapThis->nmissg = aapThis->nmissg/(Nlm_FloatHi)bsp->length;
    aapThis->cmissg = ((Nlm_FloatHi)bsp->length-1.0-aapThis->cmissg)/(Nlm_FloatHi) bsp->length;
    txsp = (TxDfLineStructPtr) MemNew(sizeof(TxDfLineStruct));
    txsp->segs_str = NULL;
    txsp->segs_buflen = 0;
    if(bsp != NULL) {
      gi_list = GetUseThisGi(sap);
      if (gi_list) {
        FilterTheDefline(bsp, gi_list, buffer, BUFFER_LENGTH, &(txsp->title));
        gi_list = SeqIdSetFree(gi_list);
        sip = SeqIdFree(sip);
        txsp->id = SeqIdParse(buffer);
      } else {
        SeqIdWrite(bsp->id, buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
        txsp->title = StringSave(BioseqGetTitle(bsp));
        txsp->id = sip;
      }
      txsp->is_na = (bsp->mol != Seq_mol_aa);
    } else {
      SeqIdWrite(sip, buffer, PRINTID_FASTA_LONG, BUFFER_LENGTH);
      txsp->title = StringSave("Unknown");
      txsp->is_na = FALSE;
      txsp->id = sip;
    }
    txsp->seqalign = sap;
    txsp->buffer_id = StringSave(buffer);
    txsp->score = score;
    txsp->bit_score = bit_score;
    txsp->evalue = evalue;
    txsp->number = number;
    txsp->found_score = found_score;
    SeqAlignSegsStr(sap, 1, &txsp->segs_str, &txsp->segs_buflen, &txsp->segs_used);
    txsp->isnew = FALSE;
    txsp->waschecked = FALSE;
    if(bsp != NULL) BioseqUnlock(bsp);
    aapThis->defline = txsp;
    aapThis->long_defline = MemNew(sizeof(Char)*(2+strlen(aapThis->defline->title)+strlen(aapThis->defline->buffer_id)));
    StrCpy(aapThis->long_defline, aapThis->defline->buffer_id);
    StrCat(aapThis->long_defline," ");
    StrCat(aapThis->long_defline,aapThis->defline->title);
    cTemp = strdup(txsp->buffer_id);
    if (strncmp(cTemp,"gnl|",4)==0) {
      strtok(cTemp,"|");
      aapThis->cDatabase = strdup(strtok(NULL,"|"));
      strcpy(path,strtok(NULL,"|"));
      aapThis->cCDDid = strdup(strtok(path," "));
    } else WRPSBHtmlError("Could not interpret subject defline!");
    MemFree(cTemp);
    aapThis->bIsProfile = FALSE;
    if (StringICmp(aapThis->cDatabase,"Smart")==0) {
      aapThis->red = 255;
      aapThis->green = aapThis->blue = iColValue;
      if (StrNCmp(aapThis->cCDDid,"smart0",6) == 0) {
        cTemp = strdup(txsp->title);
        aapThis->cGraphId = strdup(strtok(cTemp,","));
	MemFree(cTemp);
      } else {
        aapThis->cGraphId = aapThis->cCDDid;
      }
    } else if (StringICmp(aapThis->cDatabase,"Pfam") ==0) {
      aapThis->blue = 255;
      aapThis->red = aapThis->green = iColValue;
      cTemp = strdup(txsp->title);
      aapThis->cGraphId = strdup(strtok(cTemp,","));
      MemFree(cTemp);
    } else if (StringICmp(aapThis->cDatabase,"scop1.39") ==0) {
      aapThis->green = 255;
      aapThis->red = aapThis->blue = iColValue;
      aapThis->bIsProfile = TRUE;    
      aapThis->cGraphId = aapThis->cCDDid;
    } else if (StringICmp(aapThis->cDatabase,"Load") ==0) {
      aapThis->green = iColValue;
      aapThis->red = aapThis->blue = 255;    
      cTemp = strdup(aapThis->cCDDid);
      if (strstr(cTemp,":")) {
        strtok(cTemp,":");
        aapThis->cGraphId = strdup(strtok(NULL,":"));
      } else {
        strtok(cTemp,"_");
        aapThis->cGraphId = strdup(strtok(NULL,"_"));
      }
      MemFree(cTemp);
    } else {
      aapThis->green = iColValue;
      aapThis->red = aapThis->blue = 255;    
      aapThis->cGraphId = aapThis->cCDDid;
    }
    sprintf(aapThis->name,"ali%d",(Int4)random());
    aapThis->bIsOasis = bDbIsOasis;
    if (bDbIsOasis) {
      strcpy(path,CDDPrefix); strcat(path,dbversion); strcat(path,CDDPost_O);
      strcpy(hpath,OASIScgi);
    } else {
      strcpy(path,CDDPrefix); strcat(path,dbversion); strcat(path,CDDPost_C);
      strcpy(hpath,CDDcgi);
    }
    strcat(path,"/"); strcat(path,aapThis->cCDDid); strcat(path,TREextens);
    strcat(hpath,aapThis->cCDDid);
    strcat(hpath,"&version=");
    strcat(hpath,dbversion);
    aapThis->cHtmlLink = strdup(hpath);
/*---------------------------------------------------------------------------*/
/* Open Cdd tree file and add description to aapThis data structure          */
/* changed to binary read to get prepared for the v1.00 rollout. Aron 6/12/00*/
/*---------------------------------------------------------------------------*/
    pcddt = (CddTreePtr) CddTreeReadFromFile(path,TRUE);
    if (!pcddt) WRPSBHtmlError("Could not open CD tree file!");
    aapThis->description = pcddt->description;
    description = aapThis->description;
    aapThis->bHasStructure = FALSE;
    while (description) {
      if (description->choice == CddDescr_comment) {
        cCurrDesc = description->data.ptrvalue;
        if (strcmp(cCurrDesc,"linked to 3D-structure")==0) {
          aapThis->bHasStructure = TRUE;
          *bAnyPdb = TRUE;
          break;
        }
      }
      description = description->next;
    }
/*---------------------------------------------------------------------------*/
/* get the file name for the FASTA-sequence file which stores the subject..  */
/*---------------------------------------------------------------------------*/
    aapThis->cSeqFile = MemNew(PATH_MAX*sizeof(Char));
    if (bDbIsOasis) {
      strcpy(aapThis->cSeqFile,CDDPrefix); strcat(aapThis->cSeqFile,dbversion);
      strcat(aapThis->cSeqFile,CDDPost_O);
    } else {
      strcpy(aapThis->cSeqFile,CDDPrefix); strcat(aapThis->cSeqFile,dbversion);
      strcat(aapThis->cSeqFile,CDDPost_C);
    }
    strcat(aapThis->cSeqFile,"/"); strcat(aapThis->cSeqFile,aapThis->cCDDid);
    strcat(aapThis->cSeqFile,SEQextens);
       
/*---------------------------------------------------------------------------*/
/* Check for overlaps and determine row number                               */
/* Check for mutal overlap if condensed graphics is selected                 */
/*---------------------------------------------------------------------------*/
    iOvrlap = MemNew(maxrow*sizeof(Int4));
    iMutual = MemNew(maxrow*sizeof(Int4));
    for (i=0;i<maxrow;i++) iOvrlap[i]=0;
    aapThis->row = 1; aapThis->bDrawThisOne = TRUE;
    if (aapHead) {    
      aapTmp = aapHead;
      while (aapTmp) {
        if (aapTmp->bDrawThisOne) {
          if (OverlapInterval(aapTmp->mstart,aapTmp->mstop,
                              aapThis->mstart,aapThis->mstop)) iOvrlap[aapTmp->row-1] = 1;
          if (iGraphMode == 1) {
            if (OverlapMutual(aapTmp->mstart,aapTmp->mstop,
                              aapThis->mstart,aapThis->mstop)) iMutual[aapTmp->row-1] = 1;
          }
        }
        aapTmp = aapTmp->next;
      }
      aapThis->row = 0;
      for (i=0;i<maxrow;i++) {
        if (iOvrlap[i]==0) {
          aapThis->row = i+1;
          break;
        }
      }
      if (iGraphMode == 1) for (i=0;i<maxrow;i++) {
        if (iMutual[i]==1) {
          aapThis->bDrawThisOne = FALSE;
          break;
        }
      }
      if (aapThis->row == 0) aapThis->row = maxrow+1;
    }
    MemFree(iMutual);
    MemFree(iOvrlap);
    if (aapThis->row > maxrow) maxrow = aapThis->row;

    aapThis->next = NULL;

    if (aapHead==NULL) {
       aapHead = aapThis;
       aap = aapHead;
    } else {
       aap->next = aapThis;
       aap = aapThis;
    }
    sap = sap->next;
  }
  *mxr = maxrow;
  return(aapHead);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean WRPSBPrintDbInformationBasic (CharPtr database, Boolean is_aa, Int4 line_length, CharPtr definition, Int4 number_seqs, Int8 total_length, FILE *outfp, Boolean html)

{

        asn2ff_set_output(outfp, NULL);

        ff_StartPrint(0, 0, line_length, NULL);
        if (html)
                ff_AddString("<b>Database:</b> ");
        else
                ff_AddString("Database: ");
        ff_AddString(definition);
        NewContLine();
        TabToColumn(12);
        ff_AddString(Ltostr((long) number_seqs, 1));
        ff_AddString(" PSSMs; ");
        ff_AddString(Nlm_Int8tostr(total_length, 1));
        ff_AddString(" total columns");
        NewContLine();
        ff_EndPrint();

        return TRUE;

}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* this is the main wrapper routine for formatting html-rps-blast-output     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void QRPSBViewSeqAlign(SeqAlignPtr seqalign, BioseqPtr query_bsp, 
                                 ValNodePtr mask, Int4 iGraphMode, Int4 iPairMode,
                                 BLAST_OptionsBlkPtr options, Boolean believe_query,
                                 Uint4 print_options, CharPtr version, CharPtr date,
                                 CharPtr database, CharPtr dbversion, Int4 iBGram)
{
  BlastDbinfoPtr         dbinfo = NULL;
  AsnIoPtr               aip;
  BlastPruneSapStructPtr prune;
  Boolean                bAnyPdb;
  Char                   tableName[PATH_MAX]; 
  FILE                   *table = NULL;
  AlignmentAbstractPtr   aap;
  Int4                   maxrow;
  Boolean                is_na = FALSE;
  Uint1                  ColorSchema;
  CharPtr                title = NULL, cPtr;
/*  CddHitPtr              cdh; */
    

  switch (iPairMode) {
    case 0:
      ColorSchema = DDV_BLAST_COLOR1;
      break;
    case 1:
      ColorSchema = DDV_BLAST_COLOR2;
      break;
    default:
      ColorSchema = DDV_BLAST_COLOR3;
      break;
  }
/*---------------------------------------------------------------------------*/
/* for whatever reason dumping output to a file doesn't work for rpsblast on */
/* the scarecrow www-server; this must be fixed later                        */
/*---------------------------------------------------------------------------*/
#ifdef RESULTS_FILE
  strcpy(tableName,(CharPtr) GetTempName("wrpsb")); 
  if  (!(table = FileOpen(tableName,"w"))) {
    WRPSBHtmlError("Temp File Open Failed on Server");
  }
#endif
#ifndef RESULTS_FILE
  table = stdout;
#endif

  ObjMgrSetHold();
  init_buff_ex(128);

  fprintf(table,"Content-type: text/html\n\n");
  if (iBGram < 1) {
    fprintf(table,"<HTML>\n");
    fprintf(table,"<TITLE>CD-Search Results</TITLE>\n");
    fprintf(table,"<BODY BGCOLOR=#FFFFFF>\n");
  }
  if (iBGram < 0) {
    fprintf(table,"<A HREF=\"%sblast_form.map\">\n",URLcgi);
    fprintf(table,"<IMG SRC=\"%scdsearch.gif\" BORDER=0 ISMAP></A>\n",URLcgi);
    fprintf(table,"<PRE>\n");
    BlastPrintVersionInfoEx("RPS-BLAST", TRUE, version, date, table);
    fprintf(table, "\n");
    QRPSBAcknowledgeBlastQuery(query_bsp, 70, table, believe_query, TRUE);
    fprintf(table,"\n");
    init_buff_ex(85);
    if (dbinfo) {
      WRPSBPrintDbInformationBasic(database, TRUE, 70, dbinfo->definition, dbinfo->number_seqs, dbinfo->total_length, table, TRUE);
      title = StringSave(dbinfo->definition);
    }
    if (title) {
      cPtr = strstr(title,".v");
      if (cPtr) {
        cPtr++; StrCpy(dbversion,cPtr);
      } else StrCpy(dbversion, CDDefault);
    } else StrCpy(dbversion, CDDefault);
    
    free_buff();
    fprintf(table,"</PRE>\n");
  }
  prune = WRPSBlastPruneHitsFromSeqAlign(seqalign,options->hitlist_size,
                                         options->expect_value, NULL);
  if (prune->number == 0) {
    if (iGraphMode) {
      WRPSBCl3PrintGraphics(NULL,table,0,query_bsp,mask, iGraphMode);
    }
    fprintf(table, "<br><strong>...No hits found!</strong>\n");
  } else {
/*    cdh = RPSBgetCddHits(prune->sap); */
    aap = WRPSBCl3AbstractAlignment(prune,query_bsp,WRPSB_GRAPH_WIDTH-10,&maxrow,iGraphMode,dbversion, &bAnyPdb);
    if (iBGram < 0 && bAnyPdb) {
      fprintf(table, "&nbsp;<A HREF=\"cdd_help.shtml#RPSBPinkdot\"><IMG SRC=\"%spinkb.gif\" BORDER=0></A>&nbsp;&nbsp; .. This CD alignment includes 3D structure. To display structure, download \n",URLcgi);
      fprintf(table, "<STRONG><A HREF=\"/Structure/CN3D/cn3d.shtml\">Cn3D v3.00</A></STRONG>!\n");
      
    }
    if (iGraphMode) {
      WRPSBCl3PrintGraphics(aap,table,maxrow,query_bsp,mask, iGraphMode);
    }
    if (iBGram < 0) {
      WRPSBPrintDefLinesFromSeqAlign(aap,table,bAnyPdb,query_bsp,URLcgi);
      if (ISA_na(query_bsp->mol)) is_na = TRUE;
      WRPSBCl3DisplayBlastPairList(aap,mask,table,is_na,print_options,ColorSchema,query_bsp,dbversion,URLcgi);
      free_buff();
      fprintf(table,"</PRE>\n");
    }
/*    fprintf(table,"</FORM>\n"); */
  }
  if (iBGram < 1) {
    fprintf(table,"</BODY>\n");
    fprintf(table,"</HTML>\n");
  }
  fflush(table);
  if (table != stdout) {
    fclose(table);
    PrintFile(tableName);
    RemoveTempFiles();
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* output a page indicating WAIT status                                      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void QRPSBWait(CharPtr rid, Int4 iGraphMode, Int4 iPairMode, Int4 HowLong, Nlm_FloatHi expect, Int4 nhits)
{
  printf("Content-type: text/html\n\n");
  printf("<HTML>\n");
  printf("<TITLE>CD-Search request %s</TITLE>\n", rid);
  printf("<BODY BGCOLOR=#FFFFFF>\n");
  printf("<A HREF=\"blast_form.map\">\n");
  printf("<IMG SRC=\"%scdsearch.gif\" BORDER=0 ISMAP></A>\n",URLcgi);
  HowLong += 5;
  if (HowLong <= 60) {
    printf("<meta http-equiv=\"refresh\" content=\"%d; url=%s%s?RID=%s&GRAPH=%d&PAIR=%d&EXPECT=%f&NHITS=%d&WAIT=%d\">\n",
           HowLong,URLcgi,QRPSBNAME,rid,iGraphMode,iPairMode,expect,nhits,HowLong);
  }
  printf("<br>\n");
  printf("<h2>Waiting for BLAST-queue to finish</H2>\n",rid);
  printf("<FORM ACTION=\"%s\" METHOD=POST" ">\n", QRPSBNAME);
  printf("<INPUT TYPE=\"HIDDEN\" name=\"GRAPH\" value=\"%d\">\n",iGraphMode);
  printf("<INPUT TYPE=\"HIDDEN\" name=\"PAIR\" value=\"%d\">\n",iPairMode);
  printf("<INPUT TYPE=\"HIDDEN\" name=\"EXPECT\" value=\"%f\">\n",expect);
  printf("<INPUT TYPE=\"HIDDEN\" name=\"NHITS\" value=\"%d\">\n",nhits);
  printf("<INPUT TYPE=\"HIDDEN\" name=\"WAIT\" value=\"%d\">\n",HowLong);
  printf("Retrieve CD-Search request # ");
  printf("<INPUT type=\"TEXT\" name=\"RID\" value=\"%s\" maxlength=40 size=25>\n", rid);
  if (HowLong > 60) {
    printf("<h4>Page will not refresh automatically,</H4>\n");
    printf("<INPUT TYPE=\"submit\" VALUE=\"Click Here to check again!\">\n");
  } else {
    printf("<INPUT TYPE=\"submit\" VALUE=\"Check\">\n");
  }
  printf("</FORM>\n");
  printf("</BODY>\n");
  printf("</HTML>\n");
  fflush(stdout);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* "main" function                                                           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int2 Main (void)
 
{
  AsnIoPtr            aip;
  BioseqPtr           query_bsp, bsp, query;
  BLAST_OptionsBlkPtr options;
  BLAST_KarlinBlkPtr  ka_params=NULL, ka_params_gap=NULL;
  BlastResponsePtr    response;
  BlastVersionPtr     blast_version;
  CharPtr             blast_database, blast_inputfile, blast_outputfile;
  CharPtr             ret_buffer=NULL, params_buffer=NULL, chptr;
  CharPtr             date, motd, version, sequence;
  CharPtr             accession       = NULL;
  CharPtr             www_arg, cPtr;
  CharPtr             outptr          = NULL;
  CharPtr             cPrecalcName, rid;
  CharPtr             cSapDBTitle     = NULL;
  CharPtr             program, database;
  Int4Ptr             giptr           = NULL;
  SeqAnnotPtr         sap             = NULL;
  SeqAlignPtr         salp            = NULL;
  SeqIdPtr            sip;
  SeqLocPtr           slp;
  SeqEntryPtr         sep             = NULL;
  SeqIdPtr            seqid_list      = NULL;
  ValNodePtr          other_returns;
  ValNodePtr          error_returns   = NULL;
  ValNodePtr          mask_loc, vnp;
  WWWInfoPtr          www_info;
  WWWInfoDataPtr      info;
  struct rlimit       rl;
  Boolean             bQueryIsFasta;
  Boolean             db_is_na, query_is_na, show_gi, believe_query=FALSE;
  Boolean             html            = TRUE;
  Boolean             status;
  Boolean             is_network;
  Char                blast_program[32];
  Char                dbversion[6], dbname[16];
  Char                path[PATH_MAX];
  Uint1               align_type, align_view;
  Int2                num_of_queries, retval;
  Int2                Qstatus = 1;
  Int2                Qmode = FRONTPAGE;
  Uint4               align_options, print_options;
  Uint4               Qsleep = 0;
  Int4                iPassHit = 0;
  Int4                indx, gi, numgi;
  Int4                startloc, endloc;
  Int4                number_of_descriptions, number_of_alignments;
  Int4                iGraphMode      = 2;
  Int4                iPairMode       = 2;
  Int4                iBGram          = -1;
  Int4                iHowLong        = 5;
  FILE                *infp, *fp;
    
/*---------------------------------------------------------------------------*/
/* this sets up the unix time limit                                          */
/*---------------------------------------------------------------------------*/
  getrlimit(RLIMIT_CPU, &rl);
  rl.rlim_max = rl.rlim_cur = CPUTIME_MAX;
  setrlimit(RLIMIT_CPU, &rl);

/*---------------------------------------------------------------------------*/
/* Get Default Arguments (but DON'T read from command line)                  */
/*---------------------------------------------------------------------------*/
  if (!WRPSBGetArgs ("blastcl3", NUMARGS, myargs)) {
    WRPSBHtmlError("Can't read Arguments!");
  }

/*---------------------------------------------------------------------------*/
/* retrieve names for directories etc.                                       */
/*---------------------------------------------------------------------------*/
  if (!CddGetParams()) WRPSBHtmlError("Couldn't read from config file...");

  StringCpy(blast_program, myargs [0].strvalue);
  ErrSetOptFlags(EO_LOGTO_USRFILE);
  ErrSetFatalLevel (SEV_MAX);                    /* never die from ErrPostEx */

/*---------------------------------------------------------------------------*/
/* For RPS Blast - anything not "blastp" - is "tblastn"                      */
/*---------------------------------------------------------------------------*/
  if(myargs[32].intvalue) {
    if(StringICmp(blast_program, "blastp")) {
      StringCpy(blast_program, "blastx");
    }
  }

/*---------------------------------------------------------------------------*/
/* Begin processing www information block                                    */
/*---------------------------------------------------------------------------*/
  if (WWWGetArgs(&www_info) != WWWErrOk) {
    WRPSBHtmlError("Failed to process posting - check your get/post syntax.");
  }
  if (WWWGetMethod(www_info) == COMMAND_LINE) {
    if (GetArgc() <2) Qmode = FRONTPAGE; else Qmode = SUBMITJOB; 
  } else if (WWWGetMethod(www_info) == WWW_GET) {
    if (WWWGetNumEntries(www_info)<1) {
      if (!WRPSBDrawPage()) WRPSBHtmlError("Could not draw initial page...");
      return 1;
    } else Qmode = SUBMITJOB;
  } else if (WWWGetMethod(www_info) == WWW_POST) {
    Qmode = SUBMITJOB;
  } else Qmode = FRONTPAGE;
  if (Qmode == FRONTPAGE)
    if (!WRPSBDrawPage()) WRPSBHtmlError("Could not draw initial page...");

/*---------------------------------------------------------------------------*/
/* anything beyond this point assumes that a search is launched, or that     */
/* results are being formatted                                               */
/*---------------------------------------------------------------------------*/
  blast_inputfile  = myargs [2].strvalue;
  blast_outputfile = myargs [5].strvalue;
  if (myargs[26].intvalue) html = TRUE;
  align_view = (Int1) myargs[4].intvalue;
  align_type = BlastGetTypes(blast_program, &query_is_na, &db_is_na);
  if(align_type == blast_type_undefined) WRPSBHtmlError("Blast Type not defined!");
  if (StringICmp("blastx", blast_program) == 0) {
    if (align_view != 0) {
      ErrPostEx(SEV_FATAL, 0, 0, "This option is not available with blastx");
      return 1;
    }
  } else if (StringICmp("tblastx", blast_program) == 0) {
    if (align_view != 0) {
      ErrPostEx(SEV_FATAL, 0, 0, "This option is not available with tblastx");
      return 1;
    }
  }
  options = BLASTOptionNew(blast_program, (Boolean) myargs [16].intvalue);
  if (options == NULL) WRPSBHtmlError("Could not initialize Blast Options!");
/*---------------------------------------------------------------------------*/
/* If option RPS Blast set - option "program" is ignored by the engine       */
/*---------------------------------------------------------------------------*/
  if (myargs[32].intvalue) options->is_rps_blast = TRUE;
    
/*---------------------------------------------------------------------------*/
/* get the WWW arguments here                                                */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "RID")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    rid = strdup(www_arg);
    if (rid && strlen(rid)) Qmode = FORMATJOB;
  } 
  
  if ((indx = WWWFindName(www_info, "BGRAM")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    iBGram = (Int4) atoi(www_arg);
  }
/*---------------------------------------------------------------------------*/
/* everything that may have to do with output formatting should be put here  */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"EXPECT")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    myargs[3].floatvalue = (FloatHi) atof(www_arg);
  }
  options->expect_value  = (Nlm_FloatHi) myargs [3].floatvalue;
  BLASTOptionSetGapParams(options, myargs[25].strvalue, 0, 0);
  blast_database   = myargs [1].strvalue;
  if ((indx = WWWFindName(www_info,"FILTER")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    myargs[6].strvalue = strdup(www_arg);
  }
  if (StringICmp(myargs[6].strvalue, "T") == 0) {
    if (StringICmp("blastn", blast_program) == 0)
      options->filter_string = StringSave("D");
    else
      options->filter_string = StringSave("S");
  } else {
    options->filter_string = StringSave(myargs[6].strvalue);
  }
  if ((indx = WWWFindName(www_info,"NHITS")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    myargs[13].intvalue = (Int4) atoi(www_arg);
    myargs[14].intvalue = (Int4) atoi(www_arg);
  }
  if ((indx = WWWFindName(www_info,"WAIT")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
     iHowLong = (Int4) atoi(www_arg);
     if (iHowLong < 5) iHowLong = 5;
  }
  number_of_descriptions = myargs[13].intvalue;  
  number_of_alignments = myargs[14].intvalue;  
  options->hitlist_size = MAX(number_of_descriptions, number_of_alignments);
  if ((indx = WWWFindName(www_info,"GRAPH")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    iGraphMode = (Int4) atoi(www_arg);
    if (iGraphMode > 2) iGraphMode = 2;
    if (iGraphMode < 0) iGraphMode = 0;
  }
  if ((indx = WWWFindName(www_info,"PAIR")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    iPairMode = (Int4) atoi(www_arg);
    if (iPairMode > 2) iPairMode = 2;
    if (iPairMode < 0) iPairMode = 0;
  }
  if ((indx = WWWFindName(www_info,"VERSION")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    strcpy(dbversion,www_arg);
  } else {
    strcpy(dbversion,CDDefault);
  }
  if (Qmode == SUBMITJOB && (indx = WWWFindName(www_info,"DATALIB")) < 0) {
    WRPSBHtmlError("DATALIB missing from input!");
  } else if (indx >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    myargs[1].strvalue = strdup(www_arg);
  }
  if ((indx = WWWFindName(www_info,"IS_PROT")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    if ((Int4) atoi(www_arg) == 0) query_is_na = TRUE;
    else query_is_na = FALSE;
  }

/*---------------------------------------------------------------------------*/
/* .. before it's determined whether output should be formatted or not!      */
/*---------------------------------------------------------------------------*/
  if (Qmode != SUBMITJOB) {
    ReadDBBioseqFetchEnable ("qrpsb", blast_database, db_is_na, TRUE);
    Qstatus = (Int2) QBlastGetResults(rid,&sap,&query,&program,&database,&other_returns, &error_returns);
    if (Qstatus < 0) {
      WRPSBHtmlError("Error retrieving CD-Search request from BLAST Queue!");
      return(1);
    }
    if (Qstatus > 0) {
      QRPSBWait(rid,iGraphMode,iPairMode,iHowLong,options->expect_value, options->hitlist_size);
      exit(0);
    }
    if (!sap) sap = (SeqAnnotPtr) BLASTGetSeqAnnotByRID(rid);
    if (!sap)  WRPSBHtmlError("BLAST queue did not return alignment");
    if (!query)  WRPSBHtmlError("BLAST queue did not return query sequence");
    
    salp = sap->data;
    date = BlastGetReleaseDate();
    version = BlastGetVersionNumber();
    if (!salp) {
      if (error_returns) {
        WRPSBHtmlError("No sequence alignment retrieved!");
      }
    } 
    mask_loc = NULL;
/*    if (StringICmp(myargs[6].strvalue, "F")) { */
      for (vnp=other_returns; vnp; vnp = vnp->next) {
        switch (vnp->choice) {
          case SEQLOC_MASKING_NOTSET:
          case SEQLOC_MASKING_PLUS1:
          case SEQLOC_MASKING_PLUS2:
          case SEQLOC_MASKING_PLUS3:
          case SEQLOC_MASKING_MINUS1:
          case SEQLOC_MASKING_MINUS2:
          case SEQLOC_MASKING_MINUS3:
            ValNodeAddPointer(&mask_loc, vnp->choice, vnp->data.ptrvalue);
            break;
          default:
            break;
        }
      }
/*    } */
    QRPSBViewSeqAlign(salp, query, mask_loc, iGraphMode, iPairMode, options,
                      believe_query, print_options, version, date,
                      blast_database, dbversion,iBGram);
    exit(0);
  }

/*---------------------------------------------------------------------------*/
/* if query needs to be sent off, continue asking for further options        */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"SMODE")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    iPassHit = (Int4) atoi(www_arg);
    if (iPassHit == 0) {
      options->multiple_hits_only = TRUE;
      options->two_pass_method = FALSE;
    } else if (iPassHit == 1) {
      options->multiple_hits_only = FALSE;
      options->two_pass_method = FALSE;
    } else if (iPassHit == 2) {
      options->multiple_hits_only = FALSE;
      options->two_pass_method = TRUE;
    }
  }

  if (Qmode == SUBMITJOB && (indx = WWWFindName(www_info,"INPUT_TYPE")) < 0) {
    WRPSBHtmlError("INPUT_TYPE description missing from input!");
  } else if (indx >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    if (strcmp(www_arg,"access") == 0) {
      bQueryIsFasta = FALSE;
      myargs[21].intvalue = 1;
      believe_query = TRUE;
    } else {
      bQueryIsFasta = TRUE;
      myargs[21].intvalue = 0;
      believe_query = FALSE;
    }    
  }
  believe_query = FALSE; if (myargs[21].intvalue != 0) believe_query = TRUE;
  if (believe_query == FALSE && myargs[20].strvalue) {
    ErrPostEx(SEV_FATAL, 0, 0, "-J option must be TRUE to produce a SeqAlign file");
  }

  UseLocalAsnloadDataAndErrMsg ();
  if (! SeqEntryLoad()) return 1;
/*---------------------------------------------------------------------------*/
/* Initialize Entrez which may be needed to retrieve query sequence          */
/*---------------------------------------------------------------------------*/
  if (!EntrezInit("WWW_NEW_BLAST",FALSE,&is_network)) {
    WRPSBHtmlError("Cannot initialize Entrez!");
  }
  EntrezBioseqFetchEnable("blast_2.0MT",TRUE);

  if ((indx = WWWFindName(www_info,"SEQUENCE")) < 0) {
    WRPSBHtmlError("Query SEQUENCE missing from input!");
  } else {
    sequence = WWWGetValueByIndex(www_info,indx);
    if (sequence == NULL || sequence[0]==NULLB) {
      WRPSBHtmlError("Query SEQUENCE missing from input!");
    }
    if (bQueryIsFasta) {
      if((sep=FastaToSeqBuffEx(sequence, &outptr,FALSE,NULL,FALSE))==NULL)
        WRPSBHtmlError("Can not convert FASTA formatted sequence!");
      if(sep->choice != 1) WRPSBHtmlError("Conversion from FASTA failed!");
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (ISA_na(bsp->mol)) query_is_na = TRUE;
    } else {
      chptr = sequence;
      while (IS_WHITESP(*chptr) && *chptr != NULLB) chptr++;
      while (IS_ALPHANUM(*chptr) || *chptr == '_' || *chptr == '.') chptr++;
      *chptr = NULLB;
      accession = sequence;
      sip = NULL;
      numgi = AccessionToGi(accession,&giptr,TYP_AA);
      if (numgi > 0) {
        gi = giptr[0];
        if (gi > 0) ValNodeAddInt(&sip,SEQID_GI,gi); 
      } else WRPSBHtmlError("Could not find sequence!");
      bsp = BioseqLockById(sip);
      if (ISA_na(bsp->mol)) query_is_na = TRUE;
    }
    if ((query_bsp = bsp) == NULL || bsp->length <= 0)
      WRPSBHtmlError("Conversion to BioSeq failed!");         
  }

  if (query_is_na) {
    WRPSBHtmlError("Can not process nucleotide sequences!");
  }

  if (myargs[7].intvalue != 0)
    options->gap_open = myargs[7].intvalue;
  if (myargs[8].intvalue != 0)
    options->gap_extend = myargs[8].intvalue;
  if (myargs[9].intvalue != 0)
    options->gap_x_dropoff = myargs[9].intvalue;
    
  show_gi = (Boolean) myargs[10].intvalue;
  if (StringICmp("blastn", blast_program) == 0) {
    options->penalty = myargs[11].intvalue;
    options->reward = myargs[12].intvalue;
  } else {
    if (myargs[15].intvalue != 0) {
      options->threshold_first = myargs[15].intvalue;
      options->threshold_second = myargs[15].intvalue;
    }
  }
    
  options->genetic_code = myargs[17].intvalue;
  options->db_genetic_code = myargs[18].intvalue;
  options->number_of_cpus = myargs[19].intvalue;
  if (myargs[22].intvalue != 0) options->wordsize = myargs[22].intvalue;
    
  print_options = 0;
  align_options = 0;
  align_options += TXALIGN_COMPRESS;
  align_options += TXALIGN_END_NUM;
  if (StringICmp("blastx", blast_program) == 0) {
    align_options += TXALIGN_BLASTX_SPECIAL;
  }
  if (show_gi) {
    align_options += TXALIGN_SHOW_GI;
    print_options += TXALIGN_SHOW_GI;
  }
  if (myargs[16].intvalue == 0) print_options += TXALIGN_SHOW_NO_OF_SEGS;
  if (align_view) {
    align_options += TXALIGN_MASTER;
    if (align_view == 1 || align_view == 3)
      align_options += TXALIGN_MISMATCH;
    if (align_view == 3 || align_view == 4 || align_view == 6)
      align_options += TXALIGN_FLAT_INS;
    if (align_view == 5 || align_view == 6)
      align_options += TXALIGN_BLUNT_END;
  } else {
    align_options += TXALIGN_MATRIX_VAL;
    align_options += TXALIGN_SHOW_QS;
  }
  if (html) {
    align_options += TXALIGN_HTML;
    print_options += TXALIGN_HTML;
  }
  if (myargs[27].strvalue) {
      options->gifile = StringSave(myargs[27].strvalue);
  }
    
  options->hsp_range_max  = myargs[28].intvalue;
  if (options->hsp_range_max != 0) options->perform_culling = TRUE;
  options->block_width  = myargs[29].intvalue;
  if (myargs[29].floatvalue) options->searchsp_eff = (Nlm_FloatHi) myargs[30].floatvalue;
  options->strand_option = myargs[31].intvalue;
  if(myargs[33].strvalue) options->entrez_query = StringSave(myargs[33].strvalue);
    

  num_of_queries=1; retval=0;
        
/*---------------------------------------------------------------------------*/
/* Read boundaries of location                                               */
/*---------------------------------------------------------------------------*/
  startloc = myargs[23].intvalue - 1;
  if (myargs[24].intvalue == -1) endloc = query_bsp->length - 1;
  else endloc = myargs[24].intvalue - 1;
        

  if (query_bsp == NULL) {
    ErrPostEx(SEV_FATAL, 0, 0, "Unable to obtain bioseq\n");
    retval = 2;
  }
        
  rid = (CharPtr) QBlastSubmitJob(query_bsp,blast_program,blast_database,options);
  if (!rid) WRPSBHtmlError("Error submitting CD-Search job to BLAST queue!");
  QRPSBWait(rid,iGraphMode,iPairMode,2,options->expect_value,options->hitlist_size);

  if (sep) sep = SeqEntryFree(sep);
  options = BLASTOptionDelete(options);
  EntrezFini();
  return(retval);
}
