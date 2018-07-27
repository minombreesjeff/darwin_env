/*   asn2gnbp.h
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
* File Name:  asn2gnbp.h
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov, Jonathan Kans
*
* Version Creation Date:   10/21/98
*
* $Revision: 6.10 $
*
* File Description:  New GenBank flatfile generator, private header
*
* Modifications:  
* --------------------------------------------------------------------------
* ==========================================================================
*/

#ifndef _ASN2NGNBP_
#define _ASN2NGNBP_

#include <asn2gnbk.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* private flags */

#define FREE_SEQPORT_EACH_TIME  128
#define DDJB_VARIANT_FORMAT     256
#define USE_NEW_SOURCE_ORG      512
#define USE_OLD_LOCUS_LINE     1024
#define COPY_GPS_CDS_UP        2048
#define COPY_GPS_GENE_DOWN     4096

/* internal block type identifiers */

typedef enum {
  LOCUS_BLOCK = 1,
  DEFLINE_BLOCK,
  ACCESSION_BLOCK,
  VERSION_BLOCK,
  PID_BLOCK,
  DBSOURCE_BLOCK,
  DATE_BLOCK,
  KEYWORDS_BLOCK,
  SEGMENT_BLOCK,
  SOURCE_BLOCK,
  ORGANISM_BLOCK,
  REFERENCE_BLOCK,
  COMMENT_BLOCK,
  FEATHEADER_BLOCK,
  SOURCEFEAT_BLOCK,
  FEATURE_BLOCK,
  BASECOUNT_BLOCK,
  ORIGIN_BLOCK,
  SEQUENCE_BLOCK,
  CONTIG_BLOCK,
  SLASH_BLOCK
} BlockType;

#define ASN2GB_BASE_BLOCK \
  Uint2      entityID;  \
  Uint2      itemID;    \
  Uint2      itemtype;  \
  Int4       section;   \
  Int4       paragraph; \
  BlockType  blocktype; \
  CharPtr    string;    \

/* base block structure for most paragraph types */

typedef struct base_block {
  ASN2GB_BASE_BLOCK
} BaseBlock, PNTR BaseBlockPtr;

/* version block includes VERSION and NID sections */
/* organism block includes SOURCE and ORGANISM sections */
/* source (feat) block should be the same as the organism block */

/* references are grouped by published, unpublished, and cit-subs */

typedef enum {
  REF_CAT_PUB = 1,
  REF_CAT_UNP,
  REF_CAT_SUB
} RefType;

typedef struct ref_block {
  ASN2GB_BASE_BLOCK
  Int4     pmid;
  Int4     muid;
  CharPtr  uniquestr;
  Int2     serial;
  RefType  category;
  Int2     sites;
} RefBlock, PNTR RefBlockPtr;

/* featdeftype allows specific feature classes to be identified */

typedef struct feat_block {
  ASN2GB_BASE_BLOCK
  Uint1  featdeftype;
} FeatBlock, PNTR FeatBlockPtr;

/* sequences are broken up into paragraphs and use the section's internal SeqPort */

typedef struct seq_block {
  ASN2GB_BASE_BLOCK
  Int4  start;
  Int4  stop;
} SeqBlock, PNTR SeqBlockPtr;


/* structure for single segment or pop/phy/mut set component */

typedef struct asn2gb_sect {

  /* data identifiers for individual accession report */

  BioseqPtr     target;
  BioseqPtr     bsp;
  SeqLocPtr     slp;
  Uint2         seg;
  Int2          numsegs;
  Int4          from;
  Int4          to;

  /* local array pointing to all blocks in this section */

  BaseBlockPtr  PNTR blockArray;
  Int4          numBlocks;

  /* referenceks for feature citation matching, serial number assignment */

  RefBlockPtr   PNTR referenceArray;
  Int2          numReferences;

} Asn2gbSect, PNTR Asn2gbSectPtr;


/* master pointer returned to application */

typedef struct asn2gb_job {

  /* data identifiers for sequence or sequences to report */

  Uint2          entityID;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  SeqLocPtr      slp;

  /* each accession report from LOCUS to // is a single section */

  Asn2gbSectPtr  PNTR sectionArray;
  Int4           numSections;

  /* master array pointing to all blocks in all sections */

  BaseBlockPtr   PNTR paragraphArray;
  Int4           numParagraphs;

  /* sorted array to get paragraphs for entityID/itemtype/itemID */

  BaseBlockPtr   PNTR paragraphByIDs;
} Asn2gbJob,  PNTR Asn2gbJobPtr;

/*
  asn2gnbk_setup creates a structure laying out the flatfile structure.

    Of the first three parameters (bsp, bssp, slp), only one should not be NULL.
    If bsp is passed in, that is the target bioseq.  If bssp is passed in, it is
    expected to be a population/phylogenetic/mutation study, and all records are
    shown, but with no segment numbers.  slp is not yet implemented.

  asn2gnbk_format creates a string for a given paragraph.

  asn2gnbk_cleanup frees the structure and all components.

   SeqEntryToGnbk calls asn2gnbk_setup, _format, and _cleanup internally.
*/

NLM_EXTERN Asn2gbJobPtr asn2gnbk_setup (
  BioseqPtr bsp,
  BioseqSetPtr bssp,
  SeqLocPtr slp,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags
);

NLM_EXTERN CharPtr asn2gnbk_format (
  Asn2gbJobPtr ajp,
  Int4 paragrap
);

NLM_EXTERN Asn2gbJobPtr asn2gnbk_cleanup (
  Asn2gbJobPtr ajp
);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* ndef _ASN2NGNBP_ */

