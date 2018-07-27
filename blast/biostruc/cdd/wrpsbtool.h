/* $Id: wrpsbtool.h,v 1.5 2001/03/09 22:52:01 bauer Exp $
*===========================================================================
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
* File Name:  wrpsbtool.h
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 4/19/2000
*
* $Revision: 1.5 $
*
* File Description:
*         header for WWW-RPS BLAST tools
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: wrpsbtool.h,v $
* Revision 1.5  2001/03/09 22:52:01  bauer
* use JavaScript to embed graphics in results page
*
* Revision 1.4  2000/10/12 20:48:26  bauer
* added absolute URLs for graphics elements on the CD-search results pages
*
* Revision 1.3  2000/08/03 22:02:24  bauer
* added support for 3D-structure link highlighting
*
* Revision 1.2  2000/07/19 19:19:14  bauer
* added modification logging
*
* ==========================================================================
*/


#define BUFFER_LENGTH 255
#define DDVCOL_DEFAULT 0
#define DDVCOL_HOTPINK 1
#define DDVCOL_MAGENTA 2
#define DDVCOL_PURPLE 3
#define DDVCOL_BLUE 4
#define DDVCOL_SKY 5
#define DDVCOL_CYAN 6
#define DDVCOL_SEA 7
#define DDVCOL_GREEN 8
#define DDVCOL_YELLOW 9
#define DDVCOL_GOLD 10
#define DDVCOL_ORANGE 11
#define DDVCOL_RED 12
#define DDVCOL_PINK 13
#define DDVCOL_PINKTINT 14
#define DDVCOL_WHITE 15
#define DDVCOL_BLACK 16
#define DDVCOL_BLUETINT 17
#define DDVCOL_GREENTINT 18
#define DDVCOL_YELLOWTINT 19
#define DDVCOL_GRAY 20
#define DDVCOL_BROWN 21
#define DDV_COLOR_MAX 33

#include <gd_io.h>

typedef struct borkIOCtx {
  gdIOCtx	ctx;
  CharPtr	buffer;
  Int4	len;
  Int4	bufsz;
} borkIOCtx;

gdIOCtx* newBorkCtx(FILE *f);
static     Int4                   borkPutbuf(gdIOCtx*, const void *, Int4);
static     void                   borkPutchar(gdIOCtx*, Int4 );
static     void                   freeBorkCtx(gdIOCtx *ctx);
           Boolean                OverlapInterval(Int4 from1, Int4 to1, Int4 from2, Int4 to2);
           void                   WRPSBHtmlError(CharPtr cErrTxt); 
           Int4                   max(Int4 i1, Int4 i2);
           Int4                   min(Int4 i1, Int4 i2);
           Boolean                print_score_eonly(FloatHi evalue, CharPtr buf);
           Boolean                print_score_sonly(FloatHi bit_score, CharPtr buf);
           Boolean                print_score_evalue (FloatHi evalue, FloatHi bit_score, CharPtr buf);
           void                   WRPSBPrintDefLinesFromSeqAlign(AlignmentAbstractPtr aap, FILE *table, Boolean bAnyPdb, BioseqPtr query_bsp, CharPtr urlcgi);
           Boolean                WRPSBAcknowledgeBlastQuery(BioseqPtr bsp, Int4 line_length, FILE *outfp, Boolean believe_query, Boolean html);
static     Int4                   WRPSBGetSeqAlignCount(SeqAlignPtr sap);
static     SeqAlignPtr            WRPSBGetPrivateSeqAlign(SeqAlignPtr sap, Int4 number, Int4Ptr number_returned);
           BlastPruneSapStructPtr WRPSBlastPruneHitsFromSeqAlign(SeqAlignPtr sap, Int4 number, Nlm_FloatHi ecutoff, BlastPruneSapStructPtr prune);
           BlastPruneSapStructPtr WRPSBlastPruneSapStructDestruct(BlastPruneSapStructPtr prune);
static     CharPtr                StringAppend(CharPtr *dst, size_t *size, CharPtr src, size_t *used);
           Boolean                SeqAlignSegsStr(SeqAlignPtr sap, Int2 index, CharPtr *dst, size_t *size, size_t *used);
static     void                   DDV_BlastGetColorSchemaZero(DDVOptionsBlockPtr dobp);
static     void                   DDV_BlastGetColorSchemaOne(DDVOptionsBlockPtr dobp);
static     void                   DDV_BlastGetColorSchemaTwo(DDVOptionsBlockPtr dobp);
static     void                   DDV_BlastGetColorSchemaThree(DDVOptionsBlockPtr dobp);
static     void                   DDV_BlastGetColorSchemaFour(DDVOptionsBlockPtr dobp);
static     void                   DDV_BlastGetColorSchemaFive(DDVOptionsBlockPtr dobp);
static     void                   DDV_BlastGetColorSchemaSix(DDVOptionsBlockPtr dobp);
static     void                   DDV_InitDefSAPdispStyles_BLAST(DDV_Disp_OptPtr ddop);
static     Boolean                DDV_DisplayNewBLAST(SeqAlignPtr sap, ValNodePtr mask,Int4Ptr PNTR matrix,Uint4 disp_format,Pointer disp_data,FILE *fp);
           Boolean                WRPSBDisplayBlastPairList(AlignmentAbstractPtr aap,ValNodePtr mask, FILE *fp,Boolean is_na, Uint4 tx_option,Uint1 ColorSchema,RPSBlastOptionsPtr rpsbop);
           Boolean                WRPSBCl3DisplayBlastPairList(AlignmentAbstractPtr aap,ValNodePtr mask, FILE *fp,Boolean is_na, Uint4 tx_option,Uint1 ColorSchema,BioseqPtr query_bsp, CharPtr dbversion, CharPtr urlcgi);
