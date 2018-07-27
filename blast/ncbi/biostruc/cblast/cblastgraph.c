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
* File Name:  cblastgraph.c
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
* $Log: cblastgraph.c,v $
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

#include <ncbi.h>
#include <ncbistr.h>
#include <mmdbapi.h>
#include <mmdbdata.h>

#include <string.h>
#include <assert.h>
#include <unistd.h>

#include "cblastgraph.h"
#include "cblast.h"

#define ENTREZurl "http://www.ncbi.nlm.nih.gov/entrez//query.fcgi?"

extern  CharPtr queryrid;
extern  Dart_Connect	*dartcon;
extern	Int4		maxseqlen;
extern  char CGIurl[PATH_MAX];
extern  char CGIName[PATH_MAX];
extern  char MMDBcgi[PATH_MAX];
extern  char CDDhome[PATH_MAX];
extern  char CDSRVurl[PATH_MAX];
Int2    x_point, y_point;

/* infered from gui_ctools/cn3d/cn3d_colors.cpp */

Uint1 cycle1[10][3] =
{
    255,   0, 255,      /* magenta  0 */
      0,   0, 255,      /* blue     1 */
    139,  87,  66,      /* brown    2 */
      0, 255, 127,      /* lightgreen   3 */
    179, 179, 179,      /* lightgray    4 */
    255, 165,   0,      /* gold     5 */
    255, 114,  86,      /* pink     6 */
      0, 255,   0,      /* green    7 */
      0, 255, 255,      /* cyan     8 */
    255, 236, 139,      /* yellowtint   9 */
};

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
/*------------------------*/
OverLocPtr NewOverLoc()
{
    OverLocPtr tmp = NULL;
    Int2    j;

    tmp = (OverLocPtr) MemNew (sizeof(OverLoc));
    tmp->x_loc = (Int2 *) MemNew (maxseqlen*sizeof(Int2));
    for (j=0; j< maxseqlen; j++) tmp->x_loc[j] = -1;
    tmp->next = NULL;

    return(tmp);
}

/*-------------------*/
Boolean FreeOverLoc(OverLoc *head)
{
        OverLoc *tmp;

        for (tmp = head; tmp != NULL; tmp = tmp->next) {
                MemFree(tmp->x_loc);
        }

        return(TRUE);

}       /* FreeOverLoc */
/*-----------------------*/
Int2 GetY(OverLoc *head, OverLoc **end, Int2 from, Int2 to, Int2 iClus)
{
    OverLoc     *overtmp;
    Int2        overflag = 1, j;

    overtmp = head;
    while (overtmp != NULL && overflag) {
        overflag = 0;
        for (j=from; j< to; j++)
             if (overtmp->x_loc[j] >= 0) {
                        overflag = 1;
            break;
             }
        if (overflag) {
            for (j=from; j< to; j++)
            if (overtmp->x_loc[j] == iClus) return -1;

            overtmp = overtmp->next;
        }
    }
    if (overtmp == NULL) {
            overtmp = NewOverLoc();
        (*end)->next = overtmp;
        overtmp->y = (*end)->y + FontBH + 7;

        *end = overtmp;
        for (j=from; j< to; j++) overtmp->x_loc[j] = iClus;
        return((*end)->y);
    }
        else  {
        for (j=from; j< to; j++) overtmp->x_loc[j] = iClus;
        return(overtmp->y);
    }

}   /* GetY */
/*-----------------------------*/
void GetOverlap2(SeqAnnotPtr sap, Int4 from, Int4 to, OverLoc2Ptr olp)
{
  SeqAlignPtr salp = NULL;
  DenseSegPtr dsp = NULL;
  Int4 from2 = 0, to2 = 0;
  Int4Ptr starts, lens;
  Int2 numseg = 0, i = 0;
  Int4 xtmp = 0, ytmp = 0;

  olp->x = 0;
  olp->y = 0;
  olp->index = -1;
  salp = sap->data;
    /* so far, only consider the first cd link */
while(salp){
  dsp = salp->segs;
  numseg = dsp->numseg;
  starts = dsp->starts; 
  lens = dsp->lens;

  from2 = starts[0]; to2 = starts[(numseg -1)*2] + lens[numseg - 1]; 
  
  if(from < from2) {
    if(to < from2) return;
    else if(to < to2) {
      xtmp = from2; ytmp = to;
      if((ytmp-xtmp) > (olp->y-olp->x)){
        olp->x = xtmp; olp->y = ytmp; olp->index = i;
      }
    }
    else {
      xtmp = from2; ytmp = to2;
      if((ytmp-xtmp) > (olp->y-olp->x)){
        olp->x = xtmp; olp->y = ytmp; olp->index = i;
      }
    }
  }
  else {
    if(from > to2) return;
    else if(to < to2) {
      xtmp = from; ytmp = to;
      if((ytmp-xtmp) > (olp->y-olp->x)){
        olp->x = xtmp; olp->y = ytmp; olp->index = i;
      }
    }
    else {
      xtmp = from; ytmp = to2;
      if((ytmp-xtmp) > (olp->y-olp->x)){
        olp->x = xtmp; olp->y = ytmp; olp->index = i;
      }
    }
  }
  salp = salp->next;
  i++;
}

  return;
}
/*-----------------------------*/
void GetOverlap(SeqAlignPtr salp, Int4 from, Int4 to, OverLoc2Ptr olp)
{
  DenseSegPtr dsp = NULL;
  Int4 from2 = 0, to2 = 0;
  Int4Ptr starts, lens;
  Int2 numseg = 0;

  olp->x = 0;
  olp->y = 0;
    /* so far, only consider the first cd link */
  dsp = salp->segs;
  numseg = dsp->numseg;
  starts = dsp->starts;
  lens = dsp->lens;

  from2 = starts[0]; to2 = starts[(numseg -1)*2] + lens[numseg - 1];
 
  if(from < from2) {
    if(to < from2) return;
    else if(to < to2) {
      olp->x = from2; olp->y = to;
    }
    else {
      olp->x = from2; olp->y = to2;
    }
  }
  else {
    if(from > to2) return;
    else if(to < to2) {
      olp->x = from; olp->y = to;
    }
    else {
      olp->x = from; olp->y = to2;
    }
  }

  return;
}
/*------*/
static void ChainScaleImg(gdImagePtr im, Int4 seqlen,
    CharPtr label, Int4 color, Int4 labelcol)
{
    Char    cTmp[MAX_TBUFF], fontsize[100];
    Int2    x, y, x1, y1, x2, y2, left, right, i, x0, len;
    Int4    red, tick, ntick, dont;
    double  lenfrac;
    static Int4   ticksteps[14] ={5, 10, 20, 25, 50, 100, 200, 150, 500,
                     1000, 2000, 2500, 5000, 10000};

    red  = gdImageColorAllocate(im, 255, 0, 0);

    x = LabelSpace; y = MapStartY;

    left = x;
    lenfrac = (float)(seqlen-1)/(float)(maxseqlen-1);
    right= (Int4)(x + lenfrac*(float)MaxSeqImgSize);
    x1 = left; y1 = y; x2 = right; y2 = y+ FontBH+2;
    gdImageFilledRectangle(im, x1, y1, x2, y2, color);
    strcpy(cTmp, label);
    if ((right - left) >= StrLen(cTmp)*FontBW) {
        x1 = (left + right - StrLen(cTmp)*FontBW)/2;
        gdImageString(im, gdFont7X13b, x1, y1+1, cTmp, labelcol);
    }
    else if ((right-left) >= StrLen(cTmp)*FontMW) {
        x1 = (left + right - StrLen(cTmp)*FontMW)/2;
        gdImageString(im, gdFont6X12, x1, y1+1, cTmp, labelcol);
    }
    else if ((right-left) >= StrLen(label)*FontBW) {
        x1 = (left + right - StrLen(label)*FontBW)/2;
        gdImageString(im, gdFont7X13b, x1, y1+1, label, labelcol);
    }
    else if ((right-left) >= StrLen(label)*FontMW) {
        x1 = (left + right - StrLen(label)*FontMW)/2;
        gdImageString(im, gdFont6X12, x1, y1+1, label, labelcol);
    }

    x1 = left;

/*fprintf(stderr,"ChainScale x1 y1 x2 y2 %d %d %d %d\n", x1, y1, x2, y2); */
    gdImageLine(im, x1, y1, x1, y1-5, red);    /* for "1"  */
    gdImageLine(im, x2, y1, x2, y1-5, red);    /* for "len" */

    y1 = y1 - 5 - FontMH;
    gdImageString(im, gdFont6X12, x1, y1, "1", red);   /* "1" */
    x0 = x1+FontMW;
    cTmp[0] = '\0';
    sprintf(cTmp, "%d", seqlen);
    x2 -= (StrLen(cTmp)-1)*FontMW;
    if (x2 > left)
       gdImageString(im, gdFont6X12, x2, y1, (char *) cTmp, red);/* "len" */

    if (lenfrac < 0.05) return;

    dont = x2 - FontMW;


    for (i=0; i< 14; i++) {
        ntick = seqlen/ticksteps[i];
        if (ntick < 10)  {
            tick = ticksteps[i];
            break;
        }
    }

    for (i=1; i<= ntick; i++) {
         x1 = (Int4)(x +
        ((float)(i*tick)-1.0)*(float)(right-left)/(float)(seqlen-1));
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


}   /* end PrintChainScale */
/*------*/
static void ChainScaleImgA(gdImagePtr im, Int4 seqlen)
{
    Char    cTmp[MAX_TBUFF], fontsize[100];
    Int2    x, y, x1, y1, x2, y2, left, right, i, j, x0, len, x1_1, y1_1, y2_1;
    Int4    red, black, tick, ntick, dont;
    double  lenfrac;
    static Int4   ticksteps[14] ={5, 10, 20, 25, 50, 100, 200, 150, 500,
                     1000, 2000, 2500, 5000, 10000};
   
    red  = gdImageColorAllocate(im, 255, 0, 0);
    black   = gdImageColorAllocate(im,   0,   0,   0);

    x = LabelSpace; y = ColorKeyY;
    left = x;

    lenfrac = (float)(seqlen-1)/(float)(maxseqlen-1);
    right= (Int4)(left + lenfrac*(float)MaxSeqImgSize);
    x1 = left; y1 = y; x2 = right; y2 = y+ 0.4*FontBH+2;
    gdImageFilledRectangle(im, x1, y1, x2, y2, red); 

    x1 = left;

    for (i=0; i< 14; i++) {
        ntick = seqlen/ticksteps[i];
        if (ntick < 10)  {
            tick = ticksteps[i];
            break;
        }
    }

    gdImageLine(im, x1, y2, x1, y2+8, black);    /* for "1"  */
    y1_1 = y2; y2_1 = y1_1 + 4;
    for(j=1; j<5; j++){
      x1_1 = (Int4)(x1 + (float)j*tick/5*(float)(right-left)/(float)(seqlen-1));
      gdImageLine(im, x1_1, y1_1, x1_1, y2_1, black); 
    }
    gdImageLine(im, x2, y2, x2, y2+8, black);    /* for "len" */ 
    y = y2;

    y1 = y + 5;
    gdImageString(im, gdFont6X12, x1, y1, "1", black);   /* "1" */

    x0 = x1+FontMW;
    cTmp[0] = '\0';
    sprintf(cTmp, "%d", seqlen);
    x2 -= (StrLen(cTmp)-1)*FontMW;
/*  if (x2 > left) gdImageString(im, gdFont6X12, x2, y1, (char *) cTmp, red); */

    if (lenfrac < 0.05) return;

    dont = x2 - FontMW;

    for (i=1; i<= ntick; i++) {
         x1 = (Int4)(x +
        ((float)(i*tick)-1.0)*(float)(right-left)/(float)(seqlen-1));
         if (x1 > right-FontMW)  continue;
         y1 = y; y2 = y1+8;
         gdImageLine(im, x1, y1, x1, y2, black);
         y1_1 = y; y2_1 = y1_1 + 4;
         if(i < ntick){
           for(j=1; j<5; j++){
             x1_1 = (Int4)(x1 + (float)j*tick/5*(float)(right-left)/(float)(seqlen-1));
             gdImageLine(im, x1_1, y1_1, x1_1, y2_1, black); 
           }
         }
         cTmp[0] = '\0';
         sprintf(cTmp, "%d", i*tick);
         len = StrLen(cTmp) * FontMW/2;
         x2  = x1 + len;
         x1 -= len;
         y1 =  y2;
         if (x2 < dont && ((x2-x0) > len)) {
        gdImageString(im, gdFont6X12, x1, y1, cTmp, black);
        x0 = x2+FontMW;
        }
    }


}   /* end PrintChainScale */
/*----------------------*/
static void ChainScaleImgB(gdImagePtr im, Int4 seqlen)
{
    Char    cTmp[MAX_TBUFF], fontsize[100];
    Int2    x, y, x1, y1, x2, y2, left, right, i, j, x0, len, x1_1, y1_1, y2_1;
    Int4    red, black, tick, ntick, dont;
    double  lenfrac;
    static Int4   ticksteps[14] ={5, 10, 20, 25, 50, 100, 200, 150, 500,
                     1000, 2000, 2500, 5000, 10000};

    red  = gdImageColorAllocate(im, 255, 0, 0);
    black   = gdImageColorAllocate(im,   0,   0,   0);

    x = LabelSpace; y = MapStartY + 5;
    left = x;

    lenfrac = (float)(seqlen-1)/(float)(maxseqlen-1);
    right= (Int4)(left + lenfrac*(float)MaxSeqImgSize);
    x1 = left; y1 = y; x2 = right; y2 = y+ 0.4*FontBH+2;
    gdImageFilledRectangle(im, x1, y1, x2, y2, red);

    x1 = left;

    for (i=0; i< 14; i++) {
        ntick = seqlen/ticksteps[i];
        if (ntick < 10)  {
            tick = ticksteps[i];
            break;
        }
    }

    gdImageLine(im, x1, y1, x1, y1-8, black);    /* for "1"  */
    y1_1 = y1; y2_1 = y1_1 - 4;
    for(j=1; j<5; j++){
      x1_1 = (Int4)(x1 + (float)j*tick/5*(float)(right-left)/(float)(seqlen-1));
      gdImageLine(im, x1_1, y1_1, x1_1, y2_1, black);
    }
    gdImageLine(im, x2, y1, x2, y1-8, black);    /* for "len" */
    y = y1;

    y1 = y - 8 - FontMH;
    gdImageString(im, gdFont6X12, x1, y1, "1", black);   /* "1" */

    x0 = x1+FontMW;
    cTmp[0] = '\0';
    sprintf(cTmp, "%d", seqlen);
    x2 -= (StrLen(cTmp)-1)*FontMW;
/*  if (x2 > left) gdImageString(im, gdFont6X12, x2, y1, (char *) cTmp, red); */

    if (lenfrac < 0.05) return;

    dont = x2 - FontMW;

    for (i=1; i<= ntick; i++) {
         x1 = (Int4)(x +
        ((float)(i*tick)-1.0)*(float)(right-left)/(float)(seqlen-1));
         if (x1 > right-FontMW)  continue;
         y1 = y; y2 = y1-8;
         gdImageLine(im, x1, y1, x1, y2, black);
         y1_1 = y; y2_1 = y1_1 - 4;
         if(i < ntick){
           for(j=1; j<5; j++){
             x1_1 = (Int4)(x1 + (float)j*tick/5*(float)(right-left)/(float)(seqlen-1));             gdImageLine(im, x1_1, y1_1, x1_1, y2_1, black);
           }
         }
         cTmp[0] = '\0';
         sprintf(cTmp, "%d", i*tick);
         len = StrLen(cTmp) * FontMW/2;
         x2  = x1 + len;
         x1 -= len;
         y1 = y2 - FontMH;
         gdImageString(im, gdFont6X12, x1, y1, cTmp, black);
         x0 = x2+FontMW;
/*
         if (x2 < dont && ((x2-x0) > len)) {
           gdImageString(im, gdFont6X12, x1, y1, cTmp, black);
           x0 = x2+FontMW;
         }
*/
    }


}   /* end PrintChainScale */
/*----------------------*/
Int4 QueryChainImgAndMap(gdImagePtr im, BioseqPtr query_bsp, Boolean DrawMap, WWWQueryPtr wqp_head, FILE *File)
{
  Int2    x1, y1, x2, y2;
  Int4 blue, lightgreen, black;
  ValNodePtr descr = NULL;
  WWWQueryPtr wqp = NULL;
  ValNodePtr query = NULL;
  CharPtr str_query = NULL;

  blue  = gdImageColorAllocate(im, 0, 0, 255);
  black   = gdImageColorAllocate(im,   0,   0,   0);
  lightgreen = gdImageColorAllocate(im, 0, 255, 127);

  x1 = LabelStart ;
  x2 = x1 + StrLen("Query")*FontBW;
  y1 = MapStartY; y2 = y1+FontBH;

  gdImageLine(im, x1, y2, x2, y2, blue);
  gdImageString(im, gdFont7X13b, x1,y1, "Query", blue);
  if(DrawMap){
    if(query_bsp){
      descr = query_bsp->descr;
      while(descr){
        if(descr->choice == Seq_descr_title) {
          fprintf(File, "<area shape=rect coords=%d,%d,%d,%d href =\"%s%s?query_dump=TRUE", x1, y1, x2, y2, CGIurl, CGIName);
          str_query = StringSave("query_dump");
          ValNodeAddStr(&query, 0, str_query);
          print_query_args(File, wqp_head, query);
          ValNodeFreeData(query);
          fprintf(File, "\" ");

          fprintf(File, "alt=\"%s\" ONMOUSEOVER=\"self.status=\'%s'; return true\" >\n", descr->data.ptrvalue, descr->data.ptrvalue);
          break;
        }
        descr = descr->next;
      }
    }
  }

/*ChainScaleImg(im, query_bsp->length, "protein", lightgreen, black);*/
/*ChainScaleImgA(im, query_bsp->length); */
  ChainScaleImgB(im, query_bsp->length); 
  
  return y2;
}
/*----------------------*/
Int4 ConsDomImgAndMap(gdImagePtr im, BioseqPtr query_bsp, CdInfoPtr cip, Boolean DrawMap, FILE *File)
{
  ValNodePtr mapinfo = NULL, mapinfo_data = NULL;
  MapRowInfoPtr mrip = NULL;
  Char shortname[MAX_TBUFF];
  Int2 x, y_start = 0, y1 = 0, y2 = 0, x1=0, x2=0, x3=0, y3=0;
  float pix_per_res;
  Int2 len, len1, len2, right, char_num;
  Int4 black, white, blue;
  Int4 r, g, b, color;
  Int2 iColor;
  Boolean FirstCD = TRUE;

  ByteStorePtr bysp = NULL;
  CharPtr query_seq = NULL;

  if(DrawMap){
    bysp = query_bsp->seq_data;
    query_seq = BSMerge(bysp, NULL); 
  }

  white= gdImageColorAllocate(im, 255, 255, 255);
  black= gdImageColorAllocate(im,   0,   0,   0);
  blue = gdImageColorAllocate(im, 0,   0,   255);

  x = LabelSpace;
  y_start = ConsDomStartY;

  x1 = LabelStart; x2 = x1 + StrLen("CDs")*FontBW;
  y1 = y_start + CdMapRowSpace; y2 = y1+FontBH;
  gdImageLine(im, x1, y2, x2, y2, blue);
  gdImageString(im, gdFont7X13b, x1, y1, "CDs", blue);
  if(DrawMap){
    fprintf(File, "<area shape=rect coords=%d,%d,%d,%d href =\"%sdb=cdd&term=" , x1, y1, x2, y2, ENTREZurl);
    FirstCD = TRUE;
    mapinfo = cip->mapinfo;
    while(mapinfo){
      mapinfo_data = mapinfo->data.ptrvalue;
      while(mapinfo_data){
        mrip = mapinfo_data->data.ptrvalue;
        if(FirstCD){
          fprintf(File, "%s[ACCN]",mrip->CdName); 
          FirstCD = FALSE;
        }
        else fprintf(File, "+OR+%s[ACCN]",mrip->CdName);
        mapinfo_data = mapinfo_data->next;
      }
      mapinfo = mapinfo->next;
    }
    fprintf(File, "\" ");
    fprintf(File, "alt=\"Click to see Entrez Conserved Domains.\" ONMOUSEOVER=\"self.status=\'Click to see Entrez Conserved Domains.'; return true\" >\n");
  }

  pix_per_res = (float)MaxSeqImgSize/(float)(maxseqlen-1);
  right = LabelSpace + (float)(cip->query_length-1)*pix_per_res;

  mapinfo = cip->mapinfo;
  while(mapinfo){
    mapinfo_data = mapinfo->data.ptrvalue;
    y1 = ConsDomStartY + mapinfo->choice*CdMapRowSpace + (mapinfo->choice-1)*FontBH;
    y2 = y1 + FontBH;
    while(mapinfo_data){
      mrip = mapinfo_data->data.ptrvalue;
      Dart_Acc2Info(dartcon, mrip->CdName, shortname, MAX_TBUFF, NULL, NULL, NULL, NULL);
      x1 = MAX(x, (Int4)(x + (float)(mrip->from-1.5)*pix_per_res));
      x2 = MIN(right, (Int4)(x+ (float)(mrip->to-0.5)*pix_per_res));
      iColor = mrip->iColor;
      r = iDartCol[iColor][0]; 
      g = iDartCol[iColor][1]; 
      b = iDartCol[iColor][2]; 
      gdImageRoundRectangle(im, x1, y1, x2, y2, 8, 5, gdImageColorAllocate(im, r, g, b), 1);
      if(DrawMap){
/*      fprintf(File, "<area shape=rect coords=%d,%d,%d,%d href=%s?ascbin=2&maxaln=10&seltype=3&uid=%s&query=%s&aln=%s", x1, y1, x2, y2, CDSRVurl, mrip->CdName, query_seq, mrip->cdaln);     */
        fprintf(File, "<area shape=rect coords=%d,%d,%d,%d href=\"%s?ascbin=2&maxaln=10&seltype=3&uid=%s&queryrid=%s&aln=%s", x1, y1, x2, y2, CDSRVurl, mrip->CdName, queryrid, mrip->cdaln);      
        fprintf(File, "\" ");
        fprintf(File, "alt=\"%s, click to see CD alignment\" ONMOUSEOVER=\"self.status=\'%s, click to see CD alignment'; return true\" >\n", mrip->CdName, mrip->CdName);
      }
      
      len1 = StrLen(shortname)*FontBW;
      len2 = StrLen(shortname)*FontMW;
      y3 = y1+2;
      color = white;
      if(iColor == 2 || iColor==4 || iColor==0) color = black; 
      if ((x2-x1-4) > len1) {
        x3 = (x1+x2 - len1)/2;
        gdImageString(im, gdFont7X13b, x3,y3, shortname, color);
      }
      else if ((x2-x1-4) > len2) {
        x3 = (x1+x2 - len2)/2;
        gdImageString(im, gdFont6X12, x3, y3, shortname,color);
      }
      else {
        char_num = (x2-x1-4)/FontBW;
        if (char_num >= 3) strcpy(&(shortname[char_num-3]), "...");
        else switch (char_num) {
          case 1: strcpy(shortname, "."); break;
          case 2: strcpy(shortname, "..");
        }
        gdImageString(im, gdFont7X13b, x1+4, y3, shortname,color);
      }

      mapinfo_data = mapinfo_data->next;
    } 
    mapinfo = mapinfo->next;
  } 

}
/*----------------------*/
Int4 DrawBlastColorKeyImg(gdImagePtr im, Int4 seqlen)
{
  Int4  red, magenta, green, black, blue, white;
  Int2  x, left, right, x1 = 0, x2 = 0, y1 = 0, y2 = 0;
  double  lenfrac;

  white= gdImageColorAllocate(im, 255, 255, 255);
  red   = gdImageColorAllocate(im, 255,   0,   0);
  magenta = gdImageColorAllocate(im, 255,   0, 255);
  black = gdImageColorAllocate(im,   0,   0,   0);
  blue = gdImageColorAllocate(im, 0, 0, 255);
  green = gdImageColorAllocate(im, 0, 255, 0);

  x = LabelSpace; y1 = 20; 
  left = x; 
  lenfrac = (float)(seqlen-1)/(float)(maxseqlen-1);
  right = (Int4)(x + lenfrac*(float)MaxSeqImgSize); 

  y2 = y1 + FontBH;
  
  gdImageString(im, gdFont7X13b, 260, y1, "Color Key for Alignment Scores", black);

  y1 = y2 + 0.25*FontBH;
  y2 = y1 + FontBH + 2;

  x1 = left; x2 = x1 + (int)((right-left)/5);
  gdImageFilledRectangle(im, x1, y1, x2, y2, black);
  gdImageString(im, gdFont7X13b, x1+60, y1 +1, "<40", white);

  x1 = x2; x2 = x1 + (int)((right-left)/5);
  gdImageFilledRectangle(im, x1, y1, x2, y2, blue);
  gdImageString(im, gdFont7X13b, x1+40, y1 +1, "40-50", white);

  x1 = x2; x2 = x1 + (int)((right-left)/5);
  gdImageFilledRectangle(im, x1, y1, x2, y2, green);
  gdImageString(im, gdFont7X13b, x1+40, y1 +1, "50-80", black);

  x1 = x2; x2 = x1 + (int)((right-left)/5);
  gdImageFilledRectangle(im, x1, y1, x2, y2, magenta);
  gdImageString(im, gdFont7X13b, x1+40, y1 +1, "80-200", black);

  x1 = x2; x2 = x1 + (int)((right-left)/5);
  gdImageFilledRectangle(im, x1, y1, x2, y2, red);
  gdImageString(im, gdFont7X13b, x1+40, y1 +1, ">200", black);

  gdImageRectangle(im, left - 3, 19, right + 3, y2 + 3, blue);
}
/*----------------------*/
Int4 DrawQueryImgAndMap(gdImagePtr im, BioseqPtr query_bsp, CdInfoPtr cip, Boolean DrawMap, WWWQueryPtr wqp_head, FILE *File)
{
  
  y_point = QueryChainImgAndMap(im, query_bsp, DrawMap, wqp_head, File);
  if(cip != NULL) ConsDomImgAndMap(im, query_bsp, cip, DrawMap, File);

  return 0;

} 
/*-----------------------*/
static Int4 DrawAlgImgAndMap(gdImagePtr im, Int2 cdmaprow_num, BlastPDBNbPtr bpnp, Int2 pagenum, Int2 ScoreLabel, Boolean DrawMap, Boolean AlgGraph, WWWQueryPtr wqp_head, FILE *File)
{
  Int4  dart_color, red, magenta, lightgreen, green, black, white, labelcol, blue, algcolor;
  float pix_per_res;
  OverLoc2   ol2;
  Boolean    hasCdLink = FALSE;

  SeqAlignPtr salp = NULL, salp_dart = NULL;
  DenseSegPtr dssp = NULL;
  SeqIdPtr sip = NULL, sip_giList = NULL;
  PDBSeqIdPtr pdb_sip = NULL;
  Int4Ptr starts = NULL, lens = NULL;
  Int4  start_master = 0, start_slave = 0, alglen = 0;
  Int2  iseg = 0;
  Int2   x, x1, x2, x3, y1, y2, y1_str, y2_str, i=0;
  Int2   left, right;
  Int2   CdNum = 0;
  char str[1024], str_score[1024];

  ValNodePtr csip = NULL;
  CblastSeqInfoDataPtr csidp = NULL;

  ValNodePtr csp = NULL;
  CblastScoreDataPtr csdp = NULL;

  ValNodePtr   query = NULL;
  CharPtr str_query = NULL;
  
  red   = gdImageColorAllocate(im, 255,   0,   0);
  magenta = gdImageColorAllocate(im, 255,   0, 255);
  black = gdImageColorAllocate(im,   0,   0,   0);
  white = gdImageColorAllocate(im, 255, 255, 255);
  blue = gdImageColorAllocate(im, 0, 0, 255);
  green = gdImageColorAllocate(im, 0, 255, 0);
  lightgreen = gdImageColorAllocate(im, 0, 255, 127);

  y1= ConsDomStartY + cdmaprow_num * (CdMapRowSpace+FontBH) + MapRowSpace;
/*if(AlgGraph) y1 = y1-ColorKeyY; */
  
  y2 = y1 + FontBH + 2;

  if(!AlgGraph){
    if(ScoreLabel == 1){
      gdImageString(im, gdFont7X13b, ScoreLabelStart, y1, "  E  ", black);
      gdImageString(im, gdFont7X13b, ScoreLabelStart, y2, "Value", black);
    }
    else if(ScoreLabel == 2){
      gdImageString(im, gdFont7X13b, ScoreLabelStart, y1, "Score", black);
      gdImageString(im, gdFont7X13b, ScoreLabelStart, y2, "(bits)", black);
    }
    else if(ScoreLabel == 3){
      gdImageString(im, gdFont7X13b, ScoreLabelStart, y1, "Aligned", black);
      gdImageString(im, gdFont7X13b, ScoreLabelStart, y2, "Length", black);
    }
    else if(ScoreLabel == 4){
      gdImageString(im, gdFont7X13b, ScoreLabelStart, y1, " Sequence", black);
      gdImageString(im, gdFont7X13b, ScoreLabelStart, y2, "Identity(%)", black);
    }
/*  gdImageString(im, gdFont7X13b, LabelStart, y1, " Subject ", black); */
    gdImageString(im, gdFont7X13b, LabelStart, y2, "Structure", black);
  }

/*gdImageString(im, gdFont7X13b, 150, y2, "Distribution of aligned blocks on the query", black); */

  pix_per_res = (float)(MaxSeqImgSize)/(float)(maxseqlen-1);

  y1= y2 + FontBH + 2*MapRowSpace;
  y2 = y1 + MapRowHeight;
  x_point = LabelSpace;
  x = x_point; 
  right = x +(float)(bpnp->query_length-1) * pix_per_res;

  str_query = StringSave("blast_view");
  ValNodeAddStr(&query, 0, str_query);
  str_query = StringSave("blast_rep_gi");
  ValNodeAddStr(&query, 0, str_query);
  str_query = StringSave("hit");
  ValNodeAddStr(&query, 0, str_query);
  str_query = StringSave("hsp");
  ValNodeAddStr(&query, 0, str_query);

  salp = bpnp->sap->data;
  csp = bpnp->csp;
  csip = bpnp->csip;
  while(salp){
    csdp = csp->data.ptrvalue;
    csidp =csip->data.ptrvalue; 
           /* pagenum assigned only for bRep, see cblastutil.c */
    if(csidp->pagenum != pagenum  ){
      salp = salp->next;
      csp = csp->next;
      csip = csip->next;
      continue;
    }

/*
    print_bitscore(csdp->bit_score, str_score);
    if(AlgGraph) gdImageString(im, gdFont6X12, ScoreLabelStart, y1, str_score, black);
    else gdImageString(im, gdFont6X12, ScoreLabelStart, y1, str_score, blue); 

    if(DrawMap && !AlgGraph){
      fprintf(File, "<area shape=rect coords=%d,%d,%d,%d href=\"%s%s?viewmethod=1&viewmethod_option=HTML&blast_view=onepair&blast_rep_gi=%d&hit=%d&hsp=%d", ScoreLabelStart, y1, (ScoreLabelStart+StrLen(str_score)*gdFont6X12->w), (y1+gdFont6X12->h), CGIurl, CGIName, csidp->gi, csidp->PDBgi, csidp->hsp);
      print_query_args(File, wqp_head, query);
      fprintf(File, "\" ");
      fprintf(File, "alt=\"Blast bits Score, click to see alignment\" ONMOUSEOVER=\"self.status=\'Blast bits Score, click to see alignment\'; return true\" >\n");
    } 
    print_evalue(csdp->evalue, str_score);
    if(AlgGraph) gdImageString(im, gdFont6X12, EvalueLabelStart, y1, str_score, black);
    else gdImageString(im, gdFont6X12, EvalueLabelStart, y1, str_score, blue);
    if(DrawMap && !AlgGraph){
      fprintf(File, "<area shape=rect coords=%d,%d,%d,%d href=\"%s%s?viewmethod=1&viewmethod_option=HTML&blast_view=onepair&blast_rep_gi=%d&hit=%d&hsp=%d", EvalueLabelStart, y1, (EvalueLabelStart+StrLen(str_score)*gdFont6X12->w), (y1+gdFont6X12->h), CGIurl, CGIName, csidp->gi, csidp->PDBgi, csidp->hsp);

      print_query_args(File, wqp_head, query);
      fprintf(File, "\" ");
      fprintf(File, "alt=\"Blast E value, click to see the alignment\" ONMOUSEOVER=\"self.status=\'Blast E value, click to see the alignment\'; return true\" >\n");
    }
*/

    if(!AlgGraph){
      if(ScoreLabel == 1) print_evalue(csdp->evalue, str_score);
      else if(ScoreLabel == 2) print_bitscore(csdp->bit_score, str_score); 
      else if(ScoreLabel == 3)sprintf(str_score, "   %d\n", csdp->length);
      else if(ScoreLabel == 4) sprintf(str_score, "   %d\n", (int)(csdp->seq_identity*100));
      gdImageString(im, gdFont6X12, ScoreLabelStart, y1, str_score, black);
    }
                             
/*
    if(csdp->bit_score>=200)algcolor = red;
    else if(csdp->bit_score>=80) algcolor = magenta;
    else if(csdp->bit_score >=50) algcolor = green;
    else if(csdp->bit_score >=40) algcolor = blue;
    else algcolor = black; 
*/
    /* do not use blast color, just use red */
    algcolor = red;

    dssp = salp->segs;
    starts = dssp->starts; lens = dssp->lens;

    pdb_sip = csidp->pdbid->data.ptrvalue;
    x1 = LabelStart;
    sprintf(str, "%s %c", pdb_sip->mol, pdb_sip->chain);
    gdImageString(im, gdFont6X12, x1, y1 - 0.25*FontBH, str, blue);
    if(DrawMap){
       x1 = LabelStart; 
       x2= LabelStart+StrLen(str)*gdFont6X12->w;
       y1_str = y1 - 0.25*FontBH;
       y2_str = y1_str + gdFont6X12->h; 
       fprintf(File, "<area shape=rect coords=%d,%d,%d,%d href=\"%s%s\" alt=\"protein structure %s %s, click for MMDB structure summary\" ONMOUSEOVER=\"self.status=\'protein structure %s %s, click for MMDB structure summary\'; return true\" >\n", x1, y1_str, x2, y2_str, MMDBcgi, pdb_sip->mol, pdb_sip->mol, csidp->doc, pdb_sip->mol, csidp->doc);
    }
    x1 = LabelStart + StrLen(str)*gdFont6X12->w + 10;
    sprintf(str, " %d",  (*(starts+1)+1));
    gdImageString(im, gdFont6X12, x1, y1 - 0.25*FontBH, str, black);

    for(iseg = 0; iseg < dssp->numseg; iseg++){
      start_master = *starts; starts++;
      start_slave = *starts; starts++;
      alglen = *lens;
      lens++;
      if(start_master == -1 || start_slave == -1) continue;
      x1 = MAX(x, (Int4)(x + (float)(start_master -1.5)*pix_per_res));
      x2 = MIN(right, (Int4)(x+(float)(start_master+alglen -0.5)*pix_per_res));
      gdImageFilledRectangle(im, x1, y1, x2, y2, algcolor);     
      if(DrawMap && !AlgGraph){
        fprintf(File, "<area shape=rect coords=%d,%d,%d,%d href=\"%s%s?viewmethod=1&viewmethod_option=HTML&blast_view=onepair&blast_rep_gi=%d&hit=%d&hsp=%d", x1, y1, x2, y2, CGIurl, CGIName, csidp->gi, csidp->PDBgi, csidp->hsp); 
        print_query_args(File, wqp_head, query);
        fprintf(File, "\" ");

        fprintf(File, "alt=\"residue %d to %d aligned, click to see see alignment\" ONMOUSEOVER=\"self.status=\'residue %d to %d aligned, click to see alignment\'; return true\" >\n", start_slave, (start_slave+alglen), start_slave, (start_slave+alglen));
      }
    }    

    x = x_point;
    y1 = y2 + MapRowSpace;
    y2 = y1 + MapRowHeight;     

    salp = salp->next;
    csp = csp->next;
    csip = csip->next;
  }

  ValNodeFreeData(query);
  y_point = y2;
} 
/*--------------------------*/
Int4 DrawSummaryImgAndMap(BioseqPtr query_bsp, CdInfoPtr cip, BlastPDBNbPtr bpnp, Int2 imgsize, Int2 pagenum, Int2 ScoreLabel, Boolean DrawMap, Boolean AlgGraph, WWWQueryPtr wqp, FILE *File)
{
  gdImagePtr      im;
  Int4 white;

  im = gdImageCreate(GraphWidth, imgsize);

  white   = gdImageColorAllocate(im, 255, 255, 255);

/*DrawBlastColorKeyImg(im, query_bsp->length); */

  DrawQueryImgAndMap(im, query_bsp, cip, DrawMap, wqp, File); 

  if(cip) DrawAlgImgAndMap(im, cip->maprow_num, bpnp, pagenum, ScoreLabel, DrawMap, AlgGraph, wqp,File);
  else DrawAlgImgAndMap(im, 0, bpnp, pagenum, ScoreLabel, DrawMap, AlgGraph, wqp,File); 
  
  if(!DrawMap){
    printf("Content-type: image/gif\n\n");
    gdImageGif(im, stdout);                   
  }
   
  gdImageDestroy(im);

  return 0;
}
