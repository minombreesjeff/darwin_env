/*===========================================================================
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
* File Name:  cblast.c
*
* Author:  Yanli Wang           
*
* Initial Version Creation Date: 6/19/2002 
*
* File Description:
*         WWW-Server for visualizing sequence-structure alignment 
*          
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cblast.c,v $
* Revision 1.9  2002/12/31 21:00:32  ywang
* read environment variables from config file
*
* Revision 1.8  2002/12/20 21:44:09  ywang
* update CDart_Init2
*
* Revision 1.7  2002/12/18 16:58:35  ywang
* get defline from blast DB now
*
* Revision 1.6  2002/12/18 15:29:56  ywang
* improve query id label
*
* Revision 1.5  2002/12/12 15:59:45  ywang
* wording improvement
*
*
* ==========================================================================
*/

#include <stdio.h>
#include <ncbi.h>
#include <accentr.h>
#include <lsqfetch.h>
#include <netentr.h>
#include <www.h>
#include <sys/resource.h>
#include <asn.h>
#include <accutils.h>
#include <mmdbapi.h>
#include <mmdbapi1.h>
#include <asnmime.h> 
#include <objmime.h>
#include <strimprt.h>
#include <objcdd.h>
#include "cddsrv.h"
#include <pgppop.h> 
#include <alignmgr.h>
#include <taxutil.h>
#include <txcommon.h>
#include <taxinc.h>
#include <ctlibutils.h>

#include "CAV_lib/cddalignview.h" 
#include "/netopt/structure/include/dbinfo.h"
#include "/netopt/structure/include/dart.h"
#include "/netopt/structure/include/dartutil.h"

#include <objcn3d.h>
#include <cddutil.h>
#include "cdd_seedutil.h"
#include "cblast.h"
#include "cblastutil.h"
#include "cblastgraph.h"
#include <PubVastApi.h>
#include <PubStructApi.h>
#include <txalign.h>

#define MMDBDirPath "/net/vaster/mmdb/mmdb/data/"
#define HEADFILE "sshead.txt"
/* Structure Summary Header File */

#define MaxModels 1

#define BLAST_OVERVIEW 1
#define BLAST_ONEGROUP 2
#define BLAST_ONEPAIR 3

Dart_Connect    *dartcon;
Boolean psfailed = FALSE;
WWWInfoPtr  info = NULL;
Int2 DispOptChoice = 0;

CharPtr queryrid;
extern FILE *logFilePtr;
Int2 entry_page = 3;
Char subset_name[5][20] = {"All MMDB", "Non-identical seq.", "NR,Blast_p<10e-80", "NR,Blast_p<10e-40", "NR,Blast_p<10e-7"};
Char sortpara_name[4][20] = {"Blast e_value", "Blast bit_score", "Aligned length", "Seq. Identity"};
Char dispform_name[2][20] = {"Graphics", "Table"};
/*------------------------------*/
static void WWWPrintFileData(CharPtr FName,  FILE *pFile)
{

   FILE *f = NULL;
   Char fullpath [PATH_MAX];
   CharPtr ptr;
   Char pcBuf[1024];
  
   fullpath[0] = '\0';
   StringCpy(fullpath,  DATApath); /* look in DATApath */
   StringCat(fullpath,  FName);
   f = FileOpen (fullpath, "r");
   if (f == NULL) {
       f = FileOpen (FName, "r");  /* look in curent */
       if (f == NULL)  {  /* look in ./data/ */
         ProgramPath (fullpath, sizeof (fullpath) - 1);
         ptr = StringRChr (fullpath, DIRDELIMCHR);
         if (ptr != NULL) {
      *ptr = '\0';
         }
         FileBuildPath (fullpath, "data", FName);
         f = FileOpen (fullpath, "r");
         if (f == NULL)  {
           return;
         }
       }
   }

   do {
     pcBuf[0] = '\0';
     ptr = fgets(pcBuf, (size_t)1024, f);
     if (ptr) fprintf(pFile, ptr);
   } while (ptr);

   FileClose(f);
   return;
}
/*---------------------------------------------------------------------------*/
/* read parameters from configuration file                                   */
/*---------------------------------------------------------------------------*/
static Boolean CblastSrvGetParams()
{
  URLBase[0] = URLcgi[0] = ENTREZurl[0] = DOCSUMurl[0] = MAILto[0] = '\0';

  GetAppParam("cblast", "CBLAST", "ENTREZurl", "", ENTREZurl, PATH_MAX);
  if (ENTREZurl[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no ENTREZurl...\n");
    return FALSE;
  }
  GetAppParam("cblast", "CBLAST", "DOCSUMurl", "", DOCSUMurl, PATH_MAX);
  if (DOCSUMurl[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no DOCSUMurl...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "TAXcgi", "", TAXcgi, PATH_MAX);
  if (TAXcgi[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no TAXcgi ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "MMDBcgi", "", MMDBcgi, PATH_MAX);
  if (MMDBcgi[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no MMDBcgi ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "MMDBurl", "", MMDBurl, PATH_MAX);
  if (MMDBurl[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no MMDBurl ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "DARTcgi", "", DARTcgi, PATH_MAX); 
  if (DARTcgi[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no DARTcgi ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "CGIurl", "", CGIurl, PATH_MAX);
  if (CGIurl[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no CGIurl ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "CDDhome", "", CDDhome, PATH_MAX);
  if (CDDhome[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no CDDhome ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "CDSRVurl", "", CDSRVurl, PATH_MAX);
  if (CDSRVurl[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no CDSRVurl ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "CGIName", "", CGIName, PATH_MAX);
  if (CGIName[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no CGIName ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "LOGpath", "", LOGpath, PATH_MAX);
  if (LOGpath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no LOGpath ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "NRPDB", "", NRPDB, PATH_MAX);
  if (NRPDB[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no NRPDB ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "ODBCINI", "", ODBCINI, PATH_MAX);
  if (ODBCINI[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no ODBCINI ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "DARTUSER", "", DARTUSER, PATH_MAX);
  if (DARTUSER[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no DARTUSER ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "DARTPASS", "", DARTPASS, PATH_MAX);
  if (DARTPASS[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no DARTPASS ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "DARTLIB", "", DARTLIB, PATH_MAX);
  if (DARTLIB[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no NRPDB ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "LDLIBRARYPATH", "", LDLIBRARYPATH, PATH_MAX);
  if (LDLIBRARYPATH[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no LDLIBRARYPATH ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "SYBASE", "", SYBASE, PATH_MAX);
  if (SYBASE[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no NRPDB ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "BLASTDB", "", BLASTDB, PATH_MAX);
  if (BLASTDB[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no NRPDB ...\n");
    return FALSE;
  }

  GetAppParam("cblast", "CBLAST", "NCBI", "", NCBI, PATH_MAX);
  if (NCBI[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CBLAST config file\nCBLAST section has no NRPDB ...\n");
    return FALSE;
  }

  return TRUE;
}                                                    /* end CddGetParams */
/*---------------------------*/
void CblastSrvInit()
{

  Char str[1024];

  if((! VastSrvInitialize()) || (! MmdbSrvInitialize()))
  {
    printf("Error: VastSrvInitialize() or MmdbSrvInitialize() failed.\n");
    exit(0);
  }

  sprintf(str, "ODBCINI=%s", ODBCINI); putenv(str);
  putenv("LD_LIBRARY_PATH=/opt/machine/merant/lib");      

  dartcon = Dart_Init2(DARTLIB, DARTUSER, DARTPASS);     

  StringCat(LOGpath, "cblast.log");
  if( !ErrSetLogfile (LOGpath, ELOG_APPEND) ) {
      printf("Content-type: text/html\r\n\r\n");
      printf("<H2>CBLAST Error</H2>\n - Unable to Open Log.<p>\n");
      printf("LOGpath %s<br>\n", LOGpath);
        printf("Contact CBLAST administrator for this site: %s<p>\n",  MAILto);     
    exit(1);
  }

  ErrSetLogLevel(SEV_MAX);
  ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);

  psfailed = FALSE;
  if (!MMDBInit_PS()) {
    psfailed = TRUE;
    if (!MMDBInit_FF()) {
      ErrLogPrintf("#MMDB Database Access Failed to Initialize.\n");
      printf("Content-type: text/html\r\n\r\n");
      printf("<H2>CBLAST Error</H2>\n - Unable to Open MMDB Database on Server.<p>");
      printf("Contact CBLAST administrator for this site: %s<p>\n",  MAILto);

      exit(1);
    }
  }
   
  SeqAsnLoad();
  if(!objmimeAsnLoad()) printf("objmimeAsnLoad failed!\n");
  AllObjLoad ();
  objcn3dAsnLoad();

/*
  ID1BioseqFetchEnable ("CBLAST", FALSE);
*/

/*
  if (EntrezInit("CBLAST", NULL, FALSE) == FALSE)
  {
    printf("Content-type: text/html\n\n");
    printf("<h2>Error</h2>\n");
    printf("CBLAST: EntrezInit Failed.<p>\n");
    return;
  }
*/

}
/*----------*/
static CblastDumpQuery(BioseqPtr bsp, FILE *File)
{

  printf("Content-type: text/html\n\n");
  WWWPrintFileData(HEADFILE,  File);

  fprintf(File, "\n\n");

  fprintf(File, "\n\n");
  fprintf(File, "<small>\n");
  BioseqToFastaDump(bsp, File, FALSE);
  fprintf(File, "</small>\n");

  printf ("</body>\n");
  printf ("</html>");

  return 0;
}
/*----------*/
static PrintPageView(FILE *File, WWWQueryPtr wqp, BlastPDBNbPtr bpnp)
{
    WWWQueryDataPtr wqdp = NULL;
    ValNodePtr csip = NULL;
    CblastSeqInfoDataPtr csidp = NULL;
    long mmdb_id = 0;
    PDBSeqIdPtr pdbid = NULL;
    
    csip = bpnp->csip;
    csidp = csip->data.ptrvalue;

    mmdb_id = constructMmdbIdForGi(csidp->PDBgi);
    pdbid = csidp->pdbid->data.ptrvalue;

    fprintf(File, "<TABLE width=800 BORDER=0 CELLPADDING=3 CELLSPACING=0 bgcolor=#FFFFCC>\n\n");
    fprintf(File, "<TR>\n");
    fprintf(File, "<TD>\n");
    fprintf(File, "<table>   <!-- 1st subtable, in the first layer -->\n\n");
    /* insert a blank row in the table, for spacing */
    fprintf(File, "<tr>\n<td VALIGN=TOP NOWRAP> </td>\n</tr>\n\n");

    if(bpnp->query_title){
      fprintf(File, "<tr>\n");
      fprintf(File, "<td VALIGN=TOP NOWRAP class=H4>\n");
      fprintf(File, "<strong>Query:</strong></td>\n");
      fprintf(File, "<td class=\"TEXT\">%s", bpnp->query_title);
      fprintf(File, "</td>\n</tr>\n\n");
    }

    fprintf(File, "<tr>\n");
    fprintf(File, "<td VALIGN=TOP NOWRAP class=H4>\n");
    fprintf(File, "<strong>Structure:</strong></td>\n");
    fprintf(File, "<td class=\"TEXT\">%s %s.", pdbid->mol, csidp->doc);
    fprintf(File, "</td>\n</tr>\n\n");

    fprintf(File, "<tr>\n");
    fprintf(File, "<td VALIGN=TOP NOWRAP ALIGN=LEFT class=H4>");
    fprintf(File, "MMDB: <A HREF=\"%s%d\">%s_%c</A>", MMDBcgi, mmdb_id, pdbid->mol, pdbid->chain);
    fprintf(File, "&nbsp;&nbsp;\n<font class=H4>");
    fprintf(File, "</td>\n");
    fprintf(File, "<td VALIGN=TOP NOWRAP ALIGN=LEFT class=H4>");
    fprintf(File, "Reference: <A HREF=\"%sdb=structure&cmd=Display&dopt=structure_pubmed&from_uid=%ld\">PubMed</A>\n",ENTREZurl, (long) mmdb_id );
    fprintf(File, "</td>\n</tr>\n");

  /* insert a blank row in the table, for spacing */
    fprintf(File, "\n<tr>\n");
    fprintf(File, "<td VALIGN=TOP NOWRAP><BR></td>\n");
    fprintf(File, "</tr>\n");
    fprintf(File, "</table>  <!-- end of first subtable -->\n");

    fprintf(File, "</TD>\n");
    fprintf(File, "</TR>\n");
    fprintf(File, "</TABLE>\n");


    fprintf(File, "<FORM METHOD=POST ACTION=\"%s%s\">\n", CGIurl, CGIName);

    while(wqp){
      wqdp = wqp->data.ptrvalue;
      fprintf(File, "<INPUT TYPE=HIDDEN NAME=%s VALUE=%s>\n", wqdp->name, wqdp->value);
      wqp = wqp->next;
    }

/*
    fprintf(File, "<table border=0 cellpadding=0 cellspacing=0>\n");
    fprintf(File, "<tr>\n");
    fprintf(File, "<TD class=SMALL1>\n");

    fprintf(File, "<strong><INPUT TYPE=submit NAME=viewmethod VALUE=\"View Alignment\"></strong>");
    fprintf(File, "&nbsp;\n");

    fprintf(File, "<strong>using</strong>\n");
    fprintf(File, "<SELECT NAME=viewmethod_option>\n");
    if(DispOptChoice == 1) fprintf(File, "<OPTION SELECTED VALUE=HTML>HTML\n");
    else fprintf(File, "<OPTION VALUE=HTML>HTML\n");
    if(DispOptChoice == 2) fprintf(File, "<OPTION SELECTED VALUE=Graphic>Graphic\n");
    else fprintf(File, "<OPTION VALUE=Graphic>Graphic\n");
    fprintf(File, "</SELECT>\n");
    fprintf(File, "</TD>\n");

    fprintf(File, "</TR>\n");
    fprintf(File, "</table>\n");
*/

    fprintf(File, "<table border=0 cellpadding=0 cellspacing=0>\n");
    fprintf(File, "<tr>\n");
    fprintf(File, "<TD class=SMALL1>\n");

    fprintf(File, "<strong><INPUT TYPE=submit NAME=3d_viewmethod VALUE=\"View 3D Structure\"></strong>");
    fprintf(File, "&nbsp;\n");

    fprintf(File, "<strong>with</strong>\n");
    fprintf(File, "<SELECT NAME=3d_viewmethod_option>\n");
    if(DispOptChoice == 3)fprintf(File, "<OPTION SELECTED VALUE=Cn3D>Cn3D Display\n");
    else fprintf(File, "<OPTION VALUE=Cn3D>Cn3D Display\n");
    if(DispOptChoice == 4)fprintf(File, "<OPTION SELECTED VALUE=SAVE_ASN1>Save File\n");
    else fprintf(File, "<OPTION VALUE=SAVE_ASN1>Save File\n");
    if(DispOptChoice == 5)fprintf(File, "<OPTION SELECTED VALUE=PRINT_ASN1>See File\n");
    else fprintf(File, "<OPTION VALUE=PRINT_ASN1>See File\n");
    fprintf(File, "</SELECT>\n");
    fprintf(File, "</TD>\n");

    fprintf(File, "<TD>&nbsp;</TD>\n");    
    fprintf(File, "<TD><small>(To display structure, download <A HREF = \"http://www.ncbi.nlm.nih.gov/Structure/CN3D/cn3d.shtml\"><STRONG><i>Cn3D</i></STRONG></A>)</small></TD>\n");

    fprintf(File, "</TR>\n");
    fprintf(File, "</table>\n");

    fprintf(File, "</FORM>\n");    
    
    return;
}
/*-------------*/
PrintNeighborTable(NeighborInfoPtr nip, Int4 subject_str_gi, Int2 pagenum, Int2 numpages)
{
  NeighborInfoDataPtr nidp = NULL;
  Int2 i = 0;

  printf("<strong><INPUT TYPE=SUBMIT NAME=dispsub VALUE=List></strong>\n");
  printf("<strong> Structures with identical sequence on page</strong>\n");
  printf("<select name=doclistpage>\n");
  for(i = 1; i <= numpages; i++){
    if(i == pagenum) printf("<option value=%d selected>%d\n", i, i);
    else printf("<option value=%d>%d\n", i, i);
  }
  printf("</select>\n");
 
  printf("<table cellspacing=3 cellpadding=2 width=800 border=1>\n");
  printf("<tr valign=middle>\n");
  printf("<th>&nbsp;</th>\n");    
  printf("<th align=middle>Id</th>\n");
  printf("<th align=middle>Description</th>\n");
  printf("</tr>\n");

  while(nip){
    nidp = nip->data.ptrvalue;
    if(nidp->pagenum != pagenum){
      nip = nip->next;
      continue;
    }
    printf("<tr>\n");
    if(nidp->gi==subject_str_gi) printf("<td VALIGN=TOP><INPUT TYPE=\"radio\" checked NAME=\"hit\" ");
    else printf("<td VALIGN=TOP><INPUT TYPE=\"radio\" NAME=\"hit\" ");
    printf("VALUE=\"%d\"></td>\n", nidp->gi);
    printf("<td valign=top>"); printf("<A HREF=\"%s%s\">%s_%c</A>", MMDBcgi, nidp->mol, nidp->mol, nidp->chain); printf("</td>\n");    
    printf("<td valign=top>"); printf("%s", nidp->docsum); printf("</td>\n");
    printf("</tr>\n");
    nip = nip->next;
  } 

  printf("</table>\n");
}
/*--------------------------*/
void PrintPDBNeighborTable(FILE *File, BlastPDBNbPtr bpnp, Int2 pagenum)
{
  ValNodePtr csp = NULL, csip = NULL;
  CblastSeqInfoDataPtr csidp = NULL;
  CblastScoreDataPtr csdp = NULL;
  PDBSeqIdPtr pdbid = NULL;
  char str_score[1024];
  Int4 count_onpage = 0, count = 0;

  csip = bpnp->csip;
  while(csip){
    csidp = csip->data.ptrvalue;
    if(csidp->pagenum == pagenum) count_onpage++;
    count++;
    csip = csip->next;
  }
  
  if(count_onpage < count) fprintf(File, "<strong>%d out of %d hits with known structures displayed.</strong>\n", count_onpage, count);
  else fprintf(File, "<strong>%d hits with known structures displayed.</strong>\n", count);

  fprintf(File, "<table cellspacing=3 cellpadding=2 width=800 border=1>\n");
  printf("<tr valign=middle>\n");
  printf("<th align=middle>Structure</th>\n");
  printf("<th align=middle>E_value</th>\n");
  printf("<th align=middle>Bit_score</th>\n");
  printf("<th align=middle>Alg. Len</th>\n");
  printf("<th align=middle>Seq %Id</th>\n");
  printf("<th align=middle>Description</th>\n");
  printf("</tr>\n");

  csp = bpnp->csp;
  csip = bpnp->csip;
  while(csip){

    csdp = csp->data.ptrvalue;
    csidp = csip->data.ptrvalue;
    pdbid = csidp->pdbid->data.ptrvalue;

    if(csidp->pagenum == pagenum){
      fprintf(File, "<tr>\n");
      fprintf(File, "<td valign=top>"); printf("<A HREF=\"%s%s\">%s_%c</A>", MMDBcgi, pdbid->mol, pdbid->mol, pdbid->chain); printf("</td>\n");
      print_evalue(csdp->evalue, str_score);
      fprintf(File, "<td valign=top>"); printf("%s\n", str_score); printf("</td>\n");
      print_bitscore(csdp->bit_score, str_score);
      fprintf(File, "<td valign=top>"); printf("%s\n", str_score); printf("</td>\n");
      fprintf(File, "<td valign=top>"); printf("%d\n",  csdp->length); printf("</td>\n");
      fprintf(File, "<td valign=top>"); printf("%d\n", (int) (csdp->seq_identity*100)); printf("</td>\n");
      fprintf(File, "<td valign=top>"); printf("%s\n", csidp->doc); printf("</td>\n");
      fprintf(File, "</tr>\n");
    }

    csip = csip->next;
    csp = csp->next;
  }

  fprintf(File, "</table>\n");
}
/*--------------------------*/
static Int4 SendAlgSummaryGraphPage(BioseqPtr query_bsp, CdInfoPtr cip, BlastPDBNbPtr bpnp, Int2 imgsize, Int2 ScoreLabel, Boolean DrawMap, Boolean AlgGraph,  Int2 subset, Int2 sortpara, Int2 dispform, Int2 pagenum, WWWQueryPtr wqp_head, FILE *File, Int2 blastview)
{
  Int2 i = 0;
  Int4 count = 0, count_onpage = 0;
  ValNodePtr csip = NULL;
  CblastSeqInfoDataPtr csidp = NULL;
  ValNodePtr query = NULL;
  WWWQueryPtr wqp = NULL;
  WWWQueryDataPtr wqdp = NULL;
  CharPtr str_query = NULL;

  csip = bpnp->csip;
  while(csip){
    csidp = csip->data.ptrvalue;
    if(csidp->pagenum == pagenum)count_onpage++;
    count++;
    csip = csip->next;
  }

  if(DrawMap){
    printf("Content-type: text/html\n\n");
    WWWPrintFileData(HEADFILE,  stdout);

    fprintf(File, "<TABLE width=800 BORDER=0 CELLPADDING=3 CELLSPACING=0 bgcolor=#FFFFCC>\n\n");
    fprintf(File, "<TR>\n");
    fprintf(File, "<TD>\n");

    fprintf(File, "<table>");
    fprintf(File, "<tr>\n<td VALIGN=TOP NOWRAP> </td>\n</tr>\n\n");

    if(bpnp->query_title){
      fprintf(File, "<tr>\n");
      fprintf(File, "<td VALIGN=TOP NOWRAP class=H4>\n");
      fprintf(File, "<strong>Query:</strong></td>\n");
      fprintf(File, "<td class=\"TEXT\">%s", bpnp->query_title);
      fprintf(File, "</td>\n</tr>\n\n");
    }

    fprintf(File, "<tr>\n");
    wqp = wqp_head;
    while(wqp){
      wqdp = wqp->data.ptrvalue;
      if(strcmp(wqdp->name, "blast_RID")==0) {
        fprintf(File, "<td>BLAST RID: </td>\n");
        fprintf(File, "<td>%s</td>\n", wqdp->value);
        break;
      }
      wqp = wqp->next;
    }
    fprintf(File, "</tr>\n");
    fprintf(File, "</table>\n");
    fprintf(File, "</TD>\n");
    fprintf(File, "</TR>\n");
    fprintf(File, "</TABLE>");

    fprintf(File, "<FORM METHOD=POST ACTION=\"%s%s\">\n", CGIurl, CGIName);
    wqp = wqp_head;
    while(wqp){
      wqdp = wqp->data.ptrvalue;
      fprintf(File, "<INPUT TYPE=HIDDEN NAME=%s VALUE=%s>\n", wqdp->name, wqdp->value);
      wqp = wqp->next;
    }

    fprintf(File, "<table>\n");
    fprintf(File, "<tr><td>\n");
    fprintf(File, "</td></tr>\n");

    fprintf(File, "<tr><td>\n");
    fprintf(File, "<strong><INPUT TYPE=SUBMIT NAME=dispsub VALUE=List></strong>\n");
    fprintf(File, "<strong>subset grouped by</strong>\n");
    fprintf(File, "<select name=subset>\n");
    for(i = 1; i<=5; i++){
      if(subset == i)fprintf(File, "<option value=%d selected>%s\n", i, subset_name[i-1]);
      else fprintf(File, "<option value=%d>%s\n", i, subset_name[i-1]);
    }
    fprintf(File, "</select>\n");
    fprintf(File, "</td>\n");
   
    fprintf(File, "<td>\n");
    fprintf(File, "<strong>sorted by </strong>\n");
    fprintf(File, "<select name=sortpara>\n");
    for(i = 1; i <= 4; i++){
      if(sortpara == i)fprintf(File, "<option value=%d selected>%s\n", i, sortpara_name[i-1]);
      else fprintf(File, "<option value=%d>%s\n", i, sortpara_name[i-1]);
    }
    fprintf(File, "</select>\n");
    fprintf(File, "</td>\n");

    if(bpnp->numpages > 1){
      fprintf(File, "<td>\n");
      fprintf(File, "<strong>page </strong>\n");
      fprintf(File, "<select name=doclistpage>\n");
      for(i = 1; i <= bpnp->numpages; i++){
        if(pagenum == i)fprintf(File, "<option value=%d selected>%d\n", i, i);
        else fprintf(File, "<option value=%d>%d\n", i, i);
      }
      fprintf(File, "</select>\n");
      fprintf(File, "</td>\n");
    }

    fprintf(File, "<td>\n");
    fprintf(File, "<strong>in </strong>\n");
    fprintf(File, "<select name=dispform>\n");
    for(i = 1; i <= 2; i++){
      if(dispform == i)fprintf(File, "<option value=%d selected>%s\n", i, dispform_name[i-1]);
      else fprintf(File, "<option value=%d>%s\n", i, dispform_name[i-1]);
    }
    fprintf(File, "</select>\n");
    fprintf(File, "</td></tr>\n");

    fprintf(File, "<tr><td>\n");
    fprintf(File, "</td></tr>\n");
    fprintf(File, "</table>\n");
    fprintf(File, "</FORM>\n");

    if(dispform == 1){
          /* &nbsp */
      fprintf(File, "<table>\n");
      fprintf(File, "<TR><TD>\n");
      if(blastview == BLAST_OVERVIEW) fprintf(File, "BLAST found %d related structures. Click on the alignment figure to see alignments and 3D structure.", count); 
      else if(blastview == BLAST_ONEGROUP) fprintf(File, "BLAST found %d related structures with identical sequence. Click on the alignment figure to see alignments and 3D structure.", count); 
      if(blastview == BLAST_ONEPAIR) fprintf(File, "Click on the alignment figure to see alignments and 3D structure."); 
      fprintf(File, "</TD></TR>\n");
      fprintf(File, "<TR><TD>&nbsp;\n");
      fprintf(File, "</TD></TR>\n");
      fprintf(File, "</table>\n");

      fprintf(File, "<img src=\"%scblast.cgi?summary_graph=TRUE", CGIurl);
      fprintf(File, "&subset=%d",subset);
      fprintf(File, "&sortpara=%d",sortpara);
      fprintf(File, "&doclistpage=%d",pagenum);
      fprintf(File, "&dispform=%d",dispform);

      str_query = StringSave("summary_graph");
      ValNodeAddStr(&query, 0, str_query);
      str_query = StringSave("subset");
      ValNodeAddStr(&query, 0, str_query);
      str_query = StringSave("sortpara");
      ValNodeAddStr(&query, 0, str_query);
      str_query = StringSave("doclistpage");
      ValNodeAddStr(&query, 0, str_query);
      str_query = StringSave("dispform");
      ValNodeAddStr(&query, 0, str_query);

      print_query_args(File, wqp_head, query);
      ValNodeFreeData(query);

      fprintf(File, "\" usemap=#img_map border=0 ISMAP>\n");
      fprintf(File, "\n");

      fprintf(File,"<map name=img_map>\n");
    }
    else if(dispform == 2){
      PrintPDBNeighborTable(File, bpnp, pagenum);
    }
  }

  /* SetMMDBSubsetRep(bpnp, subset); */ /* ? */

  if(dispform == 1) DrawSummaryImgAndMap(query_bsp, cip, bpnp, imgsize, pagenum, ScoreLabel, DrawMap, AlgGraph, wqp_head, File);

  if(DrawMap){
    if(dispform == 1 ){
      fprintf(File, "</map>\n");
      fprintf(File, "</body>\n");
      fprintf(File, "</html>");
    }
    else if(dispform == 2){
      fprintf(File, "</body>\n");
      fprintf(File, "</html>");
    }
  }

}
/*--------------------------*/
static Int4 SendAlgGraphPage(BioseqPtr query_bsp, CdInfoPtr cip, BlastPDBNbPtr bpnp, Int2 imgsize, Int2 sortpara, Int2 ScoreLabel, Boolean DrawMap, Boolean AlgGraph, WWWQueryPtr wqp_head, FILE *File, NeighborInfoPtr nip, Int4 subject_str_gi, Int4 subject_sel_gi, Int2 pagenum, Int2 numpages)
{
  ValNodePtr query = NULL; 
  CharPtr str_query = NULL;
  ValNodePtr csp = NULL;
  CblastScoreDataPtr csdp = NULL;

  csp = bpnp->csp;
  csdp = csp->data.ptrvalue;

  if(DrawMap){
    printf("Content-type: text/html\n\n");
    WWWPrintFileData(HEADFILE,  stdout);
    PrintPageView(File, wqp_head, bpnp);

    fprintf(File, "<img src=\"%scblast.cgi?alg_graph=TRUE&viewmethod=1&viewmethod_option=HTML&hit=%d", CGIurl, subject_sel_gi);
    str_query = StringSave("alg_graph");
    ValNodeAddStr(&query, 0, str_query);
    str_query = StringSave("viewmethod");
    ValNodeAddStr(&query, 0, str_query);
    str_query = StringSave("viewmethod_option");
    ValNodeAddStr(&query, 0, str_query);
    str_query = StringSave("hit");
    ValNodeAddStr(&query, 0, str_query);
    print_query_args(File, wqp_head, query);
    ValNodeFreeData(query);
    fprintf(File, "\" usemap=#img_map border=0 ISMAP>\n");
    fprintf(File, "\n");

    fprintf(File,"<map name=img_map>\n");
  }

  DrawSummaryImgAndMap(query_bsp, cip, bpnp, imgsize, pagenum, ScoreLabel, DrawMap, AlgGraph, wqp_head, File);

  if(DrawMap){
    fprintf(File, "</map>\n");
/*  PrintNeighborTable(nip, subject_str_gi, pagenum, numpages); */
    printscore(csdp, stdout);
/*  printf("</FORM>\n"); */

    printf ("</body>\n");
    printf ("</html>");
  }

}
/*--------------------------*/
static Int4 SendAlgInHtmlPage(SeqAnnotPtr sap, SeqEntryPtr sep, BlastPDBNbPtr bpnp, NeighborInfoPtr nip, Int4 subject_str_gi, WWWQueryPtr wqp, Int2 pagenum, Int2 numpages)
{
  NcbiMimeAsn1Ptr   pvnNcbi = NULL;
  BiostrucAlignSeqPtr basp = NULL;

  Uint4       size = 0;
  BytePtr     buf = NULL;
  Nlm_ByteStorePtr bsp = NULL;
  AsnIoBSPtr  aibp = NULL;

/*
  FloatHi      evalue;
  Int4         number;
  FloatHi      bit_score;
  Int4         score;
*/
  Uint4                 uCAVoptions = 0;
  Uint4  CAV_return = 0;

  ValNodePtr csp = NULL;
  CblastScoreDataPtr csdp = NULL;

  csp = bpnp->csp;
  csdp = csp->data.ptrvalue;

  basp = BiostrucAlignSeqNew();
  basp->sequences = sep;
  basp->seqalign = sap;

  pvnNcbi = ValNodeNew(NULL);
  pvnNcbi->choice = NcbiMimeAsn1_alignseq;
  pvnNcbi->data.ptrvalue = basp;

  bsp = Nlm_BSNew(1024);
  aibp = AsnIoBSOpen("wb", bsp);
  if (!bsp || !aibp) {
      printf("Cblast Error:AsnIoBS creation failed.\n"); return 0;
  }

  if (!(NcbiMimeAsn1AsnWrite(pvnNcbi, aibp->aip, NULL)))  {
      printf("Cblast Error:C object -> AsnInBS failed.\n");
  }

  AsnIoFlush(aibp->aip);
  AsnIoBSClose(aibp); aibp = NULL;

  size = Nlm_BSLen(bsp);
  buf = MemNew(size);
  if (!buf) {
      printf("Cblast Error: buf allocation failed\n"); return 0;
  }
     
  Nlm_BSSeek(bsp, 0, 0);
  if (Nlm_BSRead(bsp, buf, size) != size) {
      printf("Cblast Error: AsnIoBS -> buf failed\n"); return 0;
  }

  Nlm_BSFree(bsp); bsp = NULL; 

  printf("Content-type: text/html\n\n");

  WWWPrintFileData(HEADFILE,  stdout);

  PrintPageView(stdout, wqp, bpnp);

  uCAVoptions = CAV_HTML;
  uCAVoptions |= CAV_SHOW_IDENTITY;
  CAV_return = CAV_DisplayMultiple(buf, uCAVoptions, 60, 0.0, NULL, 0, NULL); 

/*
  GetScoreAndEvalue(sap->data, &score, &bit_score, &evalue, &number);  
*/
  printscore(csdp, stdout);

/*PrintNeighborTable(nip, subject_str_gi, pagenum, numpages); */

  printf ("</body>\n");
  printf ("</html>");

  MemFree(buf);
  pvnNcbi->data.ptrvalue = NULL;
  NcbiMimeAsn1Free(pvnNcbi);

  return 0;
}
/*---------------*/
int SendMimePage(SeqAnnotPtr sap, SeqEntryPtr sep, Int4 gi, ValNodePtr sfp)
{
  char sql[128];
  Boolean     isFromDatabase = TRUE;
  unsigned mmdb_id = 0;
  Int4 mol_id = 0;
  MoleculeGraphPtr mgp = NULL;
  SeqIdPtr sip = NULL;
  
  AsnIoPtr aip = NULL;
  NcbiMimeAsn1Ptr mime;
  BiostrucSeqsPtr bssp = NULL;
  BiostrucPtr bsp = NULL;
  Boolean success = FALSE;

  SnpFeatureInfoPtr sfipThis = NULL;
  Cn3dUserAnnotationsPtr cuasp =NULL;
  Cn3dStyleDictionaryPtr csdp = NULL;

/*
  sprintf(sql,"select max(mmdbId) from GiDomMap where gi = %d",gi);
  mmdb_id = constructSingleVastIntValue(sql);
*/
  mmdb_id = constructMmdbIdForGi(gi);

  if(mmdb_id <=0)
  {
    printf("Error: mmdb_id = %d after calling \"%s\".",mmdb_id,sql);
    exit(0);
  }
  bsp = (BiostrucPtr) openBSP(mmdb_id, ONECOORDATOM, MaxModels, FALSE, FALSE, FALSE, &isFromDatabase, MMDBDirPath);
  if (bsp == NULL) {
    printf("Content-type: text/html\r\n\r\n");
    printf("<h2>Error</h2>\n");
    printf("Structure with UID = %ld failed to fetch on Server<p>\n", (long)mmdb_id);
    exit(0);
  }

  bssp = BiostrucSeqsNew();
  bssp->structure = bsp;
  ValNodeLink(&(bssp->sequences), sep);

  SapMSInvert(sap);
  bssp->seqalign = sap;

  if(sfp){
    mgp = bsp->chemical_graph->molecule_graphs;
    while(mgp){
      sip = mgp->seq_id;
      while(sip){
        if(sip->choice = SEQID_GI){
          if(sip->data.intvalue == gi) {
            mol_id = mgp->id;
            break;
          }
        }
        sip = sip->next;
      }
      mgp = mgp->next;
    } 

    csdp = (Cn3dStyleDictionaryPtr) MakeCn3dStyleDicForSNP(sfp);
    bssp->style_dictionary = csdp;

    cuasp = (Cn3dUserAnnotationsPtr) MakeUserAnnotForSNP(sfp, mmdb_id, mol_id);
    bssp->user_annotations = cuasp;
  }
  else {
    csdp = Cn3dStyleDictionaryNew();
    csdp->global_style = (Cn3dStyleSettingsPtr) GetDefaultCn3DStyleSettings();
    csdp->style_table = NULL;
    bssp->style_dictionary = csdp;
  }

  mime = (NcbiMimeAsn1Ptr) ValNodeNew(NULL);
  mime->choice = NcbiMimeAsn1_strucseqs;
  mime->data.ptrvalue=bssp;

  printf ("Content-type: chemical/ncbi-asn1-binary\r\n\r\n");    
/*aip = AsnIoNew(ASNIO_BIN_OUT, stdout, NULL, NULL, NULL);     */      
  aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);          
  if(!NcbiMimeAsn1AsnWrite(mime, aip, NULL)) printf("NcbiMimeAsn1AsnWrite failed!\n"); 
  AsnIoFlush(aip);
  aip = AsnIoClose(aip);

  mime->data.ptrvalue=NULL ; 
  NcbiMimeAsn1Free(mime);

  return 1;
}
/*--------------------*/
int SendAsnPage(SeqAnnotPtr sap, SeqEntryPtr sep, Int4 gi, ValNodePtr sfp, Char DispOpt)
{

  char sql[128];
  Boolean     isFromDatabase = TRUE;
  long mmdb_id = 0;
  Int4 mol_id = 0;
  MoleculeGraphPtr mgp = NULL;
  SeqIdPtr sip = NULL;

  AsnIoPtr aip = NULL;
  NcbiMimeAsn1Ptr mime;
  BiostrucSeqsPtr bssp = NULL;
  BiostrucPtr bsp = NULL;
  Boolean success = FALSE;

  SnpFeatureInfoPtr sfipThis = NULL;
  Cn3dUserAnnotationsPtr cuasp =NULL;
  Cn3dStyleDictionaryPtr csdp = NULL;

/*
  sprintf(sql,"select max(mmdbId) from GiDomMap where gi = %d",gi);
  mmdb_id = constructSingleVastIntValue(sql);
*/
  mmdb_id = constructMmdbIdForGi(gi);

  if(mmdb_id <=0)
  {
    printf("Error: mmdb_id = %d after calling \"%s\".",mmdb_id,sql);
    exit(0);
  }
  bsp = (BiostrucPtr) openBSP(mmdb_id, ONECOORDATOM, MaxModels, TRUE, FALSE, FALSE, &isFromDatabase, MMDBDirPath);
  if (bsp == NULL) {
    printf("Content-type: text/html\r\n\r\n");
    printf("<h2>Error</h2>\n");
    printf("Structure with UID = %ld failed to fetch on Server<p>\n", (long) mmdb_id);
    exit(0);
  }

  bssp = BiostrucSeqsNew();
  bssp->structure = bsp;
  ValNodeLink(&(bssp->sequences), sep);

  SapMSInvert(sap);
  bssp->seqalign = sap;

  if(sfp){
    mgp = bsp->chemical_graph->molecule_graphs;
    while(mgp){
      sip = mgp->seq_id;
      while(sip){
        if(sip->choice = SEQID_GI){
          if(sip->data.intvalue == gi) {
            mol_id = mgp->id;
            break;
          }
        }
        sip = sip->next;
      }
      mgp = mgp->next;
    }

    csdp = (Cn3dStyleDictionaryPtr) MakeCn3dStyleDicForSNP(sfp);
    bssp->style_dictionary = csdp;

    cuasp = (Cn3dUserAnnotationsPtr) MakeUserAnnotForSNP(sfp, mmdb_id, mol_id);
    bssp->user_annotations = cuasp;
  }
  else {
/*
    csdp = Cn3dStyleDictionaryNew();
    csdp->global_style = (Cn3dStyleSettingsPtr) GetDefaultCn3DStyleSettings();
    csdp->style_table = NULL;
    bssp->style_dictionary = csdp;
*/
  }

  mime = (NcbiMimeAsn1Ptr) ValNodeNew(NULL);
  mime->choice = NcbiMimeAsn1_strucseqs;
  mime->data.ptrvalue=bssp;

  if(DispOpt == 'c'){
     printf ("Content-type: chemical/ncbi-asn1-binary\r\n\r\n");
     aip = AsnIoNew(ASNIO_BIN_OUT, stdout, NULL, NULL, NULL);  
  }
  else if(DispOpt == 'a') {
     printf("Content-type: application/octet-stream\r\n\r\n");
     aip = AsnIoNew(ASNIO_BIN_OUT, stdout, NULL, NULL, NULL);  
  }
  else if(DispOpt == 'p') {
    aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);
    printf ("Content-type: text/html\r\n\r\n");
    printf ("<HTML><pre>\n");
  }
  
  NcbiMimeAsn1AsnWrite(mime, aip, NULL);    
  AsnIoFlush(aip);
  AsnIoReset(aip);
  aip = AsnIoClose(aip);

  if(DispOpt == 'p')printf ("</PRE></HTML>\r\n");
  mime->data.ptrvalue=NULL ;
  NcbiMimeAsn1Free(mime);

  return 1;
}
/*-----------------*/ 
Int2 Main()
{

  Boolean       Netscape2_OK = FALSE;
  WWWErrorCode      error; 

  Int4    IndexArgs = -1, indx = 0, NumEntries = 0;
  Int4    iBytes = 0;
  Int2    imagsize = 0;
  int     algseq = 0;

  Char DispOpt;
  CharPtr Cmd;
  CharPtr pcName = NULL, pcThis = NULL;

  CdInfoPtr cip = NULL;
  SeqAnnotPtr sap_dart = NULL;
  SeqAnnotPtr sap_set = NULL, sap_subject_pair, sap_pdb_pair = NULL;
  SeqAlignPtr salp = NULL;
  DenseSegPtr dsp = NULL;
  BioseqPtr query_bsp = NULL, cdquery_bsp = NULL, subject_bsp = NULL;
  SeqIdPtr sipQuery=NULL, sip=NULL;
  ObjectIdPtr              oidp;
  SeqEntryPtr sep = NULL;
  Int4 query_gi = 0, subject_rep_gi = 0, subject_sel_gi = 0, subject_str_gi = 0; 
  Int2 status = -3;

  BLAST_OptionsBlkPtr option = NULL;
  Int4 db_length = 90077593;    /* default */
  Int4 expect_value = 10;   /* default */
  Char blastinfo[PATH_MAX];

  BlastPDBNbPtr bpnp = NULL;
  Boolean ForGenome = FALSE, ForSNP = FALSE, DrawMap = FALSE, QueryDump = FALSE, DrawCdMap = TRUE;

  Boolean DrawBlastPDBNbSummary = FALSE, DrawBlastPDBNbSummaryOneGroup = FALSE, DrawBlastPDBNbSummaryOnePairOneHSP = FALSE, DrawBlastPDBNbSummaryOnePairMultiHSP = FALSE;
  Boolean NoHspArg = FALSE;

  NeighborInfoPtr nip = NULL;
  NeighborInfoDataPtr nidp_sel= NULL;
  ReadDBFILEPtr   rdfp=NULL;
  FILE *fp_nrpdb;

  WWWQueryPtr wqp_head = NULL;
  WWWQueryDataPtr wqdp = NULL;
  Char str[1024];
  Int2 query_num = 0;

  Int4 snp_pos = 0, snp_count = 0;
  ValNodePtr sfp = NULL, sfpThis = NULL; /*data.ptrvalue is snp feature pointer*/
  SnpFeatureInfoPtr sfipThis = NULL;

  Int2 ScoreLabel = 1;
  Int2 pagenum = 1, numpages = 0, subset = 1, sortpara = 1, dispform = 1, blastview=0;
  Int2 hsp = 0;
  SeqIdPtr subject_pdbid = NULL;
  ValNodePtr descr = NULL;

  logFilePtr = fopen("mylog","w");

  CblastSrvGetParams();
  CblastSrvInit();

  putenv("NCBI=/netopt/ncbi_tools");
  putenv("SYBASE=/netopt/Sybase/clients/current");
/*    putenv("BLASTDB=/net/fridge/vol/export/blast/db/blast"); */
  putenv("BLASTDB=/blast/db/blast");
  if(!(rdfp = readdb_new("nr", TRUE))){
    printf("Content-type: text/html\r\n\r\n");
    printf("<h2>Error</h2>\n");
    printf("readdb failed!\n");
    return;
  }

  if((error = WWWGetArgs(&info)) != WWWErrOk) {
      printf("Content-type: text/html\r\n\r\n");
      printf("<H2>CBLAST Error</H2>\n Failed to Process Posting - Check your GET/POST syntax\n");
      exit(0);
  }

   IndexArgs = -1;
   if((IndexArgs = WWWFindName(info, "blast_RID")) >= 0){
     pcThis = WWWGetValueByIndex(info,IndexArgs);
     CblastAddWWWQueryData("blast_RID", pcThis, &query_num, &wqp_head);
     status = QBlastGetResults(pcThis, &sap_set, &query_bsp, NULL, NULL, NULL, NULL); 
     sip=query_bsp->id;
     sipQuery=(SeqIdPtr) ValNodeNew(NULL);
     sipQuery->choice = SEQID_LOCAL;
     oidp = ObjectIdNew();
     oidp->str = StringSave("query");
     sipQuery->data.ptrvalue = oidp;
     sipQuery->next=NULL;
     query_bsp->id = SeqIdDup(sipQuery);
     sip = SeqIdFree(sip);
     if(!sap_set){
        printf("Content-type: text/html\r\n\r\n");
        printf("<h2>Error</h2>\n");
        printf("Can not retrieve data with RID: %s!\n", pcThis);
        exit(0);
     }
     salp=sap_set->data;
     while(salp){
       dsp = salp->segs;
       sip=dsp->ids;
       dsp->ids = SeqIdDup(sipQuery);
       dsp->ids->next=sip->next;
       sip->next=NULL;
       sip = SeqIdFree(sip); 
 
       salp = salp->next;
     }
     sipQuery=SeqIdFree(sipQuery);
/*
{
AsnIoPtr aip = NULL;
aip = AsnIoOpen("test4.sap", "w");
SeqAnnotAsnWrite(sap_set, aip, NULL);
aip = AsnIoClose(aip);
aip = AsnIoOpen("test4.bsp", "w");
BioseqAsnWrite(query_bsp, aip, NULL);
aip = AsnIoClose(aip);
}
*/

     IndexArgs = -1;
     if((IndexArgs = WWWFindName(info, "blast_CD_RID")) >= 0){
       pcThis = WWWGetValueByIndex(info,IndexArgs);
       if(strcmp(pcThis, "0")==0) DrawCdMap = FALSE; 
       else { 
         queryrid = StringSave(pcThis);
         CblastAddWWWQueryData("blast_CD_RID", pcThis, &query_num, &wqp_head);
         status = QBlastGetResults(pcThis, &sap_dart, &cdquery_bsp, NULL, NULL, NULL, NULL); 
         if(sap_dart != NULL) DrawCdMap = TRUE;
         else DrawCdMap = FALSE;
       }
     }
     else DrawCdMap = FALSE;

/*
{
AsnIoPtr aip = NULL;
aip = AsnIoOpen("test_bacteria.sap", "w");
SeqAnnotAsnWrite(sap_set, aip, NULL);
aip = AsnIoClose(aip);
}
*/

/*
{
SeqAnnotPtr sap_dart = NULL;
AsnIoPtr aip = NULL;
aip = AsnIoOpen("test3.sap", "w");
status = QBlastGetResults("1035554681-021776-25378", &sap_set, &query_bsp, NULL, NULL, NULL, NULL);
SeqAnnotAsnWrite(sap_set, aip, NULL);
aip = AsnIoClose(aip);
aip = AsnIoOpen("test2.bsp", "w");
BioseqAsnWrite(query_bsp, aip, NULL);
aip = AsnIoClose(aip);

if(Dart_Gi2Sap(dartcon, 10728126, &sap_dart, NULL)&&(sap_dart != NULL)){
  aip = AsnIoOpen("dart.sap", "w");
  SeqAnnotAsnWrite(sap_dart, aip, NULL);
  aip = AsnIoClose(aip);
}

}
*/
/*
{
AsnIoPtr aip = NULL;
aip = AsnIoOpen("test2.sap", "r");
sap_set = SeqAnnotAsnRead(aip, NULL);
aip = AsnIoClose(aip);
aip = AsnIoOpen("test2.bsp", "r");
query_bsp = BioseqAsnRead(aip, NULL);
aip = AsnIoClose(aip);
aip = AsnIoOpen("dart.sap", "r");
sap_dart = SeqAnnotAsnRead(aip, NULL);
aip = AsnIoClose(aip);
}
*/
     IndexArgs = -1;
     if((IndexArgs = WWWFindName(info, "blast_rep_gi")) >= 0){
       pcThis = WWWGetValueByIndex(info,IndexArgs);
       subject_rep_gi = atoi(pcThis);

       sprintf(str, "%d", subject_rep_gi);
       CblastAddWWWQueryData("blast_rep_gi", str, &query_num, &wqp_head);
     }

     IndexArgs = -1;
     if((IndexArgs = WWWFindName(info, "hsp")) >= 0){
       pcThis = WWWGetValueByIndex(info,IndexArgs);
       CblastAddWWWQueryData("hsp", pcThis, &query_num, &wqp_head);
       hsp = atoi(pcThis);
          /* blast service sends hsp=0 */
     }
     else {
       hsp = 1;
     }

     IndexArgs = -1;
     if((IndexArgs = WWWFindName(info, "blast_view")) >= 0){
       pcThis = WWWGetValueByIndex(info,IndexArgs);
       if(strcmp(pcThis, "overview")==0) {
         blastview = 1;
         DrawBlastPDBNbSummary = TRUE;
         if(hsp == 0) hsp = 1;
       }
       else if(strcmp(pcThis, "onegroup")==0) {
         blastview = 2;
         DrawBlastPDBNbSummaryOneGroup = TRUE;
         if(hsp == 0) hsp = 1;
       }
       else if(strcmp(pcThis, "onepair")==0) {
         blastview = 3;
         if(hsp == 0){
           if(MultiHSP(sap_set, subject_rep_gi)) DrawBlastPDBNbSummaryOnePairMultiHSP = TRUE; 
           else {
             DrawBlastPDBNbSummaryOnePairOneHSP = TRUE;
             hsp = 1;
           }
         }
         else DrawBlastPDBNbSummaryOnePairOneHSP = TRUE;
       }
       CblastAddWWWQueryData("blast_view", pcThis, &query_num, &wqp_head);
     }
     else DrawBlastPDBNbSummary = TRUE;

     IndexArgs = -1;
     if((IndexArgs = WWWFindName(info, "hit")) >= 0){
       pcThis = WWWGetValueByIndex(info,IndexArgs);
       subject_sel_gi = atoi(pcThis);
       CblastAddWWWQueryData("hit", pcThis, &query_num, &wqp_head);
       if(subject_sel_gi == 0) subject_sel_gi = subject_rep_gi;    
     }
     else subject_sel_gi = subject_rep_gi; 
     subject_str_gi = subject_sel_gi;

     if(subject_sel_gi > 0) {
       subject_bsp = CblastGetBioseqWithGi(subject_sel_gi, rdfp);
     }

     if(subject_sel_gi > 0) {
       sap_subject_pair = (SeqAnnotPtr)GetAlgForSubjectPairPerHSP(sap_set, subject_rep_gi, hsp); 
/*     sap_subject_pair = (SeqAnnotPtr)GetAlgForSubjectPair(sap_set, subject_rep_gi);  */
       if(sap_subject_pair) sap_pdb_pair = (SeqAnnotPtr) CblastGetSapPdbPair(sap_subject_pair, subject_bsp);
     }

   }

   IndexArgs = -1;
   if((IndexArgs = WWWFindName(info, "master_gi")) >= 0){
     ForGenome = TRUE;
     pcThis = WWWGetValueByIndex(info,IndexArgs);
     query_gi = atoi(pcThis);
     sprintf(str, "%d", query_gi);
     CblastAddWWWQueryData("master_gi", str, &query_num, &wqp_head);

     IndexArgs = -1;
     if((IndexArgs = WWWFindName(info, "neighbor_gi")) >= 0){
       pcThis = WWWGetValueByIndex(info,IndexArgs);
       subject_rep_gi = atoi(pcThis);
       subject_str_gi = subject_rep_gi;
       sprintf(str, "%d", subject_rep_gi);
       CblastAddWWWQueryData("neighbor_gi", str, &query_num, &wqp_head);
     }
     else {
       printf("Content-type: text/html\r\n\r\n");
       printf("<h2>Error</h2>\n");
       printf("CBLAST: neighbor GI should be given to identify the pairwise seqalign!\n");
       return;
     }

     IndexArgs = -1;
     if((IndexArgs = WWWFindName(info, "hit")) >= 0){
       pcThis = WWWGetValueByIndex(info,IndexArgs);
       subject_sel_gi = atoi(pcThis);
     }
     else subject_sel_gi = subject_rep_gi;

     option = BLASTOptionNew("blastp", TRUE);
     GetAppParam("cblast", "BLASTOPTION", "NRSize", "", blastinfo, PATH_MAX);
     if(blastinfo[0] == '\0') {
        ErrPostEx(SEV_ERROR, 0, 0, "CBLAST config file \n BLASTOPTION section has no NRSize ...\n Default databasesize will be used\n");
     }
     else {
        sscanf(blastinfo, "%d", &db_length);
        option->db_length = db_length;
     }
     GetAppParam("cblast", "BLASTOPTION", "CutOff", "", blastinfo, PATH_MAX);
     if(blastinfo[0] != '\0') {
        sscanf(blastinfo, "%d", &expect_value);
        option->expect_value = (FloatHi) expect_value;     /* default e-value 10 */
     }
                       /* one-pass non-multiple hits search with low threshold */

     option->two_pass_method  = FALSE;
     option->multiple_hits_only  = FALSE;
     option->threshold_second = 9;

     query_bsp = CblastGetBioseqWithGi(query_gi, rdfp);
     if(!query_bsp) {
        printf("Content-type: text/html\n\n");
        printf("<h2>CBLAST Error</h2>\n");
        printf("<h3>query sequence with gi number %d is not populated.\nContact %s</h3>\n", query_gi, MAILto);
     }

     subject_bsp = CblastGetBioseqWithGi(subject_sel_gi, rdfp);
     if(!subject_bsp) {
        printf("Content-type: text/html\n\n");
        printf("<h2>CBLAST Error</h2>\n");
        printf("<h3>subject sequence with gi number %d is not populated.\nContact %s</h3>\n", subject_rep_gi, MAILto);
     }

     salp = BlastTwoSequences(query_bsp, subject_bsp, "blastp", option);
     if(!salp) {
        printf("Content-type: text/html\n\n");
        printf("<h2>CBLAST Error</h2>\n");
        printf("<h3>Blast sequences with gi number %d and %d failed, no significant similarity found. \nContact %s</h3>\n", query_gi, subject_rep_gi, MAILto);
        return;
     }
     sap_subject_pair = SeqAnnotNew();
     sap_subject_pair->type = 2;
     sap_subject_pair->data = salp;
     sap_pdb_pair = (SeqAnnotPtr) CblastGetSapPdbPair(sap_subject_pair, subject_bsp);
          /* do pdb-id replacement ? */

/*******************************/
         /* to check SNP feature request */
     snp_count = 0;
     ForSNP = FALSE;
     if((indx = WWWFindName(info, "snp_pos")) >= 0) {
       ForSNP = TRUE;
       NumEntries = WWWGetNumEntries(info);
       for(IndexArgs = 0; IndexArgs < NumEntries; IndexArgs++){
         pcName = WWWGetNameByIndex(info,  IndexArgs);
         pcThis = WWWGetValueByIndex(info, IndexArgs);
         if(StrICmp(pcName, "snp_pos") == 0){
           if(isdigit(pcThis[0])){
             snp_pos = (Int4) atoi(pcThis);
             snp_count++;
             sfipThis = (SnpFeatureInfoPtr) MemNew(sizeof(SnpFeatureInfo));
             sfipThis->pos = snp_pos;
             sfipThis->feature_id = snp_count;
             sfipThis->color = 1;
             sfipThis->id = 0;
             ValNodeAddPointer(&sfp, snp_count, (SnpFeatureInfoPtr) sfipThis);
           }
         }
       }

       sfpThis = sfp;
       for(IndexArgs = 0; IndexArgs < NumEntries; IndexArgs++){
         pcName = WWWGetNameByIndex(info,  IndexArgs);
         pcThis = WWWGetValueByIndex(info, IndexArgs);
         if(StrICmp(pcName, "snp_title") == 0){
           if(sfpThis != NULL){
             sfipThis = sfpThis->data.ptrvalue;
             sfipThis->title = StringSave(pcThis);
             sfpThis = sfpThis->next;
           }
         }
       }

       sfpThis = sfp;
       for(IndexArgs = 0; IndexArgs < NumEntries; IndexArgs++){
         pcName = WWWGetNameByIndex(info,  IndexArgs);
         pcThis = WWWGetValueByIndex(info, IndexArgs);
         if(StrICmp(pcName, "snp_descr") == 0){
           if(sfpThis != NULL){
             sfipThis = sfpThis->data.ptrvalue;
             sfipThis->descr = StringSave(pcThis);
             sfpThis = sfpThis->next;
           }
         }
       }
       sfpThis = sfp;
       for(IndexArgs = 0; IndexArgs < NumEntries; IndexArgs++){
         pcName = WWWGetNameByIndex(info,  IndexArgs);
         pcThis = WWWGetValueByIndex(info, IndexArgs);
         if(StrICmp(pcName, "snp_color") == 0){
           if(sfpThis != NULL){
             sfipThis = sfpThis->data.ptrvalue;
             sfipThis->color = (Int4) atoi(pcThis);
             sfpThis = sfpThis->next;
           }
         }
       }

       sfpThis = sfp;
       for(IndexArgs = 0; IndexArgs < NumEntries; IndexArgs++){
         pcName = WWWGetNameByIndex(info,  IndexArgs);
         pcThis = WWWGetValueByIndex(info, IndexArgs);
         if(StrICmp(pcName, "snp_id") == 0){
           if(sfpThis != NULL){
             sfipThis = sfpThis->data.ptrvalue;
             sfipThis->id = (Int4) atoi(pcThis);
             sfpThis = sfpThis->next;
           }
         }
       }

     }

/* done with SNP features add */

/*******************************/
   }

     /* to be modified */
/*
   if(subject_rep_gi > 0 && DrawBlastPDBNbSummaryOnePairOneHSP){
     nip = GetRedundantPDBNeighborInfo(subject_rep_gi, rdfp);
     {  NeighborInfoPtr nipThis = NULL;
        Int2 count = 0;
        nipThis = nip;
        while(nipThis){
          count++;
          nipThis = nipThis->next;
        }
        count = count-1;
        numpages = (Int2) ((count - count%entry_page)/entry_page) + 1; 
     }

     if(nip == NULL){
       printf("Content-type: text/html\r\n\r\n");
       printf("<h2>Error</h2>\n");
       printf("CBLAST: No PDB Neighbor Identified for gi %d!\n", subject_rep_gi);
       return;
     }

     nidp_sel = (NeighborInfoDataPtr)GetNIForSel(subject_sel_gi, nip);
     if(nidp_sel){
       sap_pdb_pair = ReplaceSeqIdForSeqAnnot(sap_subject_pair, nidp_sel->pdbid);
     }
     else {
       printf("Content-type: text/html\r\n\r\n");
       printf("<h2>Error</h2>\n");
       printf("CBLAST: No Representative info available!\n");
       return;
     }

     nip = (NeighborInfoPtr)GetRankedNeighborInfoFD(nip);
   }
*/

/*
{
int i = 0;
MapRowInfoPtr mrip = NULL;
ValNodePtr mapinfo=NULL, maprow = NULL;
CdNbInfoPtr *cnip = NULL; 

mapinfo = cip->mapinfo;
while(mapinfo){
printf("mapinfo->choice: %d\n", mapinfo->choice);
  maprow = mapinfo->data.ptrvalue;
  while(maprow){
    mrip = maprow->data.ptrvalue;
    printf("mrip: from %d to %d iColor: %d CdName %s PssmId %d\n", mrip->from, mrip->to, mrip->iColor, mrip->CdName, mrip->PssmId); 
    maprow = maprow->next;
  }
  mapinfo = mapinfo->next;
}

cnip = cip->cnip;
for(i=0; i<cip->cd_num; i++){
printf("%s\n", cnip[i]->CdName);
printf("CdName: %s bRep: %d iClust: %d iMaprow: %d from: %d to: %d\n", cnip[i]->CdName, cnip[i]->bRep, cnip[i]->iClust, cnip[i]->iMaprow, cnip[i]->from, cnip[i]->to);
}

}
*/

   IndexArgs = -1;
   if((IndexArgs = WWWFindName(info, "viewmethod")) >= 0){
     if((IndexArgs = WWWFindName(info, "viewmethod_option")) >= 0){
       pcThis = WWWGetValueByIndex(info,IndexArgs);
       if(!StrICmp(pcThis, "HTML")) {DispOpt = 'h'; DispOptChoice = 1;}
       else if(!StrICmp(pcThis, "Graphic")) {DispOpt = 'g'; DispOptChoice = 2;}
     }
     else {
       DispOpt = 'h'; DispOptChoice = 1;   
     }
   }  
   else if((IndexArgs = WWWFindName(info, "3d_viewmethod")) >= 0){
     if((IndexArgs = WWWFindName(info, "3d_viewmethod_option")) >= 0){
       pcThis = WWWGetValueByIndex(info,IndexArgs);
       if(!StrICmp(pcThis, "Cn3D")) {DispOpt = 'c'; DispOptChoice = 3;}
       else if(!StrICmp(pcThis, "SAVE_ASN1")) {DispOpt = 'a'; DispOptChoice = 4;}
       else if(!StrICmp(pcThis, "PRINT_ASN1")) {DispOpt = 'p'; DispOptChoice = 5;}
     }
     else {
       DispOpt = 'c'; DispOptChoice = 3;
     }
   }
   else if(ForSNP || ForGenome){
     DispOpt = 'c';
     DispOptChoice = 3;
   }
   else {
     DispOpt = 'g'; 
     DispOptChoice = 2;   
   }


   IndexArgs = -1;
   if((IndexArgs = WWWFindName(info, "cmd")) >= 0){
     pcThis = WWWGetValueByIndex(info,IndexArgs);
     Cmd = StringSave(pcThis);
   }

   IndexArgs = -1;
   if((IndexArgs = WWWFindName(info, "query_dump")) >= 0){
     QueryDump = TRUE;
   }
   else QueryDump = FALSE;

   IndexArgs = -1;
   if((IndexArgs = WWWFindName(info, "doclistpage")) >= 0){
     pcThis = WWWGetValueByIndex(info,IndexArgs);
     pagenum = (Int2) atoi(pcThis);
   }
   else pagenum = 1;

   IndexArgs = -1;
   if((IndexArgs = WWWFindName(info, "summary_graph")) >= 0){
     DrawMap = FALSE;
   } else DrawMap = TRUE;

   IndexArgs = -1;
   if((IndexArgs = WWWFindName(info, "subset")) >= 0){
     pcThis = WWWGetValueByIndex(info,IndexArgs);
     subset = (Int2) atoi(pcThis);
   } else subset = 1;

   IndexArgs = -1;
   if((IndexArgs = WWWFindName(info, "sortpara")) >= 0){
     pcThis = WWWGetValueByIndex(info,IndexArgs);
     sortpara = (Int2) atoi(pcThis);
   } else sortpara = 1;
   ScoreLabel = sortpara;

   IndexArgs = -1;
   if((IndexArgs = WWWFindName(info, "dispform")) >= 0){
     pcThis = WWWGetValueByIndex(info,IndexArgs);
     dispform = (Int2) atoi(pcThis);
   } else dispform = 1;

   if(DrawBlastPDBNbSummary){
     bpnp = (BlastPDBNbPtr)GetBlastPDBNbInfo(sap_set, 0, 0, rdfp, "ALL_PDB");
/*     bpnp = (BlastPDBNbPtr)GetBlastPDBNbInfo(sap_set, 0, 0, rdfp, "OneEachGroup", "DrawBlastPDBNbSummary");*/
   }
   else if(DrawBlastPDBNbSummaryOneGroup){
     bpnp = (BlastPDBNbPtr)GetBlastPDBNbInfo(sap_set, subject_rep_gi, 0, rdfp, "ALLOneGroup");
   }
   else if(DrawBlastPDBNbSummaryOnePairMultiHSP){
     bpnp = (BlastPDBNbPtr)GetBlastPDBNbInfo(sap_set, subject_rep_gi, subject_sel_gi, rdfp, "OnePairMultiHSP");
   }
   else if(DrawBlastPDBNbSummaryOnePairOneHSP){
        /* case blastview assuming subject_sel_gi > 0*/
     subject_pdbid = (SeqIdPtr) CblastGetPDBIdFromBioseq(subject_bsp); 
     bpnp = (BlastPDBNbPtr)BlastPDBNbInfoBuild(sap_subject_pair, subject_rep_gi, subject_sel_gi, subject_pdbid, rdfp);
     subject_pdbid = SeqIdFree(subject_pdbid);
   }
   if(bpnp) {
     bpnp->query_length = query_bsp->length;
     if(query_bsp->descr){
       descr = query_bsp->descr;
       while(descr){
         if(descr->choice == Seq_descr_title) {
           bpnp->query_title = StringSave(descr->data.ptrvalue);
           break;
         }
         descr = descr->next;
       }
     }
     SetMMDBSubsetRep(bpnp, subset); /* ? */
     bpnp = (BlastPDBNbPtr)CblastIndexHSP(bpnp);

     EntrezFini();
     ReadDBBioseqFetchEnable("cblast", "nr", FALSE, TRUE);
     bpnp = (BlastPDBNbPtr)CblastGetAlignScoreParam(bpnp);
     ReadDBBioseqFetchDisable();

     bpnp = (BlastPDBNbPtr)SortBlastPDBNb(bpnp, sortpara);   
     bpnp = (BlastPDBNbPtr)CblastIndexPage(bpnp);
   }

/*
{
ValNodePtr csp = NULL, csip = NULL;
CblastSeqInfoDataPtr csidp = NULL;
CblastScoreDataPtr csdp = NULL;
PDBSeqIdPtr pdb_sip = NULL;
Char str_score[120];

csip = bpnp->csip;
csp = bpnp->csp;
while(csip){
  csidp = csip->data.ptrvalue;
  csdp = csp->data.ptrvalue;
  pdb_sip = csidp->pdbid->data.ptrvalue;
printf("rep_gi: %d mol: %s chain: %c\n", csidp->gi, pdb_sip->mol, pdb_sip->chain);
  print_bitscore(csdp->bit_score, str_score);
printf("bit_score: %s\n", str_score);
  print_evalue(csdp->evalue, str_score);
printf("evalue: %s\n", str_score);
printf("length: %d\n", csdp->length);
  csip = csip->next;
  csp = csp->next;
}
}
*/


   if(DrawBlastPDBNbSummary || DrawBlastPDBNbSummaryOneGroup || DrawBlastPDBNbSummaryOnePairMultiHSP || DrawBlastPDBNbSummaryOnePairOneHSP || DispOpt == 'g'){
     if(DrawCdMap){
       cip = (CdInfoPtr) CdInfoNew();
       cip->sap = AsnIoMemCopy(sap_dart, (AsnReadFunc)SeqAnnotAsnRead, (AsnWriteFunc)SeqAnnotAsnWrite);
       cip->query_length = query_bsp->length;
       cip = (CdInfoPtr) FillCdInfo(cip, dartcon);
     }
     else cip = NULL;

     maxseqlen = query_bsp->length;

     algseq = GetSelectedAlgSeqNum(bpnp, pagenum);
       /* if the pagenum out of sinchronization due to change of subdataset, set it back as 1 */
     if(algseq == 0){
       pagenum = 1;
       algseq = GetSelectedAlgSeqNum(bpnp, pagenum);
     }

     if(DrawCdMap) imagsize = ConsDomStartY + cip->maprow_num*(CdMapRowSpace+FontBH) + algseq*(MapRowSpace + MapRowHeight) + 2*MapRowSpace + 2*FontBH + 20; 
     else imagsize = ConsDomStartY + algseq*(MapRowSpace + MapRowHeight) + 2*MapRowSpace + 2*FontBH + 20;
   }

   if(QueryDump){
     CblastDumpQuery(query_bsp, stdout);
   }
/*
   else if(DispOpt == 'g') {

     IndexArgs = -1;
     if((IndexArgs = WWWFindName(info, "alg_graph")) >= 0){
       DrawMap = FALSE;
     } else DrawMap = TRUE;

     SendAlgGraphPage(query_bsp, cip, bpnp, imagsize, sortpara, ScoreLabel, DrawMap, TRUE, wqp_head, stdout, nip, subject_str_gi, subject_sel_gi, pagenum, numpages); 

   }
*/
   else if(DispOpt == 'h') {
     sep = GetSeqEntry(query_bsp, subject_bsp);
     iBytes = SendAlgInHtmlPage(sap_pdb_pair, sep, bpnp, nip, subject_str_gi, wqp_head, pagenum, numpages);
   }
   else if(DispOpt == 'c' || DispOpt == 'a' || DispOpt == 'p') {
     sep = GetSeqEntry(subject_bsp, query_bsp);
     iBytes = SendAsnPage(sap_pdb_pair, sep, subject_str_gi, sfp, DispOpt);
   }
   else if(DrawBlastPDBNbSummary || DrawBlastPDBNbSummaryOneGroup || DrawBlastPDBNbSummaryOnePairMultiHSP || DrawBlastPDBNbSummaryOnePairOneHSP){
     SendAlgSummaryGraphPage(query_bsp, cip, bpnp, imagsize, ScoreLabel, DrawMap, FALSE, subset, sortpara, dispform, pagenum, wqp_head, stdout, blastview);
      
   }

  if(bpnp) bpnp = (BlastPDBNbPtr) BlastPDBNbFree(bpnp);
  if(nip) nip = NeighborInfoFree(nip);
  if(cip) cip = (CdInfoPtr) CdInfoFree(cip);

  if(sep) sep = SeqEntryFree(sep);
  if(subject_bsp) subject_bsp = BioseqFree(subject_bsp);
  if(query_bsp) query_bsp = BioseqFree(query_bsp);
  if(cdquery_bsp) cdquery_bsp = BioseqFree(cdquery_bsp);
  if(sap_set) sap_set = SeqAnnotFree(sap_set);
  if(sap_dart) sap_dart = SeqAnnotFree(sap_dart);
  if(sap_pdb_pair) sap_pdb_pair = SeqAnnotFree(sap_pdb_pair);
  if(sap_subject_pair) sap_subject_pair = SeqAnnotFree(sap_subject_pair);

  CloseMMDBAPI();
  if (psfailed == TRUE) MMDBFini_FF();
  else MMDBFini_PS();
  Dart_Fini(dartcon);
  fflush(stdout);
  rdfp = readdb_destruct(rdfp);

  VastSrvFinish();
  MmdbSrvFinish();
  exit(0);

}
