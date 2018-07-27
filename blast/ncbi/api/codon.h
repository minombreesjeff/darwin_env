/* $Id: codon.h,v 6.1 1997/11/06 21:26:11 madden Exp $
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
* File Name:  $RCSfile: codon.h,v $
*
* Author:  Jinghui Zhang
*
* Initial Version Creation Date: 03/24/97
*
* $Revision: 6.1 $
*
* File Description:
*         
*
* $Log: codon.h,v $
* Revision 6.1  1997/11/06 21:26:11  madden
* Added print_label_to_buffer_all_ex with option to strip semicolons
*
* Revision 6.0  1997/08/25 18:05:21  madden
* Revision changed to 6.0
*
* Revision 5.6  1997/07/01 15:15:49  zjing
* fix the problem of label overflow
*
* Revision 5.5  1997/06/19 18:37:28  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.4  1997/03/24 21:00:07  shavirin
* Added protection for usage with C++ compiler. Added NCBI header
* and started logging
*
*
* ==========================================================================
*/

#ifndef _CODON_
#define _CODON_

#include <ncbi.h>
#include <objfeat.h>
#include <seqport.h>
#include <seqmgr.h>
#include <jzmisc.h> 
#include <gather.h>
#include <satutil.h>

typedef struct codonvector{
	Uint2 exonCount;	/*current count of exons*/
	CharPtr buf[3];
	Int4 dna_pos;		/*position in dna sequence*/
	SeqIdPtr sip;		/*id for the DNA sequence for label*/
	Uint1 strand;		/*orientation of CDs*/
	Uint1 frame;		/*the frame of current exon*/
	Int2 aa_index;		/*0 for partial start , 1 for normal*/
}CodonVector, PNTR CodonVectorPtr;


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/******************************************************************
*
*       aa_to_codon(sfp, aa_start, aa_stop)
*       generate a list of CodonVecotr to show the codons of an
*       amino acid sequence
*       sfp: the Seq-feat for cds
*       aa_start: the start position of protein sequence
*       aa_stop the stop position of protein sequence
*
******************************************************************/
NLM_EXTERN ValNodePtr aa_to_codon PROTO((SeqFeatPtr sfp, Int4 aa_start, Int4 aa_stop));


/****************************************************************** 
* 
*       free_cvp_list(cvp_list)
*       free a list of CodonVectorPtr 
* 
******************************************************************/ 
NLM_EXTERN ValNodePtr free_cvp_list PROTO((ValNodePtr cvp_list));

/*********************************************************************
*
*	make_cds_paragraph(sfp, aa_start, aa_stop)
*	return a buffer for the display of 3-codon under one amino 
*	acid format. It also includes the new line characters 
*	This is what Jonathan K. desires to have for the sequin 
*	doc object
*	aa_start, aa_stop: start and stop in the amino acid sequence
*
*********************************************************************/
NLM_EXTERN CharPtr make_cds_paragraph PROTO((SeqFeatPtr sfp, Int4 aa_start, Int4 aa_stop));




/******************************************************************
*
*	aa_to_dnaloc(sfp, aa_start, aa_stop)
*	map the amino acid sequence to a list of Seq-locs in the 
*	DNA sequence
*
******************************************************************/
NLM_EXTERN SeqLocPtr aa_to_dnaloc PROTO((SeqFeatPtr sfp, Int4 aa_start, Int4 aa_stop));

NLM_EXTERN Int4 print_protein_for_cds PROTO((SeqFeatPtr sfp, CharPtr buf, SeqLocPtr loc, Boolean reverse_minus));

#define POS_SPACE 10	/*empty space used to write the position*/
#define STRAND_SPACE 3	/*empty space used to laod the orientation*/
#define B_SPACE 10

/******************************************************************** 
* 
*       print_label_to_buffer(buf, label, pos, strand, extra_space) 
*
*       print a label (with label_name=label, position=pos, 
*       orientation = strand, extra_space = extra_space for partial start) 
*       into the current buffer 
*       return the offset of the buffer pointer to the current buffer 
* 
*********************************************************************/
 
NLM_EXTERN Int4 print_label_to_buffer PROTO((CharPtr buf, CharPtr label, Int4 pos, 
	Uint1 strand, Boolean extra_space, Boolean is_html, Int4 label_space, 
	Int4 num_space));

NLM_EXTERN void print_label PROTO((FILE *fp, CharPtr label, Int4 pos, Uint1 strand, Boolean extra_space));

/********************************************************************
*
*	print_label_to_buffer(buf, label, pos, strand, extra_space)
*
*	print a label (with label_name=label, position=pos, 
*	orientation = strand, extra_space = extra_space for partial start)
*	into the current buffer
*	return the offset of the buffer pointer to the current buffer
*
*********************************************************************/
NLM_EXTERN Int4 print_label_to_buffer_all PROTO((CharPtr buf, CharPtr label, 
		Int4 pos, Uint1 strand, Boolean extra_space, Boolean is_html, 
		Int4 label_space, Int4 num_space, Boolean show_strand));

/********************************************************************
*
*	print_label_to_buffer_all_ex(buf, label, pos, strand, extra_space)
*
*	print a label (with label_name=label, position=pos, 
*	orientation = strand, extra_space = extra_space for partial start)
*	into the current buffer
*	return the offset of the buffer pointer to the current buffer
*	Has an option to strip/not-strip semicolons for hardline old blast
*	users.
*
*********************************************************************/
NLM_EXTERN Int4 print_label_to_buffer_all_ex PROTO((CharPtr buf, CharPtr label, 
		Int4 pos, Uint1 strand, Boolean extra_space, Boolean is_html, 
		Int4 label_space, Int4 num_space, Boolean show_strand, 
		Boolean strip_semicolon));

NLM_EXTERN Boolean make_scale_bar_str PROTO((CharPtr PNTR bar, CharPtr PNTR num_str, 
		Int4 num_empty, Int4 line_len));

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
