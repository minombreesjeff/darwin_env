/*
* $Id: mmdbuti.h,v 1.3 2003/01/15 16:15:09 chenj Exp $
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
* $Log: mmdbuti.h,v $
* Revision 1.3  2003/01/15 16:15:09  chenj
* change the definition of ChainScaleMapOrImg()
*
* Revision 1.2  2003/01/14 19:57:17  chenj
* Minor changes
*
* Revision 1.1.1.1  2002/12/04 21:12:08  chenj
* Imported sources
*
*
*
*===========================================================================
*/



#include <ncbi.h>
#include <mmdbdata.h>
#include <gifgen.h>
#include "/net/scarecrow/str3/web/dev/htdocs/Structure/chenj/bin/utilisCJ.h"
#include "/netopt/structure/include/dart.h"
#include "/netopt/structure/include/dartutil.h"
#include "/netopt/structure/include/dbinfo.h"
#include "/net/scarecrow/export/home/he/proj/MmdbSrv/PubStructApi.h"

#define MAX_TBUFF    8192
#define ChainPerImg  10

#define MaxSeqImgSize   700
#define GraphWidth      800

#define ncycle 	10


typedef struct  domainhead {
        Int2    colidx, thisdomain;
	Int4	domid;
        ResidueIntervalPntrPtr  ripp;
        struct  domainhead PNTR next;
} IntervalHead;


Int2 ChainNameMapOrImg(Boolean ismap, gdImagePtr im, Int2 x, Int2 y, 
	PMSD pmsd,  PMMD pmmd, Int2 protein, FILE *File);


void ChainScaleMapOrImg(Boolean ismap, gdImagePtr im, Int2 x, Int2 y, 
PMMD pmmd, Int4 uid, Int4 sdid, Int4 color, Int4 labelcol, FILE *File);


Int2 ChainDomMapOrImg(Boolean ismap, gdImagePtr im, Int2 x, Int2 y, Int4 uid, 
     PMMD pmmd, Int4Ptr DomIdx, IntervalHead **DomHead, FILE *File);

Int4 MaxSeqLenProOrDRna(PMSD pmsd, Int2 protein);

PDNMM PdnmmforChainX(PDNMM pdnmm, Int2 chainx);

void GetDomFeaPtr(BiostrucPtr bsp, Boolean *hasDomain, BiostrucFeaturePtr *domain_bfp);

Int2 ChainCount(PMSD pmsd);

void GroupingChains(PMSD pmsd);

void CalDomIdx(PDNMM pdnmm, Int4Ptr *DomIdx);

void CheckDomColIdx(BiostrucFeaturePtr bfp, PDNMM pdnmm, IntervalHead **DomHead,
	Int4Ptr DomIdx, Int4Ptr ColorIdx,  Boolean hasDomain, Boolean Img);
