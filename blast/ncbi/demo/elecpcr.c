/* $Id: elecpcr.c,v 6.9 2002/07/25 14:15:09 beloslyu Exp $
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
* File Name:  $RCSfile: elecpcr.c,v $
*
* Author:  Sergei Shavirin
*
* Version Creation Date: 12/19/1996
*
* $Revision: 6.9 $
*
* File Description:
*         Main program for WWW and Command-Line Electronic PCR
*
* $Log: elecpcr.c,v $
* Revision 6.9  2002/07/25 14:15:09  beloslyu
* change www3 to www
*
* Revision 6.8  1999/07/27 18:43:04  shavirin
* Fixed problems found on PC NT computer.
*
* Revision 6.7  1999/02/24 16:49:23  kans
* use accutils copy of IS_ntdb_accession and IS_protdb_accession
*
* Revision 6.6  1999/02/23 17:28:11  shavirin
* Replaced IS_ accession verification functions by it's "relaxed"
* version
*
* Revision 6.5  1998/05/22 19:19:28  shavirin
* Updated for porting to sunweb. Fixed bug with filtering of
* organisms.
*
* Revision 6.4  1998/05/01 19:22:29  shavirin
* Improved WWW page formating
*
* Revision 6.3  1998/05/01 18:22:58  shavirin
* Next revision
*
* Revision 6.2  1998/04/28 19:32:13  shavirin
* Fixed minor bugs detected by purify
*
* Revision 6.1  1997/09/03 17:19:45  shavirin
* Added some debug information and sequence prevented from freeing
*
* Revision 6.0  1997/08/25 18:19:31  madden
* Revision changed to 6.0
*
* Revision 1.3  1997/05/23 15:32:18  shavirin
* Added ability to use local database files (for remote users)
*
* Revision 1.2  1997/05/14 19:12:41  shavirin
* Added define LF 10
*
* Revision 1.1  1996/12/19 20:54:35  shavirin
* Initial revision
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <ncbiwww.h>
#include <accentr.h>
#include <accutils.h>
#include <stsutil.h>

/****************************************************************************/
/* DEFINES */
/****************************************************************************/

#define BG_COLOR      "#FDF5E6"
#define PRIMER1_COLOR "#00BF00" 
#define PRIMER2_COLOR "red"

#define LogFile       "wwwsts.log"

#define FASTA_IN 1
#define ACC_IN   2

#define MINSEQLEN 10
#define LF 10

/****************************************************************************/
/* TYPEDEFS */
/****************************************************************************/

typedef struct AccList {
    CharPtr acc;
    struct AccList *next;
} AccList, PNTR AccListPtr;

typedef struct StsPar {
    CharPtr sequence;
    Boolean html;
    Int4 organism;
    CharPtr orgname;
    Int4 intype;
    Boolean detailed;
    CharPtr sts_db_name;
    CharPtr org_db_name;
    CharPtr map_db_name;
} StsPar, PNTR StsParPtr;

/****************************************************************************/
/* STATIC FINCTIONS */
/****************************************************************************/

static Boolean GetSTSEntry(StsResultPtr result, 
                           FILE *fd, Boolean detailed);

static StsResultPtr PrintSTSHeader(StsResultPtr result, CharPtr label, 
                                   Boolean detailed, Boolean html);
static Boolean PrintSTSDetailes(StsResultPtr result, Boolean html);

static AccListPtr GetAccList(CharPtr buffer, Int4Ptr TotalItems);
static void WWWSendSTSPage(CharPtr WWWSequence, 
                           Boolean NetscapeOK);

static StsParPtr GetStsSearchParam(void);
static FastaSeqPtr NextFastaFromBuff(CharPtr p, CharPtr PNTR next);
static Int4 FilterNucSequence(CharPtr p);

static StsParPtr StsParNew(void);
static void StsParFree(StsParPtr param);
static StsParPtr STSReadCommandLine(void);

#define STS_WWW_DATABASE  "/web/public/htdocs/STS/DB/sts.db"  
#define ORG_WWW_DATABASE  "/web/public/htdocs/STS/DB/org.db"  
#define MAP_WWW_DATABASE  "/web/public/htdocs/STS/DB/sts.map" 

static Boolean IS_NOT_accession (CharPtr word)
{
    Int4 len, i;
    
    if(word == NULL)
        return TRUE;
    
    if((len = StringLen(word)) == 0)
        return TRUE;
    
    /* Testing, that this is 6 length accession */
    
    if(len == 6 && isalpha(word[0])) {
        for(i = 1; i < len; i++) {
            if(!isdigit(word[i]))
                break;
        }
        
        if (i == len) return FALSE; /* This is accession */   
    }
    
    if(len == 8 && isalpha(word[0])  && isalpha(word[1])) {
        for(i = 2; i < len; i++) {
            if(!isdigit(word[i])) {
                break;
            }
        }
        if (i == len) return FALSE; /* This is accession */
    }

    return TRUE;
}

Int2 Main(void)
{
    StsParPtr stsp;
    Int4 NumBadChar, StsCount;
    CharPtr inbuff;
    FastaSeqPtr fseq;
    CharPtr NextChar;
    StsResultPtr result= NULL, newresult= NULL, tmpresult=NULL;
    Int4 NumAcc;
    AccListPtr MainAccList, AccTmp; 
    STSDataPtr sts_data;
    STSDbNamesPtr db_name = NULL;
    
    putenv("USER=STSSearch");
    
    if((stsp = GetStsSearchParam()) == NULL) {
        printf("Error getting Search data. Exiting ...\n");
        exit(1);
    }
    
    if(stsp->html) {
        
        printf("Content-type: text/html\r\n\r\n"); 
        printf("<HTML>"
               "<BODY bgcolor=\"%s\" link=\"blue\" vlink=#4500A0>"
               "<TITLE> STS Match Program </TITLE>"
               "<map name=TitleMap>"
               "<area shape=rect coords=3,1,43,21 "
               "href=http://www.ncbi.nlm.nih.gov>"
               "<area shape=rect coords=381,1,426,21 "
               "href=http://www.ncbi.nlm.nih.gov/dbSTS/> "
               "<area shape=rect coords=431,1,476,21 "
               "href=http://www.ncbi.nlm.nih.gov/Entrez> "
               "<area shape=rect coords=481,1,500,21 "
               "href=/STS/About_ePCR.html>"
               "</map>"
               "<IMG SRC=\"/STS/pcr_res.gif\" BORDER=0 USEMAP=#TitleMap "
               "HEIGHT=22 WIDTH=500><BR>&nbsp;<BR>\n", BG_COLOR);
        fflush(stdout);
    }
    
#ifdef WWW_VERSION
    db_name = MemNew(sizeof(STSDbNames));
    db_name->sts_db_name = StringSave(STS_WWW_DATABASE);
    db_name->sts_map_name = StringSave(MAP_WWW_DATABASE);
    db_name->sts_org_name = StringSave(ORG_WWW_DATABASE);
#else
    if(stsp->sts_db_name != NULL || 
       stsp->map_db_name != NULL || stsp->org_db_name) {
        db_name = MemNew(sizeof(STSDbNames));
        db_name->sts_db_name = StringSave(stsp->sts_db_name);
        db_name->sts_map_name = StringSave(stsp->map_db_name);
        db_name->sts_org_name = StringSave(stsp->org_db_name);
    }
#endif
    
    if((StsCount = InitSTSSearch_r(db_name, &sts_data)) < 0) {
        printf("Cannot initiate STS Search ... Exiting ...\n");
        exit(1);
    } else if(stsp->html) {
        printf("<b>STS database initialized with %d sequences, "
               "please wait for results ...</b><BR><HR><PRE>", StsCount 
               );
        fflush(stdout);
    }
    
    stsp->organism = STSGetOrganismIndex(sts_data, stsp->orgname);
    
    if(db_name != NULL) {
        MemFree(db_name->sts_db_name);
        MemFree(db_name->sts_map_name);
        MemFree(db_name);
    }
    
    if(stsp->intype != FASTA_IN) { 
        
        if (! EntrezInit("STSSearch", FALSE, NULL)) {
            printf("Cannot initialize Entrez<BR>");
        }
        
        EntrezBioseqFetchEnable("STSSearch", TRUE);
    }

    printf(
"Location of STS              GenBank\n"
"within query                 accession\n"
"sequence          dbSTS id   number    Chromosome  STS marker name\n"
"================  ========  =========  ==========  ==============================\n\n");

    fflush(stdout);
    
    if(stsp->intype == FASTA_IN) {
        if(stsp->sequence[0] == NULLB) {
            printf("No sequence present in the search query.\n");
            exit(1);
        } else if(StringLen(inbuff = stsp->sequence) < MINSEQLEN) {
            printf("Length of entered sequence too small to start search\n");
            exit(1);
        }
        while(inbuff != NULL) {
            fseq = NextFastaFromBuff(inbuff, &NextChar);
            inbuff = NextChar;
            if((NumBadChar = FilterNucSequence(fseq->seq)) < 0) {
                printf("Error filtering nucleotide sequence\n");
                exit(1);
            } else if (NumBadChar >0 ) {
                printf("<b>WARNING!!!</b> %d bad characters found "
                       "in the input sequence\n\n", 
                       NumBadChar);
            }
            
            if(!STSSearch_r(sts_data, fseq->seq, stsp->organism, &newresult)) {
                printf("Error in STS Search. Exiting ...\n");
                return 1;
            }
            
            if(newresult != NULL) {
                tmpresult = newresult;
                newresult = PrintSTSHeader(newresult, fseq->label, 
                                           stsp->detailed, stsp->html);
                newresult->next = result;
                result = tmpresult;
            } else {
                printf("%s\n Did not return any hits ...\n\n", 
                       fseq->label);
                fflush(stdout);
            } 
            MemFree(fseq->label);
            MemFree(fseq->seq);
            MemFree(fseq);
        } /* while (inbuff != NULL) */
    } else  {  /* Type = Accession or GI */
        
        /* Here first we will fetch sequence from Entrez */
        
        MainAccList = GetAccList(stsp->sequence, &NumAcc);
        
        if(NumAcc == 0) {
            printf("<b>ERROR:</b> No valid gi/accessions found in input.<HR>"); 
            exit(1);
        } 
        
        /*    printf("Retrieved %d valid accessions/gis\n", NumAcc);  */
        
        for (AccTmp = MainAccList; AccTmp != NULL; AccTmp = AccTmp->next) {  
            if((fseq = AccessionToFasta(AccTmp->acc)) == NULL) {
                printf("<b>ERROR:</b> No record was found "
                       "for %s - skipping..</b>", 
                       AccTmp->acc);
                continue;
            }
            
            if(!STSSearch_r(sts_data, fseq->seq, stsp->organism, &newresult)) {
                printf("Error in STS Search. Exiting ...\n");
                return 1;
            }
            if(newresult != NULL) {
                tmpresult = newresult;
                newresult = PrintSTSHeader(newresult, fseq->label, 
                                           stsp->detailed, stsp->html);
                newresult->next = result;
                result = tmpresult;
            } else {
                printf("%s\n Did not return any hits ...\n\n", 
                       fseq->label);
                fflush(stdout);
            }
            
            MemFree(fseq->seq);
            MemFree(fseq->label);
            MemFree(fseq);
        }
    } 
    
    if(stsp->html)
        printf("<HR>\n");  
    
    /* If detailed output requested */
    
    if(result && stsp->detailed)
        PrintSTSDetailes(result, stsp->html);
    
    if(stsp->detailed || stsp->intype != FASTA_IN) {  
        EntrezBioseqFetchDisable();
        EntrezFini();
    }
    
    StsResultFree(result);
    
    STSDataFree(sts_data);
    
    StsParFree(stsp);
    return 0;
}
static StsResultPtr PrintSTSHeader(StsResultPtr result, CharPtr label, 
                                   Boolean detailed, Boolean html) 
{
  Char TmpBuff[1024];
  Int4 tmplen;
  register Int4 i;

  printf("%s\n", label);

  while (TRUE) {
    
    /* Now printing short header information line */

    if(detailed && html) {
      sprintf(TmpBuff, " <a href=#%08s>%d..%d</a> ", 
              result->acc, 
              result->pos,  result->pos+result->real_len-1 
              );
    } else {
      sprintf(TmpBuff, " %d..%d ", 
              result->pos,  result->pos+result->real_len-1
              );
    }
    printf("%s", TmpBuff);

    if(detailed && html)
      tmplen = 41 - StringLen(TmpBuff);
    else
      tmplen = 19 - StringLen(TmpBuff);
    for(i =0; i < tmplen; i++)
      printf(" ");
    
    if(html) {
      sprintf(TmpBuff, "<a href=\"http://www2.ncbi.nlm.nih.gov/cgi-bin/"
              "birx_by_acc?dbsts+%d\">%d</a>", 
              result->id_sts, result->id_sts);
    } else {
      sprintf(TmpBuff, "%d", result->id_sts);
    }

    printf("%s", TmpBuff);
    if(html)
      tmplen = 86 - StringLen(TmpBuff);
    else
      tmplen = 10 - StringLen(TmpBuff);

    for(i =0; i < tmplen; i++)
      printf(" ");
        
    if(html) {
      sprintf(TmpBuff, "<a href=\"http://www.ncbi.nlm.nih.gov/"
              "htbin-post/Entrez/query?form=6&dopt=g&db=n&"
              "uid=%s\">%s</a>",
              result->acc, result->acc);
    } else {
      sprintf(TmpBuff, "%s", result->acc);
    }
    
    printf("%s", TmpBuff);

    if(html)
      tmplen = 112 - StringLen(TmpBuff);
    else
      tmplen = 14 - StringLen(TmpBuff);

    for(i =0; i < tmplen; i++)
      printf(" ");

    sprintf(TmpBuff, "%s", 
            StringCmp(result->chrom, "0") ? result->chrom : " ");

    printf("%s", TmpBuff);

    tmplen = 9 - StringLen(TmpBuff);
    for(i =0; i < tmplen; i++)
      printf(" ");
    
    printf("%s\n", result->sts_name);
    
    fflush(stdout);
    if(result->next == NULL)
      break;
    result = result->next;
  }
  printf("\n"); 
  fflush(stdout);
  return result;
}

static Boolean PrintSTSDetailes(StsResultPtr result, Boolean html)
{
  register Int4 i;
  Int4 len1, len2, len_tot, len_end;
  Boolean ColorSet;
  
  printf("\n%sDetailed information...%s",
         html? "<b>"  : "",  
         html? "</b>\n\n" : ""
         );
  fflush(stdout);
  while(result) {

    if(html) {
      printf("<HR><a name=%08s>\n<h3>%s</h3>\n\n</a>", 
             result->acc, result->sts_name);
      
      printf("dbSTS id: <a href=\"http://www2.ncbi.nlm.nih.gov/cgi-bin/"
             "birx_by_acc?dbsts+%d\">%d</a>, "
             "GenBank Accession: "
             "<a href=\"http://www.ncbi.nlm.nih.gov/"
             "htbin-post/Entrez/query?form=6&dopt=g&db=n&"
             "uid=%s\">%s</a><BR>Organism: %s\n"
             "Primer1: <FONT color=\"%s\">%s</FONT>\n"
             "Primer2: <FONT color=\"%s\">%s</FONT>\n"
             "STS location: %d..%d Chromosome: %s\n"
             "Expected amplicon size: %d, Observed amplicon size: %d\n",
             result->id_sts, result->id_sts,
             result->acc, result->acc, 
             result->org,
             PRIMER1_COLOR, result->pr1,
             PRIMER2_COLOR, result->pr2,
             result->pos, result->pos+result->real_len, 
             StringCmp(result->chrom, "0") ? result->chrom : " ", 
             labs(result->exp_len), result->real_len
             );
    } else {
      printf("\n\n\n-------+= %s =+-------\n\n", 
             result->acc, result->sts_name);
      
      printf("dbSTS id: %d, GenBank Accession: %s\n"
             "Organism: %s\n"
             "Primer1: %s\n"
             "Primer2: %s\n"
             "STS location: %d..%d Chromosome: %s\n"
             "Expected amplicon size: %d, Observed amplicon size: %d\n",
             result->id_sts,
             result->acc,
             result->org,
             result->pr1,
             result->pr2,
             result->pos, result->pos+result->real_len-1, 
             StringCmp(result->chrom, "0") ? result->chrom : " ", 
             labs(result->exp_len), result->real_len
             );
    }
    
    fflush(stdout);
    printf("Primers match in %s orientation\n",
           result->exp_len > 0 ? "forward" : "backward");

    printf("\n%sQuery sequence:%s\n\n",
           html? "<b>"  : "",  
           html? "</b>" : ""
           );

    fflush(stdout);
    
    len1 = StringLen(result->pr1) + result->start;
    len2 = result->real_len - StringLen(result->pr2) + result->start; 
    len_tot = StringLen(result->sequence);
    len_end = result->real_len + result->start;
    
    printf("%6d  ", result->pos - result->start);
    fflush(stdout);
    ColorSet = FALSE;

    for(i = 0; i < len_tot; i++) {

      if(!(i%10) && i)
        printf(" ");

      if(i==result->start && html) {
        printf("<FONT color=\"%s\">",
               result->exp_len > 0 ? PRIMER1_COLOR : PRIMER2_COLOR); 
        ColorSet = (result->exp_len > 0 ? 1 : 2);
      }
      if(i == len_end && html) {
        printf("</FONT>");
        ColorSet = FALSE;
      }
      if(i == len1 && html) {
        printf("</FONT>"); 
        ColorSet = FALSE;
      }
      if(i == len2 && html) {
        printf("<FONT color=\"%s\">",
               result->exp_len > 0 ? PRIMER2_COLOR : PRIMER1_COLOR);       
        ColorSet = (result->exp_len > 0 ? 2 : 1);
      }
      if(!(i%60) && i) {
        if(ColorSet > 0 && html)
          printf("</FONT>\n%6d  <FONT color=\"%s\">", 
                 result->pos - result->start + i,
                 ColorSet == 1 ? PRIMER1_COLOR : PRIMER2_COLOR );
        else
          printf("\n%6d  ", result->pos - result->start + i );
      }
      printf("%c", result->sequence[i]);
    }
 
    fflush(stdout);

    result=result->next;
    fflush(stdout);
  }

  if(html) {
    printf("</PRE>");  
    for(i=0; i<25; i++)
      printf("<BR>");
  } else {
    printf("\n\n\n");
  }

  fflush(stdout);      
  return TRUE;
}
static AccListPtr GetAccList(CharPtr buffer, Int4Ptr TotalItems) 
{
  Char TmpBuff[256];
  register Int4 i, j, k;
  Int4 FileLen = 0;
  AccListPtr acclist = NULL;
  AccListPtr acclistTmp, acclistlast;
  Int4 NumInvalid = 0;
  
  *TotalItems = 0;
  
  if(buffer == NULL || buffer[0] == NULLB)
      return NULL;
  
  MemSet(TmpBuff, '\0', sizeof(TmpBuff));
  
  FileLen = StringLen(buffer);
  
  for(i = 0; i < FileLen; i++) {

    if(NumInvalid > 10) {
      printf("<b>ERROR  :</b> Too many invalid Gi/Accession numbers <BR>");
      return NULL;
    }

    if(isspace(buffer[i])) /* Rolling spaces */
      continue;

    j= 0;
    while (!isspace(buffer[i]) && j < 10 && i < FileLen) { 
      TmpBuff[j] = buffer[i];
      j++; i++;
    }
    TmpBuff[j] = NULLB;

    /* Now validating accession/gi */

    for(k =0; k < j; k++) {
      if(!isdigit(TmpBuff[k])) {
        break;
      }
    }
    if(k != j) { 
        if(IS_NOT_accession(TmpBuff)){
            printf("<b>WARNING:</b> Gi/Accession \"%s\" "
                   "is not valid<BR>", 
                   TmpBuff);
            NumInvalid++;
            continue;
        }
    }

    /* It we come here - we got valid text ID */

    if(acclist == NULL) { /* first element */
        acclist = (AccListPtr) MemNew(sizeof(AccList));
        acclistTmp = acclist;
        acclistTmp->acc = StringSave(TmpBuff);
        acclistTmp->next = NULL;
        acclistlast=acclistTmp;
        *TotalItems = *TotalItems +1; 
    } else {
        acclistTmp =  (AccListPtr) MemNew(sizeof(AccList));
        acclistlast->next = acclistTmp;
        acclistTmp->acc = StringSave(TmpBuff);
        acclistTmp->next = NULL;
        acclistlast = acclistTmp;
        *TotalItems = *TotalItems +1;
    }
  }
  return acclist;
}

static void WWWSendSTSPage(CharPtr WWWSequence, 
                           Boolean NetscapeOK)
{
  Int4 i;
  STSOrgPtr PNTR OrgTable;



  printf("Content-type: text/html\r\n\r\n"); 
  printf("<HTML>"
         "<BODY bgcolor=\"%s\">"
         "<TITLE> STS Match Program </TITLE>\n"
         "<map name=TitleMap>\n"
         "<area shape=rect coords=3,1,43,21 "
         "href=http://www.ncbi.nlm.nih.gov>\n"
         "<area shape=rect coords=381,1,426,21 "
         "href=http://www.ncbi.nlm.nih.gov:%s/dbSTS/> "
         "<area shape=rect coords=431,1,476,21 "
         "href=http://www.ncbi.nlm.nih.gov/Entrez> "
         "<area shape=rect coords=481,1,500,21 "
         "href=/STS/About_ePCR.html>"
         "</map>\n"
         "<IMG SRC=\"/STS/pcr.gif\" BORDER=0 USEMAP=#TitleMap "
         "HEIGHT=22 WIDTH=500>\n", BG_COLOR, getenv("SERVER_PORT"));
  
  if((OrgTable = STSGetOrgTable()) == NULL) {
    printf("ERROR: Cannot initiate Organism index "
           "Exiting...\n");
    exit(1);
  }    

  printf("<FORM ACTION=\"%s/result \" METHOD=\"POST\" "
         "NAME=\"STSTOOL\" %s>\n", 
         getenv("SCRIPT_NAME") != NULL ? getenv("SCRIPT_NAME") : "NOT_SET",
         NetscapeOK? "ENCTYPE=\"multipart/form-data\"" : "");

  printf("PCR-based sequence tagged sites (STSs) "
         "have been used as landmarks for "
         "construction of various types of genomic maps. "
         "Using \"electronic PCR\" (e-PCR), "
         "these sites can be detected in DNA sequences, "
         "potentially allowing their map "
         "locations to be determined. <BR><BR>\n");
         
  printf("Enter here your input data as \n"
         "<select name = \"INPUT_TYPE\"> \n" 
         "<option> Sequence in FASTA format \n"
         "<option> Accession or GI \n"
         "</select><BR><BR> \n"
         "<textarea name=\"SEQUENCE\" rows=6 cols=60>%s</textarea> \n",
         WWWSequence == NULL ? "" : WWWSequence);
  
  if(NetscapeOK) 
    printf("<BR>Or load your input data from file: "
           "<INPUT TYPE=\"file\" NAME=\"SEQFILE\"> \n");
  
  printf("<BR><BR>\n");

  printf("Retrieve STS from &nbsp;"
         "<select name = \"ORGANISM\"> \n");

  printf(" <option> All Organisms ");
  for (i=0; i < MAXORGNUM && OrgTable[i] != NULL; i++) {	
    printf(" <option> %s (%d) \n", 
           OrgTable[i]->string, OrgTable[i]->num);
  }
  
  printf("</select><BR><BR>\n");

  printf("Print detailed information "
         "<INPUT TYPE=\"checkbox\" NAME=\"DETAILED\" CHECKED><BR><BR>\n");
  
  printf("<INPUT TYPE=\"submit\"> \n"
         "<INPUT TYPE=\"reset\" VALUE=\"Clear input\"> \n"
         "<HR></FORM>\n"
         );

  printf("%c<ADDRESS>\n", LF);
  printf("Comments and suggestions to:"
         "&lt; \n<a href=\"mailto:info@ncbi.nlm.nih.gov\">"
         "info@ncbi.nlm.nih.gov\n"
         "</a> &gt; <BR> Credits to: \n"
         "<a href=\"mailto:shavirin@ncbi.nlm.nih.gov\">"
         "Sergei B. Shavirin</a>, \n"
         "<!-- <a href=\"http://www.ncbi.nlm.nih.gov/STS/shavirin.html\">"
         "Sergei B. Shavirin</a> -->\n"
         "<a href=\"mailto:schuler@ncbi.nlm.nih.gov\">"
         "Greg Schuler</a> and \n" 
         "<a href=\"mailto:carolyn@ncbi.nlm.nih.gov\">"
         "Carolyn Tolstoshev</a>\n");
  printf("</ADDRESS>\n");
}
#define NUMARGS 9

Args pcr_args[NUMARGS] = {
  {"Input type\n"
   "         0 - FASTA file \n"
   "         1 - List of Gi/Accession numbers",
   NULL, NULL,NULL,FALSE,'t',ARG_INT,0.0,0,NULL},
  {"Format of output  \n"
   "         0 - Text \n"
   "         1 - HTML   ",
   "0", NULL,NULL,FALSE,'f',ARG_INT, 0.0,0,NULL},
  { "File with FASTA entries or GI/Accession numbers", 
    "stdin", NULL, NULL, TRUE, 'i', ARG_FILE_IN, 0.0, 0, NULL},
  {"Print detailed information \n"
   "         0 - Short         \n"
   "         1 - Detailed        ",
   "0", NULL,NULL,TRUE,'d',ARG_INT, 0.0,0,NULL},
  { "GI/Accession number for quick look", 
    NULL, NULL, NULL, TRUE, 'u', ARG_STRING, 0.0, 0, NULL},
  {"STS database file name:",
   NULL, NULL,NULL,TRUE,'s',ARG_FILE_IN, 0.0,0,NULL},
  {"ORG database file name:",
   NULL, NULL,NULL,TRUE,'o',ARG_FILE_IN, 0.0,0,NULL},
  {"MAP database file name:",
   NULL, NULL,NULL,TRUE,'m',ARG_FILE_IN, 0.0,0,NULL},
  {"Logfile name:",
   "elecpcr.log", NULL,NULL,TRUE,'l',ARG_FILE_OUT, 0.0,0,NULL}
};

static StsParPtr STSReadCommandLine(void)
{
  StsParPtr stsp;
  FILE *fd;

  if((stsp = StsParNew()) == NULL)
      return NULL;
  
  if ( !GetArgs ("elecpcr", NUMARGS, pcr_args) ) {
      exit(1);
  }
  if (!ErrSetLog (pcr_args[8].strvalue)) {
    ErrShow();
  } else {
    ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);
  }

  stsp->sts_db_name = StringSave(pcr_args[5].strvalue);
  stsp->org_db_name = StringSave(pcr_args[6].strvalue);
  stsp->map_db_name = StringSave(pcr_args[7].strvalue);

  if(pcr_args[1].intvalue == 1)
    stsp->html = TRUE;
  else
    stsp->html = FALSE;
  
  if(pcr_args[3].intvalue == 1)
    stsp->detailed = TRUE;
  else
    stsp->detailed = FALSE;
  
  if(pcr_args[0].intvalue == 1)
    stsp->intype = ACC_IN;
  else
    stsp->intype = FASTA_IN;

  if(pcr_args[4].strvalue != NULL) {   /* Quick look ? */
    stsp->sequence = pcr_args[4].strvalue;
  } else {  /* Reading from file by default */
    if((fd = FileOpen(pcr_args[2].strvalue, "rb")) == NULL)
      return NULL;
    if((stsp->sequence = WWWReadFileInMemory(fd, 0, TRUE)) == NULL)
      return NULL;
  }
  return stsp;
}


static StsParPtr GetStsSearchParam(void) 
{

  WWWInfoPtr info;
  CharPtr chptr;
  StsParPtr stsp;
  FILE *fd;
  time_t time_now;
  CharPtr TimeNowStr;
  WWWErrorCode error;
  Boolean SequenceOK = FALSE;

  if((fd = FileOpen(LogFile, "a")) == NULL) {
      if((fd = FileOpen("/tmp/wwwsts.log", "a")) == NULL) {
          printf("Cannot open logfile. Exiting...\n ");
          exit(1);
      }
  }
      
  if((error = WWWReadPosting(&info)) != WWWErrOk)
      return NULL;
  
  /* First check type of call to the program */
  
  if(WWWGetMethod(info) == COMMAND_LINE) {

    /* Reading command line and create StsPar structure */
      WWWInfoFree(info);
      return(STSReadCommandLine());
  }
  
  if(WWWGetMethod(info) == WWW_GET) { 
      if(WWWGetNumEntries(info) == 0) {
          time_now = time(NULL);
          TimeNowStr = ctime(&time_now);
          TimeNowStr[24] = '\0'; 
          fprintf(fd, "\n%s|%s|%s|%s|%d",
                  TimeNowStr, WWWGetAddress(info), 
                  WWWGetHost(info), WWWGetAgent(info), 
                  0);
          fclose(fd);
          WWWSendSTSPage(NULL, (Boolean)(WWWGetBrowser(info) == NETSCAPE));
          exit(1);
      } else { /* Here is processing of link to Electronic PCR */
          
          if((stsp = StsParNew()) == NULL)
              return NULL;
          
          /* Sequence Accession or GI */
          
          stsp->intype = ACC_IN; 
          
          if((chptr = WWWGetValueByName(info, "ID")) != NULL) {
              stsp->sequence = StringSave(chptr);
          } else {
              time_now = time(NULL);
              TimeNowStr = ctime(&time_now);
              TimeNowStr[24] = '\0'; 
              fprintf(fd, "\n%s|%s|%s|%s|%d",
                      TimeNowStr, WWWGetAddress(info), 
                      WWWGetHost(info), WWWGetAgent(info), 
                      0);
              fclose(fd);
              WWWSendSTSPage(NULL, (Boolean)(WWWGetBrowser(info) == NETSCAPE));
              exit(1);
          }
          
          /* Detailed or short */
          
          if((chptr = WWWGetValueByName(info, "D")) != NULL) {
              if(!StringICmp(chptr, "0") ||
                 !StringICmp(chptr, "FALSE") ||
                 !StringICmp(chptr, "OFF") ||
                 !StringICmp(chptr, "NO") )
                  stsp->detailed = FALSE;
          } else {
              stsp->detailed = TRUE;
          }
          
          /* Text or HTML */
          
          if((chptr = WWWGetValueByName(info, "F")) != NULL) {
              if(!StringICmp(chptr, "0") ||
                 !StringICmp(chptr, "FALSE") ||
                 !StringICmp(chptr, "OFF") ||
                 !StringICmp(chptr, "TEXT") ||
                 !StringICmp(chptr, "NO") ) {
                  stsp->html = FALSE;
                  printf("Content-type: text/html\r\n\r\n"); 
                  printf("<BODY bgcolor=\"%s\"><PRE>", BG_COLOR);
                  printf("<TITLE> STS Match Program </TITLE>");
              }
          } else {
              stsp->html = TRUE;
          }
          return stsp;
          
      }  /* Link to Electronic PCR proccesing */
  }  /* Method == GET */
  
  if((stsp = StsParNew()) == NULL)
    return NULL;
 
#ifdef TEST
  printf("Content-type: text/html\r\n\r\n"); 
  info_data = (WWWInfoDataPtr) info;
  for(i=0; i < info_data->num_entries; i++) {
    printf("%s : %s\n <BR>%c", 
           info_data->entries[i]->name, 
           info_data->entries[i]->val, LF); 
  }
  exit(1);
#endif

  if((chptr = WWWGetValueByName(info, "ORGANISM")) != NULL)
      stsp->orgname = StringSave(chptr);

  if((chptr = WWWGetValueByName(info, "SEQUENCE")) != NULL) {
    if(chptr[0] != NULLB)
      SequenceOK = TRUE;
    stsp->sequence = StringSave(chptr);
  }
  if(((chptr = WWWGetValueByName(info, "SEQFILE")) != NULL) && 
     !SequenceOK) {
    stsp->sequence = StringSave(chptr);
  }

  if((chptr = WWWGetValueByName(info, "INPUT_TYPE")) != NULL) {
    if(StringStr(chptr, "Sequence")) {
      stsp->intype = FASTA_IN; 
    } else if(StringStr(chptr, "Accession")) {
      stsp->intype = ACC_IN; 
    } 
  }

  if((chptr = WWWGetValueByName(info, "DETAILED")) != NULL) {
    stsp->detailed = TRUE;
  }

  time_now = time(NULL);
  TimeNowStr = ctime(&time_now);
  TimeNowStr[24] = '\0'; 
  fprintf(fd, "\n%s|%s|%s|%s|%d|%d|%d",
          TimeNowStr, WWWGetAddress(info), 
          WWWGetHost(info), WWWGetAgent(info), 
          stsp->intype, 
          stsp->organism, 
          stsp->detailed);
  fclose(fd);

  WWWInfoFree(info);
  return stsp;
}
static StsParPtr StsParNew(void)
{
    StsParPtr param;
    param = (StsParPtr) MemNew(sizeof(StsPar));
    param->sequence = NULL;
    param->detailed = FALSE;
    param->intype = FASTA_IN; 
    param->html = TRUE;
    
    return param;
}

static void StsParFree(StsParPtr param) {
    MemFree(param->sequence);
    MemFree(param->sts_db_name);
    MemFree(param->org_db_name);
    MemFree(param->map_db_name);
    MemFree(param->orgname);

    MemFree(param);
}

static FastaSeqPtr NextFastaFromBuff(CharPtr p, CharPtr PNTR next)
{
  FastaSeqPtr fseq;
  CharPtr t;
  CharPtr str;
  Int4 i, SeqSize = INIT_BUFF_SIZE;

  *next = NULL;

  if(p == NULL | p[0] == NULLB)
    return NULL;

  t = p;

  while (isspace(*t)) /* Rolling spaces */
    *t++;

  fseq = (FastaSeqPtr) MemNew(sizeof(FastaSeq));
  fseq->label = NULL;
  fseq->seq = NULL;

  switch (*t) {
  case NULLB:
    return NULL;
  case '>':
    /* Reading label */
    str = (CharPtr) MemNew(SeqSize + 5);
    str[0] = NULLB;
    for(i=0; *t != NULLB; i++) {
      if((str[i] = *t) == '\n' || *t == '\r')
        break;
      if (i > SeqSize) {
        SeqSize = i + INIT_BUFF_SIZE;
        str = (CharPtr) Realloc(str, SeqSize + 5);
      }
      *t++;
    }
    str[i] = NULLB;
    fseq->label = StringSave(str);
    MemFree(str);

    /* Reading sequence */

    while (isspace(*t)) /* Rolling spaces */
      *t++;
    str = (CharPtr) MemNew(SeqSize + 5);
    str[0] = NULLB;

    for(i=0; *t != NULLB; i++) {
      if((str[i] = *t) == '>') {
        *next = t;
        break;
      }

      if (i > SeqSize) {
        SeqSize = i + INIT_BUFF_SIZE;
        str = (CharPtr) Realloc(str, SeqSize + 5);
      }
      *t++;
    }
    str[i] = NULLB;
    fseq->seq = StringSave(str);
    MemFree(str);
    break;
  default:

    /* Reading sequence */

    while (isspace(*t)) /* Rolling spaces */
      *t++;
    str = (CharPtr) MemNew(SeqSize + 5);
    str[0] = NULLB;

    for(i=0; *t != NULLB; i++) {
      if((str[i] = *t) == '>') {
        *next = t;
        break;
      }
      if (i > SeqSize) {
        SeqSize = i + INIT_BUFF_SIZE;
        str = (CharPtr) Realloc(str, SeqSize + 5);
      }
      *t++;
    }
    str[i] = NULLB;
    fseq->seq = StringSave(str);
    MemFree(str);
    break;
  }
  if(fseq->label == NULL)
    fseq->label = StringSave(">Your sequence");
  return fseq;
}

static Int4 FilterNucSequence(CharPtr p) 
{
  CharPtr t, s;
  Int4 len, i =0;
  Int4 NumBadChar=0;
  len = StringLen(p);
  
  s = p;

  for(t = p; i < len; i++){
    *p = toupper(*p);
    if (isalpha(*p)) {
      *t = *p;
      *t++;
    } else {
      if(!isspace(*p)) {
        NumBadChar++;
      }
    }
    *p++;
  }
  *t = NULLB;
  p = s;
  return NumBadChar;
}
