/*   asn2gnbk.c
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
* File Name:  asn2gnbk.c
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov, Jonathan Kans
*
* Version Creation Date:   10/21/98
*
* $Revision: 6.329 $
*
* File Description:  New GenBank flatfile generator - work in progress
*
* Modifications:
* --------------------------------------------------------------------------
* ==========================================================================
*/

#include <ncbi.h>
#include <objall.h>
#include <objsset.h>
#include <objsub.h>
#include <objfdef.h>
#include <seqport.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <subutil.h>
#include <tofasta.h>
#include <explore.h>
#include <ffprint.h>
#include <utilpub.h>
#include <jzmisc.h>
#include <gbfeat.h>
#include <gbftdef.h>
#include <edutil.h>
#include <asn2gnbp.h>

#ifdef WIN_MAC
#if __profile__
#include <Profiler.h>
#endif
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
  Boolean             forGbRelease;
} Asn2gbFlags, PNTR Asn2gbFlagsPtr;

/* internal Asn2gbSect structure has fields on top of Asn2gbSect fields */

typedef struct int_Asn2gbSect {
  Asn2gbSect  asp;
  SeqPortPtr  spp;
} IntAsn2gbSect, PNTR IntAsn2gbSectPtr;

/* internal asn2gbjob structure has fields on top of Asn2gbJob fields */

typedef struct int_asn2gb_job {
  Asn2gbJob      ajp;
  FmtType        format;
  Asn2gbFlags    flags;
  Boolean        showFarTransl;
  Boolean        transientSeqPort;
  Boolean        masterStyle;
  Boolean        newSourceOrg;
  ValNodePtr     lockedBspList;
} IntAsn2gbJob, PNTR IntAsn2gbJobPtr;

/* structure for storing working parameters while building asn2gb_job structure */

typedef struct asn2gbwork {
  IntAsn2gbJobPtr  ajp;
  Uint2            entityID;

  FmtType          format;
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

  /* section fields needed for populating blocks */

  Asn2gbSectPtr    asp;

  BioseqPtr        target;
  BioseqPtr        parent;
  BioseqPtr        bsp;
  SeqLocPtr        slp;
  Uint2            seg;
  Int4             numsegs;
  Int4             from;
  Int4             to;

  Boolean          contig;
  Boolean          showconfeats;
  Boolean          showconsource;

  Boolean          citSubsFirst;
  Boolean          hideGeneFeats;
  Boolean          newLocusLine;

  Boolean          isGPS;
  Boolean          copyGpsCdsUp;
  Boolean          copyGpsGeneDown;

  Char             basename [SEQID_MAX_LEN];

  SeqFeatPtr       lastsfp;
  SeqAnnotPtr      lastsap;
  Int4             lastleft;
  Int4             lastright;

  Boolean          firstfeat;

  SeqSubmitPtr     ssp;
  Boolean          hup;

  Boolean          failed;
} Asn2gbWork, PNTR Asn2gbWorkPtr;

/* array for assigning biosource and feature data fields to qualifiers */
/* should be allocated to MAX (ASN2GNBK_TOTAL_SOURCE, ASN2GNBK_TOTAL_FEATUR) */

typedef union qualval {
  CharPtr        str;
  Boolean        bool;
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
} QualVal, PNTR QualValPtr;

/* structure passed to individual paragraph format functions */

typedef struct asn2gbformat {
  IntAsn2gbJobPtr ajp;
  Asn2gbSectPtr   asp;
  QualValPtr      qvp;

  FmtType         format;
} Asn2gbFormat, PNTR Asn2gbFormatPtr;


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
  Boolean     isCDS;     /* set if using IntCdsBlock */
  Boolean     firstfeat;
} IntFeatBlock, PNTR IntFeatBlockPtr;

/* internal cds block has fields on top of IntFeatBlock fields */

typedef struct int_cds_block {
  IntFeatBlock  ifb;
  FeatBlock     fb;
  CharPtr       fig;    /* figure string from pub */
  CharPtr       maploc; /* maploc string from pub */
} IntCdsBlock, PNTR IntCdsBlockPtr;


/* enumerated qualifier category definitions */

typedef enum {
  Qual_class_ignore = 0,
  Qual_class_string,
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
  Qual_class_region,
  Qual_class_replace,
  Qual_class_consplice,
  Qual_class_site,
  Qual_class_L_R_B,
  Qual_class_rpt,
  Qual_class_organelle,
  Qual_class_orgmod,
  Qual_class_subsource,
  Qual_class_code_break,
  Qual_class_anti_codon,
  Qual_class_codon,
  Qual_class_method,
  Qual_class_pubset,
  Qual_class_db_xref,
  Qual_class_seq_id,
  Qual_class_seq_loc,
  Qual_class_its,
  Qual_class_sec_str,
  Qual_class_trna_codons,
  Qual_class_translation,
  Qual_class_protnames,
  Qual_class_illegal,
  Qual_class_note,
  Qual_class_rpt_unit,
  Qual_class_product,
  Qual_class_model_ev
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
  SCQUAL_common,
  SCQUAL_common_name,
  SCQUAL_country,
  SCQUAL_cultivar,
  SCQUAL_db_xref,
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
  SCQUAL_genotype,
  SCQUAL_germline,
  SCQUAL_group,
  SCQUAL_haplotype,
  SCQUAL_ins_seq_name,
  SCQUAL_isolate,
  SCQUAL_isolation_source,
  SCQUAL_lab_host,
  SCQUAL_label,
  SCQUAL_macronuclear,
  SCQUAL_map,
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

/* ordering arrays for qualifiers and note components */

static Uint1 source_qual_order [] = {
  SCQUAL_organism,

  SCQUAL_organelle,

  SCQUAL_strain,
  SCQUAL_sub_strain,
  SCQUAL_variety,
  SCQUAL_serotype,
  SCQUAL_serovar,
  SCQUAL_cultivar,
  SCQUAL_isolate,
  SCQUAL_isolation_source,
  SCQUAL_spec_or_nat_host,
  SCQUAL_sub_species,
  SCQUAL_specimen_voucher,

  SCQUAL_db_xref,

  SCQUAL_chromosome,
  SCQUAL_map,
  SCQUAL_clone,
  SCQUAL_sub_clone,
  SCQUAL_haplotype,
  SCQUAL_sex,
  SCQUAL_cell_line,
  SCQUAL_cell_type,
  SCQUAL_tissue_type,
  SCQUAL_clone_lib,
  SCQUAL_dev_stage,
  SCQUAL_frequency,

  SCQUAL_germline,
  SCQUAL_rearranged,
  SCQUAL_transgenic,
  SCQUAL_environmental_sample,

  SCQUAL_lab_host,
  SCQUAL_pop_variant,
  SCQUAL_tissue_lib,

  SCQUAL_plasmid_name,
  SCQUAL_transposon_name,
  SCQUAL_ins_seq_name,

  SCQUAL_country,

  SCQUAL_focus,

  SCQUAL_note,

  SCQUAL_sequenced_mol,
  SCQUAL_label,
  SCQUAL_usedin,
  SCQUAL_citation,
  0
};

static Uint1 source_desc_note_order [] = {
  SCQUAL_seqfeat_note,
  SCQUAL_orgmod_note,
  SCQUAL_subsource_note,

  SCQUAL_type,
  SCQUAL_sub_type,
  SCQUAL_serogroup,
  SCQUAL_pathovar,
  SCQUAL_chemovar,
  SCQUAL_biovar,
  SCQUAL_biotype,
  SCQUAL_group,
  SCQUAL_sub_group,
  SCQUAL_common,
  SCQUAL_acronym,
  SCQUAL_dosage,

  SCQUAL_authority,
  SCQUAL_forma,
  SCQUAL_forma_specialis,
  SCQUAL_ecotype,
  SCQUAL_synonym,
  SCQUAL_anamorph,
  SCQUAL_teleomorph,
  SCQUAL_breed,

  SCQUAL_genotype,
  SCQUAL_plastid_name,

  SCQUAL_segment,
  SCQUAL_endogenous_virus_name,

  SCQUAL_common_name,

  SCQUAL_zero_orgmod,
  SCQUAL_one_orgmod,
  SCQUAL_zero_subsrc,

  /* SCQUAL_old_lineage, */

  /* SCQUAL_old_name, */
  0
};

static Uint1 source_feat_note_order [] = {
  SCQUAL_unstructured,

  SCQUAL_type,
  SCQUAL_sub_type,
  SCQUAL_serogroup,
  SCQUAL_pathovar,
  SCQUAL_chemovar,
  SCQUAL_biovar,
  SCQUAL_biotype,
  SCQUAL_group,
  SCQUAL_sub_group,
  SCQUAL_common,
  SCQUAL_acronym,
  SCQUAL_dosage,

  SCQUAL_authority,
  SCQUAL_forma,
  SCQUAL_forma_specialis,
  SCQUAL_ecotype,
  SCQUAL_synonym,
  SCQUAL_anamorph,
  SCQUAL_teleomorph,
  SCQUAL_breed,

  SCQUAL_genotype,
  SCQUAL_plastid_name,

  SCQUAL_segment,
  SCQUAL_endogenous_virus_name,

  SCQUAL_seqfeat_note,
  SCQUAL_orgmod_note,
  SCQUAL_subsource_note,

  SCQUAL_common_name,

  SCQUAL_zero_orgmod,
  SCQUAL_one_orgmod,
  SCQUAL_zero_subsrc,

  /* SCQUAL_old_lineage, */

  /* SCQUAL_old_name, */
  0
};

typedef struct sourcequal {
  CharPtr     name;
  Uint1       qualclass;
} SourceQual, PNTR SourceQualPtr;

static SourceQual asn2gnbk_source_quals [ASN2GNBK_TOTAL_SOURCE] = {
  { "",                     Qual_class_ignore    },
  { "acronym",              Qual_class_orgmod    },
  { "anamorph",             Qual_class_orgmod    },
  { "authority",            Qual_class_orgmod    },
  { "biotype",              Qual_class_orgmod    },
  { "biovar",               Qual_class_orgmod    },
  { "breed",                Qual_class_orgmod    },
  { "cell_line",            Qual_class_subsource },
  { "cell_type",            Qual_class_subsource },
  { "chemovar",             Qual_class_orgmod    },
  { "chromosome",           Qual_class_subsource },
  { "citation",             Qual_class_pubset    },
  { "clone",                Qual_class_subsource },
  { "clone_lib",            Qual_class_subsource },
  { "common",               Qual_class_orgmod    },
  { "common",               Qual_class_string    },
  { "country",              Qual_class_subsource },
  { "cultivar",             Qual_class_orgmod    },
  { "db_xref",              Qual_class_db_xref   },
  { "dev_stage",            Qual_class_subsource },
  { "dosage",               Qual_class_orgmod    },
  { "ecotype",              Qual_class_orgmod    },
  { "endogenous_virus",     Qual_class_subsource },
  { "environmental_sample", Qual_class_subsource },
  { "extrachromosomal",     Qual_class_boolean   },
  { "focus",                Qual_class_boolean   },
  { "forma",                Qual_class_orgmod    },
  { "forma_specialis",      Qual_class_orgmod    },
  { "frequency",            Qual_class_subsource },
  { "genotype",             Qual_class_subsource },
  { "germline",             Qual_class_subsource },
  { "group",                Qual_class_orgmod    },
  { "haplotype",            Qual_class_subsource },
  { "insertion_seq",        Qual_class_subsource },
  { "isolate",              Qual_class_orgmod    },
  { "isolation_source",     Qual_class_subsource },
  { "lab_host",             Qual_class_subsource },
  { "label",                Qual_class_label     },
  { "macronuclear",         Qual_class_boolean   },
  { "map",                  Qual_class_subsource },
  { "note",                 Qual_class_note      },
  { "old_lineage",          Qual_class_orgmod    },
  { "old_name",             Qual_class_orgmod    },
  { "organism",             Qual_class_string    },
  { "organelle",            Qual_class_organelle },
  { "orgmod_note",          Qual_class_orgmod    },
  { "pathovar",             Qual_class_orgmod    },
  { "plasmid",              Qual_class_subsource },
  { "plastid",              Qual_class_subsource },
  { "pop_variant",          Qual_class_subsource },
  { "rearranged",           Qual_class_subsource },
  { "segment",              Qual_class_subsource },
  { "seqfeat_note",         Qual_class_string    },
  { "sequenced_mol",        Qual_class_quote     },
  { "serogroup",            Qual_class_orgmod    },
  { "serotype",             Qual_class_orgmod    },
  { "serovar",              Qual_class_orgmod    },
  { "sex",                  Qual_class_subsource },
  { "specific_host",        Qual_class_orgmod    },
  { "specimen_voucher",     Qual_class_orgmod    },
  { "strain",               Qual_class_orgmod    },
  { "sub_clone",            Qual_class_subsource },
  { "subgroup",             Qual_class_orgmod    },
  { "sub_species",          Qual_class_orgmod    },
  { "sub_strain",           Qual_class_orgmod    },
  { "subtype",              Qual_class_orgmod    },
  { "subsource_note",       Qual_class_subsource },
  { "synonym",              Qual_class_orgmod    },
  { "teleomorph",           Qual_class_orgmod    },
  { "tissue_lib",           Qual_class_subsource },
  { "tissue_type",          Qual_class_subsource },
  { "transgenic",           Qual_class_subsource },
  { "transposon",           Qual_class_subsource },
  { "type",                 Qual_class_orgmod    },
  { "unstructured",         Qual_class_valnode   },
  { "usedin",               Qual_class_quote     },
  { "variety",              Qual_class_orgmod    },
  { "?",                    Qual_class_orgmod    },
  { "?",                    Qual_class_orgmod    },
  { "?",                    Qual_class_subsource },
};

static Int2 subSourceToSourceIdx [30] = {
  SCQUAL_zero_subsrc,
  SCQUAL_chromosome,
  SCQUAL_map,
  SCQUAL_clone,
  SCQUAL_sub_clone,
  SCQUAL_haplotype,
  SCQUAL_genotype,
  SCQUAL_sex,
  SCQUAL_cell_line,
  SCQUAL_cell_type,
  SCQUAL_tissue_type,
  SCQUAL_clone_lib,
  SCQUAL_dev_stage,
  SCQUAL_frequency,
  SCQUAL_germline,
  SCQUAL_rearranged,
  SCQUAL_lab_host,
  SCQUAL_pop_variant,
  SCQUAL_tissue_lib,
  SCQUAL_plasmid_name,
  SCQUAL_transposon_name,
  SCQUAL_ins_seq_name,
  SCQUAL_plastid_name,
  SCQUAL_country,
  SCQUAL_segment,
  SCQUAL_endogenous_virus_name,
  SCQUAL_transgenic,
  SCQUAL_environmental_sample,
  SCQUAL_isolation_source,
  SCQUAL_subsource_note
};

typedef enum {
  FTQUAL_allele = 1,
  FTQUAL_anticodon,
  FTQUAL_bound_moiety,
  FTQUAL_cds_product,
  FTQUAL_citation,
  FTQUAL_clone,
  FTQUAL_coded_by,
  FTQUAL_codon,
  FTQUAL_codon_start,
  FTQUAL_cons_splice,
  FTQUAL_db_xref,
  FTQUAL_direction,
  FTQUAL_EC_number,
  FTQUAL_evidence,
  FTQUAL_exception,
  FTQUAL_exception_note,
  FTQUAL_figure,
  FTQUAL_frequency,
  FTQUAL_function,
  FTQUAL_gene,
  FTQUAL_gene_desc,
  FTQUAL_gene_allele,
  FTQUAL_gene_map,
  FTQUAL_gene_syn,
  FTQUAL_gene_note,
  FTQUAL_gene_xref,
  FTQUAL_heterogen,
  FTQUAL_illegal_qual,
  FTQUAL_insertion_seq,
  FTQUAL_label,
  FTQUAL_map,
  FTQUAL_maploc,
  FTQUAL_mod_base,
  FTQUAL_modelev,
  FTQUAL_note,
  FTQUAL_number,
  FTQUAL_organism,
  FTQUAL_partial,
  FTQUAL_PCR_conditions,
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
  FTQUAL_seqfeat_note,
  FTQUAL_site,
  FTQUAL_site_type,
  FTQUAL_standard_name,
  FTQUAL_transcript_id,
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

/* ordering arrays for qualifiers and note components */

static Uint1 feat_qual_order [] = {
  FTQUAL_partial,
  FTQUAL_gene,

  FTQUAL_product,

  FTQUAL_prot_EC_number,
  FTQUAL_prot_activity,

  FTQUAL_standard_name,
  FTQUAL_coded_by,

  FTQUAL_prot_name,
  FTQUAL_region_name,
  FTQUAL_site_type,
  FTQUAL_sec_str_type,
  FTQUAL_heterogen,

  FTQUAL_note, FTQUAL_citation,

  FTQUAL_number,

  FTQUAL_pseudo,

  FTQUAL_codon_start,

  FTQUAL_anticodon,
  FTQUAL_bound_moiety,
  FTQUAL_clone,
  FTQUAL_cons_splice,
  FTQUAL_direction,
  FTQUAL_function,
  FTQUAL_evidence,
  FTQUAL_exception,
  FTQUAL_frequency,
  FTQUAL_EC_number,
  FTQUAL_gene_map,
  FTQUAL_gene_allele,
  FTQUAL_allele,
  FTQUAL_map,
  FTQUAL_mod_base,
  FTQUAL_PCR_conditions,
  FTQUAL_phenotype,
  FTQUAL_rpt_family,
  FTQUAL_rpt_type,
  FTQUAL_rpt_unit,
  FTQUAL_insertion_seq,
  FTQUAL_transposon,
  FTQUAL_usedin,

  FTQUAL_illegal_qual,

  FTQUAL_replace,

  FTQUAL_transl_except,
  FTQUAL_transl_table,
  FTQUAL_codon,
  FTQUAL_organism,
  FTQUAL_label,
  FTQUAL_cds_product,
  FTQUAL_protein_id,
  FTQUAL_transcript_id,
  FTQUAL_db_xref, FTQUAL_gene_xref,
  FTQUAL_translation,
  0
};

/*
prot_names after seqfeat_note - gi|4210642|emb|AJ005084.1|HBVAJ5084
prot_conflict after prot_desc - gi|61183|emb|V01135.1|PIVM02
figure after prot_desc - gi|400553|gb|S64006.1|
seqfeat_note after prot_desc - gi|431713|gb|L20354.1|STVPATPOLB
  but prot_desc after seqfeat_note - AF252556.1
prot_names after figure - gi|234022|gb|S56149.1|S56149
seqfeat_note after prot_conflict after figure - gi|234046|gb|S51392.1|S51392
prot_method after prot_comment (descriptor) after prot_note after prot_desc
region after seqfeat_note - gi|6554164|gb|AF043644.3|AF043644
prot_desc after prot_names - gi|6581069|gb|AF202541.1|AF202541 - cannot do !!!
gene_syn after gene_desc - gi|3386543|gb|AF079528.1|AF079528
pseudo after note - gi|6598562|gb|AC006419.3|AC006419
*/

static Uint1 feat_note_order [] = {
  FTQUAL_gene_desc,
  FTQUAL_gene_syn,
  FTQUAL_trna_codons,
  FTQUAL_prot_desc,
  FTQUAL_prot_note,
  FTQUAL_prot_comment,
  FTQUAL_prot_method,
  FTQUAL_figure,
  FTQUAL_maploc,
  FTQUAL_prot_conflict,
  FTQUAL_prot_missing,
  FTQUAL_seqfeat_note,
  FTQUAL_exception_note,
  FTQUAL_region,
  FTQUAL_selenocysteine,
  FTQUAL_prot_names,
  FTQUAL_site,
  FTQUAL_rrna_its,
  FTQUAL_xtra_prod_quals,
  FTQUAL_modelev,
  0
};

typedef struct featurqual {
  CharPtr     name;
  Uint1       qualclass;
} FeaturQual, PNTR FeaturQualPtr;

static FeaturQual asn2gnbk_featur_quals [ASN2GNBK_TOTAL_FEATUR] = {
  { "",               Qual_class_ignore       },
  { "allele",         Qual_class_quote        },
  { "anticodon",      Qual_class_anti_codon   },
  { "bound_moiety",   Qual_class_quote        },
  { "product",        Qual_class_string       },
  { "citation",       Qual_class_pubset       },
  { "clone",          Qual_class_quote        },
  { "coded_by",       Qual_class_seq_loc      },
  { "codon",          Qual_class_codon        },
  { "codon_start",    Qual_class_int          },
  { "cons_splice",    Qual_class_consplice    },
  { "db_xref",        Qual_class_db_xref      },
  { "direction",      Qual_class_L_R_B        },
  { "EC_number",      Qual_class_EC_quote     },
  { "evidence",       Qual_class_evidence     },
  { "exception",      Qual_class_string       },
  { "exception_note", Qual_class_string       },
  { "figure",         Qual_class_string       },
  { "frequency",      Qual_class_quote        },
  { "function",       Qual_class_quote        },
  { "gene",           Qual_class_sgml         },
  { "gene_desc",      Qual_class_string       },
  { "allele",         Qual_class_string       },
  { "map",            Qual_class_string       },
  { "gene_syn",       Qual_class_valnode      },
  { "gene_note",      Qual_class_string       },
  { "db_xref",        Qual_class_db_xref      },
  { "heterogen",      Qual_class_string       },
  { "illegal",        Qual_class_illegal      },
  { "insertion_seq",  Qual_class_quote        },
  { "label",          Qual_class_label        },
  { "map",            Qual_class_quote        },
  { "maploc",         Qual_class_string       },
  { "mod_base",       Qual_class_noquote      },
  { "model_evidence", Qual_class_model_ev     },
  { "note",           Qual_class_note         },
  { "number",         Qual_class_number       },
  { "organism",       Qual_class_quote        },
  { "partial",        Qual_class_boolean      },
  { "PCR_conditions", Qual_class_quote        },
  { "phenotype",      Qual_class_quote        },
  { "product",        Qual_class_product      },
  { "product",        Qual_class_quote        },
  { "function",       Qual_class_valnode      },
  { "prot_comment",   Qual_class_string       },
  { "EC_number",      Qual_class_EC_valnode   },
  { "prot_note",      Qual_class_string       },
  { "prot_method",    Qual_class_method       },
  { "prot_conflict",  Qual_class_string       },
  { "prot_desc",      Qual_class_string       },
  { "prot_missing",   Qual_class_string       },
  { "name",           Qual_class_string       },
  { "prot_names",     Qual_class_protnames    },
  { "protein_id",     Qual_class_seq_id       },
  { "pseudo",         Qual_class_boolean      },
  { "region",         Qual_class_region       },
  { "region_name",    Qual_class_string       },
  { "replace",        Qual_class_replace      },
  { "rpt_family",     Qual_class_quote        },
  { "rpt_type",       Qual_class_rpt          },
  { "rpt_unit",       Qual_class_rpt_unit     },
  { "rrna_its",       Qual_class_its          },
  { "sec_str_type",   Qual_class_sec_str      },
  { "selenocysteine", Qual_class_string       },
  { "seqfeat_note",   Qual_class_string       },
  { "site",           Qual_class_site         },
  { "site_type",      Qual_class_site         },
  { "standard_name",  Qual_class_quote        },
  { "transcript_id",  Qual_class_seq_id       },
  { "transl_except",  Qual_class_code_break   },
  { "transl_table",   Qual_class_int          },
  { "translation",    Qual_class_translation  },
  { "transposon",     Qual_class_quote        },
  { "trna_aa",        Qual_class_ignore       },
  { "trna_codons",    Qual_class_trna_codons  },
  { "usedin",         Qual_class_paren        },
  { "xtra_products",  Qual_class_xtraprds     }
};

static Boolean AllowedValQual (Uint2 featureKey, FtQualType qualKey);


/* ********************************************************************** */

/* utility functions */

/* the val node strings mechanism will be replaced by a more efficient method later  */

static CharPtr MergeValNodeStrings (
  ValNodePtr list
)

{
  size_t      len;
  CharPtr     ptr;
  CharPtr     str;
  CharPtr     tmp;
  ValNodePtr  vnp;


  if (list == NULL) return NULL;

  for (vnp = list, len = 0; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    len += StringLen (str);
  }
  if (len == 0) return NULL;

  ptr = MemNew (sizeof (Char) * (len + 2));
  if (ptr == NULL) return NULL;

  for (vnp = list, tmp = ptr; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    tmp = StringMove (tmp, str);
  }

  return ptr;
}

static void AddValNodeString (
  ValNodePtr PNTR head,
  CharPtr prefix,
  CharPtr string,
  CharPtr suffix
)

{
  Char     buf [256];
  CharPtr  freeme = NULL;
  size_t   len;
  CharPtr  newstr;
  CharPtr  strptr;

  len = StringLen (prefix) + StringLen (string) + StringLen (suffix);
  if (len == 0) return;

  if (len < sizeof (buf)) {

    /* if new string fits in stack buffer, no need to allocate */

    MemSet ((Pointer) buf, 0, sizeof (buf));
    newstr = buf;

  } else {

    /* new string bigger than stack buffer, so allocate sufficient string */

    newstr = (CharPtr) MemNew (sizeof (Char) * (len + 2));
    if (newstr == NULL) return;

    /* allocated string will be freed at end of function */

    freeme = newstr;
  }

  strptr = newstr;

  if (prefix != NULL) {
    strptr = StringMove (strptr, prefix);
  }

  if (string != NULL) {
    strptr = StringMove (strptr, string);
  }

  if (suffix != NULL) {
    strptr = StringMove (strptr, suffix);
  }

  /* currently just makes a valnode list, to be enhanced later */

  ValNodeCopyStr (head, 0, newstr);

  /* if large string was allocated, free it now */

  if (freeme != NULL) {
    MemFree (freeme);
  }
}

/*                                                                   */
/* s_isWholeWordSubstr () -- Determines if a substring that is       */
/*                           contained in another string is a whole  */
/*                           word or phrase -- i.e. is it both       */
/*                           preceded and followed by white space.   */
/*                                                                   */

static Boolean s_isWholeWordSubstr (CharPtr searchStr, CharPtr foundStr, CharPtr subStr)
{
  Char previousChar;
  Char nextChar;

  /* Is beginning of substr on a word boundary ? */

  if (searchStr != foundStr)
    {
      previousChar = *(--foundStr);
      foundStr++;
      if (!isspace(previousChar))
        return FALSE;
    }

  /* Check for case where the substring is found */
  /* at the very end of the search string.       */

  if (StringCmp(foundStr, subStr) == 0)
    return TRUE;

  /* Is end of substr on a word boundary -- i.e., is */
  /* the next character white space or punctuation?  */

  nextChar = *(foundStr + StringLen(subStr));

  if (isspace(nextChar))
    return TRUE;

  if (ispunct(nextChar))
    return TRUE;

  /* If we made it this far, the substring */
  /* is on a whole word boundary.          */

  return TRUE;
}

/* s_ValNodeListStrstr () -- Searches a valnode list to see if any   */
/*                           of its String nodes contain the given   */
/*                           string.                                 */
/*                                                                   */
/*    Returns values --                                              */
/*                                                                   */
/*          0 : No match found for search string.                    */
/*          N : Search string is a substring of node N               */
/*         -N : Node N is a substring of the search string           */

#define VALNODE_STRING 0

static Int4 s_ValNodeListStrstr (ValNodePtr valNodeList, CharPtr searchString, Boolean wholeWord)
{
  Boolean    found = FALSE;
  ValNodePtr vNode;
  CharPtr    vNodeStr;
  Int4       nodeNum = 0;
  CharPtr    foundStr;

  /* Loop through the linked list of nodes, */
  /* looking for string nodes to check.     */

  for (vNode = valNodeList;
       vNode != NULL;
       vNode = vNode->next)
    {
      nodeNum++;
      if (vNode->choice == VALNODE_STRING)
        {
          vNodeStr = (CharPtr) vNode->data.ptrvalue;

          /* Is searchstring a substring of the node's string? */

          if ((foundStr = StringStr(vNodeStr,searchString)) != NULL)
            {
              /* If whole word checking is turned */
              /* on make sure that the substring  */
              /* matches a whole word (or words). */

              if ( (wholeWord == TRUE) &&
                   (s_isWholeWordSubstr(vNodeStr, foundStr, searchString)
                    == FALSE))
                continue;

              /* Found a substring so break */

              found = TRUE;
              break;
            }

          /* Is node string a substring of the search string? */

          else if ((foundStr = StringStr(searchString, vNodeStr)) != NULL)
            {

              /* If whole word checking is turned */
              /* on make sure that the substring  */
              /* matches a whole word (or words). */

              if ( (wholeWord == TRUE) &&
                   (s_isWholeWordSubstr(searchString, foundStr, vNodeStr)
                    == FALSE))
                continue;

              /* Found a substring -- save it's number and break */

              found = TRUE;
              nodeNum = -nodeNum;
              break;
            }
        }
    }

  /* If we found a node, return it */

  if (found == TRUE)
    return nodeNum;
  else
    return 0;

}

/* s_ValNodeListDelete () - Deletes a given node N from a ValNode list */
/*                                                                     */
/*      NOTE: This is based on NodeListDelete () in ncbimisc.c but     */
/*            adds the ability to delete the first node by passing in  */
/*            a pointer to a pointer.                                  */

static Nlm_Boolean s_ValNodeListDelete (ValNodePtr PNTR headPtr, Nlm_Int2 item)

{
  ValNodePtr   prev;
  ValNodePtr   deleteNode;
  CharPtr      vNodeStr;
  CharPtr      tempStr;

  /* Check parameters */

  if ((headPtr == NULL) || (*headPtr == NULL) || (item <= 0))
    return FALSE;

  /* Logically delete the node by linking the */
  /* previous node to the following node.     */

  if (item > 1)
    {
      prev = NodeListFind (*headPtr, (Nlm_Int2)(item - 1), FALSE);
      if (prev == NULL)
        return FALSE;

      deleteNode = prev->next;
      if (deleteNode == NULL)
        return FALSE;
      prev->next = deleteNode->next;
    }

  /* Special case if we're deleting the first  */
  /* node and have to change the list pointer. */

  else
    {
      deleteNode = *headPtr;
      *headPtr = deleteNode->next;

      /* If there's a following node, it's */
      /* prefix needs to be removed.       */

      if (*headPtr != NULL)
        {
          vNodeStr = (CharPtr) (*headPtr)->data.ptrvalue;
          if (StringNCmp (vNodeStr, "; ", 2) == 0)
            {
              tempStr = (CharPtr) MemNew(StringLen(vNodeStr+2));
              StringCpy (tempStr, vNodeStr + 2);
              StringCpy (vNodeStr, tempStr);
              MemFree(tempStr);
            }
        }
    }

  /* Physically delete the node */

  Nlm_MemFree (deleteNode->data.ptrvalue);
  Nlm_MemFree (deleteNode);

  /* Return successfully */

  return TRUE;
}

/* s_AddValNodeString_NoRedund() -- Adds a string to a valnode list,  */
/*                                  but first checks to make sure     */
/*                                  that it's not redundant.          */

static void s_AddValNodeString_NoRedund (
  ValNodePtr PNTR head,
  CharPtr prefix,
  CharPtr string,
  CharPtr suffix
)

{
  Int4 isRedund;

  if (StringNICmp (string, "tRNA-", 5) == 0)
    isRedund = s_ValNodeListStrstr(*head, string+5,TRUE);
  else
    isRedund = s_ValNodeListStrstr(*head, string, TRUE);

  /* If the new string is a substring of an */
  /* already existing string, don't add it. */

  if (isRedund > 0)
    return;

  /* If a previous string is a substring, */
  /* remove it and then add the new.      */

  /* Temporarily remove 'backward' check
  else if (isRedund < 0)
    {
      s_ValNodeListDelete(head,-isRedund);
  */
      /* If we just deleted the only previous node, */
      /* then don't use the "; " prefix.            */

  /*
      if (*head == NULL)
        AddValNodeString (head, NULL, string, suffix);
      else
        AddValNodeString (head, prefix, string, suffix);
    }
  */

  /* If no redundancies, just add the string */

  else if (isRedund == 0)
    AddValNodeString (head, prefix, string, suffix);

  /* bailing on isRedund < 0 was suppress sfp->comment in gi|7715975|gb|AF203972| */

  else
    AddValNodeString (head, prefix, string, suffix);

}

/* s_AddPeriodToEnd () -- Adds a '.' to the end of a given string if */
/*                        there is not already one there.            */
/*                                                                   */
/*                        Note that this adds one character to the   */
/*                        length of the string, leading to a         */
/*                        memory overrun if space was not previously */
/*                        allocated for this.                        */

static void s_AddPeriodToEnd (CharPtr someString)
{
  Int4  len;

  if (StringHasNoText (someString)) return;
  len = StringLen (someString);
  if (len < 1) return;
  if (someString[len-1] != '.')
    {
      someString[len] = '.';
      someString[len+1] = '\0';
    }
}

/* s_RemovePeriodFromEnd () -- If the last character in a given      */
/*                             string is a '.', removes it.          */

static Boolean s_RemovePeriodFromEnd (CharPtr someString)
{
  Int4  len;

  if (StringHasNoText (someString)) return FALSE;
  len = StringLen (someString);
  if (len < 1) return FALSE;
  if (someString[len-1] == '.') {
    someString[len-1] = '\0';
    return TRUE;
  }
  return FALSE;
}

/**/
/*   isEllipsis () - Determines if a string ends in an ellipses */
/**/

static Boolean IsEllipsis (
  CharPtr str
)

{
  size_t   len;
  CharPtr  ptr;

  if (StringHasNoText (str)) return FALSE;
  len = StringLen (str);
  if (len < 3) return FALSE;
  ptr = str + len - 3;
  return (Boolean) (ptr [0] == '.' && ptr [1] == '.' && ptr [2] == '.');
}

/**/
/*   s_StringCleanup () - Performs various cosmetic cleanup functions */
/*                        on a given string.                          */
/**/

static void s_StringCleanup (CharPtr str)
{
  CharPtr p;
  CharPtr q;

  /* Remove junk from string's interior */

  p = str;
  q = str;
  while (*p) {
    if (*p == ';' && p [1] == ' ' && p [2] == ';')
      p += 2;
    else if (*p == ',' && p [1] == ' ' && p [2] == ';')
      p += 2;
    else {
      *q = *p;
      p++;
      q++;
    }
  }
  *q = '\0';

}

static Int2 gb_StartPrint (
  FmtType format,
  Int2 gb_init_indent,
  Int2 gb_cont_indent,
  CharPtr gb_label,
  Int2 gb_tab_to,
  Int2 eb_init_indent,
  Int2 eb_cont_indent,
  CharPtr eb_line_prefix,
  Boolean eb_print_xx
)

{
  Int2  result = 0;

  if (format == GENBANK_FMT || format == GENPEPT_FMT) {

    result = ff_StartPrint (gb_init_indent, gb_cont_indent, ASN2FF_GB_MAX, NULL);
    if (gb_label != NULL) {
      ff_AddString (gb_label);
    }
    if (gb_tab_to > 0) {
      TabToColumn (gb_tab_to);
    }

  } else if (format == EMBL_FMT || format == EMBLPEPT_FMT) {

    if (eb_print_xx) {
      PrintXX ();
    }
    result = ff_StartPrint (eb_init_indent, eb_cont_indent, ASN2FF_EMBL_MAX, eb_line_prefix);
  }

  return result;
}

/*                                                     */
/* s_ReplaceTildesWithSpaces () - Replace all tildes   */
/*                                with spaces in the   */
/*                                given string UNLESS: */
/*                                                     */
/*     A. The tilde is followed by a number.           */
/*       OR                                            */
/*     B. The tilde is followed by a space and then    */
/*        a number.                                    */
/*       OR                                            */
/*     C. The tilde is followed by a left paren and    */
/*        then a number.                               */
/*                                                     */

static void s_ReplaceTildesWithSpaces (CharPtr someString)
{
  while (*someString != '\0')
    {
      if ((*someString == '~') &&
          (!IS_DIGIT(*(someString+1))) &&
          (!((*(someString+1) == ' ') && IS_DIGIT(*(someString+2)))) &&
          (!((*(someString+1) == '(') && IS_DIGIT(*(someString+2)))))
        *someString = ' ';
      someString++;
    }
}

/*                                                      */
/* s_ExpandTildes () - Process tildes according to      */
/*                     the following rules.             */
/*                                                      */
/*   1) Single tilde replaced by linefeed.              */
/*   2) Double tilde replaced by single one.            */
/*                                                      */

static void s_ExpandTildes (CharPtr someString)
{
  CharPtr startOfString;

  startOfString = someString;

  /* Loop through the string checking each character */

  while (*someString != '\0')
    {
      if (*someString == '~')
        {

          /* Handle single tilde */

          if (*(someString+1) != '~')
            *someString = '\n';

          /* Handle double tilde */

          else
            {
              someString++;
              *someString = '\0';
              StringCat(startOfString,someString+1);
            }
        }
      someString++;
    }
}

static void gb_AddString (
  CharPtr prefix,
  CharPtr string,
  CharPtr suffix,
  Boolean addPeriod,
  Boolean convertQuotes,
  Int2    tildeAction
)

{
  Char     buf [256];
  Char     ch;
  CharPtr  convertHere;
  CharPtr  freeme = NULL;
  size_t   len;
  CharPtr  newstr;
  CharPtr  strptr;

  len = StringLen (prefix) + StringLen (string) + StringLen (suffix);

  if (len == 0) {
    if (addPeriod) {
      ff_AddString (".");
    }
    return;
  }

  if (len + 1 < sizeof (buf)) {

    /* if new string fits in stack buffer, no need to allocate */

    MemSet ((Pointer) buf, 0, sizeof (buf));
    newstr = buf;

  } else {

    /* new string bigger than stack buffer, so allocate sufficient string */

    newstr = (CharPtr) MemNew (sizeof (Char) * (len + 3));
    if (newstr == NULL) return;

    /* allocated string will be freed at end of function */

    freeme = newstr;
  }

  strptr = newstr;

  if (prefix != NULL) {
    strptr = StringMove (strptr, prefix);
  }

  convertHere = strptr;
  if (string != NULL) {
    strptr = StringMove (strptr, string);
  }

  /* convertQuotes retains double quotes in the prefix and suffix */

  if (convertQuotes) {
    strptr = convertHere;
    ch = *strptr;
    while (ch != '\0') {
      if (ch == '\"') {
        *strptr = '\'';
      }
      strptr++;
      ch = *strptr;
    }
  }

  if (suffix != NULL) {
    strptr = StringMove (strptr, suffix);
  }

  /* Trim junk off the end of a string */
  /* and replace it with a period.     */

  if (addPeriod) {
    for (strptr = newstr + len - 1; strptr > newstr; strptr--) {
      ch = *strptr;
      if (ch == ' ' || ch == '\t' || ch == '~' || ch == '.') {
        *strptr = '\0';
      } else {
        break;
      }
    }
    if (*strptr != '.') {
      strptr++;
      *strptr = '.';
      strptr++;
      *strptr = '\0';
    }
  }

  /* Process tildes in one of 4 possible ways */

  switch (tildeAction) {
    case TILDE_EXPAND :
      s_ExpandTildes (newstr);
      ff_AddString (newstr);
      break;
    case TILDE_OLD_EXPAND :
      ff_AddStringWithTildes (newstr);
      break;
    case TILDE_TO_SPACES :
      s_ReplaceTildesWithSpaces (newstr);
      ff_AddString (newstr);
      break;
    case TILDE_IGNORE :
    default:
      ff_AddString (newstr);
      break;
  }

  /* if large string was allocated, free it now */

  if (freeme != NULL) {
    MemFree (freeme);
  }
}

static CharPtr gb_MergeString (
  Boolean call_end_print
)

{
  if (call_end_print) {
    ff_EndPrint ();
  }

  return ff_MergeString ();
}

static CharPtr asn2gb_PrintDate (
  DatePtr dp
)

{
  Char    buf [30];
  size_t  len;

  if (dp == NULL) return NULL;

  if (DatePrint (dp, buf)) {
    if (StringICmp (buf, "Not given") != 0) {
      len = StringLen (buf);
      if (len > 0) {
        if (buf [len - 1] == '\n') {
          if (buf [len - 2] == '.') {
            buf [len - 2] = '\0';
          } else {
            buf [len - 1] = '\0';
          }
        }
      }
      return StringSave (buf);
    }
  }

  return NULL;
}

static CharPtr month_names [] = {
  "JAN", "FEB", "MAR", "APR", "MAY", "JUN",
  "JUL", "AUG", "SEP", "OCT", "NOV", "DEC",
  "??"
};

static CharPtr DateToGB (
  CharPtr buf,
  DatePtr dp,
  Boolean citSub
)

{
  Int2  day;
  Int2  month;
  Int2  year;

  if (buf != NULL) {
    *buf = '\0';
  }
  if (dp == NULL) return NULL;

  if (dp->data [0] == 0) {

    StringCpy (buf, dp->str);

  } else if (dp->data [0] == 1) {

    year = 1900 + (Int2) dp->data [1];
    month = (Int2) dp->data [2];
    day = (Int2) dp->data [3];

    if (citSub) {
      if (month < 1 || month > 12) {
        month = 13;
      }
      if (day < 1 || day > 31) {
        day = 0;
      }
    } else {
      if (month < 1 || month > 12) {
        month = 1;
      }
      if (day < 1 || day > 31) {
        day = 1;
      }
    }

    if (day < 1) {
      sprintf (buf, "??-%s-%ld",
               month_names [month-1], (long) year);
    } else if (day < 10) {
      sprintf (buf, "0%ld-%s-%ld",
               (long) day, month_names [month-1], (long) year);
    } else {
      sprintf(buf, "%ld-%s-%ld",
               (long) day, month_names [month-1], (long) year);
    }
  }

  return buf;
}


/* ********************************************************************** */

/* format functions allocate printable string for given paragraph */

static CharPtr DefaultFormatBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
)

{
  if (afp == NULL || bbp == NULL) return NULL;

  /* default format function assumes string pre-allocated by add block function */

  return StringSaveNoNull (bbp->string);
}

/* superset of http://www.ncbi.nlm.nih.gov/collab/db_xref.html and RefSeq db_xrefs */

static CharPtr legalDbXrefs [] = {
  "PIDe", "PIDd", "PIDg", "PID",
  "ATCC",
  "ATCC(in host)",
  "ATCC(dna)",
  "BDGP_EST",
  "BDGP_INS",
  "CDD",
  "CK",
  "COG",
  "dbEST",
  "dbSNP",
  "dbSTS",
  "ENSEMBL",
  "ESTLIB",
  "FANTOM_DB",
  "FLYBASE",
  "GDB",
  "GeneID",
  "GI",
  "GO",
  "InterimID",
  "LocusID",
  "MaizeDB",
  "MGD",
  "MGI",
  "MIM",
  "niaEST",
  "PIR",
  "PSEUDO",
  "RATMAP",
  "RiceGenes",
  "REMTREMBL",
  "RGD",
  "RZPD",
  "SGD",
  "SoyBase",
  "SPTREMBL",
  "SWISS-PROT",
  "taxon",
  "UniGene",
  "UniSTS",
  NULL
};

static CharPtr legalRefSeqDbXrefs [] = {
  "WormBase",
  NULL
};

static CharPtr organellePrefix [] = {
  NULL,
  NULL,
  "Chloroplast ",
  "Chromoplast ",
  "Kinetoplast ",
  "Mitochondrion ",
  "Plastid ",
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  "Cyanelle ",
  NULL,
  NULL,
  "Nucleomorph ",
  "Apicoplast ",
  "Leucoplast ",
  "Proplastid ",
  NULL
};

static CharPtr FormatSourceBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
)

{
  CharPtr            acr = NULL;
  Boolean            addPeriod = TRUE;
  IntAsn2gbJobPtr    ajp;
  CharPtr            ana = NULL;
  BioSourcePtr       biop = NULL;
  CharPtr            com = NULL;
  CharPtr            common = NULL;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  GBBlockPtr         gbp = NULL;
  Uint1              genome;
  ValNodePtr         head = NULL;
  CharPtr            lineage = NULL;
  ValNodePtr         mod = NULL;
  OrgModPtr          omp = NULL;
  OrgNamePtr         onp;
  CharPtr            organelle = NULL;
  OrgRefPtr          orp;
  SeqDescrPtr        sdp;
  CharPtr            second = NULL;
  SeqFeatPtr         sfp;
  CharPtr            str;
  CharPtr            syn = NULL;
  CharPtr            taxname = NULL;
  CharPtr            tel = NULL;
  ValNodePtr         vnp;

  if (afp == NULL || bbp == NULL) return NULL;
  ajp = afp->ajp;
  if (ajp == NULL) return NULL;

  if (! StringHasNoText (bbp->string)) return StringSave (bbp->string);

  if (bbp->itemtype == OBJ_SEQDESC) {
    sdp = SeqMgrGetDesiredDescriptor (bbp->entityID, NULL, bbp->itemID, 0, NULL, &dcontext);
    if (sdp != NULL) {
      if (dcontext.seqdesctype == Seq_descr_source) {
        biop = (BioSourcePtr) sdp->data.ptrvalue;
      } else if (dcontext.seqdesctype == Seq_descr_genbank) {
        gbp = (GBBlockPtr) sdp->data.ptrvalue;
      }
    }
  } else if (bbp->itemtype == OBJ_SEQFEAT) {
    sfp = SeqMgrGetDesiredFeature (bbp->entityID, NULL, bbp->itemID, 0, NULL, &fcontext);
    if (sfp != NULL && fcontext.seqfeattype == SEQFEAT_BIOSRC) {
      biop = (BioSourcePtr) sfp->data.value.ptrvalue;
    }
  }
  if (gbp != NULL) {
    common = gbp->source;
  }

  if (biop != NULL) {
    genome = biop->genome;
    if (genome <= 19) {
      organelle = organellePrefix [genome];
    }
    orp = biop->org;
    if (orp != NULL) {
      taxname = orp->taxname;
      common = orp->common;
      mod = orp->mod;
      onp = orp->orgname;
      if (onp != NULL) {
        lineage = onp->lineage;
        omp = onp->mod;

        if (ajp->newSourceOrg) {
          for (omp = onp->mod; omp != NULL; omp = omp->next) {
            switch (omp->subtype) {
              case ORGMOD_common :
                com = omp->subname;
                break;
              case ORGMOD_acronym :
                acr = omp->subname;
                break;
              case ORGMOD_synonym :
                syn = omp->subname;
                break;
              case ORGMOD_anamorph :
                ana = omp->subname;
                break;
              case ORGMOD_teleomorph :
                tel = omp->subname;
                break;
              default :
                break;
            }
          }
        }
      }
      vnp = orp->syn;
      if (vnp != NULL) {
        second = (CharPtr) vnp->data.ptrvalue;
      }
      if (StringHasNoText (second)) {
        second = ana;
      }
      if (StringHasNoText (second)) {
        second = tel;
      }
      if (StringHasNoText (second)) {
        second = common;
      }
      if (StringHasNoText (second)) {
        second = syn;
      }
      if (StringHasNoText (second)) {
        second = acr;
      }
      if (StringHasNoText (second)) {
        second = com;
      }
    }
  }

  /* If the organelle prefix is already on the */
  /* name, don't add it.                       */

  if (StringNCmp (organelle, taxname, StringLen (organelle)) == 0)
    organelle = "";

  if (StringHasNoText (common)) {
    common = taxname;
  }
  if (StringHasNoText (common)) {
    common = "Unknown.";
  }
  if (StringHasNoText (taxname)) {
    taxname = "Unknown.";
  }
  if (StringHasNoText (lineage)) {
    lineage = "Unclassified.";
  }

  if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {

    if (ajp->newSourceOrg) {

      if (! StringHasNoText (organelle)) {
        AddValNodeString (&head, NULL, common, " ");
      }
      AddValNodeString (&head, NULL, taxname, NULL);
      if (! StringHasNoText (second)) {
        AddValNodeString (&head, " (", second, ")");
      }
      addPeriod = FALSE;

    } else {

      AddValNodeString (&head, NULL, common, NULL);
      while (mod != NULL) {
        str = (CharPtr) mod->data.ptrvalue;
        if (! StringHasNoText (str)) {
          AddValNodeString (&head, " ", str, NULL);
        }
        mod = mod->next;
      }
    }

    str = MergeValNodeStrings (head);

    gb_StartPrint (afp->format, 0, 12, "SOURCE", 13, 5, 5, "OS", TRUE);
    gb_AddString (NULL, str, NULL, addPeriod, FALSE, TILDE_TO_SPACES);

    ValNodeFreeData (head);
    MemFree (str);

  } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {

    gb_StartPrint (afp->format, 0, 12, "SOURCE", 13, 5, 5, "OS", TRUE);
    gb_AddString (organelle, taxname, NULL, FALSE, FALSE, TILDE_TO_SPACES);
    gb_AddString (" (", common, ")", FALSE, FALSE, TILDE_TO_SPACES);
  }

  ff_EndPrint();

  return gb_MergeString (FALSE);
}

static CharPtr FormatOrganismBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
)

{
  BioSourcePtr       biop = NULL;
  CharPtr            common = NULL;
  DbtagPtr           dbt;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  Uint1              genome;
  CharPtr            lineage = NULL;
  ObjectIdPtr        oip;
  OrgNamePtr         onp;
  CharPtr            organelle = NULL;
  OrgRefPtr          orp;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;
  Int4               taxid = -1;
  CharPtr            taxname = NULL;
  ValNodePtr         vnp;

  if (afp == NULL || bbp == NULL) return NULL;

  if (! StringHasNoText (bbp->string)) return StringSave (bbp->string);

  if (bbp->itemtype == OBJ_SEQDESC) {
    sdp = SeqMgrGetDesiredDescriptor (bbp->entityID, NULL, bbp->itemID, 0, NULL, &dcontext);
    if (sdp != NULL && dcontext.seqdesctype == Seq_descr_source) {
      biop = (BioSourcePtr) sdp->data.ptrvalue;
    }
  } else if (bbp->itemtype == OBJ_SEQFEAT) {
    sfp = SeqMgrGetDesiredFeature (bbp->entityID, NULL, bbp->itemID, 0, NULL, &fcontext);
    if (sfp != NULL && fcontext.seqfeattype == SEQFEAT_BIOSRC) {
      biop = (BioSourcePtr) sfp->data.value.ptrvalue;
    }
  }
  if (biop != NULL) {
    genome = biop->genome;
    if (genome <= 19) {
      organelle = organellePrefix [genome];
    }
    orp = biop->org;
    if (orp != NULL) {
      taxname = orp->taxname;
      common = orp->common;
      onp = orp->orgname;
      if (onp != NULL) {
        lineage = onp->lineage;
      }
      for (vnp = orp->db; vnp != NULL; vnp = vnp->next) {
        dbt = (DbtagPtr) vnp->data.ptrvalue;
        if (dbt == NULL) continue;
        if (StringCmp (dbt->db, "taxon") == 0) {
          oip = dbt->tag;
          if (oip != NULL) {
            taxid = oip->id;
          }
        }
      }
    }
  }

  /* If the organelle prefix is already on the */
  /* name, don't add it.                       */

  if (StringNCmp (organelle, taxname, StringLen (organelle)) == 0)
    organelle = "";

  if (StringHasNoText (common)) {
    common = taxname;
  }
  if (StringHasNoText (common)) {
    common = "Unknown.";
  }
  if (StringHasNoText (taxname)) {
    taxname = "Unknown.";
  }
  if (StringHasNoText (lineage)) {
    lineage = "Unclassified.";
  }

  if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {

    gb_StartPrint (afp->format, 2, 12, "ORGANISM", 13, 5, 5, "OC", FALSE);
    ff_AddString (organelle);
    if (StringNICmp (taxname, "Unknown", 7) != 0) {
      www_organism (taxname, taxid);
    } else {
      ff_AddString (taxname);
    }
    /*
    gb_AddString (organelle, taxname, NULL, FALSE, FALSE, TILDE_TO_SPACES);
    */
    ff_EndPrint();

    gb_StartPrint (afp->format, 12, 12, NULL, 0, 5, 5, "OC", FALSE);
    gb_AddString (NULL, lineage, NULL, TRUE, FALSE, TILDE_TO_SPACES);
    ff_EndPrint();

  } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {

    gb_StartPrint (afp->format, 12, 12, NULL, 0, 5, 5, "OC", FALSE);
    gb_AddString (NULL, lineage, NULL, TRUE, FALSE, TILDE_TO_SPACES);
    ff_EndPrint();
  }

  return ff_print_string_mem (gb_MergeString (FALSE));
}

/* format references section */

static AuthListPtr GetAuthListPtr (
  PubdescPtr pdp,
  CitSubPtr csp
)

{
  AuthListPtr  alp = NULL;
  CitArtPtr    cap;
  CitBookPtr   cbp;
  CitGenPtr    cgp;
  CitPatPtr    cpp;
  ValNodePtr   vnp;

  if (csp != NULL) {
    alp = csp->authors;
    if (alp != NULL) return alp;
  }
  if (pdp == NULL) return NULL;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    switch (vnp->choice) {
      case PUB_Gen :
        cgp = (CitGenPtr) vnp->data.ptrvalue;
        if (cgp != NULL) {
          alp = cgp->authors;
        }
        break;
      case PUB_Sub :
        csp = (CitSubPtr) vnp->data.ptrvalue;
        if (csp != NULL) {
          alp = csp->authors;
        }
        break;
      case PUB_Article :
        cap = (CitArtPtr) vnp->data.ptrvalue;
        if (cap != NULL) {
          alp = cap->authors;
        }
        break;
      case PUB_Book :
      case PUB_Proc :
      case PUB_Man :
        cbp = (CitBookPtr) vnp->data.ptrvalue;
        if (cbp != NULL) {
          alp = cbp->authors;
        }
        break;
      case PUB_Patent :
        cpp = (CitPatPtr) vnp->data.ptrvalue;
        if (cpp != NULL) {
          alp = cpp->authors;
        }
        break;
      default :
        break;
    }

    if (alp != NULL) return alp;
  }

  return NULL;
}

static CharPtr MakeSingleAuthorString (
  FmtType format,
  CharPtr prefix,
  CharPtr name,
  CharPtr initials,
  CharPtr suffix
)

{
  Char     ch;
  Char     dummy [10];
  size_t   len;
  CharPtr  ptr;
  CharPtr  str;
  CharPtr  tmp;

  if (name == NULL) return NULL;

  /* !!! clean up 'et al' as (presumably) last author !!! */

  /* !!! temporary to suppress diff !!! */
  {
  if (StringLen (name) <= 6 &&
      (StringNICmp (name, "et al", 5) == 0 || StringNICmp (name, "et,al", 5) == 0)) {
    if (StringCmp (prefix, " and ") == 0) {
      prefix = NULL;
      dummy [0] = ' ';
      StringNCpy_0 (dummy + 1, name, sizeof (dummy) - 1);
      name = dummy;
    }
  }
  }
  /*
  if (StringLen (name) <= 6 &&
      (StringNICmp (name, "et al", 5) == 0 || StringNICmp (name, "et,al", 5) == 0)) {
    name = "et al.";
    if (StringCmp (prefix, " and ") == 0) {
      prefix = ", ";
    }
  }
  */

  len = StringLen (name) + StringLen (initials) + StringLen (suffix) + StringLen (prefix);
  str = MemNew (sizeof (Char) * (len + 4));
  if (str == NULL) return NULL;

  ptr = str;
  if (! StringHasNoText (prefix)) {
    ptr = StringMove (ptr, prefix);
  }

  /* initials and suffix to support structured name fields */

  tmp = StringMove (ptr, name);
  if (! StringHasNoText (initials)) {
    tmp = StringMove (tmp, ",");
    tmp = StringMove (tmp, initials);
  }
  if (! StringHasNoText (suffix)) {
    tmp = StringMove (tmp, " ");
    tmp = StringMove (tmp, suffix);
  }

  /* if embl, remove commas in individual names, starting after prefix */

  if (format == EMBL_FMT || format == EMBLPEPT_FMT) {
    tmp = ptr;
    ch = *tmp;
    while (ch != '\0') {
      if (ch == ',') {
        *tmp = ' ';
      }
      tmp++;
      ch = *tmp;
    }
  }

  return str;
}

static CharPtr GetAuthorsString (
  FmtType format,
  AuthListPtr alp
)

{
  AuthorPtr    ap;
  Int2         count;
  ValNodePtr   head = NULL;
  ValNodePtr   names;
  ValNodePtr   next;
  NameStdPtr   nsp;
  PersonIdPtr  pid;
  CharPtr      prefix = NULL;
  CharPtr      str;
  ValNodePtr   vnp;

  if (alp == NULL) return NULL;

  count = 0;
  if (alp->choice == 1) {
    for (names = alp->names; names != NULL; names = names->next) {
      next = names->next;
      if (next == NULL) {
        if (format == GENBANK_FMT || format == GENPEPT_FMT) {
          if (count == 0) {
            prefix = NULL;
          } else {
            prefix = " and ";
          }
        }
      }
      str = NULL;
      ap = (AuthorPtr) names->data.ptrvalue;
      if (ap != NULL) {
        pid = ap->name;
        if (pid != NULL) {
          if (pid->choice == 2) {
            nsp = (NameStdPtr) pid->data;
            if (nsp != NULL) {
              if (! StringHasNoText (nsp->names [0])) {
                str = MakeSingleAuthorString (format, prefix, nsp->names [0], nsp->names [4], nsp->names [5]);
              } else if (! StringHasNoText (nsp->names [3])) {
                str = MakeSingleAuthorString (format, prefix, nsp->names [3], NULL, NULL);
              }
            }
          } else if (pid->choice == 3 || pid->choice == 4) {
            str = MakeSingleAuthorString (format, prefix, (CharPtr) pid->data, NULL, NULL);
          }
        }
      }
      if (str != NULL) {
        ValNodeCopyStr (&head, 0, str);
        count++;
      }
      prefix = ", ";
    }
  } else if (alp->choice == 2 || alp->choice == 3) {
    for (vnp = alp->names; vnp != NULL; vnp = vnp->next) {
      next = vnp->next;
      if (next == NULL) {
        if (format == GENBANK_FMT || format == GENPEPT_FMT) {
          if (count == 0) {
            prefix = NULL;
          } else {
            prefix = " and ";
          }
        }
      }
      str = MakeSingleAuthorString (format, prefix, (CharPtr) vnp->data.ptrvalue, NULL, NULL);
      if (str != NULL) {
        ValNodeCopyStr (&head, 0, str);
        count++;
      }
      prefix = ", ";
    }
  }

  str = MergeValNodeStrings (head);

  ValNodeFreeData (head);

  return str;
}

/*
Strips all spaces in string in following manner. If the function
meet several spaces (spaces and tabs) in succession it replaces them
with one space. Strips all spaces after '(' and before ')'
*/

static void StrStripSpaces (
  CharPtr str
)

{
  CharPtr  new_str;

  if (str == NULL) return;

  new_str = str;
  while (*str != '\0') {
    *new_str++ = *str;
    if (*str == ' ' || *str == '\t' || *str == '(') {
      for (str++; *str == ' ' || *str == '\t'; str++) continue;
      if (*str == ')' || *str == ',') {
        new_str--;
      }
    } else {
      str++;
    }
  }
  *new_str = '\0';
}

static Boolean AllCaps (
  CharPtr p
)

{
  if (p == NULL) return FALSE;

  for (p++; p != NULL && *p != '\0'; p++) {
    if (IS_LOWER (*p)) return FALSE;
  }
  return TRUE;
}

static void CleanEquals (
  CharPtr p
)

{
  if (p == NULL) return;

  for (; *p != '\0'; p++) {
    if (*p == '\"') {
      *p = '\'';
    }
  }
}

static CharPtr GetPubTitle (
  FmtType format,
  PubdescPtr pdp,
  CitSubPtr csp
)

{
  CitArtPtr        cap;
  CitBookPtr       cbp;
  CitGenPtr        cgp;
  Char             ch;
  CitPatPtr        cpp;
  MedlineEntryPtr  mep;
  CharPtr          ptr;
  CharPtr          title = NULL;
  ValNodePtr       ttl = NULL;
  ValNodePtr       vnp;

  if (csp != NULL) {
    if (format == GENBANK_FMT || format == GENPEPT_FMT) {
      title = "Direct Submission";
      return StringSave (title);
    } else if (format == EMBL_FMT || format == EMBLPEPT_FMT) {
      return NULL;
    }
  }
  if (pdp == NULL) return NULL;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    switch (vnp->choice) {
      case PUB_Gen :
        cgp = (CitGenPtr) vnp->data.ptrvalue;
        if (cgp != NULL) {
          if (! StringHasNoText (cgp->title)) return StringSave (cgp->title);
          if (! StringHasNoText (cgp->cit)) {
            ptr = StringStr (cgp->cit, "Title=\"");
            if (ptr != NULL) {
              title = StringSave (ptr + 7);
              for (ptr = title; *ptr != '\0'; ptr++) {
                if (*ptr == '"') {
                  *ptr = '\0';
                  break;
                }
              }
              return title;
            }
          }
        }
        break;
      case PUB_Sub :
        csp = (CitSubPtr) vnp->data.ptrvalue;
        if (csp != NULL) {
          if (format == GENBANK_FMT || format == GENPEPT_FMT) {
            title = "Direct Submission";
            return StringSave (title);
          } else if (format == EMBL_FMT || format == EMBLPEPT_FMT) {
            return NULL;
          }
        }
        break;
      case PUB_Medline :
        mep = (MedlineEntryPtr) vnp->data.ptrvalue;
        if (mep != NULL) {
          cap = mep->cit;
          if (cap != NULL) {
            ttl = cap->title;
          }
        }
        break;
      case PUB_Article :
        cap = (CitArtPtr) vnp->data.ptrvalue;
        if (cap != NULL) {
          ttl = cap->title;
        }
        break;
      case PUB_Book :
      case PUB_Proc :
      case PUB_Man :
        cbp = (CitBookPtr) vnp->data.ptrvalue;
        if (cbp != NULL) {
          ttl = cbp->title;
          if (ttl != NULL) {
            title = (CharPtr) ttl->data.ptrvalue;
            if (! StringHasNoText (title)) {
              title = StringSave (title);
              if (StringLen (title) > 3) {
                ch = *title;
                if (IS_LOWER (ch)) {
                  *title = TO_UPPER (ch);
                }
                ptr = title;
                if (AllCaps (ptr)) {
                  for (ptr++; ptr != NULL && *ptr != '\0'; ptr++) {
                    ch = *ptr;
                    *ptr = TO_LOWER (ch);
                  }
                }
              }
              return title;
            }
          }
        }
        break;
      case PUB_Patent :
        cpp = (CitPatPtr) vnp->data.ptrvalue;
        if (cpp != NULL) {
          title = cpp->title;
          if (! StringHasNoText (title)) {
            return StringSave (title);
          }
        }
        break;
      default :
        break;
    }

    if (ttl != NULL) {
      title = (CharPtr) ttl->data.ptrvalue;
      if (! StringHasNoText (title)) {
        return StringSave (title);
      }
    }
  }

  return NULL;
}

static void CleanPubTitle (
  CharPtr title
)

{
  CharPtr  p;
  Boolean  remove_it;

  if (title == NULL) return;

  CleanEquals (title);

  for (p = title + StringLen (title) - 1; p > title + 2; p--) {
    if (*p == ' ') {
      *p = '\0';
    } else if (*p == '.') {
      remove_it = FALSE;
      if (p > title + 5) {
        if (*(p - 1) != '.' || *(p - 2) != '.') {
          remove_it = TRUE;
        }
      }
      if (remove_it) {
        *p = '\0';
      }
      break;
    } else {
      break;
    }
  }
}

/* !!! GetPubJournal needs to be implemented !!! */

/*
medline type page numbering is expanded (e.g., 125-35 -> 125-135,
F124-34 -> F124-F134, 12a-c -> 12a-12c).
If only one page is given, this is output without a dash.
Expanded numbering is validated to ensure that the
first number is smaller than or equal to the second and
that the first letter is less than or identical to the second
(i.e., a < c).  If the input is all letters (i.e., roman numerals)
this is not validated.

Return values:
 0 : valid page numbering.
-1 : invalid page numbering.
*/

#define MAX_PAGE_DIGITS 12

static Int2 FixPages (
  CharPtr out_pages,
  CharPtr in_pages
)

{
  Boolean dash=TRUE, first_alpha;
  Char firstbegin[MAX_PAGE_DIGITS];
  Char secondbegin[MAX_PAGE_DIGITS];
  Char firstend[MAX_PAGE_DIGITS];
  Char secondend[MAX_PAGE_DIGITS];
  Char temp[MAX_PAGE_DIGITS];
  CharPtr alphabegin, numbegin, alphaend, numend, ptr, in=in_pages;
  Int2 diff, index, retval=0;
  Int2 length_nb, length_ab, length_ne, length_ae;
  Int4 num1=0, num2=0;

  if (in_pages == NULL) return retval;

  while (*in != '\0')
  {      /* Check for digits in input*/
    if (IS_DIGIT(*in))
      break;
    in++;
  }

  if (*in == '\0' || (in != in_pages && *(in-1) == ' '))
  {    /* if all letters (i.e. roman numerals), put out. */
    out_pages = StringCpy(out_pages, in_pages);
    return retval;
  }

  in = in_pages;
  if (IS_DIGIT(*in))
  {      /* Do digits come first? */
    first_alpha = FALSE;
    index=0;
    while (IS_DIGIT(*in) || *in == ' ')
    {
      firstbegin[index] = *in;
      if (*in != ' ')
        index++;
      in++;
      if (*in == '-')
        break;

    }
    firstbegin[index] = '\0';
    index=0;
    if (*in != '-')
    {    /* After digits look for letters. */
      while (IS_ALPHA(*in)  || *in == ' ')
      {
        secondbegin[index] = *in;
        index++;
        in++;
        if (*in == '-')
          break;
      }
    }
    secondbegin[index] = '\0';
    if (*in == '-')    /* if dash is not present, note */
      in++;
    else
      dash=FALSE;
    index=0;
    while (IS_DIGIT(*in) || *in == ' ')
    {      /* Look for digits.  */
      firstend[index] = *in;
      if (*in != ' ')
        index++;
      in++;
    }
    firstend[index] = '\0';
    index=0;
    if (*in != '\0')
    {      /* Look for letters again. */
      while (IS_ALPHA(*in)  || *in == ' ')
      {
        secondend[index] = *in;
        index++;
        in++;
      }
    }
    secondend[index] = '\0';
  }
  else
  {      /* Do letters come first? */
    first_alpha = TRUE;
    index=0;
    while (IS_ALPHA(*in) || *in == ' ')
    {
      firstbegin[index] = *in;
      index++;
      in++;
      if (*in == '-')
        break;
    }
    firstbegin[index] = '\0';
    index=0;
    if (*in != '-')
    {    /* After letters look for digits.   */
      while (IS_DIGIT(*in)  || *in == ' ')
      {
        secondbegin[index] = *in;
        if (*in != ' ')
          index++;
        in++;
        if (*in == '-')
          break;
      }
    }
    secondbegin[index] = '\0';
    if (*in == '-')    /* Note if dash is missing. */
      in++;
    else
      dash=FALSE;
    index=0;
    while (IS_ALPHA(*in) || *in == ' ')
    {    /* Look for letters again. */
      firstend[index] = *in;
      index++;
      in++;
    }
    firstend[index] = '\0';
    index=0;
    if (*in != '\0')
    {    /* Any digits here? */
      while (IS_DIGIT(*in)  || *in == ' ')
      {
        secondend[index] = *in;
        if (*in != ' ')
          index++;
        in++;
      }
    }
    secondend[index] = '\0';
  }

  if (first_alpha)
  {
    alphabegin = firstbegin;
    numbegin = secondbegin;
    alphaend = firstend;
    numend = secondend;
  }
  else
  {
    numbegin = firstbegin;
    alphabegin = secondbegin;
    numend = firstend;
    alphaend = secondend;
  }

  length_nb = StringLen(numbegin);
  length_ab = StringLen(alphabegin);
  length_ne = StringLen(numend);
  length_ae = StringLen(alphaend);

  /* If no dash, but second letters or numbers present, reject. */
  if (dash == FALSE)
  {
    if (length_ne != 0 || length_ae != 0)
      retval = -1;
  }
  /* Check for situations like "AAA-123" or "222-ABC". */
  if (dash == TRUE)
  {
    if (length_ne == 0 && length_ab == 0)
      retval = -1;
    else if (length_ae == 0 && length_nb == 0)
      retval = -1;
  }

  /* The following expands "F502-512" into "F502-F512" and
  checks, for entries like "12a-12c" that a > c.  "12aa-12ab",
  "125G-137A", "125-G137" would be rejected. */
  if (retval == 0)
  {
    if (length_ab > 0)
    {
      if (length_ae > 0)
      {
        if (StringCmp(alphabegin, alphaend) != 0)
        {
          if (length_ab != 1 || length_ae != 1)
            retval = -1;
          else if (*alphabegin > *alphaend)
            retval = -1;
        }
      }
      else
      {
        alphaend = alphabegin;
        length_ae = length_ab;
      }
    }
    else if (length_ae > 0)
      retval = -1;
  }

/* The following expands "125-37" into "125-137".  */
  if (retval == 0)
  {
    if (length_nb > 0)
    {
      if (length_ne > 0)
      {
        diff = length_nb - length_ne;
        if (diff > 0)
        {
          index=0;
          while (numend[index] != '\0')
          {
            temp[index+diff] = numend[index];
            index++;
          }
          temp[index+diff] = numend[index];
          for (index=0; index<diff; index++)
            temp[index] = numbegin[index];
          index=0;
          while (temp[index] != '\0')
          {
            numend[index] = temp[index];
            index++;
          }
          numend[index] = temp[index];
        }
      }
      else
      {
        numend = numbegin;
        length_ne = length_nb;
      }

    }
    else if (length_ne > 0)
      retval = -1;
  /* Check that the first number is <= the second (expanded) number. */
    if (retval == 0)
    {
  /*    sscanf(numbegin, "%ld", &num_type);
      num1 = (Int4) num_type;
      sscanf(  numend, "%ld", &num_type);
      num2 = (Int4) num_type;
  */
      num1 = (Int4) atol(numbegin);
      num2 = (Int4) atol(numend);
      if (num2 < num1)
        retval = -1;
    }
  }

  if (retval == -1)
  {
    out_pages = StringCpy(out_pages, in_pages);
  }
  else
  {
    ptr = out_pages;
  /* Place expanded and validated page numbers into "out_pages". */
    if (first_alpha)
    {
      while (*alphabegin != '\0')
      {
        *ptr = *alphabegin;
        alphabegin++;
        ptr++;
      }
      while (*numbegin != '\0')
      {
        *ptr = *numbegin;
        numbegin++;
        ptr++;
      }
      if (dash == TRUE)
      {
        *ptr = '-';
        ptr++;
        while (*alphaend != '\0')
        {
          *ptr = *alphaend;
          alphaend++;
          ptr++;
        }
        while (*numend != '\0')
        {
          *ptr = *numend;
          numend++;
          ptr++;
        }
      }
      *ptr = '\0';
    }
    else
    {
      while (*numbegin != '\0')
      {
        *ptr = *numbegin;
        numbegin++;
        ptr++;
      }
      while (*alphabegin != '\0')
      {
        *ptr = *alphabegin;
        alphabegin++;
        ptr++;
      }
      if (dash == TRUE)
      {
        *ptr = '-';
        ptr++;
        while (*numend != '\0')
        {
          *ptr = *numend;
          numend++;
          ptr++;
        }
        while (*alphaend != '\0')
        {
          *ptr = *alphaend;
          alphaend++;
          ptr++;
        }
      }
      *ptr = '\0';
    }
  }
  return retval;
}

/* !!! still need to add StripParanthesis equivalent !!! */

static void DoSup (
  ValNodePtr PNTR head,
  CharPtr issue,
  CharPtr part_sup,
  CharPtr part_supi
)

{
  size_t   len;
  CharPtr  str;
  CharPtr  temp;

  len = StringLen (issue) + StringLen (part_sup) + StringLen (part_supi) + 25;
  str = MemNew (sizeof (Char) * len);
  if (str == NULL) return;
  temp = str;

  if (! StringHasNoText (part_sup)) {
    *temp = ' ';
    temp++;
    temp = StringMove (temp, part_sup);
  }
  if (StringHasNoText (issue) && StringHasNoText (part_supi)) {
    ValNodeCopyStr (head, 0, str);
    MemFree (str);
    return;
  }
  *temp = ' ';
  temp++;
  *temp = '(';
  temp++;
  if (! StringHasNoText (issue)) {
    temp = StringMove (temp, issue);
  }
  if (! StringHasNoText (part_supi)) {
    *temp = ' ';
    temp++;
    temp = StringMove (temp, part_supi);
  }
  *temp = ')';
  temp++;
  ValNodeCopyStr (head, 0, str);
  MemFree (str);
}

static CharPtr FormatCitJour (
  FmtType format,
  Boolean citArtIsoJta,
  CitJourPtr cjp
)

{
  Char        buf [256];
  DatePtr     dp;
  ValNodePtr  head = NULL;
  ImprintPtr  imp;
  CharPtr     issue = NULL;
  Char        pages [128];
  CharPtr     part_sup = NULL;
  CharPtr     part_supi = NULL;
  CharPtr     rsult = NULL;
  CharPtr     title;
  ValNodePtr  ttl;
  CharPtr     volume;
  Char        year [8];

  if (cjp == NULL) return NULL;

  ttl = cjp->title;
  if (ttl == NULL) return NULL;

  /* always use iso_jta title if present */

  while (ttl != NULL && ttl->choice != Cit_title_iso_jta) {
    ttl = ttl->next;
  }

  /* release mode requires iso_jta title */

  if (ttl == NULL) {
    if (citArtIsoJta) return NULL;
    ttl = cjp->title;
  }

  imp = cjp->imp;
  if (imp == NULL) return NULL;

  dp = imp->date;
  year [0] = '\0';
  if (dp != NULL) {
    if (dp->data [0] == 1) {
      if (dp->data [1] != 0) {
        sprintf (year, " (%ld)", (long) (1900 + dp->data [1]));
      }
    } else {
      StringCpy (year, " (");
      StringNCat (year, dp->str, 4);
      StringCat (year, ")");
    }
  }

  if (imp->prepub == 1 || imp->prepub == 255) {
    sprintf (buf, "Unpublished %s", year);
    return StringSave (buf);
  }

  title = (CharPtr) ttl->data.ptrvalue;
  if (StringLen (title) < 3) return StringSave (".");

  ValNodeCopyStr (&head, 0, title);

  volume = imp->volume;
  if (format == GENBANK_FMT || format == GENPEPT_FMT) {
    issue = imp->issue;
    part_sup = imp->part_sup;
    part_supi = imp->part_supi;
  }
  pages [0] = '\0';
  FixPages (pages, imp->pages);

  if (! StringHasNoText (volume)) {
    AddValNodeString (&head, " ", volume, NULL);
  }

  if ((! StringHasNoText (volume)) || (! StringHasNoText (pages))) {
    DoSup (&head, issue, part_sup, part_supi);
  }

  if (format == GENBANK_FMT || format == GENPEPT_FMT) {
    if (! StringHasNoText (pages)) {
      AddValNodeString (&head, ", ", pages, NULL);
    }
  } else if (format == EMBL_FMT || format == EMBLPEPT_FMT) {
    if (! StringHasNoText (pages)) {
      AddValNodeString (&head, ":", pages, NULL);
    } else if (imp->prepub == 2 || (StringHasNoText (volume))) {
      ValNodeCopyStr (&head, 0, " 0:0-0");
    }
  }

  ValNodeCopyStr (&head, 0, year);

  if (format == GENBANK_FMT || format == GENPEPT_FMT) {
    if (imp->prepub == 2) {
      ValNodeCopyStr (&head, 0, " In press");
    }
  }

  rsult = MergeValNodeStrings (head);
  ValNodeFreeData (head);

  return rsult;
}

static CharPtr MakeAffilStr (
  AffilPtr afp
)

{
  ValNodePtr  head = NULL;
  CharPtr     prefix = "";
  CharPtr     rsult = NULL;

  if (afp == NULL) return NULL;

  if (! StringHasNoText (afp->affil)) {
    ValNodeCopyStr (&head, 0, afp->affil);
    prefix = ", ";
  }

  if (afp->choice == 2) {
    if (! StringHasNoText (afp->div)) {
      AddValNodeString (&head, prefix, afp->div, NULL);
      prefix = ", ";
    }
    if (! StringHasNoText (afp->street)) {
      AddValNodeString (&head, prefix, afp->street, NULL);
      prefix = ", ";
    }
    if (! StringHasNoText (afp->city)) {
      AddValNodeString (&head, prefix, afp->city, NULL);
      prefix = ", ";
    }
    if (! StringHasNoText (afp->sub)) {
      AddValNodeString (&head, prefix, afp->sub, NULL);
      prefix = ", ";
    }
    if (! StringHasNoText (afp->country)) {
      AddValNodeString (&head, prefix, afp->country, NULL);
      prefix = ", ";
    }
  }

  rsult = MergeValNodeStrings (head);
  ValNodeFreeData (head);

  return rsult;
}

static CharPtr GetAffil (
  AffilPtr afp
)

{
  Boolean need_comma=FALSE;
  CharPtr string=NULL, temp, ptr;
  Char ch;
  Int2 aflen=15;

  if (afp == NULL) return NULL;
  if (afp) {
    if (afp -> choice == 1){
      if (afp -> affil){
        aflen += StringLen(afp -> affil);
      }
    }else if (afp -> choice == 2){
      aflen += StringLen (afp -> affil) +
      StringLen (afp -> div) +
      StringLen (afp -> city) +
      StringLen (afp -> sub) +
      StringLen (afp -> street) +
      StringLen (afp -> country) + StringLen(afp->postal_code);
    }

    temp = string = MemNew(aflen);

    if ( afp -> choice == 1){
       if (afp -> affil){
        ptr = afp->affil;
        while ((*temp = *ptr) != '\0')
        {
          temp++; ptr++;
        }
       }
    }else if (afp -> choice == 2){

      if( afp -> div) {
        if (need_comma)
        {
          *temp = ','; temp++;
          *temp = ' '; temp++;
        }
        ptr = afp->div;
        while ((*temp = *ptr) != '\0')
        {
          temp++; ptr++;
        }
        need_comma = TRUE;
      }

      if(afp -> affil) {
        if (need_comma)
        {
          *temp = ','; temp++;
          *temp = ' '; temp++;
        }
        ptr = afp->affil;
        while ((*temp = *ptr) != '\0')
        {
          temp++; ptr++;
        }
        need_comma = TRUE;
      }

      if(afp -> street) {
        if (need_comma)
        {
          *temp = ','; temp++;
          *temp = ' '; temp++;
        }
        ptr = afp->street;
        while ((*temp = *ptr) != '\0')
        {
          temp++; ptr++;
        }
        need_comma = TRUE;
      }

      if( afp -> city) {
        if (need_comma)
        {
          *temp = ','; temp++;
          *temp = ' '; temp++;
        }
        ptr = afp->city;
        while ((*temp = *ptr) != '\0')
        {
          temp++; ptr++;
        }
        need_comma = TRUE;
      }

      if( afp -> sub) {
        if (need_comma)
        {
          *temp = ','; temp++;
          *temp = ' '; temp++;
        }
        ptr = afp->sub;
        while ((*temp = *ptr) != '\0')
        {
          temp++; ptr++;
        }
        need_comma = TRUE;
      }

      if( afp -> postal_code){
        *temp = ' ';
        temp++;
        ptr = afp->postal_code;
        while ((*temp = *ptr) != '\0')
        {
          temp++; ptr++;
        }
      }

      if( afp -> country){
        if (need_comma)
        {
          *temp = ','; temp++;
          *temp = ' '; temp++;
        }
        ptr = afp->country;
        while ((*temp = *ptr) != '\0')
        {
          temp++; ptr++;
        }
        need_comma = TRUE;
      }
    }
    temp++;
    *temp = '\0';
  }

    /* convert double quotes to single quotes */

    ptr = string;
    ch = *ptr;
    while (ch != '\0') {
      if (ch == '\"') {
        *ptr = '\'';
      }
      ptr++;
      ch = *ptr;
    }

  return string;
}

static CharPtr FormatCitBookArt (
  FmtType format,
  CitBookPtr cbp
)

{
  AffilPtr     afp;
  AuthListPtr  alp;
  CharPtr      book_title = NULL;
  Char         buf [256];
  Char         ch;
  DatePtr      dp;
  ValNodePtr   head = NULL;
  ImprintPtr   imp;
  CharPtr      issue = NULL;
  ValNodePtr   names = NULL;
  Char         pages [128];
  CharPtr      part_sup = NULL;
  CharPtr      part_supi = NULL;
  CharPtr      rsult = NULL;
  CharPtr      str;
  CharPtr      title;
  ValNodePtr   ttl;
  ValNodePtr   vnp;
  CharPtr      volume;
  Char         year [8];

  if (cbp == NULL) return NULL;

  ttl = cbp->title;
  if (ttl == NULL) return NULL;

  imp = cbp->imp;
  if (imp == NULL) return NULL;

  dp = imp->date;
  year [0] = '\0';
  if (dp != NULL) {
    if (dp->data [0] == 1) {
      if (dp->data [1] != 0) {
        sprintf (year, "(%ld)", (long) (1900 + dp->data [1]));
      }
    } else {
      StringCpy (year, "(");
      StringNCat (year, dp->str, 4);
      StringCpy (year, ")");
    }
  }

  if (imp->prepub == 1 || imp->prepub == 255) {
    sprintf (buf, "Unpublished %s", year);
    return StringSave (buf);
  }

  title = (CharPtr) ttl->data.ptrvalue;
  if (StringLen (title) < 3) return StringSave (".");

  ValNodeCopyStr (&head, 0, "(in) ");

  alp = cbp->authors;
  if (alp != NULL) {
    str = GetAuthorsString (format, alp);
    if (str != NULL) {
      ValNodeCopyStr (&head, 0, str);
      names = alp->names;
      if (names != NULL) {
        if (names->next != NULL) {
          ValNodeCopyStr (&head, 0, " (Eds.);");
        } else {
          ValNodeCopyStr (&head, 0, " (Ed.);");
        }
      }
      ValNodeCopyStr (&head, 0, "\n");
    }
    MemFree (str);
  }

  book_title = StringSaveNoNull (title);
  vnp = ValNodeAddStr (&head, 0, book_title);
  if (book_title != NULL) {

    /* make book title all caps */

    title = book_title;
    ch = *title;
    while (ch != '\0') {
      *title = TO_UPPER (ch);
      title++;
      ch = *title;
    }
  }

  volume = imp->volume;
  if (format == GENBANK_FMT || format == GENPEPT_FMT) {
    issue = imp->issue;
    part_sup = imp->part_sup;
    part_supi = imp->part_supi;
  }
  pages [0] = '\0';
  FixPages (pages, imp->pages);

  if ((! StringHasNoText (volume)) && (StringCmp (volume, "0") != 0)) {
    AddValNodeString (&head, ", Vol. ", volume, NULL);
    DoSup (&head, issue, part_sup, part_supi);
  }

  if (! StringHasNoText (pages)) {
    AddValNodeString (&head, ": ", pages, NULL);
  }

  if (book_title != NULL) {
    ValNodeCopyStr (&head, 0, ";\n");
  }

  afp = imp->pub;
  if (afp != NULL) {
    str = MakeAffilStr (afp);
    if (str != NULL) {
      ValNodeCopyStr (&head, 0, str);
      ValNodeCopyStr (&head, 0, " ");
      MemFree (str);
    }
  }

  AddValNodeString (&head, NULL, year, NULL);

  if (format == GENBANK_FMT || format == GENPEPT_FMT) {
    if (imp->prepub == 2) {
      ValNodeCopyStr (&head, 0, " In press");
    }
  }

  rsult = MergeValNodeStrings (head);
  ValNodeFreeData (head);

  return rsult;
}

static CharPtr FormatCitBook (
  FmtType format,
  CitBookPtr cbp
)

{
  AffilPtr   afp;
  char       year[5];
  CharPtr    bookTitle=NULL;
  CharPtr    retval = NULL;
  CharPtr    temp;
  DatePtr    dp;
  ImprintPtr ip;
  int        aflen = 0;
  CharPtr    p;
  CharPtr    affilStr = NULL;

  /* Check parameters */

  if (cbp == NULL)
    return NULL;

  if ( cbp -> othertype != 0)
    return NULL;

  ip = cbp -> imp;

  /* Format the year */

  dp = ip -> date;
  year[0] = '\0';

  if ( dp -> data[0] == 1)
    sprintf(year,"%ld",(long) ( 1900+dp -> data[1]));
  else
    {
      StringNCpy( (CharPtr) year, (CharPtr) dp -> str, (size_t) 4);
      year[4] = '\0';
    }

  /* Get the book title */

  if (cbp->title)
    bookTitle = StringSave(cbp -> title -> data.ptrvalue);

  /* Get the affiliation length */

  if ( ip -> pub){
    afp = ip -> pub;
    aflen = StringLen(afp -> affil)+ 5;
    if ( afp -> choice == 2){
      aflen += 3 + StringLen(afp -> div);
      aflen += 3 + StringLen(afp -> street);
      aflen += 3 + StringLen(afp -> city);
      aflen += 3 + StringLen(afp -> sub);
      aflen += 3 + StringLen(afp -> country);
    }
  } else{
    aflen = 22;
  }
  if (ip->prepub == 2)
    aflen += 10;

  /* Create a Char String big enough to hold */
  /* the title, year, and affiliation.       */

  temp = retval = MemNew( (size_t) (30+StringLen( bookTitle)+StringLen( year) + aflen) );

  /* Convert the title to upper case and */
  /* add it to the string.               */

  for ( p = bookTitle; *p; p++)
    *p = TO_UPPER(*p);

  temp = StringMove(temp, "Book: ");
  temp = StringMove(temp, bookTitle);
  temp = StringMove(temp, ".");

  /* Add the affiliation to the string */

  if ( ip -> pub)
    {
      afp = ip -> pub;
      *temp = ' ';
      temp++;
      affilStr = MakeAffilStr(afp);
      temp = StringMove(temp,affilStr);
    }

  /* Add the year to the string */

  if (year[0] != '\0')
    {
      if (affilStr != NULL)
        temp = StringMove(temp," (");
      else
        temp = StringMove(temp, "(");
      temp = StringMove(temp, year);
      temp = StringMove(temp, ")");
    }

  /* If in press, add note */

  if (ip->prepub == 2)
    temp = StringMove(temp, ", In press");

  /* Clean up and return */

  if (bookTitle)
    MemFree(bookTitle);

  return retval;

}

static CharPtr FormatThesis (
  FmtType format,
  CitBookPtr cbp
)

{
  AffilPtr     afp;
  Char         ch;
  DatePtr      dp;
  ValNodePtr   head = NULL;
  ImprintPtr   imp;
  CharPtr      ptr;
  CharPtr      rsult = NULL;
  CharPtr      str;
  CharPtr      suffix = NULL;
  Char         year [8];

  if (cbp == NULL) return NULL;
  if (cbp->othertype != 2 || cbp->let_type != 3) return NULL;

  imp = cbp->imp;
  if (imp == NULL) return NULL;

  dp = imp->date;
  year [0] = '\0';
  if (dp != NULL) {
    if (dp->data [0] == 1) {
      if (dp->data [1] != 0) {
        sprintf (year, "%ld", (long) (1900 + dp->data [1]));
      }
    } else {
      StringNCpy (year, dp->str, (size_t) 4);
      year [4] = '\0';
    }
  }

  AddValNodeString (&head, "Thesis (", year, ")");

  if (imp->prepub == 2) {
    suffix = ", In press";
  }

  str = NULL;
  afp = imp->pub;
  if (afp != NULL) {
    if (afp->choice == 1) {
      if (StringLen (afp->affil) > 7) {
        str = StringSave (afp->affil);
      }
    } else if (afp->choice == 2) {
      str = MakeAffilStr (afp);
    }
  }

  if (str != NULL) {

    /* convert double quotes to single quotes */

    ptr = str;
    ch = *ptr;
    while (ch != '\0') {
      if (ch == '\"') {
        *ptr = '\'';
      }
      ptr++;
      ch = *ptr;
    }
    AddValNodeString (&head, " ", str, suffix);
    MemFree (str);
  }

  rsult = MergeValNodeStrings (head);
  ValNodeFreeData (head);

  return rsult;
}

static CharPtr FormatCitArt (
  FmtType format,
  Boolean citArtIsoJta,
  CitArtPtr cap
)

{
  CitBookPtr  cbp;
  CitJourPtr  cjp;
  CharPtr     rsult = NULL;

  if (cap == NULL) return NULL;

  switch (cap->from) {
    case 1 :
      cjp = (CitJourPtr) cap->fromptr;
      if (cjp != NULL) {
        rsult = FormatCitJour (format, citArtIsoJta, cjp);
      }
      break;
    case 2 :
      cbp = (CitBookPtr) cap->fromptr;
      if (cbp != NULL) {
        rsult = FormatCitBookArt (format, cbp);
      }
      break;
    case 3 :
      cbp = (CitBookPtr) cap->fromptr;
      if (cbp != NULL) {
        rsult = FormatCitBookArt (format, cbp);
      }
      break;
    default :
      break;
  }

  return rsult;
}

static CharPtr FormatCitPat (
  FmtType   format,
  CitPatPtr cpp,
  SeqIdPtr  seqidp
)

{
  AffilPtr       afp;
  AuthListPtr    alp;
  Char           date [40];
  ValNodePtr     head = NULL;
  CharPtr        prefix = NULL;
  CharPtr        rsult = NULL;
  SeqIdPtr       sip;
  CharPtr        suffix = NULL;
  PatentSeqIdPtr psip;
  Int4           pat_seqid = 0;
  Char           buf[10];

  if (cpp == NULL) return NULL;

  if (format == GENBANK_FMT || format == GENPEPT_FMT) {
    ValNodeCopyStr (&head, 0, "Patent: ");
    suffix = " ";
  } else if (format == EMBL_FMT || format == EMBLPEPT_FMT) {
    ValNodeCopyStr (&head, 0, "Patent number ");
  }

  if (! StringHasNoText (cpp->country)) {
    AddValNodeString (&head, NULL, cpp->country, suffix);
  }

  if (! StringHasNoText (cpp->number)) {
    ValNodeCopyStr (&head, 0, cpp->number);
  } else if (! StringHasNoText (cpp->app_number)) {
    AddValNodeString (&head, "(", cpp->app_number, ")");
  }

  if (! StringHasNoText (cpp->doc_type)) {
    AddValNodeString (&head, "-", cpp->doc_type, NULL);
  }

  /* pat_seqid test */

  for (sip = seqidp; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_PATENT) {
      psip = (PatentSeqIdPtr) sip -> data.ptrvalue;
      if (psip != NULL) {
        pat_seqid = psip->seqid;
      }
    }
  }
  if (pat_seqid > 0) {
    if (format == EMBL_FMT) {
      sprintf(buf,"%s%ld%s", "/", (long) pat_seqid, ", ");
      ValNodeCopyStr (&head, 0, buf);
    } else {
      sprintf(buf,"%s%ld ", " ", (long) pat_seqid);
      ValNodeCopyStr (&head, 0, buf);
    }
  } else {
    ValNodeCopyStr (&head, 0, " ");
  }

  /* Date */

  date [0] = '\0';
  if (cpp->date_issue != NULL) {
    DateToGB (date, cpp->date_issue, FALSE);
  } else if (cpp->app_date != NULL) {
    DateToGB (date, cpp->app_date, FALSE);
  }
  if (! StringHasNoText (date)) {
    ValNodeCopyStr (&head, 0, date);
  }

  if (format == GENBANK_FMT || format == GENPEPT_FMT) {
    ValNodeCopyStr (&head, 0, ";");
  } else if (format == EMBL_FMT || format == EMBLPEPT_FMT) {
    ValNodeCopyStr (&head, 0, ".");
  }

  alp = cpp->authors;
  if (alp != NULL) {
    afp = alp->affil;
    if (afp != NULL) {
      suffix = NULL;
      if (afp->choice == 2) {
        suffix = ";";
      }

      /* If and of the affiliation fields are */
      /* non-blank, put them on a new line.   */

      if ((! StringHasNoText (afp->affil)) ||
          (! StringHasNoText (afp->street)) ||
          (! StringHasNoText (afp->div)) ||
          (! StringHasNoText (afp->city)) ||
          (! StringHasNoText (afp->sub)) ||
          (! StringHasNoText (afp->country)))
        ValNodeCopyStr (&head, 0, "\n");

      /* Write out the affiliation fields */

      if (! StringHasNoText (afp->affil)) {
        AddValNodeString (&head, NULL, afp->affil, suffix);
        prefix = " ";
      }
      if (! StringHasNoText (afp->street)) {
        AddValNodeString (&head, prefix, afp->street, ";");
        prefix = " ";
      }
      if (! StringHasNoText (afp->div)) {
        AddValNodeString (&head, prefix, afp->div, ";");
        prefix = " ";
      }
      if (! StringHasNoText (afp->city)) {
        AddValNodeString (&head, prefix, afp->city, NULL);
        prefix = ", ";
      }
      if (! StringHasNoText (afp->sub)) {
        AddValNodeString (&head, prefix, afp->sub, NULL);
      }
      if (! StringHasNoText (afp->country)) {
        AddValNodeString (&head, ";\n", afp->country, ";");
      }
    }
  }

  rsult = MergeValNodeStrings (head);
  ValNodeFreeData (head);

  /*
  s_StringCleanup(rsult);
  */

  return rsult;
}

static CharPtr FormatCitGen (
  FmtType format,
  Boolean dropBadCitGens,
  Boolean noAffilOnUnpub,
  CitGenPtr cgp
)

{
  CharPtr      affil = NULL;
  AuthListPtr  alp = NULL;
  Char         ch;
  DatePtr      dp;
  ValNodePtr   head = NULL;
  CharPtr      inpress = NULL;
  CharPtr      journal = NULL;
  Char         pages [128];
  CharPtr      prefix = NULL;
  CharPtr      ptr;
  CharPtr      rsult = NULL;
  Char         year [8];

  if (cgp == NULL) return NULL;

  if (cgp->journal == NULL && StringNICmp (cgp->cit, "unpublished", 11) == 0) {
    if (noAffilOnUnpub) {
      return StringSave ("Unpublished");
    }

    alp = cgp->authors;
    if (alp != NULL) {
      affil = GetAffil (alp->affil);
      if (! StringHasNoText (affil)) {
        rsult = MemNew ((size_t) StringLen (affil) + (size_t) StringLen (cgp->cit) + 15);
        StringCpy (rsult, "Unpublished ");
        StringCat (rsult, affil);
        TrimSpacesAroundString (rsult);
        return rsult;
      }
    }

    rsult = StringSave (cgp->cit);
    TrimSpacesAroundString (rsult);
    return rsult;
  }

  year [0] = '\0';
  dp = cgp->date;
  if (dp != NULL) {
    if (dp->data [0] == 1) {
      if (dp->data [1] != 0) {
        sprintf (year, " (%ld)", (long) (1900 + dp->data [1]));
      }
    } else {
      StringCpy (year, " (");
      StringNCat (year, dp->str, 4);
      StringCat (year, ")");
    }
  }

  pages [0] = '\0';
  if (cgp->pages != NULL) {
    FixPages (pages, cgp->pages);
  }

  if (cgp->journal != NULL) {
    journal = (CharPtr) cgp->journal->data.ptrvalue;
  }
  if (cgp->cit != NULL) {
    ptr = StringStr (cgp->cit, "Journal=\"");
    if (ptr != NULL) {
      journal = ptr + 9;
    } else if (StringNICmp (cgp->cit, "submitted", 8) == 0 ||
               StringNICmp (cgp->cit, "unpublished", 11) == 0 ||
               StringNICmp (cgp->cit, "in press", 8) == 0 ||
               StringNICmp (cgp->cit, "to be published", 15) == 0) {

      if ((! dropBadCitGens) || journal != NULL) {
        inpress = cgp->cit;
      } else {
        inpress = "Unpublished";
      }
    } else if ((! dropBadCitGens) && journal == NULL) {
      journal = cgp->cit;
    }
  }
  if (journal != NULL) {
    journal = StringSave (journal);
    for (ptr = journal, ch = *ptr; ch != '\0'; ptr++, ch = *ptr) {
      if (ch == '=' || ch == '\"') {
        *ptr = '\0';
      }
    }
    ValNodeAddStr (&head, 0, journal);
    prefix = " ";
  }

  if (! StringHasNoText (inpress)) {
    AddValNodeString (&head, prefix, inpress, NULL);
    prefix = " ";
  }

  if (! StringHasNoText (cgp->volume)) {
    AddValNodeString (&head, prefix, cgp->volume, NULL);
  }

  if (! StringHasNoText (pages)) {
    if (format == GENBANK_FMT) {
      AddValNodeString (&head, ", ", pages, NULL);
    } else if (format == EMBL_FMT) {
      AddValNodeString (&head, ":", pages, NULL);
    }
  }

  if (! StringHasNoText (year)) {
    AddValNodeString (&head, NULL, year, NULL);
  }

  rsult = MergeValNodeStrings (head);
  ValNodeFreeData (head);

  return rsult;
}

static CharPtr FormatCitSub (
  FmtType format,
  CitSubPtr csp
)

{
  CharPtr      affil;
  AffilPtr     afp;
  AuthListPtr  alp;
  Char         buf [256];
  Char         date [40];
  ValNodePtr   head = NULL;
  CharPtr      rsult = NULL;

  if (csp == NULL) return NULL;

  date [0] = '\0';
  if (csp->date != NULL) {
    DateToGB (date, csp->date, TRUE);
  }
  if (StringHasNoText (date)) {
    StringCpy (date, "??-???-????");
  }

  sprintf (buf, "Submitted (%s)", date);
  ValNodeCopyStr (&head, 0, buf);

  alp = csp->authors;
  if (alp != NULL) {
    afp = alp->affil;
    if (afp != NULL) {
      affil = GetAffil (afp);
      if (format == EMBL_FMT || format == EMBLPEPT_FMT) {
        if (StringNCmp(affil, " to the EMBL/GenBank/DDBJ databases.", 36) != 0) {
          ValNodeCopyStr (&head, 0, " to the EMBL/GenBank/DDBJ databases.\n");
        } else {
          ValNodeCopyStr (&head, 0, " ");
        }
      } else {
        ValNodeCopyStr (&head, 0, " ");
      }
      ValNodeCopyStr (&head, 0, affil);
      MemFree (affil);
    }
  }

  rsult = MergeValNodeStrings (head);
  ValNodeFreeData (head);

  return rsult;
}

static CharPtr GetPubJournal (
  FmtType format,
  Boolean dropBadCitGens,
  Boolean noAffilOnUnpub,
  Boolean citArtIsoJta,
  PubdescPtr pdp,
  CitSubPtr csp,
  SeqIdPtr  seqidp
)

{
  CitArtPtr        cap;
  CitBookPtr       cbp;
  CitGenPtr        cgp;
  CitPatPtr        cpp;
  CharPtr          journal = NULL;
  MedlineEntryPtr  mep;
  ValNodePtr       vnp;

  if (csp != NULL) {
    return FormatCitSub (format, csp);
  }
  if (pdp == NULL) return NULL;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    switch (vnp->choice) {
      case PUB_Gen :
        cgp = (CitGenPtr) vnp->data.ptrvalue;
        if (cgp != NULL) {
          if (StringNICmp ("BackBone id_pub", cgp->cit, 15) != 0) {
            if (cgp->cit == NULL && cgp->journal == NULL && cgp->date == NULL && cgp->serial_number) {
              break; /* skip just serial number */
            }
          }
          journal = FormatCitGen (format, dropBadCitGens, noAffilOnUnpub, cgp);
        }
        break;
      case PUB_Sub :
        csp = (CitSubPtr) vnp->data.ptrvalue;
        if (csp != NULL) {
          journal = FormatCitSub (format, csp);
        }
        break;
      case PUB_Medline :
        mep = (MedlineEntryPtr) vnp->data.ptrvalue;
        if (mep != NULL) {
          cap = mep->cit;
          if (cap != NULL) {
            journal = FormatCitArt (format, citArtIsoJta, cap);
          }
        }
        break;
      case PUB_Article :
        cap = (CitArtPtr) vnp->data.ptrvalue;
        if (cap != NULL) {
          journal = FormatCitArt (format, citArtIsoJta, cap);
        }
        break;
      case PUB_Book :
      case PUB_Proc :
        cbp = (CitBookPtr) vnp->data.ptrvalue;
        if (cbp != NULL) {
          journal = FormatCitBook (format, cbp);
        }
        break;
      case PUB_Man :
        cbp = (CitBookPtr) vnp->data.ptrvalue;
        if (cbp != NULL) {
          journal = FormatThesis (format, cbp);
        }
        break;
      case PUB_Patent :
        cpp = (CitPatPtr) vnp->data.ptrvalue;
        if (cpp != NULL) {
          journal = FormatCitPat (format, cpp, seqidp);
        }
        break;
      default :
        break;
    }

    if (journal != NULL) return journal;
  }

  return NULL;
}

static Int4 GetMuid (
  PubdescPtr pdp
)

{
  MedlineEntryPtr  mep;
  ValNodePtr       vnp;

  if (pdp == NULL) return 0;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    switch (vnp->choice) {
      case PUB_Medline :
        mep = (MedlineEntryPtr) vnp->data.ptrvalue;
        if (mep != NULL) {
          return mep->uid;
        }
        break;
      case PUB_Muid :
        return vnp->data.intvalue;
        break;
      default :
        break;
    }
  }

  return 0;
}

static Int4 GetPmid (
  PubdescPtr pdp
)

{
  MedlineEntryPtr  mep;
  ValNodePtr       vnp;

  if (pdp == NULL) return 0;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    switch (vnp->choice) {
      case PUB_Medline :
        mep = (MedlineEntryPtr) vnp->data.ptrvalue;
        if (mep != NULL) {
          return mep->pmid;
        }
        break;
      case PUB_PMid :
        return vnp->data.intvalue;
        break;
      default :
        break;
    }
  }

  return 0;
}

static CharPtr remarksText [] = {
  "full automatic", "full staff_review", "full staff_entry",
  "simple staff_review", "simple staff_entry", "simple automatic",
  "unannotated automatic", "unannotated staff_review", "unannotated staff_entry",
  NULL
};

static CharPtr FormatReferenceBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
)

{
  IntAsn2gbJobPtr    ajp;
  AuthListPtr        alp;
  Asn2gbSectPtr      asp;
  BioseqPtr          bsp;
  Char               buf [150];
  CitArtPtr          cap;
  CitJourPtr         cjp;
  CitRetractPtr      crp;
  CitSubPtr          csp = NULL;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  Int4               gibbsq;
  Int2               i;
  ImprintPtr         imp;
  IntRefBlockPtr     irp;
  size_t             len;
  SeqLocPtr          loc = NULL;
  Int4               muid;
  Boolean            needsPeriod;
  SeqLocPtr          nextslp;
  Boolean            notFound;
  ObjMgrDataPtr      omdp;
  PubdescPtr         pdp = NULL;
  Int4               pmid;
  CharPtr            prefix;
  RefBlockPtr        rbp;
  SubmitBlockPtr     sbp;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp = NULL;
  SeqIdPtr           sip;
  SeqLocPtr          slp;
  SeqSubmitPtr       ssp;
  Int4               start;
  Int4               stop;
  CharPtr            str;
  CharPtr            suffix = NULL;
  Boolean            trailingPeriod;
  ValNodePtr         vnp;

  if (afp == NULL || bbp == NULL) return NULL;
  rbp = (RefBlockPtr) bbp;
  ajp = afp->ajp;
  if (ajp == NULL) return NULL;
  asp = afp->asp;
  if (asp == NULL) return NULL;
  bsp = asp->bsp;
  if (bsp == NULL) return NULL;

  if (! StringHasNoText (rbp->string)) return StringSave (rbp->string);

  /* could be descriptor, feature, or submit block citation */

  if (rbp->itemtype == OBJ_SEQDESC) {

    sdp = SeqMgrGetDesiredDescriptor (rbp->entityID, NULL, rbp->itemID, 0, NULL, &dcontext);
    if (sdp != NULL && dcontext.seqdesctype == Seq_descr_pub) {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
    }

  } else if (rbp->itemtype == OBJ_SEQFEAT) {

    sfp = SeqMgrGetDesiredFeature (rbp->entityID, NULL, rbp->itemID, 0, NULL, &fcontext);
    if (sfp != NULL && fcontext.seqfeattype == SEQFEAT_PUB) {
      pdp = (PubdescPtr) sfp->data.value.ptrvalue;
    }

  } else if (rbp->itemtype == OBJ_SEQSUB_CIT) {

    omdp = ObjMgrGetData (rbp->entityID);
    if (omdp != NULL && omdp->datatype == OBJ_SEQSUB) {
      ssp = (SeqSubmitPtr) omdp->dataptr;
      if (ssp != NULL && ssp->datatype == 1) {
        sbp = ssp->sub;
        if (sbp != NULL) {
          csp = sbp->cit;
        }
      }
    }
  }

  if (pdp == NULL && csp == NULL) return NULL;

  /* print serial number */

  gb_StartPrint (afp->format, 0, 12, "REFERENCE", 13, 5, 5, "RN", TRUE);

  if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
    sprintf (buf, "%d", (int) rbp->serial);
  } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {
    sprintf (buf, "[%d]", (int) rbp->serial);
  }

  gb_AddString (NULL, buf, NULL, FALSE, FALSE, TILDE_TO_SPACES);

  /* print base range */

  if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {

    if (rbp->sites != 3) {
      TabToColumn (16);
    }
  } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {

    if (rbp->sites == 0) {
      ff_EndPrint ();
      ff_StartPrint (5, 5, ASN2FF_EMBL_MAX, "RP");
    }
  }

  if (rbp->sites == 1 || rbp->sites == 2) {

    if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
      gb_AddString (NULL, "(sites)", NULL, FALSE, FALSE, TILDE_TO_SPACES);
    }
    ff_EndPrint ();

  } else if (rbp->sites == 3) {

      ff_EndPrint ();

  } else {

    if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
      TabToColumn (16);
      if (afp->format == GENBANK_FMT) {
        gb_AddString (NULL, "(bases ", NULL, FALSE, FALSE, TILDE_TO_SPACES);
      } else {
        gb_AddString (NULL, "(residues ", NULL, FALSE, FALSE, TILDE_TO_SPACES);
      }
    }

    irp = (IntRefBlockPtr) rbp;
    loc = irp->loc;

    if (loc != NULL) {
      if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
        suffix = "; ";
      } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {
        suffix = ", ";
      }

      slp = SeqLocFindNext (loc, NULL);
      while (slp != NULL) {
        nextslp = SeqLocFindNext (loc, slp);
        start = SeqLocStart (slp) + 1;
        stop = SeqLocStop (slp) + 1;
        if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
          sprintf (buf, "%ld to %ld", (long) start, (long) stop);
        } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {
          sprintf (buf, "%ld-%ld", (long) start, (long) stop);
        }
        if (nextslp == NULL) {
          suffix = NULL;
        }
        gb_AddString (NULL, buf, suffix, FALSE, FALSE, TILDE_TO_SPACES);
        slp = nextslp;
      }

    } else {

      /* code still used for ssp->cit */

      start = 1;
      stop = bsp->length;
      if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
        sprintf (buf, "%ld to %ld", (long) start, (long) stop);
      } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {
        sprintf (buf, "%ld-%ld", (long) start, (long) stop);
      }
      gb_AddString (NULL, buf, NULL, FALSE, FALSE, TILDE_TO_SPACES);
    }

    if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
      gb_AddString (NULL, ")", NULL, FALSE, FALSE, TILDE_TO_SPACES);
    }
    ff_EndPrint ();
  }

  /* print author list */

  gb_StartPrint (afp->format, 2, 12, "AUTHORS", 13, 5, 5, "RA", FALSE);

  str = NULL;

  alp = GetAuthListPtr (pdp, csp);
  if (alp != NULL) {
    str = GetAuthorsString (afp->format, alp);
    TrimSpacesAroundString (str);
  }

  if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
    suffix = NULL;
    trailingPeriod = TRUE;
  } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {
    trailingPeriod = FALSE;
    len = StringLen (str);
    if (len > 0 && str [len - 1] != '.') {
      suffix = ".;";
    } else {
      suffix = ";";
    }
  }

  gb_AddString (NULL, str, suffix, trailingPeriod, FALSE, TILDE_TO_SPACES);

  MemFree (str);
  ff_EndPrint ();

  /* print title */

  str = GetPubTitle (afp->format, pdp, csp);
  CleanPubTitle (str);
  StrStripSpaces (str);

  if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
    prefix = NULL;
    suffix = NULL;
  } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {
    if (str != NULL) {
      prefix = "\"";
      suffix = "\";";
    } else {
      prefix = NULL;
      suffix = ";";
    }
  }

  if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
    if (! StringHasNoText (str)) {
      gb_StartPrint (afp->format, 2, 12, "TITLE", 13, 5, 5, "RT", FALSE);

      gb_AddString (prefix, str, suffix, FALSE, FALSE, TILDE_TO_SPACES);

      ff_EndPrint ();
    }
  } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {
    gb_StartPrint (afp->format, 2, 12, "TITLE", 13, 5, 5, "RT", FALSE);
    if (! StringHasNoText (str)) {

      gb_AddString (prefix, str, suffix, FALSE, FALSE, TILDE_TO_SPACES);

    } else {
      ff_AddChar (';');
    }
    ff_EndPrint ();
  }

  MemFree (str);

  /* print journal */

  gb_StartPrint (afp->format, 2, 12, "JOURNAL", 13, 5, 5, "RL", FALSE);

  str = GetPubJournal (afp->format, ajp->flags.dropBadCitGens,
                       ajp->flags.noAffilOnUnpub, ajp->flags.citArtIsoJta,
                       pdp, csp, bsp->id);
  if (str == NULL) {
    str = StringSave ("Unpublished");
  }
  StrStripSpaces (str);
  TrimSpacesAroundString (str);

  if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
    needsPeriod = FALSE;
  } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {
    needsPeriod = TRUE;
  }

  gb_AddString (NULL, str, NULL, FALSE, FALSE, TILDE_IGNORE);
  if (needsPeriod) {
    gb_AddString (NULL, ".", NULL, FALSE, FALSE, TILDE_TO_SPACES);
  }

  MemFree (str);
  ff_EndPrint ();

  /* print muid */

  muid = GetMuid (pdp);
  if (muid > 0) {
    gb_StartPrint (afp->format, 2, 12, "MEDLINE", 13, 5, 5, "RX", FALSE);

    if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
      www_muid (muid);
    } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {
      sprintf (buf, "MEDLINE; %ld.", (long) muid);
      gb_AddString (NULL, buf, NULL, FALSE, FALSE, TILDE_TO_SPACES);
    }

    ff_EndPrint ();
  }

  if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {
    pmid = GetPmid (pdp);
    if (pmid > 0) {
      gb_StartPrint (afp->format, 3, 12, "PUBMED", 13, 5, 5, "RX", FALSE);

      www_muid (pmid);

      ff_EndPrint ();
    }
  }

  if (pdp == NULL) return gb_MergeString (FALSE);

  /* !!! remainder of fields are only for GenBank !!! */

  if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {

    prefix = "REMARK";

    if (pdp->comment != NULL) {
      for (i = 0, notFound = TRUE; notFound && remarksText [i] != NULL; i++) {
        if (StringCmp (pdp->comment, remarksText [i]) == 0) {
          notFound = FALSE;
        }
      }
      if (notFound) {
        gb_StartPrint (afp->format, 2, 12, prefix, 13, 5, 5, NULL, FALSE);
        gb_AddString (NULL, pdp->comment, NULL, FALSE, TRUE, TILDE_EXPAND);
        ff_EndPrint ();
        prefix = NULL;
      }
    }

    gibbsq = 0;
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_GIBBSQ) {
        gibbsq = sip->data.intvalue;
      }
    }
    csp = NULL;
    for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
      if (vnp->choice == PUB_Sub) {
        csp = (CitSubPtr) vnp->data.ptrvalue;
      }
    }
    if (gibbsq > 0 /* && csp == NULL */) {
      sprintf (buf, "GenBank staff at the National Library of Medicine created this entry [NCBI gibbsq %ld] from the original journal article.", (long) gibbsq);
      gb_StartPrint (afp->format, 2, 12, prefix, 13, 5, 5, NULL, FALSE);
      gb_AddString (NULL, buf, NULL, FALSE, FALSE, TILDE_EXPAND);
      ff_EndPrint ();
      prefix = NULL;

      /* gibbsq comment section (fields may be copied from degenerate pubdesc) */

      str = pdp->fig;
      if (StringHasNoText (str)) {
        str = irp->fig;
      }
      if (! StringHasNoText (str)) {
        sprintf (buf, "This sequence comes from %s", str);
        gb_StartPrint (afp->format, 2, 12, prefix, 13, 5, 5, NULL, FALSE);
        gb_AddString (NULL, buf, NULL, TRUE, TRUE, TILDE_EXPAND);
        ff_EndPrint ();
        prefix = NULL;
      }

      if (pdp->poly_a || irp->poly_a) {
        gb_StartPrint (afp->format, 2, 12, prefix, 13, 5, 5, NULL, FALSE);
        gb_AddString (NULL, "Polyadenylate residues occurring in the figure were omitted from the sequence.", NULL, TRUE, TRUE, TILDE_EXPAND);
        ff_EndPrint ();
        prefix = NULL;
      }

      str = pdp->maploc;
      if (StringHasNoText (str)) {
        str = irp->maploc;
      }
      if (! StringHasNoText (str)) {
        sprintf (buf, "Map location: %s", str);
        gb_StartPrint (afp->format, 2, 12, prefix, 13, 5, 5, NULL, FALSE);
        gb_AddString (NULL, buf, NULL, TRUE, TRUE, TILDE_EXPAND);
        ff_EndPrint ();
        prefix = NULL;
      }

    }

    for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
      if (vnp->choice == PUB_Article) {
        cap = (CitArtPtr) vnp->data.ptrvalue;
        if (cap != NULL && cap->from == 1) {
          cjp = (CitJourPtr) cap->fromptr;
          if (cjp != NULL) {
            imp = cjp->imp;
            if (imp != NULL) {
              crp = imp->retract;
              if (crp != NULL && crp->type == 3) {
                gb_StartPrint (afp->format, 2, 12, prefix, 13, 5, 5, NULL, FALSE);
                gb_AddString (NULL, "Erratum:", NULL, FALSE, FALSE, TILDE_TO_SPACES);
                gb_AddString ("[", crp->exp, "]", FALSE, TRUE, TILDE_EXPAND);
                ff_EndPrint ();
                prefix = NULL;
              }
            }
          }
        }
      } else if (vnp->choice == PUB_Sub) {
        csp = (CitSubPtr) vnp->data.ptrvalue;
        if (csp != NULL) {
          if (! StringHasNoText (csp->descr)) {
            gb_StartPrint (afp->format, 2, 12, prefix, 13, 5, 5, NULL, FALSE);
            gb_AddString (NULL, csp->descr, NULL, FALSE, TRUE, TILDE_EXPAND);
            ff_EndPrint ();
            prefix = NULL;
          }
        }
      }
    }

  }

  return ff_print_string_mem (gb_MergeString (FALSE));
}

static CharPtr FormatCommentBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
)

{
  Boolean            add_period;
  CommentBlockPtr    cbp;
  CharPtr            db;
  DbtagPtr           dbt;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  size_t             len;
  ObjectIdPtr        oip;
  CharPtr            prefix;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;
  Char               sfx [32];
  CharPtr            str;
  CharPtr            suffix;
  CharPtr            title;

  if (afp == NULL || bbp == NULL) return NULL;
  cbp = (CommentBlockPtr) bbp;

  /* some comments are allocated (along with possible first COMMENT label) */

  if (! StringHasNoText (bbp->string)) return StringSave (bbp->string);

  title = NULL;
  prefix = NULL;
  suffix = NULL;
  add_period = FALSE;
  sfx [0] = '\0';

  if (bbp->itemtype == OBJ_SEQDESC) {

    /* usually should reference comment, maploc, or region descriptor IDs */

    sdp = SeqMgrGetDesiredDescriptor (bbp->entityID, NULL, bbp->itemID, 0, NULL, &dcontext);
    if (sdp != NULL) {

      if (dcontext.seqdesctype == Seq_descr_comment) {

        title = (CharPtr) sdp->data.ptrvalue;

      } else if (dcontext.seqdesctype == Seq_descr_maploc) {

        dbt = (DbtagPtr) sdp->data.ptrvalue;
        if (dbt != NULL) {
          db = dbt->db;
          oip = dbt->tag;
          if (oip != NULL) {
            if (oip->str != NULL) {

              title = oip->str;
              prefix = ("Map location: ");

            } else if (db != NULL && oip->id != 0) {

              title = db;
              prefix = ("Map location: (Database ");
              sprintf (sfx, "; id # %ld).", (long) oip->id);
              suffix = sfx;

            }
          }
        }

      } else if (dcontext.seqdesctype == Seq_descr_region) {

        title = (CharPtr) sdp->data.ptrvalue;
        prefix = "Region: ";

      }
    }

  } else if (bbp->itemtype == OBJ_SEQFEAT) {

    /* also have to deal with comment feature across entire sequence */

    sfp = SeqMgrGetDesiredFeature (bbp->entityID, NULL, bbp->itemID, 0, NULL, &fcontext);
    if (sfp != NULL && fcontext.seqfeattype == SEQFEAT_COMMENT) {

      title = sfp->comment;
    }
  }

  if (title == NULL) return NULL;

  if (cbp->first) {
    gb_StartPrint (afp->format, 0, 12, "COMMENT", 13, 5, 5, "CC", TRUE);
  } else {
    gb_StartPrint (afp->format, 0, 12, NULL, 13, 5, 5, "CC", FALSE);
  }

  str = StringSave (title);
  TrimSpacesAndJunkFromEnds (str, TRUE);
  if (! IsEllipsis (str)) {
    s_RemovePeriodFromEnd (str);
    len = StringLen (str);
    if (len > 0 && str [len - 1] != '.') {
      add_period = TRUE;
    }
  }
  gb_AddString (prefix, str, suffix, FALSE, TRUE, TILDE_OLD_EXPAND);
  if (add_period) {
    gb_AddString (NULL, ".", NULL, FALSE, FALSE, TILDE_TO_SPACES);
  }
  MemFree (str);

  return gb_MergeString (TRUE);
}

/* format features section */

static Boolean is_real_id (
  SeqIdPtr sip,
  SeqIdPtr this_sip
)

{
  BioseqPtr  bsp;

  if (sip == NULL || this_sip == NULL) return FALSE;

  if (! SeqIdIn (sip, this_sip)) {
    bsp = BioseqFind (sip);
    if (bsp == NULL) return TRUE;  /* ??? */
    if (bsp->repr == Seq_repr_virtual) return FALSE;
  }

  return TRUE;
}

static Boolean FlatVirtLoc (
  BioseqPtr bsp,
  SeqLocPtr location
)

{
  SeqIntPtr  sintp;
  SeqIdPtr   sip;
  SeqPntPtr  spp;

  if (bsp == NULL || location == NULL) return FALSE;

  switch (location->choice) {
    case SEQLOC_WHOLE :
      sip = (SeqIdPtr) location->data.ptrvalue;
      if (sip == NULL) return TRUE;
      if (! is_real_id (sip, bsp->id)) return TRUE;
      break;
    case SEQLOC_INT :
      sintp = (SeqIntPtr) location->data.ptrvalue;
      if (sintp == NULL) return TRUE;
      sip = sintp->id;
      if (sip == NULL) return TRUE;
      if (! is_real_id (sip, bsp->id)) return TRUE;
      break;
    case SEQLOC_PNT :
      spp = (SeqPntPtr) location->data.ptrvalue;
      if (spp == NULL) return TRUE;
      sip = spp->id;
      if (sip == NULL) return TRUE;
      if (! is_real_id (sip, bsp->id)) return TRUE;
      break;
    default :
      break;
  }

  return FALSE;
}

static Uint1    order [NUM_SEQID];
static Boolean  order_initialized = FALSE;

static CharPtr lim_str [5] = { "", ">","<", ">", "<" };

static void FlatLocSeqId (
  ValNodePtr PNTR head,
  SeqIdPtr sip
)

{
  BioseqPtr    bsp;
  Char         buf [40];
  ObjectIdPtr  oip;
  SeqIdPtr     use_id = NULL;
  Boolean      was_lock = FALSE;

  if (head == NULL || sip == NULL) return;

  bsp = BioseqFind (sip);
  if (bsp == NULL) {
    bsp = BioseqLockById (sip);
    was_lock = TRUE;
  }
  if (bsp != NULL) {
    use_id = SeqIdSelect (bsp->id, order, NUM_SEQID);
  } else if (sip->choice == SEQID_GI) {
    use_id = GetSeqIdForGI (sip->data.intvalue);
  }
  if (use_id != NULL) {
    SeqIdWrite (use_id, buf, PRINTID_TEXTID_ACC_VER, sizeof (buf) - 1);
  } else if (sip->choice == SEQID_GI) {
    SeqIdWrite (sip, buf, PRINTID_FASTA_LONG, sizeof (buf) - 1);
  }
  if (was_lock) {
    BioseqUnlock (bsp);
  }
  if (StringHasNoText (buf)) {
    StringCpy (buf, "?00000");
    if (use_id != NULL && use_id->choice == SEQID_LOCAL) {
      oip = (ObjectIdPtr) use_id->data.ptrvalue;
      if (oip != NULL && (! StringHasNoText (oip->str))) {
        StringNCpy_0 (buf, oip->str, 13);
      }
    }
  }
  AddValNodeString (head, NULL, buf, ":");
}

static void FlatLocCaret (
  ValNodePtr PNTR head,
  SeqIdPtr sip,
  SeqIdPtr this_sip,
  Int4 point,
  IntFuzzPtr fuzz
)

{
  Char   buf [128];
  Uint1  index;

  if (head == NULL) return;

  if (sip != NULL && (! SeqIdIn (sip, this_sip))) {
    FlatLocSeqId (head, sip);
  }

  buf [0] = '\0';
  point++; /* orginal FlatLocHalfCaret was called with point + 1 */

  if (fuzz != NULL) {
    switch (fuzz->choice) {
      case 1 :
        sprintf (buf, "(%ld.%ld)..(%ld.%ld)",
                 (long) (point - fuzz->a),
                 (long) point,
                 (long) point,
                 (long) (point + fuzz->a));
        break;
      case 2 :
        sprintf (buf, "%ld^%ld",
                 (long) (1 + fuzz->b),
                 (long) (1 + fuzz->a));
        break;
      case 3 :
        sprintf (buf, "%ld^%ld",
                 (long) (point - point * ((double) fuzz->a / 1000.0)),
                 (long) (point + point * ((double) fuzz->a / 1000.0)));
        break;
      case 4 :
        if (fuzz->a == 3) { /* space to right */
          sprintf (buf, "%ld^%ld", (long) (point), (long) (point + 1));
        } else if (fuzz->a == 4 && point > 1) { /* space to left */
          sprintf (buf, "%ld^%ld", (long) (point - 1), (long) point);
        } else {
          index = (Uint1) fuzz->a;
          if (index > 4) {
            index = 0;
          }
          sprintf (buf, "%s%ld", lim_str [index], (long) point);
        }
        break;
      default :
        sprintf (buf, "%ld", (long) point);
        break;
    }
  } else {
    sprintf (buf, "%ld", (long) point);
  }

  ValNodeCopyStr (head, 0, buf);
}

static void FlatLocPoint (
  ValNodePtr PNTR head,
  SeqIdPtr sip,
  SeqIdPtr this_sip,
  Int4 point,
  IntFuzzPtr fuzz
)

{
  Char   buf [128];
  Uint1  index;

  if (head == NULL) return;

  if (sip != NULL && (! SeqIdIn (sip, this_sip))) {
    FlatLocSeqId (head, sip);
  }

  buf [0] = '\0';
  point++;

  if (fuzz != NULL) {
    switch (fuzz->choice) {
      case 1 :
        sprintf (buf, "(%ld.%ld)",
                 (long) (point - fuzz->a),
                 (long) (point + fuzz->a));
        break;
      case 2 :
        sprintf (buf, "(%ld.%ld)",
                 (long) (1 + fuzz->b),
                 (long) (1 + fuzz->a));
        break;
      case 3 :
        sprintf (buf, "(%ld.%ld)",
                 (long) (point - point * ((double) fuzz->a / 1000.0)),
                 (long) (point + point * ((double) fuzz->a / 1000.0)));
        break;
      case 4 :
        index = (Uint1) fuzz->a;
        if (index > 4) {
          index = 0;
        }
        sprintf (buf, "%s%ld", lim_str [index], (long) point);
        break;
      default :
        sprintf (buf, "%ld", (long) point);
        break;
    }
  } else {
    sprintf (buf, "%ld", (long) point);
  }

  ValNodeCopyStr (head, 0, buf);
}

static void FlatLocElement (
  ValNodePtr PNTR head,
  BioseqPtr bsp,
  SeqLocPtr location
)

{
  Boolean     minus_strand = FALSE;
  SeqBondPtr  sbp;
  SeqIntPtr   sintp;
  SeqIdPtr    sip;
  SeqPntPtr   spp;
  BioseqPtr   wholebsp;

  if (head == NULL || bsp == NULL || location == NULL) return;

  switch (location->choice) {
    case SEQLOC_WHOLE :
      sip = (SeqIdPtr) location->data.ptrvalue;
      if (sip == NULL) return;
      wholebsp = BioseqFind (sip);
      if (wholebsp == NULL) return;
      if (is_real_id (sip, bsp->id)) {
        FlatLocPoint (head, sip, bsp->id, 0, NULL);
        if (bsp->length > 0) {
          ValNodeCopyStr (head, 0, "..");
          FlatLocPoint (head, NULL, bsp->id, bsp->length - 1, NULL);
        }
      }
      break;
    case SEQLOC_INT :
      sintp = (SeqIntPtr) location->data.ptrvalue;
      if (sintp == NULL) return;
      sip = sintp->id;
      if (sip == NULL) return;
      if (is_real_id (sip, bsp->id)) {
        minus_strand = (Boolean) (sintp->strand == Seq_strand_minus);
        if (minus_strand) {
          ValNodeCopyStr (head, 0, "complement(");
        }
        FlatLocPoint (head, sip, bsp->id, sintp->from, sintp->if_from);
        if (sintp->to > 0 &&
            (sintp->to != sintp->from ||
             sintp->if_from != NULL ||
             sintp->if_to != NULL)) {
          ValNodeCopyStr (head, 0, "..");
          FlatLocPoint (head, NULL, bsp->id, sintp->to, sintp->if_to);
        }
        if (minus_strand) {
          ValNodeCopyStr (head, 0, ")");
        }
      }
      break;
    case SEQLOC_PNT :
      spp = (SeqPntPtr) location->data.ptrvalue;
      if (spp == NULL) return;
      sip = spp->id;
      if (sip == NULL) return;
      if (is_real_id (sip, bsp->id)) {
        minus_strand = (Boolean) (spp->strand == Seq_strand_minus);
        if (minus_strand) {
          ValNodeCopyStr (head, 0, "complement(");
        }
        if (spp->fuzz != NULL) {
          FlatLocCaret (head, sip, bsp->id, spp->point, spp->fuzz);
        } else {
          FlatLocPoint (head, sip, bsp->id, spp->point, NULL);
        }
        if (minus_strand) {
          ValNodeCopyStr (head, 0, ")");
        }
      }
      break;
    case SEQLOC_BOND :
      sbp = (SeqBondPtr) location->data.ptrvalue;
      if (sbp == NULL) return;
      spp = sbp->a;
      if (spp == NULL) return;
      sip = spp->id;
      if (sip == NULL) return;
      ValNodeCopyStr (head, 0, "bond(");
      FlatLocPoint (head, sip, bsp->id, spp->point, spp->fuzz);
      spp = sbp->b;
      if (spp != NULL) {
        ValNodeCopyStr (head, 0, ",");
        FlatLocPoint (head, NULL, bsp->id, spp->point, spp->fuzz);
      }
      ValNodeCopyStr (head, 0, ")");
      break;
    default :
      /* unexpected internal complex type or unimplemented SEQLOC_FEAT */
      return;
  }
}

static Boolean FlatNullAhead (
  BioseqPtr bsp,
  ValNodePtr location
)

{
  SeqLocPtr  next;

  if (bsp == NULL || location == NULL) return FALSE;

  next = location->next;
  if (next == NULL) return TRUE;
  if (next->choice == SEQLOC_NULL) return TRUE;
  if (FlatVirtLoc (bsp, next)) return TRUE;

  return FALSE;
}

static void FlatPackedPoint (
  ValNodePtr PNTR head,
  PackSeqPntPtr pspp,
  BioseqPtr bsp
)

{
  Uint1  dex;

  if (head == NULL || pspp == NULL || bsp == NULL) return;

  for (dex = 0; dex < pspp->used; dex++) {
    FlatLocPoint (head, pspp->id, bsp->id, pspp->pnts [dex], pspp->fuzz);
  }
}

static void DoFlatLoc (
  ValNodePtr PNTR head,
  BioseqPtr bsp,
  SeqLocPtr location,
  Boolean ok_to_complement
);

static void GroupFlatLoc (
  ValNodePtr PNTR head,
  BioseqPtr bsp,
  SeqLocPtr location,
  CharPtr prefix,
  Boolean is_flat_order
)

{
  Boolean        found_non_virt = FALSE;
  SeqIdPtr       hold_next;
  Int2           parens = 1;
  PackSeqPntPtr  pspp;
  SeqLocPtr      slp;
  Boolean        special_mode = FALSE; /* join in order */

  if (head == NULL || bsp == NULL || location == NULL) return;

  /* prefix will have the first parenthesis */

  ValNodeCopyStr (head, 0, prefix);

  for (slp = (SeqLocPtr) location->data.ptrvalue; slp != NULL; slp = slp->next) {

    if (slp->choice == SEQLOC_NULL || FlatVirtLoc (bsp, slp)) {
      if (slp != location && slp->next != NULL) {
        if (special_mode) {
          special_mode = FALSE;
          ValNodeCopyStr (head, 0, ")");
          parens--;
        }
      }
      continue;
    }

    if (found_non_virt && slp->choice != SEQLOC_EMPTY && slp->choice != SEQLOC_NULL) {
      ValNodeCopyStr (head, 0, ",");
    }

    switch (slp->choice) {
      case SEQLOC_WHOLE :
      case SEQLOC_PNT :
      case SEQLOC_BOND :
      case SEQLOC_FEAT :
        found_non_virt = TRUE;
        if (FlatVirtLoc (bsp, slp)) {
          if (slp != location && slp->next != NULL) {
            if (special_mode) {
              special_mode = FALSE;
              ValNodeCopyStr (head, 0, ")");
              parens--;
            }
          }
        } else {
          FlatLocElement (head, bsp, slp);
        }
        break;
      case SEQLOC_INT :
        found_non_virt = TRUE;
        if (is_flat_order && (! FlatNullAhead (bsp, slp))) {
          special_mode = TRUE;
          ValNodeCopyStr (head, 0, "join(");
          parens++;
        }
        FlatLocElement (head, bsp, slp);
        break;
      case SEQLOC_PACKED_PNT :
        found_non_virt = TRUE;
        pspp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (pspp != NULL) {
          FlatPackedPoint (head, pspp, bsp);
        }
        break;
      case SEQLOC_PACKED_INT :
      case SEQLOC_MIX :
      case SEQLOC_EQUIV :
        found_non_virt = TRUE;
        hold_next = slp->next;
        slp->next = NULL;
        DoFlatLoc (head, bsp, slp, FALSE);
        slp->next = hold_next;
        break;
      default :
        break;
    }

  }

  while (parens > 0) {
    ValNodeCopyStr (head, 0, ")");
    parens--;
  }
}

static void DoFlatLoc (
  ValNodePtr PNTR head,
  BioseqPtr bsp,
  SeqLocPtr location,
  Boolean ok_to_complement
)

{
  Boolean        found_null;
  SeqLocPtr      next_loc;
  PackSeqPntPtr  pspp;
  SeqLocPtr      slp;

  if (head == NULL || bsp == NULL || location == NULL) return;

  /* deal with complement of entire location */

  if (ok_to_complement && SeqLocStrand (location) == Seq_strand_minus) {
    slp = AsnIoMemCopy ((Pointer) location,
                        (AsnReadFunc) SeqLocAsnRead,
                        (AsnWriteFunc) SeqLocAsnWrite);
    if (slp != NULL) {
      SeqLocRevCmp (slp);
      ValNodeCopyStr (head, 0, "complement(");
      DoFlatLoc (head, bsp, slp, FALSE);
      ValNodeCopyStr (head, 0, ")");
    }
    SeqLocFree (slp);
    return;
  }

  /* handle each location component */

  for (slp = location; slp != NULL; slp = slp->next) {

    if (slp->choice == SEQLOC_NULL || FlatVirtLoc (bsp, slp)) continue;

    /* print comma between components */

    if (slp != location) {
      ValNodeCopyStr (head, 0, ",");
    }

    switch (slp->choice) {
      case SEQLOC_MIX :
      case SEQLOC_PACKED_INT :
        found_null = FALSE;
        for (next_loc = (SeqLocPtr) slp->data.ptrvalue;
         next_loc != NULL;
         next_loc = next_loc->next) {
          if (next_loc->choice == SEQLOC_NULL ||
          FlatVirtLoc (bsp, next_loc)     ||
          LocationHasNullsBetween (slp))
            found_null = TRUE;
        }
        if (found_null) {
          GroupFlatLoc (head, bsp, slp, "order(", TRUE);
        } else {
          GroupFlatLoc (head, bsp, slp, "join(", FALSE);
        }
        break;
      case SEQLOC_EQUIV :
        GroupFlatLoc (head, bsp, slp, "one-of(", FALSE);
        break;
      case SEQLOC_PACKED_PNT :
        pspp = (PackSeqPntPtr) slp->data.ptrvalue;
        if (pspp != NULL) {
          FlatPackedPoint (head, pspp, bsp);
        }
        break;
      default :
        FlatLocElement (head, bsp, slp);
        break;
    }

  }
}

static CharPtr FlatLoc (
  BioseqPtr bsp,
  SeqLocPtr location,
  Boolean masterStyle
)

{
  ValNodePtr  head = NULL;
  SeqLocPtr   loc;
  Boolean     noLeft;
  Boolean     noRight;
  Uint1       num = 1;
  CharPtr     str;

  if (bsp == NULL || location == NULL) return NULL;

  if (! order_initialized) {
    order [SEQID_GENBANK] = num++;
    order [SEQID_EMBL] = num++;
    order [SEQID_DDBJ] = num++;
    order [SEQID_LOCAL] = num++;
    order [SEQID_OTHER] = num++;
    order [SEQID_TPG] = num++;
    order [SEQID_TPE] = num++;
    order [SEQID_TPD] = num++;
    order [SEQID_GIBBSQ] = num++;
    order [SEQID_GIBBMT] = num++;
    order [SEQID_PRF] = num++;
    order [SEQID_PDB] = num++;
    order [SEQID_PIR] = num++;
    order [SEQID_SWISSPROT] = num++;
    order [SEQID_PATENT] = num++;
    order [SEQID_GI] = num++;;
    order [SEQID_GENERAL] = num++;
    order [SEQID_GIIM] = num++;
    order_initialized = TRUE;
  }

  if (masterStyle) {

    /* map location from parts to segmented bioseq */

    CheckSeqLocForPartial (location, &noLeft, &noRight);
    loc = SeqLocMerge (bsp, location, NULL, FALSE, TRUE, FALSE);
    if (loc == NULL) {
      return StringSave ("?");
    }
    FreeAllFuzz (loc);
    SetSeqLocPartial (loc, noLeft, noRight);

    DoFlatLoc (&head, bsp, loc, TRUE);

    SeqLocFree (loc);

  } else {
    DoFlatLoc (&head, bsp, location, TRUE);
  }

  str = MergeValNodeStrings (head);
  ValNodeFreeData (head);

  return str;
}

static void SubSourceToQualArray (
  SubSourcePtr ssp,
  QualValPtr qvp
)

{
  Int2   idx;
  Uint1  subtype;

  if (ssp == NULL || qvp == NULL) return;

  while (ssp != NULL) {
    subtype = ssp->subtype;
    if (subtype == 255) {
      subtype = 29;
    }
    if (subtype < 30) {
      idx = subSourceToSourceIdx [subtype];
      if (idx > 0 && idx < ASN2GNBK_TOTAL_SOURCE) {
        if (qvp [idx].ssp == NULL) {
          qvp [idx].ssp = ssp;
        }
      }
    }
    ssp = ssp->next;
  }
}

static Int2 orgModToSourceIdx [35] = {
  SCQUAL_zero_orgmod,
  SCQUAL_one_orgmod,
  SCQUAL_strain,
  SCQUAL_sub_strain,
  SCQUAL_type,
  SCQUAL_sub_type,
  SCQUAL_variety,
  SCQUAL_serotype,
  SCQUAL_serogroup,
  SCQUAL_serovar,
  SCQUAL_cultivar,
  SCQUAL_pathovar,
  SCQUAL_chemovar,
  SCQUAL_biovar,
  SCQUAL_biotype,
  SCQUAL_group,
  SCQUAL_sub_group,
  SCQUAL_isolate,
  SCQUAL_common,
  SCQUAL_acronym,
  SCQUAL_dosage,
  SCQUAL_spec_or_nat_host,
  SCQUAL_sub_species,
  SCQUAL_specimen_voucher,
  SCQUAL_authority,
  SCQUAL_forma,
  SCQUAL_forma_specialis,
  SCQUAL_ecotype,
  SCQUAL_synonym,
  SCQUAL_anamorph,
  SCQUAL_teleomorph,
  SCQUAL_breed,
  SCQUAL_old_lineage,
  SCQUAL_old_name,
  SCQUAL_orgmod_note
};

static void OrgModToQualArray (
  OrgModPtr omp,
  QualValPtr qvp
)

{
  Int2   idx;
  Uint1  subtype;

  if (omp == NULL || qvp == NULL) return;

  while (omp != NULL) {
    subtype = omp->subtype;
    if (subtype == 253) {
      subtype = 32;
    } else if (subtype == 254) {
      subtype = 33;
    } else if (subtype == 255) {
      subtype = 34;
    }
    if (subtype < 35) {
      idx = orgModToSourceIdx [subtype];
      if (idx > 0 && idx < ASN2GNBK_TOTAL_SOURCE) {
        if (qvp [idx].omp == NULL) {
          qvp [idx].omp = omp;
        }
      }
    }
    omp = omp->next;
  }
}

static CharPtr organelleQual [] = {
  NULL,
  NULL,
  "/organelle=\"plastid:chloroplast\"",
  "/organelle=\"plastid:chromoplast\"",
  "/organelle=\"mitochondrion:kinetoplast\"",
  "/organelle=\"mitochondrion\"",
  "/organelle=\"plastid\"",
  "/macronuclear",
  NULL,
  "/plasmid",
  "/transposon",
  "/insertion_seq",
  "/organelle=\"plastid:cyanelle\"",
  "/proviral",
  "/virion",
  "/organelle=\"nucleomorph\"",
  "/organelle=\"plastid:apicoplast\"",
  "/organelle=\"plastid:leucoplast\"",
  "/organelle=\"plastid:proplastid\"",
  NULL
};

static Boolean StringIsJustQuotes (
  CharPtr str
)

{
  Nlm_Uchar  ch;    /* to use 8bit characters in multibyte languages */

  if (str != NULL) {
    ch = *str;
    while (ch != '\0') {
      if (ch > ' ' && ch != '"' && ch != '\'') {
        return FALSE;
      }
      str++;
      ch = *str;
    }
  }
  return TRUE;
}

static CharPtr FormatSourceFeatBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
)

{
  Boolean            add_period;
  IntAsn2gbJobPtr    ajp;
  Asn2gbSectPtr      asp;
  BioSourcePtr       biop = NULL;
  BioseqPtr          bsp;
  Char               buf [80];
  CharPtr            common = NULL;
  DbtagPtr           dbt;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  ValNodePtr         head;
  Int2               i;
  Uint1              idx;
  IntSrcBlockPtr     isp;
  Boolean            is_desc = TRUE;
  Int2               j;
  Uint1              jdx;
  Uint1              lastomptype;
  Uint1              lastssptype;
  SeqLocPtr          location = NULL;
  CharPtr            notestr;
  Uint1Ptr           notetbl = NULL;
  Boolean            okay;
  ObjectIdPtr        oip;
  OrgModPtr          omp;
  OrgNamePtr         onp = NULL;
  OrgRefPtr          orp = NULL;
  CharPtr            prefix;
  Uint1Ptr           qualtbl = NULL;
  QualValPtr         qvp;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp = NULL;
  SubSourcePtr       ssp;
  CharPtr            str;
  CharPtr            taxname = NULL;
  ValNodePtr         vnp;
  CharPtr            wwwbuf;

  if (afp == NULL || bbp == NULL) return NULL;
  ajp = afp->ajp;
  if (ajp == NULL) return NULL;
  asp = afp->asp;
  if (asp == NULL) return NULL;
  bsp = asp->bsp;
  if (bsp == NULL) return NULL;
  qvp = afp->qvp;
  if (qvp == NULL) return NULL;

  if (! StringHasNoText (bbp->string)) return StringSave (bbp->string);

  isp = (IntSrcBlockPtr) bbp;

  /* could be descriptor or feature */

  if (bbp->itemtype == OBJ_SEQDESC) {
    sdp = SeqMgrGetDesiredDescriptor (bbp->entityID, NULL, bbp->itemID, 0, NULL, &dcontext);
    if (sdp != NULL && dcontext.seqdesctype == Seq_descr_source) {
      biop = (BioSourcePtr) sdp->data.ptrvalue;
    }
  } else if (bbp->itemtype == OBJ_SEQFEAT) {
    sfp = SeqMgrGetDesiredFeature (bbp->entityID, NULL, bbp->itemID, 0, NULL, &fcontext);
    if (sfp != NULL && fcontext.seqfeattype == SEQFEAT_BIOSRC) {
      biop = (BioSourcePtr) sfp->data.value.ptrvalue;
    }
    is_desc = FALSE;
  }

  if (biop == NULL) return NULL;

  gb_StartPrint (afp->format, 5, 21, NULL, 0, 5, 21, "FT", FALSE);
  ff_AddString ("source");
  TabToColumn (22);

  location = isp->loc;

  str = FlatLoc (bsp, location, ajp->masterStyle);
  if (get_www ()) {
    wwwbuf = www_featloc (str);
    ff_AddString (wwwbuf);
    MemFree (wwwbuf);
  } else {
    ff_AddString (str);
  }
  MemFree (str);

  orp = biop->org;
  if (orp != NULL) {
    taxname = orp->taxname;
    /* common = orp->common; */
  }
  if (StringHasNoText (taxname)) {
    if (ajp->flags.needOrganismQual) {
      taxname = "unknown";
      common = orp->common;
#ifdef ASN2GNBK_PRINT_UNKNOWN_ORG
    } else {
      taxname = "unknown";
      common = orp->common;
#endif
    }
  }

  /* populate qualifier table from biosource fields */

  qvp [SCQUAL_organism].str = taxname;
  qvp [SCQUAL_common_name].str = common;

  if (biop->is_focus) {
    qvp [SCQUAL_focus].bool = TRUE;
  }

  SubSourceToQualArray (biop->subtype, qvp);

  if (orp != NULL) {
    onp = orp->orgname;
    if (onp != NULL) {
      OrgModToQualArray (onp->mod, qvp);
    }

    if (! is_desc) {
      qvp [SCQUAL_unstructured].vnp = orp->mod;
    }
    qvp [SCQUAL_db_xref].vnp = orp->db;
  }

  /* organelle currently prints /mitochondrion, /virion, etc. */

  qvp [SCQUAL_organelle].num = biop->genome;

  /* some qualifiers are flags in genome and names in subsource, print once with name */

  if (qvp [SCQUAL_ins_seq_name].ssp != NULL &&
      qvp [SCQUAL_organelle].num == GENOME_insertion_seq) {
    qvp [SCQUAL_organelle].num = 0;
  }
  if (qvp [SCQUAL_plasmid_name].ssp != NULL &&
      qvp [SCQUAL_organelle].num == GENOME_plasmid) {
    qvp [SCQUAL_organelle].num = 0;
  }
  /* AF095904.1
  if (qvp [SCQUAL_plastid_name].ssp != NULL &&
      qvp [SCQUAL_organelle].num == GENOME_plastid) {
    qvp [SCQUAL_organelle].num = 0;
  }
  */
  if (qvp [SCQUAL_transposon_name].ssp != NULL &&
      qvp [SCQUAL_organelle].num == GENOME_transposon) {
    qvp [SCQUAL_organelle].num = 0;
  }

  if (sfp != NULL) {
    qvp [SCQUAL_seqfeat_note].str = sfp->comment;
  }

  /* now print qualifiers from table */

  qualtbl = source_qual_order;
  if (is_desc) {
    notetbl = source_desc_note_order;
  } else {
    notetbl = source_feat_note_order;
  }

  for (i = 0, idx = qualtbl [i]; idx != 0; i++, idx = qualtbl [i]) {

    lastomptype = 0;
    lastssptype = 0;
    switch (asn2gnbk_source_quals [idx].qualclass) {

      case Qual_class_ignore :
        break;

      case Qual_class_string :
        if (! StringHasNoText (qvp [idx].str)) {
          NewContLine ();
          sprintf (buf, "/%s=\"", asn2gnbk_source_quals [idx].name);
          gb_AddString (buf, qvp [idx].str, "\"", FALSE, TRUE, TILDE_TO_SPACES);
        }
        break;

      case Qual_class_boolean :
        if (qvp [idx].bool) {
          NewContLine ();
          sprintf (buf, "/%s", asn2gnbk_source_quals [idx].name);
          ff_AddString (buf);
        }
        break;

      case Qual_class_organelle :
        j = (Int2) qvp [idx].num;
        if (organelleQual [j] != NULL) {
          NewContLine ();
          sprintf (buf, "%s", organelleQual [j]);
          ff_AddString (buf);
        }
        break;

      case Qual_class_orgmod :
        omp = qvp [idx].omp;
        if (lastomptype == 0 && omp != NULL) {
          lastomptype = omp->subtype;
        }
        while (omp != NULL && omp->subtype == lastomptype) {
          if (StringIsJustQuotes (omp->subname)) {
            NewContLine ();
            sprintf (buf, "/%s=\"", asn2gnbk_source_quals [idx].name);
            gb_AddString (buf, NULL, "\"", FALSE, TRUE, TILDE_TO_SPACES);
          } else if (! StringHasNoText (omp->subname)) {
            NewContLine ();
            sprintf (buf, "/%s=\"", asn2gnbk_source_quals [idx].name);
            gb_AddString (buf, omp->subname, "\"", FALSE, TRUE, TILDE_TO_SPACES);
          }
          omp = omp->next;
        }
        break;

      case Qual_class_subsource :
        ssp = qvp [idx].ssp;
        if (lastssptype == 0 && ssp != NULL) {
          lastssptype = ssp->subtype;
        }
        while (ssp != NULL && ssp->subtype == lastssptype) {
          if (ssp->subtype == SUBSRC_germline ||
              ssp->subtype == SUBSRC_rearranged ||
              ssp->subtype == SUBSRC_transgenic ||
              ssp->subtype == SUBSRC_environmental_sample) {
            NewContLine ();
            sprintf (buf, "/%s", asn2gnbk_source_quals [idx].name);
            gb_AddString (buf, NULL, NULL, FALSE, TRUE, TILDE_TO_SPACES);
          } else if (StringIsJustQuotes (ssp->name)) {
            NewContLine ();
            sprintf (buf, "/%s=\"", asn2gnbk_source_quals [idx].name);
            gb_AddString (buf, NULL, "\"", FALSE, TRUE, TILDE_TO_SPACES);
          } else if (! StringHasNoText (ssp->name)) {
            NewContLine ();
            sprintf (buf, "/%s=\"", asn2gnbk_source_quals [idx].name);
            gb_AddString (buf, ssp->name, "\"", FALSE, TRUE, TILDE_TO_SPACES);
          }
          ssp = ssp->next;
        }
        break;

      case Qual_class_pubset :
        break;

      case Qual_class_quote :
        break;

      case Qual_class_noquote :
        break;

      case Qual_class_label :
        break;

      case Qual_class_db_xref :
        for (vnp = qvp [idx].vnp; vnp != NULL; vnp = vnp->next) {
          buf [0] = '\0';
          dbt = (DbtagPtr) vnp->data.ptrvalue;
          if (dbt != NULL && (! StringHasNoText (dbt->db))) {
            oip = dbt->tag;
            if (oip != NULL) {

              okay = TRUE;
              if (ajp->flags.dropBadDbxref) {
                /* if RELEASE_MODE, drop unknown dbtag */
                okay = FALSE;
                for (j = 0; legalDbXrefs [j] != NULL; j++) {
                  if (StringCmp (dbt->db, legalDbXrefs [j]) == 0) {
                    okay = TRUE;
                  }
                }
              }

              if (okay) {
                if (! StringHasNoText (oip->str)) {
                  if (StringLen (dbt->db) + StringLen (oip->str) < 80) {
                    sprintf (buf, "%s:%s", dbt->db, oip->str);
                  }
                } else {
                  sprintf (buf, "%s:%ld", dbt->db, (long) oip->id);
                }
              }
            }
          }
          if (! StringHasNoText (buf)) {
            NewContLine ();
            ff_AddString ("/db_xref=\"");
            www_db_xref (buf);
            ff_AddString ("\"");
            /*
            gb_AddString ("/db_xref=\"", buf, "\"", FALSE, TRUE, TILDE_TO_SPACES);
            */
          }
        }
        break;

      case Qual_class_illegal :
        break;

      case Qual_class_note :
        head = NULL;
        notestr = NULL;
        prefix = "";
        add_period = FALSE;

        if (biop->genome == 8) {
          AddValNodeString (&head, "", "extrachromosomal", NULL);
          prefix = "\n";
        }

        for (j = 0, jdx = notetbl [j]; jdx != 0; j++, jdx = notetbl [j]) {

          lastomptype = 0;
          lastssptype = 0;
          switch (asn2gnbk_source_quals [jdx].qualclass) {

            case Qual_class_string :
              if (! StringHasNoText (qvp [jdx].str)) {
                s_AddValNodeString_NoRedund (&head, prefix, qvp [jdx].str, NULL);
                add_period = FALSE;
                prefix = "\n";
              }
              break;

            case Qual_class_orgmod :
              omp = qvp [jdx].omp;
              if (lastomptype == 0 && omp != NULL) {
                lastomptype = omp->subtype;
              }
              while (omp != NULL && omp->subtype == lastomptype) {
                if (! StringHasNoText (omp->subname)) {
                  if (jdx == SCQUAL_orgmod_note) {
                    sprintf (buf, "%s", prefix);
                  } else {
                    sprintf (buf, "%s%s: ", prefix, asn2gnbk_source_quals [jdx].name);
                  }

                  str = StringSave (omp->subname);
                  add_period = s_RemovePeriodFromEnd (str);
                  if (jdx == SCQUAL_orgmod_note) {
                    s_AddValNodeString_NoRedund (&head, buf, str, NULL);
                  } else {
                    AddValNodeString (&head, buf, str, NULL);
                  }
                  MemFree (str);

                  if (jdx == SCQUAL_orgmod_note) {
                    if (add_period) {
                      prefix = ".\n";
                    } else {
                      prefix = "\n";
                    }
                  } else {
                    prefix = "; ";
                  }
                }
                omp = omp->next;
              }
              break;
            case Qual_class_subsource :
              ssp = qvp [jdx].ssp;
              if (lastssptype == 0 && ssp != NULL) {
                lastssptype = ssp->subtype;
              }
              while (ssp != NULL && ssp->subtype == lastssptype) {
                if (! StringHasNoText (ssp->name)) {
                  if (jdx == SCQUAL_subsource_note) {
                    sprintf (buf, "%s", prefix);
                  } else {
                    sprintf (buf, "%s%s: ", prefix, asn2gnbk_source_quals [jdx].name);
                  }

                  str = StringSave (ssp->name);
                  add_period = s_RemovePeriodFromEnd (str);
                  if (jdx == SCQUAL_subsource_note) {
                    s_AddValNodeString_NoRedund (&head, buf, str, NULL);
                  } else {
                    AddValNodeString (&head, buf, str, NULL);
                  }
                  MemFree (str);

                  if (jdx == SCQUAL_subsource_note) {
                    if (add_period) {
                      prefix = ".\n";
                    } else {
                      prefix = "\n";
                    }
                  } else {
                    prefix = "; ";
                  }
                }
                ssp = ssp->next;
              }
              break;
            case Qual_class_valnode :
              for (vnp = qvp [jdx].vnp; vnp != NULL; vnp = vnp->next) {
                str = (CharPtr) vnp->data.ptrvalue;
                if (! StringHasNoText (str)) {
                  s_AddValNodeString_NoRedund (&head, prefix, str, NULL);
                  add_period = FALSE;
                  prefix = "; ";
                }
              }
              break;
            default :
              break;
          }
        }
        if (head != NULL) {
          NewContLine ();
          notestr = MergeValNodeStrings (head);
          if (add_period) {
            s_AddPeriodToEnd (notestr);
          }


#ifdef ASN2GNBK_STRIP_NOTE_PERIODS
          if (! IsEllipsis (notestr))
            s_RemovePeriodFromEnd (notestr);
#endif

          gb_AddString (NULL, "/note=\"", NULL, FALSE, FALSE, TILDE_IGNORE);
          if (is_desc) {
            /* AB055064.1 says TILDE_IGNORE on descriptors */
            gb_AddString (NULL, notestr, NULL, FALSE, TRUE, TILDE_IGNORE);
          } else {
            /* ASZ93724.1 says TILDE_EXPAND on features */
            gb_AddString (NULL, notestr, NULL, FALSE, TRUE, TILDE_EXPAND);
          }
          gb_AddString (NULL, "\"", NULL, FALSE, FALSE, TILDE_IGNORE);

          MemFree (notestr);
          ValNodeFreeData (head);
        }
        break;
      default :
        break;
    }
  }

  /* and then deal with the various note types separately (not in order table) */

  return ff_print_string_mem (gb_MergeString (TRUE));
}

typedef struct qualfeatur {
  CharPtr     name;
  Uint1       featurclass;
} QualFeatur, PNTR QualFeaturPtr;

#define NUM_GB_QUALS 25

static QualFeatur qualToFeature [NUM_GB_QUALS] = {
  { "allele",         FTQUAL_allele         },
  { "bound_moiety",   FTQUAL_bound_moiety   },
  { "clone",          FTQUAL_clone          },
  { "codon",          FTQUAL_codon          },
  { "cons_splice",    FTQUAL_cons_splice    },
  { "direction",      FTQUAL_direction      },
  { "EC_number",      FTQUAL_EC_number      },
  { "frequency",      FTQUAL_frequency      },
  { "function",       FTQUAL_function       },
  { "insertion_seq",  FTQUAL_insertion_seq  },
  { "label",          FTQUAL_label          },
  { "map",            FTQUAL_map            },
  { "mod_base",       FTQUAL_mod_base       },
  { "number",         FTQUAL_number         },
  { "organism",       FTQUAL_organism       },
  { "PCR_conditions", FTQUAL_PCR_conditions },
  { "phenotype",      FTQUAL_phenotype      },
  { "product",        FTQUAL_product_quals  },
  { "replace",        FTQUAL_replace        },
  { "rpt_family",     FTQUAL_rpt_family     },
  { "rpt_type",       FTQUAL_rpt_type       },
  { "rpt_unit",       FTQUAL_rpt_unit       },
  { "standard_name",  FTQUAL_standard_name  },
  { "transposon",     FTQUAL_transposon     },
  { "usedin",         FTQUAL_usedin         }
};

static Int2 GbqualToFeaturIndex (
  CharPtr qualname
)

{
  Int2  L, R, mid;

  if (qualname == NULL || *qualname == '\0') return 0;

  L = 0;
  R = NUM_GB_QUALS - 1;

  while (L < R) {
    mid = (L + R) / 2;
    if (StringICmp (qualToFeature [mid].name, qualname) < 0) {
      L = mid + 1;
    } else {
      R = mid;
    }
  }

  if (StringICmp (qualToFeature [R].name, qualname) == 0) {
    return qualToFeature [R].featurclass;
  }

  return 0;
}

#define NUM_ILLEGAL_QUALS 14

static FeaturQual illegalGbqualList [NUM_ILLEGAL_QUALS] = {
  { "anticodon",      Qual_class_noquote },
  { "citation",       Qual_class_noquote },
  { "codon_start",    Qual_class_noquote },
  { "db_xref",        Qual_class_quote   },
  { "evidence",       Qual_class_noquote },
  { "exception",      Qual_class_quote   },
  { "gene",           Qual_class_quote   },
  { "note",           Qual_class_quote   },
  { "protein_id",     Qual_class_quote   },
  { "pseudo",         Qual_class_noquote },
  { "transcript_id",  Qual_class_quote   },
  { "transl_except",  Qual_class_noquote },
  { "transl_table",   Qual_class_noquote },
  { "translation",    Qual_class_quote   },
};

static Int2 IllegalGbqualToClass (
  CharPtr qualname
)

{
  Int2  L, R, mid;

  if (qualname == NULL || *qualname == '\0') return 0;

  L = 0;
  R = NUM_ILLEGAL_QUALS - 1;

  while (L < R) {
    mid = (L + R) / 2;
    if (StringICmp (illegalGbqualList [mid].name, qualname) < 0) {
      L = mid + 1;
    } else {
      R = mid;
    }
  }

  if (StringICmp (illegalGbqualList [R].name, qualname) == 0) {
    return illegalGbqualList [R].qualclass;
  }

  return 0;
}

static CharPtr trnaList [] = {
  "tRNA-Gap",
  "tRNA-Ala",
  "tRNA-Asx",
  "tRNA-Cys",
  "tRNA-Asp",
  "tRNA-Glu",
  "tRNA-Phe",
  "tRNA-Gly",
  "tRNA-His",
  "tRNA-Ile",
  "tRNA-Lys",
  "tRNA-Leu",
  "tRNA-Met",
  "tRNA-Asn",
  "tRNA-Pro",
  "tRNA-Gln",
  "tRNA-Arg",
  "tRNA-Ser",
  "tRNA-Thr",
  "tRNA-Sec",
  "tRNA-Val",
  "tRNA-Trp",
  "tRNA-OTHER",
  "tRNA-Tyr",
  "tRNA-Glx",
  NULL
};

static CharPtr evidenceText [] = {
  NULL, "experimental", "not_experimental"
};

static CharPtr secStrText [] = {
  NULL, "helix", "sheet", "turn"
};

static CharPtr  oops = "?";

static CharPtr SeqCodeNameGet (
  SeqCodeTablePtr table,
  Uint1 residue
)

{
  Uint1  index;

  if (table != NULL) {
    index = residue - table->start_at;
    if (index >= 0 && index < table->num) {
      return (table->names) [index];
    }
  }

  return oops;
}

static CharPtr Get3LetterSymbol (
  IntAsn2gbJobPtr  ajp,
  Uint1 seq_code,
  SeqCodeTablePtr table,
  Uint1 residue
)

{
  Uint1            code = Seq_code_ncbieaa;
  Int2             index;
  Uint1            new_residue;
  CharPtr          ptr;
  CharPtr          retval = NULL;
  SeqMapTablePtr   smtp;
  SeqCodeTablePtr  table_3aa;

  if (residue == 42) { /* stop codon in NCBIeaa */
    retval = "TERM";
    return retval;
  }

  if (ajp->flags.iupacaaOnly) {
    code = Seq_code_iupacaa;
  } else {
    code = Seq_code_ncbieaa;
  }

  if (seq_code != code) {
    /* if code and seq_code are identical, then smtp is NULL?? */
    smtp = SeqMapTableFind (code, seq_code);
    new_residue = SeqMapTableConvert (smtp, residue);
  } else {
    new_residue = residue;
  }

  /* The following looks for non-symbols (255) and "Undetermined" (88) */
  if ((int) new_residue == 255 || (int) new_residue == 88) {
    retval = "OTHER";
    return retval;
  } else {
    ptr = SeqCodeNameGet (table, residue);
    table_3aa=SeqCodeTableFind  (Seq_code_iupacaa3);
    if (ptr != NULL && table_3aa != NULL) {
      for (index=0; index < (int) table_3aa->num; index++) {
        if (StringCmp(ptr, (table_3aa->names) [index]) == 0) {
          retval = (table_3aa->symbols) [index];
          return retval;
        }
      }
    }
  }

  retval = "OTHER";
  return retval;
}

static Boolean MatchCit (
  ValNodePtr ppr,
  RefBlockPtr rbp
)

{
  Char        buf [121];
  size_t      len;
  Int4        uid;
  ValNodePtr  vnp;

  if (ppr == NULL || rbp == NULL) return FALSE;
  switch (ppr->choice) {
    case PUB_Muid :
      uid = ppr->data.intvalue;
      if (rbp->muid == uid) return TRUE;
      break;
    case PUB_PMid :
      uid = ppr->data.intvalue;
      if (rbp->pmid == uid) return TRUE;
      break;
    case PUB_Equiv :
      for (vnp = (ValNodePtr) ppr->data.ptrvalue; vnp != NULL; vnp = vnp->next) {
        if (MatchCit (vnp, rbp)) return TRUE;
      }
      break;
    default :
      PubLabelUnique (ppr, buf, sizeof (buf) - 1, OM_LABEL_CONTENT, TRUE);
      len = StringLen (buf);
      if (len > 0 && buf [len - 1] == '>') {
        buf [len - 1] = '\0';
        len--;
      }
      len = MIN (len, StringLen (rbp->uniquestr));
      if (StringNICmp (rbp->uniquestr, buf, len) == 0) return TRUE;
      break;
  }
  return FALSE;
}

static Int2 MatchRef (
  ValNodePtr ppr,
  RefBlockPtr PNTR rbpp,
  Int2 numReferences
)

{
  Int2         j;
  RefBlockPtr  rbp;

  if (ppr == NULL || rbpp == NULL) return 0;

  for (j = 0; j < numReferences; j++) {
    rbp = rbpp [j];
    if (rbp == NULL) continue;
    if (MatchCit (ppr, rbp)) return j + 1;
  }
  return 0;
}

/* taken from asn2ff4.c */

static Boolean LookForFuzz (SeqLocPtr head)
{
  Boolean retval=FALSE;
  IntFuzzPtr ifp;
  PackSeqPntPtr pspp;
  SeqIntPtr sip;
  SeqLocPtr slp;
  SeqPntPtr spp;

  if (head == NULL)
    return retval;

  slp=NULL;
  while ((slp = SeqLocFindNext(head, slp)) != NULL)
  {
    switch (slp->choice)
    {
      case SEQLOC_INT:
        sip = (SeqIntPtr)(slp->data.ptrvalue);
        ifp = sip->if_from;
        if (ifp != NULL)
        {
          if (ifp->choice == 4)
          {
            if (ifp->a != 0)
              retval=TRUE;
          }
          else
            retval = TRUE;
        }
        ifp = sip->if_to;
        if (ifp != NULL)
        {
          if (ifp->choice == 4)
          {
            if (ifp->a != 0)
              retval=TRUE;
          }
          else
            retval = TRUE;
        }
        break;
      case SEQLOC_PNT:
        spp = (SeqPntPtr)(slp->data.ptrvalue);
        ifp = spp->fuzz;
        if (ifp != NULL)
        {
          if (ifp->choice == 4)
          {
            if (ifp->a != 0)
              retval=TRUE;
          }
          else
            retval = TRUE;
        }
        break;
      case SEQLOC_PACKED_PNT:
        pspp = (PackSeqPntPtr)(slp->data.ptrvalue);
        ifp = pspp->fuzz;
        if (ifp != NULL)
        {
          if (ifp->choice == 4)
          {
            if (ifp->a != 0)
              retval=TRUE;
          }
          else
            retval = TRUE;
        }
        break;
      default:
        break;
    }
    if (retval == TRUE)
      break;
  }
  return retval;
}

static CharPtr siteList [] = {
  NULL,
  "active",
  "binding",
  "cleavage",
  "inhibit",
  "modified",
  "glycosylation",
  "myristoylation",
  "mutagenized",
  "metal-binding",
  "phosphorylation",
  "acetylation",
  "amidation",
  "methylation",
  "hydroxylation",
  "sulfatation",
  "oxidative-deamination",
  "pyrrolidone-carboxylic-acid",
  "gamma-carboxyglutamic-acid",
  "blocked",
  "lipid-binding",
  "np-binding",
  "DNA binding",
  "signal-peptide",
  "transit-peptide",
  "transmembrane-region",
  "unclassified"
};

static CharPtr conflict_msg =
"Protein sequence is in conflict with the conceptual translation";

static CharPtr no_protein_msg =
"Coding region translates with internal stops";

/**/
/*  s_DisplayQVP () -- Displays the strings in a QVP structure.   */
/*                     This is a debugging function only.         */
/**/

static void s_DisplayQVP(QualValPtr qvp, Uint1Ptr notetbl)
{
  Int2 j;
  Int2 jdx;

  fprintf(stderr,"\n");
  for (j = 0, jdx = notetbl [j]; jdx != 0; j++, jdx = notetbl [j])
    {
      if (((int) qvp[jdx].str != 0x1000000) &&
      ((int) qvp[jdx].str != 0x1) &&
      ((int) qvp[jdx].str != 0xb) &&
      (qvp[jdx].str != NULL))
    fprintf(stderr, "%d\t%-25s %s\n", j, asn2gnbk_featur_quals[jdx].name,
        qvp[jdx].str);
      else
    fprintf(stderr, "%d\t%-25s %s\n", j, asn2gnbk_featur_quals[jdx].name,
        "NULL");
    }
}

static Boolean NotInGeneSyn (
  CharPtr str,
  ValNodePtr gene_syn)

{
  CharPtr     syn;
  ValNodePtr  vnp;

  for (vnp = gene_syn; vnp != NULL; vnp = vnp->next) {
    syn = (CharPtr) vnp->data.ptrvalue;
    if (! StringHasNoText (syn)) {
      if (StringICmp (str, syn) == 0) return FALSE;
    }
  }
  return TRUE;
}

typedef struct valqualstruc {
  Uint2       featdef;
  FtQualType  ftqual;
} ValQual, PNTR ValQualPtr;

/*
   WARNING - This list MUST be kept sorted in FEATDEF order as the primary
   key, and within a FEATDEF group sorted by FTQUAL as the secondary key
*/

static ValQual legalGbqualList [] = {

  {FEATDEF_GENE ,  FTQUAL_allele},
  {FEATDEF_GENE ,  FTQUAL_function},
  {FEATDEF_GENE ,  FTQUAL_label},
  {FEATDEF_GENE ,  FTQUAL_map},
  {FEATDEF_GENE ,  FTQUAL_phenotype},
  {FEATDEF_GENE ,  FTQUAL_product},
  {FEATDEF_GENE ,  FTQUAL_standard_name},
  {FEATDEF_GENE ,  FTQUAL_usedin},

  {FEATDEF_CDS ,  FTQUAL_allele},
  {FEATDEF_CDS ,  FTQUAL_codon},
  {FEATDEF_CDS ,  FTQUAL_label},
  {FEATDEF_CDS ,  FTQUAL_map},
  {FEATDEF_CDS ,  FTQUAL_number},
  {FEATDEF_CDS ,  FTQUAL_standard_name},
  {FEATDEF_CDS ,  FTQUAL_usedin},

  {FEATDEF_PROT ,  FTQUAL_product},

  {FEATDEF_preRNA ,  FTQUAL_allele},
  {FEATDEF_preRNA ,  FTQUAL_function},
  {FEATDEF_preRNA ,  FTQUAL_label},
  {FEATDEF_preRNA ,  FTQUAL_map},
  {FEATDEF_preRNA ,  FTQUAL_product},
  {FEATDEF_preRNA ,  FTQUAL_standard_name},
  {FEATDEF_preRNA ,  FTQUAL_usedin},

  {FEATDEF_mRNA ,  FTQUAL_allele},
  {FEATDEF_mRNA ,  FTQUAL_function},
  {FEATDEF_mRNA ,  FTQUAL_label},
  {FEATDEF_mRNA ,  FTQUAL_map},
  {FEATDEF_mRNA ,  FTQUAL_product},
  {FEATDEF_mRNA ,  FTQUAL_standard_name},
  {FEATDEF_mRNA ,  FTQUAL_usedin},

  {FEATDEF_tRNA ,  FTQUAL_function},
  {FEATDEF_tRNA ,  FTQUAL_label},
  {FEATDEF_tRNA ,  FTQUAL_map},
  {FEATDEF_tRNA ,  FTQUAL_product},
  {FEATDEF_tRNA ,  FTQUAL_standard_name},
  {FEATDEF_tRNA ,  FTQUAL_usedin},

  {FEATDEF_rRNA ,  FTQUAL_function},
  {FEATDEF_rRNA ,  FTQUAL_label},
  {FEATDEF_rRNA ,  FTQUAL_map},
  {FEATDEF_rRNA ,  FTQUAL_product},
  {FEATDEF_rRNA ,  FTQUAL_standard_name},
  {FEATDEF_rRNA ,  FTQUAL_usedin},

  {FEATDEF_snRNA ,  FTQUAL_function},
  {FEATDEF_snRNA ,  FTQUAL_label},
  {FEATDEF_snRNA ,  FTQUAL_map},
  {FEATDEF_snRNA ,  FTQUAL_product},
  {FEATDEF_snRNA ,  FTQUAL_standard_name},
  {FEATDEF_snRNA ,  FTQUAL_usedin},

  {FEATDEF_scRNA ,  FTQUAL_function},
  {FEATDEF_scRNA ,  FTQUAL_label},
  {FEATDEF_scRNA ,  FTQUAL_map},
  {FEATDEF_scRNA ,  FTQUAL_product},
  {FEATDEF_scRNA ,  FTQUAL_standard_name},
  {FEATDEF_scRNA ,  FTQUAL_usedin},

  {FEATDEF_otherRNA ,  FTQUAL_function},
  {FEATDEF_otherRNA ,  FTQUAL_label},
  {FEATDEF_otherRNA ,  FTQUAL_map},
  {FEATDEF_otherRNA ,  FTQUAL_product},
  {FEATDEF_otherRNA ,  FTQUAL_standard_name},
  {FEATDEF_otherRNA ,  FTQUAL_usedin},

  {FEATDEF_attenuator ,  FTQUAL_label},
  {FEATDEF_attenuator ,  FTQUAL_map},
  {FEATDEF_attenuator ,  FTQUAL_phenotype},
  {FEATDEF_attenuator ,  FTQUAL_usedin},

  {FEATDEF_C_region ,  FTQUAL_label},
  {FEATDEF_C_region ,  FTQUAL_map},
  {FEATDEF_C_region ,  FTQUAL_product},
  {FEATDEF_C_region ,  FTQUAL_standard_name},
  {FEATDEF_C_region ,  FTQUAL_usedin},

  {FEATDEF_CAAT_signal ,  FTQUAL_label},
  {FEATDEF_CAAT_signal ,  FTQUAL_map},
  {FEATDEF_CAAT_signal ,  FTQUAL_usedin},

  {FEATDEF_Imp_CDS ,  FTQUAL_codon},
  {FEATDEF_Imp_CDS ,  FTQUAL_EC_number},
  {FEATDEF_Imp_CDS ,  FTQUAL_function},
  {FEATDEF_Imp_CDS ,  FTQUAL_label},
  {FEATDEF_Imp_CDS ,  FTQUAL_map},
  {FEATDEF_Imp_CDS ,  FTQUAL_number},
  {FEATDEF_Imp_CDS ,  FTQUAL_product},
  {FEATDEF_Imp_CDS ,  FTQUAL_standard_name},
  {FEATDEF_Imp_CDS ,  FTQUAL_usedin},

  {FEATDEF_conflict ,  FTQUAL_label},
  {FEATDEF_conflict ,  FTQUAL_map},
  {FEATDEF_conflict ,  FTQUAL_replace},
  {FEATDEF_conflict ,  FTQUAL_usedin},

  {FEATDEF_D_loop ,  FTQUAL_label},
  {FEATDEF_D_loop ,  FTQUAL_map},
  {FEATDEF_D_loop ,  FTQUAL_usedin},

  {FEATDEF_D_segment ,  FTQUAL_label},
  {FEATDEF_D_segment ,  FTQUAL_map},
  {FEATDEF_D_segment ,  FTQUAL_product},
  {FEATDEF_D_segment ,  FTQUAL_standard_name},
  {FEATDEF_D_segment ,  FTQUAL_usedin},

  {FEATDEF_enhancer ,  FTQUAL_label},
  {FEATDEF_enhancer ,  FTQUAL_map},
  {FEATDEF_enhancer ,  FTQUAL_standard_name},
  {FEATDEF_enhancer ,  FTQUAL_usedin},

  {FEATDEF_exon ,  FTQUAL_allele},
  {FEATDEF_exon ,  FTQUAL_EC_number},
  {FEATDEF_exon ,  FTQUAL_function},
  {FEATDEF_exon ,  FTQUAL_label},
  {FEATDEF_exon ,  FTQUAL_map},
  {FEATDEF_exon ,  FTQUAL_number},
  {FEATDEF_exon ,  FTQUAL_product},
  {FEATDEF_exon ,  FTQUAL_standard_name},
  {FEATDEF_exon ,  FTQUAL_usedin},

  {FEATDEF_GC_signal ,  FTQUAL_label},
  {FEATDEF_GC_signal ,  FTQUAL_map},
  {FEATDEF_GC_signal ,  FTQUAL_usedin},

  {FEATDEF_iDNA ,  FTQUAL_function},
  {FEATDEF_iDNA ,  FTQUAL_label},
  {FEATDEF_iDNA ,  FTQUAL_map},
  {FEATDEF_iDNA ,  FTQUAL_number},
  {FEATDEF_iDNA ,  FTQUAL_standard_name},
  {FEATDEF_iDNA ,  FTQUAL_usedin},

  {FEATDEF_intron ,  FTQUAL_allele},
  {FEATDEF_intron ,  FTQUAL_cons_splice},
  {FEATDEF_intron ,  FTQUAL_function},
  {FEATDEF_intron ,  FTQUAL_label},
  {FEATDEF_intron ,  FTQUAL_map},
  {FEATDEF_intron ,  FTQUAL_number},
  {FEATDEF_intron ,  FTQUAL_standard_name},
  {FEATDEF_intron ,  FTQUAL_usedin},

  {FEATDEF_J_segment ,  FTQUAL_label},
  {FEATDEF_J_segment ,  FTQUAL_map},
  {FEATDEF_J_segment ,  FTQUAL_product},
  {FEATDEF_J_segment ,  FTQUAL_standard_name},
  {FEATDEF_J_segment ,  FTQUAL_usedin},

  {FEATDEF_LTR ,  FTQUAL_function},
  {FEATDEF_LTR ,  FTQUAL_label},
  {FEATDEF_LTR ,  FTQUAL_map},
  {FEATDEF_LTR ,  FTQUAL_standard_name},
  {FEATDEF_LTR ,  FTQUAL_usedin},

  {FEATDEF_mat_peptide ,  FTQUAL_EC_number},
  {FEATDEF_mat_peptide ,  FTQUAL_function},
  {FEATDEF_mat_peptide ,  FTQUAL_label},
  {FEATDEF_mat_peptide ,  FTQUAL_map},
  {FEATDEF_mat_peptide ,  FTQUAL_product},
  {FEATDEF_mat_peptide ,  FTQUAL_standard_name},
  {FEATDEF_mat_peptide ,  FTQUAL_usedin},

  {FEATDEF_misc_binding ,  FTQUAL_bound_moiety},
  {FEATDEF_misc_binding ,  FTQUAL_function},
  {FEATDEF_misc_binding ,  FTQUAL_label},
  {FEATDEF_misc_binding ,  FTQUAL_map},
  {FEATDEF_misc_binding ,  FTQUAL_usedin},

  {FEATDEF_misc_difference ,  FTQUAL_clone},
  {FEATDEF_misc_difference ,  FTQUAL_label},
  {FEATDEF_misc_difference ,  FTQUAL_map},
  {FEATDEF_misc_difference ,  FTQUAL_phenotype},
  {FEATDEF_misc_difference ,  FTQUAL_replace},
  {FEATDEF_misc_difference ,  FTQUAL_standard_name},
  {FEATDEF_misc_difference ,  FTQUAL_usedin},

  {FEATDEF_misc_feature ,  FTQUAL_function},
  {FEATDEF_misc_feature ,  FTQUAL_label},
  {FEATDEF_misc_feature ,  FTQUAL_map},
  {FEATDEF_misc_feature ,  FTQUAL_number},
  {FEATDEF_misc_feature ,  FTQUAL_phenotype},
  {FEATDEF_misc_feature ,  FTQUAL_product},
  {FEATDEF_misc_feature ,  FTQUAL_standard_name},
  {FEATDEF_misc_feature ,  FTQUAL_usedin},

  {FEATDEF_misc_recomb ,  FTQUAL_label},
  {FEATDEF_misc_recomb ,  FTQUAL_map},
  {FEATDEF_misc_recomb ,  FTQUAL_organism},
  {FEATDEF_misc_recomb ,  FTQUAL_standard_name},
  {FEATDEF_misc_recomb ,  FTQUAL_usedin},

  {FEATDEF_misc_signal ,  FTQUAL_function},
  {FEATDEF_misc_signal ,  FTQUAL_label},
  {FEATDEF_misc_signal ,  FTQUAL_map},
  {FEATDEF_misc_signal ,  FTQUAL_phenotype},
  {FEATDEF_misc_signal ,  FTQUAL_standard_name},
  {FEATDEF_misc_signal ,  FTQUAL_usedin},

  {FEATDEF_misc_structure ,  FTQUAL_function},
  {FEATDEF_misc_structure ,  FTQUAL_label},
  {FEATDEF_misc_structure ,  FTQUAL_map},
  {FEATDEF_misc_structure ,  FTQUAL_standard_name},
  {FEATDEF_misc_structure ,  FTQUAL_usedin},

  {FEATDEF_modified_base ,  FTQUAL_frequency},
  {FEATDEF_modified_base ,  FTQUAL_label},
  {FEATDEF_modified_base ,  FTQUAL_map},
  {FEATDEF_modified_base ,  FTQUAL_mod_base},
  {FEATDEF_modified_base ,  FTQUAL_usedin},

  {FEATDEF_N_region ,  FTQUAL_label},
  {FEATDEF_N_region ,  FTQUAL_map},
  {FEATDEF_N_region ,  FTQUAL_product},
  {FEATDEF_N_region ,  FTQUAL_standard_name},
  {FEATDEF_N_region ,  FTQUAL_usedin},

  {FEATDEF_old_sequence ,  FTQUAL_label},
  {FEATDEF_old_sequence ,  FTQUAL_map},
  {FEATDEF_old_sequence ,  FTQUAL_replace},
  {FEATDEF_old_sequence ,  FTQUAL_usedin},

  {FEATDEF_polyA_signal ,  FTQUAL_label},
  {FEATDEF_polyA_signal ,  FTQUAL_map},
  {FEATDEF_polyA_signal ,  FTQUAL_usedin},

  {FEATDEF_polyA_site ,  FTQUAL_label},
  {FEATDEF_polyA_site ,  FTQUAL_map},
  {FEATDEF_polyA_site ,  FTQUAL_usedin},

  {FEATDEF_prim_transcript ,  FTQUAL_allele},
  {FEATDEF_prim_transcript ,  FTQUAL_function},
  {FEATDEF_prim_transcript ,  FTQUAL_label},
  {FEATDEF_prim_transcript ,  FTQUAL_map},
  {FEATDEF_prim_transcript ,  FTQUAL_standard_name},
  {FEATDEF_prim_transcript ,  FTQUAL_usedin},

  {FEATDEF_primer_bind ,  FTQUAL_label},
  {FEATDEF_primer_bind ,  FTQUAL_map},
  {FEATDEF_primer_bind ,  FTQUAL_PCR_conditions},
  {FEATDEF_primer_bind ,  FTQUAL_standard_name},
  {FEATDEF_primer_bind ,  FTQUAL_usedin},

  {FEATDEF_promoter ,  FTQUAL_function},
  {FEATDEF_promoter ,  FTQUAL_label},
  {FEATDEF_promoter ,  FTQUAL_map},
  {FEATDEF_promoter ,  FTQUAL_phenotype},
  {FEATDEF_promoter ,  FTQUAL_standard_name},
  {FEATDEF_promoter ,  FTQUAL_usedin},

  {FEATDEF_protein_bind ,  FTQUAL_bound_moiety},
  {FEATDEF_protein_bind ,  FTQUAL_function},
  {FEATDEF_protein_bind ,  FTQUAL_label},
  {FEATDEF_protein_bind ,  FTQUAL_map},
  {FEATDEF_protein_bind ,  FTQUAL_standard_name},
  {FEATDEF_protein_bind ,  FTQUAL_usedin},

  {FEATDEF_RBS ,  FTQUAL_label},
  {FEATDEF_RBS ,  FTQUAL_map},
  {FEATDEF_RBS ,  FTQUAL_standard_name},
  {FEATDEF_RBS ,  FTQUAL_usedin},

  {FEATDEF_repeat_region ,  FTQUAL_function},
  {FEATDEF_repeat_region ,  FTQUAL_insertion_seq},
  {FEATDEF_repeat_region ,  FTQUAL_label},
  {FEATDEF_repeat_region ,  FTQUAL_map},
  {FEATDEF_repeat_region ,  FTQUAL_rpt_family},
  {FEATDEF_repeat_region ,  FTQUAL_rpt_type},
  {FEATDEF_repeat_region ,  FTQUAL_rpt_unit},
  {FEATDEF_repeat_region ,  FTQUAL_standard_name},
  {FEATDEF_repeat_region ,  FTQUAL_transposon},
  {FEATDEF_repeat_region ,  FTQUAL_usedin},

  {FEATDEF_repeat_unit ,  FTQUAL_function},
  {FEATDEF_repeat_unit ,  FTQUAL_label},
  {FEATDEF_repeat_unit ,  FTQUAL_map},
  {FEATDEF_repeat_unit ,  FTQUAL_rpt_family},
  {FEATDEF_repeat_unit ,  FTQUAL_rpt_type},
  {FEATDEF_repeat_unit ,  FTQUAL_usedin},

  {FEATDEF_rep_origin ,  FTQUAL_direction},
  {FEATDEF_rep_origin ,  FTQUAL_label},
  {FEATDEF_rep_origin ,  FTQUAL_map},
  {FEATDEF_rep_origin ,  FTQUAL_standard_name},
  {FEATDEF_rep_origin ,  FTQUAL_usedin},

  {FEATDEF_S_region ,  FTQUAL_label},
  {FEATDEF_S_region ,  FTQUAL_map},
  {FEATDEF_S_region ,  FTQUAL_product},
  {FEATDEF_S_region ,  FTQUAL_standard_name},
  {FEATDEF_S_region ,  FTQUAL_usedin},

  {FEATDEF_satellite ,  FTQUAL_label},
  {FEATDEF_satellite ,  FTQUAL_map},
  {FEATDEF_satellite ,  FTQUAL_rpt_family},
  {FEATDEF_satellite ,  FTQUAL_rpt_type},
  {FEATDEF_satellite ,  FTQUAL_rpt_unit},
  {FEATDEF_satellite ,  FTQUAL_standard_name},
  {FEATDEF_satellite ,  FTQUAL_usedin},

  {FEATDEF_sig_peptide ,  FTQUAL_function},
  {FEATDEF_sig_peptide ,  FTQUAL_label},
  {FEATDEF_sig_peptide ,  FTQUAL_map},
  {FEATDEF_sig_peptide ,  FTQUAL_product},
  {FEATDEF_sig_peptide ,  FTQUAL_standard_name},
  {FEATDEF_sig_peptide ,  FTQUAL_usedin},

  {FEATDEF_stem_loop ,  FTQUAL_function},
  {FEATDEF_stem_loop ,  FTQUAL_label},
  {FEATDEF_stem_loop ,  FTQUAL_map},
  {FEATDEF_stem_loop ,  FTQUAL_standard_name},
  {FEATDEF_stem_loop ,  FTQUAL_usedin},

  {FEATDEF_STS ,  FTQUAL_label},
  {FEATDEF_STS ,  FTQUAL_map},
  {FEATDEF_STS ,  FTQUAL_standard_name},
  {FEATDEF_STS ,  FTQUAL_usedin},

  {FEATDEF_TATA_signal ,  FTQUAL_label},
  {FEATDEF_TATA_signal ,  FTQUAL_map},
  {FEATDEF_TATA_signal ,  FTQUAL_usedin},

  {FEATDEF_terminator ,  FTQUAL_label},
  {FEATDEF_terminator ,  FTQUAL_map},
  {FEATDEF_terminator ,  FTQUAL_standard_name},
  {FEATDEF_terminator ,  FTQUAL_usedin},

  {FEATDEF_transit_peptide ,  FTQUAL_function},
  {FEATDEF_transit_peptide ,  FTQUAL_label},
  {FEATDEF_transit_peptide ,  FTQUAL_map},
  {FEATDEF_transit_peptide ,  FTQUAL_product},
  {FEATDEF_transit_peptide ,  FTQUAL_standard_name},
  {FEATDEF_transit_peptide ,  FTQUAL_usedin},

  {FEATDEF_unsure ,  FTQUAL_label},
  {FEATDEF_unsure ,  FTQUAL_map},
  {FEATDEF_unsure ,  FTQUAL_replace},
  {FEATDEF_unsure ,  FTQUAL_usedin},

  {FEATDEF_V_region ,  FTQUAL_label},
  {FEATDEF_V_region ,  FTQUAL_map},
  {FEATDEF_V_region ,  FTQUAL_product},
  {FEATDEF_V_region ,  FTQUAL_standard_name},
  {FEATDEF_V_region ,  FTQUAL_usedin},

  {FEATDEF_V_segment ,  FTQUAL_label},
  {FEATDEF_V_segment ,  FTQUAL_map},
  {FEATDEF_V_segment ,  FTQUAL_product},
  {FEATDEF_V_segment ,  FTQUAL_standard_name},
  {FEATDEF_V_segment ,  FTQUAL_usedin},

  {FEATDEF_variation ,  FTQUAL_allele},
  {FEATDEF_variation ,  FTQUAL_frequency},
  {FEATDEF_variation ,  FTQUAL_label},
  {FEATDEF_variation ,  FTQUAL_map},
  {FEATDEF_variation ,  FTQUAL_phenotype},
  {FEATDEF_variation ,  FTQUAL_product},
  {FEATDEF_variation ,  FTQUAL_replace},
  {FEATDEF_variation ,  FTQUAL_standard_name},
  {FEATDEF_variation ,  FTQUAL_usedin},

  {FEATDEF_3clip ,  FTQUAL_allele},
  {FEATDEF_3clip ,  FTQUAL_function},
  {FEATDEF_3clip ,  FTQUAL_label},
  {FEATDEF_3clip ,  FTQUAL_map},
  {FEATDEF_3clip ,  FTQUAL_standard_name},
  {FEATDEF_3clip ,  FTQUAL_usedin},

  {FEATDEF_3UTR ,  FTQUAL_allele},
  {FEATDEF_3UTR ,  FTQUAL_function},
  {FEATDEF_3UTR ,  FTQUAL_label},
  {FEATDEF_3UTR ,  FTQUAL_map},
  {FEATDEF_3UTR ,  FTQUAL_standard_name},
  {FEATDEF_3UTR ,  FTQUAL_usedin},

  {FEATDEF_5clip ,  FTQUAL_allele},
  {FEATDEF_5clip ,  FTQUAL_function},
  {FEATDEF_5clip ,  FTQUAL_label},
  {FEATDEF_5clip ,  FTQUAL_map},
  {FEATDEF_5clip ,  FTQUAL_standard_name},
  {FEATDEF_5clip ,  FTQUAL_usedin},

  {FEATDEF_5UTR ,  FTQUAL_allele},
  {FEATDEF_5UTR ,  FTQUAL_function},
  {FEATDEF_5UTR ,  FTQUAL_label},
  {FEATDEF_5UTR ,  FTQUAL_map},
  {FEATDEF_5UTR ,  FTQUAL_standard_name},
  {FEATDEF_5UTR ,  FTQUAL_usedin},

  {FEATDEF_10_signal ,  FTQUAL_label},
  {FEATDEF_10_signal ,  FTQUAL_map},
  {FEATDEF_10_signal ,  FTQUAL_standard_name},
  {FEATDEF_10_signal ,  FTQUAL_usedin},

  {FEATDEF_35_signal ,  FTQUAL_label},
  {FEATDEF_35_signal ,  FTQUAL_map},
  {FEATDEF_35_signal ,  FTQUAL_standard_name},
  {FEATDEF_35_signal ,  FTQUAL_usedin},

  {FEATDEF_REGION ,  FTQUAL_function},
  {FEATDEF_REGION ,  FTQUAL_label},
  {FEATDEF_REGION ,  FTQUAL_map},
  {FEATDEF_REGION ,  FTQUAL_number},
  {FEATDEF_REGION ,  FTQUAL_phenotype},
  {FEATDEF_REGION ,  FTQUAL_product},
  {FEATDEF_REGION ,  FTQUAL_standard_name},
  {FEATDEF_REGION ,  FTQUAL_usedin},

  {FEATDEF_mat_peptide_aa ,  FTQUAL_label},
  {FEATDEF_mat_peptide_aa ,  FTQUAL_map},
  {FEATDEF_mat_peptide_aa ,  FTQUAL_product},
  {FEATDEF_mat_peptide_aa ,  FTQUAL_standard_name},
  {FEATDEF_mat_peptide_aa ,  FTQUAL_usedin},

  {FEATDEF_sig_peptide_aa ,  FTQUAL_label},
  {FEATDEF_sig_peptide_aa ,  FTQUAL_map},
  {FEATDEF_sig_peptide_aa ,  FTQUAL_product},
  {FEATDEF_sig_peptide_aa ,  FTQUAL_standard_name},
  {FEATDEF_sig_peptide_aa ,  FTQUAL_usedin},

  {FEATDEF_transit_peptide_aa ,  FTQUAL_label},
  {FEATDEF_transit_peptide_aa ,  FTQUAL_map},
  {FEATDEF_transit_peptide_aa ,  FTQUAL_product},
  {FEATDEF_transit_peptide_aa ,  FTQUAL_standard_name},
  {FEATDEF_transit_peptide_aa ,  FTQUAL_usedin},

  {FEATDEF_snoRNA ,  FTQUAL_function},
  {FEATDEF_snoRNA ,  FTQUAL_label},
  {FEATDEF_snoRNA ,  FTQUAL_map},
  {FEATDEF_snoRNA ,  FTQUAL_product},
  {FEATDEF_snoRNA ,  FTQUAL_standard_name},
  {FEATDEF_snoRNA ,  FTQUAL_usedin}

};

/* comparison of ValQual's -- first compare featdef then ftqual */
#define COMPARE_VALQUAL(av,aq,bv,bq) ( ((av)-(bv)) ? ((av)-(bv)) : ((aq)-(bq)) )

/* Returns TRUE if {featureKey, qualKey} exists in legalGbqualList */

static Boolean AllowedValQual (Uint2 featureKey, FtQualType qualKey)

{
  Int2 L, R, mid;

  L = 0;
  R = sizeof (legalGbqualList) / sizeof (ValQual) - 1;
  while (L < R) {
    mid = (L + R) / 2;
    if (COMPARE_VALQUAL (legalGbqualList [mid].featdef,
       legalGbqualList [mid].ftqual,
       featureKey, qualKey) < 0)
      L = mid + 1;
    else
      R = mid;
  }
  if (COMPARE_VALQUAL (legalGbqualList [R].featdef,
           legalGbqualList [R].ftqual,
           featureKey, qualKey)) {
    return 0;
  }
  return 1;
}


static CharPtr validRptString [] = {
  "tandem", "inverted", "flanking", "terminal", "direct", "dispersed", "other", NULL
};

static CharPtr validLRBString [] = {
  "LEFT", "RIGHT", "BOTH", NULL
};

static CharPtr validConsSpliceString [] = {
  "(5'site:YES, 3'site:YES)",
  "(5'site:YES, 3'site:NO)",
  "(5'site:YES, 3'site:ABSENT)",
  "(5'site:NO, 3'site:YES)",
  "(5'site:NO, 3'site:NO)",
  "(5'site:NO, 3'site:ABSENT)",
  "(5'site:ABSENT, 3'site:YES)",
  "(5'site:ABSENT, 3'site:NO)",
  "(5'site:ABSENT, 3'site:ABSENT)",
  NULL
};

static CharPtr validExceptionString [] = {
  "RNA editing", "reasons given in citation", NULL
};

static Boolean StringInStringList (CharPtr testString, CharPtr PNTR stringList) {
  Int2 i;
  i = 0;
  while (stringList [i] != NULL) {
    if (StringICmp (testString, stringList [i]) == 0)
      return 1;
    i++;
  }
  return 0;
}

/*
Return values are:
 0: no problem - Accession is in proper format
-1: Accession did not start with a letter (or two letters)
-2: Accession did not contain five numbers (or six numbers after 2 letters)
-3: the original Accession number to be validated was NULL
-4: the original Accession number is too long (>10)
*/

static Int2 ValidateAccession (
  CharPtr accession
)

{
  Char     ch;
  Int2     numAlpha = 0;
  Int2     numDigits = 0;
  Int2     numUndersc = 0;
  CharPtr  str;

  if (accession == NULL || accession [0] == '\0') return -3;

  if (StringLen (accession) >= 10) return -4;

  if (accession [0] < 'A' || accession [0] > 'Z') return -1;

  str = accession;
  ch = *str;
  while (IS_ALPHA (ch)) {
    numAlpha++;
    str++;
    ch = *str;
  }
  while (ch == '_') {
    numUndersc++;
    str++;
    ch = *str;
  }
  while (IS_DIGIT (ch)) {
    numDigits++;
    str++;
    ch = *str;
  }
  if (ch != '\0' && ch != ' ') return -2;

  if (numUndersc > 1) return -2;

  if (numUndersc == 0) {
    if (numAlpha == 1 && numDigits == 5) return 0;
    if (numAlpha == 2 && numDigits == 6) return 0;
    if (numAlpha == 3 && numDigits == 5) return 0;
  } else if (numUndersc == 1) {
    if (numAlpha != 2 || numDigits != 6) return -2;
    if (accession [0] == 'N' || accession [0] == 'X') {
      if (accession [1] == 'M' ||
          accession [1] == 'C' ||
          accession [1] == 'T' ||
          accession [1] == 'P' ||
          accession [1] == 'G' ||
          accession [1] == 'R') {
        return 0;
      }
    }
  }

  return -2;
}

static CharPtr mrnaevtext1 = "Derived by automated computational analysis";
static CharPtr mrnaevtext2 = "using gene prediction method:";
static CharPtr mrnaevtext3 = "Supporting evidence includes similarity to:";

static CharPtr GetStrFormRNAEvidence (
  UserObjectPtr uop
)

{
  size_t        len;
  CharPtr       method = NULL;
  Int2          nm = 0;
  ObjectIdPtr   oip;
  CharPtr       str = NULL;
  Char          tmp [20];
  UserFieldPtr  u, ufp, uu;

  if (uop == NULL) return NULL;
  oip = uop->type;
  if (oip == NULL) return NULL;
  if (StringCmp (oip->str, "ModelEvidence") != 0) return NULL;

  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (oip == NULL || ufp->data.ptrvalue == NULL) continue;
    if (StringCmp (oip->str, "Method") == 0) {
      method = StringSaveNoNull ((CharPtr) ufp->data.ptrvalue);
    }
    if (StringCmp (oip->str, "mRNA") == 0) {
      for (u = (UserFieldPtr) ufp->data.ptrvalue; u != NULL; u = u->next) {
        if (u->data.ptrvalue == NULL) continue;
        for (uu = (UserFieldPtr) u->data.ptrvalue; uu != NULL; uu = uu->next) {
          oip = uu->label;
          if (oip == NULL) continue;
          if (StringCmp (oip->str, "accession") == 0) {
            nm++;
          }
        }
      }
    }
  }

  len = StringLen (mrnaevtext1) + StringLen (mrnaevtext2) + StringLen (mrnaevtext3) + StringLen (method) + 30;
  str = (CharPtr) MemNew (len);
  if (str == NULL) return NULL;

  if (method != NULL) {
    sprintf (str, "%s %s %s.", mrnaevtext1, mrnaevtext1, method);
  } else {
    sprintf (str, "%s.", mrnaevtext1);
  }
  if (nm > 0) {
    StringCat (str, " ");
    StringCat (str, mrnaevtext3);
    if (nm > 1) {
      sprintf (tmp, " %d mRNAs", (int) nm);
    } else {
      sprintf (tmp, " %d mRNA", (int) nm);
    }
    StringCat (str, tmp);
  }

  return str;
}


/* FormatFeatureblockQuals should not be called directly,
   except from FormatFeatureBlock.  It performs no input
   validation.  (perhaps it should?) */

static void FormatFeatureBlockQuals (
  IntAsn2gbJobPtr  ajp,
  Asn2gbFormatPtr  afp,
  Asn2gbSectPtr    asp,
  BioseqPtr        bsp,
  Uint1            featdeftype,
  ValNodePtr       gene_syn,
  CharPtr          lasttype,
  SeqLocPtr        location,
  BioseqPtr        prod,
  CharPtr          protein_pid_g,
  QualValPtr       qvp,
  Int4             left,
  Int4             right,
  Uint1            strand,
  SeqFeatPtr       sfp,
  BioseqPtr        target,
  Boolean          is_other,
  Boolean          is_gps
)

{
  Boolean            add_period;
  CharPtr            ascii;
  Int2               ascii_len;
  Boolean            at_end = FALSE;
  Char               buf[80];
  Choice             cbaa;
  CodeBreakPtr       cbp;
  Char               ch;
  Uint1              choice;
  Uint1              code = Seq_code_ncbieaa;
  ValNodePtr         dbx;
  ValNodePtr         dbxrefs = NULL;
  Int4               gi;
  ValNodePtr         head;
  Boolean            hadProtDesc = FALSE;
  DbtagPtr           dbt;
  Int4               exp_ev;
  GBQualPtr          gbq;
  Int2               i;
  Uint1              idx;
  Boolean            isTRNA;
  Int2               j;
  Uint1              jdx;
  Boolean            keep;
  Int4               len;
  SeqLocPtr          newloc;
  CharPtr            notestr;
  Char               numbuf [32];
  Int2               numcodons;
  ObjectIdPtr        oip;
  Boolean            okay;
  Boolean            only_digits;
  ValNodePtr         ppr;
  CharPtr            prefix;
  CharPtr            protein_seq = NULL;
  CharPtr            ptr;
  Uint1              residue;
  SeqCodeTablePtr    sctp;
  Int4               sec_str;
  Uint1              seqcode;
  Char               seqid [50];
  SeqIntPtr          sintp;
  SeqIdPtr           sip;
  SeqLocPtr          slp;
  Boolean            split;
  SeqPortPtr         spp;
  CharPtr            start;
  CharPtr            str;
  Boolean            suppress_period;
  CharPtr            tmp;
  tRNAPtr            trna;
  UserObjectPtr      uop;
  ValNodePtr         vnp;


  for (i = 0, idx = feat_qual_order [i]; idx != 0; i++, idx = feat_qual_order [i]) {

    lasttype = NULL;
    switch (asn2gnbk_featur_quals [idx].qualclass) {

      case Qual_class_ignore :
        break;

      case Qual_class_string :
        if (! StringHasNoText (qvp [idx].str)) {
          sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
          NewContLine ();
          gb_AddString (buf, qvp [idx].str, "\"", FALSE, TRUE, TILDE_TO_SPACES);
        }
        break;

      case Qual_class_product :
        if (StringHasNoText (qvp [idx].str) ||
            (ajp->flags.dropIllegalQuals &&
             (! AllowedValQual (featdeftype, FTQUAL_product)))) break;
        sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
        NewContLine ();
        gb_AddString (buf, qvp [idx].str, "\"", FALSE, TRUE, TILDE_TO_SPACES);
        break;

      case Qual_class_sgml :
        if (! StringHasNoText (qvp [idx].str)) {
          ascii_len = Sgml2AsciiLen (qvp [idx].str);
          start = ascii = MemNew ((size_t) (10 + ascii_len));
          if (start != NULL) {
            ascii = Sgml2Ascii (qvp [idx].str, ascii, ascii_len + 1);
            sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
            NewContLine ();
            gb_AddString (buf, start, "\"", FALSE, TRUE, TILDE_TO_SPACES);
            MemFree (start);
          }
        }
        break;

      case Qual_class_boolean :
        if (qvp [idx].bool) {
          sprintf (buf, "/%s", asn2gnbk_featur_quals [idx].name);
          NewContLine ();
          ff_AddString (buf);
        }
        break;

      case Qual_class_int :
        if (qvp [idx].num > 0) {
          if (idx == FTQUAL_transl_table) {
            NewContLine ();
            sprintf (numbuf, "/%s=", asn2gnbk_featur_quals [idx].name);
            ff_AddString (numbuf);
            sprintf (numbuf, "%ld", (long) qvp [idx].num);
            www_gcode (numbuf);
          } else {
            sprintf (numbuf, "/%s=%ld", asn2gnbk_featur_quals [idx].name, (long) qvp [idx].num);
            NewContLine ();
            ff_AddString (numbuf);
          }
        }
        break;

      case Qual_class_evidence :
        exp_ev = qvp [idx].num;
        if (exp_ev > 0 && exp_ev <= 2) {
          sprintf (numbuf, "/%s=%s", asn2gnbk_featur_quals [idx].name, evidenceText [exp_ev]);
          NewContLine ();
          ff_AddString (numbuf);
        }
        break;

      case Qual_class_valnode :
        for (vnp = qvp [idx].vnp; vnp != NULL; vnp = vnp->next) {
          str = (CharPtr) vnp->data.ptrvalue;
          if (str != NULL) {
            sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
            NewContLine ();
            gb_AddString (buf, str, "\"", FALSE, TRUE, TILDE_TO_SPACES);
          }
        }
        break;

      /* !!! Eric wonders if this EC code works */
      case Qual_class_EC_valnode :
        for (vnp = qvp [idx].vnp; vnp != NULL; vnp = vnp->next) {
          str = (CharPtr) vnp->data.ptrvalue;
          okay = TRUE;

          if (str == NULL) continue;

          if (ajp->flags.dropIllegalQuals) {
            tmp = str;
            while (*tmp != '\0' && *tmp == '\"')
              tmp++;
            for (; *tmp != '\0' && *tmp != '\"'; tmp++) {
              if (!IS_DIGIT(*tmp) && *tmp != '.' && *tmp != '-') {
                okay = FALSE;
              }
            }
          }
          if (!okay) continue;
          sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
          NewContLine ();
          gb_AddString (buf, str, "\"", FALSE, TRUE, TILDE_TO_SPACES);
        }
        break;

      case Qual_class_EC_quote :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;
        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          okay = TRUE;
          if (gbq->val == NULL) {
            okay = FALSE;
          }

          if (ajp->flags.dropIllegalQuals && okay) {
            tmp = gbq->val;
            while (*tmp != '\0' && *tmp == '\"')
              tmp++;
            for (; *tmp != '\0' && *tmp != '\"'; tmp++) {
              if (!IS_DIGIT(*tmp) && *tmp != '.' && *tmp != '-') {
                okay = FALSE;
              }
            }
          }

          if (StringHasNoText (gbq->val)) {
            okay = FALSE;
          }

          if (okay) {
            sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
            NewContLine ();
            if (StringIsJustQuotes (gbq->val)) {
              gb_AddString (buf, NULL, "\"", FALSE, TRUE, TILDE_IGNORE);
            } else {
              gb_AddString (buf, gbq->val, "\"", FALSE, TRUE, TILDE_TO_SPACES);
            }
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_quote :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;
        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          if (! StringHasNoText (gbq->val)) {
            sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
            NewContLine ();
            if (StringIsJustQuotes (gbq->val)) {
              gb_AddString (buf, NULL, "\"", FALSE, TRUE, TILDE_IGNORE);
            } else {
              gb_AddString (buf, gbq->val, "\"", FALSE, TRUE, TILDE_TO_SPACES);
            }
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_noquote :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;
        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          if (! StringHasNoText (gbq->val)) {
            sprintf (buf, "/%s=", asn2gnbk_featur_quals [idx].name);
            NewContLine ();
            gb_AddString (buf, gbq->val, NULL, FALSE, TRUE, TILDE_TO_SPACES);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_label :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;
        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          if (! StringHasNoText (gbq->val)) {
            if (ajp->flags.checkQualSyntax) { /* single token, not just numeric */
              str = gbq->val;
              ch = *str;
              only_digits = TRUE;
              while (ch != '\0') {
                if (IS_WHITESP (ch)) break; /* only single token allowed */
                if (! IS_DIGIT (ch)) {
                  only_digits = FALSE;
                }
                str++;
                ch = *str;
              }
              if (only_digits) break; /* must not be just numeric */
            }
            sprintf (buf, "/%s=", asn2gnbk_featur_quals [idx].name);
            NewContLine ();
            gb_AddString (buf, gbq->val, NULL, FALSE, TRUE, TILDE_TO_SPACES);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_number :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;

        if (ajp->flags.checkQualSyntax) {
          str = gbq->val;

          if ( StringHasNoText (str) )
            break;
          while (!IS_WHITESP (*str) && *str != '\0')
            str++;
          if (! StringHasNoText (str) )
            break;
        }

        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          if (! StringHasNoText (gbq->val)) {
            sprintf (buf, "/%s=", asn2gnbk_featur_quals [idx].name);
            NewContLine ();
            gb_AddString (buf, gbq->val, NULL, FALSE, TRUE, TILDE_TO_SPACES);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_paren :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;
        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          if (! StringHasNoText (gbq->val)) {
            tmp = StringSave (gbq->val);
            str = tmp;
            len = StringLen (str);
            if (len > 1 && *str == '(' && str [len - 1] == ')' &&
                StringChr (str, ',') != NULL) {
              str++;
              while (! StringHasNoText (str)) {
                ptr = StringChr (str, ',');
                if (ptr == NULL) {
                  ptr = StringChr (str, ')');
                }
                if (ptr != NULL) {
                  *ptr = '\0';
                  ptr++;
                }
                sprintf (buf, "/%s=", asn2gnbk_featur_quals [idx].name);
                NewContLine ();
                gb_AddString (buf, str, NULL, FALSE, TRUE, TILDE_TO_SPACES);
                str = ptr;
              }
            } else {
              sprintf (buf, "/%s=", asn2gnbk_featur_quals [idx].name);
              NewContLine ();
              gb_AddString (buf, str, NULL, FALSE, TRUE, TILDE_TO_SPACES);
            }
            MemFree (tmp);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_rpt :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;

        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          if (! StringHasNoText (gbq->val)) {
            tmp = StringSave (gbq->val);
            str = tmp;
            len = StringLen (str);
            if (len > 1 && *str == '(' && str [len - 1] == ')' &&
                StringChr (str, ',') != NULL) {
              str++;
              while (! StringHasNoText (str)) {
                ptr = StringChr (str, ',');
                if (ptr == NULL) {
                  ptr = StringChr (str, ')');
                }
                if (ptr != NULL) {
                  *ptr = '\0';
                  ptr++;
                }
                if ((! ajp->flags.checkQualSyntax) || (StringInStringList (str, validRptString))) {
                  sprintf (buf, "/%s=", asn2gnbk_featur_quals [idx].name);
                  NewContLine ();
                  gb_AddString (buf, str, NULL, FALSE, TRUE, TILDE_TO_SPACES);
                }
                str = ptr;
              }
            } else {
              if ((! ajp->flags.checkQualSyntax) || (StringInStringList (str, validRptString))) {
                sprintf (buf, "/%s=", asn2gnbk_featur_quals [idx].name);
                NewContLine ();
                gb_AddString (buf, str, NULL, FALSE, TRUE, TILDE_TO_SPACES);
              }
            }
            MemFree (tmp);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_rpt_unit :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;
        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        if (gbq->val == NULL) break;

        if (ajp->flags.checkQualSyntax) { /* must be of the form 123..4567 or a single-token label */
          Nlm_StrNCpy(buf, gbq->val, sizeof(buf));
          TrimSpacesAroundString (buf);

          /* first check for a alphanumeric token */
          str = buf;
          while (IS_ALPHANUM(*str)) str++;
          if (*str != '\0') { /* wasn't pure alphanumeric; now check for xxx..yyy */
            str = buf;
            while (IS_DIGIT(*str)) str++; /* xxx */
            if (*str == '\0' /* must be something after the xxx */
                || StringLen (str) < 3  /* need at least 2 '.'s and a digit*/
                || str[0] != '.' || str[1] != '.') break;
            str+=2;
            while (IS_DIGIT(*str)) str++;
            if (*str != '\0') break;  /* mustn't be anything after the yyy */
          }
        }

        str = gbq->val;
        sprintf (buf, "/%s=", asn2gnbk_featur_quals [idx].name);
        NewContLine ();
        gb_AddString (buf, str, NULL, FALSE, TRUE, TILDE_TO_SPACES);
        break;

      case Qual_class_replace :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;
        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          if (! StringHasNoText (gbq->val)) {
            sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
            NewContLine ();
            gb_AddString (buf, gbq->val, "\"", FALSE, TRUE, TILDE_TO_SPACES);
          } else {
            sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
            NewContLine ();
            gb_AddString (buf, NULL, "\"", FALSE, TRUE, TILDE_TO_SPACES);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_consplice :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;

        if (ajp->flags.checkQualSyntax && (! StringInStringList (gbq->val, validConsSpliceString)) ) {
          break;
        }

        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          if (! StringHasNoText (gbq->val)) {
            sprintf (buf, "/%s=", asn2gnbk_featur_quals [idx].name);
            NewContLine ();
            gb_AddString (buf, gbq->val, NULL, FALSE, TRUE, TILDE_TO_SPACES);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_site :
        if (! StringHasNoText (qvp [idx].str)) {
          NewContLine ();
          sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
          gb_AddString (buf, qvp [idx].str, "\"", FALSE, TRUE, TILDE_TO_SPACES);
        }
        break;

      case Qual_class_L_R_B :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;

        if (ajp->flags.checkQualSyntax && (! StringInStringList (gbq->val, validLRBString)) ) {
          break;
        }

        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          if (! StringHasNoText (gbq->val)) {
            sprintf (buf, "/%s=", asn2gnbk_featur_quals [idx].name);
            NewContLine ();
            gb_AddString (buf, gbq->val, NULL, FALSE, TRUE, TILDE_TO_SPACES);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_sec_str :
        sec_str = qvp [idx].num;
        if (sec_str > 0 && sec_str <= 3) {
          sprintf (numbuf, "/%s=\"%s\"", asn2gnbk_featur_quals [idx].name, secStrText [sec_str]);
          NewContLine ();
          ff_AddString (numbuf);
        }
        break;

      case Qual_class_seq_loc :
        slp = qvp [idx].slp;
        if (slp != NULL) {
          sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
          NewContLine ();
          str = FlatLoc (target, slp, ajp->masterStyle);
          gb_AddString (buf, str, "\"", FALSE, TRUE, TILDE_TO_SPACES);
          MemFree (str);
        }
        break;

      case Qual_class_code_break :
        cbp = qvp [idx].cbp;
        seqcode = 0;
        sctp = NULL;
        while (cbp != NULL) {
          cbaa = cbp->aa;
          switch (cbaa.choice) {
            case 1 :
              seqcode = Seq_code_ncbieaa;
              break;
            case 2 :
              seqcode = Seq_code_ncbi8aa;
              break;
            case 3 :
              seqcode = Seq_code_ncbistdaa;
              break;
            default :
              break;
          }
          if (seqcode != 0) {
            sctp = SeqCodeTableFind (seqcode);
            if (sctp != NULL) {
              slp = NULL;
              while ((slp = SeqLocFindNext (cbp->loc, slp)) != NULL) {
                str = NULL;
                if (ajp->ajp.slp != NULL) {
                  sip = SeqIdParse ("lcl|dummy");
                  split = FALSE;
                  newloc = SeqLocCopyRegion (sip, slp, bsp, left, right, strand, &split);
                  SeqIdFree (sip);
                  if (newloc != NULL) {
                    SeqLocReplaceID (newloc, SeqLocId (ajp->ajp.slp));
                    str = FlatLoc (target, newloc, ajp->masterStyle);
                    SeqLocFree (newloc);
                  }
                } else {
                  str = FlatLoc (target, slp, ajp->masterStyle);
                }
                if (str != NULL) {
                  residue = cbaa.value.intvalue;
                  ptr = Get3LetterSymbol (ajp, seqcode, sctp, residue);
                  if (ptr == NULL) {
                    ptr = "OTHER";
                  }
                  sprintf (buf, "/transl_except=(pos:%s,aa:%s)", str, ptr);
                  NewContLine ();
                  ff_AddString (buf);
                }
                MemFree (str);
              }
            }
          }
          cbp = cbp->next;
        }
        break;

      case Qual_class_anti_codon :
        slp = qvp [FTQUAL_anticodon].slp;
        newloc = NULL;
        if (ajp->ajp.slp != NULL) {
          sip = SeqIdParse ("lcl|dummy");
          split = FALSE;
          newloc = SeqLocCopyRegion (sip, slp, bsp, left, right, strand, &split);
          SeqIdFree (sip);
          slp = newloc;
          if (newloc != NULL) {
            SeqLocReplaceID (newloc, SeqLocId (ajp->ajp.slp));
          }
        }
        if (slp != NULL && slp->choice == SEQLOC_INT) {
          sintp = (SeqIntPtr) slp->data.ptrvalue;
          if (sintp != NULL) {
            str = qvp [FTQUAL_trna_aa].str;
            if (! StringHasNoText (str)) {
              sprintf (buf, "/anticodon=(pos:%ld..%ld,aa:%s)",
                       (long) sintp->from + 1, (long) sintp->to + 1, str);
              NewContLine ();
              ff_AddString (buf);
            }
          }
        }
        if (newloc != NULL) {
          SeqLocFree (newloc);
        }
        break;

      case Qual_class_codon :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;
        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          if (! StringHasNoText (gbq->val)) {
            sprintf (buf, "/%s=", asn2gnbk_featur_quals [idx].name);
            NewContLine ();
            gb_AddString (buf, gbq->val, NULL, FALSE, FALSE, TILDE_TO_SPACES);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_pubset :
        vnp = qvp [idx].vnp;
        if (vnp != NULL && asp->referenceArray != NULL) {
          for (ppr = vnp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
            j = MatchRef (ppr, asp->referenceArray, asp->numReferences);
            if (j > 0) {
              sprintf (buf, "%d", (int) j);
              NewContLine ();
              gb_AddString ("/citation=[", buf, "]", FALSE, TRUE, TILDE_TO_SPACES);
            }
          }
        }
        break;

      case Qual_class_db_xref :
        for (vnp = qvp [idx].vnp; vnp != NULL; vnp = vnp->next) {
          buf [0] = '\0';
          dbt = (DbtagPtr) vnp->data.ptrvalue;
          if (dbt != NULL && (! StringHasNoText (dbt->db))) {
            oip = dbt->tag;
            if (oip != NULL) {

              okay = TRUE;
              if (ajp->flags.dropBadDbxref) {
                /* if RELEASE_MODE, drop unknown dbtag */
                okay = FALSE;
                for (j = 0; legalDbXrefs [j] != NULL; j++) {
                  if (StringCmp (dbt->db, legalDbXrefs [j]) == 0) {
                    okay = TRUE;
                  }
                }
                if (! okay) {
                  if (is_gps || is_other) {
                    for (j = 0; legalRefSeqDbXrefs [j] != NULL; j++) {
                      if (StringCmp (dbt->db, legalRefSeqDbXrefs [j]) == 0) {
                        okay = TRUE;
                      }
                    }
                  }
                }
              }

              if (okay) {
                if (! StringHasNoText (oip->str)) {
                  if (StringLen (dbt->db) + StringLen (oip->str) < 80) {
                    sprintf (buf, "%s:%s", dbt->db, oip->str);
                  }
                } else {
                  sprintf (buf, "%s:%ld", dbt->db, (long) oip->id);
                }
              }
            }
          }
          if (! StringHasNoText (buf)) {
            if (StringICmp (buf, protein_pid_g) != 0) {
              keep = TRUE;
              for (dbx = dbxrefs; dbx != NULL; dbx = dbx->next) {
                if (StringICmp ((CharPtr) dbx->data.ptrvalue, buf) == 0) {
                  keep = FALSE;
                }
              }
              if (keep) {
                NewContLine ();
                ff_AddString ("/db_xref=\"");
                www_db_xref (buf);
                ff_AddString ("\"");
                /*
                gb_AddString ("/db_xref=\"", buf, "\"", FALSE, TRUE, TILDE_TO_SPACES);
                */
                ValNodeCopyStr (&dbxrefs, 0, buf);
              }
            }
          }
        }
        break;

      case Qual_class_seq_id :
        sip = qvp [idx].sip;
        if (sip != NULL) {
          /* should always be found above for protein_id or transcript_id
          prod = BioseqFind (sip);
          */
          if (prod != NULL) {
            choice = 0;
            for (sip = prod->id; sip != NULL; sip = sip->next) {
              if (sip->choice == SEQID_GENBANK ||
                  sip->choice == SEQID_EMBL ||
                  sip->choice == SEQID_DDBJ ||
                  sip->choice == SEQID_OTHER ||
                  sip->choice == SEQID_TPG ||
                  sip->choice == SEQID_TPE ||
                  sip->choice == SEQID_TPD) {
                choice = sip->choice;
                if (SeqIdWrite (sip, seqid, PRINTID_TEXTID_ACC_VER, sizeof (seqid)) != NULL) {
                  sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
                  NewContLine ();
                  ff_AddString (buf);
                  www_protein_id (seqid);
                  ff_AddString ("\"");
                  /*
                  gb_AddString (buf, seqid, "\"", FALSE, TRUE, TILDE_TO_SPACES);
                  */
                }
              }
            }
            for (sip = prod->id; sip != NULL; sip = sip->next) {
              if (sip->choice == SEQID_GI) {
                /*
                if (choice != 0) {
                  sprintf (seqid, "PID:g%ld", (long) sip->data.intvalue);
                  NewContLine ();
                  gb_AddString ("/db_xref=\"", seqid, "\"", FALSE, TRUE, TILDE_TO_SPACES);
                }
                */
                if (choice == 0) {
                  sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
                  sprintf (seqid, "%ld", (long) sip->data.intvalue);
                  NewContLine ();
                  ff_AddString (buf);
                  www_protein_id (seqid);
                  ff_AddString ("\"");
                  /*
                  gb_AddString (buf, seqid, "\"", FALSE, TRUE, TILDE_TO_SPACES);
                  */
                }
                sprintf (seqid, "GI:%ld", (long) sip->data.intvalue);
                NewContLine ();
                ff_AddString ("/db_xref=\"");
                www_db_xref (seqid);
                ff_AddString ("\"");
                /*
                gb_AddString ("/db_xref=\"", seqid, "\"", FALSE, TRUE, TILDE_TO_SPACES);
                */
              } else if (sip->choice == SEQID_GENERAL) {
                dbt = (DbtagPtr) sip->data.ptrvalue;
                if (dbt != NULL && StringCmp (dbt->db, "PID") == 0) {
                  /*
                  oip = dbt->tag;
                  if (oip != NULL) {
                    if (! StringHasNoText (oip->str)) {
                      sprintf (seqid, "PID:%s", oip->str);
                      NewContLine ();
                      gb_AddString ("/db_xref=\"", seqid, "\"", FALSE, TRUE, TILDE_TO_SPACES);
                    }
                  }
                  */
                }
              }
            }
          } else {
            if (sip->choice == SEQID_GI) {
              gi = sip->data.intvalue;
              sip = GetSeqIdForGI (gi);
              if (sip != NULL && SeqIdWrite (sip, seqid, PRINTID_TEXTID_ACC_VER, sizeof (seqid)) != NULL) {
                if ((! ajp->flags.dropIllegalQuals) || ValidateAccession (seqid)) {
                  sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
                  NewContLine ();
                  ff_AddString (buf);
                  www_protein_id (seqid);
                  ff_AddString ("\"");
                  /*
                  gb_AddString (buf, seqid, "\"", FALSE, TRUE, TILDE_TO_SPACES);
                  */
                }
              } else if (! ajp->flags.dropIllegalQuals) {
                sprintf (seqid, "%ld", (long) gi);
                sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
                NewContLine ();
                ff_AddString (buf);
                www_protein_id (seqid);
                ff_AddString ("\"");
                /*
                gb_AddString (buf, seqid, "\"", FALSE, TRUE, TILDE_TO_SPACES);
                */
              }

              sprintf (seqid, "GI:%ld", (long) gi);
              NewContLine ();
              ff_AddString ("/db_xref=\"");
              www_db_xref (seqid);
              ff_AddString ("\"");
              /*
              gb_AddString ("/db_xref=\"", seqid, "\"", FALSE, TRUE, TILDE_TO_SPACES);
              */
            } else if (SeqIdWrite (sip, seqid, PRINTID_TEXTID_ACC_VER, sizeof (seqid)) != NULL) {
              if ((! ajp->flags.dropIllegalQuals) || ValidateAccession (seqid)) {
                sprintf (buf, "/%s=\"", asn2gnbk_featur_quals [idx].name);
                NewContLine ();
                ff_AddString (buf);
                www_protein_id (seqid);
                ff_AddString ("\"");
                /*
                gb_AddString (buf, seqid, "\"", FALSE, TRUE, TILDE_TO_SPACES);
                */
              }

              gi = GetGIForSeqId (sip);
              if (gi > 0) {
                sprintf (seqid, "GI:%ld", (long) gi);
                NewContLine ();
                ff_AddString ("/db_xref=\"");
                www_db_xref (seqid);
                ff_AddString ("\"");
                /*
                gb_AddString ("/db_xref=\"", seqid, "\"", FALSE, TRUE, TILDE_TO_SPACES);
                */
              }
            }
          }
        }
        break;

      case Qual_class_translation :
        if (qvp [idx].bool) {
          if (prod != NULL) {
            len = SeqLocLen (sfp->product);
            if (len > 0) {
              if (SeqLocStart (location) == 0 || SeqLocStop (location) == bsp->length - 1) {
                at_end = TRUE;
              }
              str = (CharPtr) MemNew ((size_t) (len + 1) * sizeof (Char));
              protein_seq = str;
              if (ajp->flags.iupacaaOnly) {
                code = Seq_code_iupacaa;
              } else {
                code = Seq_code_ncbieaa;
              }
              spp = SeqPortNewByLoc (sfp->product, code);
              if (spp != NULL) {
                SeqPortSet_do_virtual (spp, TRUE);
                while ((residue = SeqPortGetResidue (spp)) != SEQPORT_EOF) {
                  if (! (IS_residue (residue))) continue;
                  if (residue == INVALID_RESIDUE) {
                    residue = (Uint1) 'X';
                  }
                  *protein_seq = residue;
                  protein_seq++;
                }
                if (at_end && StringLen (str) < 0) {
                  str = MemFree (str);
                }
                if (! StringHasNoText (str)) {
                  NewContLine ();
                  gb_AddString ("/translation=\"", str, "\"", FALSE, TRUE, TILDE_TO_SPACES);
                }
                MemFree (str);
              }
              SeqPortFree (spp);
            }
          /*
          } else {
            bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
            if (bs != NULL) {
              str = BSMerge (bs, NULL);
              bs = BSFree (bs);
              if (str != NULL) {
                ptr = str;
                ch = *ptr;
                while (ch != '\0') {
                  *ptr = TO_UPPER (ch);
                  ptr++;
                  ch = *ptr;
                }
                if (! StringHasNoText (str)) {
                  NewContLine ();
                  gb_AddString ("/translation=\"", str, "\"", FALSE, TRUE, TILDE_TO_SPACES);
                }
                MemFree (str);
              }
            }
            */
          }
        }
        break;

      case Qual_class_illegal :
        for (vnp = qvp [idx].vnp; vnp != NULL; vnp = vnp->next) {
          str = (CharPtr) vnp->data.ptrvalue;
          if (str != NULL) {
            NewContLine ();
            gb_AddString (NULL, str, NULL, FALSE, FALSE, TILDE_TO_SPACES);
          }
        }
        break;

      case Qual_class_note :
        head = NULL;
        notestr = NULL;
        prefix = NULL;
        add_period = FALSE;
        suppress_period = FALSE;
        lasttype = NULL;
        isTRNA = TRUE;

#ifdef DISPLAY_STRINGS
        s_DisplayQVP (qvp, feat_note_order);
#endif
        for (j = 0, jdx = feat_note_order [j]; jdx != 0; j++, jdx = feat_note_order [j]) {
          switch (asn2gnbk_featur_quals [jdx].qualclass) {

            case Qual_class_string :
              if (! StringHasNoText (qvp [jdx].str)) {
                if (jdx == FTQUAL_figure) {
                  if (!IsEllipsis (qvp [jdx].str))
                    s_RemovePeriodFromEnd (qvp [jdx].str);
                  sprintf (buf, "This sequence comes from %s", qvp [jdx].str);
                  s_AddValNodeString_NoRedund (&head, prefix, buf, NULL);
                  add_period = FALSE;
                } else if (jdx == FTQUAL_maploc) {
                  if (!IsEllipsis (qvp [jdx].str))
                    s_RemovePeriodFromEnd (qvp [jdx].str);
                  sprintf (buf, "Map location %s", qvp [jdx].str);
                  s_AddValNodeString_NoRedund (&head, prefix, buf, NULL);
                  add_period = FALSE;
                } else if (jdx == FTQUAL_seqfeat_note) {
                  str = StringSave (qvp [jdx].str);
                  TrimSpacesAndJunkFromEnds (str, TRUE);
                  if (! IsEllipsis (str))
                    add_period = s_RemovePeriodFromEnd (str);
                  /*  NOTE -- The following function call cleans up some strings
                      (i.e., U34661 & U31565) but should be commented back
                      in only if the problem can't be fixed upstream of here

                  s_StringCleanup(str);

                  */
                  s_AddValNodeString_NoRedund (&head, prefix, str, NULL);
                  MemFree (str);
                  if (hadProtDesc) {
                    suppress_period = TRUE;
                  }
                } else if (jdx == FTQUAL_prot_note) {
                  str = StringSave (qvp [jdx].str);
                  TrimSpacesAndJunkFromEnds (str, TRUE);
                  if (! IsEllipsis (str))
                    s_RemovePeriodFromEnd (str);
                  s_AddValNodeString_NoRedund (&head, prefix, str, NULL);
                  MemFree (str);
                  add_period = FALSE;
                } else if (jdx == FTQUAL_prot_desc) {
                  str = StringSave (qvp [jdx].str);
                  TrimSpacesAndJunkFromEnds (str, TRUE);
                  if (! IsEllipsis (str))
                    add_period = s_RemovePeriodFromEnd (str);
                  s_AddValNodeString_NoRedund (&head, prefix, str, NULL);
                  MemFree (str);
                  hadProtDesc = TRUE; /* gi|347886|gb|M96268.1|ECOUBIA */
                } else {
                  if (! IsEllipsis (qvp [jdx].str))
                    s_RemovePeriodFromEnd (qvp [jdx].str);
                  s_AddValNodeString_NoRedund (&head, prefix, qvp [jdx].str, NULL);
                  add_period = FALSE;
                }
                prefix = "; ";
              }
              break;

            case Qual_class_method :
              if (! StringHasNoText (qvp [jdx].str)) {
                if (head == NULL) {
                  prefix = "Method: ";
                } else {
                  prefix = "; Method: ";
                }
                s_AddValNodeString_NoRedund (&head, prefix, qvp [jdx].str, NULL);
                prefix = "; ";
                add_period = FALSE;
              }
              break;

            case Qual_class_valnode :
              for (vnp = qvp [jdx].vnp; vnp != NULL; vnp = vnp->next) {
                str = (CharPtr) vnp->data.ptrvalue;
                if (! StringHasNoText (str)) {
                  s_AddValNodeString_NoRedund (&head, prefix, str, NULL);
                  prefix = "; ";
                  add_period = FALSE;
                }
              }
              break;

            case Qual_class_region :
              if (! StringHasNoText (qvp [jdx].str)) {
                if (head == NULL) {
                  prefix = "Region: ";
                } else {
                  prefix = "; Region: ";
                }
#ifdef ASN2GNBK_STRIP_NOTE_PERIODS
                AddValNodeString (&head, prefix, qvp [jdx].str, NULL);
#else
                s_AddValNodeString_NoRedund (&head, prefix, qvp [jdx].str, NULL);
#endif
                prefix = "; ";
                add_period = FALSE;
              }
              break;

            case Qual_class_site :
              if (! StringHasNoText (qvp [jdx].str)) {
                s_AddValNodeString_NoRedund (&head, prefix, qvp [jdx].str, " site");
                add_period = FALSE;
                prefix = "\n";
              }
              break;

            case Qual_class_protnames :
              /* process gene sgml for check against subsequent protein names */
              start = NULL;
              if (! StringHasNoText (qvp [FTQUAL_gene].str)) {
                ascii_len = Sgml2AsciiLen (qvp [FTQUAL_gene].str);
                start = ascii = MemNew ((size_t) (10 + ascii_len));
                if (start != NULL) {
                  ascii = Sgml2Ascii (qvp [FTQUAL_gene].str, ascii, ascii_len + 1);
                }
              }
              for (vnp = qvp [jdx].vnp; vnp != NULL; vnp = vnp->next) {
                str = (CharPtr) vnp->data.ptrvalue;
                if (! StringHasNoText (str)) {
                  /* case sensitive - gi|4973426|gb|AF148501.1|AF148501 */
                  /* check with and without sgml conversion */
                  if (StringCmp (start, str) != 0 &&
                      StringCmp (qvp [FTQUAL_gene].str, str) != 0) {
                    if (! StringStr (qvp [FTQUAL_prot_desc].str, str)) {
                      if (NotInGeneSyn (str, gene_syn)) {
                        s_AddValNodeString_NoRedund (&head, prefix, str, NULL);
                        prefix = "; ";
                        add_period = FALSE;
                      }
                    }
                  }
                }
              }
              MemFree (start);
              break;

            case Qual_class_xtraprds :
              gbq = qvp [jdx].gbq;
              if (lasttype == NULL && gbq != NULL) {
                lasttype = gbq->qual;
              }
              while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
                if (! StringHasNoText (gbq->val)) {
                  if (StringCmp(gbq->val,qvp[FTQUAL_gene].str) != 0 &&
                      StringCmp(gbq->val,qvp[FTQUAL_product].str) != 0) {
                    if (!isTRNA || !StringStr (gbq->val, "RNA")) {
                      s_AddValNodeString_NoRedund (&head, prefix,
                           gbq->val, NULL);
                      prefix = "; ";
                      add_period = FALSE;
                    }
                  }
                }
                gbq = gbq->next;
              }
              break;

            case Qual_class_its :
              str = qvp [jdx].str;
              if (! StringHasNoText (str)) {
                if (sfp->comment == NULL || StringStr (sfp->comment, str) == NULL) {
                  s_AddValNodeString_NoRedund (&head, prefix, str, NULL);
                  prefix = "; ";
                  add_period = FALSE;
                }
              }
              break;

            case Qual_class_trna_codons :
              trna = qvp [jdx].trp;
              if (trna) {
                numcodons = ComposeCodonsRecognizedString (trna, numbuf, sizeof (numbuf));
                if (numcodons < 1 || StringHasNoText (numbuf)) {
                } else if (numcodons == 1) {
                  isTRNA = TRUE;
                  sprintf (buf, "codon recognized: %s", numbuf);
                  if (StringStr (qvp [FTQUAL_seqfeat_note].str, buf) == NULL) {
                    s_AddValNodeString_NoRedund (&head, prefix, "codon recognized: ", numbuf);
                    prefix = "; ";
                  }
                } else {
                  isTRNA = TRUE;
                  s_AddValNodeString_NoRedund (&head, prefix, "codons recognized: ", numbuf);
                  prefix = "; ";
                  add_period = FALSE;
                }
              }
              break;

            case Qual_class_model_ev :
              uop = qvp [jdx].uop;
              if (uop != NULL) {
                str = GetStrFormRNAEvidence (uop);
                if (! StringHasNoText (str)) {
                  s_AddValNodeString_NoRedund (&head, prefix, str, NULL);
                  prefix = "; ";
                  add_period = FALSE;
                }
              }
              break;

            default :
              break;
          }
        }
        if (head != NULL) {
          notestr = MergeValNodeStrings (head);
          NewContLine ();
          TrimSpacesAroundString (notestr);
          if (add_period) {
            if (! suppress_period) {
              s_AddPeriodToEnd (notestr);
            }
          }

#ifdef ASN2GNBK_STRIP_NOTE_PERIODS
          if (! IsEllipsis (notestr))
            s_RemovePeriodFromEnd (notestr);
#endif


          gb_AddString (NULL, "/note=\"", NULL, FALSE, FALSE, TILDE_IGNORE);
          gb_AddString (NULL, notestr, NULL, FALSE, TRUE, TILDE_EXPAND);
          gb_AddString (NULL, "\"", NULL, FALSE, FALSE, TILDE_IGNORE);

          MemFree (notestr);
          ValNodeFreeData (head);
        }
        break;
      default :
        break;
    }
  }

  ValNodeFreeData (dbxrefs);
}

static void RecordUserObjectsInQVP (
  UserObjectPtr uop,
  Pointer userdata
)

{
  ObjectIdPtr  oip;
  QualValPtr   qvp;

  if (uop == NULL || userdata == NULL) return;
  qvp = (QualValPtr) userdata;
  oip = uop->type;
  if (oip == NULL) return;
  if (StringCmp (oip->str, "ModelEvidence") == 0) {
    qvp [FTQUAL_modelev].uop = uop;
  }
}

static CharPtr FormatFeatureBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
)

{
  Uint1              aa;
  IntAsn2gbJobPtr    ajp;
  Asn2gbSectPtr      asp;
  BioseqPtr          bsp;
  BioseqSetPtr       bssp;
  Char               buf [80];
  Choice             cbaa;
  CodeBreakPtr       cbp;
  BioseqPtr          cdna;
  SeqFeatPtr         cds;
  Char               ch;
  Uint1              code = Seq_code_ncbieaa;
  CdRegionPtr        crp;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  Uint1              featdeftype;
  Uint1              from;
  GBQualPtr          gbq;
  SeqMgrFeatContext  gcontext;
  ValNodePtr         gcp;
  SeqFeatPtr         gene = NULL;
  ValNodePtr         gene_syn = NULL;
  Int4               gi = 0;
  GeneRefPtr         grp;
  IntCdsBlockPtr     icp;
  Uint1              idx;
  IntFeatBlockPtr    ifp;
  ValNodePtr         illegal = NULL;
  ImpFeatPtr         imp = NULL;
  Boolean            is_gps = FALSE;
  Boolean            is_other = FALSE;
  CharPtr            key;
  CharPtr            lasttype;
  Int4               left = -1;
  SeqLocPtr          loc = NULL;
  SeqLocPtr          location = NULL;
  SeqLocPtr          locforgene = NULL;
  SeqMgrFeatContext  mcontext;
  MolInfoPtr         mip;
  SeqFeatPtr         mrna;
  SeqLocPtr          newloc;
  Boolean            noLeft;
  Boolean            noRight;
  SeqEntryPtr        oldscope;
  Uint2              partial;
  SeqMgrFeatContext  pcontext;
  BioseqPtr          prod = NULL;
  SeqFeatPtr         prot;
  Boolean            protein = FALSE;
  Char               protein_pid_g [32];
  ProtRefPtr         prp;
  Boolean            pseudo = FALSE;
  CharPtr            ptr;
  Int2               qualclass;
  QualValPtr         qvp;
  Uint1              residue;
  Int4               right = -1;
  RnaRefPtr          rrp;
  SeqCodeTablePtr    sctp;
  SeqDescrPtr        sdp;
  SeqEntryPtr        sep;
  Uint1              seqcode;
  SeqFeatPtr         sfp;
  Uint1              shift;
  SeqIdPtr           sip;
  Int2               siteidx;
  SeqMapTablePtr     smtp;
  Boolean            split;
  CharPtr            str;
  Uint1              strand = Seq_strand_unknown;
  BioseqPtr          target;
  CharPtr            tmp;
  tRNAPtr            trna;
  BioseqPtr          unlockme = NULL;
  ValNodePtr         vnp;
  CharPtr            wwwbuf;

  if (afp == NULL || bbp == NULL) return NULL;
  asp = afp->asp;
  if (asp == NULL) return NULL;
  target = asp->target;
  bsp = asp->bsp;
  if (target == NULL || bsp == NULL) return NULL;
  ajp = afp->ajp;
  if (ajp == NULL) return NULL;
  qvp = afp->qvp;
  if (qvp == NULL) return NULL;

  protein_pid_g [0] = '\0';

  /* all features in this list are known to be valid for the designated mode */

  sfp = SeqMgrGetDesiredFeature (bbp->entityID, NULL, bbp->itemID, 0, NULL, &fcontext);
  if (sfp == NULL) return NULL;

  /* may need to map location between aa and dna */

  ifp = (IntFeatBlockPtr) bbp;
  if (ifp->mapToNuc) {

    /* map mat_peptide, etc., to nucleotide coordinates */

    sip = SeqLocId (sfp->location);
    prod = BioseqFind (sip);
    cds = SeqMgrGetCDSgivenProduct (prod, NULL);
    CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
    location = aaFeatLoc_to_dnaFeatLoc (cds, sfp->location);
    SetSeqLocPartial (location, noLeft, noRight);
    locforgene = location;
    loc = location;

  } else if (ifp->mapToProt) {

    /* map CDS to protein product coordinates */

    sip = SeqLocId (sfp->product);
    prod = BioseqFind (sip);
    cds = SeqMgrGetCDSgivenProduct (prod, NULL);
    location = dnaLoc_to_aaLoc (cds, sfp->location, TRUE, NULL, FALSE);
    SetSeqLocPartial (location, FALSE, FALSE);
    locforgene = sfp->location;
    loc = location;

  } else if (ifp->mapToGen) {

    /* map CDS from cDNA to genomic Bioseq */

    cdna = BioseqFindFromSeqLoc (sfp->location);
    mrna = SeqMgrGetRNAgivenProduct (cdna, &mcontext);
    CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
    location = productLoc_to_locationLoc (mrna, sfp->location);
    SetSeqLocPartial (location, noLeft, noRight);
    locforgene = location;
    loc = location;

  } else if (ifp->mapToMrna) {

    /* map gene from genomic to cDNA Bioseq */

    sep = SeqMgrGetSeqEntryForData (bsp);
    location = CreateWholeInterval (sep);
    SetSeqLocPartial (location, FALSE, FALSE);
    locforgene = location;
    loc = location;

  } else {

    /* no aa-dna or dna-aa mapping, just use location */

    location = sfp->location;
    locforgene = sfp->location;
  }
  if (location == NULL) return NULL;

  sep = GetTopSeqEntryForEntityID (ajp->ajp.entityID);
  if (sep == NULL && IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && bssp->_class == BioseqseqSet_class_gen_prod_set) {
      is_gps = TRUE;
    }
  }

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_OTHER) {
      is_other = TRUE;
    } else if (sip->choice == SEQID_GI) {
      gi = (Int4) sip->data.intvalue;
    }
  }

  featdeftype = fcontext.featdeftype;
  if (featdeftype < FEATDEF_GENE || featdeftype >= FEATDEF_MAX) {
    featdeftype = FEATDEF_BAD;
  }
  key = FindKeyFromFeatDefType (featdeftype, TRUE);

  if (afp->format == GENPEPT_FMT && ISA_aa (bsp->mol)) {
    if (featdeftype == FEATDEF_REGION) {
      key = "Region";
    } else if (featdeftype == FEATDEF_SITE) {
      key = "Site";
    }
  }

  /* deal with unmappable impfeats */

  if (featdeftype == FEATDEF_BAD && fcontext.seqfeattype == SEQFEAT_IMP) {
    imp = (ImpFeatPtr) sfp->data.value.ptrvalue;
    if (imp != NULL) {
      key = imp->key;
    }
  }

  gb_StartPrint (afp->format, 5, 21, NULL, 0, 5, 21, "FT", ifp->firstfeat);
  if (ajp->ajp.slp != NULL) {
    ff_AddString (key);
  } else {
    www_featkey (key, gi, fcontext.entityID, fcontext.itemID);
  }
  TabToColumn (22);

  if (imp == NULL || StringHasNoText (imp->loc)) {
    if (ajp->ajp.slp != NULL) {
      sip = SeqIdParse ("lcl|dummy");
      left = GetOffsetInBioseq (ajp->ajp.slp, bsp, SEQLOC_LEFT_END);
      right = GetOffsetInBioseq (ajp->ajp.slp, bsp, SEQLOC_RIGHT_END);
      strand = SeqLocStrand (ajp->ajp.slp);
      split = FALSE;
      newloc = SeqLocCopyRegion (sip, location, bsp, left, right, strand, &split);
      SeqIdFree (sip);
      if (newloc == NULL) return NULL;
      SeqLocReplaceID (newloc, SeqLocId (ajp->ajp.slp));
      str = FlatLoc (target, newloc, ajp->masterStyle);
      SeqLocFree (newloc);
    } else {
      str = FlatLoc (target, location, ajp->masterStyle);
    }
  } else {
    str = StringSave (imp->loc);
  }
  if (get_www ()) {
    wwwbuf = www_featloc (str);
    ff_AddString (wwwbuf);
    MemFree (wwwbuf);
  } else {
    ff_AddString (str);
  }
  MemFree (str);

  /* populate qualifier table from feature fields */

  /*
  if (sfp->partial == TRUE)
    sfp->partial = FlatAnnotPartial(sfp, use_product);
  */

  if (sfp->partial) {
    partial = SeqLocPartialCheck (location);
    if (partial == SLP_COMPLETE /* || partial > SLP_OTHER */ ) {
      qvp [FTQUAL_partial].bool = TRUE;
    }
    if (LookForFuzz (location)) {
      qvp [FTQUAL_partial].bool = FALSE;
    }
    if (imp != NULL) {
      if (StringChr (imp->loc, '<') != NULL || StringChr (imp->loc, '>') != NULL) {
        qvp [FTQUAL_partial].bool = FALSE;
      }
    }

    /* a few features cannot show /partial in RELEASE_MODE - later no features will */

    if (ajp->flags.checkQualSyntax) {
      switch (featdeftype) {
      case FEATDEF_conflict:
      case FEATDEF_mutation:
      case FEATDEF_N_region:
      case FEATDEF_polyA_site:
        qvp [FTQUAL_partial].bool = FALSE;
        break;
      default:
        break;
      }
    }
  }
  if (ifp->mapToProt) {
    qvp [FTQUAL_partial].bool = FALSE;
  }

  if (sfp->pseudo) {
    pseudo = TRUE;
  }

  if (fcontext.seqfeattype == SEQFEAT_GENE) {
    grp = (GeneRefPtr) sfp->data.value.ptrvalue;
    if (grp != NULL) {
      if (! StringHasNoText (grp->locus)) {
        qvp [FTQUAL_gene].str = grp->locus;
        qvp [FTQUAL_gene_desc].str = grp->desc;
        qvp [FTQUAL_gene_syn].vnp = grp->syn;
      } else if (! StringHasNoText (grp->desc)) {
        qvp [FTQUAL_gene].str = grp->desc;
        qvp [FTQUAL_gene_syn].vnp = grp->syn;
      } else if (grp->syn != NULL) {
        vnp = grp->syn;
        qvp [FTQUAL_gene].str = (CharPtr) vnp->data.ptrvalue;
        vnp = vnp->next;
        qvp [FTQUAL_gene_syn].vnp = vnp;
      }
      qvp [FTQUAL_gene_map].str = grp->maploc;
      qvp [FTQUAL_gene_allele].str = grp->allele;
      qvp [FTQUAL_gene_xref].vnp = grp->db;
      if (grp->pseudo) {
        pseudo = TRUE;
      }
    }

  } else {

    grp = SeqMgrGetGeneXref (sfp);
    if (grp != NULL) {
      qvp [FTQUAL_gene_xref].vnp = grp->db;
    }
    if (grp == NULL) {
      sep = GetTopSeqEntryForEntityID (ajp->ajp.entityID);
      oldscope = SeqEntrySetScope (sep);
      gene = SeqMgrGetOverlappingGene (locforgene, &gcontext);
      SeqEntrySetScope (oldscope);
      if (gene != NULL) {
        qvp [FTQUAL_gene_note].str = gene->comment;
        grp = (GeneRefPtr) gene->data.value.ptrvalue;
        if (gene->pseudo) {
          pseudo = TRUE;
        }
        qvp [FTQUAL_gene_xref].vnp = grp->db;
      }
    }
    if (grp != NULL && grp->pseudo) {
      pseudo = TRUE;
    }
    if (grp != NULL && (! SeqMgrGeneIsSuppressed (grp)) &&
        (fcontext.featdeftype != FEATDEF_repeat_region || gene == NULL)) {
      if (! StringHasNoText (grp->locus)) {
        qvp [FTQUAL_gene].str = grp->locus;
        gene_syn = grp->syn;
      } else if (! StringHasNoText (grp->desc)) {
        qvp [FTQUAL_gene].str = grp->desc;
        gene_syn = grp->syn;
      } else if (grp->syn != NULL) {
        vnp = grp->syn;
        qvp [FTQUAL_gene].str = (CharPtr) vnp->data.ptrvalue;
        vnp = vnp->next;
        gene_syn = vnp;
      }
    }
    if (fcontext.seqfeattype != SEQFEAT_CDREGION &&
        fcontext.seqfeattype != SEQFEAT_RNA) {
      qvp [FTQUAL_gene_xref].vnp = NULL;
    }

    /* specific fields set here */

    switch (fcontext.seqfeattype) {
      case SEQFEAT_CDREGION :
        if (! ifp->mapToProt) {
          crp = (CdRegionPtr) sfp->data.value.ptrvalue;
          if (crp != NULL) {

            qvp [FTQUAL_codon_start].num = crp->frame;
            if (qvp [FTQUAL_codon_start].num == 0) {
              qvp [FTQUAL_codon_start].num = 1;
            }
            qvp [FTQUAL_transl_except].cbp = crp->code_break;
            for (cbp = crp->code_break; cbp != NULL; cbp = cbp->next) {
              seqcode = 0;
              sctp = NULL;
              cbaa = cbp->aa;
              switch (cbaa.choice) {
                case 1 :
                  seqcode = Seq_code_ncbieaa;
                  break;
                case 2 :
                  seqcode = Seq_code_ncbi8aa;
                  break;
                case 3 :
                  seqcode = Seq_code_ncbistdaa;
                  break;
                default :
                  break;
              }
              if (seqcode != 0) {
                sctp = SeqCodeTableFind (seqcode);
                if (sctp != NULL) {
                  residue = cbaa.value.intvalue;
                  if (residue != 42) {
                    if (seqcode != Seq_code_ncbieaa) {
                      smtp = SeqMapTableFind (seqcode, Seq_code_ncbieaa);
                      residue = SeqMapTableConvert (smtp, residue);
                    }
                    if (residue == 'U') {
                      qvp [FTQUAL_selenocysteine].str = "selenocysteine";
                    }
                  }
                }
              }
            }

            gcp = crp->genetic_code;
            if (gcp != NULL) {
              for (vnp = gcp->data.ptrvalue; vnp != NULL; vnp = vnp->next) {
                if (vnp->choice == 2 && vnp->data.intvalue != 0) {
                  qvp [FTQUAL_transl_table].num = vnp->data.intvalue;
                }
              }

              /* suppress table 1 */

              if (qvp [FTQUAL_transl_table].num == 1) {
                qvp [FTQUAL_transl_table].num = 0;
              }
            }

            if (sfp->product != NULL && SeqLocLen (sfp->product) != 0) {
              protein = TRUE;
            }
            if (crp->conflict && (protein || (! sfp->excpt))) {
              if (protein) {
                qvp [FTQUAL_prot_conflict].str = conflict_msg;
              } else {
                /*
                qvp [FTQUAL_prot_missing].str = no_protein_msg;
                */
              }
            }
          }

          prp = SeqMgrGetProtXref (sfp);
          if (prp == NULL) {
            sip = SeqLocId (sfp->product);
            qvp [FTQUAL_protein_id].sip = sip;

            sep = GetTopSeqEntryForEntityID (ajp->ajp.entityID);

            /* by default only show /translation if product bioseq is within entity */

            oldscope = SeqEntrySetScope (sep);
            prod = BioseqFind (sip);
            SeqEntrySetScope (oldscope);

            if (prod == NULL && ajp->showFarTransl) {

              /* but flag can override and force far /translation */

              prod = BioseqLockById (sip);
              unlockme = prod;
            }

            if (prod != NULL) {
              for (sip = prod->id; sip != NULL; sip = sip->next) {
                if (sip->choice == SEQID_GI) {
                  sprintf (protein_pid_g, "PID:g%ld", (long) sip->data.intvalue);
                }
              }
              sdp = SeqMgrGetNextDescriptor (prod, NULL, Seq_descr_comment, &dcontext);
              if (sdp != NULL && dcontext.level == 0) {
                if (! StringHasNoText ((CharPtr) sdp->data.ptrvalue)) {
                  qvp [FTQUAL_prot_comment].str = (CharPtr) sdp->data.ptrvalue;
                }
              }
              sdp = SeqMgrGetNextDescriptor (prod, NULL, Seq_descr_molinfo, &dcontext);
              if (sdp != NULL && dcontext.level == 0) {
                mip = (MolInfoPtr) sdp->data.ptrvalue;
                if (mip != NULL && mip->tech > 1 &&
                    mip->tech != MI_TECH_concept_trans &&
                    mip->tech != MI_TECH_concept_trans_a) {
                  str = StringForSeqTech (mip->tech);
                  if (! StringHasNoText (str)) {
                    qvp [FTQUAL_prot_method].str = str;
                  }
                }
              }
              prot = SeqMgrGetBestProteinFeature (prod, &pcontext);
              if (prot != NULL) {
                prp = (ProtRefPtr) prot->data.value.ptrvalue;
                if (prp != NULL && prp->processed < 2) {
                  qvp [FTQUAL_prot_note].str = prot->comment;
                }
              }
            }
          }

          if (prp != NULL) {
            vnp = prp->name;
            if (vnp != NULL && (! StringHasNoText ((CharPtr) vnp->data.ptrvalue))) {
              qvp [FTQUAL_cds_product].str = (CharPtr) vnp->data.ptrvalue;
              vnp = vnp->next;
              qvp [FTQUAL_prot_names].vnp = vnp;
            }
            qvp [FTQUAL_prot_desc].str = prp->desc;
            qvp [FTQUAL_prot_activity].vnp = prp->activity;
            qvp [FTQUAL_prot_EC_number].vnp = prp->ec;
          }

          if (! pseudo) {
            if (prod != NULL) {
              qvp [FTQUAL_translation].bool = TRUE;
            }
          }

          if (ifp->isCDS) {
            icp = (IntCdsBlockPtr) ifp;
            qvp [FTQUAL_figure].str = icp->fig;
            qvp [FTQUAL_maploc].str = icp->maploc;
          }
        } else {
          qvp [FTQUAL_coded_by].slp = sfp->location;

          crp = (CdRegionPtr) sfp->data.value.ptrvalue;
          if (crp != NULL) {
            gcp = crp->genetic_code;
            if (gcp != NULL) {
              for (vnp = gcp->data.ptrvalue; vnp != NULL; vnp = vnp->next) {
                if (vnp->choice == 2 && vnp->data.intvalue != 0) {
                  qvp [FTQUAL_transl_table].num = vnp->data.intvalue;
                }
              }

              /* suppress table 1 */

              if (qvp [FTQUAL_transl_table].num == 1) {
                qvp [FTQUAL_transl_table].num = 0;
              }
            }
          }
        }
        break;
      case SEQFEAT_PROT :
        prp = (ProtRefPtr) sfp->data.value.ptrvalue;
        if (prp != NULL) {
          vnp = prp->name;
          if (vnp != NULL && (! StringHasNoText ((CharPtr) vnp->data.ptrvalue))) {
            qvp [FTQUAL_product].str = (CharPtr) vnp->data.ptrvalue;
            vnp = vnp->next;
            qvp [FTQUAL_prot_names].vnp = vnp;
          }
          if (afp->format != GENPEPT_FMT) {
            qvp [FTQUAL_prot_desc].str = prp->desc;
          } else {
            qvp [FTQUAL_prot_name].str = prp->desc;
          }
          if (afp->format != GENPEPT_FMT || prp->processed != 2) {
            qvp [FTQUAL_prot_activity].vnp = prp->activity;
          }
          qvp [FTQUAL_prot_EC_number].vnp = prp->ec;
        }
        break;
      case SEQFEAT_RNA :
        rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
        if (rrp != NULL) {
          if (rrp->pseudo) {
            pseudo = TRUE;
          }
          if (rrp->type == 2) {
            sip = SeqLocId (sfp->product);
            if (sip != NULL) {
              /* currently for RefSeq records only, not in GenBank release */
              if (is_other) {
                qvp [FTQUAL_transcript_id].sip = sip;
              }
              prod = BioseqFind (sip);
            }
          }
          if (rrp->type == 3) {
            if (rrp->ext.choice == 1) {
              /* amino acid could not be parsed into structured form */
              if (! ajp->flags.dropIllegalQuals) {
                str = (CharPtr) rrp->ext.value.ptrvalue;
                qvp [FTQUAL_product].str = str;
              } else {
                qvp [FTQUAL_product].str = "tRNA-OTHER";
              }
            } else if (rrp->ext.choice == 2) {
              trna = (tRNAPtr) rrp->ext.value.ptrvalue;
              if (trna != NULL) {
                aa = 0;
                if (trna->aatype == 2) {
                  aa = trna->aa;
                } else {
                  from = 0;
                  switch (trna->aatype) {
                    case 0 :
                      from = 0;
                      break;
                    case 1 :
                      from = Seq_code_iupacaa;
                      break;
                    case 2 :
                      from = Seq_code_ncbieaa;
                      break;
                    case 3 :
                      from = Seq_code_ncbi8aa;
                      break;
                    case 4 :
                      from = Seq_code_ncbistdaa;
                      break;
                    default:
                      break;
                  }
                  if (ajp->flags.iupacaaOnly) {
                    code = Seq_code_iupacaa;
                  } else {
                    code = Seq_code_ncbieaa;
                  }
                  smtp = SeqMapTableFind (code, from);
                  if (smtp != NULL) {
                    aa = SeqMapTableConvert (smtp, trna->aa);
                    if (aa == 255 && from == Seq_code_iupacaa && trna->aa == 'U') {
                      aa = 'U';
                    }
                  }
                }
                if (ajp->flags.iupacaaOnly) {
                  smtp = SeqMapTableFind (Seq_code_iupacaa, Seq_code_ncbieaa);
                  if (smtp != NULL) {
                    aa = SeqMapTableConvert (smtp, trna->aa);
                  } else {
                    aa = 'X';
                  }
                }
                if (aa > 0 && aa != 255) {
                  if (aa <= 74) {
                    shift = 0;
                  } else if (aa > 79) {
                    shift = 2;
                  } else {
                    shift = 1;
                  }
                  idx = aa - (64 + shift);
                  if (idx > 0 && idx < 25) {
                    str = trnaList [idx];
                    qvp [FTQUAL_product].str = str;
                    if (StringNICmp (str, "tRNA-", 5) == 0) {
                      qvp [FTQUAL_trna_aa].str = str + 5;
                    }
                  }
                }
                qvp [FTQUAL_anticodon].slp = trna->anticodon;
                qvp [FTQUAL_trna_codons].trp = trna;
              }
            }
          } else {
            if (rrp->ext.choice == 1) {
              str = (CharPtr) rrp->ext.value.ptrvalue;
              qvp [FTQUAL_product].str = str;

              /*
              if (rrp->type == 255 && (! StringHasNoText (str))) {
                if        (StringICmp (str, "internal transcribed spacer 1") == 0 ||
                           StringICmp (str, "internal transcribed spacer ITS1") == 0 ||
                           StringICmp (str, "ITS1") == 0) {
                  qvp [FTQUAL_rrna_its].str = "ITS1";
                } else if (StringICmp (str, "internal transcribed spacer 2") == 0 ||
                           StringICmp (str, "internal transcribed spacer ITS2") == 0 ||
                           StringICmp (str, "ITS2") == 0) {
                  qvp [FTQUAL_rrna_its].str = "ITS2";
                } else if (StringICmp (str, "internal transcribed spacer 3") == 0 ||
                           StringICmp (str, "internal transcribed spacer ITS3") == 0 ||
                           StringICmp (str, "ITS3") == 0) {
                  qvp [FTQUAL_rrna_its].str = "ITS3";
                }
              }
              */
            }
          }
        }
        break;
      case SEQFEAT_REGION :
        if (afp->format == GENPEPT_FMT && featdeftype == FEATDEF_REGION && ISA_aa (bsp->mol)) {
          qvp [FTQUAL_region_name].str = (CharPtr) sfp->data.value.ptrvalue;
        } else {
          qvp [FTQUAL_region].str = (CharPtr) sfp->data.value.ptrvalue;
        }
        break;
      case SEQFEAT_COMMENT :
        break;
      case SEQFEAT_SITE :
        siteidx = (Int2) sfp->data.value.intvalue;
        if (siteidx == 255) {
          siteidx = 26;
        }
        if (siteidx > 0 && siteidx < 27) {
          if (afp->format == GENPEPT_FMT && ISA_aa (bsp->mol)) {
            qvp [FTQUAL_site_type].str = siteList [siteidx];
          } else {
            qvp [FTQUAL_site].str = siteList [siteidx];
          }
        }
        break;
      case SEQFEAT_PSEC_STR :
        qvp [FTQUAL_sec_str_type].num = sfp->data.value.intvalue;
        break;
      case SEQFEAT_HET :
        qvp [FTQUAL_heterogen].str = (CharPtr) sfp->data.value.ptrvalue;
        break;
      default :
        break;
    }
  }

  /* common fields set here */

  VisitUserObjectsInUop (sfp->ext, (Pointer) qvp, RecordUserObjectsInQVP);

  if (fcontext.featdeftype == FEATDEF_repeat_region) {
    pseudo = FALSE;
  }

  qvp [FTQUAL_seqfeat_note].str = sfp->comment;

  qvp [FTQUAL_pseudo].bool = pseudo;

  /* if RELEASE_MODE, check list of features that can have /pseudo */

  if (ajp->flags.dropIllegalQuals && pseudo  &&
      (fcontext.seqfeattype == SEQFEAT_RNA || fcontext.seqfeattype == SEQFEAT_IMP) ) {
    switch (featdeftype) {

    case  FEATDEF_allele:
    case  FEATDEF_attenuator:
    case  FEATDEF_CAAT_signal:
    case  FEATDEF_conflict:
    case  FEATDEF_D_loop:
    case  FEATDEF_enhancer:
    case  FEATDEF_GC_signal:
    case  FEATDEF_iDNA:
    case  FEATDEF_intron:
    case  FEATDEF_LTR:
    case  FEATDEF_misc_binding:
    case  FEATDEF_misc_difference:
    case  FEATDEF_misc_recomb:
    case  FEATDEF_misc_RNA:
    case  FEATDEF_misc_signal:
    case  FEATDEF_misc_structure:
    case  FEATDEF_modified_base:
    case  FEATDEF_mutation:
    case  FEATDEF_old_sequence:
    case  FEATDEF_polyA_signal:
    case  FEATDEF_polyA_site:
    case  FEATDEF_precursor_RNA:
    case  FEATDEF_prim_transcript:
    case  FEATDEF_primer_bind:
    case  FEATDEF_protein_bind:
    case  FEATDEF_RBS:
    case  FEATDEF_repeat_region:
    case  FEATDEF_repeat_unit:
    case  FEATDEF_rep_origin:
    case  FEATDEF_satellite:
    case  FEATDEF_stem_loop:
    case  FEATDEF_STS:
    case  FEATDEF_TATA_signal:
    case  FEATDEF_terminator:
    case  FEATDEF_unsure:
    case  FEATDEF_variation:
    case  FEATDEF_3clip:
    case  FEATDEF_3UTR:
    case  FEATDEF_5clip:
    case  FEATDEF_5UTR:
      qvp [FTQUAL_pseudo].bool = FALSE;
        break;
    default:
        break;
    }
  }

  if (afp->format != GENPEPT_FMT) {
    qvp [FTQUAL_evidence].num = sfp->exp_ev;
  }

  /* exception currently legal only on CDS */

  if ((! ajp->flags.dropIllegalQuals) || fcontext.seqfeattype == SEQFEAT_CDREGION) {
    qvp [FTQUAL_exception].str = sfp->except_text;

    if (sfp->excpt && qvp [FTQUAL_exception].str == NULL) {
      if (qvp [FTQUAL_seqfeat_note].str != NULL) {
        if (ajp->flags.dropIllegalQuals &&
            (! StringInStringList (qvp [FTQUAL_seqfeat_note].str, validExceptionString))) {
        /* !!! if ajp->flags.dropIllegalQuals, check CDS list to avoid losing note !!! */
          qvp [FTQUAL_exception].str = NULL;
        } else {
          /* if no /exception text, use text in comment, remove from /note */

          qvp [FTQUAL_exception].str = qvp [FTQUAL_seqfeat_note].str;
          qvp [FTQUAL_seqfeat_note].str = NULL;
        }
      } else {
        qvp [FTQUAL_exception].str = "No explanation supplied";
      }

      /* !!! if ajp->flags.dropIllegalQuals, check CDS list here as well !!! */
      if (ajp->flags.dropIllegalQuals &&
          (! StringInStringList (qvp [FTQUAL_seqfeat_note].str, validExceptionString)) ) {
        qvp [FTQUAL_exception].str = NULL;
      }
    }
    if (ajp->flags.dropIllegalQuals &&
        (! StringInStringList (qvp [FTQUAL_exception].str, validExceptionString))) {
      qvp [FTQUAL_exception_note].str = qvp [FTQUAL_exception].str;
      qvp [FTQUAL_exception].str = NULL;
    }
  } else {
    qvp [FTQUAL_exception_note].str = sfp->except_text;
  }
  if (StringHasNoText (qvp [FTQUAL_exception].str)) {
    qvp [FTQUAL_exception].str = NULL;
  }
  if (StringHasNoText (qvp [FTQUAL_exception_note].str)) {
    qvp [FTQUAL_exception_note].str = NULL;
  }

  qvp [FTQUAL_db_xref].vnp = sfp->dbxref;
  qvp [FTQUAL_citation].vnp = sfp->cit;

  /* /product same as sfp->comment will suppress /note */

  if (! StringHasNoText (qvp [FTQUAL_product].str) &&
      StringICmp (sfp->comment, qvp [FTQUAL_product].str) == 0) {
    qvp [FTQUAL_seqfeat_note].str = NULL;
  }
  /* case sensitive AJ011317.1 */
  if (! StringHasNoText (qvp [FTQUAL_cds_product].str) &&
      StringCmp (sfp->comment, qvp [FTQUAL_cds_product].str) == 0) {
    qvp [FTQUAL_seqfeat_note].str = NULL;
  }

  /* /gene same as sfp->comment will suppress /note */
  /* case sensitive -gi|6572973|gb|AF195052.1|AF195052 */

  if (! StringHasNoText (qvp [FTQUAL_gene].str) &&
      StringCmp (sfp->comment, qvp [FTQUAL_gene].str) == 0) {
    qvp [FTQUAL_seqfeat_note].str = NULL;
  }

  /* gene /note same as sfp->comment will suppress /note - U92435.1 says do not do this */

  /*
  if (! StringHasNoText (qvp [FTQUAL_gene_note].str) &&
      StringICmp (sfp->comment, qvp [FTQUAL_gene_note].str) == 0) {
    qvp [FTQUAL_seqfeat_note].str = NULL;
  }
  */

  /* if site sfp->comment contains site name, suppress site in /note */

  if (! StringHasNoText (qvp [FTQUAL_site].str) &&
      StringStr (sfp->comment, qvp [FTQUAL_site].str) != NULL) {
    qvp [FTQUAL_site].str = NULL;
  }

  /* /EC_number same as sfp->comment will suppress /note */

  for (vnp = qvp [FTQUAL_prot_EC_number].vnp; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if ((! StringHasNoText (str)) &&
        StringICmp (sfp->comment, str) == 0) {
      qvp [FTQUAL_seqfeat_note].str = NULL;
    }
  }


  /* now go through gbqual list */

  for (gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
    idx = GbqualToFeaturIndex (gbq->qual);
    if (idx > 0 && idx < ASN2GNBK_TOTAL_FEATUR) {
      if (qvp [idx].gbq == NULL) {
        if (idx == FTQUAL_product_quals) {
          if (qvp [FTQUAL_product].str == NULL) {
            qvp [FTQUAL_product].str = gbq->val;
          } else if (qvp [FTQUAL_xtra_prod_quals].gbq == NULL) {
            /* chain will include remaining product gbquals */
            qvp [FTQUAL_xtra_prod_quals].gbq = gbq;
          }
        } else {
          qvp [idx].gbq = gbq;
        }
      }

    } else if (idx == 0) {

      qualclass = IllegalGbqualToClass (gbq->qual);
      if (qualclass == 0) {
        qualclass = Qual_class_quote;
      }
      tmp = StringSave (gbq->val);
      if (tmp != NULL) {
        str = MemNew (sizeof (Char) * (StringLen (gbq->val) + StringLen (tmp) + 10));
        if (str != NULL) {
          if (qualclass == Qual_class_quote) {
            if (StringIsJustQuotes (tmp)) {
              sprintf (buf, "/%s", gbq->qual);
            } else {
              ptr = tmp;
              ch = *ptr;
              while (ch != '\0') {
                if (ch == '"') {
                  *ptr = '\'';
                }
                ptr++;
                ch = *ptr;
              }
              sprintf (str, "/%s=\"%s\"", gbq->qual, tmp);
            }
            ValNodeCopyStr (&illegal, 0, str);
          } else if (qualclass == Qual_class_noquote || qualclass == Qual_class_label) {
            if (StringIsJustQuotes (tmp)) {
              sprintf (str, "/%s", gbq->qual);
            } else {
              sprintf (str, "/%s=%s", gbq->qual, tmp);
            }
            ValNodeCopyStr (&illegal, 0, str);
          }
          MemFree (str);
        }
        MemFree (tmp);
      }
    }
  }

  /* illegal qualifiers are copied and formatted in valnode chain */

  if (! ajp->flags.dropIllegalQuals) {
    qvp [FTQUAL_illegal_qual].vnp = illegal;
  }

  /* remove protein description that equals the gene name, case sensitive */

  if (StringCmp (qvp [FTQUAL_gene].str, qvp [FTQUAL_prot_desc].str) == 0) {
    qvp [FTQUAL_prot_desc].str = NULL;
  }

  /* remove protein description that equals the cds product, case sensitive */

  if (StringCmp (qvp [FTQUAL_cds_product].str, qvp [FTQUAL_prot_desc].str) == 0) {
    qvp [FTQUAL_prot_desc].str = NULL;
  }

  /* remove comment contained in prot_desc - gi|4530123|gb|AF071539.1|AF071539 */

  if (StringStr (qvp [FTQUAL_prot_desc].str, qvp [FTQUAL_seqfeat_note].str) != NULL) {
    qvp [FTQUAL_seqfeat_note].str = NULL;
  }

  /* remove protein description that equals the standard name */

  if (qvp [FTQUAL_standard_name].gbq != NULL && qvp [FTQUAL_prot_desc].str != NULL) {
    gbq = qvp [FTQUAL_standard_name].gbq;
    lasttype = gbq->qual;
    while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
      if (StringICmp (gbq->val, qvp [FTQUAL_prot_desc].str) == 0) {
        qvp [FTQUAL_prot_desc].str = NULL;
      }
      gbq = gbq->next;
    }
  }

  /* remove protein description that equals a gene synonym - case insensitive AF109216.1 */

  for (vnp = gene_syn; vnp != NULL; vnp = vnp->next) {
    str = (CharPtr) vnp->data.ptrvalue;
    if ((! StringHasNoText (str)) &&
        StringCmp (str, qvp [FTQUAL_prot_desc].str) == 0) {
      qvp [FTQUAL_prot_desc].str = NULL;
    }
  }

  /* remove comment that equals a gene synonym */

  if (afp->format != GENPEPT_FMT && (! ifp->mapToProt)) {
    for (vnp = gene_syn; vnp != NULL; vnp = vnp->next) {
      str = (CharPtr) vnp->data.ptrvalue;
      if ((! StringHasNoText (str)) &&
          StringICmp (str, qvp [FTQUAL_seqfeat_note].str) == 0) {
        qvp [FTQUAL_seqfeat_note].str = NULL;
      }
    }
  }

  /* remove protein comment descriptor that equals the protein note */

  if (StringCmp (qvp [FTQUAL_prot_note].str, qvp [FTQUAL_prot_comment].str) == 0) {
    qvp [FTQUAL_prot_comment].str = NULL;
  }

  /* suppress cds comment if a subset of protein note - AF002218.1 */

  if (StringStr (qvp [FTQUAL_prot_note].str, qvp [FTQUAL_seqfeat_note].str) != NULL) {
    qvp [FTQUAL_seqfeat_note].str = NULL;
  }

  /* suppress selenocysteine note if already in comment */

  if (StringStr (sfp->comment, "selenocysteine") != NULL) {
    qvp [FTQUAL_selenocysteine].str = NULL;
  }

  /* now print qualifiers from table */

#ifdef DISPLAY_STRINGS
  s_DisplayQVP(qvp, feat_note_order);
#endif

  /* Strip duplicate notes */

  if ((StringCmp(qvp[FTQUAL_product].str,
         qvp[FTQUAL_seqfeat_note].str) == 0)) {
    qvp[FTQUAL_seqfeat_note].str = NULL;
  }

  if ((qvp[FTQUAL_standard_name].gbq != NULL) &&
      (qvp[FTQUAL_standard_name].gbq->val != NULL)) {
    if ((StringCmp(qvp[FTQUAL_seqfeat_note].str,
           qvp[FTQUAL_standard_name].gbq->val) == 0)) {
      qvp[FTQUAL_seqfeat_note].str = NULL;
    }
  }

  /* Display strings for debugging purposes */

#ifdef DISPLAY_STRINGS
  s_DisplayQVP(qvp, feat_qual_order);
#endif


  /* Build the flat file */
  FormatFeatureBlockQuals (ajp, afp, asp, bsp, featdeftype, gene_syn,
                           lasttype, location, prod,
                           protein_pid_g, qvp,
                           left, right, strand,
                           sfp, target, is_other, is_gps);

  /* ??? and then deal with the various note types separately (not in order table) ??? */

  /* free aa-dna or dna-aa mapped location */

  SeqLocFree (loc);

  ValNodeFreeData (illegal);

  BioseqUnlock (unlockme);

  return ff_print_string_mem (gb_MergeString (TRUE));
}

static CharPtr FormatBasecountBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
)

{
  IntAsn2gbJobPtr   ajp;
  Asn2gbSectPtr     asp;
  Int4              base_count [5];
  BioseqPtr         bsp;
  Char              buf [80];
  Byte              bases [400];
  Uint1             code = Seq_code_iupacna;
  Int2              ctr;
  Int2              i;
  Int4              len;
  Uint1             residue;
  SeqPortPtr        spp = NULL;
  Int4              total = 0;

  if (afp == NULL || bbp == NULL) return NULL;
  ajp = afp->ajp;
  if (ajp == NULL) return NULL;

  asp = afp->asp;
  if (asp == NULL) return NULL;
  bsp = (asp->bsp);
  if (bsp == NULL) return NULL;

  /* after first formatting, result is cached into bbp->string */

  if (! StringHasNoText (bbp->string)) return StringSave (bbp->string);

  for (i = 0; i < 5; i++) {
    base_count [i] = 0;
  }

  if (ISA_aa (bsp->mol)) {
    code = Seq_code_ncbieaa;
  }

  if (ajp->ajp.slp != NULL) {
    spp = SeqPortNewByLoc (ajp->ajp.slp, code);
    len = SeqLocLen (ajp->ajp.slp);
  } else {
    spp = SeqPortNew (bsp, 0, -1, 0, code);
    len = bsp->length;
  }
  if (spp == NULL) return NULL;
  if (bsp->repr == Seq_repr_delta || bsp->repr == Seq_repr_virtual) {
    SeqPortSet_do_virtual (spp, TRUE);
  }

  /* use SeqPortRead rather than SeqPortGetResidue for faster performance */

  ctr = SeqPortRead (spp, bases, sizeof (bases));
  i = 0;
  residue = (Uint1) bases [i];
  while (residue != SEQPORT_EOF) {
    if (IS_residue (residue)) {
      total++;
      switch (residue) {
        case 'A' :
          (base_count [0])++;
          break;
        case 'C' :
          (base_count [1])++;
          break;
        case 'G' :
          (base_count [2])++;
          break;
        case 'T' :
          (base_count [3])++;
          break;
        default :
          (base_count [4])++;
          break;
      }
    }
    i++;
    if (i >= ctr) {
      i = 0;
      ctr = SeqPortRead (spp, bases, sizeof (bases));
      if (ctr < 0) {
        bases [0] = -ctr;
      } else if (ctr < 1) {
        bases [0] = SEQPORT_EOF;
      }
    }
    residue = (Uint1) bases [i];
  }

  SeqPortFree (spp);

  if (afp->format == GENBANK_FMT || afp->format == GENPEPT_FMT) {

    if (base_count [4] == 0) {
      sprintf (buf, "%7ld a%7ld c%7ld g%7ld t",
               (long) base_count [0], (long) base_count [1],
               (long) base_count [2], (long) base_count [3]);
    } else {
      sprintf (buf, "%7ld a%7ld c%7ld g%7ld t%7ld others",
               (long) base_count [0], (long) base_count [1],
               (long) base_count [2], (long) base_count [3],
               (long) base_count [4]);
    }

  } else if (afp->format == EMBL_FMT || afp->format == EMBLPEPT_FMT) {

    sprintf (buf, "Sequence %ld BP; %ld A; %ld C; %ld G; %ld T; %ld other;",
             (long) len,
             (long) base_count [0], (long) base_count [1],
             (long) base_count [2], (long) base_count [3],
             (long) base_count [4]);

    PrintXX ();
  }

  gb_StartPrint (afp->format, 0, 0, "BASE COUNT", 13, 5, 5, "SQ", FALSE);

  gb_AddString (NULL, buf, NULL, FALSE, FALSE, TILDE_TO_SPACES);

  return gb_MergeString (TRUE);
}

static void PrintSeqLine (
  FmtType format,
  CharPtr buf,
  Int4 start,
  Int4 stop
)

{
  Char  pos [16];

#ifdef WIN_MAC
  /* !!! To eliminate spurious diffs when comparing with Sequin exported GenBank format on Mac !!! */

  {
    size_t  len;

    len = StringLen (buf);
    if (len > 0 && buf [len - 1] == ' ') {
      buf [len - 1] = '\0';
    }
  }
#endif

  if (format == GENBANK_FMT || format == GENPEPT_FMT) {

    sprintf (pos, "%9ld", (long) (start + 1));
    ff_StartPrint (0, 0, ASN2FF_GB_MAX, NULL);
    gb_AddString (NULL, pos, NULL, FALSE, FALSE, TILDE_TO_SPACES);
    TabToColumn (11);
    gb_AddString (NULL, buf, NULL, FALSE, FALSE, TILDE_TO_SPACES);
    ff_EndPrint();

  } else if (format == EMBL_FMT || format == EMBLPEPT_FMT) {

    sprintf (pos, "%8ld", (long) (stop));
    ff_StartPrint (5, 5, 0, NULL);
    gb_AddString (NULL, buf, NULL, FALSE, FALSE, TILDE_TO_SPACES);
    TabToColumn (73);
    gb_AddString (NULL, pos, NULL, FALSE, FALSE, TILDE_TO_SPACES);
    ff_EndPrint();
  }
}

static CharPtr FormatSequenceBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
)

{
  IntAsn2gbJobPtr   ajp;
  Asn2gbSectPtr     asp;
  Byte              bases [400];
  Int2              blk;
  BioseqPtr         bsp;
  Char              buf [80];
  Int2              cnt;
  Uint1             code = Seq_code_iupacna;
  Int2              count;
  Int2              ctr;
  Int2              i;
  IntAsn2gbSectPtr  iasp;
  Boolean           is_na;
  Int2              lin;
  Int4              pos;
  Uint1             residue;
  SeqBlockPtr       sbp;
  SeqPortPtr        spp;
  Int4              start;
  Int4              stop;

  if (afp == NULL || bbp == NULL) return NULL;
  sbp = (SeqBlockPtr) bbp;
  ajp = afp->ajp;
  if (ajp == NULL) return NULL;

  asp = afp->asp;
  if (asp == NULL) return NULL;
  iasp = (IntAsn2gbSectPtr) asp;
  bsp = (asp->bsp);
  if (bsp == NULL) return NULL;

  spp = iasp->spp;
  if (spp == NULL) {

    /* if first time, create SeqPort for this section */

    if (ISA_aa (bsp->mol)) {
      if (ajp->flags.iupacaaOnly) {
        code = Seq_code_iupacaa;
      } else {
        code = Seq_code_ncbieaa;
      }
    }

    if (ajp->ajp.slp != NULL) {
      spp = SeqPortNewByLoc (ajp->ajp.slp, code);
    } else {
      spp = SeqPortNew (bsp, 0, -1, 0, code);
    }
    if (spp == NULL) return NULL;
    if (bsp->repr == Seq_repr_delta || bsp->repr == Seq_repr_virtual) {
      SeqPortSet_do_virtual (spp, TRUE);
    }

    iasp->spp = spp;
  }

  start = sbp->start;
  stop = sbp->stop;

  if (start != spp->curpos) {
    SeqPortSeek (spp, start, SEEK_SET);
  }

  pos = start;

  count = 0;
  cnt = 0;
  blk = 0;
  lin = 0;

  is_na = ISA_na (bsp->mol);

  ctr = (Int2) MIN ((Int4) (stop - pos), (Int4) sizeof (bases));
  ctr = SeqPortRead (spp, bases, ctr);

  i = 0;

  if (ctr < 0) {
    residue = -ctr;
  } else if (ctr < 1) {
    residue = SEQPORT_EOF;
  } else {
    residue = (Uint1) bases [i];
  }

  while (pos < stop && residue != SEQPORT_EOF) {

    if (residue == INVALID_RESIDUE) {
      if (is_na) {
        residue = 'N';
      } else {
        residue = 'X';
      }
    }

    if (IS_residue (residue)) {

      buf [count] = (Char) (TO_LOWER (residue));
      count++;
      cnt++;
      pos++;

      blk++;
      lin++;
      if (lin >= 60) {

        buf [count] = '\0';
        PrintSeqLine (afp->format, buf, start, start + cnt);
        count = 0;
        cnt = 0;
        blk = 0;
        lin = 0;
        start += 60;

      } else if (blk >= 10) {

        buf [count] = ' ';
        count++;
        blk = 0;

      }
    }

    i++;
    if (i >= ctr) {
      i = 0;
      ctr = (Int2) MIN ((Int4) (stop - pos), (Int4) sizeof (bases));
      ctr = SeqPortRead (spp, bases, ctr);
      if (ctr < 0) {
        bases [0] = -ctr;
      } else if (ctr < 1) {
        bases [0] = SEQPORT_EOF;
      }
    }
    residue = (Uint1) bases [i];
  }

  buf [count] = '\0';
  if (count > 0) {
    PrintSeqLine (afp->format, buf, start, start + cnt);
  }

  if (ajp->transientSeqPort) {
    iasp->spp = SeqPortFree (iasp->spp);
  }

  return gb_MergeString (FALSE);
}

  static Uint1 fasta_order [NUM_SEQID] = {
    33, /* 0 = not set */
    20, /* 1 = local Object-id */
    15, /* 2 = gibbsq */
    16, /* 3 = gibbmt */
    30, /* 4 = giim Giimport-id */
    10, /* 5 = genbank */
    10, /* 6 = embl */
    10, /* 7 = pir */
    10, /* 8 = swissprot */
    15, /* 9 = patent */
    20, /* 10 = other TextSeqId */
    20, /* 11 = general Dbtag */
    255, /* 12 = gi */
    10, /* 13 = ddbj */
    10, /* 14 = prf */
    12, /* 15 = pdb */
    10, /* 16 = tpg */
    10, /* 17 = tpe */
    10  /* 18 = tpd */
  };

static void PrintGenome (SeqLocPtr slp_head, CharPtr prefix)
{
  Char         buf[14], val[166];
  Boolean      first = TRUE;
  SeqLocPtr    slp;
  Int4         from, to, start, stop;
  SeqIdPtr     sid, newid;
  BioseqPtr    bsp = NULL;
  SeqEntryPtr  sep = NULL;
  Int2         p1=0, p2=0;

#ifdef ENABLE_ENTREZ
  SeqLocPtr    sslp;
  Int4         beg, end, lcur, lprev;
  Int4         uid;
  Boolean      is_network;
#endif


  for (slp = slp_head; slp; slp = slp->next) {
    from = to = 0;
    sid = SeqLocId(slp);
    if (slp->choice == SEQLOC_INT || slp->choice == SEQLOC_WHOLE) {
      start = from = SeqLocStart(slp);
      stop = to = SeqLocStop(slp);
    } else if (slp->choice == SEQLOC_NULL){
      sprintf(val, ",%s", "gap()");
      ff_AddString(val);
      continue;
    } else {
      continue;
    }
    if (sid == NULL) {
      continue;
    }
    if (sid->choice == SEQID_GI) {
      newid = GetSeqIdForGI(sid->data.intvalue);
    } else if (sid->choice == SEQID_GENERAL) {
#ifdef ENABLE_ENTREZ
      if ((uid = GetUniGeneIDForSeqId(sid)) != 0) {
        vn.choice = SEQID_GI;
        vn.data.intvalue = uid;
        vn.next = NULL;
        bsp = BioseqLockById (&vn);
      }
      if (bsp && (bsp->seq_ext_type == 1 || bsp->seq_ext_type)) {
        lcur = lprev = 0;
        for (sslp = slp_head; sslp; sslp = sslp->next) {
          lprev = lcur;
          lcur += SeqLocLen(sslp);
          beg = SeqLocStart(sslp);
          end = SeqLocStop(sslp);
          sid = SeqLocId(sslp);
          if (from > lcur || to < lprev) {
            continue;
          }
          if (from > lprev) { /* first */
            start = beg + from - lprev;
          } else {
            start = beg;  /* middle */
          }
          if (to > lcur) {  /* middle */
            stop = end;
          } else {
            stop = beg + to - lprev;
          }
          if (first) {
            first = FALSE;
          } else {
            ff_AddChar(',');
          }
          if (sid->choice == SEQID_GI) {
            newid = GetSeqIdForGI(sid->data.intvalue);
            if (newid == NULL) {
              newid = sid;
            }
          } else {
            newid = sid;
          }
          SeqIdWrite(SeqIdSelect(newid, fasta_order, NUM_SEQID),
                     buf, PRINTID_TEXTID_ACC_VER, 13);

          if (SeqLocStrand(slp) == Seq_strand_minus) {
            ff_AddString("complement(");
          }
          ff_AddString(buf);
          if (SeqLocStrand(slp) == Seq_strand_minus) {
            sprintf(val,":%ld..%ld)",(long) start+1, (long) stop+1 );
          } else {
            sprintf(val,":%ld..%ld",(long) start+1, (long) stop+1 );
          }
          ff_AddString(val);
          p1 += StringLen(val);
          p2 += StringLen(val);
        }
        continue;
      }
      newid = sid;
#else
      newid = sid;
#endif
    } else {
      newid = sid;
    }
    if (prefix != NULL) {
      ff_AddString (prefix);
    }
    if (first) {
      first = FALSE;
    } else {
      ff_AddChar(',');
    }
    SeqIdWrite(SeqIdSelect(newid, fasta_order, NUM_SEQID),
               buf, PRINTID_TEXTID_ACC_VER, 13);

    ff_AddString( buf);
    if (SeqLocStrand(slp) == Seq_strand_minus) {
      sprintf(val,":%ld..%ld)",(long) start+1, (long) stop+1 );
    } else {
      sprintf(val,":%ld..%ld",(long) start+1, (long) stop+1 );
    }
    ff_AddString(val);
    p1 += StringLen(val);
    p2 += StringLen(val);
  }
}

static CharPtr FormatContigBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
)

{
  Asn2gbSectPtr  asp;
  BioseqPtr      bsp;
  DeltaSeqPtr    dsp;
  SeqLitPtr      litp;
  CharPtr        prefix = NULL;
  SeqLocPtr      slp_head = NULL;
  Char           val [20];

  if (afp == NULL || bbp == NULL) return NULL;

  asp = afp->asp;
  if (asp == NULL) return NULL;
  bsp = (asp->bsp);
  if (bsp == NULL) return NULL;


  ff_StartPrint (0, 12, ASN2FF_GB_MAX, NULL);
  ff_AddString ("CONTIG");
  TabToColumn (13);
  ff_AddString("join(");

  if (bsp->seq_ext_type == 1) {

    slp_head = (SeqLocPtr) bsp->seq_ext;
    PrintGenome (slp_head, prefix);

  } else if (bsp->seq_ext_type == 4) {

    for (dsp = (DeltaSeqPtr) bsp->seq_ext; dsp; dsp=dsp->next) {
      if (dsp->choice == 1) {

        slp_head = (SeqLocPtr) dsp->data.ptrvalue;
        PrintGenome (slp_head, prefix);

      } else {

        litp = (SeqLitPtr) dsp->data.ptrvalue;
        if (litp != NULL) {
          if (litp->seq_data != NULL) {
            if (litp->length == 0) {
              sprintf (val, "gap(%ld)", (long) litp->length);
              ff_AddString (val);
            } else {
              /* don't know what to do here */
            }
          } else {
            sprintf (val, ",gap(%ld)", (long) litp->length);
            ff_AddString (val);
          }
        }
      }

      prefix = ",";
    }
  }

  ff_AddChar (')');
  ff_EndPrint ();

  return gb_MergeString (FALSE);
}


/* ********************************************************************** */

/* functions to record sections or blocks in linked lists */

static Asn2gbSectPtr Asn2gbAddSection (
  Asn2gbWorkPtr awp
)

{
  Asn2gbSectPtr  asp;
  ValNodePtr     vnp;

  if (awp == NULL) return NULL;

  asp = (Asn2gbSectPtr) MemNew (sizeof (IntAsn2gbSect));
  if (asp == NULL) return NULL;

  vnp = ValNodeAddPointer (&(awp->lastsection), 0, asp);
  if (vnp == NULL) return asp;

  awp->lastsection = vnp;
  if (awp->sectionList == NULL) {
    awp->sectionList = vnp;
  }

  return asp;
}

static BaseBlockPtr Asn2gbAddBlock (
  Asn2gbWorkPtr awp,
  BlockType blocktype,
  size_t size
)

{
  BaseBlockPtr  bbp;
  ValNodePtr    vnp;

  if (awp == NULL || size < 1) return NULL;

  bbp = (BaseBlockPtr) MemNew (size);
  if (bbp == NULL) return NULL;
  bbp->blocktype = blocktype;
  bbp->section = awp->currsection;

  vnp = ValNodeAddPointer (&(awp->lastblock), 0, bbp);
  if (vnp == NULL) return bbp;

  awp->lastblock = vnp;
  if (awp->blockList == NULL) {
    awp->blockList = vnp;
  }

  return bbp;
}


/* ********************************************************************** */

/* add functions allocate specific blocks, populate with paragraph print info */

static CharPtr strd [4] = {
  "   ", "ss-", "ds-", "ms-"
};

static CharPtr gnbk_mol [14] = {
  "    ", "DNA ", "RNA ", "mRNA", "rRNA", "tRNA", "uRNA", "scRNA", " AA ", "DNA ", "DNA ", "RNA ", "snoRNA", "RNA "
};

/* EMBL_FMT in RELEASE_MODE or ENTREZ_MODE, otherwise use gnbk_mol */

static CharPtr embl_mol [14] = {
  "xxx", "DNA", "RNA", "RNA", "RNA", "RNA", "RNA", "RNA", "AA ", "DNA", "DNA", "RNA", "RNA", "RNA"
};

static CharPtr embl_divs [18] = {
  "FUN", "INV", "MAM", "ORG", "PHG", "PLN", "PRI", "PRO", "ROD"
  "SYN", "UNA", "VRL", "VRT", "PAT", "EST", "STS", "HUM", "HTC"
};

static DatePtr GetBestDate (
  DatePtr a,
  DatePtr b
)

{
  Int2  status;

  if (a == NULL) return b;
  if (b == NULL) return a;

  status = DateMatch (a, b, FALSE);
  if (status == 1) return a;

  return b;
}

/*--------------------------------------------------------*/
/*                                                        */
/*  s_IsSeperatorNeeded()                                 */
/*                                                        */
/*--------------------------------------------------------*/

static Boolean s_IsSeperatorNeeded(CharPtr baseString, Int4 baseLength, Int2 suffixLength)
{
  Char lastChar;
  Char nextToLastChar;

  lastChar = baseString[baseLength - 1];
  nextToLastChar = baseString[baseLength - 2];

  /* This first check put here to emulate what may be a  */
  /* bug in the original code (in CheckLocusLength() )   */
  /* which adds an 'S' segment seperator only if it      */
  /* DOES make the string longer than the max.           */

  if (baseLength + suffixLength < 10)
    return FALSE;

  /* If the last character is not a digit */
  /* then don't use a seperator.          */

  if (!IS_DIGIT(lastChar))
    return FALSE;

  /* If the last two characters are a non-digit   */
  /* followed by a '0', then don't use seperator. */

  if ((lastChar == '0') && (!IS_DIGIT(nextToLastChar)))
    return FALSE;

  /* If we made it to here, use a seperator */

  return TRUE;
}

/*--------------------------------------------------------*/
/*                                                        */
/*  s_LocusAddSuffix() -                                  */
/*                                                        */
/*--------------------------------------------------------*/

static Boolean s_LocusAddSuffix (CharPtr locus, Asn2gbWorkPtr awp)
{
  size_t  buflen;
  Char    ch;
  Char    segCountStr[6];
  Int2    segCountStrLen;
  Char    segSuffix[5];

  buflen = StringLen (locus);

  /* If there's one or less segments, */
  /* no suffix is needed.             */

  if (awp->numsegs <= 1)
    return FALSE;

  /* If the basestring has one or less */
  /* characters, no suffix is needed.  */

  if (buflen <=1)
    return FALSE;

  /* Add the suffix */

  ch = locus[buflen-1];
  sprintf(segCountStr,"%d",awp->numsegs);
  segCountStrLen = StringLen(segCountStr);
  segSuffix[0] = '\0';

  if (s_IsSeperatorNeeded(locus,buflen,segCountStrLen) == TRUE)
    sprintf(segSuffix,"S%0*d",segCountStrLen,awp->seg);
  else
    sprintf(segSuffix,"%0*d",segCountStrLen,awp->seg);
  StringCat(locus,segSuffix);

  /* Return successfully */

  return TRUE;
}

/*--------------------------------------------------------*/
/*                                                        */
/*  s_LocusAdjustLength() -                               */
/*                                                        */
/*--------------------------------------------------------*/

static Boolean s_LocusAdjustLength(CharPtr locus, Int2 maxLength)
{
  Int2     trimCount;
  Int2     buflen;
  CharPtr  buftmp;

  buftmp = MemNew(maxLength + 1);
  buflen = StringLen (locus);

  if (buflen <= maxLength)
    return FALSE;

  /* If the sequence id is an NCBI locus of the */
  /* form HSU00001, then make sure that if      */
  /* there is trimming the HS gets trimmed off  */
  /* as a unit, never just the 'H'.             */

  trimCount = buflen - maxLength;
  if (trimCount == 1)
    if (IS_ALPHA(locus[0]) != 0 &&
        IS_ALPHA(locus[1]) != 0 &&
        IS_ALPHA(locus[2]) != 0 &&
        IS_DIGIT(locus[3]) != 0 &&
        IS_DIGIT(locus[4]) != 0 &&
        IS_DIGIT(locus[5]) != 0 &&
        IS_DIGIT(locus[6]) != 0 &&
        IS_DIGIT(locus[7]) != 0 &&
        locus[8] == 'S' &&
        locus[9] == '\0')
      trimCount++;

  /* Left truncate the sequence id */

  StringCpy(buftmp, &locus[trimCount]);
  StringCpy(locus, buftmp);

  MemFree(buftmp);
  return TRUE;
}

/*--------------------------------------------------------*/
/*                                                        */
/*  AddLocusBlock() -                                     */
/*                                                        */
/*--------------------------------------------------------*/

static DatePtr GetBestDateForBsp (
  BioseqPtr bsp
)

{
  DatePtr            best_date = NULL;
  SeqMgrDescContext  dcontext;
  DatePtr            dp;
  EMBLBlockPtr       ebp;
  GBBlockPtr         gbp;
  PdbBlockPtr        pdp;
  PdbRepPtr          prp;
  SeqDescrPtr        sdp;
  SPBlockPtr         spp;

  if (bsp == NULL) return NULL;

  dp = NULL;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_update_date, &dcontext);
  if (sdp != NULL) {
    dp = (DatePtr) sdp->data.ptrvalue;
    best_date = GetBestDate (dp, best_date);
  }

  /* !!! temporarily also look at genbank block entry date !!! */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_genbank, &dcontext);
  if (sdp != NULL) {
    gbp = (GBBlockPtr) sdp->data.ptrvalue;
    if (gbp != NULL) {
      dp = gbp->entry_date;
      best_date = GetBestDate (dp, best_date);
    }
  }

  /* more complicated code for dates from various objects goes here */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_embl, &dcontext);
  if (sdp != NULL) {
    ebp = (EMBLBlockPtr) sdp->data.ptrvalue;
    if (ebp != NULL) {
      dp = ebp->creation_date;
      best_date = GetBestDate (dp, best_date);
      dp = ebp->update_date;
      best_date = GetBestDate (dp, best_date);
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_sp, &dcontext);
  if (sdp != NULL) {
    spp = (SPBlockPtr) sdp->data.ptrvalue;
    if (spp != NULL) {
      dp = spp->created;
      if (dp != NULL && dp->data [0] == 1) {
        best_date = GetBestDate (dp, best_date);
      }
      dp = spp->sequpd;
      if (dp != NULL && dp->data [0] == 1) {
        best_date = GetBestDate (dp, best_date);
      }
      dp = spp->annotupd;
      if (dp != NULL && dp->data [0] == 1) {
        best_date = GetBestDate (dp, best_date);
      }
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pdb, &dcontext);
  if (sdp != NULL) {
    pdp = (PdbBlockPtr) sdp->data.ptrvalue;
    if (pdp != NULL) {
      dp = pdp->deposition;
      if (dp != NULL && dp->data [0] == 1) {
        best_date = GetBestDate (dp, best_date);
      }
      prp = pdp->replace;
      if (prp != NULL) {
        dp = prp->date;
        if (dp != NULL && dp->data[0] == 1) {
          best_date = GetBestDate (dp, best_date);
        }
      }
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_create_date, &dcontext);
  if (sdp != NULL) {
    dp = (DatePtr) sdp->data.ptrvalue;
    if (dp != NULL) {
      best_date = GetBestDate (dp, best_date);
    }
  }

  return best_date;
}

static Boolean SegHasParts (
  BioseqPtr bsp
)

{
  BioseqSetPtr  bssp;
  SeqEntryPtr   sep;

  if (bsp == NULL || bsp->repr != Seq_repr_seg) return FALSE;
  sep = bsp->seqentry;
  if (sep == NULL) return FALSE;
  sep = sep->next;
  if (sep == NULL || (! IS_Bioseq_set (sep))) return FALSE;
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  if (bssp != NULL && bssp->_class == BioseqseqSet_class_parts) return TRUE;
  return FALSE;
}

static Boolean DeltaLitOnly (
  BioseqPtr bsp
)

{
  ValNodePtr  vnp;

  if (bsp == NULL || bsp->repr != Seq_repr_delta) return FALSE;
  for (vnp = (ValNodePtr)(bsp->seq_ext); vnp != NULL; vnp = vnp->next) {
    if (vnp->choice == 1) return FALSE;
  }
  return TRUE;
}

static void AddLocusBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr    ajp;
  BaseBlockPtr       bbp;
  DatePtr            best_date = NULL;
  BioSourcePtr       biop;
  Int2               bmol = 0;
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  Char               date [40];
  SeqMgrDescContext  dcontext;
  Char               div [10];
  BioseqPtr          dna;
  CharPtr            ebmol;
  EMBLBlockPtr       ebp;
  SeqMgrFeatContext  fcontext;
  GBBlockPtr         gbp;
  Boolean            genome_view;
  Int2               imol = 0;
  Int2               istrand;
  Char               len [15];
  Int4               length;
  Char               locus [41];
  MolInfoPtr         mip;
  Char               mol [30];
  OrgNamePtr         onp;
  Uint1              origin;
  OrgRefPtr          orp;
  BioseqPtr          parent;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;
  SeqIdPtr           sip;
  Uint1              tech;
  Uint1              topology;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  bbp = Asn2gbAddBlock (awp, LOCUS_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  mol [0] = '\0';
  len [0] = '\0';
  div [0] = '\0';
  date [0] = '\0';

  genome_view = FALSE;
  if (bsp->repr == Seq_repr_seg && (! SegHasParts (bsp))) {
    genome_view = TRUE;
  }
  if (bsp->repr == Seq_repr_delta && (! DeltaLitOnly (bsp))) {
    genome_view = TRUE;
  }

  /* locus id */

  sip = NULL;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GENBANK ||
        sip->choice == SEQID_EMBL ||
        sip->choice == SEQID_DDBJ ||
        sip->choice == SEQID_OTHER) break;
    if (sip->choice == SEQID_PIR ||
        sip->choice == SEQID_SWISSPROT ||
        sip->choice == SEQID_PRF ||
        sip->choice == SEQID_PDB) break;
    if (sip->choice == SEQID_TPG ||
        sip->choice == SEQID_TPE ||
        sip->choice == SEQID_TPD) break;
  }
  if (sip == NULL) {
    sip = SeqIdFindBest (bsp->id, SEQID_GENBANK);
  }

  if (genome_view) {
    SeqIdWrite (sip, locus, PRINTID_TEXTID_ACCESSION, sizeof (locus) - 1);
  } else {
    SeqIdWrite (sip, locus, PRINTID_TEXTID_LOCUS, sizeof (locus) - 1);
  }

  /* more complicated code to get parent locus, if segmented, goes here */

  if (awp->slp != NULL) {
    length = SeqLocLen (awp->slp);
  } else {
    length = bsp->length;
  }

  mip = NULL;
  tech = MI_TECH_standard;
  origin = 0;
  bmol = bsp->mol;
  if (bmol > Seq_mol_aa) {
    bmol = 0;
  }
  istrand = bsp->strand;
  if (istrand > Seq_strand_both) {
    istrand = Seq_strand_unknown;
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp != NULL) {
    bbp->entityID = dcontext.entityID;
    bbp->itemID = dcontext.itemID;
    bbp->itemtype = OBJ_SEQDESC;

    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      if (mip->biomol <= MOLECULE_TYPE_TRANSCRIBED_RNA) {
        imol = (Int2) mip->biomol;
      }
      tech = mip->tech;
    }
  }

  /* check inst.mol if mol-type is not-set or genomic */

  if (imol <= MOLECULE_TYPE_GENOMIC) {
    if (bmol == Seq_mol_aa) {
      imol = MOLECULE_TYPE_PEPTIDE;
    } else if (bmol == Seq_mol_na) {
      imol = 0;
    } else if (bmol == Seq_mol_rna) {
      imol = 2;
    }
  }

  /* if ds-DNA don't show ds */

  if (bmol == Seq_mol_dna && istrand == Seq_strand_minus) {
    istrand = Seq_strand_unknown;
  }

  /* ss=any RNA don't show ss */

  if (bmol > Seq_mol_rna && istrand == Seq_strand_plus) {
    istrand = Seq_strand_unknown;
  }

  topology = bsp->topology;
  if (awp->slp != NULL) {
    topology = TOPOLOGY_LINEAR;
  }

  /* length, topology, and molecule type */

  if (awp->format == GENBANK_FMT) {

    if (awp->newLocusLine) {

      sprintf (len, "%ld bp", (long) length);
      sprintf (mol, "%s%-4s", strd [istrand], gnbk_mol [imol]);

    } else {

      if (topology == TOPOLOGY_CIRCULAR) {
        sprintf (len, "%7ld bp", (long) length);
        sprintf (mol, "%s%-4s  circular", strd [istrand], gnbk_mol [imol]);
      } else {
        sprintf (len, "%7ld bp", (long) length);
        sprintf (mol, "%s%-4s          ", strd [istrand], gnbk_mol [imol]);
      }
    }

  } else if (awp->format == GENPEPT_FMT) {

    if (awp->newLocusLine) {
      sprintf (len, "%ld aa", (long) length);
    } else {
      sprintf (len, "%7ld aa", (long) length);
    }

  } else if (awp->format == EMBL_FMT) {

    if (imol < MOLECULE_TYPE_PEPTIDE) {
      if (ajp->flags.useEmblMolType) {
        ebmol = embl_mol [imol];
      } else {
        ebmol = gnbk_mol [imol];
      }

      if (topology == TOPOLOGY_CIRCULAR) {
        sprintf (mol, "circular %s", ebmol);
        sprintf (len, "%ld BP.", (long) length);
      } else {
        sprintf (mol, "%s", ebmol);
        sprintf (len, "%ld BP.", (long) length);
      }
    }
  }

  /* division */

  biop = NULL;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop = (BioSourcePtr) sdp->data.ptrvalue;
  } else {
    sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_BIOSRC, 0, &fcontext);
    if (sfp != NULL) {
      biop = (BioSourcePtr) sfp->data.value.ptrvalue;
    } else if (ISA_aa (bsp->mol)) {

      /* if protein with no sources, get sources applicable to DNA location of CDS */

      cds = SeqMgrGetCDSgivenProduct (bsp, &fcontext);
      if (cds != NULL) {
        sfp = SeqMgrGetOverlappingSource (cds->location, &fcontext);
        if (sfp != NULL) {
          biop = (BioSourcePtr) sfp->data.value.ptrvalue;
        } else {
          dna = BioseqFindFromSeqLoc (cds->location);
          if (dna != NULL) {
            sdp = SeqMgrGetNextDescriptor (dna, NULL, Seq_descr_source, &dcontext);
            if (sdp != NULL) {
              biop = (BioSourcePtr) sdp->data.ptrvalue;
            }
          }
        }
      }
    }
  }
  if (biop != NULL) {
    origin = biop->origin;
    orp = biop->org;
    if (orp != NULL) {
      onp = orp->orgname;
      if (onp != NULL) {
        StringNCpy_0 (div, onp->div, sizeof (div));
      }
    }
  }

  switch (tech) {
    case MI_TECH_est :
      StringCpy (div, "EST");
      break;
    case MI_TECH_sts :
      StringCpy (div, "STS");
      break;
    case MI_TECH_survey :
      StringCpy (div, "GSS");
      break;
    case MI_TECH_htgs_0 :
    case MI_TECH_htgs_1 :
    case MI_TECH_htgs_2 :
      StringCpy (div, "HTG");
      break;
    case MI_TECH_htc :
      StringCpy (div, "HTC");
      break;
    default :
      break;
  }

  if (origin == 5) {
    StringCpy (div, "SYN");
  }

  sip = SeqIdFindBest (bsp->id, SEQID_PATENT);
  if (sip != NULL && sip->choice == SEQID_PATENT) {
    StringCpy (div, "PAT");
  }

  /* more complicated code for division, if necessary, goes here */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_genbank, &dcontext);
  while (sdp != NULL) {
    gbp = (GBBlockPtr) sdp->data.ptrvalue;
    if (gbp != NULL) {
      if (StringHasNoText (div) && gbp->div != NULL) {
        StringCpy (div, gbp->div);
      }
      else if (StringCmp(gbp->div, "PAT") == 0 ||
               StringCmp(gbp->div, "SYN") == 0 ) {
        StringCpy (div, gbp->div);
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_genbank, &dcontext);
  }

  if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {

    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_embl, &dcontext);
    if (sdp != NULL) {
      ebp = (EMBLBlockPtr) sdp->data.ptrvalue;
      if (ebp != NULL) {
        if (ebp->div == 255) {
          if (mip == NULL) {
            StringCpy (div, "HUM");
          }
        } else if (ebp->div < 18)  {
          StringCpy (div, embl_divs [ebp->div]);
        }
      }
    }

    if (StringHasNoText (div)) {
      StringCpy (div, "UNA");
    }
  }

  /* empty division field if unable to find anything */

  if (StringHasNoText (div)) {
    StringCpy (div, "   ");
  }

  /* contig style (old genome_view flag) forces CON division */

  if (awp->contig) {
    StringCpy (div, "CON");
  }

  if (genome_view) {
    StringCpy (div, "CON");
  }

  /* date */

  best_date = GetBestDateForBsp (bsp);

  if (best_date == NULL) {

    /* if bsp is product of CDS or mRNA feature, get date from sfp->location bsp */

    sfp = NULL;
    if (ISA_na (bsp->mol)) {
      sfp = SeqMgrGetRNAgivenProduct (bsp, NULL);
    } else if (ISA_aa (bsp->mol)) {
      sfp = SeqMgrGetCDSgivenProduct (bsp, NULL);
    }
    if (sfp != NULL) {
      parent = BioseqFindFromSeqLoc (sfp->location);
      if (parent != NULL) {
        best_date = GetBestDateForBsp (parent);
      }
    }
  }

  /* convert best date */

  if (best_date != NULL) {
    DateToGB (date, best_date, FALSE);
  }
  if (StringHasNoText (date)) {
    StringCpy (date, "01-JAN-1900");
  }

  if (awp->format == GENBANK_FMT || awp->format == GENPEPT_FMT) {

    /* Create the proper locus name */

    parent = awp->parent;
    if (parent->repr == Seq_repr_seg) {

      if (! StringHasNoText (awp->basename)) {
        StringCpy (locus, awp->basename);
        s_LocusAddSuffix (locus, awp);
      }
    }

    /* Print the "LOCUS_NEW" line, if requested */

    if (awp->newLocusLine) {
      ff_StartPrint (0, 0, ASN2FF_GB_MAX, NULL);
      ff_AddString ("LOCUS");
      TabToColumn (13);
      parent = awp->parent;

      if (parent->repr == Seq_repr_seg)
        s_LocusAdjustLength (locus,16);

      ff_AddString (locus);
      TabToColumn (44 - StringLen (len));
      ff_AddString (len);
      TabToColumn (45);
      ff_AddString (mol);
      TabToColumn (56);
      if (topology == TOPOLOGY_CIRCULAR) {
        ff_AddString ("circular");
      } else {
        ff_AddString ("linear  ");
      }
      TabToColumn (65);
      ff_AddString (div);
      TabToColumn (69);
      ff_AddString (date);
    }

    /* Else print the "LOCUS" line */

    else {

      ff_StartPrint (0, 0, ASN2FF_GB_MAX, NULL);
      ff_AddString ("LOCUS");
      TabToColumn (13);

      if (parent->repr == Seq_repr_seg)
        s_LocusAdjustLength (locus,10);

      ff_AddString (locus);
      TabToColumn (33 - StringLen (len));
      ff_AddString (len);
      TabToColumn (34);
      ff_AddString (mol);
      TabToColumn (53);
      ff_AddString (div);
      TabToColumn (63);
      ff_AddString (date);
    }

  } else if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {

    ff_StartPrint (5, 0, ASN2FF_EMBL_MAX, "ID");
    ff_AddString (locus);
      TabToColumn (16);
    if (awp->hup) {
      ff_AddString (" confidential; ");
    } else {
      ff_AddString (" standard; ");
    }
    ff_AddString (mol);
    ff_AddString ("; ");

    /* conditional code to make div "UNA" goes here */

    ff_AddString (div);
    ff_AddString ("; ");
    ff_AddString (len);
  }

  bbp->string = gb_MergeString (TRUE);
}

static void AddDeflineBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  CharPtr            buf;
  size_t             buflen = 1001;
  SeqMgrDescContext  dcontext;
  ItemInfo           ii;
  MolInfoPtr         mip;
  SeqDescrPtr        sdp;
  Uint1              tech;

  if (awp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  bbp = Asn2gbAddBlock (awp, DEFLINE_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  tech = 0;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp != NULL) {
    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      tech = mip->tech;
    }
  }

  buf = MemNew (sizeof (Char) * (buflen + 1));
  MemSet ((Pointer) (&ii), 0, sizeof (ItemInfo));

  /* create default defline */

  if (buf != NULL && CreateDefLine (&ii, bsp, buf, buflen, tech, NULL, NULL)) {
    bbp->entityID = ii.entityID;
    bbp->itemID = ii.itemID;
    bbp->itemtype = ii.itemtype;

    gb_StartPrint (awp->format, 0, 12, "DEFINITION", 13, 5, 5, "DE", TRUE);

    gb_AddString (NULL, buf, NULL, TRUE, TRUE, TILDE_IGNORE);

    bbp->string = gb_MergeString (TRUE);
  }

  MemFree (buf);
}

/* !!! this definitely needs more work to support all classes, use proper SeqId !!! */

static void AddAccessionBlock (
  Asn2gbWorkPtr awp
)

{
  SeqIdPtr           accn = NULL;
  IntAsn2gbJobPtr    ajp;
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  Char               buf [41];
  SeqMgrDescContext  dcontext;
  EMBLBlockPtr       ebp;
  ValNodePtr         extra_access;
  CharPtr            flatloc;
  GBBlockPtr         gbp;
  SeqIdPtr           gi = NULL;
  ValNodePtr         head = NULL;
  SeqIdPtr           lcl = NULL;
  SeqDescrPtr        sdp;
  CharPtr            separator;
  SeqIdPtr           sip;
  CharPtr            str;
  ValNodePtr         vnp;
  CharPtr            xtra;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_GI :
        gi = sip;
        break;
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_DDBJ :
      case SEQID_OTHER :
        accn = sip;
        break;
      case SEQID_PIR :
      case SEQID_SWISSPROT :
      case SEQID_PRF :
      case SEQID_PDB :
        accn = sip;
        break;
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
        accn = sip;
        break;
      case SEQID_LOCAL :
        lcl = sip;
        break;
      default :
        break;
    }
  }

  sip = NULL;
  if (accn != NULL) {
    sip = accn;
  } else if (lcl != NULL) {
    sip = lcl;
  } else if (gi != NULL) {
    sip = gi;
  }

  if (sip == NULL) return;

  SeqIdWrite (sip, buf, PRINTID_TEXTID_ACC_ONLY, sizeof (buf));

  bbp = Asn2gbAddBlock (awp, ACCESSION_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  gb_StartPrint (awp->format, 0, 12, "ACCESSION", 13, 5, 5, "AC", TRUE);

  if (awp->hup && accn != NULL) {
    gb_AddString (NULL, ";", NULL, FALSE, FALSE, TILDE_TO_SPACES);

  } else if (ajp->ajp.slp != NULL) {

    flatloc =  FlatLoc (bsp, ajp->ajp.slp, ajp->masterStyle);
    gb_AddString (buf, " REGION: ", flatloc, FALSE, FALSE, TILDE_TO_SPACES);
    MemFree (flatloc);

  } else {

    gb_AddString (NULL, buf, NULL, FALSE, FALSE, TILDE_TO_SPACES);
  }

  if (awp->format == GENBANK_FMT || awp->format == GENPEPT_FMT) {
    separator = " ";
  } else if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
    separator = ";";
  }

  if (ajp->ajp.slp == NULL) {
    sdp = SeqMgrGetNextDescriptor (bsp, NULL, 0, &dcontext);
    while (sdp != NULL) {

      extra_access = NULL;

      switch (dcontext.seqdesctype) {
        case Seq_descr_genbank :
          gbp = (GBBlockPtr) sdp->data.ptrvalue;
          if (gbp != NULL) {
            extra_access = gbp->extra_accessions;
          }
          break;
        case Seq_descr_embl :
          ebp = (EMBLBlockPtr) sdp->data.ptrvalue;
          if (ebp != NULL) {
            extra_access = ebp->extra_acc;
          }
          break;
        default :
          break;
      }

      if (extra_access != NULL) {
        bbp->entityID = dcontext.entityID;
        bbp->itemID = dcontext.itemID;
        bbp->itemtype = OBJ_SEQDESC;
      }

      for (vnp = extra_access; vnp != NULL; vnp = vnp->next) {
        xtra = (CharPtr) vnp->data.ptrvalue;
        if (ValidateAccession (xtra) == 0) {
          ValNodeCopyStr (&head, 0, separator);
          ValNodeCopyStr (&head, 0, xtra);
        }
      }

      sdp = SeqMgrGetNextDescriptor (bsp, sdp, 0, &dcontext);
    }
  }

  str = MergeValNodeStrings (head);

  gb_AddString (NULL, str, NULL, FALSE, FALSE, TILDE_TO_SPACES);

  MemFree (str);
  ValNodeFreeData (head);

  bbp->string = gb_MergeString (TRUE);
}

static void AddVersionBlock (
  Asn2gbWorkPtr awp
)

{
  SeqIdPtr      accn = NULL;
  BaseBlockPtr  bbp;
  BioseqPtr     bsp;
  Char          buf [41];
  Int4          gi = -1;
  SeqIdPtr      sip;
  Char          version [64];

  if (awp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_GI :
        gi = sip->data.intvalue;
        break;
      case SEQID_GENBANK :
      case SEQID_EMBL :
      case SEQID_DDBJ :
      case SEQID_OTHER :
        accn = sip;
        break;
      case SEQID_PIR :
      case SEQID_SWISSPROT :
      case SEQID_PRF :
      case SEQID_PDB :
        accn = sip;
        break;
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
        accn = sip;
        break;
      default :
        break;
    }
  }

  /* if (gi < 1 && accn == NULL) return; */

  if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
    if (gi < 1) return;
  }

  bbp = Asn2gbAddBlock (awp, VERSION_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  /* no longer displaying NID */

  /*
  if (gi > 0) {
    sprintf (version, "g%ld", (long) gi);

    gb_StartPrint (awp->format, needInitBuff, 0, 12, "NID", 13, 5, 5, "NI", TRUE);
    needInitBuff = FALSE;

    gb_AddString (NULL, version, NULL, FALSE, FALSE, TILDE_TO_SPACES);

    ff_EndPrint();
    needEndPrint = FALSE;
  }
  */

  version [0] = '\0';

  if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
    sprintf (version, "g%ld", (long) gi);

    gb_StartPrint (awp->format, 0, 12, "VERSION", 13, 5, 5, "NI", TRUE);

    gb_AddString (NULL, version, NULL, FALSE, FALSE, TILDE_TO_SPACES);

    ff_EndPrint();

    bbp->string = gb_MergeString (FALSE);

    return;
  }

  if (accn != NULL) {

    buf [0] = '\0';
    SeqIdWrite (accn, buf, PRINTID_TEXTID_ACC_VER, sizeof (buf) - 1);

    if (gi > 0) {
      sprintf (version, "%s  GI:%ld", buf, (long) gi);
    } else {
      sprintf (version, "%s", buf);
    }

    gb_StartPrint (awp->format, 0, 12, "VERSION", 13, 5, 5, "SV", TRUE);

    gb_AddString (NULL, version, NULL, FALSE, FALSE, TILDE_TO_SPACES);

    ff_EndPrint();

  } else if (gi > 0) {

    gb_StartPrint (awp->format, 0, 0, "VERSION", 13, 5, 5, "SV", TRUE);

    sprintf (version, "  GI:%ld", (long) gi);

    gb_AddString (NULL, version, NULL, FALSE, FALSE, TILDE_TO_SPACES);

    ff_EndPrint();

  } else {

    gb_StartPrint (awp->format, 0, 0, "VERSION", 0, 5, 5, "SV", TRUE);

    ff_EndPrint();
  }

  bbp->string = gb_MergeString (FALSE);
}

/* only displaying PID in GenPept format */

static void AddPidBlock (Asn2gbWorkPtr awp)

{
  BaseBlockPtr  bbp;
  BioseqPtr     bsp;
  Int4          gi = -1;
  SeqIdPtr      sip;
  Char          version [64];

  if (awp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    switch (sip->choice) {
      case SEQID_GI :
        gi = sip->data.intvalue;
        break;
      default :
        break;
    }
  }

  if (gi < 1) return;

  bbp = Asn2gbAddBlock (awp, PID_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  gb_StartPrint (awp->format, 0, 12, "PID", 13, 5, 5, NULL, TRUE);

  sprintf (version, "g%ld", (long) gi);
  gb_AddString (NULL, version, NULL, FALSE, FALSE, TILDE_TO_SPACES);

  ff_EndPrint();

  bbp->string = gb_MergeString (FALSE);
}


static Uint1 dbsource_fasta_order [NUM_SEQID] = {
  33, /* 0 = not set */
  20, /* 1 = local Object-id */
  15, /* 2 = gibbsq */
  16, /* 3 = gibbmt */
  30, /* 4 = giim Giimport-id */
  10, /* 5 = genbank */
  10, /* 6 = embl */
  10, /* 7 = pir */
  10, /* 8 = swissprot */
  15, /* 9 = patent */
  18, /* 10 = other TextSeqId */
  20, /* 11 = general Dbtag */
  31, /* 12 = gi */
  10, /* 13 = ddbj */
  10, /* 14 = prf */
  12, /* 15 = pdb */
  10, /* 16 = tpg */
  10, /* 17 = tpe */
  10  /* 18 = tpd */
};

static void AddToUniqueSipList (
  ValNodePtr PNTR list,
  SeqIdPtr sip
)

{
  ValNodePtr  vnp;

  if (list == NULL || sip == NULL) return;
  for (vnp = *list; vnp != NULL; vnp = vnp->next) {
    if (SeqIdMatch (sip, (SeqIdPtr) vnp->data.ptrvalue)) return;
  }
  ValNodeAddPointer (list, 0, (Pointer) sip);
}

static Boolean WriteDbsourceID (
  SeqIdPtr sip,
  CharPtr str
)

{
  DbtagPtr      db;
  CharPtr       dt;
  Int4          gi;
  ObjectIdPtr   oip;
  CharPtr       pfx;
  PDBSeqIdPtr   psip = NULL;
  CharPtr       prefix;
  Boolean       rsult = FALSE;
  CharPtr       sfx;
  CharPtr       suffix;
  Char          tmp [32];
  TextSeqIdPtr  tsip = NULL;

  if (sip == NULL || str == NULL) return FALSE;
  *str = '\0';
  switch (sip->choice) {
    case SEQID_LOCAL :
      oip = (ObjectIdPtr) sip->data.ptrvalue;
      if (oip == NULL) return FALSE;
      if (! StringHasNoText (oip->str)) {
        StringCat (str, oip->str);
        return TRUE;
      } else if (oip->id > 0) {
        sprintf (tmp, "%ld", (long) oip->id);
        StringCat (str, tmp);
        return TRUE;
      }
      return FALSE;
    case SEQID_GI :
      gi = (Int4) sip->data.intvalue;
      if (gi == 0) return FALSE;
      sprintf (tmp, "gi: %ld", (long) gi);
      StringCat (str, tmp);
      return TRUE;
    case SEQID_GENERAL :
      db = (DbtagPtr) sip->data.ptrvalue;
      if (db == NULL) return FALSE;
      /* !!! still need to implement this !!! */
      return FALSE;
    case SEQID_GENBANK :
    case SEQID_EMBL :
    case SEQID_DDBJ :
    case SEQID_OTHER :
    case SEQID_PIR :
    case SEQID_SWISSPROT :
    case SEQID_PRF :
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;
      if (tsip == NULL) return FALSE;
      break;
    case SEQID_PDB :
      psip = (PDBSeqIdPtr) sip->data.ptrvalue;
      if (psip == NULL) return FALSE;
      break;
    default :
      break;
  }
  prefix = " ";
  suffix = NULL;
  switch (sip->choice) {
    case SEQID_EMBL :
      StringCat (str, "embl ");
      suffix = ",";
      break;
    case SEQID_OTHER :
      StringCat (str, "REFSEQ: ");
      break;
    case SEQID_SWISSPROT :
      StringCat (str, "swissprot: ");
      suffix = ",";
      break;
    case SEQID_PIR :
      StringCat (str, "pir: ");
      break;
    case SEQID_PRF :
      StringCat (str, "prf: ");
      break;
    case SEQID_PDB :
      StringCat (str, "pdb: ");
      suffix = ",";
      break;
    default :
      break;
  }
  pfx = NULL;
  sfx = NULL;
  if (tsip != NULL) {
    if (! StringHasNoText (tsip->name)) {
      StringCat (str, sfx);
      StringCat (str, pfx);
      StringCat (str, "locus ");
      StringCat (str, tsip->name);
      sfx = suffix;
      pfx = prefix;
      rsult = TRUE;
    }
    if (! StringHasNoText (tsip->accession)) {
      StringCat (str, sfx);
      StringCat (str, pfx);
      StringCat (str, "accession ");
      StringCat (str, tsip->accession);
      sfx = suffix;
      pfx = prefix;
      rsult = TRUE;
    }
    if (tsip->version > 0) {
      sprintf (tmp, ".%d", (int) tsip->version);
      StringCat (str, tmp);
      sfx = suffix;
      pfx = prefix;
    }
    if (! StringHasNoText (tsip->release)) {
      StringCat (str, pfx);
      StringCat (str, "release ");
      StringCat (str, tsip->release);
      sfx = suffix;
      pfx = prefix;
    }
    if (sip->choice == SEQID_SWISSPROT || sip->choice == SEQID_PIR || sip->choice == SEQID_PRF) {
      StringCat (str, ";");
    }
    return rsult;
  }
  if (psip != NULL) {
    if (! StringHasNoText (psip->mol)) {
      StringCat (str, "molecule ");
      StringCat (str, psip->mol);
      sfx = suffix;
      pfx = prefix;
      rsult = TRUE;
    }
    if (psip->chain > 0) {
      StringCat (str, sfx);
      StringCat (str, pfx);
      sprintf (tmp, "chain %d", (int) psip->chain);
      StringCat (str, tmp);
      sfx = suffix;
      pfx = prefix;
      rsult = TRUE;
    }
    if (psip->rel != NULL) {
      StringCat (str, sfx);
      StringCat (str, pfx);
      StringCat (str, "release ");
      dt = asn2gb_PrintDate (psip->rel);
      StringCat (str, dt);
      MemFree (dt);
      sfx = suffix;
      pfx = prefix;
      rsult = TRUE;
    }
    StringCat (str, ";");
    return rsult;
  }
  return rsult;
}

static void AddSPBlock (
  BioseqPtr bsp
)

{
  CharPtr            acc;
  DbtagPtr           db;
  SeqMgrDescContext  dcontext;
  Boolean            first;
  ObjectIdPtr        oip;
  SeqDescrPtr        sdp;
  SeqIdPtr           sid;
  SPBlockPtr         spb;
  CharPtr            string;
  TextSeqIdPtr       tid;
  ValNodePtr         vnp;

  if (bsp == NULL) return;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_sp, &dcontext);
  if (sdp == NULL) return;
  spb = (SPBlockPtr) sdp->data.ptrvalue;
  if (spb == NULL) return;

  NewContLine ();
  if (spb->_class == 1) {
    ff_AddString ("class: standard.");
    NewContLine ();
  } else if (spb->_class == 2) {
    ff_AddString ("class: preliminary.");
    NewContLine ();
  }

  if (spb->extra_acc) {
    ff_AddString("extra accessions:");
    for (vnp = spb->extra_acc; vnp != NULL; vnp = vnp->next) {
      ff_AddString ((CharPtr) vnp->data.ptrvalue);
      ff_AddString (",");
    }
  }

  if (spb->imeth) {
    ff_AddString ("seq starts with Met");
  }

  if (spb->plasnm != NULL) {
    ff_AddString("plasmid:");
    for (vnp = spb->plasnm; vnp != NULL; vnp = vnp->next) {
      ff_AddString ((CharPtr) vnp->data.ptrvalue);
      ff_AddString (",");
    }
  }

  if (spb->created) {
    string = PrintDate (spb->created);
    ff_AddString ("created: ");
    ff_AddString (string);
    MemFree (string);
  }

  if (spb->sequpd) {
    string = PrintDate (spb->sequpd);
    ff_AddString ("sequence updated: ");
    ff_AddString (string);
    MemFree (string);
  }

  if (spb->annotupd) {
    string = PrintDate (spb->annotupd);
    ff_AddString ("annotation updated: ");
    ff_AddString (string);
    MemFree (string);
  }

  if (spb->seqref) {
    ff_AddString ("xrefs: ");
    first = TRUE;
    for (sid = spb->seqref; sid != NULL; sid = sid->next) {
      acc = NULL;
      if (first == FALSE) {
        ff_AddString (", ");
      }
      first = FALSE;
      if (sid->choice == SEQID_GI) {
        ff_AddString ("gi: ");
        acc = (CharPtr) MemNew (10);
        sprintf (acc, "%ld", (long) sid->data.intvalue);
      } else {
        tid = (TextSeqIdPtr) sid->data.ptrvalue;
        if (tid != NULL) {
          acc = tid->accession;
        }
      }
      if (acc != NULL) {
        switch (sid->choice) {
          case SEQID_GENBANK:
            ff_AddString ("genbank accession ");
            break; 
          case SEQID_EMBL:
            ff_AddString ("embl accession ");
            break; 
          case SEQID_PIR:
            ff_AddString ("pir locus ");
            break; 
          case SEQID_SWISSPROT:
            ff_AddString ("swissprot accession ");
            break; 
          case SEQID_DDBJ:
            ff_AddString ("ddbj accession ");
            break; 
          case SEQID_PRF:
            ff_AddString ("prf accession ");
            break; 
          case SEQID_GI:
            ff_AddString ("gi: ");
            break; 
          case SEQID_TPG:
            ff_AddString ("genbank third party accession ");
            break; 
          case SEQID_TPE:
            ff_AddString ("embl third party accession ");
            break; 
          case SEQID_TPD:
            ff_AddString ("ddbj third party accession ");
            break; 
          default:
            acc = NULL;
            break; 
        }
        ff_AddString (acc);
      }
    }
  }

  first = TRUE;
  for (vnp = spb->dbref; vnp != NULL; vnp = vnp->next) {
    db = (DbtagPtr) vnp->data.ptrvalue;
    if (db == NULL) continue;
    oip = db->tag;
    if (oip == NULL) continue;
    if (first) {
      NewContLine ();
      ff_AddString ("xrefs (non-sequence databases): ");
      first = FALSE;
    } else {
      ff_AddString (", ");
    }
    ff_AddString (db->db);
    if (oip->str != NULL) {
      ff_AddString (" ");        
      ff_AddString (oip->str);
    } else if (oip->id > 0) {
      ff_AddString (" ");
      ff_AddInteger ("%ld", (long) oip->id);
    }
  }
}

static void AddPIRBlock (
  BioseqPtr bsp
)

{
  CharPtr            acc;
  SeqMgrDescContext  dcontext;
  Boolean            first;
  CharPtr            prefix = NULL;
  SeqDescrPtr        sdp;
  SeqIdPtr           sid;
  PirBlockPtr        pbp;
  TextSeqIdPtr       tid;

  if (bsp == NULL) return;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pir, &dcontext);
  if (sdp == NULL) return;
  pbp = (PirBlockPtr) sdp->data.ptrvalue;
  if (pbp == NULL) return;

  if (pbp->host != NULL) {
    NewContLine ();
    gb_AddString ("host:", pbp->host, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->source != NULL) {
    ff_AddString (prefix);
    NewContLine ();
    gb_AddString ("source: ", pbp->source, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->summary != NULL) {
    ff_AddString (prefix);
    NewContLine ();
    gb_AddString ("summary: ", pbp->summary, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->genetic != NULL) {
    ff_AddString (prefix);
    NewContLine ();
    gb_AddString ("genetic: ", pbp->genetic, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->includes != NULL) {
    ff_AddString (prefix);
    NewContLine ();
    gb_AddString ("includes: ", pbp->includes, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->placement != NULL) {
    ff_AddString (prefix);
    NewContLine ();
    gb_AddString ("placement: ", pbp->placement, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->superfamily != NULL) {
    ff_AddString (prefix);
    NewContLine ();
    gb_AddString ("superfamily: ", pbp->superfamily, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->cross_reference != NULL) {
    ff_AddString (prefix);
    NewContLine ();
    gb_AddString ("xref: ", pbp->cross_reference, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->date != NULL) {
    ff_AddString (prefix);
    NewContLine ();
    gb_AddString ("PIR dates: ", pbp->date, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }

  if (pbp->had_punct) {
    ff_AddString (prefix);
    NewContLine ();
    ff_AddString ("punctuation in sequence");
    prefix = ";";
  }

  if (pbp->seqref) {
    ff_AddString (prefix);
    NewContLine ();
    ff_AddString ("xrefs: ");
    first = TRUE;
    for (sid = pbp->seqref; sid != NULL; sid = sid->next) {
      acc = NULL;
      if (first == FALSE) {
        ff_AddString (", ");
      }
      first = FALSE;
      if (sid->choice == SEQID_GI) {
        ff_AddString ("gi: ");
        acc = (CharPtr) MemNew (10);
        sprintf (acc, "%ld", (long) sid->data.intvalue);
      } else {
        tid = (TextSeqIdPtr) sid->data.ptrvalue;
        if (tid != NULL) {
          acc = tid->accession;
        }
      }
      if (acc != NULL) {
        switch (sid->choice) {
          case SEQID_GENBANK:
            ff_AddString ("genbank ");
            break; 
          case SEQID_EMBL:
            ff_AddString ("embl ");
            break; 
          case SEQID_PIR:
            ff_AddString ("pir ");
            break; 
          case SEQID_SWISSPROT:
            ff_AddString ("swissprot ");
            break; 
          case SEQID_DDBJ:
            ff_AddString ("ddbj ");
            break; 
          case SEQID_PRF:
            ff_AddString ("prf ");
            break; 
          case SEQID_GI:
            ff_AddString ("gi: ");
            break; 
          default:
            acc = NULL;
            break; 
        }
        ff_AddString (acc);
      }
    }
  }
  ff_AddString (".");
}

static void AddPRFBlock (
  BioseqPtr bsp
)

{
  SeqMgrDescContext  dcontext;
  PrfExtSrcPtr       extra;
  CharPtr            prefix = NULL;
  SeqDescrPtr        sdp;
  PrfBlockPtr        prf;

  if (bsp == NULL) return;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_prf, &dcontext);
  if (sdp == NULL) return;
  prf = (PrfBlockPtr) sdp->data.ptrvalue;
  if (prf == NULL) return;

  extra = prf->extra_src;
  if (extra != NULL) {

    if (extra->host != NULL) {
      NewContLine ();
      gb_AddString ("host:", extra->host, NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = ";";
    }

    if (extra->part != NULL) {
      ff_AddString (prefix);
      NewContLine ();
      gb_AddString ("part: ", extra->part, NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = ";";
    }
    if (extra->state != NULL) {
      ff_AddString (prefix);
      NewContLine ();
      gb_AddString ("state: ", extra->state, NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = ";";
    }
    if (extra->strain != NULL) {
      ff_AddString (prefix);
      NewContLine ();
      gb_AddString ("strain: ", extra->strain, NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = ";";
    }
    if (extra->taxon != NULL) {
      ff_AddString (prefix);
      NewContLine ();
      gb_AddString ("taxonomy: ", extra->taxon, NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = ";";
    }

    ff_AddString (".");
  }
}

static void AddPDBBlock (
  BioseqPtr bsp
)

{
  SeqMgrDescContext  dcontext;
  CharPtr            dt;
  CharPtr            prefix = NULL;
  SeqDescrPtr        sdp;
  PdbBlockPtr        pdb;
  PdbRepPtr          replace;
  CharPtr            str;
  ValNodePtr         vnp;

  if (bsp == NULL) return;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pdb, &dcontext);
  if (sdp == NULL) return;
  pdb = (PdbBlockPtr) sdp->data.ptrvalue;
  if (pdb == NULL) return;

  if (pdb->deposition != NULL) {
    NewContLine ();
    dt = asn2gb_PrintDate (pdb->deposition);
    gb_AddString ("deposition: ", dt, NULL, FALSE, TRUE, TILDE_IGNORE);
    MemFree (dt);
    prefix = ";";
  }
  if (pdb->pdbclass != NULL) {
    ff_AddString (prefix);
    NewContLine ();
    gb_AddString ("class: ", pdb->pdbclass, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }
  if (pdb->source != NULL) {
    ff_AddString (prefix);
    NewContLine ();
    gb_AddString (NULL, "source: ", NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = NULL;
    for (vnp = pdb->source; vnp != NULL; vnp = vnp->next) {
      str = (CharPtr) vnp->data.ptrvalue;
      if (StringHasNoText (str)) continue;
      gb_AddString (prefix, str, NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = ", ";
    }
    prefix = ";";
  }
  if (pdb->exp_method != NULL) {
    ff_AddString (prefix);
    NewContLine ();
    gb_AddString ("Non X-ray method: ", pdb->exp_method, NULL, FALSE, TRUE, TILDE_IGNORE);
    prefix = ";";
  }
  replace = pdb->replace;
  if (replace != NULL) {
    if (replace->ids != NULL) {
      ff_AddString (prefix);
      NewContLine ();
      gb_AddString (NULL, "ids replaced: ", NULL, FALSE, TRUE, TILDE_IGNORE);
      prefix = NULL;
      for (vnp = replace->ids; vnp != NULL; vnp = vnp->next) {
        str = (CharPtr) vnp->data.ptrvalue;
        if (StringHasNoText (str)) continue;
        gb_AddString (prefix, str, NULL, FALSE, TRUE, TILDE_IGNORE);
        prefix = ", ";
      }
      prefix = ";";
    }
    if (replace->date != NULL) {
      ff_AddString (prefix);
      NewContLine ();
      dt = asn2gb_PrintDate (replace->date);
      gb_AddString ("replacement date: ", dt, NULL, FALSE, TRUE, TILDE_IGNORE);
      MemFree (dt);
      prefix = ";";
    }
  }

  ff_AddString (".");
}

static void AddDbsourceBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr  bbp;
  BioseqPtr     bsp;
  SeqFeatPtr    cds;
  DbtagPtr      db;
  Boolean       first = TRUE;
  SeqIdPtr      id;
  ValNodePtr    list = NULL;
  BioseqPtr     nuc;
  SeqIdPtr      sip;
  SeqLocPtr     slp;
  Char          str [256];
  Boolean       unknown = TRUE;
  ValNodePtr    vnp;

  if (awp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  bbp = Asn2gbAddBlock (awp, DBSOURCE_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  gb_StartPrint (awp->format, 0, 12, "DBSOURCE", 13, 5, 5, NULL, TRUE);

  sip = SeqIdSelect (bsp->id, dbsource_fasta_order, NUM_SEQID);

  if (sip != NULL) {

    switch (sip->choice) {
      case SEQID_PIR :
      case SEQID_SWISSPROT :
      case SEQID_PRF :
      case SEQID_PDB :
        if (WriteDbsourceID (sip, str)) {
          www_dbsource (str, TRUE, sip->choice);
          unknown = FALSE;
        }
        break;
      case SEQID_GENERAL :
        db = sip->data.ptrvalue;
        if (db == NULL) {
          break;
        }
        if (StringNCmp (db->db, "PIDe", 4) != 0 &&
            StringNCmp (db->db, "PIDd", 4) != 0 &&
            StringNCmp (db->db, "PID", 3) != 0) {
          break;
        }
        /* if (ChoicePID) found, continue on to next set of cases */
      case SEQID_EMBL :
      case SEQID_GENBANK :
      case SEQID_DDBJ :
      case SEQID_GIBBSQ :
      case SEQID_GIBBMT :
      case SEQID_OTHER :
      case SEQID_TPG :
      case SEQID_TPE :
      case SEQID_TPD :
      case SEQID_GI :
      case SEQID_GIIM :
        cds = SeqMgrGetCDSgivenProduct (bsp, NULL);
        if (cds != NULL) {
          nuc = BioseqFindFromSeqLoc (cds->location);
          if (nuc != NULL) {
            slp = SeqLocFindNext (cds->location, NULL);
            while (slp != NULL) {
              sip = SeqLocId (slp);
              AddToUniqueSipList (&list, sip);
              slp = SeqLocFindNext (cds->location, slp);
            }
            for (vnp = list; vnp != NULL; vnp = vnp->next) {
              id = (SeqIdPtr) vnp->data.ptrvalue;
              nuc = BioseqFindCore (id);
              sip = NULL;
              if (nuc != NULL) {
                sip = SeqIdSelect (nuc->id, dbsource_fasta_order, NUM_SEQID);
              } else if (id != NULL && id->choice == SEQID_GI) {
                sip = GetSeqIdForGI (id->data.intvalue);
              }
              if (sip == NULL) {
                sip = id;
              }
              if (sip != NULL) {
                if (WriteDbsourceID (sip, str)) {
                  if (! first) {
                    NewContLine ();
                    TabToColumn (13);
                  }
                  www_dbsource (str, TRUE, sip->choice);
                  first = FALSE;
                  unknown = FALSE;
                }
              }
            }
            ValNodeFree (list);
          }
        } else {
          if (WriteDbsourceID (sip, str)) {
            www_dbsource (str, TRUE, sip->choice);
            unknown = FALSE;
          }
        }
        break;
      default :
        break;
    }

    switch (sip->choice) {
      case SEQID_PIR :
        AddPIRBlock (bsp);
        break;
      case SEQID_SWISSPROT :
        AddSPBlock (bsp);
        break;
      case SEQID_PRF :
        AddPRFBlock (bsp);
        break;
      case SEQID_PDB :
        AddPDBBlock (bsp);
        break;
      default :
        break;
    }
  }

  if (unknown) {
    gb_AddString (NULL, "UNKNOWN", NULL, FALSE, FALSE, TILDE_TO_SPACES);
  }

  ff_EndPrint();

  bbp->string = ff_print_string_mem (gb_MergeString (FALSE));
}

static void AddDateBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  Char               date [40];
  SeqMgrDescContext  dcontext;
  DatePtr            dp;
  SeqDescrPtr        sdp;

  if (awp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  bbp = Asn2gbAddBlock (awp, DATE_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  date [0] = '\0';

  dp = NULL;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_create_date, &dcontext);
  if (sdp != NULL) {
    dp = (DatePtr) sdp->data.ptrvalue;
  }
  if (dp != NULL) {
    DateToGB (date, dp, FALSE);
  }
  if (StringHasNoText (date)) {
    StringCpy (date, "01-JAN-1900");
  }

  PrintXX ();
  ff_StartPrint (5, 5, ASN2FF_EMBL_MAX, "DT");
  ff_AddString (date);

  bbp->string = gb_MergeString (TRUE);

  bbp = Asn2gbAddBlock (awp, DATE_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_update_date, &dcontext);
  if (sdp != NULL) {
    dp = (DatePtr) sdp->data.ptrvalue;
  }
  if (dp != NULL) {
    DateToGB (date, dp, FALSE);
  }

  ff_StartPrint (5, 5, ASN2FF_EMBL_MAX, "DT");
  ff_AddString (date);

  bbp->string = gb_MergeString (TRUE);
}

#define TOTAL_ESTKW 11
#define TOTAL_STSKW 5
#define TOTAL_GSSKW 2

static CharPtr EST_kw_array[ TOTAL_ESTKW] = {
  "EST", "EST PROTO((expressed sequence tag)", "expressed sequence tag",
  "EST (expressed sequence tag)", "EST(expressed sequence tag)",
  "partial cDNA sequence", "transcribed sequence fragment", "TSR",
  "putatively transcribed partial sequence", "UK putts"
};

static CharPtr GSS_kw_array [TOTAL_GSSKW] = {
  "GSS", "trapped exon"
};
static CharPtr STS_kw_array[TOTAL_STSKW] = {
  "STS", "STS(sequence tagged site)", "STS (sequence tagged site)",
  "STS sequence", "sequence tagged site"
};

static Int2 MatchArrayString (
  CharPtr array_string [],
  Int2 totalstr,
  CharPtr text
)

{
  Int2 i;

  for (i = 0; i < totalstr && text != NULL; i++) {
    if (StringCmp (array_string [i], text) == 0) {
      return (i);
    }
  }

  return (-1);
}

static Boolean CheckSpecialKeyword (
  Boolean is_est,
  Boolean is_sts,
  Boolean is_gss,
  CharPtr kwd
)

{
  if (kwd == NULL) return FALSE;

  if (is_est) {
    if (MatchArrayString (STS_kw_array, TOTAL_STSKW, kwd) != -1) return FALSE;
    if (MatchArrayString (GSS_kw_array, TOTAL_GSSKW, kwd) != -1) return FALSE;
  }

  if (is_sts) {
    if (MatchArrayString (EST_kw_array, TOTAL_ESTKW, kwd) != -1) return FALSE;
    if (MatchArrayString (GSS_kw_array, TOTAL_GSSKW, kwd) != -1) return FALSE;
  }

  if (is_gss) {
    if (MatchArrayString (STS_kw_array, TOTAL_STSKW, kwd) != -1) return FALSE;
    if (MatchArrayString (EST_kw_array, TOTAL_ESTKW, kwd) != -1) return FALSE;
  }

  return TRUE;
}

static Boolean KeywordAlreadyInList (
  ValNodePtr head,
  CharPtr kwd
)

{
  ValNodePtr  vnp;

  for (vnp = head; vnp != NULL; vnp = vnp->next) {
    if (StringICmp ((CharPtr) vnp->data.ptrvalue, kwd) == 0) return TRUE;
  }

  return FALSE;
}

static void AddKeywordsBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  SeqMgrDescContext  dcontext;
  EMBLBlockPtr       ebp;
  GBBlockPtr         gbp;
  ValNodePtr         head = NULL;
  Boolean            is_est = FALSE;
  Boolean            is_gss = FALSE;
  Boolean            is_sts = FALSE;
  ValNodePtr         keywords;
  CharPtr            kwd;
  MolInfoPtr         mip;
  PirBlockPtr        pir;
  PrfBlockPtr        prf;
  SeqDescrPtr        sdp;
  SeqIdPtr           sip;
  SPBlockPtr         sp;
  CharPtr            str;
  ValNodePtr         vnp;

  if (awp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  bbp = (BaseBlockPtr) Asn2gbAddBlock (awp, KEYWORDS_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_TPG || sip->choice == SEQID_TPE || sip->choice == SEQID_TPD) {
      ValNodeCopyStr (&head, 0, "THIRD PARTY ANNOTATION");
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp != NULL) {
    bbp->entityID = dcontext.entityID;
    bbp->itemID = dcontext.itemID;
    bbp->itemtype = OBJ_SEQDESC;

    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      switch (mip->tech) {
        case MI_TECH_htgs_1 :
          ValNodeCopyStr (&head, 0, "HTG; HTGS_PHASE1");
          break;
        case MI_TECH_htgs_2 :
          ValNodeCopyStr (&head, 0, "HTG; HTGS_PHASE2");
          break;
        case MI_TECH_htgs_3 :
          ValNodeCopyStr (&head, 0, "HTG");
          break;
        case MI_TECH_est :
          is_est = TRUE;
          ValNodeCopyStr (&head, 0, "EST");
          break;
        case MI_TECH_sts :
          is_sts = TRUE;
          ValNodeCopyStr (&head, 0, "STS");
          break;
        case MI_TECH_survey :
          is_gss = TRUE;
          ValNodeCopyStr (&head, 0, "GSS");
          break;
        case MI_TECH_fli_cdna :
          ValNodeCopyStr (&head, 0, "FLI_CDNA");
          break;
        case MI_TECH_htgs_0 :
          ValNodeCopyStr (&head, 0, "HTG; HTGS_PHASE0");
          break;
        case MI_TECH_htc :
          ValNodeCopyStr (&head, 0, "HTC");
          break;
        default :
          break;
      }
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, 0, &dcontext);
  while (sdp != NULL) {

    keywords = NULL;

    switch (dcontext.seqdesctype) {
      case Seq_descr_genbank :
        gbp = (GBBlockPtr) sdp->data.ptrvalue;
        if (gbp != NULL) {
          keywords = gbp->keywords;
        }
        break;
      case Seq_descr_embl :
        ebp = (EMBLBlockPtr) sdp->data.ptrvalue;
        if (ebp != NULL) {
          keywords = ebp->keywords;
        }
        break;
      case Seq_descr_pir :
        pir = (PirBlockPtr) sdp->data.ptrvalue;
        if (pir != NULL) {
          keywords = pir->keywords;
        }
        break;
      case Seq_descr_prf :
        prf = (PrfBlockPtr) sdp->data.ptrvalue;
        if (prf != NULL) {
          keywords = prf->keywords;
        }
        break;
      case Seq_descr_sp :
        sp = (SPBlockPtr) sdp->data.ptrvalue;
        if (sp != NULL) {
          keywords = sp->keywords;
        }
        break;
      default :
        break;
    }

    if (keywords != NULL) {
      bbp->entityID = dcontext.entityID;
      bbp->itemID = dcontext.itemID;
      bbp->itemtype = OBJ_SEQDESC;
    }

    for (vnp = keywords; vnp != NULL; vnp = vnp->next) {
      kwd = (CharPtr) vnp->data.ptrvalue;
      if (CheckSpecialKeyword (is_est, is_sts, is_gss, kwd)) {
        if (! KeywordAlreadyInList (head, kwd)) {
          if (head != NULL) {
            ValNodeCopyStr (&head, 0, "; ");
          }
          ValNodeCopyStr (&head, 0, kwd);
        }
      }
    }

    sdp = SeqMgrGetNextDescriptor (bsp, sdp, 0, &dcontext);
  }

  gb_StartPrint (awp->format, 0, 12, "KEYWORDS", 13, 5, 5, "KW", TRUE);

  str = MergeValNodeStrings (head);

  /* if no keywords were found, period will still be added by this call */

  gb_AddString (NULL, str, NULL, TRUE, FALSE, TILDE_TO_SPACES);

  MemFree (str);
  ValNodeFreeData (head);

  bbp->string = gb_MergeString (TRUE);
}

static void AddSegmentBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr  bbp;
  Char          buf [32];

  if (awp == NULL) return;

  if (awp->seg < 1 || awp->numsegs < 1) return;

  bbp = Asn2gbAddBlock (awp, SEGMENT_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  sprintf (buf, "%d of %ld", (int) awp->seg, (long) awp->numsegs);

  gb_StartPrint (awp->format, 0, 12, "SEGMENT", 13, 5, 5, "XX", FALSE);

  gb_AddString (NULL, buf, NULL, FALSE, TRUE, TILDE_TO_SPACES);

  bbp->string = gb_MergeString (TRUE);
}

static void AddSourceBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  GBBlockPtr         gbp;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;

  if (awp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  bbp = Asn2gbAddBlock (awp, SOURCE_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_genbank, &dcontext);
  if (sdp != NULL) {
    gbp = (GBBlockPtr) sdp->data.ptrvalue;
    if (gbp != NULL && (! StringHasNoText (gbp->source))) {
      bbp->entityID = dcontext.entityID;
      bbp->itemID = dcontext.itemID;
      bbp->itemtype = OBJ_SEQDESC;
      return;
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    bbp->entityID = dcontext.entityID;
    bbp->itemID = dcontext.itemID;
    bbp->itemtype = OBJ_SEQDESC;
  } else {
    sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_BIOSRC, 0, &fcontext);
    if (sfp != NULL) {
      bbp->entityID = fcontext.entityID;
      bbp->itemID = fcontext.itemID;
      bbp->itemtype = OBJ_SEQFEAT;
    }
  }
}

static void AddOrganismBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  SeqMgrDescContext  dcontext;
  BioseqPtr          dna;
  SeqMgrFeatContext  fcontext;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;

  if (awp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  bbp = Asn2gbAddBlock (awp, ORGANISM_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    bbp->entityID = dcontext.entityID;
    bbp->itemID = dcontext.itemID;
    bbp->itemtype = OBJ_SEQDESC;
  } else {
    sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_BIOSRC, 0, &fcontext);
    if (sfp != NULL) {
      bbp->entityID = fcontext.entityID;
      bbp->itemID = fcontext.itemID;
      bbp->itemtype = OBJ_SEQFEAT;
    } else if (ISA_aa (bsp->mol)) {

      /* if protein with no sources, get sources applicable to DNA location of CDS */

      cds = SeqMgrGetCDSgivenProduct (bsp, &fcontext);
      if (cds != NULL) {
        sfp = SeqMgrGetOverlappingSource (cds->location, &fcontext);
        if (sfp != NULL) {
          bbp->entityID = fcontext.entityID;
          bbp->itemID = fcontext.itemID;
          bbp->itemtype = OBJ_SEQFEAT;
        } else {
          dna = BioseqFindFromSeqLoc (cds->location);
          if (dna != NULL) {
            sdp = SeqMgrGetNextDescriptor (dna, NULL, Seq_descr_source, &dcontext);
            if (sdp != NULL) {
              bbp->entityID = dcontext.entityID;
              bbp->itemID = dcontext.itemID;
              bbp->itemtype = OBJ_SEQDESC;
            }
          }
        }
      }
    }
  }
}

static RefBlockPtr AddPub (
  Asn2gbWorkPtr awp,
  ValNodePtr PNTR head,
  PubdescPtr pdp
)

{
  Char            buf [121];
  CitArtPtr       cap;
  CitBookPtr      cbp;
  CitGenPtr       cgp;
  CitJourPtr      cjp;
  CitPatPtr       cpp;
  CitSubPtr       csp;
  DatePtr         dp = NULL;
  Boolean         justuids = TRUE;
  ImprintPtr      imp = NULL;
  IntRefBlockPtr  irp;
  RefBlockPtr     rbp;
  ValNodePtr      vnp;

  if (awp == NULL || head == NULL || pdp == NULL) return NULL;

  rbp = (RefBlockPtr) MemNew (sizeof (IntRefBlock));
  if (rbp == NULL) return NULL;
  rbp->blocktype = REFERENCE_BLOCK;
  rbp->section = awp->currsection;

  rbp->serial = INT2_MAX;

  for (vnp = pdp->pub; vnp != NULL; vnp = vnp->next) {
    switch (vnp->choice) {
      case PUB_Gen :
        /* may be unpublished, or may be serial number of swiss-prot reference */
        cgp = (CitGenPtr) vnp->data.ptrvalue;
        if (cgp != NULL) {
          if (StringNICmp ("BackBone id_pub", cgp->cit, 15) != 0) {
            rbp->category = REF_CAT_UNP;
            dp = cgp->date;
            if (cgp->serial_number > 0) {
              rbp->serial = cgp->serial_number;
            }
            if (cgp->cit != NULL) {
              if (StringNICmp ("unpublished", cgp->cit, 11) != 0 &&
                  StringNICmp ("submitted", cgp->cit, 8) != 0 &&
                  StringNICmp ("to be published", cgp->cit, 15) != 0 &&
                  StringNICmp ("in press", cgp->cit, 8) != 0 &&
                  StringStr (cgp->cit, "Journal") == NULL) {
                if (cgp->serial_number == 0) {
                  MemFree (rbp);
                  return NULL;
                }
              }
            } else if (cgp->journal == NULL || cgp->date == NULL) {
              if (cgp->serial_number == 0) {
                MemFree (rbp);
                return NULL;
              }
            }
          }
        }
        break;
      case PUB_Sub :
        rbp->category = REF_CAT_SUB;
        csp = (CitSubPtr) vnp->data.ptrvalue;
        if (csp != NULL) {
          imp = csp->imp;
          if (imp != NULL) {
            dp = imp->date;
          }
          if (csp->date != NULL) {
            dp = csp->date;
          }
        }
        break;
      case PUB_Article:
        cap = (CitArtPtr) vnp->data.ptrvalue;
        if (cap != NULL) {
          switch (cap->from) {
            case 1:
              cjp = (CitJourPtr) cap->fromptr;
              if (cjp != NULL) {
                imp = (ImprintPtr) cjp->imp;
                if (imp != NULL) {
                  dp = imp->date;
                }
              }
              break;
            case 2:
              cbp = (CitBookPtr) cap->fromptr;
              if (cbp != NULL) {
                imp = (ImprintPtr) cbp->imp;
                if (imp != NULL) {
                  dp = imp->date;
                }
              }
              break;
            case 3:
              cbp = (CitBookPtr) cap->fromptr;
              if (cbp != NULL) {
                imp = (ImprintPtr) cbp->imp;
                if (imp != NULL) {
                  dp = imp->date;
                }
              }
              break;
            default:
              break;
          }
        }
        break;
      case PUB_Book:
        cbp = (CitBookPtr) vnp->data.ptrvalue;
        if (cbp != NULL) {
          imp = (ImprintPtr) cbp->imp;
          if (imp != NULL) {
            dp = imp->date;
          }
        }
        break;
      case PUB_Proc:
        cbp = (CitBookPtr) vnp->data.ptrvalue;
        if (cbp != NULL) {
          imp = (ImprintPtr) cbp->imp;
          if (imp != NULL) {
            dp = imp->date;
          }
        }
        break;
      case PUB_Patent :
        rbp->category = REF_CAT_PUB;
        cpp = (CitPatPtr) vnp->data.ptrvalue;
        if (cpp != NULL) {
          if (cpp->date_issue != NULL) {
            dp = (DatePtr) cpp->date_issue;
          } else if (cpp->app_date != NULL) {
            dp = (DatePtr) cpp->app_date;
          }
        }
        break;
      case PUB_Man:
        cbp = (CitBookPtr) vnp->data.ptrvalue;
        if (cbp != NULL) {
          imp = (ImprintPtr) cbp->imp;
          if (imp != NULL) {
            dp = imp->date;
          }
        }
        break;
      case PUB_Muid :
        rbp->muid = vnp->data.intvalue;
        rbp->category = REF_CAT_PUB;
        break;
      case PUB_PMid :
        rbp->pmid = vnp->data.intvalue;
        rbp->category = REF_CAT_PUB;
        break;
      default :
        break;
    }
    if (vnp->choice != PUB_Muid && vnp->choice != PUB_PMid) {
      justuids = FALSE;
    }
  }

  /* check for submitted vs. in-press */

  if (imp != NULL) {
    rbp->category = REF_CAT_PUB;
    switch (imp->prepub) {
      case 1 :
        rbp->category = REF_CAT_UNP;
        break;
      case 2 :
        rbp->category = REF_CAT_PUB;
        break;
      default :
        break;
    }
  }

  /* check for sites reftype */

  if (pdp->reftype != 0) {
    rbp->sites = pdp->reftype;
  }

  if (rbp->muid == 0 && rbp->pmid == 0) {
    vnp = pdp->pub;

    /* skip over just serial number */

    if (vnp != NULL && vnp->choice == PUB_Gen && vnp->next != NULL) {
      cgp = (CitGenPtr) vnp->data.ptrvalue;
      if (cgp != NULL) {
        if (StringNICmp ("BackBone id_pub", cgp->cit, 15) != 0) {
          if (cgp->cit == NULL && cgp->journal == NULL && cgp->date == NULL && cgp->serial_number) {
            vnp = vnp->next;
          }
        }
      }
    }

    if (PubLabelUnique (vnp, buf, sizeof (buf) - 1, OM_LABEL_CONTENT, TRUE) > 0) {
      rbp->uniquestr = StringSaveNoNull (buf);
    }
  }

  irp = (IntRefBlockPtr) rbp;
  irp->date = DateDup (dp);
  irp->justuids = justuids;
  /* if (justuids) { */
    irp->fig = StringSaveNoNull (pdp->fig);
    irp->maploc = StringSaveNoNull (pdp->maploc);
    irp->poly_a = pdp->poly_a;
  /* } */

  /* if not rejected by now, link in */

  ValNodeAddPointer (head, 0, rbp);

  return rbp;
}

static int LIBCALLBACK SortReferences (
  VoidPtr ptr1,
  VoidPtr ptr2,
  Boolean serialFirst
)

{
  int             compare;
  IntRefBlockPtr  irp1;
  IntRefBlockPtr  irp2;
  RefBlockPtr     rbp1;
  RefBlockPtr     rbp2;
  Int2            status;
  ValNodePtr      vnp1;
  ValNodePtr      vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  rbp1 = (RefBlockPtr) vnp1->data.ptrvalue;
  rbp2 = (RefBlockPtr) vnp2->data.ptrvalue;
  if (rbp1 == NULL || rbp2 == NULL) return 0;

  if (serialFirst) {
    if (rbp1->serial > rbp2->serial) {
      return 1;
    } else if (rbp1->serial < rbp2->serial) {
      return -1;
    }
  }

  /* usual first sort by published, unpublished, and cit-subs */

  if (rbp1->category > rbp2->category) {
    return 1;
  } else if (rbp1->category < rbp2->category) {
    return -1;
  }

  /* within class, sort by date, older publications first */

  irp1 = (IntRefBlockPtr) rbp1;
  irp2 = (IntRefBlockPtr) rbp2;
  status = DateMatch (irp1->date, irp2->date, FALSE);
  if (status == 1 || status == -1) return status;

  /* if dates (e.g., years) match, try to distinguish by uids */

  if (rbp1->pmid != 0 && rbp2->pmid != 0) {
    if (rbp1->pmid > rbp2->pmid) {
      return 1;
    } else if (rbp1->pmid < rbp2->pmid) {
      return -1;
    }
  }
  if (rbp1->muid != 0 && rbp2->muid != 0) {
    if (rbp1->muid > rbp2->muid) {
      return 1;
    } else if (rbp1->muid < rbp2->muid) {
      return -1;
    }
  }

  /* if same uid, one with just uids goes last to be excised but remembered */

  if ((rbp1->pmid != 0 && rbp2->pmid != 0) || (rbp1->muid != 0 && rbp2->muid != 0)) {
    if (irp1->justuids && (! irp2->justuids)) {
      return 1;
    } else if ((! irp1->justuids) && irp2->justuids) {
      return -1;
    }
  }

  /* put sites after pubs that refer to all or a range of bases */

  if (rbp1->sites > 0) {
    return 1;
  } else if (rbp2->sites > 0) {
    return -1;
  }

  /* for publication features, sort in explore index order */

  if (irp1->index > irp2->index) {
    return 1;
  } else if (irp1->index < irp2->index) {
    return -1;
  }

  /* next use author string */

  if (irp1->authstr != NULL && irp2->authstr != NULL) {
    compare = StringICmp (irp1->authstr, irp2->authstr);
    if (compare > 0) {
      return 1;
    } else if (compare < 0) {
      return -1;
    }
  }

  /* use unique label string to determine sort order */

  if (rbp1->uniquestr != NULL && rbp2->uniquestr != NULL) {
    compare = StringICmp (rbp1->uniquestr, rbp2->uniquestr);
    if (compare > 0) {
      return 1;
    } else if (compare < 0) {
      return -1;
    }
  }

  /* last resort for equivalent publication descriptors, sort in itemID order */

  if (rbp1->itemtype == OBJ_SEQDESC && rbp2->itemtype == OBJ_SEQDESC) {
    if (rbp1->itemID > rbp2->itemID) {
      return 1;
    } else if (rbp1->itemID < rbp2->itemID) {
      return -1;
    }
  }

  if (! serialFirst) {
    if (rbp1->serial > rbp2->serial) {
      return 1;
    } else if (rbp1->serial < rbp2->serial) {
      return -1;
    }
  }

  return 0;
}

static int LIBCALLBACK SortReferencesA (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  return SortReferences (ptr1, ptr2, FALSE);
}

static int LIBCALLBACK SortReferencesB (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  return SortReferences (ptr1, ptr2, TRUE);
}

static void GetRefsOnBioseq (
  Asn2gbWorkPtr awp,
  BioseqPtr target,
  BioseqPtr bsp,
  Int4 from,
  Int4 to
)

{
  AuthListPtr        alp;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  Int2               idx;
  IntRefBlockPtr     irp;
  Int4Ptr            ivals;
  Int2               numivals;
  Boolean            okay;
  PubdescPtr         pdp;
  RefBlockPtr        rbp;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;
  SeqInt             sint;
  Int4               start;
  Int4               stop;
  ValNode            vn;
  ValNodePtr         vnp;

  if (awp == NULL || target == NULL || bsp == NULL) return;

  /* full length loc for descriptors */

  sint.from = 0;
  sint.to = bsp->length - 1;
  sint.strand = Seq_strand_plus;
  sint.id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
  sint.if_from = NULL;
  sint.if_to = NULL;

  vn.choice = SEQLOC_INT;
  vn.data.ptrvalue = (Pointer) &sint;
  vn.next = NULL;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pub, &dcontext);
  while (sdp != NULL) {

    /* check if descriptor on part already added on segmented bioseq */

    okay = TRUE;
    for (vnp = awp->pubhead; vnp != NULL && okay; vnp = vnp->next) {
      rbp = (RefBlockPtr) vnp->data.ptrvalue;
      if (rbp != NULL) {
        if (rbp->entityID == dcontext.entityID &&
            rbp->itemID == dcontext.itemID &&
            rbp->itemtype == OBJ_SEQDESC) {
          okay = FALSE;
        }
      }
    }

    if (okay) {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
      rbp = AddPub (awp, &(awp->pubhead), pdp);
      if (rbp != NULL) {

        rbp->entityID = dcontext.entityID;
        rbp->itemID = dcontext.itemID;
        rbp->itemtype = OBJ_SEQDESC;

        irp = (IntRefBlockPtr) rbp;
        irp->loc = SeqLocMerge (target, &vn, NULL, FALSE, TRUE, FALSE);
        alp = GetAuthListPtr (pdp, NULL);
        if (alp != NULL) {
          irp->authstr = GetAuthorsString (awp->format, alp);
        }
        irp->index = 0;
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_pub, &dcontext);
  }

  SeqIdFree (sint.id);

  /* features are indexed on parent if segmented */

  bsp = awp->parent;

  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_PUB, 0, &fcontext);
  while (sfp != NULL) {
    ivals = fcontext.ivals;
    numivals = fcontext.numivals;
    if (ivals != NULL && numivals > 0) {

      idx = (numivals - 1) * 2;
      start = ivals [idx];
      stop = ivals [idx + 1];
      if (stop >= from && stop <= to) {

        /*
        start = ivals [0] + 1;
        stop = ivals [idx + 1] + 1;
        */
        pdp = (PubdescPtr) sfp->data.value.ptrvalue;
        rbp = AddPub (awp, &(awp->pubhead), pdp);
        if (rbp != NULL) {

          rbp->entityID = fcontext.entityID;
          rbp->itemID = fcontext.itemID;
          rbp->itemtype = OBJ_SEQFEAT;

          irp = (IntRefBlockPtr) rbp;
          irp->loc = SeqLocMerge (target, sfp->location, NULL, FALSE, TRUE, FALSE);
          alp = GetAuthListPtr (pdp, NULL);
          if (alp != NULL) {
            irp->authstr = GetAuthorsString (awp->format, alp);
          }
          irp->index = fcontext.index;
        }
      }
    }

    sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_PUB, 0, &fcontext);
  }
}

static Boolean LIBCALLBACK GetRefsOnSeg (
  SeqLocPtr slp,
  SeqMgrSegmentContextPtr context
)

{
  Asn2gbWorkPtr  awp;
  BioseqPtr      bsp = NULL;
  Uint2          entityID;
  Int4           from;
  SeqLocPtr      loc;
  SeqIdPtr       sip;
  Int4           to;

  if (slp == NULL || context == NULL) return FALSE;
  awp = (Asn2gbWorkPtr) context->userdata;

  from = context->cumOffset;
  to = from + context->to - context->from;

  sip = SeqLocId (slp);
  if (sip == NULL) {
    loc = SeqLocFindNext (slp, NULL);
    if (loc != NULL) {
      sip = SeqLocId (loc);
    }
  }
  if (sip == NULL) return TRUE;

  /* may remote fetch genome component if not already in memory */

  bsp = BioseqLockById (sip);

  if (bsp == NULL) return TRUE;

  entityID = ObjMgrGetEntityIDForPointer (bsp);

  if (entityID != awp->entityID) {

    /* if segment not packaged in record, may need to feature index it */

    if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
      SeqMgrIndexFeatures (entityID, NULL);
    }

    /* collect features indexed on the remote bioseq */

    from = 0;
    to = bsp->length - 1;
  }

  GetRefsOnBioseq (awp, awp->target, bsp, from, to);

  BioseqUnlock (bsp);

  return TRUE;
}

static Boolean AddReferenceBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr    ajp;
  AuthListPtr        alp;
  Asn2gbSectPtr      asp;
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  Boolean            combine;
  SeqMgrFeatContext  context;
  CitSubPtr          csp;
  BioseqPtr          dna;
  Boolean            excise;
  ValNodePtr         head = NULL;
  Int2               i;
  IntRefBlockPtr     irp;
  IntRefBlockPtr     lastirp;
  RefBlockPtr        lastrbp;
  ValNodePtr         next;
  Int2               numReferences;
  ValNodePtr         PNTR prev;
  RefBlockPtr        rbp;
  RefBlockPtr        PNTR referenceArray;
  SubmitBlockPtr     sbp;
  SeqLocPtr          slp;
  BioseqPtr          target;
  ValNodePtr         vnp;

  if (awp == NULL) return FALSE;
  ajp = awp->ajp;
  if (ajp == NULL) return FALSE;
  asp = awp->asp;
  if (asp == NULL) return FALSE;
  bsp = awp->bsp;
  if (bsp == NULL) return FALSE;

  /* collect publications on bioseq */

  awp->pubhead = NULL;
  GetRefsOnBioseq (awp, bsp, bsp, awp->from, awp->to);
  target = bsp;

  if (bsp->repr == Seq_repr_seg) {

    /* collect publications on remote segments in MASTER_STYLE */

    SeqMgrExploreSegments (bsp, (Pointer) awp, GetRefsOnSeg);
    target = awp->target;
  }

  if (awp->pubhead == NULL && ISA_aa (bsp->mol)) {

    /* if protein with no pubs, get pubs applicable to DNA location of CDS */

    cds = SeqMgrGetCDSgivenProduct (bsp, &context);
    if (cds != NULL) {
      dna = BioseqFindFromSeqLoc (cds->location);
      if (dna != NULL) {
        GetRefsOnBioseq (awp, dna, dna, context.left, context.right);
        target = dna;
      }
    }
  }

  head = awp->pubhead;
  awp->pubhead = NULL;

  if (head == NULL && awp->ssp == NULL) return FALSE;

  /* sort by pub/unpub/sites/sub, then date, finally existing serial */

  head = SortValNode (head, SortReferencesA);

  if (awp->ssp != NULL) {

    /* add seq-submit citation */

    rbp = (RefBlockPtr) MemNew (sizeof (IntRefBlock));
    if (rbp != NULL) {
      irp = (IntRefBlockPtr) rbp;

      rbp->blocktype = REFERENCE_BLOCK;
      rbp->section = awp->currsection;
      rbp->serial = INT2_MAX;
      rbp->category = REF_CAT_SUB;

      rbp->entityID = ajp->ajp.entityID;
      rbp->itemID = 1;
      rbp->itemtype = OBJ_SEQSUB_CIT;

      sbp = awp->ssp->sub;
      if (sbp != NULL) {
        csp = sbp->cit;
        if (csp != NULL) {
          alp = GetAuthListPtr (NULL, csp);
          if (alp != NULL) {
            irp->authstr = GetAuthorsString (awp->format, alp);
          }
        }
      }

      if (awp->citSubsFirst) {

        /* for DDBJ, add seq-submit citation to beginning of list */

        vnp = ValNodeNew (NULL);
        if (vnp != NULL) {
          vnp->choice = 0;
          vnp->data.ptrvalue = (VoidPtr) rbp;
          vnp->next = head;
          head = vnp;
        }

      } else {

        /* for GENBANK and EMBL add seq-submit citation to end of list */

        ValNodeAddPointer (&head, 0, rbp);
      }
    }
  }

  /* unique references, excise duplicates from list */

  prev = &(head);
  vnp = head;
  lastrbp = NULL;
  while (vnp != NULL) {
    excise = FALSE;
    combine = TRUE;
    next = vnp->next;
    rbp = (RefBlockPtr) vnp->data.ptrvalue;
    if (lastrbp != NULL) {
      lastirp = (IntRefBlockPtr) lastrbp;
      if (rbp != NULL) {
        irp = (IntRefBlockPtr) rbp;
        if (lastrbp->pmid != 0 && rbp->pmid != 0) {
          if (lastrbp->pmid == rbp->pmid) {
            excise = TRUE;
          }
        } else if (lastrbp->muid != 0 && rbp->muid != 0) {
          if (lastrbp->muid == rbp->muid) {
            excise = TRUE;
          }
        } else if (lastrbp->uniquestr != NULL && rbp->uniquestr != NULL) {
          if (StringICmp (lastrbp->uniquestr, rbp->uniquestr) == 0) {
            if (SeqLocCompare (irp->loc, lastirp->loc) == SLC_A_EQ_B) {
              if (StringICmp (irp->authstr, lastirp->authstr) == 0) {

                /* L76496.1 - removing duplicate submission pubs */
                excise = TRUE;
              }
            }
          }
        }
      }
    }
    if (rbp != NULL) {
      irp = (IntRefBlockPtr) rbp;
      if (irp->justuids) {
        /* do not allow justuids reference to appear by itself - S79174.1 */
        excise = TRUE;
        /* justuids should still combine, even if no authors - S67070.1 */
      } else if (StringHasNoText (irp->authstr)) {
        /* do not allow no author reference to appear by itself - U07000.1 */
        excise = TRUE;
        combine = FALSE;
      }
    }
    if (excise) {
      *prev = vnp->next;
      vnp->next = NULL;

      /* combine locations of duplicate references */

      irp = (IntRefBlockPtr) rbp;
      lastirp = (IntRefBlockPtr) lastrbp;
      if (combine) {
        if (lastirp != NULL) {
          slp = SeqLocMerge (target, lastirp->loc, irp->loc, FALSE, TRUE, FALSE);
          lastirp->loc = SeqLocFree (lastirp->loc);
          lastirp->loc = slp;
        }
        if (irp != NULL && lastirp != NULL) {
          if ((rbp->muid == lastrbp->muid && rbp->muid != 0) ||
              (rbp->pmid == lastrbp->pmid && rbp->pmid != 0)) {
            if (lastirp->fig == NULL) {
              lastirp->fig = StringSaveNoNull (irp->fig);
            }
            if (lastirp->maploc == NULL) {
              lastirp->maploc = StringSaveNoNull (irp->maploc);
            }
            lastirp->poly_a = irp->poly_a;
          }
        }
      }

      /* and remove duplicate reference */

      MemFree (rbp->uniquestr);
      DateFree (irp->date);
      SeqLocFree (irp->loc);
      MemFree (irp->authstr);
      MemFree (irp->fig);
      MemFree (irp->maploc);
      MemFree (rbp);
      ValNodeFree (vnp);

    } else {

      prev = &(vnp->next);
      lastrbp = rbp;
    }
    vnp = next;
  }

  /* resort by existing serial, then pub/unpub/sites/sub, then date */

  head = SortValNode (head, SortReferencesB);

  if (head == NULL) return FALSE;

  /* assign serial numbers */

  for (vnp = head, i = 1; vnp != NULL; vnp = vnp->next, i++) {
    rbp = (RefBlockPtr) vnp->data.ptrvalue;
    if (rbp != NULL) {
      rbp->serial = i;
    }
  }

  /* allocate reference array for this section */

  numReferences = i - 1;
  asp->numReferences = numReferences;

  if (numReferences > 0) {
    referenceArray = (RefBlockPtr PNTR) MemNew (sizeof (RefBlockPtr) * (numReferences + 1));
    asp->referenceArray = referenceArray;

    if (referenceArray != NULL) {

      /* fill in reference array */

      for (vnp = head, i = 0; vnp != NULL && i < numReferences; vnp = vnp->next, i++) {
        referenceArray [i] = (RefBlockPtr) vnp->data.ptrvalue;
      }
    }
  }

  /* finally link into blocks for current section */

  ValNodeLink (&(awp->lastblock), head);
  vnp = awp->lastblock;
  if (vnp == NULL) return FALSE;
  while (vnp->next != NULL) {
    vnp = vnp->next;
  }

  awp->lastblock = vnp;
  if (awp->blockList == NULL) {
    awp->blockList = vnp;
  }

  return TRUE;
}

static void AddHistCommentString (
  CharPtr prefix,
  CharPtr suffix,
  DatePtr dp,
  SeqIdPtr ids
)

{
  Char        buf [256];
  ValNodePtr  head = NULL;
  SeqIdPtr    sip;
  CharPtr     str;
  CharPtr     strd;

  if (dp == NULL || ids == NULL || prefix == NULL || suffix == NULL) return;

  strd = asn2gb_PrintDate (dp);
  if (strd == NULL) {
    strd = StringSave ("?");
  }

  sprintf (buf, "%s %s %s", prefix, strd, suffix);
  ValNodeCopyStr (&head, 0, buf);

  MemFree (strd);

  for (sip = ids; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GI) {
     sprintf (buf, " gi:%ld", (long) sip->data.intvalue);
      ValNodeCopyStr (&head, 0, buf);
    }
  }

  str = MergeValNodeStrings (head);

  gb_AddString (NULL, str, NULL, TRUE, FALSE, TILDE_EXPAND);

  MemFree (str);
  ValNodeFreeData (head);
}

static void AddHTGSCommentString (
  BioseqPtr bsp,
  MolInfoPtr mip
)

{
  CharPtr      buf = NULL;
  Char         buffer [256];
  Int4         buflen = 0;
  DeltaSeqPtr  dsp;
  ValNodePtr   head = NULL;
  Int4         num_s = 0;
  Int4         num_g = 0;
  CharPtr      str;

  if (bsp == NULL || mip == NULL || mip->tech < 2) return;

  if (bsp->repr == Seq_repr_delta) {
    for (dsp = (DeltaSeqPtr) bsp->seq_ext, buflen = 0; dsp != NULL; dsp = dsp->next) {
      buflen += 80;
    }
    if (buflen > 0) {
      buf = MemNew ((size_t) (buflen + 1));
      if (buf == NULL) return;
      CountGapsInDeltaSeq (bsp, &num_s, &num_g, NULL, NULL, buf, buflen);
    }
  }

  if (mip->tech == MI_TECH_htgs_0) {

    if (num_s > 0) {
      sprintf (buffer, "* NOTE: This record contains %ld individual~", (long) (num_s - num_g));
      ValNodeCopyStr (&head, 0, buffer);
      ValNodeCopyStr (&head, 0, "* sequencing reads that have not been assembled into~");
      ValNodeCopyStr (&head, 0, "* contigs. Runs of N are used to separate the reads~");
      ValNodeCopyStr (&head, 0, "* and the order in which they appear is completely~");
      ValNodeCopyStr (&head, 0, "* arbitrary. Low-pass sequence sampling is useful for~");
      ValNodeCopyStr (&head, 0, "* identifying clones that may be gene-rich and allows~");
      ValNodeCopyStr (&head, 0, "* overlap relationships among clones to be deduced.~");
      ValNodeCopyStr (&head, 0, "* However, it should not be assumed that this clone~");
      ValNodeCopyStr (&head, 0, "* will be sequenced to completion. In the event that~");
      ValNodeCopyStr (&head, 0, "* the record is updated, the accession number will~");
      ValNodeCopyStr (&head, 0, "* be preserved.");
    }
    ValNodeCopyStr (&head, 0, "~");
    ValNodeCopyStr (&head, 0, buf);

  } else if (mip->tech == MI_TECH_htgs_1) {

    ValNodeCopyStr (&head, 0, "* NOTE: This is a \"working draft\" sequence.");
    if (num_s > 0) {
      sprintf (buffer, " It currently~* consists of %ld contigs. The true order of the pieces~", (long) (num_s - num_g));
      ValNodeCopyStr (&head, 0, buffer);
      ValNodeCopyStr (&head, 0, "* is not known and their order in this sequence record is~");
      ValNodeCopyStr (&head, 0, "* arbitrary. Gaps between the contigs are represented as~");
      ValNodeCopyStr (&head, 0, "* runs of N, but the exact sizes of the gaps are unknown.");
    }
    ValNodeCopyStr (&head, 0, "~* This record will be updated with the finished sequence~");
    ValNodeCopyStr (&head, 0, "* as soon as it is available and the accession number will~");
    ValNodeCopyStr (&head, 0, "* be preserved.");
    ValNodeCopyStr (&head, 0, "~");
    ValNodeCopyStr (&head, 0, buf);

  } else if (mip->tech == MI_TECH_htgs_2) {

    ValNodeCopyStr (&head, 0, "* NOTE: This is a \"working draft\" sequence.");
    if (num_s > 0) {
      sprintf (buffer, " It currently~* consists of %ld contigs. Gaps between the contigs~", (long) (num_s - num_g));
      ValNodeCopyStr (&head, 0, buffer);
      ValNodeCopyStr (&head, 0, "* are represented as runs of N. The order of the pieces~");
      ValNodeCopyStr (&head, 0, "* is believed to be correct as given, however the sizes~");
      ValNodeCopyStr (&head, 0, "* of the gaps between them are based on estimates that have~");
      ValNodeCopyStr (&head, 0, "* provided by the submittor.");
    }
    ValNodeCopyStr (&head, 0, "~* This sequence will be replaced~");
    ValNodeCopyStr (&head, 0, "* by the finished sequence as soon as it is available and~");
    ValNodeCopyStr (&head, 0, "* the accession number will be preserved.");
    ValNodeCopyStr (&head, 0, "~");
    ValNodeCopyStr (&head, 0, buf);

  } else if ((str = StringForSeqTech (mip->tech)) != NULL) {

      sprintf (buffer, "Method: %s.", str);
      ValNodeCopyStr (&head, 0, buffer);
  }

  MemFree (buf);

  str = MergeValNodeStrings (head);

  gb_AddString (NULL, str, NULL, TRUE, TRUE, TILDE_EXPAND);

  MemFree (str);
  ValNodeFreeData (head);
}

static CharPtr GetMolInfoCommentString (
  BioseqPtr bsp,
  MolInfoPtr mip
)

{
  Boolean  is_aa;
  CharPtr  str = NULL;

  if (bsp == NULL || mip == NULL) return NULL;

  is_aa = ISA_aa (bsp->mol);
  switch (mip->completeness) {
    case 1 :
      str = "COMPLETENESS: full length";
      break;
    case 2 :
      str = "COMPLETENESS: not full length";
      break;
    case 3 :
      if (is_aa) {
        str = "COMPLETENESS: incomplete on the amino end";
      } else {
        str = "COMPLETENESS: incomplete on the 5' end";
      }
      break;
    case 4 :
      if (is_aa) {
        str = "COMPLETENESS: incomplete on the carboxy end";
      } else {
        str = "COMPLETENESS: incomplete on the 3' end";
      }
      break;
    case 5 :
      str = "COMPLETENESS: incomplete on both ends";
      break;
    case 6 :
      if (is_aa) {
        str = "COMPLETENESS: complete on the amino end";
      } else {
        str = "COMPLETENESS: complete on the 5' end";
      }
      break;
    case 7 :
      if (is_aa) {
        str = "COMPLETENESS: complete on the carboxy end";
      } else {
        str = "COMPLETENESS: complete on the 3' end";
      }
      break;
    default :
      str = "COMPLETENESS: unknown";
      break;
  }

  return str;
}

static CharPtr GetStrForBankit (
  UserObjectPtr uop
)

{
  CharPtr       bic = NULL, uvc = NULL, ptr;
  ObjectIdPtr   oip;
  UserFieldPtr  ufp;

  if (uop == NULL) return NULL;
  if ((oip = uop->type) == NULL) return NULL;
  if (StringCmp (oip->str, "Submission") != 0) return NULL;

  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (StringCmp(oip->str, "UniVecComment") == 0) {
      uvc = ufp->data.ptrvalue;
    } else if (StringCmp(oip->str, "AdditionalComment") == 0) {
      bic = ufp->data.ptrvalue;
    }
  }

  if (uvc == NULL || bic == NULL) return NULL;

  ptr = (CharPtr) MemNew (StringLen (uvc) + StringLen (bic) + 45);
  if (uvc != NULL && bic != NULL) {
    sprintf (ptr, "Vector Explanation: %s~Bankit Comment: %s", uvc, bic);
  } else if (uvc != NULL) {
    sprintf (ptr, "Vector Explanation: %s", uvc);
  } else if (bic != NULL) {
    sprintf (ptr, "Bankit Comment: %s", bic);
  }

  return ptr;
}

static CharPtr reftxt0 = "The reference sequence was derived from ";
static CharPtr reftxt1 = "PROVISIONAL REFSEQ: This record has not yet been subject to final NCBI review. ";
static CharPtr reftxt2 = "PREDICTED REFSEQ: The mRNA record is supported by experimental evidence; however, the coding sequence is predicted. ";
static CharPtr reftxt3 = "VALIDATED REFSEQ: This record has undergone preliminary review of the sequence, but has not yet been subject to final NCBI review. ";
static CharPtr reftxt4 = "REVIEWED REFSEQ: This record has been curated by ";

static CharPtr GetStrForRefTrack (
  UserObjectPtr uop
)

{
  CharPtr       curator = NULL, ptr = NULL, st;
  ObjectIdPtr   oip;
  UserFieldPtr  ufp, tmp, u, urf = NULL;
  Int2          i = 0;
  Char          p [41];
  Int2          review = 0,len;

  if (uop == NULL) return NULL;
  if ((oip = uop->type) == NULL) return NULL;
  if (StringCmp (oip->str, "RefGeneTracking") != 0) return NULL;
  len = StringLen (reftxt0);
  for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (StringCmp(oip->str, "Assembly") == 0) {
      urf = ufp;
    }
    if (StringCmp (oip->str, "Status") == 0) {
      st = (CharPtr) ufp->data.ptrvalue;
      if (StringCmp (st, "Provisional") == 0) {
        review = 1;
      } else if (StringCmp (st, "Predicted") == 0) {
        review = 2;
      } else if (StringCmp (st, "Validated") == 0) {
        review = 3;
      } else if (StringCmp (st, "Reviewed") == 0) {
        review = 4;
      }
    } else if (StringCmp (oip->str, "Collaborator") == 0) {
      st = (CharPtr) ufp->data.ptrvalue;
      if (! StringHasNoText (st)) {
        curator = st;
      }
    }
  }
  if (urf && urf->choice == 11) {
    for (tmp = urf->data.ptrvalue; tmp != NULL; tmp = tmp->next) {
      i++;
    }
    if (review == 1) {
      ptr = (CharPtr) MemNew ( StringLen (reftxt1) + len + 18*i + 1);
      StringCpy (ptr, reftxt1);
    } else if (review == 2) {
      ptr = (CharPtr) MemNew ( StringLen (reftxt2) + len + 18*i + 1);
      StringCpy (ptr, reftxt2);
    } else if (review == 3) {
      ptr = (CharPtr) MemNew ( StringLen (reftxt3) + len + 18*i + 1);
      StringCpy (ptr, reftxt3);
    } else if (review == 4) {
      if (curator == NULL) {
        curator = "NCBI staff";
      }
      ptr = (CharPtr) MemNew ( StringLen (reftxt4) + StringLen (curator) + len + 18*i + 1);
      StringCpy (ptr, reftxt4);
      StringCat (ptr, curator);
      StringCat (ptr, ". ");
    } else {
      ptr = (CharPtr) MemNew ( StringLen ("REFSEQ: ") + len + 18*i + 1);
      StringCpy (ptr, "REFSEQ: ");
    }
    if (i > 0) {
      StringCat (ptr, reftxt0);
    }

    for (tmp = urf->data.ptrvalue; tmp != NULL; tmp = tmp->next) {
      for (u = tmp->data.ptrvalue; u != NULL; u = u->next) {
        oip = u->label;
        if (StringCmp (oip->str, "accession") == 0) break;
      }
      if (u != NULL && tmp->next) {
        sprintf (p, "%s, ", u->data.ptrvalue);
      } else {
        sprintf (p, "%s.~~", u->data.ptrvalue);
      }
      StringCat (ptr, p);
    }
  }
  return ptr;
}

static CharPtr reftxt5 = "GENOME ANNOTATION REFSEQ:  This model reference sequence was predicted from NCBI contig";
static CharPtr reftxt6 = "by automated computational analysis";
static CharPtr reftxt7 = "using gene prediction method:";
static CharPtr reftxt8 = "~Also see:~    Documentation of NCBI's Annotation Process~    Evidence Viewer - alignments supporting this model";

static CharPtr GetAnnotationComment (
  BioseqPtr bsp
)

{
  SeqMgrDescContext  dcontext;
  CharPtr            method = NULL;
  CharPtr            name = NULL;
  ObjectIdPtr        oip;
  SeqDescrPtr        sdp;
  CharPtr            str = NULL;
  UserFieldPtr       ufp;
  UserObjectPtr      uop;

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &dcontext);
  while (sdp != NULL) {
    uop = (UserObjectPtr) sdp->data.ptrvalue;
    if (uop != NULL) {
      oip = uop->type;
      if (oip != NULL) {
        if (StringCmp(oip->str, "ModelEvidence") == 0) {
          for (ufp = uop->data; ufp != NULL; ufp = ufp->next) {
            oip = ufp->label;
            if (oip == NULL) continue;
            if (StringCmp(oip->str, "Contig Name") == 0) {
              name = (CharPtr) ufp->data.ptrvalue;
            } else if (StringCmp(oip->str, "Method") == 0) {
              method = (CharPtr) ufp->data.ptrvalue;
            }
          }
        }
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_user, &dcontext);
  }
  if (name == NULL || method == NULL) return NULL;
  str = MemNew (StringLen (reftxt5) + StringLen (reftxt6) + StringLen (reftxt7) +
                StringLen (reftxt8) + StringLen (name) + StringLen (method) + 10);
  if (str == NULL) return NULL;
  if (method != NULL) {
    sprintf (str, "%s %s %s %s %s.%s", reftxt5, name, reftxt6, reftxt7, method, reftxt8);
  } else {
    sprintf (str, "%s %s %s.%s", reftxt5, name, reftxt6, reftxt8);
  }
  return str;
}

static CharPtr reftxt9 = "GENOME ANNOTATION REFSEQ:  NCBI contigs are derived from assembled genomic sequence data. They may include both draft and finished sequence.";
static CharPtr tpaString = "THIRD PARTY ANNOTATION DATABASE: This entry contains annotation of previously submitted data.";

static void AddCommentBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr    ajp;
  BioseqPtr          bsp;
  Char               buf [128];
  CommentBlockPtr    cbp = NULL;
  Boolean            didRefTrack = FALSE;
  DbtagPtr           dbt;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  Boolean            first = TRUE;
  CommentBlockPtr    gsdbcbp = NULL;
  Int4               gsdbid = 0;
  SeqHistPtr         hist;
  Boolean            is_other = FALSE;
  MolInfoPtr         mip;
  BioseqPtr          parent;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;
  SeqIdPtr           sip;
  CharPtr            str;
  TextSeqIdPtr       tsip;
  UserObjectPtr      uop;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_OTHER) {
      tsip = (TextSeqIdPtr) sip->data.ptrvalue;

      if (tsip != NULL) {
        is_other = TRUE;
        if (StringNCmp(tsip->accession, "NT", 2) == 0) {

          cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
          if (cbp != NULL) {

            cbp->first = first;
            first = FALSE;

            if (cbp->first) {
              gb_StartPrint (awp->format, 0, 12, "COMMENT", 13, 5, 5, "CC", TRUE);
            } else {
              gb_StartPrint (awp->format, 0, 12, NULL, 13, 5, 5, "CC", FALSE);
            }

            gb_AddString (NULL, reftxt9, NULL, TRUE, FALSE, TILDE_EXPAND);

            cbp->string = gb_MergeString (TRUE);
          }

        } else if (StringNCmp(tsip->accession, "XP_", 3) == 0 || StringNCmp(tsip->accession, "XM_", 3) == 0) {

          str = GetAnnotationComment (bsp);
          if (str != NULL) {

            cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
            if (cbp != NULL) {

              cbp->first = first;
              first = FALSE;

              if (cbp->first) {
                gb_StartPrint (awp->format, 0, 12, "COMMENT", 13, 5, 5, "CC", TRUE);
              } else {
                gb_StartPrint (awp->format, 0, 12, NULL, 13, 5, 5, "CC", FALSE);
              }

              gb_AddString (NULL, str, NULL, TRUE, FALSE, TILDE_EXPAND);

              cbp->string = gb_MergeString (TRUE);
            }
          }
        }
      }

    } else if (sip->choice == SEQID_TPG || sip->choice == SEQID_TPE || sip->choice == SEQID_TPD) {

      cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
      if (cbp != NULL) {

        cbp->first = first;
        first = FALSE;

        if (cbp->first) {
          gb_StartPrint (awp->format, 0, 12, "COMMENT", 13, 5, 5, "CC", TRUE);
        } else {
          gb_StartPrint (awp->format, 0, 12, NULL, 13, 5, 5, "CC", FALSE);
        }

        gb_AddString (NULL, tpaString, NULL, TRUE, FALSE, TILDE_EXPAND);

        cbp->string = gb_MergeString (TRUE);
      }

    } else if (sip->choice == SEQID_GENERAL) {
      dbt = (DbtagPtr) sip->data.ptrvalue;

      /* show GSDB sequence identifier */

      if (dbt != NULL && StringCmp (dbt->db, "GSDB") == 0 && dbt->tag != NULL) {
        gsdbcbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
        if (gsdbcbp != NULL) {
          gsdbcbp->first = first;

          /* string will be created after we know if there are additional comments */

          gsdbid = dbt->tag->id;
          first = FALSE;
        }
      }
    }
  }

  /* RefSeq results in allocated comment string */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_user, &dcontext);
  while (sdp != NULL) {

    uop = (UserObjectPtr) sdp->data.ptrvalue;
    if (uop != NULL) {

      if (! ajp->flags.hideBankItComment) {
        str = GetStrForBankit (uop);
        if (str != NULL) {

          cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
          if (cbp != NULL) {

            cbp->entityID = dcontext.entityID;
            cbp->itemID = dcontext.itemID;
            cbp->itemtype = OBJ_SEQDESC;
            cbp->first = first;
            first = FALSE;

            if (cbp->first) {
              gb_StartPrint (awp->format, 0, 12, "COMMENT", 13, 5, 5, "CC", TRUE);
            } else {
              gb_StartPrint (awp->format, 0, 12, NULL, 13, 5, 5, "CC", FALSE);
            }

            gb_AddString (NULL, str, NULL, TRUE, FALSE, TILDE_EXPAND);

            cbp->string = gb_MergeString (TRUE);
          }
          MemFree (str);
        }
      }

      if (! didRefTrack) {
        str = GetStrForRefTrack (uop);
        if (str != NULL) {

          cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
          if (cbp != NULL) {

            cbp->entityID = dcontext.entityID;
            cbp->itemID = dcontext.itemID;
            cbp->itemtype = OBJ_SEQDESC;
            cbp->first = first;
            first = FALSE;

            if (cbp->first) {
              gb_StartPrint (awp->format, 0, 12, "COMMENT", 13, 5, 5, "CC", TRUE);
            } else {
              gb_StartPrint (awp->format, 0, 12, NULL, 13, 5, 5, "CC", FALSE);
            }

            gb_AddString (NULL, str, NULL, TRUE, FALSE, TILDE_EXPAND);

            cbp->string = gb_MergeString (TRUE);
          }
          MemFree (str);
          didRefTrack = TRUE;
        }
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_user, &dcontext);
  }

  /* Seq-hist results in allocated comment string */

  hist = bsp->hist;
  if (hist != NULL) {

    if (hist->replaced_by_ids != NULL && hist->replaced_by_date != NULL) {

      cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
      if (cbp != NULL) {
        cbp->first = first;
        first = FALSE;

        if (cbp->first) {
          gb_StartPrint (awp->format, 0, 12, "COMMENT", 13, 5, 5, "CC", TRUE);
        } else {
          gb_StartPrint (awp->format, 0, 12, NULL, 13, 5, 5, "CC", FALSE);
        }

        AddHistCommentString ("[WARNING] On", "this sequence was replaced by a newer version",
                              hist->replaced_by_date, hist->replaced_by_ids);

        cbp->string = gb_MergeString (TRUE);
      }
    }

    if (hist->replace_ids != NULL && hist->replace_date != NULL) {

      cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
      if (cbp != NULL) {
        cbp->first = first;
        first = FALSE;

        if (cbp->first) {
          gb_StartPrint (awp->format, 0, 12, "COMMENT", 13, 5, 5, "CC", TRUE);
        } else {
          gb_StartPrint (awp->format, 0, 12, NULL, 13, 5, 5, "CC", FALSE);
        }

        AddHistCommentString ("On", "this sequence version replaced",
                              hist->replace_date, hist->replace_ids);

        cbp->string = gb_MergeString (TRUE);
      }
    }

  }

  /* just save IDs for comment, maploc, and region descriptors */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_comment, &dcontext);
  while (sdp != NULL) {
    if (sdp->data.ptrvalue != NULL) {
      cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
      if (cbp != NULL) {
        cbp->entityID = dcontext.entityID;
        cbp->itemID = dcontext.itemID;
        cbp->itemtype = OBJ_SEQDESC;
        cbp->first = first;
        first = FALSE;
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_comment, &dcontext);
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_maploc, &dcontext);
  while (sdp != NULL) {
    cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
    if (cbp != NULL) {
      cbp->entityID = dcontext.entityID;
      cbp->itemID = dcontext.itemID;
      cbp->itemtype = OBJ_SEQDESC;
      cbp->first = first;
      first = FALSE;
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_maploc, &dcontext);
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_region, &dcontext);
  while (sdp != NULL) {
    if (sdp->data.ptrvalue != NULL) {
      cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
      if (cbp != NULL) {
        cbp->entityID = dcontext.entityID;
        cbp->itemID = dcontext.itemID;
        cbp->itemtype = OBJ_SEQDESC;
        cbp->first = first;
        first = FALSE;
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_region, &dcontext);
  }

  /* HTGS results in allocated comment string */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp != NULL) {

    mip = (MolInfoPtr) sdp->data.ptrvalue;
    if (mip != NULL) {
      if (mip->completeness != 0 && is_other) {

        str = GetMolInfoCommentString (bsp, mip);

        if (str != NULL) {
          cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
          if (cbp != NULL) {

            cbp->entityID = dcontext.entityID;
            cbp->itemID = dcontext.itemID;
            cbp->itemtype = OBJ_SEQDESC;
            cbp->first = first;
            first = FALSE;

            if (cbp->first) {
              gb_StartPrint (awp->format, 0, 12, "COMMENT", 13, 5, 5, "CC", TRUE);
            } else {
              gb_StartPrint (awp->format, 0, 12, NULL, 13, 5, 5, "CC", FALSE);
            }

            gb_AddString (NULL, str, NULL, TRUE, FALSE, TILDE_EXPAND);

            cbp->string = gb_MergeString (TRUE);
          }
        }

      }
      if (mip->tech == MI_TECH_htgs_0 ||
          mip->tech == MI_TECH_htgs_1 ||
          mip->tech == MI_TECH_htgs_2) {

        cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
        if (cbp != NULL) {

          /*
          cbp->entityID = dcontext.entityID;
          cbp->itemID = dcontext.itemID;
          cbp->itemtype = OBJ_SEQDESC;
          */
          cbp->first = first;
          first = FALSE;

          if (cbp->first) {
            gb_StartPrint (awp->format, 0, 12, "COMMENT", 13, 5, 5, "CC", TRUE);
          } else {
            gb_StartPrint (awp->format, 0, 12, NULL, 13, 5, 5, "CC", FALSE);
          }

          AddHTGSCommentString (bsp, mip);

          cbp->string = gb_MergeString (TRUE);
        }

      } else {
        str = StringForSeqTech (mip->tech);
        if (! StringHasNoText (str)) {

          cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
          if (cbp != NULL) {

            /*
            cbp->entityID = dcontext.entityID;
            cbp->itemID = dcontext.itemID;
            cbp->itemtype = OBJ_SEQDESC;
            */
            cbp->first = first;
            first = FALSE;

            if (cbp->first) {
              gb_StartPrint (awp->format, 0, 12, "COMMENT", 13, 5, 5, "CC", TRUE);
            } else {
              gb_StartPrint (awp->format, 0, 12, NULL, 13, 5, 5, "CC", FALSE);
            }

            gb_AddString ("Method: ", str, NULL, TRUE, FALSE, TILDE_EXPAND);

            cbp->string = gb_MergeString (TRUE);
          }
        }
      }
    }
  }

  /* add comment features that are full length on appropriate segment */

  parent = awp->parent;
  if (parent == NULL) return;

  sfp = SeqMgrGetNextFeature (parent, NULL, SEQFEAT_COMMENT, 0, &fcontext);
  while (sfp != NULL) {
    if (fcontext.left == awp->from && fcontext.right == awp->to) {
      cbp = (CommentBlockPtr) Asn2gbAddBlock (awp, COMMENT_BLOCK, sizeof (CommentBlock));
      if (cbp != NULL) {
        cbp->entityID = fcontext.entityID;
        cbp->itemID = fcontext.itemID;
        cbp->itemtype = OBJ_SEQFEAT;
        cbp->first = first;
        first = FALSE;
      }
    }
    sfp = SeqMgrGetNextFeature (parent, sfp, SEQFEAT_COMMENT, 0, &fcontext);
  }

  if (gsdbcbp != NULL) {

    /* if there were no subsequent comments, do not add period after GSDB id */

    if (cbp == NULL) {
      sprintf (buf, "GSDB:S:%ld", (long) gsdbid);
    } else {
      sprintf (buf, "GSDB:S:%ld.", (long) gsdbid);
    }

    if (gsdbcbp->first) {
      gb_StartPrint (awp->format, 0, 12, "COMMENT", 13, 5, 5, "CC", TRUE);
    } else {
      gb_StartPrint (awp->format, 0, 12, NULL, 13, 5, 5, "CC", FALSE);
    }

    /* CheckEndPunctuation, ConvertDoubleQuotes, and ExpandTildes already taken into account */

    ff_AddString (buf);

    gsdbcbp->string = gb_MergeString (TRUE);
  }
}

static void AddFeatHeaderBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr  bbp;

  if (awp == NULL) return;

  bbp = Asn2gbAddBlock (awp, FEATHEADER_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  if (awp->format == FTABLE_FMT) return;

  gb_StartPrint (awp->format, 0, 12, "FEATURES", 22, 5, 0, "FH", TRUE);

  if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
    ff_AddString ("Key");
    TabToColumn (22);
  }

  gb_AddString (NULL, "Location/Qualifiers", NULL, FALSE, FALSE, TILDE_TO_SPACES);

  if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {
    NewContLine();
  }

  bbp->string = gb_MergeString (TRUE);
}

static Uint2 ComputeSourceHash (
  CharPtr key,
  Uint2 start
)

{
  Uint4  h;
  Uint2  M;
  Uint2  S;

  if (key == NULL) return start;

  M = 101; /* prime key */
  S = 256; /* size of alphabet */

  for (h = start; *key != '\0'; key++) {
    h = (S * h + *key) % M;
  }

  return (Uint2) h;
}

static BaseBlockPtr AddSource (
  Asn2gbWorkPtr awp,
  ValNodePtr PNTR head,
  BioSourcePtr biop,
  CharPtr comment
)

{
  BaseBlockPtr    bbp;
  DbtagPtr        dbt;
  Uint2           hash;
  Int2            idx;
  IntSrcBlockPtr  isp;
  ObjectIdPtr     oip;
  OrgModPtr       omp;
  OrgNamePtr      onp;
  OrgRefPtr       orp;
  SubSourcePtr    ssp;
  CharPtr         str;
  Uint1           subtype;
  Char            tmp [16];
  ValNodePtr      vnp;

  if (awp == NULL || head == NULL || biop == NULL) return NULL;

  bbp = (BaseBlockPtr) MemNew (sizeof (IntSrcBlock));
  if (bbp == NULL) return NULL;
  bbp->blocktype = SOURCEFEAT_BLOCK;
  bbp->section = awp->currsection;

  ValNodeAddPointer (head, 0, bbp);

  isp = (IntSrcBlockPtr) bbp;
  isp->biop = biop;
  isp->is_focus = biop->is_focus;
  if (biop->origin == 5) {
    isp->is_synthetic = TRUE;
  }

  orp = biop->org;
  if (orp == NULL) return bbp;

  isp->orghash = ComputeSourceHash (orp->taxname, 0);
  isp->taxname = orp->taxname;

  hash = 0;
  onp = orp->orgname;
  if (onp != NULL) {
    if (StringICmp (onp->div, "SYN") == 0) {
      isp->is_synthetic = TRUE;
    }
    isp->omp = onp->mod;
    for (omp = onp->mod; omp != NULL; omp = omp->next) {
      subtype = omp->subtype;
      if (subtype == 254) {
        subtype = 24;
      } else if (subtype == 255) {
        subtype = 25;
      }
      if (subtype < 35) {
        idx = orgModToSourceIdx [subtype];
        if (idx > 0 && idx < ASN2GNBK_TOTAL_SOURCE) {
          str = asn2gnbk_source_quals [idx].name;
          hash = ComputeSourceHash (str, hash);
          hash = ComputeSourceHash (omp->subname, hash);
        }
      }
    }
  }
  if (comment != NULL) {
    hash = ComputeSourceHash ("note", hash);
    hash = ComputeSourceHash (comment, hash);
  }
  isp->modhash = hash;

  hash = 0;
  for (ssp = biop->subtype; ssp != NULL; ssp = ssp->next) {
    subtype = ssp->subtype;
    if (subtype == 255) {
      subtype = 29;
    }
    if (subtype < 30) {
      idx = subSourceToSourceIdx [subtype];
      if (idx > 0 && idx < ASN2GNBK_TOTAL_SOURCE) {
        str = asn2gnbk_source_quals [idx].name;
        hash = ComputeSourceHash (str, hash);
        hash = ComputeSourceHash (ssp->name, hash);
      }
    }
  }
  isp->subhash = hash;
  isp->ssp = biop->subtype;

  hash = 0;
  for (vnp = orp->db; vnp != NULL; vnp = vnp->next) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL) {
      hash = ComputeSourceHash (dbt->db, hash);
      oip = dbt->tag;
      if (oip != NULL) {
        if (oip->str != NULL) {
          hash = ComputeSourceHash (oip->str, hash);
        } else {
          sprintf (tmp, "%ld", (long) oip->id);
          hash = ComputeSourceHash (tmp, hash);
        }
      }
    }
  }
  isp->xrfhash = hash;
  isp->vnp = orp->db;

  return bbp;
}

static int LIBCALLBACK SortSourcesByHash (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  Uint4           diff;
  IntSrcBlockPtr  isp1;
  IntSrcBlockPtr  isp2;
  ValNodePtr      vnp1;
  ValNodePtr      vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  isp1 = (IntSrcBlockPtr) vnp1->data.ptrvalue;
  isp2 = (IntSrcBlockPtr) vnp2->data.ptrvalue;
  if (isp1 == NULL || isp2 == NULL) return 0;

  if (isp1->is_focus && (! isp2->is_focus)) return -1;
  if (isp2->is_focus && (! isp1->is_focus)) return 1;

  diff = isp1->orghash - isp2->orghash;
  if (diff > 0) return -1;
  if (diff < 0) return 1;

  diff = isp1->xrfhash - isp2->xrfhash;
  if (diff > 0) return -1;
  if (diff < 0) return 1;

  /* sort so that sources with modifiers come first */

  diff = isp1->modhash - isp2->modhash;
  if (diff > 0) return -1;
  if (diff < 0) return 1;

  diff = isp1->subhash - isp2->subhash;
  if (diff > 0) return -1;
  if (diff < 0) return 1;

  /* if all hashes are equal, descriptor comes first */

  if (isp1->is_descriptor && (! isp2->is_descriptor)) {
    return -1;
  } else if (isp2->is_descriptor && (! isp1->is_descriptor)) {
    return 1;
  }

  return 0;
}

static int LIBCALLBACK SortSourcesByPos (
  VoidPtr ptr1,
  VoidPtr ptr2
)

{
  IntSrcBlockPtr  isp1;
  IntSrcBlockPtr  isp2;
  ValNodePtr      vnp1;
  ValNodePtr      vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  isp1 = (IntSrcBlockPtr) vnp1->data.ptrvalue;
  isp2 = (IntSrcBlockPtr) vnp2->data.ptrvalue;
  if (isp1 == NULL || isp2 == NULL) return 0;

  /* descriptor always goes first */

  if (isp1->is_descriptor && (! isp2->is_descriptor)) {
    return -1;
  } else if (isp2->is_descriptor && (! isp1->is_descriptor)) {
    return 1;
  }

  /* feature with smallest left extreme is first */

  if (isp1->left > isp2->left) {
    return 1;
  } else if (isp1->left < isp2->left) {
    return -1;
  }

  /* if same left extreme, shortest source feature is first just for flatfile */

  if (isp1->right > isp2->right) {
    return 1;
  } else if (isp1->right < isp2->right) {
    return -1;
  }

  return 0;
}

/*                                                                   */
/* s_isFuzzyLoc () -- Determines is a location has fuzzy coordinates */
/*                                                                   */

static Boolean s_isFuzzyLoc ( SeqLocPtr pLocation )
{
  SeqIntPtr pIntLocation;

  if (pLocation == NULL)
    return FALSE;

  if (pLocation->choice != SEQLOC_INT)
    return FALSE;

  if (pLocation->data.ptrvalue == NULL)
    return FALSE;

  pIntLocation = (SeqIntPtr) pLocation->data.ptrvalue;

  if ((pIntLocation->if_from != NULL) && (pIntLocation->if_from->choice == 2))
    return TRUE;

  if ((pIntLocation->if_to != NULL) && (pIntLocation->if_to->choice == 2))
    return TRUE;

  return FALSE;
}

static void GetSourcesOnBioseq (
  Asn2gbWorkPtr awp,
  BioseqPtr target,
  BioseqPtr bsp,
  Int4 from,
  Int4 to
)

{
  BaseBlockPtr       bbp;
  BioSourcePtr       biop;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  Boolean            hasNulls;
  Boolean            loop = FALSE;
  Int2               idx;
  IntSrcBlockPtr     isp;
  Int4Ptr            ivals;
  Boolean            noLeft;
  Boolean            noRight;
  Int2               numivals;
  Boolean            okay;
  SeqDescrPtr        sdp;
  SeqFeatPtr         sfp;
  SeqInt             sint;
  SeqIdPtr           sip;
  Int4               start;
  Int4               stop;
  ValNode            vn;
  ValNodePtr         vnp;

  if (awp == NULL || target == NULL || bsp == NULL) return;

  /* full length loc for descriptors */

  sint.from = 0;
  sint.to = bsp->length - 1;
  sint.strand = Seq_strand_plus;
  sint.id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
  sint.if_from = NULL;
  sint.if_to = NULL;

  vn.choice = SEQLOC_INT;
  vn.data.ptrvalue = (Pointer) &sint;
  vn.next = NULL;

  /* if SWISS-PROT, may have multiple source descriptors */

  if (ISA_aa (bsp->mol)) {
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_SWISSPROT) {
        loop = TRUE;
      }
    }
  }

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  while (sdp != NULL) {

    /* check if descriptor on part already added on segmented bioseq */

    okay = TRUE;
    for (vnp = awp->srchead; vnp != NULL && okay; vnp = vnp->next) {
      bbp = (BaseBlockPtr) vnp->data.ptrvalue;
      if (bbp != NULL) {
        if (bbp->entityID == dcontext.entityID &&
            bbp->itemID == dcontext.itemID &&
            bbp->itemtype == OBJ_SEQDESC) {
          okay = FALSE;
        }
      }
    }

    if (okay) {
      biop = (BioSourcePtr) sdp->data.ptrvalue;
      bbp = AddSource (awp, &(awp->srchead), biop, NULL);
      if (bbp != NULL) {

        bbp->entityID = dcontext.entityID;
        bbp->itemID = dcontext.itemID;
        bbp->itemtype = OBJ_SEQDESC;

        isp = (IntSrcBlockPtr) bbp;
        isp->loc = SeqLocMerge (target, &vn, NULL, FALSE, TRUE, FALSE);
        isp->left = 0;
        isp->right = bsp->length - 1;
        isp->is_descriptor = TRUE;
      }
    }

    /* if SWISS-PROT, loop through multiple source descriptors */

    if (loop) {
      sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_source, &dcontext);
    } else {
      sdp = NULL;
    }
  }

  SeqIdFree (sint.id);

  if ((! awp->contig) || awp->showconsource) {

    /* features are indexed on parent if segmented */

    bsp = awp->parent;

    sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_BIOSRC, 0, &fcontext);
    while (sfp != NULL) {
      ivals = fcontext.ivals;
      numivals = fcontext.numivals;
      if (ivals != NULL && numivals > 0) {

        idx = (numivals - 1) * 2;
        start = ivals [idx];
        stop = ivals [idx + 1];
        if (stop >= from && stop <= to) {

          biop = (BioSourcePtr) sfp->data.value.ptrvalue;
          bbp = AddSource (awp, &(awp->srchead), biop, sfp->comment);
          if (bbp != NULL) {

            bbp->entityID = fcontext.entityID;
            bbp->itemID = fcontext.itemID;
            bbp->itemtype = OBJ_SEQFEAT;

            isp = (IntSrcBlockPtr) bbp;
            if (sfp->location != NULL && sfp->location->choice == SEQLOC_PNT) {
              isp->loc = AsnIoMemCopy ((Pointer) sfp->location,
                                       (AsnReadFunc) SeqLocAsnRead,
                                       (AsnWriteFunc) SeqLocAsnWrite);
            } else if (s_isFuzzyLoc (sfp->location)) {
              isp->loc = AsnIoMemCopy ((Pointer) sfp->location,
                                      (AsnReadFunc) SeqLocAsnRead,
                                      (AsnWriteFunc) SeqLocAsnWrite);
            } else if (SeqLocId(sfp->location) == NULL) {
              isp->loc = AsnIoMemCopy ((Pointer) sfp->location,
                                       (AsnReadFunc) SeqLocAsnRead,
                                       (AsnWriteFunc) SeqLocAsnWrite);
            } else {
              CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
              hasNulls = LocationHasNullsBetween (sfp->location);
              isp->loc = SeqLocMerge (target, sfp->location, NULL, FALSE, TRUE, hasNulls);
              SetSeqLocPartial (isp->loc, noLeft, noRight);
            }
            isp->left = fcontext.left;
            isp->right = fcontext.right;
            isp->comment = sfp->comment;
          }
        }
      }

      sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_BIOSRC, 0, &fcontext);
    }
  }
}

static Boolean LIBCALLBACK GetSourcesOnSeg (
  SeqLocPtr slp,
  SeqMgrSegmentContextPtr context
)

{
  Asn2gbWorkPtr  awp;
  BioseqPtr      bsp = NULL;
  Uint2          entityID;
  Int4           from;
  SeqLocPtr      loc;
  SeqIdPtr       sip;
  Int4           to;

  if (slp == NULL || context == NULL) return FALSE;
  awp = (Asn2gbWorkPtr) context->userdata;

  from = context->cumOffset;
  to = from + context->to - context->from;

  sip = SeqLocId (slp);
  if (sip == NULL) {
    loc = SeqLocFindNext (slp, NULL);
    if (loc != NULL) {
      sip = SeqLocId (loc);
    }
  }
  if (sip == NULL) return TRUE;

  /* may remote fetch genome component if not already in memory */

  bsp = BioseqLockById (sip);

  if (bsp == NULL) return TRUE;

  entityID = ObjMgrGetEntityIDForPointer (bsp);

  if (entityID != awp->entityID) {

    /* if segment not packaged in record, may need to feature index it */

    if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
      SeqMgrIndexFeatures (entityID, NULL);
    }

    /* collect features indexed on the remote bioseq */

    from = 0;
    to = bsp->length - 1;
  }

  GetSourcesOnBioseq (awp, awp->target, bsp, from, to);

  BioseqUnlock (bsp);

  return TRUE;
}

/* isIdenticalSource() -- Checks to see if two sources are identical */
/*                        by comparing the actual values in the      */
/*                        fields.  This only gets called if the two  */
/*                        sources hashed the same -- it's a double-  */
/*                        check since two non-identical things will  */
/*                        occassionally hash to the same value.      */

static Boolean isIdenticalSource (IntSrcBlockPtr isp1, IntSrcBlockPtr isp2)
{
  OrgModPtr     omp1;
  OrgModPtr     omp2;
  SubSourcePtr  ssp1;
  SubSourcePtr  ssp2;
  ValNodePtr    vnp1;
  ValNodePtr    vnp2;
  ObjectIdPtr   oip1;
  ObjectIdPtr   oip2;
  DbtagPtr      dbt1;
  DbtagPtr      dbt2;

  if (isp1->is_focus != isp2->is_focus)
    return FALSE;

  /* Compare the taxonomy names */

  if (StringICmp(isp1->taxname,isp2->taxname) != 0)
    return FALSE;

  /* Compare the comment */

  if (StringICmp(isp1->comment,isp2->comment) != 0)
    return FALSE;

  /* Compare the org mods */

  omp1 = isp1->omp;
  omp2 = isp2->omp;
  while (omp1 != NULL && omp2 != NULL)
    {
      if (omp1->subtype != omp2->subtype)
        return FALSE;
      if (StringICmp (omp1->subname, omp2->subname) != 0)
        return FALSE;
      omp1 = omp1->next;
      omp2 = omp2->next;
    }

  if (omp1 != NULL || omp2 != NULL)
    return FALSE;

  /* Compare the subtypes */

  ssp1 = isp1->ssp;
  ssp2 = isp2->ssp;

  while (ssp1 != NULL && ssp2 != NULL)
    {
      if (ssp1->subtype != ssp2->subtype)
        return FALSE;
      if (StringICmp(ssp1->name,ssp2->name) != 0)
        return FALSE;
      ssp1 = ssp1->next;
      ssp2 = ssp2->next;
    }

  if (ssp1 != NULL || ssp2 != NULL)
    return FALSE;

  /* Compare the DB tags */

  vnp1 = isp1->vnp;
  vnp2 = isp2->vnp;

  while (vnp1 != NULL && vnp2 != NULL)
    {
      dbt1 = (DbtagPtr) vnp1->data.ptrvalue;
      dbt2 = (DbtagPtr) vnp2->data.ptrvalue;

      if ((dbt1 != NULL) && (dbt2 != NULL)) {
        if (dbt1->db != dbt2->db)
          return FALSE;

        oip1 = dbt1->tag;
        oip2 = dbt2->tag;
        if ((oip1 != NULL) && (oip2 != NULL)) {
          if (oip1->str != NULL) {
            if (StringICmp(oip1->str, oip2->str) != 0)
              return FALSE;
          } else  {
            if (oip1->id != oip2->id)
              return FALSE;
          }
        }
        else if (oip1 != NULL)
          return FALSE;
        else if (oip2 != NULL)
          return FALSE;
      }
      else if (dbt1 != NULL)
        return FALSE;
      else if (dbt2 != NULL)
        return FALSE;

      vnp1 = vnp1->next;
      vnp2 = vnp2->next;
    }

  if (vnp1 != NULL || vnp2 != NULL)
    return FALSE;

  /* If it passed all checks, then they */
  /* are the same, so return true.      */

  return TRUE;
}

static void AddSourceFeatBlock (
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr    ajp;
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  SeqMgrFeatContext  context;
  BioseqPtr          dna;
  SeqLocPtr          duploc;
  Boolean            excise;
  ValNodePtr         head = NULL;
  IntSrcBlockPtr     isp;
  IntSrcBlockPtr     lastisp;
  IntSrcBlockPtr     descrIsp;
  ValNodePtr         next;
  ValNodePtr         PNTR prev;
  SeqInt             sint;
  SeqLocPtr          slp;
  CharPtr            str;
  BioseqPtr          target;
  ValNode            vn;
  ValNodePtr         vnp;
  CharPtr            wwwbuf;
  Boolean            descHasFocus = FALSE;

  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  /* collect biosources on bioseq */

  awp->srchead = NULL;
  GetSourcesOnBioseq (awp, bsp, bsp, awp->from, awp->to);
  target = bsp;

  if (bsp->repr == Seq_repr_seg) {

    /* collect biosources on remote segments in MASTER_STYLE */

    SeqMgrExploreSegments (bsp, (Pointer) awp, GetSourcesOnSeg);
    target = awp->target;
  }

  if (awp->srchead == NULL && ISA_aa (bsp->mol)) {

    /* if protein with no sources, get sources applicable to DNA location of CDS */

    cds = SeqMgrGetCDSgivenProduct (bsp, &context);
    if (cds != NULL) {
      dna = BioseqFindFromSeqLoc (cds->location);
      if (dna != NULL) {
        GetSourcesOnBioseq (awp, dna, dna, context.left, context.right);
        target = dna;
      }
    }
  }

  head = awp->srchead;
  awp->srchead = NULL;

  if (head == NULL) {
    sint.from = 0;
    sint.to = bsp->length - 1;
    sint.strand = Seq_strand_plus;
    sint.id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
    sint.if_from = NULL;
    sint.if_to = NULL;

    vn.choice = SEQLOC_INT;
    vn.data.ptrvalue = (Pointer) &sint;
    vn.next = NULL;

    gb_StartPrint (awp->format, 5, 21, NULL, 0, 5, 21, "FT", FALSE);
    ff_AddString ("source");
    TabToColumn (22);

    str = FlatLoc (bsp, &vn, (Boolean) (awp->style == MASTER_STYLE));
    if (get_www ()) {
      wwwbuf = www_featloc (str);
      ff_AddString (wwwbuf);
      MemFree (wwwbuf);
    } else {
      ff_AddString (str);
    }
    MemFree (str);

    if (ajp->flags.needOrganismQual) {
      NewContLine ();
      gb_AddString ("/organism=\"", "unknown", "\"", FALSE, TRUE, TILDE_TO_SPACES);
#ifdef ASN2GNBK_PRINT_UNKNOWN_ORG
    } else {
      NewContLine ();
      gb_AddString ("/organism=\"", "unknown", "\"", FALSE, TRUE, TILDE_TO_SPACES);
#endif
    }

    str = ff_print_string_mem (gb_MergeString (TRUE));

    bbp = (BaseBlockPtr) Asn2gbAddBlock (awp, SOURCEFEAT_BLOCK, sizeof (IntSrcBlock));
    if (bbp != NULL) {
      bbp->section = awp->currsection;
      bbp->string = str;
    }

    return;
  }

  /* sort by hash values */

  head = SortValNode (head, SortSourcesByHash);

  /* unique sources, excise duplicates from list */

  prev = &(head);
  vnp = head;
  lastisp = NULL;
  while (vnp != NULL) {
    excise = FALSE;
    next = vnp->next;
    isp = (IntSrcBlockPtr) vnp->data.ptrvalue;
    if (isp->is_descriptor && isp->is_focus)
      descHasFocus = TRUE;
    if (lastisp != NULL) {
      if (isp != NULL) {
        if (lastisp->is_focus == isp->is_focus &&
            lastisp->orghash == isp->orghash &&
            lastisp->xrfhash == isp->xrfhash) {

          /* check for identical modifiers */

          if (lastisp->modhash == isp->modhash &&
              lastisp->subhash == isp->subhash) {

            excise = isIdenticalSource (isp, lastisp);

          /* or modifiers only in lastisp (e.g., on part bioseq) */

          } else if (isp->modhash == 0 && isp->subhash == 0) {
            excise = isIdenticalSource (isp, lastisp);
          }
        }
      }
    }
    if (excise) {
      *prev = vnp->next;
      vnp->next = NULL;

      /* combine locations of duplicate sources */

      if (lastisp != NULL) {
        slp = SeqLocMerge (target, lastisp->loc, isp->loc, FALSE, TRUE, FALSE);
        lastisp->loc = SeqLocFree (lastisp->loc);
        lastisp->loc = slp;
        lastisp->left = MIN (lastisp->left,isp->left);
        lastisp->right = MAX (lastisp->right, isp->right);
      }

      /* and remove duplicate source */

      SeqLocFree (isp->loc);
      MemFree (isp);
      ValNodeFree (vnp);

    } else {

      prev = &(vnp->next);
      lastisp = isp;
    }
    vnp = next;
  }

  /* Sort again, by location this time */

  head = SortValNode (head, SortSourcesByPos);

  /* If the descriptor has a focus, then subtract */
  /* out all the other source locations.          */

  descrIsp = (IntSrcBlockPtr) head->data.ptrvalue; /* Sorted 1st by now */

  if ((descHasFocus) && (! descrIsp->is_synthetic))
    {

      vnp = head;
      duploc = AsnIoMemCopy ((Pointer) descrIsp->loc,
                             (AsnReadFunc) SeqLocAsnRead,
                             (AsnWriteFunc) SeqLocAsnWrite);
      vnp = vnp->next;
      while (vnp != NULL)
      {
        isp = (IntSrcBlockPtr) vnp->data.ptrvalue;
        if (SeqLocAinB (descrIsp->loc, isp->loc) >= 0) {
          vnp = NULL; /* break the chain */
          descrIsp->loc = SeqLocFree (descrIsp->loc);
          descrIsp->loc = duploc;
          duploc = NULL;
        } else {
          SeqLocSubtract (descrIsp->loc, isp->loc);
          vnp = vnp->next;
        }
      }
      descrIsp->left  = SeqLocStart (descrIsp->loc);
      descrIsp->right = SeqLocStop (descrIsp->loc);
      SeqLocFree (duploc);
    }

  /* finally link into blocks for current section */

  ValNodeLink (&(awp->lastblock), head);
  vnp = awp->lastblock;
  if (vnp == NULL) return;
  while (vnp->next != NULL) {
    vnp = vnp->next;
  }

  awp->lastblock = vnp;
  if (awp->blockList == NULL) {
    awp->blockList = vnp;
  }
}

static void GetFeatsOnCdsProduct (
  SeqFeatPtr sfp,
  BioseqPtr bsp,
  Asn2gbWorkPtr awp
)

{
  FeatBlockPtr       fbp;
  IntFeatBlockPtr    ifp;
  Int4               lastleft;
  Int4               lastright;
  SeqAnnotPtr        lastsap;
  SeqFeatPtr         lastsfp;
  SeqMgrFeatContext  pcontext;
  SeqFeatPtr         prt;
  Boolean            suppress;

  if (sfp == NULL || awp == NULL) return;
  if (bsp == NULL || (! ISA_aa (bsp->mol))) return;

  /* explore mat_peptides, sites, etc. */

  lastsfp = NULL;
  lastsap = NULL;
  lastleft = 0;
  lastright = 0;

  prt = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &pcontext);
  while (prt != NULL) {

    if (pcontext.featdeftype == FEATDEF_REGION ||
        pcontext.featdeftype == FEATDEF_SITE ||
        pcontext.featdeftype == FEATDEF_mat_peptide_aa ||
        pcontext.featdeftype == FEATDEF_sig_peptide_aa ||
        pcontext.featdeftype == FEATDEF_transit_peptide_aa) {

      if (pcontext.dnaStop >= awp->from && pcontext.dnaStop <= awp->to) {

        /* suppress duplicate features (on protein) */

        suppress = FALSE;
        if (lastsfp != NULL && lastsap != NULL) {
          if (lastsfp->idx.subtype == prt->idx.subtype &&
              lastleft == pcontext.left &&
              lastright == pcontext.right) {
              if (lastsap == pcontext.sap ||
                  (lastsap->desc == NULL && pcontext.sap->desc == NULL)) {
              if (AsnIoMemComp (lastsfp, prt, (AsnWriteFunc) SeqFeatAsnWrite)) {
                suppress = TRUE;
              }
            }
          }
        }

        if (! suppress) {

          fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntFeatBlock));
          if (fbp != NULL) {

            fbp->entityID = pcontext.entityID;
            fbp->itemID = pcontext.itemID;
            fbp->itemtype = OBJ_SEQFEAT;
            fbp->featdeftype = pcontext.featdeftype;
            ifp = (IntFeatBlockPtr) fbp;
            ifp->mapToNuc = TRUE;
            ifp->mapToProt = FALSE;
            ifp->mapToGen = FALSE;
            ifp->mapToMrna = FALSE;
            ifp->firstfeat = awp->firstfeat;
            awp->firstfeat = FALSE;
          }
        }

        lastsfp = prt;
        lastsap = pcontext.sap;
        lastleft = pcontext.left;
        lastright = pcontext.right;

      }
    }
    prt = SeqMgrGetNextFeature (bsp, prt, 0, 0, &pcontext);
  }
}

static Boolean NotEMBL (
  BioseqPtr bsp
)

{
  SeqIdPtr  sip;

  if (bsp == NULL) return TRUE;
  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_EMBL) return FALSE;
  }
  return TRUE;
}

static Boolean LIBCALLBACK GetFeatsOnBioseq (
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr fcontext
)

{
  IntAsn2gbJobPtr    ajp;
  Asn2gbSectPtr      asp;
  Asn2gbWorkPtr      awp;
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  SeqMgrFeatContext  cdscontext;
  SeqMgrDescContext  dcontext;
  FeatBlockPtr       fbp;
  SeqLocPtr          firstslp;
  GBQualPtr          gbq;
  SeqFeatPtr         gene;
  GeneRefPtr         grp;
  Boolean            juststop = FALSE;
  IntCdsBlockPtr     icp;
  Int2               idx;
  IntFeatBlockPtr    ifp;
  Int4Ptr            ivals;
  Int2               j;
  SeqAnnotPtr        lastsap;
  SeqFeatPtr         lastsfp;
  SeqLocPtr          lastslp;
  Int2               numivals;
  Boolean            okay;
  SeqEntryPtr        oldscope;
  BioseqPtr          parent;
  Boolean            partial5;
  Boolean            partial3;
  ValNodePtr         ppr;
  PubdescPtr         pdp;
  Boolean            pseudo = FALSE;
  SeqDescrPtr        sdp;
  SeqEntryPtr        sep;
  SeqIdPtr           sip;
  SeqLocPtr          slp;
  Int4               start;
  Int4               stop;
  ValNodePtr         vnp;

  if (sfp == NULL || fcontext == NULL) return FALSE;
  awp = (Asn2gbWorkPtr) fcontext->userdata;
  if (awp == NULL) return FALSE;
  ajp = awp->ajp;
  if (ajp == NULL) return FALSE;
  asp = awp->asp;
  if (asp == NULL) return FALSE;
  bsp = asp->bsp;
  if (bsp == NULL) return FALSE;

  if (fcontext->featdeftype == FEATDEF_PUB ||
      fcontext->featdeftype == FEATDEF_NON_STD_RESIDUE ||
      fcontext->featdeftype == FEATDEF_BIOSRC ||
      fcontext->featdeftype == FEATDEF_RSITE ||
      fcontext->featdeftype == FEATDEF_SEQ) return TRUE;

  if (ajp->flags.validateFeats &&
      (fcontext->featdeftype == FEATDEF_BAD ||
       fcontext->featdeftype == FEATDEF_virion)) {
    return TRUE;
  }

  if (ISA_na (bsp->mol) && fcontext->featdeftype == FEATDEF_HET) return TRUE;

  /* DDBJ does not want to show gene features */

  if (fcontext->seqfeattype == SEQFEAT_GENE && awp->hideGeneFeats) return TRUE;

  /* suppress comment features that are full length */

  if (fcontext->seqfeattype == SEQFEAT_COMMENT &&
      fcontext->left == awp->from && fcontext->right == awp->to) return TRUE;

  ivals = fcontext->ivals;
  numivals = fcontext->numivals;

  /* check to see if last interval is on this awp->from - awp->to range */

  if (ivals != NULL && numivals > 0) {
    idx = (numivals - 1) * 2;
    start = ivals [idx];
    stop = ivals [idx + 1];
    if (stop < awp->from || stop > awp->to) {

      /* may need to map sig_peptide on a different segment */

      if (fcontext->seqfeattype == SEQFEAT_CDREGION) {
        sip = SeqLocId (sfp->product);
        bsp = BioseqFind (sip);
        GetFeatsOnCdsProduct (sfp, bsp, awp);
      }

      return TRUE;

    } else if (fcontext->farloc && NotEMBL (awp->bsp)) {

      /* last interval may not have been mapped to bioseq if far */

      firstslp = NULL;
      lastslp = NULL;

      slp = SeqLocFindNext (sfp->location, NULL);
      while (slp != NULL) {
        if (slp->choice != SEQLOC_NULL) {
          lastslp = slp;
          if (firstslp == NULL) {
            firstslp = slp;
          }
        }
        slp = SeqLocFindNext (sfp->location, slp);
      }

      /* !!! EMBL may have different desired behavior on where to map !!! */

      if (firstslp != NULL && SeqLocStrand (firstslp) == Seq_strand_minus) {
        slp = firstslp;
      } else {
        slp = lastslp;
      }

      if (slp != NULL) {
        sip = SeqLocId (slp);
        if (sip != NULL) {
          bsp = BioseqFindCore (sip);
          if (bsp == NULL || (bsp != awp->parent && bsp != awp->bsp)) {

            return TRUE;
          }
        }
      }
    }
  }

  /* suppress duplicate features (on nucleotide) */

  lastsfp = awp->lastsfp;
  lastsap = awp->lastsap;
  if (lastsfp != NULL && lastsap != NULL) {
    if (lastsfp->idx.subtype == sfp->idx.subtype &&
        awp->lastleft == fcontext->left &&
        awp->lastright == fcontext->right) {
        if (lastsap == fcontext->sap ||
            (lastsap->desc == NULL && fcontext->sap->desc == NULL)) {
        if (AsnIoMemComp (lastsfp, sfp, (AsnWriteFunc) SeqFeatAsnWrite)) {
          return TRUE;
        }
      }
    }
  }

  /* if RELEASE_MODE, verify that features have all mandatory qualifiers */

  if (ajp->flags.needRequiredQuals) {
    okay = FALSE;

    switch (fcontext->featdeftype) {

    case FEATDEF_CDS:
      if (ajp->flags.checkCDSproductID) {
        /* non-pseudo CDS must have /product */
        if (sfp->pseudo) {
          pseudo = TRUE;
        }
        grp = SeqMgrGetGeneXref (sfp);
        if (grp == NULL) {
          sep = GetTopSeqEntryForEntityID (ajp->ajp.entityID);
          oldscope = SeqEntrySetScope (sep);
          gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
          SeqEntrySetScope (oldscope);
          if (gene != NULL) {
            grp = (GeneRefPtr) gene->data.value.ptrvalue;
            if (gene->pseudo) {
              pseudo = TRUE;
            }
          }
        }
        if (grp != NULL && grp->pseudo) {
          pseudo = TRUE;
        }
        if (sfp->location != NULL) {
          if (CheckSeqLocForPartial (sfp->location, &partial5, &partial3)) {
            if (partial5 && (! partial3)) {
              if (SeqLocLen (sfp->location) <= 5) {
                juststop = TRUE;
              }
            }
          }
        }
        if (pseudo || juststop) {
          okay = TRUE;
        } else if (sfp->product != NULL) {
          sip = SeqLocId (sfp->product);
          if (sip != NULL && sip->choice == SEQID_GI && sip->data.intvalue > 0) {
            okay = TRUE;
          }
        }
      } else {
        okay = TRUE;
      }
      break;

    case FEATDEF_conflict:
      /* conflict requires a publication printable on the segment */
      vnp = sfp->cit;

      if (vnp != NULL && asp->referenceArray != NULL) {
        for (ppr = vnp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
          j = MatchRef (ppr, asp->referenceArray, asp->numReferences);
          if (j > 0) {
            okay = TRUE;
            break;
          }
        }
      }
      break;

    case FEATDEF_GENE:
      /* gene requires /gene */
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      if (grp != NULL) {
        if (! StringHasNoText (grp->locus)) {
          okay = TRUE;
        } else if (! StringHasNoText (grp->desc)) {
          okay = TRUE;
        } else {
          vnp = grp->syn;
          if (vnp != NULL) {
            if (! StringHasNoText (vnp->data.ptrvalue)) {
              okay = TRUE;
            }
          }
        }
      }
      break;

    case FEATDEF_protein_bind:
    case FEATDEF_misc_binding:
      /* protein_bind or misc_binding require FTQUAL_bound_moiety */
      gbq = sfp->qual;
      while (gbq != NULL) {
        if (StringICmp (gbq->qual, "bound_moiety") == 0 && (! StringHasNoText (gbq->val))) {
          okay = TRUE;
          break;
        }
        gbq = gbq->next;
      }
      break;

    case FEATDEF_modified_base:
      /* modified_base requires FTQUAL_mod_base */
      gbq = sfp->qual;
      while (gbq != NULL) {
        if (StringICmp (gbq->qual, "mod_base") == 0 && (! StringHasNoText (gbq->val))) {
          okay = TRUE;
          break;
        }
        gbq = gbq->next;
      }
      break;

    default:
      if (fcontext->featdeftype >= FEATDEF_GENE && fcontext->featdeftype < FEATDEF_MAX) {
        okay = TRUE;
      }
      break;
    }

    if (okay == FALSE) return TRUE;

  }

  awp->lastsfp = sfp;
  awp->lastsap = fcontext->sap;
  awp->lastleft = fcontext->left;
  awp->lastright = fcontext->right;

  if (fcontext->seqfeattype == SEQFEAT_CDREGION) {
    fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntCdsBlock));
  } else {
    fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntFeatBlock));
  }
  if (fbp == NULL) return TRUE;

  fbp->entityID = fcontext->entityID;
  fbp->itemID = fcontext->itemID;
  fbp->itemtype = OBJ_SEQFEAT;
  fbp->featdeftype = fcontext->featdeftype;
  ifp = (IntFeatBlockPtr) fbp;
  ifp->mapToNuc = FALSE;
  ifp->mapToProt = FALSE;
  ifp->mapToGen = FALSE;
  ifp->mapToMrna = FALSE;
  ifp->firstfeat = awp->firstfeat;
  awp->firstfeat = FALSE;

  /* optionally map CDS from cDNA onto genomic */

  if (awp->isGPS && ISA_na (bsp->mol) && awp->copyGpsCdsUp &&
      fcontext->featdeftype == FEATDEF_mRNA) {
    sip = SeqLocId (sfp->product);
    bsp = BioseqFind (sip);
    if (bsp != NULL && ISA_na (bsp->mol)) {
      cds = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &cdscontext);
      if (cds != NULL) {
        fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntCdsBlock));
        if (fbp != NULL) {

          fbp->entityID = cdscontext.entityID;
          fbp->itemID = cdscontext.itemID;
          fbp->itemtype = OBJ_SEQFEAT;
          fbp->featdeftype = cdscontext.featdeftype;
          ifp = (IntFeatBlockPtr) fbp;
          ifp->mapToNuc = FALSE;
          ifp->mapToProt = FALSE;
          ifp->mapToGen = TRUE;
          ifp->mapToMrna = FALSE;
          ifp->firstfeat = awp->firstfeat;
          awp->firstfeat = FALSE;
        }
      }
    }
  }

  if (fcontext->seqfeattype != SEQFEAT_CDREGION) return TRUE;

  /* if CDS, collect more information from product protein bioseq - may be part */

  sip = SeqLocId (sfp->product);
  bsp = BioseqFind (sip);
  if (bsp == NULL || (! ISA_aa (bsp->mol))) return TRUE;

  ifp->isCDS = TRUE;
  icp = (IntCdsBlockPtr) ifp;

  /* first explore pubs to pick up figure and maploc */

  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pub, &dcontext);
  while (sdp != NULL) {
    pdp = (PubdescPtr) sdp->data.ptrvalue;
    if (pdp != NULL) {
      if (icp->fig == NULL) {
        icp->fig = StringSaveNoNull (pdp->fig);
      }
      if (icp->maploc == NULL) {
        icp->maploc = StringSaveNoNull (pdp->maploc);
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_pub, &dcontext);
  }

  /* product may be segmented part, and remaining features are indexed on parent */

  parent = SeqMgrGetParentOfPart (bsp, NULL);
  if (parent != NULL) {
    bsp = parent;
  }

  /* then explore mat_peptides, sites, etc. */

  GetFeatsOnCdsProduct (sfp, bsp, awp);

  return TRUE;
}

static Boolean LIBCALLBACK GetFeatsOnSeg (
  SeqLocPtr slp,
  SeqMgrSegmentContextPtr context
)

{
  Asn2gbWorkPtr  awp;
  BioseqPtr      bsp = NULL;
  Uint2          entityID;
  Int4           from;
  SeqLocPtr      loc;
  SeqIdPtr       sip;
  Int4           to;

  if (slp == NULL || context == NULL) return FALSE;
  awp = (Asn2gbWorkPtr) context->userdata;

  from = awp->from;
  to = awp->from;

  sip = SeqLocId (slp);
  if (sip == NULL) {
    loc = SeqLocFindNext (slp, NULL);
    if (loc != NULL) {
      sip = SeqLocId (loc);
    }
  }
  if (sip == NULL) return TRUE;

  /* may remote fetch genome component if not already in memory */

  bsp = BioseqLockById (sip);

  if (bsp == NULL) return TRUE;

  entityID = ObjMgrGetEntityIDForPointer (bsp);

  if (entityID != awp->entityID) {

    /* if segment not packaged in record, may need to feature index it */

    if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
      SeqMgrIndexFeatures (entityID, NULL);
    }

    /* collect features indexed on the remote bioseq */

    awp->from = 0;
    awp->to = bsp->length - 1;
  }

  awp->lastsfp = NULL;
  awp->lastsap = NULL;
  awp->lastleft = 0;
  awp->lastright = 0;

  SeqMgrExploreFeatures (bsp, (Pointer) awp, GetFeatsOnBioseq, awp->slp, NULL, NULL);

  /* restore original from and to */

  awp->from = from;
  awp->to = to;

  BioseqUnlock (bsp);

  return TRUE;
}

static void AddFeatureBlock (
  Asn2gbWorkPtr awp
)

{
  BioseqPtr          bsp;
  SeqFeatPtr         cds;
  SeqMgrDescContext  dcontext;
  SeqMgrFeatContext  fcontext;
  FeatBlockPtr       fbp;
  SeqFeatPtr         gene;
  IntFeatBlockPtr    ifp;
  MolInfoPtr         mip;
  SeqFeatPtr         mrna;
  SeqDescrPtr        sdp;

  if (awp == NULL) return;
  bsp = awp->parent;
  if (bsp == NULL) return;

  awp->lastsfp = NULL;
  awp->lastsap = NULL;
  awp->lastleft = 0;
  awp->lastright = 0;

  /* optionally map gene from genomic onto cDNA */

  if (awp->isGPS && ISA_na (bsp->mol) && awp->copyGpsGeneDown) {
    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
    if (sdp != NULL && sdp->choice == Seq_descr_molinfo) {
      mip = (MolInfoPtr) sdp->data.ptrvalue;
      if (mip != NULL) {
        if (mip->biomol == MOLECULE_TYPE_MRNA) {
          mrna = SeqMgrGetRNAgivenProduct (bsp, NULL);
          if (mrna != NULL) {
            gene = SeqMgrGetOverlappingGene (mrna->location, &fcontext);
            if (gene != NULL && gene->data.choice == SEQFEAT_GENE) {

              fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntCdsBlock));
              if (fbp != NULL) {

                fbp->entityID = fcontext.entityID;
                fbp->itemID = fcontext.itemID;
                fbp->itemtype = OBJ_SEQFEAT;
                fbp->featdeftype = fcontext.featdeftype;
                ifp = (IntFeatBlockPtr) fbp;
                ifp->mapToNuc = FALSE;
                ifp->mapToProt = FALSE;
                ifp->mapToGen = FALSE;
                ifp->mapToMrna = TRUE;
                ifp->isCDS = TRUE;
                ifp->firstfeat = awp->firstfeat;
                awp->firstfeat = FALSE;
              }
            }
          }
        }
      }
    }
  }

  SeqMgrExploreFeatures (bsp, (Pointer) awp, GetFeatsOnBioseq, awp->slp, NULL, NULL);

  if (awp->format == GENPEPT_FMT && ISA_aa (bsp->mol)) {
    cds = SeqMgrGetCDSgivenProduct (bsp, &fcontext);
    if (cds != NULL && cds->data.choice == SEQFEAT_CDREGION) {

      fbp = (FeatBlockPtr) Asn2gbAddBlock (awp, FEATURE_BLOCK, sizeof (IntCdsBlock));
      if (fbp != NULL) {

        fbp->entityID = fcontext.entityID;
        fbp->itemID = fcontext.itemID;
        fbp->itemtype = OBJ_SEQFEAT;
        fbp->featdeftype = fcontext.featdeftype;
        ifp = (IntFeatBlockPtr) fbp;
        ifp->mapToNuc = FALSE;
        ifp->mapToProt = TRUE;
        ifp->mapToGen = FALSE;
        ifp->mapToMrna = FALSE;
        ifp->isCDS = TRUE;
        ifp->firstfeat = awp->firstfeat;
        awp->firstfeat = FALSE;
      }
    }
  }

  if (bsp->repr == Seq_repr_seg) {

    /* collect features on remote segments in MASTER_STYLE */

    SeqMgrExploreSegments (bsp, (Pointer) awp, GetFeatsOnSeg);
  }

}

static void AddBasecountBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr  bbp;

  if (awp == NULL) return;

  bbp = Asn2gbAddBlock (awp, BASECOUNT_BLOCK, sizeof (BaseBlock));
}

static void AddOriginBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr       bbp;
  BioseqPtr          bsp;
  Char               buf [67];
  SeqMgrDescContext  dcontext;
  GBBlockPtr         gbp;
  SeqDescrPtr        sdp;

  if (awp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) return;

  bbp = Asn2gbAddBlock (awp, ORIGIN_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  if (awp->format == GENBANK_FMT || awp->format == GENPEPT_FMT) {

    buf [0] = '\0';

    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_genbank, &dcontext);
    if (sdp != NULL) {
      gbp = (GBBlockPtr) sdp->data.ptrvalue;
      if (gbp != NULL && (! StringHasNoText (gbp->origin))) {
        StringNCpy_0 (buf, gbp->origin, sizeof (buf));
        bbp->entityID = dcontext.entityID;
        bbp->itemID = dcontext.itemID;
        bbp->itemtype = OBJ_SEQDESC;
      }
    }

    ff_StartPrint (0, 12, ASN2FF_GB_MAX, NULL);

    ff_AddString ("ORIGIN");

#ifndef WIN_MAC
      /* !!! To eliminate spurious diffs when comparing with Sequin exported GenBank format on Mac !!! */

      TabToColumn (13);
#endif

    if (! StringHasNoText (buf)) {
#ifdef WIN_MAC
      TabToColumn (13);
#endif

      gb_AddString (NULL, buf, NULL, TRUE, FALSE, TILDE_TO_SPACES);
    }
  }

  bbp->string = gb_MergeString (TRUE);
}

#define BASES_PER_BLOCK 1200

static void AddSequenceBlock (
  Asn2gbWorkPtr awp
)

{
  BioseqPtr    bsp;
  Int4         len;
  SeqBlockPtr  sbp;
  Int4         start;
  Int4         stop;

  if (awp == NULL) return;
  bsp = awp->bsp;
  if (bsp == NULL) return;

  if (awp->slp != NULL) {
    len = SeqLocLen (awp->slp);
  } else {
    len = bsp->length;
  }

  /* populate individual sequence blocks for given range */

  for (start = 0; start < len; start += BASES_PER_BLOCK) {
    sbp = (SeqBlockPtr) Asn2gbAddBlock (awp, SEQUENCE_BLOCK, sizeof (SeqBlock));
    if (sbp == NULL) continue;

    sbp->entityID = bsp->idx.entityID;
    sbp->itemID = bsp->idx.itemID;
    sbp->itemtype = OBJ_BIOSEQ;

    stop = start + BASES_PER_BLOCK;
    if (stop >= len) {
      stop = len;
    }

    sbp->start = start;
    sbp->stop = stop;
  }
}

static void AddContigBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr  bbp;

  if (awp == NULL) return;

  bbp = Asn2gbAddBlock (awp, CONTIG_BLOCK, sizeof (BaseBlock));
}

static void AddSlashBlock (
  Asn2gbWorkPtr awp
)

{
  BaseBlockPtr  bbp;

  if (awp == NULL) return;

  bbp = Asn2gbAddBlock (awp, SLASH_BLOCK, sizeof (BaseBlock));
  if (bbp == NULL) return;

  gb_StartPrint (awp->format, 0, 0, NULL, 0, 0, 0, NULL, FALSE);

  gb_AddString (NULL, "//", NULL, FALSE, FALSE, TILDE_IGNORE);

  bbp->string = gb_MergeString (TRUE);
}


/*--------------------------------------------------------*/
/*                                                        */
/*  s_LocusGetBaseName() -                                */
/*                                                        */
/*--------------------------------------------------------*/

static Boolean s_LocusGetBaseName (BioseqPtr parent, BioseqPtr segment, CharPtr baseName)
{
  Char          parentName[SEQID_MAX_LEN];
  Char          segName[SEQID_MAX_LEN];
  SeqIdPtr      sip;
  TextSeqIdPtr  tsip;
  Char          prefix[5];
  Char          bufTmp[SEQID_MAX_LEN];
  Int2          deleteChars;
  Int2          newLength;
  Int2          i;
  Int2          segNameLen;

  /* Get the parent Sequence ID */

  parentName [0] = '\0';
  sip = NULL;
  for (sip = parent->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GENBANK ||
        sip->choice == SEQID_EMBL ||
        sip->choice == SEQID_DDBJ) break;
    if (sip->choice == SEQID_TPG ||
        sip->choice == SEQID_TPE ||
        sip->choice == SEQID_TPD) break;
  }

  if (sip != NULL) {
    tsip = (TextSeqIdPtr) sip->data.ptrvalue;
    if (tsip != NULL && (! StringHasNoText (tsip->name))) {
      StringNCpy_0 (parentName, tsip->name, sizeof (parentName));
    }
  }

  if (StringHasNoText (parentName)) {
    StringNCpy_0 (parentName, baseName, sizeof (parentName));
  }

  /* Get segment id */

  segName [0] = '\0';
  segNameLen = 0;
  sip = NULL;
  for (sip = segment->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_GENBANK ||
        sip->choice == SEQID_EMBL ||
        sip->choice == SEQID_DDBJ) break;
    if (sip->choice == SEQID_TPG ||
        sip->choice == SEQID_TPE ||
        sip->choice == SEQID_TPD) break;
    }

  if (sip != NULL) {
    tsip = (TextSeqIdPtr) sip->data.ptrvalue;
    if (tsip != NULL && (! StringHasNoText (tsip->name))) {
      StringNCpy_0 (segName, tsip->name, sizeof (segName));
      segNameLen = StringLen(segName);
    }
  }

  /* If there's no "SEG_" prefix, then */
  /* just use the parent ID.           */

  StringNCpy_0 (prefix,parentName,sizeof (prefix));
  prefix[4] = '\0';
  if (StringCmp(prefix,"SEG_") != 0)
    {
      StringCpy(baseName,parentName);
      return FALSE;
    }

  /* Otherwise, eliminate the "SEG_" ... */

  StringCpy(bufTmp, &parentName[4]);
  StringCpy(parentName,bufTmp);

  /* ... And calculate a base name */

  if (segNameLen > 0 &&
      (segName[segNameLen-1] == '1') &&
      (StringLen(parentName) == segNameLen) &&
      (parentName[segNameLen-1] == segName[segNameLen-1]))
    {
      deleteChars = 1;
      for (i = segNameLen-2; i >= 0; i--)
    if (parentName[i] == '0')
      deleteChars++;
    else
      break;
      newLength = segNameLen - deleteChars;
      StringNCpy (parentName,segName,newLength); /* not StringNCpy_0 */
      parentName[newLength] = '\0';
    }

  /* Return the base name in the basename parameter */

  StringCpy(baseName,parentName);
  return TRUE;
}

/* ********************************************************************** */

/* DoOneSection builds a single report for one bioseq or segment */

static void DoOneSection (
  BioseqPtr target,
  BioseqPtr parent,
  BioseqPtr bsp,
  SeqLocPtr slp,
  Uint2 seg,
  Int4 from,
  Int4 to,
  Boolean contig,
  Asn2gbWorkPtr awp
)

{
  IntAsn2gbJobPtr      ajp;
  Asn2gbSectPtr        asp;
  SeqMgrBioseqContext  bcontext;
  BaseBlockPtr         PNTR blockArray;
  Boolean              hasRefs;
  Int4                 i;
  IntAsn2gbSectPtr     iasp;
  Int4                 numBlocks;
  Int4                 numsegs = 0;
  SeqIdPtr             sip;
  ValNodePtr           vnp;

  if (target == NULL || parent == NULL || bsp == NULL || awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;

  if (ajp->flags.suppressLocalID) {
    sip = SeqIdSelect (bsp->id, fasta_order, NUM_SEQID);
    if (sip == NULL || sip->choice == SEQID_LOCAL) return;
  }

  if (seg == 0) {
    awp->basename[0] = '\0';
  } else if (seg == 1) {
    s_LocusGetBaseName (parent, bsp, awp->basename);
  }

  asp = Asn2gbAddSection (awp);
  if (asp == NULL) return;

  if (SeqMgrGetBioseqContext (parent, &bcontext)) {
    numsegs = bcontext.numsegs;
  }

  /* set working data fields */

  awp->asp = asp;

  awp->target = target;
  awp->parent = parent;
  awp->bsp = bsp;
  awp->slp = slp;
  awp->seg = seg;
  awp->numsegs = numsegs;
  awp->from = from;
  awp->to = to;
  awp->contig = contig;

  awp->firstfeat = TRUE;

  /* initialize empty blockList for this section */

  awp->blockList = NULL;
  awp->lastblock = NULL;

  /* and store section data into section fields */

  asp->target = target;
  asp->bsp = bsp;
  asp->slp = slp;
  asp->seg = seg;
  asp->numsegs = numsegs;
  asp->from = from;
  asp->to = to;

  iasp = (IntAsn2gbSectPtr) asp;
  iasp->spp = NULL;

  asp->blockArray = NULL;
  asp->numBlocks = 0;

  /* start exploring and populating paragraphs */

  if (awp->format == FTABLE_FMT) {

    AddFeatHeaderBlock (awp);
    AddFeatureBlock (awp);

  } else {

    AddLocusBlock (awp);

    if (awp->format == GENBANK_FMT || awp->format == GENPEPT_FMT) {

      AddDeflineBlock (awp);
      AddAccessionBlock (awp);

      if (ISA_aa (bsp->mol)) {
        AddPidBlock (awp);
      }

      AddVersionBlock (awp);

      if (ISA_aa (bsp->mol)) {
        AddDbsourceBlock (awp);
      }

    } else if (awp->format == EMBL_FMT || awp->format == EMBLPEPT_FMT) {

      AddAccessionBlock (awp);

      if (ISA_na (bsp->mol)) {
        AddVersionBlock (awp);
      }

      if (ISA_aa (bsp->mol)) {
        /* AddPidBlock (awp); */
        /* AddDbsourceBlock (awp); */
      }

      AddDateBlock (awp);

      AddDeflineBlock (awp);
    }

    AddKeywordsBlock (awp);

    if (awp->format == GENBANK_FMT || awp->format == GENPEPT_FMT) {
      AddSegmentBlock (awp);
    }

    AddSourceBlock (awp);
    AddOrganismBlock (awp);

    /* !!! RELEASE_MODE should check return value of AddReferenceBlock !!! */

    hasRefs = AddReferenceBlock (awp);
    if (! hasRefs) {
      if (ajp->flags.needAtLeastOneRef) {
        awp->failed = TRUE;
      }
    }
    AddCommentBlock (awp);

    AddFeatHeaderBlock (awp);
    AddSourceFeatBlock (awp);

    if (contig) {
      if (awp->showconfeats) {
        AddFeatureBlock (awp);
      }
      AddContigBlock (awp);
    } else {
      AddFeatureBlock (awp);

      if (ISA_na (bsp->mol)) {
        AddBasecountBlock (awp);
      }
      AddOriginBlock (awp);

      AddSequenceBlock (awp);
    }

    AddSlashBlock (awp);
  }

  /* allocate block array for this section */

  numBlocks = ValNodeLen (awp->blockList);
  asp->numBlocks = numBlocks;

  if (numBlocks > 0) {
    blockArray = (BaseBlockPtr PNTR) MemNew (sizeof (BaseBlockPtr) * (numBlocks + 1));
    asp->blockArray = blockArray;

    if (blockArray != NULL) {
      for (vnp = awp->blockList, i = 0; vnp != NULL; vnp = vnp->next, i++) {
        blockArray [i] = (BaseBlockPtr) vnp->data.ptrvalue;
      }
    }
  }

  /* free blockList, but leave data, now pointed to by blockArray elements */

  awp->blockList = ValNodeFree (awp->blockList);
  awp->lastblock = NULL;

  (awp->currsection)++;
}


/*
the following functions handle various kinds of input, all calling
DoOneSection once for each component that gets its own report
*/

static Boolean LIBCALLBACK Asn2Seg (
  SeqLocPtr slp,
  SeqMgrSegmentContextPtr context
)

{
  Asn2gbWorkPtr  awp;
  BioseqPtr      bsp = NULL;
  Uint2          entityID;
  Int4           from;
  SeqLocPtr      loc;
  BioseqPtr      parent;
  SeqIdPtr       sip;
  Int4           to;

  if (slp == NULL || context == NULL) return FALSE;
  awp = (Asn2gbWorkPtr) context->userdata;

  parent = context->parent;

  from = context->cumOffset;
  to = from + context->to - context->from;

  sip = SeqLocId (slp);
  if (sip == NULL) {
    loc = SeqLocFindNext (slp, NULL);
    if (loc != NULL) {
      sip = SeqLocId (loc);
    }
  }
  if (sip == NULL) return TRUE;

  /* may remote fetch genome component if not already in memory */

  bsp = BioseqLockById (sip);

  if (bsp == NULL) return TRUE;

  entityID = ObjMgrGetEntityIDForPointer (bsp);

  if (entityID != awp->entityID) {

    /* if segment not packaged in record, may need to feature index it */

    if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
      SeqMgrIndexFeatures (entityID, NULL);
    }

    /* collect features indexed on the remote bioseq */

    parent = bsp;
    from = 0;
    to = bsp->length - 1;
  }

  DoOneSection (bsp, parent, bsp, /* slp */ NULL, context->index, from, to, FALSE, awp);

  BioseqUnlock (bsp);

  return TRUE;
}

static void DoOneBioseq (
  BioseqPtr bsp,
  Pointer userdata
)

{
  IntAsn2gbJobPtr       ajp;
  Asn2gbWorkPtr         awp;
  SeqMgrSegmentContext  context;
  Boolean               contig = FALSE;
  Int4                  from;
  BioseqPtr             parent;
  Boolean               segmented = FALSE;
  Int4                  to;

  if (bsp == NULL) return;
  awp = (Asn2gbWorkPtr) userdata;
  if (awp == NULL) return;
  ajp = awp->ajp;
  if (ajp == NULL) return;

  /* return if molecule not right for format */

  if (ISA_na (bsp->mol)) {
    if (ajp->format == GENPEPT_FMT || ajp->format == EMBLPEPT_FMT) return;
  } else if (ISA_aa (bsp->mol)) {
    if (ajp->format == GENBANK_FMT || ajp->format == EMBL_FMT) return;
  }

  if (awp->style == SEGMENTED_STYLE) {
    segmented = TRUE;
  }
  if (awp->style == CONTIG_STYLE) {
    contig = TRUE;
  }
  if (bsp->repr == Seq_repr_seg && awp->style == NORMAL_STYLE) {

    /* if bsp followed by parts set, then do not default to contig style */

    if (SegHasParts (bsp)) {
      segmented = TRUE;
      contig = FALSE;
    } else {
      segmented = FALSE;
      contig = TRUE;
    }
  }
  if (bsp->repr == Seq_repr_delta && awp->style == NORMAL_STYLE) {
    if (! DeltaLitOnly (bsp)) {
      contig = TRUE;
    }
  }

  if (bsp->repr == Seq_repr_seg) {

    /* this is a segmented bioseq */

    if (segmented) {

      /* show all segments individually */

      SeqMgrExploreSegments (bsp, (Pointer) awp, Asn2Seg);

    } else {

      /* show as single bioseq */

      parent = bsp;
      from = 0;
      to = bsp->length - 1;

      DoOneSection (parent, parent, bsp, ajp->ajp.slp, 0, from, to, contig, awp);
    }

  } else if (bsp->repr == Seq_repr_raw ||
             bsp->repr == Seq_repr_const ||
             bsp->repr == Seq_repr_delta ||
             bsp->repr == Seq_repr_virtual) {

    parent = SeqMgrGetParentOfPart (bsp, &context);
    if (parent != NULL) {

      /* this is a part of an indexed segmented bioseq */

      from = context.cumOffset;
      to = from + context.to - context.from;

    } else {

      /* this is a regular non-segmented bioseq */

      parent = bsp;
      from = 0;
      to = bsp->length - 1;
    }

    DoOneSection (parent, parent, bsp, ajp->ajp.slp, 0, from, to, contig, awp);
  }
}

static void DoBioseqSetList (
  SeqEntryPtr seq_set,
  Asn2gbWorkPtr awp
)

{
  BioseqSetPtr  bssp;
  SeqEntryPtr   sep;

  if (seq_set == NULL || awp == NULL) return;

  /* iterate rather than recurse unless multiple nested sets > nuc-prot */

  for (sep = seq_set; sep != NULL; sep = sep->next) {

    if (IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (bssp == NULL) continue;

      if (bssp->_class == BioseqseqSet_class_genbank ||
          bssp->_class == BioseqseqSet_class_mut_set ||
          bssp->_class == BioseqseqSet_class_pop_set ||
          bssp->_class == BioseqseqSet_class_phy_set ||
          bssp->_class == BioseqseqSet_class_eco_set ||
          bssp->_class == BioseqseqSet_class_gen_prod_set) {

        /* if popset within genbank set, for example, recurse */

        DoBioseqSetList (bssp->seq_set, awp);
        return;
      }
    }

    /* at most nuc-prot set, so do main bioseqs that fit the format */

    VisitSequencesInSep (sep, (Pointer) awp, VISIT_MAINS, DoOneBioseq);
  }
}

static void DoOneBioseqSet (
  SeqEntryPtr sep,
  Asn2gbWorkPtr awp
)

{
  BioseqSetPtr  bssp;

  if (sep == NULL || awp == NULL) return;

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;

    if (bssp->_class == BioseqseqSet_class_genbank ||
        bssp->_class == BioseqseqSet_class_mut_set ||
        bssp->_class == BioseqseqSet_class_pop_set ||
        bssp->_class == BioseqseqSet_class_phy_set ||
        bssp->_class == BioseqseqSet_class_eco_set ||
        bssp->_class == BioseqseqSet_class_gen_prod_set) {

      /* this is a pop/phy/mut/eco set, catenate separate reports */

      DoBioseqSetList (bssp->seq_set, awp);

      return;
    }
  }

  /* at most nuc-prot set, so do main bioseqs that fit the format */

  VisitSequencesInSep (sep, (Pointer) awp, VISIT_MAINS, DoOneBioseq);
}

/* ********************************************************************** */

/* public functions */

static int LIBCALLBACK SortParagraphByIDProc (
  VoidPtr vp1,
  VoidPtr vp2
)

{
  BaseBlockPtr  bbp1, bbp2;

  if (vp1 == NULL || vp2 == NULL) return 0;
  bbp1 = *((BaseBlockPtr PNTR) vp1);
  bbp2 = *((BaseBlockPtr PNTR) vp2);
  if (bbp1 == NULL || bbp2 == NULL) return 0;

  if (bbp1->entityID > bbp2->entityID) return 1;
  if (bbp1->entityID < bbp2->entityID) return -1;

  if (bbp1->itemtype > bbp2->itemtype) return 1;
  if (bbp1->itemtype < bbp2->itemtype) return -1;

  if (bbp1->itemID > bbp2->itemID) return 1;
  if (bbp1->itemID < bbp2->itemID) return -1;

  if (bbp1->paragraph > bbp2->paragraph) return 1;
  if (bbp1->paragraph < bbp2->paragraph) return -1;

  return 0;
}

typedef struct modeflags {
  Boolean  flags [19];
} ModeFlags, PNTR ModeFlagsPtr;

static ModeFlags flagTable [] = {

  /* RELEASE_MODE */
  {TRUE, TRUE, TRUE, TRUE, TRUE,
   TRUE, TRUE, TRUE, TRUE, TRUE,
   TRUE, TRUE, TRUE, TRUE, TRUE,
   TRUE, TRUE, TRUE, TRUE},

  /* ENTREZ_MODE */
  {FALSE, TRUE, TRUE, TRUE, TRUE,
   TRUE, TRUE, TRUE, TRUE, TRUE,
   TRUE, TRUE, TRUE, TRUE, TRUE,
   TRUE, TRUE, FALSE, FALSE},

  /* SEQUIN_MODE */
  {FALSE, FALSE, FALSE, FALSE, FALSE,
   FALSE, FALSE, TRUE, FALSE, FALSE,
   FALSE, FALSE, FALSE, FALSE, FALSE,
   FALSE, FALSE, FALSE, FALSE},

  /* DUMP_MODE */
  {FALSE, FALSE, FALSE, FALSE, FALSE,
   FALSE, FALSE, FALSE, FALSE, FALSE,
   FALSE, FALSE, FALSE, FALSE, FALSE,
   FALSE, FALSE, FALSE, FALSE}
};

static void SetFlagsFromMode (
  IntAsn2gbJobPtr  ajp,
  ModType mode
)

{
  BoolPtr       bp;
  ModeFlagsPtr  mfp;

  if (ajp == NULL) return;
  if (! (mode >= RELEASE_MODE && mode <= DUMP_MODE)) {
    mode = DUMP_MODE;
  }
  mfp = &(flagTable [(int) (mode - 1)]);
  bp = &(mfp->flags [0]);

  ajp->flags.suppressLocalID = *(bp++);
  ajp->flags.validateFeats = *(bp++);
  ajp->flags.ignorePatPubs = *(bp++);
  ajp->flags.dropShortAA = *(bp++);
  ajp->flags.avoidLocusColl = *(bp++);

  ajp->flags.iupacaaOnly = *(bp++);
  ajp->flags.dropBadCitGens = *(bp++);
  ajp->flags.noAffilOnUnpub = *(bp++);
  ajp->flags.dropIllegalQuals = *(bp++);
  ajp->flags.checkQualSyntax = *(bp++);

  ajp->flags.needRequiredQuals = *(bp++);
  ajp->flags.needOrganismQual = *(bp++);
  ajp->flags.needAtLeastOneRef = *(bp++);
  ajp->flags.citArtIsoJta = *(bp++);
  ajp->flags.dropBadDbxref = *(bp++);

  ajp->flags.useEmblMolType = *(bp++);
  ajp->flags.hideBankItComment = *(bp++);
  ajp->flags.checkCDSproductID = *(bp++);
  ajp->flags.forGbRelease = *(bp++);
}

NLM_EXTERN Asn2gbJobPtr asn2gnbk_setup (
  BioseqPtr bsp,
  BioseqSetPtr bssp,
  SeqLocPtr slp,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags
)

{
  IntAsn2gbJobPtr  ajp = NULL;
  Asn2gbSectPtr    asp;
  Asn2gbWork       aw;
  BaseBlockPtr     bbp;
  BaseBlockPtr     PNTR blockArray;
  Uint2            entityID = 0;
  Int4             i;
  Int4             j;
  Int4             k;
  Boolean          lockFarComp;
  Boolean          lockFarLocs;
  Boolean          lockFarProd;
  Int4             numBlocks;
  Int4             numSections;
  ObjMgrDataPtr    omdp;
  BaseBlockPtr     PNTR paragraphArray;
  BaseBlockPtr     PNTR paragraphByIDs;
  Int4             numParagraphs;
  Asn2gbSectPtr    PNTR sectionArray;
  SubmitBlockPtr   sbp;
  SeqEntryPtr      sep;
  SeqIntPtr        sintp;
  SeqSubmitPtr     ssp;
  BioseqSetPtr     topbssp;
  ValNodePtr       vnp;

  if (slp != NULL) {
    bsp = BioseqFindFromSeqLoc (slp);
    if (bsp == NULL) return NULL;

    /* if location is whole, generate normal bioseq report */

    if (slp->choice == SEQLOC_WHOLE) {
      slp = NULL;
    } else if (slp->choice == SEQLOC_INT) {
      sintp = (SeqIntPtr) slp->data.ptrvalue;
      if (sintp != NULL && sintp->from == 0 && sintp->to == bsp->length - 1) {
        slp = NULL;
      }
    }
  }

  if (bsp != NULL) {
    bssp = NULL;
    entityID = ObjMgrGetEntityIDForPointer (bsp);
  } else if (bssp != NULL) {
    entityID = ObjMgrGetEntityIDForPointer (bssp);
  }

  if (entityID == 0) return NULL;

  if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
    SeqMgrIndexFeatures (entityID, NULL);
  }

  ajp = (IntAsn2gbJobPtr) MemNew (sizeof (IntAsn2gbJob));
  if (ajp == NULL) return NULL;

  init_buff ();

  asn2ff_set_output (NULL, "\n");

  ajp->ajp.entityID = entityID;
  ajp->ajp.bsp = bsp;
  ajp->ajp.bssp = bssp;
  if (slp != NULL) {
    ajp->ajp.slp = AsnIoMemCopy ((Pointer) slp,
                                 (AsnReadFunc) SeqLocAsnRead,
                                 (AsnWriteFunc) SeqLocAsnWrite);
  } else {
    ajp->ajp.slp = NULL;
  }

  ajp->format = format;

  SetFlagsFromMode (ajp, mode);

  ajp->showFarTransl = (Boolean) ((flags & SHOW_FAR_TRANSLATION) != 0);
  ajp->transientSeqPort = (Boolean) ((flags & FREE_SEQPORT_EACH_TIME) != 0);

  ajp->masterStyle = (Boolean) (style == MASTER_STYLE);
  if (format == GENBANK_FMT || format == GENPEPT_FMT) {
    ajp->newSourceOrg = (Boolean) ((flags & USE_NEW_SOURCE_ORG) != 0);
  }

  lockFarComp = (Boolean) ((flags & LOCK_FAR_COMPONENTS) != 0);
  lockFarLocs = (Boolean) ((flags & LOCK_FAR_LOCATIONS) != 0);
  lockFarProd = (Boolean) ((flags & LOCK_FAR_PRODUCTS) != 0);

  if (lockFarComp || lockFarLocs || lockFarProd) {

    /* lock all bioseqs in advance, including remote genome components */

    sep = GetTopSeqEntryForEntityID (entityID);
    ajp->lockedBspList = LockFarComponentsEx (sep, lockFarComp, lockFarLocs, lockFarProd);
  }

  MemSet ((Pointer) (&aw), 0, sizeof (Asn2gbWork));
  aw.ajp = ajp;
  aw.entityID = entityID;

  aw.sectionList = NULL;
  aw.lastsection = NULL;

  aw.currsection = 0;
  aw.showconfeats = (Boolean) ((flags & SHOW_CONTIG_FEATURES) != 0);
  aw.showconsource = (Boolean) ((flags & SHOW_CONTIG_SOURCES) != 0);

  aw.newLocusLine = (Boolean) ((flags & USE_OLD_LOCUS_LINE) == 0);

  aw.isGPS = FALSE;
  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep != NULL && IS_Bioseq_set (sep)) {
    topbssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (topbssp != NULL && topbssp->_class == BioseqseqSet_class_gen_prod_set) {
      aw.isGPS = TRUE;
      aw.copyGpsCdsUp = (Boolean) ((flags & COPY_GPS_CDS_UP) != 0);
      aw.copyGpsGeneDown = (Boolean) ((flags & COPY_GPS_GENE_DOWN) != 0);
    }
  }

  if ((Boolean) (flags & DDJB_VARIANT_FORMAT) != 0) {
    aw.citSubsFirst = TRUE;
    aw.hideGeneFeats = TRUE;
    ajp->newSourceOrg = FALSE;
  }

  aw.format = format;
  aw.style = style;

  aw.hup = FALSE;
  aw.ssp = NULL;

  aw.failed = FALSE;

  omdp = ObjMgrGetData (entityID);
  if (omdp != NULL && omdp->datatype == OBJ_SEQSUB) {
    ssp = (SeqSubmitPtr) omdp->dataptr;
    if (ssp != NULL && ssp->datatype == 1) {
      aw.ssp = ssp;
      sbp = ssp->sub;
      if (sbp != NULL) {
        aw.hup = sbp->hup;
      }
    }
  }

  if (bssp != NULL) {

    /* handle all components of a pop/phy/mut/eco set */

    sep = SeqMgrGetSeqEntryForData (bssp);
    DoOneBioseqSet (sep, &aw);

  } else {

    /* handle single bioseq, which may be segmented or a local part */

    DoOneBioseq (bsp, &aw);
  }

  /* check for failure to populate anything */

  if (ajp->flags.needAtLeastOneRef && aw.failed) return asn2gnbk_cleanup ((Asn2gbJobPtr) ajp);

  numSections = ValNodeLen (aw.sectionList);
  ajp->ajp.numSections = numSections;

  if (numSections == 0) return asn2gnbk_cleanup ((Asn2gbJobPtr) ajp);

  /* allocate section array for this job */

  sectionArray = (Asn2gbSectPtr PNTR) MemNew (sizeof (Asn2gbSectPtr) * (numSections + 1));
  ajp->ajp.sectionArray = sectionArray;

  if (sectionArray == NULL) return asn2gnbk_cleanup ((Asn2gbJobPtr) ajp);

  /* fill in section and paragraph arrays */

  numParagraphs = 0;
  for (vnp = aw.sectionList, i = 0; vnp != NULL && i < numSections; vnp = vnp->next, i++) {
    asp = (Asn2gbSectPtr) vnp->data.ptrvalue;
    sectionArray [i] = asp;
    if (asp != NULL) {
      numParagraphs += asp->numBlocks;
    }
  }

  /* allocate paragraph array pointing to all blocks in all sections */

  ajp->ajp.numParagraphs = numParagraphs;
  if (numParagraphs == 0) return asn2gnbk_cleanup ((Asn2gbJobPtr) ajp);

  paragraphArray = (BaseBlockPtr PNTR) MemNew (sizeof (BaseBlockPtr) * (numParagraphs + 1));
  ajp->ajp.paragraphArray = paragraphArray;

  paragraphByIDs = (BaseBlockPtr PNTR) MemNew (sizeof (BaseBlockPtr) * (numParagraphs + 1));
  ajp->ajp.paragraphByIDs = paragraphByIDs;

  if (paragraphArray == NULL || paragraphByIDs == NULL) return asn2gnbk_cleanup ((Asn2gbJobPtr) ajp);

  k = 0;
  for (i = 0; i < numSections; i++) {
    asp = sectionArray [i];
    if (asp != NULL) {

      numBlocks = asp->numBlocks;
      blockArray = asp->blockArray;
      if (blockArray != NULL) {

        for (j = 0; j < numBlocks; j++) {
          bbp = blockArray [j];

          paragraphArray [k] = bbp;
          paragraphByIDs [k] = bbp;
          bbp->paragraph = k;
          k++;
        }
      }
    }
  }

  /* sort paragraphByIDs array by entityID/itemtype/itemID/paragraph */

  HeapSort (paragraphByIDs, (size_t) numParagraphs, sizeof (BaseBlockPtr), SortParagraphByIDProc);

  /* free sectionList, but leave data, now pointed to by sectionArray elements */

  ValNodeFree (aw.sectionList);

  return (Asn2gbJobPtr) ajp;
}

typedef CharPtr (*FormatProc) (Asn2gbFormatPtr afp, BaseBlockPtr bbp);

static FormatProc asn2gnbk_fmt_functions [22] = {
  NULL,
  DefaultFormatBlock, DefaultFormatBlock, DefaultFormatBlock,
  DefaultFormatBlock, DefaultFormatBlock, DefaultFormatBlock,
  DefaultFormatBlock, DefaultFormatBlock, DefaultFormatBlock,
  FormatSourceBlock, FormatOrganismBlock, FormatReferenceBlock,
  FormatCommentBlock, DefaultFormatBlock, FormatSourceFeatBlock,
  FormatFeatureBlock, FormatBasecountBlock, DefaultFormatBlock,
  FormatSequenceBlock, FormatContigBlock, DefaultFormatBlock
};

static void PrintFtableIntervals (
  ValNodePtr PNTR head,
  BioseqPtr target,
  SeqLocPtr location,
  CharPtr label
)

{
  SeqLocPtr  slp;
  Int4       start;
  Int4       stop;
  Char       str [64];

  if (head == NULL || target == NULL || location == NULL || label == NULL) return;

  slp = SeqLocFindNext (location, NULL);
  if (slp == NULL) return;

  start = GetOffsetInBioseq (slp, target, SEQLOC_START) + 1;
  stop = GetOffsetInBioseq (slp, target, SEQLOC_STOP) + 1;
  sprintf (str, "%ld\t%ld\t%s\n", (long) start, (long) stop, label);
  ValNodeCopyStr (head, 0, str);

  while ((slp = SeqLocFindNext (location, slp)) != NULL) {
    start = GetOffsetInBioseq (slp, target, SEQLOC_START) + 1;
    stop = GetOffsetInBioseq (slp, target, SEQLOC_STOP) + 1;
    if (start != 0 && stop != 0) {
      sprintf (str, "%ld\t%ld\n", (long) start, (long) stop);
      ValNodeCopyStr (head, 0, str);
    }
  }
}

static void PrintFtableLocAndQuals (
  IntAsn2gbJobPtr ajp,
  ValNodePtr PNTR head,
  BioseqPtr target,
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr context
)

{
  DbtagPtr     dbt;
  GBQualPtr    gbq;
  GeneRefPtr   grp;
  CharPtr      label;
  ObjectIdPtr  oip;
  BioseqPtr    prod;
  SeqFeatPtr   prot;
  ProtRefPtr   prp;
  RnaRefPtr    rrp;
  SeqIdPtr     sip;
  Char         str [256];
  Char         tmp [300];
  tRNAPtr      trna;
  ValNodePtr   vnp;

  if (head == NULL || target == NULL || sfp == NULL || context == NULL) return;
  label = (CharPtr) FeatDefTypeLabel (sfp);
  if (StringCmp (label, "Gene") == 0) {
    label = "gene";
  }
  if (StringHasNoText (label)) {
    label = "???";
  }

  PrintFtableIntervals (head, target, sfp->location, label);
  switch (context->seqfeattype) {
    case SEQFEAT_GENE :
      grp = (GeneRefPtr) sfp->data.value.ptrvalue;
      if (grp != NULL) {
        StringNCpy_0 (str, (CharPtr) grp->locus, sizeof (str));
        if (! StringHasNoText (str)) {
          sprintf (tmp, "\t\t\tgene\t%s\n", str);
          ValNodeCopyStr (head, 0, tmp);
        }
        for (vnp = grp->syn; vnp != NULL; vnp = vnp->next) {
          StringNCpy_0 (str, (CharPtr) vnp->data.ptrvalue, sizeof (str));
          if (! StringHasNoText (str)) {
            sprintf (tmp, "\t\t\tgene_syn\t%s\n", str);
            ValNodeCopyStr (head, 0, tmp);
          }
        }
      }
      break;
    case SEQFEAT_CDREGION :
      prod = BioseqFind (SeqLocId (sfp->product));
      prot = SeqMgrGetBestProteinFeature (prod, NULL);
      if (prot != NULL) {
        prp = (ProtRefPtr) prot->data.value.ptrvalue;
        if (prp != NULL) {
          if (prp->name != NULL) {
            for (vnp = prp->name; vnp != NULL; vnp = vnp->next) {
              StringNCpy_0 (str, (CharPtr) vnp->data.ptrvalue, sizeof (str));
              if (! StringHasNoText (str)) {
                sprintf (tmp, "\t\t\tproduct\t%s\n", str);
                ValNodeCopyStr (head, 0, tmp);
              }
            }
          }
          if (prp->desc != NULL) {
            StringNCpy_0 (str, prp->desc, sizeof (str));
            if (! StringHasNoText (str)) {
              sprintf (tmp, "\t\t\tprot_desc\t%s\n", str);
              ValNodeCopyStr (head, 0, tmp);
            }
          }
          for (vnp = prp->activity; vnp != NULL; vnp = vnp->next) {
            StringNCpy_0 (str, (CharPtr) vnp->data.ptrvalue, sizeof (str));
            if (! StringHasNoText (str)) {
              sprintf (tmp, "\t\t\tfunction\t%s\n", str);
              ValNodeCopyStr (head, 0, tmp);
            }
          }
          for (vnp = prp->ec; vnp != NULL; vnp = vnp->next) {
            StringNCpy_0 (str, (CharPtr) vnp->data.ptrvalue, sizeof (str));
            if (! StringHasNoText (str)) {
              sprintf (tmp, "\t\t\tEC_number\t%s\n", str);
              ValNodeCopyStr (head, 0, tmp);
            }
          }
        }
      }
      if (prod != NULL) {
        for (sip = prod->id; sip != NULL; sip = sip->next) {
          if (sip->choice == SEQID_GENBANK ||
              sip->choice == SEQID_EMBL ||
              sip->choice == SEQID_DDBJ ||
              sip->choice == SEQID_OTHER ||
              sip->choice == SEQID_TPG ||
              sip->choice == SEQID_TPE ||
              sip->choice == SEQID_TPD) {
            if (SeqIdWrite (sip, str, PRINTID_TEXTID_ACC_VER, sizeof (str)) != NULL) {
              sprintf (tmp, "\t\t\tprotein_id\t%s\n", str);
              ValNodeCopyStr (head, 0, tmp);
            }
          } else if (sip->choice == SEQID_LOCAL && (! ajp->flags.suppressLocalID)) {
            if (SeqIdWrite (sip, str, PRINTID_TEXTID_ACC_VER, sizeof (str)) != NULL) {
              sprintf (tmp, "\t\t\tprotein_id\tlcl|%s\n", str);
              ValNodeCopyStr (head, 0, tmp);
            }
          }
        }
      }
      break;
    case SEQFEAT_RNA :
      rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
      if (rrp != NULL) {
        switch (rrp->ext.choice) {
          case 1 :
            StringNCpy_0 (str, (CharPtr) rrp->ext.value.ptrvalue, sizeof (str));
            if (! StringHasNoText (str)) {
              sprintf (tmp, "\t\t\tproduct\t%s\n", str);
              ValNodeCopyStr (head, 0, tmp);
            }
            break;
          case 2 :
            trna = rrp->ext.value.ptrvalue;
            if (trna != NULL) {
              FeatDefLabel (sfp, str, sizeof (str) - 1, OM_LABEL_CONTENT);
              if (! StringHasNoText (str)) {
                sprintf (tmp, "\t\t\tproduct\t%s\n", str);
                ValNodeCopyStr (head, 0, tmp);
              }
            }
            break;
          default :
            break;
        }
      }
      break;
    default :
      break;
  }
  if (! StringHasNoText (sfp->comment)) {
    ValNodeCopyStr (head, 0, "\t\t\tnote\t");
    ValNodeCopyStr (head, 0, sfp->comment);
    ValNodeCopyStr (head, 0, "\n");
  }
  for (gbq = sfp->qual; gbq != NULL; gbq = gbq->next) {
    if (! StringHasNoText (gbq->qual)) {
      if (! StringHasNoText (gbq->val)) {
        sprintf (tmp, "\t\t\t%s\t%s\n", gbq->qual, gbq->val);
        ValNodeCopyStr (head, 0, tmp);
      }
    }
  }
  for (vnp = sfp->dbxref; vnp != NULL; vnp = vnp->next) {
    dbt = (DbtagPtr) vnp->data.ptrvalue;
    if (dbt != NULL) {
      if (! StringHasNoText (dbt->db)) {
        oip = dbt->tag;
        if (oip->str != NULL && (! StringHasNoText (oip->str))) {
          sprintf (tmp, "\t\t\tdb_xref\t%s:%s\n", dbt->db, oip->str);
          ValNodeCopyStr (head, 0, tmp);
        } else {
          sprintf (tmp, "\t\t\tdb_xref\t%s:%ld\n", dbt->db, (long) oip->id);
          ValNodeCopyStr (head, 0, tmp);
        }
      }
    }
  }
}

NLM_EXTERN CharPtr asn2gnbk_format (
  Asn2gbJobPtr ajp,
  Int4 paragraph
)

{
  Asn2gbFormat       af;
  Asn2gbSectPtr      asp;
  BaseBlockPtr       bbp;
  BlockType          blocktype;
  SeqMgrFeatContext  fcontext;
  FormatProc         fmt;
  ValNodePtr         head;
  IntAsn2gbJobPtr    iajp;
  Char               id [42];
  size_t             max;
  QualValPtr         qv;
  Int4               section;
  SeqFeatPtr         sfp;
  SeqIdPtr           sip;
  CharPtr            str = NULL;
  BioseqPtr          target;
  Char               tmp [53];

  /* qv must hold MAX (ASN2GNBK_TOTAL_SOURCE, ASN2GNBK_TOTAL_FEATUR) */

  iajp = (IntAsn2gbJobPtr) ajp;
  if (iajp == NULL || ajp->sectionArray == NULL || ajp->paragraphArray == NULL) return NULL;
  if (paragraph < 0 || paragraph >= ajp->numParagraphs) return NULL;

  bbp = ajp->paragraphArray [paragraph];
  if (bbp == NULL) return NULL;

  section = bbp->section;
  if (section < 0 || section >= ajp->numSections) return NULL;

  asp = ajp->sectionArray [section];
  if (asp == NULL) return NULL;

  blocktype = bbp->blocktype;
  if (blocktype < LOCUS_BLOCK || blocktype > SLASH_BLOCK) return NULL;

  max = (size_t) (MAX (ASN2GNBK_TOTAL_SOURCE, ASN2GNBK_TOTAL_FEATUR));
  qv = MemNew (sizeof (QualVal) * (max + 5));
  if (qv == NULL) return NULL;

  af.ajp = (IntAsn2gbJobPtr) ajp;
  af.asp = asp;
  af.qvp = qv;
  af.format = iajp->format;

  if (iajp->format != FTABLE_FMT) {
    fmt = asn2gnbk_fmt_functions [(int) blocktype];
    if (fmt == NULL) return NULL;
    str = fmt (&af, bbp);

  } else {

    target = asp->target;
    if (target != NULL) {

      if (blocktype == FEATHEADER_BLOCK) {
        sip = SeqIdFindBest (target->id, 0);
        SeqIdWrite (sip, id, PRINTID_FASTA_LONG, sizeof (id) - 1);
        if (! StringHasNoText (id)) {
          sprintf (tmp, ">Feature %s\n", id);
          str = StringSave (tmp);
        }

      } else if (blocktype == FEATURE_BLOCK) {

        sfp = SeqMgrGetDesiredFeature (bbp->entityID, NULL, bbp->itemID, 0, NULL, &fcontext);
        if (sfp != NULL) {
          head = NULL;
          PrintFtableLocAndQuals (af.ajp, &head, target, sfp, &fcontext);
          str = MergeValNodeStrings (head);
          ValNodeFreeData (head);
        }
      }
    }
  }

  if (str == NULL) {
    str = StringSave ("???\n");
  }

  MemFree (qv);

  return str;
}

NLM_EXTERN Asn2gbJobPtr asn2gnbk_cleanup (
  Asn2gbJobPtr ajp
)

{
  Asn2gbSectPtr     asp;
  BaseBlockPtr      bbp;
  BaseBlockPtr      PNTR blockArray;
  Int4              i;
  IntAsn2gbJobPtr   iajp;
  IntAsn2gbSectPtr  iasp;
  IntCdsBlockPtr    icp;
  IntFeatBlockPtr   ifp;
  IntRefBlockPtr    irp;
  Int4              j;
  Int4              numBlocks;
  Int4              numSections;
  RefBlockPtr       rrp;
  Asn2gbSectPtr     PNTR sectionArray;

  iajp = (IntAsn2gbJobPtr) ajp;
  if (iajp == NULL) return NULL;

  SeqLocFree (iajp->ajp.slp);

  numSections = ajp->numSections;
  sectionArray = ajp->sectionArray;

  if (sectionArray != NULL) {

    for (i = 0; i < numSections; i++) {
      asp = sectionArray [i];
      if (asp != NULL) {
        iasp = (IntAsn2gbSectPtr) asp;

        numBlocks = asp->numBlocks;
        blockArray = asp->blockArray;
        if (blockArray != NULL) {

          for (j = 0; j < numBlocks; j++) {
            bbp = blockArray [j];
            if (bbp != NULL) {

              MemFree (bbp->string);

              if (bbp->blocktype == REFERENCE_BLOCK) {
                rrp = (RefBlockPtr) bbp;
                MemFree (rrp->uniquestr);
                irp = (IntRefBlockPtr) rrp;
                DateFree (irp->date);
                SeqLocFree (irp->loc);
                MemFree (irp->authstr);
                MemFree (irp->fig);
                MemFree (irp->maploc);

              } else if (bbp->blocktype == FEATURE_BLOCK) {

                ifp = (IntFeatBlockPtr) bbp;
                if (ifp->isCDS) {
                  icp = (IntCdsBlockPtr) ifp;
                  MemFree (icp->fig);
                  MemFree (icp->maploc);
                }
              }

              MemFree (bbp);
            }
          }
        }
        MemFree (asp->blockArray);
        MemFree (asp->referenceArray);
        SeqPortFree (iasp->spp);
      }
    }
  }

  MemFree (ajp->sectionArray);
  MemFree (ajp->paragraphArray);
  MemFree (ajp->paragraphByIDs);

  if (iajp->lockedBspList != NULL) {
    UnlockFarComponents (iajp->lockedBspList);
  }

  free_buff ();

  MemFree (iajp);

  return NULL;
}

NLM_EXTERN Boolean SeqEntryToGnbk (
  SeqEntryPtr sep,
  SeqLocPtr slp,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags,
  FILE *fp
)

{
  Asn2gbJobPtr  ajp;
  BioseqPtr     bsp = NULL;
  BioseqSetPtr  bssp = NULL;
  Int4          i;
  Boolean       is_html;
  Int4          numParagraphs;
  CharPtr       str;
#ifdef WIN_MAC
#if __profile__
  ValNodePtr    bsplist = NULL;
  Uint2         entityID;
  Boolean       lockFarComp;
  Boolean       lockFarLocs;
  Boolean       lockFarProd;
#endif
#endif

  if (fp == NULL) return FALSE;
  if (sep == NULL && slp == NULL) return FALSE;
  if (sep != NULL) {
    if (IS_Bioseq (sep)) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
    } else if (IS_Bioseq_set (sep)) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
    }
  }

#ifdef WIN_MAC
#if __profile__
  /* this allows profiling of just the formatter, without feature indexing, on the Mac */

  if (sep != NULL) {
    entityID = ObjMgrGetEntityIDForPointer (sep->data.ptrvalue);
    if (SeqMgrFeaturesAreIndexed (entityID) == 0) {
      SeqMgrIndexFeatures (entityID, NULL);
    }
  }

  lockFarComp = (Boolean) ((flags & LOCK_FAR_COMPONENTS) != 0);
  lockFarLocs = (Boolean) ((flags & LOCK_FAR_LOCATIONS) != 0);
  lockFarProd = (Boolean) ((flags & LOCK_FAR_PRODUCTS) != 0);

  if (lockFarComp || lockFarLocs || lockFarProd) {
    flags = flags ^ (LOCK_FAR_COMPONENTS | LOCK_FAR_LOCATIONS | LOCK_FAR_PRODUCTS);
    bsplist = LockFarComponentsEx (sep, lockFarComp, lockFarLocs, lockFarProd);
  }

  ProfilerSetStatus (TRUE);
#endif
#endif

  is_html = (Boolean) ((flags & CREATE_HTML_FLATFILE) != 0);
  if (is_html) {
    init_www ();
  }

  ajp = asn2gnbk_setup (bsp, bssp, slp, format, mode, style, flags);

  if (ajp != NULL) {

    if (is_html) {
      head_www (fp, sep);
    }

    numParagraphs = ajp->numParagraphs;
    for (i = 0; i < numParagraphs; i++) {
      str = asn2gnbk_format (ajp, i);
      if (str != NULL) {
        fprintf (fp, "%s", str);
      } else {
        fprintf (fp, "?\n");
      }
      MemFree (str);
    }

    asn2gnbk_cleanup (ajp);

    if (is_html) {
      tail_www (fp);
    }
  }

  if (is_html) {
    fini_www ();
  }

#ifdef WIN_MAC
#if __profile__
  ProfilerSetStatus (FALSE);

  UnlockFarComponents (bsplist);
#endif
#endif

  return TRUE;
}

NLM_EXTERN Boolean BioseqToGnbk (
  BioseqPtr bsp,
  SeqLocPtr slp,
  FmtType format,
  ModType mode,
  StlType style,
  FlgType flags,
  FILE *fp
)

{
  SeqEntryPtr  sep = NULL;

  if (bsp == NULL && slp == NULL) return FALSE;
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
  }
  return SeqEntryToGnbk (sep, slp, format, mode, style, flags, fp);
}

