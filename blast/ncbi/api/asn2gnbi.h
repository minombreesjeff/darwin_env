/*   asn2gnbi.h
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
* File Name:  asn2gnbi.h
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov, Jonathan Kans
*
* Version Creation Date:   12/30/03
*
* $Revision: 1.29 $
*
* File Description:  New GenBank flatfile generator, internal header
*
* Modifications:  
* --------------------------------------------------------------------------
* ==========================================================================
*/

#ifndef _ASN2NGNBI_
#define _ASN2NGNBI_

#include <asn2gnbp.h>
#include <explore.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif



#define ASN2FF_EMBL_MAX 78
#define ASN2FF_GB_MAX   79
#define SEQID_MAX_LEN   41

#define TILDE_IGNORE     0
#define TILDE_TO_SPACES  1
#define TILDE_EXPAND     2
#define TILDE_OLD_EXPAND 3


/* flags set by mode to customize behavior */

typedef struct asn2gbflags {
  Boolean             suppressLocalID;
  Boolean             validateFeats;
  Boolean             ignorePatPubs;
  Boolean             dropShortAA;
  Boolean             avoidLocusColl;
  Boolean             iupacaaOnly;
  Boolean             dropBadCitGens;
  Boolean             noAffilOnUnpub;
  Boolean             dropIllegalQuals;
  Boolean             checkQualSyntax;
  Boolean             needRequiredQuals;
  Boolean             needOrganismQual;
  Boolean             needAtLeastOneRef;
  Boolean             citArtIsoJta;
  Boolean             dropBadDbxref;
  Boolean             useEmblMolType;
  Boolean             hideBankItComment;
  Boolean             checkCDSproductID;
  Boolean             suppressSegLoc;
  Boolean             srcQualsToNote;
  Boolean             hideEmptySource;
  Boolean             goQualsToNote;
  Boolean             geneSynsToNote;
  Boolean             selenocysteineToNote;
  Boolean             extraProductsToNote;
  Boolean             forGbRelease;
} Asn2gbFlags, PNTR Asn2gbFlagsPtr;

/* internal Asn2gbSect structure has fields on top of Asn2gbSect fields */

typedef struct int_Asn2gbSect {
  Asn2gbSect  asp;
} IntAsn2gbSect, PNTR IntAsn2gbSectPtr;

/* string structure */

#define STRING_BUF_LEN  1024

typedef struct stringitem {
  struct stringitem  *curr;
  struct stringitem  *next;
  Pointer            iajp;
  Char               buf [STRING_BUF_LEN];
  Int4               pos;
} StringItem, PNTR StringItemPtr;

/* internal asn2gbjob structure has fields on top of Asn2gbJob fields */

typedef struct int_asn2gb_job {
  Asn2gbJob       ajp;
  FmtType         format;
  ModType         mode;
  Asn2gbFlags     flags;
  Boolean         showFarTransl;
  Boolean         transIfNoProd;
  Boolean         alwaysTranslCds;
  Boolean         showTranscript;
  Boolean         showPeptide;
  Boolean         masterStyle;
  Boolean         newSourceOrg;
  Boolean         produceInsdSeq;
  Boolean         refseqConventions;
  ValNodePtr      lockedBspList;
  ValNodePtr      gapvnp;
  ValNodePtr      remotevnp;
  Asn2gbLockFunc  remotelock;
  Asn2gbFreeFunc  remotefree;
  Pointer         remotedata;
  Boolean         relModeError;
  Boolean         skipProts;
  Boolean         skipMrnas;
  IndxPtr         index;
  GBSeqPtr        gbseq;
  AsnIoPtr        aip;
  AsnTypePtr      atp;
  StringItemPtr   pool;
  Boolean         www;
} IntAsn2gbJob, PNTR IntAsn2gbJobPtr;

/* array for assigning biosource and feature data fields to qualifiers */
/* should be allocated to MAX (ASN2GNBK_TOTAL_SOURCE, ASN2GNBK_TOTAL_FEATUR) */

typedef union qualval {
  CharPtr        str;
  Boolean        ble;
  Int4           num;
  ValNodePtr     vnp;
  GBQualPtr      gbq;
  OrgModPtr      omp;
  SubSourcePtr   ssp;
  CodeBreakPtr   cbp;
  SeqLocPtr      slp;
  SeqIdPtr       sip;
  tRNAPtr        trp;
  UserObjectPtr  uop;
  UserFieldPtr   ufp;
} QualVal, PNTR QualValPtr;

/* structure passed to individual paragraph format functions */

typedef struct asn2gbformat {
  IntAsn2gbJobPtr  ajp;
  Asn2gbSectPtr    asp;
  QualValPtr       qvp;
  FmtType          format;
  Asn2gbWriteFunc  ffwrite;
  Pointer          userdata;
  FILE             *fp;
  AsnIoPtr         aip;
  AsnTypePtr       atp;
} Asn2gbFormat, PNTR Asn2gbFormatPtr;

/* structure for storing working parameters while building asn2gb_job structure */

typedef struct asn2gbwork {
  IntAsn2gbJobPtr  ajp;
  Uint2            entityID;

  FmtType          format;
  ModType          mode;
  StlType          style;

  ValNodePtr       pubhead;    /* for collecting publications */
  ValNodePtr       srchead;    /* for collecting biosources */

  /* linked lists of paragraphs, sections, blocks */

  ValNodePtr       sectionList;
  ValNodePtr       blockList;    /* reset for each new section */

  /* most recent node of linked lists, for quickly adding next node */

  ValNodePtr       lastsection;
  ValNodePtr       lastblock;    /* reset for each new section */

  Int4             currsection;

  /* set if doing immediate write at time of creation for web speed */

  Asn2gbFormatPtr  afp;

  /* section fields needed for populating blocks */

  Asn2gbSectPtr    asp;

  BioseqPtr        target;
  BioseqPtr        parent;
  BioseqPtr        bsp;
  BioseqPtr        refs;
  SeqLocPtr        slp;
  Uint2            seg;
  Int4             numsegs;
  Int4             partcount;
  Int4             from;
  Int4             to;
  Boolean          showAllFeats;

  Boolean          contig;
  Boolean          showconfeats;
  Boolean          showconsource;
  Boolean          smartconfeats;

  Boolean          onlyNearFeats;
  Boolean          farFeatsSuppress;
  Boolean          nearFeatsSuppress;

  Boolean          citSubsFirst;
  Boolean          hideGeneFeats;
  Boolean          newLocusLine;
  Boolean          showBaseCount;

  Boolean          hideImpFeats;
  Boolean          hideRemImpFeats;
  Boolean          hideSnpFeats;
  Boolean          hideExonFeats;
  Boolean          hideIntronFeats;
  Boolean          hideMiscFeats;
  Boolean          hideCddFeats;
  Boolean          hideCdsProdFeats;

  Boolean          hideGeneRIFs;
  Boolean          onlyGeneRIFs;
  Boolean          onlyReviewPubs;
  Boolean          newestPubs;
  Boolean          oldestPubs;

  Boolean          showRefs;
  Boolean          hideGaps;
  Boolean          hideSources;
  Boolean          hideSequence;

  Boolean          isGPS;
  Boolean          copyGpsCdsUp;
  Boolean          copyGpsGeneDown;

  Boolean          showContigAndSeq;

  Char             basename [SEQID_MAX_LEN];

  SeqFeatPtr       lastsfp;
  SeqAnnotPtr      lastsap;
  Int4             lastleft;
  Int4             lastright;

  Boolean          firstfeat;
  Boolean          featseen;

  SeqSubmitPtr     ssp;
  Boolean          hup;

  Boolean          failed;
} Asn2gbWork, PNTR Asn2gbWorkPtr;


/* Seq-hist replacedBy is preformatted into string field, */
/* then comment descriptors, Map location:, and Region:, */
/* then comment features, finally HTGS */

typedef struct comment_block {
  ASN2GB_BASE_BLOCK
  Boolean           first;
} CommentBlock, PNTR CommentBlockPtr;

/* internal reference block has fields on top of RefBlock fields */

typedef struct int_ref_block {
  RefBlock   rb;
  DatePtr    date;     /* internal sorting use only */
  SeqLocPtr  loc;      /* final location on target bioseq */
  CharPtr    authstr;  /* author string */
  Uint2      index;    /* index if feature on target bioseq */
  Boolean    justuids; /* gibb pub with uids and Figure, etc. */
  CharPtr    fig;      /* figure string from equivalent gibb pub */
  CharPtr    maploc;   /* maploc string from equivalent gibb pub */
  Boolean    poly_a;   /* poly_a field from equivalent gibb pub */
} IntRefBlock, PNTR IntRefBlockPtr;

/* internal source block has fields on top of BaseBlock fields */

typedef struct int_src_block {
  BaseBlock         bb;
  Boolean           is_descriptor;
  Boolean           is_focus;
  Boolean           is_synthetic;
  BioSourcePtr      biop;
  Uint4             orghash;
  Uint4             modhash;
  Uint4             subhash;
  Uint4             xrfhash;
  SeqLocPtr         loc;     /* final location on target bioseq */
  CharPtr           taxname;
  CharPtr           comment;
  OrgModPtr         omp;
  SubSourcePtr      ssp;
  ValNodePtr        vnp;
  Int4              left;
  Int4              right;
} IntSrcBlock, PNTR IntSrcBlockPtr;

/* internal feature block has fields on top of FeatBlock fields */

typedef struct int_feat_block {
  FeatBlock  fb;
  Boolean     mapToNuc;
  Boolean     mapToProt;
  Boolean     mapToGen;
  Boolean     mapToMrna;
  Boolean     mapToPep;
  Boolean     isCDS;     /* set if using IntCdsBlock */
  Boolean     firstfeat;
} IntFeatBlock, PNTR IntFeatBlockPtr;

/* internal cds block has fields on top of IntFeatBlock fields */

typedef struct int_cds_block {
  IntFeatBlock  ifb;
  CharPtr       fig;    /* figure string from pub */
  CharPtr       maploc; /* maploc string from pub */
} IntCdsBlock, PNTR IntCdsBlockPtr;


/* enumerated qualifier category definitions */

typedef enum {
  Qual_class_ignore = 0,
  Qual_class_string,
  Qual_class_tilde,
  Qual_class_sgml,
  Qual_class_boolean,
  Qual_class_int,
  Qual_class_evidence,
  Qual_class_valnode,
  Qual_class_EC_valnode,
  Qual_class_xtraprds,
  Qual_class_quote,
  Qual_class_EC_quote,
  Qual_class_noquote,
  Qual_class_label,
  Qual_class_number,
  Qual_class_paren,
  Qual_class_usedin,
  Qual_class_region,
  Qual_class_replace,
  Qual_class_consplice,
  Qual_class_bond,
  Qual_class_site,
  Qual_class_L_R_B,
  Qual_class_rpt,
  Qual_class_organelle,
  Qual_class_orgmod,
  Qual_class_subsource,
  Qual_class_code_break,
  Qual_class_anti_codon,
  Qual_class_codon,
  Qual_class_compare,
  Qual_class_method,
  Qual_class_pubset,
  Qual_class_db_xref,
  Qual_class_seq_id,
  Qual_class_seq_loc,
  Qual_class_its,
  Qual_class_sec_str,
  Qual_class_trna_codons,
  Qual_class_translation,
  Qual_class_transcription,
  Qual_class_peptide,
  Qual_class_protnames,
  Qual_class_encodes,
  Qual_class_illegal,
  Qual_class_note,
  Qual_class_rpt_unit,
  Qual_class_product,
  Qual_class_model_ev,
  Qual_class_gene_syn,
  Qual_class_locus_tag,
  Qual_class_go
}  QualType;

/* source 'feature' */

/* some qualifiers will require additional content verification not
   explicitly indicated by the class type */

typedef enum {
  SCQUAL_acronym = 1,
  SCQUAL_anamorph,
  SCQUAL_authority,
  SCQUAL_biotype,
  SCQUAL_biovar,
  SCQUAL_breed,
  SCQUAL_cell_line,
  SCQUAL_cell_type,
  SCQUAL_chemovar,
  SCQUAL_chromosome,
  SCQUAL_citation,
  SCQUAL_clone,
  SCQUAL_clone_lib,
  SCQUAL_collected_by,
  SCQUAL_collection_date,
  SCQUAL_common,
  SCQUAL_common_name,
  SCQUAL_country,
  SCQUAL_cultivar,
  SCQUAL_db_xref,
  SCQUAL_org_xref,
  SCQUAL_dev_stage,
  SCQUAL_dosage,
  SCQUAL_ecotype,
  SCQUAL_endogenous_virus_name,
  SCQUAL_environmental_sample,
  SCQUAL_extrachrom,
  SCQUAL_focus,
  SCQUAL_forma,
  SCQUAL_forma_specialis,
  SCQUAL_frequency,
  SCQUAL_fwd_primer_name,
  SCQUAL_fwd_primer_seq,
  SCQUAL_gb_acronym,
  SCQUAL_gb_anamorph,
  SCQUAL_gb_synonym,
  SCQUAL_genotype,
  SCQUAL_germline,
  SCQUAL_group,
  SCQUAL_haplotype,
  SCQUAL_identified_by,
  SCQUAL_ins_seq_name,
  SCQUAL_isolate,
  SCQUAL_isolation_source,
  SCQUAL_lab_host,
  SCQUAL_label,
  SCQUAL_lat_lon,
  SCQUAL_macronuclear,
  SCQUAL_map,
  SCQUAL_mol_type,
  SCQUAL_note,
  SCQUAL_old_lineage,
  SCQUAL_old_name,
  SCQUAL_organism,
  SCQUAL_organelle,
  SCQUAL_orgmod_note,
  SCQUAL_pathovar,
  SCQUAL_plasmid_name,
  SCQUAL_plastid_name,
  SCQUAL_pop_variant,
  SCQUAL_rearranged,
  SCQUAL_rev_primer_name,
  SCQUAL_rev_primer_seq,
  SCQUAL_segment,
  SCQUAL_seqfeat_note,
  SCQUAL_sequenced_mol,
  SCQUAL_serogroup,
  SCQUAL_serotype,
  SCQUAL_serovar,
  SCQUAL_sex,
  SCQUAL_spec_or_nat_host,
  SCQUAL_specimen_voucher,
  SCQUAL_strain,
  SCQUAL_sub_clone,
  SCQUAL_sub_group,
  SCQUAL_sub_species,
  SCQUAL_sub_strain,
  SCQUAL_sub_type,
  SCQUAL_subsource_note,
  SCQUAL_synonym,
  SCQUAL_teleomorph,
  SCQUAL_tissue_lib,
  SCQUAL_tissue_type,
  SCQUAL_transgenic,
  SCQUAL_transposon_name,
  SCQUAL_type,
  SCQUAL_unstructured,
  SCQUAL_usedin,
  SCQUAL_variety,
  SCQUAL_zero_orgmod,
  SCQUAL_one_orgmod,
  SCQUAL_zero_subsrc,
  ASN2GNBK_TOTAL_SOURCE
}  SourceType;

NLM_EXTERN SourceType orgModToSourceIdx [38];

typedef enum {
  FTQUAL_allele = 1,
  FTQUAL_anticodon,
  FTQUAL_bond,
  FTQUAL_bond_type,
  FTQUAL_bound_moiety,
  FTQUAL_cds_product,
  FTQUAL_citation,
  FTQUAL_clone,
  FTQUAL_coded_by,
  FTQUAL_compare,
  FTQUAL_codon,
  FTQUAL_codon_start,
  FTQUAL_cons_splice,
  FTQUAL_db_xref,
  FTQUAL_derived_from,
  FTQUAL_direction,
  FTQUAL_EC_number,
  FTQUAL_encodes,
  FTQUAL_estimated_length,
  FTQUAL_evidence,
  FTQUAL_exception,
  FTQUAL_exception_note,
  FTQUAL_extra_products,
  FTQUAL_figure,
  FTQUAL_frequency,
  FTQUAL_function,
  FTQUAL_gene,
  FTQUAL_gene_desc,
  FTQUAL_gene_allele,
  FTQUAL_gene_map,
  FTQUAL_gene_syn,
  FTQUAL_gene_syn_refseq,
  FTQUAL_gene_note,
  FTQUAL_gene_xref,
  FTQUAL_go_component,
  FTQUAL_go_function,
  FTQUAL_go_process,
  FTQUAL_heterogen,
  FTQUAL_illegal_qual,
  FTQUAL_insertion_seq,
  FTQUAL_label,
  FTQUAL_locus_tag,
  FTQUAL_map,
  FTQUAL_maploc,
  FTQUAL_mod_base,
  FTQUAL_modelev,
  FTQUAL_note,
  FTQUAL_number,
  FTQUAL_old_locus_tag,
  FTQUAL_operon,
  FTQUAL_organism,
  FTQUAL_partial,
  FTQUAL_PCR_conditions,
  FTQUAL_peptide,
  FTQUAL_phenotype,
  FTQUAL_product,
  FTQUAL_product_quals,
  FTQUAL_prot_activity,
  FTQUAL_prot_comment,
  FTQUAL_prot_EC_number,
  FTQUAL_prot_note,
  FTQUAL_prot_method,
  FTQUAL_prot_conflict,
  FTQUAL_prot_desc,
  FTQUAL_prot_missing,
  FTQUAL_prot_name,
  FTQUAL_prot_names,
  FTQUAL_protein_id,
  FTQUAL_pseudo,
  FTQUAL_region,
  FTQUAL_region_name,
  FTQUAL_replace,
  FTQUAL_rpt_family,
  FTQUAL_rpt_type,
  FTQUAL_rpt_unit,
  FTQUAL_rrna_its,
  FTQUAL_sec_str_type,
  FTQUAL_selenocysteine,
  FTQUAL_selenocysteine_note,
  FTQUAL_seqfeat_note,
  FTQUAL_site,
  FTQUAL_site_type,
  FTQUAL_standard_name,
  FTQUAL_transcription,
  FTQUAL_transcript_id,
  FTQUAL_transcript_id_note, /* !!! remove October 15, 2003 !!! */
  FTQUAL_transl_except,
  FTQUAL_transl_table,
  FTQUAL_translation,
  FTQUAL_transposon,
  FTQUAL_trna_aa,
  FTQUAL_trna_codons,
  FTQUAL_usedin,
  FTQUAL_xtra_prod_quals,
  ASN2GNBK_TOTAL_FEATUR
}  FtQualType;

#define MAX_WWWBUF 328

NLM_EXTERN Char link_feat [MAX_WWWBUF];
NLM_EXTERN Char link_seq [MAX_WWWBUF];
NLM_EXTERN Char link_wgs [MAX_WWWBUF];
NLM_EXTERN Char link_omim [MAX_WWWBUF];
NLM_EXTERN Char ref_link [MAX_WWWBUF];
NLM_EXTERN Char nt_link [MAX_WWWBUF];
NLM_EXTERN Char doc_link [MAX_WWWBUF];
NLM_EXTERN Char ev_link [MAX_WWWBUF];
NLM_EXTERN Char ec_link [MAX_WWWBUF];
NLM_EXTERN Char link_tax [MAX_WWWBUF];
NLM_EXTERN Char link_muid [MAX_WWWBUF];
NLM_EXTERN Char link_code [MAX_WWWBUF];
NLM_EXTERN Char link_encode [MAX_WWWBUF];
NLM_EXTERN Char link_go [MAX_WWWBUF];
NLM_EXTERN Char link_sp [MAX_WWWBUF];

NLM_EXTERN void FF_www_db_xref(
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  CharPtr db, CharPtr identifier
);

NLM_EXTERN Boolean StringIsJustQuotes (
  CharPtr str
);


typedef struct sourcequal {
  CharPtr   name;
  QualType  qualclass;
} SourceQual, PNTR SourceQualPtr;

NLM_EXTERN SourceQual asn2gnbk_source_quals [ASN2GNBK_TOTAL_SOURCE];

NLM_EXTERN SourceType subSourceToSourceIdx [38];

NLM_EXTERN void DoOneSection (
  BioseqPtr target,
  BioseqPtr parent,
  BioseqPtr bsp,
  BioseqPtr refs,
  SeqLocPtr slp,
  Uint2 seg,
  Int4 from,
  Int4 to,
  Boolean contig,
  Boolean onePartOfSeg,
  Asn2gbWorkPtr awp
);

NLM_EXTERN void DoOneBioseq (
  BioseqPtr bsp,
  Pointer userdata
);

NLM_EXTERN BaseBlockPtr Asn2gbAddBlock (
  Asn2gbWorkPtr awp,
  BlockType blocktype,
  size_t size
);

NLM_EXTERN void InitWWW (IntAsn2gbJobPtr ajp);
NLM_EXTERN void FiniWWW (IntAsn2gbJobPtr ajp);
NLM_EXTERN Boolean GetWWW (IntAsn2gbJobPtr ajp);

NLM_EXTERN StringItemPtr FFGetString (IntAsn2gbJobPtr ajp);
NLM_EXTERN void FFRecycleString (IntAsn2gbJobPtr ajp, StringItemPtr ffstring);
NLM_EXTERN void FFAddOneChar (
  StringItemPtr sip, 
  Char ch,
  Boolean convertQuotes
);
NLM_EXTERN void FFAddNewLine(StringItemPtr ffstring);
NLM_EXTERN void FFAddNChar (
  StringItemPtr sip, 
  Char ch,
  Int4 n,
  Boolean convertQuotes
);
NLM_EXTERN void FFExpandTildes (StringItemPtr sip, CharPtr PNTR cpp);
NLM_EXTERN void FFReplaceTildesWithSpaces (StringItemPtr ffstring, CharPtr PNTR cpp);
NLM_EXTERN void FFOldExpand (StringItemPtr sip, CharPtr PNTR cpp);
NLM_EXTERN void AddCommentStringWithTildes (StringItemPtr ffstring, CharPtr string);
NLM_EXTERN void AddCommentWithURLlinks (
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  CharPtr prefix,
  CharPtr str,
  CharPtr suffix
);
NLM_EXTERN void AddStringWithTildes (StringItemPtr ffstring, CharPtr string);
NLM_EXTERN void FFProcessTildes (StringItemPtr sip, CharPtr PNTR cpp, Int2 tildeAction);
NLM_EXTERN void FFAddPeriod (StringItemPtr sip);
NLM_EXTERN void FFAddOneString (
  StringItemPtr sip, 
  CharPtr string,
  Boolean addPeriod, 
  Boolean convertQuotes,
  Int2 tildeAction
);
NLM_EXTERN void FFCatenateSubString (
  StringItemPtr dest,
  StringItemPtr start_sip, Int4 start_pos,
  StringItemPtr end_sip, Int4 end_pos
);
NLM_EXTERN CharPtr FFToCharPtr (StringItemPtr sip);
NLM_EXTERN void FFSkipLink (StringItemPtr PNTR iterp, Int4Ptr ip);
NLM_EXTERN Boolean FFIsStartOfLink (StringItemPtr iter, Int4 pos);
NLM_EXTERN void FFSavePosition(StringItemPtr ffstring, StringItemPtr PNTR bufptr, Int4 PNTR posptr);
NLM_EXTERN void FFTrim (
    StringItemPtr ffstring,
    StringItemPtr line_start,
    Int4 line_pos,
    Int4 line_prefix_len
);NLM_EXTERN void FFCalculateLineBreak (
  StringItemPtr PNTR break_sip, Int4 PNTR break_pos,
  Int4 init_indent, Int4 visible
);
NLM_EXTERN void FFLineWrap (
  StringItemPtr dest, 
  StringItemPtr src, 
  Int4 init_indent,
  Int4 cont_indent, 
  Int4 line_max,
  CharPtr eb_line_prefix
);
NLM_EXTERN void FFStartPrint (
  StringItemPtr sip,
  FmtType format,
  Int4 gb_init_indent,
  Int4 gb_cont_indent,
  CharPtr gb_label,
  Int4 gb_tab_to,
  Int4 eb_init_indent,
  Int4 eb_cont_indent,
  CharPtr eb_line_prefix,
  Boolean eb_print_xx 
);
NLM_EXTERN void FFAddTextToString (
  StringItemPtr ffstring, 
  CharPtr prefix,
  CharPtr string,
  CharPtr suffix,
  Boolean addPeriod,
  Boolean convertQuotes,
  Int2 tildeAction
);
NLM_EXTERN CharPtr FFEndPrint (
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  FmtType format,
  Int2 gb_init_indent,
  Int2 gb_cont_indent,
  Int2 eb_init_indent,
  Int2 eb_cont_indent,
  CharPtr eb_line_prefix
);
NLM_EXTERN Uint4 FFLength(StringItemPtr ffstring);
NLM_EXTERN Char FFCharAt(StringItemPtr ffstring, Uint4 pos);
NLM_EXTERN Char FFFindChar (
  StringItemPtr ffstring,
  StringItemPtr start_buf,
  Uint4 start_pos,
  Uint4 old_pos,
  Uint4 new_pos
);
NLM_EXTERN Boolean FFEmpty(StringItemPtr ffstring);
NLM_EXTERN Int4 FFStringSearch (
  StringItemPtr text,
  const CharPtr pattern,
  Uint4 position
);
NLM_EXTERN Boolean IsWholeWordSubstr (
  StringItemPtr searchStr,
  Uint4 foundPos,
  CharPtr subStr
);

NLM_EXTERN ValNodePtr ValNodeCopyStrToHead (ValNodePtr PNTR head, Int2 choice, CharPtr str);
NLM_EXTERN CharPtr MergeFFValNodeStrs (
  ValNodePtr list
);
NLM_EXTERN void AddValNodeString (
  ValNodePtr PNTR head,
  CharPtr prefix,
  CharPtr string,
  CharPtr suffix
);
NLM_EXTERN void FFAddString_NoRedund (
  StringItemPtr unique,
  CharPtr prefix,
  CharPtr string,
  CharPtr suffix
);
NLM_EXTERN void s_AddPeriodToEnd (CharPtr someString);
NLM_EXTERN Boolean s_RemovePeriodFromEnd (CharPtr someString);
NLM_EXTERN Boolean IsEllipsis (
  CharPtr str
);
NLM_EXTERN void A2GBSeqLocReplaceID (
  SeqLocPtr newloc,
  SeqLocPtr ajpslp
);
NLM_EXTERN CharPtr asn2gb_PrintDate (
  DatePtr dp
);
NLM_EXTERN CharPtr DateToFF (
  CharPtr buf,
  DatePtr dp,
  Boolean citSub
);

NLM_EXTERN CharPtr FFFlatLoc (
  IntAsn2gbJobPtr ajp,
  BioseqPtr bsp,
  SeqLocPtr location,
  Boolean masterStyle
);

NLM_EXTERN void FF_www_featloc(StringItemPtr ffstring, CharPtr loc);

NLM_EXTERN CharPtr GetMolTypeQual (
  BioseqPtr bsp
);

NLM_EXTERN void AddFeatureToGbseq (
  GBSeqPtr gbseq,
  GBFeaturePtr gbfeat,
  CharPtr str,
  SeqFeatPtr sfp
);

NLM_EXTERN SeqIdPtr SeqLocIdForProduct (
  SeqLocPtr product
);

NLM_EXTERN CharPtr GetAuthorsString (
  FmtType format,
  AuthListPtr alp,
  CharPtr PNTR consortP,
  IndxPtr index,
  GBReferencePtr gbref
);

NLM_EXTERN AuthListPtr GetAuthListPtr (
  PubdescPtr pdp,
  CitSubPtr csp
);

NLM_EXTERN Int2 MatchRef (
  ValNodePtr ppr,
  RefBlockPtr PNTR rbpp,
  Int2 numReferences
);

NLM_EXTERN SeqLocPtr SeqLocReMapEx (
  SeqIdPtr newid,
  SeqLocPtr seq_loc,
  SeqLocPtr location,
  Int4 offset,
  Boolean rev,
  Boolean masterStyle
);

NLM_EXTERN CharPtr Get3LetterSymbol (
  IntAsn2gbJobPtr  ajp,
  Uint1 seq_code,
  SeqCodeTablePtr table,
  Uint1 residue
);

NLM_EXTERN CharPtr CleanQualValue (
  CharPtr str
);
NLM_EXTERN CharPtr Asn2gnbkCompressSpaces (CharPtr str);
NLM_EXTERN CharPtr StripAllSpaces (
  CharPtr str
);

NLM_EXTERN Boolean GetAccnVerFromServer (Int4 gi, CharPtr buf);

NLM_EXTERN CharPtr bondList [];
NLM_EXTERN CharPtr siteList [];
NLM_EXTERN CharPtr secStrText [];

NLM_EXTERN CharPtr goQualType [];
NLM_EXTERN CharPtr goFieldType [];

NLM_EXTERN CharPtr legalDbXrefs [];
NLM_EXTERN CharPtr legalRefSeqDbXrefs [];


NLM_EXTERN void AddFeatureBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN Boolean AddReferenceBlock (
  Asn2gbWorkPtr awp,
  Boolean isRefSeq
);
NLM_EXTERN void AddSourceFeatBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddFeatureBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddLocusBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddAccessionBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddVersionBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddDbsourceBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddDateBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddDeflineBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddKeywordsBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddSegmentBlock (
  Asn2gbWorkPtr awp,
  Boolean onePartOfSeg
);
NLM_EXTERN void AddSourceBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddOrganismBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddCommentBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddPrimaryBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddFeatHeaderBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddSourceFeatBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddWGSBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddGenomeBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddContigBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddBasecountBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddOriginBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddSequenceBlock (
  Asn2gbWorkPtr awp
);
NLM_EXTERN void AddSlashBlock (
  Asn2gbWorkPtr awp
);

NLM_EXTERN CharPtr DefaultFormatBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
);
NLM_EXTERN CharPtr FormatSourceBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
);
NLM_EXTERN CharPtr FormatOrganismBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
);
NLM_EXTERN CharPtr FormatReferenceBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
);
NLM_EXTERN CharPtr FormatCommentBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
);
NLM_EXTERN CharPtr FormatFeatHeaderBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
);
NLM_EXTERN CharPtr FormatSourceFeatBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
);
NLM_EXTERN CharPtr FormatFeatureBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
);
NLM_EXTERN CharPtr FormatBasecountBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
);
NLM_EXTERN CharPtr FormatSequenceBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
);
NLM_EXTERN CharPtr FormatContigBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
);
NLM_EXTERN CharPtr FormatSlashBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
);

NLM_EXTERN void PrintFtableIntervals (
  ValNodePtr PNTR head,
  BioseqPtr target,
  SeqLocPtr location,
  CharPtr label
);
NLM_EXTERN void PrintFtableLocAndQuals (
  IntAsn2gbJobPtr ajp,
  ValNodePtr PNTR head,
  BioseqPtr target,
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr context
);
NLM_EXTERN CharPtr FormatFtableSourceFeatBlock (
  BaseBlockPtr bbp,
  BioseqPtr target
);

NLM_EXTERN void DoImmediateRemoteFeatureFormat (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp,
  SeqFeatPtr sfp
);

NLM_EXTERN void DoImmediateFormat (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
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

#endif /* ndef _ASN2NGNBI_ */

