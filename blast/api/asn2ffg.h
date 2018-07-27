/*   asn2ffg.h
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
* File Name:  asn2ffg.h
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
*
* Version Creation Date:   7/15/95
*
* $Revision: 6.17 $
* $Revision: 6.17 $
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

/*************************************
*
 * $Log: asn2ffg.h,v $
 * Revision 6.17  2001/12/05 18:14:05  cavanaug
 * Changes for new LOCUS line format
 *
 * Revision 6.16  2000/06/05 17:51:41  tatiana
 * increase size of feature arrays to Int4
 *
 * Revision 6.15  2000/04/03 23:33:36  tatiana
 * added showSeqLoc for GenBank region view
 *
 * Revision 6.14  2000/03/20 23:38:39  aleksey
 * Finally submitted the changes which have been made by serge bazhin
 * and been kept in my local directory.
 *
 * These changes allow to establish user callback functions
 * in 'Asn2ffJobPtr' structure which are called within
 * 'SeqEntryToFlatAjp' function call.
 * The new members are:
 * user_data       - pointer to a user context for passing data
 * ajp_count_index - user defined function
 * ajp_print_data  - user defined function
 * ajp_print_index - user defined function
 *
 * Revision 6.13  2000/02/09 19:34:38  kans
 * added forgbrel flag to Asn2ffJobPtr, currently used to suppress PUBMED line, which was not formally announced in release notes
 *
 * Revision 6.12  1999/11/05 14:54:04  tatiana
 * EMBL_PREFNUM increased to 5
 *
 * Revision 6.11  1999/10/27 20:57:07  tatiana
 * bankit added to ajp
 *
 * Revision 6.10  1999/09/23 18:06:04  tatiana
 * contig_view added to ajp
 *
 * Revision 6.9  1999/04/02 16:33:15  tatiana
 * added LinkStr struct and ByteStorePtr to ajp
 *
 * Revision 6.8  1999/03/11 19:04:31  tatiana
 * version added
 *
 * Revision 6.7  1998/11/10 15:12:59  bazhin
 * Macro NUM_OF_ESTIMATES and static array of integers "line_estimate"
 * moved out to "asn2ff1.c".
 *
 * Revision 6.6  1998/09/24 17:46:00  kans
 * fixed GetDBXrefFromGene problem (TT)
 *
 * Revision 6.5  1998/07/14 17:59:06  kans
 * added useSeqMgrIndexes to OrganizeFeatPtr and Asn2ffJobPtr
 *
 * Revision 6.4  1998/03/09 21:41:03  tatiana
 * accession length increased to 60
 *
 * Revision 6.3  1998/02/12 15:48:26  tatiana
 * EMBL_PREFNUM increased to 4
 *
 * Revision 6.2  1998/01/13 21:10:50  tatiana
 * added Biosrclist to organizefeat struct
 *
 * Revision 6.0  1997/08/25 18:05:13  madden
 * Revision changed to 6.0
 *
 * Revision 5.16  1997/07/23 18:35:51  tatiana
 * SeqIdPtr id_print added to Asn2ffJob structure
 *
 * Revision 5.14  1997/04/25  19:26:03  tatiana
 * #define EMBL_PREFNUM 1 added
 *
 * Revision 5.13  1997/03/13  15:42:47  tatiana
 * *** empty log message ***
 *
 * Revision 5.12  1997/01/27  18:33:51  tatiana
 * hup added to ajp
 *
 * Revision 5.11  1996/12/17  22:45:55  tatiana
 * added Boolean feat_free to SortStruct
 *
 * Revision 5.8  1996/10/25  22:22:44  tatiana
 * defline added to GBEntry
 *
 * Revision 5.7  1996/09/03  19:54:01  tatiana
 * extra_loc added
 *
 * Revision 5.6  1996/08/05  13:54:56  tatiana
 * orgname added to ajp struct
 *
 * Revision 5.4  1996/07/30  13:37:37  tatiana
 * 'show_gene' added to asn2ffJob
 *
 * Revision 5.3  1996/07/23  22:32:50  tatiana
 * added format to orf struct
 *
 * Revision 5.2  1996/07/02  18:06:20  tatiana
 * *** empty log message ***
 *
 * Revision 5.2  1996/07/02  18:06:20  tatiana
 * *** empty log message ***
 *
 * Revision 5.1  1996/06/11  15:24:59  tatiana
 * add embl ni to GBEntry struct
 *
 * Revision 4.13  1996/05/16  20:56:46  tatiana
 * source_info added to GBEntry structure
 *
 * Revision 4.12  1996/04/29  18:54:26  tatiana
 * multiple comments
 *
 * Revision 4.11  1996/04/12  03:41:44  tatiana
 * added Booleans to GBEntry
 *
 * Revision 4.10  1996/04/09  14:03:50  tatiana
 * DescrStructPtr and comms added to GBEntry
 *
 * Revision 4.9  1996/03/20  00:00:20  tatiana
 * add activity to GeneStruct
 *
 * Revision 4.8  1996/03/04  17:11:20  ostell
 * added support for ignore_top features
 *
 * Revision 4.7  1996/02/28  04:53:06  ostell
 * changes to support segmented master seeuquences
 *
 * Revision 4.6  1996/02/18  21:17:17  tatiana
 * number of pubs, feats and seqblocks added to GBEntry structure
 *
 * Revision 4.5  1996/02/15  15:57:11  tatiana
 * SortStruct changed for sorting within one entity
 *
 * Revision 4.4  1996/01/29  22:44:00  tatiana
 * genome_view added to Asn2ffJobPtr
 *
 * Revision 4.2  1995/12/13  16:35:02  tatiana
 * itemID etc. added to FFPrintArray structure
 *
 * Revision 1.1  1995/07/17  19:24:04  kans
 * Initial revision
 *
*
**************************************/

#ifndef _ASN2FFG_
#define _ASN2FFG_

#include <asn.h>
#include <objall.h>
#include <objpubd.h>

#include <seqport.h>
#include <objsub.h>
#include <prtutil.h>
#include <gather.h>

#define LINKS 20
#define EMBL_AC       "AFVXYZ"   /* patent is "A" */ /* dbEST = "F" */
#define EMBL_PREFNUM 5 /* embl two-letter prefix {XX} see asn2ff4.c is_embl()*/

#define FF_REGULAR 0
#define FF_TOP_COMPLETE 1
#define FF_TOP_CONTIG 2

typedef struct _link_str {
    CharPtr line;
    struct _link_str PNTR next;
} LinkStr, PNTR LinkStrPtr;

typedef struct genestruct {
	ValNodePtr gene;
	ValNodePtr product;
	ValNodePtr standard_name;
	CharPtr PNTR map;         /* only map[0] is used why we need PNTR? */
	ValNodePtr ECNum;
	ValNodePtr activity;
	Int2 map_size, map_index;   /* map_size is always 1 */
	Boolean pseudo;
	GeneRefPtr grp;
} GeneStruct, PNTR GeneStructPtr;

typedef struct notestruct {
	CharPtr PNTR note;
	Uint1 PNTR note_alloc;
	CharPtr PNTR note_annot;
	Int2 	note_size, note_index;
} NoteStruct, PNTR NoteStructPtr;

typedef struct sortstruct {
	BioseqPtr 	bsp;
	BioseqPtr 	seg_bsp;
	SeqFeatPtr 	sfp;		/* would be NULL if gather tempload == TRUE */
	SeqLocPtr 	slp;		/*for converted locations, free if not NULL!*/
	Uint2		entityID,	   
		    	itemID,			   
				itemtype;
	Int4		hash;
	Boolean 	dup;
	SeqLocPtr PNTR extra_loc;
	Int2 		extra_loc_cnt;
	Boolean 	feat_free;
	Boolean 	tempload;
	GeneStructPtr gsp;	/* information on Genes */ 
	NoteStructPtr nsp;	/* information on Notes. */
} SortStruct, PNTR SortStructPtr;

typedef struct descrstruct {
	ValNodePtr vnp;
	Uint2	entityID,	   
		    itemID,			   
			itemtype;
	struct descrstruct PNTR next;
} DescrStruct, PNTR DescrStructPtr;

typedef struct comstruct {
	CharPtr string;
	Boolean gsdb_id;
	Uint2	entityID,	   
		    itemID,			   
			itemtype;
	struct comstruct PNTR next;
} ComStruct, PNTR ComStructPtr;

typedef struct organizeprot {
	SortStructPtr list;
	Int2 size;
} OrganizeProt, PNTR OrganizeProtPtr;

typedef struct organizefeat {
	Boolean embl_feat;
	BioseqPtr bsp;
	BioseqPtr seg_bsp;
	Int4 sfpListsize;	
	Int4 sortListsize;	
	SortStructPtr List;	/* ptr's to "generic" features */
	Int2 sfpCommsize;	/* Number of comment features */ 
	Int2 sortCommsize;	/* Number of sorted comment features */ 
	SortStructPtr Commlist;	
	Int4 sfpGenesize;	/* Number of gene features */
	Int4 sortGenesize;	/* Number of sorted gene features */
	SortStructPtr Genelist;	
	Int4 sfpOrgsize;	/* Number of Organism features. */
	Int4 sortOrgsize;	/* Number of sorted Organism features. */
	SortStructPtr Orglist;	
	Int4 sfpSitesize;	/* Number of ImpFeat's with key "Site-ref" */
	Int4 sortSitesize;	/* Number of sorted ImpFeat's with key "Site-ref" */
	SortStructPtr Siteslist;	
	Int4 sfpSourcesize;	/* Number of ImpFeat's with key "source" */
	Int4 sortSourcesize;	/* Number of sorted ImpFeat's with key "source" */
	SortStructPtr Sourcelist;	
	Int4 sfpXrefsize;	/* Number of Seq's that go out as xref's */
	SortStructPtr Xreflist;
	Uint2	oldID;	   /* is used to sort within entity */
	NoteStructPtr source_notes;	/* Note for source feature */
	Uint1 format;				/* needed for ProtRef convertion */
	Boolean show_gene;				/* needed temporarely for 'gene' feature */
	BioseqPtr lock_bsp;
	Boolean non_strict;
	Int2 biosrcsize;	/* Number of sorted ImpFeat's with key "source" */
	SortStructPtr Biosrclist;	
	Boolean useSeqMgrIndexes;  /* new style indexing to eliminate nested gathers */
	Boolean showSeqLoc;  /* GenBank view for a region, skip truncated check */
} OrganizeFeat, PNTR OrganizeFeatPtr;

/*****************************************************************************
*
*   GBEntry
*     structure with info for a single GenBank record
*
*****************************************************************************/
typedef struct gbentry {
	BioseqPtr bsp;         /* the Bioseq for this record */
	Uint2	entityID,	   
		    itemID,		
			itemtype;
	Char date[12];
	CharPtr create_date;
	CharPtr update_date;
	CharPtr embl_rel;
	Int2 embl_ver;
	Int4 gi;
	CharPtr ni;
	CharPtr base_cnt_line;
	Boolean xref_present;
	Char div[4];			/* division */
	Char locus[25];        /* locus */
	Char accession[60];    /* primary accession */
	Int2 num_seg;          /* segment number if segmented */
	Int4 length;           /* length of entry */
	SeqPortPtr spp;        /* seqport on entry */
	OrganizeFeatPtr feat;  /* temporary struct with features */
	ValNodePtr Pub;
	DescrStructPtr descr;		/* keeps entityID, itemID, itemtype for descr */
	DescrStructPtr source_info;	/* keeps org info if no source feature found */
	Int4 feat_num;				/* number of printed features */
	Int2 comm_num;				/* number of printed cooment blocks */
	ComStructPtr comm;		/* CharPtr in com.data.ptrvalue are comments */
	Boolean map;
	CharPtr defline;
	struct gbentry PNTR next;
	Char version[60];    /* accession.version */
} GBEntry, PNTR GBEntryPtr;

/*****************************************************************************
*
*   Asn2ffWE
*     asn2ff working environment. Keeps top level information about current
*       unit under construction. Serves as head of chain of structs, one for
*       GenBank record finally produced.
*
*****************************************************************************/
typedef struct asn2ffwe {
						 /* Working environment for data object */
	SeqEntryPtr current_sep;  /* current SeqEntry  ???? Tatiana*/
	BioseqPtr current_bsp;    /* current Bioseq ???? Tatiana*/
	BioseqSetPtr current_bssp;/* current BioseqSet ???? Tatiana*/
	                        /* SeqSubmit Only */
	ValNodePtr cit_sub;       /* if a SeqSubmit, the Cit-sub for it */
	                        /* Segmented Set Only */
	Char base_name[25];       /* base LOCUS name if segmented set */
	Int2 total_seg;             /* number of segments in segmented set */
	BioseqPtr seg;            /* segmented Bioseq in segmented set */
	BioseqSetPtr parts;       /* parts set for segmented set */
	Boolean only_one;   /* only one segment of set being shown (current bsp) */
	GBEntryPtr gbp;         /* chain of data for each GB record */

} Asn2ffWE, PNTR Asn2ffWEPtr;

/*****************************************************************************
*
*   Asn2ffJob
*     top level job control structure to be filled in by caller
*     sets global options and I/O information
*     sets overall scope and target for formatting
*     points to function working environment (added by function)
*
*****************************************************************************/
typedef struct asn2ff_job {
	                      /* Set formatting options */
	Boolean show_gene,     /* show the gene feature */
			show_seq,     /* do not show the sequence */
		    show_gi,      /* do not show the GI id */
			error_msgs,   /* do not show feature validator messages */
			non_strict,   /* only strict gene/protein binding allowed */
			null_str,    /* error msg if printing NULL string */
			no_hold,      /* if TRUE, do not use ObjMgrSetHold around asn2ff_print */
			free_cache;   /* if TRUE, ObjMgrFreeCache() after asn2ff_print */
	Uint1	format,         /* from _FMT above */
			mode;           /* from _MODE above */
	FILE *  fp;             /* if not NULL, output goes to file */
	StdPrintOptionsPtr Spop; /* for templates */
	
						 /* Specify the data to be formatted */
	Uint2	entityID,	    /* could be a data object */
		    itemID,			   /* specified by ID or by pointer */
			itemtype;
	Boolean only_one, 			/* show only one top-level bioseq */
		ignore_top;            /* do not show features from top-level bioseq */
	SeqSubmitPtr ssp;          /* alternative data object pointers */
	SeqEntryPtr sep;
	SeqLocPtr slp;      	/* or could be a location on a sequence */
	Boolean gb_style;		/* only complete features are shown e.g. join cds on the last segmented bioseq */
	Boolean genome_view;	/* not printing the sequence and features*/
	Boolean map_view;	/* not printing the sequence */
	Boolean hup;		/* for submissions */
/* taken directly from Biotable, used for printing and formatting */
	Int4 pap_index;
	Uint1 pap_last;
	Boolean pseudo;
	SeqFeatPtr sfp_out;
	Int2 number_of_cds;
	Boolean help;
	
 			 /** this section filled in by function **************/
   Asn2ffWEPtr asn2ffwep;
	Boolean orgname;    /* new algorithm for /organism in source feature*/
	SeqIdPtr id_print;
	Boolean useSeqMgrIndexes;  /* new style indexing to eliminate nested gathers */
	Boolean show_version;
	ByteStorePtr byte_st;
	Boolean contig_view;	/* CONTIG line and features*/
	Boolean bankit; /* show Bankit comments*/
	Boolean forgbrel;
	Pointer user_data;
	Int4 (*ajp_print_data)(struct asn2ff_job *ajp, CharPtr str,
                               Pointer user_data);
	Int4 (*ajp_print_index)(struct asn2ff_job *ajp, Pointer user_data);
	Int4 (*ajp_count_index)(struct asn2ff_job *ajp, Int4 num, Pointer user_data);
	Boolean new_locus_fmt;	/* TRUE if utilizing the new format for the LOCUS line */
} Asn2ffJob, PNTR Asn2ffJobPtr;



/*----------- Estimates for the number of lines returned for
each of the following------------------------------------------*/

#define NUM_SEQ_LINES 10

typedef void (* FFPapFct) PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp));

typedef struct ffprintarray {
	FFPapFct fct;
	Asn2ffJobPtr ajp;
	GBEntryPtr gbp;
	Int4 index;
	Uint1 last;
	Uint1 printxx;
	Int2 estimate;
	DescrStructPtr descr;		/* keeps entityID, itemID, itemtype for descr */
} FFPrintArray, PNTR FFPrintArrayPtr;
/*****************************************************************************
*
*   Main asn2ff entry points
*
*****************************************************************************/

Boolean Asn2ff PROTO((Asn2ffJobPtr ajp));

#endif
