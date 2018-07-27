/*
* $Id: PrintBanner.c,v 1.2 2003/01/22 17:12:50 chenj Exp $
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
* $Log: PrintBanner.c,v $
* Revision 1.2  2003/01/22 17:12:50  chenj
* Get Cn3D 4.1
*
* Revision 1.1.1.1  2002/12/06 20:17:21  chenj
* Imported Scouces
*
*
*
*
* This file is used to print the html head.
*
* ==========================================================================
*/




#include <ncbi.h>
#include <accentr.h>
#include "vastgenDB.h"

#define DISP_IN_GRAPH 	'n'
#define DISP_IN_TABLE	'y'

extern	char 	URLcgi[PATH_MAX];
extern	char	CGIname[PATH_MAX];
extern 	char	MMDBCGIname[PATH_MAX];
extern 	Uint1	numSubsets;
extern 	Boolean	SubsetInfoLoaded;
/*extern	CTLibUtilsPtr   db_conn1; */
extern	Int4	aSdi;
	
void PrintHitsSortBanner(FILE *table, SortBy sortby, SubSetID subsetnum,
	Int4 pagenum, Int4 numpages, Char cTable)
{

	Int4 i, j, k;
	static char subsetname[6][100]={"NR, Blast_p<10e-7",
					"NR, Blast_p<10e-40",
					"NR, Blast_p<10e-80",
					"Non-identical seq.",
					"All of MMDB", 
					"Selected neighbor(s)"};
					
	fprintf(table, "<TR>\n<TD>\n");

/* Use the Form created in PrintAlignViewBanner */
	fprintf(table, "<input type=hidden name=presubset value=%d>\n",
			subsetnum);

    	fprintf(table, "<table cellpadding=0>   ");
	fprintf(table, "<!-- Subtable for display/sort hits -->\n");

/* Insert a blank row */
        fprintf(table, "<tr><td></td></tr>\n\n");

    	fprintf(table, "<tr>\n<td class=SMALL1>");
	fprintf(table, "<strong><INPUT TYPE=SUBMIT NAME=dispsub VALUE=List>");
	fprintf(table, "&nbsp;&nbsp;subset&nbsp;");

    	fprintf(table, "</strong>\n");

	fprintf(table, "\n<select name=subset>\n");
	
/* subset 1 should be "All of PDB"; put it second last */
	if (! SubsetInfoLoaded) loadSubsetInfo();
	for (i=2; i< numSubsets; i++) {
		fprintf(table, "<option value=\"%s\"", GetSubsetName_DB(i));
		if (i == subsetnum) fprintf(table, " selected");
		fprintf(table, ">%s\n", subsetname[i-2]); 
		/* fprintf(table, ">%s\n", GetSubsetName_DB(i));*/
	}
	
	fprintf(table, "<option value=\"%s\"", GetSubsetName_DB(1));
	if (subsetnum ==1) fprintf(table, " selected");
	fprintf(table, ">%s\n", subsetname[4]); 
	/* fprintf(table, ">%s\n", GetSubsetName_DB(1)); */

	fprintf(table, "<option value=\"%s\"", GetSubsetName_DB(6));
	if (subsetnum ==6) fprintf(table, " selected");
	fprintf(table, ">%s\n", subsetname[5]); 

	fprintf(table, "</select>\n");

	fprintf(table, "<strong>&nbsp;sorted by&nbsp;</strong>\n");

	fprintf(table, "<select name=sort>\n");
	fprintf(table, "<option value=%d", NRes);
	if (!sortby || sortby== NRes) fprintf(table, " selected");
	fprintf(table, ">Aligned Length\n");
	fprintf(table, "<option value=%d", VScore);
	if (sortby == VScore) fprintf(table, " selected");
	fprintf(table, ">Vast Score\n");
	fprintf(table, "<option value=%d", PValue);
	if (sortby== PValue) fprintf(table, " selected");
	fprintf(table, ">Vast P_value\n");
	fprintf(table, "<option value=%d", Rmsd);
	if (sortby == Rmsd)
		fprintf(table, " selected");
	fprintf(table, ">Rmsd\n");
	fprintf(table, "<option value=%d", PcntId);
	if (sortby == PcntId) fprintf(table, " selected");
	fprintf(table, ">Seq. Identity\n");
	fprintf(table, "</select>\n");

	if (pagenum && numpages > 1) {
		fprintf(table, "<strong>&nbsp;page&nbsp;</strong>\n");
		fprintf(table, "<select name=doclistpage>\n");
		for (i =1; i<= numpages; i++) { 
	     	  if (i == pagenum)
		      fprintf(table, "<option value=%d selected>%d\n", i, i);
	     	  else fprintf(table, "<option value=%d>%d\n", i, i);
		}
		fprintf(table, "</select>\n");
	}

	fprintf(table, "<strong>&nbsp;in&nbsp;</strong>\n");
	fprintf(table, "<select name=table>\n");
	fprintf(table, "<option value=n");
	if (cTable == DISP_IN_GRAPH) 
		fprintf(table, " selected");
	fprintf(table, ">Graphics\n");
	fprintf(table, "<option value=y");
	if (cTable == DISP_IN_TABLE)
		fprintf(table, " selected");
	fprintf(table, ">Table\n");
	fprintf(table, "<option value=s>ASN1\n"); 
	fprintf(table, "</select>\n");

	fprintf(table, "</td>\n</tr>\n</table>\n\n</TD>\n</TR>\n\n");

} /* end of PrintHitsSortBanner */



void 
PrintAlignViewBanner(FILE *table, Int4 iFSID, VastPageDataPtr vpp, 
	Int4 numhitsdisplayed, CharPtr JobID, CharPtr pcPass)
{
    Int4  i;
    Char  str[50], AllId[MAX_TBUFF];

    fprintf(table, "<TR>\n<TD>\n"); 

/* Create one Form with 3 submits */

    fprintf(table, "<Form method=get action=\"%s%s\">\n", URLcgi, CGIname);
    if (JobID) 
	fprintf(table, "<input type=hidden name=chaindom value=%d>\n", iFSID);
    else fprintf(table, "<input type=hidden name=sdid value=%d>\n", aSdi);

    if (JobID) {
	fprintf(table, "<input type=hidden name=vsid value=\"%s\">\n", JobID);
	fprintf(table, "<input type=hidden name=pass value=\"%s\">\n", pcPass);
    }

    sprintf(AllId, "%d,", iFSID*10);
    for (i=0; i< numhitsdisplayed; i++) {

	sprintf(str, "%d,", vpp[i].bBsfId);
	StringCat(AllId, str);
    }
    fprintf(table, "<input type=hidden name=allbsfid value=\"%s\">\n", AllId);

    fprintf(table, "<input type=hidden name=defhit value=%d>\n",vpp[0].bBsfId);

    fprintf(table, "<table cellpadding=0>");
    fprintf(table, "   <!-- Subtable for View/Save Structure-->\n");

    fprintf(table, "<tr>\n<td class=SMALL1>\n");
    fprintf(table, "<strong>");
    fprintf(table,"<INPUT TYPE=SUBMIT NAME=viewstr VALUE=\"View 3D Structure\">");
    fprintf(table, "&nbsp;&nbsp;of&nbsp;</strong>\n\n");

    fprintf(table, "<select name=atm_complexity>\n");
    fprintf(table, "<option value=0>All Atoms\n");
    fprintf(table, "<option value=1>Backbone\n");
    fprintf(table, "</select>\n\n");

    fprintf(table, "<strong>&nbsp;with&nbsp;</strong>\n\n");

    fprintf(table, "<select name=calltype>\n");
    fprintf(table, "<option value=c>Cn3D\n");
    fprintf(table, "<option value=a>Cn3D/Cache\n");
    fprintf(table, "</select>\n\n");

    fprintf(table, "<select name=action>\n");
    fprintf(table, "<option value=0>Display\n");
    fprintf(table, "<option value=1>See File\n");
    fprintf(table, "<option value=2>Save File\n");
    fprintf(table, "</select>\n\n");


    fprintf(table, "<img src=\"/Structure/new.gif\" alt=\"New\">\n");
    fprintf(table, "<a href=\"/Structure/CN3D/cn3d.shtml\"><B><I>Get Cn3D 4.1!</I></B></a>");

    fprintf(table, "</td>\n</tr>\n</table>\n\n");
    fprintf(table, "</TD>\n</TR>\n\n");



    fprintf(table, "<TR>\n<TD>\n");
    fprintf(table, "<table cellpadding=0>  "); 
    fprintf(table, "<!-- Subtable for View Alignments -->\n");

/* Insert a blank row */
    fprintf(table, "<tr><td></td></tr>\n\n");


    fprintf(table, "<tr>\n<td class=SMALL1>\n");
    fprintf(table, "<strong>");
    fprintf(table,"<INPUT TYPE=SUBMIT NAME=viewali VALUE=\"View Alignment\">");
    fprintf(table, "&nbsp;&nbsp;using&nbsp;</strong>\n\n");
    
    fprintf(table, "<select name=alitype>\n");
    fprintf(table, "<option value=h>Hypertext\n");
    fprintf(table, "<option value=t>Plain text\n");
    fprintf(table, "<option value=f>mFASTA\n");
    fprintf(table, "</select>\n\n");
 
    fprintf(table, "<strong>&nbsp;for&nbsp;</strong>\n");

    fprintf(table, "<select name=nbr_complexity>\n");
    fprintf(table, "<option value=1>Selected\n");
    fprintf(table, "<option value=0>All on page\n");
    fprintf(table, "</select>\n\n");
    
    fprintf(table, "<strong>&nbsp;VAST neighbors</strong>\n");
    fprintf(table, "</td>\n</tr>\n</table>\n");
    fprintf(table, "</TD>\n</TR>\n\n");

} /* end PrintAlignViewBanner */



void
PrintQueryInfo(FILE *table, CharPtr pcPDB, Char cChain, int iDomain, 
		Int4 iMMDBid, CharPtr JobID)
{

	  Char 	title[MAX_TBUFF];
	
	  fprintf(table, "<TR>\n<TD>\n");
	  fprintf(table, "<table cellpadding=0>  <!-- Subtable for description-->\n");

/* insert two blank rows for spacing */
	  fprintf(table, "<tr>\n");
	  fprintf(table, "<td VALIGN=TOP NOWRAP> </TD>\n");
	  fprintf(table, "</tr>\n\n");

	  fprintf(table, "<tr>\n");
	  fprintf(table, "<td width=15%% VALIGN=TOP NOWRAP ALIGN=RIGHT class=H4>");
	  fprintf(table, "<strong>Query:</strong></TD>\n");
	  fprintf(table, "<td class=TEXT>");
	  if (iMMDBid && !JobID) {
	     fprintf(table, "MMDB&nbsp;");
	     fprintf(table, "<a href=\"%s%s?uid=%s",URLcgi, MMDBCGIname, pcPDB);
	     fprintf(table, "&form=6&db=t&Dopt=s\">%d</a>", (long)iMMDBid);
	     fprintf(table, ",&nbsp;");
	  }

	  if (pcPDB[0] != NULLB) fprintf(table, "%s", pcPDB); 
  
	  if (cChain != ' ')
		fprintf(table, "&nbsp;chain&nbsp;%c", cChain);

	  if (iDomain > 0)
		fprintf(table, "&nbsp;domain&nbsp;%d", (int) iDomain);
	
	  if (JobID != NULL) {
		fprintf(table, ",&nbsp;");
		fprintf(table, "&nbsp;Vast Search Job %s", JobID);
	  }
	

	  fprintf(table, " </td>\n</tr>\n\n");
	  if (!JobID && constructPdbDescr(iMMDBid, title, MAX_TBUFF)) {
	      fprintf(table, "<tr>\n");
              fprintf(table, "<td VALIGN=TOP NOWRAP ALIGN=RIGHT class=H4>\n");
              fprintf(table, "<strong>Description:</strong></TD>\n");
              fprintf(table, "<td class=TEXT>%s", title);
              fprintf(table, "</td>\n</tr>\n\n");
	  }


	  fprintf(table, "</table>\n");
	  fprintf(table, "</TD>\n</TR>\n\n");

} /* end PrintQueryInfo */



void PrintSearchNbr(FILE *table)
{

	Int4 i, j; 

	fprintf(table, "<TR>\n<TD>\n");
	fprintf(table, "<table cellpadding=0>       ");
	fprintf(table, "<!-- Subtable for serach of str. neighbor -->\n");

/* Insert a blank row */
        fprintf(table, "<tr><td></td></tr>\n\n");

/* use the form created in PrintAlignViewBanner */

	fprintf(table, "<tr>\n<td class=SMALL1>\n<strong>\n");
	fprintf(table, "<input type=submit name=schsub value=Find>\n"); 

	fprintf(table,"&nbsp;MMDB or PDB ids:&nbsp;</strong></td>\n");
	/* fprintf(table, "<input type=text name=selnbr size=22>"); */
	fprintf(table, "<td><textarea name=selnbr cols=18 rows=1></textarea></td>\n");
	
	fprintf(table,"<td class=SMALL1><strong>&nbsp;&nbsp;or 3D-Domain ids:&nbsp;</strong></td>\n");
	/* fprintf(table, "<input type=text name=selsdid size=22>");*/
	fprintf(table, "<td><textarea name=selsdid cols=18 rows=1></textarea>");


	fprintf(table, "</td>\n</tr>\n\n");

	fprintf(table, "</table>\n");
	fprintf(table, "</TD>\n</TR>\n\n");


}	/* PrintSearchNbr */

