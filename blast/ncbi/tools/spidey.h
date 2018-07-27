/* ===========================================================================
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
*  any work or product based on this material.
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
* File Name:  spidey.h
*
* Author:  Sarah Wheelan
*
* Version Creation Date:   5/01
*
* $Revision: 6.19 $
*
* File Description: mrna-to-genomic alignment algorithms and functions
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: spidey.h,v $
* Revision 6.19  2004/03/25 21:20:03  kskatz
* All SPI_is_acceptor_* functions have been corrected: 'N' no longer contributes to nor subtracts from the score, log odds are calculated and the scores added; they are however all antilogged because there are too many places in the code where the score is expected to be between 0 and 1.  Also, corrected sequence frequency determination in SPI_is_acceptor_user and SPI_is_donor_user, as well as correcting for 'N'. Finally, and this all began with, I added matrices for Dictyostelium - command line -r -m
*
* Revision 6.18  2003/04/04 19:42:56  kskatz
* Added a new command line option (-R) to allow external users to point spidey to a repeat database that it can pass on to blast for filtering repeats
*
* Revision 6.17  2002/10/02 16:12:54  kskatz
* Added a new option to SPI_Options (bigintron_size) that holds a user-supplied maximum size (default = 220000) for introns and requires the option (bool) bigintron to be set to 'TRUE'; The functions affected are SPI_mRNAPtr SPI_AdjustForSplice(), SPI_is_consistent(), and SPI_FindPiece(); note that the default for bigintron_size is not set in SPI_OptionsNew() (yet)
*
* Revision 6.16  2002/06/27 11:53:33  wheelan
* additions to structures to support off-by-one bug fixes and printing of the cds length
*
* Revision 6.15  2002/05/07 18:42:53  wheelan
* changes to support user-defined splice matrices
*
* Revision 6.14  2002/04/16 17:54:03  wheelan
* increased XLINTRON sizes
*
* Revision 6.13  2002/04/04 17:17:24  wheelan
* changed endfuzz to 8
*
* Revision 6.12  2002/01/30 19:08:51  wheelan
* better support for revcomp
*
* Revision 6.11  2001/12/18 18:00:18  wheelan
* add strand
*
* Revision 6.10  2001/11/20 12:13:28  wheelan
* made SPI_GetProteinFrommRNA EXTERN
*
* Revision 6.9  2001/11/05 16:14:53  wheelan
* added option to print multiple alignment to a file
*
* Revision 6.8  2001/10/04 12:34:07  wheelan
* added bigintron option
*
* Revision 6.7  2001/10/03 14:19:29  wheelan
* include new alignment manager
*
* Revision 6.6  2001/09/04 13:46:37  wheelan
* made SPI_RemoveInconsistentAlnsFromSet and SPI_flip_sa_list extern
*
* Revision 6.5  2001/08/24 13:44:35  wheelan
* changed printaln to Int4
*
* Revision 6.4  2001/08/06 16:49:25  wheelan
* changed revcompthresh parameter to 55 from 65
*
* Revision 6.3  2001/07/11 17:57:07  wheelan
* added typedefs for multiple alignments
*
* Revision 6.2  2001/07/10 16:44:42  wheelan
* added functions to make a multiple alignment
*
* Revision 6.1  2001/05/24 16:27:58  wheelan
* initial checkin
*
*
* ==========================================================================
*/

#ifndef _SPIDEY_
#define _SPIDEY_

#include <ncbi.h>
#include <alignmgr2.h>
#include <actutils.h>
#include <dotseq.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define SPI_DROPOFF  50
#define SPI_GAPOPEN  10
#define SPI_GAPEXTEND  3
#define SPI_PENALTY   -5

#define SPI_MINBADEXON  6
#define SPI_MAXBADEXON  11
#define SPI_BADEXONTHRESH  40

#define SPI_MAXSEQPORT  20000

#define SPI_SPLICETHRESH  0.0001

#define SPI_MAXGAP  4 /* maximum gap allowed in SPI_ExtendAlnAlg */

#define SPI_REVCOMPTHRESH  55 /* minimum allowed % of splice sites present */
                              /* If model is < minimum, then the reverse   */
                              /* complement will be checked.               */
#define SPI_COVERDIFF  15 /* amount the %coverage is allowed to drop in the */
                          /* reverse complement models                      */
#define SPI_MISMTCHDIFF  10 /* amount the %mismatch is allowed to rise in */
                            /* the reverse complement models              */

#define SPI_TEENYEXON  6 /* smallest exon to look for */

#define SPI_ENDFUZZ  8 /* if the overall alignment misses less than or equal */
                        /* to this amount on the ends of the mRNA, the        */
                        /* alignment will be extended.                        */

#define SPI_MINBLASTSIZE  7 /* smallest bit that can go into BlastTwoSequencesByLoc */

#define SPI_MINPOLYASIZE  5 /* minimum #A's to call a poly(A)+ tail */
#define SPI_MAXPOLYASIZE  200 /* maximum number of nucleotides to bother */
                              /* scanning for a tail                     */
#define SPI_LINKERSIZE  8 /* maximum number of non-A's to allow on end of tail */

#define SPI_INTRONSIZE  35000 /* used only to decide whether an mRNA may have fallen */
                              /* off a contig */
#define SPI_INTRONSIZEXL  220000 /* if spot->bigintron TRUE, use this */

#define SPI_BIGINTRON  100000 /* max size of 1st and last introns, if 1st and last exons */
                              /* have to be found by SPI_FindPiece. */
#define SPI_BIGINTRONXL  500000 /* if spot->bigintron TRUE, use this */

#define SPI_PADDING  0 /* how much each region is padded on each side */

#define SPI_NUMSITES  4 /* number of alternative splice sites to consider per exon */

#define SPI_BIGOVERLAP  12 /* above this cutoff, the overlap won't be doubled */
                           /* to get the window in which to search for splice sites */

#define SPI_EXONMERGESIZE  15 /* exons closer than this to each other will get merged */

#define SPI_FLUFF  16 /* amount to search on either side of splice site for interspecies comp. */

#define SPI_UNKNOWN     0
#define SPI_CONSISTENT  1
#define SPI_IMPOSSIBLE  2
#define SPI_DONE1       3
#define SPI_DONE2       4

#define SPI_FUZZ  20  /* amount of overlap/underlap allowed to consider hits consistent*/

#define SPI_LEFT     0
#define SPI_RIGHT    1
#define SPI_BOTH     2
#define SPI_NEITHER  3

#define SPI_REVERSEUNKNOWN  0
#define SPI_REVERSE         1
#define SPI_NOTREVERSED     2

#define SPI_MULT     1
#define SPI_NOTMULT  2

#define SPI_LINE 60 /* line length for text alignment output -- must be more than SPI_PSPLICE */
#define SPI_PSPLICE 10 /* length of genomic sequence to print before and after each exon */
#define SPI_SPACER 12 /* space at the beginning of each printed alignment line */

#define SPI_NUMCOLS  8 /* number of columns in the tab-delimited file of position info for draft */

/* defines for organisms (determines which splice matrices to use) */
#define SPI_VERTEBRATE  1
#define SPI_FLY         2
#define SPI_PLANT       3
#define SPI_CELEGANS    4
#define SPI_DICTY       5

/* return codes for progress callback */
#define SPI_START     1
#define SPI_PROGRESS  2
#define SPI_FINISHED  3

typedef struct spi_bsinfo {
   BioseqPtr  bsp;
   SeqLocPtr  lcaseloc;
   struct spi_bsinfo PNTR next;
} SPI_bsinfo, PNTR SPI_bsinfoPtr;

typedef struct spi_alninfo {
   FloatHi      bit_score;
   SeqAlignPtr  sap;
   Int2         used;
} SPI_AlnInfo, PNTR SPI_AlnInfoPtr;

typedef struct spi_ival {
   Int4         n;
   SeqAlignPtr  sap;
   Int4         gstart;
   Int4         gstop;
   Int4         mstart;
   Int4         mstop;
   Uint2        strand;
   Int2         used;
   Int4         score;
   struct spi_ival PNTR next;
} SPI_Ival, PNTR SPI_IvalPtr;

typedef struct spiexonprof {
   Int4     exonnum;
   Int4Ptr  mismatches;
   Int4     nummismatches;
   struct spiexonprof PNTR next;
} SPI_ExonProf, PNTR SPI_ExonProfPtr;

typedef struct spi_mrna {
   Uint1            strand;
   Boolean          revcomp;
   Int4             numexons;
   FloatHiPtr       exonid; /* percent identity per exon */
   Int4Ptr          exongaps;  /* number of gaps in each exon alignment */
   Uint1Ptr         splicedon;  /* for each exon, is splice donor site (right) present? */
   Uint1Ptr         spliceacc;  /* for each exon, is splice acceptor site (left) present? */
   Uint1            missingends;  /* SPI_LEFT, SPI_RIGHT, SPI_BOTH, or SPI_NEITHER */
   Int4Ptr          mstarts; /* exon starts, in mRNA coordinates */
   Int4Ptr          mstops;  /* exon stops, in mRNA coordinates */
   Int4Ptr          gstarts; /* exon starts, in genomic coordinates */
   Int4Ptr          gstops;  /* exon stops, in genomic coordinates */
   SeqAlignPtr      PNTR saps; /*indexed alignments for exons */
   Int4             mRNAcoverage; /* percentage of the mRNA contained in this alignment */
   FloatHi          mismatch; /* percent mismatches in entire alignment */
   Int4             polyAtail; /* if +, length of polyA tail that doesn't align */
                               /* if negative, length of polyAtail that does align */
   Boolean          fallsoff; /* does this mRNA fall of the end of the contig? */
   SeqAlignPtr      parent; /* parent of exon alignment set */
   SeqAlignPtr      continuous; /* continuous alignment over whole gene */
   SPI_ExonProfPtr  epp; /* positions of mismatches (for printing) */
   CharPtr          protein; /* sequence of the protein translated from the mRNA */
   Int4             transstart; /* translation start position */
   Boolean          holes; /* are there holes in the mRNA alignment? */
   struct spi_mrna PNTR next;
} SPI_mRNA, PNTR SPI_mRNAPtr;

typedef struct spi_utrinfo {
   FloatHi  left;
   FloatHi  right;
} SPI_UTRInfo, PNTR SPI_UTRInfoPtr;

typedef struct spi_seq {
   CharPtr    seq;
   Int4       start;
} SPI_Seq, PNTR SPI_SeqPtr;

typedef struct spi_mult {
   SeqAlignPtr  PNTR exons;
   Int4         numexons;
} SPI_Mult, PNTR SPI_MultPtr;

typedef struct spi_reginfo {
   Boolean      revcomp;
   Int4         gstart;
   Int4         gstop;
   Int4         mstart;
   Int4         mstop;
   Int4         mlen;
   Uint1        strand;
   SPI_mRNAPtr  smp;
   Int4         coverage;
   Int4         score;
   Int4         polyAtail; /* length of polyA(+) tail that doesn't align */
   Boolean      fallsoff; /* this mRNA may fall off the end of the genomic sequence */
   SPI_UTRInfo  utr; /* if this is a CDS, UTR %ids are here */
   SPI_MultPtr  smu;
   Boolean      revcmp_try;
   struct spi_reginfo PNTR next;
} SPI_RegionInfo, PNTR SPI_RegionInfoPtr;

typedef struct spi_tinyinfo {
   Int4  start;
   Int4  n;
   struct spi_tinyinfo PNTR next;
} SPI_TinyInfo, PNTR SPI_TinyInfoPtr;

typedef struct spi_pos {
   Int4  lgroup;
   Int4  group;
   Int4  order;
} SPI_Pos, PNTR SPI_PosPtr;

typedef struct spi_fragmentptr {
   Int4         start;
   Int4         stop;
   Int4         fragnum; /* original fragment number, for reference */
   SeqLocPtr    slp;
   SPI_PosPtr   position_orig;  /* the position info (if any) given for this fragment */
   Int4         position_mrna;  /* after alignment to mRNA, where is this fragment */
   Int4         reverse; /* is this fragment reversed with respect to the mRNA */
   SeqAlignPtr  sap;  /* indexed parent of the alignments for this fragment to the mRNA */
   SPI_mRNAPtr  smp; /* info for the alignment to this fragment */
   Uint1        donor;  /* does this (set of) alignment(s) have a donor site to the next frag? */
   Uint1        acceptor;  /* acceptor site ? */
   struct spi_fragmentptr PNTR next;
} SPI_Frag, PNTR SPI_FragPtr;

typedef struct spi_fragherd {
   Int4         polyAtail; /* length of polyAtail on the mRNA */
   Int4         numfrags;
   SPI_FragPtr  PNTR sfparray;
} SPI_FragHerd, PNTR SPI_FragHerdPtr;

typedef struct spi_fraginfo {
   Int4        mrnastart;
   Int4        mrnastop;
   Uint1       strand;
   Int4        sfpnum;
   Int4        fragnum;
   SPI_PosPtr  position_orig;
} SPI_FragInfo, PNTR SPI_FragInfoPtr;

typedef struct spi_mRNAtoherd {
   BioseqPtr        bsp_mrna; /* mrna sequence */
   Int4             numpieces;
   Int4             numexons;
   Int4Ptr          exons; /* array which specifies which pieces go with which exons */
   Int4Ptr          fragments; /* array of fragment numbers, one per piece */
   Int4Ptr          sfpnum; /* array of sfp numbers, one for each piece */
   Uint1Ptr         fallsoff; /* for each piece, does it fall off fragment? */
   Uint1Ptr         strands; /* genomic strand, one per piece */
   Int4Ptr          mstarts; /* mrna starts, one per piece */
   Int4Ptr          mstops;  /* mrna stops, one per piece */
   Int4Ptr          gstarts; /* genomic starts, one per piece */
   Int4Ptr          gstops; /* genomic stops, one per piece */
   Int4Ptr          lens; /* length of alignment for each piece */
   Int4Ptr          pmismatch; /* number of mismatches per piece */
   Int4Ptr          pgaps; /* number of gaps in alignment for each piece */
   FloatHiPtr       exonid; /* percent identity per exon */
   Int4Ptr          exongaps; /* number of gaps per exon */
   Uint1Ptr         splicedon;  /* for each piece, is splice donor site (right) present? */
   Uint1Ptr         spliceacc;  /* for each piece, is splice acceptor site (left) present? */
   Uint1            missingends;  /* SPI_LEFT, SPI_RIGHT, SPI_BOTH, or SPI_NEITHER */
   SeqAlignPtr      PNTR saps; /* indexed alignments, one for each piece (not exon) */
   FloatHi          mRNAcoverage; /* percentage of the mRNA contained in this alignment */
   Boolean          gaps; /* are there internal pieces missing from the mRNA alignment? */
   FloatHi          mismatch; /* percent mismatches in entire alignment */
   SPI_ExonProfPtr  epp;
   struct spi_mRNAtoherd PNTR next;
} SPI_mRNAToHerd, PNTR SPI_mRNAToHerdPtr;

typedef struct spi_exonherdinfo {
   Int4         fragmentnum;
   SeqAlignPtr  sap;
   Uint1        donor;
   Uint1        acceptor;
   Int4         sfpnum;
   struct spi_exonherdinfo PNTR next;
} SPI_ExonHerdInfo, PNTR SPI_ExonHerdInfoPtr;

typedef struct spi_splice {
   Int4     i;
   FloatHi  score;
   FloatHi  score2;
   Int4     diff;
} SPI_Splice, PNTR SPI_SplicePtr;

typedef struct spi_fragsplice {
   SPI_SplicePtr  splarray;
   Int4           spllen;
   Int4           boundary;
} SPI_FragSpl, PNTR SPI_FragSplPtr;

typedef struct spi_progress {
   Int4  percentdone;
   Int4  returncode;
} SPI_Progress, PNTR SPI_ProgressPtr;

typedef Boolean (LIBCALLBACK *SPI_ProgressCallback)(SPI_ProgressPtr progress);

typedef struct spi_spliceinfo {
   FloatHi  a;
   FloatHi  c;
   FloatHi  g;
   FloatHi  t;
   struct spi_spliceinfo PNTR next;
} SPI_SpliceInfo, PNTR SPI_SpliceInfoPtr;

typedef struct spi_options {
    FloatHi               firstpasseval;
    FloatHi               secpasseval;
    FloatHi               thirdpasseval;
    Int4                  organism;
    Int4                  numreturns;
    Int4                  idcutoff;
    Int4                  lencutoff;
    Int4                  printaln;
    Boolean               interspecies;
    Boolean               printasn;
    SeqAlignPtr           PNTR sap_head;
    CharPtr               draftfile;
    CharPtr               repeat_db_file; /* path to repeat db to use */
    SeqLocPtr             lcaseloc;
    Boolean               fetchcds;
    Boolean               printheader;
    Boolean               ace;
    SPI_ProgressCallback  callback;
    Int4                  from; /* to restrict genomic interval */
    Int4                  to;   /* " */
    Boolean               makemult; /* make a multiple alignment from numerous returns? */
    Boolean               bigintron; 
    Int4                  bigintron_size; /*added by KSK */
    Uint1                 strand; /* to restrict the search to one genomic strand */
    Boolean               revcomp;
    Int4                  dsplicejunc;
    SPI_SpliceInfoPtr     dssp_head;
    Int4                  asplicejunc;
    SPI_SpliceInfoPtr     assp_head;
} SPI_Options, PNTR SPI_OptionsPtr;
    
typedef struct spi_n {
   Int4  n1;
   Int4  n2;
   Int4  n3;
   Int4  n4;
   Int4  n5;
   Int4  n6;
} SPI_n, PNTR SPI_nPtr;

typedef struct spi_block {
   SeqAlignPtr  sap;
   Int4         from_g;
   Int4         to_g;
   struct spi_block PNTR next;
} SPI_Block, PNTR SPI_BlockPtr;

NLM_EXTERN SPI_RegionInfoPtr SPI_AlnSinglemRNAToGen(SPI_bsinfoPtr spig, SPI_bsinfoPtr spim, FILE *ofp, FILE *ofp2, SPI_OptionsPtr spot);
NLM_EXTERN SPI_mRNAToHerdPtr SPI_AlnSinglemRNAToPieces(SPI_bsinfoPtr spig_head, SPI_bsinfoPtr spim, FILE *ofp, FILE *ofp2, SPI_OptionsPtr spot);
NLM_EXTERN void SPI_MakeMultipleAlignment(SPI_RegionInfoPtr srip_head);
NLM_EXTERN void SPI_PrintMultipleAlignment(SPI_RegionInfoPtr srip, Boolean html, BioseqPtr bsp, FILE * ofp);
NLM_EXTERN void SPI_RegionListFree (SPI_RegionInfoPtr srip);

/*************************************************************************************
*  
*  SPI_AlnmRNAToGenomic is available to outside programs; just pass in the two
*  bioseqs and options (to use default options, just pass in NULL, and to use 
*  other options, call SPI_OptionsNew to get an initialized options pointer and
*  make the desired changes).  If options are passed in, they should be freed
*  using SPI_OptionsFree.  SPI_AlignmRNAToGenomic returns a linked list of
*  SPI_mRNAPtrs, one per gene model (default is to only return one gene model).
*  Each SPI_mRNAPtr (see above) has arrays specifying the exon boundaries in
*  genomic and mRNA coordinates as well as information about splice sites,
*  percent identity, number of gaps, etc.  The SPI_mRNAPtr also has one alignment
*  per exon as well as a single alignment (smp->continuous) that covers the entire
*  gene, with big gaps in the mRNA for the genomic introns.  The SPI_mRNAPtr should
*  be freed by the calling function, using SPI_mRNAFree.
*
*  SPI_AlnmRNAToGenomic should only be used on finished sequence; it can handle
*  interspecies comparisons but doesn't work on draft sequence.
*
*************************************************************************************/
NLM_EXTERN SPI_mRNAPtr SPI_AlignmRNAToGenomic(BioseqPtr bsp_genomic, BioseqPtr bsp_mrna, SPI_OptionsPtr spot);

/***************************************************************************
*
*  SPI_flip_sa_list takes the head of a list of seqaligns and switches
*  the first and second row of every alignment (alignments should all have
*  two rows). Then, the indexes are freed and the alignments are reindexed.
*
***************************************************************************/
NLM_EXTERN void SPI_flip_sa_list (SeqAlignPtr sap);

/***************************************************************************
*
*  SPI_RemoveInconsistentAlnsFromSet is a greedy algorithm that first
*  sorts the alignments by score, then takes the highest-scoring
*  alignment and compares it to the next-highest-scoring alignment, which
*  is deleted if it is contained; on subsequent loops each next-highest-
*  scoring alignment is compared to the set of alignments that have
*  been kept. The alignments can be sorted along the first or
*  second sequence; the alignments will be reversed so that they are
*  all on the plus strand of the sequence to be examined.
*  The input alignment must be indexed at least at the LITE level;
*  conflicting child alignments will be deleted, not hidden, by this
*  function.  This function assumes that all children have the same two
*  rows. The 'compact' parameter tells the function whether to try to
*  keep alignments that are more to the left in genomic coordinates, or
*  more to the right.
*
***************************************************************************/
NLM_EXTERN void SPI_RemoveInconsistentAlnsFromSet(SeqAlignPtr sap, Int4 fuzz, Int4 n, Int4 compact);

NLM_EXTERN void SPI_bsinfoFreeList (SPI_bsinfoPtr spi);
NLM_EXTERN void SPI_mRNAFree (SPI_mRNAPtr smp);
NLM_EXTERN SPI_OptionsPtr SPI_OptionsNew(void);
NLM_EXTERN void SPI_OptionsFree (SPI_OptionsPtr spot);
NLM_EXTERN void SPI_is_donor (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score, Int4 org);
NLM_EXTERN void SPI_is_acceptor (Uint1Ptr sequence, Int4 seqlen, FloatHiPtr score, Int4 org);

/***************************************************************************
*
*  SPI_GetProteinFrommRNA takes an mRNA bioseq and returns a string
*  which is the best protein translation of the mRNA. First, the function
*  looks to see whether there are any annotated CDSs, and if so, it uses
*  the translation of the annotated CDS. If not, the function translates
*  the mRNA in all 3 reading frames and looks for the frame with the
*  longest protein, then returns that protein.
*
***************************************************************************/
NLM_EXTERN CharPtr SPI_GetProteinFrommRNA(BioseqPtr bsp_mrna, Int4Ptr start);

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
