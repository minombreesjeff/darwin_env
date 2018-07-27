/* $Id: wrpsbcl3.c,v 1.38 2004/03/10 14:36:02 bauer Exp $
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
* File Name:  wrpsbcl3.c
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 4/19/2000
*
* $Revision: 1.38 $
*
* File Description:
*         WWW-RPS BLAST client
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: wrpsbcl3.c,v $
* Revision 1.38  2004/03/10 14:36:02  bauer
* cosmetic change to no-hits page
*
* Revision 1.37  2004/02/17 18:04:35  bauer
* added citation, prepared for alignment resorting
*
* Revision 1.36  2003/11/19 14:34:31  bauer
* changes to support SeqAnnot export
*
* Revision 1.35  2003/10/07 21:15:15  bauer
* support generation of Sequence Annotation from CD-Search results
*
* Revision 1.34  2003/07/25 21:00:28  bauer
* changes to accomodate KOGs
*
* Revision 1.33  2003/05/12 15:23:38  bauer
* dealing with format changes in BlastRequestDbInfo output
*
* Revision 1.32  2003/04/25 14:42:12  bauer
* changes to BLAST database interface
*
* Revision 1.31  2003/01/10 14:47:47  bauer
* fixed problem with CDART connectivity
*
* Revision 1.30  2003/01/09 21:56:29  bauer
* reduced default number of hits to 25
*
* Revision 1.29  2002/12/24 18:22:49  bauer
* changes for v1.60
*
* Revision 1.28  2002/10/09 20:30:05  bauer
* increased max. number of CDART neighbors
*
* Revision 1.27  2002/08/06 02:39:10  bauer
* changes to accomodate COGs
*
* Revision 1.26  2002/07/05 21:11:50  bauer
* made PIC string a bit more compact
*
* Revision 1.25  2002/06/26 19:44:49  bauer
* CDART now returns search database versions
*
* Revision 1.24  2002/06/25 21:23:28  bauer
* revised Sequence retrieval interface
*
* Revision 1.23  2002/06/12 15:04:17  bauer
* 6/11/02 update
*
* Revision 1.22  2002/05/23 20:19:16  bauer
* intermediate version of wrpsb
*
* Revision 1.21  2002/05/06 17:18:04  bauer
* switched to graphics on the fly
*
* Revision 1.20  2002/03/07 19:12:14  bauer
* major revisions to cgi-bins and the CD-dumper
*
* Revision 1.19  2002/01/04 20:03:54  bauer
* minor changes in DefLine parsing
*
* Revision 1.18  2001/05/31 22:04:46  bauer
* changes to accomodate new type of Smart accessions
*
* Revision 1.17  2001/05/23 21:18:38  bauer
* *** empty log message ***
*
* Revision 1.16  2001/03/21 17:00:48  bauer
* fixes for changes in LOAD accessions
*
* Revision 1.15  2001/03/10 21:37:44  bauer
* JavaScript only for Mozilla 4.x and higher
*
* Revision 1.14  2001/03/09 22:52:01  bauer
* use JavaScript to embed graphics in results page
*
* Revision 1.13  2001/03/03 00:12:08  bauer
* added scaleable graphics
*
* Revision 1.12  2001/02/28 19:46:08  bauer
* fixes for public version
*
* Revision 1.11  2001/02/22 15:53:22  bauer
* support for jagged edge blocks
*
* Revision 1.10  2001/02/14 15:16:11  bauer
* changed interface to lexington
*
* Revision 1.9  2001/02/09 15:17:05  bauer
* added switch for DB-test
*
* Revision 1.8  2001/02/05 23:01:32  bauer
* added support for domain composition neighboring
*
* Revision 1.7  2000/12/08 20:13:38  bauer
* call to RPSBgetCddHits commented out
*
* Revision 1.6  2000/10/12 20:48:26  bauer
* added absolute URLs for graphics elements on the CD-search results pages
*
* Revision 1.5  2000/09/18 17:26:46  bauer
* CddHitPtr now extracted from pruned seqalign
*
* Revision 1.4  2000/08/03 22:02:24  bauer
* added support for 3D-structure link highlighting
*
* Revision 1.3  2000/07/21 21:27:08  bauer
* Added hot-links from the search page to the help document
*
* Revision 1.2  2000/07/19 19:14:18  bauer
* added modification logging
*
*
* ==========================================================================
*/

#define BLASTCLI_BUF_SIZE 255

#undef  CDSEARCH_TEST  /* use test-server instead of public rpsblast */
#define WIN_GIF

#include <ncbi.h>
#include <ncbimain.h>
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
#include <ncbiwww.h>
#include <www.h>
#include <dart.h>
#include <cddutil.h>
#include <gifgen.h>
#include <qblastnet.h>
#include <pmfapi.h>
#include <entrez2.h>

ReadDBFILEPtr rdfp        = NULL;

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
  if (CDDProper[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDProper...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDOasis", "", CDDOasis, PATH_MAX);
  if (CDDOasis[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDOasis...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "TREextens", "", TREextens, PATH_MAX);
  if (TREextens[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no TREextens...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "SEQextens", "", SEQextens, PATH_MAX);
  if (SEQextens[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no SEQextens...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "OASIScgi", "", OASIScgi, PATH_MAX);
  if (OASIScgi[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no OASIScgi...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDcgi", "", CDDcgi, PATH_MAX);
  if (CDDcgi[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDcgi...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDPrefix", "", CDDPrefix, PATH_MAX);
  if (CDDPrefix[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDPrefix...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDPost_O", "", CDDPost_O, PATH_MAX);
  if (CDDPost_O[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDPost_O...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDPost_C", "", CDDPost_C, PATH_MAX);
  if (CDDPost_C[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDPost_C...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDefault", "", CDDefault, PATH_MAX);
  if (CDDefault[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDefault...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "QUDefault", "", QUDefault, PATH_MAX);
  if (QUDefault[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no QUDefault...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSearch1", "", CDDSearch1, PATH_MAX);
  if (CDDSearch1[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSearch1...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSearch2", "", CDDSearch2, PATH_MAX);
  if (CDDSearch2[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSearch2...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSearch3", "", CDDSearch3, PATH_MAX);
  if (CDDSearch3[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSearch3...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSearch4", "", CDDSearch4, PATH_MAX);
  GetAppParam("cdd", "CDDSRV", "CDDSearch5", "", CDDSearch5, PATH_MAX);
  GetAppParam("cdd", "CDDSRV", "CDDSearch6", "", CDDSearch6, PATH_MAX);
  GetAppParam("cdd", "CDDSRV", "CDDSearch7", "", CDDSearch7, PATH_MAX);
  GetAppParam("cdd", "CDDSRV", "CDDSname1", "", CDDSname1, PATH_MAX);
  if (CDDSname1[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSname1...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSname2", "", CDDSname2, PATH_MAX);
  if (CDDSname2[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSname2...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSname3", "", CDDSname3, PATH_MAX);
  if (CDDSname3[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDSname3...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDSname4", "", CDDSname4, PATH_MAX);
  GetAppParam("cdd", "CDDSRV", "CDDSname5", "", CDDSname5, PATH_MAX);
  GetAppParam("cdd", "CDDSRV", "CDDSname6", "", CDDSname6, PATH_MAX);
  GetAppParam("cdd", "CDDSRV", "CDDSname7", "", CDDSname7, PATH_MAX);
  GetAppParam("cdd", "CDDSRV", "CDDlocat", "", CDDlocat, PATH_MAX);
  if (CDDlocat[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDlocat...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "ODBCINI", "", ODBCINI, PATH_MAX);
  if (ODBCINI[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no ODBCINI...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "DARTUSER", "", DARTUSER, PATH_MAX);
  if (DARTUSER[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no DARTUSER...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "DARTPASS", "", DARTPASS, PATH_MAX);
  if (DARTPASS[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no DARTPASS...\n");
    return FALSE;
  }
  return TRUE;
}                                                       /* end GetVastParams */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Monitor hook to print to stderr for UNIX clients.                         */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static int LIBCALLBACK UNIXMonitorHook(Nlm_MonitorPtr mon, MonCode code)
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
      "cdd",       NULL, NULL, FALSE, 'd', ARG_STRING,   0.0, 0, NULL},
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
      "1",         NULL, NULL, FALSE, 'r', ARG_INT,      0.0, 0, NULL},
    { "Number of one-line descriptions (V)",                          /* 13 */
      "25",        NULL, NULL, FALSE, 'v', ARG_INT,      0.0, 0, NULL},
    { "Number of alignments to show (B)",                             /* 14 */
      "25",        NULL, NULL, FALSE, 'b', ARG_INT,      0.0, 0, NULL},
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
/* Draw the search page, version 01/02                                       */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean WRPSBDrawSearchPage() 
{
  CharPtr databases[7] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};
  CharPtr datab_nam[7] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL};
  Char    *expect[] = { "0.000001","0.0001", "0.01", 
                        "1", "10", "100", NULL};
  Char    *nhits[] = {"10","25","50","100","250",NULL};
  Int4    i;


  databases[0] = CDDSearch1;
  databases[1] = CDDSearch2;
  databases[2] = CDDSearch3;
  databases[3] = CDDSearch4;
  datab_nam[0] = CDDSname1;
  datab_nam[1] = CDDSname2;
  datab_nam[2] = CDDSname3;
  datab_nam[3] = CDDSname4; 
  if (Nlm_StrCmp(CDDlocat,"inhouse")==0) {
    databases[4] = CDDSearch5;
    databases[5] = CDDSearch6;
    databases[6] = CDDSearch7;
    datab_nam[6] = CDDSname7;
    datab_nam[4] = CDDSname5; 
    datab_nam[5] = CDDSname6; 
  }



  WRPSBSearchHead(NULL,NULL,FALSE,FALSE);

  printf("<br><H4>Search the <A HREF=\"cdd.shtml\">Conserved Domain Database</A> with Reverse Position Specific BLAST</H4>\n");

  printf("<FORM ACTION=\"%s\" METHOD=POST" ">\n", RPSBNAME);

  printf("Search Database: <SELECT NAME=\"DATALIB\">\n");
  for (i=0; databases[i] != NULL && i<7; i++) {        
    if (Nlm_StrCmp(databases[i],DEFAULT_DATALIB)==0) {
      printf("<OPTION VALUE=\"%s\" SELECTED> %s\n",databases[i],datab_nam[i]);
    } else {
      printf("<OPTION VALUE=\"%s\"> %s\n",databases[i],datab_nam[i]);
    }
  }
  printf("</SELECT>\n");

  printf("<BR>Enter <strong>Protein</strong> Query as Accession, Gi, or Sequence in FASTA format \n");
  printf("<INPUT TYPE=\"submit\">\n");
  printf("<INPUT TYPE=\"reset\">\n");
  printf("<BR><TEXTAREA NAME=\"SEQUENCE\" rows=8 cols=64></TEXTAREA>\n");
  printf("<BR><BR>Please read about <A HREF=\"http://www.ncbi.nlm.nih.gov/BLAST/fasta.html\">"
         "FASTA</A> format description<BR>\n");
  printf("<HR>\n");

  printf("<BR><strong>Advanced options for the BLAST server:</strong><BR><BR>\n");
  
  printf("<a href=\"cdd_help.shtml#WRPSBExpect\">Expect&nbsp;</a>");

  printf("<SELECT name = \"EXPECT\">\n");
  for (i=0; expect[i] != NULL; i++) {   
    if (Nlm_StrCmp(expect[i],DEFAULT_EVALUE)==0) {
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
  printf("<OPTION VALUE=\"1\"> Single hit 1-pass\n");
  printf("<OPTION VALUE=\"2\"> 2-pass\n");
  printf("</SELECT><BR>\n");
  
  printf("<BR><BR><strong>Output formatting options:</strong><BR><BR>\n");

  printf("Display <A HREF=\"cdd_help.shtml#WRPSBHits\">up to</A> <SELECT name = \"NHITS\">\n");
  for (i=0; nhits[i] != NULL; i++) {    
    if (Nlm_StrCmp(nhits[i],DEFAULT_NHITS)==0) {
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
  printf("</SELECT><BR>\n");
  printf("Print Graphics using&nbsp<SELECT NAME=\"GW\">\n");
  printf("<OPTION VALUE=\"-5\">5 pixels per residue\n");
  printf("<OPTION VALUE=\"-2\">2 pixels per residue\n");
  printf("<OPTION SELECTED VALUE=\"-1\">Default Width\n");
  printf("<OPTION VALUE=\"1\">1 residue per pixel\n");
  printf("<OPTION VALUE=\"2\">2 residues per pixel\n");
  printf("<OPTION VALUE=\"5\">5 residues per pixel\n");
  printf("<OPTION VALUE=\"10\">10 residues per pixel\n");
  printf("</SELECT>\n");

  printf("<hr>\n");
  printf("Send job to Queue:&nbsp;<INPUT TYPE=\"checkbox\" VALUE=\"T\" NAME=\"QUEUE\">&nbsp;&nbsp;\n");

  printf("or retrieve CD-Search request #<INPUT type=\"TEXT\" name=\"RID\" maxlength=40 size=18>\n");
  printf("<INPUT TYPE=\"submit\" VALUE=\"Submit\">\n");
  printf("</FORM>\n");
  printf("<hr>\n");

  printf("<b><a href=\"http://www.ncbi.nlm.nih.gov:80/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=12520028&dopt=Abstract\">Citing CD-Search</a>:</b>\n");
  printf(" Marchler-Bauer A, Anderson JB, DeWeese-Scott C, Fedorova ND, Geer LY, He S, Hurwitz DI, Jackson JD, Jacobs AR,\n");
  printf(" Lanczycki CJ, Liebert CA, Liu C, Madej T, Marchler GH, Mazumder R, Nikolskaya AN, Panchenko AR, Rao BS, Shoemaker BA,\n");
  printf(" Simonyan V, Song JS, Thiessen RA, Vasudevan S, Wang Y, Yamashita RA, Yin JJ, and Bryant SH (2003), \n");
  printf("\"<i>CDD: a curated Entrez database of conserved domain alignments</i>\",\n");
  printf(" <b>Nucleic Acids Res. 31</b>:383-387.\n");
 
 /* 
  printf("<br>\n");
  printf("<b><a href=\"http://www.ncbi.nlm.nih.gov:80/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=9254694&dopt=Abstract\">Reference</a>:</b>\n");
  printf("Altschul, Stephen F., Thomas L. Madden, Alejandro A. Sch&auml;ffer, \n");
  printf("Jinghui Zhang, Zheng Zhang, Webb Miller, and David J. Lipman (1997), \n");
  printf("\"Gapped BLAST and PSI-BLAST: a new generation of protein database search\n");
  printf("programs\",  Nucleic Acids Res. 25:3389-3402.\n");
*/
  WRPSBSearchFoot(FALSE,FALSE);
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

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* print icon to symbolize wait state for queued CD-Search/RPS-Blast         */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void QRPSBWaitIcon()
{
  gdImagePtr    im;
  Int4          orange, black;
  Char          cTmp[30] = "See Conserved Domains in CDD\0";
  
  im = gdImageCreate(400,25);
  black  = gdImageColorAllocate(im,   0,   0,   0);
  orange = gdImageColorAllocate(im, 255, 153,   0);
  gdImageFilledRectangle(im, 0, 0, 399, 24,orange);
  gdImageRectangle(im, 0, 0, 399, 24, black);
  gdImageString(im,gdFont9X15b,200 - (Nlm_StrLen(cTmp)*gdFont9X15b->w)/2,
                5,cTmp,black);
  printf("Content-type: image/gif\r\n\r\n");
  gdImageGif(im,stdout);
  exit(0);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Print the wrapper for the graphics page plus the PNG-file used in display */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void WRPSBCl3PrintGraphics(AlignmentAbstractPtr aap, FILE *table, Int4 maxrow,
                                  BioseqPtr query_bsp, ValNodePtr mask,
                                  Int4 iGraphMode, Int4 GraphWidth, Boolean bDirect,
				  Boolean bIsPrecalc, Int4 querygi)
{
  gdImagePtr           im;
  DenseSegPtr          dsp;
  gdPoint              points[3];
  FILE                *pngout;
  Int4                 gystep = WRPSB_GRAPH_HEIGHT+WRPSB_GRAPH_SPACER;
  Int4                 colidx, qlen, white, black, lblue, titlecol;
  Int4                 ulx, uly, lrx, lry, swidth, i, nsegments;
  Int4                 tick, ntick, dont, sky, missg, oval_w;
  Char                 cTmp[16];
  Int4                 right = GraphWidth - 1;
  Char                 path[PATH_MAX], dstring[PATH_MAX];
  Char                 biasstring[PATH_MAX];
  AlignmentAbstractPtr aapThis;
  SeqLocPtr            slp, slpThis;
  SeqIntPtr            sintp;
  SeqPortPtr           spp;
  ValNodePtr           vnp, vnpThis;
  static Int4          ticksteps[14] = {5,10,20,25,50,100,200,250,500,1000,
                                        2000,2500,5000,10000};
  borkIOCtx*           bio;
  Int4                 pos, nblocks = 0, nbias = 0;
  Char                 octstr[8];
  Char                 octet;
  Uint4                uintvalue;
  Char                 aln[CDD_MAX_DESCR], cTmp2[24];
  CharPtr              buf;

  biasstring[0] = '\0';

  if (query_bsp && querygi <= 0) {
    buf = MemNew((query_bsp->length+1)*sizeof(Char));
    spp = FastaSeqPort((BioseqPtr)query_bsp,FALSE,FALSE,Seq_code_ncbieaa);
    FastaSeqLine(spp,buf,query_bsp->length,FALSE);
  }
  if (aap)
    srandom((unsigned int) (query_bsp->length + aap->gstart + (Int4)getpid()));
  else 
    srandom((unsigned int) (query_bsp->length + (Int4)getpid()));
  sprintf(path,"/tmp/%ld%ld.png",(long)random(),(long)getpid());

  if (!bDirect) {
    fprintf(table,"<CENTER>\n");
    fprintf(table,"<FORM NAME=BLASTFORM>\n");
    if (!aap) {
      fprintf(table,"<input type=text name=defline size=70 value=\"No hits found ..\">");
    } else {
      fprintf(table,"<input type=text name=defline size=70 value=\"Click on boxes for multiple alignments\">");
    }
    fprintf(table,"</FORM>\n");
    fprintf(table,"<map name=\"img_map\">\n");
  }
/*---------------------------------------------------------------------------*/
/* create the gif file                                                       */
/*---------------------------------------------------------------------------*/
  im = gdImageCreate(GraphWidth,gystep*(min(maxrow,WRPSB_GRAPH_MAXROW))+30);
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
        nbias++;      
        sintp = slp->data.ptrvalue;
        ulx = 5 + ((GraphWidth-10)*sintp->from) / qlen;
        lrx = 5 + ((GraphWidth-10)*sintp->to)   / qlen;
        gdImageFilledRectangle(im,ulx,20,lrx,25,sky);
	if (!bDirect) {
          fprintf(table,"<area shape=rect coords=%d,%d,%d,%d href=\"http://www.ncbi.nlm.nih.gov/BLAST/filtered.html\" ONMOUSEOVER=\'document.BLASTFORM.defline.value=\"Masked-out region, low complexity\"\' ONMOUSEOUT=\'document.BLASTFORM.defline.value=\"Mouse-over boxes to display more information\"\'>\n",
                  ulx+1,21,lrx+1,26);
	  sprintf(cTmp,",%d,%d\0", sintp->from, sintp->to - sintp->from);
	  Nlm_StrCat(biasstring,cTmp);
        }
      } else if (slp->choice == 5) {
        slpThis = slp->data.ptrvalue;
        while (slpThis) {
          if (slpThis->choice == 4) {
            nbias++;      
            sintp = slpThis->data.ptrvalue;
            ulx = 5 + ((GraphWidth-10)*sintp->from) / qlen;
            lrx = 5 + ((GraphWidth-10)*sintp->to)   / qlen;
            gdImageFilledRectangle(im,ulx,20,lrx,25,sky);
	    if (!bDirect) {
              fprintf(table,"<area shape=rect coords=%d,%d,%d,%d href=\"http://www.ncbi.nlm.nih.gov/BLAST/filtered.html\" ONMOUSEOVER=\'document.BLASTFORM.defline.value=\"Masked-out region, low complexity\"\' ONMOUSEOUT=\'document.BLASTFORM.defline.value=\"Mouse-over boxes to display more information\"\'>\n",
                      ulx+1,21,lrx+1,26);
	      sprintf(cTmp,",%d,%d\0", sintp->from, sintp->to - sintp->from);
  	      Nlm_StrCat(biasstring,cTmp);
            }
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
  Nlm_StrCpy(cTmp,"1");
  gdImageString(im,gdFont5X8,   5,uly,(char *) cTmp,lblue);
  cTmp[0] = '\0'; sprintf(cTmp,"%d",query_bsp->length);
  dont = right-5-(strlen(cTmp)*gdFont5X8->w);
  gdImageString(im,gdFont5X8,dont,uly,(char *) cTmp,lblue);
  for (i=0;i<14;i++) {
    ntick = qlen / ticksteps[i]; if (ntick < 10) {
      tick = ticksteps[i]; break;
    }
  }
  for (i=1;i<=ntick;i++) {
    ulx = 5 + ((i*tick-1)*(GraphWidth-10))/qlen;
    if (ulx > (right-5)) ulx=right-5;
    gdImageLine(im,ulx,20,ulx,15,lblue);
    cTmp[0] = '\0'; sprintf(cTmp,"%d",i*tick);
    if (ulx+(strlen(cTmp)*gdFont5X8->w/2) < (dont-1)) {
      gdImageString(im,gdFont5X8,ulx-(strlen(cTmp)*gdFont5X8->w/2),uly,(char *) cTmp,lblue);
    }  
  }
  aapThis = aap;
  while (aapThis) {
    if (aapThis->row <= WRPSB_GRAPH_MAXROW && (iGraphMode == 2 || aapThis->bDrawThisOne)) {
      nblocks++;
      ulx = aapThis->gstart + 5;
      lrx = aapThis->gstop  + 5;
      uly = WRPSB_GRAPH_SPACER + WRPSB_GRAPH_SPACER + gystep*aapThis->row;
      lry = WRPSB_GRAPH_SPACER + gystep             + gystep*aapThis->row;
      /* gdImageRectangle(im, ulx,uly,lrx,lry,black); */
      colidx = gdImageColorExact(im,aapThis->red,aapThis->green,aapThis->blue);
      if (colidx == -1) {
        colidx = gdImageColorAllocate(im,aapThis->red,aapThis->green,aapThis->blue);
      }
      if (!bDirect) {
        dstring[0] = '\0'; missg = 0;
        if (aapThis->long_defline) {
          missg = min(50,strlen(aapThis->long_defline));
          strncpy(dstring,aapThis->long_defline,(size_t) missg);
        }
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
/*	if (!bIsPrecalc) {
          fprintf(table,"<area shape=rect coords=%d,%d,%d,%d href=\"#%s\" ALT=\"%s\" ONMOUSEOVER=\'document.BLASTFORM.defline.value=\"%s\"\' ONMOUSEOUT=\'document.BLASTFORM.defline.value=\"Mouse-over boxes to display more information\"\'>\n",
                  ulx+1,uly+1,lrx+1,lry+1,aapThis->name,FixStringForWWW(aapThis->long_defline),FixStringForWWW(dstring));
        } else { */
	  aln[0]='\0';
	  if (!aapThis->bIsProfile) {
            dsp = aapThis->salp->segs;
            nsegments = 0;
            for (i=0;i<dsp->numseg;i++) {
              if (dsp->starts[i*2] != -1 && dsp->starts[i*2 + 1] != -1) nsegments++;
            }
            sprintf(aln,"%d",nsegments);
            for (i=0;i<dsp->numseg;i++) {
              if (dsp->starts[i*2] != -1 && dsp->starts[i*2 + 1] != -1) {
                cTmp2[0]='\0';
                sprintf(cTmp2,",%d,%d,%d",dsp->starts[i*2+1],dsp->starts[i*2],dsp->lens[i]);
                strcat(aln,cTmp2);
              }
            }
            aln[strlen(aln)]='\0';
	  }
          if (querygi > 0) {
            fprintf(table,"<area shape=rect coords=%d,%d,%d,%d href=\"%scddsrv.cgi?ascbin=2&maxaln=10&seltype=3&uid=%s&querygi=%d&aln=%s\" ALT=\"%s\" ONMOUSEOVER=\'document.BLASTFORM.defline.value=\"%s\"\' ONMOUSEOUT=\'document.BLASTFORM.defline.value=\"Click on boxes for multiple alignments\"\'>\n",
                    ulx+1,uly+1,lrx+1,lry+1,URLcgi,aapThis->cCDDid,querygi,aln,
	  	    FixStringForWWW(aapThis->long_defline),FixStringForWWW(dstring));
	  } else {
            fprintf(table,"<area shape=rect coords=%d,%d,%d,%d href=\"%scddsrv.cgi?ascbin=2&maxaln=10&seltype=3&uid=%s&query=%s&aln=%s\" ALT=\"%s\" ONMOUSEOVER=\'document.BLASTFORM.defline.value=\"%s\"\' ONMOUSEOUT=\'document.BLASTFORM.defline.value=\"Click on boxes for multiple alignments\"\'>\n",
                    ulx+1,uly+1,lrx+1,lry+1,URLcgi,aapThis->cCDDid,buf,aln,
	  	    FixStringForWWW(aapThis->long_defline),FixStringForWWW(dstring));
	  }
/*	} */
      }
      ulx++; uly++; lrx--; lry--;
      if (ulx <= lrx) {
        oval_w = (lrx - ulx) / 3; 
        if (oval_w > 5) oval_w = 5;
        gdImageRoundRectangle(im,ulx,uly,lrx,lry,oval_w,3,colidx,1);
	if (aapThis->bIsArch) {
          gdImageRoundRectangle(im,ulx,uly,lrx,lry,oval_w,3,black,0);
	}
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
	  if (aapThis->indents && aapThis->nindents > 0) {
	    for (i=0;i<aapThis->nindents;i++) {
	      points[0].x = aapThis->indents[i] - 1 + 5;
	      points[0].y = lry + 1;
	      points[1].x = aapThis->indents[i] + 1 + 5;
	      points[1].y = lry + 1;
	      points[2].x = aapThis->indents[i] + 5;
	      points[2].y = lry - 2 + 1;
	      gdImageFilledPolygon(im,points,3,white);
	      points[0].x = aapThis->indents[i] - 1 + 5;
	      points[0].y = uly - 1;
	      points[1].x = aapThis->indents[i] + 1 + 5;
	      points[1].y = uly - 1;
	      points[2].x = aapThis->indents[i] + 5;
	      points[2].y = uly - 1 + 2;
	      gdImageFilledPolygon(im,points,3,white);
	    }
	  }
	}
        swidth = lrx-ulx+1;
        ulx = 5+(aapThis->gstart+aapThis->gstop)/2;
        uly = WRPSB_GRAPH_SPACER+(2*WRPSB_GRAPH_SPACER+WRPSB_GRAPH_HEIGHT)/2+gystep*aapThis->row;
        if (Nlm_StrLen(aapThis->cGraphId) >= 16) {
	  Nlm_StrNCpy(cTmp,aapThis->cGraphId,15);
	} else Nlm_StrCpy(cTmp,aapThis->cGraphId);
	titlecol = white;
	if (aapThis->colcyc == 2 || aapThis->colcyc == 4 || aapThis->colcyc == 5 || aapThis->bIsArch) titlecol = black;
        if (strlen(cTmp)*gdFont7X13b->w < swidth) {
          gdImageString(im,
                        gdFont7X13b,
                        ulx-strlen(cTmp)*gdFont7X13b->w / 2,
                        uly-gdFont7X13b->h / 2, (char *) cTmp, titlecol);
        } else if (strlen(cTmp)*gdFont5X8->w < swidth) {
          gdImageString(im,
                        gdFont5X8,
                        ulx-strlen(cTmp)*gdFont5X8->w / 2,
                        uly-gdFont5X8->h / 2, (char *) cTmp, titlecol);
        }
      }
    }
    aapThis = aapThis->next;
  }
  if (!bDirect) {
    fprintf(table,"</map>\n");
    fprintf(table,"<img src=\"%swrpsb.cgi?PIC=%d,%d,%d",
            URLcgi,GraphWidth,query_bsp->length,nblocks);
    aapThis = aap; while (aapThis) {
      if (aapThis->row <= WRPSB_GRAPH_MAXROW && (iGraphMode == 2 || aapThis->bDrawThisOne)) {
        fprintf(table,",%d,%d,%d,%s,%d,%d,%d,%d",aapThis->gstart,aapThis->gstop - aapThis->gstart,
                aapThis->row,aapThis->cGraphId,aapThis->colcyc,
	        (Int4)(aapThis->nmissg * 100.0),(Int4)(aapThis->cmissg * 100.0),
	        aapThis->nindents);
        for (i=0;i<aapThis->nindents;i++) fprintf(table,",%d",aapThis->indents[i]);
      }
      aapThis = aapThis->next;
    }
    fprintf(table,",%d",nbias);
    if (nbias) fprintf(table,"%s",biasstring);
    fprintf(table,"\" usemap=\"#img_map\" border=0 ISMAP>\n");
    fprintf(table,"</CENTER>\n");
  } else {
    fprintf(table,"Content-type: image/gif\r\n\r\n");
    gdImageGif(im,table);
    exit(0);
  }
  gdImageDestroy(im);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Open (and close) a connection to the Dart service                         */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Dart_Connect *WRPSBConnectDart() 
{
  Dart_Connect        *Connection;
  CharPtr              cOutString;
  
  cOutString = MemNew(PATH_MAX*sizeof(Char));
  sprintf(cOutString,"ODBCINI=%s",ODBCINI);
  putenv(cOutString);
  putenv("LD_LIBRARY_PATH=/opt/machine/merant/lib");
  if  (Nlm_StrCmp(CDDlocat,"inhouse")!=0) {
    Connection = Dart_Init2("CDart", DARTUSER, DARTPASS); 
  } else {
    Connection = Dart_Init2("CDart", DARTUSER, DARTPASS); 
  } 
  iDartFamNum = 0;
  Dart_CdFamily(Connection, iDartFam, DARTFAMILYNUM, &iDartFamNum);
  return (Connection);
}

static void WRPSBDisConnectDart(Dart_Connect *Connection)
{
  if (Connection) Dart_Fini(Connection);
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
static void WRPSBCl3ViewSeqAlign(SeqAlignPtr seqalign, BioseqPtr query_bsp, 
                                 ValNodePtr mask, Int4 iGraphMode, Int4 iPairMode,
                                 BLAST_OptionsBlkPtr options, Boolean believe_query,
                                 Uint4 print_options, CharPtr version,
                                 CharPtr date, BlastNet3Hptr bl3hp,
                                 CharPtr database, CharPtr dbversion, Boolean bIsPrecalc,
				 Int4 GraphWidth, Dart_Connect *Connection, Int4 querygi,
				 CharPtr dbname, Boolean bFullResults,
				 Boolean bAnnotOnly, Boolean bNoWrap, Boolean bSeqAlign)
{
  AlignmentAbstractPtr   aap, aapThis;
  AsnIoPtr               aip;
  BlastDbinfoPtr         dbinfo;
  BlastPruneSapStructPtr prune;
  CharPtr                cPtr, title           = NULL;
  SeqAnnotPtr            seqannot;
  Boolean                bAnyPdb;
  Boolean                found_score;
  Boolean                is_na                 = FALSE;
  Boolean                bDirect               = FALSE;
  Uint1                  ColorSchema;
  Int4                   score, number;
  Int4                   maxrow;
  Int4                   nhits                 = 0;
  Nlm_FloatHi            evalue, bit_score;
  Char                   cTmp[16];
  Char                   tableName[PATH_MAX]; 
  FILE                   *table                = NULL;

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
  Nlm_StrCpy(tableName,(CharPtr) GetTempName("wrpsb")); 
  if  (!(table = FileOpen(tableName,"w"))) {
    WRPSBHtmlError("Temp File Open Failed on Server",bAnnotOnly);
  }
#endif
#ifndef RESULTS_FILE
  table = stdout;
#endif

  ObjMgrSetHold();
  init_buff_ex(128);


  if (iGraphMode < 3) {
    if (!bIsPrecalc) {
      WRPSBSearchHead(NULL,NULL,bAnnotOnly, bNoWrap);
    } else {
      WRPSBSearchHead("NCBI CD Summary","NCBI Conserved Domain Summary",bAnnotOnly, bNoWrap);
    }
    if (bIsPrecalc && bFullResults) bIsPrecalc = FALSE;
  
    if (!bAnnotOnly) {
      fprintf(table,"<PRE>\n");
      fprintf(table,"<BR>\n");
      if (!bIsPrecalc) BlastPrintVersionInfoEx("RPS-BLAST", TRUE, version, date, table);
      fprintf(table, "\n");
      WRPSBAcknowledgeBlastQuery(query_bsp, 70, table, believe_query, TRUE);
      fprintf(table,"\n");
      init_buff_ex(85);
/*---------------------------------------------------------------------------*/
/* if connected to a Blast database, get the version information there!      */
/*---------------------------------------------------------------------------*/
      if (!bIsPrecalc && bl3hp) {
        dbinfo = BlastRequestDbInfo(bl3hp, database, TRUE);
        if (dbinfo) {
          WRPSBPrintDbInformationBasic(database, TRUE, 70, dbinfo->definition, dbinfo->number_seqs, dbinfo->total_length, table, TRUE);
          title = StringSave(dbinfo->definition);
        }
        dbinfo = BlastDbinfoFree(dbinfo);
        if (title) {
          cPtr = strstr(title,".v");
          if (cPtr) {
            cPtr++; StrCpy(dbversion,cPtr);
          } else StrCpy(dbversion, CDDefault);
        } else StrCpy(dbversion, CDDefault);
      } else {
        if (dbversion[0] != '\0' && dbname[0] != '\0') {
          fprintf(table,"<strong>Database:</strong> %s.%s\n",dbname,dbversion);
        }
      }
      fprintf(table,"</PRE>\n");
    }
  } else bDirect = TRUE;
  free_buff();
  if (dbversion[Nlm_StrLen(dbversion)-1] == ' ') {
    dbversion[Nlm_StrLen(dbversion)-1] = '\0';
  }

  prune = WRPSBlastPruneHitsFromSeqAlign(seqalign,options->hitlist_size,
                                         options->expect_value, NULL);
  if (prune->number == 0) {
    if (iGraphMode && !bAnnotOnly) {
      WRPSBCl3PrintGraphics(NULL,table,0,query_bsp,mask, iGraphMode, GraphWidth, bDirect, bIsPrecalc, querygi);
    } 
    if (bAnnotOnly) {
      WRPSBCl3SeqAnnot(NULL, table, bSeqAlign, FALSE);
    } else fprintf(table, "<br><strong>...No hits found!</strong><hr>\n");
  } else {
    aap = WRPSBCl3AbstractAlignment(prune,query_bsp,GraphWidth-10,&maxrow,iGraphMode,dbversion, &bAnyPdb, Connection, TRUE,
                                    CDDefault,DATApath,CDDPrefix,CDDPost_C,CDDPost_O,
				    TREextens,SEQextens,OASIScgi,CDDcgi,NULL,iDartFam,iDartFamNum, bAnnotOnly);
    if (bAnnotOnly) {
      WRPSBCl3SeqAnnot(aap, table, bSeqAlign, FALSE);
      WRPSBSearchFoot(bAnnotOnly, bNoWrap);
    }
    if (iGraphMode && !bAnnotOnly) {
      WRPSBCl3PrintGraphics(aap,table,maxrow,query_bsp,mask, iGraphMode, GraphWidth, bDirect, bIsPrecalc, querygi);
    }
/*---------------------------------------------------------------------------*/
/* print form for submitting results to Lewis' Domain Composition neighboring*/
/*---------------------------------------------------------------------------*/
    fprintf(table,"<CENTER><TABLE BORDER=\"0\" CELLPADDING=\"2\" CELLSPACING=\"2\" WIDTH=\"%d\">\n",GraphWidth);
    fprintf(table,"<TR>\n");
    fprintf(table,"<TD NOWRAP CLASS=\"medium1\">\n");
    fprintf(table,"<FORM ACTION=\"/Structure/lexington/lexington.cgi\" METHOD=\"POST\">\n");
    fprintf(table,"<INPUT TYPE=\"HIDDEN\" NAME=\"l\" VALUE=\"%d\">\n",query_bsp->length);
    fprintf(table,"<INPUT TYPE=\"HIDDEN\" NAME=\"cmd\" VALUE=\"seq\">\n");
    aapThis = aap;
    while (aapThis) {
      if (aapThis->pssmid > 0) {
        found_score = GetScoreAndEvalue(aapThis->salp, &score, &bit_score, &evalue, &number);
        print_score_eonly(evalue,cTmp);
        fprintf(table,"<INPUT NAME=\"c\" VALUE=\"%d,%d,%d,%s\" TYPE=\"HIDDEN\">\n",
	               aapThis->pssmid,aapThis->mstart,aapThis->mstop,cTmp);
        nhits++;
      }
      aapThis = aapThis->next;
    }
      /* fprintf(table,"<INPUT TYPE=\"HIDDEN\" NAME=\"nhits\" VALUE=\"%d\">\n",nhits); */
    fprintf(table,"<INPUT TYPE=\"SUBMIT\" VALUE=\"Show\">&nbsp;Domain Relatives\n");
    fprintf(table,"</FORM></TD>\n");

    if (bIsPrecalc) {
      fprintf(table,"<TD NOWRAP CLASS=\"medium1\">\n");
      fprintf(table,"<FORM ACTION=\"/entrez/query.fcgi\" METHOD=\"POST\">\n");
      fprintf(table,"<INPUT TYPE=\"HIDDEN\" NAME=\"db\" VALUE=\"cdd\">\n");
      fprintf(table,"<INPUT TYPE=\"HIDDEN\" NAME=\"term\" VALUE=\"");
      aapThis = aap;
      while (aapThis) {
        if (aapThis->pssmid > 0) {
          fprintf(table,"%d,",aapThis->pssmid);
        }
        aapThis = aapThis->next;
      }
      fprintf(table,"\">\n");
      fprintf(table,"<INPUT TYPE=\"SUBMIT\" VALUE=\"Show\">&nbsp;Domains in Entrez\n");
      fprintf(table,"</FORM></TD>\n");
    }
    
    if (!bIsPrecalc) {
      fprintf(table,"</TR></TABLE></CENTER>\n");
      if (bAnyPdb) {
        fprintf(table, "&nbsp;<A HREF=\"cdd_help.shtml#RPSBPinkdot\"><IMG SRC=\"%spinkb.gif\" BORDER=0></A>&nbsp;&nbsp; .. This CD alignment includes 3D structure. To display structure, download \n",URLcgi);
        fprintf(table, "<STRONG><A HREF=\"/Structure/CN3D/cn3d.shtml\">Cn3D</A></STRONG>!\n");
      }
      WRPSBPrintDefLinesFromSeqAlign(aap,table,bAnyPdb,query_bsp,URLcgi,querygi);
      if (ISA_na(query_bsp->mol)) is_na = TRUE;
      WRPSBCl3DisplayBlastPairList(aap,mask,table,is_na,print_options,ColorSchema,query_bsp,dbversion,URLcgi);
      free_buff();
      fprintf(table,"</PRE>\n");
      fprintf(table,"</FORM>\n");
    } else {
      fprintf(table,"<TD NOWRAP CLASS=\"medium1\" COLSPAN=\"2\" ALIGN=\"CENTER\">\n");
      fprintf(table,"<FORM ACTION=\"%swrpsb.cgi\" METHOD=\"POST\">\n",URLcgi);
      fprintf(table,"<INPUT TYPE=\"HIDDEN\" NAME=\"SEQUENCE\" VALUE=\"%d\">\n",querygi);
      fprintf(table,"<INPUT TYPE=\"HIDDEN\" NAME=\"INPUT_TYPE\" VALUE=\"precalc\">\n");
      fprintf(table,"<INPUT TYPE=\"HIDDEN\" NAME=\"FULL\">\n");
      fprintf(table,"<INPUT TYPE=\"SUBMIT\" VALUE=\"Show\">&nbsp;Details\n");
      fprintf(table,"</FORM></TD></TR>\n");
      fprintf(table,"</TABLE></CENTER><HR>\n");
    }
    
  }
  fflush(table);
  if (table != stdout) {
    fclose(table);
    PrintFile(tableName);
    RemoveTempFiles();
  }

  if (!bAnnotOnly) {
    printf("<b><a href=\"http://www.ncbi.nlm.nih.gov:80/entrez/query.fcgi?cmd=Retrieve&db=PubMed&list_uids=12520028&dopt=Abstract\">Citing CD-Search</a>:</b>\n");
    printf(" Marchler-Bauer A, Anderson JB, DeWeese-Scott C, Fedorova ND, Geer LY, He S, Hurwitz DI, Jackson JD, Jacobs AR,\n");
    printf(" Lanczycki CJ, Liebert CA, Liu C, Madej T, Marchler GH, Mazumder R, Nikolskaya AN, Panchenko AR, Rao BS, Shoemaker BA,\n");
    printf(" Simonyan V, Song JS, Thiessen RA, Vasudevan S, Wang Y, Yamashita RA, Yin JJ, and Bryant SH (2003), \n");
    printf("\"<i>CDD: a curated Entrez database of conserved domain alignments</i>\",\n");
    printf(" <b>Nucleic Acids Res. 31</b>:383-387.\n");
  }
  WRPSBSearchFoot(bAnnotOnly,bNoWrap);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* output a page indicating WAIT status                                      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void QRPSBWait(CharPtr rid, Int4 iGraphMode, Int4 iPairMode, Int4 HowLong, Nlm_FloatHi expect, Int4 nhits)
{
  CharPtr  cTitle;
  
  cTitle = MemNew(sizeof(char) * 50);
  sprintf(cTitle,"CD-Search request %s",rid);
  WRPSBSearchHead(cTitle,"Pending Conserved Domain Search Request",FALSE, FALSE);
  printf("<BR>\n");
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

  WRPSBSearchFoot(FALSE,FALSE);
  fflush(stdout);
  MemFree(cTitle);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* process the WWW arguments                                                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static AlignmentAbstractPtr WRPSBWWWargs(WWWInfoPtr www_info, Boolean *bIsQueued,
                                         Int4 *qlength, Boolean *bMode,
					 Int4 *iGraphMode, Int4 *GraphWidth,
			                 Int4 *iPairMode, ValNodePtr *vnout,
					 Boolean *bIsPic, Boolean *bIsPrecalc,
					 Boolean *bQueryIsFasta, CharPtr *sequence,
					 CharPtr *rid, Int4 *iHowLong,
					 Boolean *bFullResults,
					 Boolean *bAnnotOnly,
					 Boolean *bNoWrap,
					 Boolean *bSeqAlign)
{
  CharPtr              www_arg;
  Int4                 indx, i, j;
  Int4Ptr              indents;
  Int4                 nhits, from, to, row, colcyc, nmiss, cmiss, nindent;
  CharPtr              name;
  AlignmentAbstractPtr aapThis, aapTail = NULL, aapHead = NULL;
  SeqIntPtr            sintp;
  SeqLocPtr            slp;
  ValNodePtr           vnp = NULL, vnpThis;

  if (WWWGetMethod(www_info) == COMMAND_LINE) {
    if (GetArgc() <2) *bMode = FALSE; else *bMode = TRUE; 
  } else if (WWWGetMethod(www_info) == WWW_GET) {
    if (WWWGetNumEntries(www_info)<1) {
      if (!WRPSBDrawSearchPage()) WRPSBHtmlError("Could not draw initial page...", FALSE);
    } else *bMode = TRUE;
  } else if (WWWGetMethod(www_info) == WWW_POST) {
    *bMode = TRUE;
  } else *bMode = FALSE;

/*---------------------------------------------------------------------------*/
/* see if wrpsb.cgi should return a SeqAnnot instead of HTML-formatted data  */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"AONLY")) >= 0) {
    *bAnnotOnly = TRUE;
  }
  if ((indx = WWWFindName(www_info,"LONLY")) >= 0) {
    *bSeqAlign = TRUE;
    *bAnnotOnly = TRUE;
  }
  if ((indx = WWWFindName(www_info,"NOHTML")) >= 0) {
    *bNoWrap = TRUE;
  }
/*---------------------------------------------------------------------------*/
/* parse data refering to queued RPS-Blast searches                          */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"QUEUE")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    if (Nlm_StrCmp(www_arg,"T") == 0) {
      *bIsQueued = TRUE;
    }
  }

/*---------------------------------------------------------------------------*/
/* parse input designating wrpsb.cgi as a graphics formatter                 */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"PIC")) >= 0) {
    *bIsPic = TRUE;
    www_arg = WWWGetValueByIndex(www_info, indx);
    *GraphWidth = (Int4) atoi(strtok(www_arg,","));
    if (*GraphWidth <= 0 || *GraphWidth > 10000) WRPSBHtmlError("Error in image formatting - invalid Graphics Width!", FALSE);  
    *qlength = (Int4) atoi(strtok(NULL,","));
    if (*qlength <= 0) WRPSBHtmlError("Error in image formatting - invalid query length!", FALSE);  
    nhits = (Int4) atoi(strtok(NULL,","));
    if (nhits < 0) WRPSBHtmlError("Error in image formatting - invalid number of hits", FALSE);
    for (i=0;i<nhits;i++) {
      aapThis = MemNew(sizeof(AlignmentAbstract));
      from = (Int4) atoi(strtok(NULL,","));
      if (from < 0) WRPSBHtmlError("Error in image formatting - invalid interval", FALSE);
      aapThis->gstart = from;
      to = (Int4) atoi(strtok(NULL,",")) + from;
      if (to < 0 || to < from) WRPSBHtmlError("Error in image formatting - invalid interval", FALSE);
      aapThis->gstop = to;
      row = (Int4) atoi(strtok(NULL,","));
      aapThis->row = row;
      if (row < 0) WRPSBHtmlError("Error in image formatting - invalid row", FALSE);
      name = StringSave(strtok(NULL,","));
      aapThis->cGraphId = name;
      colcyc = (Int4) atoi(strtok(NULL,","));
      if (colcyc < 0) WRPSBHtmlError("Error in image formatting - invalid color", FALSE);
      aapThis->colcyc = colcyc;
      if (colcyc == 255) {
        aapThis->bIsArch = TRUE;
        aapThis->red   = 200;
        aapThis->green = 200;
        aapThis->blue  = 200;
      } else {
        aapThis->red   = iDartCol[aapThis->colcyc][0];
        aapThis->green = iDartCol[aapThis->colcyc][1];
        aapThis->blue  = iDartCol[aapThis->colcyc][2];
        aapThis->bIsArch = FALSE;
      }
      nmiss = (Int4) atoi(strtok(NULL,","));
      if (nmiss < 0 || nmiss >= 100) WRPSBHtmlError("Error in image formatting - invalid truncation", FALSE);
      aapThis->nmissg = (Nlm_FloatHi) nmiss / 100.0;
      cmiss = (Int4) atoi(strtok(NULL,","));
      if (cmiss < 0 || cmiss >= 100) WRPSBHtmlError("Error in image formatting - invalid truncation", FALSE);
      aapThis->cmissg = (Nlm_FloatHi) cmiss / 100.0;
      nindent = (Int4) atoi(strtok(NULL,","));
      if (nindent < 0) WRPSBHtmlError("Error in image formatting - invalid indentation", FALSE);
      aapThis->nindents = nindent;
      indents = (Int4Ptr) MemNew(nindent * sizeof(Int4));
      for (j=0;j<nindent;j++) {
        indents[j] = (Int4) atoi(strtok(NULL,","));
      }
      aapThis->indents = indents;
      aapThis->bDrawThisOne = TRUE;
      if (aapTail) { 
        aapTail->next = aapThis;
	aapTail = aapThis;
      } else {
        aapHead = aapThis;
	aapTail = aapThis;
      }
    }

    nhits = (Int4) atoi(strtok(NULL,","));
    if (nhits < 0) WRPSBHtmlError("Error in image formatting - invalid number of biased regions", FALSE);
    for (i=0;i<nhits;i++) {
      sintp = SeqIntNew();
      sintp->from = (Int4) atoi(strtok(NULL,","));
      sintp->to   = (Int4) atoi(strtok(NULL,",")) + sintp->from;
      slp = ValNodeNew(NULL);
      slp->choice = 4;
      slp->data.ptrvalue = sintp;
      vnpThis = ValNodeNew(NULL);
      vnpThis->data.ptrvalue = slp;
      ValNodeLink(&(vnp),vnpThis);
    }
    *vnout = vnp;
  }

/*---------------------------------------------------------------------------*/
/* parse data refering to graphics drawing options - need to know qlength to */
/* rescale graph width if requested to be different from default             */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"GRAPH")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    *iGraphMode = (Int4) atoi(www_arg);
    if (*iGraphMode > 2) *iGraphMode = 2;
    if (*iGraphMode < 0) *iGraphMode = 0;
  }
  if ((indx = WWWFindName(www_info,"GW")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    *GraphWidth = (Int4) atoi(www_arg);
    if (*GraphWidth == -1) *GraphWidth = WRPSB_GRAPH_WIDTH;
    else {
      if (*GraphWidth > 50) *GraphWidth = 50;
      if (*GraphWidth ==0 || *GraphWidth < -5) *GraphWidth = 1;
    }
  }

/*---------------------------------------------------------------------------*/
/* options for output formatting                                             */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"PAIR")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    *iPairMode = (Int4) atoi(www_arg);
    if (*iPairMode > 2) *iPairMode = 2;
    if (*iPairMode < 0) *iPairMode = 0;
  }
  if (*bAnnotOnly && !(*bSeqAlign)) {
    myargs[13].intvalue = (Int4) 1000;
    myargs[14].intvalue = (Int4) 1000;
  } else if ((indx = WWWFindName(www_info,"NHITS")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    myargs[13].intvalue = (Int4) atoi(www_arg);
    myargs[14].intvalue = (Int4) atoi(www_arg);
  }


/*---------------------------------------------------------------------------*/
/* CD-search parameters like E-value cutoff                                  */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"EXPECT")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    myargs[3].floatvalue = (FloatHi) atof(www_arg);
  }
  if ((indx = WWWFindName(www_info,"FILTER")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    myargs[6].strvalue = StringSave(www_arg);
  }

/*---------------------------------------------------------------------------*/
/* parse the waiting time span in case we're dealing with queue retrieval    */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"WAIT")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
     *iHowLong = (Int4) atoi(www_arg);
     if (*iHowLong < 5) *iHowLong = 5;
  }
/*---------------------------------------------------------------------------*/
/* parse the request-id (rid) in case this is a queued blast request         */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"RID")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    *rid = www_arg;
    if ((indx = WWWFindName(www_info,"JUSTPIC")) >= 0) {
      *iGraphMode = 3;
    }
  }

/*---------------------------------------------------------------------------*/
/* read in the input_type info                                               */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"INPUT_TYPE")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    if (Nlm_StrCmp(www_arg,"access") == 0 || Nlm_StrCmp(www_arg,"precalc") == 0) {
      *bQueryIsFasta = FALSE;
      myargs[21].intvalue = 1;
      if (Nlm_StrCmp(www_arg,"precalc") == 0) *bIsPrecalc = TRUE;
    } else {
      *bQueryIsFasta = TRUE;
      if (!*bAnnotOnly) myargs[21].intvalue = 0;
    }
    if (*bIsPrecalc) {
      if ((indx = WWWFindName(www_info,"FULL")) >= 0) {
        *bFullResults = TRUE;
      }
    }
  }
  
/*---------------------------------------------------------------------------*/
/* read in query sequence information                                        */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"SEQUENCE")) >= 0) {
    *sequence = WWWGetValueByIndex(www_info,indx);
    if ((*sequence == NULL || *sequence[0]==NULLB) && *rid == NULL) {
      WRPSBHtmlError("Query SEQUENCE missing from input!", *bAnnotOnly);
    }
  }
  return(aapHead);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return a gi for a particular accession                                    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Int4 WRPSBGiForAcc(CharPtr accession)
{
  Entrez2RequestPtr       e2rq;
  Entrez2ReplyPtr         e2ry;
  Entrez2IdListPtr        e2id;
  Entrez2BooleanReplyPtr  e2br;
  Int4                    gi;
  CharPtr                 thisacc;

  thisacc = MemNew(sizeof(Char) * (Nlm_StrLen(accession) + 7));
  Nlm_StrCpy(thisacc,accession);
  Nlm_StrCat(thisacc,"[ACCN]");
  e2rq = EntrezCreateBooleanRequest(TRUE, FALSE, "Protein", thisacc,
				    0, 0, NULL, 1, 0);
  MemFree(thisacc);
  if (!e2rq) return 0;
  e2ry = EntrezSynchronousQuery (e2rq);
  e2rq = Entrez2RequestFree (e2rq);
  if (!e2ry) return 0;
  e2br = EntrezExtractBooleanReply (e2ry);
  if (!e2br ) return 0;
  if (e2br->count > 0) {
    e2id = e2br->uids;
    if (e2id && e2id->num > 0 && e2id->uids ) {
      BSSeek (e2id->uids, 0, SEEK_SET);
      gi = (Int4) Nlm_BSGetUint4 (e2id->uids);
    }
  }
  else gi = 0;
  Entrez2BooleanReplyFree (e2br);
  return gi;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return a bioseq using the SEQUENCE field supplied to the cgi-bin.         */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static BioseqPtr WRPSBGetSequence(CharPtr pcsq, Int4 *gi, Boolean bBelieveQuery,
                                  Boolean bAnnotOnly)
{
  BioseqPtr    bsp;
  SeqEntryPtr  sep;
  SeqIdPtr     sip      = NULL;
  CharPtr      chptr, outptr;
  CharPtr      sequence, accession, tempstr;
  CharPtr      errmsg;
  Int4Ptr      giptr;
  Int4         numgi    = 0;
  Boolean      bIsFasta = FALSE;
  PDBSeqIdPtr  pdbsip;
  TextSeqIdPtr tsip;


/*---------------------------------------------------------------------------*/
/* Old code - retrieve sequence via Entrez, use EntrezInit call in Main      */
/*if (!pcsq) WRPSBHtmlError("No query sequence - nothing to report!");
  sequence = StringSave(pcsq);
  chptr = sequence;
  while (IS_WHITESP(*chptr) && *chptr != NULLB) chptr++;
  if (chptr[0] == '>') {
    bIsFasta = TRUE;
  } else {
    while (IS_ALPHANUM(*chptr) || *chptr == '_' || *chptr == '.') chptr++;
    *chptr = NULLB;
    accession = sequence;
    sip = NULL;
    numgi = AccessionToGi(accession,&giptr,TYP_AA);
    if (numgi > 0) {
      *gi = giptr[0];
      if (*gi > 0) ValNodeAddInt(&sip,SEQID_GI,*gi);
    } else bIsFasta = TRUE;
    if (sip) bsp = BioseqLockById(sip);
    if (!bsp) bIsFasta = TRUE;
  }
  if (bIsFasta) {
    if((sep=FastaToSeqBuffEx(pcsq, &outptr,FALSE,NULL,FALSE))==NULL)
      WRPSBHtmlError("Can not convert FASTA formatted sequence!");
    if(sep->choice != 1) WRPSBHtmlError("Conversion from FASTA failed!");
    bsp = (BioseqPtr) sep->data.ptrvalue;
  }
  if (!bsp) WRPSBHtmlError("Could not find sequence!");
  return(bsp);
}                                                                            */
/*---------------------------------------------------------------------------*/

  if (!pcsq) WRPSBHtmlError("No query sequence - nothing to report!", bAnnotOnly);
  sequence = StringSave(pcsq);
  sequence = Nlm_StrUpper(sequence);
  chptr = sequence;
  while (IS_WHITESP(*chptr) && *chptr != NULLB) chptr++;
  if (chptr[0] == '>') {
    bIsFasta = TRUE;
  } else {
    while (IS_ALPHANUM(*chptr) || *chptr == '_' || *chptr == '.') chptr++;
    *chptr = NULLB;
    accession = sequence;
    if (Nlm_StrLen(accession) >= 25) {
      bIsFasta = TRUE;
    } else {
      tempstr = Nlm_MemNew((Nlm_StrLen(accession)+1)*sizeof(Char));
      sprintf(tempstr,"%d",atoi(accession));
      if (Nlm_StrCmp(tempstr,accession) ==0) {
        numgi = 1;
        giptr = Nlm_MemNew(1*sizeof(Int4));
        giptr[0] = atoi(accession);
      } else {
        numgi = WRPSBGiForAcc(accession);
        if (numgi) {
          giptr = Nlm_MemNew(1*sizeof(Int4));
          giptr[0] = numgi;
          numgi = 1;
        }
      }
      if (numgi > 0) {
        *gi = giptr[0];
        if (*gi > 0) ValNodeAddInt(&sip,SEQID_GI,*gi);
      } else bIsFasta = TRUE;
      if (sip) bsp = CddReadDBGetBioseq(sip, -1, rdfp);
      if (sip && !bsp) bsp = BioseqLockById(sip);
      if (sip && !bsp) {
        bsp = BioseqLockById(sip);
      }
      if (!bsp) bIsFasta = TRUE;
    }
  }
  if (bIsFasta) {
    errmsg = MemNew(1024*sizeof(Char));
    if((sep=FastaToSeqBuffEx(pcsq, &outptr,FALSE,&errmsg,bBelieveQuery))==NULL)
      WRPSBHtmlError("Can not convert FASTA formatted sequence!", bAnnotOnly);
    if(sep->choice != 1) WRPSBHtmlError("Conversion from FASTA failed!", bAnnotOnly);
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sip = bsp->id;
    while (sip) {
      if (sip->choice == SEQID_GI) {
        *gi = sip->data.intvalue;
	bsp->id = sip;
	break;
      }
      sip = sip->next;
    }
/*
    if (bBelieveQuery) {
      if (*gi > 0) {
        WRPSBHtmlError("Sequence conversion successful");
      } else {
        if (errmsg) WRPSBHtmlError(errmsg);
	else WRPSBHtmlError(pcsq);
      }
    }
 */   
  }
  if (!bsp) WRPSBHtmlError("Could not find sequence!", bAnnotOnly);
  return(bsp);

/*---------------------------------------------------------------------------*/
/* Intermediate Code to try use GetGiForSeqId                                */
/*
  if (!pcsq) WRPSBHtmlError("No query sequence - nothing to report!");
  sequence = StringSave(Nlm_StrUpper(pcsq));
  chptr = sequence;
  while (IS_WHITESP(*chptr) && *chptr != NULLB) chptr++;
  if (chptr[0] == '>') {
    bIsFasta = TRUE;
  } else {
    while (IS_ALPHANUM(*chptr) || *chptr == '_' || *chptr == '.') chptr++;
    *chptr = NULLB;
    accession = sequence;
    if (Nlm_StrLen(accession) >= 25) bIsFasta = TRUE;
    else {
      tempstr = Nlm_MemNew((Nlm_StrLen(accession)+1)*sizeof(Char));
      sprintf(tempstr,"%d",atoi(accession));
      if (Nlm_StrCmp(tempstr,accession) ==0) {
        numgi = 1;
        giptr = Nlm_MemNew(1*sizeof(Int4));
        giptr[0] = atoi(accession);
        sip = NULL;
      } else {
        sip = SeqIdFromAccessionDotVersion(accession);
        numgi = GetGIForSeqId(sip);
        if (numgi > 0) {
          giptr = Nlm_MemNew(1*sizeof(Int4));
          giptr[0] = numgi;
          numgi = 1;
        }
        sip = SeqIdFree(sip);
      }
      MemFree(tempstr);
      if (!numgi) {
        if (Nlm_StrLen(accession) < 6) {
          pdbsip = PDBSeqIdNew();
	  pdbsip->mol = StringSave(accession);
	  if (Nlm_StrLen(accession) > 4) pdbsip->mol[4] = '\0';
	  if (Nlm_StrLen(accession) >= 5) pdbsip->chain = (Uint1) *(accession+4);
	  pdbsip->rel = NULL;
          sip = ValNodeNew(NULL);
	  sip->choice = SEQID_PDB;
	  sip->data.ptrvalue = pdbsip;
          numgi = GetGIForSeqId(sip);
          if (numgi > 0) {
            giptr = Nlm_MemNew(1*sizeof(Int4));
            giptr[0] = numgi;
            numgi = 1;
          }
          sip = SeqIdFree(sip);
	  pdbsip = PDBSeqIdFree(pdbsip);
        }
      }
      if (!numgi) {
        if (Nlm_StrLen(accession) < 12) {
          tsip = TextSeqIdNew();
	  if (accession[4] == '_') { 
	    tsip->name = StringSave(accession);
	  } else {
	    tsip->accession = StringSave(accession);
	  }
          sip = ValNodeNew(NULL);
	  sip->choice = SEQID_SWISSPROT;
	  sip->data.ptrvalue = tsip;
          numgi = GetGIForSeqId(sip);
          if (numgi > 0) {
            giptr = Nlm_MemNew(1*sizeof(Int4));
            giptr[0] = numgi;
            numgi = 1;
          }
          sip = SeqIdFree(sip);
	  tsip = TextSeqIdFree(tsip);
        }
      }
      if (!numgi) {
        if (Nlm_StrLen(accession) < 12) {
          tsip = TextSeqIdNew();
	  tsip->accession = StringSave(accession);
          sip = ValNodeNew(NULL);
	  sip->choice = SEQID_PIR;
	  sip->data.ptrvalue = tsip;
          numgi = GetGIForSeqId(sip);
          if (numgi > 0) {
            giptr = Nlm_MemNew(1*sizeof(Int4));
            giptr[0] = numgi;
            numgi = 1;
          }
          sip = SeqIdFree(sip);
	  tsip = TextSeqIdFree(tsip);
        }
      }
      if (numgi > 0) {
        *gi = giptr[0];
        if (*gi > 0) ValNodeAddInt(&sip,SEQID_GI,*gi);
      } else bIsFasta = TRUE;
      if (sip) bsp = BioseqLockById(sip);
      if (!bsp) bIsFasta = TRUE;
    }
  }
  if (bIsFasta) {
    if((sep=FastaToSeqBuffEx(pcsq, &outptr,FALSE,NULL,FALSE))==NULL)
      WRPSBHtmlError("Can not convert FASTA formatted sequence!");
    if(sep->choice != 1) WRPSBHtmlError("Conversion from FASTA failed!");
    bsp = (BioseqPtr) sep->data.ptrvalue;
  }
  if (!bsp) WRPSBHtmlError("Could not find sequence!");
  return(bsp);
*/

}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* "main" function to call blast for the client.                             */
/*                                                                           */
/*  This function checks the command-line arguments, opens the               */
/*  connection to the server, processes all the entries in                   */
/*  the FASTA file (obtained using FastaToSeqEntry), and                     */
/*  closes the connection.                                                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int2 Main (void)
 
{
  AlignmentAbstractPtr aap, aapThis;
  AsnIoPtr             aip;
  BioseqPtr            query_bsp, bsp;
  BlastErrorMsgPtr     pBEM;
  BLAST_OptionsBlkPtr  options;
  BLAST_KarlinBlkPtr   ka_params=NULL, ka_params_gap=NULL;
  BlastResponsePtr     response;
  BlastNet3Hptr        bl3hp                  = NULL;
  BlastVersionPtr      blast_version;
  CharPtr              blast_database, blast_inputfile, blast_outputfile;
  CharPtr              ret_buffer=NULL, params_buffer = NULL, chptr;
  CharPtr              date, motd, version, sequence;
  CharPtr              dPtr, accession        = NULL;
  CharPtr              www_arg, cPtr, outptr  = NULL;
  CharPtr              cPrecalcName, rid      = NULL;
  CharPtr              cSapDBTitle            = NULL;
  Dart_Connect        *Connection;
  DenseSegPtr          dsp;
  Int4Ptr              giptr                  = NULL;
  SeqAnnotPtr          sap                    = NULL;
  SeqAlignPtr          salp                   = NULL;
  SeqLocPtr            slp;
  SeqEntryPtr          sep                    = NULL;
  SeqIdPtr             sip, seqid_list        = NULL;
  ValNodePtr           error_returns          = NULL;
  ValNodePtr           other_returns, mask_loc, vnp;
  WWWInfoPtr           www_info;
  struct rlimit        rl;
  Boolean              bMode                  = FALSE;
  Boolean              bQueryIsFasta, status, is_network;
  Boolean              db_is_na, query_is_na, show_gi;
  Boolean              believe_query          = FALSE;
  Boolean              html                   = TRUE;
  Boolean              bIsPrecalc             = FALSE;
  Boolean              bIsQueued              = FALSE;
  Boolean              bIsPic                 = FALSE;
  Boolean              bFullResults           = FALSE;
  Boolean              bEntrez                = FALSE;
  Boolean              bAnnotOnly             = FALSE;
  Boolean              bSeqAlign              = FALSE;
  Boolean              bNoWrap                = FALSE;
  CharPtr              blast_program;
  Char                 dbversion[6], dbname[16], dbtemp[128];
  Char                 path[PATH_MAX];
  Uint1                align_type, align_view;
  Uint1                iRunMode               = DRAWSEARCHPAGE;
  Int2                 retval, Qstatus        = 1;
  Uint4                align_options, print_options;
  Int4                 iPassHit               = 0;
  Int4                 indx, gi = 0, numgi, mxr = 0;
  Int4                 qlength, startloc, endloc;
  Int4                 iGraphMode             = 2;
  Int4                 iHowLong               = 5;
  Int4                 GraphWidth             = WRPSB_GRAPH_WIDTH;
  Int4                 iDartRet, iPairMode    = 2;
  FILE                *infp, *outfp = stdout, *fp;

/*---------------------------------------------------------------------------*/
/* this sets up the unix time limit                                          */
/*---------------------------------------------------------------------------*/
  getrlimit(RLIMIT_CPU, &rl);
  rl.rlim_max = rl.rlim_cur = CPUTIME_MAX;
  setrlimit(RLIMIT_CPU, &rl);

/*---------------------------------------------------------------------------*/
/* Get Default Arguments (but DON'T read from command line)                  */
/*---------------------------------------------------------------------------*/
  if (!WRPSBGetArgs ("blastcl3", NUMARGS, myargs)) WRPSBHtmlError("Can't read Arguments!", FALSE);

/*---------------------------------------------------------------------------*/
/* retrieve names for directories etc.                                       */
/*---------------------------------------------------------------------------*/
  if (!CddGetParams()) WRPSBHtmlError("Couldn't read from config file...", FALSE);
  blast_program = StringSave(myargs[0].strvalue);

/*---------------------------------------------------------------------------*/
/* Begin processing www information block                                    */
/*---------------------------------------------------------------------------*/
  if (WWWGetArgs(&www_info) != WWWErrOk) {
    WRPSBHtmlError("Failed to process posting - check your get/post syntax.", FALSE);
  }

/*---------------------------------------------------------------------------*/
/* set the BLASTDB environment variable                                      */
/*---------------------------------------------------------------------------*/
  if (putenv("BLASTDB=/blast/db/blast")) {
    WRPSBHtmlError("Error setting environment variable BLASTDB", FALSE);
  }

/*---------------------------------------------------------------------------*/
/* read in the WWW information block                                         */
/*---------------------------------------------------------------------------*/
  aap = WRPSBWWWargs(www_info, &bIsQueued, &qlength, &bMode, &iGraphMode,
                     &GraphWidth,&iPairMode, &mask_loc, &bIsPic,
		     &bIsPrecalc, &bQueryIsFasta, &sequence, &rid, &iHowLong,
		     &bFullResults, &bAnnotOnly, &bNoWrap, &bSeqAlign);
  if (rid) {
   if (Nlm_StrLen(rid) < 10 || rid[0]==' ') rid = NULL;
  }

/*---------------------------------------------------------------------------*/
/* If wrpsb has been instructed to draw a picture only, call PrintGraphics   */
/* and exit. The PNG will be written to stdout                               */
/*---------------------------------------------------------------------------*/
  if (bIsPic) {
    aapThis = aap; while (aapThis) {
      if (aapThis->row > mxr) mxr = aapThis->row;
      aapThis = aapThis->next;
    }
    query_bsp = BioseqNew();
    query_bsp->length = qlength;
    WRPSBCl3PrintGraphics(aap,stdout,mxr,query_bsp,mask_loc,iGraphMode,
                          GraphWidth,TRUE,bIsPrecalc, gi);
  }

/*---------------------------------------------------------------------------*/
/* if called without arguments, draw the initial WRPSB page                  */
/*---------------------------------------------------------------------------*/
  if (!bMode) if (!WRPSBDrawSearchPage()) WRPSBHtmlError("Could not draw initial page...", bAnnotOnly);

/*---------------------------------------------------------------------------*/
/* start initializing stuff required for database access                     */
/*---------------------------------------------------------------------------*/
  UseLocalAsnloadDataAndErrMsg ();
  ErrSetOptFlags(EO_LOGTO_USRFILE);
  ErrSetFatalLevel (SEV_MAX); /* never die from ErrPostEx */
  if (! SeqEntryLoad()) return 1;
    
/*---------------------------------------------------------------------------*/
/* Initialize Entrez, PubSeq, and  blast sequence database access            */
/*---------------------------------------------------------------------------*/
  if (!rid) {
    EntrezSetService ("Entrez2");
    if (!PubSeqFetchEnable()) WRPSBHtmlError("Cannot initialize PubSeqFetch!", bAnnotOnly);
    if(!(rdfp = readdb_new_ex("nr", READDB_DB_IS_PROT, FALSE)))
     WRPSBHtmlError("Readdb init failed", bAnnotOnly);
  }
    
/*---------------------------------------------------------------------------*/
/* Have to deal with the environment CHANGE here from development server to  */
/* using the production servers! (cdsearch_test vs. rpsblast) - this will    */
/* not affect the queued version of RPS-Blast                                */
/*---------------------------------------------------------------------------*/
  if (getenv("NI_SERVICE_NAME_NETBLAST")==NULL) {
/* #ifdef CDSEARCH_TEST
    if (putenv("NI_SERVICE_NAME_NETBLAST=cdsearch_test")) {
      WRPSBHtmlError("Error setting environment");
    }
#else */
    if (putenv("NI_SERVICE_NAME_NETBLAST=rpsblast")) {
      WRPSBHtmlError("Error setting environment", bAnnotOnly);
    }
/* #endif */
  }

/*---------------------------------------------------------------------------*/
/* For RPS Blast - anything not "blastp" - is "tblastn"                      */
/*---------------------------------------------------------------------------*/
  if(StringICmp(blast_program, "blastp")) {
      StringCpy(blast_program, "blastx");
  }

/*---------------------------------------------------------------------------*/
/* anything beyond this point assumes that a search is launched and results  */
/* are being formatted                                                       */
/*---------------------------------------------------------------------------*/
  blast_inputfile  = myargs [2].strvalue;
  blast_outputfile = myargs [5].strvalue;
  if (myargs[26].intvalue) html = TRUE;
  align_view = (Int1) myargs[4].intvalue;
  align_type = BlastGetTypes(blast_program, &query_is_na, &db_is_na);
  if(align_type == blast_type_undefined) WRPSBHtmlError("Blast Type not defined!", bAnnotOnly);
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
  if (options == NULL) WRPSBHtmlError("Could not initialize Blast Options!", bAnnotOnly);
/*---------------------------------------------------------------------------*/
/* If option RPS Blast set - option "program" is ignored by the engine       */
/*---------------------------------------------------------------------------*/
  options->is_rps_blast = TRUE;

/*---------------------------------------------------------------------------*/
/* deal with sequence information - unless the query is retrieved from queue */
/*---------------------------------------------------------------------------*/
  believe_query = FALSE;
  if (myargs[21].intvalue != 0 || bAnnotOnly) believe_query = TRUE;
  if (!rid) {
    gi = 0;
    bsp = WRPSBGetSequence(sequence, &gi, believe_query, bAnnotOnly);
    if (gi || bAnnotOnly) {
      myargs[21].intvalue = 1;
    } else {
      myargs[21].intvalue = 0;
    }
    query_bsp = bsp;
  }

/*---------------------------------------------------------------------------*/
/* Section to get data needed for displaying pre-calculated RPS-Blast alignmt*/
/*---------------------------------------------------------------------------*/
  if (bIsPrecalc) {
    Connection = WRPSBConnectDart();
    Dart_Gi2Seq(Connection,(unsigned)gi,NULL,NULL,NULL,0,NULL,0,NULL,0,
                NULL,0,NULL,&sap,NULL);
    if (!sap) WRPSBHtmlError("Could not read precalculated alignment!", bAnnotOnly);
    myargs[13].intvalue = myargs[14].intvalue = 250;       /* number of hits */
  }

  if ((indx = WWWFindName(www_info,"VERSION")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    Nlm_StrCpy(dbversion,www_arg);
  } else {
    Nlm_StrCpy(dbversion,CDDefault);
    if (rid) {
      Nlm_StrCpy(dbversion, QUDefault);
    }
  }

  if ((indx = WWWFindName(www_info,"DATALIB")) < 0) {
    if (Nlm_StrCmp(CDDlocat,"inhouse")==0) {
      myargs[1].strvalue = StringSave("cdd_loc");
    } else {
      myargs[1].strvalue = StringSave("cdd");
    }
  } else {
    www_arg = WWWGetValueByIndex(www_info, indx);
    myargs[1].strvalue = StringSave(www_arg);
  }
  Nlm_StrCpy(dbname, myargs[1].strvalue);
#ifdef CDSEARCH_TEST
  Nlm_StrCpy(dbtemp,"cdsearch_test/");
  Nlm_StrCat(dbtemp,myargs[1].strvalue);
  myargs[1].strvalue = dbtemp;
#endif
  blast_database = myargs[1].strvalue;
  
  options->expect_value  = (Nlm_FloatHi) myargs [3].floatvalue;
  BLASTOptionSetGapParams(options, myargs[25].strvalue, 0, 0);
  if (StringICmp(myargs[6].strvalue, "T") == 0) {
    if (StringICmp("blastn", blast_program) == 0)
      options->filter_string = StringSave("D");
    else
      options->filter_string = StringSave("S");
  } else {
    options->filter_string = StringSave(myargs[6].strvalue);
  }
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
  options->hitlist_size = MAX(myargs[13].intvalue, myargs[14].intvalue);

  if ((indx = WWWFindName(www_info,"IS_PROT")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    if ((Int4) atoi(www_arg) == 0) query_is_na = TRUE;
    else query_is_na = FALSE;
  }

  if (!bsp) WRPSBHtmlError("Query SEQUENCE missing from input!", bAnnotOnly);
  if (ISA_na(bsp->mol)) query_is_na = TRUE;
  if (query_is_na) {
    WRPSBHtmlError("Can not process nucleotide sequences!", bAnnotOnly);
  }
  qlength = query_bsp->length;
  
  if (GraphWidth != WRPSB_GRAPH_WIDTH) {
    if (GraphWidth > 0) GraphWidth = qlength / GraphWidth;
    else GraphWidth = qlength * -1 * GraphWidth;
    if (GraphWidth < 10) GraphWidth = 10;
    if (GraphWidth > 10000) GraphWidth = 10000;
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
    
/*---------------------------------------------------------------------------*/
/* need to initialize BLAST service if not a request for the BLAST queue     */
/*---------------------------------------------------------------------------*/
  if (!rid && !bIsQueued /* && !bIsPrecalc */) {
    if (! BlastInit("blastcl3", &bl3hp, &response)) {
      ErrPostEx(SEV_FATAL, 0, 0, "Unable to initialize BLAST service");
      WRPSBHtmlError("Unable to initialize BLAST service!", bAnnotOnly);
      return (1);
    }
    
    if (response && response->choice == BlastResponse_init) {
      blast_version = response->data.ptrvalue;
      version = blast_version->version;
      date = blast_version->date;
    } else {
#ifdef CDSEARCH_TEST
      if (putenv("NI_SERVICE_NAME_NETBLAST=dart_test")) {
        WRPSBHtmlError("Error setting environment", bAnnotOnly);
      }
      if (! BlastInit("blastcl3", &bl3hp, &response)) {
        ErrPostEx(SEV_FATAL, 0, 0, "Unable to initialize BLAST service");
        WRPSBHtmlError("Unable to initialize BLAST service!", bAnnotOnly);
        return (1);
      }
      if (response && response->choice == BlastResponse_init) {
        blast_version = response->data.ptrvalue;
        version = blast_version->version;
        date = blast_version->date;
      } else {
        ErrPostEx(SEV_FATAL, 0, 0, "Unable to connect to service");
        WRPSBHtmlError("Unable to connect to service!", bAnnotOnly);
        return (1);
      }
#else
      ErrPostEx(SEV_FATAL, 0, 0, "Unable to connect to service");
      WRPSBHtmlError("Unable to connect to service!", bAnnotOnly);
      return (1);
#endif
    }
    if (!BlastNetBioseqFetchEnable(bl3hp, blast_database, db_is_na, TRUE)) 
      WRPSBHtmlError("Unable to connect to Blast Database!", bAnnotOnly);
  }

  retval=0;
        
/*---------------------------------------------------------------------------*/
/* Read boundaries of location                                               */
/*---------------------------------------------------------------------------*/
  startloc = myargs[23].intvalue - 1;
  if (myargs[24].intvalue == -1) endloc = query_bsp->length - 1;
  else endloc = myargs[24].intvalue - 1;
        
/*---------------------------------------------------------------------------*/
/* Create the SeqLoc                                                         */
/*---------------------------------------------------------------------------*/
  if (!rid) {
    slp = SeqLocIntNew(startloc, endloc, Seq_strand_both, query_bsp->id);
        
    if (query_bsp == NULL) {
      ErrPostEx(SEV_FATAL, 0, 0, "Unable to obtain bioseq\n");
      retval = 2;
    }
    
  }    
  if (!bIsPrecalc && !bIsQueued && !rid) {
    if (startloc || endloc != query_bsp->length - 1)
      salp = BlastSeqLocNetCore(bl3hp, slp, blast_program, blast_database, options,
                                &other_returns, &error_returns, NULL, NULL, &status);
    else 
      salp = BlastBioseqNetCore(bl3hp, query_bsp, blast_program, blast_database, options,
                                &other_returns, &error_returns, NULL, NULL, &status);
  }
  else {
    if (sap) salp = sap->data;
  }
  if (rid) {
    sap            = NULL; query_bsp     = NULL; blast_program = NULL;
    blast_database = NULL; other_returns = NULL; error_returns = NULL;
    Qstatus = (Int2) QBlastGetResults(rid,&sap,&query_bsp,&blast_program,&blast_database,&other_returns, &error_returns);
    if (Qstatus < 0) {
      WRPSBHtmlError("Error retrieving CD-Search request from BLAST Queue!", bAnnotOnly);
      return(1);
    }
    if (Qstatus > 0) {
      if (iGraphMode == 3) {
        QRPSBWaitIcon();
      }
      QRPSBWait(rid,iGraphMode,iPairMode,iHowLong,options->expect_value, options->hitlist_size);
      exit(0);
    }
    ReadDBBioseqFetchEnable ("wrpsb", blast_database, db_is_na, TRUE);
    if (!sap) sap = (SeqAnnotPtr) BLASTGetSeqAnnotByRID(rid);
    if (!sap)  WRPSBHtmlError("BLAST queue did not return alignment", bAnnotOnly);
    if (!query_bsp) {
      salp = sap->data;
      dsp = salp->segs;
      sip = dsp->ids;
      query_bsp = BioseqLockById(sip);
    }
    if (!query_bsp)  WRPSBHtmlError("BLAST queue did not return query sequence", bAnnotOnly);
    else {
      sip = query_bsp->id;
      if (sip->choice != SEQID_LOCAL) believe_query = TRUE;
      else believe_query = FALSE;
    }
    salp = sap->data;
    date = BlastGetReleaseDate();
    version = BlastGetVersionNumber();
    if (!salp) {
      if (error_returns) {
        WRPSBHtmlError("No sequence alignment retrieved from BLAST queue!", bAnnotOnly);
      }
    } 
  }

  if (!salp && !bIsQueued) {
    if (error_returns) {
      while (error_returns) {
        pBEM = error_returns->data.ptrvalue;
          ErrPostEx(SEV_ERROR, 0, 0, pBEM->msg);
        error_returns = error_returns->next;
      }
      WRPSBHtmlError(pBEM->msg, bAnnotOnly);
    }
  } 

  if (!bIsQueued) {
    mask_loc = NULL;
    if (StringICmp(myargs[6].strvalue, "F") || rid) {
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
    }
  }

/*---------------------------------------------------------------------------*/
/* try to get database information from the SeqAnnots - if queued or precalc */
/*---------------------------------------------------------------------------*/
  if (bIsPrecalc || rid) {
    iDartRet = 0;
    if (bIsPrecalc) {
      cSapDBTitle = MemNew(128*sizeof(Char));
      dPtr = MemNew(128*sizeof(Char));
      iDartRet = Dart_Version(Connection,dPtr,128,cSapDBTitle,128); 
      if (iDartRet) {
        cPtr = Nlm_StrStr(cSapDBTitle,".v");
        if (cPtr) {
          cPtr++; Nlm_StrCpy(dbversion,cPtr);
        } else StrCpy(dbversion, CDDefault);
        cPtr = Nlm_StrStr(cSapDBTitle,".v");
        *cPtr = '\0';
        Nlm_StrCpy(dbname,cSapDBTitle);
      }
      MemFree(dPtr);
      MemFree(cSapDBTitle);
    }
    if (!iDartRet) {
      cSapDBTitle = BLASTGetDatabaseTitleFromSeqAnnot(sap);
      if (cSapDBTitle) {
        cPtr = Nlm_StrStr(cSapDBTitle,".v");
        if (cPtr) {
          cPtr++; Nlm_StrCpy(dbversion,cPtr);
        } else StrCpy(dbversion, CDDefault);
        cPtr = Nlm_StrStr(cSapDBTitle,".v");
        *cPtr = '\0';
        Nlm_StrCpy(dbname,cSapDBTitle);
      } else {
        if (dbname[0] == '\0') {   /* worst case - don't know a thing about db */
          Nlm_StrCpy(dbname,"cdd");
          Nlm_StrCpy(dbversion,"v1.63");
        }
      }
    }
    if (Nlm_StrCmp(dbname,"unknown")) {
      if (! BlastInit("blastcl3", &bl3hp, &response)) {
        ErrPostEx(SEV_FATAL, 0, 0, "Unable to initialize BLAST service");
        WRPSBHtmlError("Unable to initialize BLAST service!", bAnnotOnly);
        return (1);
      }
      BlastNetBioseqFetchEnable(bl3hp, dbname, db_is_na, TRUE);
    }
  }

  if (!bIsPrecalc && !bIsQueued) Connection = WRPSBConnectDart();

  if (bIsQueued) {
    rid = QBlastSubmitJob(query_bsp,blast_program,blast_database,options);
    if (!rid) WRPSBHtmlError("Error submitting CD-Search job to BLAST queue!", bAnnotOnly);
    QRPSBWait(rid,iGraphMode,iPairMode,iHowLong,options->expect_value,options->hitlist_size);
  }
  
  if (bIsPrecalc && !bFullResults) iGraphMode = 1;
  
  WRPSBCl3ViewSeqAlign(salp, query_bsp, mask_loc, iGraphMode, iPairMode, options,
                       believe_query, print_options, version, date, bl3hp,
                       blast_database, dbversion, bIsPrecalc, GraphWidth,
		       Connection,gi,dbname,bFullResults,bAnnotOnly,bNoWrap,bSeqAlign);

  if (sep) sep = SeqEntryFree(sep);
  options = BLASTOptionDelete(options);
  if (bl3hp) BlastFini(bl3hp);

  if (Connection) WRPSBDisConnectDart(Connection);
  if (rdfp) rdfp = readdb_destruct(rdfp);
  return(retval);
}
