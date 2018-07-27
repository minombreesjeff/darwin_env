/*   sequin6.c
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
* File Name:  sequin6.c
*
* Author:  Jonathan Kans
*
* Version Creation Date:   11/12/97
*
* $Revision: 6.208 $
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

#include "sequin.h"
#include <ncbilang.h>
#include <gather.h>
#include <asn2gnbp.h>
#include <bspview.h>
#include <import.h>
#include <objsub.h>
#include <explore.h>
#include <subutil.h>
#include <gbftdef.h>
#include <edutil.h>

#define NUMBER_OF_SUFFIXES    7

static ENUM_ALIST(name_suffix_alist)
  {" ",    0},
  {"Jr.",  1},
  {"Sr.",  2},
  {"II",   4},
  {"III",  5},
  {"IV",   6},
  {"V",    7},
  {"VI",   8},
END_ENUM_ALIST

#define GENE_TYPE             1
#define CDS_TYPE              2
#define PROT_TYPE             3
#define RNA_TYPE              4
#define BIOSOURCE_TYPE        5
#define ORGMOD_SUBSOURCE_TYPE 6
#define IMPORT_FEAT_TYPE      7
#define DEFLINE_TYPE          8
#define FEATURE_NOTE_TYPE     9
#define PUBLICATION_TYPE      10

#define NUMBER_OF_TYPES               7
#define NUMBER_OF_TYPES_WITH_DEFLINE  8

static ENUM_ALIST(target_field_alist)
  {" ",                    0},
  {"Gene",                 GENE_TYPE},
  {"CDS",                  CDS_TYPE},
  {"Prot",                 PROT_TYPE},
  {"RNA",                  RNA_TYPE},
  {"BioSource",            BIOSOURCE_TYPE},
  {"OrgMod and SubSource", ORGMOD_SUBSOURCE_TYPE},
  {"Import Feature",       IMPORT_FEAT_TYPE},
  {"DefLine",              DEFLINE_TYPE},
END_ENUM_ALIST

#define NUMBER_OF_SEGREGATE_TYPES 10
#define NUMBER_OF_PARSE_TYPES 9

static ENUM_ALIST(segregate_target_field_alist)
  {" ",                    0},
  {"Gene",                 GENE_TYPE},
  {"CDS",                  CDS_TYPE},
  {"Prot",                 PROT_TYPE},
  {"RNA",                  RNA_TYPE},
  {"BioSource",            BIOSOURCE_TYPE},
  {"OrgMod and SubSource", ORGMOD_SUBSOURCE_TYPE},
  {"Import Feature",       IMPORT_FEAT_TYPE},
  {"DefLine",              DEFLINE_TYPE},
  {"Feature Note",         FEATURE_NOTE_TYPE},
  {"Publication",          PUBLICATION_TYPE},
END_ENUM_ALIST

#define NUMBER_OF_PARSE_TYPES 9

static ENUM_ALIST(parse_target_field_alist)
  {" ",                    0},
  {"Gene",                 GENE_TYPE},
  {"CDS",                  CDS_TYPE},
  {"Prot",                 PROT_TYPE},
  {"RNA",                  RNA_TYPE},
  {"BioSource",            BIOSOURCE_TYPE},
  {"OrgMod and SubSource", ORGMOD_SUBSOURCE_TYPE},
  {"Import Feature",       IMPORT_FEAT_TYPE},
  {"DefLine",              DEFLINE_TYPE},
  {"Feature Note",         FEATURE_NOTE_TYPE},
END_ENUM_ALIST

#define PUBLICATION_PUBLISHED_FIELD 1
#define PUBLICATION_INPRESS_FIELD   2
#define PUBLICATION_UNPUB_FIELD     3

static ENUM_ALIST (publication_field_alist)
  {" ",                0},
  {"Published",        PUBLICATION_PUBLISHED_FIELD},
  {"In Press",         PUBLICATION_INPRESS_FIELD},
  {"Unpublished",      PUBLICATION_UNPUB_FIELD},
END_ENUM_ALIST

#define EXT_NUMBER_OF_TYPES       7

#define GENE_LOCUS_FIELD        1
#define GENE_DESCRIPTION_FIELD  2
#define GENE_ALLELE_FIELD       3
#define GENE_MAPLOC_FIELD       4
#define GENE_LOCUS_TAG_FIELD    5
#define GENE_SYNONYM_FIELD      6
#define GENE_COMMENT_FIELD      7

static ENUM_ALIST(gene_field_alist)
  {" ",                    0},
  {"Gene locus",           GENE_LOCUS_FIELD},
  {"Gene description",     GENE_DESCRIPTION_FIELD},
  {"Gene allele",          GENE_ALLELE_FIELD},
  {"Gene maploc",          GENE_MAPLOC_FIELD},
  {"Locus_tag",            GENE_LOCUS_TAG_FIELD},
  {"Gene synonym",         GENE_SYNONYM_FIELD},
  {"Gene comment",         GENE_COMMENT_FIELD},
END_ENUM_ALIST

#define CDS_COMMENT   1
#define CDS_GENE_XREF 2
#define CDS_DB_XREF   3

static ENUM_ALIST(cds_field_alist)
  {" ",                    0},
  {"CDS comment",          CDS_COMMENT},
  {"gene xref",            CDS_GENE_XREF},
  {"db_xref",              CDS_DB_XREF},
END_ENUM_ALIST

static ENUM_ALIST(cds_short_field_alist)
  {" ",                    0},
  {"CDS comment",          CDS_COMMENT},
END_ENUM_ALIST

#define PROT_NAME_FIELD        1
#define PROT_DESCRIPTION_FIELD 2
#define PROT_ECNUM_FIELD       3
#define PROT_ACTIVITY_FIELD    4
#define PROT_COMMENT_FIELD     5

static ENUM_ALIST(prot_field_alist)
  {" ",                    0},
  {"Protein name",         PROT_NAME_FIELD},
  {"Protein description",  PROT_DESCRIPTION_FIELD},
  {"Protein E.C. number",  PROT_ECNUM_FIELD},
  {"Protein activity",     PROT_ACTIVITY_FIELD},
  {"Protein comment",      PROT_COMMENT_FIELD},
END_ENUM_ALIST

#define RNA_NAME_FIELD       1
#define RNA_COMMENT_FIELD    2
#define RNA_GENE_XREF_FIELD  3

static ENUM_ALIST(rnax_field_alist)
  {" ",                    0},
  {"RNA Name",             RNA_NAME_FIELD},
  {"RNA Comment",          RNA_COMMENT_FIELD},
  {"gene xref",            RNA_GENE_XREF_FIELD},
END_ENUM_ALIST

static ENUM_ALIST(rnax_short_field_alist)
  {" ",                    0},
  {"RNA Name",             RNA_NAME_FIELD},
  {"RNA Comment",          RNA_COMMENT_FIELD},
  {"gene xref",            RNA_GENE_XREF_FIELD},
END_ENUM_ALIST

#define ORGREF_SCI_NAME_FIELD    1
#define ORGREF_COMMON_NAME_FIELD 2
#define ORGREF_LINEAGE_FIELD     3
#define ORGREF_DIVISION_FIELD    4

static ENUM_ALIST(orgref_field_alist)
  {" ",                    0},
  {"Scientific Name",      ORGREF_SCI_NAME_FIELD},
  {"Common Name",          ORGREF_COMMON_NAME_FIELD},
  {"Lineage",              ORGREF_LINEAGE_FIELD},
  {"Division",             ORGREF_DIVISION_FIELD},
END_ENUM_ALIST

static ENUM_ALIST(subsource_and_orgmod_note_subtype_alist)
  {" ",                         0},
  {"Acronym",                  19},
  {"Anamorph",                 29},
  {"Authority",                24},
  {"Biotype",                  14},
  {"Biovar",                   13},
  {"Breed",                    31},
  {"Cell-line",              1008},
  {"Cell-type",              1009},
  {"Chemovar",                 12},
  {"Chromosome",             1001},
  {"Clone",                  1003},
  {"Clone-lib",              1011},
  {"Collected-by",           1031},
  {"Collection-date",        1030},
  {"Common",                   18},
  {"Country",                1023},
  {"Cultivar",                 10},
  {"Dev-stage",              1012},
  {"Dosage",                   20},
  {"Ecotype",                  27},
  {"Endogenous-virus-name",  1025},
  {"Environmental-sample",   1027},
  {"Forma",                    25},
  {"Forma-specialis",          26},
  {"Frequency",              1013},
  {"Fwd-primer-name",        1035},
  {"Fwd-primer-seq",         1033},
  {"Genotype",               1006},
  {"Germline",               1014},
  {"Group",                    15},
  {"Haplotype",              1005},
  {"Identified_by",          1032},
  {"Ins-seq-name",           1021},
  {"Isolate",                  17},
  {"Isolation-source",       1028},
  {"Lab-host",               1016},
  {"Lat-lon",                1029},
  {"Map",                    1002},
  {"Note -- OrgMod",          255},
  {"Note -- SubSource",      1255},
  {"Old Lineage",             253},
  {"Old Name",                254},
  {"Pathovar",                 11},
  {"Plasmid-name",           1019},
  {"Plastid-name",           1022},
  {"Pop-variant",            1017},
  {"Rearranged",             1015},
  {"Rev-primer-name",        1036},
  {"Rev-primer-seq",         1034},
  {"Segment",                1024},
  {"Serogroup",                 8},
  {"Serotype",                  7},
  {"Serovar",                   9},
  {"Sex",                    1007},
  {"Specific-host",            21},
  {"Specimen-voucher",         23},
  {"Strain",                    2},
  {"Subclone",               1004},
  {"Subgroup",                 16},
  {"Sub-species",              22},
  {"Substrain",                 3},
  {"Subtype",                   5},
  {"Synonym",                  28},
  {"Teleomorph",               30},
  {"Tissue-lib",             1018},
  {"Tissue-type",            1010},
  {"Transgenic",             1026},
  {"Transposon-name",        1020},
  {"Type",                      4},
  {"Variety",                   6},
END_ENUM_ALIST

#define IMPORT_GBQUAL_FIELD  1
#define IMPORT_COMMENT_FIELD 2

static ENUM_ALIST(impfeat_field_alist)
  {" ",        0},
  {"GBQual",   IMPORT_GBQUAL_FIELD},
  {"Comment",  IMPORT_COMMENT_FIELD},
END_ENUM_ALIST

static ENUM_ALIST (orgmod_subtype_and_note_alist)
  {" ",                 0},
  {"Acronym",          19},
  {"Anamorph",         29},
  {"Authority",        24},
  {"Biotype",          14},
  {"Biovar",           13},
  {"Breed",            31},
  {"Chemovar",         12},
  {"Common",           18},
  {"Dosage",           20},
  {"Cultivar",         10},
  {"Ecotype",          27},
  {"Forma",            25},
  {"Forma-specialis",  26},
  {"Group",            15},
  {"Isolate",          17},
  {"Note",            255},
  {"Old Lineage",     253},
  {"Old Name",        254},
  {"Pathovar",         11},
  {"Serogroup",         8},
  {"Serotype",          7},
  {"Serovar",           9},
  {"Specific-host",    21},
  {"Specimen-voucher", 23},
  {"Strain",            2},
  {"Subgroup",         16},
  {"Sub-species",      22},
  {"Substrain",         3},
  {"Subtype",           5},
  {"Synonym",          28},
  {"Teleomorph",       30},
  {"Type",              4},
  {"Variety",           6},
END_ENUM_ALIST

static ENUM_ALIST (subsource_subtype_and_note_alist)
  {" ",                      0},
  {"Cell-line",              8},
  {"Cell-type",              9},
  {"Chromosome",             1},
  {"Clone",                  3},
  {"Clone-lib",             11},
  {"Collected-by",          31},
  {"Collection-date",       30},
  {"Country",               23},
  {"Dev-stage",             12},
  {"Endogenous-virus-name", 25},
  {"Environmental-sample",  27},
  {"Frequency",             13},
  {"Fwd-primer-name",       35},
  {"Fwd-primer-seq",        33},
  {"Genotype",               6},
  {"Germline",              14},
  {"Haplotype",              5},
  {"Identified_by",         32},
    /*
  {"Ins-seq-name",          21},
    */
  {"Isolation-source",      28},
  {"Lab-host",              16},
  {"Lat-lon",               29},
  {"Map",                    2},
  {"Note",                 255},
  {"Plasmid-name",          19},
  {"Plastid-name",          22},
  {"Pop-variant",           17},
  {"Rearranged",            15},
  {"Rev-primer-name",       36},
  {"Rev-primer-seq",        34},
  {"Segment",               24},
  {"Sex",                    7},
  {"Subclone",               4},
  {"Tissue-lib",            18},
  {"Tissue-type",           10},
  {"Transgenic",            26},
    /*
  {"Transposon-name",       20},
    */
END_ENUM_ALIST


#define NUM_SUBTARGET_POPUPS 11

static GbFeatName ParseQualifierList[] = {
 {"allele", Class_text}, {"anticodon", Class_pos_aa},
 {"bound_moiety", Class_text},
 {"chromosome", Class_text},
 {"citation", Class_bracket_int},
 {"codon", Class_seq_aa},
 {"codon_start", Class_int_or}, {"cons_splice", Class_site},
 {"db_xref", Class_text},
 {"direction", Class_L_R_B}, {"EC_number", Class_ecnum},
 {"evidence", Class_exper}, {"exception", Class_text},
 {"frequency", Class_text}, {"function", Class_text},
 {"gene", Class_text}, {"gdb_xref", Class_text},
 {"insertion_seq", Class_text},
 {"label", Class_token},
 {"map", Class_text},
 {"mod_base", Class_token}, {"note", Class_note},
 {"number", Class_number}, {"organism", Class_text},
 {"partial", Class_none}, {"PCR_conditions", Class_text},
 {"phenotype", Class_text},
 {"plasmid", Class_text}, {"product", Class_text},
 {"pseudo", Class_none},
 {"rearranged", Class_none}, { "replace", Class_text},
 {"rpt_family", Class_text}, {"rpt_type", Class_rpt},
 {"rpt_unit", Class_token},
 {"sequenced_mol", Class_text},
 {"standard_name", Class_text},
 {"translation", Class_text}, {"transl_except", Class_pos_aa},
 {"transl_table", Class_int}, {"transposon", Class_text},
 {"usedin", Class_token},
 {"focus", Class_none},
 {"protein_id", Class_text},
 {"organelle", Class_text}, {"transcript_id", Class_text},
 {"transgenic", Class_none}, {"environmental_sample", Class_none},
 {"locus_tag", Class_text}, {"mol_type", Class_text},
};

const Int4 NumParseQualifiers = sizeof (ParseQualifierList) / sizeof (GbFeatName);


static void PopulateParseQualPopup (PopuP p)
{
  Int4 i;
  for (i = 0; i < NumParseQualifiers; i++) {
    PopupItem (p, ParseQualifierList[i].name);
  }
}

typedef struct convertformdata {
  FEATURE_FORM_BLOCK

  GrouP              asnOrFlat;
  TexT               atleft;
  TexT               atright;
  TexT               deleteText;
  CharPtr            deleteStr;
  GrouP              deleteLevel;
  Int2               deleteLevelInt;
  PopuP              target;
  EnumFieldAssocPtr  target_alist;
  PopuP              subtarget [NUM_SUBTARGET_POPUPS];
  EnumFieldAssocPtr  alists [NUM_SUBTARGET_POPUPS];
  EnumFieldAssocPtr  impfeat_alist;
  GrouP              impfeatGroup;
  LisT               impfeat_select;
  ValNodePtr         feature_list;
  Int2               impfeat_type;
  ButtoN             accept;
  ButtoN             leaveDlgUp;
  Int2               report;
  Int2               type;
  Int2               subtype;
  CharPtr            leftstr;
  CharPtr            rightstr;
  CharPtr            foundstr;
  Boolean            endQuotesFlag;
  GrouP              leftbehav;
  GrouP              rightbehav;
  Boolean            includeleft;
  Boolean            includeright;
  Boolean            replaceOldAsked;
  Boolean            doReplaceAll;
  Boolean            use_semicolon;
  Boolean            isDirty;
  ButtoN             removeBefore;
  ButtoN             removeAfter;
  GrouP              includeBeforeGroup;
  GrouP              includeAfterGroup;
  GrouP              ifNotFoundGroup;
  Int2               ifNotFound;
  Int2               index;
  Int4               parsetype;
  BioseqSetPtr       target_set;
  PupActnProc        set_accept_proc;
  PopuP              gb_qual_pup;
  Int2               gb_qual_type;
} ConvertFormData, PNTR ConvertFormPtr;

static ConvertFormPtr ConvertFormNew (void)
{
  ConvertFormPtr cfp;

  cfp = (ConvertFormPtr) MemNew (sizeof (ConvertFormData));
  if (cfp == NULL) return NULL;
  MemSet (cfp, 0, sizeof (ConvertFormData));
  return cfp;
}

/* values for parse type */
#define PARSETYPE_FILE    0
#define PARSETYPE_COMMENT 1
#define PARSETYPE_LOCALID 2

/* Values for ifNotFound field */

#define DO_NOTHING       2
#define REMOVE_ALL_TEXT  3

/* End values for string trimming */

#define TRIM_LEFT_END    1
#define TRIM_RIGHT_END   2

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* TrimOffEndQuotes () -- Trim double-quotes off the ends of a string.     */
/*                                                                         */
/*-------------------------------------------------------------------------*/

static Boolean TrimOffEndQuotes (CharPtr trimString, 
				 Int2    whichEnd)
{
  Int4 strLen;
  Int4 i;

  strLen = StringLen(trimString);
  if (strLen == 0)
    return FALSE;

  /* If there is a quote at the end, remove it */

  if (TRIM_RIGHT_END == whichEnd) {
    if (trimString[strLen-1] == '"') {
      strLen--;
      trimString [strLen] = '\0';
      return TRUE;
    }
  }

  /* If there is a quote at the beginning, remove it */

  else {
    if (trimString[0] == '"') {
      for (i = 0; trimString[i] != '\0'; i++)
	trimString[i] = trimString [i+1];
      return TRUE;
    }
  }

  return FALSE;
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* SaveStringFromTextNoStripSpaces () -                                    */
/*                                                                         */
/*-------------------------------------------------------------------------*/

static CharPtr SaveStringFromTextNoStripSpaces (TexT t)

{
  size_t   len;
  CharPtr  str;

  len = TextLength (t);
  if (len > 0) {
    str = (CharPtr) MemNew(len + 1);
    if (str != NULL) {
      GetTitle (t, str, len + 1);
      return str;
    } else {
      return NULL;
    }
  } else {
    return NULL;
  }
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* SetConvertAcceptButton () -- Enable/Disable the Accept button depending */
/*                              on the condition of other window objects.  */
/*                                                                         */
/*-------------------------------------------------------------------------*/

static void SetConvertAcceptButton (Handle a)

{
  ConvertFormPtr  cfp;
  UIEnum          val;
  Int2            item_num;
  ValNodePtr      vnp;

  cfp = (ConvertFormPtr) GetObjectExtra (a);
  if (cfp == NULL) return;
  if (GetEnumPopup (cfp->target, cfp->target_alist, &val) && val > 0) {
    cfp->type = (Int2) val;
    if (val == FEATURE_NOTE_TYPE) {
      item_num = 1;
      for (vnp = cfp->feature_list, item_num = 1;
           vnp != NULL && ! GetItemStatus (cfp->subtarget [FEATURE_NOTE_TYPE], item_num);
           vnp = vnp->next, item_num++) 
      {}
      if (vnp == NULL) {
        SafeDisable (cfp->accept);
        return;
      }
    } else if ((DEFLINE_TYPE == cfp->type) || 
	(GetEnumPopup (cfp->subtarget [cfp->type],
		      cfp->alists [cfp->type], &val) &&	val > 0)) {
      if (GetValue (cfp->asnOrFlat) > 0) {
        if ((! TextHasNoText (cfp->atleft)) ||
	    (! TextHasNoText (cfp->atright))) {
          SafeEnable (cfp->accept);
          return;
        }
      }
      if (cfp->asnOrFlat == NULL)
      {
        if ((! TextHasNoText (cfp->atleft)) ||
          (! TextHasNoText (cfp->atright)) ||
          cfp->parsetype == PARSETYPE_COMMENT ||
          cfp->parsetype == PARSETYPE_LOCALID)
        {
          SafeEnable (cfp->accept);
          return;
        }
      }
    }
  }
  SafeDisable (cfp->accept);
}

/*-------------------------------------------------------------------------*/
/*                                                                         */
/* SetDeleteAcceptButton () -- Enable/Disable the Accept button depending  */
/*                             on the condition of other window objects.   */
/*                                                                         */
/*-------------------------------------------------------------------------*/

static void SetDeleteAcceptButton (Handle a)

{
  ConvertFormPtr  cfp;
  UIEnum          val;

  cfp = (ConvertFormPtr) GetObjectExtra (a);
  if (cfp == NULL)
    return;

  /* Disable if a target has not been selected */

  if (!GetEnumPopup (cfp->target, cfp->target_alist, &val)) {
    SafeDisable (cfp->accept);
    return;
  }

  if (val <= 0) {
    SafeDisable (cfp->accept);
    return;
  }

  /* Disable if a subtarget has not been selected and target is not defline */

  if (val != DEFLINE_TYPE) {
    cfp->type = (Int2) val;
  
    if (!GetEnumPopup (cfp->subtarget [cfp->type],
	  	      cfp->alists [cfp->type], &val)) {
      SafeDisable (cfp->accept);
      return;
    }

    if (val <= 0) {
      SafeDisable (cfp->accept);
      return;
    }
  }

  /* Disable if there is no delete string */

  if (TextHasNoText (cfp->deleteText)) {
    SafeDisable (cfp->accept);
    return;
  }

  /* If we made it to here, then we passed all */
  /* test and can enable the accept button.    */

  SafeEnable (cfp->accept);
}


static void ChangeSubtarget (PopuP p)

{
  ConvertFormPtr  cfp;
  Int2            i;
  UIEnum          val;

  cfp = (ConvertFormPtr) GetObjectExtra (p);
  if (cfp == NULL) return;
  if (GetEnumPopup (cfp->target, cfp->target_alist, &val) && val > 0) {
    for (i = 0; i < NUM_SUBTARGET_POPUPS; i++) {
      if (i != (Int2) val) {
        SafeHide (cfp->subtarget [i]);
      }
    }
    SafeShow (cfp->subtarget [(Int2) val]);
    if (val == IMPORT_FEAT_TYPE && cfp->gb_qual_pup != NULL) {
      SafeShow (cfp->gb_qual_pup);
    } else if (cfp->gb_qual_pup != NULL) {
      SafeHide (cfp->gb_qual_pup);
    }
  }
  if (cfp->set_accept_proc != NULL) {
    cfp->set_accept_proc ((Handle) p);
  }
}


/*=========================================================================*/
/*                                                                         */
/* CheckForString () -- Searches for a given string with another string.   */
/*                                                                         */
/*=========================================================================*/

static Boolean CheckForString (CharPtr searchStr,
			       CharPtr sourceStr)
{
  if (NULL == SearchForString (sourceStr, searchStr, FALSE, FALSE))
    return FALSE;
  else
    return TRUE;
}

/*=========================================================================*/
/*                                                                         */
/* GeneRefPtr () -- Search a GeneRef feature for a given string.           */
/*                                                                         */
/*=========================================================================*/

static Boolean SearchGeneRef (GeneRefPtr     grp,
			      SeqFeatPtr     sfp,
			      ConvertFormPtr cfp)
{
  ValNodePtr vnp;

  /* Check parameters */

  if ((NULL == grp) || (NULL == sfp))
    return FALSE;

  /* Check each text field for the given string */

  switch (cfp->subtype) {
  case 1 :
    return CheckForString (cfp->deleteStr, grp->locus);
    break;
  case 2 :
    return CheckForString (cfp->deleteStr, grp->desc);
    break;
  case 3 :
    return CheckForString (cfp->deleteStr, grp->allele);
    break;
  case 4 :
    return CheckForString (cfp->deleteStr, grp->maploc);
    break;
  case 5 :
    return CheckForString (cfp->deleteStr, grp->locus_tag);
    break;
  case 6 :
    for (vnp = grp->syn; vnp != NULL; vnp = vnp->next) {
      if (TRUE == CheckForString (cfp->deleteStr,
				  vnp->data.ptrvalue))
	return TRUE;
    }
    return FALSE;
    break;
  case 7 :
    return CheckForString (cfp->deleteStr, sfp->comment);
    break;
  default :
    break;
  }

  return FALSE;
}

/*=========================================================================*/
/*                                                                         */
/* SearchCDSFeat () -- Search a CDS feature for a given string.            */
/*                                                                         */
/*=========================================================================*/

static Boolean SearchCDSFeat (SeqFeatPtr     sfp,
			      ConvertFormPtr cfp)
{

  /* Check parameters */

  if (NULL == sfp)
    return FALSE;

  /* Check each text field for the given string */

  switch (cfp->subtype) {
  case 1 :
    return CheckForString (cfp->deleteStr, sfp->comment);
    break;
  case 2 :
  default :
    break;
  }

  /* If no match found, return FALSE */

  return FALSE;
}

/*=========================================================================*/
/*                                                                         */
/* SearchRnaRef () -- Search an RnaRef feature for a given string.         */
/*                                                                         */
/*=========================================================================*/

static Boolean SearchRnaRef (RnaRefPtr      rrp,
			     SeqFeatPtr     sfp,
			     ConvertFormPtr cfp)
{
  /* Check parameters */

  if ((NULL == rrp) || (NULL == sfp))
    return FALSE;

  /* Check each text field for the given string */

  switch (cfp->subtype) {
  case 1 :
    if ((0 == rrp->ext.choice) || (1 == rrp->ext.choice)) {
      return CheckForString (cfp->deleteStr, rrp->ext.value.ptrvalue);
    }
    break;
  case 2 :
    return CheckForString (cfp->deleteStr, sfp->comment);
    break;
  case 3 :
  default :
    break;
  }

  /* If no match found, return FALSE */

  return FALSE;
}

/*=========================================================================*/
/*                                                                         */
/* SearchProtRef () -- Search a ProtRef feature for a given string.        */
/*                                                                         */
/*=========================================================================*/

static Boolean SearchProtRef (ProtRefPtr     prp,
			      SeqFeatPtr     sfp,
			      ConvertFormPtr cfp)
{
  ValNodePtr vnp;

  /* Check parameters */

  if (NULL == prp)
    return FALSE;

  /* Check each text field for the given string */

  switch (cfp->subtype) {

    /* Search the name field */  

  case 1 :
    for (vnp = prp->name; vnp != NULL; vnp = vnp->next) {
      if (TRUE == CheckForString (cfp->deleteStr,
				  (CharPtr) vnp->data.ptrvalue))
	return TRUE;
    }
    break;

    /* Search the desc field */

  case 2 :
    return CheckForString (cfp->deleteStr, prp->desc);
    break;

    /* Search the ec field */

  case 3 :
    for (vnp = prp->ec; vnp != NULL; vnp = vnp->next) {
      if (TRUE == CheckForString (cfp->deleteStr, vnp->data.ptrvalue))
	return TRUE;
    }
    break;

    /* Search the activity field */

  case 4 :
    for (vnp = prp->activity; vnp != NULL; vnp = vnp->next) {
      if (TRUE == CheckForString (cfp->deleteStr, vnp->data.ptrvalue))
	return TRUE;
    }
    break;

    /* Search the comment field */

  case 5 :
    return CheckForString (cfp->deleteStr, sfp->comment);
    break;

    /* Default check */

  default :
    break;
  }

  /* If we made it this far no match was found */

  return FALSE;
}

/*=========================================================================*/
/*                                                                         */
/* SearchImpFeat () -- Search an ImpFeat feature for a given string.       */
/*                                                                         */
/*=========================================================================*/

static Boolean SearchImpFeat (SeqFeatPtr     sfp,
			      ConvertFormPtr cfp)
{
  GBQualPtr       gbqp;

  /* Check parameters */

  if (NULL == sfp)
    return FALSE;

  switch (cfp->subtype) {

    /* Search the GB Quals */

  case IMPORT_GBQUAL_FIELD :
    gbqp = sfp->qual;
    while (NULL != gbqp) {
      if (NULL != gbqp->val)
	if (TRUE == CheckForString (cfp->deleteStr, gbqp->val))
	  return TRUE;
      gbqp = gbqp->next;
    }
    return FALSE;
    break;

    /* Search the comment field */

  case IMPORT_COMMENT_FIELD :
    return CheckForString (cfp->deleteStr, sfp->comment);
    break;
  default :
    break;
  }

  return FALSE;
}

/*=========================================================================*/
/*                                                                         */
/* MarkObjectsByText_Callback () - Called for each object in a SeqEntry    */
/*                                 this will mark the item for deletion    */
/*                                 if it matches the given criteria.       */
/*                                                                         */
/*=========================================================================*/

static void DeleteFeaturesByText_Callback (SeqEntryPtr sep,
					   Pointer     mydata,
					   Int4        index,
					   Int2        indent)
{
  ConvertFormPtr cfp;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  SeqAnnotPtr    sap;
  SeqFeatPtr     sfp;
  GeneRefPtr     grp;
  ProtRefPtr     prp;
  RnaRefPtr      rrp;
  Boolean        found;

  /* Check parameters */

  if ((NULL == sep) || (NULL == sep->data.ptrvalue))
    return;

  cfp = (ConvertFormPtr) mydata;
  if (NULL == cfp)
    return;

  /* Get the list of annotations */

  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  }
  else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  }
  else
    return;

  /* Search the requested item for the given string */

  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_GENE && cfp->type == GENE_TYPE) {
          grp = (GeneRefPtr) sfp->data.value.ptrvalue;
	  found = SearchGeneRef (grp, sfp, cfp);
        }
	else if (sfp->data.choice == SEQFEAT_CDREGION &&
		 cfp->type == CDS_TYPE) {
	  found = SearchCDSFeat (sfp, cfp);
        }
	else if (sfp->data.choice == SEQFEAT_PROT &&
		 cfp->type == PROT_TYPE) {
          prp = (ProtRefPtr) sfp->data.value.ptrvalue;
	  found = SearchProtRef (prp, sfp, cfp);
        }
	else if (sfp->data.choice == SEQFEAT_RNA && cfp->type == RNA_TYPE) {
          rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
	  found = SearchRnaRef (rrp, sfp, cfp);
        }
	else if (sfp->data.choice == SEQFEAT_IMP &&
		 cfp->type == IMPORT_FEAT_TYPE) {
	  found = SearchImpFeat (sfp, cfp);
        }
	if (TRUE == found)
	  break;
	else
	  sfp = sfp->next;
      }
    }
    if (TRUE == found)
      break;
    else
      sap = sap->next;
  }

  /* If we found the string, do the deletion */

  if (TRUE == found) {
    cfp->isDirty = TRUE;
    switch (cfp->deleteLevelInt) {
    case 1 :
      sfp->idx.deleteme = TRUE;
      break;
    case 2 :
      if (IS_Bioseq (sep))
	bsp->idx.deleteme = TRUE;
      break;
    case 3 :
      if (IS_Bioseq_set (sep))
	bssp->idx.deleteme = TRUE;
      else {
	if (bsp->idx.parenttype == OBJ_BIOSEQSET) {
	  bssp = (BioseqSetPtr) bsp->idx.parentptr;
	  bssp->idx.deleteme = TRUE;
	}
      }
      break;
    default:
      break;
    }
  }

  /* Return successfully */

  return;
}

static Boolean DoesBioSourceContainText 
(BioSourcePtr biop,
 ConvertFormPtr cfp)
{
  OrgRefPtr     orp;
  Boolean       found;
  OrgNamePtr    onp;

  if (biop == NULL || cfp == NULL) return FALSE;

  found = FALSE;

  orp = biop->org;
  if (orp == NULL) return FALSE;
  switch (cfp->subtype) {
    case ORGREF_SCI_NAME_FIELD :
      found = CheckForString (cfp->deleteStr, orp->taxname);
      break;
    case ORGREF_COMMON_NAME_FIELD :
	  found = CheckForString (cfp->deleteStr, orp->common);
      break;
    case ORGREF_LINEAGE_FIELD :
      onp = orp->orgname;
      if (onp == NULL) {
        onp = OrgNameNew ();
        orp->orgname = onp;
      }
      if (onp != NULL)
	    found = CheckForString (cfp->deleteStr, onp->lineage);
	  else
	    found = FALSE;
      break;
    case ORGREF_DIVISION_FIELD :
      onp = orp->orgname;
      if (onp == NULL) {
        onp = OrgNameNew ();
        orp->orgname = onp;
      }
      if (onp != NULL)
        found = CheckForString (cfp->deleteStr, onp->div);
	  else
	    found = FALSE;
      break;
    default :
      break;
  }
  return found;
}

static Boolean DoSubSourcesContainText 
(BioSourcePtr biop,
 ConvertFormPtr cfp)
{
  OrgRefPtr     orp;
  OrgModPtr     mod;
  SubSourcePtr  ssp;
  OrgNamePtr    onp;
  Boolean found = FALSE;

  if (biop == NULL || cfp == NULL) return FALSE;
  if (cfp->subtype < 1000) {
	orp = biop->org;
	if (orp == NULL) {
	  orp = OrgRefNew ();
	  biop->org = orp;
	}
	if (orp != NULL) {
	  onp = orp->orgname;
	  if (onp == NULL) {
	    onp = OrgNameNew ();
	    orp->orgname = onp;
	  }
	  if (onp != NULL) {
	    mod = onp->mod;
	    while (mod != NULL && mod->subtype != cfp->subtype) {
	      mod = mod->next;
	    }
	    if (mod != NULL)
	      found = CheckForString (cfp->deleteStr, mod->subname);
	    else
	      found = FALSE;
	  }
	  else
	    found = FALSE;
	}
  } else {
	ssp = biop->subtype;
	while (ssp != NULL && ssp->subtype != (cfp->subtype - 1000)) {
	  ssp = ssp->next;
	}
	while (ssp != NULL) {
	  if (ssp->subtype == (cfp->subtype - 1000)) {
	    found = CheckForString (cfp->deleteStr, ssp->name);
	    if (found)
	      break;
	  }
	  ssp = ssp->next;
	}
	if (NULL == ssp)
	  found = FALSE;
  }
  return found;
}

/*=========================================================================*/
/*                                                                         */
/* DeleteSourceByText () - Given a text string, delete all items of a      */
/*                         given type that contain that string.            */
/*                                                                         */
/*=========================================================================*/

static void DeleteSourceByText (SeqDescrPtr    sdp,
				SeqEntryPtr    sep,
				BioseqPtr      bsp,
				ConvertFormPtr cfp)

{
  Boolean       found;
  BioseqSetPtr  bssp;
  BioSourcePtr  biop;

  /* Check parameters */

  if (sdp == NULL || cfp == NULL)
    return;

  if (Seq_descr_source != sdp->choice)
    return;

  biop = sdp->data.ptrvalue;
  if (NULL == biop)
    return;

  /* Search the source for the given string */

  switch (cfp->type) {
    case BIOSOURCE_TYPE :
      found = DoesBioSourceContainText (biop, cfp);
      break;
    case ORGMOD_SUBSOURCE_TYPE :
      found = DoSubSourcesContainText (biop, cfp);
    default:
      break;
  }

  /* If we found anything to delete then */
  /* delete it and set the dirty flag.   */

  if (TRUE == found) {
    cfp->isDirty = TRUE;
    switch (cfp->deleteLevelInt) {
    case 1 :
      break;
    case 2 :
      if (IS_Bioseq (sep))
	bsp->idx.deleteme = TRUE;
      break;
    case 3 :
      if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
	bssp->idx.deleteme = TRUE;
      } else {
	if (bsp->idx.parenttype == OBJ_BIOSEQSET) {
	  bssp = (BioseqSetPtr) bsp->idx.parentptr;
	  bssp->idx.deleteme = TRUE;
	}
      }
      break;
    default:
      break;
    }
  }

  /* Return successfully */

  return;

}

/*=========================================================================*/
/*                                                                         */
/* DeleteItemsByText () - Given a text string, delete all items of a given */
/*                        type that contain that string.                   */
/*                                                                         */
/*=========================================================================*/

static void DeleteItemsByText (SeqEntryPtr    sep,
			       ConvertFormPtr cfp)
{
  BioseqSetPtr      bssp;
  BioseqPtr         bsp;
  SeqMgrDescContext descContext;
  /*
  Uint2             parenttype;
  Pointer           parentptr;
  SeqEntryPtr       parentSep;
  */
  SeqDescrPtr       sdp, prev_sdp, next_sdp;

  /* If we have a Bioseq Set, then recurse until */
  /* we get down to an actual Bioseq.            */

  if (IS_Bioseq_set (sep)) {

    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (NULL == bssp)
      return;

    for (sep = bssp->seq_set; sep != NULL; sep = sep->next)
      DeleteItemsByText (sep, cfp);

    return;
  }

  /* If we made it this far, then we have a Bioseq */

  bsp = (BioseqPtr) sep->data.ptrvalue;

  switch (cfp->type) {
    case GENE_TYPE :
    case CDS_TYPE :
    case PROT_TYPE :
    case RNA_TYPE :
    case IMPORT_FEAT_TYPE :
      SeqEntryExplore (sep, (Pointer) cfp, DeleteFeaturesByText_Callback);
      break;
    case BIOSOURCE_TYPE :
    case ORGMOD_SUBSOURCE_TYPE :
      sdp = SeqMgrGetNextDescriptor (bsp, NULL, 0, &descContext);
      while (NULL != sdp) {
	    sdp = SeqMgrGetNextDescriptor (bsp, sdp, 0, &descContext);
	    DeleteSourceByText (sdp, sep, bsp, cfp);
      }
      /*
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      if (NULL == biop) {
	GetSeqEntryParent (sep, &parentptr, &parenttype);
	SeqEntryToBioSource (parentSep, NULL, NULL, 0, &biop);
      }
      DeleteSourceByText (sep, bsp, biop, cfp);
      */
      break;
    case DEFLINE_TYPE :
      prev_sdp = NULL;
      sdp = bsp->descr;
      while (sdp != NULL) {
        next_sdp = sdp->next;
        if (sdp->choice == Seq_descr_title
            && sdp->data.ptrvalue != NULL 
            && CheckForString (cfp->deleteStr, sdp->data.ptrvalue)) {
          if (prev_sdp == NULL) {
            bsp->descr = sdp->next;
          } else {
              prev_sdp->next = sdp->next;
          }
          sdp->next = NULL;
          SeqDescrFree (sdp);
        } else {
          prev_sdp = sdp;
        }
        sdp = next_sdp;
      }
    default:
      break;
  }

  /* Return succesfully */

  return;
}

/*=========================================================================*/
/*                                                                         */
/* DeleteByText_Callback () - Finds and deletes all items of a selected    */
/*                            type that contain a given text phrase.       */
/*                                                                         */
/*=========================================================================*/

static void DeleteByText_Callback (ButtoN b)
{
  ConvertFormPtr  cfp;
  SeqEntryPtr     sep;
  UIEnum          val;

  /* Check the initial conditions and get the sequence */

  cfp = (ConvertFormPtr) GetObjectExtra (b);
  if (cfp == NULL || cfp->input_entityID == 0) {
    Remove (cfp->form);
    return;
  }

  sep = GetTopSeqEntryForEntityID (cfp->input_entityID);
  if (sep == NULL) {
    Remove (cfp->form);
    return;
  }

  /* Get the string to search for */

  cfp->deleteStr = SaveStringFromTextNoStripSpaces (cfp->deleteText);
  if (StringHasNoText (cfp->deleteStr)){ 
    Remove (cfp->form);
    return;
  }

  /* Get the type of items to search */

  GetEnumPopup (cfp->target, cfp->target_alist, &val);
  if (0 == val) {
    Remove (cfp->form);
    return;
  }
  else
    cfp->type = (Int2) val;

  if (cfp->type != DEFLINE_TYPE) {
    GetEnumPopup (cfp->subtarget [cfp->type], cfp->alists [cfp->type], &val);
    if (0 == val) {
      Remove (cfp->form);
      return;
    } else {
      cfp->subtype = (Int2) val;
    }
  }

  /* Get the delete level */

  cfp->deleteLevelInt = GetValue (cfp->deleteLevel);

  /* Display the 'working' cursor */

  WatchCursor ();
  Update ();

  /* Do the search and mark and found objects for deletion */

  cfp->isDirty = FALSE;
  DeleteItemsByText (sep, cfp);

  /* Remove the window and update things */

  if (cfp->isDirty) {
    DeleteMarkedObjects (cfp->input_entityID, 0, NULL);
    ObjMgrSetDirtyFlag (cfp->input_entityID, TRUE);
    ObjMgrSendMsg (OM_MSG_UPDATE, cfp->input_entityID, 0, 0);
  }

  ArrowCursor ();
  Update ();
  Remove (cfp->form);

  /* Return successfully */

  return;
}

/*=========================================================================*/
/*                                                                         */
/* CreateDeleteByTextWindow () - Creates and then display the window for   */
/*                               getting delete by text info from the user.*/
/*                                                                         */
/*=========================================================================*/

extern Int2 LIBCALLBACK CreateDeleteByTextWindow (Pointer data)
{
  EnumFieldAssocPtr  ap;
  GrouP              c;
  ConvertFormPtr     cfp;
  GrouP              g;
  GrouP              h;
  GrouP              k;
  Int2               j;
  OMProcControlPtr   ompcp;
  GrouP              p;
  StdEditorProcsPtr  sepp;
  WindoW             w;
  GrouP              x;

  /* Check parameters and get a pointer to the current data */

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL)
    return OM_MSG_RET_ERROR;

  /* Create a new window, and a struct */
  /* to pass around the data in.       */

  cfp = ConvertFormNew ();
  if (cfp == NULL)
    return OM_MSG_RET_ERROR;
  cfp->parsetype = PARSETYPE_FILE;
  cfp->target_alist = target_field_alist;
  cfp->set_accept_proc = (PupActnProc) SetDeleteAcceptButton;

  w = FixedWindow (-50, -33, -10, -10, "Delete By Text String",
		   StdCloseWindowProc);
  SetObjectExtra (w, cfp, StdCleanupFormProc);
  cfp->form = (ForM) w;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    cfp->appmessage = sepp->handleMessages;
  }

  cfp->input_entityID = ompcp->input_entityID;
  cfp->input_itemID = ompcp->input_itemID;
  cfp->input_itemtype = ompcp->input_itemtype;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    cfp->appmessage = sepp->handleMessages;
  }

  /* Add the popup lists */

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  k = HiddenGroup (h, 3, 0, NULL);

  StaticPrompt (k, "Delete at the level of", 0, dialogTextHeight,
		programFont, 'l');
  cfp->deleteLevel = HiddenGroup (k, 2, 0, NULL);
  RadioButton (cfp->deleteLevel, "Feature");
  RadioButton (cfp->deleteLevel, "Bioseq");
  RadioButton (cfp->deleteLevel, "Bioseq Set");
  SetValue (cfp->deleteLevel, 1);

  g = HiddenGroup (h, 3, 0, NULL);

  StaticPrompt (g, "Delete objects with the string", 0, dialogTextHeight,
		programFont, 'l');
  cfp->deleteText = DialogText (g, "", 10, (TxtActnProc) cfp->set_accept_proc);
  SetObjectExtra (cfp->deleteText, cfp, NULL);

  p = HiddenGroup (h, 6, 0, NULL);
  StaticPrompt (p, "Find string in", 0, popupMenuHeight, programFont, 'l');
  cfp->target = PopupList (p, TRUE, ChangeSubtarget);
  SetObjectExtra (cfp->target, cfp, NULL);
  InitEnumPopup (cfp->target, cfp->target_alist, NULL);
  SetEnumPopup (cfp->target, cfp->target_alist, 0);

  cfp->alists [GENE_TYPE]             = gene_field_alist;
  cfp->alists [CDS_TYPE]              = cds_field_alist;
  cfp->alists [PROT_TYPE]             = prot_field_alist;
  cfp->alists [RNA_TYPE]              = rnax_field_alist;
  cfp->alists [BIOSOURCE_TYPE]        = orgref_field_alist;
  cfp->alists [ORGMOD_SUBSOURCE_TYPE] = subsource_and_orgmod_note_subtype_alist;
  cfp->alists [IMPORT_FEAT_TYPE]      = impfeat_field_alist;

  x = HiddenGroup (p, 0, 0, NULL);

  for (j = 1; j <= NUMBER_OF_TYPES; j++) {
    if (j == ORGMOD_SUBSOURCE_TYPE) {
      cfp->subtarget [j] =
	(PopuP) SingleList (x, 10, 8, (LstActnProc) cfp->set_accept_proc);
      SetObjectExtra (cfp->subtarget [j], cfp, NULL);
      for (ap = cfp->alists [j]; ap->name != NULL; ap++)
	ListItem ((LisT) cfp->subtarget [j], ap->name);
      Hide (cfp->subtarget [j]);
    }
    else {
      cfp->subtarget [j] = PopupList (x, TRUE, cfp->set_accept_proc);
      SetObjectExtra (cfp->subtarget [j], cfp, NULL);
      InitEnumPopup (cfp->subtarget [j], cfp->alists [j], NULL);
      SetEnumPopup (cfp->subtarget [j], cfp->alists [j], 0);
      Hide (cfp->subtarget [j]);
    }
  }

  /* Add Accept and Cancel buttons */

  c = HiddenGroup (h, 4, 0, NULL);
  cfp->accept = DefaultButton (c, "Accept", DeleteByText_Callback);
  SetObjectExtra (cfp->accept, cfp, NULL);
  Disable (cfp->accept);
  PushButton (c, "Cancel", StdCancelButtonProc);

  /* Line things up nicely */

  AlignObjects (ALIGN_LEFT, (HANDLE) p, (HANDLE) c, (HANDLE) h,
		(HANDLE) k, NULL);

  /* Display the window now */

  RealizeWindow (w);
  Show (w);
  Select (w);
  Select (cfp->accept);
  Update ();
  return OM_MSG_RET_OK;
}

typedef Boolean (LIBCALLBACK *wantSegregateNucProtSetFunction) 
( BioseqSetPtr bssp,
  Pointer      userdata);

typedef Boolean (LIBCALLBACK *wantSegregateSequenceFunction) 
( BioseqPtr bsp,
  Pointer   userdata);
  
/*=========================================================================*/
/*                                                                         */
/* SegregateItemsRecursor () - Given a functions for determining which bioseqs     */
/*                     meet conditions, segregate bioseqs into separate    */
/*                     sets.                                               */
/*                                                                         */
/*=========================================================================*/

static void SegregateItemsRecursor 
(SeqEntryPtr                     seqlist,
 BioseqSetPtr                    set1,
 BioseqSetPtr                    set2,
 wantSegregateNucProtSetFunction do_np,
 wantSegregateSequenceFunction   do_seq,
 Pointer                         userdata
 )
{
  
  BioseqPtr         bsp;
  BioseqSetPtr      this_bssp;
  SeqEntryPtr       this_list;
  SeqEntryPtr       sep, next_sep;
  SeqEntryPtr       set1last, set2last;


  if (set1 == NULL || set2 == NULL || seqlist == NULL)
    return;

  set1last = set1->seq_set;
  while (set1last != NULL && set1last->next != NULL) {
    set1last = set1last->next;
  }
  set2last = set2->seq_set;
  while (set2last != NULL && set2last->next != NULL) {
    set2last = set2last->next;
  }

  sep = seqlist;
  while (sep != NULL) {
    next_sep = sep->next;
    if (IS_Bioseq_set (sep)) {
      this_bssp = (BioseqSetPtr) sep->data.ptrvalue;
      if (this_bssp->_class == BioseqseqSet_class_nuc_prot) {
        if (do_np != NULL && do_np (this_bssp, userdata)) {
          if (set2last == NULL) {
            set2->seq_set = sep;
          } else {
            set2last->next = sep;
          }
          set2last = sep;
        } else {
          if (set1last == NULL) {
            set1->seq_set = sep;
          } else {
            set1last->next = sep;
          }
          set1last = sep;
        }
        sep->next = NULL;
      } else {
        this_list = this_bssp->seq_set;
        this_bssp->seq_set = NULL;
        SegregateItemsRecursor (this_list, set1, set2, do_np, do_seq, userdata);
      }
    } else if (IS_Bioseq (sep)) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (do_seq != NULL && do_seq (bsp, userdata)) {
        if (set2last == NULL) {
          set2->seq_set = sep;
        } else {
          set2last->next = sep;
        }
        set2last = sep;
      } else {
        if (set1last == NULL) {
          set1->seq_set = sep;
        } else {
          set1last->next = sep;
        }
        set1last = sep;
      }
      sep->next = NULL;
    }
    sep = next_sep;
  }
}


static Boolean DoFeaturesInAnnotContainText (SeqAnnotPtr sap, ConvertFormPtr cfp)
{
  SeqFeatPtr     sfp;
  GeneRefPtr     grp;
  ProtRefPtr     prp;
  RnaRefPtr      rrp;
  Boolean        found = FALSE;
  ValNodePtr     vnp;
  Int2           item_num;
  
  if (sap == NULL || cfp == NULL) return FALSE;
  
  /* Search the requested item for the given string */

  while (sap != NULL && !found) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL && !found) {
        if (sfp->data.choice == SEQFEAT_GENE && cfp->type == GENE_TYPE) {
          grp = (GeneRefPtr) sfp->data.value.ptrvalue;
	      found = SearchGeneRef (grp, sfp, cfp);
        } else if (sfp->data.choice == SEQFEAT_CDREGION && cfp->type == CDS_TYPE) {
	      found = SearchCDSFeat (sfp, cfp);
        } else if (sfp->data.choice == SEQFEAT_PROT && cfp->type == PROT_TYPE) {
          prp = (ProtRefPtr) sfp->data.value.ptrvalue;
	      found = SearchProtRef (prp, sfp, cfp);
        } else if (sfp->data.choice == SEQFEAT_RNA && cfp->type == RNA_TYPE) {
          rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
	      found = SearchRnaRef (rrp, sfp, cfp);
        } else if (sfp->data.choice == SEQFEAT_IMP &&
		  cfp->type == IMPORT_FEAT_TYPE) {
	      found = SearchImpFeat (sfp, cfp);
        } else if (cfp->type == FEATURE_NOTE_TYPE) {
          for (vnp = cfp->feature_list, item_num = 1;
               vnp != NULL && ! found;
               vnp = vnp->next, item_num++) {
            if (GetItemStatus (cfp->subtarget [FEATURE_NOTE_TYPE], item_num)
                && (vnp->choice == 255 || vnp->choice == sfp->idx.subtype)
                 && CheckForString (cfp->deleteStr, sfp->comment)) {
              found = TRUE;
            }
          }
        }
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
  return found;  
}

static Boolean DoFeaturesContainText_Callback 
(BioseqPtr      bsp,
 ConvertFormPtr cfp)
{
  
  SeqAnnotPtr    sap;

  /* Check parameters */

  if (bsp == NULL || cfp == NULL)
    return FALSE;

  /* Get the list of annotations */

  sap = bsp->annot;

  /* Search the requested item for the given string */

  return DoFeaturesInAnnotContainText (sap, cfp);
}

typedef struct objstringdata 
{
  CharPtr match;
  Boolean found;	
} ObjStringData, PNTR ObjStringPtr;

static void LIBCALLBACK AsnWriteRemoveForDCallBack (AsnExpOptStructPtr pAEOS)

{
  CharPtr        pchFind;
  CharPtr        pchSource;
  ObjStringPtr   osp;

  osp = (ObjStringPtr) pAEOS->data;
  if (ISA_STRINGTYPE (AsnFindBaseIsa (pAEOS->atp))) {
	pchSource = (CharPtr) pAEOS->dvp->ptrvalue;
	pchFind = osp->match;
	if (StringSearch (pchSource, pchFind) != NULL) {
	  osp->found = TRUE;
	}
  }
}

static Boolean ObjectHasSubstring (ObjMgrTypePtr omtp, AsnIoPtr aip, Pointer ptr, ObjStringPtr osp)

{
  osp->found = FALSE;
  (omtp->asnwrite) (ptr, aip, NULL);
  return osp->found;
}

static Uint1 GetPubStatus (PubdescPtr pdp)
{
  ValNodePtr vnp;
  CitGenPtr  cgp;
  CitArtPtr  cap;
  CitJourPtr cjp;
  CitBookPtr cbp;
  CitSubPtr  csp;
  MedlineEntryPtr mlp;
  ImprintPtr ip = NULL;
  Uint1      status = 255; /* 255 is currently not a valid status */
  
  if (pdp == NULL) return status;
  
  for (vnp = pdp->pub; vnp != NULL && ip == NULL; vnp = vnp->next)
  {
  	switch (vnp->choice)
  	{
  	  case PUB_Gen:
        cgp = (CitGenPtr) vnp->data.ptrvalue;
  	    if (cgp != NULL && StringICmp (cgp->cit, "Unpublished"))
  	    {
  	  	  return PUB_STATUS_UNPUBLISHED;
  	    }
  	    break;
  	  case PUB_Article:
  	  case PUB_Medline:
  	    if (vnp->choice == PUB_Article)
  	    {
  	      cap = (CitArtPtr) vnp->data.ptrvalue;
  	    }
  	    else
  	    {
  	      cap = NULL;
  	      mlp = (MedlineEntryPtr) vnp->data.ptrvalue;
  	      if (mlp != NULL)
  	      {
  	        cap = mlp->cit;
  	      }
  	    }
  	    if (cap != NULL && cap->from == 1)
  	    {
  	      cjp = (CitJourPtr) cap->fromptr;
  	      if (cjp != NULL)
  	      {
  	      	ip = cjp->imp;
  	      }
  	    }
  	    break;
  	  case PUB_Man:
      case PUB_Book:
        cbp = (CitBookPtr) vnp->data.ptrvalue;
        if (cbp != NULL)
        {
          ip = cbp->imp;
        }
        break;
  	  case PUB_Sub:
  	    csp = (CitSubPtr) vnp->data.ptrvalue;
  	    if (csp != NULL)
  	    {
  	      ip = csp->imp;
  	    }
  	    break; 
  	}
  }
  if (ip != NULL)
  {
  	status = ip->prepub;
  }
  return status;  
}

static Boolean DoesPubStatusMatch (PubdescPtr pdp, ConvertFormPtr cfp)
{
  Uint1 pub_status;
  
  if (pdp == NULL || cfp == NULL) return FALSE;
  if (cfp->subtype == 0) return TRUE;
  
  pub_status = GetPubStatus (pdp);
  
  if (cfp->subtype == PUBLICATION_PUBLISHED_FIELD 
      && pub_status == PUB_STATUS_PUBLISHED)
  {
  	return TRUE;
  }
  else if (cfp->subtype == PUBLICATION_INPRESS_FIELD
      && pub_status == PUB_STATUS_IN_PRESS)
  {
  	return TRUE;
  }
  else if (cfp->subtype == PUBLICATION_UNPUB_FIELD
      && pub_status == PUB_STATUS_UNPUBLISHED)
  {
  	return TRUE;
  }
  else
  {
  	return FALSE;
  }
}

static Boolean DoesSequenceHavePubWithText (BioseqPtr bsp, ConvertFormPtr cfp)
{
  AsnExpOptPtr      aeop;
  AsnIoPtr          aip;
  ObjStringData     osd;
  SeqMgrDescContext dcontext;
  SeqDescrPtr       sdp;
  SeqMgrFeatContext fcontext;
  SeqFeatPtr        sfp;
  Boolean           rval = FALSE;
  ObjMgrPtr         omp;
  ObjMgrTypePtr     omtp;
  PubdescPtr        pdp;

  if (bsp == NULL || cfp == NULL) return FALSE;
  omp = ObjMgrGet ();
  if (omp == NULL) return FALSE;
  omtp = ObjMgrTypeFind (omp, OBJ_SEQDESC, NULL, NULL);
  if (omtp == NULL) return FALSE;
  
  aip = AsnIoNullOpen ();
  aeop = AsnExpOptNew (aip, NULL, NULL, AsnWriteRemoveForDCallBack);
  if (aeop != NULL) {
    aeop->user_data = (Pointer) &osd;
  }
  osd.match = cfp->deleteStr;

  /* look for publication descriptors */
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pub, &dcontext);
  while (sdp != NULL && !rval) {
    if (ObjectHasSubstring (omtp, aip, (Pointer) sdp, &osd)) {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
      if (DoesPubStatusMatch (pdp, cfp))
      {
        rval = TRUE;
      }
    }
    sdp = SeqMgrGetNextDescriptor (bsp, sdp, Seq_descr_pub, &dcontext);
  }
  
  if (!rval)
  { 
    omtp = ObjMgrTypeFind (omp, OBJ_SEQFEAT, NULL, NULL);
    if (omtp != NULL) 
    {	
      /* look for publication features */
      sfp = SeqMgrGetNextFeature (bsp, NULL, 0, FEATDEF_PUB, &fcontext);
      while (sfp != NULL && !rval) 
      {
        if (ObjectHasSubstring (omtp, aip, (Pointer) sfp, &osd))
        {
          pdp = (PubdescPtr) sfp->data.value.ptrvalue;
          if (DoesPubStatusMatch (pdp, cfp))
          {
            rval = TRUE;
          }
        }
        sfp = SeqMgrGetNextFeature (bsp, sfp, 0, FEATDEF_PUB, &fcontext);
      }
    }
  }
  
  AsnIoClose (aip); 
  return rval;
}

static Boolean DoesNucProtSetHavePubWithText (BioseqSetPtr bssp, ConvertFormPtr cfp)
{
  AsnExpOptPtr      aeop;
  AsnIoPtr          aip;
  ObjStringData     osd;
  SeqDescrPtr       sdp;
  Boolean           rval = FALSE;
  ObjMgrPtr         omp;
  ObjMgrTypePtr     omtp;
  PubdescPtr        pdp;
  
  if (bssp == NULL || cfp == NULL) return FALSE;  
  omp = ObjMgrGet ();
  if (omp == NULL) return FALSE;
  omtp = ObjMgrTypeFind (omp, OBJ_SEQDESC, NULL, NULL);
  if (omtp == NULL) return FALSE;

  aip = AsnIoNullOpen ();
  aeop = AsnExpOptNew (aip, NULL, NULL, AsnWriteRemoveForDCallBack);
  if (aeop != NULL) {
    aeop->user_data = (Pointer) &osd;
  }
  osd.match = cfp->deleteStr;

  /* look for publication descriptors */
  sdp = bssp->descr;
  while (sdp != NULL && !rval) {
    if (sdp->choice == Seq_descr_pub && ObjectHasSubstring (omtp, aip, (Pointer) sdp, &osd)) {
      pdp = (PubdescPtr) sdp->data.ptrvalue;
      if (DoesPubStatusMatch (pdp, cfp))
      {
        rval = TRUE;
      }
    }
    sdp = sdp->next;
  }
  
  AsnIoClose (aip); 
  return rval;
}

static Boolean LIBCALLBACK DoesSequenceContainText (BioseqPtr bsp, Pointer userdata)
{
  BioSourcePtr biop;
  ConvertFormPtr cfp;
  Boolean      found = FALSE;
  SeqDescrPtr       sdp;
  SeqMgrDescContext descContext;

  cfp = (ConvertFormPtr) userdata;
  if (bsp == NULL || cfp == NULL) return FALSE;

  switch (cfp->type) {
    case GENE_TYPE :
    case CDS_TYPE :
    case PROT_TYPE :
    case RNA_TYPE :
    case IMPORT_FEAT_TYPE :
    case FEATURE_NOTE_TYPE :
      found = DoFeaturesContainText_Callback (bsp, cfp);
      break;
    case BIOSOURCE_TYPE :
    case ORGMOD_SUBSOURCE_TYPE :
      sdp = SeqMgrGetNextDescriptor (bsp, NULL, 0, &descContext);
      while (NULL != sdp && ! found) {
        if (Seq_descr_source == sdp->choice 
            && (biop = sdp->data.ptrvalue) != NULL) {
          if (cfp->type == BIOSOURCE_TYPE) {
            found = DoesBioSourceContainText (biop, cfp);
          } else if (cfp->type == ORGMOD_SUBSOURCE_TYPE) {
            found = DoSubSourcesContainText (biop, cfp);
          }
        }
        sdp = SeqMgrGetNextDescriptor (bsp, sdp, 0, &descContext);
      }
      break;
    case DEFLINE_TYPE :
      sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_title, &descContext);
      if (sdp != NULL && sdp->data.ptrvalue != NULL
          && CheckForString (cfp->deleteStr, sdp->data.ptrvalue)) {
        found = TRUE;
      }
      break;
  	case PUBLICATION_TYPE :
  	  found = DoesSequenceHavePubWithText (bsp, cfp);
  	  break;
    default:
      break;
  }
  return found;
}

static Boolean LIBCALLBACK  DoesNucProtSetContainText (BioseqSetPtr bssp, Pointer userdata)
{
  SeqEntryPtr sep;
  BioseqPtr   bsp;
  ConvertFormPtr cfp;

  if (bssp == NULL) return FALSE;
  cfp = (ConvertFormPtr) userdata;
  if (cfp == NULL) return FALSE;
  if (cfp->type == PUBLICATION_TYPE && DoesNucProtSetHavePubWithText (bssp, cfp))
  {
  	return TRUE;
  }
  if (DoFeaturesInAnnotContainText (bssp->annot, cfp))
  {
  	return TRUE;
  }

  for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
    if (IS_Bioseq (sep)) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (DoesSequenceContainText (bsp, (Pointer)cfp)) {
        return TRUE;
      }
    }
  }
  return FALSE;
}

static void SegregateItemsByText 
(SeqEntryPtr                     seqlist,
 ConvertFormPtr                  cfp,
 BioseqSetPtr                    set1,
 BioseqSetPtr                    set2)
{
  SegregateItemsRecursor (seqlist, set1, set2, 
                  DoesNucProtSetContainText,
                  DoesSequenceContainText, (Pointer)cfp);
}

typedef void (LIBCALLBACK *segregateFunction) (
  SeqEntryPtr  seqlist,
  Pointer      userdata,
  BioseqSetPtr set1,
  BioseqSetPtr set2
);

static void SegregateItemsGenericCallback
(SeqEntryPtr       sep,
 BioseqSetPtr      bssp,
 Uint2             entityID,
 Pointer           userdata,
 segregateFunction seg_func)
{
  ObjMgrDataPtr     omdptop;
  ObjMgrData        omdata;
  BioseqSetPtr      parent_set;
  BioseqSetPtr      newset1;
  BioseqSetPtr      newset2;
  SeqEntryPtr       tmp1, tmp2, last_sep;
  Uint2             parenttype;
  Pointer           parentptr;
  SeqEntryPtr       seqlist;

  if (sep == NULL || seg_func == NULL) return;
  /* Display the 'working' cursor */

  WatchCursor ();
  Update ();


  SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
  GetSeqEntryParent (sep, &parentptr, &parenttype);

  parent_set = (BioseqSetPtr)(bssp->idx.parentptr);
  seqlist = bssp->seq_set;
  bssp->seq_set = NULL;

  if (parent_set == NULL || parent_set->seq_set == NULL) {
    newset1 = BioseqSetNew ();
    if (newset1 == NULL) return;
    newset2 = BioseqSetNew ();
    if (newset2 == NULL) return;
    newset1->_class = bssp->_class;
    newset2->_class = bssp->_class;
    tmp1 = SeqEntryNew ();
    if (tmp1 == NULL) return;
    tmp1->choice = 2;
    tmp1->data.ptrvalue = (Pointer) newset1;
    tmp2 = SeqEntryNew ();
    if (tmp2 == NULL) return;
    tmp2->choice = 2;
    tmp2->data.ptrvalue = (Pointer) newset2;
    bssp->seq_set = tmp1;
    tmp1->next = tmp2;
    bssp->_class = BioseqseqSet_class_genbank;
    /* Propagate descriptors down */
    ValNodeLink (&(newset1->descr),
                 AsnIoMemCopy ((Pointer) bssp->descr,
                               (AsnReadFunc) SeqDescrAsnRead,
                               (AsnWriteFunc) SeqDescrAsnWrite));
    ValNodeLink (&(newset2->descr),
                 AsnIoMemCopy ((Pointer) bssp->descr,
                               (AsnReadFunc) SeqDescrAsnRead,
                               (AsnWriteFunc) SeqDescrAsnWrite));
    bssp->descr = SeqDescrFree (bssp->descr);
  } else {
    last_sep = parent_set->seq_set;
    newset1 = bssp;
    newset2 = BioseqSetNew ();
    if (newset2 == NULL) return;
    newset2->_class = newset1->_class;
    tmp1 = SeqEntryNew ();
    if (tmp1 == NULL) return;
    tmp1->choice = 2;
    tmp1->data.ptrvalue = (Pointer) newset2;
    while (last_sep != NULL && last_sep->next != NULL) {
      last_sep = last_sep->next;
    }
    if (last_sep == NULL) return;
    last_sep->next = tmp1;
    /* copy descriptors horizontally */
    ValNodeLink (&(newset2->descr),
                 AsnIoMemCopy ((Pointer) bssp->descr,
                               (AsnReadFunc) SeqDescrAsnRead,
                               (AsnWriteFunc) SeqDescrAsnWrite));
  }

  /* Do the search and move sequences */
  (*seg_func)(seqlist, userdata, newset1, newset2);

  /* Remove the window and update things */
  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  RestoreSeqEntryObjMgrData (sep, omdptop, &omdata); 
  ObjMgrSetDirtyFlag (entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, entityID, 0, 0);

  ArrowCursor ();
  Update ();
  
  /* Return successfully */
  return;
  
}
  
/*=========================================================================*/
/*                                                                         */
/* SegregateByText_Callback () - Finds and deletes all items of a selected */
/*                            type that contain a given text phrase.       */
/*                                                                         */
/*=========================================================================*/

static void SegregateByText_Callback (ButtoN b)
{
  ConvertFormPtr    cfp;
  SeqEntryPtr       sep;
  SeqEntryPtr       tmp1, tmp2;
  UIEnum            val;
  BioseqSetPtr      bssp;
  BioseqSetPtr      parent_set;
  SeqEntryPtr       seqlist;
  BioseqSetPtr      newset1, newset2;
  ObjMgrDataPtr     omdptop;
  ObjMgrData        omdata;
  Uint2             parenttype;
  Pointer           parentptr;
  SeqEntryPtr       last_sep;
  Boolean           leaveDlgUp = FALSE;

  /* Check the initial conditions and get the sequence */
  cfp = (ConvertFormPtr) GetObjectExtra (b);
  if (cfp == NULL || cfp->input_entityID == 0 || cfp->target_set == NULL) {
    Remove (cfp->form);
    return;
  }

  sep = GetTopSeqEntryForEntityID (cfp->input_entityID); 
  if (sep == NULL) {
    Remove (cfp->form);
    return;
  }
  
  if (GetStatus (cfp->leaveDlgUp))
  {
  	leaveDlgUp = TRUE;
  }

  SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
  GetSeqEntryParent (sep, &parentptr, &parenttype);

  bssp = cfp->target_set;

  parent_set = (BioseqSetPtr)(bssp->idx.parentptr);
  seqlist = bssp->seq_set;
  bssp->seq_set = NULL;

  if (parent_set == NULL || parent_set->seq_set == NULL) {
    newset1 = BioseqSetNew ();
    if (newset1 == NULL) return;
    newset2 = BioseqSetNew ();
    if (newset2 == NULL) return;
    newset1->_class = bssp->_class;
    newset2->_class = bssp->_class;
    tmp1 = SeqEntryNew ();
    if (tmp1 == NULL) return;
    tmp1->choice = 2;
    tmp1->data.ptrvalue = (Pointer) newset1;
    tmp2 = SeqEntryNew ();
    if (tmp2 == NULL) return;
    tmp2->choice = 2;
    tmp2->data.ptrvalue = (Pointer) newset2;
    bssp->seq_set = tmp1;
    tmp1->next = tmp2;
    bssp->_class = BioseqseqSet_class_genbank;
    /* Propagate descriptors down */
    ValNodeLink (&(newset1->descr),
                 AsnIoMemCopy ((Pointer) bssp->descr,
                               (AsnReadFunc) SeqDescrAsnRead,
                               (AsnWriteFunc) SeqDescrAsnWrite));
    ValNodeLink (&(newset2->descr),
                 AsnIoMemCopy ((Pointer) bssp->descr,
                               (AsnReadFunc) SeqDescrAsnRead,
                               (AsnWriteFunc) SeqDescrAsnWrite));
    bssp->descr = SeqDescrFree (bssp->descr);
  } else {
    last_sep = parent_set->seq_set;
    newset1 = bssp;
    newset2 = BioseqSetNew ();
    if (newset2 == NULL) return;
    newset2->_class = newset1->_class;
    tmp1 = SeqEntryNew ();
    if (tmp1 == NULL) return;
    tmp1->choice = 2;
    tmp1->data.ptrvalue = (Pointer) newset2;
    while (last_sep != NULL && last_sep->next != NULL) {
      last_sep = last_sep->next;
    }
    if (last_sep == NULL) return;
    last_sep->next = tmp1;
    /* copy descriptors horizontally */
    ValNodeLink (&(newset2->descr),
                 AsnIoMemCopy ((Pointer) bssp->descr,
                               (AsnReadFunc) SeqDescrAsnRead,
                               (AsnWriteFunc) SeqDescrAsnWrite));
  }

  /* Get the string to search for */

  cfp->deleteStr = SaveStringFromTextNoStripSpaces (cfp->deleteText);
  if (StringHasNoText (cfp->deleteStr)){ 
    if (!leaveDlgUp)
    {
      Remove (cfp->form);
    }
    return;
  }

  /* Get the type of items to search */

  GetEnumPopup (cfp->target, cfp->target_alist, &val);
  if (0 == val) {
    if (!leaveDlgUp)
    {
      Remove (cfp->form);
    }
    return;
  }
  else
    cfp->type = (Int2) val;
  
  if (cfp->type == PUBLICATION_TYPE)
  {
    GetEnumPopup (cfp->subtarget [cfp->type], cfp->alists [cfp->type], &val);
    cfp->subtype = (Int2) val;
  }
  else if (cfp->type != DEFLINE_TYPE && cfp->type != FEATURE_NOTE_TYPE) {
    GetEnumPopup (cfp->subtarget [cfp->type], cfp->alists [cfp->type], &val);
    if (0 == val) {
      if (!leaveDlgUp)
      {
        Remove (cfp->form);
      }
      return;
    } else {
      cfp->subtype = (Int2) val;
    }
  }

  /* Display the 'working' cursor */

  WatchCursor ();
  Update ();

  /* Do the search and move sequences */
  SegregateItemsByText (seqlist, cfp, newset1, newset2);

  /* Remove the window and update things */
  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  RestoreSeqEntryObjMgrData (sep, omdptop, &omdata); 
  ObjMgrSetDirtyFlag (cfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, cfp->input_entityID, 0, 0);

  ArrowCursor ();
  Update ();
  if (!leaveDlgUp)
  {
    Remove (cfp->form);
  }

  /* Return successfully */
  return;
}




/*-------------------------------------------------------------------------*/
/*                                                                         */
/* SetSegregateAcceptButton () -- Enable/Disable the Accept button depending  */
/*                             on the condition of other window objects.   */
/*                                                                         */
/*-------------------------------------------------------------------------*/

static void SetSegregateAcceptButton (Handle a)

{
  ConvertFormPtr  cfp;
  UIEnum          val;
  ValNodePtr      vnp;
  Int2            item_num;
  Boolean         beforeChecked;
  Boolean         afterChecked;

  cfp = (ConvertFormPtr) GetObjectExtra (a);
  if (cfp == NULL)
    return;

  /* Disable if a target has not been selected */

  if (!GetEnumPopup (cfp->target, cfp->target_alist, &val)) {
    SafeDisable (cfp->accept);
    return;
  }

  if (val <= 0) {
    SafeDisable (cfp->accept);
    return;
  }

  /* Disable if a subtarget has not been selected and target is not defline */

  if (val == FEATURE_NOTE_TYPE) {
    item_num = 1;
    for (vnp = cfp->feature_list, item_num = 1;
         vnp != NULL && ! GetItemStatus (cfp->subtarget [FEATURE_NOTE_TYPE], item_num);
         vnp = vnp->next, item_num++) 
    {}
    if (vnp == NULL) {
      SafeDisable (cfp->accept);
      return;
    }
  } else if (val != DEFLINE_TYPE && val != PUBLICATION_TYPE) {
    cfp->type = (Int2) val;
  
    if (!GetEnumPopup (cfp->subtarget [cfp->type],
	  	      cfp->alists [cfp->type], &val)) {
      SafeDisable (cfp->accept);
      return;
    }

    if (val <= 0) {
      SafeDisable (cfp->accept);
      return;
    }
    if (cfp->type == IMPORT_FEAT_TYPE && cfp->gb_qual_pup != NULL) {
      cfp->gb_qual_type = GetValue (cfp->gb_qual_pup);
      if (cfp->gb_qual_type <= 0) {
        SafeDisable (cfp->accept);
        return;
      }
    }
  }

  if (cfp->deleteText != NULL) {
    /* Disable if there is no delete string */ 
    if (TextHasNoText (cfp->deleteText)) {
      SafeDisable (cfp->accept);
      return;
    }
  }

  if (cfp->removeBefore != NULL && cfp->removeAfter != NULL) {
    beforeChecked = GetStatus (cfp->removeBefore);
    afterChecked  = GetStatus (cfp->removeAfter);

    /* If both before and after text are disabled */
    /* then disable the accept button.            */

    if ((FALSE == beforeChecked) && (FALSE == afterChecked)) {
      SafeDisable (cfp->accept);
      return;
    }

    /* If the before field is enabled but empty */
    /* then disable the accept button.          */

    if ((TRUE == beforeChecked) && (TextHasNoText (cfp->atleft))) {
      SafeDisable (cfp->accept);
      return;
    }

    /* If the before field is enabled but empty */
    /* then disable the accept button.          */

    if ((TRUE == afterChecked) && (TextHasNoText (cfp->atright))) {
      SafeDisable (cfp->accept);
      return;
    }
  }

  /* If we made it to here, then we passed all */
  /* test and can enable the accept button.    */

  SafeEnable (cfp->accept);
}

static void CleanupSegregatePage (GraphiC g, VoidPtr data)

{
  ConvertFormPtr  cfp;

  cfp = (ConvertFormPtr) data;
  if (cfp != NULL) {
    ValNodeFreeData (cfp->feature_list);
  }
  StdCleanupFormProc (g, data);
}

/*=========================================================================*/
/*                                                                         */
/* CreateSegregateByTextWindow () - Creates and then displays the window   */
/*                        for getting segregate by text info from the user.*/
/*                                                                         */
/*=========================================================================*/

extern Int2 LIBCALLBACK CreateSegregateByTextWindow (Pointer data)
{
  EnumFieldAssocPtr  ap;
  GrouP              c;
  ConvertFormPtr     cfp;
  GrouP              g;
  GrouP              h;
  Int2               j;
  OMProcControlPtr   ompcp;
  GrouP              p;
  StdEditorProcsPtr  sepp;
  WindoW             w;
  GrouP              x;
  ValNodePtr         vnp;

  /* Check parameters and get a pointer to the current data */

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL)
    return OM_MSG_RET_ERROR;

  if (ompcp->input_itemtype != OBJ_BIOSEQSET || ompcp->input_data == NULL) {
    Message (MSG_ERROR, "Must select Bioseq Set!");
    return OM_MSG_RET_ERROR;
  }

  /* Create a new window, and a struct */
  /* to pass around the data in.       */

  cfp = ConvertFormNew ();
  if (cfp == NULL)
    return OM_MSG_RET_ERROR;
  cfp->parsetype = PARSETYPE_FILE;
  cfp->target_set = (BioseqSetPtr)ompcp->input_data;
  cfp->target_alist = segregate_target_field_alist;
  cfp->set_accept_proc = (PupActnProc) SetSegregateAcceptButton;

  w = FixedWindow (-50, -33, -10, -10, "Segregate By Text String",
		   StdCloseWindowProc);
  SetObjectExtra (w, cfp, CleanupSegregatePage);
  cfp->form = (ForM) w;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    cfp->appmessage = sepp->handleMessages;
  }

  cfp->input_entityID = ompcp->input_entityID;
  cfp->input_itemID = ompcp->input_itemID;
  cfp->input_itemtype = ompcp->input_itemtype;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    cfp->appmessage = sepp->handleMessages;
  }

  /* Add the popup lists */

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 3, 0, NULL);

  StaticPrompt (g, "Segregate sequences with the string", 0, dialogTextHeight,
		programFont, 'l');
  cfp->deleteText = DialogText (g, "", 10, (TxtActnProc) cfp->set_accept_proc);
  SetObjectExtra (cfp->deleteText, cfp, NULL);

  p = HiddenGroup (h, 6, 0, NULL);
  StaticPrompt (p, "Find string in", 0, popupMenuHeight, programFont, 'l');
  cfp->target = PopupList (p, TRUE, ChangeSubtarget);
  SetObjectExtra (cfp->target, cfp, NULL);
  InitEnumPopup (cfp->target, cfp->target_alist, NULL);
  SetEnumPopup (cfp->target, cfp->target_alist, 0);

  cfp->alists [GENE_TYPE]             = gene_field_alist;
  cfp->alists [CDS_TYPE]              = cds_field_alist;
  cfp->alists [PROT_TYPE]             = prot_field_alist;
  cfp->alists [RNA_TYPE]              = rnax_field_alist;
  cfp->alists [BIOSOURCE_TYPE]        = orgref_field_alist;
  cfp->alists [ORGMOD_SUBSOURCE_TYPE] = subsource_and_orgmod_note_subtype_alist;
  cfp->alists [IMPORT_FEAT_TYPE]      = impfeat_field_alist;
  cfp->alists [PUBLICATION_TYPE]      = publication_field_alist;
  cfp->feature_list = BuildFeatureValNodeList (TRUE, "All", 255, TRUE, FALSE);

  x = HiddenGroup (p, 0, 0, NULL);

  for (j = 1; j <= NUMBER_OF_SEGREGATE_TYPES; j++) {
    if (j == ORGMOD_SUBSOURCE_TYPE) {
      cfp->subtarget [j] =	(PopuP) SingleList (x, 10, 8, (LstActnProc) cfp->set_accept_proc);
      SetObjectExtra (cfp->subtarget [j], cfp, NULL);
      for (ap = cfp->alists [j]; ap->name != NULL; ap++) {
	    ListItem ((LisT) cfp->subtarget [j], ap->name);
      }
      Hide (cfp->subtarget [j]);
    } else if (j == DEFLINE_TYPE) {
      /* do nothing */
    } else if (j == FEATURE_NOTE_TYPE) {
      cfp->subtarget [j] = (PopuP) MultiList (x, 10, 8, (LstActnProc) cfp->set_accept_proc);
      SetObjectExtra (cfp->subtarget [j], cfp, NULL);
      for (vnp = cfp->feature_list; vnp != NULL; vnp = vnp->next) {
        ListItem ((LisT) cfp->subtarget [j], vnp->data.ptrvalue);
      }
      Hide (cfp->subtarget [j]);
    } else {
      cfp->subtarget [j] = PopupList (x, TRUE, cfp->set_accept_proc);
      SetObjectExtra (cfp->subtarget [j], cfp, NULL);
      InitEnumPopup (cfp->subtarget [j], cfp->alists [j], NULL);
      SetEnumPopup (cfp->subtarget [j], cfp->alists [j], 0);
      Hide (cfp->subtarget [j]);
    }
  }

  /* Add Accept and Cancel buttons */

  c = HiddenGroup (h, 4, 0, NULL);
  cfp->accept = DefaultButton (c, "Accept", SegregateByText_Callback);
  SetObjectExtra (cfp->accept, cfp, NULL);
  Disable (cfp->accept);
  PushButton (c, "Cancel", StdCancelButtonProc);
  cfp->leaveDlgUp = CheckBox (c, "Leave Dialog Up", NULL);

  /* Line things up nicely */

  AlignObjects (ALIGN_LEFT, (HANDLE) p, (HANDLE) c, (HANDLE) h, NULL);

  /* Display the window now */

  RealizeWindow (w);
  Show (w);
  Select (w);
  Select (cfp->accept);
  Update ();
  return OM_MSG_RET_OK;
}


typedef struct segregatefeatdata {
  FEATURE_FORM_BLOCK

  PopuP        type_popup;
  ValNodePtr   type_list;
  ButtoN       accept;
  
  BioseqSetPtr target_set;
  Uint2        segregate_type;
  Boolean      is_feat;
} SegregateFeatData, PNTR SegregateFeatPtr;

static void CleanupSegregateFeatPage (GraphiC g, VoidPtr data)

{
  SegregateFeatPtr  sfp;

  sfp = (SegregateFeatPtr) data;
  MemFree (sfp);
  StdCleanupFormProc (g, data);
}

static Boolean LIBCALLBACK DoesSequenceContainFeatureType (BioseqPtr bsp, Pointer userdata)
{
  SeqMgrFeatContext context;
  SeqFeatPtr        feat;
  SegregateFeatPtr  sfp;
  
  sfp = (SegregateFeatPtr) userdata;
  if (sfp == NULL || bsp == NULL) return FALSE;
  
  feat = NULL;
  while ((feat = SeqMgrGetNextFeature (bsp, feat, 0, 0, &context)) != NULL)
  {
  	if (feat->idx.subtype == sfp->segregate_type)
  	{
  	  return TRUE;
  	}
  }
  return FALSE;
}

static Boolean LIBCALLBACK DoesNucProtSetContainFeatureType (BioseqSetPtr bssp, Pointer userdata)
{
  SeqEntryPtr sep;
  BioseqPtr   bsp;
  SegregateFeatPtr  sfp;
  
  sfp = (SegregateFeatPtr) userdata;
  if (sfp == NULL || bssp == NULL) return FALSE;

  for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
    if (IS_Bioseq (sep)) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      if (DoesSequenceContainFeatureType (bsp, sfp)) {
        return TRUE;
      }
    }
  }
  return FALSE;	
}

static Boolean LIBCALLBACK DoesSequenceContainDescriptorType (BioseqPtr bsp, Pointer userdata)
{
  SeqMgrDescContext context;
  SeqDescPtr        desc;
  SegregateFeatPtr  sfp;
  
  sfp = (SegregateFeatPtr) userdata;
  if (sfp == NULL || bsp == NULL) return FALSE;
  
  if((desc = SeqMgrGetNextDescriptor (bsp, NULL, sfp->segregate_type, &context)) != NULL)
  {
	return TRUE;
  }
  return FALSE;
}

typedef struct checkdescdata {
  Uint2   segregate_type;
  Boolean found;
} CheckDescData, PNTR CheckDescPtr;

static void DoesSetContainDescriptorType_Callback (SeqDescPtr sdp, Pointer userdata)
{
  CheckDescPtr p;
  
  if (sdp == NULL || userdata == NULL) return;
  p = (CheckDescPtr) userdata;
  if (p->found) return;
  if (sdp->choice == p->segregate_type) p->found = TRUE;
}

static Boolean LIBCALLBACK DoesNucProtSetContainDescriptorType (BioseqSetPtr bssp, Pointer userdata)
{
  CheckDescData d;
  SegregateFeatPtr  sfp;
  
  sfp = (SegregateFeatPtr) userdata;
  if (sfp == NULL || bssp == NULL) return FALSE;

  d.found = FALSE;
  d.segregate_type = sfp->segregate_type;
  VisitDescriptorsInSet (bssp, &d, DoesSetContainDescriptorType_Callback);
  return d.found;	
}

/*=========================================================================*/
/*                                                                         */
/* SegregateItemsByFeature () - Given a feature type, move bioseqs         */
/*                        containing those features to a new popset.       */
/*                                                                         */
/*=========================================================================*/

static void SegregateItemsByFeatureOrDescriptor 
(SeqEntryPtr      seqlist,
 SegregateFeatPtr sfp,
 BioseqSetPtr     set1,
 BioseqSetPtr     set2)
{
  

  if (sfp == NULL || set1 == NULL || set2 == NULL || seqlist == NULL)
    return;
  
  if (sfp->is_feat)
  {
  	SegregateItemsRecursor (seqlist, set1, set2, 
  	                               DoesNucProtSetContainFeatureType,
  	                               DoesSequenceContainFeatureType,
  	                               (Pointer) sfp);
  } 
  else 
  {
  	SegregateItemsRecursor (seqlist, set1, set2, 
  	                               DoesNucProtSetContainDescriptorType,
  	                               DoesSequenceContainDescriptorType,
  	                               (Pointer) sfp);
  }
  
}


/*=========================================================================*/
/*                                                                         */
/* SegregateByFeatureOrDescriptor_Callback () - Segregates sequences that  */
/*                            contain a selected feature.                  */
/*                                                                         */
/*=========================================================================*/

static void SegregateByFeatureOrDescriptor_Callback (ButtoN b)
{
  SegregateFeatPtr  sfp;
  SeqEntryPtr       sep;
  SeqEntryPtr       tmp1, tmp2;
  UIEnum            val;
  BioseqSetPtr      bssp;
  BioseqSetPtr      parent_set;
  SeqEntryPtr       seqlist;
  BioseqSetPtr      newset1, newset2;
  ObjMgrDataPtr     omdptop;
  ObjMgrData        omdata;
  Uint2             parenttype;
  Pointer           parentptr;
  SeqEntryPtr       last_sep;
  ValNodePtr        vnp;

  /* Check the initial conditions and get the sequence */
  sfp = (SegregateFeatPtr) GetObjectExtra (b);
  if (sfp == NULL || sfp->input_entityID == 0 || sfp->target_set == NULL) {
    Remove (sfp->form);
    return;
  }

  sep = GetTopSeqEntryForEntityID (sfp->input_entityID); 
  if (sep == NULL) {
    Remove (sfp->form);
    return;
  }

  SaveSeqEntryObjMgrData (sep, &omdptop, &omdata);
  GetSeqEntryParent (sep, &parentptr, &parenttype);

  bssp = sfp->target_set;

  parent_set = (BioseqSetPtr)(bssp->idx.parentptr);
  seqlist = bssp->seq_set;
  bssp->seq_set = NULL;

  if (parent_set == NULL || parent_set->seq_set == NULL) {
    newset1 = BioseqSetNew ();
    if (newset1 == NULL) return;
    newset2 = BioseqSetNew ();
    if (newset2 == NULL) return;
    newset1->_class = bssp->_class;
    newset2->_class = bssp->_class;
    tmp1 = SeqEntryNew ();
    if (tmp1 == NULL) return;
    tmp1->choice = 2;
    tmp1->data.ptrvalue = (Pointer) newset1;
    tmp2 = SeqEntryNew ();
    if (tmp2 == NULL) return;
    tmp2->choice = 2;
    tmp2->data.ptrvalue = (Pointer) newset2;
    bssp->seq_set = tmp1;
    tmp1->next = tmp2;
    bssp->_class = BioseqseqSet_class_genbank;
    /* Propagate descriptors down */
    ValNodeLink (&(newset1->descr),
                 AsnIoMemCopy ((Pointer) bssp->descr,
                               (AsnReadFunc) SeqDescrAsnRead,
                               (AsnWriteFunc) SeqDescrAsnWrite));
    ValNodeLink (&(newset2->descr),
                 AsnIoMemCopy ((Pointer) bssp->descr,
                               (AsnReadFunc) SeqDescrAsnRead,
                               (AsnWriteFunc) SeqDescrAsnWrite));
    bssp->descr = SeqDescrFree (bssp->descr);
  } else {
    last_sep = parent_set->seq_set;
    newset1 = bssp;
    newset2 = BioseqSetNew ();
    if (newset2 == NULL) return;
    newset2->_class = newset1->_class;
    tmp1 = SeqEntryNew ();
    if (tmp1 == NULL) return;
    tmp1->choice = 2;
    tmp1->data.ptrvalue = (Pointer) newset2;
    while (last_sep != NULL && last_sep->next != NULL) {
      last_sep = last_sep->next;
    }
    if (last_sep == NULL) return;
    last_sep->next = tmp1;
    /* copy descriptors horizontally */
    ValNodeLink (&(newset2->descr),
                 AsnIoMemCopy ((Pointer) bssp->descr,
                               (AsnReadFunc) SeqDescrAsnRead,
                               (AsnWriteFunc) SeqDescrAsnWrite));
  }

  /* Get the feature to look for */
  val = GetValue (sfp->type_popup);
  for (vnp = sfp->type_list; vnp != NULL && val > 1; vnp = vnp->next, val--)
  {  	
  }
  if (vnp == NULL || val != 1)
  {
    Remove (sfp->form);
    return;
  }
  sfp->segregate_type = vnp->choice;
  
  /* Display the 'working' cursor */

  WatchCursor ();
  Update ();

  /* Do the search and move sequences */
  SegregateItemsByFeatureOrDescriptor (seqlist, sfp, newset1, newset2);

  /* Remove the window and update things */
  SeqMgrLinkSeqEntry (sep, parenttype, parentptr);
  RestoreSeqEntryObjMgrData (sep, omdptop, &omdata); 
  ObjMgrSetDirtyFlag (sfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, sfp->input_entityID, 0, 0);

  ArrowCursor ();
  Update ();
  Remove (sfp->form);

  /* Return successfully */
  return;
}


/*=========================================================================*/
/*                                                                         */
/* CreateSegregateByFeatureWindow () - Creates and then displays the window*/
/*                        for getting segregate by text info from the user.*/
/*                                                                         */
/*=========================================================================*/

static Int2 LIBCALLBACK CreateSegregateByFeatureOrDescriptorWindow (Pointer data, Boolean is_feat)
{
  GrouP              c;
  SegregateFeatPtr   sfp;
  GrouP              g;
  GrouP              h;
  OMProcControlPtr   ompcp;
  StdEditorProcsPtr  sepp;
  WindoW             w;
  ValNodePtr         vnp;

  /* Check parameters and get a pointer to the current data */

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL)
    return OM_MSG_RET_ERROR;

  if (ompcp->input_itemtype != OBJ_BIOSEQSET || ompcp->input_data == NULL) {
    Message (MSG_ERROR, "Must select Bioseq Set!");
    return OM_MSG_RET_ERROR;
  }

  /* Create a new window, and a struct */
  /* to pass around the data in.       */

  sfp = (SegregateFeatPtr) MemNew (sizeof (SegregateFeatData));
  if (sfp == NULL)
    return OM_MSG_RET_ERROR;
  sfp->is_feat = is_feat;

  if (sfp->is_feat)
  {
    w = FixedWindow (-50, -33, -10, -10, "Segregate By Feature",
	                 StdCloseWindowProc);
  }
  else
  {
    w = FixedWindow (-50, -33, -10, -10, "Segregate By Descriptor",
	                 StdCloseWindowProc);
  }
		   
  SetObjectExtra (w, sfp, CleanupSegregateFeatPage);
  sfp->form = (ForM) w;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    sfp->appmessage = sepp->handleMessages;
  }

  sfp->input_entityID = ompcp->input_entityID;
  sfp->input_itemID = ompcp->input_itemID;
  sfp->input_itemtype = ompcp->input_itemtype;
  sfp->target_set = (BioseqSetPtr)ompcp->input_data;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    sfp->appmessage = sepp->handleMessages;
  }

  /* Add the popup lists */

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 3, 0, NULL);

  if (sfp->is_feat)
  {
    StaticPrompt (g, "Segregate sequences with the feature", 0, dialogTextHeight,
	              programFont, 'l');
    sfp->type_list = BuildFeatureValNodeList (TRUE, NULL, 0, TRUE, FALSE);
  }
  else
  {
    StaticPrompt (g, "Segregate sequences with the descriptor", 0, dialogTextHeight,
	              programFont, 'l');
	sfp->type_list = BuildDescriptorValNodeList ();
  }

  sfp->type_popup = PopupList (g, TRUE, NULL);
  SetObjectExtra (sfp->type_popup, sfp, NULL);
  for (vnp = sfp->type_list; vnp != NULL; vnp = vnp->next)
  {
    PopupItem (sfp->type_popup, (CharPtr) vnp->data.ptrvalue);
  }
  SetValue (sfp->type_popup, 1);

  /* Add Accept and Cancel buttons */

  c = HiddenGroup (h, 4, 0, NULL);
  sfp->accept = DefaultButton (c, "Accept", SegregateByFeatureOrDescriptor_Callback);
  SetObjectExtra (sfp->accept, sfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  /* Line things up nicely */

  AlignObjects (ALIGN_LEFT, (HANDLE) g, (HANDLE) c, (HANDLE) h, NULL);

  /* Display the window now */

  RealizeWindow (w);
  Show (w);
  Select (w);
  Select (sfp->accept);
  Update ();
  return OM_MSG_RET_OK;
}

extern Int2 LIBCALLBACK CreateSegregateByFeatureWindow (Pointer data)
{
  return CreateSegregateByFeatureOrDescriptorWindow (data, TRUE);
}

extern Int2 LIBCALLBACK CreateSegregateByDescriptorWindow (Pointer data)
{
  return CreateSegregateByFeatureOrDescriptorWindow (data, FALSE);
}

typedef struct segregatemoltypedata
{
  FEATURE_FORM_BLOCK

  PopuP        type_popup;
  ButtoN       use_mol_type;
  PopuP        class_popup;
  ButtoN       use_mol_class;
  ButtoN       accept;
  Uint1        moltype;
  Uint1        molclass;
  BioseqSetPtr target_set;
} SegregateMolTypeData, PNTR SegregateMolTypePtr;

static ENUM_ALIST(molinfo_biomol_alist)
  {"Genomic DNA or RNA",     1},
  {"Precursor RNA",          2},
  {"mRNA [cDNA]",            3},
  {"Ribosomal RNA",          4},
  {"Transfer RNA",           5},
  {"Small nuclear RNA",      6},
  {"Small cytoplasmic RNA",  7},
  {"Peptide",                8},
  {"Other-Genetic",          9},
  {"Genomic-mRNA",          10},
  {"cRNA",                  11},
  {"Small nucleolar RNA",   12},
  {"Transcribed RNA",       13},
  {"Other",                255},
END_ENUM_ALIST

static ENUM_ALIST(mol_class_alist)
{"DNA",             Seq_mol_dna},
{"RNA",             Seq_mol_rna},
{"Protein",         Seq_mol_aa},
{"Nucleotide",      Seq_mol_na},
{"Other",           Seq_mol_other},
END_ENUM_ALIST

static Boolean LIBCALLBACK DoesSequenceHaveMoleculeType (BioseqPtr bsp, Pointer userdata)
{
  SegregateMolTypePtr  smp;
  ValNodePtr           sdp;
  MolInfoPtr           mip;
  
  smp = (SegregateMolTypePtr) userdata;	
  if (bsp == NULL || smp == NULL) return FALSE;
  
  if (GetStatus (smp->use_mol_class) && bsp->mol != smp->molclass)
  {
    return FALSE;
  }

  if (GetStatus (smp->use_mol_type))
  {
    sdp = bsp->descr;
    while (sdp != NULL)
    {
      if (sdp->choice == Seq_descr_molinfo && sdp->data.ptrvalue != NULL) 
      {
        mip = (MolInfoPtr) sdp->data.ptrvalue;
        if (mip->biomol == smp->moltype)
        {
      	  return TRUE;
        }
      }
      sdp = sdp->next;
    }
    return FALSE;
  }
  return TRUE;
}

typedef struct lookformoltype
{
  Boolean found;
  Uint1   moltype;  
  Uint1   molclass;
} LookForMolTypeData, PNTR LookForMolTypePtr;

static void DoesNucProtSetHaveMoleculeTypeCallback (SeqDescPtr sdp, Pointer userdata)
{
  LookForMolTypePtr l;
  MolInfoPtr           mip;
  
  if (sdp == NULL || userdata == NULL) return;
  l = (LookForMolTypePtr) userdata;
  
  if (sdp->choice == Seq_descr_molinfo && sdp->data.ptrvalue != NULL)
  {
  	mip = (MolInfoPtr) sdp->data.ptrvalue;
  	if (mip->biomol == l->moltype)
  	{
  	  l->found = TRUE;
  	}
  }
}

typedef struct lookformolclass
{
  Boolean found;
  Uint1   molclass;
} LookForMolClassData, PNTR LookForMolClassPtr;

static void FindMolClassCallback (BioseqPtr bsp, Pointer userdata)
{
  LookForMolClassPtr lcp;
  
  if (bsp == NULL || userdata == NULL) return;
  lcp = (LookForMolClassPtr) userdata;
  
  if (bsp->mol == lcp->molclass)
  {
    lcp->found = TRUE;
  }
}

static Boolean LIBCALLBACK DoesNucProtSetHaveMoleculeType (BioseqSetPtr bssp, Pointer userdata)
{
  LookForMolTypeData   lm;
  LookForMolClassData  lc;
  SegregateMolTypePtr  smp;

  if (bssp == NULL || userdata == NULL) return FALSE;
  smp = (SegregateMolTypePtr) userdata;
  
  if (GetStatus (smp->use_mol_class))
  {
    lc.found = FALSE;
    lc.molclass = smp->molclass;
    VisitBioseqsInSet (bssp, &lc, FindMolClassCallback);
    if (!lc.found) return FALSE;
  }
  
  if (GetStatus (smp->use_mol_type))
  {
    lm.moltype = smp->moltype;
    lm.found = FALSE;
    VisitDescriptorsInSet (bssp, &lm, DoesNucProtSetHaveMoleculeTypeCallback);
    if (!lm.found) return FALSE;
  }
  return TRUE;
}

static void LIBCALLBACK SegregateByMoleculeType 
(SeqEntryPtr  seqlist,
 Pointer      userdata,
 BioseqSetPtr set1,
 BioseqSetPtr set2)
{
  SegregateItemsRecursor (seqlist, set1, set2, 
                          DoesNucProtSetHaveMoleculeType,
                          DoesSequenceHaveMoleculeType, userdata);
}


/*=========================================================================*/
/*                                                                         */
/* SegregateByMoleculeType_Callback () - Segregates sequences that  */
/*                            have a selected molecule type.                  */
/*                                                                         */
/*=========================================================================*/

static void SegregateByMoleculeType_Callback (ButtoN b)
{
  SegregateMolTypePtr  smp;
  SeqEntryPtr          sep;
  UIEnum               val;

  /* Check the initial conditions and get the sequence */
  smp = (SegregateMolTypePtr) GetObjectExtra (b);
  if (smp == NULL || smp->input_entityID == 0 || smp->target_set == NULL) {
    Remove (smp->form);
    return;
  }

  sep = GetTopSeqEntryForEntityID (smp->input_entityID); 
  if (sep == NULL) {
    Remove (smp->form);
    return;
  }
  
  if (!GetEnumPopup (smp->type_popup, molinfo_biomol_alist, &val)) 
  {
    return;
  }
  smp->moltype = val;
  if (!GetEnumPopup (smp->class_popup, mol_class_alist, &val)) 
  {
    return;
  }
  smp->molclass = val;

  SegregateItemsGenericCallback (sep, smp->target_set, smp->input_entityID,
                                 (Pointer) smp, SegregateByMoleculeType);

  Remove (smp->form);

  /* Return successfully */
  return;
}

static void EnableMolInfoPopups (ButtoN b)
{
  SegregateMolTypePtr smp;
  Boolean             ok_to_accept = FALSE;
  
  smp = (SegregateMolTypePtr) GetObjectExtra (b);
  if (smp == NULL) return;
  
  if (GetStatus (smp->use_mol_type))
  {
    Enable (smp->type_popup);
    ok_to_accept = TRUE;
  }
  else
  {
    Disable (smp->type_popup);
  }
  if (GetStatus (smp->use_mol_class))
  {
    Enable (smp->class_popup);
    ok_to_accept = TRUE;
  }
  else
  {
    Disable (smp->class_popup);
  }  
  if (ok_to_accept)
  {
    Enable (smp->accept);
  }
  else
  {
    Disable (smp->accept);
  }
}

/*=========================================================================*/
/*                                                                         */
/* CreateSegregateByFeatureWindow () - Creates and then displays the window*/
/*                        for getting segregate by text info from the user.*/
/*                                                                         */
/*=========================================================================*/

extern Int2 LIBCALLBACK CreateSegregateByMoleculeTypeWindow (Pointer data)
{
  GrouP               c;
  SegregateMolTypePtr smp;
  GrouP               g, k;
  GrouP               h;
  OMProcControlPtr    ompcp;
  StdEditorProcsPtr   sepp;
  WindoW              w;

  /* Check parameters and get a pointer to the current data */

  ompcp = (OMProcControlPtr) data;
  if (ompcp == NULL)
    return OM_MSG_RET_ERROR;

  if (ompcp->input_itemtype != OBJ_BIOSEQSET || ompcp->input_data == NULL) {
    Message (MSG_ERROR, "Must select Bioseq Set!");
    return OM_MSG_RET_ERROR;
  }

  /* Create a new window, and a struct */
  /* to pass around the data in.       */

  smp = (SegregateMolTypePtr) MemNew (sizeof (SegregateMolTypeData));
  if (smp == NULL)
    return OM_MSG_RET_ERROR;
  
  w = FixedWindow (-50, -33, -10, -10, "Segregate By Molecule Type",
	               StdCloseWindowProc);
		   
  SetObjectExtra (w, smp, CleanupSegregateFeatPage);
  smp->form = (ForM) w;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    smp->appmessage = sepp->handleMessages;
  }

  smp->input_entityID = ompcp->input_entityID;
  smp->input_itemID = ompcp->input_itemID;
  smp->input_itemtype = ompcp->input_itemtype;
  smp->target_set = (BioseqSetPtr)ompcp->input_data;

  /* Add the popup lists */

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 1, 0, NULL);

  StaticPrompt (g, "Segregate sequences with:", 0, dialogTextHeight,
	              programFont, 'l');
	k = HiddenGroup (g, 2, 0, NULL);
	smp->use_mol_type = CheckBox (k, "Molecule Type", EnableMolInfoPopups);
  SetObjectExtra (smp->use_mol_type, smp, NULL);
  smp->type_popup = PopupList (k, TRUE, NULL);
  InitEnumPopup (smp->type_popup, molinfo_biomol_alist, NULL);
  SetValue (smp->type_popup, 1);
  SetStatus (smp->use_mol_type, FALSE);
  Disable (smp->type_popup);
	k = HiddenGroup (g, 2, 0, NULL);
  smp->use_mol_class = CheckBox (k, "Molecule Class", EnableMolInfoPopups);
  SetObjectExtra (smp->use_mol_class, smp, NULL);
  smp->class_popup = PopupList (k, TRUE, NULL);
  InitEnumPopup (smp->class_popup, mol_class_alist, NULL);
  SetValue (smp->class_popup, 1);
  SetStatus (smp->use_mol_class, FALSE);
  Disable (smp->class_popup);

  /* Add Accept and Cancel buttons */

  c = HiddenGroup (h, 4, 0, NULL);
  smp->accept = DefaultButton (c, "Accept", SegregateByMoleculeType_Callback);
  SetObjectExtra (smp->accept, smp, NULL);
  Disable (smp->accept);
  PushButton (c, "Cancel", StdCancelButtonProc);

  /* Line things up nicely */

  AlignObjects (ALIGN_LEFT, (HANDLE) g, (HANDLE) c, (HANDLE) h, NULL);

  /* Display the window now */

  RealizeWindow (w);
  Show (w);
  Select (w);
  Select (smp->accept);
  Update ();
  return OM_MSG_RET_OK;
}


static CharPtr SaveOrReplaceStringCopy (ConvertFormPtr cfp, CharPtr str, CharPtr current)

{
  if (cfp == NULL) return NULL;
  if (StringHasNoText (str)) return current;
  AppendOrReplaceString (&current, str, &(cfp->replaceOldAsked),
                         &(cfp->doReplaceAll), &(cfp->use_semicolon));
  return current;
}

static void ConvertToGBQual (ConvertFormPtr cfp, SeqFeatPtr sfp)
{
  GBQualPtr gbqual, newqual;
  CharPtr   qualname;

  if (cfp == NULL || sfp == NULL) return;

  if (cfp->gb_qual_type < 1 || cfp->gb_qual_type > NumParseQualifiers) return;
  qualname = ParseQualifierList [ cfp->gb_qual_type - 1].name;
  for (gbqual = sfp->qual;
       gbqual != NULL && StringCmp(gbqual->qual, qualname) != 0 && gbqual->next != NULL;
       gbqual = gbqual->next)
  {}
  
  if (gbqual == NULL) {
    /* add new qual to beginning */
    newqual = GBQualNew();
    if (newqual != NULL) {
      newqual->qual = StringSave (qualname);
      newqual->val = StringSave (cfp->foundstr);
      sfp->qual = newqual;
    }
  } else if (StringCmp (gbqual->qual, qualname) == 0) {
    /* append to this qual */
    gbqual->val = SaveOrReplaceStringCopy (cfp, cfp->foundstr, gbqual->val);
  } else {
    /* append new qual to end */
    newqual = GBQualNew();
    if (newqual != NULL) {
      newqual->qual = StringSave (qualname);
      newqual->val = StringSave (cfp->foundstr);
      gbqual->next = newqual;
    }
  }

}  

static void ConvertAFeatureField (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  ConvertFormPtr  cfp;
  GBQualPtr       gbq;
  GeneRefPtr      grp;
  ProtRefPtr      prp;
  RnaRefPtr       rrp;
  SeqAnnotPtr     sap;
  SeqFeatPtr      sfp;
  ValNodePtr      vnp;
  SeqFeatXrefPtr  xref;
  CharPtr         cp;
  Int2            item_num;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  cfp = (ConvertFormPtr) mydata;
  if (cfp == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_GENE && cfp->type == GENE_TYPE) {
          grp = (GeneRefPtr) sfp->data.value.ptrvalue;
          if (grp != NULL) {
            switch (cfp->subtype) {
              case 1 :
                grp->locus = SaveOrReplaceStringCopy (cfp, cfp->foundstr, grp->locus);
                break;
              case 2 :
                grp->desc = SaveOrReplaceStringCopy (cfp, cfp->foundstr, grp->desc);
                break;
              case 3 :
                grp->allele = SaveOrReplaceStringCopy (cfp, cfp->foundstr, grp->allele);
                break;
              case 4 :
                grp->maploc = SaveOrReplaceStringCopy (cfp, cfp->foundstr, grp->maploc);
                break;
              case 5 :
                grp->locus_tag = SaveOrReplaceStringCopy (cfp, cfp->foundstr, grp->locus_tag);
                break;
              case 6 :
                if (! StringHasNoText (cfp->foundstr)) {
                  if (grp->syn != NULL) {
                    vnp = grp->syn;
                    cp = vnp->data.ptrvalue;
                    AppendOrReplaceString (&cp,
                                           cfp->foundstr,
                                           &(cfp->replaceOldAsked),
                                           &(cfp->doReplaceAll),
                                           &(cfp->use_semicolon));
                    vnp->data.ptrvalue = cp;
                  } else {
                    vnp = ValNodeNew (NULL);
                    if (vnp != NULL) {
                      vnp->next = grp->syn;
                      grp->syn = vnp;
                      vnp->data.ptrvalue = StringSave (cfp->foundstr);;
                    }
                  }
                }
                break;
              case 7 :
                sfp->comment = SaveOrReplaceStringCopy (cfp, cfp->foundstr, sfp->comment);
                break;
              default :
                break;
            }
          }
        } else if (sfp->data.choice == SEQFEAT_CDREGION &&
		   cfp->type == CDS_TYPE) {
          switch (cfp->subtype) {
            case 1 :
              sfp->comment = SaveOrReplaceStringCopy (cfp, cfp->foundstr, sfp->comment);
              break;
            case 2 :
              if (! StringHasNoText (cfp->foundstr)) {
                xref = SeqFeatXrefNew ();
                if (xref != NULL) {
                  grp = CreateNewGeneRef (cfp->foundstr, NULL, NULL, FALSE);
                  if (grp != NULL) {
                    xref->data.choice = SEQFEAT_GENE;
                    xref->data.value.ptrvalue = grp;
                    xref->next = sfp->xref;
                    sfp->xref = xref;
                  }
                }
              }
              break;
            case 3 :
              gbq = GBQualNew ();
              if (gbq != NULL) {
                /* BasicSeqEntryCleanup will convert to real sfp->dbxref */
                gbq->qual = StringSave ("db_xref");
                gbq->val = StringSave (cfp->foundstr);
                gbq->next = sfp->qual;
                sfp->qual = gbq;
              }
              break;
            default :
              break;
          }
        } else if (sfp->data.choice == SEQFEAT_PROT &&
		   cfp->type == PROT_TYPE) {
          prp = (ProtRefPtr) sfp->data.value.ptrvalue;
          if (prp != NULL) {
            switch (cfp->subtype) {
              case 1 :
                if (! StringHasNoText (cfp->foundstr)) {
                  if (prp->name != NULL) {
                    vnp = prp->name;
                    cp = vnp->data.ptrvalue;
                    AppendOrReplaceString (&cp,
                                           cfp->foundstr,
                                           &(cfp->replaceOldAsked),
                                           &(cfp->doReplaceAll),
                                           &(cfp->use_semicolon));
                    vnp->data.ptrvalue = cp;
                  } else {
                    vnp = ValNodeNew (NULL);
                    if (vnp != NULL) {
                      vnp->next = prp->name;
                      prp->name = vnp;
                      vnp->data.ptrvalue = StringSave (cfp->foundstr);
                    }
                  }
                }
                break;
              case 2 :
                prp->desc = SaveOrReplaceStringCopy (cfp, cfp->foundstr, prp->desc);
                break;
              case 3 :
                if (! StringHasNoText (cfp->foundstr)) {
                  if (prp->ec != NULL) {
                    vnp = prp->ec;
                    cp = vnp->data.ptrvalue;
                    AppendOrReplaceString (&cp,
                                           cfp->foundstr,
                                           &(cfp->replaceOldAsked),
                                           &(cfp->doReplaceAll),
                                           &(cfp->use_semicolon));
                    vnp->data.ptrvalue = cp;
                  } else {
                    vnp = ValNodeNew (NULL);
                    if (vnp != NULL) {
                      vnp->next = prp->ec;
                      prp->ec = vnp;
                      vnp->data.ptrvalue = StringSave (cfp->foundstr);
                    }
                  }
                }
                break;
              case 4 :
                if (! StringHasNoText (cfp->foundstr)) {
                  if (prp->activity != NULL) {
                    vnp = prp->activity;
                    cp = vnp->data.ptrvalue;
                    AppendOrReplaceString (&cp,
                                           cfp->foundstr,
                                           &(cfp->replaceOldAsked),
                                           &(cfp->doReplaceAll),
                                           &(cfp->use_semicolon));
                    vnp->data.ptrvalue = cp;
                  } else {
                    vnp = ValNodeNew (NULL);
                    if (vnp != NULL) {
                      vnp->next = prp->activity;
                      prp->activity = vnp;
                      vnp->data.ptrvalue = StringSave (cfp->foundstr);
                    }
                  }
                }
                break;
              case 5 :
                sfp->comment = SaveOrReplaceStringCopy (cfp, cfp->foundstr, sfp->comment);
                break;
              default :
                break;
            }
          }
        } else if (sfp->data.choice == SEQFEAT_RNA &&
		   cfp->type == RNA_TYPE) {
          rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
          if (rrp != NULL) {
            switch (cfp->subtype) {
              case 1 :
                if (rrp->ext.choice == 0 || rrp->ext.choice == 1) {
                  rrp->ext.choice = 1;
                  rrp->ext.value.ptrvalue = SaveOrReplaceStringCopy (cfp, cfp->foundstr, rrp->ext.value.ptrvalue);
                }
                break;
              case 2 :
                sfp->comment = SaveOrReplaceStringCopy (cfp, cfp->foundstr, sfp->comment);
                break;
              case 3 :
                if (! StringHasNoText (cfp->foundstr)) {
                  xref = SeqFeatXrefNew ();
                  if (xref != NULL) {
                    grp = CreateNewGeneRef (cfp->foundstr, NULL, NULL, FALSE);
                    if (grp != NULL) {
                      xref->data.choice = SEQFEAT_GENE;
                      xref->data.value.ptrvalue = grp;
                      xref->next = sfp->xref;
                      sfp->xref = xref;
                    }
                  }
                }
              default :
                break;
            }
          }
        } else if (cfp->type == IMPORT_FEAT_TYPE) {
          if (cfp->gb_qual_pup == NULL) {
            if (cfp->subtype == sfp->idx.subtype) {
              sfp->comment = SaveOrReplaceStringCopy (cfp, cfp->foundstr, sfp->comment);
            }
          } else {
            if (cfp->subtype == sfp->idx.subtype) {
              ConvertToGBQual (cfp, sfp);
            }
          }
        } else if (cfp->type == FEATURE_NOTE_TYPE) {
          for (vnp = cfp->feature_list, item_num = 1;
               vnp != NULL;
               vnp = vnp->next, item_num++) {
            if (GetItemStatus (cfp->subtarget [FEATURE_NOTE_TYPE], item_num)
                && (vnp->choice == 255 || vnp->choice == sfp->idx.subtype)) {
              sfp->comment = SaveOrReplaceStringCopy (cfp, cfp->foundstr,
				    sfp->comment);
            }
          }
        }

        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
}

static void ConvertASourceField (BioSourcePtr biop, ConvertFormPtr cfp)

{
  OrgModPtr     mod;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  SubSourcePtr  ssp;
  OrgModPtr     tmpmod;
  SubSourcePtr  tmpssp;
  CharPtr       tmptaxname;

  if (biop == NULL || cfp == NULL) return;
  switch (cfp->type) {
    case BIOSOURCE_TYPE :
      orp = biop->org;
      if (orp == NULL) return;
      switch (cfp->subtype) {
        case ORGREF_SCI_NAME_FIELD :
		  tmptaxname = StringSave (orp->taxname);
          SetTaxNameAndRemoveTaxRef (orp, SaveOrReplaceStringCopy (cfp, cfp->foundstr, tmptaxname));
          break;
        case ORGREF_COMMON_NAME_FIELD :
          orp->common = SaveOrReplaceStringCopy (cfp, cfp->foundstr, orp->common);
          break;
        case ORGREF_LINEAGE_FIELD :
          onp = orp->orgname;
          if (onp == NULL) {
            onp = OrgNameNew ();
            orp->orgname = onp;
          }
          if (onp != NULL) {
            onp->lineage = SaveOrReplaceStringCopy (cfp, cfp->foundstr, onp->lineage);
          }
          break;
        case ORGREF_DIVISION_FIELD :
          onp = orp->orgname;
          if (onp == NULL) {
            onp = OrgNameNew ();
            orp->orgname = onp;
          }
          if (onp != NULL) {
            onp->div = SaveOrReplaceStringCopy (cfp, cfp->foundstr, onp->div);
          }
          break;
        default :
          break;
      }
      break;
    case ORGMOD_SUBSOURCE_TYPE :
      if (cfp->subtype < 1000) {  /* Orgmod Notes */
	orp = biop->org;
	if (orp == NULL) {
	  orp = OrgRefNew ();
	  biop->org = orp;
	}
	if (orp != NULL) {
	  onp = orp->orgname;
	  if (onp == NULL) {
	    onp = OrgNameNew ();
	    orp->orgname = onp;
	  }
	  if (onp != NULL) {
	    mod = onp->mod;
	    while (mod != NULL && mod->subtype != cfp->subtype) {
	      mod = mod->next;
	    }
	    if (mod == NULL) {
	      mod = OrgModNew ();
	      if (onp->mod == NULL) {
		onp->mod = mod;
	      } else {
		tmpmod = onp->mod;
		while (tmpmod->next != NULL) {
		  tmpmod = tmpmod->next;
		}
		tmpmod->next = mod;
	      }
	    }
	    if (mod != NULL) {
	      mod->subtype = cfp->subtype;
	      mod->subname = SaveOrReplaceStringCopy (cfp,
						      cfp->foundstr,
						      mod->subname);
	    }
	  }
	}
      }
      else {  /* SubSource Notes */
	ssp = biop->subtype;
	while (ssp != NULL && ssp->subtype != (cfp->subtype - 1000)) {
	  ssp = ssp->next;
	}
	if (ssp == NULL) {
	  ssp = SubSourceNew ();
	  if (biop->subtype == NULL) {
	    biop->subtype = ssp;
	  } else {
	    tmpssp = biop->subtype;
	    while (tmpssp->next != NULL) {
	      tmpssp = tmpssp->next;
	    }
	    tmpssp->next = ssp;
	  }
	}
	if (ssp != NULL) {
	  ssp->subtype = (cfp->subtype - 1000);
	  ssp->name = SaveOrReplaceStringCopy (cfp, cfp->foundstr, ssp->name);
	}
      }
      break;
  default:
    break;
  }
}

static void ConvertToDefLine (SeqDescrPtr sdp, Pointer userdata)
{
  ConvertFormPtr cfp;

  /* Check parameters */

  if (sdp == NULL || (cfp = (ConvertFormPtr) userdata) == NULL 
      || sdp->choice != Seq_descr_title)
    return;

   sdp->data.ptrvalue = SaveOrReplaceStringCopy (cfp, cfp->foundstr, sdp->data.ptrvalue);

}

static void FoundStringForConversion (Uint2 entityID, SeqEntryPtr sep, ConvertFormPtr cfp, CharPtr str)

{
  BioSourcePtr  biop;
  OrgRefPtr     orp;
  ValNodePtr    vnp;

  if (sep == NULL || cfp == NULL) return;

  cfp->foundstr = str;
  switch (cfp->type) {
    case GENE_TYPE :
    case CDS_TYPE  :
    case PROT_TYPE :
    case RNA_TYPE  :
      SeqEntryExplore (sep, (Pointer) cfp, ConvertAFeatureField);
      break;
    case BIOSOURCE_TYPE :
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      if (biop == NULL && cfp->subtype == 1) {
        biop = BioSourceNew ();
        if (biop == NULL) return;
        orp = OrgRefNew ();
        biop->org = orp;
        vnp = CreateNewDescriptor (sep, Seq_descr_source);
        if (vnp != NULL) {
          vnp->data.ptrvalue = (Pointer) biop;
        }
      }
      ConvertASourceField (biop, cfp);
      break;
    case ORGMOD_SUBSOURCE_TYPE :
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      if (biop == NULL && cfp->parsetype == PARSETYPE_LOCALID
        && IS_Bioseq (sep))
      {
        sep = GetBestTopParentForData (cfp->input_entityID,
                                       sep->data.ptrvalue);
        SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      }
      ConvertASourceField (biop, cfp);
      break;
    case IMPORT_FEAT_TYPE :
      SeqEntryExplore (sep, (Pointer) cfp, ConvertAFeatureField);
      break;
    case FEATURE_NOTE_TYPE :
      SeqEntryExplore (sep, (Pointer) cfp, ConvertAFeatureField);
      break;
    case DEFLINE_TYPE :
      VisitDescriptorsInSep (sep, (Pointer) cfp, ConvertToDefLine);
      break;
    default:
      break;
  }
}

typedef struct commentconvertdata {
  ConvertFormPtr cfp;
  SeqEntryPtr    sep;
  Uint2          entityID;
} CommentConvertData, PNTR CommentConvertPtr;

static CharPtr FindStringForConversion (CharPtr string, ConvertFormPtr cfp)
{
  CharPtr           lft = NULL;
  Boolean           okay;
  CharPtr           rgt = NULL;
  CharPtr           tmp;

  if (string == NULL || *string == '\0') return NULL;

  CompressSpaces (string);
  okay = TRUE;
  tmp = string;
  if (cfp->leftstr != NULL && cfp->leftstr [0] != '\0') {
    lft = SearchForString (string, cfp->leftstr, FALSE, FALSE);
    if (lft != NULL) {
      if (! cfp->includeleft) {
        lft += StringLen (cfp->leftstr);
      }
      tmp = lft;
    } else {
      okay = FALSE;
    }
  }
  if (cfp->rightstr != NULL && cfp->rightstr [0] != '\0') {
    rgt = SearchForString (tmp, cfp->rightstr, FALSE, FALSE);
    if (rgt != NULL) {
      if (cfp->includeright) {
        rgt += StringLen (cfp->rightstr);
      }
      *rgt = '\0';
    } else {
      okay = FALSE;
    }
  }
  else if (TRUE == cfp->endQuotesFlag) {
    rgt = SearchForString (tmp, "\"", FALSE, FALSE);
    if (rgt != NULL) {
      if (cfp->includeright) {
        rgt += StringLen (cfp->rightstr);
      }
      *rgt = '\0';
    } else {
      okay = FALSE;
    }
  }
  if (okay) {
    if (lft != NULL) {
      lft = tmp;
    } else {
      lft = string;
    }
    if (! StringHasNoText (lft)) {
      return lft;
    }
  }
  return NULL;
}


static void SearchAndExciseText (CharPtr PNTR strptr, ConvertFormPtr cfp)

{
  Char     ch;
  CharPtr  lft = NULL;
  CharPtr  rgt = NULL;
  CharPtr  string;
  CharPtr  tmp;

  if (strptr == NULL || cfp == NULL) return;
  string = *strptr;
  if (string == NULL) return;
  tmp = string;
  if (cfp->leftstr != NULL && cfp->leftstr [0] != '\0') {
    lft = SearchForString (string, cfp->leftstr, FALSE, FALSE);
    if (lft == NULL) return;
    if (! cfp->includeleft) {
      lft += StringLen (cfp->leftstr);
    }
    tmp = lft;
  }
  if (cfp->rightstr != NULL && cfp->rightstr [0] != '\0') {
    rgt = SearchForString (tmp, cfp->rightstr, FALSE, FALSE);
    if (rgt == NULL) return;
    if (cfp->includeright) {
      rgt += StringLen (cfp->rightstr);
    }
  }

  if (lft == rgt)   /* No text to delete */
    return;

  if (lft != NULL) {
    *tmp = '\0';
    lft = tmp;
  } else {
    lft = string;
  }
  if (rgt != NULL) {
    ch = *rgt;
    while (ch != '\0') {
      *lft = ch;
      rgt++;
      lft++;
      ch = *rgt;
    }
    *lft = '\0';
  }
}

static void RemoveAFeatureText (SeqEntryPtr sep,
				Pointer     mydata,
				Int4        index,
				Int2        indent)
{
  BioseqPtr       bsp;
  BioseqSetPtr    bssp;
  ConvertFormPtr  cfp;
  GBQualPtr       gbqp;
  GeneRefPtr      grp;
  ProtRefPtr      prp;
  RnaRefPtr       rrp;
  SeqAnnotPtr     sap;
  SeqFeatPtr      sfp;
  CharPtr PNTR    strp;
  ValNodePtr      vnp;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  cfp = (ConvertFormPtr) mydata;
  if (cfp == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sap = bsp->annot;
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sap = bssp->annot;
  } else return;
  while (sap != NULL) {
    if (sap->type == 1) {
      sfp = (SeqFeatPtr) sap->data;
      while (sfp != NULL) {
        if (sfp->data.choice == SEQFEAT_GENE && cfp->type == GENE_TYPE) {
          grp = (GeneRefPtr) sfp->data.value.ptrvalue;
          if (grp != NULL) {
            switch (cfp->subtype) {
              case 1 :
                SearchAndExciseText (&grp->locus, cfp);
                break;
              case 2 :
                SearchAndExciseText (&grp->desc, cfp);
                break;
              case 3 :
                SearchAndExciseText (&grp->allele, cfp);
                break;
              case 4 :
                SearchAndExciseText (&grp->maploc, cfp);
                break;
              case 5 :
                SearchAndExciseText (&grp->locus_tag, cfp);
                break;
              case 6 :
                for (vnp = grp->syn; vnp != NULL; vnp = vnp->next) {
                  strp = (CharPtr PNTR) &vnp->data.ptrvalue;
                  SearchAndExciseText (strp, cfp);
                }
                break;
              case 7 :
                SearchAndExciseText (&sfp->comment, cfp);
                break;
              default :
                break;
            }
          }
        } else if (sfp->data.choice == SEQFEAT_CDREGION &&
		   cfp->type == CDS_TYPE) {
          switch (cfp->subtype) {
            case CDS_COMMENT :
              SearchAndExciseText (&sfp->comment, cfp);
              break;
            case CDS_GENE_XREF :
              break;
            default :
              break;
          }
        } else if (sfp->data.choice == SEQFEAT_PROT &&
		   cfp->type == PROT_TYPE) {
          prp = (ProtRefPtr) sfp->data.value.ptrvalue;
          if (prp != NULL) {
            switch (cfp->subtype) {
              case 1 :
                for (vnp = prp->name; vnp != NULL; vnp = vnp->next) {
                  strp = (CharPtr PNTR) &vnp->data.ptrvalue;
                  SearchAndExciseText (strp, cfp);
                }
                break;
              case 2 :
                SearchAndExciseText (&prp->desc, cfp);
                break;
              case 3 :
                for (vnp = prp->ec; vnp != NULL; vnp = vnp->next) {
                  strp = (CharPtr PNTR) &vnp->data.ptrvalue;
                  SearchAndExciseText (strp, cfp);
                }
                break;
              case 4 :
                for (vnp = prp->activity; vnp != NULL; vnp = vnp->next) {
                  strp = (CharPtr PNTR) &vnp->data.ptrvalue;
                  SearchAndExciseText (strp, cfp);
                }
                break;
              case 5 :
                SearchAndExciseText (&sfp->comment, cfp);
                break;
              default :
                break;
            }
          }
        } else if (sfp->data.choice == SEQFEAT_RNA &&
		   cfp->type == RNA_TYPE) {
          rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
          if (rrp != NULL) {
            switch (cfp->subtype) {
              case 1 :
                if (rrp->ext.choice == 0 || rrp->ext.choice == 1) {
                  rrp->ext.choice = 1;
                  strp = (CharPtr PNTR) &rrp->ext.value.ptrvalue;
                  SearchAndExciseText (strp, cfp);
                }
                break;
              case 2 :
                SearchAndExciseText (&sfp->comment, cfp);
                break;
              case 3 :
              default :
                break;
            }
          }
        } else if (sfp->data.choice == SEQFEAT_IMP &&
		   cfp->type == IMPORT_FEAT_TYPE) {
          switch (cfp->subtype) {
            case IMPORT_GBQUAL_FIELD :
	      gbqp = sfp->qual;
	      while (NULL != gbqp)
		{
		  if (NULL != gbqp->val)
		    SearchAndExciseText (&gbqp->val, cfp);
		  gbqp = gbqp->next;
		}
              break;
            case IMPORT_COMMENT_FIELD :
              SearchAndExciseText (&sfp->comment, cfp);
              break;
            default :
              break;
          }
	}
        sfp = sfp->next;
      }
    }
    sap = sap->next;
  }
}

static void RemoveASourceText (BioSourcePtr biop, ConvertFormPtr cfp)

{
  OrgModPtr     mod;
  OrgNamePtr    onp;
  OrgRefPtr     orp;
  SubSourcePtr  ssp;
  CharPtr       tmp_str;

  if (biop == NULL || cfp == NULL) return;
  switch (cfp->type) {
    case BIOSOURCE_TYPE :
      orp = biop->org;
      if (orp == NULL) return;
      switch (cfp->subtype) {
        case ORGREF_SCI_NAME_FIELD :
          tmp_str = StringSave (orp->taxname);
          SearchAndExciseText (&tmp_str, cfp);
          SetTaxNameAndRemoveTaxRef (orp, tmp_str);
          break;
        case ORGREF_COMMON_NAME_FIELD :
          SearchAndExciseText (&orp->common, cfp);
          break;
        case ORGREF_LINEAGE_FIELD :
          onp = orp->orgname;
          if (onp == NULL) {
            onp = OrgNameNew ();
            orp->orgname = onp;
          }
          if (onp != NULL) {
            SearchAndExciseText (&onp->lineage, cfp);
          }
          break;
        case ORGREF_DIVISION_FIELD :
          onp = orp->orgname;
          if (onp == NULL) {
            onp = OrgNameNew ();
            orp->orgname = onp;
          }
          if (onp != NULL) {
            SearchAndExciseText (&onp->div, cfp);
          }
          break;
        default:
          break;
      }
      break;
    case ORGMOD_SUBSOURCE_TYPE :
      if (cfp->subtype < 1000) {  /* Orgmod Note */
	orp = biop->org;
	if (orp == NULL) {
	  orp = OrgRefNew ();
	  biop->org = orp;
	}
	if (orp != NULL) {
	  onp = orp->orgname;
	  if (onp == NULL) {
	    onp = OrgNameNew ();
	    orp->orgname = onp;
	  }
	  if (onp != NULL) {
	    mod = onp->mod;
	    while (mod != NULL && mod->subtype != cfp->subtype) {
	      mod = mod->next;
	    }
	    if (mod != NULL) {
	      SearchAndExciseText (&mod->subname, cfp);
	    }
	  }
	}
      }
      else {  /* subsource note */
	ssp = biop->subtype;
	while (ssp != NULL && ssp->subtype != (cfp->subtype - 1000)) {
	  ssp = ssp->next;
	}
	if (ssp != NULL) {
	  SearchAndExciseText (&ssp->name, cfp);
	}
      }
      break;
    default:
      break;
  }
}

static Boolean LIBCALLBACK DeflineRemInTxt_DoBioseq (BioseqPtr bsp,
				   SeqMgrBioseqContextPtr context)
{
  ConvertFormPtr cfp;
  CharPtr        title;

  /* Check parameters */

  if (NULL == context)
    return FALSE;

  cfp = (ConvertFormPtr) context->userdata;
  if (NULL == cfp)
    return FALSE;

  /* Remove requested text from the defline */

  title = BioseqGetTitle (bsp);
  SearchAndExciseText (&title, cfp);

  /* Return TRUE to continue on to the next Bioseq */

  return TRUE;
}

static void DoOneRemoveText (Uint2          entityID,
			     SeqEntryPtr    sep,
			     ConvertFormPtr cfp,
			     MonitorPtr     mon)

{
  BioSourcePtr  biop;
  BioseqSetPtr  bssp;
  Char          str [64];

  if (sep == NULL || cfp == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        DoOneRemoveText (entityID, sep, cfp, mon);
      }
      return;
    }
  }
  (cfp->index)++;
  if (mon != NULL) {
    sprintf (str, "Processing component %d", (int) cfp->index);
    MonitorStrValue (mon, str);
  }
  switch (cfp->type) {
    case GENE_TYPE :
    case CDS_TYPE :
    case PROT_TYPE :
    case RNA_TYPE :
      SeqEntryExplore (sep, (Pointer) cfp, RemoveAFeatureText);
      break;
    case BIOSOURCE_TYPE :
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      RemoveASourceText (biop, cfp);
      break;
    case ORGMOD_SUBSOURCE_TYPE :
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      RemoveASourceText (biop, cfp);
      break;
    case IMPORT_FEAT_TYPE :
      SeqEntryExplore (sep, (Pointer) cfp, RemoveAFeatureText);
      break;
    default:
      break;
  }
}

static void DoRemoveTextProc (ButtoN b)

{
  ConvertFormPtr  cfp;
  MonitorPtr      mon;
  SeqEntryPtr     sep;
  UIEnum          val;

  /* Get the current sequence and associated data */

  cfp = (ConvertFormPtr) GetObjectExtra (b);
  if (cfp == NULL || cfp->input_entityID == 0)
    return;

  sep = GetTopSeqEntryForEntityID (cfp->input_entityID);
  if (sep == NULL)
    return;

  /* Hide the window and set the 'working' cursor */

  Hide (cfp->form);
  WatchCursor ();
  Update ();

  /* Get the feature and subfeature types */

  GetEnumPopup (cfp->target, cfp->target_alist, &val);
  if (0 == val) {
    ArrowCursor ();
    Update ();
    Remove (cfp->form);
    return;
  }

  cfp->type = (Int2) val;

  if (DEFLINE_TYPE != cfp->type) {
    GetEnumPopup (cfp->subtarget [cfp->type], cfp->alists [cfp->type], &val);
    if (0 == val) {
      ArrowCursor ();
      Update ();
      Remove (cfp->form);
      return;
    }
    cfp->subtype = (Int2) val;
  }

  /* Get the before (left) and after (right) strings */

  cfp->leftstr = SaveStringFromTextNoStripSpaces (cfp->atleft);
  cfp->rightstr = SaveStringFromTextNoStripSpaces (cfp->atright);
  TrimOffEndQuotes (cfp->leftstr, TRIM_LEFT_END);
  TrimOffEndQuotes (cfp->rightstr, TRIM_RIGHT_END);

  /* Get the states of the radio buttons */

  cfp->includeleft = (Boolean) (GetValue (cfp->leftbehav) == 2);
  cfp->includeright = (Boolean) (GetValue (cfp->rightbehav) == 2);

  /* Do actual work of removing text */

  mon = MonitorStrNewEx ("Removing Text From String", 20, FALSE);
  cfp->index = 0;
  if (cfp->type == DEFLINE_TYPE)
    SeqMgrExploreBioseqs (cfp->input_entityID, NULL, cfp,
			  DeflineRemInTxt_DoBioseq, TRUE, TRUE, TRUE);
  else
    DoOneRemoveText (cfp->input_entityID, sep, cfp, mon);
  MonitorFree (mon);
  
  /* Clean up and exit */

  cfp->leftstr = MemFree (cfp->leftstr);
  cfp->rightstr = MemFree (cfp->rightstr);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (cfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, cfp->input_entityID, 0, 0);
  if (GetStatus (cfp->leaveDlgUp))
  {
  	Show (cfp->form);
  }
  else
  {
    Remove (cfp->form);  	
  }
}

static void ConvertMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

static void CleanupParseForm (GraphiC g, VoidPtr data)

{
  ConvertFormPtr  cfp;

  cfp = (ConvertFormPtr) data;
  if (cfp != NULL) {
    if (cfp->alists [9] != NULL) {
      FreeEnumFieldAlist (cfp->alists [9]);
    }
  }
  StdCleanupFormProc (g, data);
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_SetAcceptButtonStatus () -- Enable/Disable the Accept     */
/*                                       button depending on the       */
/*                                       condition of other window     */
/*                                       objects.                      */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void RemOutTxt_SetAcceptButtonStatus (ConvertFormPtr cfp)
{
  Boolean beforeChecked;
  Boolean afterChecked;
  UIEnum  val;

  beforeChecked = GetStatus (cfp->removeBefore);
  afterChecked  = GetStatus (cfp->removeAfter);

  /* If both before and after text are disabled */
  /* then disable the accept button.            */

  if ((FALSE == beforeChecked) && (FALSE == afterChecked)) {
    SafeDisable (cfp->accept);
    return;
  }

  /* If the before field is enabled but empty */
  /* then disable the accept button.          */

  if ((TRUE == beforeChecked) && (TextHasNoText (cfp->atleft))) {
    SafeDisable (cfp->accept);
    return;
  }

  /* If the before field is enabled but empty */
  /* then disable the accept button.          */

  if ((TRUE == afterChecked) && (TextHasNoText (cfp->atright))) {
    SafeDisable (cfp->accept);
    return;
  }

  /* Disable if a target has not been selected */

  if (!GetEnumPopup (cfp->target, cfp->target_alist, &val)) {
    SafeDisable (cfp->accept);
    return;
  }

  if (val <= 0) {
    SafeDisable (cfp->accept);
    return;
  }

  /* Disable if a subtarget has not been selected */
  /* unless the selected target is DefLine.       */

  cfp->type = (Int2) val;
  if (cfp->type != DEFLINE_TYPE) {
  
    if (!GetEnumPopup (cfp->subtarget [cfp->type],
		       cfp->alists [cfp->type], &val)) {
      SafeDisable (cfp->accept);
      return;
    }
    
    if (val <= 0) {
      SafeDisable (cfp->accept);
      return;
    }
  }

  /* If we made it to here then we've met */
  /* all conditions for accepting.        */

  SafeEnable (cfp->accept);
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutText_BeforeCheckBoxCallback () -- Called when the CheckBox    */
/*                                         for the 'Before' text field */
/*                                         is clicked on.              */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void RemOutTxt_BeforeCheckBoxCallback (ButtoN b)
{
  ConvertFormPtr   cfp;

  cfp = (ConvertFormPtr) GetObjectExtra (b);
  if (NULL == cfp)
    return;

  if (GetStatus (b)) {
    SafeEnable (cfp->atleft);
    SafeEnable (cfp->includeBeforeGroup);
  } else {
    SafeDisable (cfp->atleft);
    SafeDisable (cfp->includeBeforeGroup);
  }

  RemOutTxt_SetAcceptButtonStatus (cfp);
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_AfterCheckBoxCallback () -- Called when the CheckBox for  */
/*                                       the 'After' text field is     */
/*                                       clicked on.                   */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void RemOutTxt_AfterCheckBoxCallback (ButtoN b)
{
  ConvertFormPtr   cfp;

  cfp = (ConvertFormPtr) GetObjectExtra (b);
  if (NULL == cfp)
    return;

  if (GetStatus (b)) {
    SafeEnable (cfp->atright);
    SafeEnable (cfp->includeAfterGroup);
  } else {
    SafeDisable (cfp->atright);
    SafeDisable (cfp->includeAfterGroup);
  }

  RemOutTxt_SetAcceptButtonStatus (cfp);
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_SearchAndExcise () -- Removes all text from a string that */
/*                                 does not match a specified substring*/
/*                                                                     */
/*---------------------------------------------------------------------*/

static void RemOutTxt_SearchAndExcise (CharPtr        sourceStr,
				       ConvertFormPtr cfp)
{
  CharPtr  leftEnd = NULL;
  CharPtr  rightEnd = NULL;
  Int4     strLen;
  Int4     i;

  /* Check parameters */

  if (cfp == NULL)
    return;

  if ((NULL == cfp->leftstr) && (NULL == cfp->rightstr))
    return;

  if (sourceStr == NULL)
    return;

  /* Find the left end of the string */

  if (cfp->leftstr != NULL && cfp->leftstr [0] != '\0') {
    leftEnd = SearchForString (sourceStr, cfp->leftstr, FALSE, FALSE);
    if (NULL == leftEnd)  /* String not found */
    {
      if (DO_NOTHING == cfp->ifNotFound)
      {
	return;
      }
      else
      {
	sourceStr [0] = '\0';
	return;
      }
    }
    if (cfp->includeleft)
      leftEnd += StringLen (cfp->leftstr);
  }
  else
    leftEnd = sourceStr;

  /* Find the right end of the string */

  if (cfp->rightstr != NULL && cfp->rightstr [0] != '\0') {
    rightEnd = SearchForString (leftEnd, cfp->rightstr, FALSE, FALSE);
    if (NULL == rightEnd) /* String not found */
    {
      if (DO_NOTHING == cfp->ifNotFound)
      {
	return;
      }
      else
      {
	sourceStr [0] = '\0';
	return;
      }
    }
    if (!cfp->includeright)
      rightEnd += StringLen (cfp->rightstr);
  }
  else
    rightEnd = sourceStr + StringLen(sourceStr);

  /* End the original string at rightEnd and  */
  /* then move everything starting at leftEnd */
  /* to the beginning of the original.        */

  rightEnd[0] = '\0';
  if (leftEnd == sourceStr)
    return;

  strLen = StringLen (leftEnd);

  for (i = 0; i <= strLen; i++)
    sourceStr[i] = leftEnd[i];

  sourceStr[i] = '\0';
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_DoProt () -- Does the requested search and delete         */
/*                        on one protein feature.                      */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK RemOutTxt_DoProt (SeqFeatPtr sfp,
				 SeqMgrFeatContextPtr context)
{
  ConvertFormPtr cfp;
  ProtRefPtr     prp;
  ValNodePtr     vnp;
  CharPtr        str;

  /* Check parameters */

  cfp = (ConvertFormPtr) context->userdata;
  prp = (ProtRefPtr) sfp->data.value.ptrvalue;

  if (NULL == prp)
    return TRUE;

  /* Do the removal on the requested field */

  switch (cfp->subtype) {
  case PROT_NAME_FIELD :
    for (vnp = prp->name; vnp != NULL; vnp = vnp->next) {
      str = (CharPtr) vnp->data.ptrvalue;
      RemOutTxt_SearchAndExcise (str, cfp);
    }
    break;
  case PROT_DESCRIPTION_FIELD :
    RemOutTxt_SearchAndExcise (prp->desc, cfp);
    break;
  case PROT_ECNUM_FIELD :
    for (vnp = prp->ec; vnp != NULL; vnp = vnp->next) {
      str = (CharPtr) vnp->data.ptrvalue;
      RemOutTxt_SearchAndExcise (str, cfp);
    }
    break;
  case PROT_ACTIVITY_FIELD :
    for (vnp = prp->activity; vnp != NULL; vnp = vnp->next) {
      str = (CharPtr) vnp->data.ptrvalue;
      RemOutTxt_SearchAndExcise (str, cfp);
    }
    break;
  case PROT_COMMENT_FIELD :
    RemOutTxt_SearchAndExcise (sfp->comment, cfp);
    break;
  default :
    break;
  }

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_DoCDS () -- Does the requested search and delete on one   */
/*                       CDS feature.                                  */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK RemOutTxt_DoCDS (SeqFeatPtr sfp,
				SeqMgrFeatContextPtr context)
{
  ConvertFormPtr cfp;

  cfp = (ConvertFormPtr) context->userdata;

  switch (cfp->subtype) {
  case CDS_COMMENT :
    RemOutTxt_SearchAndExcise (sfp->comment, cfp);
    break;
  default:
    break;
  }

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_DoRNA () -- Does the requested search and delete on one   */
/*                       RNA feature.                                  */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK RemOutTxt_DoRNA (SeqFeatPtr sfp,
				SeqMgrFeatContextPtr context)
{
  ConvertFormPtr  cfp;
  RnaRefPtr       rrp;
  CharPtr         str;

  /* Check parameters */

  cfp = (ConvertFormPtr) context->userdata;
  rrp = (RnaRefPtr) sfp->data.value.ptrvalue;

  if (NULL == rrp)
    return TRUE;

  /* Do the removal on the requested field */

  switch (cfp->subtype) {
  case RNA_NAME_FIELD :
    if (rrp->ext.choice == 0 || rrp->ext.choice == 1) {
      rrp->ext.choice = 1;
      str = (CharPtr) rrp->ext.value.ptrvalue;
      RemOutTxt_SearchAndExcise (str, cfp);
    }
    break;
  case RNA_COMMENT_FIELD :
    RemOutTxt_SearchAndExcise (sfp->comment, cfp);
    break;
  default :
    break;
  }

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_DoOrgRef () -- Does the requested search and delete on    */
/*                          one OrgRef.                                */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void RemOutTxt_DoOrgRef (OrgRefPtr      orp,
				ConvertFormPtr cfp)
{
  OrgNamePtr      onp;

  switch (cfp->subtype) {
    case ORGREF_SCI_NAME_FIELD :
      RemOutTxt_SearchAndExcise (orp->taxname, cfp);
      break;
    case ORGREF_COMMON_NAME_FIELD :
      RemOutTxt_SearchAndExcise (orp->common, cfp);
      break;
    case ORGREF_LINEAGE_FIELD :
      onp = orp->orgname;
      if (onp != NULL)
	RemOutTxt_SearchAndExcise (onp->lineage, cfp);
      break;
    case ORGREF_DIVISION_FIELD :
      onp = orp->orgname;
      if (onp != NULL)
	RemOutTxt_SearchAndExcise (onp->div, cfp);
      break;
    default:
      break;
  }
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_DoBioSourceFeat () -- Does the requested search and       */
/*                                 delete on one orgref feature.       */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK RemOutTxt_DoBioSourceFeat (SeqFeatPtr sfp,
					  SeqMgrFeatContextPtr context)
				      
{
  BioSourcePtr    biop;
  ConvertFormPtr  cfp;
  OrgRefPtr       orp;

  /* Check parameters */

  cfp = (ConvertFormPtr) context->userdata;
  if (NULL == cfp)
    return FALSE;

  if (NULL == sfp)
    return FALSE;

  biop = (BioSourcePtr) sfp->data.value.ptrvalue;
  if (NULL == biop)
    return FALSE;
 
  orp = biop->org;
  if (NULL == orp)
    return TRUE;

  /* Do the removal on the requested field */
  /* in the BioSource OrgRef.              */

  RemOutTxt_DoOrgRef (orp, cfp);

  /* Return TRUE to continue to next descriptor */

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_DoBioSourceDescr () -- Does the requested search and      */
/*                                  delete on one BioSource descriptor.*/
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK RemOutTxt_DoBioSourceDescr (SeqDescrPtr sdp,
					   SeqMgrDescContextPtr context)
				      
{
  BioSourcePtr    biop;
  ConvertFormPtr  cfp;
  OrgRefPtr       orp;

  /* Check parameters */

  cfp = (ConvertFormPtr) context->userdata;
  if (NULL == cfp)
    return FALSE;

  if (NULL == sdp)
    return FALSE;

  biop = (BioSourcePtr) sdp->data.ptrvalue;
  if (NULL == biop)
    return FALSE;
 
  orp = biop->org;
  if (NULL == orp)
    return TRUE;

  /* Do the removal on the requested field */
  /* in the BioSource OrgRef.              */

  RemOutTxt_DoOrgRef (orp, cfp);

  /* Return TRUE to continue to next descriptor */

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_DoOMSS () -- Does the requested search and delete on      */
/*                        one orgmod or subsource.                     */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean RemOutTxt_DoOMSS (BioSourcePtr biop,
				 ConvertFormPtr cfp)
				      
{
  OrgRefPtr       orp;
  OrgNamePtr      onp;
  OrgModPtr       omp;
  SubSourcePtr    ssp;

  /* Either do the removal on the requested */
  /* orgmod subtype ...                     */

  if (cfp->subtype < 1000) {
    orp = biop->org;
    if (NULL == orp)
      return TRUE;
    onp = orp->orgname;
    if (NULL == onp)
      return TRUE;
    omp = onp->mod;
    while (omp != NULL && omp->subtype != cfp->subtype)
      omp = omp->next;
    if (omp != NULL)
	RemOutTxt_SearchAndExcise (omp->subname, cfp);
  }

  /* ... or do it on the requested subsource subtype */

  else {
    ssp = biop->subtype;
    while (ssp != NULL) {
      if (ssp->subtype == (cfp->subtype - 1000))
	RemOutTxt_SearchAndExcise (ssp->name, cfp);
      ssp = ssp->next;
    }
  }

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_DoOMSSFeat () -- Does the requested search and delete on  */
/*                            one orgmod or subsource feature.         */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK RemOutTxt_DoOMSSFeat (SeqFeatPtr sfp,
				     SeqMgrFeatContextPtr context)
				      
{
  BioSourcePtr    biop;
  ConvertFormPtr  cfp;

  /* Check parameters */

  cfp = (ConvertFormPtr) context->userdata;
  if (NULL == cfp)
    return FALSE;

  if (NULL == sfp)
    return FALSE;

  biop = (BioSourcePtr) sfp->data.value.ptrvalue;
  if (NULL == biop)
    return FALSE;

  /* Check the orgmod or subsource fields of the BioSource */

  RemOutTxt_DoOMSS (biop, cfp);
  
  /* Return TRUE to continue to next descriptor */

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_DoOMSSDescr () -- Does the requested search and           */
/*                             delete on one Orgmod or SubSource       */
/*                             descriptor.                             */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK RemOutTxt_DoOMSSDescr (SeqDescrPtr sdp,
				      SeqMgrDescContextPtr context)
				      
{
  BioSourcePtr    biop;
  ConvertFormPtr  cfp;

  /* Check parameters */

  cfp = (ConvertFormPtr) context->userdata;
  if (NULL == cfp)
    return FALSE;

  if (NULL == sdp)
    return FALSE;

  biop = (BioSourcePtr) sdp->data.ptrvalue;
  if (NULL == biop)
    return FALSE;
 
  /* Check the orgmod or subsource fields of the BioSource */

  RemOutTxt_DoOMSS (biop, cfp);
  
  /* Return TRUE to continue to next descriptor */

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_DoGene () -- Does the requested search and delete on one  */
/*                        gene feature.                                */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK RemOutTxt_DoGene (SeqFeatPtr sfp,
				 SeqMgrFeatContextPtr context)
{
  ConvertFormPtr  cfp;
  GeneRefPtr      grp;
  ValNodePtr      vnp;
  CharPtr         str;

  /* Check parameters */

  cfp = (ConvertFormPtr) context->userdata;
  if (NULL == cfp)
    return FALSE;

  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (NULL == grp)
    return TRUE;

  /* Do the removal on the requested field */

  switch (cfp->subtype) {
    case GENE_LOCUS_FIELD :
      RemOutTxt_SearchAndExcise (grp->locus, cfp);
      break;
    case GENE_DESCRIPTION_FIELD :
      RemOutTxt_SearchAndExcise (grp->desc, cfp);
      break;
    case GENE_ALLELE_FIELD :
      RemOutTxt_SearchAndExcise (grp->allele, cfp);
      break;
    case GENE_MAPLOC_FIELD :
      RemOutTxt_SearchAndExcise (grp->maploc, cfp);
      break;
    case GENE_LOCUS_TAG_FIELD :
      RemOutTxt_SearchAndExcise (grp->locus_tag, cfp);
      break;
    case GENE_SYNONYM_FIELD :
      for (vnp = grp->syn; vnp != NULL; vnp = vnp->next) {
	str = (CharPtr) vnp->data.ptrvalue;
	RemOutTxt_SearchAndExcise (str, cfp);
      }
      break;
    case GENE_COMMENT_FIELD :
      RemOutTxt_SearchAndExcise (sfp->comment, cfp);
      break;
    default :
      break;
  }

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_DoImpFeat () -- Does the requested search and delete on   */
/*                           one import feature.                       */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK RemOutTxt_DoImpFeat (SeqFeatPtr sfp,
				    SeqMgrFeatContextPtr context)
{
  GBQualPtr         gbqp;
  ConvertFormPtr    cfp;
  Int4              i;
  EnumFieldAssocPtr ap;
  ImpFeatPtr        ifp;

  /* Check parameters */

  cfp = (ConvertFormPtr) context->userdata;
  if (NULL == cfp)
    return FALSE;

  /* only do removal for requested types of import feats */
  if (cfp->impfeat_type != 1) {
    ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
    if (ifp == NULL || ifp->key == NULL) return TRUE;
    ap = cfp->impfeat_alist;
    for (i = 2, ap ++; ap->name != NULL && i != cfp->impfeat_type; i ++, ap++)
    {}
    if (i != cfp->impfeat_type || StringCmp (ap->name, ifp->key) != 0) {
      return TRUE;
    }
  }

  /* Do the removal on the requested field */

  switch (cfp->subtype) {
  case IMPORT_GBQUAL_FIELD :
    gbqp = sfp->qual;
    while (NULL != gbqp)
      {
	if (NULL != gbqp->val)
	  RemOutTxt_SearchAndExcise (gbqp->val, cfp);
	gbqp = gbqp->next;
      }
    break;
  case IMPORT_COMMENT_FIELD :
    RemOutTxt_SearchAndExcise (sfp->comment, cfp);
    break;
  default :
    break;
  }

  /* Return TRUE to tell explore function to */
  /* go on to the next feature.              */

  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_DoBioseq () -- Does the requested search and delete on    */
/*                          one Bioseq.                                */
/*                                                                     */
/*---------------------------------------------------------------------*/

static Boolean LIBCALLBACK RemOutTxt_DoBioseq (BioseqPtr bsp,
				   SeqMgrBioseqContextPtr context)
{
  Boolean        featureFilterArray [SEQFEAT_MAX];
  Boolean        descFilterArray [SEQDESCR_MAX];
  ConvertFormPtr cfp;
  CharPtr        title;

  /* Check parameters */

  if (NULL == context)
    return FALSE;

  cfp = (ConvertFormPtr) context->userdata;
  if (NULL == cfp)
    return FALSE;

  /* Initialize the filter arrays */

  MemSet ((Pointer) (featureFilterArray), (int) FALSE, SEQFEAT_MAX);
  MemSet ((Pointer) (descFilterArray), (int) FALSE, SEQDESCR_MAX);

  /* Explore the requested feature or descriptor type */

  switch (cfp->type) {
    case GENE_TYPE :
      featureFilterArray[SEQFEAT_GENE] = TRUE;
      SeqMgrExploreFeatures (bsp, cfp, RemOutTxt_DoGene,
			     NULL, featureFilterArray, NULL);
      break;
    case CDS_TYPE :
      featureFilterArray[SEQFEAT_CDREGION] = TRUE;
      SeqMgrExploreFeatures (bsp, cfp, RemOutTxt_DoCDS,
			     NULL, featureFilterArray, NULL);
      break;
    case PROT_TYPE :
      featureFilterArray[SEQFEAT_PROT] = TRUE;
      SeqMgrExploreFeatures (bsp, cfp, RemOutTxt_DoProt,
			     NULL, featureFilterArray, NULL);
      break;
    case RNA_TYPE :
      featureFilterArray[SEQFEAT_RNA] = TRUE;
      SeqMgrExploreFeatures (bsp, cfp, RemOutTxt_DoRNA,
			     NULL, featureFilterArray, NULL);
      break;
    case BIOSOURCE_TYPE :

      /* A BioSource may be either a descriptor or a */
      /* feature so we need to explore both.         */

      featureFilterArray[SEQFEAT_ORG] = TRUE;
      SeqMgrExploreFeatures (bsp, cfp, RemOutTxt_DoBioSourceFeat,
			     NULL, featureFilterArray, NULL);

      descFilterArray[Seq_descr_source] = TRUE;
      SeqMgrExploreDescriptors (bsp, cfp, RemOutTxt_DoBioSourceDescr,
 				descFilterArray);

      break;
    case ORGMOD_SUBSOURCE_TYPE :

      /* OrgMods and SubSources may be either a descriptor */
      /* or a feature so we need to explore both.          */

      featureFilterArray[SEQFEAT_ORG] = TRUE;
      SeqMgrExploreFeatures (bsp, cfp, RemOutTxt_DoOMSSFeat,
			     NULL, featureFilterArray, NULL);

      descFilterArray[Seq_descr_source] = TRUE;
      SeqMgrExploreDescriptors (bsp, cfp, RemOutTxt_DoOMSSDescr,
 				descFilterArray);

      break;
    case IMPORT_FEAT_TYPE :
      featureFilterArray[SEQFEAT_IMP] = TRUE;
      SeqMgrExploreFeatures (bsp, cfp, RemOutTxt_DoImpFeat,
			     NULL, featureFilterArray, NULL);
      break;
    case DEFLINE_TYPE :
      title = BioseqGetTitle (bsp);
      RemOutTxt_SearchAndExcise (title, cfp);
      break;
    default:
      break;
  }
  
  return TRUE;
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_AcceptCallback ()                                         */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void RemOutTxt_AcceptCallback (ButtoN acceptButton)
{
  ConvertFormPtr  cfp;
  SeqEntryPtr     sep;
  UIEnum          val;

  /* Get the current sequence and associated data */

  cfp = (ConvertFormPtr) GetObjectExtra (acceptButton);
  if (cfp == NULL || cfp->input_entityID == 0)
    return;

  sep = GetTopSeqEntryForEntityID (cfp->input_entityID);
  if (sep == NULL)
    return;

  /* Hide the window and set the 'working' cursor */

  Hide (cfp->form);
  WatchCursor ();
  Update ();

  /* Get the states of the radio buttons */

  if (GetValue (cfp->includeBeforeGroup) == 1)
    cfp->includeleft = TRUE;
  else
    cfp->includeleft = FALSE;

  if (GetValue (cfp->includeAfterGroup) == 1)
    cfp->includeright = TRUE;
  else
    cfp->includeright = FALSE;

  if (GetValue (cfp->ifNotFoundGroup) == DO_NOTHING)
    cfp->ifNotFound = DO_NOTHING;
  else
    cfp->ifNotFound = REMOVE_ALL_TEXT;

  /* Get the feature and subfeature types */

  GetEnumPopup (cfp->target, cfp->target_alist, &val);
  if (0 == val)
    return;
  else
    cfp->type = (Int2) val;

  if (DEFLINE_TYPE != cfp->type) {
    GetEnumPopup (cfp->subtarget [cfp->type], cfp->alists [cfp->type], &val);
    if (0 == val)
      return;
    else
      cfp->subtype = (Int2) val;
  }

  if (cfp->type == IMPORT_FEAT_TYPE) {
    cfp->impfeat_type = GetValue (cfp->impfeat_select);
  }

  /* Get the before (left) and after (right) strings */

  cfp->leftstr = SaveStringFromTextNoStripSpaces (cfp->atleft);
  cfp->rightstr = SaveStringFromTextNoStripSpaces (cfp->atright);

  TrimOffEndQuotes (cfp->leftstr, TRIM_LEFT_END);
  TrimOffEndQuotes (cfp->rightstr, TRIM_RIGHT_END);

  /* Do actual work of removing text */

  SeqMgrExploreBioseqs (cfp->input_entityID, NULL, cfp,
			RemOutTxt_DoBioseq, TRUE, TRUE, TRUE);

  /* Clean up and exit */

  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (cfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, cfp->input_entityID, 0, 0);
  if (GetStatus (cfp->leaveDlgUp))
  {
  	Show (cfp->form);
  }
  else
  {
    Remove (cfp->form);
  }
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_TextCallback () -- Called when text is entered into       */
/*                              either the before or after text fields */
/*                              in the "Remove Text Outside String"    */
/*                              window.                                */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void RemOutTxt_TextCallback (Handle a)

{
  ConvertFormPtr  cfp;

  cfp = (ConvertFormPtr) GetObjectExtra (a);
  if (cfp == NULL)
    return;

  RemOutTxt_SetAcceptButtonStatus (cfp);
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_SubFeatureCallback () -- Called when a subfeature is      */
/*                                    selected from the popup list     */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void RemOutTxt_SubFeatureCallback (Handle a)
{
  ConvertFormPtr  cfp;
  cfp = (ConvertFormPtr) GetObjectExtra (a);
  if (cfp == NULL)
    return;

  RemOutTxt_SetAcceptButtonStatus (cfp);
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemOutTxt_FeatureCallback () -- Called when a feature is selected   */
/*                                 from the popup list.                */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void RemOutTxt_FeatureCallback (PopuP p)
{
  ConvertFormPtr  cfp;
  Int2            i;
  UIEnum          val;

  /* Get the current data */

  cfp = (ConvertFormPtr) GetObjectExtra (p);
  if (cfp == NULL) 
    return;

  /* Hide the old sub-feature popup */
  /* and display the new one.       */

  if (GetEnumPopup (cfp->target, cfp->target_alist, &val) && val > 0) {
    for (i = 0; i < NUM_SUBTARGET_POPUPS; i++) {
      if (i != (Int2) val) {
        SafeHide (cfp->subtarget [i]);
      }
    }
    if (val == IMPORT_FEAT_TYPE) {
      SafeShow (cfp->subtarget [(Int2) val]);
      SafeShow (cfp->impfeatGroup);
    } else {
      SafeHide (cfp->impfeatGroup);
      SafeShow (cfp->subtarget [(Int2) val]);
    }
  }

  /* Adjust the 'Accept' button status */

  RemOutTxt_SetAcceptButtonStatus (cfp);
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RemoveTextOutsideString ()                                          */
/*                                                                     */
/*---------------------------------------------------------------------*/

extern void RemoveTextOutsideString (IteM i)
{
  EnumFieldAssocPtr  ap;
  BaseFormPtr        bfp;
  GrouP              buttonGroup;
  ConvertFormPtr     cfp;
  GrouP              featureGroup;
  GrouP              headerGroup;
  Int2               j;
  GrouP              mainGroup;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  GrouP              subFeatureGroup;
  GrouP              textGroup;
  WindoW             w;

  /* Get current sequence */

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL)
    return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL)
    return;

  /* Create a form for passing user data on to callbacks */

  cfp = ConvertFormNew ();
  if (cfp == NULL)
    return;

  cfp->parsetype = PARSETYPE_FILE;
  cfp->target_alist = target_field_alist;

  /* Create a window for getting user input */

  w = FixedWindow (-50, -33, -10, -10, "Remove Text Outside String",
		   StdCloseWindowProc);
  SetObjectExtra (w, cfp, StdCleanupFormProc);
  cfp->form = (ForM) w;
  cfp->formmessage = ConvertMessageProc;

  /* Attach some basic data and callbacks to the data form */

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    cfp->appmessage = sepp->handleMessages;
  }

  cfp->input_entityID = bfp->input_entityID;
  cfp->input_itemID   = bfp->input_itemID;
  cfp->input_itemtype = bfp->input_itemtype;

  /* Main object group for holding all the others */

  mainGroup = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (mainGroup, 10, 10);

  /* Set up Text group and headers */

  headerGroup = HiddenGroup (mainGroup, 1, 0, NULL);
  textGroup   = HiddenGroup (mainGroup, 4, 0, NULL);

  StaticPrompt (headerGroup, "Remove Text :                       "
		"Also Remove Entered Text?", 0, 0,
		programFont, 'l');

  /* 'Before' text */

  cfp->removeBefore = CheckBox (textGroup, "",
				RemOutTxt_BeforeCheckBoxCallback);
  SetObjectExtra (cfp->removeBefore, cfp, NULL);
  StaticPrompt (textGroup, "Before", 0, dialogTextHeight,
		programFont, 'l');

  cfp->atleft = DialogText (textGroup, "", 10,
			    (TxtActnProc) RemOutTxt_TextCallback);
  SetObjectExtra (cfp->atleft, cfp, NULL);
  SafeDisable (cfp->atleft);

  cfp->includeBeforeGroup = HiddenGroup (textGroup, 2, 0, NULL);
  RadioButton (cfp->includeBeforeGroup, "Yes");
  RadioButton (cfp->includeBeforeGroup, "No");
  SetValue (cfp->includeBeforeGroup, 2);
  SafeDisable (cfp->includeBeforeGroup);

  /* 'After' text */

  cfp->removeAfter = CheckBox (textGroup, "",
			       RemOutTxt_AfterCheckBoxCallback);
  SetObjectExtra (cfp->removeAfter, cfp, NULL);
  StaticPrompt (textGroup, "After", 0, dialogTextHeight,
		programFont, 'l');

  cfp->atright = DialogText (textGroup, "", 10,
			     (TxtActnProc) RemOutTxt_TextCallback);
  SetObjectExtra (cfp->atright, cfp, NULL);
  SafeDisable (cfp->atright);

  cfp->includeAfterGroup = HiddenGroup (textGroup, 2, 0, NULL);
  RadioButton (cfp->includeAfterGroup, "Yes");
  RadioButton (cfp->includeAfterGroup, "No");
  SetValue (cfp->includeAfterGroup, 2);
  SafeDisable (cfp->includeAfterGroup);

  /* The "If not found" group */

  cfp->ifNotFoundGroup = HiddenGroup (mainGroup, 0, 3, NULL);
  StaticPrompt (cfp->ifNotFoundGroup, "If field doesn't contain searched" 
		" for text:", 0, 0, programFont, 'l');

  RadioButton (cfp->ifNotFoundGroup, "     Do nothing to field");
  RadioButton (cfp->ifNotFoundGroup, "     Remove entire field text");
  SetValue (cfp->ifNotFoundGroup, DO_NOTHING);

  /* Add the popup list for the feature type */

  featureGroup = HiddenGroup (mainGroup, 2, 0, NULL);
  SetGroupSpacing (featureGroup, 10, 10);

  cfp->target = PopupList (featureGroup, TRUE, RemOutTxt_FeatureCallback);
  SetObjectExtra (cfp->target, cfp, NULL);
  InitEnumPopup (cfp->target, cfp->target_alist, NULL);
  SetEnumPopup (cfp->target, cfp->target_alist, 0);

  /* Create and fill in the submenus for */
  /* each of the Feature types.          */

  cfp->alists [GENE_TYPE]             = gene_field_alist;
  cfp->alists [CDS_TYPE]              = cds_short_field_alist;
  cfp->alists [PROT_TYPE]             = prot_field_alist;
  cfp->alists [RNA_TYPE]              = rnax_short_field_alist;
  cfp->alists [BIOSOURCE_TYPE]        = orgref_field_alist;
  cfp->alists [ORGMOD_SUBSOURCE_TYPE] = subsource_and_orgmod_note_subtype_alist;
  cfp->alists [IMPORT_FEAT_TYPE]      = impfeat_field_alist;

  subFeatureGroup = HiddenGroup (featureGroup, 0, 0, NULL);

  for (j = 1; j <= NUMBER_OF_TYPES_WITH_DEFLINE; j++) {
    if (j == DEFLINE_TYPE) {
      /* No Subtypes for defline, so do nothing */
    }
    else if (j == ORGMOD_SUBSOURCE_TYPE) {
      cfp->subtarget [j] =
	(PopuP) SingleList (subFeatureGroup, 10, 8,
		    (LstActnProc) RemOutTxt_SubFeatureCallback);
      SetObjectExtra (cfp->subtarget [j], cfp, NULL);
      for (ap = cfp->alists [j]; ap->name != NULL; ap++)
	ListItem ((LisT) cfp->subtarget [j], ap->name);
      Hide (cfp->subtarget [j]);
    } else if (j == IMPORT_FEAT_TYPE) {
      cfp->impfeatGroup = HiddenGroup (subFeatureGroup, 2, 0, NULL);
      ap = import_featdef_alist (FALSE, FALSE, FALSE);
      SortEnumFieldAssocPtrArray (ap, CompareImpFeatEnumFieldAssoc);
      cfp->impfeat_alist = ap;
      cfp->impfeat_select = SingleList (cfp->impfeatGroup, 12, 6, NULL);
      for (ap = cfp->impfeat_alist; ap->name != NULL; ap++) {
        if (ap == cfp->impfeat_alist) {
          ListItem (cfp->impfeat_select, "All import features");
        } else {
          ListItem (cfp->impfeat_select, ap->name);
        }
      }
      SetValue (cfp->impfeat_select, 1);
      cfp->subtarget [j] = PopupList (cfp->impfeatGroup, TRUE,
		         (PupActnProc) RemOutTxt_SubFeatureCallback);
      SetObjectExtra (cfp->subtarget [j], cfp, NULL);
      InitEnumPopup (cfp->subtarget [j], cfp->alists [j], NULL);
      SetEnumPopup (cfp->subtarget [j], cfp->alists [j], 0);
      Hide (cfp->impfeatGroup);
      
    } else {
      cfp->subtarget [j] = PopupList (subFeatureGroup, TRUE,
		         (PupActnProc) RemOutTxt_SubFeatureCallback);
      SetObjectExtra (cfp->subtarget [j], cfp, NULL);
      InitEnumPopup (cfp->subtarget [j], cfp->alists [j], NULL);
      SetEnumPopup (cfp->subtarget [j], cfp->alists [j], 0);
      Hide (cfp->subtarget [j]);
    }
  }

  /* Accept and Cancel buttons */

  buttonGroup = HiddenGroup (mainGroup, 4, 0, NULL);
  cfp->accept = DefaultButton (buttonGroup, "Accept",
			    RemOutTxt_AcceptCallback);
  SetObjectExtra (cfp->accept, cfp, NULL);
  Disable (cfp->accept);
  PushButton (buttonGroup, "Cancel", StdCancelButtonProc);
  cfp->leaveDlgUp = CheckBox (buttonGroup, "Leave Dialog Up", NULL);

  /* Line things up and display the window */

  AlignObjects (ALIGN_CENTER,
		(HANDLE) headerGroup,
		(HANDLE) textGroup,
		(HANDLE) buttonGroup,
		(HANDLE) featureGroup,
		NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
  Update ();
}

extern void RemoveTextInsideString (IteM i)
{
  EnumFieldAssocPtr  ap;
  BaseFormPtr        bfp;
  GrouP              c;
  ConvertFormPtr     cfp;
  GrouP              g;
  GrouP              h;
  Int2               j;
  GrouP              p;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  WindoW             w;
  GrouP              x;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  cfp = ConvertFormNew ();
  if (cfp == NULL) return;

  cfp->parsetype = PARSETYPE_FILE;
  cfp->target_alist = target_field_alist;
  cfp->set_accept_proc = (PupActnProc) SetConvertAcceptButton;

  w = FixedWindow (-50, -33, -10, -10, "General Text Excision", StdCloseWindowProc);
  SetObjectExtra (w, cfp, StdCleanupFormProc);
  cfp->form = (ForM) w;
  cfp->formmessage = ConvertMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    cfp->appmessage = sepp->handleMessages;
  }

  cfp->input_entityID = bfp->input_entityID;
  cfp->input_itemID = bfp->input_itemID;
  cfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, 3, 0, NULL);
  StaticPrompt (g, "Remove text between", 0, dialogTextHeight, programFont, 'l');
  cfp->leftbehav = HiddenGroup (g, 2, 0, NULL);
  RadioButton (cfp->leftbehav, "just after");
  RadioButton (cfp->leftbehav, "starting at");
  SetValue (cfp->leftbehav, 1);
  cfp->atleft = DialogText (g, "", 10, (TxtActnProc) cfp->set_accept_proc);
  SetObjectExtra (cfp->atleft, cfp, NULL);
  StaticPrompt (g, "and", 0, dialogTextHeight, programFont, 'l');
  cfp->rightbehav = HiddenGroup (g, 2, 0, NULL);
  RadioButton (cfp->rightbehav, "up to");
  RadioButton (cfp->rightbehav, "including");
  SetValue (cfp->rightbehav, 1);
  cfp->atright = DialogText (g, "", 10, (TxtActnProc) cfp->set_accept_proc);
  SetObjectExtra (cfp->atright, cfp, NULL);

  p = HiddenGroup (h, 6, 0, NULL);
  StaticPrompt (p, "Perform excision in", 0, popupMenuHeight, programFont, 'l');
  cfp->target = PopupList (p, TRUE, ChangeSubtarget);
  SetObjectExtra (cfp->target, cfp, NULL);
  InitEnumPopup (cfp->target, cfp->target_alist, NULL);
  SetEnumPopup (cfp->target, cfp->target_alist, 0);

  cfp->alists [GENE_TYPE]             = gene_field_alist;
  cfp->alists [CDS_TYPE]              = cds_field_alist;
  cfp->alists [PROT_TYPE]             = prot_field_alist;
  cfp->alists [RNA_TYPE]              = rnax_field_alist;
  cfp->alists [BIOSOURCE_TYPE]        = orgref_field_alist;
  cfp->alists [ORGMOD_SUBSOURCE_TYPE] = subsource_and_orgmod_note_subtype_alist;
  cfp->alists [IMPORT_FEAT_TYPE]      = impfeat_field_alist;

  cfp->replaceOldAsked = FALSE;
  cfp->doReplaceAll = FALSE;

  x = HiddenGroup (p, 0, 0, NULL);

  for (j = 1; j <= NUMBER_OF_TYPES_WITH_DEFLINE; j++) {
    if (j == DEFLINE_TYPE) {
      /* No subtypes for DefLines, so do nothing */
    }
    else if (j == ORGMOD_SUBSOURCE_TYPE) {
      cfp->subtarget [j] =
	(PopuP) SingleList (x, 10, 8, (LstActnProc) cfp->set_accept_proc);
      SetObjectExtra (cfp->subtarget [j], cfp, NULL);
      for (ap = cfp->alists [j]; ap->name != NULL; ap++)
	ListItem ((LisT) cfp->subtarget [j], ap->name);
      Hide (cfp->subtarget [j]);
    }
    else {
      cfp->subtarget [j] = PopupList (x, TRUE, cfp->set_accept_proc);
      SetObjectExtra (cfp->subtarget [j], cfp, NULL);
      InitEnumPopup (cfp->subtarget [j], cfp->alists [j], NULL);
      SetEnumPopup (cfp->subtarget [j], cfp->alists [j], 0);
      Hide (cfp->subtarget [j]);
    }
  }

  c = HiddenGroup (h, 4, 0, NULL);
  cfp->accept = DefaultButton (c, "Accept", DoRemoveTextProc);
  SetObjectExtra (cfp->accept, cfp, NULL);
  Disable (cfp->accept);
  PushButton (c, "Cancel", StdCancelButtonProc);
  cfp->leaveDlgUp = CheckBox (c, "Leave Dialog Up", NULL);


  AlignObjects (ALIGN_CENTER, (HANDLE) p, (HANDLE) c, (HANDLE) g, NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
  Select (cfp->atleft);
  Update ();
}

/* AAForCodon is extern in seqport.c */
/* NLM_EXTERN Uint1 AAForCodon (Uint1Ptr codon, CharPtr codes); */

/*
static Boolean CorrectStartCodonCallback (GatherContextPtr gcp)

{
  Uint1           aa;
  Boolean         bad_base;
  CodeBreakPtr    cbp;
  Uint1           codon [3];
  CharPtr         codes;
  CdRegionPtr     crp;
  GeneticCodePtr  gc;
  Int2            i;
  Uint1           residue;
  SeqEntryPtr     sep;
  SeqFeatPtr      sfp;
  SeqLocPtr       slp;
  SeqPntPtr       spntp;
  SeqPortPtr      spp;
  ValNodePtr      vnp;

  if (gcp == NULL) return TRUE;
  sep = (SeqEntryPtr) gcp->userdata;
  if (sep == NULL ) return TRUE;
  if (gcp->thistype != OBJ_SEQFEAT) return TRUE;
  sfp = (SeqFeatPtr) gcp->thisitem;
  if (sfp == NULL || sfp->data.choice != SEQFEAT_CDREGION) return TRUE;
  crp = (CdRegionPtr) sfp->data.value.ptrvalue;
  if (crp == NULL) return TRUE;

  if (crp->code_break != NULL) return TRUE;

	gc = NULL;
	if (crp->genetic_code != NULL)
	{
		vnp = (ValNodePtr)(crp->genetic_code->data.ptrvalue);
		while ((vnp != NULL) && (gcp == NULL))
		{
			switch (vnp->choice)
			{
			case 1:
				gc = GeneticCodeFind(0, (CharPtr)vnp->data.ptrvalue);
				break;
			case 2:
				gc = GeneticCodeFind(vnp->data.intvalue, NULL);
				break;
			case 3:
			case 6:
			case 4:
			case 5:
			case 7:
			case 8:
			default:
				break;
			}
			vnp = vnp->next;
		}
	}
	if (gc == NULL)
		gc = GeneticCodeFind(1, NULL);
	if (gc == NULL) return TRUE;

	codes = NULL;
	for (vnp = (ValNodePtr)gc->data.ptrvalue; vnp != NULL; vnp = vnp->next)
	{
		if (vnp->choice == 3)
			codes = (CharPtr)vnp->data.ptrvalue;
	}
	if (codes == NULL) return TRUE;

  if (crp->frame == 2 || crp->frame == 3) return TRUE;

  spp = SeqPortNewByLoc (sfp->location, Seq_code_ncbi4na);
  if (spp == NULL) return TRUE;
  bad_base = FALSE;
  for (i = 0; i < 3; i++) {
    residue = SeqPortGetResidue (spp);
    if (residue == SEQPORT_EOF)
      break;
    if (residue == INVALID_RESIDUE)
      bad_base = TRUE;
    codon[i] = residue;
  }
  SeqPortFree (spp);
  if (i != 3 || bad_base) return TRUE;
  aa = AAForCodon (codon, codes);

  spp = SeqPortNewByLoc (sfp->product, Seq_code_ncbieaa);
  if (spp == NULL) return TRUE;
  residue = SeqPortGetResidue (spp);
  SeqPortFree (spp);
  if (residue == SEQPORT_EOF || residue == INVALID_RESIDUE) return TRUE;

  if (residue != aa) {
    cbp = CodeBreakNew ();
    if (cbp != NULL) {
      spntp = SeqPntNew ();
      slp = ValNodeNew (NULL);
      slp->choice = SEQLOC_PNT;
      slp->data.ptrvalue = (Pointer) spntp;
      spntp->point = (Int4) 0;
      spntp->id = SeqIdStripLocus (SeqIdDup (SeqLocId (sfp->product)));
      cbp->loc = slp;
      slp = aaLoc_to_dnaLoc (sfp, cbp->loc);
      cbp->loc = SeqLocFree (cbp->loc);
      cbp->loc = slp;
      cbp->aa.value.intvalue = aa;
      cbp->aa.choice = 1;
      cbp->next = crp->code_break;
      crp->code_break = cbp;
    }
  }

  return TRUE;
}

static void CorrectStartCodon (SeqEntryPtr sep, Uint2 entityID)

{
  BioseqSetPtr  bssp;
  GatherScope   gs;

  if (sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 || bssp->_class == 13 ||
                         bssp->_class == 14 || bssp->_class == 15)) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        CorrectStartCodon (sep, entityID);
      }
      return;
    }
  }
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = FALSE;
  MemSet((Pointer)(gs.ignore), (int)(TRUE), (size_t)(OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.scope = sep;
  GatherEntity (entityID, (Pointer) sep, CorrectStartCodonCallback, &gs);
}

extern void CorrectCDSStartCodon (IteM i);
extern void CorrectCDSStartCodon (IteM i)

{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  CorrectStartCodon (sep, bfp->input_entityID);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}
*/

static ENUM_ALIST(rnax_subtype_alist)
  {" ",           99},
  {"unknown",      0},
  {"preRna",       1},
  {"mRNA",         2},
  {"tRNA",         3},
  {"rRNA",         4},
  {"snRNA",        5},
  {"scRNA",        6},
  {"snoRNA",       7},
  {"misc_RNA",   255},
END_ENUM_ALIST

typedef struct applyformdata {
  FEATURE_FORM_BLOCK

  Int2           type;
  Int2           errcount;
  ValNodePtr     ambigList;
  Boolean        noLeft;
  Boolean        noRight;
  ButtoN         applyToParts;
  ButtoN         partial5;
  ButtoN         partial3;
  TexT           onlyThisPart;
  TexT           geneName;
  TexT           protName;
  TexT           protDesc;
  TexT           rnaName;
  TexT           featcomment;
  TexT           defline;
  LisT           key;
  PopuP          rnaSubType;
  Uint1          rnatype;
  EnumFieldAssoc PNTR realalist;
  EnumFieldAssoc PNTR alist;
  GrouP          strand_group;
  PopuP          reading_frame;
  GrouP          use_whole_interval;
  TexT           left_end;
  TexT           right_end;
  ButtoN         add_to_seq_with_like_feature;
  ButtoN         leaveDlgUp;
} ApplyFormData, PNTR ApplyFormPtr;

typedef struct alreadyhas {
  Boolean        rsult;
  Uint1          featchoice;
  Uint1          descchoice;
  Uint1          rnatype;
} AlreadyHas, PNTR AlreadyHasPtr;

static Boolean SeeIfAlreadyHasGatherFunc (GatherContextPtr gcp)

{
  AlreadyHasPtr  ahp;
  RnaRefPtr      rrp;
  ValNodePtr     sdp;
  SeqFeatPtr     sfp;

  if (gcp == NULL) return TRUE;

  ahp = (AlreadyHasPtr) gcp->userdata;
  if (ahp == NULL ) return TRUE;

  if (gcp->thistype == OBJ_SEQFEAT && ahp->featchoice != 0) {
    sfp = (SeqFeatPtr) gcp->thisitem;
    if (sfp != NULL && sfp->data.choice == ahp->featchoice && sfp->data.value.ptrvalue != NULL) {
      if (sfp->data.choice == SEQFEAT_RNA) {
        rrp = (RnaRefPtr) sfp->data.value.ptrvalue;
        if (rrp->type != ahp->rnatype) return TRUE;
      }
      ahp->rsult = TRUE;
      return FALSE;
    }
  } else if (gcp->thistype == OBJ_SEQDESC && ahp->descchoice != 0) {
    sdp = (ValNodePtr) gcp->thisitem;
    if (sdp != NULL && sdp->choice == ahp->descchoice && sdp->data.ptrvalue != NULL) {
      ahp->rsult = TRUE;
      return FALSE;
    }
  }
  return TRUE;
}

static Boolean AlreadyHasFeatOrDesc (SeqEntryPtr sep, Uint1 featchoice, Uint1 descchoice, Uint1 rnatype)

{
  AlreadyHas   ah;
  BioseqPtr    bsp;
  GatherScope  gs;
  SeqEntryPtr  nsep;
  SeqIdPtr     sip;
  SeqLocPtr    slp;

  ah.rsult = FALSE;
  ah.featchoice = featchoice;
  ah.descchoice = descchoice;
  ah.rnatype = rnatype;
  if (sep == NULL) return FALSE;
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 1;
  gs.get_feats_location = TRUE;
  MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQFEAT] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  gs.ignore[OBJ_SEQANNOT] = FALSE;
  gs.scope = sep;
  if (descchoice != 0) {
    nsep = FindNucSeqEntry (sep);
    if (nsep != NULL && IS_Bioseq (nsep)) {
      bsp = (BioseqPtr) nsep->data.ptrvalue;
      if (bsp != NULL) {
        slp = ValNodeNew (NULL);
        slp->choice = SEQLOC_WHOLE;
        sip = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
        slp->data.ptrvalue = sip;
        gs.target = slp;
      }
    }
  }
  GatherSeqEntry (sep, (Pointer) (&ah), SeeIfAlreadyHasGatherFunc, &gs);
  gs.target = SeqLocFree (gs.target);
  return ah.rsult;
}

static void setSeqFeatStrand(SeqFeatPtr sfp, Uint1 strand)
{
  SeqIntPtr          sqip;

  if (sfp == NULL) return;
  if (sfp->location == NULL) return;
  if (sfp->location->choice != SEQLOC_INT) return;
  sqip = (SeqIntPtr) sfp->location->data.ptrvalue;
  if (sqip != NULL)
  {
    sqip->strand = Seq_strand_minus;
  }
}

/* must be called after partials and strand are set*/
static void AdjustSeqLocForApply (SeqFeatPtr sfp, ApplyFormPtr afp)
{
  BioseqPtr bsp;
  SeqLocPtr slp;
  SeqIntPtr sip;
  CharPtr   num_str;
  Int4      from;
  Int4      to;
  Int4      tmp;
  Boolean   partial3, partial5;
  Uint1     strand;
  
  if (sfp == NULL || afp == NULL) return;
  
  bsp = BioseqFindFromSeqLoc (sfp->location);
  if (bsp == NULL) return;
  
  num_str = SaveStringFromText (afp->left_end);
  if (num_str == NULL) return;
  from = atoi (num_str);
  num_str = MemFree (num_str);
  num_str = SaveStringFromText (afp->right_end);
  if (num_str == NULL) return;
  to = atoi (num_str);
  num_str = MemFree (num_str);
  if (from > to)
  {
  	tmp = from;
  	from = to;
  	to = from;
  }
  if (from < 1)
  {
  	from = 1;
  }
  if (to < 1)
  {
  	to = 1;
  }
  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  strand = SeqLocStrand (sfp->location);
  if (to > bsp->length)
  {
  	to = bsp->length;
  	partial3 = TRUE;
  }

  slp = ValNodeNew (NULL);
  if (slp != NULL) {
    sip = SeqIntNew ();
    if (sip != NULL) {
      slp->choice = SEQLOC_INT;
      slp->data.ptrvalue = (Pointer) sip;
      sip->from = from - 1;
      sip->to = to - 1;
      sip->strand = strand;
      sip->id = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
    }
  }
  
  sfp->location = SeqLocFree (sfp->location);
  sfp->location = slp;
  
  SetSeqLocPartial (sfp->location, partial5, partial3);
  if (partial5 || partial3)
  {
  	sfp->partial = TRUE;
  }
  
}

static void AddGeneXrefToFeat (SeqFeatPtr sfp, CharPtr str)
{
  SeqFeatXrefPtr    xref;
  GeneRefPtr        grp;
  
  if (sfp == NULL || StringHasNoText (str)) return;
  
  /* add gene xref to feature */
  xref = SeqFeatXrefNew ();
  if (xref != NULL)
  {
    grp = CreateNewGeneRef (str, NULL, NULL, FALSE);
    if (grp != NULL) 
    {
      xref->data.choice = SEQFEAT_GENE;
      xref->data.value.ptrvalue = grp;
      xref->next = sfp->xref;
      sfp->xref = xref;
    }
  }
}

static SeqFeatPtr ApplyGene (CharPtr str, ApplyFormPtr afp, SeqEntryPtr gene_sep, SeqFeatPtr sfp)
{
  GeneRefPtr        grp;
  SeqFeatPtr        gene_sfp;
  SeqFeatXrefPtr    xref;
  SeqMgrFeatContext fcontext;
  BioseqPtr         bsp = NULL;
  SeqFeatPtr        other_feat;
  SeqFeatPtr        overlap_gene;
  Boolean           added_xrefs = FALSE;
  SeqFeatPtr        misc_feat = NULL;
  SeqLocPtr         overlap_loc;

  if (StringHasNoText (str) || afp == NULL || gene_sep == NULL) return NULL;

  /* we need a location to use when we're checking for feature-stealing genes */
  if (sfp != NULL)
  {
    overlap_loc = sfp->location;
  }
  else
  {
    misc_feat = CreateNewFeature (gene_sep, NULL, SEQFEAT_COMMENT, NULL);
    if (NULL == misc_feat)
    return NULL;

    SetSeqLocPartial (misc_feat->location, afp->noLeft, afp->noRight);

    if (afp->use_whole_interval != NULL
        && GetValue (afp->use_whole_interval) != 1)
    {
      /* adjust location to match coordinates from user */
      AdjustSeqLocForApply (misc_feat, afp);
    }      
    
    if (GetValue (afp->strand_group) == 2) 
    {
      /* reverse strand direction - strand direction is plus by default */
      setSeqFeatStrand (misc_feat, Seq_strand_minus);
    }

    misc_feat->partial = (afp->noLeft || afp->noRight);
    overlap_loc = misc_feat->location;
  }
  
  /* first, add gene xrefs to all features on bioseq that are contained in the location */
  /* maintain list of features that had xrefs before, should not remove them later */
  if (IS_Bioseq (gene_sep))
  {
    bsp = (BioseqPtr) gene_sep->data.ptrvalue;
  }
  else if (sfp != NULL)
  {
    bsp = BioseqFindFromSeqLoc (sfp->location);
  }
  if (bsp != NULL)
  {
    other_feat = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
    while (other_feat != NULL)
    {
      if (other_feat != sfp && other_feat->data.choice != SEQFEAT_GENE)
      {
        for (xref = other_feat->xref;
             xref != NULL && xref->data.choice != SEQFEAT_GENE;
             xref = xref->next)
        {}
        if (xref == NULL
            && SeqLocCompare (other_feat->location, overlap_loc) == SLC_A_EQ_B)
        {
          overlap_gene = SeqMgrGetOverlappingGene (other_feat->location, NULL);
          if (overlap_gene != NULL)
          {
            AddGeneXrefToFeat (other_feat, fcontext.label);
            added_xrefs = TRUE;
          }
        }
      }
      other_feat = SeqMgrGetNextFeature (bsp, other_feat, 0, 0, &fcontext);
    }   
  }
  
  if (misc_feat != NULL)
  {
    misc_feat->idx.deleteme = TRUE;
    DeleteMarkedObjects (0, OBJ_SEQENTRY, gene_sep);
  }
  
  grp = CreateNewGeneRef (str, NULL, NULL, FALSE);
  if (NULL == grp)
    return NULL;

  gene_sfp = CreateNewFeature (gene_sep, NULL, SEQFEAT_GENE, NULL);
  if (NULL == gene_sfp)
    return NULL;

  gene_sfp->data.value.ptrvalue = (Pointer) grp;
  SetSeqLocPartial (gene_sfp->location, afp->noLeft, afp->noRight);

  if (afp->use_whole_interval != NULL
      && GetValue (afp->use_whole_interval) != 1)
  {
    /* adjust location to match coordinates from user */
    AdjustSeqLocForApply (gene_sfp, afp);
  }      
    
  if (GetValue (afp->strand_group) == 2) {
    /* reverse strand direction - strand direction is plus by default */
    setSeqFeatStrand (gene_sfp, Seq_strand_minus);
  }

  gene_sfp->partial = (afp->noLeft || afp->noRight);

  if (added_xrefs && sfp != NULL)
  {
    /* add gene xref to feature */
    AddGeneXrefToFeat (sfp, str);
  }
  
  return gene_sfp;
}

static SeqLocPtr RemoveGapsFromSegmentedLocation (SeqLocPtr slp, BioseqPtr bsp)
{
  SeqLocPtr loc_slp, slp_new, slp_tmp, loc_list = NULL, loc_last = NULL;
  SeqIdPtr  sip;
  Uint1     strand;
  Int4      seq_offset, start_pos, end_pos, piece_len;
  
  if (slp == NULL || bsp == NULL || bsp->repr != Seq_repr_seg)
  {
    return slp;
  }
  
  loc_slp = SeqLocFindNext (slp, NULL);

  while (loc_slp != NULL)
  {
    strand = SeqLocStrand (loc_slp);
    start_pos = SeqLocStart (loc_slp);
    end_pos = SeqLocStop (loc_slp);
  
    /* create list of locations */
    slp_tmp = bsp->seq_ext;
    seq_offset = 0;
    while (slp_tmp != NULL)
    {
      piece_len = SeqLocLen (slp_tmp);
      
      if (seq_offset < end_pos
          && seq_offset + piece_len >= start_pos)
      {
        sip = SeqLocId (slp_tmp);
      
        slp_new = SeqLocIntNew (MAX (0, start_pos - seq_offset),
                                MIN (piece_len, end_pos - seq_offset),
                                strand, sip);

        if (slp_new != NULL)
        {
          if (loc_last == NULL)
          {
            loc_list = slp_new;
          }
          else
          {
            loc_last->next = slp_new;
          }
          loc_last = slp_new;
        }
      }
      seq_offset += piece_len;
      slp_tmp = slp_tmp->next;
    }
    loc_slp = SeqLocFindNext (slp, loc_slp);
  }
  
  if (loc_list == NULL)
  {
    /* failed to convert - do not change */
  }
  else if (loc_list->next == NULL)
  {
    /* only found one piece */
    slp = SeqLocFree (slp);
    slp = loc_list;
  }
  else
  {
    /* make mixed location */
    slp_new = ValNodeNew (NULL);
    if (slp_new != NULL)
    {
      slp_new->choice = SEQLOC_MIX;
      slp_new->data.ptrvalue = loc_list;
      slp = SeqLocFree (slp);
      slp = slp_new;
    }
  }
  return slp;
}

extern SeqLocPtr RemoveGapsFromDeltaLocation (SeqLocPtr slp, BioseqPtr bsp)
{
  DeltaSeqPtr dsp;
  Int4        seq_offset = 0;
  SeqLitPtr   slip;
  SeqLocPtr   far_loc, loc_slp, piece_loc, slp_new;
  SeqLocPtr   loc_list = NULL, last_loc = NULL;
  SeqIdPtr    sip;
  Uint1       strand;
  Int4        start_pos, end_pos, piece_len;
  Boolean     add_piece;
  
  if (slp == NULL || bsp == NULL 
      || bsp->repr != Seq_repr_delta
      || bsp->seq_ext_type != 4
      || bsp->seq_ext == NULL)
  {
    return slp;
  }
  
  sip = SeqLocId (slp);
  if (sip == NULL)
  {
    return slp;
  }
  
  loc_slp = SeqLocFindNext (slp, NULL);
  while (loc_slp != NULL)
  {
    strand = SeqLocStrand (slp);
    start_pos = SeqLocStart (loc_slp);
    end_pos = SeqLocStop (loc_slp);
    
    seq_offset = 0;
    dsp = (DeltaSeqPtr) bsp->seq_ext;
    while (dsp != NULL)
    {
      piece_len = 0;
      add_piece = FALSE;
      if (dsp->data.ptrvalue == NULL)
      {
        /* bad delta seq - skip */
      }
      else if (dsp->choice == 1)
      {
        far_loc = (SeqLocPtr) dsp->data.ptrvalue;
        piece_len = SeqLocLen (far_loc);
        add_piece = TRUE;
      }
      else if (dsp->choice == 2)
      {
        slip = (SeqLitPtr) (dsp->data.ptrvalue);  
        piece_len = slip->length;
        if (slip->seq_data != NULL)
        {
          add_piece = TRUE;
        }
      }
      if (seq_offset >= end_pos
          || seq_offset + piece_len < start_pos)
      {
        add_piece = FALSE;
      }
      if (add_piece)
      {
        piece_loc = SeqLocIntNew (MAX (seq_offset, start_pos),
                                  MIN (end_pos, seq_offset + piece_len - 1), 
                                  strand, sip);
        if (piece_loc != NULL)
        {
          if (last_loc == NULL)
          {
            loc_list = piece_loc;
          }
          else
          {
            last_loc->next = piece_loc;
          }
          last_loc = piece_loc;
        }
      }
      seq_offset += piece_len;
      dsp = dsp->next; 
    }
    loc_slp = SeqLocFindNext (slp, loc_slp);
  }
  
  if (loc_list == NULL)
  {
    /* failed to convert - do not change */
  }
  else if (loc_list->next == NULL)
  {
    /* only found one piece */
    slp = SeqLocFree (slp);
    slp = loc_list;
  }
  else
  {
    /* make mixed location */
    slp_new = ValNodeNew (NULL);
    if (slp_new != NULL)
    {
      slp_new->choice = SEQLOC_MIX;
      slp_new->data.ptrvalue = loc_list;
      slp = SeqLocFree (slp);
      slp = slp_new;
    }
  }
  return slp;
}

static SeqLocPtr RemoveGapsFromLocation (SeqLocPtr slp)
{
  BioseqPtr   bsp;
  SeqIdPtr    sip;
  
  if (slp == NULL)
  {
    return slp;
  }
  
  sip = SeqLocId (slp);
  if (sip == NULL)
  {
    return slp;
  }
  
  bsp = BioseqFind (sip);
  if (bsp == NULL)
  {
    return slp;
  }
  else if (bsp->repr == Seq_repr_seg)
  {
    return RemoveGapsFromSegmentedLocation (slp, bsp);
  }
  else if (bsp->repr == Seq_repr_delta
          && bsp->seq_ext_type == 4
          && bsp->seq_ext != NULL)
  {
    return RemoveGapsFromDeltaLocation (slp, bsp);
  }
  else
  {
    return slp;
  }
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* Apply_AddCDS () --                                                  */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void Apply_AddCDS (Uint2        entityID,
			  SeqEntryPtr  sep,
			  SeqEntryPtr  nsep,
			  ApplyFormPtr afp,
			  Boolean      suppressDups)
{
  ByteStorePtr       bs;
  BioseqPtr          bsp;
  Char               ch;
  CdRegionPtr        crp;
  ValNodePtr         descr;
  Uint1              frame;
  Int2               genCode;
  Int2               i;
  Int4               len;
  Int4               lens [4];
  Int4               max;
  MolInfoPtr         mip;
  SeqEntryPtr        old;
  CharPtr            prot;
  ProtRefPtr         prp;
  SeqEntryPtr        psep;
  CharPtr            ptr;
  SeqFeatPtr         sfp;
  SeqIdPtr           sip;
  Char               str [128];
  Char               desc_str [128];
  ValNodePtr         vnp;
  SeqEntryPtr        parent_sep;
  SeqEntryPtr        gene_sep;
  SeqFeatPtr         prot_sfp;

  /* If necessary then check for duplication before adding */

  if (suppressDups &&
      entityID > 0 &&
      AlreadyHasFeatOrDesc (sep, SEQFEAT_CDREGION, 0, 0))
    return;

  /* determine the parent of this sequence (for use when segmented) */
  parent_sep = NULL;
  if (IS_Bioseq (sep))
  {
    parent_sep = GetBestTopParentForData (entityID, sep->data.ptrvalue);
  }
  if (parent_sep == NULL)
  {
    parent_sep = sep;
  }

  /*Create a new CDS feature */

  genCode = SeqEntryToGeneticCode (parent_sep, NULL, NULL, 0);
  crp = CreateNewCdRgn (1, FALSE, genCode);
  if (NULL == crp)
    return;
  
  sfp = CreateNewFeature (nsep, NULL, SEQFEAT_CDREGION, NULL); 	

  if (NULL == sfp)
    return;
  
  sfp->data.value.ptrvalue = (Pointer) crp;

  /* Fill in the fields of the new CDS feature */

  SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
  if (GetValue (afp->strand_group) == 2) {
    /* reverse strand direction - strand direction is plus by default */
    setSeqFeatStrand (sfp, Seq_strand_minus);
  }
  sfp->partial = (afp->noLeft || afp->noRight);
  if (! TextHasNoText (afp->featcomment)) {
    sfp->comment = SaveStringFromTextAndStripNewlines (afp->featcomment);
  }
  
  if (afp->use_whole_interval != NULL
      && GetValue (afp->use_whole_interval) != 1)
  {
    /* adjust location to match coordinates from user */
    AdjustSeqLocForApply (sfp, afp);
  }  
  
  sfp->location = RemoveGapsFromLocation (sfp->location);

  if (GetValue (afp->reading_frame) < 1 || GetValue (afp->reading_frame) > 3)
  {
    max = 0;
    frame = 0;
    for (i = 1; i <= 3; i++) {
      crp->frame = (Uint1) i;
      bs = ProteinFromCdRegionEx (sfp, FALSE, FALSE);
      len = BSLen (bs);
      BSFree (bs);
      lens [i] = len;
      if (len > max) {
        max = len;
        frame = (Uint1) i;
      }
    }
    str [0] = '\0';
    if (IS_Bioseq (nsep)) {
      bsp = (BioseqPtr) nsep->data.ptrvalue;
      if (bsp != NULL) {
        sip = SeqIdFindBest (bsp->id, 0);
        SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str) - 1);
      }
    }
    for (i = 1; i <= 3; i++) {
      if (lens [i] == max && i != frame) {
        (afp->errcount)++;
        ValNodeCopyStr (&(afp->ambigList), 0, str);
      }
    }
    crp->frame = frame;
  }
  else
  {
    crp->frame = GetValue (afp->reading_frame);
  }

  /* Create corresponding protein sequence data for the CDS */

  bs = ProteinFromCdRegionEx (sfp, TRUE, FALSE);
  if (NULL == bs)
    return;

  prot = BSMerge (bs, NULL);
  bs = BSFree (bs);
  if (NULL == prot)
    return;

  ptr = prot;
  ch = *ptr;
  while (ch != '\0') {
    *ptr = TO_UPPER (ch);
    ptr++;
    ch = *ptr;
  }
  i = (Int2) StringLen (prot);
  if (i > 0 && prot [i - 1] == '*') {
    prot [i - 1] = '\0';
  }
  bs = BSNew (1000);
  if (bs != NULL) {
    ptr = prot;
    BSWrite (bs, (VoidPtr) ptr, (Int4) StringLen (ptr));
  }

  /* Create the product protein Bioseq */
  
  bsp = BioseqNew ();
  if (NULL == bsp)
    return;
  
  bsp->repr = Seq_repr_raw;
  bsp->mol = Seq_mol_aa;
  bsp->seq_data_type = Seq_code_ncbieaa;
  bsp->seq_data = bs;
  bsp->length = BSLen (bs);
  bs = NULL;
  old = SeqEntrySetScope (sep);
  bsp->id = MakeNewProteinSeqId (sfp->location, NULL);
  SeqMgrAddToBioseqIndex (bsp);
  SeqEntrySetScope (old);
  
  /* Create a new SeqEntry for the Prot Bioseq */
  
  psep = SeqEntryNew ();
  if (NULL == psep)
    return;
  
  psep->choice = 1;
  psep->data.ptrvalue = (Pointer) bsp;
  SeqMgrSeqEntry (SM_BIOSEQ, (Pointer) bsp, psep);
  
  /* Add a descriptor to the protein Bioseq */
  
  mip = MolInfoNew ();
  if (NULL == mip)
    return;
  
  mip->biomol = 8;
  mip->tech = 8;
  if (afp->noLeft && afp->noRight) {
    mip->completeness = 5;
  } else if (afp->noLeft) {
    mip->completeness = 3;
  } else if (afp->noRight) {
    mip->completeness = 4;
  }
  vnp = CreateNewDescriptor (psep, Seq_descr_molinfo);
  if (NULL == vnp)
    return;
  
  vnp->data.ptrvalue = (Pointer) mip;
  
  /**/
  
  descr = ExtractBioSourceAndPubs (parent_sep);

  AddSeqEntryToSeqEntry (parent_sep, psep, TRUE);
  nsep = FindNucSeqEntry (parent_sep);
  ReplaceBioSourceAndPubs (parent_sep, descr);
  SetSeqFeatProduct (sfp, bsp);
  
  /* create a full-length protein feature for the new protein sequence */
  GetTitle (afp->protName, str, sizeof (str));
  GetTitle (afp->protDesc, desc_str, sizeof (desc_str));
  if (! StringHasNoText (str) && ! StringHasNoText (desc_str))
  {
    prp = CreateNewProtRef (str, desc_str, NULL, NULL);
  }
  else if (!StringHasNoText (str))
  {
    prp = CreateNewProtRef (str, NULL, NULL, NULL);
  }
  else if (!StringHasNoText (desc_str))
  {
    prp = CreateNewProtRef (NULL, desc_str, NULL, NULL);
  }
  else
  { 
    prp = ProtRefNew ();
  }
  
  if (prp != NULL) {
    prot_sfp = CreateNewFeature (psep, NULL, SEQFEAT_PROT, NULL);
    if (prot_sfp != NULL) {
      prot_sfp->data.value.ptrvalue = (Pointer) prp;
      SetSeqLocPartial (prot_sfp->location, afp->noLeft, afp->noRight);
      prot_sfp->partial = (afp->noLeft || afp->noRight);
    }
  }

  /* Create a Gene ref feature on the nuc seq or segment */
  /* we can only create a feature where the sep->choice is 1 */
  if (sep->choice == 1)
  {
    gene_sep = sep;
  }
  else
  {
    gene_sep = nsep;
  }

  GetTitle (afp->geneName, str, sizeof (str));
  if (! StringHasNoText (str)) {
    if (entityID > 0 
        && suppressDups
        && AlreadyHasFeatOrDesc (gene_sep, SEQFEAT_GENE, 0, 0))
    {
      return;
    }

    ApplyGene (str, afp, gene_sep, sfp);
  }
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* RealApplyBioFeatToAll () --                                         */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void RealApplyBioFeatToAll (Uint2        entityID,
				   SeqEntryPtr  sep,
				   SeqEntryPtr  nsep,
                                   ApplyFormPtr afp,
				   Boolean      suppressDups)

{
  EnumFieldAssocPtr  ap;
  BioseqSetPtr       bssp;
  Int2               i;
  ImpFeatPtr         ifp;
  RnaRefPtr          rrp;
  ValNodePtr         sdp;
  SeqFeatPtr         sfp = NULL;
  Char               str [128];
  Int2               val;
  SeqFeatPtr         gene_sfp;
  Boolean            put_comment_on_gene = FALSE;

  /* Check parameters */

  if (sep == NULL || nsep == NULL || afp == NULL)
    return;

  /* Add a title feature */

  if (afp->type == ADD_TITLE) 
  {
    if (suppressDups && entityID > 0 
        && AlreadyHasFeatOrDesc (sep, 0, Seq_descr_title, 0))
    {
      return;
    }
    if (entityID == 0 && SeqEntryGetTitle (sep) != NULL)
    {
      return;
    }
    if (! TextHasNoText (afp->defline)) {
      if (IS_Bioseq_set (sep)) {
        bssp = (BioseqSetPtr) sep->data.ptrvalue;
        if (bssp != NULL && bssp->_class == 1 && bssp->seq_set != NULL) {
          sep = bssp->seq_set;
        }
      }
      sdp = CreateNewDescriptor (sep, Seq_descr_title);
      if (sdp != NULL) {
        sdp->data.ptrvalue =
	  (Pointer) SaveStringFromTextAndStripNewlines (afp->defline);
      }
    }
    return;
  }

  /* Add a CDS feature */

  else if (afp->type == ADD_CDS)
    Apply_AddCDS (entityID, sep, nsep, afp, suppressDups);

  /* Add an rRNA feature */

  else if (afp->type == ADD_RRNA) {
    if (suppressDups && entityID > 0 &&
	AlreadyHasFeatOrDesc (sep, SEQFEAT_RNA, 0, afp->rnatype)) return;
    rrp = RnaRefNew ();
    if (rrp != NULL) {
      rrp->type = afp->rnatype;
      if (! TextHasNoText (afp->rnaName)) {
        rrp->ext.choice = 1;
        rrp->ext.value.ptrvalue = SaveStringFromText (afp->rnaName);
      }
      sfp = CreateNewFeature (nsep, NULL, SEQFEAT_RNA, NULL);
      if (sfp != NULL) {
        sfp->data.value.ptrvalue = (Pointer) rrp;
        if (afp->use_whole_interval != NULL
            && GetValue (afp->use_whole_interval) != 1)
        {
          /* adjust location to match coordinates from user */
          AdjustSeqLocForApply (sfp, afp);
        }  

        SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
        if (GetValue (afp->strand_group) == 2) {
          /* reverse strand direction - strand direction is plus by default */
          setSeqFeatStrand (sfp, Seq_strand_minus);
        }
        sfp->partial = (afp->noLeft || afp->noRight);
        if (! TextHasNoText (afp->featcomment)) {
          sfp->comment =
	    SaveStringFromTextAndStripNewlines (afp->featcomment);
        }
      }
    }
    GetTitle (afp->geneName, str, sizeof (str));
    if (! StringHasNoText (str)) {
      if (entityID > 0 
        && suppressDups
	      && AlreadyHasFeatOrDesc (sep, SEQFEAT_GENE, 0, 0))
      {
	       return;        
      }
      ApplyGene (str, afp, nsep, sfp);

    }
  }

  /* Add an Import feature */

  else if (afp->type == ADD_IMP) {
    val = GetValue (afp->key);
    if (val <= 1)
    {
      put_comment_on_gene = TRUE;
    }
    else 
    {
      val --;
      ifp = ImpFeatNew ();
      if (ifp != NULL) {
        if (val > 0) {
          for (i = 1, ap = afp->alist; ap->name != NULL; i++, ap++) {
            if (i == val) {
              ifp->key = StringSave (ap->name);
            }
          }
        } else {
          ifp->key = StringSave ("misc_feature");
        }
        sfp = CreateNewFeature (nsep, NULL, SEQFEAT_IMP, NULL);
        if (sfp != NULL) {
          sfp->data.value.ptrvalue = (Pointer) ifp;
          if (afp->use_whole_interval != NULL
              && GetValue (afp->use_whole_interval) != 1)
          {
            /* adjust location to match coordinates from user */
            AdjustSeqLocForApply (sfp, afp);
          }  

          SetSeqLocPartial (sfp->location, afp->noLeft, afp->noRight);
          if (GetValue (afp->strand_group) == 2) {
            /* reverse strand direction - strand direction is plus by default */
            setSeqFeatStrand (sfp, Seq_strand_minus);
          }
          sfp->partial = (afp->noLeft || afp->noRight);
          if (! TextHasNoText (afp->featcomment)) 
          {
            sfp->comment =
	            SaveStringFromTextAndStripNewlines (afp->featcomment);
          }
          sfp->qual = DialogToPointer (afp->gbquals);
        }
      }
    }
    GetTitle (afp->geneName, str, sizeof (str));
    if (! StringHasNoText (str)) 
    {
      if (entityID > 0 
          && suppressDups
	        && AlreadyHasFeatOrDesc (sep, SEQFEAT_GENE, 0, 0)) 
      {
	      return;        
      }
      gene_sfp = ApplyGene (str, afp, nsep, sfp);

      if (gene_sfp != NULL && ! TextHasNoText (afp->featcomment) && put_comment_on_gene)
      {
        gene_sfp->comment = SaveStringFromTextAndStripNewlines (afp->featcomment);
      }
    }
  }
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* ApplyBioFeatToRaw () --                                             */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void ApplyBioFeatToRaw (Uint2        entityID,
			       SeqEntryPtr  parentSep,
			       SeqEntryPtr  sep,
			       ApplyFormPtr afp,
			       Int2         onlythis)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Int2          count;
  SeqEntryPtr   nucSep;

  /* Check parameters */

  if (sep == NULL || afp == NULL)
    return;

  /* If it is a set then recurse until we get to the raw Bioseq */

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL) {
      if (onlythis != 0 && bssp->_class == BioseqseqSet_class_parts) {
        for (sep = bssp->seq_set, count = 1;
             sep != NULL && count != onlythis;
             sep = sep->next, count++)
	  continue;
        if (sep != NULL) {
          ApplyBioFeatToRaw (entityID, parentSep, sep, afp, onlythis);
        }
      } else {
        for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
          ApplyBioFeatToRaw (entityID, parentSep, sep, afp, onlythis);
        }
      }
      return;
    }
  }

  /* Get the nucleotide Bioseq */

  nucSep = FindNucSeqEntry (sep);
  if (nucSep == NULL)
    return;

  bsp = (BioseqPtr) nucSep->data.ptrvalue;
  if (bsp == NULL)
    return;
  
  /* If we've got a raw Bioseq then do the apply */

  if (bsp->repr == Seq_repr_raw) {
    RealApplyBioFeatToAll (entityID, nucSep, nucSep, afp, FALSE);
    /*
    RealApplyBioFeatToAll (entityID, parentSep, nucSep, afp, FALSE);
    */
  }
}

/*---------------------------------------------------------------------*/
/*                                                                     */
/* ApplyBioFeatToAll () --                                             */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void ApplyBioFeatToAll (Uint2        entityID,
			       SeqEntryPtr  sep,
			       ApplyFormPtr afp)

{
  BioseqSetPtr  bssp;
  SeqEntryPtr   nsep;
  Int2          onlythis;
  Char          str [32];
  Boolean       suppressDups = FALSE;

  /* Check parameters */

  if (sep == NULL || afp == NULL)
    return;
  
  if (afp->add_to_seq_with_like_feature != NULL)
  {
    suppressDups = ! GetStatus (afp->add_to_seq_with_like_feature);
  }

  /* If it is a set then recurse until we get to a Bioseq */

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        ApplyBioFeatToAll (entityID, sep, afp);
      }
      return;
    }
  }

  /* Get the nucleotide Bioseq */

  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL)
    return;

  /* Apply the feature */

  if (afp->applyToParts != NULL && GetStatus (afp->applyToParts)) {
    GetTitle (afp->onlyThisPart, str, sizeof (str));
    if (! StrToInt (str, &onlythis)) {
      onlythis = 0;
    }
    ApplyBioFeatToRaw (entityID, sep, sep, afp, onlythis);
  } else {
    RealApplyBioFeatToAll (entityID, sep, nsep, afp, suppressDups);
  }
}

Int2 ApplyAnnotationToAll (Int2 type, SeqEntryPtr sep,
                           ButtoN partialLft, ButtoN partialRgt,
                           TexT geneName, TexT protName, 
                           TexT protDesc, TexT rnaName,
                           TexT featcomment, TexT defline)

{
  ApplyFormData  afd;

  MemSet ((Pointer) (&afd), 0, sizeof (ApplyFormData));
  afd.type = type;
  if (ADD_RRNA == type)
    afd.rnatype = 4;
  afd.errcount = 0;
  afd.ambigList = NULL;
  afd.partial5 = partialLft;
  afd.partial3 = partialRgt;
  afd.noLeft = GetStatus (afd.partial5);
  afd.noRight = GetStatus (afd.partial3);
  afd.geneName = geneName;
  afd.protName = protName;
  afd.protDesc = protDesc;
  afd.rnaName = rnaName;
  afd.featcomment = featcomment;
  afd.defline = defline;
  ApplyBioFeatToAll (0, sep, &afd);
  if (afd.type == ADD_CDS) {
    return afd.errcount;
  }
  return 0;
}

static void CommonApplyToAllProcBfpInfo (Uint2 entityID,
                                         Uint4 itemID,
                                         Uint2 itemtype,
                                         Int2 type);

static void NowReadyToApplyToAll (ApplyFormPtr afp, DialoG gbquals)

{
  Uint2        parenttype;
  Pointer      parentptr;
  CharPtr      plural;
  SeqEntryPtr  sep;
  CharPtr      tmp;
  SeqEntryPtr  top;
  UIEnum       val;
  ValNodePtr   vnp;
  Char         path [PATH_MAX];
  FILE         *fp;


  if (afp == NULL) return;
  afp->gbquals = gbquals;
  sep = GetTopSeqEntryForEntityID (afp->input_entityID);
  if (sep == NULL) return;
  Hide (afp->form);
  WatchCursor ();
  Update ();
  afp->noLeft = GetStatus (afp->partial5);
  afp->noRight = GetStatus (afp->partial3);
  top = sep;
  if (afp->type == ADD_CDS) {
    GetSeqEntryParent (top, &parentptr, &parenttype);
  }
  afp->rnatype = 0;
  if (afp->rnaSubType != NULL) {
    if (GetEnumPopup (afp->rnaSubType, rnax_subtype_alist, &val)) {
      afp->rnatype = val;
    }
  }
  ApplyBioFeatToAll (afp->input_entityID, sep, afp);
  ArrowCursor ();
  Update ();
  if (afp->errcount > 0 && afp->type == ADD_CDS) {
    TmpNam (path);
    fp = FileOpen (path, "w");
    if (fp != NULL) {
      if (afp->errcount > 1) {
        plural = "records";
      } else {
        plural = "record";
      }
      fprintf (fp, "Possible ambiguous frames detected in %d %s\n",
               (int) afp->errcount, plural);
      for (vnp = afp->ambigList; vnp != NULL; vnp = vnp->next) {
        tmp = (CharPtr) vnp->data.ptrvalue;
        fprintf (fp, "%s\n", tmp);
      }
      FileClose (fp);
      LaunchGeneralTextViewer (path, "Ambiguous Frames");
      FileRemove (path);
    }
  }
  ObjMgrSetDirtyFlag (afp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, afp->input_entityID, 0, 0);
  if (GetStatus (afp->leaveDlgUp))
  {
    afp->ambigList = ValNodeFreeData (afp->ambigList);
    Show (afp->form);
  }
  else
  {
    Remove (afp->form);
  }
}

typedef struct qualsform {
  FEATURE_FORM_BLOCK

  ApplyFormPtr  afp;
} QualsForm, PNTR QualsFormPtr;

static void CallNowReady (ButtoN b)

{
  QualsFormPtr  qfp;

  qfp = (QualsFormPtr) GetObjectExtra (b);
  if (qfp == NULL) return;
  Hide (qfp->form);
  NowReadyToApplyToAll (qfp->afp, qfp->gbquals);
  Remove (qfp->form);
}

static void DoTheApplyToAllProc (ButtoN b)

{
  ApplyFormPtr       afp;
  EnumFieldAssocPtr  ap;
  Int2               i;
  CharPtr            name;
  QualsFormPtr       qfp;
  Int2               val;
  WindoW             w;

  afp = GetObjectExtra (b);
  if (afp == NULL) {
    Remove (ParentWindow (b));
    return;
  }
  if (afp->type == ADD_IMP) {
    val = GetValue (afp->key);
    /* if gene, do not collect quals */
    if (val > 1) 
    {
      val--;
      qfp = (QualsFormPtr) MemNew (sizeof (QualsForm));
      if (qfp != NULL) {
        Hide (afp->form);
        Update ();
        name = NULL;
        if (val > 1) {
          for (i = 1, ap = afp->alist; ap->name != NULL; i++, ap++) {
            if (i == val) {
              name = ap->name;
            }
          }
        } else {
          name = "misc_feature";
        }
        qfp->afp = afp;
        w = FixedWindow (-50, -33, -10, -10, "Qualifiers", StdCloseWindowProc);
        SetObjectExtra (w, qfp, StdCleanupFormProc);
        qfp->form = (ForM) w;
        qfp->gbquals = CreateImportFields (w, name, NULL, FALSE);
        b = PushButton (w, "Okay", CallNowReady);
        SetObjectExtra (b, qfp, NULL);
        AlignObjects (ALIGN_CENTER, (HANDLE) qfp->gbquals, (HANDLE) b, NULL);
        RealizeWindow (w);
        Show (w);
        Select (w);
      }
    } else {
      NowReadyToApplyToAll (afp, NULL);
    }
  } else {
    NowReadyToApplyToAll (afp, NULL);
  }
}

static void ApplyToPartsProc (ButtoN b)

{
  ApplyFormPtr  afp;

  afp = (ApplyFormPtr) GetObjectExtra (b);
  if (afp == NULL) return;
  if (GetStatus (b)) {
    SafeEnable (afp->onlyThisPart);
  } else {
    SafeDisable (afp->onlyThisPart);
  }
}

static void LookForParts (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqSetPtr  bssp;
  BoolPtr       rsult;

  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && bssp->_class == BioseqseqSet_class_parts) {
      rsult = (BoolPtr) mydata;
      if (rsult != NULL) {
        *rsult = TRUE;
      }
    }
  }
}

extern Boolean HasPartsSet (SeqEntryPtr sep);
extern Boolean HasPartsSet (SeqEntryPtr sep)

{
  Boolean  rsult = FALSE;

  SeqEntryExplore (sep, (Pointer) (&rsult), LookForParts);
  return rsult;
}

static void EnableApplyCdsCoords (GrouP g)
{
  ApplyFormPtr afp;
  
  afp = (ApplyFormPtr) GetObjectExtra (g);
  if (afp == NULL) return;
  if (GetValue (g) == 1)
  {
  	Disable (afp->left_end);
  	Disable (afp->right_end);
  }
  else
  {
  	Enable (afp->left_end);
  	Enable (afp->right_end);
  }
}

static void ApplyMessageProc (ForM f, Int2 mssg)

{
  ApplyFormPtr  afp;

  afp = (ApplyFormPtr) GetObjectExtra (f);
  if (afp != NULL) {
    if (afp->appmessage != NULL) {
      afp->appmessage (f, mssg);
    }
  }
}

static void CleanupApplyToAllForm (GraphiC g, VoidPtr data)

{
  Int2          j;
  ApplyFormPtr  afp;

  afp = (ApplyFormPtr) data;
  if (afp != NULL) {
    if (afp->realalist != NULL) {
      for (j = 0; afp->realalist [j].name != NULL; j++) {
        MemFree (afp->realalist [j].name);
      }
    }
    MemFree (afp->realalist);
    ValNodeFreeData (afp->ambigList);
  }
  StdCleanupFormProc (g, data);
}

static void CommonApplyToAllProcBfpInfo (Uint2 entityID,
                                         Uint4 itemID,
                                         Uint2 itemtype,
                                         Int2 type)
{
  ApplyFormPtr       afp;
  EnumFieldAssocPtr  ap;
  ButtoN             b;
  GrouP              c;
  GrouP              g = NULL;
  GrouP              h;
  GrouP              r, r2, r3, r4;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  WindoW             w;
  GrouP              x;
  GrouP              parts_group = NULL;
  GrouP              feature_details = NULL;
  GrouP              indexer_only_group = NULL;
  GrouP              text_group = NULL;

  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return;
  afp = (ApplyFormPtr) MemNew (sizeof (ApplyFormData));
  if (afp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Automatic Processing", StdCloseWindowProc);
  SetObjectExtra (w, afp, CleanupApplyToAllForm);
  afp->form = (ForM) w;
  afp->formmessage = ApplyMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    afp->appmessage = sepp->handleMessages;
  }

  afp->input_entityID = entityID;
  afp->input_itemID = itemID;
  afp->input_itemtype = itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  afp->type = type;
  afp->errcount = 0;

  if (HasPartsSet (sep)) {
    parts_group = HiddenGroup (h, 1, 0, NULL);
    afp->applyToParts = CheckBox (parts_group, "Apply to segmented parts, not segmented sequence", ApplyToPartsProc);
    SetObjectExtra (afp->applyToParts, afp, NULL);
    x = HiddenGroup (parts_group, 2, 0, NULL);
    StaticPrompt (x, "Apply only to particular numbered segment", 0, dialogTextHeight, programFont, 'l');
    afp->onlyThisPart = DialogText (x, "", 4, NULL);
    Disable (afp->onlyThisPart);
  }

  afp->strand_group = NULL;
  afp->add_to_seq_with_like_feature = NULL;

  if (type == ADD_CDS || type == ADD_RRNA || type == ADD_IMP) {
    /* create group to hold feature details */
    feature_details = HiddenGroup (h, -1, 0, NULL);
    
    /* group for feature completeness */
    g = HiddenGroup (feature_details, 2, 0, NULL);
    afp->partial5 = CheckBox (g, "Incomplete at 5' end", NULL);
    afp->partial3 = CheckBox (g, "Incomplete at 3' end", NULL);
    
    /* group for strand */
    afp->strand_group = HiddenGroup (feature_details, 2, 0, NULL);
    SetObjectExtra (afp->strand_group, afp, NULL);
    RadioButton (afp->strand_group, "Plus Strand");
    RadioButton (afp->strand_group, "Minus Strand");
    SetValue (afp->strand_group, 1);
    
    /* coordinates */
    if (indexerVersion)
    {
      indexer_only_group = HiddenGroup (feature_details, -1, 0, NULL);
      r2 = HiddenGroup (indexer_only_group, 5, 0, NULL);
      afp->use_whole_interval = HiddenGroup (r2, 0, 2, EnableApplyCdsCoords);
      SetObjectExtra (afp->use_whole_interval, afp, NULL);
      RadioButton (afp->use_whole_interval, "Use Whole Sequence Interval");
      RadioButton (afp->use_whole_interval, "Use these coordinates:");
      r3 = HiddenGroup (r2, 0, 2, NULL);
      StaticPrompt (r3, "", 0, dialogTextHeight, programFont, 'l');
      r4 = HiddenGroup (r3, 4, 0, NULL);
      StaticPrompt (r4, "From", 0, dialogTextHeight, programFont, 'l');
      afp->left_end = DialogText (r4, "1", 5, NULL);
      StaticPrompt (r4, "To", 0, dialogTextHeight, programFont, 'l');
      afp->right_end = DialogText (r4, "1", 5, NULL);
      SetValue (afp->use_whole_interval, 1);
      Disable (afp->left_end);
      Disable (afp->right_end);
      
      /* add to features that already have same */
      if (type == ADD_CDS)
      {
        afp->add_to_seq_with_like_feature = CheckBox (indexer_only_group, "Also add to sequences that already have a CDS", NULL);
      }
      else if (type == ADD_RRNA)
      {
        afp->add_to_seq_with_like_feature = CheckBox (indexer_only_group, "Also add to sequences that already have an rRNA", NULL);
      }
      else if (type == ADD_IMP)
      {
        afp->add_to_seq_with_like_feature = CheckBox (indexer_only_group, "Also add to sequences that already have this feature", NULL);
      }
      SafeSetStatus (afp->add_to_seq_with_like_feature, TRUE);
      AlignObjects (ALIGN_CENTER, (HANDLE) r2,
                                   (HANDLE) afp->add_to_seq_with_like_feature,
                                   NULL);
    }
    else
    {
      afp->use_whole_interval = NULL;
    }
    
    /* codon start controls */
    if (type == ADD_CDS)
    {
      r = HiddenGroup (feature_details, 2, 0, NULL);
      StaticPrompt (r, "Codon Start", 0, dialogTextHeight, programFont, 'l');
      afp->reading_frame = PopupList (r, TRUE, NULL);
      PopupItem (afp->reading_frame, "1");
      PopupItem (afp->reading_frame, "2");
      PopupItem (afp->reading_frame, "3");
      PopupItem (afp->reading_frame, "Best");
      SetValue (afp->reading_frame, 4);
    }
    else if (type == ADD_RRNA) 
    {
      r = HiddenGroup (feature_details, 2, 0, NULL);
      StaticPrompt (r, "RNA subtype", 0, dialogTextHeight, programFont, 'l');
      afp->rnaSubType = PopupList (r, TRUE, NULL);
      InitEnumPopup (afp->rnaSubType, rnax_subtype_alist, NULL);
      SetEnumPopup (afp->rnaSubType, rnax_subtype_alist, (UIEnum) 4);
    }
    else
    {
      r = NULL;
    }
    
    if (indexer_only_group == NULL)
    {
      AlignObjects (ALIGN_CENTER, (HANDLE) g,
                                  (HANDLE) afp->strand_group,
                                  (HANDLE) r,
                                  NULL);
    } 
    else
    {
      AlignObjects (ALIGN_CENTER, (HANDLE) g,
                                  (HANDLE) afp->strand_group,
                                  (HANDLE) indexer_only_group,
                                  (HANDLE) r,
                                  NULL);
    }
  }

  text_group = HiddenGroup (h, 0, 2, NULL);
  if (type == ADD_TITLE) {
    StaticPrompt (text_group, "Title", 0, 0, programFont, 'c');
    afp->defline = ScrollText (text_group, 20, 4, programFont, TRUE, NULL);
  } else {
    text_group = HiddenGroup (h, 2, 0, NULL);
    if (type == ADD_CDS) {
      StaticPrompt (text_group, "Gene Symbol", 0, dialogTextHeight, programFont, 'l');
      afp->geneName = DialogText (text_group, "", 20, NULL);
      StaticPrompt (text_group, "Protein Name", 0, dialogTextHeight, programFont, 'l');
      afp->protName = DialogText (text_group, "", 20, NULL);
      StaticPrompt (text_group, "Protein Description", 0, dialogTextHeight, programFont, 'l');
      afp->protDesc = DialogText (text_group, "", 20, NULL);
    } else if (type == ADD_RRNA) {
      StaticPrompt (text_group, "RNA Name", 0, dialogTextHeight, programFont, 'l');
      afp->rnaName = DialogText (text_group, "", 20, NULL);
      StaticPrompt (text_group, "Gene Symbol", 0, dialogTextHeight, programFont, 'l');
      afp->geneName = DialogText (text_group, "", 20, NULL);
    } else if (type == ADD_IMP) {
      ap = import_featdef_alist (FALSE, FALSE, FALSE);
      SortEnumFieldAssocPtrArray (ap, CompareImpFeatEnumFieldAssoc);
      afp->realalist = ap;
      ap++;
      afp->alist = ap;
      StaticPrompt (text_group, "Type", 0, 6 * Nlm_stdLineHeight, programFont, 'l');
      afp->key = SingleList (text_group, 12, 6, NULL);
      ListItem (afp->key, "Gene");
      for (ap = afp->alist; ap->name != NULL; ap++) {
        ListItem (afp->key, ap->name);
      }
      SetValue (afp->key, 1);
      StaticPrompt (text_group, "Gene Symbol", 0, dialogTextHeight, programFont, 'l');
      afp->geneName = DialogText (text_group, "", 20, NULL);
    }
    StaticPrompt (text_group, "Comment", 0, 4 * Nlm_stdLineHeight, programFont, 'l');
    afp->featcomment = ScrollText (text_group, 20, 4, programFont, TRUE, NULL);
  }
  afp->gbquals = NULL;

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoTheApplyToAllProc);
  SetObjectExtra (b, afp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  afp->leaveDlgUp = CheckBox (c, "Leave Dialog Up", NULL);

  if (parts_group == NULL)
  {
    if (feature_details == NULL)
    {
      AlignObjects (ALIGN_CENTER, (HANDLE) text_group, (HANDLE) c, NULL);
    }
    else
    {
      AlignObjects (ALIGN_CENTER, (HANDLE) feature_details, 
                                  (HANDLE) text_group,
                                  (HANDLE) c,
                                  NULL);
    }
  }
  else
  {
    if (feature_details == NULL)
    {
      AlignObjects (ALIGN_CENTER, (HANDLE) parts_group,
                                  (HANDLE) text_group, 
                                  (HANDLE) c, 
                                  NULL);
    }
    else
    {
      AlignObjects (ALIGN_CENTER, (HANDLE) parts_group,
                                  (HANDLE) feature_details, 
                                  (HANDLE) text_group,
                                  (HANDLE) c,
                                  NULL);
    }
  }
  
  RealizeWindow (w);
  Show (w);
  if (type == ADD_TITLE) {
    Select (afp->defline);
  } else if (type == ADD_RRNA) {
    Select (afp->rnaName);
  } else {
    Select (afp->geneName);
  }
  Update ();
}

extern void CommonApplyToAllProc (BaseFormPtr bfp, Int2 type)
{
  if (bfp == NULL) return;
  CommonApplyToAllProcBfpInfo (bfp->input_entityID,
                               bfp->input_itemID,
                               bfp->input_itemtype, type);
}

static void CommonApplyToAllProcMenuItem (IteM i, Int2 type)
{
  BaseFormPtr bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  CommonApplyToAllProc (bfp, type);
}

extern void ApplyTitle (IteM i)

{
  CommonApplyToAllProcMenuItem (i, ADD_TITLE);
}

extern void ApplyCDS (IteM i)

{
  CommonApplyToAllProcMenuItem (i, ADD_CDS);
}

extern void ApplyRRNA (IteM i)

{
  CommonApplyToAllProcMenuItem (i, ADD_RRNA);
}

extern void ApplyImpFeat (IteM i)

{
  CommonApplyToAllProcMenuItem (i, ADD_IMP);
}


#define SUBMISSION_PAGE   0
#define CONTACT_PAGE      1
#define AUTHOR_PAGE       2
#define AFFILIATION_PAGE  3

typedef struct submitform {
  FORM_MESSAGE_BLOCK
  GrouP           pages [4];
  Int2            currentPage;
  DialoG          tbs;

  GrouP           hup;
  GrouP           dateGrp;

  TexT            title;
  DialoG          reldate;
  TexT            firstname;
  TexT            middleinit;
  TexT            lastname;
  PopuP           suffix;
  DialoG          phonefaxemail;
  DialoG          authors;
  TexT            consortium;
  DialoG          affil;

  Boolean         visitedContact;
  Boolean         visitedAuthor;

  ButtoN          nextBtn;
  ButtoN          prevBtn;
  BtnActnProc     goToNext;
  BtnActnProc     goToPrev;
} SubmitForm, PNTR SubmitFormPtr;

static AuthListPtr AddConsortiumToAuthList (AuthListPtr alp, TexT consortium)

{
  AuthorPtr    ap;
  ValNodePtr   names;
  PersonIdPtr  pid;

  if (TextHasNoText (consortium)) return alp;
  if (alp == NULL) {
    alp = AuthListNew ();
    alp->choice = 1;
  }
  pid = PersonIdNew ();
  if (pid == NULL) return NULL;
  pid->choice = 5;
  pid->data = SaveStringFromText (consortium);
  ap = AuthorNew ();
  if (ap == NULL) return NULL;
  ap->name = pid;
  names = ValNodeAdd (&(alp->names));
  names->choice = 1;
  names->data.ptrvalue = ap;
  return alp;
}

static void AuthListToConsortium (AuthListPtr alp, TexT consortium)

{
  AuthorPtr    ap;
  ValNodePtr   names;
  PersonIdPtr  pid;
  CharPtr      str;

  if (alp == NULL || consortium == NULL) return;
  if (alp->choice != 1) return;
  for (names = alp->names; names != NULL; names = names->next) {
    ap = names->data.ptrvalue;
    if (ap == NULL) continue;
    pid = ap->name;
    if (pid == NULL || pid->choice != 5) continue;
    str = (CharPtr) pid->data;
    SafeSetTitle (consortium, str);
  }
}

static void SequinBlockPtrToSubmitForm (ForM f, Pointer data)

{
  AuthorPtr       ap;
  DatePtr         dp;
  NameStdPtr      nsp;
  PersonIdPtr     pid;
  SubmitFormPtr   sbfp;
  SequinBlockPtr  sbp;
  CharPtr         str;
  CharPtr         txt;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  sbp = (SequinBlockPtr) data;
  if (sbfp != NULL) {
    if (sbp != NULL) {
      SafeSetTitle (sbfp->title, sbp->citsubtitle);
      PointerToDialog (sbfp->reldate, (Pointer) sbp->releasedate);
      ap = sbp->contactperson;
      if (ap != NULL) {
        pid = ap->name;
        if (pid != NULL && pid->choice == 2) {
          nsp = pid->data;
          if (nsp != NULL) {
            str = NameStdPtrToAuthorSpreadsheetString (nsp);
            if (str != NULL) {
              txt = ExtractTagListColumn (str, 0);
              SafeSetTitle (sbfp->firstname, txt);
              MemFree (txt);
              txt = ExtractTagListColumn (str, 1);
              SafeSetTitle (sbfp->middleinit, txt);
              MemFree (txt);
              txt = ExtractTagListColumn (str, 2);
              SafeSetTitle (sbfp->lastname, txt);
              MemFree (txt);
              txt = ExtractTagListColumn (str, 3);
              if (! StringHasNoText (txt)) {
                SetEnumPopupByName (sbfp->suffix, name_suffix_alist, txt);
              }
              /*
              SafeSetTitle (sbfp->suffix, txt);
              */
              MemFree (txt);
              MemFree (str);
            }
          }
        }
        PointerToDialog (sbfp->phonefaxemail, (Pointer) ap->affil);
      }
      PointerToDialog (sbfp->authors, (Pointer) sbp->citsubauthors);
      AuthListToConsortium (sbp->citsubauthors, sbfp->consortium);
      PointerToDialog (sbfp->affil, (Pointer) sbp->citsubaffil);
      if (sbp->holduntilpublished) {
        SafeSetValue (sbfp->hup, 2);
        SafeShow (sbfp->dateGrp);
      } else {
        SafeSetValue (sbfp->hup, 1);
        SafeHide (sbfp->dateGrp);
      }
      sbfp->visitedAuthor = TRUE;
      SetValue (sbfp->tbs, 0);
    } else {
      SafeSetTitle (sbfp->title, NULL);
      dp = DateCurr ();
      if (dp != NULL) {
        dp->data [3] = 0; /* force to end of month */
        dp = DateAdvance (dp, 12);
        /*
        (dp->data [1])++;
        if (dp->data [2] == 2 && dp->data [3] > 28) {
          dp->data [3] = 28;
        }
        */
        PointerToDialog (sbfp->reldate, (Pointer) dp);
      } else {
        PointerToDialog (sbfp->reldate, NULL);
      }
      DateFree (dp);
      SafeSetTitle (sbfp->firstname, "");
      SafeSetTitle (sbfp->middleinit, "");
      SafeSetTitle (sbfp->lastname, "");
      PointerToDialog (sbfp->phonefaxemail, NULL);
      PointerToDialog (sbfp->authors, NULL);
      SafeSetTitle (sbfp->consortium, "");
      PointerToDialog (sbfp->affil, NULL);
      SafeSetValue (sbfp->hup, 1);
      SafeHide (sbfp->dateGrp);
      SetValue (sbfp->tbs, 0);
    }
  }
}

static Pointer SubmitFormToSequinBlockPtr (ForM f)

{
  AffilPtr        affil;
  AuthorPtr       ap;
  NameStdPtr      nsp;
  PersonIdPtr     pid;
  SubmitFormPtr   sbfp;
  SequinBlockPtr  sbp;
  Char            sfx [32];
  Char            str [128];
  Uint2           suffixVal;
  CharPtr         txt;

  sbp = NULL;
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    sbp = (SequinBlockPtr) MemNew (sizeof (SequinBlock));
    if (sbp != NULL) {
      sbp->citsubtitle = SaveStringFromTextAndStripNewlines (sbfp->title);
      ap = AuthorNew ();
      if (ap != NULL) {
        pid = PersonIdNew ();
        ap->name = pid;
        if (pid != NULL) {
          pid->choice = 2;
          str [0] = '\0';
          txt = SaveStringFromText (sbfp->firstname);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          txt = SaveStringFromText (sbfp->middleinit);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          txt = SaveStringFromText (sbfp->lastname);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          suffixVal = GetValue (sbfp->suffix);
          sprintf (sfx, "%d", (int) (suffixVal - 1));
          StringCat (str, sfx);
          StringCat (str, "\n");
          txt = StringSave (str);
          nsp = AuthorSpreadsheetStringToNameStdPtr (txt);
          MemFree (txt);
          pid->data = nsp;
          if (nsp != NULL) {
            if (StringHasNoText (nsp->names [0])) {
              ap = AuthorFree (ap);
            }
          }
        }
        affil = (AffilPtr) DialogToPointer (sbfp->phonefaxemail);
        if (affil != NULL) {
          if (affil->choice == 2) {
            affil->affil = MemFree (affil->affil);
            affil->div = MemFree (affil->div);
            affil->city = MemFree (affil->city);
            affil->sub = MemFree (affil->sub);
            affil->country = MemFree (affil->country);
            affil->street = MemFree (affil->street);
            affil->postal_code = MemFree (affil->postal_code);
            if (affil->phone == NULL && affil->fax == NULL &&
                affil->email == NULL) {
              affil = AffilFree (affil);
            }
          } else {
            affil = AffilFree (affil);
          }
        }
        ap->affil = affil;
      }
      sbp->contactperson = ap;
      sbp->citsubauthors = (AuthListPtr) DialogToPointer (sbfp->authors);
      sbp->citsubauthors = AddConsortiumToAuthList (sbp->citsubauthors, sbfp->consortium);
      sbp->citsubaffil = (AffilPtr) DialogToPointer (sbfp->affil);
      if (GetValue (sbfp->hup) == 2) {
        sbp->holduntilpublished = TRUE;
        sbp->releasedate = (DatePtr) DialogToPointer (sbfp->reldate);
      }
      if (sbp->contactperson == NULL &&
          sbp->citsubauthors == NULL &&
          sbp->citsubaffil == NULL) {
        sbp = SequinBlockFree (sbp);
      }
    }
  }
  return (Pointer) sbp;
}

static void SubmitBlockPtrToSubmitForm (ForM f, Pointer data)

{
  AuthorPtr       ap;
  AuthListPtr     authors;
  ContactInfoPtr  cip;
  CitSubPtr       csp;
  DatePtr         dp;
  NameStdPtr      nsp;
  PersonIdPtr     pid;
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;
  CharPtr         str;
  CharPtr         txt;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  sbp = (SubmitBlockPtr) data;
  if (sbfp != NULL) {
    if (sbp != NULL) {
      PointerToDialog (sbfp->reldate, (Pointer) sbp->reldate);
      cip = sbp->contact;
      if (cip != NULL) {
        ap = cip->contact;
        if (ap != NULL) {
          pid = ap->name;
          if (pid != NULL && pid->choice == 2) {
            nsp = pid->data;
            if (nsp != NULL) {
              str = NameStdPtrToAuthorSpreadsheetString (nsp);
              if (str != NULL) {
                txt = ExtractTagListColumn (str, 0);
                SafeSetTitle (sbfp->firstname, txt);
                MemFree (txt);
                txt = ExtractTagListColumn (str, 1);
                SafeSetTitle (sbfp->middleinit, txt);
                MemFree (txt);
                txt = ExtractTagListColumn (str, 2);
                SafeSetTitle (sbfp->lastname, txt);
                MemFree (txt);
                txt = ExtractTagListColumn (str, 3);
                if (! StringHasNoText (txt)) {
                  SetEnumPopupByName (sbfp->suffix, name_suffix_alist, txt);
                }
                /*
                SafeSetTitle (sbfp->suffix, txt);
                */
                MemFree (txt);
                MemFree (str);
              }
            }
          }
          PointerToDialog (sbfp->phonefaxemail, (Pointer) ap->affil);
        }
      }
      csp = sbp->cit;
      if (csp != NULL) {
        authors = csp->authors;
        if (authors != NULL) {
          PointerToDialog (sbfp->authors, (Pointer) authors);
          AuthListToConsortium (authors, sbfp->consortium);
          PointerToDialog (sbfp->affil, (Pointer) authors->affil);
        }
      }
      if (sbp->hup) {
        SafeSetValue (sbfp->hup, 2);
        SafeShow (sbfp->dateGrp);
      } else {
        SafeSetValue (sbfp->hup, 1);
        SafeHide (sbfp->dateGrp);
      }
      sbfp->visitedAuthor = TRUE;
      SetValue (sbfp->tbs, 0);
    } else {
      SafeSetTitle (sbfp->title, NULL);
      dp = DateCurr ();
      if (dp != NULL) {
        dp->data [3] = 0; /* force to end of month */
        dp = DateAdvance (dp, 12);
        PointerToDialog (sbfp->reldate, (Pointer) dp);
      } else {
        PointerToDialog (sbfp->reldate, NULL);
      }
      DateFree (dp);
      SafeSetTitle (sbfp->firstname, "");
      SafeSetTitle (sbfp->middleinit, "");
      SafeSetTitle (sbfp->lastname, "");
      SafeSetValue (sbfp->suffix, 0);
      PointerToDialog (sbfp->phonefaxemail, NULL);
      PointerToDialog (sbfp->authors, NULL);
      SafeSetTitle (sbfp->consortium, "");
      PointerToDialog (sbfp->affil, NULL);
      SafeSetValue (sbfp->hup, 1);
      SafeHide (sbfp->dateGrp);
      SetValue (sbfp->tbs, 0);
    }
  }
}

static ValNodePtr TestSubmitForm (ForM f)

{
  AffilPtr       affil;
  AuthListPtr    authors;
  DatePtr        dp;
  ValNodePtr     head;
  SubmitFormPtr  sbfp;

  head = NULL;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {

    if (TextHasNoText (sbfp->firstname)) {
      head = AddStringToValNodeChain (head, "first name", 1);
    }
    if (TextHasNoText (sbfp->lastname)) {
      head = AddStringToValNodeChain (head, "last name", 1);
    }
    affil = DialogToPointer (sbfp->phonefaxemail);
    if (affil != NULL) {
      if (StringHasNoText (affil->phone)) {
        head = AddStringToValNodeChain (head, "telephone number", 0);
      }
      if (StringHasNoText (affil->fax)) {
        head = AddStringToValNodeChain (head, "fax number", 0);
      }
      if (StringHasNoText (affil->email)) {
        head = AddStringToValNodeChain (head, "e-mail address", 0);
      }
    } else {
      head = AddStringToValNodeChain (head, "telephone number", 0);
      head = AddStringToValNodeChain (head, "fax number", 0);
      head = AddStringToValNodeChain (head, "e-mail address", 0);
    }
    affil = AffilFree (affil);

    if (GetValue (sbfp->hup) == 2) {
      dp = DialogToPointer (sbfp->reldate);
      if (dp == NULL) {
        head = AddStringToValNodeChain (head, "release date", 1);
      }
      dp = DateFree (dp);
    }
    if (TextHasNoText (sbfp->title)) {
      head = AddStringToValNodeChain (head, "manuscript title", 1);
    }

    authors = DialogToPointer (sbfp->authors);
    authors = AddConsortiumToAuthList (authors, sbfp->consortium);
    if (authors == NULL) {
      head = AddStringToValNodeChain (head, "author names", 1);
    }
    authors = AuthListFree (authors);
    affil = DialogToPointer (sbfp->affil);
    if (affil == NULL) {
      head = AddStringToValNodeChain (head, "affiliation", 1);
    }
    affil = AffilFree (affil);

  }

  return head;
}

/*
static Boolean ReadSubmitBlock (ForM f, CharPtr filename)

{
  AsnIoPtr        aip;
  Char            path [PATH_MAX];
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      aip = AsnIoOpen (path, "r");
      if (aip != NULL) {
        sbp = SubmitBlockAsnRead (aip, NULL);
        AsnIoClose (aip);
        if (sbp != NULL) {
          SubmitBlockPtrToSubmitForm (f, (Pointer) sbp);
          sbp = SubmitBlockFree (sbp);
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}
*/

static Boolean ReadSubmitBlock (ForM f, CharPtr filename)

{
  Pointer         dataptr;
  Uint2           datatype;
  Uint2           entityID;
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;
  SeqSubmitPtr    ssp;
  Char            path [PATH_MAX];

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      dataptr = ObjMgrGenericAsnTextFileRead (path, &datatype, &entityID);
      if (dataptr != NULL && entityID > 0) {
        sbp = NULL;
        switch (datatype) {
          case OBJ_SUBMIT_BLOCK :
            sbp = (SubmitBlockPtr) dataptr;
            break;
          case OBJ_SEQSUB :
            ssp = (SeqSubmitPtr) dataptr;
            if (ssp != NULL) {
              sbp = ssp->sub;
            }
            break;
          default :
            break;
        }
        if (sbp != NULL) {
          SubmitBlockPtrToSubmitForm (f, sbp);
        }
        ObjMgrDelete (datatype, dataptr);
        if (sbp != NULL) {
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static SeqDescrPtr MakeUnpubPub (CharPtr title, AuthListPtr alp, AffilPtr affil)

{
  CitGenPtr    cgp;
  PubdescPtr   pdp;
  ValNodePtr   pep;
  SeqDescrPtr  vnp;

  if (StringHasNoText (title) || alp == NULL) return NULL;
  pdp = PubdescNew ();
  if (pdp == NULL) return NULL;
  vnp = SeqDescrNew (NULL);
  if (vnp == NULL) return NULL;
  vnp->choice = Seq_descr_pub;
  vnp->data.ptrvalue = (Pointer) pdp;
  pdp->reftype = 0;
  pep = ValNodeNew (NULL);
  pdp->pub = pep;
  if (pep != NULL) {
    cgp = CitGenNew ();
    if (cgp != NULL) {
      pep->choice = PUB_Gen;
      pep->data.ptrvalue = cgp;
      cgp->cit = StringSave ("unpublished");
      cgp->authors = alp;
      if (alp != NULL) {
        alp->affil = affil;
        if (affil != NULL) {
          affil->phone = MemFree (affil->phone);
          affil->fax = MemFree (affil->fax);
          affil->email = MemFree (affil->email);
        }
      }
      cgp->title = StringSave (title);
    }
  }
  return vnp;
}

extern SubmitBlockPtr ConvertSequinBlockToSubmitBlock (SequinBlockPtr sqp);

NLM_EXTERN void AsnPrintNewLine PROTO((AsnIoPtr aip));

static Boolean WriteSubmitBlock (ForM f, CharPtr filename)

{
  AffilPtr        affil;
  AsnIoPtr        aip;
  AuthListPtr     alp;
  Char            path [PATH_MAX];
  SubmitFormPtr   sbfp;
  SubmitBlockPtr  sbp;
  SeqDescrPtr     sdp;
  SequinBlockPtr  sqp;
  CharPtr         title;
#ifdef WIN_MAC
  FILE            *fp;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        FileClose (fp);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      sqp = (SequinBlockPtr) SubmitFormToSequinBlockPtr (f);
      if (sqp != NULL) {
        title = StringSaveNoNull (sqp->citsubtitle);
        alp = AsnIoMemCopy ((Pointer) sqp->citsubauthors,
                            (AsnReadFunc) AuthListAsnRead,
                            (AsnWriteFunc) AuthListAsnWrite);
        affil = AsnIoMemCopy ((Pointer) sqp->citsubaffil,
                              (AsnReadFunc) AffilAsnRead,
                              (AsnWriteFunc) AffilAsnWrite);
        sbp = ConvertSequinBlockToSubmitBlock (sqp);
        if (sbp != NULL) {
          aip = AsnIoOpen (path, "w");
          if (aip != NULL) {
            sbp->tool = MemFree (sbp->tool);
            SubmitBlockAsnWrite (sbp, aip, NULL);
            AsnPrintNewLine (aip);
            sdp = MakeUnpubPub (title, alp, affil);
            if (sdp != NULL) {
              AsnIoReset (aip);
              SeqDescAsnWrite (sdp, aip, NULL);
              AsnPrintNewLine (aip);
              SeqDescFree (sdp);
            }
            AsnIoClose (aip);
            sbp = SubmitBlockFree (sbp);
            return TRUE;
          }
        }
      }
    }
  }
  return FALSE;
}

static Boolean ReadContactPage (ForM f, CharPtr filename)

{
  AsnIoPtr        aip;
  AuthorPtr       ap;
  ContactInfoPtr  cip;
  NameStdPtr      nsp;
  Char            path [PATH_MAX];
  PersonIdPtr     pid;
  SubmitFormPtr   sbfp;
  CharPtr         str;
  CharPtr         txt;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      aip = AsnIoOpen (path, "r");
      if (aip != NULL) {
        cip = ContactInfoAsnRead (aip, NULL);
        AsnIoClose (aip);
        if (cip != NULL) {
          ap = cip->contact;
          if (ap != NULL) {
            pid = ap->name;
            if (pid != NULL && pid->choice == 2) {
              nsp = pid->data;
              if (nsp != NULL) {
                str = NameStdPtrToAuthorSpreadsheetString (nsp);
                if (str != NULL) {
                  txt = ExtractTagListColumn (str, 0);
                  SafeSetTitle (sbfp->firstname, txt);
                  MemFree (txt);
                  txt = ExtractTagListColumn (str, 1);
                  SafeSetTitle (sbfp->middleinit, txt);
                  MemFree (txt);
                  txt = ExtractTagListColumn (str, 2);
                  SafeSetTitle (sbfp->lastname, txt);
                  MemFree (txt);
                  txt = ExtractTagListColumn (str, 3);
                  if (! StringHasNoText (txt)) {
                    SetEnumPopupByName (sbfp->suffix, name_suffix_alist, txt);
                  }
                  MemFree (txt);
                  MemFree (str);
                }
              }
            }
            PointerToDialog (sbfp->phonefaxemail, (Pointer) ap->affil);
          }
          cip = ContactInfoFree (cip);
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean WriteContactPage (ForM f, CharPtr filename)

{
  AffilPtr        affil;
  AsnIoPtr        aip;
  AuthorPtr       ap;
  ContactInfoPtr  cip;
  NameStdPtr      nsp;
  Char            path [PATH_MAX];
  PersonIdPtr     pid;
  SubmitFormPtr   sbfp;
  Char            sfx [32];
  Char            str [128];
  Uint2           suffixVal;
  CharPtr         txt;
#ifdef WIN_MAC
  FILE            *fp;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        FileClose (fp);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      aip = AsnIoOpen (path, "w");
      if (aip != NULL) {
        cip = ContactInfoNew ();
        if (cip != NULL) {
          ap = AuthorNew ();
          if (ap != NULL) {
            pid = PersonIdNew ();
            ap->name = pid;
            if (pid != NULL) {
              pid->choice = 2;
              str [0] = '\0';
              txt = SaveStringFromText (sbfp->firstname);
              StringCat (str, txt);
              StringCat (str, "\t");
              MemFree (txt);
              txt = SaveStringFromText (sbfp->middleinit);
              StringCat (str, txt);
              StringCat (str, "\t");
              MemFree (txt);
              txt = SaveStringFromText (sbfp->lastname);
              StringCat (str, txt);
              StringCat (str, "\t");
              MemFree (txt);
              suffixVal = GetValue (sbfp->suffix);
              sprintf (sfx, "%d", (int) (suffixVal - 1));
              StringCat (str, sfx);
              StringCat (str, "\n");
              txt = StringSave (str);
              nsp = AuthorSpreadsheetStringToNameStdPtr (txt);
              MemFree (txt);
              pid->data = nsp;
              if (nsp != NULL) {
                if (StringHasNoText (nsp->names [0])) {
                  ap = AuthorFree (ap);
                }
              }
            }
            affil = (AffilPtr) DialogToPointer (sbfp->phonefaxemail);
            if (affil != NULL) {
              if (affil->choice == 2) {
                affil->affil = MemFree (affil->affil);
                affil->div = MemFree (affil->div);
                affil->city = MemFree (affil->city);
                affil->sub = MemFree (affil->sub);
                affil->country = MemFree (affil->country);
                affil->street = MemFree (affil->street);
                affil->postal_code = MemFree (affil->postal_code);
                if (affil->phone == NULL && affil->fax == NULL &&
                    affil->email == NULL) {
                  affil = AffilFree (affil);
                }
              } else {
                affil = AffilFree (affil);
              }
            }
            ap->affil = affil;
          }
          cip->contact = ap;
          ContactInfoAsnWrite (cip, aip, NULL);
          AsnIoClose (aip);
          cip = ContactInfoFree (cip);
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean ReadAuthListPage (ForM f, CharPtr filename)

{
  AsnIoPtr       aip;
  AuthListPtr    alp;
  Char           path [PATH_MAX];
  SubmitFormPtr  sbfp;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      aip = AsnIoOpen (path, "r");
      if (aip != NULL) {
        alp = AuthListAsnRead (aip, NULL);
        AsnIoClose (aip);
        if (alp != NULL) {
          PointerToDialog (sbfp->authors, alp);
          AuthListToConsortium (alp, sbfp->consortium);
          alp = AuthListFree (alp);
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean WriteAuthListPage (ForM f, CharPtr filename)

{
  AsnIoPtr       aip;
  AuthListPtr    alp;
  Char           path [PATH_MAX];
  SubmitFormPtr  sbfp;
#ifdef WIN_MAC
  FILE           *fp;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        FileClose (fp);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      aip = AsnIoOpen (path, "w");
      if (aip != NULL) {
        alp = DialogToPointer (sbfp->authors);
        alp = AddConsortiumToAuthList (alp, sbfp->consortium);
        AuthListAsnWrite (alp, aip, NULL);
        AsnIoClose (aip);
        alp = AuthListFree (alp);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static Boolean ReadAffilPage (ForM f, CharPtr filename)

{
  AffilPtr       affil;
  AsnIoPtr       aip;
  Char           path [PATH_MAX];
  SubmitFormPtr  sbfp;

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetInputFileName (path, sizeof (path), "", "TEXT")) {
      aip = AsnIoOpen (path, "r");
      if (aip != NULL) {
        affil = AffilAsnRead (aip, NULL);
        AsnIoClose (aip);
        if (affil != NULL) {
          affil->phone = MemFree (affil->phone);
          affil->fax = MemFree (affil->fax);
          affil->email = MemFree (affil->email);
          PointerToDialog (sbfp->affil, affil);
          affil = AffilFree (affil);
          Update ();
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static Boolean WriteAffilPage (ForM f, CharPtr filename)

{
  AffilPtr       affil;
  AsnIoPtr       aip;
  Char           path [PATH_MAX];
  SubmitFormPtr  sbfp;
#ifdef WIN_MAC
  FILE           *fp;
#endif

  path [0] = '\0';
  StringNCpy_0 (path, filename, sizeof (path));
  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    if (path [0] != '\0' || GetOutputFileName (path, sizeof (path), NULL)) {
#ifdef WIN_MAC
      fp = FileOpen (path, "r");
      if (fp != NULL) {
        FileClose (fp);
      } else {
        FileCreate (path, "TEXT", "ttxt");
      }
#endif
      aip = AsnIoOpen (path, "w");
      if (aip != NULL) {
        affil = DialogToPointer (sbfp->affil);
        affil->phone = MemFree (affil->phone);
        affil->fax = MemFree (affil->fax);
        affil->email = MemFree (affil->email);
        AffilAsnWrite (affil, aip, NULL);
        AsnIoClose (aip);
        affil = AffilFree (affil);
        return TRUE;
      }
    }
  }
  return FALSE;
}

static Boolean ImportSubmitForm (ForM f, CharPtr filename)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    switch (sbfp->currentPage) {
      case SUBMISSION_PAGE :
        return ReadSubmitBlock (f, filename);
      case CONTACT_PAGE :
        return ReadContactPage (f, filename);
      case AUTHOR_PAGE :
        return ReadAuthListPage (f, filename);
      case AFFILIATION_PAGE :
        return ReadAffilPage (f, filename);
      default :
        break;
    }
  }
  return FALSE;
}

static Boolean ExportSubmitForm (ForM f, CharPtr filename)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    switch (sbfp->currentPage) {
      case SUBMISSION_PAGE :
        return WriteSubmitBlock (f, filename);
      case CONTACT_PAGE :
        return WriteContactPage (f, filename);
      case AUTHOR_PAGE :
        return WriteAuthListPage (f, filename);
      case AFFILIATION_PAGE :
        return WriteAffilPage (f, filename);
      default :
        break;
    }
  }
  return FALSE;
}

static void CopyContactToAuthors (SubmitFormPtr sbfp)

{
  AuthListPtr  alp;
  AuthorPtr    ap;
  ValNodePtr   names;
  NameStdPtr   nsp;
  PersonIdPtr  pid;
  Char         sfx [32];
  Char         str [128];
  Uint2        suffixVal;
  CharPtr      txt;

  if (sbfp == NULL) return;
  ap = NULL;
  alp = AuthListNew ();
  if (alp != NULL) {
    alp->choice = 1;
    names = ValNodeNew (NULL);
    alp->choice = 1;
    alp->names = names;
    if (names != NULL) {
      ap = AuthorNew ();
      if (ap != NULL) {
        pid = PersonIdNew ();
        ap->name = pid;
        if (pid != NULL) {
          pid->choice = 2;
          str [0] = '\0';
          txt = SaveStringFromText (sbfp->firstname);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          txt = SaveStringFromText (sbfp->middleinit);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          txt = SaveStringFromText (sbfp->lastname);
          StringCat (str, txt);
          StringCat (str, "\t");
          MemFree (txt);
          suffixVal = GetValue (sbfp->suffix);
          sprintf (sfx, "%d", (int) (suffixVal - 1));
          /*
          txt = SaveStringFromText (sbfp->suffix);
          MemFree (txt);
          */
          StringCat (str, sfx);
          StringCat (str, "\n");
          txt = StringSave (str);
          nsp = AuthorSpreadsheetStringToNameStdPtr (txt);
          MemFree (txt);
          pid->data = nsp;
          if (nsp != NULL) {
            if (StringHasNoText (nsp->names [0])) {
              ap = AuthorFree (ap);
            }
          }
        }
      }
      names->choice = 1;
      names->data.ptrvalue = ap;
    }
    if (ap == NULL) {
      alp = AuthListFree (alp);
    }
    if (alp != NULL) {
       PointerToDialog (sbfp->authors, (Pointer) alp);
       AuthListToConsortium (alp, sbfp->consortium);
    }
  }
  alp = AuthListFree (alp);
}

static void SetSubmitterImportExportItems (SubmitFormPtr sbfp)

{
  IteM  exportItm;
  IteM  importItm;

  if (sbfp != NULL) {
    importItm = FindFormMenuItem ((BaseFormPtr) sbfp, VIB_MSG_IMPORT);
    exportItm = FindFormMenuItem ((BaseFormPtr) sbfp, VIB_MSG_EXPORT);
    switch (sbfp->currentPage) {
      case SUBMISSION_PAGE :
        SafeSetTitle (importItm, "Import Submitter Info...");
        SafeSetTitle (exportItm, "Export Submitter Info...");
        SafeEnable (importItm);
        SafeEnable (exportItm);
        break;
      case CONTACT_PAGE :
        SafeSetTitle (importItm, "Import Contact...");
        SafeSetTitle (exportItm, "Export Contact...");
        SafeEnable (importItm);
        SafeEnable (exportItm);
        break;
      case AUTHOR_PAGE :
        SafeSetTitle (importItm, "Import Authors...");
        SafeSetTitle (exportItm, "Export Authors...");
        SafeEnable (importItm);
        SafeEnable (exportItm);
        break;
      case AFFILIATION_PAGE :
        SafeSetTitle (importItm, "Import Affiliation...");
        SafeSetTitle (exportItm, "Export Affiliation...");
        SafeEnable (importItm);
        SafeEnable (exportItm);
        break;
      default :
        break;
    }
  }
}

static void EnterSubmitPage (SubmitFormPtr sbfp, Int2 page)

{
  AuthListPtr  alp;

  if (sbfp != NULL) {
    switch (page) {
      case SUBMISSION_PAGE :
        Select (sbfp->title);
        SafeSetTitle (sbfp->prevBtn, "<< Prev Form");
        SafeSetTitle (sbfp->nextBtn, "Next Page >>");
        break;
      case CONTACT_PAGE :
        Select (sbfp->firstname);
        sbfp->visitedContact = TRUE;
        SafeSetTitle (sbfp->prevBtn, "<< Prev Page");
        SafeSetTitle (sbfp->nextBtn, "Next Page >>");
        break;
      case AUTHOR_PAGE :
        alp = (AuthListPtr) DialogToPointer (sbfp->authors);
        alp = AddConsortiumToAuthList (alp, sbfp->consortium);
        if (sbfp->visitedContact && alp == NULL) {
          CopyContactToAuthors (sbfp);
        }
        AuthListFree (alp);
        /*
        if (sbfp->visitedContact && (! sbfp->visitedAuthor)) {
          CopyContactToAuthors (sbfp);
        }
        */
        SendMessageToDialog (sbfp->authors, VIB_MSG_ENTER);
        sbfp->visitedAuthor = TRUE;
        SafeSetTitle (sbfp->prevBtn, "<< Prev Page");
        SafeSetTitle (sbfp->nextBtn, "Next Page >>");
        break;
      case AFFILIATION_PAGE :
        SendMessageToDialog (sbfp->affil, VIB_MSG_ENTER);
        SafeSetTitle (sbfp->prevBtn, "<< Prev Page");
        SafeSetTitle (sbfp->nextBtn, "Next Form >>");
        break;
      default :
        break;
    }
  }
}

static void ChangeSubmitFormPage (VoidPtr data, Int2 newval, Int2 oldval)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) data;
  if (sbfp != NULL) {
    sbfp->currentPage = newval;
    SafeHide (sbfp->pages [oldval]);
    Update ();
#ifdef WIN_MAC
    EnterSubmitPage (sbfp, newval);
#endif
    SetSubmitterImportExportItems (sbfp);
    SafeShow (sbfp->pages [newval]);
#ifndef WIN_MAC
    EnterSubmitPage (sbfp, newval);
#endif
    Update ();
    switch (newval) {
      case SUBMISSION_PAGE :
        SendHelpScrollMessage (helpForm, "Submitting Authors Form", "Submission Page");
        break;
      case CONTACT_PAGE :
        SendHelpScrollMessage (helpForm, "Submitting Authors Form", "Contact Page");
        break;
      case AUTHOR_PAGE :
        SendHelpScrollMessage (helpForm, "Submitting Authors Form", "Authors Page");
        break;
      case AFFILIATION_PAGE :
        SendHelpScrollMessage (helpForm, "Submitting Authors Form", "Affiliation Page");
        break;
      default :
        break;
    }
  }
}

static void NextSubmitFormBtn (ButtoN b)

{
  SubmitFormPtr  sbfp;
  CharPtr        txt;
  Boolean        ok = TRUE;

  sbfp = (SubmitFormPtr) GetObjectExtra (b);
  if (sbfp != NULL) {
    if (sbfp->currentPage == 0)
    {
      txt = SaveStringFromText (sbfp->title);
      if (StringHasNoText (txt))
      {
      	Message (MSG_ERROR, "You must supply a tentative title for your manuscript.");
      	MemFree (txt);
      	return;
      }
      MemFree (txt);
    }
    else if (sbfp->currentPage == 1)
    {
      txt = SaveStringFromText (sbfp->firstname);
      if (StringHasNoText (txt))
      {
      	ok = FALSE;
      }
      MemFree (txt);
      txt = SaveStringFromText (sbfp->lastname);
      if (StringHasNoText (txt))
      {
      	ok = FALSE;
      }
      MemFree (txt);
      if (!ok)
      {
      	Message (MSG_ERROR, "You must supply a first and last name for the contact.");
      	return;
      }
    }
    if (sbfp->currentPage < 3) {
      SetValue (sbfp->tbs, sbfp->currentPage + 1);
    } else if (sbfp->goToNext != NULL) {
      (sbfp->goToNext) (b);
    }
  }
}

static void PrevSubmitFormBtn (ButtoN b)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (b);
  if (sbfp != NULL) {
    if (sbfp->currentPage > 0) {
      SetValue (sbfp->tbs, sbfp->currentPage - 1);
    } else if (sbfp->goToPrev != NULL) {
      (sbfp->goToPrev) (b);
    }
  }
}

static void ChangeHup (GrouP g)

{
  Boolean        hup;
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (g);
  if (sbfp != NULL) {
    hup = (Boolean) (GetValue (sbfp->hup) == 2);
    if (hup) {
      SafeShow (sbfp->dateGrp);
    } else {
      SafeHide (sbfp->dateGrp);
    }
  }
}

static CharPtr  submitFormTabs [] = {
  "Submission", "Contact", "Authors", "Affiliation", NULL
};

static void SubmitFormMessage (ForM f, Int2 mssg)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (f);
  if (sbfp != NULL) {
    switch (mssg) {
      case VIB_MSG_IMPORT :
        ImportSubmitForm (f, NULL);
        break;
      case VIB_MSG_EXPORT :
        ExportSubmitForm (f, NULL);
        break;
      case VIB_MSG_CUT :
        StdCutTextProc (NULL);
        break;
      case VIB_MSG_COPY :
        StdCopyTextProc (NULL);
        break;
      case VIB_MSG_PASTE :
        StdPasteTextProc (NULL);
        break;
      case VIB_MSG_DELETE :
        StdDeleteTextProc (NULL);
        break;
      default :
        if (sbfp->appmessage != NULL) {
          sbfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void InitSubmitterFormActivate (WindoW w)

{
  SubmitFormPtr  sbfp;

  sbfp = (SubmitFormPtr) GetObjectExtra (w);
  if (sbfp != NULL) {
    if (sbfp->activate != NULL) {
      sbfp->activate (w);
    }
    SetSubmitterImportExportItems (sbfp);
  }
}

extern ForM CreateInitSubmitterForm (Int2 left, Int2 top, CharPtr title,
                                     BtnActnProc goToNext,
                                     BtnActnProc goBack,
                                     WndActnProc activateForm)

{
  GrouP              c;
  GrouP              g;
  GrouP              h;
  GrouP              j;
  GrouP              m;
  GrouP              n;
  GrouP              q;
  SubmitFormPtr      sbfp;
  StdEditorProcsPtr  sepp;
  GrouP              t;
  WindoW             w;
  GrouP              x;
  GrouP              z;
  GrouP              g1, g2;

  w = NULL;
  sbfp = MemNew (sizeof (SubmitForm));
  if (sbfp != NULL) {
    w = FixedWindow (left, top, -10, -10, title, NULL);
    SetObjectExtra (w, sbfp, StdCleanupFormProc);
    sbfp->form = (ForM) w;
    sbfp->toform = SequinBlockPtrToSubmitForm;
    sbfp->fromform = SubmitFormToSequinBlockPtr;
    sbfp->testform = TestSubmitForm;
    sbfp->importform = ImportSubmitForm;
    sbfp->exportform = ExportSubmitForm;
    sbfp->formmessage = SubmitFormMessage;

#ifndef WIN_MAC
    CreateSqnInitialFormMenus (w);
#endif

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      sbfp->appmessage = sepp->handleMessages;
    }

    SetGroupSpacing (w, 10, 10);

    j = HiddenGroup (w, -1, 0, NULL);
    SetGroupSpacing (j, 10, 10);

    sbfp->tbs = CreateFolderTabs (j, submitFormTabs, 0, 0, 0,
                                  SYSTEM_FOLDER_TAB,
                                  ChangeSubmitFormPage, (Pointer) sbfp);
    sbfp->currentPage = SUBMISSION_PAGE;

    sbfp->visitedContact = FALSE;
    sbfp->visitedAuthor = FALSE;

    h = HiddenGroup (w, 0, 0, NULL);

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    m = HiddenGroup (q, -1, 0, NULL);
    g = HiddenGroup (m, 0, -4, NULL);
    SetGroupSpacing (g, 3, 10);
    StaticPrompt (g, "When may we release your sequence record?",
                  0, stdLineHeight, programFont, 'l');
    sbfp->hup = HiddenGroup (g, 0, -2, ChangeHup);
    SetObjectExtra (sbfp->hup, sbfp, NULL);
    RadioButton (sbfp->hup, "Immediately After Processing");
    RadioButton (sbfp->hup, "Release Date:");
    SetValue (sbfp->hup, 1);
    sbfp->dateGrp = HiddenGroup (m, 10, 0, NULL);
    /* StaticPrompt (sbfp->dateGrp, "Release Date: ", 0, popupMenuHeight, programFont, 'l'); */
    sbfp->reldate = CreateDateDialog (sbfp->dateGrp, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) sbfp->dateGrp, NULL);
    Hide (sbfp->dateGrp);
    t = HiddenGroup (q, 1, 0, NULL);
    StaticPrompt (t, "Tentative title for manuscript (required)", 0, 0, programFont, 'c');
    sbfp->title = ScrollText (t, 25, 4, programFont, TRUE, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) sbfp->reldate,
                  (HANDLE) sbfp->title, (HANDLE) t, NULL);
    sbfp->pages [SUBMISSION_PAGE] = q;
    Hide (sbfp->pages [SUBMISSION_PAGE]);

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    n = HiddenGroup (q, 4, 0, NULL);
    SetGroupSpacing (n, -1, 2);
    StaticPrompt (n, "First Name", 0, 0, programFont, 'c');
    StaticPrompt (n, "M.I.", 0, 0, programFont, 'c');
    StaticPrompt (n, "Last Name", 0, 0, programFont, 'c');
    StaticPrompt (n, "Sfx", 0, 0, programFont, 'c');
    sbfp->firstname = DialogText (n, "", 8, NULL);
    sbfp->middleinit = DialogText (n, "", 4, NULL);
    sbfp->lastname = DialogText (n, "", 9, NULL);
    /*
    sbfp->suffix = DialogText (n, "", 3, NULL);
    sbfp->suffix = PopupList (n, TRUE, SuffixPopup_Callback);
    */
    sbfp->suffix = PopupList (n, TRUE, NULL);
    SetObjectExtra (sbfp->suffix, sbfp, NULL);
    InitEnumPopup (sbfp->suffix, name_suffix_alist, NULL);
    SetEnumPopup (sbfp->suffix, name_suffix_alist, 0);

    sbfp->phonefaxemail = CreateExtAffilDialog (q, NULL, NULL, &x);
    Show (x);
    Show (sbfp->phonefaxemail);
    AlignObjects (ALIGN_CENTER, (HANDLE) n, (HANDLE) sbfp->phonefaxemail, NULL);
    sbfp->pages [CONTACT_PAGE] = q;
    Hide (sbfp->pages [CONTACT_PAGE]);

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    sbfp->authors = CreateAuthorDialog (q, 3, -1);
    z = HiddenGroup (q, 0, 2, NULL);
    g1 = HiddenGroup (z, 2, 0, NULL);
    StaticPrompt (g1, "Consortium", 0, stdLineHeight, programFont, 'l');
    sbfp->consortium = DialogText (g1, "", 16, NULL);
    g2 = HiddenGroup (z, 1, 0, NULL);
    MultiLinePrompt (g2, "The consortium field should be used when a "
                         "consortium is responsible for the sequencing or "
                         "publication of the data.  Individual authors may "
                         "be listed along with a consortium name.",
                          25 * stdCharWidth, programFont);
    AlignObjects (ALIGN_CENTER, (HANDLE) g1, (HANDLE) g2, NULL);
    AlignObjects (ALIGN_CENTER, (HANDLE) sbfp->authors, (HANDLE) z, NULL);
    sbfp->pages [AUTHOR_PAGE] = q;
    Hide (sbfp->pages [AUTHOR_PAGE]);

    q = HiddenGroup (h, -1, 0, NULL);
    SetGroupSpacing (q, 10, 20);
    sbfp->affil = CreateExtAffilDialog (q, NULL, &g, NULL);
    Show (g);
    Show (sbfp->affil);
    sbfp->pages [AFFILIATION_PAGE] = q;
    Hide (sbfp->pages [AFFILIATION_PAGE]);

    c = HiddenGroup (w, 4, 0, NULL);
    SetGroupSpacing (c, 10, 2);
    sbfp->goToPrev = goBack;
    sbfp->prevBtn = PushButton (c, " << Prev Form ", PrevSubmitFormBtn);
    SetObjectExtra (sbfp->prevBtn, sbfp, NULL);
    sbfp->goToNext = goToNext;
    sbfp->nextBtn = PushButton (c, " Next Page >> ", NextSubmitFormBtn);
    SetObjectExtra (sbfp->nextBtn, sbfp, NULL);

    AlignObjects (ALIGN_CENTER,
                  (HANDLE) sbfp->pages [SUBMISSION_PAGE],
                  (HANDLE) sbfp->pages [CONTACT_PAGE],
                  (HANDLE) sbfp->pages [AUTHOR_PAGE],
                  (HANDLE) sbfp->pages [AFFILIATION_PAGE],
                  (HANDLE) sbfp->tbs, (HANDLE) c, NULL);

    RealizeWindow (w);

    SafeSetTitle (sbfp->prevBtn, "<< Prev Form");
    SafeSetTitle (sbfp->nextBtn, "Next Page >>");

    sbfp->activate = activateForm;
    SetActivate (w, InitSubmitterFormActivate);

    Show (sbfp->pages [sbfp->currentPage]);
    EnterSubmitPage (sbfp, sbfp->currentPage);
  }
  return (ForM) w;
}

extern void AddCodonListTotRNA (tRNAPtr trna, ValNodePtr codons)
{
  ValNodePtr vnp;
  Int2       j, k, q;
  Char       str [8];
  Char       ch;
  Uint1      codon [4]; 
  Uint1      code;

  if (trna == NULL) return;

  for (j = 0; j < 6; j++) {
    trna->codon [j] = 255;
  }
  
  if (codons == NULL)
  {
    return;
  }
  
  for (vnp = codons, j = 0; vnp != NULL && j < 6; vnp = vnp->next) {
    str [0] = '\0';
    StringNCpy_0 (str, (CharPtr) vnp->data.ptrvalue, sizeof (str));
    if (str [0] != '\0') {
      k = 0;
      q = 0;
      ch = str [k];
      while (ch != '\0' && q < 3) {
        ch = TO_UPPER (ch);
        if (StringChr ("ACGTU", ch) != NULL) {
          if (ch == 'U') {
            ch = 'T';
          }
          codon [q] = (Uint1) ch;
          q++;
        }
        k++;
        ch = str [k];
      }
      codon [q] = 0;
      if (q == 3) {
        code = IndexForCodon (codon, Seq_code_iupacna);
        if (code != INVALID_RESIDUE) {
          trna->codon [j] = code;
        }
      }
      j++;
    }
  }
}


static void ParseCodonsFromtRNACommentProc (SeqFeatPtr sfp, Pointer userdata)
{
  RnaRefPtr rrp;
  tRNAPtr   trna;
  CharPtr   cp;
  CharPtr   codon_name;
  Char      codon[4];
  Int4      k, q;
  Char      ch;
  Uint1     code;

  if (sfp == NULL 
      || sfp->comment == NULL
      || sfp->data.choice != SEQFEAT_RNA
      || (rrp = (RnaRefPtr)sfp->data.value.ptrvalue) == NULL
      || rrp->type != 3 || rrp->ext.choice != 2) {
    return;
  }
  trna = rrp->ext.value.ptrvalue;
  if (trna == NULL) return;

  cp = StringStr (sfp->comment, "recognized codon");
  if (cp == NULL) {
    cp = StringStr (sfp->comment, "codon recognized");
  }
  if (cp == NULL) return;
  cp = StringStr (cp, "=");
  if (cp == NULL) {
    Message (MSG_ERROR, "Unable to read codon from %s", sfp->comment);
    return;
  }

  codon_name = cp + 1;

  q = 0;
  for (k = 0; q < 3 && codon_name[k] != 0; k++) {
    ch = TO_UPPER (codon_name [k]);
    if (StringChr ("ACGTU", ch) != NULL) {
      if (ch == 'U') {
        ch = 'T';
      }
      codon [q] = (Uint1) ch;
      q++;
    }
  }
  codon [q] = 0;
  if (q == 3) {
    code = IndexForCodon ((Uint1Ptr) codon, Seq_code_iupacna);
    if (code != INVALID_RESIDUE) {
      trna->codon[0] = code;
      for (k = 1; k < 6; k++) {
        trna->codon[k] = 255;
      }
    } else {
      Message (MSG_ERROR, "Invalid codon in %s", sfp->comment);
    }
  } else {
    Message (MSG_ERROR, "Invalid codon in %s", sfp->comment);
  }
}

extern void ParseCodonsFromtRNAComment (IteM i)
{
  BaseFormPtr bfp;
  SeqEntryPtr sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitFeaturesInSep (sep, NULL, ParseCodonsFromtRNACommentProc);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

static Boolean GetAntiCodonPositionFromComment (CharPtr comment, Int4Ptr start, Int4Ptr stop)
{
  CharPtr cp, cpend;
  Int4    val;
  Char    ch;
  
  if (comment == NULL || start == NULL || stop == NULL) return FALSE;
  cp = StringStr (comment, "anticodon");
  if (cp == NULL) return FALSE;
  cp += 8;
  while (*cp != 0 && !isdigit ((Int4)(*cp)))
  {
  	cp++;
  }
  if (*cp == 0)
  {
  	Message (MSG_ERROR, "Found 'anticodon' but no numbers for position in %s", comment);
  	return FALSE;
  }
  
  cpend = cp + 1;
  while (*cpend != 0 && isdigit ((Int4)(*cpend)))
  {
  	cpend ++;
  }
  if (*cpend == 0)
  {
  	Message (MSG_ERROR, "Found 'anticodon' but no numbers for position in %s", comment);
  	return FALSE;
  }
  ch = *cpend;
  *cpend = 0;
  val = atoi (cp);
  *cpend = ch;
  cp = cpend + 1;
  
  while (*cp != 0 && !isdigit ((Int4)(*cp)))
  {
  	cp++;
  }
  if (*cp == 0)
  {
  	Message (MSG_ERROR, "Found 'anticodon' but no numbers for position in %s", comment);
  	return FALSE;
  }
  cpend = cp + 1;
  while (*cpend != 0 && isdigit ((Int4)(*cpend)))
  {
  	cpend ++;
  }
  ch = *cpend;
  *cpend = 0;
  *start = val;
  *stop = atoi (cp);
  *cpend = ch;
  return TRUE;
}

static void ParseAntiCodonsFromtRNACommentProc (SeqFeatPtr sfp, Pointer userdata)
{
  RnaRefPtr rrp;
  tRNAPtr   trna;
  Int4      start;
  Int4      stop;
  SeqLocPtr slp;
  SeqIntPtr sip;

  if (sfp == NULL 
      || sfp->comment == NULL
      || sfp->data.choice != SEQFEAT_RNA
      || (rrp = (RnaRefPtr)sfp->data.value.ptrvalue) == NULL
      || rrp->type != 3 || rrp->ext.choice != 2) {
    return;
  }
  trna = rrp->ext.value.ptrvalue;
  if (trna == NULL) return;

  if (! GetAntiCodonPositionFromComment (sfp->comment, &start, &stop)) return;
  sip = SeqIntNew ();
  if (sip == NULL) return;
  if (start <= stop) {
    sip->from = start - 1;
    sip->to = stop - 1;
  } else {
    sip->from = stop - 1;
    sip->to = start - 1;
  }
  sip->strand = SeqLocStrand (sfp->location);
  sip->id = SeqIdDup (SeqLocId (sfp->location));
  slp = ValNodeNew (NULL);
  if (slp == NULL) return;
  slp->choice = 4;
  slp->data.ptrvalue = sip;

  if (trna->anticodon != NULL) {
    SeqLocFree (trna->anticodon);
  }
  trna->anticodon = slp;
}

extern void ParseAntiCodonsFromtRNAComment (IteM i)
{
  BaseFormPtr bfp;
  SeqEntryPtr sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitFeaturesInSep (sep, NULL, ParseAntiCodonsFromtRNACommentProc);
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

typedef struct orgnamechangedata {
  CharPtr oldname;
  CharPtr newname;
} OrgNameChangeData, PNTR OrgNameChangePtr;

static void ReplaceOldOrgNameInTitle (SeqDescrPtr sdp, OrgNameChangePtr oncp)
{
  CharPtr oldtitle, newtitle, orgnamestart;
  Int4    oldorglen;

  if (sdp == NULL || oncp == NULL
    || oncp->oldname == NULL || oncp->newname == NULL)
  {
    return;
  }

  oldtitle = sdp->data.ptrvalue;

  if (oldtitle == NULL) return;

  orgnamestart = StringRChr (oldtitle, '[');
  if (orgnamestart == NULL) return;
  oldorglen = StringLen (oncp->oldname);
  if (StringNCmp (orgnamestart + 1, oncp->oldname, oldorglen) == 0
    && orgnamestart [ oldorglen + 1] == ']')
  {
    newtitle = MemNew (StringLen (oldtitle) - StringLen (oncp->oldname) + StringLen (oncp->newname) + 2);   
    if (newtitle == NULL) return;
    StringNCpy (newtitle, oldtitle, orgnamestart - oldtitle + 1);
    StringCat (newtitle, oncp->newname);
    StringCat (newtitle, "]");
    MemFree (sdp->data.ptrvalue);
    sdp->data.ptrvalue = newtitle;
  }
}

static void FixProteinTitleAfterOrganismNameChange (BioseqPtr bsp, Pointer userdata)
{
  OrgNameChangePtr oncp;
  SeqDescrPtr sdp;
  BioSourcePtr biop;

  if (bsp == NULL
    || ! ISA_aa (bsp->mol)
    || (oncp = (OrgNameChangePtr)userdata) == NULL
    || oncp->oldname == NULL
    || oncp->newname == NULL)
  {
    return;
  }

  for (sdp = bsp->descr; sdp != NULL; sdp = sdp->next) {
    if (sdp->choice == Seq_descr_title)
    {
      ReplaceOldOrgNameInTitle (sdp, oncp);
    }
    else if (sdp->choice == Seq_descr_source)
    {
      biop = sdp->data.ptrvalue;
      if (biop != NULL && biop->org != NULL 
        && StringCmp (biop->org->taxname, oncp->oldname) == 0)
      {
        SetTaxNameAndRemoveTaxRef (biop->org, StringSave (oncp->newname));
      }   
    }   
  }
}

typedef struct parseformdata {
  FEATURE_FORM_BLOCK

  TexT           atleft;
  TexT           atright;
  PopuP          orgmod;
  PopuP          subsource;
  PopuP          taxname;
  Boolean        parsedef;
  Char           path [PATH_MAX];
  FILE           *fp;
  Boolean        replaceOldAsked;
  Boolean        doReplaceAll;
  Boolean        use_semicolon;
  ButtoN         leaveDlgUp;
} ParseFormData, PNTR ParseFormPtr;

typedef struct localidfinddata {
  CharPtr str;
  Int4    str_size;
} LocalIDFindData, PNTR LocalIDFindPtr;
  
static void LookForLocalID (BioseqPtr bsp, Pointer userdata)
{
  LocalIDFindPtr lidfp;
  SeqIdPtr      sip;

  if (bsp == NULL || (lidfp = (LocalIDFindPtr) userdata) == NULL) {
    return;
  }
  if (lidfp->str [0] != 0) return;

  for (sip = bsp->id; sip != NULL; sip = sip->next) {
    if (sip->choice == SEQID_LOCAL) {
      SeqIdWrite (sip, lidfp->str, PRINTID_REPORT, lidfp->str_size);
      if (StringNICmp (lidfp->str, "tmpseq_", 7) == 0 ||
          StringNICmp (lidfp->str, "segseq_", 7) == 0 ||
          StringNICmp (lidfp->str, "SEG_dna", 7) == 0)
      {
        /* don't want to use this one */
        lidfp->str [0] = 0;
      } else {
        return;
      }
    }
  }
}

static void FindLocalIDForOrganism (
  SeqEntryPtr  sep,
  CharPtr      str,
  Int4         str_size
)
{
  LocalIDFindData lidfd;

  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  lidfd.str = str;
  lidfd.str_size = str_size;
  VisitBioseqsInSep (sep, (Pointer) &lidfd, LookForLocalID);
}

static void GetDataForOrganism (
  SeqEntryPtr  sep,
  CharPtr      str,
  Int4         str_size,
  CharPtr      useme,
  ParseFormPtr pfp
)
{
  SeqEntryPtr nsep;
  ValNodePtr    ttl;
  Char          txt [128];
  CharPtr       ptr;

  str [0] = 0;
  if (sep == NULL || pfp == NULL) return;
  nsep = FindNucSeqEntry (sep);
  if (nsep == NULL || nsep->data.ptrvalue == NULL) return;

  if (useme != NULL) {
    StringNCpy_0 (str, useme, str_size);
  } else if (pfp->parsedef) {
    ttl = SeqEntryGetSeqDescr (nsep, Seq_descr_title, NULL);
    if (ttl == NULL && sep != nsep) {
      ttl = SeqEntryGetSeqDescr (sep, Seq_descr_title, NULL);
    }
    if (ttl == NULL || ttl->data.ptrvalue == NULL) return;
    StringNCpy_0 (str, (CharPtr) ttl->data.ptrvalue, str_size);
    GetTitle (pfp->atleft, txt, sizeof (txt));
    if (! StringHasNoText (txt)) {
      ptr = StringISearch (str, txt);
      if (ptr != NULL) {
        StringNCpy_0 (str, ptr + StringLen (txt), str_size);
      }
    }
    GetTitle (pfp->atright, txt, sizeof (txt));
    if (txt [0] != '\0' /* ! StringHasNoText (txt) */) {
      ptr = StringISearch (str, txt);
      if (ptr != NULL) {
        *ptr = '\0';
      }
    }
  } else {
    FindLocalIDForOrganism (nsep, str, str_size);
    if (str [0] == 0) {
      FindLocalIDForOrganism (sep, str, str_size);
    }
  }
  return;
}

static void ApplyDataToBioSourcePtr (
  ParseFormPtr pfp,
  BioSourcePtr biop,
  BioSourcePtr topbiop,
  SeqEntryPtr  sep,
  CharPtr      data
)
{
  Int2          taxnameval;
  ValNodePtr    vnp;
  OrgRefPtr     orp;
  OrgModPtr     mod;
  Uint1         orgmodtype;
  Uint1         subsourcetype;
  UIEnum        val;
  Uint1         subtype;
  OrgNamePtr    onp;
  OrgModPtr     tmpmod;
  SubSourcePtr  ssp;
  SubSourcePtr  tmpssp;

  taxnameval = GetValue (pfp->taxname);
  if (taxnameval == 2) {
    if (biop == NULL) {
      biop = BioSourceNew ();
      if (biop != NULL) {
        orp = OrgRefNew ();
        biop->org = orp;
        vnp = CreateNewDescriptor (sep, Seq_descr_source);
        if (vnp != NULL) {
          vnp->data.ptrvalue = (Pointer) biop;
        }
      }
    }
    if (biop == NULL) return;
    orp = biop->org;
    if (orp == NULL) return;
    SetTaxNameAndRemoveTaxRef (orp, StringSave (data));
  }
  if (biop == NULL && topbiop != NULL) {
    biop = (BioSourcePtr) AsnIoMemCopy ((Pointer) topbiop,
                                        (AsnReadFunc) BioSourceAsnRead,
                                        (AsnWriteFunc) BioSourceAsnWrite);
    if (biop != NULL) {
      vnp = CreateNewDescriptor (sep, Seq_descr_source);
      if (vnp != NULL) {
        vnp->data.ptrvalue = (Pointer) biop;
      }
    }
  }
  if (biop == NULL) return;
  orgmodtype = 0;
  subsourcetype = 0;
  if (taxnameval == 3) {
    orgmodtype = 255;
  } else if (taxnameval == 4) {
    subsourcetype = 255;
  } else if (GetEnumPopup (pfp->orgmod, orgmod_subtype_and_note_alist, &val) && val > 0) {
    orgmodtype = val;
  } else if (GetEnumPopup (pfp->subsource, subsource_subtype_and_note_alist, &val) && val > 0) {
    subsourcetype = val;
  }
  if (! StringHasNoText (data)) {
    if (orgmodtype > 0) {
      subtype = orgmodtype;
      orp = biop->org;
      if (orp == NULL) {
        orp = OrgRefNew ();
        biop->org = orp;
      }
      if (orp != NULL) {
        onp = orp->orgname;
        if (onp == NULL) {
          onp = OrgNameNew ();
          orp->orgname = onp;
        }
        if (onp != NULL) {
          mod = onp->mod;
          while (mod != NULL && mod->subtype != subtype) {
            mod = mod->next;
          }
          if (mod == NULL) {
            mod = OrgModNew ();
            if (onp->mod == NULL) {
              onp->mod = mod;
            } else {
              tmpmod = onp->mod;
              while (tmpmod->next != NULL) {
                tmpmod = tmpmod->next;
              }
              tmpmod->next = mod;
            }
          }
          if (mod != NULL) {
            mod->subtype = subtype;
            AppendOrReplaceString (&(mod->subname), data,
                                   &(pfp->replaceOldAsked),
                                   &(pfp->doReplaceAll),
                                   &(pfp->use_semicolon));
          }
        }
      }
    } else if (subsourcetype > 0) {
      subtype = subsourcetype;
      ssp = biop->subtype;
      while (ssp != NULL && ssp->subtype != subtype) {
        ssp = ssp->next;
      }
      if (ssp == NULL) {
        ssp = SubSourceNew ();
        if (biop->subtype == NULL) {
          biop->subtype = ssp;
        } else {
          tmpssp = biop->subtype;
          while (tmpssp->next != NULL) {
            tmpssp = tmpssp->next;
          }
          tmpssp->next = ssp;
        }
      }
      if (ssp != NULL) {
        ssp->subtype = subtype;
        AppendOrReplaceString (&(ssp->name), data,
                               &(pfp->replaceOldAsked),
                               &(pfp->doReplaceAll),
                               &(pfp->use_semicolon));
      }
    }
  }
}

static void DoOneParseItem (Uint2 entityID, SeqEntryPtr sep, ParseFormPtr pfp, BioSourcePtr topbiop, CharPtr useme, SeqEntryPtr nps)

{
  BioSourcePtr  biop;
  BioseqSetPtr  bssp;
  Char          str [256];
  SeqEntryPtr   tmp;
  OrgNameChangeData oncd;

  if (sep == NULL || pfp == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL) {
      /* if nucprot set, look for biosource here */
      biop = NULL;
      SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
      if (biop != NULL && topbiop != NULL) {
        topbiop = biop;
        if (bssp->_class == BioseqseqSet_class_nuc_prot)
        {
          GetDataForOrganism ( sep, str, sizeof (str), useme, pfp);
          if (topbiop->org != NULL && topbiop->org->taxname != NULL)
          {
            oncd.oldname = topbiop->org->taxname;
            oncd.newname = str;
            VisitBioseqsInSep (sep, &oncd, FixProteinTitleAfterOrganismNameChange);
          }
          ApplyDataToBioSourcePtr (pfp, topbiop, NULL, sep, str);
          return;
        }
      }
      for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
        DoOneParseItem (entityID, tmp, pfp, topbiop, useme, sep);
      }
      return;
    }
  }
  GetDataForOrganism ( sep, str, sizeof (str), useme, pfp);
  if (StringHasNoText (str)) return;
  biop = NULL;
  SeqEntryToBioSource (sep, NULL, NULL, 0, &biop);
  ApplyDataToBioSourcePtr (pfp, biop, topbiop, sep, str);
}

static void DoOneParse (Uint2 entityID, SeqEntryPtr sep, ParseFormPtr pfp, BioSourcePtr topbiop)

{
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Char          gb [256];
  CharPtr       ptr;
  SeqIdPtr      sip;
  Char          str [250];
  SeqEntryPtr   tmp;

  if (sep == NULL || pfp == NULL) return;
  if (pfp->fp != NULL) {
    ReadLine (pfp->fp, str, sizeof (str)); /* FileGets on Mac sometimes sees '\r' instead of '\n' */
    while (Nlm_fileDone || str [0] != 0) {
      if (! StringHasNoText (str)) {
        ptr = StringChr (str, '\t');
        if (ptr != NULL) {
          *ptr = '\0';
          ptr++;
          TrimSpacesAroundString (str);
          TrimSpacesAroundString (ptr);
          sip = MakeSeqID (str);
          bsp = BioseqFind (sip);
          SeqIdFree (sip);
          if (bsp == NULL) {
            sprintf (gb, "gb|%s|", str);
            sip = MakeSeqID (gb);
            bsp = BioseqFind (sip);
            SeqIdFree (sip);
          }
          if (bsp != NULL) {
            tmp = GetBestTopParentForData (entityID, bsp);
             DoOneParseItem (entityID, tmp, pfp, topbiop, ptr, NULL);
          }
        }
      }
      str [0] = 0;
      ReadLine (pfp->fp, str, sizeof (str));
    }
    return;
  }
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL && (bssp->_class == 7 ||
                         (IsPopPhyEtcSet (bssp->_class)))) {
      for (tmp = bssp->seq_set; tmp != NULL; tmp = tmp->next) {
        DoOneParse (entityID, tmp, pfp, topbiop);
      }
      return;
    }
  }
  DoOneParseItem (entityID, sep, pfp, topbiop, NULL, NULL);
}

static void DoParseDeflineProc (ButtoN b)

{
  BioSourcePtr  topbiop;
  ParseFormPtr  pfp;
  SeqEntryPtr   sep;

  pfp = (ParseFormPtr) GetObjectExtra (b);
  if (pfp == NULL || pfp->input_entityID == 0) return;
  sep = GetTopSeqEntryForEntityID (pfp->input_entityID);
  if (sep == NULL) return;
  Hide (pfp->form);
  WatchCursor ();
  Update ();
  /* if popset, look for top biosource */
  SeqEntryToBioSource (sep, NULL, NULL, 0, &topbiop);
  if (! StringHasNoText (pfp->path)) {
    pfp->fp = FileOpen (pfp->path, "r");
  }
  DoOneParse (pfp->input_entityID, sep, pfp, topbiop);
  FileClose (pfp->fp);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (pfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, pfp->input_entityID, 0, 0);
  if (GetStatus (pfp->leaveDlgUp)) {
    Show (pfp->form);
  } else {
    Remove (pfp->form);
  }
}

static void ParseDeflineMessageProc (ForM f, Int2 mssg)

{
  StdEditorProcsPtr  sepp;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    if (sepp->handleMessages != NULL) {
      sepp->handleMessages (f, mssg);
    }
  }
}

static void ParseDefOrLocalIDToSource (BaseFormPtr bfp, Boolean parsedef, CharPtr path)

{
  ButtoN             b;
  GrouP              c;
  GrouP              g;
  GrouP              h;
  GrouP              p;
  ParseFormPtr       pfp;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  WindoW             w;

  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  pfp = (ParseFormPtr) MemNew (sizeof (ParseFormData));
  if (pfp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "Parse Def Line", StdCloseWindowProc);
  SetObjectExtra (w, pfp, StdCleanupFormProc);
  pfp->form = (ForM) w;
  pfp->formmessage = ParseDeflineMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    pfp->appmessage = sepp->handleMessages;
  }

  pfp->input_entityID = bfp->input_entityID;
  pfp->input_itemID = bfp->input_itemID;
  pfp->input_itemtype = bfp->input_itemtype;

  pfp->parsedef = parsedef;
  StringNCpy_0 (pfp->path, path, sizeof (pfp->path));
  pfp->fp = NULL;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = NULL;
  if (parsedef) {
    g = HiddenGroup (h, 4, 0, NULL);
    StaticPrompt (g, "Get text between", 0, dialogTextHeight, programFont, 'l');
    pfp->atleft = DialogText (g, "", 10, NULL);
    StaticPrompt (g, "and", 0, dialogTextHeight, programFont, 'l');
    pfp->atright = DialogText (g, "", 10, NULL);
  }

  p = HiddenGroup (h, 6, 0, NULL);
  if (parsedef || path != NULL) {
    StaticPrompt (p, "Place in", 0, popupMenuHeight, programFont, 'l');
  } else {
    StaticPrompt (p, "Place localID in", 0, popupMenuHeight, programFont, 'l');
  }
  pfp->taxname = PopupList (p, TRUE, NULL);
  SetObjectExtra (pfp->taxname, pfp, NULL);
  PopupItem (pfp->taxname, " ");
  PopupItem (pfp->taxname, "Taxname");
  PopupItem (pfp->taxname, "OrgMod Note");
  PopupItem (pfp->taxname, "SubSource Note");
  SetValue (pfp->taxname, 1);
  StaticPrompt (p, "or", 0, popupMenuHeight, programFont, 'l');
  pfp->orgmod = PopupList (p, TRUE, NULL);
  SetObjectExtra (pfp->orgmod, pfp, NULL);
  InitEnumPopup (pfp->orgmod, orgmod_subtype_and_note_alist, NULL);
  SetEnumPopup (pfp->orgmod, orgmod_subtype_and_note_alist, 0);
  StaticPrompt (p, "or", 0, popupMenuHeight, programFont, 'l');
  pfp->subsource = PopupList (p, TRUE, NULL);
  SetObjectExtra (pfp->subsource, pfp, NULL);
  InitEnumPopup (pfp->subsource, subsource_subtype_and_note_alist, NULL);
  SetEnumPopup (pfp->subsource, subsource_subtype_and_note_alist, 0);

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoParseDeflineProc);
  SetObjectExtra (b, pfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);
  pfp->leaveDlgUp = CheckBox (c, "Leave Dialog Up", NULL);

  AlignObjects (ALIGN_CENTER, (HANDLE) p, (HANDLE) c, (HANDLE) g, NULL);
  RealizeWindow (w);
  Show (w);
  Select (w);
  Select (pfp->atleft);
  Update ();
}

static void ParseDefOrLocalIDToSourceMenuItem (IteM i, Boolean parsedef, CharPtr path)
{
  BaseFormPtr bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif

  if (bfp == NULL) return;

  ParseDefOrLocalIDToSource (bfp, parsedef, path);
}

extern void ParseFileToSource (IteM i)

{
  Char  path [PATH_MAX];

  if (GetInputFileName (path, sizeof (path), "", "TEXT")) {
    ParseDefOrLocalIDToSourceMenuItem (i, FALSE, path);
  }
}

static void TrimOrgNameCallback (BioSourcePtr biop, Pointer userdata)
{
  OrgRefPtr     orp;
  CharPtr       tmp;
  CharPtr       word_break;
  Int4          space_len;

  if (biop == NULL) return;

  orp = biop->org;
  if (orp == NULL || StringHasNoText (orp->taxname)) return;

  tmp = StringSave (orp->taxname);

  word_break = StringStr (tmp, " ");
  if (word_break == NULL) return;
  
  space_len = StringSpn (word_break, " ");

  if (StringNCmp (word_break + space_len, "sp.", 3) == 0
      || StringNCmp (word_break + space_len, "cf.", 3) == 0
      || StringNCmp (word_break + space_len, "aff.", 4) == 0)
  {
    word_break = StringStr (word_break + space_len, " ");
    if (word_break == NULL) return;
  }

  word_break = StringStr (word_break + space_len, " ");
  if (word_break == NULL) return;
  *word_break = 0;
  
  SetTaxNameAndRemoveTaxRef (orp, tmp);
}

extern void TrimOrganismName (IteM i)
{
  BaseFormPtr bfp;
  SeqEntryPtr sep;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  VisitBioSourcesInSep (sep, NULL, TrimOrgNameCallback);
  Update ();
  ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
}

typedef struct addmodinfo {
  Boolean isOrgMod;
  Uint1 subtype;
  Boolean only_sp;
  Boolean only_cf;
  Boolean only_aff;
  CharPtr abbreviation;
  Boolean use_abbreviation;
} AddModInfo, PNTR AddModInfoPtr;

typedef struct addmodlistitem {
  Boolean isOrgMod;
  Uint1 subtype;
  CharPtr name;
  CharPtr abbreviation;
} AddModListItem, PNTR AddModListItemPtr;

static AddModListItem mods_list[] = {
{ TRUE, ORGMOD_authority, "Authority", NULL },
{ FALSE, SUBSRC_clone, "Clone", NULL },
{ FALSE, SUBSRC_genotype, "Genotype", NULL },
{ FALSE, SUBSRC_haplotype, "Haplotype", NULL },
{ TRUE, ORGMOD_isolate, "Isolate", NULL },
{ TRUE, ORGMOD_pathovar, "Pathovar", "pv."},
{ TRUE, ORGMOD_serovar, "Serovar", "serovar" },
{ TRUE, ORGMOD_specimen_voucher, "Specimen voucher", NULL },
{ TRUE, ORGMOD_strain, "Strain", NULL },
{ TRUE, ORGMOD_sub_species, "Sub-species", "subsp." },
{ TRUE, ORGMOD_variety, "Variety", "var." }
};

static void AddModToOrgProc (BioSourcePtr biop, Pointer userdata)
{
  OrgRefPtr     orp;
  OrgModPtr     mod;
  OrgNamePtr    onp;
  Boolean       influenza;
  CharPtr	str_to_add;
  size_t        len;
  CharPtr	ptr;
  CharPtr	str;
  AddModInfoPtr	ami;
  SubSourcePtr  ssp;
  Boolean       ok_to_add = FALSE;

  if (biop == NULL) return;

  ami = (AddModInfoPtr) userdata;

  orp = biop->org;
  if (orp == NULL) return;

  if (! ami->only_sp && ! ami->only_cf && ! ami->only_aff)
  {
    ok_to_add = TRUE;
  }
  if (ami->only_sp && StringStr (orp->taxname, " sp.") != NULL)
  {
    ok_to_add = TRUE;
  }
  if (ami->only_cf && StringStr (orp->taxname, " cf.") != NULL)
  {
    ok_to_add = TRUE;
  }
  if (ami->only_aff && StringStr (orp->taxname, " aff.") != NULL)
  {
    ok_to_add = TRUE;
  }
  if (!ok_to_add)
  {
    return;
  }
  
  onp = orp->orgname;
  if (onp == NULL) return;

  str_to_add = NULL;

  if (ami->isOrgMod)
  {
    mod = onp->mod;
    str_to_add = NULL;
    while (mod != NULL) {
      if (mod->subtype == ami->subtype) {
        str_to_add = mod->subname;
      }
      mod = mod->next;
    }
  } else {
    if (biop->subtype != NULL) {
      ssp = biop->subtype;
      while (ssp != NULL) {
        if (ssp->subtype == ami->subtype) {
          str_to_add = ssp->name;
        }
        ssp = ssp->next;
      }
    }
  }
  if (str_to_add != NULL) {
    ptr = StringStr (orp->taxname, " (");
    if (ptr != NULL) {
      *ptr = '\0';
    } else {
      ptr = StringStr (orp->taxname, " (");
      if (ptr != NULL) {
        *ptr = '\0';
      }
    }
    influenza = FALSE;
    if (ami->subtype == 2 && 
	(StringICmp (orp->taxname, "Influenza A virus") == 0 ||
        StringICmp (orp->taxname, "Influenza B virus") == 0)) {
      influenza = TRUE;
    }
    len = StringLen (orp->taxname) + StringLen (str_to_add) + 6;
    if (influenza) {
      len += 2;
    }
    if (ami->abbreviation != NULL && ami->use_abbreviation)
    {
      len += StringLen (ami->abbreviation) + 1;
    }
    str = MemNew (len);
    if (str != NULL) {
      StringCpy (str, orp->taxname);
      StringCat (str, " ");
      if (ami->abbreviation != NULL && ami->use_abbreviation)
      {
        StringCat (str, ami->abbreviation);
        StringCat (str, " ");
      }
      if (influenza) {
        StringCat (str, "(");
      }
      StringCat (str, str_to_add);
      if (influenza) {
        StringCat (str, ")");
      }
      SetTaxNameAndRemoveTaxRef (biop->org, str);
    }
  }
}

typedef struct addmodformptr {
  FEATURE_FORM_BLOCK

  PopuP modifier_to_add;
  ButtoN only_sp;
  ButtoN only_cf;
  ButtoN only_aff;
  ButtoN use_abbreviation;

} AddModFormData, PNTR AddModFormPtr;

static void DoAddModToOrg (ButtoN b)
{
  AddModFormPtr	amfp;
  AddModInfoPtr ami;
  SeqEntryPtr  sep;
  Int4		mod_index;

  amfp = GetObjectExtra (b);
  if (amfp == NULL) return;
  Hide (amfp->form);

  sep = GetTopSeqEntryForEntityID (amfp->input_entityID);
  if (sep == NULL) return;
  ami = (AddModInfoPtr) MemNew (sizeof (AddModInfo));
  mod_index = GetValue (amfp->modifier_to_add);
  if (mod_index < 1
    || mod_index > sizeof (mods_list) / sizeof (AddModListItem))
    return;
  mod_index = mod_index - 1;

  ami->isOrgMod = mods_list[mod_index].isOrgMod;
  ami->subtype = mods_list[mod_index].subtype;
  ami->abbreviation = mods_list[mod_index].abbreviation;
  ami->only_sp = GetStatus (amfp->only_sp);
  ami->only_cf = GetStatus (amfp->only_cf);
  ami->only_aff = GetStatus (amfp->only_aff);
  ami->use_abbreviation = GetStatus (amfp->use_abbreviation);
  /* always use abbreviation for serovar */
  if (ami->isOrgMod && ami->subtype == ORGMOD_serovar)
  {
  	ami->use_abbreviation = TRUE;
  }
  VisitBioSourcesInSep (sep, ami, AddModToOrgProc);
  Update ();
  ObjMgrSetDirtyFlag (amfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, amfp->input_entityID, 0, 0);
  Remove (amfp->form);
}

static void ChangeModPopup (PopuP p)
{
  AddModFormPtr amfp;
  Int4          mod_index;
  
  amfp = (AddModFormPtr) GetObjectExtra (p);
  if (amfp == NULL) return;

  mod_index = GetValue (amfp->modifier_to_add);
  if (mod_index < 1
    || mod_index > sizeof (mods_list) / sizeof (AddModListItem))
    return;
  mod_index = mod_index - 1;
  if ( mods_list [mod_index].abbreviation != NULL)
  {
    Enable (amfp->use_abbreviation);
  }
  else
  {
    Disable (amfp->use_abbreviation);
  }
}

extern void AddModToOrg (IteM i)
{
  BaseFormPtr  bfp;
  AddModFormPtr amfp;
  WindoW w;
  GrouP	g;
  GrouP	h;
  GrouP c;
  Int2 index;
  ButtoN b;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  amfp = (AddModFormPtr) MemNew (sizeof (AddModFormData));
  if (amfp == NULL) return;
  w = FixedWindow (-50, -33, -20, -10, "Append to Organism Name",
	StdCloseWindowProc);
  SetObjectExtra (w, amfp, StdCleanupFormProc);
  amfp->form = (ForM) w;

  amfp->input_entityID = bfp->input_entityID;
  amfp->input_itemID = bfp->input_itemID;

  g = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (g, 10, 10);

  h = HiddenGroup (g, 1, 0, NULL);
  amfp->modifier_to_add = PopupList (h, TRUE, ChangeModPopup);
  for (index=0; index< sizeof (mods_list) / sizeof (AddModListItem); index++)
  {
    PopupItem (amfp->modifier_to_add, mods_list[index].name);
  }
  SetObjectExtra (amfp->modifier_to_add, amfp, NULL);
  SetValue (amfp->modifier_to_add, 1);

  amfp->only_sp = CheckBox (g, "Only append to sp. organisms", NULL);
  amfp->only_cf = CheckBox (g, "Only append to cf. organisms", NULL);
  amfp->only_aff = CheckBox (g, "Only append to aff. organisms", NULL);
  amfp->use_abbreviation = CheckBox (g, "Use abbreviation (pv. or subsp. or var.)", NULL);
  SetStatus (amfp->use_abbreviation, TRUE);
  Disable (amfp->use_abbreviation);

  c = HiddenGroup (g, 2, 0, NULL);
  b = DefaultButton(c, "Accept", DoAddModToOrg);
  SetObjectExtra(b, amfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) h, 
                              (HANDLE) amfp->only_sp, 
                              (HANDLE) amfp->only_cf,
                              (HANDLE) amfp->only_aff,
                              (HANDLE) c, NULL);
  RealizeWindow(w);
  Show(w);
  Update();
  
}

typedef struct descformdata {
  FEATURE_FORM_BLOCK

  Uint2          oldEntityID;
  Uint2          oldItemID;
  Uint2          oldItemtype;
  Uint2          lookfor;
  Boolean        found;
  ObjMgrProcPtr  ompp;
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  SeqEntryPtr    sep;
  Handle         target;
  Boolean        usePopupForTarget;
  Boolean        hasMutPopPhySet;
  Int2           targetScratchSpace;
  Int2           nucProtCount;
  Int2           segSetCount;
  Uint2          descsubtype;
  ButtoN         createNewBtn;
} DescFormData, PNTR DescFormPtr;

static Boolean FindDescrFunc (GatherContextPtr gcp)

{
  DescFormPtr  dfp;
  ValNodePtr   vnp;

  if (gcp == NULL) return TRUE;
  dfp = (DescFormPtr) gcp->userdata;
  if (dfp == NULL) return TRUE;
  if (gcp->thistype == OBJ_SEQDESC) {
    vnp = (ValNodePtr) gcp->thisitem;
    if (vnp != NULL && vnp->choice == dfp->lookfor) {
      dfp->oldEntityID = gcp->entityID;
      dfp->oldItemID = gcp->itemID;
      dfp->oldItemtype = gcp->thistype;
      dfp->found = TRUE;
      return FALSE;
    }
  }
  return TRUE;
}

static CharPtr segClassList [] = {
  " ", "Nucleotide-Protein Set", "Segmented Nucleotide Set",
  "conset", "parts", "gibb", "gi", "genbank", "pir", "pub-set",
  "equiv", "swissprot", "pdb-entry", "MUTATION SET",
  "POPULATION SET", "PHYLOGENETIC SET", "ENVIRONMENTAL SAMPLES",
  "genomic product set", "other", NULL
};

static Int4 AllButPartsList (SeqEntryPtr sep, Pointer mydata,
                             SeqEntryFunc mycallback,
                             Int4 index, Int2 indent)

{
  BioseqSetPtr  bssp;

  if (sep == NULL) return index;
  if (IS_Bioseq (sep)) {
    if (mycallback != NULL)
      (*mycallback) (sep, mydata, index, indent);
    return index + 1;
  }
  if (Bioseq_set_class (sep) != 4) {
    if (mycallback != NULL)
      (*mycallback) (sep, mydata, index, indent);
    index++;
  }
  bssp = (BioseqSetPtr) sep->data.ptrvalue;
  sep = bssp->seq_set;
  indent++;
  while (sep != NULL) {
    index = AllButPartsList (sep, mydata, mycallback, index, indent);
    sep = sep->next;
  }
  return index;
}

#define AllButPartsCount( a )  AllButPartsList( a ,NULL,NULL,0,0);
#define AllButPartsExplore(a,b,c) AllButPartsList(a, b, c, 0L, 0);

static void PopTargetProc (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  DescFormPtr   dfp;
  BioseqPtr     bsp;
  BioseqSetPtr  bssp;
  Uint1         _class;
  CharPtr       ptr;
  SeqIdPtr      sip;
  Char          str [128];

  dfp = (DescFormPtr) mydata;
  if (sep != NULL && sep->data.ptrvalue != NULL) {
    if (sep->choice == 1) {
      bsp = (BioseqPtr) sep->data.ptrvalue;
      sip = SeqIdFindWorst (bsp->id);
      SeqIdWrite (sip, str, PRINTID_REPORT, sizeof (str));
      ptr = StringChr (str, '|');
      if (ptr == NULL) {
        ptr = str;
      } else {
        ptr++;
      }
      if (dfp->usePopupForTarget) {
        PopupItem (dfp->target, ptr);
      } else {
        ListItem (dfp->target, ptr);
      }
      if (bsp == dfp->bsp) {
        dfp->targetScratchSpace = index + 1;
      }
    } else if (sep->choice == 2) {
      bssp = (BioseqSetPtr) sep->data.ptrvalue;
      _class = bssp->_class;
      if (_class > 17) {
        _class = 18;
      }
      if (_class == 7 || (IsPopPhyEtcSet (bssp->_class))) {
        dfp->hasMutPopPhySet = TRUE;
      }
      if (! dfp->hasMutPopPhySet) {
        sprintf (str, "[%s]", segClassList [_class]);
      } else if (_class == 1) {
        (dfp->nucProtCount)++;
        sprintf (str, "[%s %d]", segClassList [_class], (int) dfp->nucProtCount);
        (dfp->segSetCount)++;
      } else if (_class == 2) {
        sprintf (str, "[%s %d]", segClassList [_class], (int) dfp->segSetCount);
      } else {
        sprintf (str, "[%s]", segClassList [_class]);
      }
      if (dfp->usePopupForTarget) {
        PopupItem (dfp->target, str);
      } else {
        ListItem (dfp->target, str);
      }
    }
  }
}

static Int2 PopulateTarget (DescFormPtr dfp, BioseqPtr bsp, Uint2 entityID)

{
  SeqEntryPtr  sep;
  Int2         val;

  val = 0;
  if (dfp != NULL) {
    if (bsp != NULL) {
      sep = SeqMgrGetSeqEntryForData (bsp);
      entityID = ObjMgrGetEntityIDForPointer (sep);
    }
    sep = GetTopSeqEntryForEntityID (entityID);
    dfp->sep = sep;
    if (sep != NULL) {
      dfp->hasMutPopPhySet = FALSE;
      dfp->targetScratchSpace = 1;
      dfp->nucProtCount = 0;
      dfp->segSetCount = 0;
      AllButPartsExplore (sep, (Pointer) dfp, PopTargetProc);
      val = dfp->targetScratchSpace;
    }
  }
  return val;
}

static void FindNewTargetProc (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  DescFormPtr   dfp;

  dfp = (DescFormPtr) mydata;
  if (sep != NULL && sep->data.ptrvalue != NULL) {
    if (index + 1 == dfp->targetScratchSpace) {
      if (sep->choice == 1) {
        dfp->bsp = (BioseqPtr) sep->data.ptrvalue;
      } else if (sep->choice == 2) {
        dfp->bssp = (BioseqSetPtr) sep->data.ptrvalue;
      }
    }
  }
}

static Boolean ChangeTargetItemID (GatherContextPtr gcp)

{
  DescFormPtr  dfp;

  if (gcp == NULL) return TRUE;
  dfp = (DescFormPtr) gcp->userdata;
  if (dfp == NULL) return TRUE;
  if (gcp->thistype == OBJ_BIOSEQ) {
    if (dfp->bsp == (BioseqPtr) gcp->thisitem) {
      dfp->input_entityID = gcp->entityID;
      dfp->input_itemID = gcp->itemID;
      dfp->input_itemtype = gcp->thistype;
      return FALSE;
    }
  } else if (gcp->thistype == OBJ_BIOSEQSET) {
    if (dfp->bssp == (BioseqSetPtr) gcp->thisitem) {
      dfp->input_entityID = gcp->entityID;
      dfp->input_itemID = gcp->itemID;
      dfp->input_itemtype = gcp->thistype;
      return FALSE;
    }
  }
  return TRUE;
}

static void PreventDupTitleCreation (DescFormPtr dfp)

{
  ValNodePtr  sdp;

  if (dfp == NULL) return;
  if (dfp->bsp != NULL) {
    sdp = dfp->bsp->descr;
  } else if (dfp->bssp != NULL) {
    sdp = dfp->bssp->descr;
  } else return;
  if (dfp->descsubtype != Seq_descr_title) return;
  while (sdp != NULL) {
    if (sdp->choice == Seq_descr_title) {
      SafeDisable (dfp->createNewBtn);
      return;
    }
    sdp = sdp->next;
  }
  SafeEnable (dfp->createNewBtn);
}

static void ChangeNewDescTarget (Handle obj)

{
  DescFormPtr  dfp;
  GatherScope  gs;

  dfp = (DescFormPtr) GetObjectExtra (obj);
  if (dfp != NULL) {
    dfp->bsp = NULL;
    dfp->bssp = NULL;
    dfp->targetScratchSpace = GetValue (dfp->target);
    AllButPartsExplore (dfp->sep, (Pointer) dfp, FindNewTargetProc);
    MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
    gs.seglevels = 0;
    MemSet((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
    gs.ignore[OBJ_BIOSEQ] = FALSE;
    gs.ignore[OBJ_BIOSEQSET] = FALSE;
    gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
    GatherEntity (dfp->input_entityID, (Pointer) dfp, ChangeTargetItemID, &gs);
    PreventDupTitleCreation (dfp);
  }
}

static void CreateNewDescProc (ButtoN b)

{
  DescFormPtr    dfp;
  OMProcControl  ompc;
  ObjMgrProcPtr  ompp;
  Int2           retval;

  dfp = (DescFormPtr) GetObjectExtra (b);
  if (dfp != NULL) {
    Hide (dfp->form);
    ompp = dfp->ompp;
    MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
    ompc.input_entityID = dfp->input_entityID;
    ompc.input_itemID = dfp->input_itemID;
    ompc.input_itemtype = dfp->input_itemtype;
    GatherDataForProc (&ompc, FALSE);
    ompc.proc = ompp;
    retval = (*(ompp->func)) (&ompc);
    if (retval == OM_MSG_RET_ERROR) {
      ErrShow ();
    }
    Update ();
    Remove (dfp->form);
  }
}

static void EditOldDescProc (ButtoN b)

{
  DescFormPtr  dfp;
  Int2         handled;

  dfp = (DescFormPtr) GetObjectExtra (b);
  if (dfp != NULL) {
    Hide (dfp->form);
    if (dfp->oldEntityID > 0 && dfp->oldItemID > 0 && dfp->oldItemtype > 0) {
      WatchCursor ();
      handled = GatherProcLaunch (OMPROC_EDIT, FALSE, dfp->oldEntityID,
                                  dfp->oldItemID, dfp->oldItemtype,
                                  0, 0, dfp->oldItemtype, 0);
      ArrowCursor ();
      if (handled != OM_MSG_RET_DONE || handled == OM_MSG_RET_NOPROC) {
        Message (MSG_ERROR, "Unable to edit existing descriptor.");
      }
    }
    Update ();
    Remove (dfp->form);
  }
}

static CharPtr  newDescMsg = "\
Descriptors may apply to a single sequence or to a set of \
sequences.  Please set the target control to the desired \
set or sequence.";

static CharPtr  editOldMsg = "\
A descriptor of this type already exists at or above the \
displayed target.  You may want to edit it instead of \
creating a new descriptor.";

static void NewDescriptorMessageProc (ForM f, Int2 mssg)

{
  DescFormPtr  dfp;

  dfp = (DescFormPtr) GetObjectExtra (f);
  if (dfp != NULL) {
    if (dfp->appmessage != NULL) {
      dfp->appmessage (f, mssg);
    }
  }
}

extern void NewDescriptorMenuFunc (ObjMgrProcPtr ompp, BaseFormPtr bfp, Uint2 descsubtype)

{
  ButtoN             b;
  BioseqPtr          bsp;
  GrouP              c;
  Int4               count;
  DescFormPtr        dfp;
  Uint2              entityID;
  GrouP              g;
  GatherScope        gs;
  GrouP              h;
  GrouP              p1, p2;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  SeqIdPtr           sip;
  SeqLocPtr          slp;
  Int2               val;
  WindoW             w;

#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#endif
  if (bfp == NULL) return;
  if (ompp == NULL || ompp->func == NULL) return;
  if (ompp->inputtype != OBJ_SEQDESC) return;
  dfp = (DescFormPtr) MemNew (sizeof (DescFormData));
  if (dfp == NULL) return;
  dfp->ompp = ompp;
  dfp->lookfor = ompp->subinputtype;
  dfp->found = FALSE;
  dfp->descsubtype = descsubtype;
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
  gs.seglevels = 0;
  gs.get_feats_location = TRUE;
  MemSet((Pointer) (gs.ignore), (int) (TRUE), (size_t) (OBJ_MAX * sizeof (Boolean)));
  gs.ignore[OBJ_BIOSEQ] = FALSE;
  gs.ignore[OBJ_BIOSEQSET] = FALSE;
  gs.ignore[OBJ_BIOSEQ_SEG] = FALSE;
  gs.ignore[OBJ_SEQDESC] = FALSE;
  if (bsp != NULL) {
    slp = ValNodeNew (NULL);
    slp->choice = SEQLOC_WHOLE;
    sip = SeqIdStripLocus (SeqIdDup (SeqIdFindBest (bsp->id, 0)));
    slp->data.ptrvalue = sip;
    gs.target = slp;
  }
  dfp->bsp = bsp;
  GatherEntity (bfp->input_entityID, (Pointer) dfp, FindDescrFunc, &gs);
  SeqLocFree (gs.target);
  w = FixedWindow (-50, -33, -10, -10, "Descriptor Target Control", StdCloseWindowProc);
  SetObjectExtra (w, dfp, StdCleanupFormProc);
  dfp->form = (ForM) w;
  dfp->formmessage = NewDescriptorMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    dfp->appmessage = sepp->handleMessages;
  }

  dfp->input_entityID = bfp->input_entityID;
  dfp->input_itemID = bfp->input_itemID;
  dfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  p1 = MultiLinePrompt (h, newDescMsg, 25 * stdCharWidth, programFont);

  entityID = bfp->input_entityID;
  if (bsp != NULL) {
    sep = SeqMgrGetSeqEntryForData (bsp);
    entityID = ObjMgrGetEntityIDForChoice (sep);
  }
  sep = GetTopSeqEntryForEntityID (entityID);
  count = AllButPartsCount (sep);
  if (count < 32) {
    g = HiddenGroup (h, 2, 0, NULL);
    StaticPrompt (g, "Target", 0, popupMenuHeight, programFont, 'l');
    dfp->usePopupForTarget = TRUE;
    dfp->target = PopupList (g, TRUE, (PupActnProc) ChangeNewDescTarget);
  } else {
    g = HiddenGroup (h, 0, 2, NULL);
    StaticPrompt (g, "Target", 0, 0, programFont, 'c');
    dfp->usePopupForTarget = FALSE;
    dfp->target = SingleList (g, 14, 3, (LstActnProc) ChangeNewDescTarget);
  }
  SetObjectExtra (dfp->target, dfp, NULL);
  val = PopulateTarget (dfp, bsp, bfp->input_entityID);
  SetValue (dfp->target, val);
  ChangeNewDescTarget ((Handle) dfp->target);

  p2 = NULL;
  if (dfp->found) {
    p2 = MultiLinePrompt (h, editOldMsg, 25 * stdCharWidth, programFont);
  }

  c = HiddenGroup (h, 4, 0, NULL);
  SetGroupSpacing (c, 10, 2);
  dfp->createNewBtn = PushButton (c, "Create New", CreateNewDescProc);
  SetObjectExtra (dfp->createNewBtn, dfp, NULL);
  if (dfp->found) {
    b = DefaultButton (c, "Edit Old", EditOldDescProc);
    SetObjectExtra (b, dfp, NULL);
  }
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) p1, (HANDLE) g, (HANDLE) c, (HANDLE) p2, NULL);
  RealizeWindow (w);
  PreventDupTitleCreation (dfp);
  Show (w);
  Update ();
}

static void NewDescriptorMenuProc (IteM i)

{
  NewObjectPtr  nop;

  nop = (NewObjectPtr) GetObjectExtra (i);
  if (nop == NULL) return;
  NewDescriptorMenuFunc (nop->ompp, nop->bfp, nop->descsubtype);
}

extern void SetupNewDescriptorsMenu (MenU m, BaseFormPtr bfp)

{
  Boolean        allowgenbank;
  IteM           i;
  NewObjectPtr   nop;
  ObjMgrPtr      omp;
  ObjMgrProcPtr  ompp;
  ObjMgrTypePtr  omtp;

  if (m == NULL) return;
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, 0, 0, NULL);
  if (ompp == NULL) return;
  omtp = NULL;
  allowgenbank = FALSE;
/*#ifdef EXTRA_SERVICES*/
  if (extraServices) {
    allowgenbank = TRUE;
  }
/*#endif*/
  while ((omtp = ObjMgrTypeFindNext (omp, omtp)) != NULL) {
    ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, omtp->datatype, 0, NULL);
    if (ompp != NULL) {
      switch (omtp->datatype) {
        case OBJ_SEQDESC :
          ompp = NULL;
          while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				  omtp->datatype, 0, ompp)) != NULL) {
            if (ompp->subinputtype != Seq_descr_pub) {
              if (allowgenbank || ompp->subinputtype != Seq_descr_genbank) {
                i = CommandItem (m, ompp->proclabel, NewDescriptorMenuProc);
                nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
                if (nop != NULL) {
                  nop->ompp = ompp;
                  nop->bfp = bfp;
                  nop->descsubtype = ompp->subinputtype;
                }
                SetObjectExtra (i, (Pointer) nop, StdCleanupExtraProc);
              }
            }
          }
          break;
        default :
          break;
      }
    }
  }
}

static CharPtr  editOldDescMsg = "\
You may really want to edit an existing BioSource descriptor instead.\n\
Proceed anyway?";

static void NewFeatureMenuProc (IteM i)

{
  MsgAnswer      ans;
  BaseFormPtr    bfp;
  NewObjectPtr   nop;
  OMProcControl  ompc;
  ObjMgrProcPtr  ompp;
  Int2           retval;

  nop = (NewObjectPtr) GetObjectExtra (i);
  if (nop == NULL) return;
#ifdef WIN_MAC
  bfp = (BaseFormPtr) currentFormDataPtr;
#else
  bfp = nop->bfp;
#endif
  if (bfp == NULL) return;
  ompp = nop->ompp;
  if (ompp == NULL || ompp->func == NULL) return;
  if (ompp->subinputtype == FEATDEF_BIOSRC) {
    ans = Message (MSG_YN, editOldDescMsg);
    if (ans == ANS_NO) return;
  }
  MemSet ((Pointer) (&ompc), 0, sizeof (OMProcControl));
  ompc.input_entityID = bfp->input_entityID;
  ompc.input_itemID = bfp->input_itemID;
  ompc.input_itemtype = bfp->input_itemtype;
  GatherDataForProc (&ompc, FALSE);
  ompc.proc = ompp;
  retval = (*(ompp->func)) (&ompc);
  if (retval == OM_MSG_RET_ERROR) {
    ErrShow ();
  }
}

#ifdef WIN_MAC
VoidPtr macUserDataPtr = NULL;
#endif

extern void EnableFeaturesPerTarget (BaseFormPtr bfp)

{
  BioseqPtr     bsp;
  Uint1         mol;
  NewObjectPtr  nop;

  if (bfp == NULL) return;
#ifdef WIN_MAC
  nop = (NewObjectPtr) macUserDataPtr;
#else
  nop = (NewObjectPtr) bfp->userDataPtr;
#endif
  bsp =  GetBioseqGivenIDs (bfp->input_entityID, bfp->input_itemID, bfp->input_itemtype);
  mol = 0;
  if (bsp != NULL) {
    mol = bsp->mol;
  }
  while (nop != NULL) {
    if (nop->kind == 2) {
      /* analysis menu item, ignore it */
    } else if (mol == 0) {
      SafeDisable (nop->item);
    } else if (ISA_na (mol) && (nop->molgroup == 2 || nop->molgroup == 3)) {
      SafeEnable (nop->item);
    } else if (ISA_aa (mol) && (nop->molgroup == 1 || nop->molgroup == 3)) {
      SafeEnable (nop->item);
    } else {
      SafeDisable (nop->item);
    }
    nop = nop->next;
  }
}

static VoidPtr LinkNewObjectLists (NewObjectPtr list1, NewObjectPtr list2)

{
  NewObjectPtr  nop;

  if (list1 == NULL) return list2;
  nop = list1;
  while (nop->next != NULL) {
    nop = nop->next;
  }
  nop->next = list2;
  return list1;
}

extern void SetupNewFeaturesMenu (MenU m, BaseFormPtr bfp)

{
  FeatDispGroupPtr  fdgp;
  FeatDefPtr        fdp;
  NewObjectPtr      first;
  IteM              i;
  Uint1             key;
  CharPtr           label;
  NewObjectPtr      last;
  NewObjectPtr      nop;
  ObjMgrPtr         omp;
  ObjMgrProcPtr     ompp;
  ObjMgrTypePtr     omtp;
  MenU              sub;
  Uint2             subtype;

  if (m == NULL) return;
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, 0, 0, NULL);
  if (ompp == NULL) return;
  omtp = NULL;
  first = NULL;
  last = NULL;
  while ((omtp = ObjMgrTypeFindNext (omp, omtp)) != NULL) {
    ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, omtp->datatype, 0, NULL);
    if (ompp != NULL) {
      switch (omtp->datatype) {
        case OBJ_SEQFEAT :
          fdgp = NULL;
          while ((fdgp = DispGroupFindNext (fdgp, &key, &label)) != NULL) {
            if (fdgp->groupkey != 0) {
              sub = SubMenu (m, fdgp->groupname);
              fdp = NULL;
              label = NULL;
              while ((fdp = FeatDefFindNext (fdp, &key, &label,
                     fdgp->groupkey, FALSE)) != NULL) {
                if (key != FEATDEF_BAD) {
                  ompp = NULL;
                  while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				          omtp->datatype, 0, ompp)) != NULL) {
                    if (ompp->subinputtype == fdp->featdef_key &&
                        ompp->subinputtype != FEATDEF_PUB) {
                      i = CommandItem (sub, ompp->proclabel, NewFeatureMenuProc);
                      nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
                      if (nop != NULL) {
                        nop->kind = 1; /* feature creation item */
                        nop->ompp = ompp;
                        nop->bfp = bfp;
                        nop->item = i;
                        nop->molgroup = fdp->molgroup;
                      }
                      if (first == NULL) {
                        first = nop;
                      }
                      if (last != NULL) {
                        last->next = nop;
                      }
                      last = nop;
                      SetObjectExtra (i, (Pointer) nop, StdCleanupExtraProc);
                    }
                  }
                }
              }
            }
          }
          /* if (indexerVersion) { */
            sub = SubMenu (m, "Remaining Features");
            fdp = NULL;
            label = NULL;
            while ((fdp = FeatDefFindNext (fdp, &key, &label, 0, FALSE)) != NULL) {
              if (key != FEATDEF_BAD) {
                ompp = NULL;
                while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				        omtp->datatype, 0, ompp)) != NULL) {
				  subtype = ompp->subinputtype;
                  if (subtype == fdp->featdef_key &&
                      subtype != FEATDEF_PUB &&
                      subtype != FEATDEF_Imp_CDS &&
                      subtype != FEATDEF_misc_RNA &&
                      subtype != FEATDEF_precursor_RNA &&
                      subtype != FEATDEF_mat_peptide &&
                      subtype != FEATDEF_sig_peptide &&
                      subtype != FEATDEF_transit_peptide &&
                      subtype != FEATDEF_source &&
                      subtype != FEATDEF_virion &&
                      subtype != FEATDEF_mutation &&
                      subtype != FEATDEF_allele &&
                      subtype != FEATDEF_site_ref &&
                      subtype != FEATDEF_gap) {
                    i = CommandItem (sub, ompp->proclabel, NewFeatureMenuProc);
                    nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
                    if (nop != NULL) {
                      nop->kind = 1; /* feature creation item */
                      nop->ompp = ompp;
                      nop->bfp = bfp;
                      nop->item = i;
                      nop->molgroup = fdp->molgroup;
                    }
                    if (first == NULL) {
                      first = nop;
                    }
                    if (last != NULL) {
                      last->next = nop;
                    }
                    last = nop;
                    SetObjectExtra (i, (Pointer) nop, StdCleanupExtraProc);
                  }
                }
              }
            }
          /* } */
          break;
        default :
          break;
      }
    }
  }
#ifdef WIN_MAC
  macUserDataPtr = LinkNewObjectLists (macUserDataPtr, first);
#else
  bfp->userDataPtr = LinkNewObjectLists (bfp->userDataPtr, first);
#endif
}

extern void SetupNewPublicationsMenu (MenU m, BaseFormPtr bfp)

{
  NewObjectPtr   first;
  IteM           i;
  NewObjectPtr   nop;
  ObjMgrPtr      omp;
  ObjMgrProcPtr  ompp;
  ObjMgrTypePtr  omtp;

  if (m == NULL) return;
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, 0, 0, NULL);
  if (ompp == NULL) return;
  omtp = NULL;
  while ((omtp = ObjMgrTypeFindNext (omp, omtp)) != NULL) {
    ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, omtp->datatype, 0, NULL);
    if (ompp != NULL) {
      switch (omtp->datatype) {
        case OBJ_SEQFEAT :
            ompp = NULL;
            while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				    omtp->datatype, 0, ompp)) != NULL) {
              if (ompp->subinputtype == FEATDEF_PUB) {
                i = CommandItem (m, "Publication Feature", NewFeatureMenuProc);
                nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
                if (nop != NULL) {
                  nop->kind = 1; /* feature creation item */
                  nop->ompp = ompp;
                  nop->bfp = bfp;
                  nop->item = i;
                  nop->molgroup = 3;
#ifdef WIN_MAC
                  first = (NewObjectPtr) macUserDataPtr;
#else
                  first = (NewObjectPtr) bfp->userDataPtr;
#endif
                  if (first != NULL) {
                    while (first->next != NULL) {
                      first = first->next;
                    }
                    first->next = nop;
                  }
                }
                SetObjectExtra (i, (Pointer) nop, StdCleanupExtraProc);
              }
            }
          break;
        case OBJ_SEQDESC :
          ompp = NULL;
          while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				  omtp->datatype, 0, ompp)) != NULL) {
            if (ompp->subinputtype == Seq_descr_pub) {
              i = CommandItem (m, "Publication Descriptor", NewDescriptorMenuProc);
              nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
              if (nop != NULL) {
                nop->ompp = ompp;
                nop->bfp = bfp;
              }
              SetObjectExtra (i, (Pointer) nop, StdCleanupExtraProc);
            }
          }
          break;
        default :
          break;
      }
    }
  }
}

#ifdef WIN_MAC
extern IteM  addSecondaryItem;
#endif

extern void SetupEditSecondary (MenU m, BaseFormPtr bfp)

{
  IteM           i;
  NewObjectPtr   nop;
  ObjMgrPtr      omp;
  ObjMgrProcPtr  ompp;
  ObjMgrTypePtr  omtp;

  if (m == NULL) return;
  omp = ObjMgrGet ();
  if (omp == NULL) return;
  ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, 0, 0, NULL);
  if (ompp == NULL) return;
  omtp = NULL;
  while ((omtp = ObjMgrTypeFindNext (omp, omtp)) != NULL) {
    ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT, omtp->datatype, 0, NULL);
    if (ompp != NULL) {
      switch (omtp->datatype) {
        case OBJ_SEQDESC :
          ompp = NULL;
          while ((ompp = ObjMgrProcFindNext (omp, OMPROC_EDIT,
				  omtp->datatype, 0, ompp)) != NULL) {
            if (ompp->subinputtype != Seq_descr_pub) {
              if (ompp->subinputtype == Seq_descr_genbank) {
                i = CommandItem (m, "Add Secondary", NewDescriptorMenuProc);
                nop = (NewObjectPtr) MemNew (sizeof (NewObjectData));
                if (nop != NULL) {
                  nop->ompp = ompp;
                  nop->bfp = bfp;
                }
                SetObjectExtra (i, (Pointer) nop, StdCleanupExtraProc);
#ifdef WIN_MAC
                if (addSecondaryItem == NULL) {
                  addSecondaryItem = i;
                }
#endif
                return;
              }
            }
          }
          break;
        default :
          break;
      }
    }
  }
}

typedef struct gbformdata {
  FEATURE_FORM_BLOCK

  ButtoN         clearExtraAccns;
  ButtoN         clearSource;
  ButtoN         clearKeywords;
  ButtoN         clearOrigin;
  ButtoN         clearOldDate;
  ButtoN         clearEntryDate;
  ButtoN         clearDivision;
  ButtoN         clearTaxonomy;
} GbeditFormData, PNTR GbeditFormPtr;

static void EditGenbankCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent)

{
  BioseqPtr      bsp;
  BioseqSetPtr   bssp;
  Boolean        empty;
  GBBlockPtr     gbp;
  GbeditFormPtr  gfp;
  ValNodePtr     nextsdp;
  Pointer PNTR   prevsdp;
  ValNodePtr     sdp;

  if (mydata == NULL) return;
  if (sep == NULL || sep->data.ptrvalue == NULL) return;
  gfp = (GbeditFormPtr) mydata;
  if (gfp == NULL) return;
  if (IS_Bioseq (sep)) {
    bsp = (BioseqPtr) sep->data.ptrvalue;
    sdp = bsp->descr;
    prevsdp = (Pointer PNTR) &(bsp->descr);
  } else if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    sdp = bssp->descr;
    prevsdp = (Pointer PNTR) &(bssp->descr);
  } else return;
  while (sdp != NULL) {
    nextsdp = sdp->next;
    empty = FALSE;
    if (sdp->choice == Seq_descr_genbank && sdp->data.ptrvalue != NULL) {
      gbp = (GBBlockPtr) sdp->data.ptrvalue;
      if (GetStatus (gfp->clearExtraAccns)) {
        gbp->extra_accessions = ValNodeFreeData (gbp->extra_accessions);
      }
      if (GetStatus (gfp->clearSource)) {
        gbp->source = MemFree (gbp->source);
      }
      if (GetStatus (gfp->clearKeywords)) {
        gbp->keywords = ValNodeFreeData (gbp->keywords);
      }
      if (GetStatus (gfp->clearOrigin)) {
        gbp->origin = MemFree (gbp->origin);
      }
      if (GetStatus (gfp->clearOldDate)) {
        gbp->date = MemFree (gbp->date);
      }
      if (GetStatus (gfp->clearEntryDate)) {
        gbp->entry_date = DateFree (gbp->entry_date);
      }
      if (GetStatus (gfp->clearDivision)) {
        gbp->div = MemFree (gbp->div);
      }
      if (GetStatus (gfp->clearTaxonomy)) {
        gbp->taxonomy = MemFree (gbp->taxonomy);
      }
      if (gbp->extra_accessions == NULL && gbp->source == NULL &&
          gbp->keywords == NULL && gbp->origin == NULL &&
          gbp->date == NULL && gbp->entry_date == NULL &&
          gbp->div == NULL && gbp->taxonomy == NULL) {
        empty = TRUE;
        ObjMgrDeSelect (0, 0, 0, 0, NULL);
      }
    }
    if (empty) {
      *(prevsdp) = sdp->next;
      sdp->next = NULL;
      SeqDescFree (sdp);
    } else {
      prevsdp = (Pointer PNTR) &(sdp->next);
    }
    sdp = nextsdp;
  }
}

static void DoEditGenbank (ButtoN b)

{
  GbeditFormPtr  gfp;
  SeqEntryPtr    sep;

  gfp = GetObjectExtra (b);
  if (gfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (gfp->input_entityID);
  if (sep == NULL) return;
  Hide (gfp->form);
  WatchCursor ();
  Update ();
  SeqEntryExplore (sep, (Pointer) gfp, EditGenbankCallback);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (gfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, gfp->input_entityID, 0, 0);
  Remove (gfp->form);
}

static void EditGenbankMessageProc (ForM f, Int2 mssg)

{
  GbeditFormPtr  gfp;

  gfp = (GbeditFormPtr) GetObjectExtra (f);
  if (gfp != NULL) {
    if (gfp->appmessage != NULL) {
      gfp->appmessage (f, mssg);
    }
  }
}

extern void EditGenbankElements (Handle i)

{
  BaseFormPtr        bfp;
  ButtoN             b;
  GrouP              c;
  GrouP              g;
  GbeditFormPtr      gfp;
  GrouP              h;
  SeqEntryPtr        sep;
  StdEditorProcsPtr  sepp;
  WindoW             w;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;
  gfp = (GbeditFormPtr) MemNew (sizeof (GbeditFormData));
  if (gfp == NULL) return;
  w = FixedWindow (-50, -33, -10, -10, "GenBank Block Removal", StdCloseWindowProc);
  SetObjectExtra (w, gfp, StdCleanupFormProc);
  gfp->form = (ForM) w;
  gfp->formmessage = EditGenbankMessageProc;

  sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
  if (sepp != NULL) {
    SetActivate (w, sepp->activateForm);
    gfp->appmessage = sepp->handleMessages;
  }

  gfp->input_entityID = bfp->input_entityID;
  gfp->input_itemID = bfp->input_itemID;
  gfp->input_itemtype = bfp->input_itemtype;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  g = HiddenGroup (h, -1, 0, NULL);

  gfp->clearExtraAccns = CheckBox (g, "Clear Secondary Accessions", NULL);
  gfp->clearSource = CheckBox (g, "Clear Source Line", NULL);
  gfp->clearKeywords = CheckBox (g, "Clear Keywords", NULL);
  gfp->clearOrigin = CheckBox (g, "Clear Origin", NULL);
  gfp->clearOldDate = CheckBox (g, "Clear Old Date", NULL);
  gfp->clearEntryDate = CheckBox (g, "Clear Entry Date", NULL);
  gfp->clearDivision = CheckBox (g, "Clear Division", NULL);
  gfp->clearTaxonomy = CheckBox (g, "Clear Lineage", NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoEditGenbank);
  SetObjectExtra (b, gfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();
}

typedef struct helpindex {
  Int2           item;
  CharPtr        heading;
  CharPtr        section;
  CharPtr        combined;
} HelpIndex, PNTR HelpIndexPtr;

typedef struct helpform {
  FORM_MESSAGE_BLOCK
  DoC            doc;
  DoC            list;
  TexT           findTxt;
  ButtoN         findBtn;
  GrouP          dismissGrp;
  ValNodePtr     mainStrings;
  ValNodePtr     indexStrings;
  ValNodePtr     index;
  Char           file [PATH_MAX];
} HelpForm, PNTR HelpFormPtr;

#define TBL_FMT 1
#define TXT_FMT 2
#define HDG_FMT 3
#define SUB_FMT 4  /* and beyond, encoding indent */

static ParData lstParFmt = {FALSE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData lstColFmt = {0, 0, 80, 0, NULL, 'l', FALSE, FALSE, FALSE, FALSE, TRUE};

static ParData hdgParFmt = {TRUE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData hdgColFmt = {0, 0, 80, 0, NULL, 'c', TRUE, FALSE, FALSE, FALSE, TRUE};

static ParData subParFmt = {TRUE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData subColFmt = {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

static ParData tblParFmt = {TRUE, FALSE, FALSE, FALSE, TRUE, 0, 0};
static ColData tblColFmt = {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

static ParData txtParFmt = {TRUE, FALSE, FALSE, FALSE, FALSE, 0, 0};
static ColData txtColFmt = {0, 0, 80, 0, NULL, 'l', TRUE, FALSE, FALSE, FALSE, TRUE};

static void ResetHelpLists (HelpFormPtr hfp)

{
  HelpIndexPtr  hip;
  ValNodePtr    vnp;

  if (hfp != NULL) {
    hfp->mainStrings = ValNodeFreeData (hfp->mainStrings);
    hfp->indexStrings = ValNodeFreeData (hfp->indexStrings);
    vnp = hfp->index;
    while (vnp != NULL) {
      hip = (HelpIndexPtr) vnp->data.ptrvalue;
      if (hip != NULL) {
        hip->heading = MemFree (hip->heading);
        hip->section = MemFree (hip->section);
        hip->combined = MemFree (hip->combined);
      }
      vnp = vnp->next;
    }
    hfp->index = ValNodeFreeData (hfp->index);
  }
}

static void TrimTrailingSpaces (CharPtr str)

{
  size_t  len;

  if (str != NULL && str [0] != '\0') {
    len = StringLen (str);
    while (len > 0 && str [len - 1] == ' ') {
      len--;
    }
    str [len] = '\0';
  }
}

static Boolean ParseHelpFile (HelpFormPtr hfp, Boolean printPath)

{
  Char          ch;
  Uint1         choice;
  FILE          *fp;
  Boolean       goOn;
  Char          heading [64];
  HelpIndexPtr  hip;
  Boolean       inTable;
  Int2          level;
  ValNodePtr    list;
  Int2          numItems;
  Char          path [PATH_MAX];
  CharPtr       ptr;
  Char          section [64];
  Char          str [256];
  ValNodePtr    vnp;

  if (hfp == NULL || hfp->doc == NULL) return FALSE;
  Hide (hfp->list);
  Hide (hfp->doc);
  Update ();
  Reset (hfp->list);
  Reset (hfp->doc);
  ResetHelpLists (hfp);
  if (hfp->file == NULL || hfp->file [0] == '\0') return FALSE;
  numItems = 0;
  ProgramPath (path, sizeof (path));
  ptr = StringRChr (path, DIRDELIMCHR);
  if (ptr != NULL) {
    *ptr = '\0';
  }
  FileBuildPath (path, NULL, hfp->file);
  fp = FileOpen (path, "r");
  if (fp == NULL) {
    if (GetAppParam ("NCBI", "ErrorProcessing", "MsgPath", NULL, path, sizeof (path))) {
      FileBuildPath (path, NULL, hfp->file);
      fp = FileOpen (path, "r");
    }
  }
  if (fp == NULL) {
    if (GetAppParam ("NCBI", "NCBI", "DATA", NULL, path, sizeof (path))) {
      FileBuildPath (path, NULL, hfp->file);
      fp = FileOpen (path, "r");
    }
  }
  if (fp != NULL) {
    if (printPath) {
      SetTitle (hfp->form, path);
    }
    list = NULL;
    heading [0] = '\0';
    section [0] = '\0';
    inTable = FALSE;
    goOn = (FileGets (str, sizeof (str), fp) != NULL);
    while (goOn) {
      ptr = str;
      ch = *ptr;
      while (ch != '\n' && ch != '\r' && ch != '\0') {
        ptr++;
        ch = *ptr;
      }
      *ptr = '\0';
      if (inTable) {
        TrimTrailingSpaces (str);
      } else {
        TrimSpacesAroundString (str);
      }
      ch = str [0];
      if (ch == '>' || ch == '*' || ch == '#' || ch == '!') {
        if (list != NULL) {
          ptr = MergeValNodeStrings (list, inTable);
          if (inTable) {
            choice = TBL_FMT;
          } else {
            choice = TXT_FMT;
          }
          numItems++;
          vnp = ValNodeAdd (&(hfp->mainStrings));
          if (vnp != NULL) {
            vnp->choice = choice;
            vnp->data.ptrvalue = ptr;
          }
          /* ptr = MemFree (ptr); */
          list = ValNodeFreeData (list);
        }
        ch = str [0];
        if (ch == '>') {
          StringNCpy_0 (heading, str + 1, sizeof (heading));
          numItems++;
          vnp = ValNodeAdd (&(hfp->mainStrings));
          if (vnp != NULL) {
            vnp->choice = HDG_FMT;
            vnp->data.ptrvalue = StringSave (heading);
          }
          vnp = ValNodeAdd (&(hfp->indexStrings));
          if (vnp != NULL) {
            vnp->choice = 0;
            vnp->data.ptrvalue = StringSave (heading);
          }
          vnp = ValNodeAdd (&(hfp->index));
          if (vnp != NULL) {
            hip = (HelpIndexPtr) MemNew (sizeof (HelpIndex));
            vnp->data.ptrvalue = (Pointer) hip;
            if (hip != NULL) {
              hip->item = numItems;
              hip->heading = StringSave (heading);
            }
          }
        } else if (ch == '*') {
          level = 1;
          ch = str [level];
          while (ch == '*') {
            level++;
            ch = str [level];
          }
          StringNCpy_0 (section, str + level, sizeof (section));
          numItems++;
          vnp = ValNodeAdd (&(hfp->mainStrings));
          if (vnp != NULL) {
            if (level < 2) {
              vnp->choice = SUB_FMT;
            } else {
              vnp->choice = 5 * (level - 1);
            }
            vnp->data.ptrvalue = StringSave (section);
          }
          vnp = ValNodeAdd (&(hfp->indexStrings));
          if (vnp != NULL) {
            vnp->choice = 5 * level;
            vnp->data.ptrvalue = StringSave (section);
          }
          vnp = ValNodeAdd (&(hfp->index));
          if (vnp != NULL) {
            hip = (HelpIndexPtr) MemNew (sizeof (HelpIndex));
            vnp->data.ptrvalue = (Pointer) hip;
            if (hip != NULL) {
              hip->item = numItems;
              hip->heading = StringSave (heading);
              hip->section = StringSave (section);
              sprintf (str, "%s|%s", heading, section);
              hip->combined = StringSave (str);
            }
          }
        } else if (ch == '#' || ch == '!') {
          inTable = (Boolean) (ch == '!');
          vnp = ValNodeAdd (&list);
          if (vnp != NULL) {
            if (! StringHasNoText (str + 1)) {
              vnp->data.ptrvalue = StringSave (str + 1);
            }
          }
        }
      } else if (ch == '<') {
      } else if (ch != '\0') {
        vnp = ValNodeAdd (&list);
        if (vnp != NULL) {
          if (! StringHasNoText (str)) {
            vnp->data.ptrvalue = StringSave (str);
          }
        }
      }
      goOn = (Boolean) (goOn && (FileGets (str, sizeof (str), fp) != NULL));
    }
    if (list != NULL) {
      ptr = MergeValNodeStrings (list, inTable);
      if (inTable) {
        choice = TBL_FMT;
      } else {
        choice = TXT_FMT;
      }
      numItems++;
      vnp = ValNodeAdd (&(hfp->mainStrings));
      if (vnp != NULL) {
        vnp->choice = choice;
        vnp->data.ptrvalue = ptr;
      }
      /* ptr = MemFree (ptr); */
      list = ValNodeFreeData (list);
    }
    FileClose (fp);
    return TRUE;
  } else {
    return FALSE;
  }
}

static Int2 GetSequinAppParam (CharPtr section, CharPtr type, CharPtr dflt, CharPtr buf, Int2 buflen)

{
  Int2  rsult;

  rsult = GetAppParam ("SEQUINCUSTOM", section, type, NULL, buf, buflen);
  if (rsult) return rsult;
  rsult = GetAppParam ("SEQUIN", section, type, dflt, buf, buflen);
  return rsult;
}

static FonT GetHelpFontFromConfig (CharPtr param, FonT dfault)

{
  FonT  f;
  Char  str [128];

  f = dfault;
  if (GetSequinAppParam ("SCREEN", param, NULL, str, sizeof (str))) {
    f = Nlm_ParseFontEx (str, NULL);
  }
  if (f == NULL) {
    f = dfault;
  }
  return f;
}

static void SetupHelpFonts (void)

{
  if (IsJapanese()) {
#ifdef WIN_MAC
    /* Osaka is common font for Japanese on Macintosh. */
    hdgColFmt.font = Nlm_ParseFontEx ("Osaka,14,b", NULL);
    subColFmt.font = Nlm_ParseFontEx ("Osaka,10,b", NULL);
    txtColFmt.font = Nlm_ParseFontEx ("Osaka,10", NULL);
    lstColFmt.font = Nlm_ParseFontEx ("Osaka\x81\x7c\x93\x99\x95\x9d,12", NULL);
    tblColFmt.font = Nlm_ParseFontEx ("Osaka\x81\x7c\x93\x99\x95\x9d,12", NULL);
/*    hdgColFmt.font = GetResidentFont (Nlm_Minchou(14, STYLE_BOLD));	*/
/*    subColFmt.font = GetResidentFont (Nlm_Minchou(10, STYLE_BOLD));	*/
/*    txtColFmt.font = GetResidentFont (Nlm_Gothic(10, STYLE_REGULAR));	*/
/*    lstColFmt.font = GetResidentFont (Nlm_MinchouFixed(12, STYLE_REGULAR));	*/
/*    tblColFmt.font = GetResidentFont (Nlm_MinchouFixed(12, STYLE_REGULAR));	*/
#endif
#ifdef WIN_MSWIN
    hdgColFmt.font = Nlm_ParseFontEx ("\x82\x6c\x82\x72\x20\x82\x6f\x96\xbe\x92\xa9,14,b,Kanji", NULL);
    subColFmt.font = Nlm_ParseFontEx ("\x82\x6c\x82\x72\x20\x82\x6f\x96\xbe\x92\xa9,11,b,Kanji", NULL);
    txtColFmt.font = Nlm_ParseFontEx ("\x82\x6c\x82\x72\x20\x82\x6f\x83\x53\x83\x56\x83\x62\x83\x4e,11,,Kanji", NULL);
    lstColFmt.font = Nlm_ParseFontEx ("\x82\x6c\x82\x72\x20\x96\xbe\x92\xa9,12,f,Kanji", NULL);
    tblColFmt.font = Nlm_ParseFontEx ("\x82\x6c\x82\x72\x20\x96\xbe\x92\xa9,12,f,Kanji", NULL);
/*    hdgColFmt.font = GetResidentFont (Nlm_Minchou(14, STYLE_BOLD));	*/
/*    subColFmt.font = GetResidentFont (Nlm_Minchou(11, STYLE_BOLD));	*/
/*    txtColFmt.font = GetResidentFont (Nlm_Gothic(11, STYLE_REGULAR));	*/
/*    lstColFmt.font = GetResidentFont (Nlm_MinchouFixed(12, STYLE_REGULAR));	*/
/*    tblColFmt.font = GetResidentFont (Nlm_MinchouFixed(12, STYLE_REGULAR));	*/
#endif
#ifdef WIN_MOTIF
    hdgColFmt.font = ParseFont ("Times,18,b");
    subColFmt.font = ParseFont ("Helvetica,12,b");
    txtColFmt.font = ParseFont ("fixed,13");
    lstColFmt.font = programFont;
    tblColFmt.font = programFont;
#endif
  } else if (IsEnglish() || IsFrench() || IsGerman() || IsItalian() || IsSystemLang()) {
#ifdef WIN_MAC
    hdgColFmt.font = ParseFont ("Times,14,b");
    subColFmt.font = ParseFont ("Geneva,10,b");
    txtColFmt.font = ParseFont ("Geneva,10");
#endif /*WIN_MAC*/
#ifdef WIN_MSWIN
    hdgColFmt.font = ParseFont ("Times New Roman,14,b");
    subColFmt.font = ParseFont ("Arial,11,b");
    txtColFmt.font = ParseFont ("Times New Roman,11");
#endif /*WIN_MSWIN*/
#ifdef WIN_MOTIF
    hdgColFmt.font = ParseFont ("Times,18,b");
    subColFmt.font = ParseFont ("Helvetica,12,b");
    txtColFmt.font = ParseFont ("Times,14");
#endif /*WIN_MOTIF*/
    lstColFmt.font = programFont;
    tblColFmt.font = programFont;
  } else {
  /* above call to IsSystemLang should override this section */
    /* default system font is often native character set */
    /* because native character sets have a set of ascii letters,
       english help file can also be drawn as well as native letters. */
    hdgColFmt.font = systemFont;
    subColFmt.font = systemFont;
    txtColFmt.font = systemFont;
    lstColFmt.font = systemFont;
    tblColFmt.font = systemFont;
  }

  /* now allow override by sequin config file */
  hdgColFmt.font = GetHelpFontFromConfig ("HEADING", hdgColFmt.font);
  subColFmt.font = GetHelpFontFromConfig ("SUBHEAD", subColFmt.font);
  txtColFmt.font = GetHelpFontFromConfig ("TEXT", txtColFmt.font);
  lstColFmt.font = GetHelpFontFromConfig ("LIST", lstColFmt.font);
  tblColFmt.font = GetHelpFontFromConfig ("TABLE", tblColFmt.font);
}

static Boolean PopulateHelpForm (HelpFormPtr hfp)

{
  Int2        firstLine;
  Int2        firstShown;
  RecT        r;
  BaR         sb;
  Int4        startsAt;
  CharPtr     text;
  ValNodePtr  vnp;

  if (hfp == NULL || hfp->doc == NULL) return FALSE;
  Hide (hfp->list);
  Hide (hfp->doc);
  Update ();
  if (! GetScrlParams4 (hfp->doc, NULL, &firstShown, &firstLine)) {
    firstShown = 0;
    firstLine = 0;
  }
  sb = GetSlateVScrollBar ((SlatE) hfp->doc);
  Reset (hfp->list);
  Reset (hfp->doc);
  if (hfp->file == NULL || hfp->file [0] == '\0') return FALSE;
  if (hfp->mainStrings == NULL || hfp->indexStrings == NULL) return FALSE;
  ObjectRect (hfp->doc, &r);
  InsetRect (&r, 4, 4);
  lstColFmt.pixWidth = r.right - r.left;
  hdgColFmt.pixWidth = r.right - r.left;
  subColFmt.pixWidth = r.right - r.left;
  /*
  tblColFmt.pixWidth = screenRect.right - screenRect.left;
  */
  tblColFmt.pixWidth = r.right - r.left;
  tblColFmt.pixInset = 10;
  txtColFmt.pixWidth = r.right - r.left;
  txtColFmt.pixInset = 10;

  for (vnp = hfp->indexStrings; vnp != NULL; vnp = vnp->next) {
    lstColFmt.pixInset = vnp->choice;
    AppendText (hfp->list, vnp->data.ptrvalue, &lstParFmt, &lstColFmt, programFont);
  }

  for (vnp = hfp->mainStrings; vnp != NULL; vnp = vnp->next) {
    switch (vnp->choice) {
      case TBL_FMT :
        AppendText (hfp->doc, vnp->data.ptrvalue, &tblParFmt, &tblColFmt, programFont);
        break;
      case TXT_FMT :
        AppendText (hfp->doc, vnp->data.ptrvalue, &txtParFmt, &txtColFmt, programFont);
        break;
      case HDG_FMT :
        AppendText (hfp->doc, vnp->data.ptrvalue, &hdgParFmt, &hdgColFmt, systemFont);
        break;
      case SUB_FMT :
        subColFmt.pixInset = 0;
        AppendText (hfp->doc, vnp->data.ptrvalue, &subParFmt, &subColFmt, programFont);
        break;
      default :
        subColFmt.pixInset = vnp->choice;
        AppendText (hfp->doc, vnp->data.ptrvalue, &subParFmt, &subColFmt, programFont);
        break;
    }
  }

  UpdateDocument (hfp->list, 0, 0);
  /*
  UpdateDocument (hfp->doc, 0, 0);
  */
  text = GetDocText (hfp->doc, firstShown, 0, 0);
  MemFree (text);
  AdjustDocScroll (hfp->doc);
  GetItemParams4 (hfp->doc, firstShown, &startsAt, NULL, NULL, NULL, NULL);
  CorrectBarValue (sb, startsAt + firstLine);
  Show (hfp->list);
  Show (hfp->doc);
  Update ();
  return TRUE;
}

static void RefreshHelpForm (ButtoN b)

{
  HelpFormPtr  hfp;

  hfp = (HelpFormPtr) GetObjectExtra (b);
  if (hfp == NULL) return;
  ParseHelpFile (hfp, TRUE);
  PopulateHelpForm (hfp);
  Update ();
}

static void CleanupHelpForm (GraphiC g, VoidPtr data)

{
  ResetHelpLists ((HelpFormPtr) data);
  StdCleanupFormProc (g, data);
}

static void HelpListNotify (DoC d, Int2 item, Int2 row, Int2 col, Boolean dblclick)

{
  HelpFormPtr   hfp;
  HelpIndexPtr  hip;
  BaR           sb;
  Int2          startsAt;
  ValNodePtr    vnp;

  hfp = (HelpFormPtr) GetObjectExtra (d);
  if (hfp == NULL || hfp->doc == NULL) return;
  if (item == 0 || row == 0 || col == 0) return;
  vnp = hfp->index;
  while (vnp != NULL && item > 1) {
    item--;
    vnp = vnp->next;
  }
  if (vnp != NULL) {
    hip = (HelpIndexPtr) vnp->data.ptrvalue;
    if (hip != NULL) {
      GetItemParams (hfp->doc, hip->item, &startsAt, NULL, NULL, NULL, NULL);
      ResetClip ();
      sb = GetSlateVScrollBar ((SlatE) hfp->doc);
      SetValue (sb, startsAt);
      Update ();
    }
  }
}

static void ResizeHelpForm (WindoW w)

{
  Int2         delta;
  Int2         diff;
  Int2         gap;
  Int2         height;
  HelpFormPtr  hfp;
  RecT         r;
  RecT         s;
  RecT         t;
  Int2         width;

  hfp = (HelpFormPtr) GetObjectExtra (w);
  if (hfp != NULL) {
    ObjectRect (w, &r);
    width = r.right - r.left;
    height = r.bottom - r.top;
    GetPosition (hfp->doc, &s);
    GetPosition (hfp->dismissGrp, &t);
    diff = t.bottom - t.top;
    gap = t.top - s.bottom;
    t.bottom = height - s.left;
    t.top = t.bottom - diff;
    delta = (width - t.right - t.left) / 2;
    t.left += delta;
    t.right += delta;
    s.right = width - s.left;
    /*
    s.bottom = height - s.left;
    */
    s.bottom = t.top - gap;
    SetPosition (hfp->dismissGrp, &t);
    SetPosition (hfp->doc, &s);
    AdjustPrnt (hfp->doc, &s, FALSE);
    PopulateHelpForm (hfp);
    Update ();
  }
}

static Boolean ScrollToTextInDoc (DoC d, Int2 item, CharPtr text)

{
  Pointer  data;
  Boolean  found;
  RecT     rct;
  BaR      sb;
  Int4     startsAt;
  CharPtr  str;
  WindoW   tempPort;

  found = FALSE;
  GetItemParams4 (d, item, &startsAt, NULL, NULL, NULL, &data);
  str = (CharPtr) data;
  if (StringISearch (str, text) != NULL) {
    found = TRUE;
  }
  if (found) {
    tempPort = SavePort (d);
    Select (d);
    sb = GetSlateVScrollBar ((SlatE) d);
    CorrectBarValue (sb, startsAt);
    ObjectRect (d, &rct);
    InsetRect (&rct, 4, 4);
    InsetRect (&rct, -1, -1);
    InvalRect (&rct);
    RestorePort (tempPort);
    Update ();
  }
  return found;
}

static void FindHelpBtnProc (ButtoN b)

{
  Int2         firstShown;
  HelpFormPtr  hfp;
  Int2         i;
  Int2         numItems;
  Char         str [256];

  hfp = (HelpFormPtr) GetObjectExtra (b);
  if (hfp == NULL) return;
  GetTitle (hfp->findTxt, str, sizeof (str) - 1);
  GetDocParams (hfp->doc, &numItems, NULL);
  if (GetScrlParams (hfp->doc, NULL, &firstShown, NULL)) {
    for (i = firstShown + 1; i <= numItems; i++) {
      if (ScrollToTextInDoc (hfp->doc, i, str)) {
        return;
      }
    }
    for (i = 1; i < firstShown; i++) {
      if (ScrollToTextInDoc (hfp->doc, i, str)) {
        return;
      }
    }
  }
}

static void FindHelpTextProc (TexT t)

{
  HelpFormPtr  hfp;

  hfp = (HelpFormPtr) GetObjectExtra (t);
  if (hfp == NULL) return;
  if (TextLength (t) > 0) {
    SafeEnable (hfp->findBtn);
  } else {
    SafeDisable (hfp->findBtn);
  }
}

static void HelpFormMessage (ForM f, Int2 mssg)

{
  FILE         *fp;
  HelpFormPtr  hfp;
  Char         path [PATH_MAX];

  hfp = (HelpFormPtr) GetObjectExtra (f);
  if (hfp != NULL) {
    switch (mssg) {
      case VIB_MSG_CLOSE :
        Hide (f);
        break;
      case VIB_MSG_PRINT :
        PrintDocument (hfp->doc);
        break;
      case VIB_MSG_EXPORT :
        if (GetOutputFileName (path, sizeof (path), "help.txt")) {
          WatchCursor ();
#ifdef WIN_MAC
          fp = FileOpen (path, "r");
          if (fp != NULL) {
            FileClose (fp);
          } else {
            FileCreate (path, "TEXT", "ttxt");
          }
#endif
          fp = FileOpen (path, "w");
          if (fp != NULL) {
            SaveDocument (hfp->doc, fp);
            FileClose (fp);
          }
          ArrowCursor ();
        }
        break;
      default :
        if (hfp->appmessage != NULL) {
          hfp->appmessage (f, mssg);
        }
        break;
    }
  }
}

static void HelpFormActivate (WindoW w)

{
  IteM         exportItm;
  HelpFormPtr  hfp;

  hfp = (HelpFormPtr) GetObjectExtra (w);
  if (hfp != NULL) {
    if (hfp->activate != NULL) {
      hfp->activate (w);
    }
    exportItm = FindFormMenuItem ((BaseFormPtr) hfp, VIB_MSG_EXPORT);
    SafeSetTitle (exportItm, "Export Help...");
  }
}

extern ForM CreateHelpForm (Int2 left, Int2 top, CharPtr title,
                            CharPtr file, BtnActnProc closeForm,
                            WndActnProc activateForm)

{
  ButtoN             b;
  GrouP              c;
  GrouP              h;
  Int2               height;
  HelpFormPtr        hfp;
  StdEditorProcsPtr  sepp;
  WindoW             w;
#ifndef WIN_MAC
  MenU               m;
#endif

  w = NULL;
  hfp = MemNew (sizeof (HelpForm));
  if (hfp != NULL) {
    w = DocumentWindow (left, top, -10, -10, title,
                        StdSendCloseWindowMessageProc, ResizeHelpForm);
    SetObjectExtra (w, hfp, CleanupHelpForm);
    hfp->form = (ForM) w;
    hfp->formmessage = HelpFormMessage;

    sepp = (StdEditorProcsPtr) GetAppProperty ("StdEditorForm");
    if (sepp != NULL) {
      hfp->appmessage = sepp->handleMessages;
    }

    StringNCpy_0 (hfp->file, file, sizeof (hfp->file));

#ifndef WIN_MAC
    m = PulldownMenu (w, "File");
    FormCommandItem (m, "Export", (BaseFormPtr) hfp, VIB_MSG_EXPORT);
    SeparatorItem (m);
#ifdef WIN_MSWIN
    FormCommandItem (m, "Print", (BaseFormPtr) hfp, VIB_MSG_PRINT);
    SeparatorItem (m);
#endif
    FormCommandItem (m, "Close", (BaseFormPtr) hfp, VIB_MSG_CLOSE);
#endif

    c = HiddenGroup (w, 4, 0, NULL);
    StaticPrompt (c, "Find", 0, dialogTextHeight, programFont, 'l');
    hfp->findTxt = DialogText (c, "", 20, FindHelpTextProc);
    SetObjectExtra (hfp->findTxt, hfp, NULL);
    hfp->findBtn = PushButton (c, "Find", FindHelpBtnProc);
    SetObjectExtra (hfp->findBtn, hfp, NULL);
    Disable (hfp->findBtn);

    h = HiddenGroup (w, -1, 0, NULL);
    SelectFont (programFont);
    height = LineHeight ();
    SelectFont (systemFont);
    hfp->list = DocumentPanel (h, stdCharWidth * 28, height * 5);
    SetDocAutoAdjust (hfp->list, FALSE);
    SetObjectExtra (hfp->list, hfp, NULL);
    SetDocNotify (hfp->list, HelpListNotify);

    hfp->doc = DocumentPanel (h, stdCharWidth * 33, stdLineHeight * 20);
    SetDocAutoAdjust (hfp->doc, FALSE);
    SetObjectExtra (hfp->doc, hfp, NULL);

    hfp->dismissGrp = HiddenGroup (w, 2, 0, NULL);
    PushButton (hfp->dismissGrp, "Dismiss", closeForm);
    if (indexerVersion) {
      b = PushButton (hfp->dismissGrp, "Refresh", RefreshHelpForm);
      SetObjectExtra (b, hfp, NULL);
    }

    AlignObjects (ALIGN_CENTER, (HANDLE) hfp->doc, (HANDLE) hfp->dismissGrp, NULL);

    RealizeWindow (w);

    SetupHelpFonts ();

    if (activateForm != NULL) {
      hfp->activate = activateForm;
    } else {
      if (sepp != NULL) {
        hfp->activate = sepp->activateForm;
      }
    }
    SetActivate (w, HelpFormActivate);
    HelpFormActivate ((WindoW) hfp->form);

    if (ParseHelpFile (hfp, FALSE)) {
      if (! PopulateHelpForm (hfp)) {
        w = Remove (w);
      }
    } else {
      w = Remove (w);
    }
  }
  return (ForM) w;
}

extern void SendHelpScrollMessage (ForM f, CharPtr heading, CharPtr section)

{
  HelpFormPtr   hfp;
  HelpIndexPtr  hip;
  BaR           sb;
  Int2          startsAt;
  CharPtr       str;
  Char          txt [256];
  Boolean       useBoth;
  Boolean       useHeading;
  Boolean       useSection;
  ValNodePtr    vnp;

  hfp = (HelpFormPtr) GetObjectExtra (f);
  if (hfp != NULL) {
    vnp = hfp->index;
    txt [0] = '\0';
    useBoth = FALSE;
    useHeading = FALSE;
    useSection = FALSE;
    if (heading != NULL && *heading != '\0' && section != NULL && *section != '\0') {
      useBoth = TRUE;
      if (StringLen (heading) + StringLen (section) < sizeof (txt) - 2) {
        StringCpy (txt, heading);
        StringCat (txt, "|");
        StringCat (txt, section);
      }
    } else if (heading != NULL && *heading != '\0') {
      useHeading = TRUE;
      StringNCpy_0 (txt, heading, sizeof (txt));
    } else if (section != NULL && *section != '\0') {
      useSection = TRUE;
      StringNCpy_0 (txt, section, sizeof (txt));
    }
    while (vnp != NULL) {
      hip = (HelpIndexPtr) vnp->data.ptrvalue;
      if (hip != NULL) {
        if (useBoth) {
          str = hip->combined;
        } else if (useHeading) {
          str = hip->heading;
        } else if (useSection) {
          str = hip->section;
        } else {
          str = NULL;
        }
        if (str != NULL && StringICmp (txt, str) == 0) {
          GetItemParams (hfp->doc, hip->item, &startsAt, NULL, NULL, NULL, NULL);
          sb = GetSlateVScrollBar ((SlatE) hfp->doc);
          ResetClip ();
          SetValue (sb, startsAt);
          Update ();
          return;
        }
      }
      vnp = vnp->next;
    }
  }
}

