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
* File Name:  cblastgraph.h
*
* Author:  Yanli Wang
*
* Initial Version Creation Date: 6/25/2002
*
* File Description:
*         header file for the CBLAST server CGI-bin
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cblastgraph.h,v $
* Revision 1.5  2002/12/12 15:59:45  ywang
* wording improvement
*
*
*
* ==========================================================================
*/
#include <gifgen.h>
#include "/netopt/structure/include/dart.h"
#include "/netopt/structure/include/dartutil.h"

#define MAX_TBUFF    8192

#define FontBH   	gdFont7X13b->h
#define FontBW   	gdFont7X13b->w
#define FontMH		gdFont6X12->h
#define FontMW		gdFont6X12->w
#define	FontSH		gdFont5X8->h
#define	FontSW		gdFont5X8->w

/*#define ColorKeyY  (20 + 3.5*FontBH)  */
#define ColorKeyY  0 
#define MapStartY  (20 + ColorKeyY)
#define ConsDomStartY  (MapStartY + FontBH + 2) 
#define LabelStart 10
#define LabelSpace (10+12*FontBW)
    /* 1ABC C 1234 */
#define MapRowSpace 1.5*FontBH 
#define CdMapRowSpace 0.3*FontBH 
#define MapRowHeight 0.5*FontBH 

#define MaxSeqImgSize 	600
#define ScoreSpace 120 
#define ScoreLabelStart (MaxSeqImgSize + LabelSpace + 30)
#define EvalueLabelStart (ScoreLabelStart + 50)
#define GraphWidth    (MaxSeqImgSize + LabelSpace + 10 + ScoreSpace)  	

typedef struct overloc { Int2 *x_loc, y;
                         struct overloc *next;
                       } OverLoc, PNTR OverLocPtr;

typedef struct overloc2 {
Int2 x, y;
Int2 index;
} OverLoc2, PNTR OverLoc2Ptr;
