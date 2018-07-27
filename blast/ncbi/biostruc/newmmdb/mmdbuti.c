/*
* $Id: mmdbuti.c,v 1.3 2003/01/15 16:13:42 chenj Exp $
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
* $Log: mmdbuti.c,v $
* Revision 1.3  2003/01/15 16:13:42  chenj
* add uid as an argument to ChainScaleMapOrImg()
*
* Revision 1.2  2002/12/26 21:21:20  chenj
* use Dart_CdNum() to get all Cds in database
*
* Revision 1.1.1.1  2002/12/04 21:12:08  chenj
* Imported sources
*
*
*
*=============================================================================
*/



#include "mmdbuti.h"

extern	double 	pix_per_res;
extern 	ENTREZurl[PATH_MAX], VASTurl[PATH_MAX], CDDurl[PATH_MAX];
extern	Int4	*ColorIdx, white, black, blue, red, gray, maxseqlen;
extern	Uint1	cycle1[10][3];
extern	Dart_Connect    *dartcon;

#define EntrezLink "href=\"%s?cmd=Retrieve&db=%s&list_uids=%d&dopt=%s\""

Int2 ChainNameMapOrImg(Boolean ismap, gdImagePtr im, Int2 x, Int2 y, 
	PMSD pmsd,  PMMD pmmd, Int2 protein, FILE *File)
{
    Int2 	x1, x2, y2;
    char 	tmpchain[20], tmpstr[20], db[20], dopt[20], *PDBid, *chain;
    char	str[MAX_TBUFF];
    Int4 	iGi;

    PDBid = pmsd->pcPDBName;
    chain = pmmd->pcMolName;  /* what if pmmd->pcMolName[0] == ' ');*/
    iGi = pmmd->iGi;

    if (protein) {
	sprintf(tmpchain, "Protein");
	sprintf(db, "protein");
	sprintf(dopt, "GenPept");
    }
    else {
	sprintf(tmpchain, "Nucleotide");
	sprintf(db, "nucleotide");
	sprintf(dopt, "GenBank");
    }

    x1 = x+10;
    x2 = x1 + StrLen(tmpchain)*FontBW;
    y2 = y+FontBH;

    if (ismap == TRUE) {

        strcpy(str, PDBid);
	if (chain[0] != ' ') {
	    StringCat(str, "_");
	    StringCat(str, chain);
	}
        sprintf(tmpstr, ", gi %d, ", iGi);
	StringCat(str, tmpstr);
        StringCat(str, "click for Entrez sequence summary.");
        fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ",
                                                        x1-2, y, x2, y2);
        fprintf(File, EntrezLink, ENTREZurl, db, iGi, dopt);
        fprintf(File, " alt=\"%s\"", str);
        fprintf(File, " ONMOUSEOVER=\"self.status=\'%s\'; return true\"", str);
        fprintf(File, ">\n");
    }
    else {        
	gdImageString(im, gdFont7X13b, x1, y, tmpchain, blue);
        gdImageLine(im, x1, y2, x2, y2, blue);
    }

    return(x2);
}


#define VastLink   "href=\"%s?sdid=%d\""

void 
ChainScaleMapOrImg(Boolean ismap, gdImagePtr im, Int2 x, Int2 y, PMMD pmmd, 
Int4 uid, Int4 sdid, Int4 color, Int4 labelcol, FILE *File)
{
    Char    cTmp[MAX_TBUFF], *chain;
    Int2    x0, x1, y1, x2, y2, right, seqlen;
    Int4    i, len, tick, ntick, dont; 
    static  Int4   ticksteps[14] ={5, 10, 20, 25, 50, 100, 200, 150, 500,
				  1000, 2000, 2500, 5000, 10000};

    seqlen = pmmd->iResCount;
    chain = pmmd->pcMolName;
    right = x2 = (Int4)(x+ (seqlen-1)*pix_per_res);
    y2 = y + FontBH + 2;

    if (ismap ==TRUE)  {

        sprintf(cTmp, "residues, click to see its structure neighbors.");

	if (!sdid) {
	   Int4 maxuid = constructMaxMmdbId();
	   
   	   if (uid <= maxuid) sdid=-1;
	}

        fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ", x, y, x2, y2);
        fprintf(File, VastLink, VASTurl, sdid);
        fprintf(File, " alt=\"%d %s\"", seqlen, cTmp);
        fprintf(File, " ONMOUSEOVER=\"self.status=\'%d %s\'; return true\">\n",
                                seqlen, cTmp);
    }
    else {
        gdImageFilledRectangle(im, x, y, x2, y2, color);
        sprintf(cTmp, "Chain %s", chain);

        if ((right - x) >= StrLen(cTmp)*FontBW) {
	    x1 = (x + right - StrLen(cTmp)*FontBW)/2;
	    gdImageString(im, gdFont7X13b, x1, y+1, cTmp, labelcol);
        }
        else if ((right-x) >= StrLen(cTmp)*FontMW) {
	    x1 = (x + right - StrLen(cTmp)*FontMW)/2;
	    gdImageString(im, gdFont6X12, x1, y+1, cTmp, labelcol);
        }
        else if ((right-x) >= StrLen(chain)*FontBW) {
	    x1 = (x + right - StrLen(chain)*FontBW)/2;
	    gdImageString(im, gdFont7X13b, x1, y+1, chain, labelcol);
        }
        else if ((right-x) >= StrLen(chain)*FontMW) {
	    x1 = (x + right - StrLen(chain)*FontMW)/2;
	    gdImageString(im, gdFont6X12, x1, y+1, chain, labelcol);
        }

	/* add ticks */
       	gdImageLine(im, x, y, x, y-5, red);    /* for "1"  */
        gdImageLine(im, x2, y, x2, y-5, red);    /* for "len" */

        y1= y- (5 + FontMH);
        gdImageString(im, gdFont6X12, x-2, y1, "1", red);   /* "1" */
        x0 = x+FontMW;
        cTmp[0] = '\0';
        sprintf(cTmp, "%d", seqlen);
        x2 -= (StrLen(cTmp)-1)*FontMW;
        if (x2 > x)
           gdImageString(im, gdFont6X12, x2, y1, cTmp, red);    /* "len" */
        if ((float)(seqlen-1)/(float)(maxseqlen-1)< 0.05) return;

        dont = x2 - FontMW;

        for (i=0; i< 14; i++) {
	    ntick = seqlen/ticksteps[i];
	    if (ntick < 10)  {
		tick = ticksteps[i];
		break;
	    }
        }

        for (i=1; i<= ntick; i++) {
             x1 = (Int4)(x + ((float)(i*tick)-1.0)*pix_per_res);
             if (x1 > right-FontMW)  continue;
             y1 = y; y2 = y-5;
             gdImageLine(im, x1, y1, x1, y2, red);
             cTmp[0] = '\0';
             sprintf(cTmp, "%d", i*tick);
             len = StrLen(cTmp) * FontMW/2;
             x2  = x1 + len;
             x1 -= len;
             y1 -=  (5+ FontMH);
             if (x2 < dont && ((x2-x0) > len)) {
                gdImageString(im, gdFont6X12, x1, y1, cTmp, red);
                x0 = x2+FontMW;
            }
        }
     }

     return;


} /* end of ChainScaleMapOrImg */


#define DomOpt "db=structure&cmd=Display&dopt=structure_domains&from_uid="

Int2 ChainDomMapOrImg(Boolean ismap, gdImagePtr im, Int2 x, Int2 y, Int4 uid, 
		PMMD pmmd, Int4Ptr DomIdx, IntervalHead **DomHead, FILE *File)
{
    Char                    cTom[MAX_TBUFF];
    Int4                    x1, x2, y1, y2, ytmp = 0, idx, chnNo;
    Int4                    colidx, from, to, seqlen, color=0; 
    Int4		    right, labelcol=0, chndom0;
    ResidueIntervalPntrPtr  ripp;
    IntervalHead	    *tmp_head;
	
    chnNo = pmmd->iChainId;
    idx = DomIdx[chnNo]; 
    chndom0 = 10000*uid + 100 * chnNo;
    seqlen = pmmd->iResCount;
    right = (Int4)(x + (float)(seqlen-1)*pix_per_res);

    if (DomHead[idx] == NULL) { 	/* no 3d domain */
	if (ismap == FALSE) {
	    colidx = ColorIdx[idx];
	    color = gdImageColorAllocate(im, cycle1[colidx][0], 
					cycle1[colidx][1], cycle1[colidx][2]);
	    labelcol = white;
	    if (colidx==3 || colidx==4 || colidx==5 || colidx==7
		|| colidx==8 || colidx==9)
		labelcol = black;
	}

	ChainScaleMapOrImg(ismap, im, x, y, pmmd, uid, LongDomId2Sdi(chndom0), 
		color, labelcol, File);

	ytmp = y+FontBH+4;
    }
    else {		/* has 3d domain */
	Int4	i;

	x1 = 10;
	x2 = x1 + StrLen("3d Domains")*FontBW;
	y1 = y + FontBH + 6;
	y2 = y1+ FontBH;
	if (ismap == TRUE) {
	    sprintf(cTom, "Click to learn about 3d Domains.");
	    fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ",x1,y1,x2,y2);
    	fprintf(File, "href=\"%s?%s%d\" alt=\"%s\"",ENTREZurl,DomOpt,uid,cTom);
	fprintf(File, " ONMOUSEOVER=\"self.status=\'%s\'; return true\">\n", 
		cTom);
	}
	else {
	    gdImageLine(im, x1, y2, x2, y2, blue);
	    gdImageString(im, gdFont7X13b, x1, y1, "3d Domains", blue);
	}


	for (tmp_head = DomHead[idx], i=1; tmp_head != NULL;
	     tmp_head = tmp_head->next, i++) {
                colidx = tmp_head->colidx;
                ripp = tmp_head->ripp;

                if (i == 1) {
		    Int4   lablecol;

		    if (ismap == TRUE) color = lablecol = 0;
		    else {
			color=gray;
			lablecol = white;
		    }
		    ChainScaleMapOrImg(ismap, im, x, y, pmmd, uid, 
			LongDomId2Sdi(chndom0), color, lablecol, File);
                }

                y1 = y + FontBH + 6;
                y2 = y1 + FontBH + 2;
                ytmp = y2;

                for (; ripp != NULL; ripp = ripp->next) {
		    from = ripp->from;   /* domain range */
		    to = ripp->to;
		    CalCoor(&x1, &x2, x, from, to, pix_per_res, MaxSeqImgSize);
		    x1 = MAX(x1, x);
		    x2 = MIN(x2, right);
		    if (ismap == TRUE) {
                        sprintf(cTom, "Residues %d to %d, click for structure neighbors", from, to);                        
                        fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ",
                                x1, y1, x2, y2);
                        fprintf(File, VastLink, VASTurl, 
				LongDomId2Sdi(chndom0+tmp_head->domid));
                        fprintf(File, "alt=\"%s\"", cTom);
                        fprintf(File, " ONMOUSEOVER=\"self.status=\'%s\'; return true\"", cTom);
                        fprintf(File, ">\n");
		    }
		    else {
                        gdImageFilledRectangle(im, x1, y1, x2, y2,
			 	gdImageColorAllocate(im, cycle1[colidx][0], 
					cycle1[colidx][1], cycle1[colidx][2]));

                        sprintf(cTom, "%d", tmp_head->thisdomain);

                        if ((x2 - x1 - 4) > StrLen(cTom)*FontBW) {
			    Int4  x3;

			    x3=(x1+x2 - StrLen(cTom)*FontBW)/2;
			    if (colidx==3 || colidx==4 || colidx==5
				|| colidx==7 || colidx==8 || colidx==9)
				color = black;
			    else color = white;
			    gdImageString(im, gdFont7X13b, x3, y1+1,cTom,color);
                        }
		    }
                }  /* ; ripp != NULL */

           }    /* tmp_head */
    }

    return(ytmp);
}


#define CDDsrv "href=\"%s?ascbin=2&maxaln=10&seltype=3&uid=%s&aln=%s&querygi=%d&querynm=%s&version=%s\""
#define CDDOpt "db=cdd&term="

Int2 ConsDomMapOrImg(Boolean ismap, gdImagePtr im, Int4 x, Int4 y, PMSD pmsd,
						PMMD pmmd, FILE *File)
{
    Char		cTmp[MAX_TBUFF], *PDBid,*Chain,**CddName,str[MAX_TBUFF];
    OverLoc		*end, *head;
    Int4		right, i, j, dy=0, y0, x1, x2, y1, y2, gi, seqlen;
    Int4		numseg, from, to, len1, len2, alinumseg, index;
    Int4Ptr		starts, lens;
    SeqAnnotPtr		sap = NULL;
    SeqAlignPtr     	salp = NULL;
    DenseSegPtr     	*dsp;

/*---------------------------------------------------------------------------*/
/* DART color scheme                                                         */
/*---------------------------------------------------------------------------*/
static Int4    iNcolors = 13;
static Int4    iDartCol[13][3] = {{153,204,255},     /* sky b */
                                  {  0,  0,255},     /* blue  */
                                  {  0,255,  0},     /* green */
                                  {204,102,  0},     /* orange */
                                  {204,204,  0},     /* yeller */
                                  {255,  0,  0},     /* red  */
                                  {102,153,  0},     /* spring */
                                  {204,102,153},     /* lavender */
                                  {  0,204,204},     /* cyan */
                                  {153,153,  0},     /* brown */
                                  {153, 51,255},     /* violet */
                                  {  0,153,153},     /* blue-green */
                                  {  0,204,102},     /* teal */
                                };


    gi = pmmd->iGi;
    if (!gi) return (y);

    PDBid = pmsd->pcPDBName;
    Chain = pmmd->pcMolName;
    seqlen = pmmd->iResCount;
    cTmp[0] = '\0';
    right = (Int4)(x + (float)(seqlen-1)*pix_per_res);

    if (Dart_Gi2Sap(dartcon, gi, &sap, NULL)  && (sap != NULL)) {


	unsigned    	*Pssmlist, *PssmId, count;
	Int2        	*iColor, CdNum, thisColor=0, *iClus;
	Int4        	NumRows;
	Char		querynm[MAX_TBUFF], shortname[MAX_TBUFF];
	Char	 	defline[MAX_TBUFF], definition[MAX_TBUFF];
	Char		def2[MAX_TBUFF], *chtmp, cdalign[MAX_TBUFF];

	Char 	CDDdb[1024], Version[1024];
	Dart_Version(dartcon, CDDdb, 100, Version, 1024);
	if (StrLen(CDDdb))
	    StrCut(Version, Version, StrLen(CDDdb)+2, StrLen(Version));
	
        if (!Dart_CdNum(dartcon, &count))
	    PrtMes("chenj@ncbi.nlm.nih.gov", 
			"MmdbSrv", "Can't do Dart_CdNum()", NULL, FALSE);
  	Pssmlist = (unsigned *) MemNew (count * sizeof (unsigned));

	end = head = NewOverLoc(seqlen);
	head->y = y0 = y+5;
   
	for (salp = (SeqAlignPtr)sap->data, CdNum=0; salp!=NULL;
	     salp=salp->next, CdNum++);

	iColor = (Int2Ptr) MemNew (CdNum * sizeof(Int2));
	PssmId = (unsigned *) MemNew (CdNum * sizeof(unsigned));
	dsp = (DenseSegPtr *) MemNew (CdNum * sizeof(DenseSegPtr));
	CddName = (CharPtr *) MemNew (CdNum * sizeof(CharPtr));
	for (j=0; j< CdNum; j++) CddName[j] = (CharPtr) MemNew (30);
	iClus = (Int2Ptr) MemNew (CdNum * sizeof(Int2));

	for (salp = (SeqAlignPtr)sap->data, i=0; salp!=NULL; 
						salp=salp->next, i++) {
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
	    if (ismap == FALSE) iColor[i] = (thisColor++) % iNcolors;
	    iClus[i] = i;

	    if (Dart_Related(dartcon, CddName[i],Pssmlist,count,&NumRows,NULL)){
		Int2 k;
		
		if (NumRows > count)
		   PrtMes("chenj@ncbi.nlm.nih.gov",
			"MMDBSRV", "Dart_Related(): NumRows > MaxPssm",
			NULL, FALSE);

		for (j=0; j< NumRows; j++) {
		    for (k=i+1; k< CdNum; k++)
			if (PssmId[k] == Pssmlist[j]) {
			    if (ismap == FALSE) iColor[k] = iColor[i];
			    iClus[k] = i;
			}
		}
	    }
	}

	sprintf(querynm, "%s%s(query)", PDBid, Chain);
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
	    x1 = MAX(x, x1);
	    x2 = MIN(x2, right);
	    y1= GetY1(head, &end, from, to, seqlen, iClus[i], 7);

	    if (y1 < 0) continue;
	    y2 = y1+FontBH+4;

	    Dart_Acc2Info(dartcon, CddName[i], shortname, MAX_TBUFF, defline,
			  MAX_TBUFF, definition, MAX_TBUFF);
	    
	    if (ismap == TRUE) {
		if (StrLen(defline) == 254) 
			strcpy((defline+StrLen(defline)-3), "...");
		if (defline[StrLen(defline)-1] != '.') StringCat(defline, ".");

		chtmp = strchr(defline, ';');
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
		    else StringCat(def2, ".");
		}
		sprintf(str, "%s:%s Click for the CD alignment.", CddName[i], def2);
		sprintf(def2, str);

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
			x1, y1, x2, y2);
		fprintf(File, CDDsrv, CDDurl, CddName[i], str, gi, querynm,Version);
		fprintf(File, "alt=\"%s: %s %s\" ", CddName[i], defline,
                        "Click to see CD alignment");
		fprintf(File, "ONMOUSEOVER=\"self.status=\'%s\'; return true\">\n",
			def2);
	
        	StringCat(cTmp, "+OR+");
        	StringCat(cTmp, CddName[i]);
        	StringCat(cTmp, "[ACCN]"); 
	    }
	    else {    /* ismap == FALSE */
		Int4 r, g, b, color;

		r = iDartCol[iColor[i]][0];
		g = iDartCol[iColor[i]][1];
		b = iDartCol[iColor[i]][2];
		gdImageRoundRectangle(im, x1, y1, x2, y2, 8, 5,
		     gdImageColorAllocate(im, r, g, b), 1);

		len1 = StrLen(shortname)*FontBW;
		len2 = StrLen(shortname)*FontMW;
		color = white;
		if (iColor[i] == 2 || iColor[i] == 4 || iColor[i] == 0) color=black;
		if ((x2-x1-4)> len1) 
		    gdImageString(im, gdFont7X13b, (x1+x2-len1)/2, y1+2,
				  shortname, color);
		else if ((x2-x1-4) > len2) 
		    gdImageString(im, gdFont6X12, (x1+x2-len2)/2, y1+2,
				  shortname, color);
		else {
		    Int4  char_num;

		    char_num = (x2-x1-4)/FontBW;
		    if (char_num >= 3) strcpy(&(shortname[char_num-3]), "...");
		    else switch (char_num) {
		    	case 1: strcpy(shortname, "."); break;
		    	case 2: strcpy(shortname, "..");
		    }
		    gdImageString(im,gdFont7X13b,x1+4, y1+2, shortname, color);
		}
	    }

	    if (!dy) {
		dy = y0-y1;
		y0 = y1;
	    }
	}	/* for */

	y0 = y2;

	if (!dy) dy=6;
	else dy = 15;

	x1 = 10;
	x2 = x1+ StrLen("CDs") * FontBW;
	y1 = y+dy;
	y2 = y1+FontBH;
	if (ismap == FALSE) {
	    gdImageLine(im, x1, y2, x2, y2, blue);
	    gdImageString(im, gdFont7X13b, x1, y1, "CDs", blue);
	}
	else {

	    sprintf(str, "Click to see Entrez Conserved Domains.");
	    fprintf(File, "<area shape=rect coords=%d,%d,%d,%d ", x1, y1, x2, y2);
	/*   x, head->y+dy, x+StrLen("CDs")*FontBW, head->y+dy+FontBH);*/
	    fprintf(File, "href=\"%s?%s%s\" alt=\"%s\" ",
		    ENTREZurl, CDDOpt, cTmp+4, str);
	    fprintf(File,"ONMOUSEOVER=\"self.status=\'%s\'; return true\">\n",str);

	}
	    y2 = y0;
   
    	FreeOverLoc(head);
    	MemFree(iColor);
    	MemFree(PssmId);
    	MemFree(dsp);
    	for (i=0; i< CdNum; i++) MemFree(CddName[i]);
    	MemFree(CddName);
    	MemFree(iClus);
    	MemFree(Pssmlist);
    }
    else y2 = y;

    return(y2);

}  /* end of ConsDomMapOrImg */




Int4 MaxSeqLenProOrDRna(PMSD pmsd, Int2 protein)
{
        Int4    maxseqlen = 0;
        PMMD    pmmd;
        PDNMM   pdnmm;

        maxseqlen = 0;
        for (pdnmm = pmsd->pdnmmHead; pdnmm != NULL; pdnmm = pdnmm->next)
        {
                pmmd = (PMMD) pdnmm->data.ptrvalue;
                if ((protein && (pmmd->bWhat & AM_PROT)) 
			|| (!protein && (pmmd->bWhat & (AM_DNA | AM_RNA)))) {
                        if (maxseqlen < pmmd->iResCount)
                                maxseqlen = pmmd->iResCount;
                }
        }

        return(maxseqlen);

}       /* end MaxSeqLen */




PDNMM PdnmmforChainX(PDNMM pdnmm, Int2 chainx)
{
        Int2    i = 1;
        PDNMM   pdnmmtmp;
        PMMD    pmmd;

        pdnmmtmp = pdnmm;

        while (i <= chainx)
        {
                pmmd = (PMMD)(pdnmmtmp->data.ptrvalue);
                if ((pmmd->bWhat & (AM_PROT |AM_DNA | AM_RNA)) && pdnmm)
                        i++;
                if (i <= chainx) pdnmmtmp = pdnmmtmp->next;
        }

        return(pdnmmtmp);

}       /* end PdnmmforChainX */




void
GetDomFeaPtr(BiostrucPtr bsp, Boolean *hasDomain, BiostrucFeaturePtr *domain_bfp)
{
        BiostrucFeatureSetPtr bfsp = NULL;
        ValNodePtr      vnp = NULL;

        *domain_bfp = NULL;
        *hasDomain = FALSE;

        for (bfsp = bsp->features; bfsp != NULL; bfsp = bfsp->next)
        {
             for (vnp = bfsp->descr; vnp != NULL; vnp = vnp->next)
                  if (vnp->choice == BiostrucFeatureSetDescr_name &&
                        !StringCmp(vnp->data.ptrvalue, "NCBI Domains"))
                  {
                        *hasDomain = TRUE;
                        *domain_bfp = bfsp->features;
                        break;
                  }
        }


}	/* end of GetDomFeaPtr */




Int2 ChainCount(PMSD pmsd)
{
        Int2            cnt;
        PMMD            pmmd;
        PDNMM           pdnmm;

        for (pdnmm = pmsd->pdnmmHead, cnt=0; pdnmm != NULL; pdnmm =pdnmm->next)        {
                pmmd = (PMMD) pdnmm->data.ptrvalue;
                if ((pmmd->bWhat) & (AM_PROT |AM_DNA | AM_RNA))
                        cnt++;
        }

        return(cnt);

}       /* end ChainCount */



void GroupingChains(PMSD pmsd)
{

        Int2    i, cnt;
        PDNMM   prohead, proend, nuhead, nuend, pdnmm, pdnmmnext;
        PDNMM   elsehead, elseend;
        PMMD    pmmd;


        prohead = proend = nuhead = nuend = elsehead = elseend = NULL;
        pdnmm = pmsd->pdnmmHead;

        cnt = ChainCount(pmsd);

        for (i=0, pdnmm = pmsd->pdnmmHead; i< cnt; i++)
        {
                pmmd = (PMMD) pdnmm->data.ptrvalue;
                pdnmmnext = pdnmm->next;

                if (pmmd->bWhat & AM_PROT)
                {
                    if (!prohead) prohead = proend = pdnmm; 
		    else {
                        proend->next = pdnmm;
                        proend = pdnmm;
                    }
                }
                else if (pmmd->bWhat & (AM_DNA | AM_RNA)) {
                        if (!nuhead) nuhead = nuend = pdnmm;
                        else {
                                nuend->next = pdnmm;
                                nuend = pdnmm;
                        }
                }
                else i--;

                pdnmm->next = NULL;
                pdnmm = pdnmmnext;
        }


        if (!pdnmm)
                if (!elsehead) elsehead = pdnmm;
                else elseend->next = pdnmm;
        elsehead = elseend = NULL;  /* remove non_PROT,non_DNA and non_RNA */
	/* ??? */

        if (!prohead) {
                if (!nuhead) pmsd->pdnmmHead = elsehead;
                else {
                        pmsd->pdnmmHead = nuhead;
                        nuend->next = elsehead;
                }
        }
        else {
                pmsd->pdnmmHead = prohead;
                if (!nuhead) proend->next = elsehead;
                else {
                        proend->next = nuhead;
                        nuend->next = elsehead;
                }
        }

        ((PDNMM)pmsd->pdnmmHead)->last = NULL;

}       /* end GroupingChains */



void CalDomIdx(PDNMM pdnmm, Int4Ptr *DomIdx)
{
	int	cnt, chnNo, maxchnNo=0;
	PMMD	pmmd;
	PDNMM	tmp_pdnmm;

        for (tmp_pdnmm = pdnmm; tmp_pdnmm != NULL; tmp_pdnmm = tmp_pdnmm->next)
        {
              pmmd = tmp_pdnmm->data.ptrvalue;
              if (pmmd->bWhat & (AM_PROT | AM_DNA |AM_RNA)) {
                    chnNo = pmmd->iChainId;
                    if (maxchnNo < chnNo) maxchnNo = chnNo;
              }
        }

        *DomIdx = (Int4Ptr) MemNew (++maxchnNo * sizeof(Int4));

        for (tmp_pdnmm = pdnmm, cnt=0; tmp_pdnmm != NULL;
                                        tmp_pdnmm = tmp_pdnmm->next, cnt++)
        {
             pmmd = tmp_pdnmm->data.ptrvalue;
             if (pmmd->bWhat & (AM_PROT | AM_DNA |AM_RNA))
                 (*DomIdx)[pmmd->iChainId] = cnt;
        }

}	/* CalDomIdx */


void 
CheckDomColIdx(BiostrucFeaturePtr bfp, PDNMM pdnmm, IntervalHead **DomHead,
	Int4Ptr DomIdx, Int4Ptr ColorIdx,  Boolean hasDomain, Boolean forImg)
{
	BiostrucFeaturePtr	tmp_bfp;
	Int2			colidx = 0, thisdomain;
	Int4			chnNo;
	IntervalHead		*tmp_head;
	PDNMM			tmp_pdnmm;
	PMMD			pmmd;
	ResidueIntervalPntrPtr	ripp;
	ValNodePtr		vnp, vnp1, vnp2;
	
	if (hasDomain) {
	   for (tmp_bfp = bfp; tmp_bfp != NULL; tmp_bfp = tmp_bfp->next)
	   {
		Int4 idx;

                vnp = tmp_bfp->Location_location;
                vnp1 = (ValNodePtr) vnp->data.ptrvalue;
                vnp2 = (ValNodePtr) vnp1->data.ptrvalue;

                ripp = (ResidueIntervalPntrPtr) vnp2->data.ptrvalue;
		chnNo = ripp->molecule_id;

		colidx %= ncycle;
		idx = DomIdx[chnNo];
		 if (DomHead[idx] == NULL)  { 
		     thisdomain = 1;
		     if (forImg == TRUE) ColorIdx[idx] = -11; /*just a label */
		}
		else thisdomain++;

		tmp_head = (IntervalHead *) MemNew (sizeof(IntervalHead));
		tmp_head->colidx = colidx;
		tmp_head->ripp = ripp;
		tmp_head->next = DomHead[idx];
		tmp_head->thisdomain = thisdomain;
		tmp_head->domid = tmp_bfp->id;
		DomHead[idx] = tmp_head;

		colidx++;
	   }	/* tmp_bfp */
	}	/* if (hasDomain) */

	for (tmp_pdnmm = pdnmm; tmp_pdnmm != NULL; tmp_pdnmm = tmp_pdnmm->next)
	{
	     Int4 idx;

	     pmmd = tmp_pdnmm->data.ptrvalue;
	     if (pmmd->bWhat & (AM_PROT | AM_DNA |AM_RNA)) {

		chnNo = pmmd->iChainId; 
		idx = DomIdx[chnNo];
		if (DomHead[idx] == NULL) { 
			colidx %= ncycle; 
			if (forImg==TRUE) ColorIdx[idx] = colidx;
			colidx++;
		}
	     }

	}	/* for (tmp_pdnmm) */

/*	MemFree(DomIdx); */

}	/* CheckDomColIdx */
