/*
* $Id: mmdbgraph.c,v 1.2 2003/01/15 16:09:59 chenj Exp $
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
* $Log: mmdbgraph.c,v $
* Revision 1.2  2003/01/15 16:09:59  chenj
* add uid as argument in ChainScaleMapOrImg()
*
* Revision 1.1.1.1  2002/12/04 21:12:08  chenj
* Imported sources
*
*
*============================================================================
*/


#include <ncbi.h>
#include <mmdbapi.h> 
#include "mmdbuti.h"

Int4 			*ColorIdx, white, black, blue, red, gray;

extern	Boolean		psok;
Int4			maxseqlen;
extern  Char 		MAILTO[PATH_MAX], Database[PATH_MAX];
double  pix_per_res;


/* infered from gui_ctools/cn3d/cn3d_colors.cpp */

Uint1 cycle1[10][3] = 
{
	255,   0, 255, 		/* magenta 	0 */
	  0,   0, 255,		/* blue		1 */
	139,  87,  66, 		/* brown	2 */
	  0, 255, 127, 		/* lightgreen	3 */
	179, 179, 179, 		/* lightgray	4 */
	255, 165,   0, 		/* gold		5 */
	255, 114,  86, 		/* pink		6 */
	  0, 255,   0, 		/* green 	7 */
	  0, 255, 255, 		/* cyan		8 */
	255, 236, 139, 		/* yellowtint	9 */
};



Int4 ModelMapOrImg(Boolean ismap, gdImagePtr im, Int4 uid, PMSD pmsd, 
   Int4 chain1, Int4 chain2, IntervalHead **DomHead, Int4Ptr DomIdx, FILE *File)
{
    PDNMM	pdnmm, pdnmm1;
    PMMD 	pmmd;
    Int4 	i,x, y, colidx, r, g, b; 
    
    pdnmm = (PDNMM)pmsd->pdnmmHead;
    pdnmm1 = PdnmmforChainX(pdnmm, chain1);
    pdnmm = pdnmm1;

/* Protein Chains */

    maxseqlen = MaxSeqLenProOrDRna(pmsd, 1);
    pix_per_res = (float)MaxSeqImgSize/(float)(maxseqlen-1);

    if (maxseqlen) {
	for (i = chain1; i <= chain2; i++, pdnmm = pdnmm->next) {
   
	    pmmd = pdnmm->data.ptrvalue;
	    
	    if ((pmmd->bWhat) & (AM_DNA || AM_RNA)) break;
	    if ((pmmd->bWhat) & AM_PROT) {

        /* Name */
		if (i == 1 || i==chain1) y = 50;
		else y += 60;
		x =ChainNameMapOrImg(ismap, im, 0, y, pmsd, pmmd,1,File);

	/* Chain & Domains */

		x += 30; 
		y =ChainDomMapOrImg(ismap,im,x,y,uid,pmmd,DomIdx,DomHead,File);

	/* Conservative Domain Image */

		y = ConsDomMapOrImg(ismap, im, x, y, pmsd, pmmd, File);
		
	    }  /* AM_PROT */
	    
	}    /* end for (i=chain1 */

    } /* end Protein Chains */


/* Nucleotide Chains */
    maxseqlen = MaxSeqLenProOrDRna(pmsd, 0);
    pix_per_res = (float)MaxSeqImgSize/(float)(maxseqlen-1);

    if (maxseqlen) {
	for (i = chain1, pdnmm = pdnmm1; i <= chain2; i++, pdnmm =pdnmm->next){
           
	    pmmd = pdnmm->data.ptrvalue;

	    if ((pmmd->bWhat) & (AM_DNA | AM_RNA)) {
                
		Int4  labelcol;

	/* Name */
		if (i==1 || i == chain1) y = 50;
		else y += 80;
		x = ChainNameMapOrImg(ismap, im, 0, y, pmsd, pmmd, 0, File);

	/* Chain Image */

		if (ismap == FALSE) {
		    x += 9; 
		    colidx = ColorIdx[DomIdx[pmmd->iChainId]];
		    r = cycle1[colidx][0];
		    g = cycle1[colidx][1];
		    b = cycle1[colidx][2];
		    labelcol = white;
		    if (colidx==3 || colidx==4 || colidx==5 || colidx==7
		        || colidx==8 || colidx==9)
		        labelcol = black;
		    ChainScaleMapOrImg(ismap, im, x, y, pmmd, 0, 0, 
		      gdImageColorAllocate(im, r, g, b), labelcol, File);
		}
		
	    } /* AM_DNA | AM_RNA */

	} /* end nucleotide Chains */

    }  /* end maxseqlen */

    return(y);

}	/* end of ModelMapOrImg */



Int4
DrawImg(Int4Ptr Uid, Int2 chain1, Int2 chain2, Int2 imgsize)
{
	Boolean		hasDomain = FALSE;
	BiostrucFeaturePtr	domain_bfp;
	BiostrucPtr	bsp=NULL;
	gdImagePtr      im;
	Int4		i, uid, cnt, *DomIdx;
	IntervalHead	*tmp_head;
	PDNMS		ModelStruc=NULL;
	PDNMM		pdnmm, pdnmm1;
	PMSD		pmsd;
	IntervalHead	**DomHead;
	
	uid = *Uid;
 
	bsp = openBSP(uid, 3, 0, TRUE, FALSE, FALSE, &psok, Database);
 	if (!bsp) {	
	   Char str[10];

	   sprintf(str, "%d", uid);
	   PrtMes(NULL, "MMDBSRV (MMDBGraph)",
	      "No such structure available in MMDB: uid = ", str, TRUE);
    	   return 0;
	}
 
	ModelStruc = MakeAModelstruc(bsp);
	pmsd = (PMSD) ModelStruc->data.ptrvalue;
        pdnmm = pmsd->pdnmmHead;
	cnt = ChainCount(pmsd);
	if (!cnt) return 0;

	ColorIdx = (Int4Ptr) MemNew (cnt * sizeof(Int4));
	DomHead = (IntervalHead **) MemNew (cnt * sizeof(IntervalHead *));	
	for (i=0; i< cnt; i++) {
		ColorIdx[i] = -1;
		DomHead[i] = NULL;
	}
	CalDomIdx(pdnmm, &DomIdx);

	GetDomFeaPtr(bsp, &hasDomain, &domain_bfp);	
	CheckDomColIdx(domain_bfp, pdnmm, DomHead, DomIdx, ColorIdx, hasDomain, TRUE);

        GroupingChains(pmsd);

        im = gdImageCreate(GraphWidth, imgsize);
        white = gdImageColorAllocate(im, 255, 255, 255);
        black = gdImageColorAllocate(im,   0,   0,   0);
	blue =  gdImageColorAllocate(im,   0,   0, 225);
	red  =  gdImageColorAllocate(im, 255,   0,   0);
	gray = gdImageColorAllocate(im, (Uint1)102, (Uint1)102, (Uint1)102);

	printf("Content-type: image/gif\n\n");
	ModelMapOrImg(FALSE, im, uid,pmsd, chain1, chain2,DomHead,DomIdx,NULL);
        gdImageGif(im, stdout);
        gdImageDestroy(im);

	free(ColorIdx);
	for (i = 0; i< cnt; i++)
	  if (DomHead[i] != NULL)  {
		for (tmp_head = DomHead[i]; tmp_head != NULL; 
					tmp_head=tmp_head->next)
			free(tmp_head);
		free(DomHead[i]);
	  }
	free(DomHead);
	MemFree(DomIdx);
	return 0;
	
}	/* end DrawImg */



