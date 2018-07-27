/*
 * $Id: vastsrvDB.c,v 1.8 2003/01/31 16:11:44 chenj Exp $
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
 * File Name: vastsrvDB.c --- The database version of vastsrv.c
 *
 * Author: Christopher Hogue, Tom Madej, Siqian He, Jie Chen
 *
 *
 * $Log: vastsrvDB.c,v $
 * Revision 1.8  2003/01/31 16:11:44  chenj
 * set ODBCINI correctly
 *
 * Revision 1.7  2003/01/15 17:15:39  chenj
 * delete all SelfNbr stuff
 *
 * Revision 1.6  2003/01/15 16:24:52  chenj
 * Change some posting messages
 *
 * Revision 1.5  2003/01/14 21:15:19  chenj
 * change VastToCn3d() to VastToCn3DAndAli()
 *
 *
 *
 * This file with others together produce graphical display of structure 
 * alignment.
 *
 *
 * ===========================================================================
 *
 */

/* Main program for VAST structure neighbor server. */

/* Use vastlocl.c instead of vastdtbs.c because it's faster to read in the
 * existing BiostrucAnnotSet file than to load it from database.
 */

/* define DEBUG_1 1 */

#include "vastuti.h"

#define CPUTIME_MAX		120
#define DEFAULT_SUBSET_NUM    	NRBlast10e_40 	/* the NR set BLAST 10e-40 */
#define DEFAULT_SORT_BY		NRes 	/* Number of Aligned Residues */
#define NUM_HITS_PER_PAGE	60
#define DEFAULT_PAGE            1
#define DEF_ALL_NBR		1
#define SELECTED_NBR		0
#define VIEW_STR_SUBMIT		0
#define VIEW_ALI_SUBMIT		1
#define DEF_LIST_SUBMIT		2
#define FIND_SUBMIT		3
#define MAX_KEPT_NBR		30
#define DOMID_SIZE              7       /* used to be 6, as length of domId */
#define MaxVPP			150

#define HEADFILE   "sshead.txt"

Char 		VSPATH[PATH_MAX];
static Char 	URLBase[PATH_MAX];
Char 		URLcgi[PATH_MAX];
Char 		DATApath[PATH_MAX];
static Char 	VASTpath[PATH_MAX];
Char 		ENTREZurl[PATH_MAX];
static Char 	DOCSUMurl[PATH_MAX];
Char 		MAILto[PATH_MAX];
Char 		CGIname[PATH_MAX];
Char 		MMDBCGIname[PATH_MAX];
static Char 	HELPname[PATH_MAX];
Char		Database[PATH_MAX];

static Char 	DART[PATH_MAX];
static Char 	LOGIN[PATH_MAX];
static Char 	PASSWD[PATH_MAX];
static Char 	INITpath[PATH_MAX];
static Char 	LIBpath[PATH_MAX];
Char		CDDurl[PATH_MAX];
Int4		aSdi = 0;

Uint1 			numSubsets = 0;
SubsetNameData 	subsetNames[6];
Boolean SubsetInfoLoaded = FALSE;
Dart_Connect	*dartcon;
extern BiostrucAnnotSetPtr LocalGetBiostrucAnnotSet(Int4 mmdbid, CharPtr JobID);
Boolean		psok;


static Uint1 getSubsetNbr(Char *subname)
{
  Uint1 i = 0;
  if(! SubsetInfoLoaded) loadSubsetInfo();
  for(i = 0; i < numSubsets; i++) 
    if(strcmp(subsetNames[i].subName,subname)==0)
      return subsetNames[i].subId;

  PrtMes(MAILto, "VASTSRV", "GetSubsetNumber_DB() failed.", NULL, FALSE);

} /* getSubsetNbr() */




static void 
VastTableBegin (FILE *table, Int4 iMMDBid, Int4 iFSID, SortBy sortby,
	SubSetID subsetnum, Int4 iKept,  CharPtr NonNbr, VastPageDataPtr vpp, 
	Int4 numhitsdisplayed, Int4 numhits, Int4 numpages,
	Int4 pagenum, char cTable, Int4 NbrFlag, CharPtr JobID, CharPtr pcPass)
{
	 Int4 domNo;
	 Char pdbId[5], chainLett;


/* J. Chen */

	StrCut(pdbId, vpp[0].aDomName, 1, 4);
	chainLett = vpp[0].aDomName[4];
	domNo = (Int4) atol(vpp[0].aDomName+5);

        fprintf(table, "Content-type: text/html\n\n");
        WWWPrintFileData(HEADFILE,  stdout);

        fprintf(table, "<TABLE width=800 BORDER=0 CELLPADDING=3 CELLSPACING=0 bgcolor=#FFFFCC>\n\n");
        PrintQueryInfo(table, pdbId, chainLett, domNo, iMMDBid, JobID);

  /* insert a blank row for spacing */
 /*       fprintf(table, "<TR>\n<TD VALIGN=TOP NOWRAP></TD>\n</TR>\n\n"); */

        PrintAlignViewBanner(table, iFSID, vpp, numhitsdisplayed,JobID,pcPass);

        PrintHitsSortBanner(table, sortby, subsetnum, pagenum, numpages,cTable);

	PrintSearchNbr(table);

        fprintf(table, "</TABLE>\n\n");
        fflush(table);

/* J. Chen */

    fprintf(table, "<br>\n"); 
    fprintf(table, "<font class=SMALL1><strong>");
    if (NbrFlag == DEF_ALL_NBR) {
	if (numhitsdisplayed > iKept) {
            int numtmp;

            numtmp  = numhits - numhitsdisplayed + iKept;
            if (numtmp)
                fprintf(table, "%d out of %d ", numhitsdisplayed-iKept,numhits);
            else fprintf(table, "%d ", numhits);
            if (iKept)
                fprintf(table, "with %d additional selected ", iKept);
            if (numhitsdisplayed > 1)
                fprintf(table, "structure neighbors displayed");
            else fprintf(table, "structure neighbor displayed");

        }
        else if (iKept > 1)
                fprintf(table,"%d selected neighbors displayed",iKept);
        else fprintf(table, "1 selected neighbor displayed");
    }
    else {
	if (numhitsdisplayed > iKept)  {
            int numtmp;

            numtmp = numhitsdisplayed - iKept;

            if (numtmp>1)
                fprintf(table, "%d neighbors found", numtmp);
            else fprintf(table, "1 neighbor found");
            if (iKept>1)
                fprintf(table, ", and displayed with %d additional selected ones",iKept);
            else if (iKept ==1)
                fprintf(table, ", and displayed with 1 additional selected neighbor");

	}
        else if (iKept >1)
                fprintf(table, "%d selected neighbors displayed", iKept);
        else fprintf(table, "1 selected neighbor displayed");

        if (NonNbr && NonNbr[0] != NULLB) {
	   Char str[MAX_TBUFF];
	
	   if (strchr(NonNbr, ','))
		sprintf(str, " are not structure neighbors");
	   else sprintf(str, " is not a structure neighbor");
	   if (StrLen(NonNbr) <= 41) 
	      fprintf(table,", but <font color=#CC6600>%s</font>%s",NonNbr,str);
	   else fprintf(table, ", but others are not structure neighbors");
 	}
    }
    fprintf(table, ".</strong></font>\n");

} /* end of VastTableBegin */




static Int2 CalCellHForCDs(Int4 Fsid)
{
  CharPtr	*CddName;
  Int4 		i, j, k, gi, seqlen, numseg, from, to, y, maxy=0, NumRows;
  Int2		CdNum, *iClus;
  Int4Ptr	starts, lens;
  OverLoc       *head, *end;
  DenseSegPtr   *dsp;
  SeqAnnotPtr   sap = NULL;
  SeqAlignPtr   salp= NULL;
  unsigned	*PssmId, *Pssmlist, count; 
 
  gi = constructGi(Fsid/10000, (Fsid % 10000)/100);
  seqlen = constructChainLength(Fsid/10000, (Fsid % 10000)/100); 

  if (Dart_Gi2Sap(dartcon, gi, &sap, NULL) && (sap != NULL)) {

    if (!Dart_CdNum(dartcon, &count))
            PrtMes("chenj@ncbi.nlm.nih.gov",
                        "MmdbSrv", "Can't do Dart_CdNum()", NULL, FALSE);
    Pssmlist = (unsigned *) MemNew (count * sizeof (unsigned));

    end=head = NewOverLoc(seqlen);
    head->y = 90+FontBH;
    head->next = NULL;

    for (salp = (SeqAlignPtr)sap->data, CdNum=0; salp!=NULL; 
		salp = salp->next, CdNum++);

    dsp = (DenseSegPtr *) MemNew (CdNum * sizeof(DenseSegPtr));
    PssmId = (unsigned *) MemNew (CdNum * sizeof(unsigned));
    iClus = (Int2Ptr) MemNew (CdNum * sizeof(Int2));
    CddName = (CharPtr *) MemNew (CdNum * sizeof(CharPtr));
    for (j=0; j< CdNum; j++) CddName[j] = (CharPtr) MemNew (30);

    for (salp = (SeqAlignPtr)sap->data, i=0; salp!=NULL; salp=salp->next,i++) {
      dsp[i] = (DenseSegPtr)salp->segs;
      PssmId[i] = GetPSSMID(dsp[i]);
      iClus[i] = -1;

      if (PssmId[i]) Dart_CDGi2Acc(dartcon, PssmId[i], CddName[i], 30);
      else {
      	CharPtr strtmp;

        strtmp = GetCDDName(dsp[i]);
        sprintf(CddName[i], strtmp);
      }
    }

    for (i=0; i< CdNum; i++) {

        if (iClus[i] >= 0) continue;
        iClus[i] = i;

        if (Dart_Related(dartcon, CddName[i],Pssmlist,count,&NumRows, NULL)) {
	
	  if (NumRows > count)
	     PrtMes("chenj@ncbi.nlm.nih.gov", "VASTSRV",
		"Dart_Related(): NumRows>MaxPssm", NULL, FALSE);

          for (j=0; j< NumRows; j++)
            for (k=i+1; k< CdNum; k++)
              if (PssmId[k] == Pssmlist[j]) iClus[k] = i;

        }
    }

    for (i=0; i< CdNum; i++) {
      numseg = dsp[i]->numseg;
      starts = dsp[i]->starts;
      lens = dsp[i]->lens;
      from = starts[0]+1;
      to = starts[(numseg-1)*2] + lens[numseg-1];
      y = GetY1(head, &end, from , to, seqlen, iClus[i], 5);
      if (y >=0) y += FontBH+2;
      else y = maxy;
      maxy = MAX(maxy, y);
    }

    FreeOverLoc(head);
    MemFree(dsp);
    MemFree(PssmId);
    MemFree(iClus);
    for (i=0; i< CdNum; i++) MemFree(CddName[i]);
    MemFree(CddName);
    MemFree(Pssmlist);

  }

  if (!maxy) maxy = 90;
  return(maxy);

}	/* end of CalCellHForCDs */



static void 
VastInfoRows(FILE *table, Int4 iFSID, CharPtr JobID, CharPtr pcPass, 
	VastPageDataPtr vpp, Int4 numhitsdisplayed, Int4 iKept,
	CharPtr selnbrstring, CharPtr selsdidstring, SortBy sortby, 
	SubSetID subsetnum, Int4 pagenum)
{
  Int4 	i, cellh;
  Char 	str[100];

   fprintf(table, "<TABLE border=0 CELLPADDING=0 CELLSPACING=0 width=800>\n");
   fprintf(table, "                     <!-- table for checkbox -->\n");

   if (JobID) cellh = 100;
   else cellh = CalCellHForCDs(iFSID); 
   fprintf(table, "<tr>\n");
   fprintf(table, "<td height=%d valign=middle><br></td>\n", cellh);
   fprintf(table, "<td width=770 rowspan=%d>\n", numhitsdisplayed+1);
   fprintf(table, "<img src=");
   if (JobID) fprintf(table, VSParURL, URLcgi, CGIname, iFSID, sortby); 
   else fprintf(table, ParURL, URLcgi, CGIname, aSdi, sortby); 
   if (selnbrstring == NULL && selsdidstring == NULL) {
	if (pagenum) 
	    fprintf(table, PageSubsetURL, pagenum, subsetnum, subsetnum);
	else fprintf(table, "schsub=Find");
   }
   else {
     fprintf(table, "schsub=Find");
     if (selnbrstring) fprintf(table, "&selnbr=%s", selnbrstring);
     if (selsdidstring) fprintf(table, "&selsdid=%s", selsdidstring);
   }

   if (iKept) 
	for (i=0; i< iKept; i++) 
	    fprintf(table, "&hit=%d", vpp[i].bBsfId);
   if (JobID) fprintf(table, "&vsid=%s&pass=%s", JobID, pcPass);

   fprintf(table,"&cmd=graph&imgsize=%d\" usemap=#chain_map border=0 ismap>",
		(numhitsdisplayed*30 + cellh));
   fprintf(table, "\n</td>\n</tr>\n\n");

   fprintf(table, "<map name=chain_map>\n");

   ImgMapOrDraw(TRUE, iFSID, vpp, numhitsdisplayed, iKept, selnbrstring, 
	selsdidstring,	sortby, subsetnum, pagenum, cellh, JobID, pcPass,table);
   fprintf(table, "</map>\n");

  for (i = 0; i < numhitsdisplayed; i++) {

    fprintf(table, "<tr>\n");
    fprintf(table, "<td VALIGN=TOP height=30 width=30 class=SMALL1>\n");
    fprintf(table, "<INPUT TYPE=checkbox NAME=hit VALUE=%d", vpp[i].bBsfId);
    if (i < iKept) fprintf(table, " checked");
    fprintf(table, "></td>\n");
    fprintf(table, "</tr>\n\n");

    fflush(table);

  } /* end of for */

  fprintf(table, "</TABLE>\n\n");
  fprintf(table, "</FORM>\n"); 		/* close Form opened in PrintAlignView */

} /* end of VastInfoRows */



#define SetTd	"<td VALIGN=top ALIGN=center>"
#define SetFont "<font color=#CC6600>"

static void 
VastTableRows(FILE *table, CharPtr JobID, VastPageDataPtr vpp, 
	Int4 numhitsdisplayed, Int4 iKept, SortBy sortby)
{
  Int4 	bMMDBid;
  Uint1 domNo;
  Char 	pcSlaveName[2 * DBStrSize + 4];
  float f;
  int i;

   fprintf(table, "<br>\n");
   fprintf(table, "<TABLE cellspacing=3 cellpadding=2 width=800 border=1>\n");
   fprintf(table,"<tr valign=middle>\n");
   fprintf(table,"<th>&nbsp;</th>\n");
   fprintf(table,"<th align=left><pre> <a href=\"%s/vasthelp.html#Structure\">PDB</a>", VASTpath);
   fprintf(table," <a href=\"%s/vasthelp.html#C\">C</a>", VASTpath);
   fprintf(table," <a href=\"%s/vasthelp.html#D\">D</a></pre></th>\n",VASTpath);

   fprintf(table,"<th><pre><a href=\"%s/vasthelp.html#NRES\">", 
		VASTpath);
   if (sortby == NRes) 
	fprintf(table, "%sAli. Len.</font></a></pre></th>\n", SetFont);
   else fprintf(table, "Ali. Len.</a></pre></th>\n");

   fprintf(table,"<th><pre><a href=\"%s/vasthelp.html#SCORE\">", 
		VASTpath);
   if (sortby == VScore) 
	fprintf(table, "%sSCORE</font></a></pre></th>\n", SetFont);
   else fprintf(table, "SCORE</a></pre></th>\n");

   fprintf(table,"<th><pre><a href=\"%s/vasthelp.html#P-VAL\">", 
		VASTpath);
   if (sortby == PValue)
	fprintf(table, "%sP-VAL</font></a></pre></th>\n", SetFont);
   else fprintf(table, "P-VAL</a></pre></th>\n");

   fprintf(table,"<th><pre><a href=\"%s/vasthelp.html#RMSD\">", 
		VASTpath);
   if (sortby == Rmsd) 
	fprintf(table, "%sRMSD</font></a></pre></th>\n", SetFont);
   else fprintf(table, "RMSD</a></pre></th>\n");

   fprintf(table,"<th><pre><a href=\"%s/vasthelp.html#Id\">", 
		VASTpath);
   if (sortby == PcntId)
	fprintf(table, "%s%%Id</font></a></pre></th>\n", SetFont);
   else fprintf(table, "%%Id</a></pre></th>\n");

   fprintf(table,"<th><pre><a href=\"%s/vasthelp.html#Contents\">Description</pre></th>\n",VASTpath);
   fprintf(table,"</tr><br>\n");
   fflush(table);


  for (i = 0; i < numhitsdisplayed; i++) {
      
    Char	pdbId[5], strtmp[100];

    bMMDBid = vpp[i].bBsfId / 100000;
    domNo = (Int4) atoi(vpp[i].bDomName+5);

    pcSlaveName[0] = NULLB;
    if(vpp[i].name[0] != NULLB || vpp[i].name2[0] != NULLB) {
      if(vpp[i].name[0] != NULLB) strcpy(pcSlaveName,vpp[i].name);
      if(vpp[i].name2[0] != NULLB) strcat(pcSlaveName,vpp[i].name2);
    }
    else if (JobID) 
	constructPdbDescr(bMMDBid, pcSlaveName, 2 * DBStrSize + 4);

    fprintf(table, "<tr>\n");
    fprintf(table, "<td VALIGN=TOP>"); 

    fprintf(table, "<INPUT TYPE=checkbox NAME=hit VALUE=%d", vpp[i].bBsfId);
    if (i < iKept) fprintf(table, " checked");
    fprintf(table, "></td>\n");
    fprintf(table, "<td VALIGN=TOP><pre>");
    StrCut(pdbId, vpp[i].bDomName, 1, 4);
    fprintf(table, "<a href=\"%s%s?uid=%ld&form=6&db=t&Dopt=s\">%s</a>", 
		URLcgi, MMDBCGIname, (long) bMMDBid, pdbId);

    if (vpp[i].bDomName[4] == ' ') fprintf(table,"&nbsp;");
    else
      fprintf(table," <a href=\"%s%s?uid=%ld&form=6&db=t&Dopt=s\">%c</a>", 
		URLcgi, MMDBCGIname, (long) bMMDBid, vpp[i].bDomName[4]);

    if (domNo > 0)
      fprintf(table," <a href=\"%s%s?uid=%ld&form=6&db=t&Dopt=s\">%d</a></pre></td>\n", 
		URLcgi, MMDBCGIname, (long) bMMDBid, domNo);
    else
      fprintf(table," </pre></td>\n");

    if (vpp[i].nres > 0) {
      	fprintf(table,"%s", SetTd);
	if (sortby == NRes)
	    fprintf(table, "%s%d</font></td>\n", SetFont, vpp[i].nres);
	else fprintf(table, "%d</td>\n", vpp[i].nres);
    }
    else fprintf(table,"<td> </td>\n");

    if (vpp[i].vScore > 0) {
        f = (FloatLo) (vpp[i].vScore);
        f = f/(FloatLo) ASP_SCALE_FACTOR;
        fprintf(table, "%s", SetTd);
	if (sortby == VScore)
	    fprintf(table, "%s%.1f</font></td>\n",SetFont, f);
	else fprintf(table, "%0.1f</td>\n", f);
    }
    else fprintf(table, "<td> </td>\n");

    if (vpp[i].mlogp > 0) {
        f = (float) (vpp[i].mlogp);
        f = f/(float) ASP_SCALE_FACTOR;

        /* adjust for database size */
        f -= LOG10_500;

        if (f <= 4.0) {
          f = (float) exp(-LOG_10*f);
          fprintf(table, "%s", SetTd);
	  if (sortby == PValue)
	      fprintf(table, "%s%.4f</font></td>\n", SetFont, f);
	  else fprintf(table, "%.4f</td>\n", f);
        }
        else {
          fprintf(table, "%s", SetTd);
	  if (sortby == PValue) 
	      fprintf(table, "%s10e-%.1f</font></td>\n", SetFont, f);
	  else fprintf(table, "10e-%.1f</td>\n", f);
	}
    }
    else fprintf(table,"<td> </td>\n");

    if (vpp[i].rmsd > 0) {
      f = (FloatLo) (vpp[i].rmsd);
      f = f/(FloatLo) ASP_SCALE_FACTOR;
      fprintf(table, "%s", SetTd);
      if (sortby == Rmsd) 
	fprintf(table, "%s%.1f</font></td>\n", SetFont, f);
      else fprintf(table,"%.1f</td>\n", f);
    }
    else fprintf(table,"<td> </td>\n");  

    if (vpp[i].pcntId > 0) {
      f = (FloatLo) (vpp[i].pcntId);
      f = f/(FloatLo) ASP_SCALE_FACTOR;
      fprintf(table, "%s", SetTd);
      if (sortby == PcntId) 
      	  fprintf(table, "%s%.1f</font></td>\n", SetFont, f * 100.0); 
      else fprintf(table, "%.1f</td>\n", f*100.0);
    }
    else
      fprintf(table,"<td VALIGN=top ALIGN=center>0.0</td>\n");  

    fprintf(table,"<td VALIGN=top>\n");
    fprintf(table,"%s" , pcSlaveName);
    fprintf(table,"<BR>\n");
    fprintf(table,"</td>\n</tr>\n");
    fflush(table);
  } /* end of for */


  fprintf(table, "</TABLE>\n\n");
  fprintf(table, "</FORM>\n"); 		/* close Form opened in PrintAlignView */
} /* end of VastTableRows */



static void 
SaveBioAnnotSetInFile(Int4 Fsid, SubSetID subsetnum, SortBy sortby, CharPtr JobID)
{
        Int4                  	numhitsdisplayed, alloc_size;
        BiostrucAnnotSetPtr     pbsa = NULL;
        AsnIoPtr                aip;
	VastPageDataPtr 	vpp=NULL;

  	if (JobID) {
	    OpenMMDBAPI((POWER_VIEW), NULL);
	    pbsa = LocalGetFeatureSet(Fsid/10000, Fsid, JobID);
	    CloseMMDBAPI();
	}
	else {
        	vpp = constructVastPagesForAllNbrs(aSdi, subsetnum, sortby, 
			(unsigned *)&alloc_size, (unsigned *)&numhitsdisplayed);
        	pbsa = constructBASPFromVastPagePtr(vpp, numhitsdisplayed);
	}

        printf("Content-type: application/octet-stream\r\n\r\n"); 
/*
        printf("Content-type: text/html\r\n\r\n");
        printf("<HTML><PRE>\r\n");
*/
        aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);
        BiostrucAnnotSetAsnWrite(pbsa, aip, NULL);
        AsnIoFlush(aip);
        AsnIoClose(aip);
	free(vpp);
}






static void
MakeVastTableGraph(VastPageDataPtr vpp, Int4 numhitsdisplayed, Int4 FSID, 
	Int4 iKept, CharPtr selnbrstring, CharPtr selsdidstring, 
	CharPtr NonNbr, SortBy sortby, SubSetID subsetnum, 
	Int4 pagenum, Int4 numhits, char cTable, Int4 NbrFlag, CharPtr JobID, 
	CharPtr pcPass, Int2 ImgSize, Int2 GraphFlag)
{
  Int4 iMMDBid = 0, numpages;

  if (FSID <= 0) return;
	
  iMMDBid = (Int4) (FSID / 10000);

  numpages = numhits / NUM_HITS_PER_PAGE;
  numpages = numhits % NUM_HITS_PER_PAGE == 0 ? numpages : (numpages+1);

/*
  if(numhits<=0) {
      fprintf(stdout, "<h1><a href=\"%s%s#VASTNonbr\">%s</a></h1>\n", 
							URLBase, HELPname);
      fprintf(stdout, "VAST did not find any structure neighbors.");
      fprintf(stdout, "<HR SIZE=5 NOSHADE>\n");
      fprintf(stdout, "</body></html>\n");
      return;
  }
*/

  if (NbrFlag == DEF_ALL_NBR) {
     if (numhits < NUM_HITS_PER_PAGE) pagenum = DEFAULT_PAGE;
  }
  else pagenum = 0;

  if (GraphFlag) 
      	DrawStrucNbr(FSID, vpp, numhitsdisplayed, iKept, selnbrstring, 
	     selsdidstring, sortby, subsetnum, pagenum, ImgSize, JobID, pcPass);
  else {
  	VastTableBegin(stdout, iMMDBid, FSID, sortby, subsetnum, iKept,NonNbr,
		vpp, numhitsdisplayed, numhits, numpages, 
		pagenum, cTable, NbrFlag, JobID, pcPass);
	switch (cTable) {
	    case 'n': 
		     VastInfoRows(stdout, FSID, JobID, pcPass, vpp, 
			numhitsdisplayed, iKept, selnbrstring, selsdidstring,
			sortby, subsetnum, pagenum);
		     break;
	    case 'y':
		     VastTableRows(stdout, JobID, vpp, numhitsdisplayed, iKept, 
				sortby);
		     break;
	    default:
		     PrtMes(NULL, "VASTSERV", 
                        "Please select \"Graph\" or \"Tabler\" for the display on screen or \"File\" for a local copy.",
			NULL, FALSE);
	}

	fprintf(stdout, "\n<BR><HR SIZE=5 NOSHADE><BR>\n");
        fprintf(stdout, "</body></html>\n");

  }

  RemoveTempFiles();  

  return;       

} /* MakeVastTableGraph() */




/* Extract vastsrv parameters from the config file. */
static Boolean GetVastParams()
{
  URLBase[0] = URLcgi[0] = ENTREZurl[0] = DOCSUMurl[0] = HELPname[0]
  = MAILto[0] = CGIname[0] = MMDBCGIname[0] = DART[0] = LOGIN[0] = PASSWD[0] 
  = INITpath[0] = LIBpath[0] = CDDurl[0] = Database[0] = NULLB;

  GetAppParam("vast", "VASTSRV", "URLBase", "", URLBase, PATH_MAX);
  if (URLBase[0] == NULLB) {
    ErrPostEx(SEV_FATAL,0,0,
		"VAST config file\nVASTSRV section has no URLBase...\n");
    return FALSE;
  }

  GetAppParam("vast", "VASTSRV", "URLcgi", "", URLcgi, PATH_MAX);
  if (URLcgi[0] == NULLB) {
    ErrPostEx(SEV_FATAL,0,0,
			"VAST config file\nVASTSRV section has no URLcgi...\n");
    return FALSE;
  }

  GetAppParam("vast", "VASTSRV", "ENTREZurl", "", ENTREZurl, PATH_MAX);
  if (ENTREZurl[0] == NULLB) {
    ErrPostEx(SEV_FATAL,0,0,
		"VAST config file\nVASTSRV section has no ENTREZurl...\n");
    return FALSE;
  }

  GetAppParam("vast", "VASTSRV", "DOCSUMurl", "", DOCSUMurl, PATH_MAX);
  if (DOCSUMurl[0] == NULLB) {
    ErrPostEx(SEV_FATAL,0,0,
		"VAST config file\nVASTSRV section has no DOCSUMurl...\n");
    return FALSE;
  }

  GetAppParam("vast", "VASTSRV", "CGIname", "", CGIname, PATH_MAX);
  if (CGIname[0] == NULLB) {
    ErrPostEx(SEV_FATAL,0,0,
		"VAST config file\nVASTSRV section has no CGIname...\n");
    return FALSE;
  }

  GetAppParam("vast", "VASTSRV", "MMDBCGIname", "", MMDBCGIname, PATH_MAX);
  if (MMDBCGIname[0] == NULLB) {
    ErrPostEx(SEV_FATAL,0,0,
		"VAST config file\nVASTSRV section has no MMDBCGIname...\n");
    return FALSE;
  }

  GetAppParam("vast", "VASTSRV", "HELPname", "", HELPname, PATH_MAX);
  if (HELPname[0] == NULLB) {
    ErrPostEx(SEV_FATAL,0,0,
		"VAST config file\nVASTSRV section has no HELPname...\n");
    return FALSE;
  }

  GetAppParam("vast", "VASTSRV", "MAILto", "", MAILto, PATH_MAX);
  if (MAILto[0] == NULLB) {
    ErrPostEx(SEV_FATAL,0,0,
		"VAST config file\nVASTSRV section has no MAILto...\n");
    return FALSE;
  }

  GetAppParam("vast", "VASTSRV", "VSPATH", "", VSPATH, PATH_MAX);
  if (VSPATH[0] == NULLB) {
    ErrPostEx(SEV_FATAL,0,0,
	"VAST config file\nVASTSRV section has no VSPATH...\n");
    return FALSE;
  }

  GetAppParam("vast", "VASTSRV", "DATApath", "", DATApath, PATH_MAX);
  if (DATApath[0] == NULLB) {
    ErrPostEx(SEV_FATAL, 0, 0, 
		"VAST config file\nVASTSRV section has no VAST DATApath...\n");
    return FALSE;
  }

  GetAppParam("vast", "VASTSRV", "VASTpath", "", VASTpath, PATH_MAX);
  if (VASTpath[0] == NULLB) {
    ErrPostEx(SEV_FATAL, 0, 0, 
		"VAST config file\nVASTSRV section has no VASTpath...\n");
    return FALSE;
  }

  GetAppParam("mmdb", "MMDB", "Database", "", Database, PATH_MAX);
  if (Database[0] == NULLB) {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nMMDB section no Database\n");      return FALSE;
  }

  GetAppParam("mmdb", "CD", "DARTdb", "", DART, PATH_MAX);
  if (DART[0] == NULLB) {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nCD section no DARTdb\n");
      return FALSE;
  }

  GetAppParam("mmdb", "CD", "LOGINname", "", LOGIN, PATH_MAX);
  if (LOGIN[0] == NULLB) {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nCD section no LOGINname\n");
      return FALSE;
  }

  GetAppParam("mmdb", "CD", "PASSwd", "", PASSWD, PATH_MAX);
  if (PASSWD[0] == NULLB) {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nCD section no PASSwd\n");
      return FALSE;
  }

  GetAppParam("mmdb", "CD", "INITpath", "", INITpath, PATH_MAX);
  if (INITpath[0] == NULLB) {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nCD section no INITpath\n");
      return FALSE;
  }

  GetAppParam("mmdb", "CD", "LIBpath", "", LIBpath, PATH_MAX);
  if (LIBpath[0] == NULLB) {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nCD section no LIBpath\n");
      return FALSE;
  }

  GetAppParam("mmdb", "CD", "CDDurl", "", CDDurl, PATH_MAX);
  if (CDDurl[0] == NULLB) {
      ErrPostEx(SEV_FATAL,0,0, "MMDB config file \nCD section no CDDurl\n");
      return FALSE;
  }

  return TRUE;
} /* GetVastParams() */




/* The following ValidateMMDBID() was changed by J. Chen, May, 2002) */

static Boolean ValidateMMDBID(CharPtr pdbcode, Int4 iMMDBid)
{
  DocUid 	uid;
  Uint1		i;

  for (i=0; i< StrLen(pdbcode); i++) pdbcode[i] = toupper(pdbcode[i]); 
  uid = constructLiveOrDeadMmdbIdForPdbId(pdbcode, &psok, Database);
  
  if ((Int4)uid == iMMDBid) return TRUE;
  else return FALSE;

}



static BiostrucFeaturePtr FilterHitsByDomainSubset(BiostrucFeaturePtr pbsf, 
	Int4 subsetnum, Int4 *KepId, Int4 iKept)
{
  BiostrucFeaturePtr current, pbsfHead = NULL, pbsfTail;
  Int4 i, n, dombsfid;
  Int4 gn, gr, hcnt, *min_ranks, *group_num, *group_rank;
  Char domid[DOMID_SIZE + 1], pdbcode[4+1];
 
/* The next bit of code is used for filtering the hit lists.  When we go through a hit
 * list we skip domains that do not belong to the subset of interest, or which belong to
 * the subset but for which a group representative has already been encountered.
*/
  for (i = 0; i <= DOMID_SIZE; i++)
    domid[i] = NULLB;

  n = GetNumberOfDomains();
  min_ranks = (Int4 *) MemNew(n*sizeof(Int4));
  group_num = (Int4 *) MemNew(n*sizeof(Int4));
  group_rank = (Int4 *) MemNew(n*sizeof(Int4));

  /* use a first pass to "flag" the selected hits in the specified subset 
     If group_num[i] = 0 or group_rank[i] is larger than min_ranks[i], 
     then the ith neighbor should not be included in summary page */

  if (group_rank != NULL) {	      /* check mem alloc */

    for (i = 0; i < n; i++) {	      /* initializations */
         group_num[i] = 0;
         min_ranks[i] = n + 1;
         group_rank[i] = n + 1;
    }

    for (current = pbsf, hcnt = 0; current != NULL; current = current->next, hcnt++) {
       /* copy domain identifier into domid[] */
      if (current->name[6] == ' '){
	 StrCut(domid, current->name, 8, 13);
	 if (current->name[13] != ' ') domid[6] = current->name[13];
      }
      else {
	StrCut(domid, current->name, 9, 14);
	if (current->name[14] != ' ') domid[6] = current->name[14];
      }
      
      if (domid[5] == '0') domid[5] = ' ';

       /* if not in subset then skip over this domain */
      if (BelongsToSubset(domid, subsetnum, &gn, &gr) <= 0) {
	continue;
      }

      /* otherwise record group data for this hit */
      group_num[hcnt] = gn;
      group_rank[hcnt] = gr;

      /* and reset minimum rank for this group */
      if (gr < min_ranks[gn - 1])
	min_ranks[gn - 1] = gr;
    }
  }

  /* Now use the values just set in group_num, group_rank, and min_ranks
     to decide whether or not the current neighbor should be linked
     into the new feature list */

  current = pbsf;
  hcnt = 0;
  while (current) {
    dombsfid = current->id;
/*
    if (current->name[6] == ' '){
	 StrCut(domid, current->name, 8, 13);
	 if (current->name[13] != ' ') domid[6] = current->name[13];
    }
    else {
	StrCut(domid, current->name, 9, 14);
	if (current->name[14] != ' ') domid[6] = current->name[14];
    }
*/

    if (group_rank != NULL)		      /* check mem alloc */
    {
      if (group_num[hcnt] == 0)	     /* group_num not set so NOT in subset */
      {					      /* incr hcnt and do NOT link */
        hcnt++;
	current = current->next;
        continue;
      }
   
      gn = group_num[hcnt];

      if (group_rank[hcnt] != min_ranks[gn - 1])  
			/* neighbor is in subset but of lower rank */
      {
        hcnt++;			         /* incr hcnt and do NOT link */
	current = current->next;
        continue;
      }
    }
    
    /* With the new rcsb depositions we now need to validate mmdbids
       in the .bas files. Extract pdbcode and use it with mmdbID to validate */

    StrCut(pdbcode, current->name, 8, 11);
    pdbcode[4]= NULLB;
       
    if (!ValidateMMDBID(pdbcode, (current->id)/100000)) {
      hcnt++;
      current = current->next;	         /* incr hcnt and do NOT link */
      continue;
    }

/* remove duplicated lists, J. Chen */
    i=0; 
    while (i< iKept && dombsfid != KepId[i]) i++; 
    if (i < iKept) {
	hcnt++;
	current = current->next;
	continue;
    }

    if (pbsfHead == NULL)    /* neighbor has passed all tests - include it!*/
    {
      pbsfHead = current;
      pbsfTail = pbsfHead;
      hcnt++;
      current = current->next;
      pbsfTail->next = NULL;
    }
    else
    {
      pbsfTail->next = current;
      pbsfTail = current;
      hcnt++;
      current = current->next;
      pbsfTail->next = NULL;
    }
  }
  
  MemFree(min_ranks);
  MemFree(group_num);
  MemFree(group_rank);
  return pbsfHead;

}	/* FilterHitsByDomainSubset */

 

static void FilterHitsByPageNew(BiostrucFeatureSetPtr pbsfs, Int4 PageNum, 
	Int4 HitsPerPage, Int4 *numhits, Int4 *numpages)
{
  
  BiostrucFeaturePtr pbsf, pbfHead=NULL, pbfTail=NULL;
  Int4 index, FidCount, RemainFids, CompleteFidSet; 
  Int4 UpperLimit, LowerLimit;
 
  pbsf = pbsfs->features;
  if (pbsf == NULL) {
    *numpages = 1;
    *numhits = 0;
  }
  
  FidCount = 0;
  
  while (pbsf)
  {
    FidCount++;
    pbsf = pbsf->next;
  }
  *numhits = FidCount;

  if (FidCount <= HitsPerPage) *numpages = 1;
  else
  { 
    RemainFids = FidCount % HitsPerPage;
  
    if (RemainFids)
    {
     CompleteFidSet = FidCount - RemainFids;
     *numpages = (CompleteFidSet/HitsPerPage) + 1;
    }
    else *numpages = FidCount/HitsPerPage;
  }
  
  UpperLimit = HitsPerPage * PageNum;
  LowerLimit = UpperLimit - HitsPerPage + 1;
    
  if ((FidCount < HitsPerPage ) || (LowerLimit > FidCount)) {
    UpperLimit = FidCount;
    LowerLimit = 1;
  }
  
  if (UpperLimit > FidCount) UpperLimit = FidCount;
  
  for (index = 1, pbsf = pbsfs->features; index <= FidCount; 
	index++, pbsf=pbsf->next)
  {
    if ((index >= LowerLimit) && (index <= UpperLimit)) {
	if (!pbfHead) pbfHead = pbfTail = pbsf;
	else {
		pbfTail->next = pbsf;
		pbfTail = pbsf;
	}
    }
  }
  
  if (pbfTail) pbfTail->next = NULL;
  pbsfs->features = pbfHead;

}	/* FilterHitsByPageNew */


static 
BiostrucAnnotSetPtr GetValidBiostrucAnnotSet(BiostrucAnnotSetPtr basp,Int4 Fsid)
{
    BiostrucAnnotSetPtr basp2 = NULL;
    BiostrucFeatureSetPtr pbsfs = NULL, pbsfsLast= NULL;
    BiostrucIdPtr               pbsidThis = NULL;

    if (basp == NULL) return NULL;

    pbsfs = basp->features;
    while (pbsfs) {
       if (pbsfs->id == Fsid)
        {
          basp2 = BiostrucAnnotSetNew();
          basp2->id = basp->id;
          basp->id = NULL; /* unlink the id valnode from basp object */
          basp2->descr = basp->descr;
          basp->descr = NULL;  /* unlink the descr from basp object */
          basp2->features = pbsfs;
          if (pbsfsLast) /* relink next to prev */
            pbsfsLast->next = pbsfs->next;
          else
            basp->features = pbsfs->next;
          basp2->features->next = NULL;
	  break;
	}
       pbsfsLast = pbsfs;
       pbsfs = pbsfs->next;
    }
    BiostrucAnnotSetFree(basp);
    
    if (basp2 == NULL) {
	char    str[15];

        sprintf(str, "%d", Fsid);
	PrtMes(NULL, "VASTSRV", "Incorrect sdid or chaindom: chaindom = ", str, FALSE);
    }

    pbsidThis = ValNodeFindNext(basp2->id, NULL, BiostrucId_mmdb_id);
    if (!pbsidThis) {
	PrtMes(MAILto, "VASTSRV", "No MMDB-ID in Data on Server.", NULL, TRUE);
	BiostrucAnnotSetFree(basp2);
	return NULL;
    }

    return(basp2);

} /* GetValidBiostrucAnnotSet */


static Int4 
MakeVppByVS(Int4 Fsid, CharPtr JobID, VastPageDataPtr vpp, SubSetID subsetnum,
 	SortBy sortby, Int4 pagenum, Int4 *KepBsfId, Int4 iKept)
{

    BiostrucAnnotSetPtr 	basp = NULL, basp2 = NULL;
    BiostrucFeatureSetPtr 	pbsfs = NULL, pbsfs2 = NULL;
    BiostrucIdPtr 	  	pbsidThis = NULL;
    Int4			numhits, numpages, numhitsdisplayed;
    Char                        str[15];

    basp = LocalGetBiostrucAnnotSet(Fsid/10000, JobID);
    if (!basp) {
	sprintf(str, "%d", Fsid);
        PrtMes(NULL, "VASTSRV", "Incorrect sdid or chaindom: chaindom = ", str, FALSE);
    }

    basp2 = GetValidBiostrucAnnotSet(basp, Fsid);
    if (!basp2) return 0;

    pbsfs = basp2->features;
    while (pbsfs)
    {
     if (pbsfs->id == Fsid)
      {

	pbsidThis = ValNodeFindNext(pbsfs->descr, NULL,
					BiostrucFeatureSetDescr_name);
        if (pbsfs->features != NULL) {
          pbsfs2 = BiostrucFeatureSetNew();
          pbsfs2->id = pbsfs->id;
          pbsfs->id = NULL;
          pbsfs2->descr = pbsfs->descr;
          pbsfs->descr = NULL;

          pbsfs2->features =
		FilterHitsByDomainSubset(pbsfs->features, subsetnum, 
			KepBsfId, iKept);
          pbsfs->features = NULL;
          if (pbsfs2->features != NULL) VastTableSort(pbsfs2, sortby);
          FilterHitsByPageNew(pbsfs2, pagenum, NUM_HITS_PER_PAGE, 
					&numhits, &numpages);
/* numhits=60; */
          if (numhits < NUM_HITS_PER_PAGE) pagenum = DEFAULT_PAGE;
	  

	  numhitsdisplayed = BFSP2VastPageDataPtr(pbsfs2, vpp, numhits);
        }
        else numhitsdisplayed = 0;

	BiostrucAnnotSetFree(basp2);
	return (numhitsdisplayed);
      }
     pbsfs = pbsfs->next;
   }

  BiostrucAnnotSetFree(basp2);
  sprintf(str, "%d", Fsid);
  PrtMes(NULL, "VASTSRV", "Incorrect sdid or chaindom: chaindom = ", str,FALSE);

}	/* MakeVppByVS */




static BiostrucFeaturePtr
FilterHitsByNbrs(BiostrucFeaturePtr bfp, CharPtr *SelNames, Int4 *SelId, 
	Int4 issdid, Int4 iSel, Int4 *KepId, Int4 iKept)
{
   BiostrucFeaturePtr pbfHead = NULL, pbfTail=NULL;
   Char		*str, sName[10];
   Int4 	i, j, id, sdi;

   if (bfp == NULL) return NULL;
   
   while (bfp) {
     if (SelNames != NULL) {
	str = bfp->name;
     	if (str[6] == ' ') {
	    StrCut(sName, str, 8, 13); 
	    if (StrLen(str) >= 14 && str[13] != ' ') StringCat(sName, str+13);
        }
     	else {
	    StrCut(sName, str, 9, 14);
	    if (StrLen(str) >=15 && str[14] != ' ') StringCat(sName, str+14);
     	}
     }
     id = bfp->id;
     sdi = LongDomIdToSdi(id/10);

     for (i=0; i< iSel; i++) {
	if (SelNames != NULL) {
     	    if (StringNCmp(SelNames[i], sName, StrLen(SelNames[i]))) continue;
	}
	else {
	     if (issdid) {
		if (sdi != SelId[i]) continue;
	     }
	     else if (id != SelId[i]) continue;
  	}

	j=0; 
	while (j< iKept && id != KepId[j]) j++;
        if (j < iKept) continue;
        if (pbfHead == NULL)  {
		pbfHead = pbfTail = bfp;
	}
        else {
		pbfTail->next = bfp;
		pbfTail = bfp;
	}
	break; 
    }
    bfp = bfp->next;

  }
		
  if (pbfTail) pbfTail->next = NULL;

  return pbfHead;
} 	/* end of FilterHitsByNbrs */
					



static Int4 MakeVppByVSNbr(Int4 Fsid, CharPtr JobID, VastPageDataPtr vpp, 
  	CharPtr *SelNames, Int4 *SelId, Int4 issdid, Int4 iSel, Int4 *KepId, 		Int4 iKept, SortBy sortby)
{
    BiostrucAnnotSetPtr 	basp = NULL, basp2=NULL;
    BiostrucFeatureSetPtr 	pbsfs = NULL, pbsfs2 = NULL;
    Int4 			numhitsdisplayed;
    Char                        str[15];

    basp = LocalGetBiostrucAnnotSet(Fsid/10000, JobID);
    if (!basp) {
        sprintf(str, "%d", Fsid);
        PrtMes(NULL, "VASTSRV", "Incorrect sdid or chaindom. chaindom  = ", str, FALSE);
    }

    basp2 = GetValidBiostrucAnnotSet(basp, Fsid);
    pbsfs = basp2->features;
    while (pbsfs) {
	if (pbsfs->id == Fsid) {
	    if (pbsfs->features != NULL) {
		pbsfs2 = BiostrucFeatureSetNew();
		pbsfs2->id = pbsfs->id;
		pbsfs->id = NULL;
		pbsfs2->descr = pbsfs->descr;
		pbsfs->descr = NULL;

		pbsfs2->features = 
		   FilterHitsByNbrs(pbsfs->features, SelNames, SelId, issdid, 
				iSel, KepId, iKept);
		if (pbsfs2->features) {
		   if (sortby) VastTableSort(pbsfs2, sortby);
		   pbsfs->features = NULL;
		
		   numhitsdisplayed= 
			BFSP2VastPageDataPtr(pbsfs2, vpp, MaxVPP);
	       } else numhitsdisplayed = 0;
	    }
	    else numhitsdisplayed = 0;

	    BiostrucAnnotSetFree(basp2);
	    return (numhitsdisplayed);
	}
	pbsfs = pbsfs->next;
    }

    BiostrucAnnotSetFree(basp2);
    sprintf(str, "%d", Fsid);
    PrtMes(NULL, "VASTSRV", "Incorrect sdid or chaindom: chaindom = ", str, FALSE);

} /* MakeVppByVSNbrs */




static Int4 CheckNbrs(CharPtr arg, CharPtr *SelNames, Int4 iSel, VastPageDataPtr vpp, Int4 vppnum)
{
   Char		savedchar, name[10];
   CharPtr	str, ptr0=NULL, ptr = NULL, ptr2 = NULL;
   Int4		i, j, count=0;
  
   arg[0] = NULLB;
   if (!vppnum) {
	for (i=0; i< iSel; i++) {
	   char tmp[10];	

           sprintf(tmp, "%s, ", SelNames[i]);
	   StringCat(arg, tmp);
 	}
	return;
   }

   str  = (CharPtr) MemNew (vppnum*8+1); 
   ptr0 = (CharPtr) MemNew (vppnum*8+1);

   str[0] = NULLB;
   for (i=0; i< vppnum; i++) {
	StringCat(str, vpp[i].bDomName);
	StringCat(str, ",");
   }

   for (i=0; i< iSel; i++) {
     sprintf(ptr0, str);
     ptr = ptr0;
     while (*ptr) {
	sprintf(name, SelNames[i]);
	if (IsMmdbId(name)) { 
	    j = (Int4) atol (name);
	    constructPdbIdForMmdbId(j, name);
	    name[4] = NULLB;
	} 
	ptr = SkipToSet(ptr, name);
	ptr2 = SkipToSet(ptr, ",");
	savedchar = *ptr2;
	*ptr2 = NULLB;
 	if (!strncmp(name, ptr, StrLen(name))) break;
	*ptr2 = savedchar;
	ptr = SkipSet(ptr2, ",");
     }
     if (ptr[0] == NULLB) {
	char	tmp[10];
	
	sprintf(tmp, "%s, ", SelNames[i]);
	StringCat(arg, tmp);
	count++;
     }
  }

  MemFree(str); MemFree(ptr0);
  return (count);
}




CheckNbrsbysdid(CharPtr NonNbr, Int4Ptr SelSds, Int4 iSds,
        VastPageDataPtr vpp, Int4 numhits)
{
   Int4  i, j;
   Char  str[10];

   NonNbr[0] = NULLB;

   for (i=0; i< iSds; i++) {
        j=0;
        while (j < numhits && SelSds[i] != vpp[j].bSdi) j++;
        if (j == numhits) {
            sprintf(str, "%d, ", SelSds[i]);
            StringCat(NonNbr, str);
        }
   }
   

}       /* CheckNbrsbysdid */




Int2 Main()
{
  Char 		cTable='n', ODBCInitStr[100], LIBPathStr[100], str[15];
  Char          NonNbr[MAX_TBUFF];
  CharPtr 	JobID = NULL, pcPass, www_arg, Cmd=NULL, *SelNames=NULL;
  CharPtr	selnbrstring = NULL, selsdidstring=NULL;
  Int2 		ret, ImgSize;
  Int4 		aDomId, indx, pagenum, numhitsdisplayed, totalnumhits;
  Int4 		maxAsdi = 0, iSel=0, iKept=0, i, *KepBsfId;
  Int4		NbrFlag, iSubBut=DEF_LIST_SUBMIT, *SelSds, iSds;
  struct rlimit rl;
  SubSetID 	pre_subsetnum=DEFAULT_SUBSET_NUM; 
  SubSetID	subsetnum=DEFAULT_SUBSET_NUM;
  SortBy 	sortby = DEFAULT_SORT_BY;
  WWWInfoPtr 	www_info;
  VastPageData 	vpp[MaxVPP];


  /* this sets up the unix time limit */
  getrlimit(RLIMIT_CPU, &rl);
  rl.rlim_max = rl.rlim_cur = CPUTIME_MAX;
  setrlimit(RLIMIT_CPU, &rl);

/* Begin processing www information block */
  if (WWWGetArgs(&www_info) != WWWErrOk) 
	PrtMes(NULL, "VASTSRV", 
		"Failed to process posting - check your get/post syntax.",
		 NULL, FALSE);

  if (WWWGetNumEntries(www_info) == 0) 
	PrtMes(NULL, "VASTSERV", "No input - nothing to report.", NULL, FALSE);


  if (! GetVastParams())
        PrtMes(MAILto, "VASTSRV",
                "Couldn't read the config file \".vastrc\"...", NULL, FALSE);

  sprintf(ODBCInitStr, "ODBCINI=%s", INITpath);
  putenv(ODBCInitStr);

  VastSrvInitialize();
  MmdbSrvInitialize();

  if ((indx= WWWFindName(www_info, "dispsub")) >=0) 
		iSubBut = DEF_LIST_SUBMIT;
  else if ((indx=WWWFindName(www_info,"viewstr")) >=0) 
		iSubBut=VIEW_STR_SUBMIT;
  else if ((indx=WWWFindName(www_info,"viewali")) >=0) 
		iSubBut=VIEW_ALI_SUBMIT;
  else if ((indx= WWWFindName(www_info,"schsub")) >=0)
		iSubBut=FIND_SUBMIT;
/*
   else if ((indx= WWWFindName(www_info, "selnbr")) >=0) {
       www_arg = WWWGetValueByIndex(www_info, indx);
       if (www_arg[0] != NULLB) iSubBut = FIND_SUBMIT;
       else if ((indx= WWWFindName(www_info, "selsdid")) >=0) {
               www_arg = WWWGetValueByIndex(www_info, indx);
               if (www_arg[0] != NULLB) iSubBut = FIND_SUBMIT;
         }
   }
   else if ((indx= WWWFindName(www_info, "selsdid")) >=0) {
                 www_arg = WWWGetValueByIndex(www_info, indx);
                 if (www_arg[0] != NULLB) iSubBut = FIND_SUBMIT;
   }
*/

  if (iSubBut == VIEW_STR_SUBMIT) {
				/* check whether or not to launch a viewer */
    if ((indx = WWWFindName(www_info, "calltype")) >= 0) {
      www_arg = WWWGetValueByIndex(www_info, indx);

      switch (www_arg[0]) {
      	case 'a':			/* Cn3d 4.0 */
      	case 'c':			/* Cn3d 3.0 */
        	  (Boolean) VastToCn3DAndAli(www_info);
        	  exit(0);
      	default:
	   PrtMes(NULL, "VASTSRV",
		"Bad calltype -- check you viewer selection.", NULL, FALSE);
      }
    }
    else PrtMes(NULL, "VASTSrv", 
		"No calltype -- please select a viewer.", NULL, FALSE);
  }
  else if (iSubBut == VIEW_ALI_SUBMIT) {
    if ((indx = WWWFindName(www_info, "alitype")) >= 0) {
      www_arg = WWWGetValueByIndex(www_info, indx);

      switch (www_arg[0]) {

      	case 'h':			/* HTML */
      	case 'f':			/* FASTA */
      	case 't':			/* TEXT */
		  (Boolean) VastToCn3DAndAli(www_info); 
                  exit(0);
        default:
	   PrtMes(NULL, "VASTSRV", 
	     "Bad alitype -- please select a correct alignment display format", 
	      NULL, FALSE);

     }
   }
   else PrtMes(NULL, "VASTSRV", 
	   	"No alitype -- please select a alignment display format.", 
		 NULL, FALSE);
 }


/* Jie Chen: Initialization of dart database */

  sprintf(LIBPathStr, "LD_LIBRARY_PATH=%s", LIBpath);
  putenv(LIBPathStr);

  dartcon = Dart_Init2(DART, LOGIN, PASSWD);
 
  if (dartcon == NULL ) 
	PrtMes(MAILto, "VASTSRV", "Unable to open dartcon", NULL, FALSE);

  /* load in the chaindom into memory */

  objmmdb1AsnLoad();
  objmmdb2AsnLoad();
  objmmdb3AsnLoad();

  if ((indx = WWWFindName(www_info, "chaindom")) >= 0) {
  	www_arg = WWWGetValueByIndex(www_info, indx);
  	if (isdigit(www_arg[0])) aDomId = (long) atol(www_arg);
	else 
	   PrtMes(MAILto, "VASTSRV",
		"Invalid Biostruc-feature-set-id: chaindom = ", www_arg, FALSE);
  }
  else if ((indx = WWWFindName(www_info, "sdid")) >=0) {
	int maxsdi;

	www_arg = WWWGetValueByIndex(www_info, indx);
	if (isdigit(www_arg[0])) aSdi = (long) atol(www_arg);
	else if (www_arg[0] == '-') aSdi = atol(www_arg);
	else 
	   PrtMes(MAILto, "VASTSRV",
                "Non-numeric Structure Domain Identifier (sdid) = ",
                 www_arg,FALSE);

	maxsdi = constructMaxSdi();
	if (!aSdi || aSdi > maxsdi) 
           PrtMes(NULL, "VASTSRV",
               "Vast neighbor data for this domain is not yet available. Please try later again.", NULL, FALSE);

	if (aSdi < 0)
	   PrtMes(NULL, "VASTSRV",
	     "This is an obsolete domain, no Vast information.", NULL, FALSE);

	aDomId = SdiToLongDomId(aSdi);
        if (!aDomId) {

           sprintf(str, "%d", aSdi);
           PrtMes(MAILto, "VASTSRV",
	    "Error: No domain exists for this identifier: sdid  = ",str,FALSE);
        }

  }
  else PrtMes(NULL, "VASTSRV", 
		"No chaindom or sdid -- Please input chaindom or Structure Domain Identifier.", NULL, FALSE);


  if ((indx = WWWFindName(www_info, "vsid")) >= 0) { /* a VAST Search job */
      www_arg = WWWGetValueByIndex(www_info, indx);
      JobID = StringSave(www_arg);

      if ((indx = WWWFindName(www_info, "pass")) < 0) 
	PrtMes(NULL, "VASTSRV", 
	  "Password required -- input password of your Vast Search job",
	   NULL, FALSE);

      www_arg = WWWGetValueByIndex(www_info, indx);
      pcPass = StringSave(www_arg);
      if ((ret = Check_VastSearch_Password(pcPass, JobID)) != 1) {
	  if (ret == 2) exit(0);
	  PrtMes(NULL, "VASTSRV", 
		"Incorrect password of a Vast Search result. It could be a wrong Vast Search identifier or a wrong password.", NULL, FALSE);
      }

  }
  else {
      if (!aSdi) aSdi = LongDomIdToSdi(aDomId);
      if(aSdi <= 0)  {
        sprintf(str, "%d", aDomId);

	PrtMes(NULL, "VASTSRV", 
	 "VAST structure neighbor calculations for this entry are in progress or an incorrect chaindom = ", str, FALSE);
      }

      maxAsdi = constructMaxASdi();
 
      if(aSdi > maxAsdi) 
	PrtMes(NULL, "VASTSRV", 
	 "VAST structure neighbor calculations for this entry are in progress.",
	  NULL, FALSE);
  }

  if (!VASTInit(JobID)) 
      PrtMes(MAILto, "VASTSRV", 
	"Cannot find VAST data on server or in database.", NULL, FALSE);

  /* subset filtering; identify which subset we're working with */
  if ((indx = WWWFindName(www_info, "subset")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) subsetnum = atoi(www_arg);
    else subsetnum = getSubsetNbr(www_arg); 
					/* www_arg is the subsetname now */
  }
/*  else if (JobID) subsetnum = 1; */
  if (iSubBut == FIND_SUBMIT) subsetnum = 1;

  if ((indx = WWWFindName(www_info, "presubset")) >=0) {
	www_arg = WWWGetValueByIndex(www_info, indx);
	if (isdigit(www_arg[0])) pre_subsetnum = atoi(www_arg);
	else pre_subsetnum = getSubsetNbr(www_arg);
  }


  if (pre_subsetnum != subsetnum) pagenum = DEFAULT_PAGE;
  else {
     if ((indx = WWWFindName(www_info, "doclistpage")) < 0)
     	pagenum = DEFAULT_PAGE;
     else {
    	www_arg = WWWGetValueByIndex(www_info, indx);
    	if (isdigit(www_arg[0])) pagenum = (Int4) atoi(www_arg);
    	else pagenum = DEFAULT_PAGE;
	if (!pagenum) pagenum = DEFAULT_PAGE; 
     }
  }

  if ((indx = WWWFindName(www_info, "sort")) < 0)
    sortby = DEFAULT_SORT_BY;
  else {
    www_arg = WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0]))
      sortby = (Int2) atoi(www_arg);
    else
      sortby = DEFAULT_SORT_BY;
  }


  if ((indx = WWWFindName(www_info, "table")) >= 0) {
	www_arg = WWWGetValueByIndex(www_info, indx);
	cTable = www_arg[0];

        if (cTable == 's' && (indx=WWWFindName(www_info, "hit")) <0 ) {
                SaveBioAnnotSetInFile(aDomId, subsetnum, sortby, JobID);
                return;
        }
  }


  iKept = 0;
  KepBsfId = NULL;
  if ((iSubBut==DEF_LIST_SUBMIT || iSubBut==FIND_SUBMIT) 
		&& (indx = WWWFindName(www_info, "hit")) >=0) {
    
    Int4 	NumLabels;
    CharPtr 	Name; 
    VastPageDataPtr vpptmp;

    NumLabels = WWWGetNumEntries(www_info);

    for (indx=0; indx < NumLabels; indx++) {
	Name = WWWGetNameByIndex(www_info, indx);
	if (!StrICmp(Name, "hit")) iKept++;
    }

    if (iKept > MAX_KEPT_NBR) {
	Char   str[100];

	sprintf(str, "%d", MAX_KEPT_NBR);
	PrtMes(NULL, "VASTSRV", 
		"Too many selected neighbors. The maximum allowed is ",
		 str, FALSE);
    }

    KepBsfId = (Int4Ptr) MemNew (iKept * sizeof(Int4));
    vpptmp = (VastPageDataPtr) MemNew (iKept*5 * sizeof(VastPageData));

    iKept = 0;
    for (indx = 0; indx< NumLabels; indx++) {
	Name = WWWGetNameByIndex(www_info, indx);
	if (!StrICmp(Name, "hit")) {

	   CharPtr ptr= NULL;

	   www_arg = WWWGetValueByIndex(www_info, indx);
	   if (isdigit(www_arg[0])) 
		KepBsfId[iKept] = vpptmp[iKept++].bBsfId = atoi(www_arg);
	   else {
               PrtMes(NULL, "VASTSRV", "The hit format is not correct.",
                        NULL, TRUE);
               return 0;
           }
	}
    }

    if (JobID)  
	numhitsdisplayed =
	    MakeVppByVSNbr(aDomId, JobID, vpp, NULL,KepBsfId,0, iKept,NULL,0,0);
    else { 
	numhitsdisplayed = 
	    constructVastPagesByNbrs(vpptmp, (unsigned *)KepBsfId, iKept, aSdi);
    	OrderCopyVpp(vpptmp, vpp, iKept, KepBsfId);
    }

    MemFree(vpptmp);

  }

  NbrFlag = DEF_ALL_NBR;
  if (iSubBut==FIND_SUBMIT) {

    Int4        i, numhitsbyuid = 0, numhitsbysdid = 0;
    CharPtr 	NonNbrbyuid=NULL, NonNbrbysdid=NULL;

    if ((indx=WWWFindName(www_info, "selnbr")) >= 0) {

      www_arg = WWWGetValueByIndex(www_info, indx);


    for (i=0; i< strlen(www_arg); i++)
	if (!isdigit(www_arg[i]) && (!isalpha(www_arg[i])) && (www_arg[i] != ','))
           {
		if (i && www_arg[i-1] == ',') www_arg[i] = ' ';
		else www_arg[i] = ',';
	   }


      if (www_arg[0] != NULLB) { 
	MakeNbrList(www_arg, &SelNames, NULL, &iSel, 1);

        if (iSel) {
	   if (JobID) 
		numhitsbyuid = 
		   MakeVppByVSNbr(aDomId, JobID, vpp+iKept, SelNames, NULL, 0,
				iSel, KepBsfId, iKept, sortby); 
  	   else 
		numhitsbyuid =
	    	   constructVastPagesByNbrsWithLikeUnlikeBsfId(vpp+iKept,
			MaxVPP, aSdi, SelNames, iSel,
				(unsigned *)KepBsfId, iKept, sortby);

           numhitsdisplayed += numhitsbyuid;
	   NonNbrbyuid = (CharPtr) MemNew (iSel * 10);
	   CheckNbrs(NonNbrbyuid,SelNames, iSel, vpp, numhitsdisplayed);
	   sprintf(NonNbr, NonNbrbyuid);

	   selnbrstring =(CharPtr)MemNew((size_t)(StrLen(www_arg) +3*iSel));
           for (i=0; i< iSel; i++) {
		if (SelNames[i][4] == ' ') SelNames[i][4] = '.';
		StringCat(selnbrstring, SelNames[i]);
		StringCat(selnbrstring, "%2C");
           }
           
	}	/* if (iSel) */
      }		/* if (www_arg[0]) */
    }		/* if (selnbr) */

    if ((indx=WWWFindName(www_info, "selsdid")) >=0) {
	int i;

        www_arg = WWWGetValueByIndex(www_info, indx);

/* change return carriage. */

	for (i=0; i< strlen(www_arg); i++)
           if (!isdigit(www_arg[i]) && (!isalpha(www_arg[i])) && 
						(www_arg[i] != ','))
           {
                if (i && www_arg[i-1] == ',') www_arg[i] = ' ';
                else www_arg[i] = ',';
           }

        if (www_arg[0] != NULLB) {
            MakeNbrList(www_arg, NULL, &SelSds, &iSds, 0);
        }

        if (iSds) {
	    char str[10];
	    sprintf(www_arg, "%d", SelSds[0]);
	    for (i=1; i< iSds-1; i++) {

		sprintf(str, ",%d", SelSds[i]);
		StringCat(www_arg, str);
	    }
	    if (iSds >1) {
		sprintf(str, ",%d", SelSds[iSds-1]);
	    	StringCat(www_arg, str);
	    }

            if (JobID) {        /* Vast Search */
		numhitsbysdid = 
		  MakeVppByVSNbr(aDomId, JobID, vpp+numhitsdisplayed, NULL,
			SelSds, 1, iSds, KepBsfId,iKept,0);
            }
            else 
                numhitsbysdid = 
                   constructVPPBySdi(vpp+numhitsdisplayed, MaxVPP, aSdi,
				www_arg, (unsigned *)KepBsfId,iKept,sortby);

            numhitsdisplayed += numhitsbysdid;
            NonNbrbysdid = (CharPtr) MemNew (iSds *10);
            CheckNbrsbysdid(NonNbrbysdid, SelSds, iSds, vpp, numhitsdisplayed);

            selsdidstring = (CharPtr) MemNew (StrLen(www_arg)+3*iSds);
            for (i=0; i< iSds; i++) {
                Char str[20];

                sprintf(str, "%d%%2C", SelSds[i]);
                StringCat(selsdidstring, str);
            }
        }

    }

    if (NonNbrbyuid && NonNbrbyuid[0] != NULLB) {
        sprintf(NonNbr, NonNbrbyuid);
        MemFree(NonNbrbyuid);

        if (NonNbrbysdid && NonNbrbysdid[0] != NULLB) {
                StringCat(NonNbr, NonNbrbysdid);
                MemFree(NonNbrbysdid);
        }
    }
    else if (NonNbrbysdid && NonNbrbysdid[0] != NULLB) {
        sprintf(NonNbr, NonNbrbysdid);
        MemFree(NonNbrbysdid);
    }

    if (NonNbr[0] != NULLB) NonNbr[StrLen(NonNbr)-2] = NULLB;

    if (!numhitsdisplayed && NonNbr[0] == NULLB)
        PrtMes(NULL, "VASTSRV",
          "Please select a neighbor by using checkbox or input a name in the test box following \"Find\", then click \"Find\" again.",
          NULL, FALSE);

    NbrFlag = SELECTED_NBR;
    vpp[0].totalNumHits = numhitsdisplayed;

  }     /* if (Find) */

  if (NbrFlag == DEF_ALL_NBR) {
	if (JobID) 
	    numhitsdisplayed = MakeVppByVS(aDomId, JobID, vpp+iKept, subsetnum,
				      sortby, pagenum, KepBsfId, iKept);
	else if (subsetnum < 6)
    	    numhitsdisplayed=
		constructVastPages(vpp+iKept,NUM_HITS_PER_PAGE,
			aDomId, (unsigned *)KepBsfId, iKept, subsetnum, sortby,
			NUM_HITS_PER_PAGE, pagenum);

    	if (subsetnum < 6) {
		vpp[0].totalNumHits = vpp[iKept].totalNumHits;
    		numhitsdisplayed += iKept;
	}
	else vpp[0].totalNumHits = iKept;
  }

  if (numhitsdisplayed <= 0) {
      if (subsetnum == 6) 
	PrtMes(NULL, "VASTSRV", 
		"Please use checkbox(es) to select neighbor(s).", NULL, FALSE); 
      if (NbrFlag == DEF_ALL_NBR) {
     	printf("Content-type: text/html\n\n");
     	printf("<body bgcolor = \"#f0f0f0\">\n");
     	printf("<br>\n");
     	printf("<h2><a href=\"%s%s#VASTNonbr\">", URLBase, HELPname);
	printf("VAST did not find any structure neighbor.</a></h2>");
      }
      else {
	Char str[MAX_TBUFF];

	printf("Content-type: text/html\n\n");
        printf("<body bgcolor = \"#f0f0f0\">\n");
	printf("<br><h2>VASTSRV:<p>\n");
	if (NonNbr && NonNbr[0] != NULLB) {
	   if (strchr(NonNbr, ',')) 
	      sprintf(str," are not structure neighbors. ");
	   else sprintf(str," is not a structure neighbor. ");
 	   printf("<font color=#CC6600>%s</font>%s\n", NonNbr, str);
	}
      }
 
      exit(0);
  }


  if (iSubBut == DEF_LIST_SUBMIT || iSubBut == FIND_SUBMIT) { 

    totalnumhits = vpp[0].totalNumHits;
    if (NbrFlag == DEF_ALL_NBR && totalnumhits <=0) 
	PrtMes(MAILto, "VASTSRV", 
	   "TotalNumberOfHits == 0, yet TotalNumberOfHitsDisplayed > 0.",
	   NULL, FALSE);
    if (!JobID) {
    	aSdi = vpp[0].aSdi;
    	if (aSdi <= 0 ) 
	    PrtMes(MAILto, "VASTSRV", 
		"TotalNumberOfHits > 0, yet aSdi == 0.", NULL, FALSE);
    }

    if ((indx = WWWFindName(www_info, "cmd")) >=0) {
	Cmd = WWWGetValueByIndex(www_info, indx);
	if (!StrICmp(Cmd, "graph")) {
	    if ((indx = WWWFindName(www_info, "imgsize")) <0) 
		PrtMes(NULL, "VASTSRV", 
			"Missing imgsize -- please use \"imgsize = number\".",
			 NULL, FALSE);
	    else {
		www_arg = WWWGetValueByIndex(www_info, indx);
		if (isdigit(www_arg[0])) ImgSize = atoi(www_arg);
		else {
		    PrtMes(MAILto, "VASTSRV", 
                 	"Missing imgsize: imgsize = ", www_arg, FALSE);
		}  /* sth is wrong */
		MakeVastTableGraph(vpp, numhitsdisplayed, aDomId, iKept, 
			selnbrstring, selsdidstring, NonNbr, sortby, 
			subsetnum, pagenum, totalnumhits, cTable, NbrFlag, 
			JobID, pcPass, ImgSize,1);
	   }
	}
    }
    else if (cTable == 's') {
	BiostrucAnnotSetPtr     pbsa = NULL;
	AsnIoPtr                aip;
	
	pbsa = constructBASPFromVastPagePtr(vpp, numhitsdisplayed);
	printf("Content-type: application/octet-stream\r\n\r\n");
 	        aip = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);
        BiostrucAnnotSetAsnWrite(pbsa, aip, NULL);
        AsnIoFlush(aip);
        AsnIoClose(aip);
    }
    else   
    	MakeVastTableGraph(vpp,numhitsdisplayed,aDomId, iKept, selnbrstring,
		selsdidstring, NonNbr, sortby, subsetnum, pagenum, 
		totalnumhits, cTable, NbrFlag, JobID, pcPass, 0, 0);
  } 


  MMDBFini();
  VASTFini();
  VastSrvFinish();
  MmdbSrvFinish();
 
  if (selnbrstring != NULL) {
 	MemFree(selnbrstring);
  	for (i=0; i< iSel; i++) MemFree(SelNames[i]);
  	MemFree(SelNames);
  }

  if (selsdidstring != NULL) {
       MemFree(selsdidstring);
       MemFree(SelSds);
  }

  Dart_Fini(dartcon); 

} /* end Main */

