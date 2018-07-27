/* $Id: txalign.h,v 6.7 2001/07/23 20:20:12 dondosha Exp $
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
* File Name:  $RCSfile: txalign.h,v $
*
* Author:  Jinghui Zhang
*
* Initial Version Creation Date: 03/13/94
*
* $Revision: 6.7 $
*
* File Description:
*         External include file for various alignments
* Revision 5.13  1997/06/05 20:55:34  madden
 * Added PrintDefLinesFromSeqAlign prototype
*
*
* $Log: txalign.h,v $
* Revision 6.7  2001/07/23 20:20:12  dondosha
* Made replace_bytestore_data function public for use in web blast2seq
*
* Revision 6.6  2001/06/21 19:42:18  shavirin
* Moved here definitions related to Taxonomy names.
*
* Revision 6.5  2001/06/21 18:26:27  shavirin
* Moved here functions to get Taxonomy names information encoded in
* the Bioseq returned from the Blast database.
*
* Revision 6.4  2001/05/15 17:18:26  egorov
* Added txalign_options to AlignStatOption structure
*
* Revision 6.3  2001/03/23 17:24:44  madden
* Add FDGetDeflineAsnFromBioseq from readdb.[ch]
*
* Revision 6.2  2000/12/14 17:08:53  shavirin
* Added additinal label "<name=" for the single alignment. This link will
* be shown only in Human Genome viewer.
*
* Revision 6.1  2000/11/16 22:20:16  shavirin
* File moved to distrib/tools from distrib/api .
*
* Revision 6.28  2000/10/06 17:55:46  shavirin
* Added usage of correct matrix in OOF case.
*
* Revision 6.27  2000/07/11 20:51:06  shavirin
* Added major functions for displaying Out-Of-Frame alignments.
*
* Revision 6.26  2000/07/10 20:45:54  shavirin
* Added parameter ooframe for Out-Of-frame alignment and corresponding changes
* to accomodate this parameter.
*
* Revision 6.25  2000/06/09 19:00:06  shavirin
* Function GetGeneticCodeFromSeqId() made external and added to header file.
*
* Revision 6.24  2000/06/08 20:44:50  shavirin
* Added calculation of start/stop values in the function find_score_in_align().
*
* Revision 6.23  2000/03/07 21:58:41  shavirin
* Now will use PSSM Matrix to show positives in PSI Blast
*
* Revision 6.22  1999/11/24 21:24:33  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* Revision 6.21  1999/11/09 22:15:08  shavirin
* Added parameter follower to the Blast score printing function
*
* Revision 6.20  1999/10/07 16:08:05  shavirin
* Passed matrix to the function FormatScoreFromSeqAlign().
*
* Revision 6.19  1999/09/29 17:15:38  shavirin
* Added new funtion FormatScoreFromSeqAlign()
*
* Revision 6.18  1999/06/07 18:43:17  madden
* added TXALIGN_NO_DUMPGNL if dumpgnl is not desired
*
* Revision 6.17  1999/04/15 20:57:23  madden
* overview printing for vector stuff
*
* Revision 6.16  1999/04/06 15:13:25  madden
* Add support for non-gnl queries with dumpgnl syntax
*
* Revision 6.15  1999/02/26 21:28:06  victorov
* taking different sections of config file depending on WWW_BLAST_TYPE
*
* Revision 6.14  1999/02/19 20:51:07  victorov
* changed URL to the tool reporting incomplete
* sequences. URL now includes starts/stops for all hits
*
* Revision 6.13  1999/01/13 21:52:43  victorov
* added links to incomplete genomes in hit details
*
* Revision 6.12  1999/01/06 22:51:00  victorov
* added hyperlinks for incomplete sequences
*
* Revision 6.11  1999/01/05 14:52:04  madden
* Add frame and strand information
*
* Revision 6.10  1998/11/09 19:06:47  vakatov
* Added "NLM_EXTERN" to the ShowTextAlignFromAnnotExtra() prototype
*
* Revision 6.9  1998/09/01 13:27:02  madden
* PrintDefLinesExtra function
*
* Revision 6.8  1998/08/26 21:32:23  madden
* Added ShowTextAlignFromAnnotExtra for PHI-BLAST
*
* Revision 6.7  1998/07/23 13:35:29  egorov
* Allow print specified number of descriptions in PrintDefLinesFromSeqAlign()
*
* Revision 6.6  1998/07/02 21:22:56  madden
* Changes for random-access BLAST
*
* Revision 6.5  1998/03/25 22:38:50  egorov
* Change prototypes for PrintDefLinesFromAnnot and PrintDefLinesFromSeqAlign
*
* Revision 6.4  1997/10/06 14:01:11  zjing
* move TxGetSubjectId, GetScoreAndEvalue to sequtil.ch
*
* Revision 6.3  1997/09/25 17:17:37  zjing
* Add the option for showing blunt-end alignment
*
* Revision 6.2  1997/09/25 02:00:27  vakatov
* Added NLM_EXTERN specifier to some functions(necessary for MS-Win DLLs)
*
* Revision 6.1  1997/09/18 22:24:23  madden
* Made TxGetSubjectIdFromSeqAlign public
*
* Revision 6.0  1997/08/25 18:08:14  madden
* Revision changed to 6.0
*
* Revision 5.20  1997/08/14 17:55:49  zjing
* minor changes
*
* Revision 5.18  1997/07/28 13:55:46  madden
* Added mask_loc to prototypes.
*
* Revision 5.17  1997/07/11 15:28:13  madden
* Added TXALIGN_HTML_RELATIVE define
*
* Revision 5.16  1997/07/07 20:22:26  madden
* changes to show the results as query-subect
*
* Revision 5.15  1997/06/19 18:39:42  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.14  1997/06/09 21:47:25  madden
* Added Boolean follower to AlignStatOptions
*
 * Revision 5.12  1997/03/13  21:43:13  shavirin
 * Added protection for C++ compiler
 *
*
* ==========================================================================
*/
#ifndef _TXALIGN_
#define _TXALIGN_

/****************************************************************************/
/* INCLUDES */
/****************************************************************************/

#include <jzcoll.h>
#include <ffprint.h>
#include <fdlobj.h>

/****************************************************************************/
/* DEFINES */
/****************************************************************************/

#define WEBB_asize 23		/* webb's matrix */
#define TX_MATRIX_SIZE 128	/*size of the matrix for showing the 
                                  text alignment*/

#define TXALIGN_LOCUS_NAME	((Uint4)256)	/*display the locus name*/
#define TXALIGN_MASTER		((Uint4)2)	/*display the alignment as multiple pairwise alignment*/
#define TXALIGN_MISMATCH	((Uint4)4)	/*display the mismatched residue of the sequence */
#define TXALIGN_MATRIX_VAL	((Uint4)8)	/*display the matrix of the alignment */
#define TXALIGN_HTML		((Uint4)16)	/*display the format in a HTML page*/
#define TXALIGN_HTML_RELATIVE	((Uint4)8192)	/*the HTML (if enabled by TXALIGN_HTML) should be relative*/
#define TXALIGN_SHOW_RULER	((Uint4)32)	/*display the ruler for the text alignment*/
#define TXALIGN_COMPRESS	((Uint4)64)	/*make the space for label smaller*/
#define TXALIGN_END_NUM		((Uint4)128)	/*show the number at the end */
#define TXALIGN_FLAT_INS	((Uint4)1)	/*flat the insertions in multiple pairwise alignment */
#define TXALIGN_SHOW_GI         ((Uint4)512)    /*show the gi in the defline. */
#define TXALIGN_SHOW_NO_OF_SEGS ((Uint4)1024)    /*show the number of (sum statistics) segments in the one-line descriptions? */

#define TXALIGN_BLASTX_SPECIAL  ((Uint4)2048)	/*display the BLASTX results 
						  as protein alignment */
#define	TXALIGN_SHOW_QS		((Uint4)4096)	/*show the results as query-subect*/
#define TXALIGN_SPLIT_ANNOT	((Uint4)16384)	/*for Seq-annot from the same alignment, split the 
												the display into individual panel*/
#define TXALIGN_SHOW_STRAND	((Uint4)32768)	/*for displaying the stradn even in the compact form*/
#define TXALIGN_BLUNT_END	((Uint4)65536)	/*showing the blunt-end for the end gaps*/
#define TXALIGN_DO_NOT_PRINT_TITLE	((Uint4)131072)	/* do not print title before list of deflines */
#define TXALIGN_CHECK_BOX	((Uint4)262144)	/* place checkbox before the line (HTML only) */
#define TXALIGN_CHECK_BOX_CHECKED	((Uint4)524288)	/* make default value for checkboxes ON (HTML only) */
#define TXALIGN_NEW_GIF		((Uint4)1048576)	/* print new.gif near new alignments (HTML only) */
#define TXALIGN_NO_ENTREZ	((Uint4)2097152)	/* Use dumpgnl syntax instead of ENTREZ. */
#define TXALIGN_NO_DUMPGNL	((Uint4)4194304)	/* No dumpgnl output, even if GNL. */
#define TXALIGN_TARGET_IN_LINKS	((Uint4)8388608)	/* Put TARGET in Entrez links */
/*
	Used by psi-blast to distinguish first from subsequent passes.
*/

#define FIRST_PASS 1
#define NOT_FIRST_PASS_REPEATS 2
#define NOT_FIRST_PASS_NEW  3

#define ASN_DEFLINE_OBJ_LABEL "ASN1_BlastDefLine"
#define TAX_DATA_OBJ_LABEL    "TaxNamesData"

/* Bit meanings in membership element of ASN.1 structured 
   definition lines */
#define EST_HUMAN_BIT 0x1
#define EST_MOUSE_BIT 0x2
#define SWISSPROT_BIT 0x4
#define PDB_BIT       0x8
#define REFSEQ_BIT    0x10
#define CONTIG_BIT    0x20

#define NUM_TAX_NAMES 4
#define SCI_NAME_POS    0
#define COMMON_NAME_POS 1
#define BLAST_NAME_POS  2
#define S_KING_POS      3

/* ---------------------------------------------------------------------*/
/* -- Here is set of definitions used with taxonomy info database ----- */
/* ---------------------------------------------------------------------*/
typedef	struct _RDBTaxNames {
    Int4 tax_id;
    CharPtr sci_name;
    CharPtr common_name;
    CharPtr blast_name;
    Char  s_king[3];
} RDBTaxNames, *RDBTaxNamesPtr;

/****************************************************************************/
/* TYPEDEFS */
/****************************************************************************/

typedef struct text_buf{	/*for a generic feature comment*/
	Int4 pos;	        /*position for label*/
	Uint1 strand;	        /*the orientation*/
	CharPtr label;	        /*label for the feature*/
	CharPtr buf;	   /*the buffer for features other than cds for aa*/
	Int2Ptr matrix_val;	/*the value of each residue from the matrix */
	CharPtr codon[3];	/*for features such as cds for aa*/
	Int2 frame;	        /*for cds for feature*/
	Int4 f_pos;	        /*position of the current buf*/
	Uint2 exonCount;	/*count the number of exons, useded in 
                                  cds for aa*/
        Uint2 itemID;	/*feature's itemID. It is used to check identity*/
	Uint2 feattype;
	Uint2 subtype;
	Uint2 entityID;
	Uint2 seqEntityID;	/*the entityID for the sequence*/
	Uint2 bsp_itemID;	/*itemID for the Bioseqs*/
	Boolean extra_space;
}TextAlignBuf, PNTR TextAlignBufPtr;

typedef struct align_summary {
    Int4 positive;	        /*number of positive residues*/
    Int4 identical;	        /*number of identical residues*/
    Int4 gaps;		        /*number of the gaps*/
    Int4 totlen;	        /*total length of the alignemtns*/
    Int4Ptr PNTR matrix;	/*matrix for protein alignments*/
    Int4Ptr PNTR posMatrix;	/*matrix for PSSM protein alignments*/
    SeqIdPtr master_sip;	/*the Seq-id of the master sequence*/
    SeqIdPtr target_sip;	/*the Seq-id for the target sequence*/
    Boolean is_aa;              /*are the sequences nucleotide or protein?*/
    Uint1 m_strand,	        /* strand of the query. */
          t_strand;	        /* strand of the database sequence. */
    Int4  m_frame,	        /* Frame of the query. */
        t_frame;	        /* Frame of the database sequence. */
    Boolean	m_frame_set,	/* query frame was set. */
        t_frame_set;	        /* database sequence frame was set. */
    Int4 master_from;           /* from for master sequence */
    Int4 master_to;             /* to for master sequence */
    Int4 target_from;           /* from for target sequence */
    Int4 target_to;             /* to region for master sequence */
    Boolean ooframe;            /* Is this out-of-frame alignment ? */
}AlignSum, PNTR AlignSumPtr;

typedef struct align_stat_option { /*options for printing the statistics*/
    Int2 line_len;
    Int2 indent_len;
    Boolean html_hot_link;			/* Prepare HTML output. */
    Boolean html_hot_link_relative;		/* Make the HTML link relative. */
    Boolean show_gi;
    Boolean no_entrez;			/* Do not use Entrez format for HTML links. */
    Boolean no_dumpgnl;			/* Do not use dumpgnl format even if GNL. */
    FILE *fp;
    CharPtr buf;
    BioseqPtr bsp;
    ScorePtr sp;
    Int4 identical;             /*number of identical residues*/
    Int4 gaps;		        /*number of the gaps*/
    Int4 positive;	        /*number of the positive residues*/
    Int4 align_len;	        /*the length of the alignment. EXCLUDE the GAPS*/
    Boolean follower;           /* If TRUE, this is NOT the first alignment for this sequences. */
    Uint1 	m_strand,	/* strand of the query. */
        t_strand;	        /* strand of the database sequence. */
    Int2	m_frame,	/* Frame of the query. */
        t_frame;	        /* Frame of the database sequence. */

    /* This information was added first only for creation of very
       specific links to the single alignment. However - may be it will
       be used later for something else */

    Int4 master_from;           /* from for master sequence */
    Int4 master_to;             /* to for master sequence */
    Int4 target_from;           /* from for target sequence */
    Int4 target_to;             /* to region for master sequence */

    CharPtr segs; /* <start> "-" <stop> ("," <start> "-" <stop>)* */
    CharPtr db_name; /* searched databases list */
    CharPtr blast_type; /* string used to choose proper config parms */
    Uint4	txalign_options;/* the TXALIGN_* options */
}AlignStatOption, PNTR AlignStatOptionPtr;

/****************************************************************************/
/* FINCTION DEFINITIONS */
/****************************************************************************/

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************
*
*	find_score_in_align(align, chain, asp)
*	align: the Seq-align point
*	chain: for multiple segment Seq-aligns, such as DenseDiag and 
*	StdSeg, the order within the Seq-align
*	asp:   the structure that records and stores the positive, 
*			identical residues
*	the function only works for DenseDiag and Stdseg for now
*
*****************************************************************/
NLM_EXTERN ScorePtr find_score_in_align PROTO((SeqAlignPtr align, 
                                               Uint2 chain, AlignSumPtr asp));

/*the default formatting function for printing the scores*/

NLM_EXTERN int LIBCALLBACK FormatScoreFunc PROTO((AlignStatOptionPtr asop));

/**********************************************************************************
*
*       Given a chain of annots (ValNodePtrs) they are all printed out, one pattern
*       at a time.
*
*************************************************************************************/

NLM_EXTERN Boolean LIBCALL ShowTextAlignFromAnnotExtra PROTO((BioseqPtr bsp, ValNodePtr vnp, SeqLocPtr seqloc,
        Int4 line_len, FILE *fp,
        Uint1Ptr featureOrder, Uint1Ptr groupOrder, Uint4 option, Int4Ptr PNTR matrix,
        ValNodePtr mask_loc, int (LIBCALLBACK *fmt_score_func)PROTO((AlignStatOptionPtr))));

/*****************************************************************************
*
*	ShowTextAlignFromAnnot(annot, locus, line_len, fp, master, f_order)
*	display the alignment stored in a Seq-annot in a text file
*	annot: the Seq-annot pointer
*	locus: if TRUE, show the locus name as the sequence label, otherwise, 
*		use the accession
*	line_len: the number of sequence char per line
*	fp: The file pointer to store the text output
*	master: if TRUE, show the result as a master-slave type multiple pair
*	wise alignment. if FALSE, display one alignment after the other
*	f_order: the user selected feature type and order to be shown together
*	with the alignment
*	return TRUE for success, FALSE for fail
*
*****************************************************************************/
NLM_EXTERN Boolean ShowTextAlignFromAnnot PROTO((
                    SeqAnnotPtr hannot, Int4 line_len, 
                    FILE *fp, Uint1Ptr featureOrder, 
                    Uint1Ptr groupOrder, Uint4 option, 
                    Int4Ptr PNTR matrix, ValNodePtr mask_loc,
                    int (LIBCALLBACK *fmt_score_func)
                    PROTO((AlignStatOptionPtr))
                    ));
/**
 * same as ShowTextAlignFromAnnot
 * the db_name argument is used to make links to
 * incomplete genomes
 */
NLM_EXTERN Boolean ShowTextAlignFromAnnot2 PROTO((
                    SeqAnnotPtr hannot, Int4 line_len, 
                    FILE *fp, Uint1Ptr featureOrder, 
                    Uint1Ptr groupOrder, Uint4 option, 
                    Int4Ptr PNTR matrix, ValNodePtr mask_loc,
                    int (LIBCALLBACK *fmt_score_func)
                    PROTO((AlignStatOptionPtr)),
                    CharPtr db_name,
                    CharPtr blast_type
                    ));
/**
 * same as ShowTextAlignFromAnnot
 * the posMatrix used to show alignments using PSSM
 */
NLM_EXTERN Boolean ShowTextAlignFromAnnot3 PROTO((
                    SeqAnnotPtr hannot, Int4 line_len, 
                    FILE *fp, Uint1Ptr featureOrder, 
                    Uint1Ptr groupOrder, Uint4 option, 
                    Int4Ptr PNTR matrix, ValNodePtr mask_loc,
                    int (LIBCALLBACK *fmt_score_func)
                    PROTO((AlignStatOptionPtr)),
                    CharPtr db_name,
                    CharPtr blast_type,
                    Int4Ptr PNTR posMatrix
                    ));


/* Simple printing function:
   Can be used while debugging.. options kept to a minimum 
   fp==NULL ==> stdout 
*/
NLM_EXTERN void LIBCALL SeqAlignPrint(SeqAlignPtr salp,FILE* fp);

/***********************************************************************
*
*	ShowAlignNodeText(anp_list, num_node, line_len, locus,
*	fp)
*	convert the alignment data in the list of AlignNode into text written
*	to a file
*	anp_list: a list (ValNodePtr) of AlignNode processed from Seq-aligns
*	num_node: the number of AlignNode to be processed currently. It can
*	be used in the cases where only the top num_node in the anp_list is 
*	going to be processed. This can be useful to make vertically cashed
*	buffer
*	line_len: the length of sequence char per line
*	locus: if TRUE, show the locus name
*	fp: the file Pointer
*	left: the leftmost position for display
*	right: the rightmost position for display
*	align_type:	the type of alignment. DNA-protein alignment?
*
*	return TRUE for success, FALSE for fail
*
************************************************************************/

NLM_EXTERN Boolean ShowAlignNodeText PROTO((
                     ValNodePtr anp_list, Int2 num_node, 
                     Int4 line_len, FILE *fp, Int4 left, 
                     Int4 right, Uint4 option, 
                     Int4Ptr PNTR u_matrix, 
                     int (LIBCALLBACK *fmt_score_func)
                     PROTO((AlignStatOptionPtr))
                     ));

NLM_EXTERN Boolean ShowAlignNodeText2 PROTO((
                     ValNodePtr anp_list, Int2 num_node, 
                     Int4 line_len, FILE *fp, Int4 left, 
                     Int4 right, Uint4 option, 
                     Int4Ptr PNTR u_matrix, 
                     int (LIBCALLBACK *fmt_score_func)
                     PROTO((AlignStatOptionPtr)),
                     CharPtr db_name,
                     CharPtr blast_type,
                     Int4Ptr PNTR posMatrix
                     ));

/*
  Print a summary of the Sequences producing significant alignments.
*/

NLM_EXTERN Boolean LIBCALL PrintDefLinesExtra PROTO((
		ValNodePtr vnp, Int4 line_length, FILE *outfp, Uint4 options, 
		Int4 mode, Int2Ptr marks, SeqLocPtr seqloc));


NLM_EXTERN Boolean LIBCALL PrintDefLinesFromAnnot PROTO((
                    SeqAnnotPtr seqannot, 
                    Int4 line_length, FILE *fp, 
                    Uint4 options, Int4 mode, 
                    Int2Ptr marks
                    ));

NLM_EXTERN Boolean LIBCALL PrintDefLinesFromSeqAlign PROTO((
                    SeqAlignPtr seqalign, 
                    Int4 line_length, FILE *fp, 
                    Uint4 options, Int4 mode, 
                    Int2Ptr marks
                    ));

NLM_EXTERN Boolean LIBCALL PrintDefLinesFromSeqAlignEx PROTO((
		    SeqAlignPtr seqalign, 
		    Int4 line_length, 
		    FILE *outfp, 
		    Uint4 options, 
		    Int4 mode, 
		    Int2Ptr marks, 
		    Int4 number_of_descriptions
		    ));

NLM_EXTERN Boolean LIBCALL PrintDefLinesFromSeqAlignEx2 PROTO((
		    SeqAlignPtr seqalign, 
		    Int4 line_length, 
		    FILE *outfp, 
		    Uint4 options, 
		    Int4 mode, 
		    Int2Ptr marks, 
		    Int4 number_of_descriptions,
		    CharPtr db_name,
		    CharPtr blast_type
		    ));

/*
	Fills in the slots with score, bit_score, etc. from the SeqAlign.
*/

/*options for display of the text alignment*/
#define TEXT_MP_MISMATCH	1	/*multiple pairwise alignment with mismatch*/
#define TEXT_MP			2	/*multiple pairwise without mismatch*/
#define TEXT_MPFLAT_MISMATCH	3	/*flat multile with mismatch*/
#define TEXT_MPFLAT		4	/*flat multiple without mismatch*/
#define TEXT_BLAST		5	/*traditional blast output*/


/*can the current alignnode be printed for text view*/
NLM_EXTERN Boolean PrintAlignForText PROTO((AnnotInfoPtr info, AlignNodePtr anp));

/*
*
*	determine the option for alignment based on the named tx_option
*
*/
NLM_EXTERN Uint4 GetTxAlignOptionValue PROTO((Uint1 tx_option, BoolPtr hide_feature, 
	BoolPtr print_score, BoolPtr split_display));

/*
        Gets the SeqIdPtr for the subject sequence from the first SeqAlign.
        The SeqIdPtr is not saved  and should not be deleted.
*/

/* Marks structure is used for PSI Blast to print .gif marsk 
   near alignments and to check for convergence */

#define	SEQ_ALIGN_MARK_PREVGOOD		1
#define	SEQ_ALIGN_MARK_PREVCHECKED	2
/* the following serves only for old stuff which uses posRepeat... */
#define	SEQ_ALIGN_MARK_REPEAT		4


typedef struct MarkSeqAlign {
    Int4		kind;	/* bitmask for the mark */
    struct MarkSeqAlign	*next;
} MarkSeqAlign, PNTR MarkSeqAlignPtr;


NLM_EXTERN Boolean LIBCALL FilterTheDefline PROTO((BioseqPtr bsp, SeqIdPtr gi_list_head, CharPtr buffer_id, Int4 buffer_id_length, CharPtr PNTR titlepp));

NLM_EXTERN Boolean FormatScoreFromSeqAlign
(SeqAlignPtr sap, Uint4 option, FILE *fp,
Int4Ptr PNTR matrix, Boolean follower);    

NLM_EXTERN SeqFeatPtr make_fake_cds(BioseqPtr m_bsp, Int4 start, Int4 stop, Uint1 strand);

/*
  Obtains the genetic code from a BioseqPtr, assuming that a fetch function
  has been enabled.
*/
NLM_EXTERN CharPtr GetGeneticCodeFromSeqId (SeqIdPtr sip);

/* 
   Translate DNA sequence in all frames and create protein
   sequence for Out-Of-Frame gap algorithm 
*/
NLM_EXTERN CharPtr OOFTranslateDNAInAllFrames(Uint1Ptr dna, Int4 length, 
                                              SeqIdPtr query_id);

/*************************************************************************

  Function : OOFShowBlastAlignment();
  
  Purpose : function to display a BLAST output with Out-of-Frame
            information
  
  Parameters : 	sap; seqalign
                mask; list of masked regions in the query
                fp; output file;
                tx_option; some display options
				
  Return value : FALSE if failure

***************************************************************************/
NLM_EXTERN Boolean OOFShowBlastAlignment(SeqAlignPtr sap, ValNodePtr mask,
                                         FILE *fp, Uint4 tx_option, 
                                         Int4Ptr PNTR matrix);
/* 
   Test functions to display Out-of-Frame traceback 
*/
NLM_EXTERN void OOFDisplayTraceBack1(Int4Ptr a, CharPtr dna, 
                                     CharPtr pro, Int4 ld, Int4 lp, 
                                     Int4 q_start, Int4 p_start);
NLM_EXTERN void OOFDisplayTraceBack2(Int4Ptr a, CharPtr dna, CharPtr pro, 
                                     Int4 ld, Int4 lp, 
                                     Int4 q_start, Int4 p_start);

BlastDefLinePtr FDGetDeflineAsnFromBioseq(BioseqPtr bsp);
RDBTaxNamesPtr FDGetTaxNamesFromBioseq(BioseqPtr bsp, Int4 taxid);

NLM_EXTERN Boolean replace_bytestore_data PROTO((BioseqPtr bsp, ValNodePtr bs_list, Uint1 frame));

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
