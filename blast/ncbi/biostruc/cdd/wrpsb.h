/* $Id: wrpsb.h,v 1.16 2003/11/19 14:34:31 bauer Exp $
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
* File Name:  wrpsb.h
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 1/19/2000
*
* $Revision: 1.16 $
*
* File Description:
*         Header file for WWW-RPS BLAST client
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: wrpsb.h,v $
* Revision 1.16  2003/11/19 14:34:31  bauer
* changes to support SeqAnnot export
*
* Revision 1.15  2003/10/07 21:15:15  bauer
* support generation of Sequence Annotation from CD-Search results
*
* Revision 1.14  2003/07/25 21:00:28  bauer
* changes to accomodate KOGs
*
* Revision 1.13  2003/01/09 21:56:29  bauer
* reduced default number of hits to 25
*
* Revision 1.12  2002/12/24 18:23:18  bauer
* changes for v1.60
*
* Revision 1.11  2002/08/16 19:50:36  bauer
* increased timeout span
*
* Revision 1.10  2002/08/06 02:39:09  bauer
* changes to accomodate COGs
*
* Revision 1.9  2002/06/12 15:03:49  bauer
* 6/11/02 update
*
* Revision 1.8  2002/05/23 20:19:16  bauer
* intermediate version of wrpsb
*
* Revision 1.7  2002/05/06 17:18:04  bauer
* switched to graphics on the fly
*
* Revision 1.6  2002/03/07 19:12:14  bauer
* major revisions to cgi-bins and the CD-dumper
*
* Revision 1.5  2001/02/22 15:53:22  bauer
* support for jagged edge blocks
*
* Revision 1.4  2000/12/08 20:15:22  bauer
* added support for Queued RPS-Blast
*
* Revision 1.3  2000/09/18 17:26:46  bauer
* CddHitPtr now extracted from pruned seqalign
*
* Revision 1.2  2000/07/19 19:29:34  bauer
* added modification logging
*
*
* ==========================================================================
*/

#include "objcdd.h"
#include "wrpsbutil.h"

#define CPUTIME_MAX        900
#define RPSBNAME           "wrpsb.cgi"
#define QRPSBNAME          "wrpsb.cgi"
#define DEFAULT_DATALIB    "cdd"
#define DEFAULT_EVALUE     "0.01"
#define DEFAULT_NHITS      "25"
/* #define NUMARGS            (sizeof(myargs)/sizeof(myargs[0])) */
#define RPS_MAGIC_NUMBER   7702
#define WRPSB_GRAPH_WIDTH  600
#define WRPSB_GRAPH_MAXROW 12
#define WRPSB_GRAPH_HEIGHT 16
#define WRPSB_GRAPH_SPACER  4
#define BUFFER_LENGTH      3072
#define CDD_MAX_DESCR      3072
#define MUTUAL_OVERLAP     .5
#define USE_PNG
#undef RESULTS_FILE

#define DRAWSEARCHPAGE     0
#define SENDREQUEST        1
#define SENDQUEUEDREQUEST  2
#define RETRIEVEQUEUE      3
#define RETRIEVEPRECALC    4
#define DRAWQUEUED         5
#define DRAWONLY           6
#define DARTSIZELIMIT      1500
#define DARTFAMILYNUM      15000



typedef struct _txdfline_struct {
  struct _txdfline_struct *next;
  SeqAlignPtr             seqalign;
  SeqIdPtr                id;
  Char                    *buffer_id;
  Char                    *title;
  Nlm_FloatHi             bit_score, evalue;
  Int4                    score, number;
  Boolean                 is_na, found_score;
  Boolean isnew;          /* used to print mark "new.gif" near defline */
  Boolean waschecked;     /* used to print some .gif near such defline */
  CharPtr segs_str;       /* Used to print segs for dumpgnl program.   */
  size_t                  segs_buflen, segs_used;
} TxDfLineStruct, *TxDfLineStructPtr;


typedef struct _rps_blast_options {
  BLAST_OptionsBlkPtr options;
  CharPtr             blast_database;
  BioseqPtr           query_bsp, fake_bsp;
  Int4                number_of_descriptions, number_of_alignments;
  FILE                *outfp;
  AsnIoPtr            aip_out;
  Boolean             html;
  Boolean             believe_query;
  Uint4               align_options, print_options;
  /* RPS Blast variables */
  CharPtr             rps_database;
  CharPtr             rps_matrix;
  CharPtr             rps_lookup;
  RPSInfoPtr          rpsinfo;
} RPSBlastOptions, PNTR RPSBlastOptionsPtr;

typedef struct _cdd_hit {
  CharPtr              CDDid;
  CharPtr              ShortName;
  CharPtr              Definition;
  Int4                 start;
  Int4                 stop;
  Int4                 score;
  Nlm_FloatHi          evalue;
  Nlm_FloatHi          bit_score;
  struct _cdd_hit PNTR next;
} CddHit, PNTR CddHitPtr;

typedef struct _alignment_abstract {
  ScorePtr                        score;
  Int4                            red, green, blue;
  Int4                            colcyc, nindents;
  Int4                            mstart, mstop, gstart, gstop;
  Int4                           *indents;
  Nlm_FloatHi                     nmissg, cmissg;
  Int4                            row;
  Int4                            pssmid;
  SeqAlignPtr                     salp;
  Char                            name[16];
  Boolean                         bDrawThisOne;
  Boolean                         bIsOasis;
  Boolean                         bIsProfile;
  Boolean                         bHasStructure;
  Boolean                         bIsArch;
  Boolean                         bIsArchComplete;
  TxDfLineStructPtr               defline;
  CharPtr                         long_defline;
  CharPtr                         cCDDid;
  CharPtr                         cGraphId;
  CharPtr                         cDatabase;
  CharPtr                         cHtmlLink;
  CharPtr                         cSeqFile;
  CddDescrPtr                     description;
  struct _alignment_abstract PNTR next;
} AlignmentAbstract, PNTR AlignmentAbstractPtr;


/*---------------------------------------------------------------------------*/
/* static global variables                                                   */
/*---------------------------------------------------------------------------*/
static Char    URLBase[PATH_MAX];
static Char    URLcgi[PATH_MAX];
static Char    DATApath[PATH_MAX];
static Char    CDDpath[PATH_MAX];
static Char    ENTREZurl[PATH_MAX];
static Char    DOCSUMurl[PATH_MAX];
static Char    MAILto[PATH_MAX];
static FILE    *OutputFile;
static Char    OutputName[200];
static Char    gunzip[PATH_MAX];
static Char    MMDBpath[PATH_MAX];
static Char    CDDdpath[PATH_MAX];
static Char    CDDvpath[PATH_MAX];
static Char    CDDextens[PATH_MAX];
static Char    RAWextens[PATH_MAX];
static Char    TREextens[PATH_MAX];
static Char    SEQextens[PATH_MAX];
static Int2    SortOn;
static Int4    cnt_MMDBid;
static Char    cCDDid[PATH_MAX];
static Char    CDDdescr[PATH_MAX];
static Char    database[PATH_MAX];
static Char    RPSBname[PATH_MAX];
static Char    CDDProper[PATH_MAX];
static Char    CDDOasis[PATH_MAX];
static Char    OASIScgi[PATH_MAX];
static Char    CDDcgi[PATH_MAX];
static Char    CDDPrefix[PATH_MAX];
static Char    CDDPost_O[PATH_MAX];
static Char    CDDPost_C[PATH_MAX];
static Char    CDDefault[PATH_MAX];
static Char    QUDefault[PATH_MAX];
static Char    CDDSearch1[PATH_MAX];
static Char    CDDSearch2[PATH_MAX];
static Char    CDDSearch3[PATH_MAX];
static Char    CDDSearch4[PATH_MAX];
static Char    CDDSearch5[PATH_MAX];
static Char    CDDSearch6[PATH_MAX];
static Char    CDDSearch7[PATH_MAX];
static Char    CDDSname1[PATH_MAX];
static Char    CDDSname2[PATH_MAX];
static Char    CDDSname3[PATH_MAX];
static Char    CDDSname4[PATH_MAX];
static Char    CDDSname5[PATH_MAX];
static Char    CDDSname6[PATH_MAX];
static Char    CDDSname7[PATH_MAX];
static Char    CDDlocat[PATH_MAX];
static Char    ODBCINI[PATH_MAX];
static Char    DARTUSER[PATH_MAX];
static Char    DARTPASS[PATH_MAX];

static unsigned iDartFam[DARTFAMILYNUM];
static Int4   iDartFamNum;

/*---------------------------------------------------------------------------*/
/* DART color scheme                                                         */
/*---------------------------------------------------------------------------*/
static Int4    iNcolors = 13;
static Int4    iDartCol[26][3] = {{255,  0,  0},     /* red   */
                                  {  0,  0,255},     /* blue  */
                                  {  0,255,  0},     /* green */
                                  {204,102,  0},     /* orange */
                                  {204,204,  0},     /* yeller */
                                  {153,204,255},     /* sky b */
                                  {102,153,  0},     /* spring */
                                  {204,102,153},     /* lavender */
                                  {  0,204,204},     /* cyan */
                                  {153,153,  0},     /* brown */
                                  {153, 51,255},     /* violet */
                                  {  0,153,153},     /* blue-green */
                                  {  0,204,102},     /* teal */
                                  {255,102,102},     /* pale red */
                                  {102,102,255},     /* pale blue */
                                  {102,255,102},     /* pale green */
                                  {255,204,102},     /* pale orange */
                                  {255,255,102},     /* pale yeller */
                                  {204,255,255},     /* pale sky b */
                                  {204,255,102},     /* pale spring */
                                  {255,204,255},     /* pale lavender */
                                  {102,255,255},     /* pale cyan */
                                  {204,204, 51},     /* pale brown */
                                  {204,102,255},     /* pale violet */
                                  {102,204,204},     /* pale blue green */
                                  {102,255,204}};    /* pale teal */

/*---------------------------------------------------------------------------*/
/* Function Prototypes                                                       */
/*---------------------------------------------------------------------------*/
static Boolean            CddGetParams();
static void               PGPGetPrintOptions(Boolean gapped, Uint4Ptr align_options_out,Uint4Ptr print_options_out);
static void               WRPSBlastOptionsFree(RPSBlastOptionsPtr rpsbop);
static RPSBlastOptionsPtr WRPSBReadBlastOptions(RPSBlastOptionsPtr rpsbop);
static void               WRPSBViewSeqAlign(SeqAlignPtr seqalign, RPSBlastOptionsPtr rpsbop, ValNodePtr mask);
static Boolean            WRPSBDrawPage();
static Int4               AccessionToGi(CharPtr string,  Int4Ptr PNTR giptr, Int2 seqtype);
static Int4               OldAccessionToGi(CharPtr string);
static Boolean            WRPSBGetArgs(CharPtr progname, Int2 numargs, ArgPtr ap);

