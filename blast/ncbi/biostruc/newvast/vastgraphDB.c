/* 
 * $Id: vastgraphDB.c,v 1.3 2003/01/14 20:49:20 chenj Exp $
 *
 *
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
 *
 * Author: Jie Chen
 *
 * $Log: vastgraphDB.c,v $
 * Revision 1.3  2003/01/14 20:49:20  chenj
 * Imported sources
 *
 *
 * This is to show the "red-cloud" graph. 
 * The footprint alignment is broken at the position of insert.
 *
 * ===========================================================================
*/

#include <ncbi.h>
#include <ncbistr.h>
#include <ncbistd.h> 
#include <mmdbapi.h>
#include <mmdbdata.h>
#include <time.h>
#include <objgen.h>
#include "vastgenDB.h"

#define iNcolors 13

static double pix_per_res;
static CharPtr	QueryBit;
static gdImagePtr im;
static Int2    white, black, red, blue, gray;
static Int4 iDartCol[13];

extern Dart_Connect	*dartcon;
extern Char URLcgi[MAX_TBUFF],MMDBCGIname[MAX_TBUFF], CGIname[MAX_TBUFF];
extern Char MAILto[PATH_MAX],  ENTREZurl[PATH_MAX];
extern Char VSPATH[PATH_MAX], CDDurl[PATH_MAX];
extern Int4	aSdi;

static void
GetLeftAnglePoints(gdPoint *points, Int4 x1, Int4 x2, Int4 dx, Int4 y, Int4 dy)
{
        Int4    midp;

        midp = (x1+x2)/2;

        points[0].x = MIN(midp, x1+dx); points[0].y = y;
        points[1].x = x1;               points[1].y = y+0.5*dy;
        points[2].x = MIN(midp, x1+dx); points[2].y = y+dy;

}       /* GetLeftAnglePoints */




static void
GetRightAnglePoints(gdPoint *points, Int4 x1, Int4 x2, Int4 dx, Int4 y, Int4 dy)
{
        Int4 midp;

        midp = (x2+x1)/2;

        points[0].x = MAX(midp, x2-dx); points[0].y = y;
        points[1].x = x2;               points[1].y = y+0.5*dy;
        points[2].x = MAX(midp, x2-dx); points[2].y = y+dy;

}       /* GetRightAnglePoints */




static void GetPoints(gdPoint *points, Int2 *n, CharPtr flag, 
			Int4 x1, Int4 x2, Int4 y, Int4 dy)
{
	Int4 midp, dx;

	midp = (x1+x2)/2;

	switch (flag[0]) {
	case 'L':  	/* Left zigzag */
                GetRightAnglePoints(points, x1, x2, 7, y, dy);
                points[3].x = x1;               points[3].y = y+dy;
                points[4].x = MIN(x1+3, midp);  points[4].y = y+dy*0.75;
                points[5].x = x1;               points[5].y = y+dy*0.5;
                points[6].x = MIN(x1+3, midp);  points[6].y = y+dy*0.25;
                points[7].x = x1;               points[7].y = y;
                *n = 8;
		break;
	case 'R':	/* right zigzag */
                GetLeftAnglePoints(points, x1, x2, 7, y, dy);
                points[3].x = x2;               points[3].y = y+dy;
                points[4].x = MAX(x2-3, midp);  points[4].y = y+dy*0.75;
                points[5].x = x2;               points[5].y = y+dy*0.5;
                points[6].x = MAX(x2-3, midp);  points[6].y = y+dy*0.25;
                points[7].x = x2;               points[7].y = y;
                *n = 8;
		break;
	case 'W':	/* two zigzag lines */
                points[0].x = x1;               points[0].y = y;
                points[1].x = x2;               points[1].y = y;
                points[2].x = MAX(x2-3, midp);  points[2].y = y+dy*0.25;
                points[3].x = x2;               points[3].y = y+dy*0.5;
                points[4].x = MAX(x2-3, midp);  points[4].y = y+dy*0.75;
                points[5].x = x2;               points[5].y = y+dy;
                points[6].x = x1;               points[6].y = y+dy;
                points[7].x = MIN(x1+3, midp);  points[7].y = y+dy*0.75;
                points[8].x = x1;               points[8].y = y+dy*0.5;
                points[9].x = MIN(x1+3, midp);  points[9].y = y+dy*0.25;
                *n = 10;
                break;
        case 'A':	/* Aligned, so two angles */
		if (x2-x1 <=8) dx=3;
		else if (x2-x1 <= 18) dx = 4;
		else dx = 7;

                points[0].x = MIN(midp, x1+dx);  points[0].y = y;
                if (x2-x1 < 8) {
			points[1].x = x2;  	/* points[1].y = y; */
			points[3].x = x2;  	/* points[3].y = y + dy; */
		}
		else {
			points[1].x = MAX(midp, x2-dx); 
			points[3].x = MAX(midp, x2-dx);  
		}
                points[2].x = x2;                points[2].y = y+0.5*dy;
                points[4].x = MIN(midp, x1+dx);  points[4].y = y+dy;
                points[5].x = x1;                points[5].y = y+0.5*dy;

		points[1].y = y; 		 points[3].y = y+dy;

                *n = 6;
                break;
        case 'U':	/* Un...,  special for query */
                points[0].x = x1;       points[0].y = y;
                points[1].x = x2;       points[1].y = y;
                points[2].x = x2;       points[2].y = y+dy;
                points[3].x = x1;       points[3].y = y+dy;
                *n = 4;
                break;
	default:
		PrtMes(MAILto, "VASTSRV (VastGraph)", 
		    "Error in GetPoints -- didn't choose points", NULL, FALSE);
	}
		
}	/* GetPoints */



#define HTMLurl "href=\"%s%s?sdid=%d&viewali=View&action=0&alitype=h"
#define VSHTMLurl "href=\"%s%s?chaindom=%d&viewali=View&action=0&alitype=h"

static Int4 
BlockMapOrImg(Boolean ismap, Int4 x, Int4 y, Int4 x0, Int4 from, Int4 to, 
	Int4 tick_from, Int4 tick_to, Int2 color, Int4 dy, Int4 pre_t,
	Int4 next_f, Boolean isaligned, Int4 Fsid, VastPageDataPtr vpp, 
	Int4 iSlv, Int4 numhitsdisplayed, CharPtr JobID, CharPtr pcPass, 
	FILE *File)
{
  char	altstr[200], str[MAX_TBUFF], str2[20], strnum[MAX_TBUFF];
  Int2 	i;
  Int4 	x1=0, x2=0;
  gdPoint  points[10];
  
  CalCoor(&x1, &x2, x, from, to, pix_per_res, MaxSeqImgSize);
  if (iSlv < 0) if (x1-x0 <2) x1 = x0+2;

  if (ismap == TRUE) {

    if (iSlv < 0) {
    	if (dy==8) sprintf(altstr, "Unaligned residues ");
    	else sprintf(altstr, "Maximal aligned region: from residues ");
	sprintf(str, "%d to %d, ", tick_from, tick_to);
      	StringCat(altstr, str);
	StringCat(altstr,"click for multiple alignment with neighbors.");
        sprintf(strnum, "%d%%2C", Fsid*10);
	for (i=0; i< numhitsdisplayed; i++) {
            sprintf(str2, "%d%%2C", vpp[i].bBsfId);
            StringCat(strnum, str2);
	}	
    }
    else {
	sprintf(altstr, 
		"Aligned residues %d to %d, click for sequence alignment.",
		tick_from, tick_to);
    }

    fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ", 
		x1, y, x2, y+dy);

    if (JobID) fprintf(File, VSHTMLurl, URLcgi, CGIname, Fsid);
    else fprintf(File, HTMLurl, URLcgi, CGIname, aSdi);
    if (iSlv < 0)  {
	fprintf(File, "&nbr_complexity=0&allbsfid=%s", strnum);
    }
    else fprintf(File, "&nbr_complexity=1&hit=%d", vpp[iSlv].bBsfId);
    if (JobID) fprintf(File, "&vsid=%s&pass=%s", JobID, pcPass);
    fprintf(File, "\"");
    fprintf(File, " alt=\"%s\" ", altstr);
    fprintf(File, "ONMOUSEOVER=\"self.status=\'%s\'; return true\">\n", altstr);
  }
  else {
     if (isaligned == TRUE) {
  	if (!pre_t)  {
	    Int2   n;

	    if (!next_f) {
		GetLeftAnglePoints(points, x1, x2, 7, y, dy);
		points[3].x = x2; 	points[3].y = y+dy;
		points[4].x = x2; 	points[4].y = y;
		n = 5;
	    }
	    else GetPoints(points, &n, "A", x1, x2, y, dy);

	    gdImageFilledPolygon(im, points, n, color);
	
	}
	else if (next_f) {
		GetRightAnglePoints(points, x1, x2, 7, y, dy);
		points[3].x = x1; 	points[3].y = y+dy;
		points[4].x = x1; 	points[4].y = y;
		gdImageFilledPolygon(im, points, 5, color);
	}
	else gdImageFilledRectangle(im, x1, y, x2, y+dy, color);
     }
     else gdImageFilledRectangle(im, x1, y, x2, y+dy, color);
  }
		
  return(x2);

}  /* end of BlockImg */


#define EntrezLink "href=\"%s?cmd=Retrieve&db=%s&list_uids=%d&dopt=%s\""

static void NameMapOrImg(Boolean ismap, Int4 *x, Int4 y, CharPtr pdbname, 
		Char chainname, Int4 iDomain, Int4 iGi, Int4 seqlen, 
		CharPtr pcSlaveName, FILE *File)
{  

  Char 	cTmp[10], str[10], altstr[MAX_TBUFF];
  Int4	x0;

  sprintf(cTmp, "%s %c", pdbname, chainname);
  if (iDomain) {
	sprintf(str, " %d", iDomain);
	StringCat(cTmp, str);
  }
  if (ismap == TRUE) {
    sprintf(altstr, "%s", pdbname);
    if (chainname != ' ') {
	sprintf(str, "_%c", chainname);
	StringCat(altstr, str);
    }
    if (iDomain) {
		StringCat(altstr, "_");
		sprintf(str, "%d", iDomain);
		StringCat(altstr, str);
    }
    fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ", 
	    *x, y, *x+StrLen(cTmp)*FontBW, y+FontBH);
    if (pcSlaveName != NULL) {
	sprintf(str, ": %d", seqlen);
    	StringCat(altstr, str),
    	StringCat(altstr, " residues");
        if (!iDomain) StringCat(altstr, ", ");
	else {
	   if (chainname != ' ') sprintf(str, " on chain %c, ", chainname);
	   else sprintf(str, " on chain, ");
	   StringCat(altstr, str);
	}
	StringCat(altstr, pcSlaveName);
        fprintf(File, "href=\"%s%s?uid=%s", URLcgi, MMDBCGIname, pdbname);
        fprintf(File, "&form=6&db=t&Dopt=s\" ");
    }
    else if (iGi) {
	StringCat(altstr, ", click for Entrez sequence summary");
	fprintf(File, EntrezLink, ENTREZurl, "protein", iGi, "GenPept"); 
    }

    fprintf(File, "alt=\"%s\" ", altstr);
    fprintf(File, "ONMOUSEOVER=\"self.status=\'%s\'; return true\">\n",
	    altstr);
  }
  else {
    gdImageString(im, gdFont7X13b, *x, y, cTmp, blue); 
    y += FontBH;
    x0 = *x + StrLen(pdbname)*FontBW;
    gdImageLine(im, *x, y, x0, y, blue);
    x0 += FontBW;
    if (chainname != ' ') gdImageLine(im, x0, y, x0+FontBW, y, blue);
    x0 += 2*FontBW;
    if (iDomain) gdImageLine(im, x0, y, x0+FontBW, y, blue);
  }
 
  *x += 11*FontBW;

}  /* end NameMapOrImg */


static void PrintAliMap(Int4 x1, Int4 y1, Int4 x2, Int4 y2, Int4 tick_f, 
	Int4 tick_t, Int4 Fsid, VastPageDataPtr vpp, Int4 iSlv, CharPtr JobID, 
	CharPtr pcPass, FILE *File)
{
   char cTmp[MAX_TBUFF], str[100];

   sprintf(cTmp, "Aligned residues %d to %d, click for sequence alignment", 
		tick_f, tick_t);
   fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ", 
		x1, y1, x2, y2);
   if (JobID) fprintf(File, VSHTMLurl, URLcgi, CGIname, Fsid);
   else fprintf(File, HTMLurl, URLcgi, CGIname, aSdi);
   fprintf(File, "&nbr_complexity=1&hit=%d", vpp[iSlv].bBsfId);
   if (JobID) fprintf(File, "&vsid=%s&pass=%s", JobID, pcPass);
   fprintf(File, "\"");
   fprintf(File, " alt=\"%s\" ", cTmp);
   fprintf(File, "ONMOUSEOVER=\"self.status=\'%s\'; return true\"", cTmp);
   fprintf(File, ">\n");
}



static Int4
InsertOnSlaveMapOrImg(Boolean ismap, Int4 x, Int4 y, Int4 x0, Int4 from, 
	Int4 to, Int4 tick_from, Int4 tick_to, Int4 pre_to, Int4 next_from, 
	IntvlPairDataPtr ipdp, Int4 *segNo,Int4 segNum, Int4 Fsid, 
	VastPageDataPtr vpp, Int4 iSlv, CharPtr JobID, CharPtr pcPass, FILE *File)
{
  Int2 n=0; 
  Int4 x1, x2, dy,i, idx, next, f_res, t_res;
  gdPoint points[10];

  dy = 10;
  idx = segNo[0];
  t_res = ipdp[idx-1].mstto;
  CalCoor(&x1, &x2, x, from, t_res, pix_per_res, MaxSeqImgSize);
  if (x1-x0 < 2) x1 = x0+2;
  x2 -= 1.0 + MIN(0, (Int4)(pix_per_res/2.0));
  if (ismap == TRUE) 
	PrintAliMap(x1, y, x2, y+dy, tick_from, ipdp[idx-1].slvto, 
		Fsid, vpp, iSlv, JobID, pcPass, File);
  else {
     if (!pre_to) {
	Int2 	n;

	GetLeftAnglePoints(points, x1, x2, 7, y, dy);
	points[3].x = x2;       points[3].y = y+dy;
        points[4].x = x2;       points[4].y = y;
        n = 5;

	gdImageFilledPolygon(im, points, n, red);
     }
     else 
     	gdImageFilledRectangle(im, x1, y, x2, y+dy, red);
  }
	
  for (i=0; i< segNum; i++) {
    idx = segNo[i];
    f_res = ipdp[idx].mstfrom;
    CalCoor(&x1, NULL, x, f_res, 0, pix_per_res, MaxSeqImgSize);
    x1 += 1.0 + MIN(0, (Int4)(pix_per_res/2.0));
/*    if (ismap == FALSE) gdImageFilledRectangle(im, x2, y, x1, y+dy, sky);  */
    if (i< segNum-1) {
      next = segNo[i+1];
      t_res = ipdp[next].mstfrom-1;
      CalCoor(NULL, &x2, x, 0, t_res, pix_per_res, MaxSeqImgSize);
      x2 -= 1.0 + MIN(0, (Int4)(pix_per_res/2.0));
      if (ismap==TRUE) 
	PrintAliMap(x1, y, x2, y+dy, ipdp[idx].slvfrom, ipdp[next].slvfrom-1, 
		Fsid, vpp, iSlv, JobID, pcPass, File);
      else gdImageFilledRectangle(im, x1, y, x2, y+dy, red);
    }
  }

  CalCoor(NULL, &x2, x, 0, to, pix_per_res, MaxSeqImgSize);
  if (ismap == TRUE) { 
       PrintAliMap(x1, y, x2, y+dy, ipdp[idx].slvfrom, tick_to, Fsid,
		vpp, iSlv, JobID, pcPass, File);
  }
  else {
  	if (next_from) {
  	     GetRightAnglePoints(points, x1, x2, 7, y, dy);
  	     points[3].x = points[4].x = x1;
  	     points[3].y = y+dy; points[4].y = y;
	     n = 5;
	     gdImageFilledPolygon(im, points, 5, red);

        }
  	else gdImageFilledRectangle(im, x1, y, x2, y+dy, red);
  }

  return(x2);

}	/* end InsertOnSlaveMap... */




static void
AlignmentMapOrImg(Boolean ismap, Int4 x, Int4 y, Int4 Fsid, Int4 iSlv, 
	Int4 seqlen_s, VastPageDataPtr vpp, CharPtr stats, Int2 sortnamelen, 
	CharPtr JobID, CharPtr pcPass, FILE *File)
{
  char		pdbId_s[5], chnLett_s;
  CharPtr	pcSlaveName = NULL;
  Int2		i, j;
  Int4		x0=-1, dy, from_m,from_s;
  Int4		to_m, to_s, ali_from_m, ali_to_m=0, ali_from_s=0;
  Int4		ali_to_s=-1, pre_to_s=0, segNo[20];
  Int4		segNum = 0, domNo_s;
  IntvlPairData	*ipdp;

  dy = 10;

  if (ismap == TRUE) {
      pcSlaveName = (CharPtr) MemNew ((size_t)(2*DBStrSize+4));
      pcSlaveName[0] = '\0';
      if(vpp[iSlv].name[0] != '\0' || vpp[iSlv].name2[0] != '\0') {
          if(vpp[iSlv].name[0] != '\0')
                strcpy(pcSlaveName,vpp[iSlv].name);
          if(vpp[iSlv].name2[0] != '\0')
                strcat(pcSlaveName,vpp[iSlv].name2);
      }
  }

  StrCut(pdbId_s, vpp[iSlv].bDomName, 1, 4);
  chnLett_s = (vpp[iSlv].bDomName)[4];
  domNo_s = (Int4)atoi(vpp[iSlv].bDomName+5);
  ipdp = vpp[iSlv].Ipdp;

  NameMapOrImg(ismap, &x, y, pdbId_s, chnLett_s, domNo_s, 0, seqlen_s, 
		pcSlaveName, File);

  if (ismap==TRUE) MemFree(pcSlaveName);

  for (i = 0; i< vpp[iSlv].IpdpLen; i++) {
    from_m = ipdp[i].mstfrom;
    to_m = ipdp[i].mstto;
    from_s = ipdp[i].slvfrom;
    to_s = ipdp[i].slvto;

/*  red piece for aligned region */		   
    if (!i) {
	ali_from_m = from_m;
	ali_from_s = from_s;
    }

    
    if (i && from_m == ali_to_m+1) {
		if (from_s > ali_to_s+1) segNo[segNum++] = i;
    }
    else {
  	if (i) {
	
	   if (!segNum)
	 	x0 = BlockMapOrImg(ismap, x, y, x0, ali_from_m, ali_to_m, 
			ali_from_s, ali_to_s, red, dy, pre_to_s, 0, TRUE, Fsid,
			vpp, iSlv, 0, JobID, pcPass, File); 
	   else
		x0 = InsertOnSlaveMapOrImg(ismap, x, y, x0, ali_from_m, 
			ali_to_m, ali_from_s, ali_to_s, pre_to_s, 0, ipdp, 
			segNo, segNum, Fsid, vpp, iSlv, JobID, pcPass, File);

	   for (j = ali_from_m-1; j< ali_to_m; j++) QueryBit[j] = '1';

      	   if (!pre_to_s) pre_to_s = ali_to_s;
      	}
	

      ali_from_m = from_m;
      ali_from_s = from_s;
      segNum = 0;
    }

    ali_to_m = to_m;
    ali_to_s = to_s;

  }	/* for (i = 0 ... */	


/* rectangle with red color from from_m to to_m ----  aligned block */

  if (!segNum) 
  	x0 =BlockMapOrImg(ismap, x, y, x0, ali_from_m, ali_to_m, ali_from_s,
		ali_to_s, red, dy, pre_to_s, 1, TRUE, Fsid, vpp, iSlv, 0, 
		JobID, pcPass, File);
  else 
	x0 = InsertOnSlaveMapOrImg(ismap, x, y, x0, ali_from_m, ali_to_m, 
		ali_from_s, ali_to_s, pre_to_s, 1, ipdp, segNo, segNum, 
		Fsid, vpp, iSlv, JobID, pcPass, File);

  for (j = ali_from_m-1; j< ali_to_m; j++) QueryBit[j] = '1';

/* print stats */

  x =GraphWidth - (MAX(sortnamelen, StrLen(stats)) + StrLen(stats))*FontBW/2.0;
  if (ismap==FALSE)
	gdImageString(im, gdFont7X13b, x, y, stats, blue);

} /* end of AlignImg() */




static void 
GetLeft4Points(Int4 x1, Int4 x2, Int4 y1, Int4 y2, gdPointPtr points)
{
	Int4 i=0;

	points[i].x = x2; 	points[i++].y = y1;
	points[i].x = x1; 	points[i++].y = y1+2;
	points[i].x = x1; 	points[i++].y = y2-2;
	points[i].x = x2; 	points[i++].y = y2;

}




static void 
GetRight4Points(Int4 x1, Int4 x2, Int4 y1, Int4 y2, gdPointPtr points) 
{
	Int4 i=0;

	points[i].x = x1; 	points[i++].y = y2;
	points[i].x = x2; 	points[i++].y = y2-2;
	points[i].x = x2; 	points[i++].y = y1+2;
	points[i].x = x1; 	points[i++].y = y1;
}


#define CDDsrv "href=\"%s?ascbin=2&maxaln=10&seltype=3&uid=%s&aln=%s&querygi=%d&querynm=%s&version=%s\""
#define CDDOpt "db=cdd&term="
#define DomOpt "db=structure&cmd=Display&dopt=structure_domains&from_uid="


static void QueryMapOrImg(Boolean ismap, Int4 x, Int4 y, CharPtr pcPDB, 
		Int4 iMMDBid, Int4 Fsid, Char cChain, Int4 domNo, Int4 gi, 
		Int4 seqlen, DomIntvlDataPtr dip, Int4 domintvl_no, VastPageDataPtr vpp,
		Int4 numhitsdisplayed, CharPtr JobID, CharPtr pcPass,FILE *File)
{
  char		cTmp[MAX_TBUFF], shortname[MAX_TBUFF], str[MAX_TBUFF];
  char		defline[MAX_TBUFF], def2[MAX_TBUFF], *chtmp;
  char		cdalign[MAX_TBUFF], querynm[MAX_TBUFF];
  char		definition[MAX_TBUFF];
  CharPtr	*CddName;
  DenseSegPtr	*dsp;
  Int2 		i, color, char_num;
  Int4 		alinumseg,j, x0=-1,y0, x1, x2, y1, y2, len, dont, x_ori, numseg;
  Int4Ptr	starts, lens;
  Int4 		index, ntick, tick, right, dy=0, from, to, len1, len2, minali; 
  OverLoc	*head, *end;
  SeqAnnotPtr	sap = NULL;
  SeqAlignPtr	salp= NULL;
  static Int4 	ticksteps[9] = {5, 10, 20, 25, 50, 100, 200, 150, 500};

  x_ori = x;

  NameMapOrImg(ismap, &x, y, pcPDB, cChain, 0, gi, seqlen, NULL, File);

  y1 = y+3;

/* sequence ruler */

  j = minali = 0;
  while (j < seqlen) {
     char  	bitj;
     Int4 	color, k, l, dy, pre_t, next_f, y0;
     Boolean	aliflag;

     bitj = QueryBit[j];
     switch (bitj) {
	case '0': color=gray; aliflag = FALSE; dy = 8;
		  pre_t = next_f = 1; y0 = y1;
		  break;
	case '1': color = red; aliflag = TRUE; dy = 14;
		  if (!minali) {
			minali = 1; pre_t = 0;
		  }
		  else pre_t = 1;
		  y0 = y;
		  break;
     }

     k = j+1;
     while (k< seqlen && bitj == QueryBit[k]) k++;

     if (bitj == '1') {
	l = k+2;
     	while (l< seqlen && QueryBit[l] != bitj) l++;
     	if (l >= seqlen) next_f = 1;
 	else next_f = 0;
     }	
     
     x0 = BlockMapOrImg(ismap, x, y0, x0, ++j, k, j, k, color, dy, pre_t, 
		next_f, aliflag, Fsid, vpp, -1, numhitsdisplayed, JobID, 
		pcPass, File); 
     j = k;
  }

  
  if (ismap == FALSE) {

    /* Add ticks */

    CalCoor(NULL, &right, x, 0, seqlen, pix_per_res, MaxSeqImgSize);

    y2 = y-5;
    if (QueryBit[0] == '1') gdImageLine(im, x, y, x, y2, red);
    else  gdImageLine(im, x, y1, x, y2, red);
    if (QueryBit[seqlen-1] == '1') gdImageLine(im, right, y, right, y2, red);
    else gdImageLine(im, right, y1, right, y2, red);

    y2 -= FontMH;

    gdImageString(im, gdFont6X12, x, y2, "1", red);
    sprintf(cTmp, "%d", seqlen);
    gdImageString(im, gdFont6X12, right-(StrLen(cTmp)-1)*FontMW, y2, cTmp, red);

    y2 += FontMH;

    for (i=0; i< 9; i++) {
      ntick = seqlen/ticksteps[i];
      if (ntick < 10) {
	tick = ticksteps[i];
	break;
      }
    }

    x0 = x + 2*FontMW;
    dont = right - StrLen(cTmp)*FontMW;
    for (i=1; i<=ntick; i++){
      x1 = (Int4)(x+(float)(i*tick-1) * pix_per_res);
      if (x1 > dont) continue;

      if (QueryBit[i*tick-1] == '1') gdImageLine(im, x1, y, x1, y2, red);
      else gdImageLine(im, x1, y1, x1, y2, red);

      sprintf(cTmp, "%d", i*tick);
      len = StrLen(cTmp)*FontMW/2;
      x2 = x1 + len;
      x1 -= len;
      if (x2<dont && (x2-x0 > len)) { 
	gdImageString(im, gdFont6X12, x1, y2-FontMH, cTmp, red); 
	x0 = x2 + FontMW;
      }
    } 
  }

/* "3d Domains" & "CDs" */

  if (!gi) return;

  if (domintvl_no > 1) {
    x = x_ori;  y += FontBH+5;

    if (ismap == FALSE) 
	NameMapOrImg(ismap, &x, y, "3d Dom.", ' ', 0, 0, 0, NULL, NULL);
    else {
	fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ",
		x, y, x+StrLen("3d Dom.")*FontBW, y+FontBH);
	sprintf(cTmp, "Click for Entrez 3d Domains");
	fprintf(File, "href=\"%s?%s%d\" alt=\"%s\" ", 
			ENTREZurl, DomOpt, iMMDBid, cTmp);
	fprintf(File,"ONMOUSEOVER=\"self.status=\'%s\'; return true\">\n",cTmp);
	x += 11*FontBW;
    }
	
    if (!domNo) domintvl_no = 1;
    for (i = 0; i< domintvl_no; i++) {

      if (domNo && !dip[i].domNo) continue; 

      y2 = y+FontBH+2;
      if (dip[i].domNo != domNo) {
	if (ismap == FALSE) color = gray;
	y1 = y+1;
	y2 -= 2;
      }
      else {
	if (ismap == FALSE) color = red;
	y1 = y;
      }

      CalCoor(&x1, &x2, x, dip[i].frm, dip[i].tu, pix_per_res, MaxSeqImgSize);
      if (ismap == TRUE) {
	
	  sprintf(cTmp, "Residue %d to %d", dip[i].frm, dip[i].tu);
	  StringCat(cTmp, ", click for structure neighbors");

	  fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ",
		  x1, y1, x2, y2);
	  if (JobID) {
		fprintf(File, "href=\"%s%s?chaindom=%d", URLcgi, CGIname,
			(Fsid+(dip[i].domNo - domNo)));
	  }
	  else 	{
		fprintf(File, "href=\"%s%s?sdid=%d", URLcgi, CGIname, 
			LongDomIdToSdi(Fsid +(dip[i].domNo - domNo)));
	  }
	  fprintf(File, "&form=6&db=t&Dopt=s\" alt=\"%s\" ", cTmp);
	  fprintf(File, "ONMOUSEOVER=\"self.status=\'%s\'; return true\">\n",
		  cTmp);
      }
      else {
	  gdPoint points[10];
	  Int2	  n;	
	  
	  if (dip[i].domNo != domNo) {
	     if (dip[i].frm == 1) {
		points[0].x = x1;   	points[0].y = y1;
		points[1].x = x1;	points[1].y = y2;
		n = 2;
	     }
	     else {
		GetLeft4Points(x1, MIN((x1+x2)/2, x1+2), y1, y2,points);
		n = 4;
	     }

	     if (dip[i].tu == seqlen) {
		points[n].x = x2;	points[n++].y = y2;
		points[n].x = x2;	points[n++].y = y1;
	     }
	     else {
		GetRight4Points(MAX((x1+x2)/2, x2-2), x2, y1, y2, points+n);
		n += 4;
	     }
	     gdImageFilledPolygon(im, points, n, color);
	  }
	  else 
	  	gdImageFilledRectangle(im, x1, y1, x2, y2-2, color); 

	  if (dip[i].domNo) sprintf(cTmp, "%d", dip[i].domNo);
	  else sprintf(cTmp, "Chain %c", cChain);
	  if (x2-x1>StrLen(cTmp)*FontBW) {
	    x0 = (x1+x2-StrLen(cTmp)*FontBW)/2.0; 
	    gdImageString(im, gdFont7X13b, x0, y+1, cTmp, white);
	  }
      }

    } /* for (i=0; i< domintvl_no  */
  }

  cTmp[0] = '\0';

  if (Dart_Gi2Sap(dartcon, gi, &sap, NULL)  && (sap != NULL)) {

    unsigned	*Pssmlist, *PssmId, count;
    Int2	*iColor, CdNum, thisColor=0, *iClus;
    Int4	NumRows;

    Char CDDdb[1024], Version[1024];
    Dart_Version(dartcon, CDDdb, 100, Version, 1024);
    if (StrLen(CDDdb))
    	StrCut(Version, Version, StrLen(CDDdb)+2, StrLen(Version));

    if (!Dart_CdNum(dartcon, &count))
            PrtMes("chenj@ncbi.nlm.nih.gov",
                        "MmdbSrv", "Can't do Dart_CdNum()", NULL, FALSE);
    Pssmlist = (unsigned *) MemNew (count * sizeof (unsigned));

    end = head = NewOverLoc(seqlen);
    head->y = y0 = y+7+FontBH;
    head->next = NULL;

    for (salp = (SeqAlignPtr)sap->data, CdNum=0; salp!=NULL; 
		salp=salp->next, CdNum++); 
    
    iColor = (Int2Ptr) MemNew (CdNum * sizeof(Int2));
    PssmId = (unsigned *) MemNew (CdNum * sizeof(unsigned));
    dsp = (DenseSegPtr *) MemNew (CdNum * sizeof(DenseSegPtr));
    CddName = (CharPtr *) MemNew (CdNum * sizeof(CharPtr));
    for (j=0; j< CdNum; j++) CddName[j] = (CharPtr) MemNew (30);
    iClus = (Int2Ptr) MemNew (CdNum * sizeof(Int2));
    
    for (salp = (SeqAlignPtr)sap->data, i=0; salp!=NULL; salp=salp->next, i++) {
      dsp[i] = (DenseSegPtr)salp->segs;
      PssmId[i] = GetPSSMID(dsp[i]); 
      iClus[i] = -1;

      if (PssmId[i]) 
      	Dart_CDGi2Acc(dartcon, PssmId[i], CddName[i], 30);
      else {
	CharPtr strtmp;

	strtmp = GetCDDName(dsp[i]);
	sprintf(CddName[i], strtmp);
      }
     
    }
 

    for (i=0; i< CdNum; i++) {

	if (iClus[i] >= 0) continue;

	if (ismap == FALSE) iColor[i] = (thisColor++) % iNcolors;
	iClus[i] = i;

	if (Dart_Related(dartcon, CddName[i],Pssmlist,count,&NumRows, NULL)) {

          Int2 k;

	  if (NumRows > count)
             PrtMes("chenj@ncbi.nlm.nih.gov", "VASTSRV",
                "Dart_Related(): NumRows>MaxPssm", NULL, FALSE);

	  for (j=0; j< NumRows; j++) {
	    for (k=i+1; k< CdNum; k++) 
	      if (PssmId[k] == Pssmlist[j]) {
		    if (ismap == FALSE) iColor[k] = iColor[i];
		    iClus[k] = i;
	 	}
	  }
	}
    }


    sprintf(querynm, "%s%c(query)", pcPDB, cChain);
    for (i=0; i< StrLen(querynm); i++)
             if (isalpha(querynm[i])) querynm[i] = tolower(querynm[i]);
    if (querynm[4] == ' ') querynm[4] = '+';

    for (i=0; i< CdNum; i++) {
	
      numseg = (dsp[i])->numseg;
      starts = (dsp[i])->starts;
      lens = (dsp[i])->lens;
      from  = starts[0] + 1;
      to = starts[(numseg-1)*2] + lens[numseg-1];

      CalCoor(&x1, &x2, x, from, to, pix_per_res, MaxSeqImgSize);
      y = GetY1(head, &end, from, to, seqlen, iClus[i], 5);
      if (y < 0) continue;
      y2 = y+FontBH+2;

      Dart_Acc2Info(dartcon, CddName[i], shortname, MAX_TBUFF, defline,
		    MAX_TBUFF, definition, MAX_TBUFF);

      if (ismap == TRUE) {
	if (StrLen(defline) == 254) strcpy((defline+StrLen(defline)-3), "...");
	if (defline[StrLen(defline)-1] != '.') StringCat(defline, ".");

	chtmp = strchr(defline, ';');
	sprintf(str, "%s:", CddName[i]);
	sprintf(def2, defline);
	if (chtmp) {
	    strncpy(def2, def2, (chtmp-defline-1));
	    def2[chtmp-defline]='.';
            def2[chtmp-defline+1]='\0';
	}
        else {
            chtmp = strchr(defline, '.');
            if (chtmp) {
            	strncpy(def2, def2, (chtmp-defline));
                def2[chtmp-defline+1]='\0';
            }
            else
                StringCat(def2, ".");
        }
        StringCat(str, def2);
        StringCat(str, " Click for the CD alignment.");
        strcpy(def2, str);

	alinumseg = 0;
	cdalign[0] = '\0';
	for (j=0; j< numseg; j++) {
             index = 2*j;
             if (starts[index]!= -1 && starts[index+1] != -1) {
		 sprintf(str, ",%d,%d,%d", starts[index+1], starts[index],
				lens[j]);
		 StringCat(cdalign, str);
                 alinumseg++;
              }
        }
        sprintf(str, "%d", alinumseg);
        StringCat(str, cdalign);

	fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ",
		x1, y, x2, y2);
	fprintf(File, CDDsrv, CDDurl, CddName[i], str, gi, querynm, Version);
	fprintf(File, "alt=\"%s: %s %s\" ", CddName[i], defline, 
			"Click for CD alignment");
	fprintf(File, "ONMOUSEOVER=\"self.status=\'%s\'; return true\">\n",
		def2);

        StringCat(cTmp, "+OR+");
        StringCat(cTmp, CddName[i]);
        StringCat(cTmp, "[ACCN]");

      }
      else {  	/* ismap == FALSE */

	gdImageRoundRectangle(im, x1, y, x2, y2, 8, 5, 
		iDartCol[iColor[i]], 1);
    
	len1 = StrLen(shortname)*FontBW;
	len2 = StrLen(shortname)*FontMW;
	color = white;
	if (iColor[i] == 2 || iColor[i] == 4 || iColor[i] == 0) color=black;
	if ((x2-x1)> len1) 
		gdImageString(im, gdFont7X13b, (x1+x2-len1)/2, y+1, 
				shortname, color);
	else if ((x2-x1) > len2) 
		gdImageString(im, gdFont6X12, (x1+x2-len2)/2, y+1, 
			      shortname, color);
	else {
		char_num = (x2-x1)/FontBW;
	         if (char_num >= 3) strcpy(&(shortname[char_num-3]), "...");
         	else switch (char_num) {
                	case 1: strcpy(shortname, "."); break;
                	case 2: strcpy(shortname, "..");
              	}
	  	gdImageString(im, gdFont7X13b, x1+2, y+1, shortname, color);
	}
      }

      if (!dy) {
	dy = y0-y;
	y0 = y;
      }
    }

    if (!dy) dy=4;
    else dy = 10;

    x = x_ori;
    if (ismap == FALSE)
      NameMapOrImg(ismap, &x, head->y+2, "CDs", ' ', 0, 0, 0, NULL, NULL);
    else {
	sprintf(str, "Click for Entrez Conserved Domains.");
	fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ",
		x, head->y+dy, x+StrLen("CDs")*FontBW, head->y+dy+FontBH);
	fprintf(File, "href=\"%s?%s%s\" alt=\"%s\" ", 
			ENTREZurl, CDDOpt, cTmp+4, str);
	fprintf(File, "ONMOUSEOVER=\"self.status=\'%s\'; return true\">\n",str);
    }
	

    FreeOverLoc(head);
    MemFree(iColor);
    MemFree(PssmId);
    MemFree(dsp);
    for (i=0; i< CdNum; i++) MemFree(CddName[i]);
    MemFree(CddName);
    MemFree(iClus);
    MemFree(Pssmlist);
  }

} /* end of QueryMapOrImg */


#define MaxDipLen 120

void ImgMapOrDraw(Boolean ismap, Int4 Fsid, VastPageDataPtr vpp, 
	Int4 numhitsdisplayed, Int4 iKept, CharPtr selnbrstring, 
	CharPtr selsdidstring, SortBy sortby,
	SubSetID subsetnum, Int4 pagenum, Int4 StartLoc, CharPtr JobID, 
	CharPtr pcPass, FILE *File)
{
  char          stats[100], str[100], a_pdbId[5];
  char		a_chainLett;
  DomIntvlData	dip[MaxDipLen];
  float         f;
  Int4          i, a_MMDBid, a_chainid, a_domNo, a_gi=0, a_seqlen, b_seqlen;
  Int4		b_MMDBid, b_chainid;
  Int4          x, y, a_domintvl_no=0;
  static char SortBy_name[6][10] ={"Ali_Len", "Score", "P_value",
                                "Rmsd", "Ali_Res.", "%Id."};
  static char altstr[6][100]={"Number of Aligned Residues", 
				"Vast Score",
				"Vast P_value",
				"Room-mean square deviation",
                                "Number of Aligned Residues",
				"Identity"};

  a_MMDBid = Fsid/10000;
  a_chainid = (Fsid % 10000) / 100;

  StrCut(a_pdbId, vpp[0].aDomName, 1, 4);
  a_chainLett = vpp[0].aDomName[4];
  a_domNo = (Int4) atol(vpp[0].aDomName+5);

  if (!JobID) {
  	a_domintvl_no = 
	    constructDomIntvlData(a_MMDBid, a_chainid, dip, MaxDipLen);

  	a_seqlen = dip[0].tu;		
  	a_gi = constructGi(a_MMDBid, a_chainid);
  }
  else {
	AsnIoPtr		aipr=NULL;
	BiostrucPtr		a_bstp = NULL;
        BioseqPtr		bsqp = NULL; 
	BioseqSetPtr		bssp = NULL;
	BiostrucResidueGraphSetPtr    stdDictionary;
        Char 			AsnPath[MAX_TBUFF], str[10];
	ObjectIdPtr		objidp = NULL;
	SeqEntryPtr		sep = NULL;
 	SeqIdPtr		sip = NULL;

	sprintf(AsnPath, VSPATH);
	StringCat(AsnPath, JobID);
	StringCat(AsnPath, "/b");
	StringCat(AsnPath, a_pdbId);

	a_bstp = FetchBS(AsnPath, 0, ONECOORDRES, 1, POWER_VIEW);

	aipr = AsnIoOpen("bstdt", "rb");
	stdDictionary = BiostrucResidueGraphSetAsnRead(aipr, NULL);
	AsnIoFlush(aipr);
    	aipr = AsnIoClose(aipr);

	sep = (SeqEntryPtr) MakeBioseqs(a_bstp, stdDictionary);

	if (!sep) 
	     PrtMes(MAILto, "VASTSRV (VastGraph, VS1)", 
			"Unable to get SeqEntry", NULL, FALSE);

	if (sep->choice == 2){
	   Int2 i=1;

	   bssp = (BioseqSetPtr)sep->data.ptrvalue;
	   sep  = (SeqEntryPtr) bssp->seq_set;
	   while (i< a_chainid) {
		sep = sep->next;
		i++;
	   }
	   if (!sep)  
		PrtMes(MAILto, "VASTSRV (VastGraph, VS2)",
                        "Unable to get SeqEntry", NULL, FALSE);

	}
 	bsqp = (BioseqPtr)sep->data.ptrvalue;	
	sip = bsqp->id;
	while (sip) {
	     if (sip->choice == SEQID_LOCAL) {
	     	sprintf(str, "%s %c %d", a_pdbId, a_chainLett, a_domNo);
	     	objidp = (ObjectIdPtr)sip->data.ptrvalue;
	        if (!StringNCmp(objidp->str, str, 6)) {
		    a_seqlen = bsqp->length; 
		    break;
	      	}
	     }
   	     sip = sip->next;
	}

	BiostrucFree(a_bstp);
  }

  pix_per_res= (float)MaxSeqImgSize/(float)a_seqlen;
  QueryBit = (CharPtr) MemNew ((size_t)(a_seqlen+1));
  for (i=0; i< a_seqlen; i++) QueryBit[i] = '0';

  y = StartLoc+3;

  x = GraphWidth - StrLen(SortBy_name[sortby])*FontBW;
  if (ismap == TRUE) {

    sprintf(str, "%s. Click for a printable table.", altstr[sortby]);
    
    fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ",
	     x, 25, GraphWidth, 25+FontBH); 
    fprintf(File, "href=");
    if (JobID) fprintf(File, VSParURL, URLcgi, CGIname, Fsid, sortby);
    else fprintf(File, ParURL, URLcgi, CGIname, aSdi, sortby);
    if (selnbrstring == NULL && selsdidstring == NULL) {
	if (pagenum)
            fprintf(File, PageSubsetURL, pagenum, subsetnum, subsetnum);
	else fprintf(File, "schsub=Find");
    }
    else {
	fprintf(File, "schsub=Find");
	if (selnbrstring) fprintf(File, "&selnbr=%s", selnbrstring);
	if (selsdidstring) fprintf(File, "&selsdid=%s", selsdidstring);
    }

    if (iKept) 
      	for (i=0; i< iKept; i++)
       		fprintf(File, "&hit=%d", vpp[i].bBsfId);
    if (JobID) fprintf(File, "&vsid=%s&pass=%s", JobID, pcPass);
    fprintf(File, "&table=y\" alt=\"%s\" ", str);
    fprintf(File, "ONMOUSEOVER=\"self.status=\'%s\'; return true\">\n", str);
  }
  else 
	NameMapOrImg(ismap, &x, 25, SortBy_name[sortby], ' ', 0, 0, 0, 
			NULL, NULL);

  x = 10;
  for (i=0; i< numhitsdisplayed; i++) { 

      b_MMDBid = vpp[i].bBsfId / 100000;
      b_chainid = (vpp[i].bBsfId % 100000) / 1000;
      b_seqlen = constructChainLength(b_MMDBid, b_chainid);

      if (!sortby || sortby==4) sprintf(stats, "%d", vpp[i].nres);
      else {
	if (sortby == 1) f = (float) vpp[i].vScore;
	else if (sortby == 2)f = (float) vpp[i].mlogp;
	else if (sortby == 3) f = (float) vpp[i].rmsd;
	else f = (float) vpp[i].pcntId;
	f /= (float) ASP_SCALE_FACTOR;
	if (sortby == 1 || sortby == 3) sprintf(stats, "%.1f", f);
	else if (sortby == 5)sprintf(stats, "%.1f", f*100.0);
	else {

          /* adjust for database size */
	  f -= LOG10_500;
	  if (f <= 4.0) {
	    f = (float) exp(-LOG_10*f);
	    sprintf(stats, "%.4f", f);
	  }
	  else sprintf(stats, "10e-%.1f", f);
	}
      } 

    AlignmentMapOrImg(ismap, x, y, Fsid, i, b_seqlen, vpp, stats, 
		StrLen(SortBy_name[sortby]), JobID, pcPass, File);
    y += 30;

  }  

  /* draw query domain as a ruller */
  
  y = 30;
  QueryMapOrImg(ismap, x, y, a_pdbId, a_MMDBid, Fsid, a_chainLett, a_domNo, 
		a_gi, a_seqlen, dip, a_domintvl_no, vpp, numhitsdisplayed,
		JobID, pcPass, File);

}  /* ImgMapOrDraw */



void DrawStrucNbr(Int4 Fsid, VastPageDataPtr vpp, Int4 numhitsdisplayed,
		Int4 iKept, CharPtr selnbrstring, CharPtr selsdidstring,
		SortBy sortby, 
		SubSetID subsetnum, Int4 pagenum, Int2 ImageSize,
		CharPtr JobID, CharPtr pcPass)
{

	Int4 StartLoc;

	printf("Content-type: image/gif\n\n");
	im = gdImageCreate(GraphWidth, ImageSize);
	white   = gdImageColorAllocate(im, 255, 255, 255);
	black   = gdImageColorAllocate(im,   0,   0,   0);
        blue    = gdImageColorAllocate(im, 0, 0, 255);
	red     = gdImageColorAllocate(im, 255, 0, 0);
        /* magenta = gdImageColorAllocate(im, 255, 0, 255); */
        gray 	= gdImageColorAllocate(im, 102, 102, 102);

/* Dart Color scheme */
	iDartCol[5]  = red;
	iDartCol[1]  = blue;
	iDartCol[2]  = gdImageColorAllocate(im, 0, 255, 0);   /* green */
	iDartCol[3]  = gdImageColorAllocate(im, 204,102,  0); /* orange */
	iDartCol[4]  = gdImageColorAllocate(im, 204,204,  0); /* yeller */
	iDartCol[0]  = gdImageColorAllocate(im, 153,204,255); /* sky b */
	iDartCol[6]  = gdImageColorAllocate(im, 102,153,  0); /* spring */
	iDartCol[7]  = gdImageColorAllocate(im, 204,102,153); /* lavender */
	iDartCol[8]  = gdImageColorAllocate(im,   0,204,204); /* cyan */
	iDartCol[9]  = gdImageColorAllocate(im, 153,153,  0); /* brown */
	iDartCol[10] = gdImageColorAllocate(im, 153, 51,255); /* violet */
	iDartCol[11] = gdImageColorAllocate(im,   0,153,153); /* blue-green */
	iDartCol[12] = gdImageColorAllocate(im,   0,204,102); /* teal */

	StartLoc = ImageSize - numhitsdisplayed*30;
	ImgMapOrDraw(FALSE, Fsid, vpp, numhitsdisplayed, iKept, selnbrstring, 
		selsdidstring, sortby, subsetnum, pagenum, StartLoc, JobID, 
		pcPass, NULL);

	gdImageGif(im, stdout);
        gdImageDestroy(im);

} /* DrawStrucNeig */

