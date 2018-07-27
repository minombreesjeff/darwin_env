/* 
* $Id: mmdbsrv.c,v 1.7 2003/01/31 16:05:11 chenj Exp $
*
*
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
*
* Author:  Jie Chen 
*
*
* $Log: mmdbsrv.c,v $
* Revision 1.7  2003/01/31 16:05:11  chenj
* minor changes
*
* Revision 1.6  2003/01/29 19:51:12  chenj
* minor changes
*
* Revision 1.5  2003/01/23 20:51:06  chenj
* fixed bug in dealing with obsolete structure
*
* Revision 1.4  2003/01/22 17:04:45  chenj
* change Cn3D 4.0 to 4.1
*
* Revision 1.3  2003/01/15 17:54:59  chenj
* bugs in view of an obsolete structure was fixed
*
* Revision 1.2  2003/01/15 16:12:04  chenj
* Change the font color (replaced by)
*
* Revision 1.1.1.1  2002/12/04 21:12:07  chenj
* Imported sources
*
*
*
* This file with others: mmdbgraph.c mmdbutil.c, is used to created new 
* version of mmdbsrv.cgi which displays the structure features and annotations 
* with graphics.
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
as obtained from ftp://ncbi.nlm.nih.gov/mmdb/mmdbdata  
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
ftp://ncbi.nlm.nih.gov/mmdb/www/mmdbsrv
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
at ftp://ncbi.nlm.nih.gov/mmdb/www/http
You must hand edit or use scripts or SED to change links from 
NCBI's to your own in all these HTML files.

The second contains the GIF library as obtained from
the NCBI ftp site ftp://ncbi.nlm.nih.gov/mmdb/mmdbgifs

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
; ENTREZurl = http://www.ncbi.nlm.nih.gov/htbin-post/Entrez/query
ENTREZurl = http://www.ncbi.nlm.nih.gov/entrez/query.fcgi
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
TAXurl = http://www.ncbi.nlm.nih.gov/htbin-post/Taxonomy/wgetorg

[CD]
DARTdatabase = CDart
LOGINname = lewisg
PASSwd = lewisg
INITpath = /netopt/structure/ini/.odbc.sassy.ini
LIBpath = /opt/machine/merant/lib
CDDurl = http://scarecrow:5701/Structure/cdd/cddsrv.cgi

-------------------- cut here --------------------
********************************************/
 
#include <ncbi.h>
#include <ncbiwww.h>
#include <sys/resource.h>
#include <mmdbapi.h>
#include <objmime.h>
#include "mmdbuti.h"
#include <sys/utsname.h>

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

#define HEADFILE "sshead.txt" /* Structure Summary Header File */

#define TAILFILE "sstail.txt" /* Structure Summary Tail File  */
 
#define CHAIN_CHOICE_NUM	1
#define ORG_CHOICE_NUM		6

/* some defines needed for the taxonomy code */

#define TAX_ID_CHOICE_NUM	0
#define MAX_CHAINS		128

static WWWInfoPtr 	info = NULL;

static Boolean GlobalTitles = TRUE;
static Boolean GlobalNonHtmlOutput = FALSE;

static Char HTMLPath[PATH_MAX];
static Char URLBase[PATH_MAX];
static Char URLHelp[PATH_MAX];
static Char HELPname[PATH_MAX];
Char	Database[PATH_MAX];

static Char CGIPath[PATH_MAX];
static Char URLcgi[PATH_MAX];
static Char URLcgi_absolute_path[PATH_MAX];
static Char CGIName[256];
static Char DATApath[PATH_MAX];
static Char LOGpath[PATH_MAX];

Char 	ENTREZurl[PATH_MAX];
Char 	VASTurl[PATH_MAX];
static Char MMDBurl[PATH_MAX];
static Char PDBurl[PATH_MAX];
static Char TDBurl[PATH_MAX];
static Char TAXurl[PATH_MAX];
static Char MAILto[256];
Char MAILTO[PATH_MAX];
static Char ARROW[PATH_MAX];

static Char DART[PATH_MAX];
static Char LOGIN[PATH_MAX];
static Char PASSWD[PATH_MAX];
static Char INITpath[PATH_MAX];
static Char LIBpath[PATH_MAX];
Char 	CDDurl[PATH_MAX];


/* Global Variables defined by J. Chen */

Boolean		psok = TRUE;
Dart_Connect	*dartcon;

extern Int4 ModelMapOrImg(Boolean ismap, gdImagePtr im, Int4 uid, PMSD pmsd,
Int4 chain1, Int4 chain2, IntervalHead **DomHead, Int4Ptr DomIdx, FILE *File);

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

    

static Int4 ConvertMMDBUID(CharPtr pcString)  
{
    Int4 iUID;
    unsigned i;
    CharPtr pcTemp = NULL;
        
    if (pcString == NULL)
    return 0;

    iUID = 0;
    pcTemp = StringSave(pcString);
    
    for (i=0; i< StrLen(pcTemp); i++) pcTemp[i] = toupper(pcTemp[i]);
    CleanSpaces(pcTemp);
    if (!StrToInt4(pcTemp, &iUID))
	iUID = constructLiveOrDeadMmdbIdForPdbId(pcTemp, &psok, Database);
	
    MemFree(pcTemp);
    return iUID; 
} /* ConvertMMDBUID() */

 
static void MakeUIDList(CharPtr pcString,  ValNodePtr PNTR ppvnUIDS)
{
  CharPtr Ptr = NULL;
  CharPtr Ptr2 = NULL, Ptr3 = NULL;
  Char SavedChar = NULL, SavedChar3 = NULL;
  Int4 Uid = 0,  Uid1, Uid2, tmpuid1, tmpuid2;
  
  Ptr = SkipSpaces(pcString);
  if (*Ptr == NULLB)
    return;

  while (*Ptr)
    {
      Ptr2 = SkipToSet(Ptr," ,");
      SavedChar = *Ptr2;
      *Ptr2 = NULLB;

      Ptr3 = SkipToSet(Ptr, " -");
      if (*Ptr3 != NULLB) {
	  SavedChar3 = *Ptr3;
          *Ptr3 = NULLB;
          Uid1 = 0;
          Uid1 = ConvertMMDBUID(Ptr);
	  if (Uid1 <0) tmpuid1 = -Uid1;
	  else tmpuid1 = Uid1;
          *Ptr3 = SavedChar3;
          Ptr = SkipSet(Ptr3, " -");
          Uid2 = 0;
          Uid2 = ConvertMMDBUID(Ptr);
	  if (Uid2<0) tmpuid2 = -Uid2;
	  else tmpuid2 = Uid2;
          for (Uid = tmpuid1;  Uid <= tmpuid2; Uid++) {
	    if (Uid == -Uid1) Uid = Uid1;
	    if (Uid == -Uid2) Uid = Uid2;
     	    if (Uid) ValNodeAddInt(ppvnUIDS, 0, Uid);
	  }
      }
      else {
      	  Uid = ConvertMMDBUID(Ptr);
      	  if (Uid) ValNodeAddInt(ppvnUIDS, 0, Uid);
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


/* J. Chen (begin) */


static void
PrintStrucViewNew_page(Int4 uid, FILE *File)
{
	
	fprintf(File, "<table border=0 cellpadding=0 cellspacing=0>\n");
	fprintf(File, "<FORM METHOD=post ACTION=\"%s%s\">\n", URLcgi,CGIName);
	fprintf(File, "<tr>\n");
  	fprintf(File, "<TD class=SMALL1>\n");

	fprintf(File, "<strong>");
	fprintf(File, "<INPUT TYPE=submit NAME= submit VALUE=\"View 3D Structure\">");
	fprintf(File, "&nbsp;&nbsp;of&nbsp;</strong>\n");

	fprintf(File, "<SELECT NAME=Complexity>\n");
	fprintf(File, "<OPTION VALUE=\"Cn3D Subset\">Best Model\n");
	fprintf(File, "<OPTION VALUE=\"Virtual Bond Model\">Virtual Bond\n");
	fprintf(File, "<OPTION VALUE=\"All Models\">All Models\n");
	fprintf(File, "</SELECT>\n");

	fprintf(File, "<strong>&nbsp;with&nbsp;</strong>\n");

	fprintf(File, "<SELECT NAME=dopt>\n");
	fprintf(File, "<OPTION VALUE=j>Cn3D\n");
	fprintf(File, "<OPTION VALUE=r>RasMol\n");
	fprintf(File, "<OPTION VALUE=k>Mage\n");
	fprintf(File, "</SELECT>\n");

	fprintf(File, "<SELECT NAME=save>\n");
	fprintf(File, "<OPTION VALUE=Viewer>Display\n");
	fprintf(File, "<OPTION VALUE=See>See File\n");
	fprintf(File, "<OPTION VALUE=Save>Save File\n");
	fprintf(File, "</SELECT>\n");
	
    	fprintf(File, "<img src=\"/Structure/new.gif\" alt=\"New\">\n");
    	fprintf(File, "<a href=\"/Structure/CN3D/cn3d.shtml\">");
	fprintf(File, "<B><I>Get Cn3D 4.1!</I></B></a>");

	if (uid <0) uid = -uid;
        fprintf(File, "<INPUT TYPE=HIDDEN NAME=uid VALUE=%d>\n", uid);
        fprintf(File, "<INPUT TYPE=HIDDEN NAME=form VALUE=6>\n");
        fprintf(File, "<INPUT TYPE=HIDDEN NAME=db VALUE=t>\n");
	fprintf(File, "<INPUT TYPE=HIDDEN NAME=KinemageColor VALUE=\"Molecule Number\">\n");	
	fprintf(File, "<INPUT TYPE=HIDDEN NAME=KinemageRender VALUE=63>\n");

	fprintf(File, "</td>\n</tr>\n");
	fprintf(File, "</FORM>\n");
	fprintf(File, "</table>\n");

}	/* end PrintStrucViewNew_page */




#define MMDBOpt "SUBMIT=y&db=structure&orig_db=structure&term="

static void
PrintStrucInfoNew(PDNMS ModelStruc, Int4 LiveUid, unsigned short Live, 
			CharPtr tax_save, Int4 uid, FILE *File)
{
   Int2 chaincnt;
   Int4 depyear, depday;
   PMSD pmsdThis = NULL;
   BiostrucSourcePtr pbssThis = NULL;
   ValNodePtr pvnThis = NULL;
   BiostrucHistoryPtr pbshThis = NULL;
   CharPtr pcAuthors = NULL;
   
   if (!ModelStruc) 
     return;

   pmsdThis = (PMSD) ModelStruc->data.ptrvalue;
   pvnThis =ValNodeFindNext(pmsdThis->pbsBS->descr,NULL,BiostrucDescr_history);

   if (pvnThis) {
      pbshThis = (BiostrucHistoryPtr) pvnThis->data.ptrvalue;
      pbssThis = pbshThis->data_source;
   }


  fprintf(File, "\n<!-- Structure Info table -->\n");
  fprintf(File, "<TABLE width=800 BORDER=0 CELLPADDING=3 CELLSPACING=0 bgcolor=#FFFFCC>\n\n");
  
  fprintf(File, "<TR>\n");
  fprintf(File, "<TD>\n");

  fprintf(File, "<table>   <!-- 1st subtable, in the first layer -->\n\n");

  /* insert a blank row in the table, for spacing */
  fprintf(File, "<tr>\n<td VALIGN=TOP NOWRAP> </td>\n</tr>\n\n");

  fprintf(File, "<tr>\n");
  fprintf(File, "<td VALIGN=TOP NOWRAP ALIGN=RIGHT class=H4>\n");
  pvnThis =ValNodeFindNext(pmsdThis->pGraphDescr,NULL,BiomolDescr_pdb_comment);
  chaincnt = ChainCount(pmsdThis);
  fprintf(File, "<strong>Description:</strong></td>\n");
  fprintf(File, "<td class=\"TEXT\">%s.",pmsdThis->pcChemName);

  if (!chaincnt) {
      fprintf(File, "<br><font color=red>");
      fprintf(File, "There is no protein and nucleotide chains in this structure.</font></br>\n");
  }
  fprintf(File, "</td>\n</tr>\n\n");


  pcAuthors =  AuthorListPDB(pmsdThis->pbsBS);
  if (pcAuthors) {
  	fprintf(File, "<tr>\n");
	fprintf(File, "<td VALIGN=TOP NOWRAP ALIGN=RIGHT class=H4>");
	fprintf(File, "<strong>Deposition:</strong></td>\n");
	fprintf(File, "<td class=\"TEXT\">%s, ", pcAuthors);

        depyear = pbssThis->database_entry_date->data[1];
        depday = pbssThis->database_entry_date->data[3];
        fprintf(File, "%2d-%3s-%02d</td>\n",(int) depday,
                NCBI_months[pbssThis->database_entry_date->data[2] - 1],
             (int) depyear%100);

	fprintf(File, "</tr>\n\n");
	fflush(File);
	MemFree(pcAuthors);
  }
  else {
	fprintf(File, "<tr>\n");
	fprintf(File, "<td><H2>");
	fprintf(File, "No Authors' information -- Contact structure group.");
	fprintf(File, "</H2></td>\n</tr>\n");
	fprintf(File, "</table>\n");
	exit(1);
  } 


  if (tax_save != NULL) fprintf(File, "%s", tax_save);
  else if (pmsdThis->pcPdbSource) {
        fprintf(File, "<tr>\n");
	fprintf(File, "<td NOWRAP class=H4>");
        fprintf(File, "<strong>Source</strong>:</td>\n");
	fprintf(File, "<td class=\"TEXT\">%s</td>\n</tr>\n", 
						pmsdThis->pcPdbSource);
  }

  fprintf(File, "<tr>\n");
  fprintf(File, "<td VALIGN=TOP NOWRAP ALIGN=RIGHT class=H4>");
  fprintf(File, "<strong>Reference:</strong></td>\n");
  fprintf(File, "<td class=TEXT>\n");
  fprintf(File, "<A HREF=\"%s?db=structure&cmd=Display&dopt=structure_pubmed&from_uid=%ld\">PubMed</A>\n",
                        ENTREZurl, (long) pmsdThis->iMMDBid );
  if (!Live && (pmsdThis->iMMDBid == LiveUid)) {
  	fprintf(File, "<font class=H4><strong>&nbsp;&nbsp;&nbsp;&nbsp;MMDB:</strong></font>\n\n");
  	fprintf(File, "<A HREF=\"%s?%s%s\">%d</A>",
           	ENTREZurl, MMDBOpt, pmsdThis->pcPDBName, pmsdThis->iMMDBid);
  }
  else { 
    	fprintf(File, "</td>\n</tr>\n\n");
	fprintf(File, "<tr>\n");
	fprintf(File, "<td VALIGN=TOP NOWRAP ALIGN=RIGHT class=H4>");
	fprintf(File, "<strong>MMDB:</strong></td>\n");
	fprintf(File, "<td class=TEXT>\n");
	fprintf(File, "<A HREF=\"%s?%s%s\">%d</A>",
                ENTREZurl, MMDBOpt, pmsdThis->pcPDBName, pmsdThis->iMMDBid);
	
    	/* J. Chen, for an obsolete or removed model */
  	if (psok && Live) {
		fprintf(File, "\n<FONT color=MAROON>(</FONT>");
		fprintf(File, "<a href=\"%s%s#ObsoNbr\">", URLBase, HELPname);
		fprintf(File, "<FONT color=MAROON>obsolete, from archive</FONT></a>");
		fprintf(File, "<FONT color=MAROON>)</FONT>\n");
  	}
  	else if (psok && pmsdThis->iMMDBid != LiveUid) {
		fprintf(File, "\n<FONT color=red>(</FONT>");
		fprintf(File, "<a href=\"%s%s#ObsoNbr\">", URLBase, HELPname);
		fprintf(File, "<FONT color=MAROON>replaced by:</FONT></a> ");
       		fprintf(File, "<A HREF=\"%s%s?uid=%d&form=6&db=t&Dopt=s\">",
        					URLcgi, CGIName, LiveUid);
		fprintf(File, "<FONT COLOR=RED>%d</FONT></a>", LiveUid);
		fprintf(File, "<FONT color=red>)</FONT>\n");
  	}
  }

  fprintf(File, "<font class=H4>");
  fprintf(File, "<strong>&nbsp;&nbsp;&nbsp;&nbsp;PDB:</strong></font>\n<spacer size=4>\n");
  fprintf(File, "<A HREF=\"%s=%s\">%s</A>\n",
  		TDBurl, pmsdThis->pcPDBName, pmsdThis->pcPDBName);

  fprintf(File, "</td>\n</tr>\n");

  /* insert a blank row in the table, for spacing */
  fprintf(File, "\n<tr>\n");
  fprintf(File, "<td VALIGN=TOP NOWRAP><BR></td>\n");
  fprintf(File, "</tr>\n");
  fprintf(File, "</table>  <!-- end of first subtable -->\n");

  fprintf(File, "</TD>\n</TR>\n\n");
  fprintf(File, "<TR>\n<TD>\n\n   <!-- 2nd subtable -->\n");
  PrintStrucViewNew_page(uid, File);
  fprintf(File, "	<!-- end of 3rd subtable -->\n");
  fprintf(File, "\n</TD>\n</TR>\n"); 

  /* J. Chen: close the TABLE */
  fprintf(File, "</TABLE>\n");

  return;

} 	/* end PrintStrucInfoNew */




static void 
PrintChainsMap (BiostrucPtr bsp, PDNMS ModelStruc, Int2 chain1, FILE *File)
{
	BiostrucFeaturePtr domain_bfp = NULL;
	Boolean		hasDomain = FALSE; 
	Int4		uid, imgsize = 0, *DomIdx;
	Int2		i, y, cnt=0, chain2;
	PMSD		pmsd;
	PDNMM		pdnmm, pdnmm1;
	IntervalHead	**DomHead;

	pmsd = (PMSD) ModelStruc->data.ptrvalue;
	pdnmm = pmsd->pdnmmHead;
        if (pdnmm == NULL) return;
	cnt  = ChainCount(pmsd);
	if (!cnt) return;
	
	DomHead = (IntervalHead **) MemNew (cnt * sizeof(IntervalHead *));
	for (i=0; i< cnt; i++) DomHead[i] = NULL;

	CalDomIdx(pdnmm, &DomIdx);
	GetDomFeaPtr(bsp, &hasDomain, &domain_bfp); 
	CheckDomColIdx(domain_bfp, pdnmm, DomHead,DomIdx,NULL,hasDomain,FALSE);

	GroupingChains(pmsd); 
			
	pdnmm = pmsd->pdnmmHead;
	pdnmm1 = PdnmmforChainX(pdnmm, chain1);
	pdnmm = pdnmm1;
	
	uid = (Int4)pmsd->iMMDBid;
	chain2 = MIN(cnt, chain1+ChainPerImg-1);

	fprintf(File, "<map name=chain_map>\n");
	y =ModelMapOrImg(TRUE,NULL,uid,pmsd,chain1,chain2,DomHead,DomIdx,File);
	imgsize = y + FontBH + 80;

        fprintf(File, "</map>\n");
/*
         {
         struct utsname name;
           char *chp = getenv("SERVER_NAME");

         uname(&name);
           if (!strcmp(chp,"web.ncbi.nlm.nih.gov") &&
                                       !strcmp(name.nodename, "rosencrantz"))
               strcpy(URLcgi_absolute_path,
                       "http://rosencrantz.nlm.nih.gov:2441/Structure/mmdb/");
         else if (!strcmp(chp, "web.ncbi.nlm.nih.gov") &&
                                       !strcmp(name.nodename, "guildenstern"))
             strcpy(URLcgi_absolute_path,
               "http://guildenstern.nlm.nih.gov:2441/Structure/mmdb/");
         else if (!strcmp(chp, "www.ncbi.nlm.nih.gov") &&
                                       !strcmp(name.nodename, "rosencrantz"))
               strcpy(URLcgi_absolute_path,
                       "http://rosencrantz.nlm.nih.gov:80/Structure/mmdb/");
         else if (!strcmp(chp, "www.ncbi.nlm.nih.gov") &&
                                       !strcmp(name.nodename, "guildenstern"))
               strcpy(URLcgi_absolute_path,
                       "http://guildenstern.nlm.nih.gov:80/Structure/mmdb/");
         else strcpy(URLcgi_absolute_path, URLcgi);

         }
 */

       strcpy(URLcgi_absolute_path, URLcgi);


	fprintf(File, "<img src=\"%s%s?cmd=graph&uid=%d&chbeg=%d&chend=%d&imgsize=%d\" usemap=#chain_map border=0 ismap>\n",
                URLcgi_absolute_path, CGIName, uid, chain1, chain2, imgsize);

	MemFree(DomIdx);

}	/* end PrintChainsMap */

	

	
static void PrintPageView(Int4 UidNum, Int4Ptr Uids, Int4 TotalPg, Int2 PageCur,FILE *File)
{
	Int2 i, tmpuid;

	if (TotalPg == 1) return;

	fprintf(File, "<FORM name=pagequery method=post action=\"%s%s\">\n",
		URLcgi, CGIName);	
        if (UidNum == 1) {
	      if (Uids[0] <0) tmpuid = -Uids[0];
	      else tmpuid = Uids[0];
              fprintf(File,"<INPUT TYPE=HIDDEN NAME=uid VALUE=%d>\n",tmpuid);
	}
        else {
		if (Uids[0] <0) tmpuid = -Uids[0];
		else tmpuid = Uids[0];
                fprintf(File, "<INPUT TYPE=HIDDEN NAME=uid VALUE=%d",tmpuid);
                for (i=1; i< UidNum; i++) {
		    if (Uids[i] <0) tmpuid = -Uids[i];
		    else tmpuid = Uids[i];
                    fprintf(File, ",%d", Uids[i]);
		}
                fprintf(File, ">\n");
        }
        fprintf(File, "<INPUT TYPE=HIDDEN NAME=form VALUE=6>\n");
        fprintf(File, "<INPUT TYPE=HIDDEN NAME=db VALUE=t>\n");
	fprintf(File, "<input type=hidden name=page VALUE=%d>\n", PageCur);
	fprintf(File, "</FORM>\n");

	fprintf(File, "<table>\n<tr>\n");
	fprintf(File, "<td width=\"270\" align=right class=H4><strong>Summary page:</strong></td>\n"); 
	fprintf(File, "<td width=\"60\" class=TEXT>");
	if (PageCur == 1) 
		fprintf(File, "Previous</td>\n");
	else 
		fprintf(File, "<a href=\"about:\" onClick=\"pageview(\'%d\'); return false\">Previous</a></td>\n", 
		PageCur-1);
	for (i = 1; i<= TotalPg; i++)
	{
		if (i==PageCur) 
		    fprintf(File, "<td width=\"20\" class=TEXT><strong>%d</strong></td>\n",i);
		    
		else {
		    fprintf(File, "<td width=\"20\" class=TEXT>");
		    fprintf(File, "<a href=\"about:\" onClick=\"pageview(\'%d\'); return false;\">%d</a></td>\n", i, i);
		}
	}
	
	fprintf(File, "<td width=\"60\" class=TEXT>");
	if (PageCur == TotalPg)
		fprintf(File, "Next</td>\n");
	else
		fprintf(File, "<a href=\"about:\" onClick=\"pageview(\'%d\'); return false\">Next</a></td>\n",
		PageCur+1);
	fprintf(File, "</tr>\n</table>\n");

	fprintf(File, "<script>\n");
	fprintf(File, "function pageview(page)\n");
	fprintf(File, "{\n");
        fprintf(File, "document.pagequery.reset();\n");
        fprintf(File, "document.pagequery.page.value=page;\n");
        fprintf(File, "document.pagequery.submit();\n");
	fprintf(File, "}\n");
	fprintf(File, "</script>\n");
	
}





/* J. Chen (end) */


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
	Int2 i, j;

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
  Int4  error_flag = 0;
  AsnIoPtr aip; 
        
  /* yanli start */
  NcbiMimeAsn1Ptr mime;
  BiostrucSeqPtr bssp;
  SeqEntryPtr sep;

  bsp = (BiostrucPtr) openBSP(uid, Complexity, Models, TRUE, FALSE, FALSE, 
							&psok, Database);

  if (bsp == NULL) {
    char str[10];

    sprintf(str, "%d", uid);
    PrtMes(NULL, "MMDBSRV", 
    	"No such structure available on this MMDB server: uid=", str, TRUE);

    return 0;
  }

  sep = (SeqEntryPtr) constructSeqEntryForMmdbId(uid, TRUE, Database);
  if (sep == NULL) {
	char str[10];

	sprintf(str, "%d", uid);
	PrtMes(NULL, "MMDBSRV",
	  "No sequence information available on this MMDB server, maybe an obsolete structure: uid=", str, TRUE);

	return 0;
  }

  bssp = BiostrucSeqNew();
  bssp->structure = bsp;
  ValNodeLink(&(bssp->sequences), sep);     
        
  mime = (NcbiMimeAsn1Ptr) ValNodeNew(NULL);    /* yanli */
  mime->choice = NcbiMimeAsn1_strucseq;
  mime->data.ptrvalue = bssp;

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
    ClearStructures();

  }
  else if (Filetype == 'i') {
    /* Cn3D v1.0 asn.1 format.  Included for backwards compatibility, in case users
     * want to stick with their old Cn3D for some reason.  */
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
    ClearStructures();

  }
  else if (Filetype == 'r') {
    /* RasMol format */
    if (Mime == LAUNCH_VIEWER)
      printf ("Content-type: chemical/x-pdb\r\n\r\n");
    
    ModelStruc = MakeAModelstruc(bsp);
    bssp->structure = NULL;  /* already linked into modelstruc */
    WritePDBAllModel(ModelStruc, stdout);
    ClearStructures();
  }
  else if (Filetype == 'k') {
    /* Mage format */
    if (Mime == LAUNCH_VIEWER)
      printf ("Content-type: chemical/x-kinemage\r\n\r\n");
    
    ModelStruc = MakeAModelstruc(bsp);
    bssp->structure = NULL;  /* already linked into modelstruc */
    WriteKinAllModel(ModelStruc, stdout, Color, Render);
    ClearStructures();
  }
  else {
    error_flag = 1;
    PrtMes(MAILTO, "MMDBSRV","Unknown file type on structure fetch.",NULL,TRUE);
  }
  
  if ((Mime == SEE_FILE) && (!error_flag))
    printf ("</PRE></HTML>\r\n");

  NcbiMimeAsn1Free(mime);           /* yanli */
  return 1;
  
} /* end SendStructureMIME */



/* Output the taxonomy data, per chain, and also links to the taxonomy database.
 * Return '1' in case there was a non-trivial taxonomy assignment, '0' if there
 * was no taxonomy data found.  The success or failure is passed ultimately to
 * PrintStructureInfo, where the PDB source is displayed in case no taxonomy data
 * was found.
 */

static Int2
SaveTaxonomyInfoNew(PDNMS ModelStruc, CharPtr save, Int2 n, CharPtr pname)
{
	PMSD	pmsd;
	PDNMM	pdnmm;
	PMMD	pmmd;	
	Int4 i, j, k, cnt, txcnt, uacnt;
	ValNodePtr descr, db;
	CharPtr chainp, names[MAX_CHAINS];
	Char chn, chns[MAX_CHAINS], org_names[MAX_TBUFF];
	Int2 flg, name_here, cnt_vec[MAX_CHAINS];
	BioSourcePtr bios;
	OrgRefPtr org;
	Int4 molecule_type, tax_id, tax_ids[MAX_CHAINS];

	if ((ModelStruc == NULL) || (save == NULL))
		return 0;

	for (i = 0; i < MAX_TBUFF; i++)
		org_names[i] = '\0';

	pmsd = (PMSD) ModelStruc->data.ptrvalue;
	pdnmm = (PDNMM) pmsd->pdnmmHead;

	name_here = 0;

	for (cnt = uacnt = 0; (pdnmm != NULL) && (cnt < MAX_CHAINS); pdnmm = pdnmm->next) {
		/* zap the OrgRefPtr, there may be no taxonomy for this chain! */
		pmmd = pdnmm->data.ptrvalue;
		org = NULL;

		for (descr = pmmd->pMolDescr; descr != NULL; descr = descr->next) {
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
	sprintf(save, "<tr bgcolor=#FFFFCC>\n");
	k = StrLen(save);
	sprintf(&save[k], "<td VALIGN=TOP NOWRAP ALIGN=RIGHT class=H4>");
	k = StrLen(save);
	sprintf(&save[k], "<strong>Taxonomy:</strong></td>\n");

	if (cnt == 0) {
		k = StrLen(save);
		sprintf(&save[k], "<td class=\"TEXT\">(unassigned)</td>\n");
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
			sprintf(&save[k], ";&nbsp;&nbsp;&nbsp;");
		else {
			sprintf(&save[k], "<td class=\"TEXT\">");
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

			if (txcnt < cnt) {
				k = StrLen(save);

				if (txcnt-- > 1)
					sprintf(&save[k], "%c,&nbsp;", chns[j]);
				else
					sprintf(&save[k], "%c", chns[j]);
			}
		}

		k = StrLen(save);

		if (tax_id > 0) {
			if (chns[i] != '\n' && txcnt < cnt)
				sprintf(&save[k], "&nbsp;<A HREF=\"");
			else
				sprintf(&save[k], "<A HREF=\"");

			k = StrLen(save);
			sprintf(&save[k], TAXurl, tax_id);
			k = StrLen(save);
			sprintf(&save[k], "\">%s</A>&nbsp;", names[i]);
		}
		else
			sprintf(&save[k], "&nbsp;(unassigned)");
	}

	k = StrLen(save);
	sprintf(&save[k], "</td>\n</tr>\n");
	return 1;

} /* end SaveTaxonomyInfoNew */




static Int4 SendSummaryPageNew_page_fly(Int4Ptr Uids, Int4 NumToDisplay, Int4Ptr LiveUids, unsigned short * Live, Int2 PageCur, Int4 Save)
{
  	BiostrucPtr 	*bsp;
  	Char 		tax_save[MAX_TBUFF];
 	Int2 		taxret, chaincnt=0, *pageThis, prepg, pgbeg, pgend;
	Int2		totalpg=0, chbeg;
  	Int4 		uid, count;
	Int4 		liveuid;
	unsigned short	live;
  	PDNMS 		*ModelStruc;
  	ValNodePtr 	descr;

  	if (Save == 2)
    		printf ("Content-type: application/octet-stream\n\n");
  	else
    		printf ("Content-type: text/html\n\n");

  	if (!GlobalNonHtmlOutput) {
    	     if (GlobalTitles) WWWPrintFileData(HEADFILE,  stdout);
    	     else
      		printf ("<html>\n<title>MMDB Structure Summary</title><body>");
  	}

	bsp =(BiostrucPtr *)MemNew((size_t)(sizeof(BiostrucPtr) *NumToDisplay));
	ModelStruc = (PDNMS *) MemNew ((size_t)(sizeof(PDNMS) * NumToDisplay));
	pageThis = (Int2Ptr) MemNew ((size_t) (sizeof(Int2) * NumToDisplay));

	prepg = 0;
	for (count = 0; count < NumToDisplay; count++)
	{
		uid = Uids[count];
		if (uid <0) uid = -uid;
	        bsp[count] = (BiostrucPtr) openBSP(uid, 3, 0, TRUE, FALSE, 
						FALSE, &psok, Database);

      		if (bsp[count] == NULL) pageThis[count] = prepg + 1;
		else {
		  	PMSD  pmsd;

			ModelStruc[count] = MakeAModelstruc(bsp[count]);
			pmsd = ModelStruc[count]->data.ptrvalue;
			chaincnt = ChainCount(pmsd);

/* pageThis is the last page no. of This model */

			if (chaincnt == 0) pageThis[count] = prepg + 1;
			else if (chaincnt % ChainPerImg > 0)
			     pageThis[count]=prepg + chaincnt / ChainPerImg +1;
			else  pageThis[count] = prepg + chaincnt / ChainPerImg;
		}
		
		prepg = pageThis[count];
	}
	totalpg += pageThis[count-1];

	pgend = pageThis[0]; count=0;
	while (pgend < PageCur) pgend = pageThis[++count];	

	if (bsp[count] == NULL) {
            printf ("%sNo structure found for UID %d.%s\n",
                		GlobalNonHtmlOutput ? "\n" : "<P><h4>",uid,
                  		GlobalNonHtmlOutput ? "\n" : "</h4><P>");
	    printf("<h4>- UID specified dose not match an MMDB-ID or PDB-ID</h4>\n");
	}	
	else {
		uid = Uids[count];
		liveuid = LiveUids[count];
		live = Live[count];

		if (!GlobalNonHtmlOutput) {   	/* ? */
        	    CharPtr name;

        	    for (descr = bsp[count]->descr, name = NULL; descr != NULL; descr = descr->next) {
          		if (descr->choice == BiomolDescr_name) {
            			name = descr->data.ptrvalue;
            			break;
          	     	}
        	     }
        	    taxret = SaveTaxonomyInfoNew(ModelStruc[count],tax_save,
							MAX_TBUFF, name);
      		}

		PrintStrucInfoNew(ModelStruc[count], liveuid, live, 
		     (taxret) ? tax_save : NULL, uid, stdout);
		
		if (!count) pgbeg = 1;
		else pgbeg = pageThis[count - 1] + 1;
		chbeg = (PageCur - pgbeg) * ChainPerImg + 1;

		PrintChainsMap(bsp[count], ModelStruc[count], chbeg, stdout);
		PrintPageView(NumToDisplay, Uids, totalpg, PageCur, stdout);
	}

	printf("<BR><HR SIZE=5 NOSHADE><BR>\n");

  	if (!GlobalNonHtmlOutput)
    		if (GlobalTitles)
      			WWWPrintFileData(TAILFILE, stdout);
    	else {
		printf ("</body>\n");
      		printf ("</html>");
	}

	for (count = 0; count < NumToDisplay; count++) {
		ClearStructures();
		MemFree(bsp[count]);
		MemFree(ModelStruc[count]);
	}

	MemFree(bsp);
	MemFree(ModelStruc);
	MemFree(pageThis);

  	return 1;

}	/* end 	SendSummaryPageNew */



static Boolean InitMMDBSrv(void)
{
  Int2 i;

  HTMLPath[0] = URLBase[0] = URLHelp[0] = HELPname[0] = CGIPath[0] = URLcgi[0]
  = CGIName[0] = DATApath[0] = LOGpath[0] = ENTREZurl[0] = VASTurl[0]
  = MMDBurl[0] = PDBurl[0] = TDBurl[0] = MAILto[0] = DART[0] = Database[0]
  = LOGIN[0] = PASSWD[0] = INITpath[0] = LIBpath[0] = CDDurl[0] = '\0';


  for (i = 0; i < PATH_MAX; i++)
    TAXurl[i] = '\0';

  GetAppParam("mmdb", "MMDB", "Database", "", Database, PATH_MAX);
  if (Database[0] == '\0')
    {
        ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no Database...\n");
        return FALSE;
    }

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
  GetAppParam("mmdb", "MMDBSRV", "HELPname", "", HELPname, PATH_MAX);
  if (HELPname[0] == '\0')
    {
      	ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDBSRV section no HELPname...\n");
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

  GetAppParam("mmdb", "CD", "DARTdatabase", "", DART, PATH_MAX);
  if (DART[0] == '\0') {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nCD section no DARTdatabase\n");
      return FALSE;
  }

  GetAppParam("mmdb", "CD", "LOGINname", "", LOGIN, PATH_MAX);
  if (LOGIN[0] == '\0') {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nCD section no LOGINname\n");
      return FALSE;
  }

  GetAppParam("mmdb", "CD", "PASSwd", "", PASSWD, PATH_MAX);
  if (PASSWD[0] == '\0') {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nCD section no PASSwd\n");
      return FALSE;
  }

  GetAppParam("mmdb", "CD", "INITpath", "", INITpath, PATH_MAX);
  if (INITpath[0] == '\0') {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nCD section no INITpath\n");
      return FALSE;
  }

  GetAppParam("mmdb", "CD", "LIBpath", "", LIBpath, PATH_MAX);
  if (LIBpath[0] == '\0') {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nCD section no LIBpath\n");
      return FALSE;
  }

  GetAppParam("mmdb", "CD", "CDDurl", "", CDDurl, PATH_MAX);
  if (CDDurl[0] == '\0') {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nCD section no CDDurl\n");
      return FALSE;
  }


  return TRUE;
}



/************************************************************************
 *
 *  Int2 Main() - main function     
 *
 ***********************************************************************/



Int2 Main ()
{
  time_t 		time_now;
  Char 			DispOpt = '\0';
  CharPtr 		TimeNowStr = NULL, pcThis = NULL,  pcName = NULL;
  Int4                  IndexArgs = -1, Complex = 0, MaxModels = 1;
  Int4 			KinColor = 0, KinRender = 0, Save = 0, Form = 0;
  Int4Ptr 		MMDBids = NULL; /* (list of MMDB ids ) */
  Int4                  iMMDBids = 0, iCount = 0, iBytes = 0, NumEntries = 0;
  ValNodePtr		pvnUIDS = NULL, pvnThis = NULL;
  WWWErrorCode          error;

/* J. Chen */
  Int4Ptr 		LiveMMDBids = NULL;  /* list of live MMDB ids */
  unsigned short *      Live = NULL;  /* state of live or non-live */
  CharPtr		Cmd = NULL, pcThis2 = NULL, pcThis3 = NULL;
  Int2			PageNo=1, i, chain1, chain2, imgsize;
  Char			ODBCInitStr[100], LIBPathStr[100];


/* this sets up the unix time limit */  

#ifdef OS_UNIX
 {
  struct rlimit		rl;
  getrlimit(RLIMIT_CPU, &rl);
  rl.rlim_max = rl.rlim_cur = CPUTIME_MAX;
  setrlimit(RLIMIT_CPU, &rl);
 }
#endif 

  if (!InitMMDBSrv()) 
	PrtMes(MAILTO, "MMDBSRV", "Unable to Open Server Configuration File.",
		NULL, FALSE);

  sprintf(ODBCInitStr, "ODBCINI=%s", INITpath);
  putenv(ODBCInitStr);

  sprintf(LIBPathStr, "LD_LIBRARY_PATH=%s", LIBpath); 
  putenv(LIBPathStr);

  if ((dartcon = Dart_Init2(DART, LOGIN, PASSWD)) == NULL) 
	PrtMes(MAILTO, "MMDBSRV", "Dart Initialization failed", NULL, FALSE);

  StringCat(LOGpath, "mmdbsrv.log");
  if( !ErrSetLogfile (LOGpath, ELOG_APPEND) ) 
	PrtMes(MAILTO, "MMDBSRV", "Unable to Open Log. LOGpath = ", 
								LOGpath, FALSE);
  
  ErrSetLogLevel(SEV_MAX); 
  ErrSetOpts (ERR_CONTINUE, ERR_LOG_ON);

  if((error = WWWGetArgs(&info)) != WWWErrOk) {
 	PrtMes(NULL, "MMDBSRV", 
		"Failed to Process Posting - Check your GET/POST syntax", 
		 NULL, TRUE);
    goto earlyout;
  }

 
  if (!OpenMMDBAPI(0, NULL)) {
       ErrLogPrintf("#OpenMMDBAPI failed.\n");
       PrtMes(MAILTO, "MMDBSRV Error", " - Unable OpenMMDBAPI.", NULL, FALSE);
    }

    SeqAsnLoad();
    objmimeAsnLoad();

    if (!MmdbSrvInitialize()) {
     	psok = FALSE;
	ErrLogPrintf("#MMDB Database Access Failed to Initialize.\n");
       	PrtMes(MAILTO, "MMDBSRV",
	   "Unable to Open MMDB Database on Server.", NULL, FALSE);
  }


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
	PrtMes(NULL, "MMDBSRV", "No Identifier specified: missing uid",
		NULL, TRUE);
 else
   {
     /* loop over entire set of tag=value entries and add all the UID's found together */
     /* must handle uid=1234&uid=1OMD&uid=1234,2345,2134&dopt=s&uid=3INS              */   
      NumEntries = WWWGetNumEntries(info);
      for (IndexArgs = 0; IndexArgs < NumEntries; IndexArgs++)
        {
	    pcName = WWWGetNameByIndex(info,  IndexArgs);
	    pcThis = WWWGetValueByIndex(info,  IndexArgs);

	    if (StrICmp(pcName, "uid") == 0) MakeUIDList(pcThis, &pvnUIDS);
	}		 
     if (pvnUIDS == NULL) {
	 PrtMes(NULL, "MMDBSRV", 
		"No structure found for this UID: UID specified is not a MMDB_ID or PDB_ID", NULL, TRUE);
            goto cleanout;
	}
     pvnThis = pvnUIDS;
     while (pvnThis)
       {
          iMMDBids++;
          pvnThis = pvnThis->next;
       }
     MMDBids = (Int4Ptr) MemNew((size_t) (sizeof(Int4) * iMMDBids));

     LiveMMDBids = (Int4Ptr) MemNew ((size_t) (sizeof(Int4) * iMMDBids));
     Live = (unsigned short *) MemNew ((size_t)(sizeof(unsigned short)*iMMDBids));

     for (i=0; i< iMMDBids; i++) Live[i] = 0;

     pvnThis = pvnUIDS;
     iCount = 0;
     while (pvnThis)
     {
          MMDBids[iCount] =  (Int4) pvnThis->data.intvalue;

 	  LiveMMDBids[iCount] = MMDBids[iCount];
	  if (psok) 
          	 i =constructLiveMmdbId(&(LiveMMDBids[iCount]),&(Live[iCount]));
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
  else DispOpt = 's';  /* structure summary is default when no dopt supplied */

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
	    case 4 :
	     MaxModels = INT2_MAX; 
	     break;
	    case 5:
	     Complex = ONECOORDATOM; 
	      break;
	    default:
	       {
		if (DispOpt != 's') {
		    PrtMes(NULL, "MMDBSRV", 
			"Wrong Complexity Type: Complexity = ", pcThis, TRUE);
		    goto cleanout2;
		}
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
    {	/* these don't affect structure summaries */ 
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
	    Save = SEND_FILE; /* writes out asn text without the html header */
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
    
    
/* J. Chen */

  	IndexArgs = -1;
  	PageNo = 1;		/* default */
  	if ((IndexArgs = WWWFindName(info, "page")) >=0)
  	{
		pcThis = WWWGetValueByIndex(info, IndexArgs);
		for (i = 0; i< StrLen(pcThis); i++)
			if (!isdigit(pcThis[i])) break;
		if (i == StrLen(pcThis))
			PageNo = (Int2) atoi(pcThis);
		else {
		   PrtMes(NULL, "MMDBSRV", "Non-numeric Page type: page = ",
			pcThis, TRUE);
		   goto cleanout;
		}
  	}	

	IndexArgs = -1;
	if ((IndexArgs = WWWFindName(info, "cmd")) >=0)
	{
		pcThis = WWWGetValueByIndex(info, IndexArgs);
		Cmd = pcThis;

		if (!StrICmp(Cmd, "graph"))
		{
	    		IndexArgs = -1;
	    		if ((IndexArgs = WWWFindName(info, "chbeg")) >=0)
				pcThis = WWWGetValueByIndex(info, IndexArgs);
	    		else {
			    PrtMes(NULL, "MMDBSRV", "Missing first chain id",
				NULL, TRUE);
			    goto cleanout;
	    		}

			IndexArgs = -1;
			if ((IndexArgs = WWWFindName(info, "chend")) >=0)
				pcThis2 = WWWGetValueByIndex(info,IndexArgs);
            		else {
			    PrtMes(NULL, "MMDBSRV", "Missing last chain id",
				NULL, TRUE);
                	    goto cleanout;
            		}

			IndexArgs = -1;
                        if ((IndexArgs = WWWFindName(info, "imgsize")) >=0)
                                pcThis3 = WWWGetValueByIndex(info,IndexArgs);
                        else {
			    PrtMes(NULL, "MMDBSRV", "Missing image size",
				NULL, TRUE);
                            goto cleanout;
                        }


			for (i = 0; i < StrLen(pcThis); i++) 
				if (!isdigit(pcThis[i])) break;

			if (i == StrLen(pcThis)) chain1 = atoi(pcThis);
			else {
			    PrtMes(NULL, "MMDBSRV",
				"Non-numeric chain id: chbeg = ", pcThis, TRUE);
                   		goto cleanout;
                	}

			for (i = 0; i < StrLen(pcThis2); i++) 
				if (!isdigit(pcThis2[i])) break;
                	if (i == StrLen(pcThis2)) chain2 = atoi(pcThis2);
                	else {
			    PrtMes(NULL, "MMDBSRV", 
				"Non-numeric chain id: chend = ", pcThis, TRUE);
                   	    goto cleanout;
                	}

                        for (i = 0; i < StrLen(pcThis3); i++)
                                if (!isdigit(pcThis3[i])) break;
                        if (i == StrLen(pcThis3)) imgsize = atoi(pcThis3);
                        else {
			    PrtMes(NULL, "MMDBSRV",
				"Non-numeric image size: imgsize = ", pcThis,
				 TRUE);
                            goto cleanout;
			}

            	}
	
	}



/* J. Chen (end) */

 if  (DispOpt == 's') {
       if ((Cmd != NULL)  && !StrICmp(Cmd, "graph")) 
		iBytes = DrawImg(MMDBids, chain1, chain2, imgsize); 
	else 
       		iBytes = SendSummaryPageNew_page_fly(MMDBids, iMMDBids,  
					LiveMMDBids, Live, PageNo,Save);
   }
 else 
	iBytes = SendStructureMIME(DispOpt,  MMDBids[0], Save  , 
                         Complex, MaxModels,  KinColor, KinRender);
 
   MemFree(MMDBids);
   MemFree(LiveMMDBids);
   MemFree(Live);
 
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
  fflush(stdout);  
  Dart_Fini(dartcon);
  if (psok) MmdbSrvFinish();
/*   else MMDBFini(); */
  exit(0);
  
cleanout2:
  MemFree(MMDBids);  
  CloseMMDBAPI();
  if (psok)  MmdbSrvFinish();
 /*  else MMDBFini(); */
  Dart_Fini(dartcon);
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
  if (psok) MmdbSrvFinish();
 /*  else MMDBFini();*/
  Dart_Fini(dartcon);

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

