/* $Id: mmdbsrv.c,v 6.29 2005/01/12 22:49:13 vakatov Exp $
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
* File Name:  $RCSfile: mmdbsrv.c,v $
*
* Author:  Christopher Hogue
*
* Version Creation Date: 6 January 1997
*
* $Revision: 6.29 $
*
* File Description:
*        MMDB WWW-server 
*
* $Log: mmdbsrv.c,v $
* Revision 6.29  2005/01/12 22:49:13  vakatov
* /htbin-post/Taxonomy/wgetorg -> /Taxonomy/Browser/wwwtax.cgi
*
* Revision 6.28  2001/12/12 20:43:38  beloslyu
* change the name of ftp site to ftp.ncbi.nih.gov
*
* Revision 6.27  2001/01/25 23:02:56  lewisg
* get rid of content-disp, add mdlLvl and maxModels args
*
* Revision 6.26  2000/08/21 16:12:42  lewisg
* add save=asntext option
*
* Revision 6.25  2000/06/26 18:05:26  lewisg
* fix another hardcoded url
*
* Revision 6.24  2000/06/23 22:35:58  lewisg
* get rid of hardcoded urls
*
* Revision 6.23  2000/01/31 19:59:12  lewisg
* fix output of date in pdb header
*
* Revision 6.22  1999/10/13 20:13:18  zimmerma
* DZ: Removed use of temporary files - html ouput redirected to stdout
*
* Revision 6.21  1999/06/17 14:44:09  zimmerma
* Added static char variable for cvsId in order to use unix "what"
*
* Revision 6.20  1999/06/15 18:15:34  addess
* Initialized this_uid to 0 on line 1286
*
* Revision 6.19  1999/05/13 15:51:33  kimelman
* bugfix: memory deallocation fixed
*
* Revision 6.19  1999/05/13 15:51:33  kimelman
* bugfix: memory deallocation fixed
*
* Revision 6.18  1999/05/11 23:36:34  kimelman
* make StrToInt4 static
*
* Revision 6.17  1999/05/11 17:50:23  kimelman
* style
*
* Revision 6.16  1999/04/28 21:08:30  lewisg
* fixed double freeing of Biostruc
*
* Revision 6.15  1999/04/28 18:26:15  kimelman
* exit codes changed to 0
*
* Revision 6.14  1999/04/22 01:56:44  kimelman
* 1. pdb reference read completely from configurstion files
* 2. indentaiton
* 3. NcbiMimeAsn1Free(mime) commented out <-- lead to core
*    dump AFTER sending MIME (quick & dirty fix)
*
* Revision 6.13  1999/04/19 16:52:57  kimelman
* spelling fix
*
* Revision 6.12  1999/03/05 19:18:47  addess
* changed URL to link to RCSB structure explorer
*
* Revision 6.11  1998/11/06 16:25:31  addess
* added features for layer-1 and PDB TITLE records
*
 * Revision 6.10  1998/07/17  18:45:53  madej
 * Got rid of some unused URLs.
 *
 * Revision 6.9  1998/06/11  19:12:53  madej
 * Changes in a message type for Cn3D by Yanli.
 *
 * Revision 6.8  1998/04/16  17:51:38  madej
 * Tweak to print out author names in new/old format.
 *
 * Revision 6.7  1998/04/15  14:54:05  kimelman
 * 1. Make retrieval unifirm from open server and sql server.
 * 2. mmdbsrv retrival performance tuning:
 * 	- size of read-in buffers
 * 	- direct pdb2mmdb DB lookup instead of full loading pdb to mmdb translataion
 * 	  table
 * 3. cleaning mmdblocl.* and make.mmdbsrv in order to remove obsolete code
 *
* Revision 6.6  1998/04/03 20:25:24  kimelman
* PubStruct access code added to mmdbsrv
*
* Revision 6.5  1998/02/20 18:25:26  madej
* Fix using unlink to stop file dumps into /var/tmp.
*
 * Revision 6.4  1997/11/25  18:27:45  madej
 * Fix to save asn.1 files in binary.
 *
 * Revision 6.3  1997/11/05  19:05:16  madej
 * Updated comments for config file.
 *
 * Revision 6.2  1997/11/05  15:58:57  madej
 * Bug fix for tax names in multiple listings
 *
 * Revision 6.1  1997/10/31  15:29:57  madej
 * Major reformat of structure summary page, including taxonomy links.
 *
 * Revision 6.0  1997/08/25  18:11:28  madden
 * Revision changed to 6.0
 *
* Revision 1.2  1997/05/16 21:25:41  hogue
* Added checks with vast database entries before showing vast links.
*
 * Revision 1.1  1997/02/18  18:03:30  hogue
 * Initial revision
 *
*
* ==========================================================================
*/

/************** 
*mmdbsrv GET Useage:
*
*db=t&form=6&uid=1OMD&dopt=s  - PDB or MMDB-ids allowed
*db=t&form=6&uid=1OMD&Dopt=i  - launch Cn3D Mime-type - first UID only.
*db=t&uid=1234,2345,1254,1232&dopt=s - multiple structure summaries 
*db=t&uid=3INS,1234,1OMD&dopt=s  - mixed id types OK
*
*Optional,  but if wrong,  error: 
*        db=t    any other is an error message
*        form=6  4 is an error message - any other ignored
*	
*Required	
*	uid=(list of MMDB ids or PDB ids)
*
*Optional dopt=
*     's' Structure summary
*     'r' PDB file 
*     'k' Kinemage file
*     'i' ASN.1 file
*     Others - error; None assumes 's'
*
*Optional - Interpreted in Context:
*        complexity="Cn3D+Subset"  ONECOORDATOM,  1
*                   "Virtual+Bond+Model" ONECOORDRES,  1
*                   "Up+to+5+Models" ALLMDL,  5
*                   "Up+to+10+Models" ALLMDL,  10
*	            "All+Models" ALLMDL,  100
* 
*        KinemageColor="Molecule+Number" KIN_COLOR_NUMBER
*                      "Secondary+Structure"  KIN_COLOR_TYPE
*                      "Thermal+Factor" KIN_COLOR_TEMP
*                      "Element" KIN_COLOR_ATOM
*        KinemageRender=(integer)  - not seen on Struc Summary 
*        save="See"
*             "s" - saves
*             "m" MacSave format
*        header="no" same as save="s"
*        title="no" - omit title bar and bottom options
*        html="no" 
*/


/*******************************************
MMDBSRV MIRROR installation:

This describes what you need to run MMDBSRV:

1] YOUR MMDB DATA DIRECTORIES, e.g.:
...mmdb/mmdbdata/
...mmdb/vastdata/

These contain the MMDB data on your local filesystem,
as obtained from ftp://ftp.ncbi.nih.gov/mmdb/mmdbdata  
and updated regularly by MIRROR.  You can 
put them in the CGI-BIN or HTTP (HTML) directories
if you prefer. If you wish to implement a local
decompression/compression scheme see the source code mmdblocl.c.

2] YOUR CGI-BIN directory & URL, e.g.:
...cgi-bin/mmdb/
...cgi-bin/mmdb/logs/
...cgi-bin/mmdb/data/
http://123.456.789.10/cgi-bin/mmdb/

The first contains the compiled mmdbsrv program and the
config file, obtained from 
ftp://ftp.ncbi.nih.gov/mmdb/www/mmdbsrv
The second will contain the log files written out.
The third needs sshead.txt, sscode.txt, sstail.txt
NOTE - you must edit "sstail.txt"
in order to get the proper links to your local
copy of help documentation & server.

3] YOUR HTTP (HTML) directory & URL, e.g.:
...html/mmdb
...html/mmdb/mmdbgifs
http://123.456.789.10/mmdb/

The first contains the html files to launch mmdbsrv,
arrow.gif, strucsum.gif and any other gif files, and the
associated MMDB help files struchelp.html, cn3d.html,
strucinstall.html, vast.html, etc.  Master copies are kept
at ftp://ftp.ncbi.nih.gov/mmdb/www/http
You must hand edit or use scripts or SED to change links from 
NCBI's to your own in all these HTML files.

The second contains the GIF library as obtained from
the NCBI ftp site ftp://ftp.ncbi.nih.gov/mmdb/mmdbgifs

6] Your WWW-Server - make it aware of the following 
new MIME-types:
chemical/ncbi-asn1-binary  *.val
chemical/x-pdb  *.ent; *.pdb
chemical/x-kinemage *.kin
 

-----------------------------
7]
MMDBSRV gets its directory structure; link URLS and CGI call names 
from a config file.
Change these as required and make a config file.
Unix ".mmdbrc" or Win "mmdb.ini" from this example:

Don't change:
URLHelp, ENTREZurl, MMDBurl, PDBurl, 3DBurl 
unless there are broken external links to these.

-------------------- cut here --------------------
[MMDB]
;Database and Index required when local MMDB database is used.
; Database = ./Junk/
Database = /net/dorothy/cbb3/mmdb/data/
Index    = mmdb.idx

[MMDBSRV]
; File for configuring the MMDB server.
; Base Directory & URLs for HTML documents.
HTMLPath  = /net/vaster/usr/attic/httpd.local/htdocs/Structure/
URLBase   = http://inhouse.ncbi.nlm.nih.gov:6224/Structure/
URLHelp   = http://www.ncbi.nlm.nih.gov/Entrez/ 
;

; Base Directory for CGI-BIN code, data (gifs, html text) and logs.
CGIPath   = /net/vaster/usr/attic/httpd.local/cgi-bin/Structure/
URLcgi    = http://inhouse.ncbi.nlm.nih.gov:6224/cgi-bin/Structure/
CGIname   = mmdbsrv
DATApath  = ./data/
LOGpath   = ./logs/
;

; Base Directory for MMDB-gif set
GIFpath  = /net/vaster/usr/attic/httpd.public/htdocs/Structure/mmdbgifs/
GIFurl   = http://www.ncbi.nlm.nih.gov/Structure/mmdbgifs/
;

; URL's for links to Entrez, VAST.
ENTREZurl = http://www.ncbi.nlm.nih.gov/htbin-post/Entrez/query
VASTurl = http://inhouse.ncbi.nlm.nih.gov:6224/cgi-bin/Structure/vastsrv
MMDBurl = http://www.ncbi.nlm.nih.gov/Structure/
PDBurl = http://www.pdb.bnl.gov/index.html
TDBurl = http://www.pdb.bnl.gov/cgi-bin/opdbshort

; Location of gunzip function.
Gunzip = /usr/sbin/gunzip

;
MAILto   = info@ncbi.nlm.nih.gov
;
[VAST]
;Database required for local VAST fetches.
;Database = /net/vaster/usr/people6/madej/vastdata.binary/
Database = /net/dorothy/cbb3/vast/data/

; URL for link to taxonomy database (wgetorg for retrieval)
[TAXLINK]
TAXurl = http://www.ncbi.nlm.nih.gov/Taxonomy/Browser/wwwtax.cgi

-------------------- cut here --------------------
********************************************/
 
#include <ncbi.h>
#include <ncbiwww.h>
#include <accentr.h>
#include <ncbistr.h>
#include <miscutils.h>
#include <sys/resource.h>
#include <mmdbapi.h>
#include <mmdbdata.h>
#include <taxinc.h>
#include <sys/resource.h>

#include <lsqfetch.h>
#include <netentr.h>
#include <asnmime.h>
#include <objmime.h>

#ifdef OS_UNIX
#include <signal.h>
#include <unistd.h>
#define CPUTIME_MAX 120
#endif
  


#define NUM_COMPLEXITY_TYPES 6
static char *ComplexityDescriptions[] = 
{"Virtual Bond Model","All Atom Model","Up to 5 Models",
   "Up to 10 Models","All Models","Cn3D Subset"};

#define NUM_COLOR_TYPES 5
static char *ColorDescriptions[] =
{"","Molecule Number","Secondary Structure","Thermal Factor","Element"};

/* the config file DATApath is used for the location of these files */

#define HEADFILE "sshead.txt"
/* Structure Summary Header File */

#define TAILFILE "sstail.txt"
/* Structure Summary Tail File  */
 
#define CODEFILE "sscode.txt"
/* Structure Summary View Code */

#define CHAIN_CHOICE_NUM	1
#define ORG_CHOICE_NUM		6

/* some defines needed for the taxonomy code */

#define TAX_ID_CHOICE_NUM	0
#define MAX_CHAINS		128
#define MAX_TBUFF		8192

WWWInfoPtr 		info = NULL;

static Boolean GlobalTitles = TRUE;
static Boolean GlobalNonHtmlOutput = FALSE;

static Char HTMLPath[PATH_MAX];
static Char URLBase[PATH_MAX];
static Char URLHelp[PATH_MAX];

static Char CGIPath[PATH_MAX];
static Char URLcgi[PATH_MAX];
static Char CGIName[256];
static Char DATApath[PATH_MAX];
static Char LOGpath[PATH_MAX];

static Char ENTREZurl[PATH_MAX];
static Char VASTurl[PATH_MAX];
static Char MMDBurl[PATH_MAX];
static Char PDBurl[PATH_MAX];
static Char TDBurl[PATH_MAX];
static Char TAXurl[PATH_MAX];
static Char MAILto[256];
static Char MAILTO[PATH_MAX];
static Char ARROW[PATH_MAX];

static char* cvsId_ = "@(#)$Id: mmdbsrv.c,v 6.29 2005/01/12 22:49:13 vakatov Exp $";

/*****************************************************
 * WWWPrintFileData looks in the current CGI-BIN directory 
 *  or the "data" subdirectory for the data file.
 *  and prints it out to pFile
 */
 
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


static Boolean
StrToInt4 (CharPtr str, Int4Ptr longval)
{
  Nlm_Int2     i;
  Nlm_Int2     len;
  Nlm_Char     local [64];
  Nlm_Boolean  nodigits;
  long int     val;

  if (longval != NULL) {
    *longval = (Nlm_Int4) 0;
  }
  len = (Nlm_Int2) Nlm_StringLen (str);
  if (len == 0)
    return FALSE;

  nodigits = TRUE;
  for (i = 0; i < len; i++)
    {
      Nlm_Char     ch;
      
      ch = str [i];
      if ('0' <= ch && ch <= '9')
        nodigits = FALSE;
      else if ( !/*!!*/  (ch == ' ' || ch == '+' || ch == '-') || nodigits==FALSE)
        return FALSE;
    }
  if (nodigits)
    return FALSE;
  if (longval != NULL)
    {
      Nlm_StringNCpy (local, str, sizeof (local));
      if (sscanf (local, "%ld", &val) == 1)
        *longval = val;
    }
  return TRUE;
}

    
static Int4 CountUIDs(char *String)
{
  Int4 count = 0;

  for (;;)
    {
      String = SkipSet(String,", ");
      if (*String == NULLB)
	break;
      count++;
      String = SkipToSet(String,", ");
    }
  
  return(count);
}    

static Int4 ConvertMMDBUID(CharPtr pcString)  
{
    Int4 iUID;
    CharPtr pcTemp = NULL;
        
    if (pcString == NULL)
    return 0;
    
    iUID = 0;
    pcTemp = StringSave(pcString);
    CleanSpaces(pcTemp);
    if (!StrToInt4(pcTemp, &iUID))
      {
	   iUID = MMDBEvalPDB(pcTemp);
      }
    MemFree(pcTemp);
    return iUID; 
}

 


static void MakeUIDList(CharPtr pcString,  ValNodePtr PNTR ppvnUIDS)
{
  CharPtr Ptr = NULL;
  CharPtr Ptr2 = NULL;
  Char SavedChar = NULL;
  Int4 Uid = 0,  i;
  Int4Ptr Uids = NULL;

  
  Ptr = SkipSpaces(pcString);
  if (*Ptr == NULLB)
    return;

  while (*Ptr)
    {
      Ptr2 = SkipToSet(Ptr," ,");
      SavedChar = *Ptr2;
      *Ptr2 = NULLB;
      Uid = 0;
      Uid = ConvertMMDBUID(Ptr);
/* printf("{%s}[%ld]\n", Ptr,  Uid); */
      if (Uid != 0)
	{
          ValNodeAddInt(ppvnUIDS, 0, Uid);
        }
      *Ptr2 = SavedChar;
      Ptr = SkipSet(Ptr2," ,");
    }
 
  return;  
}


static Int4 GetOptionValue(CharPtr Value, Int4 NumElements, CharPtr *ElementText)
{
  Int4 count, Len;
  Int4 valnum;

  if (StrToInt4(Value,  &valnum))
    return(valnum);

  Len = StringLen(Value);
  for (count = 0; count < NumElements; count++)
    if (StrNICmp(ElementText[count],Value, Len) == 0)
      break;

  if (count == NumElements)
    return -1;

  return(count);
}



/* Output links from the protein and nucleotide chains to GenPept reports. */

#define EntrezLink	"<A HREF=\"%s?uid=%ld&form=6&db=%c&dopt=%c\">%s</A>"

/************ Added by Ken for Layer One PDB releases ************/

static void My_StringNCpy(CharPtr str1, CharPtr str2, Int4 len)
{
  StringNCpy(str1,str2,len);
  str1[len] = '\0';
} 

static Boolean IsLayerOne(ValNodePtr pvn)
{
  Boolean Layer = FALSE;
  CharPtr comment_txt;
  CharPtr LayerOne = {"This is Layer 1 Release"};
  CharPtr temp_string;
  Int4 len;
  
  while (pvn)
  {
     if (pvn->choice == BiostrucDescr_pdb_comment)
     {
        comment_txt = (CharPtr)pvn->data.ptrvalue;
        temp_string = StringSave(LayerOne);
        len = StringLen(LayerOne);
        My_StringNCpy(temp_string, &comment_txt[18], len);
        if (!StringICmp(temp_string, LayerOne))
        {
          Layer = TRUE;
          break;
        }
     }
     pvn = pvn->next;
   }
   
   return Layer;
}
/***** End of functions added by Ken ********************/
static void
PrintMolecularContents(PDNMS ModelStruc, FILE *gdf)
{
	PMSD pmsd;
	PDNMM pdnmm;
	PMMD pmmd;
	Int2 cnt;



	if (ModelStruc == NULL)
		return;

	/* protein chains */
	pmsd = (PMSD) ModelStruc->data.ptrvalue;

	for (pdnmm = pmsd->pdnmmHead, cnt = 0; pdnmm != NULL; pdnmm = pdnmm->next) {
		pmmd = (PMMD) pdnmm->data.ptrvalue;

		if ((pmmd->bWhat) & AM_PROT) {
			if (cnt++ == 0) {
				fprintf(gdf, "<TR>\n<TD VALIGN=TOP NOWRAP>");
				fprintf(gdf, "<strong>Protein Chains:</strong></TD>\n<TD>");

				if (pmmd->pcMolName[0] == ' ')
					fprintf(gdf, EntrezLink, ENTREZurl, (long) pmmd->iGi, 'p', 'g',
						"(single chain)");
				else
					fprintf(gdf, EntrezLink, ENTREZurl, (long) pmmd->iGi, 'p', 'g',
						pmmd->pcMolName);
			}
			else {
				fprintf(gdf, ", ");
				fprintf(gdf, EntrezLink, ENTREZurl, (long) pmmd->iGi, 'p', 'g',
					pmmd->pcMolName);
			}
		}
	}

	if (cnt > 0)
		fprintf(gdf, "</TD>\n</TR>\n");

	/* nucleotide chains */
	for (pdnmm = pmsd->pdnmmHead, cnt = 0; pdnmm != NULL; pdnmm = pdnmm->next) {
		pmmd = (PMMD) pdnmm->data.ptrvalue;

		if ((pmmd->bWhat) & (AM_DNA | AM_RNA)) {
			if (cnt++ == 0) {
				fprintf(gdf, "<TR>\n<TD VALIGN=TOP NOWRAP>");
				fprintf(gdf, "<strong>Nucleotide chains:</strong></TD>\n<TD>");

				if (pmmd->pcMolName[0] == ' ')
					fprintf(gdf, EntrezLink, ENTREZurl, (long) pmmd->iGi, 'n', 'g',
						"(single chain)");
				else
					fprintf(gdf, EntrezLink, ENTREZurl, (long) pmmd->iGi, 'n', 'g',
						pmmd->pcMolName);
			}
			else {
				fprintf(gdf, ", ");
				fprintf(gdf, EntrezLink, ENTREZurl, (long) pmmd->iGi, 'n', 'g',
					pmmd->pcMolName);
			}
		}
	}

	if (cnt > 0)
		fprintf(gdf, "</TD>\n</TR>\n");

} /* end PrintMolecularContents */



static void
PrintStructureInfo(PDNMS ModelStruc,  FILE *File, CharPtr tax_save)
{
   char Name[200];
   Int4 depyear, depday;
   PMSD pmsdThis = NULL;
   BiostrucSourcePtr pbssThis = NULL;
   ValNodePtr pvnThis = NULL;
   BiostrucHistoryPtr pbshThis = NULL;
   CharPtr pcAuthors = NULL;
   char Paragraph[10];
   
   if (!ModelStruc) 
     return;

   pmsdThis = (PMSD) ModelStruc->data.ptrvalue;
   pvnThis = ValNodeFindNext(pmsdThis->pbsBS->descr,NULL,BiostrucDescr_history);

   if (pvnThis) {
      pbshThis = (BiostrucHistoryPtr) pvnThis->data.ptrvalue;
      pbssThis = pbshThis->data_source;
   }

  /*****
  fprintf(File, "<font size=+2>MMDB Id: %ld&nbsp ", (long) pmsdThis->iMMDBid);
  *****/
  fprintf(File, "<font size=+2>");
  fprintf(File, "MMDB Id: <A HREF=\"%s%s?uid=%ld&form=6&db=t&Dopt=s\">%ld</A>&nbsp ",
	URLcgi, CGIName, (long) pmsdThis->iMMDBid, (long) pmsdThis->iMMDBid);
  fprintf(File, "PDB Id: <A HREF=\"%s=%s\">%s</a>\n", TDBurl,pmsdThis->pcPDBName, pmsdThis->pcPDBName);
  if (IsLayerOne(pmsdThis->pbsBS->descr))
    fprintf(File, " - Warning: <A HREF=\"http://www.pdb.bnl.gov/pdb-docs/what_is_LR.html\">Not validated!</A>\n");
  fprintf(File, "</font>");  
  fprintf(File, "<BR>\n<BR>\n");
  fprintf(File, "<TABLE>\n");
  PrintMolecularContents(ModelStruc, File);
  fprintf(File, "<TR>\n<TD VALIGN=TOP NOWRAP>");
  fprintf(File, "<strong>MEDLINE</strong>:</TD>\n<TD><A HREF=\"%s?db=t&form=6&uid=", ENTREZurl);
  fprintf(File, "%ld&Dopt=m\">PubMed</A></TD>\n</TR>\n", (long) pmsdThis->iMMDBid );

  if (tax_save != NULL)
	fprintf(File, "%s\n", tax_save);
  else if (pmsdThis->pcPdbSource) {
  	fprintf(File, "<TR>\n<TD VALIGN=TOP NOWRAP>");
	fprintf(File, "<strong>Source</strong>:</TD>\n<TD>%s</TD>\n</TR>\n", pmsdThis->pcPdbSource);
  }

  /* insert a blank row in the table, for spacing */
  fprintf(File, "<TR>\n<TD VALIGN=TOP NOWRAP> </TD>\n<TD><BR></TD>\n</TR>\n");
  pcAuthors =  AuthorListPDB(pmsdThis->pbsBS);

  if (pcAuthors) {
  	fprintf(File, "<TR>\n<TD VALIGN=TOP NOWRAP>");
	fprintf(File, "<strong>PDB Authors</strong>:</TD>\n<TD>%s</TD>\n</TR>\n", pcAuthors);
	fflush(File);
	MemFree(pcAuthors);
  }

  if (pbssThis) {
   	depyear = pbssThis->database_entry_date->data[1];
   	depday = pbssThis->database_entry_date->data[3];
  	fprintf(File, "<TR>\n<TD VALIGN=TOP NOWRAP>");
	fprintf(File, "<strong>PDB Deposition</strong>:</TD>\n<TD>%2d-%3s-%02d</TD>\n</TR>\n",
		(int) depday, NCBI_months[pbssThis->database_entry_date->data[2] - 1], (int) depyear%100);
  }

  if (pmsdThis->pcPdbClass) {
  	fprintf(File, "<TR>\n<TD VALIGN=TOP NOWRAP>");
	fprintf(File, "<strong>PDB Class</strong>:</TD>\n<TD>%s</TD>\n</TR>\n", pmsdThis->pcPdbClass);
  }
  
  fprintf(File, "<TR>\n<TD VALIGN=TOP NOWRAP>");
  pvnThis = ValNodeFindNext(pmsdThis->pGraphDescr,NULL,BiomolDescr_pdb_comment);
  if (pvnThis)
    fprintf(File, "<strong>PDB Title</strong>:</TD>\n<TD>%s</TD>\n</TR>\n", pmsdThis->pcChemName);
  else
    fprintf(File, "<strong>PDB Compound</strong>:</TD>\n<TD>%s</TD>\n</TR>\n", pmsdThis->pcChemName);
  /* insert a blank row in the table, for spacing */
  fprintf(File, "<TR>\n<TD VALIGN=TOP NOWRAP> </TD>\n<TD><BR></TD>\n</TR>\n");

  /* Note: The table continues with "Sequence Neighbors" and "Structure Neighbors". */
  return;

}



static void
PrintStructureView(PDNMS ModelStruc, FILE *File)
{
	PMSD pmsd;
      
        if (ModelStruc == NULL) return;         

	pmsd = (PMSD) ModelStruc->data.ptrvalue;
	fprintf(File, "<FORM METHOD=POST ACTION=\"%s%s\">\n", URLcgi, CGIName);
	fprintf(File, "<INPUT TYPE=HIDDEN NAME=uid VALUE=%ld>\n", (long) pmsd->iMMDBid);
	WWWPrintFileData(CODEFILE, File);
}



/* This routine creates the last two rows of the table started by PrintStructureInfo.
 * These last two rows contain the links for the "Sequence Neighbors" of the chains of
 * the structure, and for "Structure Neighbors" of the chains and domains of the
 * structure.  The first part of the code is very similar to PrintMolecularContents.
 * The 2nd part creates the links for the VAST structure neighbors.
 *
 * Note there are 3 different formats for Vast neighbor links, purely for cosmetics.
 */

#define VastLink1	"<A HREF=\"%s?uid=%ld&chaindom=%ld\">%s</A>"
#define VastLink2	"<A HREF=\"%s?uid=%ld&chaindom=%ld\">%s.%d</A>"
#define VastLink3	"<A HREF=\"%s?uid=%ld&chaindom=%ld\">%d</A>"

static void
PrintStructureLinks(PDNMS ModelStruc, FILE *gdf)
{
	PMSD pmsd;
	PDNMM pdnmm;
	PMMD pmmd;
	PDNMG pdnmg;
	PMGD pmgd;
	Int2 cnt, pflg;
	Int4 uid, ichn, idom, chndom, chndom0;



	if (ModelStruc == NULL)
		return;

	/* insert a blank row in the table, for spacing */
	fprintf(gdf, "<TR>\n<TD VALIGN=TOP NOWRAP> </TD>\n<TD> </TD>\n</TR>\n");

	/* protein chains */
	pmsd = (PMSD) ModelStruc->data.ptrvalue;

	for (pdnmm = pmsd->pdnmmHead, cnt = pflg = 0; pdnmm != NULL; pdnmm = pdnmm->next) {
		pmmd = (PMMD) pdnmm->data.ptrvalue;

		if ((pmmd->bWhat) & AM_PROT) {
			if ((pmmd->iResCount <= 1) || (pmmd->iGi <= 0))
				continue;

			if (cnt++ == 0) {
				pflg = 1;
				fprintf(gdf, "<TR>\n<TD VALIGN=TOP NOWRAP>");
				fprintf(gdf, "<strong>Sequence Neighbors:</strong></TD>\n<TD>");

				if (pmmd->pcMolName[0] == ' ')
					fprintf(gdf, EntrezLink, ENTREZurl, (long) pmmd->iGi, 'p', 'p',
						"(single chain)");
				else
					fprintf(gdf, EntrezLink, ENTREZurl, (long) pmmd->iGi, 'p', 'p',
						pmmd->pcMolName);
			}
			else {
				fprintf(gdf, ", ");
				fprintf(gdf, EntrezLink, ENTREZurl, (long) pmmd->iGi, 'p', 'p',
					pmmd->pcMolName);
			}
		}
	}

	/* nucleotide chains */
	for (pdnmm = pmsd->pdnmmHead; pdnmm != NULL; pdnmm = pdnmm->next) {
		pmmd = (PMMD) pdnmm->data.ptrvalue;

		if ((pmmd->bWhat) & (AM_DNA | AM_RNA)) {
			if ((pmmd->iResCount <= 1) || (pmmd->iGi <= 0))
				continue;

			if (cnt++ == 0) {
				fprintf(gdf, "<TR>\n<TD VALIGN=TOP NOWRAP>");
				fprintf(gdf, "<strong>Sequence Neighbors:</strong></TD>\n<TD>");

				if (pmmd->pcMolName[0] == ' ')
					fprintf(gdf, EntrezLink, ENTREZurl, (long) pmmd->iGi, 'n', 'n',
						"(single chain)");
				else
					fprintf(gdf, EntrezLink, ENTREZurl, (long) pmmd->iGi, 'n', 'n',
						pmmd->pcMolName);
			}
			else {
				fprintf(gdf, ", ");
				fprintf(gdf, EntrezLink, ENTREZurl, (long) pmmd->iGi, 'n', 'n',
					pmmd->pcMolName);
			}
		}
	}

	if (pflg)
		fprintf(gdf, "</TD>\n</TR>\n");

	if (pmsd->pdnmlModels == NULL) {
		/* end the table */
		fprintf(gdf, "</TABLE>\n<BR>\n");
		return;
	}

	/* VAST neighbors: loop over protein chains */
	uid = (Int4) pmsd->iMMDBid;

	for (pdnmm = pmsd->pdnmmHead, cnt = ichn = 0; pdnmm != NULL; pdnmm = pdnmm->next) {
		pmmd = (PMMD) pdnmm->data.ptrvalue;
		ichn++;

		if ((pmmd->bWhat) & AM_PROT) {
			if ((pmmd->iResCount <= 1) || (pmmd->iGi <= 0))
				continue;

			chndom0 = 10000*uid + 100*(Int4) ichn;

			if (cnt++ == 0) {
				fprintf(gdf, "<TR>\n<TD VALIGN=TOP NOWRAP>");
				fprintf(gdf, "<strong>Structure Neighbors:</strong></TD>\n<TD>");

				if (pmmd->pcMolName[0] == ' ')
					fprintf(gdf, VastLink1, VASTurl, (long) uid, (long) chndom0,
						"(single chain)");
				else
					fprintf(gdf, VastLink1, VASTurl, (long) uid, (long) chndom0,
						pmmd->pcMolName);
			}
			else {
				fprintf(gdf, ", ");
				fprintf(gdf, VastLink1, VASTurl, (long) uid, (long) chndom0,
					pmmd->pcMolName);
			}

			/* now loop over domains */
			idom = 0;

			for (pdnmg = pmmd->pdnmgHead; pdnmg != NULL; pdnmg = pdnmg->next) {
				pmgd = (PMGD) pdnmg->data.ptrvalue;

				if (pmgd->iDomain > idom) {
					idom = (Int4) pmgd->iDomain;
					chndom = chndom0 + idom;
					fprintf(gdf, ", ");

					if (pmmd->pcMolName[0] == ' ')
						fprintf(gdf, VastLink3, VASTurl, (long) uid,
							(long) chndom, (int) idom);
					else
						fprintf(gdf, VastLink2, VASTurl, (long) uid,
							(long) chndom, pmmd->pcMolName, (int) idom);
				}
			}
		}
	}

	/* end the table */
	fprintf(gdf, "</TD>\n</TR>\n</TABLE>\n");

} /* end PrintStructureLinks */



/* Utility routine needed below.  Return a vector marking the distinct integers
 * appearing in the input array.  A naive algorithm is used since we never expect
 * the array size 'n' to be large.  If an entry vec[i] is 0, this means it is
 * a duplicate.  If vec[i] > 0 then this is the first occurrence of the number,
 * and the value vec[i] is the number of times the corresponding number nums[i]
 * appears.
 */

static void
mark_distinct(Int4Ptr nums, Int2Ptr vec, Int2 n)
{
	Int2 i, j, found;

	if (n == 0)
		return;

	/* initialize the vector */
	for (i = 0; i < n; i++)
		vec[i] = 1;

	for (i = 1; i < n; i++) {
		for (j = 0; j < i; j++) {
			if (nums[i] == nums[j]) {
				vec[i] = 0;
				vec[j]++;
				break;
			}
		}
	}

} /* end mark_distinct */



/* Output the taxonomy data, per chain, and also links to the taxonomy database.
 * Return '1' in case there was a non-trivial taxonomy assignment, '0' if there
 * was no taxonomy data found.  The success or failure is passed ultimately to
 * PrintStructureInfo, where the PDB source is displayed in case no taxonomy data
 * was found.
 */

static Int2
SaveTaxonomyInfo(BiostrucPtr bsp, CharPtr save, Int2 n, CharPtr pname)
{
	MoleculeGraphPtr mgp;
	Int4 i, j, k, cnt, txcnt, uacnt;
	ValNodePtr descr, db;
	CharPtr chainp, name, names[MAX_CHAINS];
	Char chn, chns[MAX_CHAINS], org_names[MAX_TBUFF];
	Int2 flg, name_here, cnt_vec[MAX_CHAINS];
	BioSourcePtr bios;
	OrgRefPtr org;
	Int4 molecule_type, tax_id, tax_ids[MAX_CHAINS];



	if ((bsp == NULL) || (save == NULL))
		return 0;

	for (i = 0; i < MAX_TBUFF; i++)
		org_names[i] = '\0';

	name_here = 0;
	mgp = bsp->chemical_graph->molecule_graphs;

	for (cnt = uacnt = 0; (mgp != NULL) && (cnt < MAX_CHAINS); mgp = mgp->next) {
		/* zap the OrgRefPtr, there may be no taxonomy for this chain! */
		org = NULL;

		for (descr = mgp->descr; descr != NULL; descr = descr->next) {
			if (descr->choice == BiomolDescr_molecule_type)
				molecule_type = descr->data.intvalue;

			if (descr->choice == BiomolDescr_name) {
				chainp = descr->data.ptrvalue;
				chn = *chainp;
			}

			if (descr->choice == BiomolDescr_organism) {
				bios = descr->data.ptrvalue;
				org = (OrgRefPtr) bios->org;
			}
		}

		if ((molecule_type == Molecule_type_protein) ||
			(molecule_type == Molecule_type_dna) ||
			(molecule_type == Molecule_type_rna)) {

			/* save the chain and get the taxonomy data */
			chns[cnt] = (chn == ' ') ? '\n' : chn;

			if (org != NULL) {
				DbtagPtr dbtag;
				ObjectIdPtr object_id;

				for (db = org->db, tax_id = -1; db != NULL; db = db->next) {
					if (db->choice == TAX_ID_CHOICE_NUM) {
						dbtag = db->data.ptrvalue;
						tax_id = dbtag->tag->id;
						break;
					}
				}

				names[cnt] = &org_names[name_here];

				if (org->taxname != NULL) {
					StringCat(names[cnt], org->taxname);
					name_here += StrLen(org->taxname) + 1;
				}
				else if (org->common != NULL) {
					StringCat(names[cnt], org->common);
					name_here += StrLen(org->common) + 1;
				}
				else {
					StringCat(names[cnt], "(unassigned)");
					name_here += 13;
				}

				tax_ids[cnt++] = tax_id;
			}
			else {
				/* track the number of unassigned chains */
				uacnt++;
				names[cnt] = &org_names[name_here];
				StringCat(names[cnt], "(unassigned)");
				name_here += 13;
				tax_ids[cnt++] = -1;
			}
		}
	}

	/* zap the save area */
	for (i = 0; i < n; i++)
		save[i] = '\0';

	/* flag the taxonomy section */
	sprintf(save, "<TR>\n<TD VALIGN=TOP NOWRAP><strong>Taxonomy</strong>:</TD>\n");

	if (cnt == 0) {
		k = StrLen(save);
		sprintf(&save[k], "<TD>(unassigned)</TD>\n");
		return 1;
	}

	/* group chains by organism type, i.e. tax id */
	mark_distinct(tax_ids, cnt_vec, cnt);

	for (i = flg = 0; i < cnt; i++) {
		if (cnt_vec[i] == 0)
			continue;

		tax_id = tax_ids[i];
		k = StrLen(save);

		if (flg)
			sprintf(&save[k], "; ");
		else {
			sprintf(&save[k], "<TD>");
			flg = 1;
		}

		/* collect chains with the same tax id */
		txcnt = cnt_vec[i];
		k = StrLen(save);

		/*****
		if (txcnt > 1)
			sprintf(&save[k], "chains ");
		else if (chns[i] != '\n')
			sprintf(&save[k], "chain ");
		*****/

		for (j = i; (j < cnt) && (txcnt > 0); j++) {
			if (tax_ids[j] != tax_id)
				continue;

			k = StrLen(save);

			if (txcnt-- > 1)
				sprintf(&save[k], "%c, ", chns[j]);
			else
				sprintf(&save[k], "%c", chns[j]);
		}

		k = StrLen(save);

		if (tax_id > 0) {
			if (chns[i] != '\n')
				sprintf(&save[k], "&nbsp <A HREF=\"");
			else
				sprintf(&save[k], "<A HREF=\"");

			k = StrLen(save);
			sprintf(&save[k], TAXurl, tax_id);
			k = StrLen(save);
			sprintf(&save[k], "\">%s</A> ", names[i]);
		}
		else
			sprintf(&save[k], " (unassigned)");
	}

	k = StrLen(save);
	sprintf(&save[k], "</TD>\n</TR>\n");
	return 1;

} /* end SaveTaxonomyInfo */



/* Old version of DumpMime, preserved for compatibility with Cn3D v1.0. */

static void DumpMime_v1(AsnIoPtr aip, CharPtr title, Int2 style, Int2 choice, VoidPtr datum)
{
  NcbiMimeAsn1Ptr mime;
  EntrezGeneralPtr egp;

  mime = (NcbiMimeAsn1Ptr) ValNodeNew(NULL);
  mime->choice = NcbiMimeAsn1_entrez;
  egp = EntrezGeneralNew();
  mime->data.ptrvalue = (VoidPtr) egp;
  egp->Data_data = ValNodeNew(NULL);
  egp->title = StringSave(title);
  egp->style = style;
  egp->Data_data->choice = choice;
  egp->Data_data->data.ptrvalue = datum;

  NcbiMimeAsn1AsnWrite(mime, aip, NULL);
  AsnIoFlush(aip);

  egp->Data_data->data.ptrvalue = NULL; /* for clean free */
  NcbiMimeAsn1Free(mime);

} /* end DumpMime_v1 */



                                   /* yanli */
static void DumpMime(AsnIoPtr aip, CharPtr title, VoidPtr datum)
{
  NcbiMimeAsn1Ptr mime;
  EntrezGeneralPtr egp;

  mime = (NcbiMimeAsn1Ptr) ValNodeNew(NULL);
  mime->choice = NcbiMimeAsn1_strucseq;
  mime->data.ptrvalue= datum;

  NcbiMimeAsn1AsnWrite(mime, aip, NULL);
  AsnIoFlush(aip);

  mime->data.ptrvalue=NULL ; /* for clean free */
  NcbiMimeAsn1Free(mime);
}



#define LAUNCH_VIEWER	0
#define SEE_FILE	1
#define SAVE_FILE	2
#define SEND_FILE   4

/* Handle the various View/See/Save file options. */

static Int4
SendStructureMIME(Char Filetype, Int4 uid, Int4 Mime, Int4 Complexity,
		  Int4 Models, Int4 Color, Int4 Render)
{
  BiostrucPtr bsp;
  PDNMS ModelStruc;
  Int4 count, i4Sent = 0, error_flag = 0;
  AsnIoPtr aip;
        
  /* yanli start */
  NcbiMimeAsn1Ptr mime;
  BiostrucSeqPtr bssp;
  BiostrucGraphPtr bsgp;
  MoleculeGraphPtr mgp;
  SeqIdPtr sip;
  SeqEntryPtr sep;
  Int2 retcode = 3;
  Int4 This_uid = 0;

  if ((bsp = MMDBBiostrucGet((DocUid) uid, Complexity, Models)) == NULL) {
    printf("Content-type: text/html\r\n\r\n");
    printf("<h2>Error</h2>\n");
    printf("Structure with UID = %ld failed to fetch on Server<p>\n", (long) uid);
    printf("No such structure available on this MMDB server.\n");
    return 0;
  }

  /*yanli*/
  bsgp = bsp->chemical_graph;
  mgp = bsgp->molecule_graphs;
  sip = mgp->seq_id;
  /*  This_uid =  EntrezFindSeqId(sip);    */
  while(sip != NULL){
    if(sip->choice == 12) This_uid = sip->data.intvalue;
    sip = sip->next;
  }
  sep = EntrezSeqEntryGet(This_uid, retcode);
     
  bssp = BiostrucSeqNew();
  bssp->structure = bsp;
  ValNodeLink(&(bssp->sequences), sep);     
        
  mime = (NcbiMimeAsn1Ptr) ValNodeNew(NULL);    /* yanli */
  mime->choice = NcbiMimeAsn1_strucseq;
  mime->data.ptrvalue = bssp;
  /*---yanli*/
        
  /* the following headers are format-independent */
  switch (Mime) {
  case SAVE_FILE:
    printf("Content-type: application/octet-stream\r\n\r\n");
    break;
  case SEE_FILE:
    printf ("Content-type: text/html\r\n\r\n");
    printf ("<HTML><PRE>\r\n");
    break;
  case SEND_FILE:
    printf ("Content-type: text/html\r\n\r\n");
    break;
  }

  if (Filetype == 'j') {
    /* Cn3D v2.0 asn.1 format */
    switch (Mime) {
    case LAUNCH_VIEWER:
      printf ("Content-type: chemical/ncbi-asn1-binary\r\n\r\n");
      aip = AsnIoNew(ASNIO_BIN_OUT, stdout, NULL, NULL, NULL);
      DumpMime(aip, "MIME", bssp);    
      break;
    case SAVE_FILE:
      aip = AsnIoNew(ASNIO_BIN_OUT, stdout, NULL, NULL, NULL);   
      NcbiMimeAsn1AsnWrite(mime, aip, NULL);
      break;
    case SEE_FILE:
      aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);
      NcbiMimeAsn1AsnWrite(mime, aip, NULL);
      break;
    case SEND_FILE:
      aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);
      NcbiMimeAsn1AsnWrite(mime, aip, NULL);
      break;
    }

    AsnIoReset(aip);
    AsnIoClose(aip);
  }
  else if (Filetype == 'i') {
    /* Cn3D v1.0 asn.1 format.  Included for backwards compatibility, in case users
     * want to stick with their old Cn3D for some reason.
     */
    switch (Mime) {
    case LAUNCH_VIEWER:
      printf ("Content-type: chemical/ncbi-asn1-binary\r\n\r\n");
      /* printf ("Content-disposition: filename=\"%d.val\"\r\n\r\n", uid);*/
      aip = AsnIoNew(ASNIO_BIN_OUT, stdout, NULL, NULL, NULL);
      DumpMime_v1(aip, "MIME", Entrez_style_report, Data_data_structure, bsp);
      break;
    case SAVE_FILE:
      aip = AsnIoNew(ASNIO_BIN_OUT, stdout, NULL, NULL, NULL);
      BiostrucAsnWrite(bsp, aip, NULL);
      break;
    case SEE_FILE:
      aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);
      BiostrucAsnWrite(bsp, aip, NULL);
      break;
    case SEND_FILE:
      aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);
      BiostrucAsnWrite(bsp, aip, NULL);
      break;
    }
    
    AsnIoReset(aip);
    AsnIoClose(aip);
  }
  else if (Filetype == 'r') {
    /* RasMol format */
    if (Mime == LAUNCH_VIEWER)
      printf ("Content-type: chemical/x-pdb\r\n\r\n");
    /*      printf ("Content-disposition: filename=\"%d.pdb\"\r\n\r\n", uid);*/
    
    ModelStruc = MakeAModelstruc(bsp);
    bssp->structure = NULL;  /* already linked into modelstruc */
    WritePDBAllModel(ModelStruc, stdout);
    ClearStructures();
  }
  else if (Filetype == 'k') {
    /* Mage format */
    if (Mime == LAUNCH_VIEWER)
      printf ("Content-type: chemical/x-kinemage\r\n\r\n");
    /* printf ("Content-disposition: filename=\"%d.kin\"\r\n\r\n", uid);*/
    
    ModelStruc = MakeAModelstruc(bsp);
    bssp->structure = NULL;  /* already linked into modelstruc */
    WriteKinAllModel(ModelStruc, stdout, Color, Render);
    ClearStructures();
  }
  else {
    error_flag = 1;
    printf("Content-type: text/html\r\n\r\n");
    printf("<h2>Error</h2>\n");
    printf("Unknown file type on structure fetch.<p>\n");
    printf("Contact %s\n", MAILTO);
  }
  
  if ((Mime == SEE_FILE) && (!error_flag))
    printf ("</PRE></HTML>\r\n");
  
  NcbiMimeAsn1Free(mime);           /* yanli */
  return 1;
  
} /* end SendStructureMIME */


static Int4  SendSummaryPage(Int4Ptr Uids, Int4 NumToDisplay, Int4 Save)
{
  BiostrucPtr bsp;
  ValNodePtr descr;
  PDNMS ModelStruc;
  Int4 uid, count, i4Sent = 0;
  Int2 taxret;
  Char tax_save[MAX_TBUFF];
  FILE *sendfile;
   
  if (Save == 2)
    printf ("Content-type: application/octet-stream\n\n");  
  else
    printf ("Content-type: text/html\n\n");  
     
  if (!GlobalNonHtmlOutput) {
    if (GlobalTitles) 
      WWWPrintFileData(HEADFILE,  stdout);
    else
      printf ( "<html>\n<title>MMDB Structure Summary</title><body>"); 
  }
  for (count = 0; count < NumToDisplay; count++)
    {
      uid = Uids[count];
      bsp = MMDBBiostrucGet((DocUid) uid,3,0);
      if (bsp == NULL)
	{
	  printf ("%sNo structure found for UID %ld.%s\n",
		  GlobalNonHtmlOutput ? "\n" : "<P><h4>",uid,
		  GlobalNonHtmlOutput ? "\n" : "</h4><P>");
	  if (!GlobalNonHtmlOutput) 
	    printf ("<HR SIZE=5 NOSHADE><BR>\n");
	  continue;
	}

      if (!GlobalNonHtmlOutput) {
	CharPtr name;

	for (descr = bsp->descr, name = NULL; descr != NULL; descr = descr->next) {
	  if (descr->choice == BiomolDescr_name) {
	    name = descr->data.ptrvalue;
	    break;
	  }
	}
	taxret = SaveTaxonomyInfo(bsp, tax_save, MAX_TBUFF, name);
      }

      ModelStruc = MakeAModelstruc(bsp);  
      PrintStructureInfo(ModelStruc, stdout, (taxret) ? tax_save : NULL);

      if (!GlobalNonHtmlOutput) /* Rest of View/Save stuff */
	{
	  PrintStructureLinks(ModelStruc, stdout);
	  PrintStructureView(ModelStruc,  stdout);
	  printf ("<HR SIZE=5 NOSHADE><BR>\n");
	}
	 
      ClearStructures();
    }
  if (!GlobalNonHtmlOutput)
    if (GlobalTitles)
      WWWPrintFileData(TAILFILE, stdout);
    else
      printf ("</html>");
  
  return 1;
} /* SendSummaryPage() */


static void SendGetNetscape(void) {

  printf("HTTP/1.0 200 OK\r\n");
  printf("MIME-Version: 1.0\r\n");
  printf("Content-type: text/html\r\n\r\n"); 
  printf("<BR><BR><B>Sorry, you need a <a href=\"http://home.netscape.com/\">",
         "Netscape</a> version 2 or higher WWW-browser "
         "to be able to enter this page</B><BR>\n\n");
}  /* SendGetNetscape */




static Boolean InitMMDBSrv(void)
{
  Int2 i;

  HTMLPath[0] = '\0';
  URLBase[0] = '\0';
  URLHelp[0] = '\0';
  CGIPath[0] = '\0';
  URLcgi[0] = '\0';
  CGIName[0] = '\0';
  DATApath[0] = '\0';
  LOGpath[0] = '\0';
  ENTREZurl[0] = '\0';
  VASTurl[0] = '\0';
  MMDBurl[0] = '\0';
  PDBurl[0] = '\0';
  TDBurl[0] = '\0';
  MAILto[0] = '\0';

  for (i = 0; i < PATH_MAX; i++)
    TAXurl[i] = '\0';

  FindPath("mmdb", "MMDBSRV", "HTMLPath", HTMLPath, PATH_MAX);
  if (HTMLPath[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no HTMLPath...\n");
	return FALSE;
    } 
  GetAppParam("mmdb", "MMDBSRV", "URLBase", "", URLBase, PATH_MAX);
  if (URLBase[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no URLBase...\n");
	return FALSE;
    }
  GetAppParam("mmdb", "MMDBSRV", "URLHelp", "", URLHelp, PATH_MAX);
  if (URLHelp[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no URLHelp...\n");
	return FALSE;
    }
  FindPath("mmdb", "MMDBSRV", "CGIPath", CGIPath, PATH_MAX);
  if (CGIPath[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no CGIPath...\n");
	return FALSE;
    }
    
  GetAppParam("mmdb", "MMDBSRV", "URLcgi", "", URLcgi, PATH_MAX);
  if (URLcgi[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no URLcgi...\n");
	return FALSE;
    }
  GetAppParam("mmdb", "MMDBSRV", "CGIName", "", CGIName, (size_t) 256 *  sizeof(char));
  if (CGIName[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no CGIName...\n");
	return FALSE;
    }
  FindPath("mmdb", "MMDBSRV", "DATApath", DATApath, PATH_MAX);
  if (DATApath[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no DATApath...\n");
	return FALSE;
    }
  ARROW[0] = '\0';
  sprintf(ARROW, "<IMG alt=\"o\" SRC=\"%sarrow.gif\" ALT=\"o\" BORDER=0>\n", URLBase);

  FindPath("mmdb", "MMDBSRV", "LOGpath", LOGpath, PATH_MAX);
  if (LOGpath[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no LOGpath...\n");
	return FALSE;
    }
  GetAppParam("mmdb", "MMDBSRV", "ENTREZurl", "", ENTREZurl, PATH_MAX);
  if (ENTREZurl[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no ENTREZurl...\n");
	return FALSE;
    } 
  GetAppParam("mmdb", "MMDBSRV", "VASTurl", "", VASTurl, PATH_MAX);
  if (VASTurl[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no VASTurl...\n");
	return FALSE;
    }
  GetAppParam("mmdb", "MMDBSRV", "MMDBurl", "", MMDBurl, PATH_MAX);
  if (MMDBurl[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no MMDBurl...\n");
	return FALSE;
    }
  GetAppParam("mmdb", "MMDBSRV", "PDBurl", "", PDBurl, PATH_MAX);
  if (PDBurl[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no PDBurl...\n");
	return FALSE;
    }
  GetAppParam("mmdb", "MMDBSRV", "TDBurl", "", TDBurl, PATH_MAX);
  if (TDBurl[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no TDBurl...\n");
	return FALSE;
    }  
  GetAppParam("mmdb", "TAXLINK", "TAXurl", "", TAXurl, PATH_MAX);
  if (TAXurl[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nTAXLINK section no TAXurl...\n");
	return FALSE;
    }
  StringCat(TAXurl, "?id=%d&lvl=0");
  GetAppParam("mmdb", "MMDBSRV", "MAILto", "", MAILto, PATH_MAX);
  if (MAILto[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no Mailto...\n");
	return FALSE;
    }

  MAILTO[0] = '\0';
  sprintf(MAILTO, "&lt <a href=\"%s\">%s</a> &gt",  MAILto,  MAILto);

  return TRUE;
}

   

/************************************************************************
 *
 *  Int2 Main() - main function     
 *
 ***********************************************************************/



Int2 Main ()
{
  Boolean 		Netscape2_OK = FALSE;
  time_t 		time_now;
  CharPtr 		TimeNowStr = NULL;
  WWWErrorCode 		error;
  Int4                  IndexArgs = -1;
  Int4 			Complex = 0;  
  Int4 			MaxModels = 1;
  Int4 			KinColor = 0;
  Int4 			KinRender = 0;
  Int4 			Save = 0;
  Char 			DispOpt = '\0';
  Int4                  Form = 0;
  CharPtr 		pcThis = NULL,  pcName = NULL;
  CharPtr 		Uids;  /* (list of MMDB ids or PDB ids) */
  Int4Ptr 		MMDBids = NULL; /* (list of MMDB ids ) */
  Int4                  iMMDBids = 0;
  Int4			iCount = 0;
  Int4			iBytes = 0;
  Int4			NumEntries = 0;
  ValNodePtr		pvnUIDS = NULL, pvnThis = NULL;
  
/* this sets up the unix time limit */  

#ifdef OS_UNIX
 {
  struct rlimit		rl;
  getrlimit(RLIMIT_CPU, &rl);
  rl.rlim_max = rl.rlim_cur = CPUTIME_MAX;
  setrlimit(RLIMIT_CPU, &rl);
 }
#endif 

  if (!InitMMDBSrv()) {
       printf("Content-type: text/html\r\n\r\n");
       printf("<H2>MMDBSRV Error</H2>\n - Unable to Open Server Configuration File.<p>\n");
       printf("Contact the MMDBSRV administrator for this site.<p>\n");   
	exit(1);
  }

  StringCat(LOGpath, "mmdbsrv.log");
  if( !ErrSetLogfile (LOGpath, ELOG_APPEND) ) {  
      printf("Content-type: text/html\r\n\r\n");
      printf("<H2>MMDBSRV Error</H2>\n - Unable to Open Log.<p>\n");
      printf("Contact MMDBSRV administrator for this site: %s<p>\n",  MAILTO);   
	exit(1);
  }
  
  ErrSetLogLevel(SEV_MAX); 
  ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);



  if((error = WWWGetArgs(&info)) != WWWErrOk) {
      printf("Content-type: text/html\r\n\r\n");
      printf("<H2>MMDBSRV Error</H2>\n Failed to Process Posting - Check your GET/POST syntax\n");   
    goto earlyout;
  }

     
 /* if(WWWGetBrowser(info) != NETSCAPE) {   
       SendGetNetscape();    
       exit(1); 
  } */

  if ((!OpenMMDBAPI(0, NULL)) || (!MMDBInit()))
    {
       ErrLogPrintf("#MMDB Database Access Failed to Initialize.\n"); 
       printf("Content-type: text/html\r\n\r\n");
       printf("<H2>MMDBSRV Error</H2>\n - Unable to Open MMDB Database on Server.<p>");
       printf("Contact MMDBSRV administrator for this site: %s<p>\n",  MAILTO);   
	exit(1);
    }
                                                   /* yanli */
   if (EntrezInit("mmdbsrv", NULL, FALSE) == FALSE)    
     {
       printf("Content-type: text/html\n\n");
       printf("<h2>Error</h2>\n");
       printf("MMDBSERV: EntrezInit Failed.<p>\n");
       return;
     }       
                                                   /* yanli */
/*if( !BioseqFetchInit(TRUE))                    
    {
      ErrLogPrintf("BioseqFetchInit failed! \n");
      printf("Content-type: text/html\r\n\r\n");
      printf("<H2>MMDBSRV Error</H2>\n - BioseqFetchInit failed! <p>"); 
      printf("Contact MMDBSRV administrator for this site: %s<p>\n",  MAILTO);
    exit(1);
    }   */

    SeqAsnLoad();
    objmimeAsnLoad();

  IndexArgs = -1;
  if ((IndexArgs = WWWFindName(info,"db")) >= 0)
    {
      pcThis = WWWGetValueByIndex(info,IndexArgs);
      DispOpt = pcThis[0];
      if ((DispOpt != 't') && (DispOpt != 'T'))
        {
          printf("Content-type: text/html\r\n\r\n");
          printf("<H2>MMDBSRV Error</H2>\n - Wrong Database Type db=%c.<p>\n",  DispOpt );
          printf("Try <a href=\"%s\">WWW-Entrez</a> for this<p>\n",  URLHelp);   
	  goto cleanout;
        } 
    }

  IndexArgs = -1;
  if ((IndexArgs = WWWFindName(info, "form")) >= 0)
    {
      pcThis = WWWGetValueByIndex(info,IndexArgs);
      if ((!StrToInt4(pcThis,&Form)) || (Form != 6))
        {
          printf("Content-type: text/html\r\n\r\n");
          printf("<H2>MMDBSRV Error</H2>\n - Wrong Form Type form=%s.<p>\n",  pcThis );
          printf("<pre>Only supports form=6</pre><p>\n"); 
          printf("Try <a href=\"%s\">WWW-Entrez</a> for this<p>\n",  URLHelp);
	  goto cleanout;
        } 
    }




 IndexArgs = -1;
 if ((IndexArgs = WWWFindName(info, "uid")) < 0)
   {
       printf("Content-type: text/html\r\n\r\n");
       printf("<H2>MMDBSRV Error</H2>\n - No Identifier specified:<p>\n<pre>Missing uid=</pre><p>\n");   
       goto cleanout;
   }
 else
   {
     /* loop over entire set of tag=value entries and add all the UID's found together */
     /* must handle uid=1234&uid=1OMD&uid=1234,2345,2134&dopt=s&uid=3INS              */   
      NumEntries = WWWGetNumEntries(info);
      for (IndexArgs = 0; IndexArgs < NumEntries; IndexArgs++)
        {
	    pcName = WWWGetNameByIndex(info,  IndexArgs);
	    pcThis = WWWGetValueByIndex(info,  IndexArgs);

	    if (StrICmp(pcName, "uid") == 0)
	      {
	 /*         printf("[%s][%s]\n", pcThis, pcName);  */
                  MakeUIDList(pcThis, &pvnUIDS);
	      }
	}		 
     if (pvnUIDS == NULL)
         {
	    printf("Content-type: text/html\r\n\r\n");
            printf("<H2>MMDBSRV Error</H2>\n - UID specified does not match an MMDB-ID or PDB-Code:<p>\n");   
            goto cleanout;
	 }
     pvnThis = pvnUIDS;
     while (pvnThis)
       {
          iMMDBids++;
          pvnThis = pvnThis->next;
       }
     MMDBids = (Int4Ptr) MemNew((size_t) (sizeof(Int4) * iMMDBids));
     pvnThis = pvnUIDS;
     iCount = 0;
     while (pvnThis)
     {
          MMDBids[iCount] =  (Int4) pvnThis->data.intvalue;
/* printf("%ld \n",(long) MMDBids[iCount]); */
          pvnThis = pvnThis->next;
	  iCount++;
     }   	 
   }
   
  DispOpt = ' '; 
  IndexArgs = -1;
  if ((IndexArgs = WWWFindName(info, "dopt")) >= 0)
    {
      pcThis = WWWGetValueByIndex(info,IndexArgs);
      switch (pcThis[0])
      {
	 case 's':
	 case 'S':
	    DispOpt = 's';
	   break;
	 case 'r':
	 case 'R':
	    DispOpt = 'r';
	   break;
	 case 'i':
	 case 'I':
	    DispOpt = 'i';
	   break;
	 case 'j':
	 case 'J':
		DispOpt = 'j';
		break;
	 case 'k':
	 case 'K':
	     DispOpt = 'k';
	   break;
         case 'a':
         case 'A':
             DispOpt = 'a';
             Save = 1;
          break;
         default:
	    {
	    printf("Content-type: text/html\r\n\r\n");
	    printf("<H2>MMDBSRV Error</H2>\n - Wrong Display Option Type: dopt=%s.<p>\n",  pcThis);
	    printf("<pre>dopt supports:\n's' Structure summary\n");
	    printf("'r' RasMol(PDB) format\n'k' Kinemage format\n'i', 'j', or 'a' ASN.1</pre><p>\n"); 
	    goto cleanout2;
	    }
      } 
    }
  else
    {
	DispOpt = 's';  /* structure summary is default when no dopt supplied */
    }


  IndexArgs = -1;
  if ((IndexArgs = WWWFindName(info, "Complexity")) >= 0)
    {
      pcThis = WWWGetValueByIndex(info,IndexArgs);


	  for (iCount = 0; iCount < NUM_COMPLEXITY_TYPES; iCount++)
	    if (StrICmp(pcThis,ComplexityDescriptions[iCount]) == 0)
	      break;

	  Complex = ALLMDL;
	  MaxModels = 1;

	  switch (iCount)
	    {
	    case 0 :
	     Complex = ONECOORDRES; 
	     break;
	    case 1:
	     Complex = ONECOORDATOM; 
	     break;
	    case 2:
	     MaxModels = 5; 
	     break;
	    case 3 :
	     MaxModels = 10; 
	     break;
	    case 4 :
	     MaxModels = INT2_MAX; 
	     break;
	    case 5:
	     Complex = ONECOORDATOM; 
	      break;
	    default:
	       {
		printf("Content-type: text/html\r\n\r\n");
		printf("<H2>MMDBSRV Error</H2>\n - Wrong Complexity Type: Complexity=%s.<p>\n", pcThis );
		goto cleanout2;
	       } 
	    }    
    }  

  IndexArgs = -1;
  if ((IndexArgs = WWWFindName(info, "mdlLvl")) >= 0)
    {
      pcThis = WWWGetValueByIndex(info, IndexArgs);
      Complex = atoi(pcThis);
    }

  IndexArgs = -1;
  if ((IndexArgs = WWWFindName(info, "maxModels")) >= 0)
    {
      pcThis = WWWGetValueByIndex(info, IndexArgs);
      MaxModels = atoi(pcThis);
    }
  
  IndexArgs = -1;
  if ((IndexArgs = WWWFindName(info, "KinemageColor")) >= 0)
    {
      pcThis = WWWGetValueByIndex(info, IndexArgs);
      KinColor = GetOptionValue(pcThis, NUM_COLOR_TYPES, ColorDescriptions);
    }
  
	 
	 
  IndexArgs = -1;
  if ((IndexArgs = WWWFindName(info, "KinemageRender")) >= 0)
    {
      pcThis = WWWGetValueByIndex(info,IndexArgs);
      StrToInt4(pcThis, &KinRender); 
    }	 
  
  IndexArgs = -1;
  if ((IndexArgs = WWWFindName(info, "save")) >= 0)
    {  /* these don't affect structure summaries */
      pcThis = WWWGetValueByIndex(info,IndexArgs);
      if (StrICmp(pcThis, "See") == 0)
        {
	    Save = 1;  /* sends MIMED-type files to view in ascii-readable form */
	}
      else if (StrICmp(pcThis, "Save") == 0)
        {
	    Save = 2;  /* sends MIMED-type files in raw save form */
	}	
      else if (StrICmp(pcThis, "M") == 0)
        {
	    Save = 3;  /* MacSave - not implemented */
	}	
      else if (StrICmp(pcThis, "asntext") == 0)
        {
	    Save = SEND_FILE;  /* writes out asn text without the html header */
	}	
    }	 
  
  IndexArgs = -1;
  if ((IndexArgs = WWWFindName(info, "header")) >= 0)
    {
      pcThis = WWWGetValueByIndex(info,IndexArgs);
      if (StrICmp(pcThis, "no") == 0)
        {
	    Save = 2;
	}
    }	
    
    
  IndexArgs = -1;
  if ((IndexArgs = WWWFindName(info, "title")) >= 0)
    {
      pcThis = WWWGetValueByIndex(info,IndexArgs);
      if (StrICmp(pcThis, "no") == 0)
        {
	  
          GlobalTitles = FALSE;
	}
    }	  
   
  IndexArgs = -1;
  if ((IndexArgs = WWWFindName(info, "html")) >= 0)
    {
      pcThis = WWWGetValueByIndex(info,IndexArgs);
      if (StrICmp(pcThis, "no") == 0)
        {
          GlobalNonHtmlOutput = TRUE;
	  GlobalTitles = FALSE;
	}
    }	
    
    
    
/* call the pertinent Page Sender */

 if  (DispOpt == 's')
   {
       iBytes = SendSummaryPage(MMDBids, iMMDBids,  Save);
   }
 else 
   {   
        iBytes = SendStructureMIME(DispOpt,  MMDBids[0], Save  , 
                         Complex, MaxModels,  KinColor, KinRender);
   }
 
   
   MemFree(MMDBids);
 
   /* log first MMDB-id, number of id's asked for, number of bytes sent */
   time_now = GetSecs();
   TimeNowStr = ctime(&time_now);
   TimeNowStr[24] = '\0'; 
   ErrLogPrintf("\n%s|%s|%s|%s|Do %c|Id %ld|No %ld|By %ld|Sv %ld|Cp %ld|Md %ld|Kc %ld|Kr %ld",
                WWWGetAddress(info), WWWGetHost(info), WWWGetAgent(info), TimeNowStr, 
                (char) DispOpt,
                (long) MMDBids[0], (long) iMMDBids, (long)iBytes, (long) Save,
                (long) Complex, (long) MaxModels, (long) KinColor, (long) KinRender); 
  
  
  CloseMMDBAPI();
  MMDBFini();  
  fflush(stdout);  
  exit(0);
  
cleanout2:
  MemFree(MMDBids);  
  CloseMMDBAPI();
  MMDBFini();
  /* log first MMDB-id, number of id's asked for, number of bytes sent */
   time_now = GetSecs();
   TimeNowStr = ctime(&time_now);
   TimeNowStr[24] = '\0'; 
   ErrLogPrintf("\n#Syntax Error %s|%s|%s|%s|%ld",
                WWWGetAddress(info), WWWGetHost(info), WWWGetAgent(info), TimeNowStr, 
                (long)iBytes); 
   fflush(stdout);
   exit(0);

cleanout:
  CloseMMDBAPI();
  MMDBFini();
  /* log first MMDB-id, number of id's asked for, number of bytes sent */
   time_now = GetSecs();
   TimeNowStr = ctime(&time_now);
   TimeNowStr[24] = '\0'; 
   ErrLogPrintf("\n#Query Not Supported %s|%s|%s|%s|%ld",
                WWWGetAddress(info), WWWGetHost(info), WWWGetAgent(info), TimeNowStr, 
                (long)iBytes); 
   fflush(stdout);
   exit(0);

earlyout:
   time_now = GetSecs();
   TimeNowStr = ctime(&time_now);
   TimeNowStr[24] = '\0'; 
   ErrLogPrintf("\n#Get/Post Error %s|%s|%s|%s|%ld",
                WWWGetAddress(info), WWWGetHost(info), WWWGetAgent(info), TimeNowStr, 
                 (long)iBytes); 
  
  
  fflush(stdout);
  exit(0);
}

