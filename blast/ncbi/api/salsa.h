/*   salsa.h
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
* File Name:  salsa.h
*
* Author:  Colombe Chappey
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.5 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _SALSA_
#define _SALSA_

#include <ncbi.h>
#include <objseq.h>
#include <objfdef.h>
#include <objmgr.h>
#include <gather.h>

#define COMPSEG           6

#define TypeEmpty         0
#define TypeSeqInt        1 
#define TypeSeqId         2 
#define TypeSeqLoc        3
#define TypeSelStruct     4
#define TypeSelEdStruct   5

#define NO_RESIDUE       -9
#define GAP_RESIDUE      -1

#define SEQ_VIEW          0
#define SEQ_EDIT          1
#define ALIGN_EDIT        2
#define BLOCK_EDIT        3
#define PRETTY_EDIT       4

#define TMP_BUFFERLENGTH  6000
#define TMP_EDITBUFFER    200

#define WINPERBUF         3
#define MAXCharLine     200
#define MAXLineWindow   150
#define LENGTHMAX     10000
#define SALSALENLIMIT 50000

#define HRZ_BORDER_WIDTH  4
#define VER_BORDER_WIDTH  4
#define EDIT_MARGIN_RIGHT 10
#define EDIT_MARGIN_BOT   5

#define SEQ_SHOW1         1
#define SEQ_SHOWALG       2
#define SEQ_FEAT0         0
#define SEQ_FEAT1         1
#define SEQ_FEATALL       2
#define SEQ_NUM0          0
#define SEQ_NUM1          1
#define SEQ_NUM2          2

#define SALSA_ND          0
#define SALSA_FASTA       1  
#define SALSA_ASN1        2 
#define SALSA_CONTIGUOUS  3 
#define SALSA_FASTGAP     4 
#define SALSA_PAUP        5 
#define SALSA_MACAW       6
#define SALSA_INTERLEAVE  7 
#define SALSA_NEXUS       8 
#define SALSA_PHYLIP      9 
#define SALSA_CLUSTALV   10
#define SALSA_SHWTXT     11 
#define SALSAA_FASTA     12 
#define SALSAA_CONTIGUOUS 13 
#define SALSAA_FASTGAP   14 
#define SALSAA_INTERLEAVE 15 
#define SALSAA_PHYLIP    16
#define SALSAA_NEXUS     17
#define SALSAA_GCG       18

#define PRG_ANYALIGN      0
#define PRG_FASTA_IMPORT  1 
#define PRGALIGNBANDBL    6 
#define PRG_BLAST         7
#define PRGALIGNALL       8
#define PRGALIGNDEFAULT  10 

#define PROPAG_SETSTOP 1 
#define PROPAG_GETSTOP 2 

#define LINE0             0

#define COLOR_STAR        0
#define COLOR_SCALE       1
#define COLOR_ID          2
#define COLOR_GAP         3
#define COLOR_ID_MASTER   4 
#define COLOR_CDS         5 
#define COLOR_SELECT      6 

#define ALLPROTAA         1
#define MPROTAA           2
#define PUTPROT           3

#define EDITDEF_SCA   101 
#define EDITDEF_SCB   102
#define EDITDEF_CPL   103
#define EDITDEF_RF1   104
#define EDITDEF_RF2   105
#define EDITDEF_RF3   106
#define EDITDEF_RF4   107
#define EDITDEF_RF5   108
#define EDITDEF_RF6   109
#define FEATDEF_TRSL  110

#define LINE_NOGAP      1
#define LINE_WITHGAP    2
#define LINE_ONLYGAP    3

#define PRINTID_GIcc   99 

/******************************
**  Types define  
*******************************/
typedef struct seledstruct 
{
        Boolean    dirty;
        Boolean    visible;
        Uint2      entityID,
                   itemID,
                   itemtype,
                   itemsubtype;
        Uint2      bsp_itemID;
        Pointer    region;       
        Uint1      regiontype;     /* 0 = not set
                                 1 = SeqLocPtr of type SEQLOC_INT       */
        Char       label[101];
        ValNodePtr data;
        Uint1      codonstart;
        Int4       offset;
        struct seledstruct PNTR prev,
                           PNTR next;
} SelEdStruct, PNTR SelEdStructPtr;

typedef struct seqparam {
        Uint2    entityID;
        Uint2    itemID;
        Boolean  complement;
        Boolean  rf[10];
        Uint2    group;
        SeqIdPtr sip_cons;
} 
SeqParam, PNTR SeqParamPtr;

typedef struct alignstyle {
  Handle          font;
  Int2            interline;        /* space between lines          */
  Boolean         marginwithindex;  /* index in left margin    */ 
  Boolean         marginwithIds;    /* Ids in left margin      */
  Boolean         marginwithfeatid;
  Boolean         marginwithpos;    /* position in left margin */
  Boolean         marginwithgroup;
  Int2            marginleft;       /* left margin (char)           */
  Boolean         draw_scale;
  Boolean         draw_bars;
  Uint1           rowpcell;         /* rows per cell */
  Uint1           columnpcell;      /* columns per cell */
  Uint4           colorRefs[256];
 
} AlignStyle, PNTR AlignStylePtr;


typedef  struct  rectc {
  Int2  left;
  Int2  top;
  Int2  right;
  Int2  bottom;
} RecTc, PNTR RectcPtr;
 
typedef struct editaligndata {

  Boolean          Cn3D_On;

  Pointer         wdp;
  Uint2           input_entityID,
                  input_itemID,
                  input_itemtype;
  Int2            seqnumber;        /* number of sequences       */
  Int4            length;           /* alignment length          */
  Boolean         seg_bioseq;
  Uint1           mol_type;         /* NA, DNA, RNA, AA        */

  ValNodePtr      anp_list;         /* list of AlignNode / sequence */ 
  ValNodePtr      anp_graph;
  SelEdStructPtr  seq_info;
  ValNodePtr      sqloc_list;       /* SeqLocPtr list  on sequences*/
  ValNodePtr      bsqloc_list;
  Int2            size_labels;

  SelStruct       master;           /* entityID, item, SeqLoc of master seq*/
  GatherRange     gr;

  SeqAnnotPtr     sap_align;        /* SeqAnnotPtr-SeqAlignPtr-CompSeg   */
  SeqAnnotPtr     sap_original;     /* SeqAnnotPtr-SeqAlignPtr-DenSeg */
  SeqAnnotPtr     sap1_original;    /* SeqAnnotPtr-SeqAlignPtr-DenDiag */
  SeqAlignPtr     blocks;        /* list of DenDiag to be aligned     */

  Boolean         dirty;

  /*    Features   */

  Uint1          featOrder[FEATDEF_ANY];  /* features displayed  */
  Uint1          groupOrder[FEATDEF_ANY]; /* ? */
  Uint1          gap_choice;       /* used only for propagation features */
                                   /* through the Dialog */
  /*    PaneL   */

  Int4            x, xoff, y, yoff;
  Int4            ybutt;
  Boolean         vscrollbar_mode;
  Int4            hoffset;          /* horizontal offset - align coordi. */
  Int4            voffset;          /* vertical offset - align coordina. */
  Int4            pnlLine;          /* number of visible lines     */  
  Int4            pnlWidth;         /* number of visible characters*/
  Int4            vPage;            /* lines/page = pnlLine - 1    */
  Int4            hPage;            /* car/page   = pnlWidth- 1    */
  Int4            nlines;
  Int4            curalignline;
  SelStructPtr    firstssp;
  SelEdStructPtr  lastses;
  Int4            numberalignline;
  Uint2Ptr        item_id;          /* array of itemID / line      */
  Uint2Ptr        seqEntity_id;     /* array of entityID / line    */
  Uint2Ptr        itemtype;         /* array of feattype / line    */
  Uint2Ptr        itemsubtype;      /* array of feattype / line    */
  Uint4Ptr        alignline;        /* array of alignment line index*/  

  /*    text   */

  Handle          font;             /* font type                   */
  Int2            ascent;
  Int2            leading;
  Int2            charw;
  Int2            lineheight;
  Uint4           colorRefs[256];
  Int2            popcolor[10];
  Int2            scaleheight;      /* scale height= 2 x lineheight */
  Int2            interline;        /* space between lines          */
  RecTc            margin;           /* margin (pix) around sequences*/
  Int2            marginleft;       /* left margin (char)           */
  Int2            intersalpwidth;

  /*   buffer of aligned char  - alignment coordinates */

  SelStructPtr    buffer; 
  SelStructPtr    buffertail;
  ValNodePtr      linebuff; 
  Int4            minbufferlength;  /* MIN (2 screens or .length)   */
  Int4            bufferlength;     /* varies with edit inser-del   */
  Int4            bufferstart;      /* in alignment coordinates     */
  Int4            editbuffer;       /* length of edit buffer        */
  Int4Ptr         colonne;          /* seq position in the buffer   */
 
  /*   visible part of the buffer - buffer coordinates */

  Int4            visibleWidth;     /* alignt length per line in Panel*/
  Int4            visibleLength;

  /*   options   */

  Int2            styleNum;
  SelStruct       edit_item;
  AlignStylePtr   newstyle;
  Boolean         showfeat;
  Boolean         drawrfp, drawrfm;
  Boolean         drawcomplt;
  Boolean         draw_scale;
  Boolean         draw_bars;
  Uint1           nscaleline;
  Uint1           rowpcell;         /* rows per cell */
  Uint1           columnpcell;      /* columns per cell */
  Boolean         displaytype;
  Boolean         charmode;         /* diff y/n >>> SetStatus ItemI */
  Boolean         marginwithindex;  /* index in left margin    */ 
  Boolean         marginwithIds;    /* Ids in left margin      */
  Boolean         marginwithfeatid;
  Boolean         marginwithpos;    /* position in left margin */
  Boolean         marginwithgroup;
  Int4            edit_pos; 
  Int4            feat_pos; 
  Int4            start_select;
  Uint1           click_feat;
  Uint1           clickwhat;
  Uint1           mouse_mode;
  DenseDiagPtr    select_block;
  SelStruct       caret;            /* cursor within alignment */
  Uint2           caret_line;
  Uint2           feat_line;
  Int4            caret_orig;
  ValNodePtr      feat;
  ValNodePtr      seqfeat;
  ValNodePtr      allseqfeat;
  Uint4           nfeat;
  Uint1           prot_mode;
  Uint2           printid;
  Int4            stoptransl;
  ValNodePtr      params;

  Boolean         spliteditmode;

  SelEdStructPtr  curfeat;
  Uint1           align_format;     /* PHYLIP, MACAW, FASTA, ..*/ 
  Uint2           ngroup;

  CharPtr         current_pattern;
  SelStructPtr    match_pat;
  SelStructPtr    cur_pat;

  /*   dialog   */

  VoidPtr         extra_data;
  Int4            int4value2;       /* any value from dialog */
  Int4            int4value;        /* any value from dialog */

  Uint1           display_panel;
  Boolean         all_sequences;
  Uint1           edit_mode;
  Uint1           input_format;
  Boolean         draw_emptyline;
  Char            tmpfile [PATH_MAX];  /* path + filename         */ 

} EditAlignData, PNTR EditAlignDataPtr;

typedef struct compressseg {

  Int2            dim,
                  numseg;
  SeqIdPtr        ids;
  Int4Ptr         from;
  Int4Ptr         lens;
  BoolPtr         starts;
  Uint1Ptr        strands;
  ScorePtr        scores;

} CompSeg, PNTR CompSegPtr;

typedef struct propastruct {
                                     
  SeqAnnotPtr  sap;
  Uint2        source_entityID, 
               source_bspitemID, 
               target_entityID, 
               target_bsp_itemID;
  SeqEntryPtr  target_sep,
               source_sep;
  ValNodePtr   source_seqfeat, 
               target_seqfeat;
  Uint1        gap_choice;
  Int4         stoptransl;
  Boolean      keep_protID;

} PropaStruct, PNTR PropaStructPtr;

#endif
