/*   asn2gnb4.c
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
* File Name:  asn2gnb4.c
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov, Jonathan Kans,
*          Mati Shomrat
*
* Version Creation Date:   10/21/98
*
* $Revision: 1.51 $
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
#include <objpubme.h>
#include <seqport.h>
#include <sequtil.h>
#include <sqnutils.h>
#include <subutil.h>
#include <tofasta.h>
#include <explore.h>
#include <gbfeat.h>
#include <gbftdef.h>
#include <edutil.h>
#include <alignmgr2.h>
#include <asn2gnbi.h>

#ifdef WIN_MAC
#if __profile__
#include <Profiler.h>
#endif
#endif

/* ordering arrays for qualifiers and note components */

static FtQualType feat_qual_order [] = {
  FTQUAL_partial,
  FTQUAL_gene,

  FTQUAL_locus_tag,
  FTQUAL_old_locus_tag,
  FTQUAL_gene_syn_refseq,

  FTQUAL_gene_allele,

  FTQUAL_operon,

  FTQUAL_product,

  FTQUAL_prot_EC_number,
  FTQUAL_prot_activity,

  FTQUAL_standard_name,
  FTQUAL_coded_by,
  FTQUAL_derived_from,

  FTQUAL_prot_name,
  FTQUAL_region_name,
  FTQUAL_bond_type,
  FTQUAL_site_type,
  FTQUAL_sec_str_type,
  FTQUAL_heterogen,

  FTQUAL_note, 
  FTQUAL_citation,

  FTQUAL_number,

  FTQUAL_pseudo,
  FTQUAL_selenocysteine,

  FTQUAL_codon_start,

  FTQUAL_anticodon,
  FTQUAL_bound_moiety,
  FTQUAL_clone,
  FTQUAL_compare,
  FTQUAL_cons_splice,
  FTQUAL_direction,
  FTQUAL_function,
  FTQUAL_evidence,
  FTQUAL_exception,
  FTQUAL_frequency,
  FTQUAL_EC_number,
  FTQUAL_gene_map,
  FTQUAL_estimated_length,
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
  FTQUAL_extra_products,
  FTQUAL_protein_id,
  FTQUAL_transcript_id,
  FTQUAL_db_xref, 
  FTQUAL_gene_xref,
  FTQUAL_translation,
  FTQUAL_transcription,
  FTQUAL_peptide,
  (FtQualType) 0
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

static FtQualType feat_note_order [] = {
  FTQUAL_transcript_id_note, /* !!! remove October 15, 2003 !!! */
  FTQUAL_gene_desc,
  FTQUAL_gene_syn,
  FTQUAL_trna_codons,
  FTQUAL_encodes,
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
  FTQUAL_selenocysteine_note,
  FTQUAL_prot_names,
  FTQUAL_bond,
  FTQUAL_site,
  FTQUAL_rrna_its,
  FTQUAL_xtra_prod_quals,
  FTQUAL_modelev,
  /* GO terms appear as own qualifiers in RefSeq records, Sequin or Dump mode */
  FTQUAL_go_component,
  FTQUAL_go_function,
  FTQUAL_go_process,
  (FtQualType) 0
};

typedef struct featurqual {
  CharPtr   name;
  QualType  qualclass;
} FeaturQual, PNTR FeaturQualPtr;

static FeaturQual asn2gnbk_featur_quals [ASN2GNBK_TOTAL_FEATUR] = {
  { "",                 Qual_class_ignore        },
  { "allele",           Qual_class_quote         },
  { "anticodon",        Qual_class_anti_codon    },
  { "bond",             Qual_class_bond          },
  { "bond_type",        Qual_class_bond          },
  { "bound_moiety",     Qual_class_quote         },
  { "product",          Qual_class_string        },
  { "citation",         Qual_class_pubset        },
  { "clone",            Qual_class_quote         },
  { "coded_by",         Qual_class_seq_loc       },
  { "compare",          Qual_class_compare       },
  { "codon",            Qual_class_codon         },
  { "codon_start",      Qual_class_int           },
  { "cons_splice",      Qual_class_consplice     },
  { "db_xref",          Qual_class_db_xref       },
  { "derived_from",     Qual_class_seq_loc       },
  { "direction",        Qual_class_L_R_B         },
  { "EC_number",        Qual_class_EC_quote      },
  { "encodes",          Qual_class_encodes       },
  { "estimated_length", Qual_class_number        },
  { "evidence",         Qual_class_evidence      },
  { "exception",        Qual_class_string        },
  { "exception_note",   Qual_class_string        },
  { "product",          Qual_class_valnode       },
  { "figure",           Qual_class_string        },
  { "frequency",        Qual_class_quote         },
  { "function",         Qual_class_quote         },
  { "gene",             Qual_class_sgml          },
  { "gene_desc",        Qual_class_string        },
  { "allele",           Qual_class_string        },
  { "map",              Qual_class_string        },
  { "gene_syn",         Qual_class_gene_syn      },
  { "synonym",          Qual_class_gene_syn      },
  { "gene_note",        Qual_class_string        },
  { "db_xref",          Qual_class_db_xref       },
  { "go_component",     Qual_class_go            },
  { "go_function",      Qual_class_go            },
  { "go_process",       Qual_class_go            },
  { "heterogen",        Qual_class_string        },
  { "illegal",          Qual_class_illegal       },
  { "insertion_seq",    Qual_class_quote         },
  { "label",            Qual_class_label         },
  { "locus_tag",        Qual_class_locus_tag     },
  { "map",              Qual_class_quote         },
  { "maploc",           Qual_class_string        },
  { "mod_base",         Qual_class_noquote       },
  { "model_evidence",   Qual_class_model_ev      },
  { "note",             Qual_class_note          },
  { "number",           Qual_class_number        },
  { "old_locus_tag",    Qual_class_paren         },
  { "operon",           Qual_class_quote         },
  { "organism",         Qual_class_quote         },
  { "partial",          Qual_class_boolean       },
  { "PCR_conditions",   Qual_class_quote         },
  { "peptide",          Qual_class_peptide       },
  { "phenotype",        Qual_class_quote         },
  { "product",          Qual_class_product       },
  { "product",          Qual_class_quote         },
  { "function",         Qual_class_valnode       },
  { "prot_comment",     Qual_class_string        },
  { "EC_number",        Qual_class_EC_valnode    },
  { "prot_note",        Qual_class_string        },
  { "prot_method",      Qual_class_method        },
  { "prot_conflict",    Qual_class_string        },
  { "prot_desc",        Qual_class_string        },
  { "prot_missing",     Qual_class_string        },
  { "name",             Qual_class_tilde         },
  { "prot_names",       Qual_class_protnames     },
  { "protein_id",       Qual_class_seq_id        },
  { "pseudo",           Qual_class_boolean       },
  { "region",           Qual_class_region        },
  { "region_name",      Qual_class_string        },
  { "replace",          Qual_class_replace       },
  { "rpt_family",       Qual_class_quote         },
  { "rpt_type",         Qual_class_rpt           },
  { "rpt_unit",         Qual_class_rpt_unit      },
  { "rrna_its",         Qual_class_its           },
  { "sec_str_type",     Qual_class_sec_str       },
  { "selenocysteine",   Qual_class_boolean       },
  { "selenocysteine",   Qual_class_string        },
  { "seqfeat_note",     Qual_class_string        },
  { "site",             Qual_class_site          },
  { "site_type",        Qual_class_site          },
  { "standard_name",    Qual_class_quote         },
  { "transcription",    Qual_class_transcription },
  { "transcript_id",    Qual_class_seq_id        },
  { "tscpt_id_note",    Qual_class_seq_id        }, /* !!! remove October 15, 2003 !!! */
  { "transl_except",    Qual_class_code_break    },
  { "transl_table",     Qual_class_int           },
  { "translation",      Qual_class_translation   },
  { "transposon",       Qual_class_quote         },
  { "trna_aa",          Qual_class_ignore        },
  { "trna_codons",      Qual_class_trna_codons   },
  { "usedin",           Qual_class_usedin        },
  { "xtra_products",    Qual_class_xtraprds      }
};


typedef struct qualfeatur {
  CharPtr     name;
  FtQualType  featurclass;
} QualFeatur, PNTR QualFeaturPtr;

#define NUM_GB_QUALS 29

static QualFeatur qualToFeature [NUM_GB_QUALS] = {
  { "allele",           FTQUAL_allele           },
  { "bound_moiety",     FTQUAL_bound_moiety     },
  { "clone",            FTQUAL_clone            },
  { "codon",            FTQUAL_codon            },
  { "compare",          FTQUAL_compare          },
  { "cons_splice",      FTQUAL_cons_splice      },
  { "direction",        FTQUAL_direction        },
  { "EC_number",        FTQUAL_EC_number        },
  { "estimated_length", FTQUAL_estimated_length },
  { "frequency",        FTQUAL_frequency        },
  { "function",         FTQUAL_function         },
  { "insertion_seq",    FTQUAL_insertion_seq    },
  { "label",            FTQUAL_label            },
  { "map",              FTQUAL_map              },
  { "mod_base",         FTQUAL_mod_base         },
  { "number",           FTQUAL_number           },
  { "old_locus_tag",    FTQUAL_old_locus_tag    },
  { "operon",           FTQUAL_operon           },
  { "organism",         FTQUAL_organism         },
  { "PCR_conditions",   FTQUAL_PCR_conditions   },
  { "phenotype",        FTQUAL_phenotype        },
  { "product",          FTQUAL_product_quals    },
  { "replace",          FTQUAL_replace          },
  { "rpt_family",       FTQUAL_rpt_family       },
  { "rpt_type",         FTQUAL_rpt_type         },
  { "rpt_unit",         FTQUAL_rpt_unit         },
  { "standard_name",    FTQUAL_standard_name    },
  { "transposon",       FTQUAL_transposon       },
  { "usedin",           FTQUAL_usedin           }
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
  "tRNA-TERM",
  NULL
};

static CharPtr evidenceText [] = {
  NULL, "experimental", "not_experimental"
};

NLM_EXTERN CharPtr secStrText [] = {
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
    if ( /*index >= 0 && */ index < table->num) {
      return (table->names) [index];
    }
  }

  return oops;
}

NLM_EXTERN CharPtr Get3LetterSymbol (
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

NLM_EXTERN Int2 MatchRef (
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

NLM_EXTERN CharPtr bondList [] = {
  NULL,
  "disulfide",
  "thiolester",
  "xlink",
  "thioether",
  "unclassified"
};

NLM_EXTERN CharPtr siteList [] = {
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
  "nitrosylation",
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

  { FEATDEF_GENE , FTQUAL_allele },
  { FEATDEF_GENE , FTQUAL_function },
  { FEATDEF_GENE , FTQUAL_label },
  { FEATDEF_GENE , FTQUAL_map },
  { FEATDEF_GENE , FTQUAL_old_locus_tag },
  { FEATDEF_GENE , FTQUAL_operon },
  { FEATDEF_GENE , FTQUAL_phenotype },
  { FEATDEF_GENE , FTQUAL_product },
  { FEATDEF_GENE , FTQUAL_standard_name },
  { FEATDEF_GENE , FTQUAL_usedin },

  { FEATDEF_CDS , FTQUAL_allele },
  { FEATDEF_CDS , FTQUAL_codon },
  { FEATDEF_CDS , FTQUAL_label },
  { FEATDEF_CDS , FTQUAL_map },
  { FEATDEF_CDS , FTQUAL_number },
  { FEATDEF_CDS , FTQUAL_old_locus_tag },
  { FEATDEF_CDS , FTQUAL_operon },
  { FEATDEF_CDS , FTQUAL_standard_name },
  { FEATDEF_CDS , FTQUAL_usedin },

  { FEATDEF_PROT , FTQUAL_product },

  { FEATDEF_preRNA , FTQUAL_allele },
  { FEATDEF_preRNA , FTQUAL_function },
  { FEATDEF_preRNA , FTQUAL_label },
  { FEATDEF_preRNA , FTQUAL_map },
  { FEATDEF_preRNA , FTQUAL_old_locus_tag },
  { FEATDEF_preRNA , FTQUAL_operon },
  { FEATDEF_preRNA , FTQUAL_product },
  { FEATDEF_preRNA , FTQUAL_standard_name },
  { FEATDEF_preRNA , FTQUAL_usedin },

  { FEATDEF_mRNA , FTQUAL_allele },
  { FEATDEF_mRNA , FTQUAL_function },
  { FEATDEF_mRNA , FTQUAL_label },
  { FEATDEF_mRNA , FTQUAL_map },
  { FEATDEF_mRNA , FTQUAL_old_locus_tag },
  { FEATDEF_mRNA , FTQUAL_operon },
  { FEATDEF_mRNA , FTQUAL_product },
  { FEATDEF_mRNA , FTQUAL_standard_name },
  { FEATDEF_mRNA , FTQUAL_usedin },

  { FEATDEF_tRNA , FTQUAL_allele },
  { FEATDEF_tRNA , FTQUAL_function },
  { FEATDEF_tRNA , FTQUAL_label },
  { FEATDEF_tRNA , FTQUAL_map },
  { FEATDEF_tRNA , FTQUAL_old_locus_tag },
  { FEATDEF_tRNA , FTQUAL_product },
  { FEATDEF_tRNA , FTQUAL_standard_name },
  { FEATDEF_tRNA , FTQUAL_usedin },

  { FEATDEF_rRNA , FTQUAL_allele },
  { FEATDEF_rRNA , FTQUAL_function },
  { FEATDEF_rRNA , FTQUAL_label },
  { FEATDEF_rRNA , FTQUAL_map },
  { FEATDEF_rRNA , FTQUAL_old_locus_tag },
  { FEATDEF_rRNA , FTQUAL_product },
  { FEATDEF_rRNA , FTQUAL_standard_name },
  { FEATDEF_rRNA , FTQUAL_usedin },

  { FEATDEF_snRNA , FTQUAL_allele },
  { FEATDEF_snRNA , FTQUAL_function },
  { FEATDEF_snRNA , FTQUAL_label },
  { FEATDEF_snRNA , FTQUAL_map },
  { FEATDEF_snRNA , FTQUAL_old_locus_tag },
  { FEATDEF_snRNA , FTQUAL_product },
  { FEATDEF_snRNA , FTQUAL_standard_name },
  { FEATDEF_snRNA , FTQUAL_usedin },

  { FEATDEF_scRNA , FTQUAL_allele },
  { FEATDEF_scRNA , FTQUAL_function },
  { FEATDEF_scRNA , FTQUAL_label },
  { FEATDEF_scRNA , FTQUAL_map },
  { FEATDEF_scRNA , FTQUAL_old_locus_tag },
  { FEATDEF_scRNA , FTQUAL_product },
  { FEATDEF_scRNA , FTQUAL_standard_name },
  { FEATDEF_scRNA , FTQUAL_usedin },

  { FEATDEF_otherRNA , FTQUAL_allele },
  { FEATDEF_otherRNA , FTQUAL_function },
  { FEATDEF_otherRNA , FTQUAL_label },
  { FEATDEF_otherRNA , FTQUAL_map },
  { FEATDEF_otherRNA , FTQUAL_old_locus_tag },
  { FEATDEF_otherRNA , FTQUAL_operon },
  { FEATDEF_otherRNA , FTQUAL_product },
  { FEATDEF_otherRNA , FTQUAL_standard_name },
  { FEATDEF_otherRNA , FTQUAL_usedin },

  { FEATDEF_attenuator , FTQUAL_allele },
  { FEATDEF_attenuator , FTQUAL_label },
  { FEATDEF_attenuator , FTQUAL_map },
  { FEATDEF_attenuator , FTQUAL_old_locus_tag },
  { FEATDEF_attenuator , FTQUAL_operon },
  { FEATDEF_attenuator , FTQUAL_phenotype },
  { FEATDEF_attenuator , FTQUAL_usedin },

  { FEATDEF_C_region , FTQUAL_allele },
  { FEATDEF_C_region , FTQUAL_label },
  { FEATDEF_C_region , FTQUAL_map },
  { FEATDEF_C_region , FTQUAL_old_locus_tag },
  { FEATDEF_C_region , FTQUAL_product },
  { FEATDEF_C_region , FTQUAL_standard_name },
  { FEATDEF_C_region , FTQUAL_usedin },

  { FEATDEF_CAAT_signal , FTQUAL_allele },
  { FEATDEF_CAAT_signal , FTQUAL_label },
  { FEATDEF_CAAT_signal , FTQUAL_map },
  { FEATDEF_CAAT_signal , FTQUAL_old_locus_tag },
  { FEATDEF_CAAT_signal , FTQUAL_usedin },

  { FEATDEF_Imp_CDS , FTQUAL_codon },
  { FEATDEF_Imp_CDS , FTQUAL_EC_number },
  { FEATDEF_Imp_CDS , FTQUAL_function },
  { FEATDEF_Imp_CDS , FTQUAL_label },
  { FEATDEF_Imp_CDS , FTQUAL_map },
  { FEATDEF_Imp_CDS , FTQUAL_number },
  { FEATDEF_Imp_CDS , FTQUAL_old_locus_tag },
  { FEATDEF_Imp_CDS , FTQUAL_operon },
  { FEATDEF_Imp_CDS , FTQUAL_product },
  { FEATDEF_Imp_CDS , FTQUAL_standard_name },
  { FEATDEF_Imp_CDS , FTQUAL_usedin },

  { FEATDEF_conflict , FTQUAL_allele },
  { FEATDEF_conflict , FTQUAL_compare },
  { FEATDEF_conflict , FTQUAL_label },
  { FEATDEF_conflict , FTQUAL_map },
  { FEATDEF_conflict , FTQUAL_old_locus_tag },
  { FEATDEF_conflict , FTQUAL_replace },
  { FEATDEF_conflict , FTQUAL_usedin },

  { FEATDEF_D_loop , FTQUAL_allele },
  { FEATDEF_D_loop , FTQUAL_label },
  { FEATDEF_D_loop , FTQUAL_map },
  { FEATDEF_D_loop , FTQUAL_old_locus_tag },
  { FEATDEF_D_loop , FTQUAL_usedin },

  { FEATDEF_D_segment , FTQUAL_allele },
  { FEATDEF_D_segment , FTQUAL_label },
  { FEATDEF_D_segment , FTQUAL_map },
  { FEATDEF_D_segment , FTQUAL_old_locus_tag },
  { FEATDEF_D_segment , FTQUAL_product },
  { FEATDEF_D_segment , FTQUAL_standard_name },
  { FEATDEF_D_segment , FTQUAL_usedin },

  { FEATDEF_enhancer , FTQUAL_allele },
  { FEATDEF_enhancer , FTQUAL_label },
  { FEATDEF_enhancer , FTQUAL_map },
  { FEATDEF_enhancer , FTQUAL_old_locus_tag },
  { FEATDEF_enhancer , FTQUAL_standard_name },
  { FEATDEF_enhancer , FTQUAL_usedin },

  { FEATDEF_exon , FTQUAL_allele },
  { FEATDEF_exon , FTQUAL_EC_number },
  { FEATDEF_exon , FTQUAL_function },
  { FEATDEF_exon , FTQUAL_label },
  { FEATDEF_exon , FTQUAL_map },
  { FEATDEF_exon , FTQUAL_number },
  { FEATDEF_exon , FTQUAL_old_locus_tag },
  { FEATDEF_exon , FTQUAL_product },
  { FEATDEF_exon , FTQUAL_standard_name },
  { FEATDEF_exon , FTQUAL_usedin },

  { FEATDEF_GC_signal , FTQUAL_allele },
  { FEATDEF_GC_signal , FTQUAL_label },
  { FEATDEF_GC_signal , FTQUAL_map },
  { FEATDEF_GC_signal , FTQUAL_old_locus_tag },
  { FEATDEF_GC_signal , FTQUAL_usedin },

  { FEATDEF_iDNA , FTQUAL_allele },
  { FEATDEF_iDNA , FTQUAL_function },
  { FEATDEF_iDNA , FTQUAL_label },
  { FEATDEF_iDNA , FTQUAL_map },
  { FEATDEF_iDNA , FTQUAL_number },
  { FEATDEF_iDNA , FTQUAL_old_locus_tag },
  { FEATDEF_iDNA , FTQUAL_standard_name },
  { FEATDEF_iDNA , FTQUAL_usedin },

  { FEATDEF_intron , FTQUAL_allele },
  { FEATDEF_intron , FTQUAL_cons_splice },
  { FEATDEF_intron , FTQUAL_function },
  { FEATDEF_intron , FTQUAL_label },
  { FEATDEF_intron , FTQUAL_map },
  { FEATDEF_intron , FTQUAL_number },
  { FEATDEF_intron , FTQUAL_old_locus_tag },
  { FEATDEF_intron , FTQUAL_standard_name },
  { FEATDEF_intron , FTQUAL_usedin },

  { FEATDEF_J_segment , FTQUAL_allele },
  { FEATDEF_J_segment , FTQUAL_label },
  { FEATDEF_J_segment , FTQUAL_map },
  { FEATDEF_J_segment , FTQUAL_old_locus_tag },
  { FEATDEF_J_segment , FTQUAL_product },
  { FEATDEF_J_segment , FTQUAL_standard_name },
  { FEATDEF_J_segment , FTQUAL_usedin },

  { FEATDEF_LTR , FTQUAL_allele },
  { FEATDEF_LTR , FTQUAL_function },
  { FEATDEF_LTR , FTQUAL_label },
  { FEATDEF_LTR , FTQUAL_map },
  { FEATDEF_LTR , FTQUAL_old_locus_tag },
  { FEATDEF_LTR , FTQUAL_standard_name },
  { FEATDEF_LTR , FTQUAL_usedin },

  { FEATDEF_mat_peptide , FTQUAL_allele },
  { FEATDEF_mat_peptide , FTQUAL_EC_number },
  { FEATDEF_mat_peptide , FTQUAL_function },
  { FEATDEF_mat_peptide , FTQUAL_label },
  { FEATDEF_mat_peptide , FTQUAL_map },
  { FEATDEF_mat_peptide , FTQUAL_old_locus_tag },
  { FEATDEF_mat_peptide , FTQUAL_product },
  { FEATDEF_mat_peptide , FTQUAL_standard_name },
  { FEATDEF_mat_peptide , FTQUAL_usedin },

  { FEATDEF_misc_binding , FTQUAL_allele },
  { FEATDEF_misc_binding , FTQUAL_bound_moiety },
  { FEATDEF_misc_binding , FTQUAL_function },
  { FEATDEF_misc_binding , FTQUAL_label },
  { FEATDEF_misc_binding , FTQUAL_map },
  { FEATDEF_misc_binding , FTQUAL_old_locus_tag },
  { FEATDEF_misc_binding , FTQUAL_usedin },

  { FEATDEF_misc_difference , FTQUAL_allele },
  { FEATDEF_misc_difference , FTQUAL_clone },
  { FEATDEF_misc_difference , FTQUAL_compare },
  { FEATDEF_misc_difference , FTQUAL_label },
  { FEATDEF_misc_difference , FTQUAL_map },
  { FEATDEF_misc_difference , FTQUAL_old_locus_tag },
  { FEATDEF_misc_difference , FTQUAL_phenotype },
  { FEATDEF_misc_difference , FTQUAL_replace },
  { FEATDEF_misc_difference , FTQUAL_standard_name },
  { FEATDEF_misc_difference , FTQUAL_usedin },

  { FEATDEF_misc_feature , FTQUAL_allele },
  { FEATDEF_misc_feature , FTQUAL_function },
  { FEATDEF_misc_feature , FTQUAL_label },
  { FEATDEF_misc_feature , FTQUAL_map },
  { FEATDEF_misc_feature , FTQUAL_number },
  { FEATDEF_misc_feature , FTQUAL_old_locus_tag },
  { FEATDEF_misc_feature , FTQUAL_phenotype },
  { FEATDEF_misc_feature , FTQUAL_product },
  { FEATDEF_misc_feature , FTQUAL_standard_name },
  { FEATDEF_misc_feature , FTQUAL_usedin },

  { FEATDEF_misc_recomb , FTQUAL_allele },
  { FEATDEF_misc_recomb , FTQUAL_label },
  { FEATDEF_misc_recomb , FTQUAL_map },
  { FEATDEF_misc_recomb , FTQUAL_old_locus_tag },
  { FEATDEF_misc_recomb , FTQUAL_organism },
  { FEATDEF_misc_recomb , FTQUAL_standard_name },
  { FEATDEF_misc_recomb , FTQUAL_usedin },

  { FEATDEF_misc_signal , FTQUAL_allele },
  { FEATDEF_misc_signal , FTQUAL_function },
  { FEATDEF_misc_signal , FTQUAL_label },
  { FEATDEF_misc_signal , FTQUAL_map },
  { FEATDEF_misc_signal , FTQUAL_old_locus_tag },
  { FEATDEF_misc_signal , FTQUAL_operon },
  { FEATDEF_misc_signal , FTQUAL_phenotype },
  { FEATDEF_misc_signal , FTQUAL_standard_name },
  { FEATDEF_misc_signal , FTQUAL_usedin },

  { FEATDEF_misc_structure , FTQUAL_allele },
  { FEATDEF_misc_structure , FTQUAL_function },
  { FEATDEF_misc_structure , FTQUAL_label },
  { FEATDEF_misc_structure , FTQUAL_map },
  { FEATDEF_misc_structure , FTQUAL_old_locus_tag },
  { FEATDEF_misc_structure , FTQUAL_standard_name },
  { FEATDEF_misc_structure , FTQUAL_usedin },

  { FEATDEF_modified_base , FTQUAL_allele },
  { FEATDEF_modified_base , FTQUAL_frequency },
  { FEATDEF_modified_base , FTQUAL_label },
  { FEATDEF_modified_base , FTQUAL_map },
  { FEATDEF_modified_base , FTQUAL_mod_base },
  { FEATDEF_modified_base , FTQUAL_old_locus_tag },
  { FEATDEF_modified_base , FTQUAL_usedin },

  { FEATDEF_N_region , FTQUAL_allele },
  { FEATDEF_N_region , FTQUAL_label },
  { FEATDEF_N_region , FTQUAL_map },
  { FEATDEF_N_region , FTQUAL_old_locus_tag },
  { FEATDEF_N_region , FTQUAL_product },
  { FEATDEF_N_region , FTQUAL_standard_name },
  { FEATDEF_N_region , FTQUAL_usedin },

  { FEATDEF_old_sequence , FTQUAL_allele },
  { FEATDEF_old_sequence , FTQUAL_compare },
  { FEATDEF_old_sequence , FTQUAL_label },
  { FEATDEF_old_sequence , FTQUAL_map },
  { FEATDEF_old_sequence , FTQUAL_old_locus_tag },
  { FEATDEF_old_sequence , FTQUAL_replace },
  { FEATDEF_old_sequence , FTQUAL_usedin },

  { FEATDEF_polyA_signal , FTQUAL_allele },
  { FEATDEF_polyA_signal , FTQUAL_label },
  { FEATDEF_polyA_signal , FTQUAL_map },
  { FEATDEF_polyA_signal , FTQUAL_old_locus_tag },
  { FEATDEF_polyA_signal , FTQUAL_usedin },

  { FEATDEF_polyA_site , FTQUAL_allele },
  { FEATDEF_polyA_site , FTQUAL_label },
  { FEATDEF_polyA_site , FTQUAL_map },
  { FEATDEF_polyA_site , FTQUAL_old_locus_tag },
  { FEATDEF_polyA_site , FTQUAL_usedin },

  { FEATDEF_prim_transcript , FTQUAL_allele },
  { FEATDEF_prim_transcript , FTQUAL_function },
  { FEATDEF_prim_transcript , FTQUAL_label },
  { FEATDEF_prim_transcript , FTQUAL_map },
  { FEATDEF_prim_transcript , FTQUAL_old_locus_tag },
  { FEATDEF_prim_transcript , FTQUAL_operon },
  { FEATDEF_prim_transcript , FTQUAL_standard_name },
  { FEATDEF_prim_transcript , FTQUAL_usedin },

  { FEATDEF_primer_bind , FTQUAL_allele },
  { FEATDEF_primer_bind , FTQUAL_label },
  { FEATDEF_primer_bind , FTQUAL_map },
  { FEATDEF_primer_bind , FTQUAL_old_locus_tag },
  { FEATDEF_primer_bind , FTQUAL_PCR_conditions },
  { FEATDEF_primer_bind , FTQUAL_standard_name },
  { FEATDEF_primer_bind , FTQUAL_usedin },

  { FEATDEF_promoter , FTQUAL_allele },
  { FEATDEF_promoter , FTQUAL_function },
  { FEATDEF_promoter , FTQUAL_label },
  { FEATDEF_promoter , FTQUAL_map },
  { FEATDEF_promoter , FTQUAL_old_locus_tag },
  { FEATDEF_promoter , FTQUAL_operon },
  { FEATDEF_promoter , FTQUAL_phenotype },
  { FEATDEF_promoter , FTQUAL_standard_name },
  { FEATDEF_promoter , FTQUAL_usedin },

  { FEATDEF_protein_bind , FTQUAL_allele },
  { FEATDEF_protein_bind , FTQUAL_bound_moiety },
  { FEATDEF_protein_bind , FTQUAL_function },
  { FEATDEF_protein_bind , FTQUAL_label },
  { FEATDEF_protein_bind , FTQUAL_map },
  { FEATDEF_protein_bind , FTQUAL_old_locus_tag },
  { FEATDEF_protein_bind , FTQUAL_standard_name },
  { FEATDEF_protein_bind , FTQUAL_usedin },

  { FEATDEF_RBS , FTQUAL_allele },
  { FEATDEF_RBS , FTQUAL_label },
  { FEATDEF_RBS , FTQUAL_map },
  { FEATDEF_RBS , FTQUAL_old_locus_tag },
  { FEATDEF_RBS , FTQUAL_standard_name },
  { FEATDEF_RBS , FTQUAL_usedin },

  { FEATDEF_repeat_region , FTQUAL_allele },
  { FEATDEF_repeat_region , FTQUAL_function },
  { FEATDEF_repeat_region , FTQUAL_insertion_seq },
  { FEATDEF_repeat_region , FTQUAL_label },
  { FEATDEF_repeat_region , FTQUAL_map },
  { FEATDEF_repeat_region , FTQUAL_old_locus_tag },
  { FEATDEF_repeat_region , FTQUAL_rpt_family },
  { FEATDEF_repeat_region , FTQUAL_rpt_type },
  { FEATDEF_repeat_region , FTQUAL_rpt_unit },
  { FEATDEF_repeat_region , FTQUAL_standard_name },
  { FEATDEF_repeat_region , FTQUAL_transposon },
  { FEATDEF_repeat_region , FTQUAL_usedin },

  { FEATDEF_repeat_unit , FTQUAL_allele },
  { FEATDEF_repeat_unit , FTQUAL_function },
  { FEATDEF_repeat_unit , FTQUAL_label },
  { FEATDEF_repeat_unit , FTQUAL_map },
  { FEATDEF_repeat_unit , FTQUAL_old_locus_tag },
  { FEATDEF_repeat_unit , FTQUAL_rpt_family },
  { FEATDEF_repeat_unit , FTQUAL_rpt_type },
  { FEATDEF_repeat_unit , FTQUAL_usedin },

  { FEATDEF_rep_origin , FTQUAL_allele },
  { FEATDEF_rep_origin , FTQUAL_direction },
  { FEATDEF_rep_origin , FTQUAL_label },
  { FEATDEF_rep_origin , FTQUAL_map },
  { FEATDEF_rep_origin , FTQUAL_old_locus_tag },
  { FEATDEF_rep_origin , FTQUAL_standard_name },
  { FEATDEF_rep_origin , FTQUAL_usedin },

  { FEATDEF_S_region , FTQUAL_allele },
  { FEATDEF_S_region , FTQUAL_label },
  { FEATDEF_S_region , FTQUAL_map },
  { FEATDEF_S_region , FTQUAL_old_locus_tag },
  { FEATDEF_S_region , FTQUAL_product },
  { FEATDEF_S_region , FTQUAL_standard_name },
  { FEATDEF_S_region , FTQUAL_usedin },

  { FEATDEF_satellite , FTQUAL_allele },
  { FEATDEF_satellite , FTQUAL_label },
  { FEATDEF_satellite , FTQUAL_map },
  { FEATDEF_satellite , FTQUAL_old_locus_tag },
  { FEATDEF_satellite , FTQUAL_rpt_family },
  { FEATDEF_satellite , FTQUAL_rpt_type },
  { FEATDEF_satellite , FTQUAL_rpt_unit },
  { FEATDEF_satellite , FTQUAL_standard_name },
  { FEATDEF_satellite , FTQUAL_usedin },

  { FEATDEF_sig_peptide , FTQUAL_allele },
  { FEATDEF_sig_peptide , FTQUAL_function },
  { FEATDEF_sig_peptide , FTQUAL_label },
  { FEATDEF_sig_peptide , FTQUAL_map },
  { FEATDEF_sig_peptide , FTQUAL_old_locus_tag },
  { FEATDEF_sig_peptide , FTQUAL_product },
  { FEATDEF_sig_peptide , FTQUAL_standard_name },
  { FEATDEF_sig_peptide , FTQUAL_usedin },

  { FEATDEF_stem_loop , FTQUAL_allele },
  { FEATDEF_stem_loop , FTQUAL_function },
  { FEATDEF_stem_loop , FTQUAL_label },
  { FEATDEF_stem_loop , FTQUAL_map },
  { FEATDEF_stem_loop , FTQUAL_old_locus_tag },
  { FEATDEF_stem_loop , FTQUAL_operon },
  { FEATDEF_stem_loop , FTQUAL_standard_name },
  { FEATDEF_stem_loop , FTQUAL_usedin },

  { FEATDEF_STS , FTQUAL_allele },
  { FEATDEF_STS , FTQUAL_label },
  { FEATDEF_STS , FTQUAL_map },
  { FEATDEF_STS , FTQUAL_old_locus_tag },
  { FEATDEF_STS , FTQUAL_standard_name },
  { FEATDEF_STS , FTQUAL_usedin },

  { FEATDEF_TATA_signal , FTQUAL_allele },
  { FEATDEF_TATA_signal , FTQUAL_label },
  { FEATDEF_TATA_signal , FTQUAL_map },
  { FEATDEF_TATA_signal , FTQUAL_old_locus_tag },
  { FEATDEF_TATA_signal , FTQUAL_usedin },

  { FEATDEF_terminator , FTQUAL_allele },
  { FEATDEF_terminator , FTQUAL_label },
  { FEATDEF_terminator , FTQUAL_map },
  { FEATDEF_terminator , FTQUAL_old_locus_tag },
  { FEATDEF_terminator , FTQUAL_operon },
  { FEATDEF_terminator , FTQUAL_standard_name },
  { FEATDEF_terminator , FTQUAL_usedin },

  { FEATDEF_transit_peptide , FTQUAL_allele },
  { FEATDEF_transit_peptide , FTQUAL_function },
  { FEATDEF_transit_peptide , FTQUAL_label },
  { FEATDEF_transit_peptide , FTQUAL_map },
  { FEATDEF_transit_peptide , FTQUAL_old_locus_tag },
  { FEATDEF_transit_peptide , FTQUAL_product },
  { FEATDEF_transit_peptide , FTQUAL_standard_name },
  { FEATDEF_transit_peptide , FTQUAL_usedin },

  { FEATDEF_unsure , FTQUAL_allele },
  { FEATDEF_unsure , FTQUAL_compare },
  { FEATDEF_unsure , FTQUAL_label },
  { FEATDEF_unsure , FTQUAL_map },
  { FEATDEF_unsure , FTQUAL_old_locus_tag },
  { FEATDEF_unsure , FTQUAL_replace },
  { FEATDEF_unsure , FTQUAL_usedin },

  { FEATDEF_V_region , FTQUAL_allele },
  { FEATDEF_V_region , FTQUAL_label },
  { FEATDEF_V_region , FTQUAL_map },
  { FEATDEF_V_region , FTQUAL_old_locus_tag },
  { FEATDEF_V_region , FTQUAL_product },
  { FEATDEF_V_region , FTQUAL_standard_name },
  { FEATDEF_V_region , FTQUAL_usedin },

  { FEATDEF_V_segment , FTQUAL_allele },
  { FEATDEF_V_segment , FTQUAL_label },
  { FEATDEF_V_segment , FTQUAL_map },
  { FEATDEF_V_segment , FTQUAL_old_locus_tag },
  { FEATDEF_V_segment , FTQUAL_product },
  { FEATDEF_V_segment , FTQUAL_standard_name },
  { FEATDEF_V_segment , FTQUAL_usedin },

  { FEATDEF_variation , FTQUAL_allele },
  { FEATDEF_variation , FTQUAL_compare },
  { FEATDEF_variation , FTQUAL_frequency },
  { FEATDEF_variation , FTQUAL_label },
  { FEATDEF_variation , FTQUAL_map },
  { FEATDEF_variation , FTQUAL_old_locus_tag },
  { FEATDEF_variation , FTQUAL_phenotype },
  { FEATDEF_variation , FTQUAL_product },
  { FEATDEF_variation , FTQUAL_replace },
  { FEATDEF_variation , FTQUAL_standard_name },
  { FEATDEF_variation , FTQUAL_usedin },

  { FEATDEF_3clip , FTQUAL_allele },
  { FEATDEF_3clip , FTQUAL_function },
  { FEATDEF_3clip , FTQUAL_label },
  { FEATDEF_3clip , FTQUAL_map },
  { FEATDEF_3clip , FTQUAL_old_locus_tag },
  { FEATDEF_3clip , FTQUAL_standard_name },
  { FEATDEF_3clip , FTQUAL_usedin },

  { FEATDEF_3UTR , FTQUAL_allele },
  { FEATDEF_3UTR , FTQUAL_function },
  { FEATDEF_3UTR , FTQUAL_label },
  { FEATDEF_3UTR , FTQUAL_map },
  { FEATDEF_3UTR , FTQUAL_old_locus_tag },
  { FEATDEF_3UTR , FTQUAL_standard_name },
  { FEATDEF_3UTR , FTQUAL_usedin },

  { FEATDEF_5clip , FTQUAL_allele },
  { FEATDEF_5clip , FTQUAL_function },
  { FEATDEF_5clip , FTQUAL_label },
  { FEATDEF_5clip , FTQUAL_map },
  { FEATDEF_5clip , FTQUAL_old_locus_tag },
  { FEATDEF_5clip , FTQUAL_standard_name },
  { FEATDEF_5clip , FTQUAL_usedin },

  { FEATDEF_5UTR , FTQUAL_allele },
  { FEATDEF_5UTR , FTQUAL_function },
  { FEATDEF_5UTR , FTQUAL_label },
  { FEATDEF_5UTR , FTQUAL_map },
  { FEATDEF_5UTR , FTQUAL_old_locus_tag },
  { FEATDEF_5UTR , FTQUAL_standard_name },
  { FEATDEF_5UTR , FTQUAL_usedin },

  { FEATDEF_10_signal , FTQUAL_allele },
  { FEATDEF_10_signal , FTQUAL_label },
  { FEATDEF_10_signal , FTQUAL_map },
  { FEATDEF_10_signal , FTQUAL_old_locus_tag },
  { FEATDEF_10_signal , FTQUAL_operon },
  { FEATDEF_10_signal , FTQUAL_standard_name },
  { FEATDEF_10_signal , FTQUAL_usedin },

  { FEATDEF_35_signal , FTQUAL_allele },
  { FEATDEF_35_signal , FTQUAL_label },
  { FEATDEF_35_signal , FTQUAL_map },
  { FEATDEF_35_signal , FTQUAL_old_locus_tag },
  { FEATDEF_35_signal , FTQUAL_operon },
  { FEATDEF_35_signal , FTQUAL_standard_name },
  { FEATDEF_35_signal , FTQUAL_usedin },

  { FEATDEF_REGION , FTQUAL_function },
  { FEATDEF_REGION , FTQUAL_label },
  { FEATDEF_REGION , FTQUAL_map },
  { FEATDEF_REGION , FTQUAL_number },
  { FEATDEF_REGION , FTQUAL_old_locus_tag },
  { FEATDEF_REGION , FTQUAL_phenotype },
  { FEATDEF_REGION , FTQUAL_product },
  { FEATDEF_REGION , FTQUAL_standard_name },
  { FEATDEF_REGION , FTQUAL_usedin },

  { FEATDEF_preprotein , FTQUAL_allele },
  { FEATDEF_preprotein , FTQUAL_label },
  { FEATDEF_preprotein , FTQUAL_map },
  { FEATDEF_preprotein , FTQUAL_old_locus_tag },
  { FEATDEF_preprotein , FTQUAL_product },
  { FEATDEF_preprotein , FTQUAL_standard_name },
  { FEATDEF_preprotein , FTQUAL_usedin },

  { FEATDEF_mat_peptide_aa , FTQUAL_allele },
  { FEATDEF_mat_peptide_aa , FTQUAL_label },
  { FEATDEF_mat_peptide_aa , FTQUAL_map },
  { FEATDEF_mat_peptide_aa , FTQUAL_old_locus_tag },
  { FEATDEF_mat_peptide_aa , FTQUAL_product },
  { FEATDEF_mat_peptide_aa , FTQUAL_standard_name },
  { FEATDEF_mat_peptide_aa , FTQUAL_usedin },

  { FEATDEF_sig_peptide_aa , FTQUAL_allele },
  { FEATDEF_sig_peptide_aa , FTQUAL_label },
  { FEATDEF_sig_peptide_aa , FTQUAL_map },
  { FEATDEF_sig_peptide_aa , FTQUAL_old_locus_tag },
  { FEATDEF_sig_peptide_aa , FTQUAL_product },
  { FEATDEF_sig_peptide_aa , FTQUAL_standard_name },
  { FEATDEF_sig_peptide_aa , FTQUAL_usedin },

  { FEATDEF_transit_peptide_aa , FTQUAL_allele },
  { FEATDEF_transit_peptide_aa , FTQUAL_label },
  { FEATDEF_transit_peptide_aa , FTQUAL_map },
  { FEATDEF_transit_peptide_aa , FTQUAL_old_locus_tag },
  { FEATDEF_transit_peptide_aa , FTQUAL_product },
  { FEATDEF_transit_peptide_aa , FTQUAL_standard_name },
  { FEATDEF_transit_peptide_aa , FTQUAL_usedin },

  { FEATDEF_snoRNA , FTQUAL_allele },
  { FEATDEF_snoRNA , FTQUAL_function },
  { FEATDEF_snoRNA , FTQUAL_label },
  { FEATDEF_snoRNA , FTQUAL_map },
  { FEATDEF_snoRNA , FTQUAL_old_locus_tag },
  { FEATDEF_snoRNA , FTQUAL_product },
  { FEATDEF_snoRNA , FTQUAL_standard_name },
  { FEATDEF_snoRNA , FTQUAL_usedin },

  { FEATDEF_gap , FTQUAL_estimated_length },
  { FEATDEF_gap , FTQUAL_map },

  { FEATDEF_operon , FTQUAL_allele },
  { FEATDEF_operon , FTQUAL_function },
  { FEATDEF_operon , FTQUAL_label },
  { FEATDEF_operon , FTQUAL_map },
  { FEATDEF_operon , FTQUAL_operon },
  { FEATDEF_operon , FTQUAL_phenotype },
  { FEATDEF_operon , FTQUAL_standard_name },
  { FEATDEF_operon , FTQUAL_usedin },

  { FEATDEF_oriT , FTQUAL_allele },
  { FEATDEF_oriT , FTQUAL_direction },
  { FEATDEF_oriT , FTQUAL_label },
  { FEATDEF_oriT , FTQUAL_map },
  { FEATDEF_oriT , FTQUAL_old_locus_tag },
  { FEATDEF_oriT , FTQUAL_rpt_type },
  { FEATDEF_oriT , FTQUAL_rpt_type },
  { FEATDEF_oriT , FTQUAL_rpt_unit },
  { FEATDEF_oriT , FTQUAL_standard_name },
  { FEATDEF_oriT , FTQUAL_usedin }
};

/* comparison of ValQual's -- first compare featdef then ftqual */

/* macro did not work properly on linux machine, so using function instead */
/* #define COMPARE_VALQUAL(av,aq,bv,bq) ( ((av)-(bv)) ? ((av)-(bv)) : ((aq)-(bq)) ) */

static Int2 CompareValQual (Uint2 av, FtQualType aq, Uint2 bv, FtQualType bq)

{
  if (av == bv) return (aq - bq);
  return (av - bv);
}

/* Returns TRUE if {featureKey, qualKey} exists in legalGbqualList */

static Boolean AllowedValQual (Uint2 featureKey, FtQualType qualKey)

{
  Int2 L, R, mid;

  L = 0;
  R = sizeof (legalGbqualList) / sizeof (ValQual) - 1;
  while (L < R) {
    mid = (L + R) / 2;
    if (CompareValQual (legalGbqualList [mid].featdef,
       legalGbqualList [mid].ftqual,
       featureKey, qualKey) < 0)
      L = mid + 1;
    else
      R = mid;
  }
  if (CompareValQual (legalGbqualList [R].featdef,
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
  "RNA editing",
  "reasons given in citation",
  NULL
};

static CharPtr validRefSeqExceptionString [] = {
  "RNA editing",
  "reasons given in citation",
  "ribosomal slippage",
  "trans-splicing",
  "alternative processing",
  "artificial frameshift",
  "nonconsensus splice site",
  "rearrangement required for product",
  "modified codon recognition",
  "alternative start codon",
  "unclassified transcription discrepancy",
  "unclassified translation discrepancy",
  "mismatches in transcription",
  "mismatches in translation",
  NULL
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
Functions now public and prototyped in sequtil.h
Return values are:
 0: no problem - Accession is in proper format
-1: Accession did not start with a letter (or two letters)
-2: Accession did not contain five numbers (or six numbers after 2 letters)
-3: the original Accession number to be validated was NULL
-4: the original Accession number is too long (>16)
-5: missing or bad version number (required by ValidateAccnDotVer)
*/

static Int2 ValidateAccnInternal (
  CharPtr accession,
  CharPtr PNTR strptr
)

{
  Char     ch;
  Int2     numAlpha = 0;
  Int2     numDigits = 0;
  Int2     numUndersc = 0;
  CharPtr  str;

  if (accession == NULL || accession [0] == '\0') return -3;

  if (StringLen (accession) >= 16) return -4;

  if (accession [0] < 'A' || accession [0] > 'Z') return -1;

  str = accession;
  if (StringNCmp (str, "NZ_", 3) == 0) {
    str += 3;
  }
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
  if (ch != '\0' && ch != ' ' && ch != '.') return -2;

  if (numUndersc > 1) return -2;

  if (strptr != NULL) {
    /* pass back current position for version check */
    *strptr = str;
  }

  if (numUndersc == 0) {
    if (numAlpha == 1 && numDigits == 5) return 0;
    if (numAlpha == 2 && numDigits == 6) return 0;
    if (numAlpha == 3 && numDigits == 5) return 0;
    if (numAlpha == 4 && numDigits == 8) return 0;
  } else if (numUndersc == 1) {
    if (numAlpha != 2 || (numDigits != 6 && numDigits != 8 && numDigits != 9)) return -2;
    if (accession [0] == 'N' || accession [0] == 'X' || accession [0] == 'Z') {
      if (accession [1] == 'M' ||
          accession [1] == 'C' ||
          accession [1] == 'T' ||
          accession [1] == 'P' ||
          accession [1] == 'G' ||
          accession [1] == 'R' ||
          accession [1] == 'S' ||
          accession [1] == 'W' ||
          accession [1] == 'Z') {
        return 0;
      }
    }
    if (accession [0] == 'A' || accession [0] == 'Y') {
      if (accession [1] == 'P') return 0;
    }
  }

  return -2;
}

NLM_EXTERN Int2 ValidateAccn (
  CharPtr accession
)

{
  return ValidateAccnInternal (accession, NULL);
}

NLM_EXTERN Int2 ValidateAccnDotVer (
  CharPtr accession
)

{
  Char     ch;
  Int2     numVersion = 0;
  Int2     rsult;
  CharPtr  str = NULL;

  rsult = ValidateAccnInternal (accession, &str);
  if (rsult != 0) return rsult;

  if (str == NULL) return -5;
  ch = *str;
  if (ch != '.') return -5;
  str++;
  ch = *str;
  while (IS_DIGIT (ch)) {
    numVersion++;
    str++;
    ch = *str;
  }
  if (numVersion < 1) return -5;
  if (ch != '\0' && ch != ' ') return -5;

  return 0;
}

NLM_EXTERN Int2 ValidateSeqID (
  SeqIdPtr sip
)

{
  Char  buf [41];

  if (sip == NULL) return -3;
  SeqIdWrite (sip, buf, PRINTID_TEXTID_ACC_VER, sizeof (buf) - 1);
  return ValidateAccn (buf);
}

static Boolean ValidateCompareQual (CharPtr accession, Boolean is_ged)

{
  if (ValidateAccnDotVer (accession) != 0) return FALSE;
  if (StringChr (accession, '_') == NULL) return TRUE;
  if (is_ged) return FALSE;
  return TRUE;
}

static CharPtr mrnaevtext1 = "Derived by automated computational analysis";
static CharPtr mrnaevtext2 = "using gene prediction method:";
static CharPtr mrnaevtext3 = "Supporting evidence includes similarity to:";

static void GetStrFormRNAEvidence (
  UserObjectPtr uop,
  Pointer userdata
)

{
  size_t        len;
  CharPtr       method = NULL;
  Int2          nm = 0;
  ObjectIdPtr   oip;
  CharPtr       str = NULL;
  CharPtr PNTR  strp;
  Char          tmp [20];
  UserFieldPtr  u, ufp, uu;

  if (uop == NULL) return;
  oip = uop->type;
  if (oip == NULL) return;
  if (StringCmp (oip->str, "ModelEvidence") != 0) return;
  strp = (CharPtr PNTR) userdata;

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
  if (str == NULL) return;

  if (method != NULL) {
    sprintf (str, "%s %s %s.", mrnaevtext1, mrnaevtext2, method);
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

  *strp = str;
}

static Boolean ValidateRptUnit (
  CharPtr buf
)

{
#if 0
  CharPtr  str;
  Char     tmp [255];

  StringNCpy_0 (tmp, buf, sizeof (tmp));
  TrimSpacesAroundString (tmp);

  str = tmp;
  /* first check for sequence letters with optional semicolons */
  while (IS_ALPHA (*str) || *str == ';') str++;
  if (*str == '\0') return TRUE;
  /* next check for letters, digits, commas, parentheses, dashes, and underscores */
  str = tmp;
  while (IS_ALPHANUM (*str) || *str == '(' || *str == ')' || *str == ',' || *str == ';' || *str == '-' || *str == '_') str++;
  if (*str == '\0') return TRUE;
  /* now check for officially legal styles */
  str = tmp;
  while (IS_ALPHANUM (*str)) str++;
  if (*str != '\0') { /* wasn't pure alphanumeric; now check for xxx..yyy */
    str = buf;
    while (IS_DIGIT (*str)) str++; /* xxx */
    if (*str == '\0' /* must be something after the xxx */
      || StringLen (str) < 3  /* need at least 2 '.'s and a digit*/
      || str[0] != '.' || str[1] != '.') return FALSE;
    str+=2;
    while (IS_DIGIT (*str)) str++;
    if (*str != '\0') return FALSE;  /* mustn't be anything after the yyy */
  }
#endif
  return TRUE;
}


NLM_EXTERN CharPtr goFieldType [] = {
  "", "text string", "go id", "pubmed id", "evidence", NULL
};

static CharPtr GetGOtext (
  UserFieldPtr topufp,
  IntAsn2gbJobPtr ajp
)

{
  CharPtr        evidence = NULL;
  StringItemPtr  ffstring;
  Char           gid [32];
  CharPtr        goid = NULL;
  Boolean        is_www;
  Int2           j;
  ObjectIdPtr    oip;
  Int4           pmid = 0;
  CharPtr        ptr;
  CharPtr        str;
  CharPtr        textstr = NULL;
  Char           tmp [32];
  UserFieldPtr   ufp;

  if (topufp == NULL || ajp == NULL) return NULL;
  is_www = GetWWW (ajp);

  for (ufp = topufp; ufp != NULL; ufp = ufp->next) {
    oip = ufp->label;
    if (oip == NULL) continue;
    for (j = 0; goFieldType [j] != NULL; j++) {
      if (StringICmp (oip->str, goFieldType [j]) == 0) break;
    }
    if (goFieldType [j] == NULL) continue;
    switch (j) {
      case 1 :
        if (ufp->choice == 1) {
          textstr = (CharPtr) ufp->data.ptrvalue;
        }
        break;
      case 2 :
        if (ufp->choice == 1) {
          goid = (CharPtr) ufp->data.ptrvalue;
        } else if (ufp->choice == 2) {
          sprintf (gid, "%ld", (long) (Int4) ufp->data.intvalue);
          goid = (CharPtr) gid;
        }
        break;
      case 3 :
        if (ufp->choice == 2) {
          pmid = (Int4) ufp->data.intvalue;
        }
        break;
      case 4 :
        if (ufp->choice == 1) {
          evidence = (CharPtr) ufp->data.ptrvalue;
        }
        break;
      default :
        break;
    }
  }
  /* if (StringHasNoText (textstr)) return NULL; */

  str = (CharPtr) MemNew (StringLen (textstr) + StringLen (goid) + StringLen (evidence) + StringLen (link_go) + 80);
  if (str == NULL) return NULL;

  StringCpy (str, textstr);
  if (! StringHasNoText (goid)) {
    StringCat (str, " [goid ");
    if (is_www) {
      ffstring = FFGetString (ajp);
      if (ffstring != NULL) {
        FFAddOneString(ffstring, "<a href=", FALSE, FALSE, TILDE_IGNORE);
        FFAddOneString(ffstring, link_go, FALSE, FALSE, TILDE_IGNORE);
        FFAddOneString(ffstring, goid, FALSE, FALSE, TILDE_IGNORE);
        FFAddOneChar(ffstring, '>', FALSE); 
        FFAddOneString(ffstring, goid, FALSE, FALSE, TILDE_IGNORE);
        FFAddOneString(ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
        ptr = FFToCharPtr (ffstring);
        FFRecycleString (ajp, ffstring);
        StringCat (str, ptr);
        MemFree (ptr);
      } else {
        StringCat (str, goid);
      }
    } else {
      StringCat (str, goid);
    }
    StringCat (str, "]");
  }
  if (! StringHasNoText (evidence)) {
    StringCat (str, " [evidence ");
    StringCat (str, evidence);
    StringCat (str, "]");
  }
  if (pmid != 0) {
    sprintf (tmp, " [pmid %ld", (long) pmid);
    StringCat (str, tmp);
    StringCat (str, "]");
  }
  TrimSpacesAroundString (str);

  return str;
}

static Boolean DbxrefAlreadyInGeneXref (
  DbtagPtr dbt,
  ValNodePtr dbxref
)

{
  DbtagPtr    gdbt;
  ValNodePtr  vnp;

  if (dbt == NULL) return FALSE;

  for (vnp = dbxref; vnp != NULL; vnp = vnp->next) {
    gdbt = (DbtagPtr) vnp->data.ptrvalue;
    if (gdbt == NULL) continue;
    if (DbtagMatch (dbt, gdbt)) return TRUE;
  }

  return FALSE;
}

static void FF_www_protein_id(
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  CharPtr seqid
)
{

  if ( GetWWW(ajp) ) {
    FFAddOneString(ffstring, "<a href=", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, link_seq, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, "val=", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, seqid, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneChar(ffstring, '>', FALSE);  
    FFAddOneString(ffstring, seqid, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
  } else {
    FFAddOneString(ffstring, seqid, FALSE, FALSE, TILDE_IGNORE);
  }
}

static void FF_www_gcode (
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  CharPtr gcode
)
{

  if ( GetWWW(ajp) ) {
    FFAddOneString(ffstring, "<a href=", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, link_code, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, "mode=c#SG", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, gcode, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneChar(ffstring, '>', FALSE);  
    FFAddOneString(ffstring, gcode, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
  } else {
    FFAddOneString(ffstring, gcode, FALSE, FALSE, TILDE_IGNORE);
  }
}

static void FF_AddECnumber (
  IntAsn2gbJobPtr ajp,
  StringItemPtr ffstring,
  CharPtr str
)
{
  if ( GetWWW(ajp) ) {
    FFAddOneString(ffstring, "<a href=", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, ec_link, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, str, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneChar(ffstring, '>', FALSE);  
    FFAddOneString(ffstring, str, FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
  } else {
    FFAddOneString(ffstring, str, FALSE, FALSE, TILDE_IGNORE);
  }
}


/* FormatFeatureblockQuals should not be called directly,
   except from FormatFeatureBlock.  It performs no input
   validation.  (perhaps it should?) */

static void LIBCALLBACK SaveGBSeqSequence (
  CharPtr sequence,
  Pointer userdata
)

{
  CharPtr       tmp;
  CharPtr PNTR  tmpp;

  tmpp = (CharPtr PNTR) userdata;
  tmp = *tmpp;

  tmp = StringMove (tmp, sequence);

  *tmpp = tmp;
}

static int LIBCALLBACK SortVnpByInt (VoidPtr ptr1, VoidPtr ptr2)

{
  ValNodePtr  vnp1;
  ValNodePtr  vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;

  if (vnp1->data.intvalue > vnp2->data.intvalue) {
    return 1;
  } else if (vnp1->data.intvalue < vnp2->data.intvalue) {
    return -1;
  }

  return 0;
}

static void FormatFeatureBlockQuals (
  StringItemPtr    ffstring,
  IntAsn2gbJobPtr  ajp,
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
  Boolean          is_journalscan,
  Boolean          is_gps,
  Boolean          is_ged
)

{
  Boolean            add_period;
  CharPtr            ascii;
  Int2               ascii_len;
  Boolean            at_end = FALSE;
  ByteStorePtr       bs;
  Char               buf[80];
  Choice             cbaa;
  CodeBreakPtr       cbp;
  Char               ch;
  Uint1              choice;
  ValNodePtr         citlist;
  Int4               gi;
  Boolean            hadProtDesc = FALSE;
  DbtagPtr           dbt;
  UserFieldPtr       entry;
  Int4               exp_ev;
  GBQualPtr          gbq;
  Int2               i;
  FtQualType         idx;
  Boolean            isTRNA;
  Int2               j;
  FtQualType         jdx;
  Int4               len;
  SeqLocPtr          newloc;
  CharPtr            notestr;
  Char               numbuf [32];
  Int2               numcodons;
  Int2               numsyns;
  ObjectIdPtr        oip;
  Boolean            okay;
  Boolean            only_digits;
  BioseqPtr          pbsp;
  Int4               pmid;
  ValNodePtr         pmidlist;
  ValNodePtr         ppr;
  CharPtr            prefix;
  CharPtr            protein_seq = NULL;
  size_t             prtlen;
  CharPtr            ptr;
  CharPtr            region;
  Uint1              residue;
  SeqCodeTablePtr    sctp;
  Int4               sec_str;
  Uint1              seqcode;
  Char               seqid [50];
  SeqIntPtr          sintp;
  SeqIdPtr           sip;
  SeqLocPtr          slp;
  Boolean            split;
  CharPtr            start;
  CharPtr            str;
  Boolean            suppress_period;
  CharPtr            tmp;
  tRNAPtr            trna;
  UserFieldPtr       ufp;
  UserObjectPtr      uop;
  ValNodePtr         vnp;
  StringItemPtr      unique;

  unique = FFGetString(ajp);
  if ( unique == NULL ) return;

  for (i = 0, idx = feat_qual_order [i]; idx != (FtQualType) 0; i++, idx = feat_qual_order [i]) {

    lasttype = NULL;
    switch (asn2gnbk_featur_quals [idx].qualclass) {

      case Qual_class_ignore :
        break;

      case Qual_class_string :
        if (! StringHasNoText (qvp [idx].str)) {
          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
            FALSE, TRUE, TILDE_TO_SPACES);
          FFAddTextToString(ffstring, "\"", qvp [idx].str, "\"",
            FALSE, TRUE, TILDE_TO_SPACES);
          FFAddOneChar(ffstring, '\n', FALSE);
        }
        break;

      case Qual_class_locus_tag :
        if (! StringHasNoText (qvp [idx].str)) {
          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
            FALSE, TRUE, TILDE_TO_SPACES);
          FFAddTextToString(ffstring, "\"", qvp [idx].str, "\"",
            FALSE, TRUE, TILDE_TO_SPACES);
          FFAddOneChar(ffstring, '\n', FALSE);
        }
        break;

      case Qual_class_tilde :
        if (! StringHasNoText (qvp [idx].str)) {
          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
            FALSE, TRUE, TILDE_EXPAND);
          FFAddTextToString(ffstring, "\"", qvp [idx].str, "\"",
            FALSE, TRUE, TILDE_EXPAND);
          FFAddOneChar(ffstring, '\n', FALSE);
        }
        break;

      case Qual_class_product :
        if (StringHasNoText (qvp [idx].str) ||
            (ajp->flags.dropIllegalQuals &&
             (! AllowedValQual (featdeftype, FTQUAL_product)))) break;
          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
            FALSE, TRUE, TILDE_TO_SPACES);
          FFAddTextToString(ffstring, "\"", qvp [idx].str, "\"",
            FALSE, TRUE, TILDE_TO_SPACES);
          FFAddOneChar(ffstring, '\n', FALSE);
        break;

      case Qual_class_sgml :
        if (! StringHasNoText (qvp [idx].str)) {
          if (is_journalscan) {
            ascii_len = Sgml2AsciiLen (qvp [idx].str);
            start = ascii = MemNew ((size_t) (10 + ascii_len));
            if (start != NULL) {
              ascii = Sgml2Ascii (qvp [idx].str, ascii, ascii_len + 1);

              FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
                FALSE, TRUE, TILDE_TO_SPACES);
              FFAddTextToString(ffstring, "\"", start, "\"",
                FALSE, TRUE, TILDE_TO_SPACES);
              FFAddOneChar(ffstring, '\n', FALSE);

              MemFree (start);
            }
          } else {
              FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
                FALSE, TRUE, TILDE_TO_SPACES);
              FFAddTextToString(ffstring, "\"", qvp[idx].str, "\"",
                FALSE, TRUE, TILDE_TO_SPACES);
              FFAddOneChar(ffstring, '\n', FALSE);
          }
        }
        break;

      case Qual_class_boolean :
        if (qvp [idx].ble) {
          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "\n",
                            FALSE, TRUE, TILDE_IGNORE);
        }
        break;

      case Qual_class_int :
        if (qvp [idx].num > 0) {
          if (idx == FTQUAL_transl_table) {
            sprintf (numbuf, "%ld", (long) qvp [idx].num);
            FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
              FALSE, TRUE, TILDE_IGNORE);
            FF_www_gcode (ajp, ffstring, numbuf);
          } else {
            sprintf (numbuf, "%ld", (long) qvp [idx].num);
            FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
              FALSE, TRUE, TILDE_IGNORE);
            FFAddTextToString(ffstring, NULL, numbuf, NULL,
              FALSE, TRUE, TILDE_IGNORE);
          }
          FFAddOneChar(ffstring, '\n', FALSE);
        }
        break;

      case Qual_class_evidence :
        exp_ev = qvp [idx].num;
        if (exp_ev > 0 && exp_ev <= 2) {
          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
              FALSE, TRUE, TILDE_IGNORE);
          FFAddOneString(ffstring, evidenceText [exp_ev], FALSE, TRUE, TILDE_IGNORE);
          FFAddOneChar(ffstring, '\n', FALSE);
        }
        break;

      case Qual_class_valnode :
        for (vnp = qvp[idx].vnp; vnp != NULL; vnp = vnp->next) {
          str = (CharPtr) vnp->data.ptrvalue;
          if (str != NULL) {
              FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
                  FALSE, TRUE, TILDE_TO_SPACES);
              FFAddTextToString(ffstring, "\"", str, "\"",
                  FALSE, TRUE, TILDE_TO_SPACES);
              FFAddOneChar(ffstring, '\n', FALSE);
          }
        }
        break;

      case Qual_class_gene_syn :
        /*
        for (vnp = qvp[idx].vnp; vnp != NULL; vnp = vnp->next) {
          str = (CharPtr) vnp->data.ptrvalue;
          if (str != NULL) {
              FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
                  FALSE, TRUE, TILDE_TO_SPACES);
              FFAddTextToString(ffstring, "\"", str, "\"",
                  FALSE, TRUE, TILDE_TO_SPACES);
              FFAddOneChar(ffstring, '\n', FALSE);
          }
        }
        */
        numsyns = 0;
        for (vnp = qvp [idx].vnp; vnp != NULL; vnp = vnp->next) {
          str = (CharPtr) vnp->data.ptrvalue;
          if (! StringHasNoText (str)) {
            numsyns++;
          }
        }
        if (numsyns > 0) {
          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=\"",
                FALSE, TRUE, TILDE_TO_SPACES);
          prefix = NULL;
          for (vnp = qvp [idx].vnp; vnp != NULL; vnp = vnp->next) {
            str = (CharPtr) vnp->data.ptrvalue;
            if (! StringHasNoText (str)) {
              FFAddTextToString (ffstring, prefix, str, NULL, FALSE, FALSE, TILDE_IGNORE);
              prefix = ", ";
            }
          }
          FFAddOneChar(ffstring, '\"', FALSE);
          FFAddOneChar(ffstring, '\n', FALSE);
        }
        break;

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

          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
                FALSE, TRUE, TILDE_TO_SPACES);
          FFAddOneChar(ffstring, '\"', FALSE);
          FF_AddECnumber(ajp, ffstring, str);
          FFAddOneChar(ffstring, '\"', FALSE);
          FFAddOneChar(ffstring, '\n', FALSE);
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
            FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
                  FALSE, TRUE, TILDE_TO_SPACES);
            FFAddOneChar(ffstring, '\"', FALSE);
            if (!StringIsJustQuotes (gbq->val)) {
              FF_AddECnumber (ajp, ffstring, gbq->val);
            }
            FFAddOneChar(ffstring, '\"', FALSE);
            FFAddOneChar(ffstring, '\n', FALSE);
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
            FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=\"",
                  FALSE, TRUE, TILDE_IGNORE);
            if (!StringIsJustQuotes (gbq->val)) {
              FFAddOneString(ffstring, gbq->val, FALSE, TRUE, TILDE_IGNORE);
            }
            FFAddOneChar(ffstring, '\"', FALSE);
            FFAddOneChar(ffstring, '\n', FALSE);
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
            FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=",
                  FALSE, TRUE, TILDE_IGNORE);
            FFAddOneString(ffstring, gbq->val, FALSE, TRUE, TILDE_TO_SPACES);
            FFAddOneChar(ffstring, '\n', FALSE);
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
            FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                  FALSE, TRUE, TILDE_IGNORE);
            FFAddOneString(ffstring, gbq->val, FALSE, TRUE, TILDE_TO_SPACES);
            FFAddOneChar(ffstring, '\n', FALSE);
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
            FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                  FALSE, TRUE, TILDE_IGNORE);
            FFAddOneString(ffstring, gbq->val, FALSE, TRUE, TILDE_TO_SPACES);
            FFAddOneChar(ffstring, '\n', FALSE);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_usedin :
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
                FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                  FALSE, TRUE, TILDE_IGNORE);
                FFAddOneString(ffstring, str, FALSE, TRUE, TILDE_TO_SPACES);
                FFAddOneChar(ffstring, '\n', FALSE);
                str = ptr;
              }
            } else {
              FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                  FALSE, TRUE, TILDE_IGNORE);
              FFAddOneString(ffstring, str, FALSE, TRUE, TILDE_TO_SPACES);
              FFAddOneChar(ffstring, '\n', FALSE);
            }
            MemFree (tmp);
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
                FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=\"",
                  FALSE, TRUE, TILDE_IGNORE);
                FFAddOneString(ffstring, str, FALSE, TRUE, TILDE_TO_SPACES);
                FFAddOneChar(ffstring, '\"', FALSE);
                FFAddOneChar(ffstring, '\n', FALSE);
                str = ptr;
              }
            } else {
              FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=\"",
                  FALSE, TRUE, TILDE_IGNORE);
              FFAddOneString(ffstring, str, FALSE, TRUE, TILDE_TO_SPACES);
              FFAddOneChar(ffstring, '\"', FALSE);
              FFAddOneChar(ffstring, '\n', FALSE);
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
                  FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                                    FALSE, TRUE, TILDE_IGNORE);
                  FFAddOneString(ffstring, str, FALSE, TRUE, TILDE_TO_SPACES);
                  FFAddOneChar(ffstring, '\n', FALSE);
                }
                str = ptr;
              }
            } else {
              if ((! ajp->flags.checkQualSyntax) || (StringInStringList (str, validRptString))) {
                FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                                  FALSE, TRUE, TILDE_IGNORE);
                FFAddOneString(ffstring, str, FALSE, TRUE, TILDE_TO_SPACES);
                FFAddOneChar(ffstring, '\n', FALSE);
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

        /* in release_mode, must be of the form 123..4567 or a single-token label,
           or (technically illegal but common) letters and semicolons - NO LONGER CHECKED */

        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          if (! StringHasNoText (gbq->val)) {
            tmp = StringSave (gbq->val);
            str = tmp;
            len = StringLen (str);
            if (len > 1 && *str == '(' && str [len - 1] == ')' &&
                StringChr (str + 1, '(') == NULL /* && StringChr (str, ',') != NULL */) {
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
                if ((! ajp->flags.checkQualSyntax) || (ValidateRptUnit (str))) {
                  TrimSpacesAroundString (str);
                  FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=\"",
                                    FALSE, TRUE, TILDE_IGNORE);
                  FFAddOneString(ffstring, str, FALSE, TRUE, TILDE_TO_SPACES);
                  FFAddOneChar(ffstring, '\"', FALSE);
                  FFAddOneChar(ffstring, '\n', FALSE);
                }
                str = ptr;
              }
            } else {
              if ((! ajp->flags.checkQualSyntax) || (ValidateRptUnit (str))) {
                TrimSpacesAroundString (str);
                FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=\"",
                                  FALSE, TRUE, TILDE_IGNORE);
                FFAddOneString(ffstring, str, FALSE, TRUE, TILDE_TO_SPACES);
                FFAddOneChar(ffstring, '\"', FALSE);
                FFAddOneChar(ffstring, '\n', FALSE);
              }
            }
            MemFree (tmp);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_compare :
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
                if ((! ajp->flags.checkQualSyntax) || ValidateCompareQual (str, is_ged)) {
                  FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                                    FALSE, TRUE, TILDE_IGNORE);
                  FFAddOneString(ffstring, str, FALSE, TRUE, TILDE_TO_SPACES);
                  FFAddOneChar(ffstring, '\n', FALSE);
                }
                str = ptr;
              }
            } else {
              if ((! ajp->flags.checkQualSyntax) || ValidateCompareQual (str, is_ged)) {
                FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                                  FALSE, TRUE, TILDE_IGNORE);
                FFAddOneString(ffstring, str, FALSE, TRUE, TILDE_TO_SPACES);
                FFAddOneChar(ffstring, '\n', FALSE);
              }
            }
            MemFree (tmp);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_replace :
        gbq = qvp [idx].gbq;
        if (gbq == NULL || (ajp->flags.dropIllegalQuals && (! AllowedValQual (featdeftype, idx)))) break;
        if (lasttype == NULL) {
          lasttype = gbq->qual;
        }
        while (gbq != NULL && StringICmp (gbq->qual, lasttype) == 0) {
          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                            FALSE, TRUE, TILDE_IGNORE);
          FFAddOneChar(ffstring, '\"', FALSE);
          if (!StringHasNoText (gbq->val)) {
            FFAddOneString(ffstring, gbq->val, FALSE, TRUE, TILDE_TO_SPACES);
          }
          FFAddOneChar(ffstring, '\"', FALSE);
          FFAddOneChar(ffstring, '\n', FALSE);
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
            FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                              FALSE, TRUE, TILDE_IGNORE);
            FFAddOneString(ffstring, gbq->val, FALSE, TRUE, TILDE_TO_SPACES);
            FFAddOneChar(ffstring, '\n', FALSE);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_site :
        if (! StringHasNoText (qvp [idx].str)) {
          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                            FALSE, TRUE, TILDE_IGNORE);
          FFAddTextToString(ffstring, "\"", qvp[idx].str, "\"", FALSE, TRUE, TILDE_TO_SPACES);
          FFAddOneChar(ffstring, '\n', FALSE);
        }
        break;

      case Qual_class_bond :
        if (! StringHasNoText (qvp [idx].str)) {
          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                            FALSE, TRUE, TILDE_IGNORE);
          FFAddTextToString(ffstring, "\"", qvp[idx].str, "\"", FALSE, TRUE, TILDE_TO_SPACES);
          FFAddOneChar(ffstring, '\n', FALSE);
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
            FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                              FALSE, TRUE, TILDE_IGNORE);
            FFAddOneString(ffstring, gbq->val, FALSE, TRUE, TILDE_TO_SPACES);
            FFAddOneChar(ffstring, '\n', FALSE);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_sec_str :
        sec_str = qvp [idx].num;
        if (sec_str > 0 && sec_str <= 3) {
          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                            FALSE, TRUE, TILDE_IGNORE);
          FFAddTextToString(ffstring, "\"", secStrText[sec_str], "\"", 
                            FALSE, FALSE, TILDE_IGNORE);
          FFAddOneChar(ffstring, '\n', FALSE);
        }
        break;

      case Qual_class_seq_loc :
        slp = qvp [idx].slp;
        if (slp != NULL) {
          FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                            FALSE, TRUE, TILDE_IGNORE);
          str = FFFlatLoc (ajp, target, slp, /* ajp->masterStyle */ FALSE);
          FFAddTextToString(ffstring, "\"", str, "\"", 
                            FALSE, TRUE, TILDE_TO_SPACES);
          FFAddOneChar(ffstring, '\n', FALSE);
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
                  newloc = SeqLocReMapEx (sip, ajp->ajp.slp, slp, 0, FALSE, ajp->masterStyle);
 
                  SeqIdFree (sip);
                  if (newloc != NULL) {
                    A2GBSeqLocReplaceID (newloc, ajp->ajp.slp);
                    str = FFFlatLoc (ajp, target, newloc, ajp->masterStyle);
                    SeqLocFree (newloc);
                  }
                } else {
                  str = FFFlatLoc (ajp, target, slp, ajp->masterStyle);
                }
                if (str != NULL) {
                  residue = cbaa.value.intvalue;
                  ptr = Get3LetterSymbol (ajp, seqcode, sctp, residue);
                  if (ptr == NULL) {
                    ptr = "OTHER";
                  }
                  FFAddOneString(ffstring, "/transl_except=", FALSE, FALSE, TILDE_IGNORE);
                  FFAddTextToString(ffstring, "(pos:", str, ",", FALSE, FALSE, TILDE_IGNORE);
                  FFAddTextToString(ffstring, "aa:", ptr, ")", FALSE, FALSE, TILDE_IGNORE);
                  FFAddOneChar(ffstring, '\n', FALSE);
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
        if (slp != NULL && ajp->ajp.slp != NULL) {
          sip = SeqIdParse ("lcl|dummy");
          split = FALSE;
          newloc = SeqLocReMapEx (sip, ajp->ajp.slp, slp, 0, FALSE, ajp->masterStyle);
          /*
          newloc = SeqLocCopyRegion (sip, slp, bsp, left, right, strand, &split);
          */
          SeqIdFree (sip);
          slp = newloc;
          if (newloc != NULL) {
            A2GBSeqLocReplaceID (newloc, ajp->ajp.slp);
          }
        }
        str = qvp [FTQUAL_trna_aa].str;
        if (slp != NULL && StringDoesHaveText (str)) {
          if (ajp->mode == RELEASE_MODE) { /* !!! quarantined pending collab approval !!! */
            if (slp->choice == SEQLOC_INT) {
              sintp = (SeqIntPtr) slp->data.ptrvalue;
              if (sintp != NULL) {
                sprintf(numbuf, "%ld", (long) sintp->from + 1);
                FFAddTextToString (ffstring, "/anticodon=(pos:", numbuf, "..",
                                   FALSE, FALSE, TILDE_IGNORE);
                sprintf (numbuf, "%ld", (long) sintp->to + 1);
                FFAddTextToString (ffstring, NULL, numbuf, ",",
                                   FALSE, FALSE, TILDE_IGNORE);
                FFAddTextToString (ffstring, "aa:", str, ")",
                                  FALSE, FALSE, TILDE_IGNORE);
                FFAddOneChar (ffstring, '\n', FALSE);
              }
            }
          } else {
            tmp = FFFlatLoc (ajp, target, slp, ajp->masterStyle);
            if (tmp != NULL) {
              FFAddTextToString (ffstring, "/anticodon=(pos:", tmp, ",",
                                 FALSE, FALSE, TILDE_IGNORE);
              FFAddTextToString(ffstring, "aa:", str, ")",
                                FALSE, FALSE, TILDE_IGNORE);
              FFAddOneChar(ffstring, '\n', FALSE);
            }
            MemFree (tmp);
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
            FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[idx].name, "=",
                              FALSE, FALSE, TILDE_IGNORE);
            FFAddOneString(ffstring, gbq->val, FALSE, FALSE, TILDE_TO_SPACES);
            FFAddOneChar(ffstring, '\n', FALSE);
          }
          gbq = gbq->next;
        }
        break;

      case Qual_class_pubset :
        vnp = qvp [idx].vnp;
        if (vnp != NULL && asp != NULL && asp->referenceArray != NULL) {
          citlist = NULL;
          pmidlist = NULL;
          for (ppr = vnp->data.ptrvalue; ppr != NULL; ppr = ppr->next) {
            j = MatchRef (ppr, asp->referenceArray, asp->numReferences);
            if (j > 0) {
              ValNodeAddInt (&citlist, 0, (Int4) j);
            } else if (is_other && ppr->choice == PUB_PMid && ajp->mode != RELEASE_MODE) {
              pmid = ppr->data.intvalue;
              ValNodeAddInt (&pmidlist, 0, (Int4) pmid);
            }
          }
          citlist = ValNodeSort (citlist, SortVnpByInt);
          pmidlist = ValNodeSort (pmidlist, SortVnpByInt);
          for (vnp = citlist; vnp != NULL; vnp = vnp->next) {
            j = (Int2) vnp->data.intvalue;
            if (j > 0) {
              sprintf (numbuf, "%d", (int) j);
              FFAddTextToString(ffstring, "/citation=[", numbuf, "]",
                                FALSE, TRUE, TILDE_TO_SPACES);
              FFAddOneChar(ffstring, '\n', FALSE);
            }
          }
          for (vnp = pmidlist; vnp != NULL; vnp = vnp->next) {
            pmid = (Int4) vnp->data.intvalue;
            if (pmid > 0) {
              sprintf (numbuf, "%ld", (long) pmid);
              FFAddTextToString(ffstring, "/citation=[PUBMED ", numbuf, "]",
                                FALSE, TRUE, TILDE_TO_SPACES);
              FFAddOneChar(ffstring, '\n', FALSE);
            }
          }
          citlist = ValNodeFree (citlist);
          pmidlist = ValNodeFree (pmidlist);
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
              if (StringCmp (dbt->db, "PID") == 0 || StringCmp (dbt->db, "GI") == 0) {
                okay = FALSE;
              }
              if (okay && idx == FTQUAL_db_xref && qvp [FTQUAL_gene_xref].vnp != NULL) {
                if (DbxrefAlreadyInGeneXref (dbt, qvp [FTQUAL_gene_xref].vnp)) {
                  okay = FALSE;
                }
              }

              if (okay) {
                if (! StringHasNoText (oip->str)) {
                  if (StringLen (oip->str) < 80) {
                    sprintf (buf, "%s", oip->str);
                  }
                } else {
                  sprintf (buf, "%ld", (long) oip->id);
                }
              }
            }
          }
          if (! StringHasNoText (buf)) {
            if (StringICmp (buf, protein_pid_g) != 0) {
              /* already sorted and uniqued by BasicSeqEntryCleanup, per feature */
              if (StringICmp (dbt->db, "LocusID") == 0 || StringICmp (dbt->db, "InterimID") == 0) {
                if (FFStringSearch (ffstring, dbt->db, 0) >= 0) {
                  okay = FALSE;
                }
              }
              if (okay) {
                FFAddOneString(ffstring, "/db_xref=\"", FALSE, FALSE, TILDE_IGNORE);
                FF_www_db_xref(ajp, ffstring, dbt->db, buf);
                FFAddOneString(ffstring, "\"\n", FALSE, FALSE, TILDE_IGNORE);
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
                  sip->choice == SEQID_TPD ||
                  sip->choice == SEQID_GPIPE) {
                choice = sip->choice;
                if (SeqIdWrite (sip, seqid, PRINTID_TEXTID_ACC_VER, sizeof (seqid)) != NULL) {
                  FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=\"",
                                    FALSE, FALSE, TILDE_IGNORE);
                  FF_www_protein_id(ajp, ffstring, seqid);
                  FFAddOneString(ffstring, "\"\n", FALSE, FALSE, TILDE_IGNORE);
                }
              } else if (sip->choice == SEQID_GI) {
                if (choice == 0) {
                  sprintf (seqid, "%ld", (long) sip->data.intvalue);
                  FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=\"",
                                    FALSE, FALSE, TILDE_IGNORE);
                  FF_www_protein_id(ajp, ffstring, seqid);
                  FFAddOneString(ffstring, "\"\n", FALSE, FALSE, TILDE_IGNORE);
                }
                sprintf (seqid, "%ld", (long) sip->data.intvalue);
                FFAddOneString(ffstring, "/db_xref=\"", FALSE, FALSE, TILDE_IGNORE);
                FF_www_db_xref(ajp, ffstring, "GI", seqid);
                FFAddOneString(ffstring, "\"\n", FALSE, FALSE, TILDE_IGNORE);
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
                } else if (dbt != NULL) {
                  pbsp = BioseqFind (sip);
                  if (pbsp != NULL && pbsp->id != NULL && pbsp->id->next == NULL) {
                    if (SeqIdWrite (sip, seqid, PRINTID_REPORT, sizeof (seqid)) != NULL) {
                      FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=\"",
                                        FALSE, FALSE, TILDE_IGNORE);
                      FF_www_protein_id(ajp, ffstring, seqid);
                      FFAddOneString(ffstring, "\"\n", FALSE, FALSE, TILDE_IGNORE);
                    }
                  }
                }
              }
            }
          } else {
            if (sip->choice == SEQID_GI) {
              gi = sip->data.intvalue;
              if (GetAccnVerFromServer (gi, seqid)) {
                if ((! ajp->flags.dropIllegalQuals) || ValidateAccn (seqid) == 0) {
                  FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=\"",
                                    FALSE, FALSE, TILDE_IGNORE);
                  FF_www_protein_id(ajp, ffstring, seqid);
                  FFAddOneString(ffstring, "\"\n", FALSE, FALSE, TILDE_IGNORE);
                } else {
                  ajp->relModeError = TRUE;
                }
              } else {
                sip = GetSeqIdForGI (gi);
                if (sip != NULL && SeqIdWrite (sip, seqid, PRINTID_TEXTID_ACC_VER, sizeof (seqid)) != NULL) {
                  if ((! ajp->flags.dropIllegalQuals) || ValidateAccn (seqid) == 0) {
                    FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=\"",
                                    FALSE, FALSE, TILDE_IGNORE);
                    FF_www_protein_id(ajp, ffstring, seqid);
                    FFAddOneString(ffstring, "\"\n", FALSE, FALSE, TILDE_IGNORE);
                  } else {
                    ajp->relModeError = TRUE;
                  }
                } else if (! ajp->flags.dropIllegalQuals) {
                  sprintf (seqid, "%ld", (long) gi);
                  FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=\"",
                                    FALSE, FALSE, TILDE_IGNORE);
                  FF_www_protein_id(ajp, ffstring, seqid);
                  FFAddOneString(ffstring, "\"\n", FALSE, FALSE, TILDE_IGNORE);
                } else {
                  ajp->relModeError = TRUE;
                }
              }

              sprintf (seqid, "%ld", (long) gi);
              FFAddOneString(ffstring, "/db_xref=\"", FALSE, FALSE, TILDE_IGNORE);
              FF_www_db_xref(ajp, ffstring, "GI", seqid);
              FFAddOneString(ffstring, "\"\n", FALSE, FALSE, TILDE_IGNORE);
            } else if (SeqIdWrite (sip, seqid, PRINTID_TEXTID_ACC_VER, sizeof (seqid)) != NULL) {
              if ((! ajp->flags.dropIllegalQuals) || ValidateAccn (seqid) == 0) {
                FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals [idx].name, "=\"",
                                  FALSE, FALSE, TILDE_IGNORE);
                FF_www_protein_id(ajp, ffstring, seqid);
                FFAddOneString(ffstring, "\"\n", FALSE, FALSE, TILDE_IGNORE);
              } else {
                ajp->relModeError = TRUE;
              }

              gi = GetGIForSeqId (sip);
              if (gi > 0) {
                sprintf (seqid, "%ld", (long) gi);
                FFAddOneString(ffstring, "/db_xref=\"", FALSE, FALSE, TILDE_IGNORE);
                FF_www_db_xref(ajp, ffstring, "GI", seqid);
                FFAddOneString(ffstring, "\"\n", FALSE, FALSE, TILDE_IGNORE);
              }
            }
          }
        }
        break;

      case Qual_class_translation :
        if (qvp [idx].ble) {
          if ((prod == NULL && ajp->transIfNoProd) || ajp->alwaysTranslCds) {
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
                prtlen = StringLen (str);
                if (prtlen > 1) {
                   if (str [prtlen - 1] == '*') {
                     str [prtlen - 1] = '\0';
                   }
                }
                if (! StringHasNoText (str)) {
                  FFAddTextToString(ffstring, "/translation=\"", str, "\"", 
                                    FALSE, TRUE, TILDE_TO_SPACES);
                  FFAddOneChar(ffstring, '\n', FALSE);
                }
                MemFree (str);
              }
            } else {
              ajp->relModeError = TRUE;
            }
          } else if (prod != NULL) {
            len = SeqLocLen (sfp->product);
            if (len > 0) {
              if (SeqLocStart (location) == 0 || (bsp != NULL && SeqLocStop (location) == bsp->length - 1)) {
                at_end = TRUE;
              }
              str = (CharPtr) MemNew ((size_t) (len + 1) * sizeof (Char));
              protein_seq = str;
              /*
              if (ajp->flags.iupacaaOnly) {
                code = Seq_code_iupacaa;
              } else {
                code = Seq_code_ncbieaa;
              }
              */
              SeqPortStreamLoc (sfp->product, STREAM_EXPAND_GAPS | STREAM_CORRECT_INVAL, (Pointer) &protein_seq, SaveGBSeqSequence);
              if (! StringHasNoText (str)) {
                FFAddTextToString(ffstring, "/translation=\"", str, "\"", 
                                  FALSE, TRUE, TILDE_TO_SPACES);
                FFAddOneChar(ffstring, '\n', FALSE);
              }
              MemFree (str);
            } else {
              ajp->relModeError = TRUE;
            }
          }
        }
        break;
      
      case Qual_class_transcription :
        if (qvp [idx].ble && ajp->showTranscript) {
          if ((prod == NULL && ajp->transIfNoProd) || ajp->alwaysTranslCds) {
            str = GetSequenceByFeature (sfp);
            if (str != NULL) {
              ptr = str;
              ch = *ptr;
              while (ch != '\0') {
                *ptr = TO_UPPER (ch);
                ptr++;
                ch = *ptr;
              }
              if (! StringHasNoText (str)) {
                FFAddTextToString(ffstring, "/transcription=\"", str, "\"", 
                                  FALSE, TRUE, TILDE_TO_SPACES);
                FFAddOneChar(ffstring, '\n', FALSE);
              }
              MemFree (str);
            }
          } else if (prod != NULL) {
            len = SeqLocLen (sfp->product);
            if (len > 0) {
              str = (CharPtr) MemNew ((size_t) (len + 2) * sizeof (Char));
              SeqPortStreamLoc (sfp->product, STREAM_EXPAND_GAPS | STREAM_CORRECT_INVAL, (Pointer) str, NULL);
              if (! StringHasNoText (str)) {
                FFAddTextToString(ffstring, "/transcription=\"", str, "\"", 
                                  FALSE, TRUE, TILDE_TO_SPACES);
                FFAddOneChar(ffstring, '\n', FALSE);
              }
              MemFree (str);
            }
          } else {
            str = GetSequenceByFeature (sfp);
            if (str != NULL) {
              ptr = str;
              ch = *ptr;
              while (ch != '\0') {
                *ptr = TO_UPPER (ch);
                ptr++;
                ch = *ptr;
              }
              if (! StringHasNoText (str)) {
                FFAddTextToString(ffstring, "/transcription=\"", str, "\"", 
                                  FALSE, TRUE, TILDE_TO_SPACES);
                FFAddOneChar(ffstring, '\n', FALSE);
              }
              MemFree (str);
            }
          }
        }
        break;
      
      case Qual_class_peptide :
        if (qvp [idx].ble) {
          if (ajp->showPeptide) {
            str = GetSequenceByFeature (sfp);
            if (str != NULL) {
              ptr = str;
              ch = *ptr;
              while (ch != '\0') {
                *ptr = TO_UPPER (ch);
                ptr++;
                ch = *ptr;
              }
              if (! StringHasNoText (str)) {
                FFAddTextToString(ffstring, "/peptide=\"", str, "\"", 
                                  FALSE, TRUE, TILDE_TO_SPACES);
                FFAddOneChar(ffstring, '\n', FALSE);
              }
              MemFree (str);
            }
          }
        }
        break;
      
      case Qual_class_illegal :
        for (vnp = qvp [idx].vnp; vnp != NULL; vnp = vnp->next) {
          str = (CharPtr) vnp->data.ptrvalue;
          if (str != NULL) {
            FFAddOneString(ffstring, str, FALSE, FALSE, TILDE_TO_SPACES);
            FFAddNewLine(ffstring);
          }
        }
        break;

      case Qual_class_note :
        if (! ajp->flags.goQualsToNote) {

          /* in sequin_mode and dump_mode in RefSeq, GO terms show up as separate /qualifiers */

          for (j = 0, jdx = feat_note_order [j]; jdx != 0; j++, jdx = feat_note_order [j]) {
            switch (asn2gnbk_featur_quals [jdx].qualclass) {

              case Qual_class_go :
                if (qvp [jdx].ufp != NULL) {
                  for (entry = qvp [jdx].ufp; entry != NULL; entry = entry->next) {
                    if (entry == NULL || entry->choice != 11) break;
                    ufp = (UserFieldPtr)  entry->data.ptrvalue;
                    str = GetGOtext (ufp, ajp);
                    if (! StringHasNoText (str)) {
                      FFAddTextToString(ffstring, "/", asn2gnbk_featur_quals[jdx].name, "=",
                                        FALSE, TRUE, TILDE_IGNORE);
                      FFAddTextToString(ffstring, "\"", str, "\"", 
                                        FALSE, FALSE, TILDE_IGNORE);
                      FFAddOneChar(ffstring, '\n', FALSE);
                    }
                    MemFree (str);
                  }
                }
                break;

              default :
                break;
            }
          }
        }

        /*head = NULL;*/
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
                  FFAddString_NoRedund (unique, prefix, buf, NULL);
                  add_period = FALSE;
                } else if (jdx == FTQUAL_maploc) {
                  if (!IsEllipsis (qvp [jdx].str))
                    s_RemovePeriodFromEnd (qvp [jdx].str);
                  sprintf (buf, "Map location %s", qvp [jdx].str);
                  FFAddString_NoRedund (unique, prefix, buf, NULL);
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
                  FFAddString_NoRedund (unique, prefix, str, NULL);
                  MemFree (str);
                  if (hadProtDesc) {
                    suppress_period = TRUE;
                  }
                } else if (jdx == FTQUAL_prot_note) {
                  str = StringSave (qvp [jdx].str);
                  TrimSpacesAndJunkFromEnds (str, TRUE);
                  if (! IsEllipsis (str))
                    s_RemovePeriodFromEnd (str);
                  FFAddString_NoRedund (unique, prefix, str, NULL);
                  MemFree (str);
                  add_period = FALSE;
                } else if (jdx == FTQUAL_prot_desc) {
                  str = StringSave (qvp [jdx].str);
                  TrimSpacesAndJunkFromEnds (str, TRUE);
                  if (! IsEllipsis (str))
                    add_period = s_RemovePeriodFromEnd (str);
                  FFAddString_NoRedund (unique, prefix, str, NULL);
                  MemFree (str);
                  hadProtDesc = TRUE; /* gi|347886|gb|M96268.1|ECOUBIA */
                } else {
                  if (! IsEllipsis (qvp [jdx].str)) {
                    s_RemovePeriodFromEnd (qvp [jdx].str);
                  }
                  FFAddString_NoRedund (unique, prefix, qvp [jdx].str, NULL);
                  add_period = FALSE;
                }
                prefix = "; ";
              }
              break;

            case Qual_class_encodes :
              if (! StringHasNoText (qvp [jdx].str)) {
                if (! IsEllipsis (qvp [jdx].str)) {
                  s_RemovePeriodFromEnd (qvp [jdx].str);
                }
                FFAddTextToString (unique, prefix, "encodes ", NULL, FALSE, FALSE, TILDE_IGNORE);
                FFAddString_NoRedund (unique, NULL, qvp [jdx].str, NULL);
                prefix = "; ";
                add_period = FALSE;
              }
              break;

            case Qual_class_locus_tag :
              if (! StringHasNoText (qvp [jdx].str)) {
                if (! IsEllipsis (qvp [jdx].str)) {
                  s_RemovePeriodFromEnd (qvp [jdx].str);
                }
                FFAddTextToString (unique, prefix, "locus_tag: ", NULL, FALSE, FALSE, TILDE_IGNORE);
                FFAddString_NoRedund (unique, NULL, qvp [jdx].str, NULL);
                prefix = "; ";
                add_period = FALSE;
              }
              break;

            case Qual_class_go :
              if (ajp->flags.goQualsToNote && qvp [jdx].ufp != NULL) {
                for (entry = qvp [jdx].ufp; entry != NULL; entry = entry->next) {
                  if (entry == NULL || entry->choice != 11) break;
                  ufp = (UserFieldPtr)  entry->data.ptrvalue;
                  str = GetGOtext (ufp, ajp);
                  if (! StringHasNoText (str)) {
                    if (StringCmp (prefix, "; ") == 0) {
                      prefix = ";\n";
                    }
                    FFAddTextToString (unique, prefix, asn2gnbk_featur_quals[jdx].name, ": ", FALSE, FALSE, TILDE_IGNORE);
                    FFAddTextToString(unique, NULL, str, NULL, FALSE, FALSE, TILDE_IGNORE);
                  }
                  MemFree (str);
                  prefix = "; ";
                  add_period = FALSE;
                }
              }
              break;

            case Qual_class_method :
              if (! StringHasNoText (qvp [jdx].str)) {
                if ( FFEmpty(unique) ) {
                  prefix = "Method: ";
                } else {
                  prefix = "; Method: ";
                }
                FFAddString_NoRedund (unique, prefix, qvp [jdx].str, NULL);
                prefix = "; ";
                add_period = FALSE;
              }
              break;

            case Qual_class_valnode :
              for (vnp = qvp [jdx].vnp; vnp != NULL; vnp = vnp->next) {
                str = (CharPtr) vnp->data.ptrvalue;
                if (! StringHasNoText (str)) {
                  FFAddString_NoRedund (unique, prefix, str, NULL);
                  prefix = "; ";
                  add_period = FALSE;
                }
              }
              break;

            case Qual_class_gene_syn :
              numsyns = 0;
              for (vnp = qvp [jdx].vnp; vnp != NULL; vnp = vnp->next) {
                str = (CharPtr) vnp->data.ptrvalue;
                if (! StringHasNoText (str)) {
                  numsyns++;
                }
              }
              if (numsyns > 0) {
                if (numsyns > 1) {
                  FFAddTextToString (unique, prefix, "synonyms: ", NULL, FALSE, FALSE, TILDE_IGNORE);
                } else {
                  FFAddTextToString (unique, prefix, "synonym: ", NULL, FALSE, FALSE, TILDE_IGNORE);
                }
                prefix = NULL;
                for (vnp = qvp [jdx].vnp; vnp != NULL; vnp = vnp->next) {
                  str = (CharPtr) vnp->data.ptrvalue;
                  if (! StringHasNoText (str)) {
                    FFAddTextToString (unique, prefix, str, NULL, FALSE, FALSE, TILDE_IGNORE);
                    prefix = ", ";
                  }
                }
                prefix = "; ";
                add_period = FALSE;
              }
              break;

            case Qual_class_region :
#ifdef ASN2GNBK_STRIP_NOTE_PERIODS
              FFAddTextToString(unique, prefix, qvp [jdx].str, NULL, FALSE, FALSE, TILDE_IGNORE);
#else
              region = NULL;
              if (! StringHasNoText (qvp [jdx].str)) {
                if ( FFEmpty(unique) ) {
                    prefix = "Region: ";
                } else {
                  prefix = "; Region: ";
                }
                region = MemNew(StringLen(prefix) + StringLen(qvp [jdx].str) + 1);
                if ( region != NULL ) {
                    sprintf(region, "%s%s", prefix, (qvp [jdx].str));
                    FFAddString_NoRedund(unique, NULL, region, NULL);
                    region = MemFree(region);
                } else {
                    FFAddTextToString(unique, prefix, qvp [jdx].str, NULL, FALSE, FALSE, TILDE_IGNORE);
                }
                prefix = "; ";
                add_period = FALSE;
              }
#endif
              break;

            case Qual_class_site :
              if (! StringHasNoText (qvp [jdx].str)) {
                FFAddString_NoRedund (unique, prefix, qvp [jdx].str, " site");
                add_period = FALSE;
                prefix = "\n";
              }
              break;

            case Qual_class_bond :
              if (! StringHasNoText (qvp [jdx].str)) {
                FFAddString_NoRedund (unique, prefix, qvp [jdx].str, " bond");
                add_period = FALSE;
                prefix = "\n";
              }
              break;

            case Qual_class_protnames :
              /* process gene sgml for check against subsequent protein names */
              start = NULL;
              if (! StringHasNoText (qvp [FTQUAL_gene].str)) {
                if (is_journalscan) {
                  ascii_len = Sgml2AsciiLen (qvp [FTQUAL_gene].str);
                  start = ascii = MemNew ((size_t) (10 + ascii_len));
                  if (start != NULL) {
                    ascii = Sgml2Ascii (qvp [FTQUAL_gene].str, ascii, ascii_len + 1);
                  }
                } else {
                  start = StringSaveNoNull (qvp [FTQUAL_gene].str);
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
                      /* if (NotInGeneSyn (str, gene_syn)) { */
                        FFAddString_NoRedund (unique, prefix, str, NULL);
                        prefix = "; ";
                        add_period = FALSE;
                      /* } */
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
                      FFAddString_NoRedund (unique, prefix, gbq->val, NULL);
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
                  FFAddString_NoRedund (unique, prefix, str, NULL);
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
                    FFAddString_NoRedund (unique, prefix, "codon recognized: ", numbuf);
                    prefix = "; ";
                  }
                } else {
                  isTRNA = TRUE;
                  FFAddString_NoRedund (unique, prefix, "codons recognized: ", numbuf);
                  prefix = "; ";
                  add_period = FALSE;
                }
              }
              break;

            case Qual_class_model_ev :
              uop = qvp [jdx].uop;
              if (uop != NULL) {
                str = NULL;
                VisitUserObjectsInUop (sfp->ext, (Pointer) &str, GetStrFormRNAEvidence);
                if (! StringHasNoText (str)) {
                  FFAddString_NoRedund (unique, prefix, str, NULL);
                  prefix = "; ";
                  add_period = FALSE;
                }
              }
              break;

            case Qual_class_seq_id :
              sip = qvp [jdx].sip;
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
                        sip->choice == SEQID_TPD ||
                        sip->choice == SEQID_GPIPE) {
                      choice = sip->choice;
                      if (SeqIdWrite (sip, seqid, PRINTID_TEXTID_ACC_VER, sizeof (seqid)) != NULL) {
                        FFAddTextToString(unique, prefix, "transcript found in: ", seqid,
                                          FALSE, FALSE, TILDE_IGNORE);
                      }
                    } else if (sip->choice == SEQID_GI) {
                      if (choice == 0) {
                        sprintf (seqid, "%ld", (long) sip->data.intvalue);
                        FFAddTextToString(unique, prefix, "transcript found in: ", seqid,
                                          FALSE, FALSE, TILDE_IGNORE);
                      }
                    }
                  }
                } else {
                  if (sip->choice == SEQID_GI) {
                    gi = sip->data.intvalue;
                    if (GetAccnVerFromServer (gi, seqid)) {
                      if ((! ajp->flags.dropIllegalQuals) || ValidateAccn (seqid) == 0) {
                        FFAddTextToString(unique, prefix, "transcript found in: ", seqid,
                                          FALSE, FALSE, TILDE_IGNORE);
                      }
                    } else {
                      sip = GetSeqIdForGI (gi);
                      if (sip != NULL && SeqIdWrite (sip, seqid, PRINTID_TEXTID_ACC_VER, sizeof (seqid)) != NULL) {
                        if ((! ajp->flags.dropIllegalQuals) || ValidateAccn (seqid) == 0) {
                          FFAddTextToString(unique, prefix, "transcript found in: ", seqid,
                                          FALSE, FALSE, TILDE_IGNORE);
                        }
                      } else if (! ajp->flags.dropIllegalQuals) {
                        sprintf (seqid, "%ld", (long) gi);
                          FFAddTextToString(unique, prefix, "transcript found in: ", seqid,
                                          FALSE, FALSE, TILDE_IGNORE);
                      }
                    }
                  } else if (SeqIdWrite (sip, seqid, PRINTID_TEXTID_ACC_VER, sizeof (seqid)) != NULL) {
                    if ((! ajp->flags.dropIllegalQuals) || ValidateAccn (seqid) == 0) {
                          FFAddTextToString(unique, prefix, "transcript found in: ", seqid,
                                          FALSE, FALSE, TILDE_IGNORE);
                    }
                  }
                }
                prefix = "; ";
                add_period = FALSE;
              }
              break;

            default :
              break;
          }
        }

        if ( !FFEmpty(unique) ) {
          notestr = FFToCharPtr(unique);
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
    
          FFAddOneString(ffstring, "/note=\"", FALSE, FALSE, TILDE_IGNORE);
          FFAddOneString(ffstring, notestr, FALSE, TRUE, TILDE_EXPAND);
          FFAddOneString(ffstring, "\"\n", FALSE, FALSE, TILDE_IGNORE);

          MemFree (notestr);
          /*ValNodeFreeData (head);*/
        }
        break;

      default:
        break;

    }
  }
  FFRecycleString(ajp, unique);
}



static void FF_asn2gb_www_featkey (
  StringItemPtr ffstring,
  CharPtr key,
  SeqLocPtr slp,
  Int4 from,
  Int4 to,
  Uint1 strand,
  Uint4 itemID
)

{
  BioseqPtr  bsp;
  Int4       gi = 0;
  SeqIdPtr   sip;
  Boolean    is_aa = FALSE;
  Char gi_buf[16];
  Char itemID_buf[16];

  bsp = BioseqFindFromSeqLoc (slp);
  if (bsp != NULL) {
    is_aa = ISA_aa (bsp->mol);
    for (sip = bsp->id; sip != NULL; sip = sip->next) {
      if (sip->choice == SEQID_GI) {
        gi = (Int4) sip->data.intvalue;
      }
    }
  }

  sprintf(gi_buf, "%ld", (long)gi);
  sprintf(itemID_buf, "%ld", (long)itemID);


  FFAddOneString(ffstring, "<a href=", FALSE, FALSE, TILDE_IGNORE);
  FFAddOneString(ffstring, link_feat, FALSE, FALSE, TILDE_IGNORE);
  FFAddOneString(ffstring, "val=", FALSE, FALSE, TILDE_IGNORE);
  FFAddOneString(ffstring, gi_buf, FALSE, FALSE, TILDE_IGNORE);
  if (itemID > 0) {
    FFAddOneString(ffstring, "&itemID=", FALSE, FALSE, TILDE_IGNORE);
    FFAddOneString(ffstring, itemID_buf, FALSE, FALSE, TILDE_IGNORE);
  }
  

  if ( is_aa ) {
    FFAddOneString(ffstring, "&view=gpwithparts>", FALSE, FALSE, TILDE_IGNORE);
  } else {
    FFAddOneString(ffstring, "&view=gbwithparts>", FALSE, FALSE, TILDE_IGNORE);
  }

  FFAddOneString(ffstring, key, FALSE, FALSE, TILDE_IGNORE);
  FFAddOneString(ffstring, "</a>", FALSE, FALSE, TILDE_IGNORE);
}


NLM_EXTERN SeqIdPtr SeqLocIdForProduct (
  SeqLocPtr product
)

{
  SeqIdPtr   sip;
  SeqLocPtr  slp;

  /* in case product is a SEQLOC_EQUIV */

  if (product == NULL) return NULL;
  sip = SeqLocId (product);
  if (sip != NULL) return sip;
  slp = SeqLocFindNext (product, NULL);
  while (slp != NULL) {
    sip = SeqLocId (slp);
    if (sip != NULL) return sip;
    slp = SeqLocFindNext (product, slp);
  }
  return NULL;
}

NLM_EXTERN CharPtr goQualType [] = {
  "", "Process", "Component", "Function", NULL
};

static void RecordGoFieldsInQVP (
  UserFieldPtr ufp,
  Pointer userdata
)

{
  UserFieldPtr  entry;
  Int2          i;
  ObjectIdPtr   oip;
  QualValPtr    qvp;

  qvp = (QualValPtr) userdata;

  if (ufp == NULL || ufp->choice != 11) return;
  oip = ufp->label;
  if (oip == NULL) return;
  for (i = 0; goQualType [i] != NULL; i++) {
    if (StringICmp (oip->str, goQualType [i]) == 0) break;
  }
  if (goQualType [i] == NULL) return;

  entry = ufp->data.ptrvalue;
  if (entry == NULL || entry->choice != 11) return;

  /* ufp = (UserFieldPtr)  entry->data.ptrvalue; */
  switch (i) {
    case 1 :
      qvp [FTQUAL_go_process].ufp = entry;
      break;
    case 2 :
      qvp [FTQUAL_go_component].ufp = entry;
      break;
    case 3 :
      qvp [FTQUAL_go_function].ufp = entry;
      break;
    default :
      break;
  }
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
  } else if (StringCmp (oip->str, "GeneOntology") == 0) {
    VisitUserFieldsInUop (uop, (Pointer) qvp, RecordGoFieldsInQVP);
  }
}

static void AddIntervalsToGbfeat (
  GBFeaturePtr gbfeat,
  SeqLocPtr location,
  BioseqPtr target
)

{
  Char           accn [41];
  SeqLocPtr      copy = NULL;
  Int4           from;
  GBIntervalPtr  gbint;
  Int4           gi;
  GBIntervalPtr  last = NULL;
  Int4           point;
  SeqIntPtr      sint;
  SeqIdPtr       sip;
  SeqLocPtr      slp;
  SeqPntPtr      spp;
  Int4           to;
  Int4           swap;

  if (gbfeat == NULL || location == NULL) return;
  if (target != NULL) {
    copy = SeqLocMerge (target, location, NULL, FALSE, TRUE, FALSE);
    location = copy;
  }

  slp = SeqLocFindNext (location, NULL);
  while (slp != NULL) {
    from = 0;
    to = 0;
    point = 0;
    sip = NULL;
    switch (slp->choice) {
      case SEQLOC_WHOLE :
        sip = (SeqIdPtr) slp->data.ptrvalue;
        if (sip != NULL) {
          from = 1;
          to = SeqLocLen (slp);
          if (to < 0) {
            sip = NULL;
          }
        }
        break;
      case SEQLOC_INT :
        sint = (SeqIntPtr) slp->data.ptrvalue;
        if (sint != NULL) {
          from = sint->from + 1;
          to = sint->to + 1;
          sip = sint->id;
          if (sint->strand == Seq_strand_minus && from < to) {
            swap = from;
            from = to;
            to = swap;
          }
        }
        break;
      case SEQLOC_PNT :
        spp = (SeqPntPtr) slp->data.ptrvalue;
        if (spp != NULL) {
          point = spp->point + 1;
          sip = spp->id;
        }
        break;
      default :
        break;
    }
    if (sip != NULL) {
      accn [0] = '\0';
      if (sip->choice == SEQID_GI) {
        gi = sip->data.intvalue;
        if (! GetAccnVerFromServer (gi, accn)) {
          accn [0] = '\0';
        }
        if (StringHasNoText (accn)) {
          sip = GetSeqIdForGI (gi);
          SeqIdWrite (sip, accn, PRINTID_TEXTID_ACC_VER, sizeof (accn));
          SeqIdFree (sip);
        }
      } else {
        SeqIdWrite (sip, accn, PRINTID_TEXTID_ACC_VER, sizeof (accn));
      }
      if (! StringHasNoText (accn)) {
        gbint = GBIntervalNew ();
        if (gbint != NULL) {
          gbint->from = from;
          gbint->to = to;
          gbint->point = point;
          gbint->accession = StringSave (accn);
          if (gbfeat->intervals == NULL) {
            gbfeat->intervals = gbint;
          } else {
            last->next = gbint;
          }
          last = gbint;
        }
      }
    }
    slp = SeqLocFindNext (location, slp);
  }

  SeqLocFree (copy);
}

static void ParseException (
  CharPtr original,
  CharPtr PNTR exception_string,
  CharPtr PNTR exception_note,
  Boolean dropIllegalQuals,
  Boolean isRefSeq
)

{
  ValNodePtr  excpt = NULL, note = NULL, vnp;
  Boolean     first, found;
  Int2        i;
  size_t      len;
  CharPtr     ptr, str, tmp;

  if (StringHasNoText (original) || exception_string == NULL || exception_note == NULL) return;

  if (! dropIllegalQuals) {
    *exception_string = StringSave (original);
    return;
  }

  str = StringSave (original);
  if (str == NULL) return;

  tmp = str;
  while (! StringHasNoText (tmp)) {
    ptr = StringChr (tmp, ',');
    if (ptr != NULL) {
      *ptr = '\0';
      ptr++;
    }
    TrimSpacesAroundString (tmp);
    found = FALSE;
    for (i = 0; validExceptionString [i] != NULL; i++) {
      if (StringICmp (tmp, validExceptionString [i]) == 0) {
        found = TRUE;
        break;
      }
    }
    if (! found) {
      if (isRefSeq) {
        for (i = 0; validRefSeqExceptionString [i] != NULL; i++) {
          if (StringICmp (tmp, validRefSeqExceptionString [i]) == 0) {
            found = TRUE;
            break;
          }
        }
      }
    }
    if (found) {
      ValNodeCopyStr (&excpt, 0, tmp);
    } else {
      ValNodeCopyStr (&note, 0, tmp);
    }
    tmp = ptr;
  }

  if (excpt != NULL) {
    for (vnp = excpt, len = 0; vnp != NULL; vnp = vnp->next) {
      tmp = (CharPtr) vnp->data.ptrvalue;
      len += StringLen (tmp) + 3;
    }
    ptr = (CharPtr) MemNew (len + 2);
    if (ptr != NULL) {
      for (vnp = excpt, first = TRUE; vnp != NULL; vnp = vnp->next) {
        if (! first) {
          StringCat (ptr, ", ");
        }
        tmp = (CharPtr) vnp->data.ptrvalue;
        StringCat (ptr, tmp);
        first = FALSE;
      }
    }
    *exception_string = ptr;
  }

  if (note != NULL) {
    for (vnp = note, len = 0; vnp != NULL; vnp = vnp->next) {
      tmp = (CharPtr) vnp->data.ptrvalue;
      len += StringLen (tmp) + 3;
    }
    ptr = (CharPtr) MemNew (len + 2);
    if (ptr != NULL) {
      for (vnp = note, first = TRUE; vnp != NULL; vnp = vnp->next) {
        if (! first) {
          StringCat (ptr, ", ");
        }
        tmp = (CharPtr) vnp->data.ptrvalue;
        StringCat (ptr, tmp);
        first = FALSE;
      }
    }
    *exception_note = ptr;
  }

  ValNodeFreeData (excpt);
  ValNodeFreeData (note);
  MemFree (str);
}

static SeqFeatPtr GetOverlappingGeneInEntity (
  Uint2 entityID,
  SeqMgrFeatContextPtr fcontext,
  SeqMgrFeatContextPtr gcontext,
  SeqLocPtr locforgene
)

{
  SeqFeatPtr   gene = NULL;
  SeqEntryPtr  sep, oldscope;
  SeqInt       sint;
  SeqIntPtr    sintp;
  SeqPntPtr    spp;
  SeqPnt       spt;
  ValNode      vn;

  sep = GetTopSeqEntryForEntityID (entityID);
  oldscope = SeqEntrySetScope (sep);
  if (fcontext->featdeftype == FEATDEF_variation && locforgene != NULL) {
    /* first check same strand for variation */
    gene = SeqMgrGetOverlappingGene (locforgene, gcontext);
    if (gene == NULL) {
      /* special case variation feature - copy location but set strand both */
      if (locforgene->choice == SEQLOC_INT && locforgene->data.ptrvalue != NULL) {
        sintp = (SeqIntPtr) locforgene->data.ptrvalue;
        MemSet ((Pointer) &sint, 0, sizeof (SeqInt));
        MemSet ((Pointer) &vn, 0, sizeof (ValNode));
        sint.from = sintp->from;
        sint.to = sintp->to;
        sint.id = sintp->id;
        sint.if_from = sintp->if_from;
        sint.if_to = sintp->if_to;
        sint.strand = Seq_strand_both;
        vn.choice = SEQLOC_INT;
        vn.data.ptrvalue = (Pointer) &sint;
        gene = SeqMgrGetOverlappingGene (&vn, gcontext);

      } else if (locforgene->choice == SEQLOC_PNT && locforgene->data.ptrvalue != NULL) {
        spp = (SeqPntPtr) locforgene->data.ptrvalue;
        MemSet ((Pointer) &spt, 0, sizeof (SeqPnt));
        MemSet ((Pointer) &vn, 0, sizeof (ValNode));
        spt.point = spp->point;
        spt.id = spp->id;
        spt.fuzz = spp->fuzz;
        spt.strand = Seq_strand_both;
        vn.choice = SEQLOC_PNT;
        vn.data.ptrvalue = (Pointer) &spt;
        gene = SeqMgrGetOverlappingGene (&vn, gcontext);

      } else {
        gene = SeqMgrGetOverlappingGene (locforgene, gcontext);
      }
    }
  } else {
    gene = SeqMgrGetOverlappingGene (locforgene, gcontext);
  }
  SeqEntrySetScope (oldscope);
  return gene;
}

static CharPtr FormatFeatureBlockEx (
  IntAsn2gbJobPtr ajp,
  Asn2gbSectPtr asp,
  BioseqPtr bsp,
  BioseqPtr target,
  SeqFeatPtr sfp,
  SeqMgrFeatContextPtr fcontext,
  QualValPtr qvp,
  FmtType format,
  IntFeatBlockPtr ifp,
  Boolean isProt,
  Boolean doKey
)

{
  Uint1              aa;
  Int2               bondidx;
  BioseqSetPtr       bssp;
  Choice             cbaa;
  CodeBreakPtr       cbp;
  BioseqPtr          cdna;
  SeqFeatPtr         cds;
  Char               ch;
  Uint1              code = Seq_code_ncbieaa;
  CdRegionPtr        crp;
  SeqMgrDescContext  dcontext;
  Boolean            encode_prefix = FALSE;
  CharPtr            exception_note = NULL;
  CharPtr            exception_string = NULL;
  Uint1              featdeftype;
  Uint1              from;
  GBQualPtr          gbq;
  GBFeaturePtr       gbfeat = NULL;
  GBSeqPtr           gbseq;
  SeqMgrFeatContext  gcontext;
  ValNodePtr         gcp;
  SeqFeatPtr         gene = NULL;
  ValNodePtr         gene_syn = NULL;
  GeneRefPtr         grp;
  IntCdsBlockPtr     icp;
  Uint2              idx;
  ValNodePtr         illegal = NULL;
  ImpFeatPtr         imp = NULL;
  IndxPtr            index;
  Boolean            is_ged = FALSE;
  Boolean            is_gps = FALSE;
  Boolean            is_journalscan = FALSE;
  Boolean            is_other = FALSE;
  Uint4              itemID;
  CharPtr            key = NULL;
  CharPtr            lasttype = NULL;
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
  SeqMgrFeatContext  ocontext;
  SeqEntryPtr        oldscope;
  SeqFeatPtr         operon = NULL;
  Uint2              partial;
  SeqMgrFeatContext  pcontext;
  BioseqPtr          prd;
  CharPtr            precursor_comment = NULL;
  BioseqPtr          prod = NULL;
  SeqFeatPtr         prot;
  Boolean            protein = FALSE;
  Char               protein_pid_g [32];
  ProtRefPtr         prp;
  ProtRefPtr         prpxref;
  Boolean            pseudo = FALSE;
  CharPtr            ptr;
  Int2               qualclass;
  Uint1              residue;
  Int4               right = -1;
  RnaRefPtr          rrp;
  SeqCodeTablePtr    sctp;
  SeqDescrPtr        sdp;
  SeqEntryPtr        sep;
  Uint1              seqcode;
  Uint1              shift;
  SeqIdPtr           sip;
  Int2               siteidx;
  SeqMapTablePtr     smtp;
  Boolean            split;
  CharPtr            str;
  Uint1              strand = Seq_strand_unknown;
  CharPtr            tmp;
  tRNAPtr            trna;
  BioseqPtr          unlockme = NULL;
  ValNodePtr         vnp;
  StringItemPtr      ffstring;


  if (ajp == NULL || fcontext == NULL || qvp == NULL || ifp == NULL) return NULL;

  ffstring = FFGetString(ajp);
  if ( ffstring == NULL ) return NULL;

  if (ajp->index && asp != NULL) {
    index = &asp->index;
  } else {
    index = NULL;
  }

  if (ajp->gbseq && asp != NULL) {
    gbseq = &asp->gbseq;
  } else {
    gbseq = NULL;
  }
  
  protein_pid_g [0] = '\0';

  itemID = fcontext->itemID;

  if (doKey) {
	/* may need to map location between aa and dna */
  
	if (ifp->mapToNuc) {
  
	  /* map mat_peptide, etc., to nucleotide coordinates */
  
	  sip = SeqLocId (sfp->location);
	  prd = BioseqFind (sip);
	  cds = SeqMgrGetCDSgivenProduct (prd, NULL);
	  CheckSeqLocForPartial (sfp->location, &noLeft, &noRight);
	  location = aaFeatLoc_to_dnaFeatLoc (cds, sfp->location);
	  SetSeqLocPartial (location, noLeft, noRight);
	  locforgene = location;
	  loc = location;
  
	} else if (ifp->mapToProt) {
  
	  /* map CDS to protein product coordinates */
  
	  sip = SeqLocIdForProduct (sfp->product);
	  prd = BioseqFind (sip);
	  cds = SeqMgrGetCDSgivenProduct (prd, NULL);
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
  
	} else if (ifp->mapToPep) {
  
	  /* map protein processing from precursor to subpeptide Bioseq */
  
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

    if (bsp != NULL) {
	  for (sip = bsp->id; sip != NULL; sip = sip->next) {
	    switch (sip->choice) {
	      case SEQID_OTHER :
	        is_other = TRUE;
	        break;
	      case SEQID_GIBBSQ :
	      case SEQID_GIBBMT :
	      case SEQID_GIIM :
	        is_journalscan = TRUE;
	        break;
	      case SEQID_GENBANK :
	      case SEQID_EMBL :
	      case SEQID_DDBJ :
	      case SEQID_TPG :
	      case SEQID_TPE :
	      case SEQID_TPD :
	        is_ged = TRUE;
	        break;
	      default :
	      break;
	    }
	  }
    }
 
    if (ajp->refseqConventions) {
      is_other = TRUE;
    }

	featdeftype = fcontext->featdeftype;
	if (featdeftype < FEATDEF_GENE || featdeftype >= FEATDEF_MAX) {
	  featdeftype = FEATDEF_BAD;
	}
	key = FindKeyFromFeatDefType (featdeftype, TRUE);
  
	if (format == GENPEPT_FMT && isProt) {
	  if (featdeftype == FEATDEF_REGION) {
		key = "Region";
	  } else if (featdeftype == FEATDEF_BOND) {
		key = "Bond";
	  } else if (featdeftype == FEATDEF_SITE) {
		key = "Site";
	  }
	  if (ifp->mapToPep) {
		if (featdeftype >= FEATDEF_preprotein && featdeftype <= FEATDEF_transit_peptide_aa) {
		  key = "Precursor";
		  itemID = 0;
		}
	  }
	}
	if (! isProt) {
	  if (featdeftype == FEATDEF_preprotein) {
	    if (! is_other) {
	      key = "misc_feature";
	      encode_prefix = TRUE;
	    }
	  }
	}
  
	/* deal with unmappable impfeats */
  
	if (featdeftype == FEATDEF_BAD && fcontext->seqfeattype == SEQFEAT_IMP) {
	  imp = (ImpFeatPtr) sfp->data.value.ptrvalue;
	  if (imp != NULL) {
		key = imp->key;
	  }
	}
  
	FFStartPrint(ffstring, format, 5, 21, NULL, 0, 5, 21, "FT", /* ifp->firstfeat */ FALSE);
	if (ajp->ajp.slp != NULL) {
	  FFAddOneString(ffstring, key, FALSE, FALSE, TILDE_IGNORE);
	} else if ( GetWWW(ajp) /* && SeqMgrGetParentOfPart (bsp, NULL) == NULL */ ) {
	  FF_asn2gb_www_featkey (ffstring, key, sfp->location, fcontext->left + 1, fcontext->right + 1, fcontext->strand, itemID);
	} else {
	  FFAddOneString(ffstring, key, FALSE, FALSE, TILDE_IGNORE);
	}
	FFAddNChar(ffstring, ' ', 21 - 5 - StringLen(key), FALSE);
  
	if (gbseq != NULL) {
	  gbfeat = GBFeatureNew ();
	  if (gbfeat != NULL) {
		gbfeat->key = StringSave (key);
	  }
	}
  
	if (imp == NULL || StringHasNoText (imp->loc)) {
  
	  
	  if (ajp->ajp.slp != NULL) {
		sip = SeqIdParse ("lcl|dummy");
		left = GetOffsetInBioseq (ajp->ajp.slp, bsp, SEQLOC_LEFT_END);
		right = GetOffsetInBioseq (ajp->ajp.slp, bsp, SEQLOC_RIGHT_END);
		strand = SeqLocStrand (ajp->ajp.slp);
		split = FALSE;
		newloc = SeqLocReMapEx (sip, ajp->ajp.slp, location, 0, FALSE, ajp->masterStyle);
		/*
		newloc = SeqLocCopyRegion (sip, location, bsp, left, right, strand, &split);
		*/
		SeqIdFree (sip);
		if (newloc == NULL) return NULL;
		A2GBSeqLocReplaceID (newloc, ajp->ajp.slp);
		str = FFFlatLoc (ajp, target, newloc, ajp->masterStyle);
		SeqLocFree (newloc);
	  } else {
		str = FFFlatLoc (ajp, target, location, ajp->masterStyle);
	  }
	} else {
	  str = StringSave (imp->loc);
	}
	if ( GetWWW(ajp) ) {
	  FF_www_featloc (ffstring, str);
	} else {
	  FFAddOneString(ffstring, str, FALSE, FALSE, TILDE_IGNORE);
	}
  
	if (gbseq != NULL) {
	  if (gbfeat != NULL) {
		gbfeat->location = StringSave (str);
		if (ajp->masterStyle) {
		  AddIntervalsToGbfeat (gbfeat, location, target);
		} else {
		  AddIntervalsToGbfeat (gbfeat, location, NULL);
		}
	  }
	}
  
	MemFree (str);

  } else {

    location = sfp->location;
    locforgene = sfp->location;
  }

  /* populate qualifier table from feature fields */

  /*
  if (sfp->partial == TRUE)
    sfp->partial = FlatAnnotPartial(sfp, use_product);
  */

  if (sfp->partial) {
    partial = SeqLocPartialCheck (location);
    if (partial == SLP_COMPLETE /* || partial > SLP_OTHER */ ) {
      qvp [FTQUAL_partial].ble = TRUE;
    }
    if (LookForFuzz (location)) {
      qvp [FTQUAL_partial].ble = FALSE;
    }
    if (imp != NULL) {
      if (StringChr (imp->loc, '<') != NULL || StringChr (imp->loc, '>') != NULL) {
        qvp [FTQUAL_partial].ble = FALSE;
      }
    }

    /* hide unclassified /partial in RELEASE_MODE and ENTREZ_MODE */

    if (ajp->mode == RELEASE_MODE || ajp->mode == ENTREZ_MODE) {
      qvp [FTQUAL_partial].ble = FALSE;
    }
    /*
    if (ajp->flags.checkQualSyntax) {
      switch (featdeftype) {
      case FEATDEF_conflict:
      case FEATDEF_mutation:
      case FEATDEF_N_region:
      case FEATDEF_polyA_site:
        qvp [FTQUAL_partial].ble = FALSE;
        break;
      default:
        break;
      }
    }
    */
  }
  if (ifp->mapToProt) {
    qvp [FTQUAL_partial].ble = FALSE;
  }

  if (sfp->pseudo) {
    pseudo = TRUE;
  }

  if (fcontext->seqfeattype == SEQFEAT_GENE) {
    grp = (GeneRefPtr) sfp->data.value.ptrvalue;
    if (grp != NULL) {
      if (! StringHasNoText (grp->locus)) {
        qvp [FTQUAL_gene].str = grp->locus;
        qvp [FTQUAL_locus_tag].str = grp->locus_tag;
        qvp [FTQUAL_gene_desc].str = grp->desc;
        qvp [FTQUAL_gene_syn].vnp = grp->syn;
      } else if (grp->locus_tag != NULL) {
        qvp [FTQUAL_locus_tag].str = grp->locus_tag;
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
    if (! ajp->flags.geneSynsToNote) {
      qvp [FTQUAL_gene_syn_refseq].vnp = qvp [FTQUAL_gene_syn].vnp;
      qvp [FTQUAL_gene_syn].vnp = NULL;
    }
    operon = SeqMgrGetOverlappingOperon (locforgene, &ocontext);
    if (operon != NULL) {
      for (gbq = operon->qual; gbq != NULL; gbq = gbq->next) {
        if (StringCmp (gbq->qual, "operon") == 0) {
          qvp [FTQUAL_operon].gbq = gbq;
        }
      }
    }

  } else if (fcontext->featdeftype != FEATDEF_operon && fcontext->featdeftype != FEATDEF_gap) {

    grp = SeqMgrGetGeneXref (sfp);
    if (grp != NULL) {
      qvp [FTQUAL_gene_xref].vnp = grp->db;
    }
    if (grp == NULL) {
      gene = GetOverlappingGeneInEntity (ajp->ajp.entityID, fcontext, &gcontext, locforgene);
      if (gene == NULL && ajp->ajp.entityID != sfp->idx.entityID) {
        gene = GetOverlappingGeneInEntity (sfp->idx.entityID, fcontext, &gcontext, locforgene);
      }
      if (gene != NULL) {
        qvp [FTQUAL_gene_note].str = gene->comment;
        grp = (GeneRefPtr) gene->data.value.ptrvalue;
        if (gene->pseudo) {
          pseudo = TRUE;
        }
        if (grp != NULL && grp->db != NULL) {
          qvp [FTQUAL_gene_xref].vnp = grp->db;
        } else {
          qvp [FTQUAL_gene_xref].vnp = gene->dbxref;
        }
      }
    }
    if (grp != NULL && grp->pseudo) {
      pseudo = TRUE;
    }
    if (grp != NULL && (! SeqMgrGeneIsSuppressed (grp)) &&
        (fcontext->featdeftype != FEATDEF_repeat_region || gene == NULL)) {
      if (! StringHasNoText (grp->locus)) {
        qvp [FTQUAL_gene].str = grp->locus;
        qvp [FTQUAL_locus_tag].str = grp->locus_tag;
        gene_syn = grp->syn;
      } else if (! StringHasNoText (grp->locus_tag)) {
        qvp [FTQUAL_locus_tag].str = grp->locus_tag;
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
    if (grp != NULL && fcontext->featdeftype != FEATDEF_variation) {
      qvp [FTQUAL_gene_allele].str = grp->allele; /* now propagating /allele */
    }
    if (fcontext->seqfeattype != SEQFEAT_CDREGION &&
        fcontext->seqfeattype != SEQFEAT_RNA) {
      qvp [FTQUAL_gene_xref].vnp = NULL;
    }
    if (fcontext->featdeftype != FEATDEF_operon) {
      grp = SeqMgrGetGeneXref (sfp);
      if (grp == NULL || (! SeqMgrGeneIsSuppressed (grp))) {
        operon = SeqMgrGetOverlappingOperon (locforgene, &ocontext);
        if (operon != NULL) {
          for (gbq = operon->qual; gbq != NULL; gbq = gbq->next) {
            if (StringCmp (gbq->qual, "operon") == 0) {
              qvp [FTQUAL_operon].gbq = gbq;
            }
          }
        }
      }
    }

    /* specific fields set here */

    switch (fcontext->seqfeattype) {
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
                      if (ajp->flags.selenocysteineToNote) {
                        qvp [FTQUAL_selenocysteine_note].str = "selenocysteine";
                      } else {
                        qvp [FTQUAL_selenocysteine].ble = TRUE;
                      }
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

              /* suppress table 1, but always show it in GBSeqXML */

              if (qvp [FTQUAL_transl_table].num == 1 && ajp->gbseq == NULL) {
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

          sip = SeqLocIdForProduct (sfp->product);
          qvp [FTQUAL_protein_id].sip = sip;

          sep = GetTopSeqEntryForEntityID (ajp->ajp.entityID);

          if (! ajp->alwaysTranslCds) {

            /* by default only show /translation if product bioseq is within entity */

            oldscope = SeqEntrySetScope (sep);
            prod = BioseqFind (sip);
            SeqEntrySetScope (oldscope);

            if (prod == NULL && ajp->showFarTransl) {

              /* but flag can override and force far /translation */

              prod = BioseqLockById (sip);
              unlockme = prod;
            }
          }

          prp = NULL;

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

          /* protein xref overrides names, but should not prevent /protein_id, etc. */

          prpxref = SeqMgrGetProtXref (sfp);
          if (prpxref != NULL) {
            prp = prpxref;
          }
          if (prp != NULL) {
            vnp = prp->name;
            if (vnp != NULL && (! StringHasNoText ((CharPtr) vnp->data.ptrvalue))) {
              qvp [FTQUAL_cds_product].str = (CharPtr) vnp->data.ptrvalue;
              vnp = vnp->next;
              if (ajp->flags.extraProductsToNote) {
                qvp [FTQUAL_prot_names].vnp = vnp;
              } else {
                qvp [FTQUAL_extra_products].vnp = vnp;
              }
            }
            qvp [FTQUAL_prot_desc].str = prp->desc;
            qvp [FTQUAL_prot_activity].vnp = prp->activity;
            qvp [FTQUAL_prot_EC_number].vnp = prp->ec;
          }

          if (! pseudo) {
            if (prod != NULL || ajp->transIfNoProd || ajp->alwaysTranslCds) {
              if (doKey) {
                qvp [FTQUAL_translation].ble = TRUE;
              }
            }
          }

          if (ifp->isCDS) {
            icp = (IntCdsBlockPtr) ifp;
            qvp [FTQUAL_figure].str = icp->fig;
            qvp [FTQUAL_maploc].str = icp->maploc;
          }
        } else {
          qvp [FTQUAL_coded_by].slp = sfp->location;

          /* show /evidence on coded_by CDS */

          qvp [FTQUAL_evidence].num = sfp->exp_ev;

          crp = (CdRegionPtr) sfp->data.value.ptrvalue;
          if (crp != NULL) {
            if (crp->frame > 1) {
              qvp [FTQUAL_codon_start].num = crp->frame;
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
                      if (ajp->flags.selenocysteineToNote) {
                        qvp [FTQUAL_selenocysteine_note].str = "selenocysteine";
                      } else {
                        qvp [FTQUAL_selenocysteine].ble = TRUE;
                      }
                    }
                  }
                }
              }
            }
          }
          if (gene != NULL) {

            /* propagate old_locus_tag to CDS in GenPept format */

            for (gbq = gene->qual; gbq != NULL; gbq = gbq->next) {
              if (StringHasNoText (gbq->val)) continue;
              idx = GbqualToFeaturIndex (gbq->qual);
              if (idx == FTQUAL_old_locus_tag) {
                qvp [FTQUAL_old_locus_tag].gbq = gbq;
              }
            }
          }
        }
        break;
      case SEQFEAT_PROT :
        if (! ifp->mapToPep) {
          prp = (ProtRefPtr) sfp->data.value.ptrvalue;
          if (prp != NULL) {
            vnp = prp->name;
            if (vnp != NULL && (! StringHasNoText ((CharPtr) vnp->data.ptrvalue))) {
              qvp [FTQUAL_product].str = (CharPtr) vnp->data.ptrvalue;
              vnp = vnp->next;
              qvp [FTQUAL_prot_names].vnp = vnp;
            }
            if (format != GENPEPT_FMT) {
              qvp [FTQUAL_prot_desc].str = prp->desc;
            } else {
              qvp [FTQUAL_prot_name].str = prp->desc;
            }
            if (format != GENPEPT_FMT || prp->processed != 2) {
              qvp [FTQUAL_prot_activity].vnp = prp->activity;
            }
            qvp [FTQUAL_prot_EC_number].vnp = prp->ec;
          }
          sip = SeqLocIdForProduct (sfp->product);
          if (sip != NULL) {
            /* for RefSeq records or GenBank not release_mode */
            if (is_other || (! ajp->flags.forGbRelease)) {
              qvp [FTQUAL_protein_id].sip = sip;
            }
            prod = BioseqFind (sip);
          }
        } else {
          qvp [FTQUAL_derived_from].slp = sfp->location;
          sip = SeqLocIdForProduct (sfp->product);
          if (sip != NULL) {
            prod = BioseqFind (sip);
            if (prod != NULL) {
              prot = SeqMgrGetBestProteinFeature (prod, NULL);
              if (prot != NULL) {
                precursor_comment = prot->comment;
              }
            }
          }
        }
        prp = (ProtRefPtr) sfp->data.value.ptrvalue;
        if (prp != NULL) {
          if (! pseudo) {
            if (ajp->showPeptide) {
              if (prp->processed == 2 || prp->processed == 3 || prp->processed == 4) {
                qvp [FTQUAL_peptide].ble = TRUE;
              }
            }
          }
          if (prp->processed == 3 || prp->processed == 4) {
            if (! is_other) {
              /* Only RefSeq allows product on signal or transit peptide */
              qvp [FTQUAL_product].str = NULL;
            }
          }
          if (prp->processed == 1 && encode_prefix && (! is_other)) {
            qvp [FTQUAL_encodes].str = qvp [FTQUAL_product].str;
            qvp [FTQUAL_product].str = NULL;
          }
        }
        break;
      case SEQFEAT_RNA :
        rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
        if (rrp != NULL) {
          if (rrp->pseudo) {
            pseudo = TRUE;
          }
          sip = SeqLocIdForProduct (sfp->product);
          if (sip != NULL) {
            /* for RefSeq records or GenBank not release_mode or entrez_mode */
            if (is_other || (ajp->mode == SEQUIN_MODE || ajp->mode == DUMP_MODE)) {
              qvp [FTQUAL_transcript_id].sip = sip;
            } else {
              /* otherwise now goes in note */
              qvp [FTQUAL_transcript_id_note].sip = sip; /* !!! remove October 15, 2003 !!! */
            }

            if (! ajp->alwaysTranslCds) {

              /* by default only show /transcription if product bioseq is within entity */

              oldscope = SeqEntrySetScope (sep);
              prod = BioseqFind (sip);
              SeqEntrySetScope (oldscope);

              if (prod == NULL && ajp->showFarTransl) {

                /* but flag can override and force far /transcription */

                prod = BioseqLockById (sip);
                unlockme = prod;
              }
            }
          }
          if (rrp->type == 2) {
            if (! pseudo) {
              if (ajp->showTranscript) {
                qvp [FTQUAL_transcription].ble = TRUE;
              }
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
                  if (aa != '*') {
                    idx = aa - (64 + shift);
                  } else {
                    idx = 25;
                  }
                  if (idx > 0 && idx < 26) {
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
        if (format == GENPEPT_FMT && featdeftype == FEATDEF_REGION && isProt) {
          qvp [FTQUAL_region_name].str = (CharPtr) sfp->data.value.ptrvalue;
        } else {
          qvp [FTQUAL_region].str = (CharPtr) sfp->data.value.ptrvalue;
        }
        break;
      case SEQFEAT_COMMENT :
        break;
      case SEQFEAT_BOND :
        bondidx = (Int2) sfp->data.value.intvalue;
        if (bondidx == 255) {
          bondidx = 5;
        }
        if (bondidx > 0 && bondidx < 6) {
          if (format == GENPEPT_FMT && isProt) {
            qvp [FTQUAL_bond_type].str = bondList [bondidx];
          } else {
            qvp [FTQUAL_bond].str = bondList [bondidx];
          }
        }
        break;
      case SEQFEAT_SITE :
        siteidx = (Int2) sfp->data.value.intvalue;
        if (siteidx == 255) {
          siteidx = 27;
        }
        if (siteidx > 0 && siteidx < 28) {
          if (format == GENPEPT_FMT && isProt) {
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

  if (fcontext->featdeftype == FEATDEF_repeat_region) {
    pseudo = FALSE;
  }

  qvp [FTQUAL_pseudo].ble = pseudo;

  qvp [FTQUAL_seqfeat_note].str = sfp->comment;

  /* if RELEASE_MODE, check list of features that can have /pseudo */

  if (ajp->flags.dropIllegalQuals && pseudo  &&
      (fcontext->seqfeattype == SEQFEAT_RNA || fcontext->seqfeattype == SEQFEAT_IMP) ) {
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
    case  FEATDEF_10_signal:
    case  FEATDEF_35_signal:
      qvp [FTQUAL_pseudo].ble = FALSE;
        break;
    default:
        break;
    }
  }

  /*
  if (format != GENPEPT_FMT) {
    qvp [FTQUAL_evidence].num = sfp->exp_ev;
  }
  */
  qvp [FTQUAL_evidence].num = sfp->exp_ev;

  /* exception currently legal only on CDS */

  if ((! ajp->flags.dropIllegalQuals) || fcontext->seqfeattype == SEQFEAT_CDREGION) {
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
          (! StringInStringList (qvp [FTQUAL_seqfeat_note].str, validExceptionString))) {
        qvp [FTQUAL_exception].str = NULL;
      }
    }
    if (ajp->flags.dropIllegalQuals) {
      str = qvp [FTQUAL_exception].str;
      ParseException (str, &exception_string, &exception_note, TRUE, is_other);
      qvp [FTQUAL_exception].str = exception_string;
      qvp [FTQUAL_exception_note].str = exception_note;
      /*
      if (is_other) {
        if (! StringInStringList (qvp [FTQUAL_exception].str, validRefSeqExceptionString)) {
          qvp [FTQUAL_exception_note].str = qvp [FTQUAL_exception].str;
          qvp [FTQUAL_exception].str = NULL;
        }
      } else {
        if (! StringInStringList (qvp [FTQUAL_exception].str, validExceptionString)) {
          qvp [FTQUAL_exception_note].str = qvp [FTQUAL_exception].str;
          qvp [FTQUAL_exception].str = NULL;
        }
      }
      */
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

  /* mat_peptide precursor same as sfp->comment will suppress /note in GenPept */

  if (precursor_comment != NULL && StringCmp (precursor_comment, sfp->comment) == 0) {
    qvp [FTQUAL_seqfeat_note].str = NULL;
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
        str = MemNew (sizeof (Char) * (StringLen (gbq->qual) + StringLen (tmp) + 10));
        if (str != NULL) {
          if (qualclass == Qual_class_quote) {
            if (StringIsJustQuotes (tmp)) {
              sprintf (str, "/%s", gbq->qual);
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
      /* NC_001823 leave in prot_desc if no cds_product */
      if (qvp [FTQUAL_cds_product].str != NULL) {
        qvp [FTQUAL_prot_desc].str = NULL;
      }
    }
  }

  /* remove comment that equals a gene synonym */

  if (format != GENPEPT_FMT && (! ifp->mapToProt)) {
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
    qvp [FTQUAL_selenocysteine_note].str = NULL;
  }

  /* if /allele inherited from gene, suppress allele gbqual on feature */

  if (qvp [FTQUAL_gene_allele].str != NULL) {
    qvp [FTQUAL_allele].gbq = NULL;
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

  /* optionally populate indexes for NCBI internal database */

  if (index != NULL) {
    if (! StringHasNoText (qvp [FTQUAL_gene].str)) {
      ValNodeCopyStrToHead (&(index->genes), 0, qvp [FTQUAL_gene].str);
    }
  }

  if (doKey) {
    FFAddOneChar(ffstring, '\n', FALSE);
  }

  /* Build the flat file */
  FormatFeatureBlockQuals (ffstring, ajp, asp, bsp, featdeftype, gene_syn,
                           lasttype, location, prod,
                           protein_pid_g, qvp,
                           left, right, strand,
                           sfp, target, is_other,
                           is_journalscan, is_gps, is_ged);

  /* ??? and then deal with the various note types separately (not in order table) ??? */

  /* free aa-dna or dna-aa mapped location */

  SeqLocFree (loc);

  ValNodeFreeData (illegal);
  MemFree (exception_string);
  MemFree (exception_note);

  BioseqUnlock (unlockme);

  str = FFEndPrint (ajp, ffstring, format, 21, 21, 21, 21, "FT");

  /* optionally populate gbseq for XML-ized GenBank format */

  if (gbseq != NULL) {
    if (gbfeat != NULL) {
      AddFeatureToGbseq (gbseq, gbfeat, str, sfp);
    }
  }

  FFRecycleString(ajp, ffstring);
  return str;
}

NLM_EXTERN CharPtr FormatFeatureBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
)

{
  IntAsn2gbJobPtr    ajp;
  Asn2gbSectPtr      asp;
  BioseqPtr          bsp;
  SeqMgrFeatContext  fcontext;
  FmtType            format;
  ValNodePtr         head;
  QualValPtr         qvp;
  SeqFeatPtr         sfp;
  CharPtr            str;
  BioseqPtr          target;

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
  format = afp->format;

  /* all features in this list are known to be valid for the designated mode */

  sfp = SeqMgrGetDesiredFeature (bbp->entityID, NULL, bbp->itemID, 0, NULL, &fcontext);
  if (sfp == NULL) return NULL;

  /* five-column feature table uses special code for formatting */

  if (ajp->format == FTABLE_FMT) {
    head = NULL;
    PrintFtableLocAndQuals (ajp, &head, target, sfp, &fcontext);
    str = MergeFFValNodeStrs (head);
    ValNodeFreeData (head);
    return str;
  }

  /* otherwise do regular flatfile formatting */

  return FormatFeatureBlockEx (ajp, asp, bsp, target, sfp, &fcontext, qvp,
                               format, (IntFeatBlockPtr) bbp, ISA_aa (bsp->mol), TRUE);
}

NLM_EXTERN CharPtr FormatFeatHeaderBlock (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp
)

{
  IntAsn2gbJobPtr  ajp;
  Asn2gbSectPtr    asp;
  BioseqPtr        bsp;
  Char             ch;
  Boolean          has_space;
  Char             id [64];
  ObjectIdPtr      oip;
  CharPtr          ptr;
  SeqIdPtr         sip;
  SeqIdPtr         sip2;
  CharPtr          str = NULL;
  BioseqPtr        target;
  Char             tmp [53];

  if (afp == NULL || bbp == NULL) return NULL;
  ajp = afp->ajp;
  if (ajp == NULL) return NULL;
  asp = afp->asp;
  if (asp == NULL) return NULL;
  target = asp->target;
  bsp = asp->bsp;
  if (target == NULL || bsp == NULL) return NULL;

  /* five-column feature table uses special code for formatting */

  if (ajp->format == FTABLE_FMT) {
    sip = SeqIdFindBest (target->id, 0);
    if (sip == NULL) return NULL;
    id [0] = '\0';

    if (sip->choice == SEQID_GI) {
      sip2 = GetSeqIdForGI (sip->data.intvalue);
      if (sip2 != NULL) {
        sip = sip2;
      }
    }
    if (sip->choice == SEQID_LOCAL) {
      oip = (ObjectIdPtr) sip->data.ptrvalue;
      if (oip != NULL && StringDoesHaveText (oip->str)) {
        has_space = FALSE;
        ptr = oip->str;
        ch = *ptr;
        while (ch != '\0') {
          if (IS_WHITESP (ch)) {
            has_space = TRUE;
          }
          ptr++;
          ch = *ptr;
        }
        if (has_space) {
          sprintf (id, "lcl|%c%s%c", (char) '"', oip->str, (char) '"');
        }
      }
    }

    if (id [0] == '\0') {
      SeqIdWrite (sip, id, PRINTID_FASTA_LONG, sizeof (id) - 1);
    }
    if (! StringHasNoText (id)) {
      sprintf (tmp, ">Feature %s\n", id);
      str = StringSave (tmp);
    }
    return str;
  }

  /* otherwise do regular flatfile formatting */

  return StringSaveNoNull (bbp->string);
}


/* stand alone function to produce qualifiers in genbank style */

static void StripLeadingSpaces (
  CharPtr str
)

{
  Uchar    ch;
  CharPtr  dst;
  CharPtr  ptr;


  if (str == NULL || str [0] == '\0') return;

  dst = str;
  ptr = str;
  ch = *ptr;
  while (ch != '\0') {
    while (ch == ' ') {
      ptr++;
      ch = *ptr;
    }
    while (ch != '\n' && ch != '\r') {
      *dst = ch;
      dst++;
      ptr++;
      ch = *ptr;
    }
    *dst = ch;
    dst++;
    ptr++;
    ch = *ptr;
  }
  *dst = '\0';
}

NLM_EXTERN void DoImmediateRemoteFeatureFormat (
  Asn2gbFormatPtr afp,
  BaseBlockPtr bbp,
  SeqFeatPtr sfp
)

{
  IntAsn2gbJobPtr    ajp;
  Asn2gbSectPtr      asp;
  BlockType          blocktype;
  BioseqPtr          bsp;
  SeqMgrFeatContext  fcontext;
  size_t             max;
  SeqEntryPtr        oldscope;
  QualValPtr         qvp = NULL;
  SeqEntryPtr        sep;
  SeqLocPtr          slp;
  CharPtr            str = NULL;
  BioseqPtr          target;

  if (afp == NULL || bbp == NULL || sfp == NULL) return;
  asp = afp->asp;
  if (asp == NULL) return;
  target = asp->target;
  bsp = asp->bsp;
  if (target == NULL || bsp == NULL) return;
  ajp = afp->ajp;
  if (ajp == NULL) return;

  blocktype = bbp->blocktype;
  if (blocktype < LOCUS_BLOCK || blocktype > SLASH_BLOCK) return;

  max = (size_t) (MAX (ASN2GNBK_TOTAL_SOURCE, ASN2GNBK_TOTAL_FEATUR));
  qvp = MemNew (sizeof (QualVal) * (max + 5));
  if (qvp == NULL) return;

  MemSet ((Pointer) &fcontext, 0, sizeof (SeqMgrFeatContext));
  fcontext.itemID = 0;
  fcontext.featdeftype = sfp->idx.subtype;
  fcontext.seqfeattype = sfp->data.choice;
  slp = sfp->location;
  fcontext.left = GetOffsetInBioseq (slp, bsp, SEQLOC_LEFT_END);
  fcontext.right = GetOffsetInBioseq (slp, bsp, SEQLOC_RIGHT_END);
  fcontext.strand = SeqLocStrand (slp);

  sep = GetTopSeqEntryForEntityID (bbp->entityID);

  oldscope = SeqEntrySetScope (sep);

  if (ajp->format != FTABLE_FMT) {
    str = FormatFeatureBlockEx (ajp, asp, bsp, target, sfp, &fcontext, qvp,
                                ajp->format, (IntFeatBlockPtr) bbp, ISA_aa (bsp->mol), TRUE);
  }

  SeqEntrySetScope (oldscope);

  if (str != NULL) {
    if (afp->fp != NULL) {
      fprintf (afp->fp, "%s", str);
    }
    if (afp->ffwrite != NULL) {
      afp->ffwrite (str, afp->userdata, blocktype);
    }
  } else {
    if (afp->fp != NULL) {
      fprintf (afp->fp, "?\n");
    }
    if (afp->ffwrite != NULL) {
      afp->ffwrite ("?\n", afp->userdata, blocktype);
    }
  }

  MemFree (str);
  MemFree (qvp
  );
}

NLM_EXTERN CharPtr FormatFeatureQuals (
  SeqFeatPtr sfp
)

{
  IntAsn2gbJob       ajb;
  IntAsn2gbJobPtr    ajp;
  BioseqPtr          bsp;
  SeqMgrFeatContext  fcontext;
  IntCdsBlock        ifb;
  IntFeatBlockPtr    ifp;
  size_t             max;
  QualValPtr         qvp;
  CharPtr            str;

  if (sfp == NULL) return NULL;
  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp == NULL) return NULL;

  if (SeqMgrGetDesiredFeature (0, bsp, 0, 0, sfp, &fcontext) != sfp) return NULL;

  MemSet ((Pointer) &ajb, 0, sizeof (IntAsn2gbJob));
  ajp = &ajb;
  MemSet ((Pointer) &ifb, 0, sizeof (IntCdsBlock));
  ifp = (IntFeatBlockPtr) &ifb;

  max = (size_t) (MAX (ASN2GNBK_TOTAL_SOURCE, ASN2GNBK_TOTAL_FEATUR));
  qvp = MemNew (sizeof (QualVal) * (max + 5));
  if (qvp == NULL) return NULL;

  str = FormatFeatureBlockEx (ajp, NULL, NULL, NULL, sfp, &fcontext, qvp,
                              GENBANK_FMT, ifp, FALSE, FALSE);

  MemFree (qvp);
  StripLeadingSpaces (str);
  return str;
}

