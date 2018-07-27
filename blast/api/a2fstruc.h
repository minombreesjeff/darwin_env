/*   a2fstruc.h
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
* File Name:  a2fstruc.h
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
*
* Version Creation Date:   7/15/95
*
* $Revision: 6.0 $
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

/*********************************************************************
*
*	Structures for asn2ff and makerpt
*
**********************************************************************/

/*************************************
*
* $Log: a2fstruc.h,v $
* Revision 6.0  1997/08/25 18:04:30  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.0  1995/07/26  13:49:01  ostell
 * force revision to 4.0
 *
 * Revision 1.11  1995/07/17  19:33:20  kans
 * parameters combined into Asn2ffJobPtr structure
 *
 * Revision 1.10  1995/06/19  21:40:02  kans
 * Tatiana's first major reorganization, moving printing, adding HTML
 *
 * Revision 1.9  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*************************************/

#ifndef _A2FSTRUC_
#define _A2FSTRUC_

#include <asn.h>
#include <objall.h>
#include <objpubd.h>
#include <objsub.h>
#include <prtutil.h>
#include <asn2ffg.h>

#define LINKS 20


/*--------------the structure for information on Genes-----*/

typedef struct genestruct {
	ValNodePtr gene;
	ValNodePtr product;
	ValNodePtr standard_name;
	CharPtr PNTR map;         /* only map[0] is used why we need PNTR? */
	ValNodePtr ECNum;
	Int2 map_size, map_index;   /* map_size is always 1 */
	Int2 matchlist_size;
	Int2Ptr genes_matched;
	Boolean pseudo;
} GeneStruct, PNTR GeneStructPtr;

/*--------------the structure for the composition of notes-----*/

typedef struct notestruct {
	CharPtr PNTR note;
	Uint1 PNTR note_alloc;
	CharPtr PNTR note_annot;
	Int2 	note_size, note_index;
} NoteStruct, PNTR NoteStructPtr;

/* ---------------The Biotable Structure ------------*/

typedef struct biotable {
	SeqEntryPtr sep;	/* the SeqEntryPtr passed to asn2ff_setup */
	SeqEntryPtr PNTR sepp;	/* An array of SeqEntryPtr's, used by makerpt */
	BioseqPtr PNTR bsp;	/* This array lists the (valid) entries to be 
					formatted */
	BioseqPtr PNTR table;	/* This array lists the BioseqPtr's with repr
				"seg" in a segmented set.  If the entry is not 
				segmented, this is NULL */
	BioseqContextPtr PNTR bcp;	/* The BioseqContextPtr for each
					BioseqPtr */
	BioseqContextPtr PNTR bcp_na;	/* BioseqContextPtr of a nucleotide
				sequence, used for an amino acid sequence;
				set in EstablishProteinLink */ 
	Uint1 PNTR bcp_na_alloc;	/* flag that says whether the bcp is
					allocated */
	SeqFeatPtr PNTR cds;	/* Used by genPept to determine which CDS
				a protein comes from, set in 
				EstablishProteinLink */
	Int2 FlatLoc_context;
	GeneStructPtr PNTR PNTR gspp;	/* information on Genes */ 
	NoteStructPtr PNTR PNTR nspp;	/* information on Notes. */
	NoteStructPtr source_notes;	/* Note for source feature */
	SeqFeatPtr sfp_out;	/* ImpFeat used to hole SeqFeat info while
				they're being formatted */
/* The next several variables, all starting with "sfp", are set in 
OrganizeSeqFeat */
	Int2Ptr sfpListsize;	/* Number of "generic" (i.e., not in any
				category below) features/entry */
	SeqFeatPtr PNTR PNTR sfpList;	/* ptr's to "generic" features */
	Int2Ptr sfpCommsize;	/* Number of comment features */ 
	SeqFeatPtr PNTR PNTR sfpCommlist;	/* ptr's to comment features */
	Int2Ptr sfpGenesize;	/* Number of gene features */
	SeqFeatPtr PNTR PNTR sfpGenelist;	/* Ptr's to gene features */
	Int2Ptr sfpOrgsize;	/* Number of Organism features. */
	SeqFeatPtr PNTR PNTR sfpOrglist;	/* ptr's to Org features. */
	Int2Ptr sfpSitesize;	/* Number of ImpFeat's with key "Site-ref" */
	SeqFeatPtr PNTR PNTR sfpSitelist; /* ptr's to ImpFeat's with key "Site-ref" */
	Int2Ptr sfpSourcesize;	/* Number of ImpFeat's with key "source" */
	SeqFeatPtr PNTR PNTR sfpSourcelist; /* ptr's to ImpFeat's with key "source" */
	Int2Ptr sfpXrefsize;	/* Number of Seq's that go out as xref's */
	SeqFeatPtr PNTR PNTR sfpXreflist;/* ptr's to Seq's that are xref's */
	ValNodePtr PNTR Pub;	/* Linked list of references */
	CharPtr PNTR base_cnt_line;
	CharPtr PNTR access;
	CharPtr PNTR locus;
	CharPtr PNTR division;
	CharPtr PNTR date;
	CharPtr PNTR create_date;
	CharPtr PNTR update_date;
	CharPtr PNTR embl_rel;
	Int2Ptr embl_ver;
	Int4Ptr gi;
	Int2 count;
	Int2Ptr seg_num;
	Int2Ptr seg_total;
	Int4 pap_index;
	Uint1 pap_last;
	Char  newline;
	Boolean pseudo;
	Uint1 gene_binding;
	Int2 number_of_cds;
	SeqPortPtr spp;
        Int4 count_set;
        Boolean found_match;
	struct biotable PNTR btp_other;
} Biotable, PNTR BiotablePtr;

/*----------- Estimates for the number of lines returned for
each of the following------------------------------------------*/

#define NUM_OF_ESTIMATES 20
#define NUM_SEQ_LINES 10

static Int2 line_estimate[NUM_OF_ESTIMATES] = {
1, /* 0; Locus, Segment, Base Count, Origin, Feature Header lines */
1, /* 1; Definition line(s) */
1, /* 2; Accession line(s) */
1, /* 3; Keyword line(s) */
2, /* 4; Source lines */
6, /* 5; Reference (pub) lines */
10, /* 6; GBComAndFH */
11, /* 7; GBComAndXref */
6, /* 8; Features */
NUM_SEQ_LINES, /* 9; Sequence lines */
2, /* 10; EMBL Date lines */
2, /* 11; EMBL Organism lines */
4, /* 12; DBSOURCE field */
};



typedef void (* FFPapFct) PROTO ((Asn2ffJobPtr ajp, BiotablePtr btp, Int2 count));

typedef struct ffprintarray {
	FFPapFct fct;
	Asn2ffJobPtr ajp;
	BiotablePtr btp;
	Int2 count;
	Int4 index;
	Uint1 last;
	Uint1 printxx;
	Int2 estimate;
	SeqFeatPtr sfp;
	ValNodePtr pub;
	ValNodePtr descr;
} FFPrintArray, PNTR FFPrintArrayPtr;

#endif
