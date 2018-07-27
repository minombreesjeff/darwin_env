/*   sequin10.c
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
* File Name:  sequin10.c
*
* Author:  Colleen Bollin
*
* Version Creation Date:   9/3/2003
*
* $Revision: 1.213 $
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
#include <sqnutils.h>
#include <subutil.h>
#include <explore.h>
#include <edutil.h>
#include <tofasta.h>
#include <gbftdef.h>
#include <gbfeat.h>

/* The matchFunction functions are used to identify features that meet
 * specific requirements, usually that the feature is of a particular type.
 * This function is used instead of simply using the subtype for the feature
 * because some features are identified based on the contents or presence of
 * certain modifiers.
 * Functions of this type should always return FALSE if handed a NULL argument.
 */
typedef Boolean (LIBCALLBACK *matchFunction) (
  SeqFeatPtr sfp
);

static void ListClauses (
  ValNodePtr clauselist,
  ValNodePtr PNTR strings,
  Boolean    allow_semicolons,
  Boolean    suppress_final_and
);

static void LabelClauses (
  ValNodePtr clause_list,
  Uint1 biomol,
  BioseqPtr bsp
);

static CharPtr GetProductName (
  SeqFeatPtr cds,
  BioseqPtr bsp
);

#define DEFLINE_FEATLIST    1
#define DEFLINE_CLAUSEPLUS  2
#define DEFLINE_REMOVEFEAT  3

typedef struct featurelabeldata {
  Boolean pluralizable;
  Boolean is_typeword_first;
  CharPtr typeword;
  CharPtr description;
  CharPtr productname;
} FeatureLabelData, PNTR FeatureLabelPtr;


typedef struct featureclause {
  ValNodePtr       featlist;
  FeatureLabelData feature_label_data;
  CharPtr          allelename;
  CharPtr          interval;
  Boolean          is_alt_spliced;
  SeqLocPtr        slp;
  GeneRefPtr       grp;
  Boolean          clause_info_only;
  Boolean          is_unknown;
  Boolean          make_plural;
  Boolean          delete_me;
  /* this information used only for segments */
  Int2             numivals;
  Int4Ptr          ivals;
} FeatureClauseData, PNTR FeatureClausePtr;

FeatureClausePtr NewFeatureClause (
  SeqFeatPtr sfp,
  BioseqPtr bsp
);

static void PluralizeConsolidatedClauseDescription (
  FeatureClausePtr fcp
);

typedef Boolean (LIBCALLBACK *ShouldRemoveFunction) (
  SeqFeatPtr sfp,
  FeatureClausePtr parent_fcp,
  FeatureClausePtr this_fcp,
  BioseqPtr        bsp,
  Boolean          isLonely,
  Boolean          isRequested,
  Boolean          isSegment
);

/* This section of the code contains some functions for dealing with
 * linked lists of strings */

/* This function finds the first occurrence of "search" in one of the
 * strings in list "strings".
 * "search" could be part of the string or could be the entire string.
 */
static ValNodePtr FindStringInStrings (
  ValNodePtr strings,
  CharPtr search
)
{
  while (strings != NULL)
  {
    if (StringStr (strings->data.ptrvalue, search))
    {
      return strings;
    }
    strings = strings->next;
  }
  return NULL;
}

/* This function finds the first item in "strings" that is identical to
 * "value".
 */
extern ValNodePtr FindExactStringInStrings (
  ValNodePtr strings,
  CharPtr    value
)
{
  ValNodePtr    string_match;

  for (string_match = FindStringInStrings (strings, value);
       string_match != NULL
         && StringCmp (string_match->data.ptrvalue, value) != 0;
       string_match = FindStringInStrings (string_match->next, value))
  {}
  return string_match;
}

/* This function creates a new linked list of strings with copies of
 * contents of orig.
 */
static ValNodePtr CopyStrings (
  ValNodePtr orig
)
{
  ValNodePtr new_string_start = NULL;

  while (orig != NULL)
  {
    ValNodeAddStr (&new_string_start, 0,
      StringSave (orig->data.ptrvalue));
    orig = orig->next;
  }
  return new_string_start;
}

/*
 * This section of the code contains functions and structures for obtaining a
 * description of the organism in the record, including functions for finding
 * the combination of modifiers that will make each organism description 
 * unique.
 * The method used for determining the best combination of modifiers involves
 * creating a list of required modifiers, and then creating a list of
 * combinations of modifiers by adding modifiers one at a time
 * to see if the additional modifiers provide any more differentiation in
 * the list.
 * In order to do this, I start with a list of required modifiers, and 
 * then create copies of this list.  For each copy I add one of the modifiers
 * that are present in the bio sources and not already on the list.
 * If adding the modifier increases the differentiation, I add that copy to
 * the list of possible combinations, otherwise I discard it.
 * I then make copies of all of the new items I added to the list and
 * add another modifier to each list, keeping the combinations that increase
 * the differentiation and discarding the rest.
 * This process continues until I have a combination that produces completely
 * differentiated bio sources, or I run out of possible combinations.
 * If I run out of possible combinations, I select the best combination from
 * the list.
 * This search process occurs in FindBestCombo.  The majority of the functions
 * in this section are here to support FindBestCombo, specifically to create,
 * copy, and grow lists of combinations. 
 */

/* BioSrcDescData is used to calculate the best possible combination of
 * source and organism modifiers for uniqueness.
 * biop contains the BioSourcePtr from a sequence in the record.
 * strings contains a list of string representations of the modifiers
 * for this combination for this organism.
 */
typedef struct biosrcdescdata {
  BioSourcePtr  biop;
  ValNodePtr    strings;
  Pointer       next;
} BioSrcDescData, PNTR BioSrcDescPtr;

/* OrgGroupData is used to calculate the best possible combination of
 * source and organism modifiers for uniqueness.
 * org_list is a list of all organisms that have identical descriptions
 * using the current set of modifiers.
 * num_organisms contains the number of organisms with identical descriptions.
 */
typedef struct orggroupdata {
  BioSrcDescPtr org_list;
  Int4          num_organisms;
  Pointer       next;
} OrgGroupData, PNTR OrgGroupPtr;

/* ModifierCombinationData is used to calculate the best possible combination
 * of source and organism modifiers for uniqueness.
 * num_groups is the number of groups of organisms with identical descriptions
 *           using the modifiers specified in modifier_indices.
 * num_mods is the number of modifiers specified in modifier_indices.
 * max_orgs_in_group is the maximum number of organisms in any one group.
 * num_unique_orgs is the number of organisms that are alone in their groups
 *           i.e., their description is unique.
 * modifier_indices is the list of modifier indices for this combination.
 * group_list is the list of groups of organisms with identical descriptions
 *           using the modifiers specified in modifier_indices.
 */
typedef struct modifiercombination {
  Int4         num_groups;
  Int4         num_mods;
  Int4         max_orgs_in_group;
  Int4         num_unique_orgs;
  ValNodePtr   modifier_indices;
  OrgGroupPtr  group_list;
  Pointer      next;
} ModifierCombinationData, PNTR ModifierCombinationPtr;

/* ModifierItemGlobalData is used to store information about the available
 * modifiers - the name to use when displaying a list of checkboxes, whether
 * the modifier is an Organism modifier or a Source modifier, the subtype
 * to use when looking for the modifier in the organism qualifier list,
 * and whether this modifier is required by default.
 */
typedef struct modifieritemglobal {
  CharPtr       name;
  Boolean       isOrgMod;
  Uint1         subtype;
  Uint1         default_required;
} ModifierItemGlobalData, PNTR ModifierItemGlobalPtr;

/* ModifierItemLocalData is used to store information about the results of
 * a search of the set of organisms in a record and the results of user
 * input to a dialog for deciding which modifiers should be used in the
 * organism description.
 */
typedef struct modifieritemlocal {
  ButtoN        button;
  Boolean       any_present;
  Boolean       all_present;
  Boolean       is_unique;
  CharPtr       first_value_seen;
  ValNodePtr    values_seen;
  Boolean       all_unique;
  CharPtr       status;
  Boolean       required;
} ModifierItemLocalData, PNTR ModifierItemLocalPtr;

/* OrganismDescriptionModifiers is used to apply specific user preferences
 * for how to construct the organism descriptions - whether or not to use
 * labels for the modifiers, whether and how to limit the number of modifiers
 * used in any one organism description, whether to keep or remove modifier
 * information in parentheses in the organism taxonomy name, and whether or
 * not to apply modifiers to organisms with "sp." in the middle of the
 * taxonomy name.
 */
typedef struct organismdescriptionmodifiers {
  Boolean  use_labels;
  Int2     max_mods;
  Boolean  keep_paren;
  Boolean  exclude_sp;
  Int4     clone_isolate_HIV_rule_num;
  Boolean  use_modifiers;
} OrganismDescriptionModifiers, PNTR OrganismDescriptionModifiersPtr;
/* This is a list of the modifiers that are of interest */
/* Note that if you modify the DefLineModifiers array, */ 
/* you should make the corresponding change to the DefLinePos enum. */

static ModifierItemGlobalData DefLineModifiers[] = {
  { "Acronym"              , TRUE , ORGMOD_acronym              , FALSE },
  { "Anamorph"             , TRUE , ORGMOD_anamorph             , FALSE },
  { "Authority"            , TRUE , ORGMOD_authority            , FALSE },
  { "Biotype"              , TRUE , ORGMOD_biotype              , FALSE },
  { "Biovar"               , TRUE , ORGMOD_biovar               , FALSE },
  { "Breed"                , TRUE , ORGMOD_breed                , FALSE },
  { "Cell-line"            , FALSE, SUBSRC_cell_line            , FALSE },
  { "Cell-type"            , FALSE, SUBSRC_cell_type            , FALSE },
  { "Chemovar"             , TRUE , ORGMOD_chemovar             , FALSE },
  { "Chromosome"           , FALSE, SUBSRC_chromosome           , FALSE },
  { "Clone"                , FALSE, SUBSRC_clone                , FALSE },
  { "Clone-lib"            , FALSE, SUBSRC_clone_lib            , FALSE },
  { "Common"               , TRUE , ORGMOD_common               , FALSE },
  { "Country"              , FALSE, SUBSRC_country              , FALSE },
  { "Cultivar"             , TRUE , ORGMOD_cultivar             , FALSE },
  { "Dev-stage"            , FALSE, SUBSRC_dev_stage            , FALSE },
  { "Dosage"               , TRUE , ORGMOD_dosage               , FALSE },
  { "Ecotype"              , TRUE , ORGMOD_ecotype              , FALSE },
  { "Endogenous-virus-name", FALSE, SUBSRC_endogenous_virus_name, TRUE  },
  { "Environmental-sample" , FALSE, SUBSRC_environmental_sample , FALSE },
  { "Forma"                , TRUE , ORGMOD_forma                , FALSE },
  { "Forma-specialis"      , TRUE , ORGMOD_forma_specialis      , FALSE },
  { "Frequency"            , FALSE, SUBSRC_frequency            , FALSE },
  { "Genotype"             , FALSE, SUBSRC_genotype             , FALSE },
  { "Germline"             , FALSE, SUBSRC_germline             , FALSE },
  { "Group"                , TRUE , ORGMOD_group                , FALSE },
  { "Haplotype"            , FALSE, SUBSRC_haplotype            , FALSE },
  { "Ins-seq-name"         , FALSE, SUBSRC_insertion_seq_name   , FALSE },
  { "Isolate"              , TRUE , ORGMOD_isolate              , FALSE },
  { "Isolation-source"     , FALSE, SUBSRC_isolation_source     , FALSE },
  { "Lab-host"             , FALSE, SUBSRC_lab_host             , FALSE },
  { "Map"                  , FALSE, SUBSRC_map                  , FALSE },
  { "Old-lineage"          , TRUE , ORGMOD_old_lineage          , FALSE },
  { "Old-name"             , TRUE , ORGMOD_old_name             , FALSE },
  { "Pathovar"             , TRUE , ORGMOD_pathovar             , FALSE },
  { "Plasmid-name"         , FALSE, SUBSRC_plasmid_name         , TRUE  },
  { "Plastid-name"         , FALSE, SUBSRC_plastid_name         , FALSE },
  { "Pop-variant"          , FALSE, SUBSRC_pop_variant          , FALSE },
  { "Rearranged"           , FALSE, SUBSRC_rearranged           , FALSE },
  { "Segment"              , FALSE, SUBSRC_segment              , FALSE },
  { "Serogroup"            , TRUE , ORGMOD_serogroup            , FALSE },
  { "Serotype"             , TRUE , ORGMOD_serotype             , FALSE },
  { "Serovar"              , TRUE , ORGMOD_serovar              , FALSE },
  { "Sex"                  , FALSE, SUBSRC_sex                  , FALSE },
  { "Specific-host"        , TRUE , ORGMOD_nat_host             , FALSE },
  { "Specimen voucher"     , TRUE , ORGMOD_specimen_voucher     , FALSE },
  { "Strain"               , TRUE , ORGMOD_strain               , FALSE },
  { "Subclone"             , FALSE, SUBSRC_subclone             , FALSE },
  { "Subgroup"             , TRUE , ORGMOD_subgroup             , FALSE },
  { "Sub-species"          , TRUE , ORGMOD_sub_species          , FALSE },
  { "Substrain"            , TRUE , ORGMOD_substrain            , FALSE },
  { "Subtype"              , TRUE , ORGMOD_subtype              , FALSE },
  { "Synonym"              , TRUE , ORGMOD_synonym              , FALSE },
  { "Teleomorph"           , TRUE , ORGMOD_teleomorph           , FALSE },
  { "Tissue-lib"           , FALSE, SUBSRC_tissue_lib           , FALSE },
  { "Tissue-type"          , FALSE, SUBSRC_tissue_type          , FALSE },
  { "Transgenic"           , FALSE, SUBSRC_transgenic           , TRUE  },
  { "Transposon-name"      , FALSE, SUBSRC_transposon_name      , FALSE },
  { "Type"                 , TRUE , ORGMOD_type                 , FALSE },
  { "Variety"              , TRUE , ORGMOD_variety              , FALSE }
};

typedef enum {
  DEFLINE_POS_Acronym = 0,
  DEFLINE_POS_Anamorph,
  DEFLINE_POS_Authority,
  DEFLINE_POS_Biotype,
  DEFLINE_POS_Biovar,
  DEFLINE_POS_Breed,
  DEFLINE_POS_Cell_line,
  DEFLINE_POS_Cell_type,
  DEFLINE_POS_Chemovar,
  DEFLINE_POS_Chromosome,
  DEFLINE_POS_Clone,
  DEFLINE_POS_Clone_lib,
  DEFLINE_POS_Common,
  DEFLINE_POS_Country,
  DEFLINE_POS_Cultivar,
  DEFLINE_POS_Dev_stage,
  DEFLINE_POS_Dosage,
  DEFLINE_POS_Ecotype,
  DEFLINE_POS_Endogenous_virus_name,
  DEFLINE_POS_Environmental_sample,
  DEFLINE_POS_Forma,
  DEFLINE_POS_Forma_specialis,
  DEFLINE_POS_Frequency,
  DEFLINE_POS_Genotype,
  DEFLINE_POS_Germline,
  DEFLINE_POS_Group,
  DEFLINE_POS_Haplotype,
  DEFLINE_POS_Ins_seq_name,
  DEFLINE_POS_Isolate,
  DEFLINE_POS_Isolation_source,
  DEFLINE_POS_Lab_host,
  DEFLINE_POS_Map,
  DEFLINE_POS_Old_lineage,
  DEFLINE_POS_Old_name,
  DEFLINE_POS_Pathovar,
  DEFLINE_POS_Plasmid_name,
  DEFLINE_POS_Plastid_name,
  DEFLINE_POS_Pop_variant,
  DEFLINE_POS_Rearranged,
  DEFLINE_POS_Segment,
  DEFLINE_POS_Serogroup,
  DEFLINE_POS_Serotype,
  DEFLINE_POS_Serovar,
  DEFLINE_POS_Sex,
  DEFLINE_POS_Specific_host,
  DEFLINE_POS_Specimen_voucher,
  DEFLINE_POS_Strain,
  DEFLINE_POS_Subclone,
  DEFLINE_POS_Subgroup,
  DEFLINE_POS_Sub_species,
  DEFLINE_POS_Substrain,
  DEFLINE_POS_Subtype,
  DEFLINE_POS_Synonym,
  DEFLINE_POS_Teleomorph,
  DEFLINE_POS_Tissue_lib,
  DEFLINE_POS_Tissue_type,
  DEFLINE_POS_Transgenic,
  DEFLINE_POS_Transposon_name,
  DEFLINE_POS_Type,
  DEFLINE_POS_Variety
} DefLinePos;

#define NumDefLineModifiers (sizeof (DefLineModifiers) / sizeof (ModifierItemGlobalData))

typedef enum {
  clone_isolate_HIV_rule_prefer_clone = 1,
  clone_isolate_HIV_rule_prefer_isolate,
  clone_isolate_HIV_rule_want_both
} clone_isolate_HIV_rule_values;
  
/*
 * The CountModifiersProc is used as the callback function for
 * VisitBioSourcesInSep when we are getting a list of all the modifiers
 * that appear in the sources.  We also obtain, for each modifier class,
 * the first value seen, whether or not each value seen is unique for
 * for the modifier, and whether or not the modifier is present for all
 * sources.
 */
static void CountModifiersProc (
  BioSourcePtr biop,
  Pointer userdata
)
{
  ModifierItemLocalPtr ItemList;
  OrgModPtr     mod;
  SubSourcePtr  ssp;
  Int2 i;
  Boolean       found_this_modifier;

  if (biop == NULL) return;
  ItemList = (ModifierItemLocalPtr) userdata;

  for (i=0; i < NumDefLineModifiers; i++)
  {
    found_this_modifier = FALSE;
    if (DefLineModifiers[i].isOrgMod)
    {
      if ( biop->org == NULL || biop->org->orgname == NULL) continue;
      mod = biop->org->orgname->mod;
      while (mod != NULL
        && mod->subtype != DefLineModifiers[i].subtype)
      {
        mod = mod->next;
      }
      if (mod != NULL && mod->subname != NULL)
      {
        found_this_modifier = TRUE;
        if (ItemList[i].first_value_seen != NULL)
        {
          if (StringCmp (ItemList[i].first_value_seen, mod->subname) != 0)
          {
            ItemList[i].is_unique = FALSE;
          }
        }
        else
        {
          ItemList[i].first_value_seen = mod->subname;
        }
        if ( FindExactStringInStrings (ItemList[i].values_seen, mod->subname)
          == NULL)
        {
          ValNodeAddStr (&ItemList[i].values_seen, 0, mod->subname);
        }
        else
        {
          ItemList[i].all_unique = FALSE;
        }
      }
    } else {
      ssp = biop->subtype;
      while (ssp != NULL && ssp->subtype != DefLineModifiers[i].subtype)
      {
        ssp = ssp->next;
      }
      if (ssp != NULL && ssp->name != NULL)
      {
        found_this_modifier = TRUE;
        if (ItemList[i].first_value_seen != NULL)
        {
          if (StringCmp (ItemList[i].first_value_seen, ssp->name) != 0)
          {
            ItemList[i].is_unique = FALSE;
          }
        }
        else
        {
          ItemList[i].first_value_seen = ssp->name;
        }
        if ( FindExactStringInStrings (ItemList[i].values_seen, ssp->name)
          == NULL)
        {
          ValNodeAddStr (&ItemList[i].values_seen, 0, ssp->name);
        }
        else
        {
          ItemList[i].all_unique = FALSE;
        }
      }
    }
    if (found_this_modifier)
    {
      ItemList[i].any_present = TRUE;
    } else {
      ItemList[i].all_present = FALSE;
    }
  }
} 

/* The CountModifiers function visits all of the bio sources, determining
 * which modifiers are present, which modifiers have only one value,
 * which modifiers have all different values, and which modifiers are
 * present in all sources.
 * After this survey is complete, the function prepares a short summary
 * of the above information for each modifier, which is used in the 
 * definition line options dialog.
 */ 
static void CountModifiers (
  ModifierItemLocalPtr ItemList,
  SeqEntryPtr sep
)
{
  Int2 i;

  for (i=0; i < NumDefLineModifiers; i++)
  {
    ItemList[i].all_present = TRUE;
    ItemList[i].is_unique = TRUE;
    ItemList[i].first_value_seen = NULL;
    ItemList[i].values_seen = NULL;
    ItemList[i].all_unique = TRUE;
  }

  VisitBioSourcesInSep (sep, ItemList, CountModifiersProc);
 
  for (i=0; i < NumDefLineModifiers; i++)
  {
    if (ItemList[i].all_present && ItemList[i].all_unique)
    {
      ItemList[i].status = "All present, all unique values";
    }
    else if (ItemList[i].all_present && ItemList[i].is_unique)
    {
      ItemList[i].status = "All present, one unique value";
    }
    else if (ItemList[i].all_present && ! ItemList[i].is_unique)
    {
      ItemList[i].status = "All present, mixed values";
    }
    else if (! ItemList[i].all_present && ItemList[i].all_unique)
    {
      ItemList[i].status = "Some missing, all unique values";
    }
    else if (! ItemList[i].all_present && ItemList[i].is_unique)
    {
      ItemList[i].status = "Some missing, one unique value present";
    }
    else if (! ItemList[i].all_present && ! ItemList[i].is_unique)
    {
      ItemList[i].status = "Some missing, mixed values";
    }
  }
}

/* The BioSrcDescData structure is used to hold a BioSourcePtr, a list
 * of strings used to describe the biosource, including the taxonomy name
 * and the values of all of the modifiers selected so far for this bio
 * source, and a pointer to the next BioSrcDescData structure in the list.
 */

/* The CopyBioSrcDescPtr function creates a copy of the linked list of
 * BioSrcDescData structures.
 */
static BioSrcDescPtr CopyBioSrcDescPtr (
  BioSrcDescPtr orig
)
{
  BioSrcDescPtr new_bsdp_start;

  if (orig == NULL) return NULL;

  new_bsdp_start = (BioSrcDescPtr) MemNew (sizeof (BioSrcDescData));
  if (new_bsdp_start == NULL) return NULL;

  new_bsdp_start->biop = orig->biop;
  new_bsdp_start->strings = CopyStrings (orig->strings);
  new_bsdp_start->next = CopyBioSrcDescPtr (orig->next);
  return new_bsdp_start;
}

/* The FreeBioSrcDescPtr function frees the memory associated with a
 * linked list of BioSrcDescData structures.
 */
static void FreeBioSrcDescPtr (
  BioSrcDescPtr bsdp
)
{
  if (bsdp == NULL) return;
  FreeBioSrcDescPtr (bsdp->next);
  bsdp->biop = NULL;
  ValNodeFreeData (bsdp->strings);
  MemFree (bsdp);
}

/* The AddQualToBioSrcDescPtr function finds the qualifier at the
 * feature_index position in the DefLineModifiers array in the
 * BioSourcePtr and adds the value for that modifier to the array
 * of strings describing the bio source.
 */
static void AddQualToBioSrcDescPtr (
  BioSrcDescPtr bsdp,
  ModifierItemLocalPtr qual,
  Int2 feature_index
)
{
  OrgModPtr          mod;
  SubSourcePtr       ssp;

  if (bsdp == NULL) return;
  if (bsdp->biop == NULL) return;

  if (DefLineModifiers[feature_index].isOrgMod)
  {
    if (bsdp->biop->org == NULL || bsdp->biop->org->orgname == NULL) return;
    mod = bsdp->biop->org->orgname->mod;
    while (mod != NULL
        && mod->subtype != DefLineModifiers[feature_index].subtype)
    {
      mod = mod->next;
    }
    if (mod != NULL && mod->subname != NULL)
    {
      ValNodeCopyStr( &(bsdp->strings), 0, mod->subname);
    }
  } else {
    ssp = bsdp->biop->subtype;
    while (ssp != NULL
        && ssp->subtype != DefLineModifiers[feature_index].subtype)
    {
      ssp = ssp->next;
    }
    if (ssp != NULL)
    {
      if (ssp->subtype == SUBSRC_transgenic)
      {
        ValNodeCopyStr( &(bsdp->strings), 0, "transgenic");
      }
      else if (ssp->name != NULL)
      {
        ValNodeCopyStr( &(bsdp->strings), 0, ssp->name);
      }
    }
  }
}
 
/* The CompareOrganismDescriptors function compares the contents of the
 * lists of strings for each BioSrcDesc item.
 * The function returns:
 *     -1 if org1 < org2
 *      0 if org1 = org2
 *      1 if org1 > org2
 */
static int CompareOrganismDescriptors (
  BioSrcDescPtr org1,
  BioSrcDescPtr org2
)
{
  ValNodePtr vnp1, vnp2;
  int cmpval;

  vnp1 = org1->strings;
  vnp2 = org2->strings;

  while (vnp1 != NULL && vnp2 != NULL)
  {
    cmpval = StringCmp (vnp1->data.ptrvalue, vnp2->data.ptrvalue);
    if (cmpval != 0) return cmpval;

    vnp1 = vnp1->next;
    vnp2 = vnp2->next;
  }
  if (vnp1 == NULL && vnp2 == NULL)
  {
    return 0;
  }
  else if (vnp1 != NULL && vnp2 == NULL)
  {
    return 1;
  }
  else
  {
    return -1;
  }
}

/* The OrgGroupData structure contains a list of BioSrcDescData items
 * for which the contents of the descriptive strings list are identical,
 * i.e., all the organisms in the group would have the same description
 * if you used the modifiers used to generate this list of strings.
 * The structure also contains the number of organisms in the list
 * so that it will be easy to tell that the OrgGroup now contains a
 * single organism with a unique description.
 */

/* The CopyOrgGroupList function creates a copy of the list of OrgGroups */
static OrgGroupPtr CopyOrgGroupList (
  OrgGroupPtr orig
)
{
  OrgGroupPtr new_ogp_start;

  if (orig == NULL) return NULL;

  new_ogp_start = (OrgGroupPtr) MemNew (sizeof (OrgGroupData));
  if (new_ogp_start == NULL) return NULL;

  new_ogp_start->num_organisms = orig->num_organisms;
  new_ogp_start->org_list = CopyBioSrcDescPtr (orig->org_list);
  new_ogp_start->next = CopyOrgGroupList (orig->next);
  return new_ogp_start;
}

/* The FreeOrgGroupPtr function frees the memory associated with a
 * list of OrgGroups */
static void FreeOrgGroupPtr (
  OrgGroupPtr ogp
)
{
  if (ogp == NULL) return;
  FreeOrgGroupPtr (ogp->next);
  FreeBioSrcDescPtr (ogp->org_list);
  MemFree (ogp);
  return;
}

/* The ReorderGroupOrgs function sorts the OrgGroup list based on the results
 * of the CompareOrganismDescriptors function.
 */
static void ReorderGroupOrgs (
  OrgGroupPtr this_group
)
{
  BioSrcDescPtr bsdp;
  BioSrcDescPtr nextBsdp;
  BioSrcDescPtr prevBsdp;
  Boolean swap_needed = TRUE;

  if (this_group->org_list == NULL) return;
  if (this_group->org_list->next == NULL) return;

  while (swap_needed)
  {
    swap_needed = FALSE;
    bsdp = this_group->org_list;
    prevBsdp = NULL;
    while (bsdp->next != NULL)
    {
      nextBsdp = bsdp->next;
      if (CompareOrganismDescriptors (bsdp, nextBsdp) > 0)
      {
        swap_needed = TRUE;
        bsdp->next = nextBsdp->next;
        nextBsdp->next = bsdp;
        if (prevBsdp == NULL)
        {
          this_group->org_list = nextBsdp;
        }
        else
        {
          prevBsdp->next = nextBsdp;
        }
        prevBsdp = nextBsdp;
      }
      else
      {
        prevBsdp = bsdp;
        bsdp = bsdp->next;
      }
    }
  }
}

/* The ReGroupOrgs function operates on a single OrgGroup item.
 * If any of the BioSrcDesc items in the group now have different
 * descriptions, the function breaks it up into smaller, homogenous OrgGroups.
 */
static void ReGroupOrgs (
  OrgGroupPtr this_group
)
{
  BioSrcDescPtr bsdp;
  OrgGroupPtr new_group;
  int num_organisms;

  if (this_group == NULL) return;
  bsdp = this_group->org_list;
  if (bsdp == NULL) return;
  num_organisms = 0;
  while (bsdp->next != NULL)
  {
    num_organisms ++;
    if (CompareOrganismDescriptors (bsdp, bsdp->next) != 0)
    {
      /* create new group to hold next set of organisms */
      new_group = (OrgGroupPtr) MemNew (sizeof (OrgGroupData));
      if (new_group == NULL) return;
      new_group->org_list = bsdp->next;
      new_group->num_organisms = this_group->num_organisms - num_organisms;
      new_group->next = this_group->next;
      this_group->next = new_group;
      this_group->num_organisms = num_organisms;
      bsdp->next = NULL;
      ReGroupOrgs (new_group);
    }
    else
    {
      bsdp = bsdp->next;
    }
  }
}

/* The AddQualToGroup function operates on a single OrgGroup item.
 * The function adds a qualifier to each BioSrcDesc item in the OrgGroup,
 * breaks the group into multiple groups if the group is no longer
 * homogenous, and sorts the new list.
 */
static void AddQualToGroup (
  OrgGroupPtr this_group,
  ModifierItemLocalPtr qual,
  Int2 feature_index
)
{
  BioSrcDescPtr bsdp;

  if (this_group == NULL) return;

  bsdp = this_group->org_list;
  while (bsdp != NULL)
  {
    AddQualToBioSrcDescPtr (bsdp, qual, feature_index);
    bsdp= bsdp->next;
  }

  /* now reorder organisms and break up group */
  ReorderGroupOrgs (this_group);

  ReGroupOrgs (this_group);
}

/* The AddQualToGroupList function operates on a list of OrgGroup items.
 * It calls AddQualToGroup for each item in the list.
 */
static void AddQualToGroupList (
  OrgGroupPtr group_list,
  ModifierItemLocalPtr qual,
  Int2 feature_index
)
{
  OrgGroupPtr ogp;

  ogp = group_list;
  while (ogp != NULL)
  {
    AddQualToGroup (ogp, qual, feature_index);
    ogp = ogp->next;
  }
}

/* The CopyModifierIndices function creates a new ValNode list with the
 * same data.intvalue values for each node as the original modifier_indices
 * ValNode list.
 */
static ValNodePtr CopyModifierIndices (
  ValNodePtr modifier_indices
)
{
  ValNodePtr new_indices;

  if (modifier_indices == NULL) return NULL;
  new_indices = ValNodeNew (NULL);
  if (new_indices == NULL) return NULL;
  new_indices->choice = modifier_indices->choice;
  new_indices->data.intvalue = modifier_indices->data.intvalue;
  new_indices->next = CopyModifierIndices (modifier_indices->next);
  return new_indices;
}
 
/* The CopyModifierCombo creates a copy of a ModificationCombination item.
 * This includes creating a copy of the number and list of modifiers
 * and a copy of the number and list of OrgGroups, as well as copying the
 * maximum number of organisms in any one group and the number of unique
 * organism descriptions produced by this combination of modifiers.
 */
static ModifierCombinationPtr CopyModifierCombo (
  ModifierCombinationPtr m
)
{
  ModifierCombinationPtr newm;
  ValNodePtr  vnp;
  ValNodePtr  newval;

  newm = (ModifierCombinationPtr) MemNew (sizeof (ModifierCombinationData));
  if (newm == NULL) return NULL;

  newm->next = NULL;

  /* copy list of modifier indices */
  newm->num_mods = m->num_mods;
  newm->modifier_indices = NULL;
  vnp = m->modifier_indices;
  if (vnp != NULL)
  {
    newm->modifier_indices = ValNodeNew (NULL);
    if (newm->modifier_indices == NULL) return NULL;
    newm->modifier_indices->data.intvalue = vnp->data.intvalue;
    vnp = vnp->next;
    while (vnp != NULL)
    {
      newval = ValNodeNew (newm->modifier_indices);
      if (newval == NULL) return NULL;
      newval->data.intvalue = vnp->data.intvalue;
      vnp = vnp->next;
    }
  }
  
  /* copy groups */
  newm->num_groups = m->num_groups;
  newm->group_list = CopyOrgGroupList (m->group_list);
 
  return newm; 
}

/* This function creates a new ModifierCombination item using the supplied
 * OrgGroup list.  It calculates the number of groups, maximum number of 
 * organisms in any one group, and number of unique organisms.
 * Initially there are no modifiers.
 */
static ModifierCombinationPtr NewModifierCombo (
  OrgGroupPtr group_list
)
{
  ModifierCombinationPtr newm;
  OrgGroupPtr  ogp;

  newm = (ModifierCombinationPtr) MemNew (sizeof (ModifierCombinationData));
  if (newm == NULL) return NULL;

  newm->num_mods = 0;
  newm->modifier_indices = NULL;
  newm->num_unique_orgs = 0;
  
  /* copy groups */
  newm->group_list = CopyOrgGroupList (group_list);
 
  ogp = newm->group_list;
  newm->max_orgs_in_group = 0;
  newm->num_groups = 0;
  while (ogp != NULL)
  {
    if (newm->max_orgs_in_group < ogp->num_organisms)
      newm->max_orgs_in_group = ogp->num_organisms;
    if (ogp->num_organisms == 1)
      newm->num_unique_orgs ++;
    newm->num_groups ++;
    ogp = ogp->next;
  }

  newm->next = NULL;
  return newm; 
}

/* This function frees the memory associated with a list of
 * ModifierCombination items.
 */
static void FreeModifierCombo (
  ModifierCombinationPtr m
)
{
  if (m == NULL) return;
  FreeModifierCombo (m->next);
  ValNodeFree (m->modifier_indices);
  FreeOrgGroupPtr (m->group_list);
  MemFree (m);
}

/* This function adds the qualifier at the feature_index position in the
 * DefLineModifiers array to each OrgGroup in the list and recalculates
 * the maximum number of organisms in any one group and the number of
 * unique organism descriptions generated by this new combination of 
 * modifiers.
 */
static void AddQualToModifierCombo (
  ModifierCombinationPtr m,
  ModifierItemLocalPtr qual,
  Int2 feature_index
)
{
  OrgGroupPtr ogp;
  ValNodePtr vnp;

  if (m == NULL) return;

  /* now try adding the modifier, see if the number of groups goes up */
  /* if the number of organisms in each group is one, we can stop */
  vnp = ValNodeNew (m->modifier_indices);
  if (vnp == NULL) return;
  if (m->modifier_indices == NULL)
  {
    m->modifier_indices = vnp;
  }
  vnp->data.intvalue = feature_index;
  m->num_mods ++;
  AddQualToGroupList (m->group_list, qual, feature_index);
  ogp = m->group_list;
  m->max_orgs_in_group = 0;
  m->num_unique_orgs = 0;
  m->num_groups = 0;
  while (ogp != NULL)
  {
    if (m->max_orgs_in_group < ogp->num_organisms)
      m->max_orgs_in_group = ogp->num_organisms;
    if (ogp->num_organisms == 1)
      m->num_unique_orgs ++;
    m->num_groups ++;
    ogp = ogp->next;
  }
}

/* This function creates the initial OrgGroup list that is copied for every
 * ModifierCombination item.
 */
static void BuildTaxOrgGroupList (
  BioSourcePtr biop,
  Pointer userdata
)
{
  OrgGroupPtr   ogp;
  OrgGroupPtr   prevOgp;
  OrgGroupPtr PNTR pogp;
  BioSrcDescPtr newBsdp;
  OrgRefPtr     orp;
  int cmpval;

  pogp = (OrgGroupPtr PNTR) userdata;
  ogp = *pogp;

  newBsdp = (BioSrcDescPtr) MemNew (sizeof (BioSrcDescData));
  if (newBsdp == NULL) return;
  newBsdp->biop = biop;
  newBsdp->next = NULL;
  newBsdp->strings = NULL;

  /* add tax name as first string */
  /* later, move this into a separate function and add special handling */
  orp = biop->org;
  if (orp != NULL && orp->taxname != NULL)
  {
    ValNodeCopyStr (&(newBsdp->strings), 0, orp->taxname);
  }

  prevOgp = NULL;
  cmpval = -1;
  while (ogp != NULL && cmpval < 0)
  {
    if (ogp->org_list != NULL)
    {
      cmpval = CompareOrganismDescriptors (ogp->org_list, newBsdp);
      if (cmpval == 0)
      {
        newBsdp->next = ogp->org_list;
        ogp->org_list = newBsdp;
        ogp->num_organisms ++;
      }
    }
    if (cmpval < 0)
    {
      prevOgp = ogp;
      ogp = ogp->next;
    }
  }
  if (cmpval != 0)
  {
    /* create new group */
    ogp = (OrgGroupPtr) MemNew (sizeof (OrgGroupData));
    if (ogp == NULL) return;
    ogp->org_list = newBsdp;
    ogp->num_organisms = 1;
    ogp->next = NULL;
    if (prevOgp == NULL)
    {
      ogp->next = *pogp;
      *pogp = ogp;
    }
    else
    {
      ogp->next = prevOgp->next;
      prevOgp->next = ogp;
    }
  }
}

typedef struct bestsortdata {
  Int4    feature_index;
  Boolean all_unique;
  Boolean all_present;
  Boolean is_unique;
} BestSortData, PNTR BestSortPtr;

static Boolean Index1FoundBeforeIndex2 (
  Int4 index1,
  Int4 index2,
  ValNodePtr list
)
{
  ValNodePtr  vnp;
  BestSortPtr bsp;
  for (vnp = list; vnp != NULL; vnp = vnp->next)
  {
    if ((bsp = vnp->data.ptrvalue) == NULL)
    {
      continue;
    }
    if (bsp->feature_index == index1) return TRUE;
    if (bsp->feature_index == index2) return FALSE;
  }
  return FALSE;
}

/* This function determines whether or not we should try adding this modifier
 * to our combination.  If we've already tried it and not added it to the list,
 * there's no reason to try adding it again.
 */
static Boolean OkToTryAddingQual (
  ModifierCombinationPtr m,
  ModifierItemLocalPtr ItemList,
  ValNodePtr           available_modifiers_list,
  Int2 feature_index
)
{
  ValNodePtr vnp;

  /* if feature_index indicates a value we don't use for best combos, skip */
  if (feature_index == DEFLINE_POS_Old_name
    || feature_index == DEFLINE_POS_Old_lineage
    || feature_index == DEFLINE_POS_Dosage
    || feature_index == DEFLINE_POS_Map)
  {
    return FALSE;
  }

  if (m == NULL) return TRUE;

  /* if feature_index is lower than anything else on list (other than */
  /* a required value, this is a repeat combination, so skip it */
  vnp = m->modifier_indices;
  while (vnp != NULL)
  {
    if (feature_index == m->modifier_indices->data.intvalue)
      return FALSE;
    if (! ItemList[m->modifier_indices->data.intvalue].required &&
      Index1FoundBeforeIndex2 (feature_index,
                               m->modifier_indices->data.intvalue, 
                               available_modifiers_list))
    {
      return FALSE;
    }
    vnp = vnp->next;
  }
  return TRUE;
}

static ValNodePtr GetListOfAvailableModifiers ( ModifierItemLocalPtr ItemList)
{
  ValNodePtr  vnp, head;
  Int2        feature_index;
  BestSortPtr bsp;

  head = NULL;
  for (feature_index = 0; feature_index < NumDefLineModifiers; feature_index++)
  {
    if ( ItemList[feature_index].any_present)
    {
      bsp = (BestSortPtr) MemNew (sizeof (BestSortData));
      if (bsp == NULL) return NULL;
      bsp->feature_index = feature_index;
      bsp->all_unique = ItemList[feature_index].all_unique;
      bsp->all_present = ItemList[feature_index].all_present;
      bsp->is_unique = ItemList[feature_index].is_unique;
      vnp = ValNodeNew (head);
      if (vnp == NULL) return NULL;
      vnp->data.ptrvalue = bsp;
      if (head == NULL) head = vnp;
    }
  }
  return head;
}

static Int4 DefLineQualSortOrder [] = {
  DEFLINE_POS_Transgenic,
  DEFLINE_POS_Plasmid_name,
  DEFLINE_POS_Endogenous_virus_name,
  DEFLINE_POS_Strain,
  DEFLINE_POS_Clone,
  DEFLINE_POS_Isolate,
  DEFLINE_POS_Haplotype,
  DEFLINE_POS_Cultivar,
  DEFLINE_POS_Specimen_voucher,
  DEFLINE_POS_Ecotype,
  DEFLINE_POS_Type,
  DEFLINE_POS_Serotype,
  DEFLINE_POS_Authority,
  DEFLINE_POS_Breed
};

static int LIBCALLBACK SortByImportanceAndPresence (
  VoidPtr ptr1,
  VoidPtr ptr2
)
{
  ValNodePtr  vnp1;
  ValNodePtr  vnp2;
  BestSortPtr bsp1, bsp2; 
  Int4       num_defline_qual_sort_order, index;

  if (ptr1 == NULL && ptr2 == NULL) return 0;
  
  if (ptr1 == NULL && ptr2 != NULL) return -1;
  if (ptr1 != NULL && ptr2 == NULL) return 1;
 
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  if (vnp1->data.ptrvalue == NULL || vnp2->data.ptrvalue == NULL) return 0;

  bsp1 = vnp1->data.ptrvalue;
  bsp2 = vnp2->data.ptrvalue;
  if (bsp1->feature_index == bsp2->feature_index) return 0;

  if (bsp1->all_present && bsp1->all_unique
    && (! bsp2->all_present || ! bsp2->all_unique))
  {
    return -1;
  }
  if (bsp2->all_present && bsp2->all_unique
    && (! bsp1->all_present || ! bsp1->all_unique))
  {
    return 1;
  }

  if ( ! bsp1->is_unique && bsp2->is_unique) return -1;
  if ( ! bsp2->is_unique && bsp1->is_unique) return 1;

  num_defline_qual_sort_order = sizeof (DefLineQualSortOrder) / sizeof (Int4);
  for (index = 0; index < num_defline_qual_sort_order; index++)
  {
    if (bsp1->feature_index == DefLineQualSortOrder [ index ]) return -1;
    if (bsp2->feature_index == DefLineQualSortOrder [ index ]) return 1;
  }

  if (bsp1->feature_index > bsp2->feature_index) return 1;
  if (bsp1->feature_index < bsp2->feature_index) return -1;
  return 0;
}


/* The function FindBestCombo tries to find the best combination of modifiers
 * to create unique organism descriptions.  This is accomplished by
 * creating a list of required modifiers, and then creating a list of
 * combinations of modifiers by adding modifiers one at a time
 * to see if the additional modifiers provide any more differentiation in
 * the list.
 * In order to do this, I start with a list of required modifiers, and 
 * then create copies of this list.  For each copy I add one of the modifiers
 * that are present in the bio sources and not already on the list.
 * If adding the modifier increases the differentiation, I add that copy to
 * the list of possible combinations, otherwise I discard it.
 * The function then makes copies of all of the new items added to the list,
 * starting with the item pointed to by start_of_expand, and adds another
 * modifier to each combination, keeping the combinations that increase
 * the differentiation and discarding the rest.
 * This process continues until I have a combination that produces completely
 * differentiated bio sources, or I run out of possible combinations.
 * If the list of possible combinations is exhausted before each organism
 * has a unique description, the function selects the combination from the
 * list with the largest number of unique organism descriptions.  If more 
 * than one combination produces the largest number of unique organisms,
 * the combination with the largest number of unique organisms and the
 * largest number of groups will be selected.
 */
static ModifierCombinationPtr FindBestCombo(
  SeqEntryPtr sep,
  ModifierItemLocalPtr ItemList
)
{
  OrgGroupPtr group_list;
  ModifierCombinationPtr mc_list, start_of_expand, best_found, end_of_list;
  ModifierCombinationPtr next_start_of_expand, m, newm;
  Int4 num_to_expand, next_num_to_expand;
  Int2 i;
  ValNodePtr available_modifier_list, vnp;
  BestSortPtr bsp;

  best_found = NULL;

  /* first, get list of organisms */
  group_list = NULL;
  VisitBioSourcesInSep (sep, &group_list, BuildTaxOrgGroupList);

  /* create combo with just the org groups */
  mc_list = NewModifierCombo (group_list);
  if (mc_list == NULL) return NULL;

  available_modifier_list = GetListOfAvailableModifiers (ItemList);

  /* next, add in any required qualifiers */
  for (vnp = available_modifier_list; vnp != NULL; vnp = vnp->next)
  {
    bsp = vnp->data.ptrvalue;
    if (bsp == NULL) return NULL;
    if (ItemList[bsp->feature_index].required)
    {
      AddQualToModifierCombo (mc_list, ItemList + bsp->feature_index,
                                       bsp->feature_index);
    }
  }
  if (mc_list->max_orgs_in_group == 1)
  {
    /* we're done - they're all unique */
    best_found = mc_list;
    return best_found;
  }

  available_modifier_list = SortValNode (available_modifier_list,
                                         SortByImportanceAndPresence);
  start_of_expand = mc_list;
  end_of_list = mc_list;
  num_to_expand = 1;
  while (best_found == NULL && start_of_expand != NULL)
  {
    next_num_to_expand = 0;
    next_start_of_expand = NULL;
    for (i=0; i < num_to_expand && start_of_expand != NULL; i++)
    {
      /* try adding qualifiers */ 
      for (vnp = available_modifier_list;
           vnp != NULL && best_found == NULL;
           vnp = vnp->next)
      {
        bsp = vnp->data.ptrvalue;
        if (bsp == NULL) return NULL;
        if (OkToTryAddingQual (start_of_expand, ItemList,
                               available_modifier_list,
                               bsp->feature_index))
        {
          newm = CopyModifierCombo (start_of_expand);
          AddQualToModifierCombo (newm, ItemList + bsp->feature_index,
                                  bsp->feature_index);
          if (start_of_expand->num_groups >= newm->num_groups)
          {
            /* situation didn't get better, don't bother to add this one */
            FreeModifierCombo (newm);
            newm = NULL;
          }
          else if (newm->max_orgs_in_group == 1)
          {
            best_found = newm;
          }
          else
          {
            end_of_list->next = newm;
            end_of_list = end_of_list->next;
            if (next_start_of_expand == NULL)
              next_start_of_expand = newm;
            next_num_to_expand++;
          }
        }
      }
      if (start_of_expand != NULL)
      {
        start_of_expand = start_of_expand->next;
      }
    }
    num_to_expand = next_num_to_expand;
    if (start_of_expand != NULL)
    {
      start_of_expand = start_of_expand->next;
    }
  }

  if (best_found != NULL)
  {
    FreeModifierCombo (mc_list);
    return best_found;
  }
  
  /* we want to find the one with the highest number of unique organisms */
  best_found = mc_list;
  m = mc_list->next;
  while (m!= NULL)
  {
    if (m->num_unique_orgs > best_found->num_unique_orgs)
    {
      best_found = m;
    }
    else if (m->num_unique_orgs == best_found->num_unique_orgs
           && m->num_groups > best_found->num_groups)
    {
      best_found = m;
    }
    else if (m->num_unique_orgs == best_found->num_unique_orgs
           && m->num_groups == best_found->num_groups
           && m->num_mods < best_found->num_groups)
    {
      best_found = m;
    }
    m = m->next;
  }

  m = mc_list;
  while (m != NULL)
  {
    if (m != best_found)
    {
      newm = m->next;
      m->next = NULL;
      FreeModifierCombo (m);
      m = newm;
    }
    else
    {
      FreeModifierCombo (m->next);
      m->next = NULL;
      m = NULL;
    }
  }
  return best_found;
}

/* create combo with the specified modifiers */
static ModifierCombinationPtr CreateComboFromModList (
  ModifierItemLocalPtr modList
)
{
  ModifierCombinationPtr m;
  Int4                   feature_index;

  m = NewModifierCombo (NULL);
  if (m == NULL) return NULL;
  for (feature_index = 0; feature_index < NumDefLineModifiers; feature_index++)
  {
    if (modList[feature_index].any_present && modList [feature_index].required)
    {
      AddQualToModifierCombo (m, modList + feature_index, feature_index);
    }
  }
  return m;
}


/* This is the callback function for sorting the modifier list.  It 
 * implements an order specified by the indexers.
 */
static Int4 DefLineQualPresentationOrder [] = {
  DEFLINE_POS_Transgenic,
  DEFLINE_POS_Strain,
  DEFLINE_POS_Isolate,
  DEFLINE_POS_Cultivar,
  DEFLINE_POS_Specimen_voucher,
  DEFLINE_POS_Ecotype,
  DEFLINE_POS_Type,
  DEFLINE_POS_Serotype,
  DEFLINE_POS_Authority,
  DEFLINE_POS_Breed
};

static int LIBCALLBACK SortByImportance (
  VoidPtr ptr1,
  VoidPtr ptr2
)
{
  ValNodePtr vnp1;
  ValNodePtr vnp2;
  Int4       num_defline_qual_sort_order, index;

  if (ptr1 == NULL && ptr2 == NULL) return 0;
  
  if (ptr1 == NULL && ptr2 != NULL) return -1;
  if (ptr1 != NULL && ptr2 == NULL) return 1;
 
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);
  if (vnp1 == NULL || vnp2 == NULL) return 0;
  if (vnp1->data.intvalue == vnp2->data.intvalue) return 0;

  num_defline_qual_sort_order = sizeof (DefLineQualPresentationOrder) / sizeof (Int4);
  for (index = 0; index < num_defline_qual_sort_order; index++)
  {
    if (vnp1->data.intvalue == DefLineQualPresentationOrder [ index ]) return -1;
    if (vnp2->data.intvalue == DefLineQualPresentationOrder [ index ]) return 1;
  }

  if ((vnp1->data.intvalue < 0 || vnp1->data.intvalue > NumDefLineModifiers)
    && (vnp2->data.intvalue < 0 || vnp2->data.intvalue > NumDefLineModifiers))
  {
    return 0;
  }
  if (vnp1->data.intvalue < 0 || vnp1->data.intvalue > NumDefLineModifiers)
  {
    return 1;
  }
  if (vnp2->data.intvalue < 0 || vnp2->data.intvalue > NumDefLineModifiers)
  {
    return -1;
  }

  if (DefLineModifiers [ vnp1->data.intvalue].isOrgMod
    && (! DefLineModifiers [ vnp2->data.intvalue].isOrgMod
      || vnp2->data.intvalue == DEFLINE_POS_Plasmid_name
      || vnp2->data.intvalue == DEFLINE_POS_Endogenous_virus_name))
  {
    return -1;
  }
  if (DefLineModifiers [ vnp2->data.intvalue].isOrgMod
    && (! DefLineModifiers [ vnp1->data.intvalue].isOrgMod
      || vnp1->data.intvalue == DEFLINE_POS_Plasmid_name
      || vnp1->data.intvalue == DEFLINE_POS_Endogenous_virus_name))
  {
    return 1;
  }
  
  if (vnp1->data.intvalue == DEFLINE_POS_Plasmid_name)
  {
    return -1;
  }
  if (vnp2->data.intvalue == DEFLINE_POS_Plasmid_name)
  {
    return 1;
  }
    
  if (vnp1->data.intvalue == DEFLINE_POS_Endogenous_virus_name)
  {
    return -1;
  }
  if (vnp2->data.intvalue == DEFLINE_POS_Endogenous_virus_name)
  {
    return 1;
  }

  if (! DefLineModifiers [ vnp1->data.intvalue].isOrgMod
     && vnp2->data.intvalue == DEFLINE_POS_Clone)
  {
    return 1;
  }
  if (! DefLineModifiers [ vnp2->data.intvalue].isOrgMod
     && vnp1->data.intvalue == DEFLINE_POS_Clone)
  {
    return -1;
  }

  if (! DefLineModifiers [ vnp1->data.intvalue].isOrgMod
     && vnp2->data.intvalue == DEFLINE_POS_Haplotype)
  {
    return 1;
  }
  if (! DefLineModifiers [ vnp2->data.intvalue].isOrgMod
     && vnp1->data.intvalue == DEFLINE_POS_Haplotype)
  {
    return -1;
  }

  if (vnp1->data.intvalue > vnp2->data.intvalue) return 1;
  if (vnp1->data.intvalue < vnp2->data.intvalue) return -1;
  return 0;
}

static Boolean RecordHasModifier (
  BioSourcePtr biop,
  Int4         modifier_index
)
{
  OrgModPtr     mod;
  OrgNamePtr    onp;
  SubSourcePtr  ssp;

  if (biop == NULL
    || modifier_index < 0
    || modifier_index >= NumDefLineModifiers)
  {
    return FALSE;
  }
  if (DefLineModifiers[modifier_index].isOrgMod)
  {
    if (biop->org == NULL || (onp = biop->org->orgname) == NULL)
    {
      return FALSE;
    }
    mod = onp->mod;
    while (mod != NULL
        && mod->subtype != DefLineModifiers[modifier_index].subtype)
    {
      mod = mod->next;
    }
    if (mod != NULL && mod->subname != NULL)
    {
      return TRUE;
    }
  } else {
    ssp = biop->subtype;
    while (ssp != NULL && ssp->subtype != DefLineModifiers[modifier_index].subtype)
    {
      ssp = ssp->next;
    }
    if (ssp != NULL && ssp->name != NULL)
    {
      return TRUE;
    }
  }
  return FALSE;
}
  
/* This function adds in required modifiers for HIV sequences */
static void AddHIVModifierIndices (
  ValNodePtr PNTR modifier_indices,
  BioSourcePtr biop,
  ModifierItemLocalPtr modList,
  CharPtr taxName,
  Int4    clone_isolate_HIV_rule_num
)
{
  ValNodePtr  vnp;
  Boolean have_country_in_list;
  Boolean have_isolate_in_list;
  Boolean have_clone_in_list;
  Boolean have_country_mod;
  Boolean have_isolate_mod;
  Boolean have_clone_mod;

  /* special handling for HIV */
  if (StringICmp (taxName, "HIV-1") != 0
    && StringICmp (taxName, "HIV-2") != 0)
  {
    return;
  }

  have_country_in_list = FALSE;
  have_isolate_in_list = FALSE;
  have_clone_in_list = FALSE;
  have_country_mod = RecordHasModifier (biop, DEFLINE_POS_Country);
  have_isolate_mod = RecordHasModifier (biop, DEFLINE_POS_Isolate);
  have_clone_mod = RecordHasModifier (biop, DEFLINE_POS_Clone);

  if (modifier_indices != NULL)
  {
    for (vnp = *modifier_indices;
         vnp != NULL
           && (! have_country_in_list
             || ! have_isolate_in_list
             || ! have_clone_in_list);
         vnp = vnp->next)
    {
      if (vnp->data.intvalue == DEFLINE_POS_Country)
      {
        have_country_in_list = TRUE;
      }
      else if (vnp->data.intvalue == DEFLINE_POS_Isolate)
      {
        have_isolate_in_list = TRUE;
      }
      else if (vnp->data.intvalue == DEFLINE_POS_Clone)
      {
        have_clone_in_list = TRUE;
      }
    }
  }

  if ( ! have_country_in_list && have_country_mod)
  {
    vnp = ValNodeNew (*modifier_indices);
    vnp->data.intvalue = DEFLINE_POS_Country;
    if (*modifier_indices == NULL) *modifier_indices = vnp;
  }

  if ( ! have_isolate_in_list
      && have_isolate_mod
      && ( clone_isolate_HIV_rule_num == clone_isolate_HIV_rule_prefer_isolate
        || clone_isolate_HIV_rule_num == clone_isolate_HIV_rule_want_both
        || ! have_clone_mod))
  {
    vnp = ValNodeNew (*modifier_indices);
    vnp->data.intvalue = DEFLINE_POS_Isolate;
    if (*modifier_indices == NULL) *modifier_indices = vnp;
  }
    
  if ( ! have_clone_in_list
      && have_clone_mod
      && ( clone_isolate_HIV_rule_num == clone_isolate_HIV_rule_prefer_clone
        || clone_isolate_HIV_rule_num == clone_isolate_HIV_rule_want_both
        || ! have_isolate_mod))
  {
    vnp = ValNodeNew (*modifier_indices);
    vnp->data.intvalue = DEFLINE_POS_Clone;
    if (*modifier_indices == NULL) *modifier_indices = vnp;
  }
}

/* This function provides a label to be used in the definition line for
 * each modifier that requires one.  Most modifiers use a label that is 
 * similar to the name of the modifier displayed in the definition line
 * options dialog.
 */
static void AddModifierLabel (
  Boolean use_labels,
  Int2 feature_index,
  CharPtr modifier_text
)
{
  CharPtr cp;
  if (feature_index == DEFLINE_POS_Endogenous_virus_name)
  {
    StringCpy (modifier_text, "endogenous virus");
  }
  else if (feature_index == DEFLINE_POS_Specimen_voucher)
  {
    if (use_labels)
    {
      StringCpy (modifier_text, "voucher");
    } 
    else
    {
      modifier_text [0] = 0;
    }
  }
  else if (use_labels || feature_index == DEFLINE_POS_Transgenic
    || feature_index == DEFLINE_POS_Plasmid_name)
  {
    StringCpy (modifier_text, DefLineModifiers[feature_index].name);
    modifier_text[0] = tolower(modifier_text[0]);
    cp = StringStr (modifier_text, "-name");
    if (cp != NULL) *cp = 0;
  }
  else
  {
    modifier_text[0] = 0;
  }
}

typedef struct orgmodabbrevdata {
  Int2    subtype;
  CharPtr abbrev;
} OrgModAbbrevData, PNTR OrgModAbbrevPtr;

static OrgModAbbrevData orgmod_abbrevs[] = {
  { ORGMOD_variety, "var." },
  { ORGMOD_forma, "f." },
  { ORGMOD_forma_specialis, "f. sp." },
  { ORGMOD_pathovar, "pv." }
};

/* The UseOrgModifier function looks for the values of certain kinds of 
 * modifiers in the taxonomy name, so that they will not be added to the
 * definition line as modifiers if they are already present in the
 * taxonomy name.
 */
static Boolean UseOrgModifier (
  OrgModPtr mod,
  CharPtr   taxName
)
{
  CharPtr value_found;
  Int4    value_len;
  Int4    num_abbrevs, i;
  CharPtr abbrev_start;
  Boolean other_abbrev_found;

  if (mod == NULL || mod->subname == NULL) return FALSE;

  num_abbrevs = sizeof (orgmod_abbrevs) / sizeof (OrgModAbbrevData);

  /* If selected modifiers already appear in the tax Name, */
  /* don't use them in the organism description again */
  if (mod->subtype == ORGMOD_strain
    || mod->subtype == ORGMOD_variety
    || mod->subtype == ORGMOD_sub_species
    || mod->subtype == ORGMOD_forma
    || mod->subtype == ORGMOD_forma_specialis
    || mod->subtype == ORGMOD_pathovar)
  {
    value_found = StringStr (taxName, mod->subname);
    value_len = StringLen (mod->subname);
    while (value_found != NULL)
    {
      if (value_found == taxName)
      {
        value_found = StringStr (value_found + 1, mod->subname);
        continue;
      }
      if (*(value_found - 1) != ' ' && *(value_found - 1) != '(')
      {
        value_found = StringStr (value_found + 1, mod->subname);
        continue;
      }
      if (*(value_found - 1) == ')' && *(value_found + value_len) != ')')
      {
        value_found = StringStr (value_found + 1, mod->subname);
        continue;
      }
      if (*(value_found + value_len) != ' ' && *(value_found + value_len) != 0)
      {
        value_found = StringStr (value_found + 1, mod->subname);
        continue;
      }
      other_abbrev_found = FALSE;
      for (i = 0; i < num_abbrevs; i++)
      {
        abbrev_start = value_found - StringLen (orgmod_abbrevs[i].abbrev) - 1;
        if (abbrev_start > taxName
          && StringNCmp (abbrev_start,
                         orgmod_abbrevs[i].abbrev,
                         StringLen (orgmod_abbrevs[i].abbrev)) == 0)
        {
          if (mod->subtype == orgmod_abbrevs[i].subtype)
          {
            return FALSE;
          }
          else
          {
            other_abbrev_found = TRUE;
          }
        }
      }
      if ( ! other_abbrev_found 
        && ( mod->subtype == ORGMOD_strain
          || mod->subtype == ORGMOD_sub_species))
      {
        return FALSE;
      }
      value_found = StringStr (value_found + 1, mod->subname);
    }
  }
  return TRUE;
}

/* The SetRequiredModifiers function copies the default required values from
 * the global DefLineModifiers array into the local list of modifier
 * information.
 */
static void SetRequiredModifiers (
  ModifierItemLocalPtr modList
)
{
  Int4  item_index;

  for (item_index = 0; item_index < NumDefLineModifiers; item_index++)
  {
    modList[item_index].required = DefLineModifiers[item_index].default_required;
  }
  
}

/* The AddBestComboModifiersToModList function sets the modifiers
 * selected by the FindBestCombo process as selected.
 */
static void AddBestComboModifiersToModList (
  ModifierCombinationPtr m,
  ModifierItemLocalPtr modList
)
{
  ValNodePtr vnp;

  for (vnp = m->modifier_indices; vnp != NULL; vnp = vnp->next)
  {
    modList[vnp->data.intvalue].required = TRUE;
  }
}

/* This function fixes HIV abbreviations, removes items in parentheses,
 * and trims spaces around the taxonomy name.
 */
static void CleanUpTaxName (
  CharPtr taxName,
  Boolean keep_in_paren
)
{
  CharPtr ptr;

  if (StringICmp (taxName, "Human immunodeficiency virus type 1") == 0
    || StringICmp (taxName, "Human immunodeficiency virus 1") == 0)
  {
    StringCpy (taxName, "HIV-1");
  }
  else if (StringICmp (taxName, "Human immunodeficiency virus type 2") == 0
    || StringICmp (taxName, "Human immunodeficiency virus 2") == 0)
  {
    StringCpy (taxName, "HIV-2");
  }
  else
  {
    if (! keep_in_paren)
    {
      ptr = StringStr (taxName, "(");
      if (ptr != NULL)
        *ptr = '\0';
    }
    TrimSpacesAroundString (taxName);
  }
}

/* This function gets the BioSource descriptor for the BioSeq. */
static BioSourcePtr GetBiopForBsp (
  BioseqPtr bsp
)
{
  SeqMgrDescContext  dcontext;
  SeqDescrPtr    sdp;
  BioSourcePtr    biop;

  if (bsp == NULL) return NULL;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_source, &dcontext);
  if (sdp != NULL) {
    biop = (BioSourcePtr) sdp->data.ptrvalue;
    return biop;
  }
  
  return NULL;
}

/* This function generates a string describing the organism based on the
 * modifiers selected and other organism description options.
 */
static CharPtr GetOrganismDescription (
  BioseqPtr bsp, 
  ModifierItemLocalPtr modList,
  ModifierCombinationPtr m,
  OrganismDescriptionModifiersPtr odmp
)
{
  Char    taxName [196];
  Char    modifier_text [256];
  ValNodePtr  strings;
  BioSourcePtr  biop;
  OrgModPtr  mod;
  SubSourcePtr  ssp;
  ValNodePtr  modifier_indices;
  ValNodePtr  vnp;
  Int2    feature_index;
  CharPtr  org_desc;
  CharPtr  cp;
  Int4    no_semicolon_len;
  Int4    label_len;

  biop = NULL;
  strings = NULL;
  taxName [0] = '\0';

  biop = GetBiopForBsp (bsp);
  if (biop == NULL) return NULL;
  if (biop->org == NULL) return NULL;
  if (biop->org->taxname == NULL) return NULL;
  StringNCpy (taxName, biop->org->taxname, sizeof (taxName) - 1);
  taxName [ sizeof (taxName) - 1] = 0;

  CleanUpTaxName (taxName, odmp->keep_paren);

  if (odmp->exclude_sp)
  {
    cp = StringStr (taxName, " sp.");
    if (cp != NULL && cp[4] == ' '
      && (cp - taxName < 2 || *(cp - 2) != 'f' || *(cp - 1) != '.'))
    {
      return StringSave (taxName);
    }
  }

  ValNodeAddStr (&strings, 0, StringSave (taxName));

  modifier_indices = CopyModifierIndices (m->modifier_indices);
  AddHIVModifierIndices (&modifier_indices, biop, modList, taxName,
                         odmp->clone_isolate_HIV_rule_num);
  modifier_indices = SortValNode (modifier_indices, SortByImportance);
  for (vnp = modifier_indices;
       vnp != NULL && (odmp->max_mods == -99 || odmp->max_mods > 0);
       vnp = vnp->next)
  {
    feature_index = vnp->data.intvalue;
    if (! odmp->use_modifiers && !DefLineModifiers[feature_index].default_required)
    {
      /* do nothing */
    }
    else if (DefLineModifiers[feature_index].isOrgMod)
    {
      if (biop->org == NULL || biop->org->orgname == NULL) continue;
      mod = biop->org->orgname->mod;
      while (mod != NULL
        && mod->subtype != DefLineModifiers[feature_index].subtype)
      {
        mod = mod->next;
      }
      if ( UseOrgModifier (mod, taxName))
      {
        no_semicolon_len = StringCSpn (mod->subname, ";");

        if (mod->subtype == ORGMOD_nat_host)
        {
          sprintf (modifier_text, "from ");
          if (no_semicolon_len > sizeof (modifier_text) - 6)
          {
            no_semicolon_len = sizeof (modifier_text) - 6;
          }
          StringNCpy (modifier_text + 5, mod->subname,
                      no_semicolon_len);
          modifier_text[no_semicolon_len + 5] = 0;
        }
        else
        {
          AddModifierLabel (odmp->use_labels, feature_index, modifier_text);
          if (modifier_text[0] != 0)
            StringCat (modifier_text, " ");
          label_len = StringLen (modifier_text);
          if (no_semicolon_len > (Int4) sizeof (modifier_text) - label_len - 1)
          {
            no_semicolon_len = (Int4) sizeof (modifier_text) - label_len - 1;
          } 
          StringNCat (modifier_text, mod->subname,
                      no_semicolon_len);
          modifier_text [ no_semicolon_len + label_len] = 0;
        }
        ValNodeCopyStr( &strings, 0, modifier_text);
        if (odmp->max_mods != -99)
          odmp->max_mods --;
      }
    } else {
      ssp = biop->subtype;
      while (ssp != NULL
          && ssp->subtype != DefLineModifiers[feature_index].subtype)
      {
        ssp = ssp->next;
      }
      if (ssp != NULL)
      {
        no_semicolon_len = StringCSpn (ssp->name, ";");
        AddModifierLabel (odmp->use_labels, feature_index, modifier_text);
        if (ssp->subtype == SUBSRC_transgenic)
        {
          /* do nothing, transgenic already captured from label */
        }
        else if (ssp->subtype == SUBSRC_country)
        {
          sprintf (modifier_text, "from ");
          if (no_semicolon_len > sizeof (modifier_text) - 6)
          {
            no_semicolon_len = sizeof (modifier_text) - 6;
          }
          StringNCpy (modifier_text + 5, ssp->name, no_semicolon_len);
          modifier_text[5 + no_semicolon_len] = 0;
          cp = StringChr (modifier_text, ':');
          if (cp != NULL) *cp = 0;
        }
        else if (ssp->name != NULL && ssp->name[0] != 0
          && (ssp->subtype != SUBSRC_plasmid_name
            || StringCmp (ssp->name, "unnamed") != 0))
        {
          if (modifier_text[0] != 0)
            StringCat (modifier_text, " ");
          label_len = StringLen (modifier_text);
          if (no_semicolon_len > sizeof (modifier_text) - 1 - label_len)
          {
            no_semicolon_len = sizeof (modifier_text) - 1 - label_len;
          }
          StringNCat (modifier_text, ssp->name, no_semicolon_len);
          modifier_text [ no_semicolon_len + label_len ] = 0;
        }
        ValNodeCopyStr( &strings, 0, modifier_text);
        if (odmp->max_mods != -99)
          odmp->max_mods --;
      }
    }
  }
  org_desc = MergeValNodeStrings (strings, FALSE);
  ValNodeFreeData (strings);
  ValNodeFree (modifier_indices);
  return org_desc;

}

/* end of organism description section */

/* This section of code contains functions which are useful for dealing
 * with locations of features (SeqLocPtr objects).
 */

/* This function determines whether location A is on the same strand as
 * location B
 */
static Boolean AreAAndBOnSameStrand (
  SeqLocPtr slp1,
  SeqLocPtr slp2
)
{
  Uint1 strand1;
  Uint2 strand2;

  strand1 = SeqLocStrand (slp1);
  strand2 = SeqLocStrand (slp2);
  if (strand1 == Seq_strand_minus && strand2 != Seq_strand_minus)
    return FALSE;
  else if (strand1 != Seq_strand_minus && strand2 == Seq_strand_minus)
    return FALSE;
  else
    return TRUE;
}

/* This function determines whether location A is contained in or equal to
 * location B and on the same strand as location B.
 */
static Boolean IsLocAInBonSameStrand (
  SeqLocPtr slp1,
  SeqLocPtr slp2
)
{
  if (! AreAAndBOnSameStrand ( slp1, slp2))
  {
    return FALSE;
  }
  else if ( SeqLocAinB (slp1, slp2) < 0)
  {
    return FALSE;
  }
  else
  {
    return TRUE;
  }
}

/* This function calculates the intersection between two locations.
 */
static SeqLocPtr SeqLocIntersection (
  SeqLocPtr slp1,
  SeqLocPtr slp2,
  BioseqPtr bsp
)
{
  SeqLocPtr diff1, diff2, result;

  diff1 = SeqLocMerge ( bsp, slp1, NULL, FALSE, TRUE, FALSE);
  diff1 = SeqLocSubtract (diff1, slp2);
  diff2 = SeqLocMerge ( bsp, slp2, NULL, FALSE, TRUE, FALSE);
  diff2 = SeqLocSubtract (diff2, slp1);
  result = SeqLocMerge ( bsp, slp1, slp2, FALSE, TRUE, FALSE);
  
  if (diff1 != NULL)
  {
    result = SeqLocSubtract (result, diff1);
    SeqLocFree (diff1);
    if (result == NULL) return NULL;
  }
  if (diff2 != NULL)
  {
    result = SeqLocSubtract (result, diff2);
    SeqLocFree (diff2);
    if (result == NULL) return NULL;
  }
  return result;
}

#define ADJACENT_TYPE_ANY        0
#define ADJACENT_TYPE_UPSTREAM   1
#define ADJACENT_TYPE_DOWNSTREAM 2

/* This function determines whether A is "next to" B and upstream or downstream
 * from B.  A cannot overlap B.  If allow_interval is TRUE, there can be
 * space between A and B.
 */
static Boolean IsAAdjacentToB (
  SeqLocPtr a,
  SeqLocPtr b,
  BioseqPtr bsp,
  Int2      adjacent_type,
  Boolean   allow_interval
)
{
  Int4      a_end, b_end;
  Uint2     strand;
 
  if (adjacent_type != ADJACENT_TYPE_ANY
    && adjacent_type != ADJACENT_TYPE_UPSTREAM
    && adjacent_type != ADJACENT_TYPE_DOWNSTREAM)
  {
    return FALSE;
  }
  
  if ( ! AreAAndBOnSameStrand (a, b))
  {
    return FALSE;
  }

  strand = SeqLocStrand (a);
  if ( adjacent_type == ADJACENT_TYPE_ANY)
  {
    a_end = GetOffsetInBioseq (a, bsp, SEQLOC_RIGHT_END);
    b_end = GetOffsetInBioseq (b, bsp, SEQLOC_LEFT_END);
    if ((allow_interval && b_end < a_end)
      || b_end == a_end + 1)
    {
      return TRUE;
    }
    a_end = GetOffsetInBioseq (a, bsp, SEQLOC_LEFT_END);
    b_end = GetOffsetInBioseq (b, bsp, SEQLOC_RIGHT_END);
    if ((allow_interval && b_end > a_end)
      || a_end == b_end + 1)
    {
      return TRUE;
    }
    return FALSE;
  }
  else if ( (strand == Seq_strand_minus
      && adjacent_type == ADJACENT_TYPE_UPSTREAM)
    || (strand != Seq_strand_minus
      && adjacent_type == ADJACENT_TYPE_DOWNSTREAM))
  {
    a_end = GetOffsetInBioseq (a, bsp, SEQLOC_RIGHT_END);
    b_end = GetOffsetInBioseq (b, bsp, SEQLOC_LEFT_END);
    if ((allow_interval && b_end < a_end)
      || b_end == a_end + 1)
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
  else
  {
    a_end = GetOffsetInBioseq (a, bsp, SEQLOC_LEFT_END);
    b_end = GetOffsetInBioseq (b, bsp, SEQLOC_RIGHT_END);
    if ((allow_interval && b_end > a_end)
      || a_end == b_end + 1)
    {
      return TRUE;
    }
    else
    {
      return FALSE;
    }
  }
}

/* This section of code deals with identifying and labeling features
 * for the definition line.
 * The features currently handled are:
 *     genes
 *     exons
 *     introns
 *     LTRs
 *     3' UTRs
 *     5' UTRs
 *     CDSs
 *     rRNA
 *     mRNA
 *     misc RNA
 *     snRNA
 *     snoRNA
 *     insertion sequences
 *     D-loops
 *     mRNA
 *     tRNA
 *     control regions
 *     misc feature listed as intergenic spacer in comment
 *     satellite sequences
 *     promoter regions
 *     endogenous virus source features
 *     transposons
 */

static Boolean LIBCALLBACK IsGene (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->data.choice != SEQFEAT_GENE) return FALSE;
  return TRUE;
}

static CharPtr GetGeneName (
GeneRefPtr grp
)
{
  ValNodePtr syn;

  if (grp == NULL) return NULL;
  if (SeqMgrGeneIsSuppressed (grp)) return NULL;
  if (StringDoesHaveText (grp->locus)) return grp->locus;
  if (StringDoesHaveText (grp->locus_tag)) return grp->locus_tag;
  if (StringDoesHaveText (grp->desc)) return grp->desc;
  for (syn = grp->syn; syn != NULL; syn = syn->next)
  {
    if (syn != NULL && syn->data.ptrvalue != NULL)
      return syn->data.ptrvalue;
  }
  return NULL;
}

static CharPtr GetAlleleName (
GeneRefPtr grp
)
{
  size_t  lenallele;
  size_t  lengenename;
  CharPtr  gene_name;
  CharPtr  buffer;

  if (grp == NULL) return NULL;
  if (StringHasNoText (grp->allele)) return NULL;
  gene_name = GetGeneName (grp);
  if (StringHasNoText (gene_name)) return NULL;
  lenallele = StringLen (grp->allele);
  lengenename = StringLen (gene_name);
  
  if (lenallele > lengenename
    && StringNICmp (gene_name, grp->allele, lengenename) == 0)
  {
    return StringSave (grp->allele);
  }
  else if (grp->allele[0] == '-')
  {
    buffer = MemNew (lenallele + lengenename + 1);
    if (buffer == NULL) return NULL;
    StringCpy (buffer, gene_name);
    StringCat (buffer, grp->allele);
  }
  else
  {
    buffer = MemNew (lenallele + lengenename + 2);
    if (buffer == NULL) return NULL;
    StringCpy (buffer, gene_name);
    StringCat (buffer, "-");
    StringCat (buffer, grp->allele);
  }

  return buffer;
}

/* This function compares the gene names and allele names of the gene
 * to see if they match.
 */
static Boolean DoGenesMatch (
  GeneRefPtr grp1,
  GeneRefPtr grp2
)
{
  CharPtr name1;
  CharPtr name2;

  name1 = GetGeneName (grp1);
  name2 = GetGeneName (grp2);
  if (StringCmp (name1, name2) != 0) return FALSE;
  
  name1 = GetAlleleName (grp1);
  name2 = GetAlleleName (grp2);
  if ((name1 == NULL && name2 != NULL)
    || (name1 != NULL && name2 == NULL))
  {
    if (name1 != NULL) MemFree (name1);
    if (name2 != NULL) MemFree (name2);
    return FALSE;
  }

  if ((name1 == NULL && name2 == NULL)
           || (StringCmp (name1, name2) == 0))
  {
    if (name1 != NULL) MemFree (name1);
    if (name2 != NULL) MemFree (name2);
    return TRUE;
  }
  
  if (name1 != NULL) MemFree (name1);
  if (name2 != NULL) MemFree (name2);
  return  FALSE;
}

/* This function looks at the pseudo flag on the object itself as well as
 * the pseudo flag on the gene reference for the object (if one is present).
 */
static Boolean IsPseudo (
  SeqFeatPtr sfp
)
{
  GeneRefPtr grp;

  if (sfp == NULL) return FALSE;
  if (sfp->pseudo) return TRUE;
  if (sfp->data.choice == SEQFEAT_GENE)
  {
    grp = sfp->data.value.ptrvalue;
  }
  else
  { 
    grp = SeqMgrGetGeneXref (sfp);
  }
  if (grp == NULL) return FALSE;
  if (grp->pseudo) return TRUE;
  return FALSE;
}

static Boolean LIBCALLBACK IsExon (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_exon) return FALSE;
  return TRUE;
}

static Boolean LIBCALLBACK IsIntron (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_intron) return FALSE;
  return TRUE;
}

static Boolean LIBCALLBACK IsLTR (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_LTR) return FALSE;
  return TRUE;
}

static CharPtr GetLTRDescription (
  SeqFeatPtr sfp
)
{
  CharPtr description;
  size_t comment_len;
  if (sfp == NULL) return NULL;
  if (sfp->comment == NULL) return NULL;
  comment_len = StringLen (sfp->comment);
  if (comment_len > 3 && StringCmp (sfp->comment + comment_len - 3, "LTR") == 0)
  {
    description = (CharPtr) MemNew (comment_len - 3);
    if (description == NULL) return NULL;
    StringNCpy (description, sfp->comment, comment_len - 4);
    description[comment_len - 4] = 0;
  }
  else
  {
    description = StringSave (sfp->comment);
  }
  return description;
}

static Boolean LIBCALLBACK Is3UTR (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_3UTR) return FALSE;
  return TRUE;
}

static Boolean LIBCALLBACK Is5UTR (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_5UTR) return FALSE;
  return TRUE;
}

static Boolean LIBCALLBACK IsCDS (SeqFeatPtr sfp)
{
  if (sfp == NULL) return FALSE;
  if (sfp->data.choice == SEQFEAT_CDREGION)
    return TRUE;
  return FALSE;
}

static Boolean LIBCALLBACK IsrRNA (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_rRNA) return FALSE;
  return TRUE;
}

static Boolean LIBCALLBACK IsMiscRNA (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL 
    || (sfp->idx.subtype != FEATDEF_misc_RNA
      && sfp->idx.subtype != FEATDEF_otherRNA)) 
  {
    return FALSE;
  }
  return TRUE;
}

static Boolean LIBCALLBACK IssnoRNA (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_snoRNA) return FALSE;
  return TRUE;
}

static Boolean LIBCALLBACK IssnRNA (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_snRNA) return FALSE;
  return TRUE;
}

static Boolean LIBCALLBACK IsInsertionSequence (
  SeqFeatPtr sfp
)
{
  GBQualPtr gbqual;
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_repeat_region) return FALSE;
  
  for (gbqual = sfp->qual; gbqual != NULL; gbqual = gbqual->next)
  {
    if (StringCmp (gbqual->qual, "insertion_seq") == 0)
      return TRUE;
  }
  return FALSE;
}

static CharPtr GetInsertionSequenceDescription (
  SeqFeatPtr sfp
)
{
  GBQualPtr gbqual;

  if (sfp == NULL || sfp->qual == NULL) return NULL;
  
  gbqual = sfp->qual;
  while (gbqual != NULL && StringCmp (gbqual->qual, "insertion_seq") != 0)
  {
    gbqual = gbqual->next;
  }
  if (gbqual != NULL)
  {
    if (StringDoesHaveText (gbqual->val)
      && StringCmp (gbqual->val, "unnamed") != 0)
    {
      return StringSave (gbqual->val);
    }
  }
  return NULL;
}

static Boolean LIBCALLBACK IsEndogenousVirusSequence (
  SeqFeatPtr sfp
)
{
  GBQualPtr gbqual;
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_repeat_region) return FALSE;
  
  for (gbqual = sfp->qual; gbqual != NULL; gbqual = gbqual->next)
  {
    if (StringCmp (gbqual->qual, "endogenous_virus") == 0)
      return TRUE;
  }
  return FALSE;
}

static CharPtr GetEndogenousVirusSequenceDescription (
  SeqFeatPtr sfp
)
{
  GBQualPtr gbqual;

  if (sfp == NULL) return NULL;
  
  gbqual = sfp->qual;
  while (gbqual != NULL && StringCmp (gbqual->qual, "endogenous_virus") != 0)
  {
    gbqual = gbqual->next;
  }
  if (gbqual != NULL)
  {
    if (StringDoesHaveText (gbqual->val)
      && StringCmp (gbqual->val, "unnamed") != 0)
    {
      return StringSave (gbqual->val);
    }
  }
  return NULL;
}

static Boolean LIBCALLBACK IsDloop (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_D_loop) return FALSE;
  return TRUE;
}

static Boolean LIBCALLBACK IsmRNA (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_mRNA) return FALSE;
  return TRUE;
}

static Boolean LIBCALLBACK IstRNA (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_tRNA) return FALSE;
  return TRUE;
}

static Boolean LIBCALLBACK IsControlRegion (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL
    || sfp->idx.subtype != FEATDEF_misc_feature
    || sfp->comment == NULL
    || StringNCmp (sfp->comment, "control region", StringLen ("control region")) != 0)
  {
    return FALSE;
  }
  return TRUE;
}

static Boolean LIBCALLBACK IsIntergenicSpacer (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL
    || sfp->idx.subtype != FEATDEF_misc_feature
    || sfp->comment == NULL
    || StringStr (sfp->comment, "intergenic spacer") == NULL)
  {
    return FALSE;
  }
  return TRUE;
}

static void LIBCALLBACK GetIntergenicSpacerFeatureLabel (
  ValNodePtr      featlist,
  BioseqPtr       bsp,
  Uint1           biomol,
  FeatureLabelPtr flp
)
{
  SeqFeatPtr main_feat;
  CharPtr    cp, buffer;
  Int4       datalen, offset;

  if (featlist == NULL || featlist->data.ptrvalue == NULL) return;
  main_feat = featlist->data.ptrvalue;
  if (StringHasNoText (main_feat->comment)) return;
  if (StringNCmp (main_feat->comment, "contains ", 9) == 0)
  {
    buffer = main_feat->comment + 9;
  }
  else
  {
    buffer = main_feat->comment;
  }
  cp = StringStr (buffer, "intergenic spacer");
  if (cp == NULL) return;
  flp->typeword = StringSave ("intergenic spacer");
  flp->pluralizable = FALSE;
  if (cp == buffer)
  {
    flp->is_typeword_first = TRUE;
    offset = StringLen ("intergenic spacer") + 1;
    if (StringNCmp (cp + offset, "and ", 4) == 0)
    {
      flp->description = NULL;
    }
    else
    {
      flp->description = StringSave (cp + StringLen ("intergenic spacer") + 1);
    }
  }
  else
  {
    flp->is_typeword_first = FALSE;
    datalen = cp - buffer;
    flp->description = MemNew ( datalen + 1);
    if (flp->description == NULL) return;
    StringNCpy (flp->description, buffer, datalen);
    flp->description [datalen] = 0;
    TrimSpacesAroundString (flp->description);
  }
}

static Boolean LIBCALLBACK IsSatelliteSequence (
  SeqFeatPtr sfp
)
{
  GBQualPtr gbqual;
  if (sfp == NULL
    || sfp->idx.subtype != FEATDEF_repeat_region
    || StringHasNoText (sfp->comment)
    || (StringNCmp (sfp->comment, "microsatellite", 14) != 0
      && StringNCmp (sfp->comment, "satellite", 9) != 0))
  {
    return FALSE;
  }

  for (gbqual = sfp->qual; gbqual != NULL; gbqual = gbqual->next)
  {
    if (StringCmp (gbqual->qual, "rpt_type") == 0
      && StringCmp (gbqual->val, "tandem") == 0)
    {
      return TRUE;
    }
  }
  return FALSE;
}

static Boolean LIBCALLBACK IsPromoter (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL || sfp->idx.subtype != FEATDEF_promoter) return FALSE;
  return TRUE;
}

static Boolean LIBCALLBACK IsEndogenousVirusSourceFeature (
  SeqFeatPtr sfp
)
{
  BioSourcePtr biop;
  SubSourcePtr  ssp;

  if (sfp == NULL || sfp->idx.subtype != FEATDEF_BIOSRC) return FALSE;
  if ((biop = sfp->data.value.ptrvalue) == NULL) return FALSE;
  ssp = biop->subtype;
  while (ssp != NULL && ssp->subtype != SUBSRC_endogenous_virus_name)
  {
    ssp = ssp->next;
  }
  if (ssp != NULL) return TRUE;
  return FALSE;
}

static CharPtr GetEndogenousVirusSourceFeatureDescription (
  SeqFeatPtr sfp
)
{
  BioSourcePtr biop;
  SubSourcePtr  ssp;

  if (sfp == NULL || sfp->idx.subtype != FEATDEF_BIOSRC) return NULL;
  if ((biop = sfp->data.value.ptrvalue) == NULL) return NULL;
  ssp = biop->subtype;
  while (ssp != NULL && ssp->subtype != SUBSRC_endogenous_virus_name)
  {
    ssp = ssp->next;
  }
  if (ssp != NULL && ssp->name != NULL)
  {
    return StringSave (ssp->name);
  }
  return NULL;
}

static Boolean LIBCALLBACK IsTransposon (
  SeqFeatPtr sfp
)
{
  GBQualPtr gbqual;

  if (sfp == NULL || sfp->idx.subtype != FEATDEF_repeat_region) return FALSE;
  
  for (gbqual = sfp->qual; gbqual != NULL; gbqual = gbqual->next)
  {
    if (StringCmp (gbqual->qual, "transposon") == 0)
      return TRUE;
  }
  return FALSE;
}

static CharPtr transposon_keywords [] = {
  "retrotransposon",
  "P-element",
  "transposable element",
  "integron",
  "superintegron",
  "MITE"
};
  
static void LIBCALLBACK GetTransposonFeatureLabel (
  ValNodePtr      featlist,
  BioseqPtr       bsp,
  Uint1           biomol,
  FeatureLabelPtr flp
)
{
  GBQualPtr  gbqual;
  Int4    i;
  Int4    keyword_len;
  Int4    val_len;
  SeqFeatPtr      sfp;

  flp->pluralizable = TRUE;
  flp->is_typeword_first = FALSE;
  flp->typeword = NULL;
  flp->description = NULL;

  if (featlist == NULL) return;
  sfp = featlist->data.ptrvalue;
  if (sfp == NULL) return;

  gbqual = sfp->qual;
  while (gbqual != NULL && StringCmp (gbqual->qual, "transposon") != 0)
  {
    gbqual = gbqual->next;
  }
  if (gbqual == NULL) return;
  if (StringHasNoText (gbqual->val))
  {
    flp->is_typeword_first = FALSE;
    flp->typeword = StringSave ("transposon");
    flp->description = StringSave ("unnamed");
    return;
  }

  for (i=0; i < sizeof (transposon_keywords) / sizeof (CharPtr); i++)
  {
    keyword_len = StringLen (transposon_keywords[i]);
    val_len = StringLen (gbqual->val);
    if (val_len == keyword_len)
    {
      if (StringCmp (gbqual->val, transposon_keywords[i]) == 0)
      {
        flp->is_typeword_first = FALSE;
        flp->typeword = StringSave (transposon_keywords[i]);
        flp->description = StringSave ("unnamed");
        return;
      }
    } 
    else if (val_len > keyword_len)
    {
      if (StringNCmp (transposon_keywords[i], gbqual->val, keyword_len) == 0
        && gbqual->val[keyword_len] == ' ')
      {
        flp->is_typeword_first = TRUE;
        flp->typeword = StringSave (transposon_keywords[i]);
        flp->description = StringSave (gbqual->val + keyword_len + 1);
        return;
      }
      else if (StringCmp (gbqual->val + val_len - keyword_len,
                          transposon_keywords[i]) == 0)
      {
        flp->is_typeword_first = FALSE;
        flp->typeword = StringSave (transposon_keywords[i]);
        flp->description = MemNew (val_len - keyword_len);
        if (flp->description == NULL) return;
        StringNCpy (flp->description, gbqual->val, val_len - keyword_len - 1);
        flp->description[val_len - keyword_len -1] = 0;
        return;
      }
    }
  }
  flp->is_typeword_first = TRUE;
  flp->typeword = StringSave ("transposon");
  flp->description = StringSave (gbqual->val);
}

static CharPtr noncoding_feature_keywords[] = {
  "contains ",
  "similar to "
};

static CharPtr find_noncoding_feature_keyword (
  CharPtr comment
)
{
  Int4 i, num_noncoding_feature_keywords, keywordlen;
  CharPtr cp, buffer;

  if (comment == NULL) return NULL;
  num_noncoding_feature_keywords = sizeof (noncoding_feature_keywords) / sizeof (CharPtr);
  for (i=0; i < num_noncoding_feature_keywords; i++)
  {
    keywordlen = StringLen (noncoding_feature_keywords [i]);
    buffer = comment;
    while ((cp = StringStr (buffer, noncoding_feature_keywords [i])) != NULL)
    {
      if ( StringNCmp (cp + keywordlen,
                       "GenBank Accession Number",
                       StringLen ("GenBank Accession Number")) != 0)
      {
        return cp + keywordlen;
      }
      else
      {
        buffer = cp + 1;
      }
    }
  }
  return NULL;
}

static Boolean LIBCALLBACK IsNoncodingProductFeat (
  SeqFeatPtr sfp
)
{
  if ( sfp == NULL
    || sfp->idx.subtype != FEATDEF_misc_feature
    || sfp->comment == NULL
    || (find_noncoding_feature_keyword (sfp->comment) == NULL
      && (StringStr (sfp->comment, "nonfunctional ") == NULL
        || StringStr (sfp->comment, " due to ") == NULL)))
  {
    return FALSE;
  }

  return TRUE;
}

static CharPtr GetNoncodingProductFeatProduct (
  SeqFeatPtr sfp
)
{
  CharPtr productname;
  Int4    namelen, compare_len;
  CharPtr name_start, sep;

  if (sfp == NULL || sfp->comment == NULL) return NULL;

  if ((name_start = StringStr (sfp->comment, "nonfunctional ")) != NULL
    && (sep = StringStr (sfp->comment, " due to ")) != NULL
    && sep > name_start)
  {
    productname = StringSave (name_start);
    productname [ sep - name_start] = 0;
    return productname;
  }

  name_start = find_noncoding_feature_keyword (sfp->comment);
  if (name_start == NULL) return NULL;

  sep = StringStr (name_start, ";");
  if (sep == NULL)
  {
    namelen = StringLen (name_start);
  }
  else
  {
    namelen = sep - name_start;
  }

  productname = MemNew (namelen + 6);
  if (productname == NULL) return NULL;

  StringNCpy (productname, name_start, namelen);
  productname [namelen] = 0;
  
  /* remove sequence from end of name if present */
  compare_len = StringLen (" sequence");
  if (StringCmp (productname + namelen - compare_len, " sequence") == 0)
  {
    productname [ namelen - compare_len] = 0;
    namelen = StringLen (productname);
  }
  /* add "-like" if not present */
  compare_len = StringLen ("-like");
  if (StringCmp (productname + namelen - compare_len, "-like") != 0)
  {
    StringCat (productname, "-like");
    namelen = StringLen (productname);
  }
  return productname;
}

static Boolean LIBCALLBACK IsMiscFeat (
  SeqFeatPtr sfp
)
{
  if ( sfp == NULL
    || sfp->idx.subtype != FEATDEF_misc_feature
    || sfp->comment == NULL)
  {
    return FALSE;
  }

  return TRUE;
}

static Boolean LIBCALLBACK IsOperon (
  SeqFeatPtr sfp
)
{
  if (sfp == NULL 
    || sfp->idx.subtype != FEATDEF_operon)
  {
    return FALSE;
  }

  return TRUE;
}

static Boolean IsRecognizedFeature (
  SeqFeatPtr sfp
)
{
  if (IsGene (sfp)
    || IsCDS (sfp)
    || IsExon (sfp)
    || IsIntron (sfp)
    || IsLTR (sfp)
    || IsrRNA (sfp)
    || IstRNA (sfp)
    || IsmRNA (sfp)
    || IsMiscRNA (sfp)
    || IssnoRNA (sfp)
    || IssnRNA (sfp)
    || Is3UTR (sfp)
    || Is5UTR (sfp)
    || IsInsertionSequence (sfp)
    || IsEndogenousVirusSequence (sfp)
    || IsEndogenousVirusSourceFeature (sfp)
    || IsDloop (sfp)
    || IsSatelliteSequence (sfp)
    || IsControlRegion (sfp)
    || IsIntergenicSpacer (sfp)
    || IsTransposon (sfp)
    || IsNoncodingProductFeat (sfp)
    || IsPromoter (sfp)
    || IsMiscFeat (sfp)
    || IsOperon (sfp))
  {
    return TRUE;
  }
  else
  {
    return FALSE;
  }
}

/* The following section of code contains functions for dealing with lists of
 * clauses.
 */

/* The functions for freeing the memory associated with lists of clauses 
 * are recursive.
 */
static void FreeListElement (ValNodePtr element);

/* This function simply frees the ValNodePtr, since there is no extra
 * memory associated with a DEFLINE_FEATLIST item - the sfp that is
 * pointed to by data.ptrvalue came from the sequence indexing functions
 * and should under no circumstances be freed.
 */
static void FreeFeatlist (
  ValNodePtr featlist
)
{

  if (featlist == NULL) return;
  ValNodeFree (featlist);
}
 
/* This function frees the memory associated with a FeatureClause, including
 * the memory associated with any subclauses.
 */
static void FreeClausePlusData (
  FeatureClausePtr fcp
)
{
  if (fcp->interval != NULL)
  {
    MemFree (fcp->interval);
    fcp->interval = NULL;
  }
  if (fcp->allelename != NULL)
  {
    MemFree (fcp->allelename);
    fcp->allelename = NULL;
  }
  if (fcp->feature_label_data.typeword != NULL)
  {
    MemFree (fcp->feature_label_data.typeword);
    fcp->feature_label_data.typeword = NULL;
  }
  if (fcp->feature_label_data.description != NULL)
  {
    MemFree (fcp->feature_label_data.description);
    fcp->feature_label_data.description = NULL;
  }
  if (fcp->feature_label_data.productname != NULL)
  {
    MemFree (fcp->feature_label_data.productname);
    fcp->feature_label_data.productname = NULL;
  }
  if (fcp->featlist != NULL)
  {
    FreeListElement (fcp->featlist);
    fcp->featlist = NULL;
  }
  if (fcp->slp != NULL)
  {
    SeqLocFree (fcp->slp);
  }
}
    
/* This function frees the data associated with the FeatureClause
 * and then frees the ValNode.
 */
static void FreeClausePlus (
  ValNodePtr clauseplus
)
{
  FeatureClausePtr data_struct;

  if (clauseplus == NULL) return;
  data_struct = (FeatureClausePtr) clauseplus->data.ptrvalue;
  if (data_struct != NULL)
  {
    FreeClausePlusData (data_struct); 
    MemFree (data_struct);
    clauseplus->data.ptrvalue = NULL;
  }
  ValNodeFree (clauseplus);
}
 
/* This function frees a list of DEFLINE_FEATLIST, DEFLINE_REMOVEFEAT,
 * and DEFLINE_CLAUSEPLUS items, starting with the last item in the list.
 * It recursively frees memory associated with subclauses.
 */ 
static void FreeListElement (
  ValNodePtr element
)
{
  if (element == NULL) return;

  FreeListElement (element->next);
  element->next = NULL;
  if (element->choice == DEFLINE_FEATLIST 
    || element->choice == DEFLINE_REMOVEFEAT)
  {
    FreeFeatlist (element);
  }
  else if (element->choice == DEFLINE_CLAUSEPLUS)
  {
    FreeClausePlus (element);
  }
}

/* This function excises from the list pointed to by head all of the clauses
 * with the delete_me flag set to TRUE and all of the ValNodes with a choice
 * of DEFLINE_REMOVEFEAT.
 */
static void DeleteFeatureClauses (
  ValNodePtr PNTR head
)
{
  ValNodePtr vnp, prev;
  FeatureClausePtr fcp;
  Boolean          delete_this_one;

  if (head == NULL) return;

  prev = NULL;
  vnp = *head;
  while (vnp != NULL)
  {
    delete_this_one = FALSE;

    if (vnp->choice == DEFLINE_CLAUSEPLUS)
    {
      fcp = vnp->data.ptrvalue;
      if (fcp == NULL || fcp->delete_me || fcp->featlist == NULL) 
      {
        delete_this_one = TRUE;
      }
      else
      {
        DeleteFeatureClauses (&fcp->featlist);
        if (fcp->featlist == NULL) delete_this_one = TRUE;
      }
    }
    else if (vnp->choice == DEFLINE_REMOVEFEAT)
    {
      delete_this_one = TRUE;
    }

    if (delete_this_one)
    {
      if (prev == NULL)
      {
        *head = vnp->next;
        vnp->next = NULL;
        FreeListElement (vnp);
        if (*head == NULL) return;
        vnp = *head;
      }
      else
      {
        prev->next = vnp->next;
        vnp->next = NULL;
        FreeListElement (vnp);
        vnp = prev->next;
      }
    }
    else
    {
      prev = vnp;
      vnp = vnp->next;
    }
  }
}

/* This function counts the number of features in the feature list that
 * satisfy the itemmatch function (or all of them, if itemmatch is NULL).
 * If recurse_past_found_item, the function will not count features in
 * subclauses of features that satisfy itemmatch.
 */
static Int4 CountFeatures (
  ValNodePtr clause_list,
  matchFunction  itemmatch,
  Boolean    recurse_past_found_item
)
{
  ValNodePtr       vnp;
  Int4             num_features;
  FeatureClausePtr fcp;

  num_features = 0;
  for (vnp = clause_list;
       vnp != NULL;
       vnp = vnp->next)
  {
    if (vnp->choice == DEFLINE_FEATLIST 
      && (itemmatch == NULL || itemmatch (vnp->data.ptrvalue)))
    {
      num_features++;
      if (! recurse_past_found_item)
      {
        return num_features;
      }
    }
    else if (vnp->choice == DEFLINE_CLAUSEPLUS
      && (fcp = vnp->data.ptrvalue) != NULL)
    {
      num_features += CountFeatures (fcp->featlist,
                                     itemmatch,
                                     recurse_past_found_item);   
    }
  }
  return num_features;
}

/* The following section of code contains functions for grouping features. */

typedef struct matchruledata {
  matchFunction is_item;
  Int4          num_match_rules;
  matchFunction *match_rules;
} MatchRuleData, PNTR MatchRulePtr;

/* NumGroupingRules is the number of features for which there is a list of
 * features to group under.
 * When grouping features, each feature in the list is examined sequentially.
 * If there is a rule set that applies to that feature, the entire feature 
 * list is searched for each feature type that this feature might group
 * beneath.  This preserves the biological order that was generated by the
 * original listing of features by sequence indexing.
 */
#define  NumGroupingRules 11
static MatchRulePtr InitializeGroupingRules()
{
  MatchRulePtr grouping_rules;

  grouping_rules = MemNew (NumGroupingRules * sizeof (MatchRuleData));
  if (grouping_rules == NULL) return NULL;

  grouping_rules[0].is_item = IsExon;
  grouping_rules[0].num_match_rules = 7;
  grouping_rules[0].match_rules = MemNew (grouping_rules[0].num_match_rules
                                    * sizeof (matchFunction));
  if (grouping_rules[0].match_rules == NULL) return NULL;
  grouping_rules[0].match_rules[0] = IsCDS;
  grouping_rules[0].match_rules[1] = IsNoncodingProductFeat;
  grouping_rules[0].match_rules[2] = IsDloop;
  grouping_rules[0].match_rules[3] = IsmRNA;
  grouping_rules[0].match_rules[4] = IsGene;
  grouping_rules[0].match_rules[5] = IsEndogenousVirusSourceFeature;
  grouping_rules[0].match_rules[6] = IsOperon;

  grouping_rules[1].is_item = IsIntron;
  grouping_rules[1].num_match_rules = 7;
  grouping_rules[1].match_rules = MemNew (grouping_rules[1].num_match_rules
                                    * sizeof (matchFunction));
  if (grouping_rules[1].match_rules == NULL) return NULL;
  grouping_rules[1].match_rules[0] = IsCDS;
  grouping_rules[1].match_rules[1] = IsNoncodingProductFeat;
  grouping_rules[1].match_rules[2] = IstRNA;
  grouping_rules[1].match_rules[3] = IsDloop;
  grouping_rules[1].match_rules[4] = IsGene;
  grouping_rules[1].match_rules[5] = IsEndogenousVirusSourceFeature;
  grouping_rules[1].match_rules[6] = IsOperon;
  
  grouping_rules[2].is_item = IsPromoter;
  grouping_rules[2].num_match_rules = 5;
  grouping_rules[2].match_rules = MemNew (grouping_rules[2].num_match_rules
                                    * sizeof (matchFunction));
  if (grouping_rules[2].match_rules == NULL) return NULL;
  grouping_rules[2].match_rules[0] = IsCDS;
  grouping_rules[2].match_rules[1] = IsmRNA;
  grouping_rules[2].match_rules[2] = IsGene;
  grouping_rules[2].match_rules[3] = IsEndogenousVirusSourceFeature;
  grouping_rules[2].match_rules[4] = IsOperon;
 
  grouping_rules[3].is_item = IsCDS;
  grouping_rules[3].num_match_rules = 5;
  grouping_rules[3].match_rules = MemNew (grouping_rules[3].num_match_rules
                                    * sizeof (matchFunction));
  if (grouping_rules[3].match_rules == NULL) return NULL;
  grouping_rules[3].match_rules[0] = IsmRNA;
  grouping_rules[3].match_rules[1] = IsInsertionSequence;
  grouping_rules[3].match_rules[2] = IsTransposon;
  grouping_rules[3].match_rules[3] = IsEndogenousVirusSourceFeature;
  grouping_rules[3].match_rules[4] = IsOperon;
  
  grouping_rules[4].is_item = IsInsertionSequence;
  grouping_rules[4].num_match_rules = 4;
  grouping_rules[4].match_rules = MemNew (grouping_rules[4].num_match_rules
                                    * sizeof (matchFunction));
  if (grouping_rules[4].match_rules == NULL) return NULL;
  grouping_rules[4].match_rules[0] = IsTransposon;
  grouping_rules[4].match_rules[1] = IsInsertionSequence;
  grouping_rules[4].match_rules[2] = IsEndogenousVirusSourceFeature;
  grouping_rules[4].match_rules[3] = IsOperon;
 
  grouping_rules[5].is_item = Is3UTR;
  grouping_rules[5].num_match_rules = 5;
  grouping_rules[5].match_rules = MemNew (grouping_rules[5].num_match_rules
                                    * sizeof (matchFunction));
  if (grouping_rules[5].match_rules == NULL) return NULL;
  grouping_rules[5].match_rules[0] = IsCDS;
  grouping_rules[5].match_rules[1] = IsmRNA;
  grouping_rules[5].match_rules[2] = IsGene;
  grouping_rules[5].match_rules[3] = IsEndogenousVirusSourceFeature;
  grouping_rules[5].match_rules[4] = IsOperon;
 
  grouping_rules[6].is_item = Is5UTR;
  grouping_rules[6].num_match_rules = 5;
  grouping_rules[6].match_rules = MemNew (grouping_rules[6].num_match_rules
                                    * sizeof (matchFunction));
  if (grouping_rules[6].match_rules == NULL) return NULL;
  grouping_rules[6].match_rules[0] = IsCDS;
  grouping_rules[6].match_rules[1] = IsmRNA;
  grouping_rules[6].match_rules[2] = IsGene;
  grouping_rules[6].match_rules[3] = IsEndogenousVirusSourceFeature;
  grouping_rules[6].match_rules[4] = IsOperon;
 
  grouping_rules[7].is_item = IsLTR;
  grouping_rules[7].num_match_rules = 5;
  grouping_rules[7].match_rules = MemNew (grouping_rules[7].num_match_rules
                                    * sizeof (matchFunction));
  if (grouping_rules[7].match_rules == NULL) return NULL;
  grouping_rules[7].match_rules[0] = IsCDS;
  grouping_rules[7].match_rules[1] = IsmRNA;
  grouping_rules[7].match_rules[2] = IsGene;
  grouping_rules[7].match_rules[3] = IsEndogenousVirusSourceFeature;
  grouping_rules[7].match_rules[4] = IsOperon;
 
  grouping_rules[8].is_item = IsGene;
  grouping_rules[8].num_match_rules = 4;
  grouping_rules[8].match_rules = MemNew (grouping_rules[8].num_match_rules
                                    * sizeof (matchFunction));
  if (grouping_rules[8].match_rules == NULL) return NULL;
  grouping_rules[8].match_rules[0] = IsTransposon;
  grouping_rules[8].match_rules[1] = IsInsertionSequence;
  grouping_rules[8].match_rules[2] = IsEndogenousVirusSourceFeature;
  grouping_rules[8].match_rules[3] = IsOperon;

  grouping_rules[9].is_item = IsTransposon;
  grouping_rules[9].num_match_rules = 4;
  grouping_rules[9].match_rules = MemNew (grouping_rules[9].num_match_rules
                                    * sizeof (matchFunction));
  if (grouping_rules[9].match_rules == NULL) return NULL;
  grouping_rules[9].match_rules[0] = IsTransposon;
  grouping_rules[9].match_rules[1] = IsInsertionSequence;
  grouping_rules[9].match_rules[2] = IsEndogenousVirusSourceFeature;
  grouping_rules[9].match_rules[3] = IsOperon;
 
  grouping_rules[10].is_item = IsNoncodingProductFeat;
  grouping_rules[10].num_match_rules = 4;
  grouping_rules[10].match_rules = MemNew (grouping_rules[10].num_match_rules
                                    * sizeof (matchFunction));
  if (grouping_rules[10].match_rules == NULL) return NULL;
  grouping_rules[10].match_rules[0] = IsTransposon;
  grouping_rules[10].match_rules[1] = IsInsertionSequence;
  grouping_rules[10].match_rules[2] = IsEndogenousVirusSourceFeature;
  grouping_rules[10].match_rules[3] = IsOperon;
 
  return grouping_rules; 
}

static void FreeGroupingRules(
  MatchRulePtr grouping_rules
)
{
  Int4 i;

  if (grouping_rules == NULL) return;

  for (i = 0; i < NumGroupingRules; i++)
  {
    if (grouping_rules[i].match_rules != NULL)
    MemFree (grouping_rules[i].match_rules);
    grouping_rules[i].match_rules = NULL;
  }
  MemFree (grouping_rules);
}

/* This function searches the search_list for features that satisfy the
 * match function and satisfy locational requirements relative to the
 * clause.
 * If such a feature is found, the clause is added to the end of the list
 * of clauses for that feature's parent clause.
 */
static Boolean GroupClauseByMatch (
  FeatureClausePtr clause,
  ValNodePtr       search_list,
  FeatureClausePtr search_parent,
  matchFunction    match,
  BioseqPtr        bsp
)
{
  ValNodePtr       search_clause, newfeat;
  SeqFeatPtr       addsfp;
  FeatureClausePtr searchfcp;
  SeqLocPtr        slp;

  if (clause == NULL || clause->slp == NULL) return FALSE;

  for (search_clause = search_list;
       search_clause != NULL;
       search_clause = search_clause->next)
  {
    if (search_clause->data.ptrvalue == clause)
      continue;
    if (search_clause->choice == DEFLINE_FEATLIST
      && search_clause->data.ptrvalue != NULL)
    {
      addsfp = search_clause->data.ptrvalue;
      if (search_parent != NULL)
      {
        slp = search_parent->slp;
      }
      else
      {
        slp = addsfp->location;
      }
      if (match (search_clause->data.ptrvalue))
      { 
        /* Transposons, insertion sequences, and endogenous virii
         * take subfeatures regardless of whether the subfeature is
         * on the same strand.
         * Promoters will match up to features that are adjacent.
         * All other feature matches must be that the feature to
         * go into the clause must fit inside the location of the
         * other clause.
         */
        if (((match == IsTransposon
             || match == IsInsertionSequence
             || match == IsEndogenousVirusSourceFeature)
            && SeqLocAinB (clause->slp, slp) > -1)
          || IsLocAInBonSameStrand (clause->slp, slp)
          || ( IsPromoter (clause->featlist->data.ptrvalue)
            && IsAAdjacentToB (clause->slp, search_parent->slp, bsp,
                               ADJACENT_TYPE_UPSTREAM, TRUE)))
        {
          newfeat = ValNodeNew (search_clause);
          if (newfeat == NULL) return FALSE;
          newfeat->choice = DEFLINE_CLAUSEPLUS;
          newfeat->data.ptrvalue = clause;
          return TRUE;
        }
      }
    }
    else if (search_clause->choice == DEFLINE_CLAUSEPLUS 
      && search_clause->data.ptrvalue != NULL)
    {
      searchfcp = search_clause->data.ptrvalue;
      if (GroupClauseByMatch (clause, searchfcp->featlist, searchfcp,
                              match, bsp))
      {
        return TRUE;
      }
    }
  }
  return FALSE;
}

/* This function iterates through the matches in the specified grouping rule.
 */
static Boolean GroupClauseByRule (
  FeatureClausePtr clause,
  ValNodePtr       search_list,
  MatchRulePtr     grouping_rule,
  BioseqPtr        bsp
)
{
  Int4       rule_index;

  for (rule_index = 0;
       rule_index < grouping_rule->num_match_rules;
       rule_index ++)
  {
    if ( GroupClauseByMatch (clause,
                            search_list,
                            NULL,
                            grouping_rule->match_rules[rule_index],
                            bsp))
    {
      return TRUE;
    }
  }
  return FALSE;
}

/* This function iterates over the list of features, attempting to find and
 * apply grouping rules for each feature.
 */
static void GroupAllClauses (
  ValNodePtr PNTR clause_list,
  BioseqPtr       bsp
)
{
  MatchRulePtr     grouping_rules;
  ValNodePtr       vnp, prev;
  FeatureClausePtr clause;
  SeqFeatPtr       main_feat;
  Int4             rule_index;

  grouping_rules = InitializeGroupingRules();
  if (grouping_rules == NULL) return;

  for (vnp = *clause_list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice == DEFLINE_CLAUSEPLUS && vnp->data.ptrvalue != NULL)
    {
      clause = vnp->data.ptrvalue;
      if (clause->featlist != NULL
        && clause->featlist->choice == DEFLINE_FEATLIST
        && clause->featlist->data.ptrvalue != NULL)
      {
        main_feat = clause->featlist->data.ptrvalue;
        for (rule_index = 0;
             rule_index < NumGroupingRules 
               && ! grouping_rules[rule_index].is_item (main_feat);
             rule_index++)
        {}
        if (rule_index < NumGroupingRules)
        {
          if ( GroupClauseByRule (clause, *clause_list,
                                  grouping_rules + rule_index, bsp))
          {
            vnp->data.ptrvalue = NULL;
          }
        }
      }
    }
  }
  FreeGroupingRules(grouping_rules);

  vnp = *clause_list;
  prev = NULL;
  while (vnp != NULL)
  {
    if (vnp->data.ptrvalue == NULL)
    {
      if (prev == NULL)
      {
        *clause_list = vnp->next;
        vnp->next = NULL;
        ValNodeFree (vnp);
        vnp = *clause_list;
      }
      else
      {
        prev->next = vnp->next;
        vnp->next = NULL;
        ValNodeFree (vnp);
        vnp = prev->next;
      }
    }
    else
    {
      prev = vnp;
      vnp = vnp->next;
    }
  }
  
}

/* This function exists to handle the special case where two or more exons
 * are alternatively spliced, but there are no CDSs to represent some of the
 * alternatively spliced forms.  In order to make sure that all of the exons
 * that are alternatively spliced together appear with the CDS, they are
 * temporarily consolidated into a single clause with a location that
 * is the intersection of the exons' locations.  The clause will be
 * re-expanded after grouping by the ExpandAltSplicedExons function.
 */
static void GroupAltSplicedExons (
  ValNodePtr PNTR clause_list,
  BioseqPtr       bsp,
  Boolean         delete_now
)
{
  ValNodePtr       clause, search_clause, vnp;
  FeatureClausePtr fcp, search_fcp;
  SeqFeatPtr       sfp, search_sfp;
  SeqLocPtr        new_slp;

  if (clause_list == NULL) return;
 
  for (clause = *clause_list; clause != NULL; clause = clause->next)
  {
    if (clause->choice != DEFLINE_CLAUSEPLUS
      || clause->data.ptrvalue == NULL)
    {
      continue;
    }
    fcp = clause->data.ptrvalue;
    if ( ! fcp->is_alt_spliced
      || fcp->delete_me
      || fcp->featlist == NULL
      || fcp->featlist->choice != DEFLINE_FEATLIST)
    {
      continue;
    }
    sfp = fcp->featlist->data.ptrvalue;
    if ( ! IsExon (sfp))
    {
      continue;
    }

    for ( search_clause = clause->next; 
          search_clause != NULL
            && search_clause->choice == DEFLINE_CLAUSEPLUS
            && search_clause->data.ptrvalue != NULL
            && (search_fcp = search_clause->data.ptrvalue) != NULL
            && ! search_fcp->delete_me
            && search_fcp->is_alt_spliced
            && search_fcp->featlist != NULL
            && search_fcp->featlist->choice == DEFLINE_FEATLIST
            && (search_sfp = search_fcp->featlist->data.ptrvalue) != NULL
            && IsExon (search_sfp)
            && TestFeatOverlap (sfp, search_sfp, SIMPLE_OVERLAP) != -1;
          search_clause = search_clause->next)
    {
      vnp = ValNodeNew (fcp->featlist);
      if (vnp == NULL) return;
      vnp->choice = DEFLINE_FEATLIST;
      vnp->data.ptrvalue = search_sfp;
      search_fcp->delete_me = TRUE;
      new_slp = SeqLocIntersection (fcp->slp, search_fcp->slp, bsp);
      SeqLocFree (fcp->slp);
      fcp->slp = new_slp;
    }
  }
  if (delete_now)
  {
    DeleteFeatureClauses (clause_list);
  }
}

/* This function expands a clause filled with alternatively-spliced exons
 * that was created in the GroupAltSplicedExons function.
 */
static void ExpandAltSplicedExons (
  ValNodePtr clause_list,
  BioseqPtr  bsp
)
{
  ValNodePtr clause, rest_of_list, featlist, new_clause;
  FeatureClausePtr fcp, new_fcp;
  SeqFeatPtr sfp;

  for (clause = clause_list;
       clause != NULL;
       clause = clause->next)
  {
    if (clause->choice != DEFLINE_CLAUSEPLUS
      || (fcp = clause->data.ptrvalue) == NULL
      || fcp->featlist == NULL)
    {
      continue;
    }
    if ( fcp->featlist->choice == DEFLINE_FEATLIST
      && (sfp = fcp->featlist->data.ptrvalue) != NULL
      && IsExon (sfp)
      && fcp->featlist->next != NULL
      && fcp->featlist->next->choice == DEFLINE_FEATLIST
      && IsExon (fcp->featlist->next->data.ptrvalue))
    {
      rest_of_list = clause->next;
      clause->next = NULL;
      for (featlist = fcp->featlist->next;
           featlist != NULL
             && featlist->choice == DEFLINE_FEATLIST
             && IsExon (featlist->data.ptrvalue);
           featlist = featlist->next)
      {
        new_clause = ValNodeNew (clause);
        if (new_clause == NULL) return;
        new_fcp = NewFeatureClause (featlist->data.ptrvalue, bsp);
        if (new_fcp == NULL) return;
        new_fcp->grp = fcp->grp;
        new_fcp->is_alt_spliced = fcp->is_alt_spliced;
        new_fcp->make_plural = fcp->make_plural;
        new_clause->choice = DEFLINE_CLAUSEPLUS;
        new_clause->data.ptrvalue = new_fcp;
      }
      ValNodeFree (fcp->featlist->next);
      fcp->featlist->next = NULL;
      new_clause->next = rest_of_list;

      /* put back location for first exon - was reduced to union of 
       * all exon intervals in GroupAltSplicedExons
       */
      SeqLocFree (fcp->slp);
      sfp = fcp->featlist->data.ptrvalue;
      fcp->slp = SeqLocMerge (bsp, sfp->location, NULL, FALSE, TRUE, FALSE);
    }
    else
    {
      ExpandAltSplicedExons (fcp->featlist, bsp);
    }
  }
}

/* This function searches this list for clauses to which this gene should
 * apply.  This is not taken care of by the GroupAllClauses function
 * because genes are added to clauses as a GeneRefPtr instead of as an
 * additional feature in the list, and because a gene can apply to more
 * than one clause, while other features should really only belong to
 * one clause.
 */
static Boolean AddGeneToClauses (
  SeqFeatPtr gene,
  CharPtr    gene_productname,
  ValNodePtr clause_list
)
{
  ValNodePtr    clause;
  FeatureClausePtr fcp;
  SeqFeatPtr    sfp, found_gene;
  GeneRefPtr    grp;
  Boolean    used_gene;
  
  if (gene == NULL || gene->data.value.ptrvalue == NULL) return FALSE;
  if (clause_list == NULL) return FALSE;

  used_gene = FALSE;
  grp = gene->data.value.ptrvalue;
  for (clause = clause_list; clause != NULL; clause = clause->next)
  {
    fcp = clause->data.ptrvalue;
    if (fcp == NULL || fcp->featlist == NULL) return FALSE;
    sfp = fcp->featlist->data.ptrvalue;
    if (sfp != NULL && (IsCDS (sfp)
                        || IsrRNA (sfp)
                        || IstRNA (sfp)
                        || IsmRNA (sfp)
                        || IsMiscRNA (sfp)
                        || IssnRNA (sfp)
                        || IssnoRNA (sfp)
                        || IsNoncodingProductFeat (sfp)))
    {
      if (fcp->grp == NULL)
      {
        found_gene = SeqMgrGetOverlappingGene (sfp->location, NULL);
        if (found_gene != NULL)
        {
          fcp->grp = (GeneRefPtr) found_gene->data.value.ptrvalue;
        }
      }

      if (fcp->grp != NULL && DoGenesMatch (fcp->grp, grp))
      {
        used_gene = TRUE;
        if (gene_productname != NULL
          && fcp->feature_label_data.productname == NULL
          && IsCDS (sfp))
        {
          fcp->feature_label_data.productname =
              StringSave (gene_productname);
        }
      }
      else if (fcp->grp == NULL
        && IsLocAInBonSameStrand (sfp->location, gene->location))
      {
        fcp->grp = grp;
        used_gene = TRUE;
        if (gene_productname != NULL
          && fcp->feature_label_data.productname == NULL
          && IsCDS (sfp))
        {
          fcp->feature_label_data.productname =
              StringSave (gene_productname);
        }
      }
    }
  }
  return used_gene;
}

/* This function iterates through the list of features and calls
 * AddGeneToClauses for each gene feature it finds.
 */
static void GroupGenes (
  ValNodePtr PNTR clause_list
)
{
  ValNodePtr  vnp;
  ValNodePtr  featlist;
  FeatureClausePtr fcp;

  for (vnp = *clause_list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice != DEFLINE_CLAUSEPLUS) return;
    fcp = (FeatureClausePtr) vnp->data.ptrvalue;
    if (fcp == NULL) return;

    featlist = fcp->featlist;
    if (featlist != NULL
      && featlist->choice == DEFLINE_FEATLIST
      && IsGene (featlist->data.ptrvalue))
    {
      AddGeneToClauses (featlist->data.ptrvalue,
                        fcp->feature_label_data.productname,
                        vnp->next);
    }
  } 
}

/* This function searches this list for clauses to which this mRNA should
 * apply.  This is not taken care of by the GroupAllClauses function
 * because when an mRNA is added to a CDS, the product for the clause is
 * replaced and the location for the clause is expanded, rather than simply
 * adding the mRNA as an additional feature in the list, and because an 
 * mRNA can apply to more than one clause, while other features should 
 * really only belong to one clause.
 */
static Boolean AddmRNAToClauses (
  SeqFeatPtr mRNA,
  ValNodePtr clause_list,
  BioseqPtr bsp
)
{
  ValNodePtr    clause;
  FeatureClausePtr fcp;
  SeqFeatPtr    sfp;
  Boolean    used_mRNA;
  CharPtr       productname;
  SeqLocPtr     new_slp;
  
  if (mRNA == NULL || mRNA->data.value.ptrvalue == NULL) return FALSE;
  if (clause_list == NULL) return FALSE;

  used_mRNA = FALSE;
  productname = GetProductName (mRNA, bsp);
  if (productname == NULL) return TRUE;

  for (clause = clause_list; clause != NULL; clause = clause->next)
  {
    fcp = clause->data.ptrvalue;
    if (fcp == NULL || fcp->featlist == NULL) return FALSE;
    sfp = fcp->featlist->data.ptrvalue;
    if (sfp == NULL)
    {
    }
    else if (IsCDS (sfp)
      && fcp->feature_label_data.productname != NULL
      && StringCmp (fcp->feature_label_data.productname, productname) == 0)
    {
      used_mRNA = TRUE;
      if (IsLocAInBonSameStrand (sfp->location, mRNA->location))
      {
        new_slp = SeqLocMerge (bsp, fcp->slp, mRNA->location,
                                 FALSE, TRUE, FALSE);
        if (new_slp == NULL) return FALSE;
        if (fcp->slp != NULL)
        {
          SeqLocFree (fcp->slp);
        }
        fcp->slp = new_slp;
      }
    }
    else if (fcp->feature_label_data.productname == NULL
      && (IsCDS (sfp) || IsGene (sfp))
      && (IsLocAInBonSameStrand (sfp->location, mRNA->location)
        || IsLocAInBonSameStrand (mRNA->location, sfp->location)))
    {
      fcp->feature_label_data.productname = StringSave (productname);
      used_mRNA = TRUE;
      new_slp = SeqLocMerge (bsp, fcp->slp, mRNA->location,
                                 FALSE, TRUE, FALSE);
      if (new_slp == NULL) return FALSE;
      if (fcp->slp != NULL)
      {
        SeqLocFree (fcp->slp);
      }
      fcp->slp = new_slp;
    }
  }
  return used_mRNA;
}

/* This function iterates through the list of features and calls
 * AddmRNAToClauses for each mRNA feature it finds.
 */
static void GroupmRNAs (
  ValNodePtr PNTR clause_list,
  BioseqPtr  bsp
)
{
  ValNodePtr  vnp;
  ValNodePtr  featlist;
  FeatureClausePtr fcp;

  for (vnp = *clause_list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice != DEFLINE_CLAUSEPLUS) return;
    fcp = (FeatureClausePtr) vnp->data.ptrvalue;
    if (fcp == NULL) return;

    featlist = fcp->featlist;
    if (featlist != NULL
      && featlist->choice == DEFLINE_FEATLIST
      && IsmRNA (featlist->data.ptrvalue))
    {
      if (AddmRNAToClauses (featlist->data.ptrvalue, *clause_list, bsp))
      {
        fcp->delete_me = TRUE;
      }
    }
  } 
  DeleteFeatureClauses (clause_list);
}

/* This section of code contains functions for generating labels for
 * clauses for the definition lines.
 */

/* This function examines the specified typeword and determines whether it
 * should appear before or after the description of the feature in the
 * definition line.
 */
static Boolean IsTypeWordFirst (
  CharPtr typeword
)
{
  Int4 i;
  if (typeword == NULL) return FALSE;
  if (StringCmp (typeword, "exon") == 0
    || StringCmp (typeword, "intron") == 0
    || StringCmp (typeword, "transposon") == 0
    || StringCmp (typeword, "insertion sequence") == 0
    || StringCmp (typeword, "endogenous virus") == 0)
  {
    return TRUE;
  }
  else
  {
    for (i=0; i < sizeof (transposon_keywords) / sizeof (CharPtr); i++)
    {
      if (StringCmp (typeword, transposon_keywords[i]) == 0)
      {
        return TRUE;
      }
    }
    return FALSE;
  }
}

/* This function determines the word to use to indicate what type of feature
 * is being described in the definition line.  For certain feature types,
 * the word to use in the definition line varies based on the type of
 * molecule in the record.
 */
static CharPtr GetFeatureTypeWord (
  Uint1 biomol,
  SeqFeatPtr sfp
)
{
  if (sfp == NULL) return NULL;
  if ( IsExon (sfp))
  {
    return StringSave ("exon");
  } 
  else if(IsIntron (sfp))
  {
    return StringSave ("intron");
  }
  else if (IsInsertionSequence (sfp))
  {
    return StringSave ("insertion sequence");
  }
  else if (IsEndogenousVirusSequence (sfp))
  {
    return StringSave ("endogenous virus");
  }
  else if (IsControlRegion (sfp))
  {
    return StringSave ("control region");
  }
  else if (IsEndogenousVirusSourceFeature (sfp))
  {
    return StringSave ("endogenous virus");
  }
  else if (IsDloop (sfp))
  {
    return StringSave ("D-loop");
  }
  else if (IsLTR (sfp))
  {
    return StringSave ("LTR");
  }
  else if (Is3UTR (sfp))
  {
    return StringSave ("3' UTR");
  }
  else if (Is5UTR (sfp))
  {
    return StringSave ("5' UTR");
  }
  else if (IsOperon (sfp))
  {
    return StringSave ("operon");
  }
  else if (biomol == MOLECULE_TYPE_GENOMIC)
  {
    if (IsPseudo (sfp))
    {
      return StringSave ("pseudogene");
    }
    else
    {
      return StringSave ("gene");
    }
  }
  else if ( IsrRNA (sfp) || IssnoRNA (sfp) || IssnRNA (sfp))
  {
    return NULL;
  }
  else if (biomol == MOLECULE_TYPE_MRNA)
  {
    if (IsPseudo (sfp))
    {
      return StringSave ("pseudogene mRNA");
    }
    else
    {
      return StringSave ("mRNA");
    }
  }
  else if (biomol == MOLECULE_TYPE_PRE_MRNA)
  {
    if (IsPseudo (sfp))
    {
      return StringSave ("pseudogene precursor RNA");
    }
    else
    {
      return StringSave ("precursor RNA");
    }
  }
  else if (biomol == MOLECULE_TYPE_OTHER_GENETIC_MATERIAL)
  {
    return StringSave ("gene");
  }
  return StringSave ("");
}

/* Frequently the product associated with a feature is listed as part of the
 * description of the feature in the definition line.  This function determines
 * the name of the product associated with this specific feature.  Some
 * features will be listed with the product of a feature that is associated
 * with the feature being described - this function does not look at other
 * features to determine a product name.
 * If the feature is a misc_feat with particular keywords in the comment,
 * the product will be determined based on the contents of the comment.
 * If the feature is a CDS and is marked as pseudo, the product will be
 * determined based on the contents of the comment.
 * If the feature is a gene and has different strings in the description than
 * in the locus or locus tag, the description will be used as the product for
 * the gene.
 * If none of the above conditions apply, the sequence indexing context label
 * will be used to obtain the product name for the feature.
 */
static CharPtr GetProductName (
  SeqFeatPtr cds,
  BioseqPtr bsp
)
{
  CharPtr protein_name;
  CharPtr semicolon;
  size_t len_to_copy;
  SeqMgrFeatContext  context;
  GeneRefPtr  grp;
  CharPtr gene_name;

  if (cds == NULL) return NULL;
  protein_name = NULL;
  if (IsNoncodingProductFeat (cds))
  {
    return GetNoncodingProductFeatProduct (cds);
  }
  else if (cds->data.choice == SEQFEAT_CDREGION && cds->pseudo)
  {
    if (cds->comment != NULL)
    {
      semicolon = StringChr (cds->comment, ';');
      if (semicolon != NULL)
      {
        len_to_copy = semicolon - cds->comment;
      }
      else
      {
        len_to_copy = StringLen (cds->comment);
      }
      protein_name = MemNew (len_to_copy + 1);
      if (protein_name == NULL) return NULL;
      StringNCpy (protein_name, cds->comment, len_to_copy);
      protein_name[len_to_copy] = 0;
    }
    return protein_name;
  }
  else if (cds->data.choice == SEQFEAT_GENE)
  {
    grp = (GeneRefPtr) cds->data.value.ptrvalue;
    if (grp == NULL) return NULL;
    gene_name = GetGeneName (grp);
    if (grp->desc != NULL
      && StringCmp (grp->desc, gene_name) != 0)
    {
      return StringSave (grp->desc);
    }
    if (grp->locus_tag != NULL
      && StringCmp (grp->locus_tag, gene_name) != 0)
    {
      return StringSave (grp->locus_tag);
    }
  }
  else
  {
    if (SeqMgrGetDesiredFeature (0, bsp, 0, 0, cds, &context) == cds
      && context.label != NULL)
    {
      if ( IstRNA (cds) )
      {
        if (StringCmp (context.label, "Xxx") == 0) {
          protein_name = StringSave ("tRNA-OTHER");
        } else {
          protein_name = MemNew ( StringLen (context.label) + 6);
          if ( protein_name == NULL) return NULL;
          sprintf (protein_name, "tRNA-%s", context.label);
        }
        return protein_name;
      }
      else if ((IsCDS(cds) && StringCmp (context.label, "CDS") != 0)
  || (IsmRNA(cds) && StringCmp (context.label, "mRNA") != 0)
        || (! IsCDS(cds) && ! IsmRNA(cds)))
      {
        protein_name = StringSave (context.label);
        return protein_name;
      }
    }
  }
  return NULL;
}

/* This function searches a list of features recursively for a
 * feature that satisfies the itemmatch condition and is associated with
 * the same gene as the fcp clause passed to the function.
 * This is used to obtain a product for a feature that may share a gene with
 * a product-producing feature but may not be contained in the interval of
 * the product-producing feature.
 */
static FeatureClausePtr FindProductInFeatureList (
  FeatureClausePtr fcp,
  ValNodePtr       clause_list,
  matchFunction    itemmatch
)
{
  ValNodePtr       vnp;
  FeatureClausePtr vnp_fcp;
  
  for (vnp = clause_list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice == DEFLINE_CLAUSEPLUS && vnp->data.ptrvalue != NULL)
    {
      vnp_fcp = vnp->data.ptrvalue;
      if (DoGenesMatch (vnp_fcp->grp, fcp->grp)
        && vnp_fcp->featlist != NULL
        && vnp_fcp->featlist->choice == DEFLINE_FEATLIST
        && itemmatch (vnp_fcp->featlist->data.ptrvalue))
      {
        return vnp_fcp;
      }
      else
      {
        vnp_fcp = FindProductInFeatureList (fcp, vnp_fcp->featlist, itemmatch);
        if (vnp_fcp != NULL) return vnp_fcp;
      }
    }
  }
  return NULL;
}

/* This function uses the available information in the clause to generate
 * a description from the name of the gene (if any) and the name of the
 * product for the feature (if any).
 * If there is only a gene, the description will be the name of the gene.
 * If there is only a product, the description will be the name of the product.
 * If there is a gene and a product, the description will be the name of
 * the product followed by the name of the gene in parentheses.
 */
static CharPtr GetGeneProtDescription (
  FeatureClausePtr fcp,
  BioseqPtr        bsp
)
{
  SeqFeatPtr    sfp;
  CharPtr    protein_name;
  CharPtr    gene_name;
  size_t    description_length;
  CharPtr    description;

  if (fcp == NULL
    || fcp->featlist == NULL
    || fcp->featlist->data.ptrvalue == NULL)
  {
    return NULL;
  }
  sfp = fcp->featlist->data.ptrvalue;

  description_length = 0;

  if (fcp->feature_label_data.productname != NULL)
  {
    protein_name = StringSave (fcp->feature_label_data.productname);
  }
  else
  {
    protein_name = GetProductName (sfp, bsp);
    if (protein_name == NULL && IsGene (sfp))
    {
      
    }
  }
  if (protein_name != NULL)
  {
    description_length += StringLen (protein_name);
  }
     
  gene_name = GetGeneName (fcp->grp);
  if (gene_name != NULL)
  {
    description_length += StringLen (gene_name);
    if (protein_name != NULL)
    {
      description_length += 3;
    }
  }
  description = (CharPtr) MemNew (description_length + 1);
  if (description == NULL) return NULL;
  if (protein_name != NULL)
  {
    if (gene_name != NULL)
    {
      sprintf (description, "%s (%s)", protein_name, gene_name);
    }
    else
    {
      sprintf (description, "%s", protein_name);
    }
  }
  else
  {
    if (gene_name != NULL)
      sprintf (description, gene_name);
  }
  if (protein_name != NULL) MemFree (protein_name);
  return description;
}

/* This array of match functions is used to identify, in order of preference,
 * the features that might be used to generate a product for a gene-protein
 * description if the feature has not already been grouped with a product
 * feature.
 */
static matchFunction productfeatures[] = {
  IsCDS, IsmRNA, IstRNA
}; 

/* This function finds gene features without products and looks for 
 * features that might provide products for them.
 */
static void FindGeneProducts (
  ValNodePtr clause_list,
  BioseqPtr  bsp
)
{
  ValNodePtr       vnp;
  FeatureClausePtr fcp, productfcp;
  Int4             i, NumProductFeatureTypes;

  NumProductFeatureTypes = sizeof (productfeatures) / sizeof (matchFunction);

  for (vnp = clause_list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice == DEFLINE_CLAUSEPLUS
      && (fcp = vnp->data.ptrvalue) != NULL
      && fcp->featlist != NULL)
    {
      if (fcp->featlist->choice == DEFLINE_FEATLIST
        && IsGene (fcp->featlist->data.ptrvalue)
        && fcp->feature_label_data.productname == NULL)
      {
        productfcp = NULL;
        for (i=0; i < NumProductFeatureTypes && productfcp == NULL; i++)
        {
          productfcp = FindProductInFeatureList (fcp, clause_list,
                                                 productfeatures[i]);
        }
        if (productfcp != NULL)
        {
          fcp->is_alt_spliced = productfcp->is_alt_spliced;
          if (productfcp->feature_label_data.productname != NULL)
          {
            fcp->feature_label_data.productname =
                  StringSave (productfcp->feature_label_data.productname);
          }
          else
          {
            fcp->feature_label_data.productname
                  = GetProductName (productfcp->featlist->data.ptrvalue, bsp);
          }
          if (fcp->feature_label_data.description != NULL)
          {
            MemFree (fcp->feature_label_data.description);
            fcp->feature_label_data.description = NULL;
          }
          fcp->feature_label_data.description =
            GetGeneProtDescription (fcp, bsp);
        }
      }
      else
      {
        FindGeneProducts (fcp->featlist, bsp);
      }
    }
  }
}

static Boolean ShowInterval (
  SeqFeatPtr sfp
)
{
  if (IsSatelliteSequence (sfp) || IsExon (sfp) || IsIntron (sfp)
    || IsPromoter (sfp) || Is3UTR (sfp) || Is5UTR (sfp))
    return FALSE;
  return TRUE;
}

static CharPtr GetExonDescription (
  BioseqPtr bsp,
  SeqFeatPtr sfp
)
{
  SeqMgrFeatContext  context;
  SeqFeatPtr new_sfp;
  CharPtr    label;

  if ((new_sfp = SeqMgrGetDesiredFeature (sfp->idx.entityID, bsp, 0, 0, sfp, &context)) != sfp
      || context.label == NULL)
  {
    if ((new_sfp = SeqMgrGetDesiredFeature (0, bsp, 0, 0, sfp, &context)) != sfp
      || context.label == NULL)
    {
      return NULL;
    }
  }
  if ((IsExon (sfp) && StringCmp (context.label, "exon") == 0)
    || (IsIntron (sfp) && StringCmp (context.label, "intron") == 0))
  {
    return NULL;
  }
  
  label = StringSave (context.label);
  return label;
}

static CharPtr GetFeatureDescription (
  FeatureClausePtr fcp,
  BioseqPtr bsp
)
{
  SeqFeatPtr    sfp;

  if ( fcp == NULL
    || fcp->featlist == NULL
    || fcp->featlist->data.ptrvalue == NULL)
  {
    return NULL;
  }
  sfp = fcp->featlist->data.ptrvalue;
  if (sfp == NULL) return NULL;

  if (IsExon (sfp) || IsIntron (sfp))
  {
    return GetExonDescription (bsp, sfp);
  }
  else if (IsInsertionSequence (sfp))
  {
    return GetInsertionSequenceDescription (sfp);
  }
  else if (IsEndogenousVirusSequence (sfp))
  {
    return GetEndogenousVirusSequenceDescription (sfp);
  }
  else if (IsEndogenousVirusSourceFeature (sfp))
  {
    return GetEndogenousVirusSourceFeatureDescription (sfp);
  }
  else if (IsControlRegion (sfp))
  {
    return NULL;
  }
  else if (IsDloop (sfp))
  {
    return NULL;
  }
  else if (Is3UTR (sfp))
  {
    return NULL;
  }
  else if (Is5UTR (sfp))
  {
    return NULL;
  }
  else if (IsLTR (sfp))
  {
    return GetLTRDescription (sfp);
  }
  else
  {
    return GetGeneProtDescription (fcp, bsp);
  }
}

static void LIBCALLBACK GetSatelliteFeatureLabel (
  ValNodePtr      featlist,
  BioseqPtr       bsp,
  Uint1           biomol,
  FeatureLabelPtr flp
)
{
  SeqFeatPtr main_feat;
  CharPtr    semicolon;

  flp->description = NULL; 
  flp->typeword = StringSave ("sequence");
  flp->pluralizable = FALSE;
  flp->is_typeword_first = FALSE;
 
  if (featlist == NULL) return;
  main_feat = featlist->data.ptrvalue;
  if (main_feat == NULL) return;
  if (StringHasNoText (main_feat->comment)) return;

  flp->description = StringSave (main_feat->comment);
  if ((semicolon = StringStr (flp->description, ";")) != NULL)
  {
    *semicolon = 0;
  }
}

static void LIBCALLBACK GetPromoterFeatureLabel (
  ValNodePtr      featlist,
  BioseqPtr       bsp,
  Uint1           biomol,
  FeatureLabelPtr flp
)
{
  SeqFeatPtr main_feat;
  
  flp->description = NULL;
  flp->typeword = StringSave ("promoter region");

  if (featlist == NULL) return;
  main_feat = featlist->data.ptrvalue;
  if (main_feat == NULL) return;

  flp->description =  NULL;
  flp->pluralizable = FALSE;
  flp->is_typeword_first = FALSE;

}

/* This function calculates the "interval" for a clause in the definition
 * line.  The interval could be an empty string, it could indicate whether
 * the location of the feature is partial or complete and whether or not
 * the feature is a CDS, the interval could be a description of the
 * subfeatures of the clause, or the interval could be a combination of the
 * last two items if the feature is a CDS.
 */
static CharPtr GetGenericInterval (
  FeatureClausePtr fcp,
  Uint1 biomol,
  BioseqPtr bsp
)
{
  CharPtr interval;
  Boolean partial5, partial3;
  SeqFeatPtr sfp;
  ValNodePtr featlist, strings;
  CharPtr subfeatlist;
  Int4 len;
  Boolean suppress_final_and;

  if ( fcp == NULL || fcp->featlist == NULL) return NULL;
  if (fcp->is_unknown) return NULL;
  featlist = fcp->featlist;
  sfp = featlist->data.ptrvalue;
  if (sfp == NULL) return NULL;
  if ( IsExon (sfp) && fcp->is_alt_spliced)
  {
    return StringSave ("alternatively spliced");
  }
  if ( ! ShowInterval (sfp)) return NULL;

  subfeatlist = NULL;
  len = 50;
  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);

  strings = NULL;
  while (featlist != NULL && featlist->choice != DEFLINE_CLAUSEPLUS)
  {
    featlist = featlist->next;
  }
  if (featlist != NULL)
  {
    suppress_final_and = FALSE;
    if ( IsCDS (sfp) && ! fcp->clause_info_only)
    {
      suppress_final_and = TRUE;
    }
    LabelClauses (featlist, biomol, bsp);
    ListClauses (featlist, &strings, FALSE, suppress_final_and);
    subfeatlist = MergeValNodeStrings (strings, FALSE);
	ValNodeFreeData (strings);
    len += StringLen (subfeatlist) + 7;
  }
    

  interval = (CharPtr) MemNew (len * sizeof (Char));
  if (interval == NULL) return NULL;
  interval[0] = 0;

  if (StringDoesHaveText (subfeatlist))
  {
    StringCat (interval, subfeatlist);
    if ( ! IsCDS (sfp) || fcp->clause_info_only)
    {
      if (subfeatlist != NULL) MemFree (subfeatlist);
      return interval;
    }
    StringCat (interval, " and ");
  }

  if (subfeatlist != NULL) MemFree (subfeatlist);
    
  if (partial5 || partial3)
  {
    StringCat (interval, "partial ");
  }
  else
  {
    StringCat (interval, "complete ");
  }
  if (IsCDS (sfp))
  {
    StringCat (interval, "cds");
    if (fcp->is_alt_spliced)
      StringCat (interval, ", alternatively spliced");
  }
  else
  {
    StringCat (interval, "sequence");
    if (IsNoncodingProductFeat (sfp) && fcp->is_alt_spliced)
    {
      StringCat (interval, ", alternatively spliced");
    }
  }
  return interval;
} 

/* This function is used to generate feature label information for
 * a feature clause.  It is called by the LabelFeature function if
 * a "GetFeatureLabel" function is not found for the specific feature
 * type.
 * In the future it may be advisable to create "GetFeatureLabel" functions
 * for more of the specific feature types, to reduce the number of times
 * that the feature must be identified as being a certain type.
 */ 
static void LIBCALLBACK GetGenericFeatureLabel (
  FeatureClausePtr fcp,
  BioseqPtr       bsp,
  Uint1           biomol,
  FeatureLabelPtr flp
)
{
  SeqFeatPtr main_feat;
  
  if (fcp == NULL
    || fcp->featlist == NULL
    || fcp->featlist->data.ptrvalue == NULL)
  {
    return;
  }
  main_feat = fcp->featlist->data.ptrvalue;
  if (main_feat == NULL) return;

  if (flp->typeword == NULL)
  {
    flp->typeword = GetFeatureTypeWord (biomol, main_feat);
    flp->is_typeword_first = IsTypeWordFirst (flp->typeword);
    flp->pluralizable = TRUE;
  }
  if (flp->productname == NULL)
  {
    flp->productname = GetProductName (main_feat, bsp);
  }
  if (flp->description == NULL
    && (! IsMiscRNA (main_feat)
      || StringStr (flp->productname, "spacer") == NULL ))
  {
    flp->description = GetFeatureDescription (fcp, bsp);
  }

}

typedef void (LIBCALLBACK *GetFeatureLabelFunction) (
  ValNodePtr      featlist,
  BioseqPtr       bsp,
  Uint1           biomol,
  FeatureLabelPtr flp
);

typedef struct matchlabelfunction {
  matchFunction           itemmatch;
  GetFeatureLabelFunction labelfunction;
} MatchLabelFunctionData, PNTR MatchLabelFunctionPtr;

static MatchLabelFunctionData label_functions[] = {
 { IsSatelliteSequence, GetSatelliteFeatureLabel         },
 { IsTransposon,        GetTransposonFeatureLabel        },
 { IsPromoter,          GetPromoterFeatureLabel          },
 { IsIntergenicSpacer,  GetIntergenicSpacerFeatureLabel  }
};

typedef enum {
 DEFLINE_FEATLABEL_Satellite = 0,
 DEFLINE_FEATLABEL_Transposon,
 DEFLINE_FEATLABEL_Promoter,
 DEFLINE_FEATLABEL_IntergenicSpacer,
 NumDefLineFeatLabels
} DefLineFeatLabel;

static void LabelFeature (
  BioseqPtr       bsp,
  Uint1           biomol,
  FeatureClausePtr new_clauseplus
)
{
  Int4             i;
  SeqFeatPtr       main_feat;

  if (new_clauseplus == NULL || new_clauseplus->featlist == NULL) return;

  if (new_clauseplus->featlist->choice == DEFLINE_FEATLIST)
  {
    main_feat = (SeqFeatPtr) new_clauseplus->featlist->data.ptrvalue;
    
    new_clauseplus->allelename = GetAlleleName (new_clauseplus->grp);
    if (new_clauseplus->interval == NULL)
    {
      new_clauseplus->interval =
                  GetGenericInterval (new_clauseplus, biomol, bsp);
    }

    for (i=0; i < NumDefLineFeatLabels; i++)
    {
      if (label_functions [i].itemmatch (main_feat))
      {
        label_functions [i].labelfunction ( new_clauseplus->featlist,
                                          bsp, biomol,
                                          &new_clauseplus->feature_label_data);
        return;
      }
    }

    GetGenericFeatureLabel ( new_clauseplus, bsp, biomol, 
                           &new_clauseplus->feature_label_data);
    return;
  }
}

/* This function is used to calculate the parts of a product name that
 * are "the same" for use as the name of an alternatively spliced product.
 * The common portion of the string must end at a recognized separator,
 * such as a space, comma, or dash instead of in the middle of a word.
 * The matching portions of the string could occur at the beginning or end
 * of the string, or even occasionally at the beginning and end of a
 * string, but not as the center of the string with a different beginning
 * and ending.
 */
static CharPtr FindStringIntersection (
  CharPtr str1,
  CharPtr str2,
  Boolean str1_previously_stripped
)
{
  Int4 matchleftlen = 0;
  Int4 matchlefttoken = 0;
  Int4 matchrightidx1 = 0;
  Int4 matchrightidx2 = 0;
  Int4 matchrighttoken = 0;
  CharPtr match_string;
  Int4 len1;
  Int4 len2;
  Int4 match_len;

  if (str1 == NULL || str2 == NULL) return NULL;
  if (StringCmp (str1, str2) == 0) return StringSave (str1);
  len1 = StringLen (str1);
  len2 = StringLen (str2);

  while (str1[matchleftlen] != 0 && str2[matchleftlen] != 0
         && str1[matchleftlen] == str2[matchleftlen])
  {
    if (str1 [matchleftlen] == ','
      || str1 [matchleftlen] == '-')
    {
      matchlefttoken = matchleftlen;
    }
    else if (str1 [matchleftlen] == ' '
      && matchlefttoken != matchleftlen - 1)
    {
      matchlefttoken = matchleftlen;
    }
    matchleftlen++;
  }
  if (matchleftlen == len1 && str1_previously_stripped) 
  {
    matchlefttoken = matchleftlen;
  }
  else
  {
    matchleftlen = matchlefttoken;
  }

  matchrightidx1 = len1;
  matchrightidx2 = len2;

  while (matchrightidx1 > -1 && matchrightidx2 > -1
         && str1[matchrightidx1] == str2[matchrightidx2])
  {
    if (str1 [matchrightidx1] == ' '
      || str1[matchrightidx1] == ','
      || str1[matchrightidx1] == '-')
    {
      matchrighttoken = matchrightidx1;
    }
    matchrightidx1--;
    matchrightidx2--;
  }
  if (matchrightidx1 == -1)
  {
    matchrighttoken = matchrightidx1;
  }
  else if (matchrighttoken > 0 
    || (str1_previously_stripped && matchrightidx1 < len1 - 1))
  {
    matchrightidx1 = matchrighttoken;
  }
  else
  {
    matchrightidx1 = len1;
  }

  match_len = matchleftlen;
  if (matchrightidx1 < len1 - 1)
  {
    match_len += len1 - matchrightidx1 - 1;
  }

  if (match_len <= 0) return NULL;

  match_string = MemNew (match_len + 2);
  if (match_string == NULL) return NULL;
  if (matchleftlen != 0)
  {
    StringNCpy (match_string, str1, matchleftlen);
    match_string[matchleftlen] = 0;
  }
  else
  {
    match_string[0] = 0;
  }
  if (matchrightidx1 < len1)
  {
    if (match_string[0] != 0) StringCat (match_string, " ");
    StringCat (match_string, str1 + matchrightidx1 + 1);
  }
  return match_string;
}

/* These are the words that are used to introduced the part of the protein
 * name that differs in alt-spliced products - they should not be part of
 * the alt-spliced product name.
 * Note that splice variant is listed before "variant" so that it will be
 * found first and "variant" will not be removed from "splice variant", leaving
 * splice as an orphan.
 */

static CharPtr UnwantedWords [] = {
 "splice variant",
 "splice product",
 "variant",
 "isoform"
};

static void TrimUnwantedWordsFromAltSpliceProductName (
  CharPtr productname
)
{
  Int4    num_unwanted_words, i;
  size_t  unwanted_word_len;
  CharPtr cp, tmp;
  
  num_unwanted_words = sizeof (UnwantedWords) / sizeof (CharPtr);
  for (i = 0; i < num_unwanted_words; i++)
  {
    unwanted_word_len = StringLen (UnwantedWords [i]);
    cp = StringStr (productname, UnwantedWords [i]);
    if (cp != NULL)
    {
      if (cp == productname)
      {
        /* word occurs in beginning of phrase */
        tmp = StringSave (productname + unwanted_word_len);
        StringCpy (productname, tmp);
        MemFree (tmp);
      }
      else if (cp - productname < StringLen (productname) - unwanted_word_len)
      {
        /* word occurs in middle of phrase */
        tmp = StringSave (cp + unwanted_word_len);
        StringCpy (cp - 1, tmp);
        MemFree (tmp);
      }
      else
      {
        /* word occurs at end of phrase */
        *cp = 0;
      }
    }
  }
}

/* This function determines whether two CDSs meet the conditions for
 * alternative splicing, and if so, it returns the name of the alternatively
 * spliced product.  In order to be alternatively spliced, the two CDSs 
 * must have the same gene, must share a complete interval, and must have
 * similarly named products.
 */
static CharPtr MeetAltSpliceRules (
  FeatureClausePtr cdsfcp1,
  FeatureClausePtr cdsfcp2,
  BioseqPtr bsp
)
{
  SeqFeatPtr cds1, cds2;
  CharPtr match_string;
  Int4    res;
  
  if (cdsfcp1 == NULL || cdsfcp2 == NULL
    || cdsfcp1->featlist == NULL || cdsfcp2->featlist == NULL)
  {
    return NULL;
  }

  cds1 = cdsfcp1->featlist->data.ptrvalue;
  cds2 = cdsfcp2->featlist->data.ptrvalue;
  if (! DoGenesMatch (cdsfcp1->grp, cdsfcp2->grp))
    return NULL;

  if ( (res = TestFeatOverlap (cds1, cds2, COMMON_INTERVAL)) != -1)
  {
    match_string = FindStringIntersection (
                     cdsfcp1->feature_label_data.productname,
                     cdsfcp2->feature_label_data.productname,
                     cdsfcp1->is_alt_spliced);
    return match_string;
  }
  return NULL;
}

/* This function is used by the FindAltSplices function to locate the
 * next CDS in a list of feature clauses.
 */
static ValNodePtr FindNextCDSClause (ValNodePtr vnp)
{
  FeatureClausePtr fcp;

  while (vnp != NULL)
  {
    if (vnp->choice == DEFLINE_CLAUSEPLUS)
    {
      fcp = vnp->data.ptrvalue;
      if (fcp != NULL && !fcp->delete_me && fcp->featlist != NULL
        && IsCDS (fcp->featlist->data.ptrvalue))
      {
        return vnp;
      }
    }
    vnp = vnp->next;
  }
  return NULL;
}

/* This function is used by the FindAltSplices function to move the features
 * and subclauses from the second CDS in an alternatively spliced pair of 
 * CDSs to the feature clause for the first CDS, so that the subfeatures
 * can be properly listed.
 */
static void MoveSubclauses (
  FeatureClausePtr dstfcp,
  FeatureClausePtr srcfcp
)
{
  ValNodePtr dst_last_feat, dst_first_clause, dst_last_clause;
  ValNodePtr src_last_feat, src_first_clause;

  if (dstfcp == NULL || srcfcp == NULL || srcfcp->featlist == NULL) return;

  dst_first_clause = NULL;
  dst_last_clause = NULL;
  src_first_clause = NULL;

  dst_last_feat = dstfcp->featlist;
  while (dst_last_feat != NULL 
      && dst_last_feat->next != NULL
      && dst_last_feat->next->choice == DEFLINE_FEATLIST)
  {
    dst_last_feat = dst_last_feat->next;
  }
  if (dst_last_feat != NULL)
  {
    dst_first_clause = dst_last_feat->next;
  }
  dst_last_clause = dst_first_clause;
  while (dst_last_clause != NULL && dst_last_clause->next != NULL)
  {
    dst_last_clause = dst_last_clause->next;
  }

  src_last_feat = srcfcp->featlist;
  while (src_last_feat != NULL 
      && src_last_feat->next != NULL
      && src_last_feat->next->choice == DEFLINE_FEATLIST)
  {
    src_last_feat = src_last_feat->next;
  }
  if (src_last_feat != NULL)
  {
    src_first_clause = src_last_feat->next;
  }

  /* insert features before clauses */
  if (dst_last_feat == NULL)
  {
    dstfcp->featlist = srcfcp->featlist;
    dst_last_feat = src_last_feat;
  }
  else
  {
    dst_last_feat->next = srcfcp->featlist;
  }
  /* insert clauses after feats */
  if (dst_first_clause != NULL)
  {
    src_last_feat->next = dst_first_clause;
    dst_last_clause->next = src_first_clause;
  }
  srcfcp->featlist = NULL;
}
 
/* we want to look through the list for CDS features */
/* if we find two CDSs that are alternatively spliced, */
/* we replace the first alternatively spliced CDS feature */
/* with a new CDS feature that has the new protein name as */
/* a comment and a data.choice value that indicates alt splicing */
/* we remove the second alternatively spliced CDS feature from the list */

static void FindAltSplices (
  ValNodePtr clause_list,
  BioseqPtr bsp
)
{
  FeatureClausePtr  fcp1, fcp2;
  ValNodePtr cdsclause1, cdsclause2;
  ValNodePtr searchclause;
  CharPtr  combined_protein_name;
  Boolean    partial3_1, partial5_1, partial3_2, partial5_2;
  Int4       left1, left2, right1, right2;

  if (clause_list == NULL) return;
  
  cdsclause1 = FindNextCDSClause (clause_list);
  while (cdsclause1 != NULL)
  {
    fcp1 = (FeatureClausePtr) cdsclause1->data.ptrvalue;
    if (fcp1->feature_label_data.productname == NULL)
    {
      fcp1->feature_label_data.productname = 
           GetProductName (fcp1->featlist->data.ptrvalue, bsp);
    }
    searchclause = cdsclause1->next;
    cdsclause2 = FindNextCDSClause (searchclause);
    while (cdsclause2 != NULL) 
    {
      fcp2 = (FeatureClausePtr) cdsclause2->data.ptrvalue;
      if (fcp2->feature_label_data.productname == NULL)
      {
        fcp2->feature_label_data.productname =
           GetProductName (fcp2->featlist->data.ptrvalue, bsp);
      }
      combined_protein_name = MeetAltSpliceRules (fcp1, fcp2, bsp);
      if (combined_protein_name != NULL)
      {
        /* get rid of variant, splice variant, splice product, isoform, etc.*/
        TrimUnwantedWordsFromAltSpliceProductName (combined_protein_name);

        /* get rid of trailing spaces in protein name */
        TrimSpacesAroundString (combined_protein_name);

        /* copy new protein name into first clause */
        MemFree (fcp1->feature_label_data.productname);
        fcp1->feature_label_data.productname = combined_protein_name;
        CheckSeqLocForPartial (fcp1->slp, &partial5_1, &partial3_1);
        left1 = GetOffsetInBioseq (fcp1->slp, bsp, SEQLOC_LEFT_END);
        right1 = GetOffsetInBioseq (fcp1->slp, bsp, SEQLOC_RIGHT_END);
        CheckSeqLocForPartial (fcp2->slp, &partial5_2, &partial3_2);
        left2 = GetOffsetInBioseq (fcp2->slp, bsp, SEQLOC_LEFT_END);
        right2 = GetOffsetInBioseq (fcp2->slp, bsp, SEQLOC_RIGHT_END);
        fcp1->slp = SeqLocMerge (bsp, fcp1->slp, fcp2->slp,
                                 FALSE, TRUE, FALSE);
        if (left1 == left2)
        {
          partial5_1 |= partial5_2;
        }
        else
        {
          partial5_1 = left1 < left2 ? partial5_1 : partial5_2;
        }
        if (right1 == right2)
        {
          partial3_1 |= partial3_2;
        }
        else
        {
          partial3_1 = right1 > right2 ? partial3_1 : partial3_2;
        }
        SetSeqLocPartial (fcp1->slp, partial5_1, partial3_1);
        fcp1->is_alt_spliced = TRUE;

        /* copy over fcp2 subclauses */
        MoveSubclauses (fcp1, fcp2);

        /* remove second clause */
        fcp2->delete_me = TRUE;
      }
      searchclause = cdsclause2->next;
      cdsclause2 = FindNextCDSClause (searchclause);
    }
    cdsclause1 = FindNextCDSClause (cdsclause1->next);
  } 
  DeleteFeatureClauses (&clause_list);
}

static void LabelClauses (
  ValNodePtr clause_list,
  Uint1 biomol,
  BioseqPtr bsp
)
{
  ValNodePtr clause;
 
  clause = clause_list;
  while (clause != NULL)
  { 
    LabelFeature ( bsp, biomol, clause->data.ptrvalue);
    clause = clause->next;
  }
}

static CharPtr misc_words [] = {
  "internal transcribed spacer",
  "external transcribed spacer",
  "ribosomal RNA intergenic spacer",
  "ribosomal RNA",
  "intergenic spacer"
};

typedef enum {
  MISC_RNA_WORD_INTERNAL_SPACER = 0,
  MISC_RNA_WORD_EXTERNAL_SPACER,
  MISC_RNA_WORD_RNA_INTERGENIC_SPACER,
  MISC_RNA_WORD_RNA,
  MISC_RNA_WORD_INTERGENIC_SPACER,
  NUM_MISC_RNA_WORDS
} MiscWord;

static CharPtr separators [] = {
  ", and ",
  " and ",
  ", ",
  "; "
};

#define num_separators 3

static ValNodePtr GetMiscRNAelements (
  SeqFeatPtr misc_rna,
  BioseqPtr  bsp
)
{
  CharPtr buffer;
  Int4    i, best_i;
  CharPtr cansep [num_separators];
  CharPtr dataval;
  Int4    datalen;
  FeatureClausePtr fcp;
  FeatureClausePtr lastfcp;
  Int4    num_clauses;
  ValNodePtr head, vnp;
  Boolean partial5, partial3;
  Int4    word_i;
  CharPtr to_free;
  CharPtr word_loc;

  to_free = NULL;

  if (misc_rna == NULL) return NULL;
  buffer = GetProductName (misc_rna, bsp);
  to_free = buffer;
  if (buffer == NULL) 
  {
    buffer = misc_rna->comment;
  }
  else if (StringNCmp (buffer, misc_rna->comment, StringLen (buffer) -1) == 0
    && buffer [ StringLen (buffer) - 1] == '>')
  {
    MemFree (buffer);
    to_free = NULL;
    buffer = misc_rna->comment;
  }

  if ( StringStr (buffer, "spacer") == NULL)
  {
    if (to_free != NULL)
    {
      MemFree (to_free);
    }
    return NULL;
  }
  
  if (StringNCmp (buffer, "contains ", 9) == 0)
  {
    buffer = buffer + 9;
  }

  head = NULL;
  CheckSeqLocForPartial (misc_rna->location, &partial5, &partial3);
  num_clauses = 0;

  while (buffer != NULL 
         && buffer[0] != 0)
  {
    best_i = -1;
    for (i=0; i < num_separators; i++)
    {
      cansep [i] = StringStr (buffer, separators [i]);
      if (cansep [i] != NULL 
        && (best_i == -1 
          || cansep [i] < cansep [best_i]))
      {
        best_i = i;
      }
    }

    if (best_i == -1)
    {
      datalen = StringLen (buffer);
    }
    else
    {
      datalen = cansep [best_i] - buffer;
    }

    dataval = MemNew (datalen + 1);
    if (dataval == NULL) return NULL;
    StringNCpy (dataval, buffer, datalen);
    dataval [datalen] = 0;

    for (word_i=0;
         word_i < NUM_MISC_RNA_WORDS
           && (word_loc = StringStr (dataval, misc_words [word_i])) == NULL;
         word_i++) {}
    if (word_i < NUM_MISC_RNA_WORDS)
    {
      fcp = NewFeatureClause ( misc_rna, bsp);
      if (fcp == NULL) return NULL;
      if (word_i == MISC_RNA_WORD_INTERNAL_SPACER
        || word_i == MISC_RNA_WORD_EXTERNAL_SPACER
        || word_i == MISC_RNA_WORD_RNA_INTERGENIC_SPACER
        || word_i == MISC_RNA_WORD_INTERGENIC_SPACER)
      {
        if (word_loc == dataval)
        {
          fcp->feature_label_data.is_typeword_first = TRUE;
          fcp->feature_label_data.typeword = StringSave (misc_words [word_i]);
          if (StringLen (misc_words [word_i]) + 1 < StringLen (dataval))
          {
            fcp->feature_label_data.description =
                  StringSave ( dataval + StringLen (misc_words [word_i]) + 1);
          }
        }
        else
        {
          fcp->feature_label_data.is_typeword_first = FALSE;
          fcp->feature_label_data.typeword = StringSave (misc_words [word_i]);
          if (StringLen (misc_words [word_i]) + 1 < StringLen (dataval))
          {
            fcp->feature_label_data.description =
                  StringSave ( dataval);
            fcp->feature_label_data.description [word_loc - dataval - 1] = 0;
          }
        }
        MemFree (dataval);
      }
      else if (word_i == MISC_RNA_WORD_RNA)
      {
        fcp->feature_label_data.description = dataval;
      }
      if (num_clauses == 0 && partial5)
      {
        fcp->interval = StringSave ("partial sequence");
      }
      else
      {
        fcp->interval = StringSave ("complete sequence");
      }
   
      num_clauses ++;
      lastfcp = fcp;
      
      vnp = ValNodeNew (head);
      if (head == NULL) head = vnp;
      if (vnp == NULL) return NULL;
      vnp->data.ptrvalue = fcp;
      vnp->choice = DEFLINE_CLAUSEPLUS;
    }
    else
    {
      MemFree (dataval);
    }
    buffer += datalen;
    if (best_i != -1)
    {
      buffer += StringLen ( separators [best_i]);
    }
  }

  if ((num_clauses == 1 && partial3 && ! partial5)
    || (num_clauses > 1 && partial3))
  {
    MemFree (lastfcp->interval);
    lastfcp->interval = StringSave ("partial sequence");
  }
  if (to_free != NULL) MemFree (to_free);
  return head;
}

/* Some misc_RNA clauses have a comment that actually lists multiple
 * features.  This function creates a clause for each element in the
 * comment and inserts the list of new clauses into the feature list
 * at the point where the single previous clause was.
 */
static void ReplaceRNAClauses (
  ValNodePtr PNTR clause_list,
  BioseqPtr       bsp
)
{
  FeatureClausePtr fcp;
  SeqFeatPtr main_feat;
  ValNodePtr clause, replacement_clauses, nextclause, vnp;

  if (clause_list == NULL || *clause_list == NULL) return;
  clause = *clause_list;
  while (clause != NULL)
  {
    nextclause = clause->next;
    fcp = (clause->data.ptrvalue);
    if (fcp == NULL
      || fcp->featlist == NULL
      || fcp->featlist->choice != DEFLINE_FEATLIST)
    {
      return;
    }
    main_feat = (SeqFeatPtr) fcp->featlist->data.ptrvalue;
  
    if (IsrRNA (main_feat) || IsMiscRNA (main_feat))
    {
      replacement_clauses = GetMiscRNAelements ( main_feat, bsp );
      if (replacement_clauses != NULL)
      {
        for (vnp = replacement_clauses; vnp->next != NULL; vnp = vnp->next) {}
        vnp->next = clause->next;
        clause->next = replacement_clauses;
        fcp->delete_me = TRUE;
      }
    }
    clause = nextclause;
  }
  DeleteFeatureClauses (clause_list);
}

/* If we are applying a different rule for misc_feats, we need to recalculate
 * their descriptions.
 */
static void RenameMiscFeats (ValNodePtr clause_list, Uint1 biomol)
{
  ValNodePtr       vnp, featlist;
  FeatureClausePtr fcp, featlistclause;
  SeqFeatPtr       sfp;
  Int4             name_len;

  for (vnp = clause_list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice != DEFLINE_CLAUSEPLUS || vnp->data.ptrvalue == NULL)
    {
      continue;
    }
    fcp = vnp->data.ptrvalue;
    for (featlist = fcp->featlist; featlist != NULL; featlist = featlist->next)
    {
      if ( featlist->data.ptrvalue == NULL)
      {
        continue;
      }
      if (featlist->choice == DEFLINE_CLAUSEPLUS)
      {
        featlistclause = featlist->data.ptrvalue;
        RenameMiscFeats (featlistclause->featlist, biomol);
        continue;
      }
      if (featlist->choice != DEFLINE_FEATLIST)
      {
        continue;
      }
      sfp = featlist->data.ptrvalue;
      if (sfp->idx.subtype != FEATDEF_misc_feature
        || sfp->comment == NULL
        || IsIntergenicSpacer (sfp)
        || IsControlRegion (sfp)) 
      {
        continue;
      }
      if (fcp->feature_label_data.description != NULL) 
      {
        fcp->feature_label_data.description
                   = MemFree (fcp->feature_label_data.description);
      }
      name_len = StringCSpn (sfp->comment, ";");
      fcp->feature_label_data.description = MemNew (name_len * sizeof (Char));
      if (fcp->feature_label_data.description == NULL) return;
      StringNCpy (fcp->feature_label_data.description, sfp->comment, name_len);
      fcp->feature_label_data.description [ name_len ] = 0;
      fcp->feature_label_data.typeword =
            MemFree (fcp->feature_label_data.typeword);
      if (biomol == MOLECULE_TYPE_GENOMIC)
      {
        fcp->feature_label_data.typeword = StringSave ("genomic sequence");
      }
      else if (biomol == MOLECULE_TYPE_MRNA)
      {
        fcp->feature_label_data.typeword = StringSave ("mRNA sequence");
      }
      else
      {
        fcp->feature_label_data.typeword = StringSave ("sequence");
      }
     
      fcp->interval = MemFree (fcp->interval);
      fcp->interval = StringSave ("");
    }
  }
}

static void RemoveUnwantedMiscFeats (
  ValNodePtr PNTR clause_list,
  Boolean delete_now
)
{
  ValNodePtr       vnp, featlist;
  FeatureClausePtr fcp, featlistclause;
  SeqFeatPtr       sfp;

  for (vnp = *clause_list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice != DEFLINE_CLAUSEPLUS || vnp->data.ptrvalue == NULL)
    {
      continue;
    }
    fcp = vnp->data.ptrvalue;
    for (featlist = fcp->featlist; featlist != NULL; featlist = featlist->next)
    {
      if ( featlist->data.ptrvalue == NULL)
      {
        continue;
      }
      if (featlist->choice == DEFLINE_CLAUSEPLUS)
      {
        featlistclause = featlist->data.ptrvalue;
        RemoveUnwantedMiscFeats (&(featlistclause->featlist), FALSE);
        continue;
      }
      if (featlist->choice != DEFLINE_FEATLIST)
      {
        continue;
      }
      sfp = featlist->data.ptrvalue;
      if ( sfp->idx.subtype == FEATDEF_misc_feature
        && ! IsNoncodingProductFeat (sfp)
        && ! IsControlRegion (sfp)
        && ! IsIntergenicSpacer (sfp))
      {
        fcp->delete_me = TRUE;
      }
    }
  }
  DeleteFeatureClauses (clause_list);
}

/* When a feature is on the minus strand, the clauses are listed by 
 * sequence indexing in reverse biological order - we reverse the subclauses
 * for the feature in order to have them listed in the definition line
 * in biological order.
 * This is most noticeable when the main feature is a CDS with multiple
 * exons numbered sequentially.  If the exons are on the minus strand and
 * appear as 9, 8, 7, 6, we want to display them in the definition line as
 * 6, 7, 8, 9.
 */
static void ReverseClauses (
  ValNodePtr PNTR clause_list,
  matchFunction   itemmatch
)
{
  ValNodePtr vnp, last_feat, first_feat, next_item, new_list;
  FeatureClausePtr fcp;

  if (clause_list == NULL || *clause_list == NULL) return;

  last_feat = NULL;
  first_feat = NULL;
  new_list = NULL;
  vnp = *clause_list;
  while (vnp != NULL)
  {
    next_item = vnp->next;
    fcp = NULL;
    if (vnp->choice == DEFLINE_CLAUSEPLUS
      && (fcp = vnp->data.ptrvalue) != NULL
      && fcp->slp != NULL
      && SeqLocStrand (fcp->slp) == Seq_strand_minus
      && fcp->featlist != NULL
      && fcp->featlist->choice == DEFLINE_FEATLIST
      && itemmatch (fcp->featlist->data.ptrvalue))
    {
      vnp->next = new_list;
      new_list = vnp;
    }
    else
    {
      if (first_feat == NULL)
      {
        first_feat = vnp;
        last_feat = vnp;
      }
      else
      {
        last_feat->next = vnp;
        last_feat = vnp;
        last_feat->next = NULL;
      }
    }
    if (fcp != NULL)
    {
      ReverseClauses (&(fcp->featlist), itemmatch);
    }
    vnp = next_item;
  }
  if (first_feat == NULL)
  {
    *clause_list = new_list;
  }
  else
  {
    last_feat->next = new_list;
    *clause_list = first_feat;
  }
}

/* This function is used to determine whether two features are both exons
 * and whether they are numerically sequential - i.e., exon 7 and exon 8
 * are a pair of consecutive exons, exon 7 and exon 9 are not, and exon 7
 * and intron 9 are not.
 */
static Boolean ClausePairIsTwoConsecutiveExons (
  ValNodePtr vnp1,
  ValNodePtr vnp2,
  BioseqPtr  bsp
)
{
  FeatureClausePtr fcp1, fcp2;
  SeqFeatPtr       exon1, exon2;
  Int4 num1, num2;
  CharPtr          exdesc1, exdesc2;

  if (vnp1 == NULL || vnp2 == NULL 
    || vnp1->choice != DEFLINE_CLAUSEPLUS
    || vnp2->choice != DEFLINE_CLAUSEPLUS
    || vnp1->data.ptrvalue == NULL
    || vnp2->data.ptrvalue == NULL)
  {
    return FALSE;
  }
  fcp1 = vnp1->data.ptrvalue;
  fcp2 = vnp2->data.ptrvalue;
  if ( fcp1->featlist == NULL
    || fcp1->featlist->data.ptrvalue == NULL
    || fcp2->featlist == NULL
    || fcp2->featlist->data.ptrvalue == NULL
    || fcp1->featlist->choice != DEFLINE_FEATLIST
    || fcp2->featlist->choice != DEFLINE_FEATLIST
    || ! IsExon (fcp1->featlist->data.ptrvalue)
    || ! IsExon (fcp2->featlist->data.ptrvalue)
    || (fcp1->is_alt_spliced && ! fcp2->is_alt_spliced)
    || (! fcp1->is_alt_spliced && fcp2->is_alt_spliced))
  {
    return FALSE;
  }

  exon1 = (SeqFeatPtr)(fcp1->featlist->data.ptrvalue);
  exon2 = (SeqFeatPtr)(fcp2->featlist->data.ptrvalue);

  exdesc1 = GetExonDescription (bsp, exon1);
  exdesc2 = GetExonDescription (bsp, exon2);
  if (exdesc1 == NULL || exdesc2 == NULL)
  {
    if (exdesc1 != NULL) MemFree (exdesc1);
    if (exdesc2 != NULL) MemFree (exdesc2);
    return FALSE;
  }
  
  num1 = atoi (exdesc1);
  num2 = atoi (exdesc2);
  MemFree (exdesc1);
  MemFree (exdesc2);

  if (abs (num1 - num2) == 1)
  {
    return TRUE;
  }

  return FALSE; 
}

/* This function counts the number of consecutive exons in a list.
 */
static Int4 GetNumberOfConsecutiveExons (
  ValNodePtr list,
  BioseqPtr  bsp
)
{
  ValNodePtr check;
  Int4       num_exons;
 
  num_exons = 0;
  check = list->next;
  if ( ! ClausePairIsTwoConsecutiveExons (list, check, bsp)) return 0;
  
  num_exons = 2;
  while ( check != NULL
    && ClausePairIsTwoConsecutiveExons (check, check->next, bsp))
  {
    num_exons++;
    check = check->next;
  }
  return num_exons;
}

/* This function replaces a list of three or more consecutive exon clauses
 * with a single "summary" clause that gives the range of exons present -
 * i.e., if you have exons 1, 2, 3, and 4, a clause will be created that
 * contains all four of those features and has a description of "1 through 4".
 */
static void ReplaceExonClauseList (
  FeatureClausePtr fcp,
  ValNodePtr       clause,
  Int4             num_exons,
  BioseqPtr        bsp
)
{
  ValNodePtr       lastfeat, tmpclause;
  FeatureClausePtr tmpfcp;
  Int4             i;
  CharPtr          new_description;
  Int4             new_description_len;
  CharPtr          exdesc1, exdesc2;

  if (fcp == NULL || clause == NULL) return;

  lastfeat = fcp->featlist;
  while (lastfeat != NULL && lastfeat->next != NULL)
  {
    lastfeat = lastfeat->next;
  }
  tmpclause = clause->next;
  for (i=0; i < num_exons - 1 && tmpclause != NULL; i++)
  {
    tmpfcp = tmpclause->data.ptrvalue;
    tmpfcp->delete_me = TRUE;
    if (lastfeat == NULL)
    {
      fcp->featlist = tmpfcp->featlist;
    }
    else
    {
      lastfeat->next = tmpfcp->featlist;
    }
    tmpfcp->featlist = NULL;
    while (lastfeat != NULL && lastfeat->next != NULL)
    {
      lastfeat = lastfeat->next;
    }
          
    tmpclause = tmpclause->next;
  }

  exdesc1 = GetExonDescription (bsp, fcp->featlist->data.ptrvalue);
  exdesc2 = GetExonDescription (bsp, lastfeat->data.ptrvalue);
  if (exdesc1 == NULL || exdesc2 == NULL)
  {
    if (exdesc1 != NULL) MemFree (exdesc1);
    if (exdesc2 != NULL) MemFree (exdesc2);
    return;
  }
  new_description_len =
        StringLen (exdesc1)
      + StringLen (exdesc2)
      + StringLen (" through ")
      + 1;
  new_description = MemNew (new_description_len * sizeof (Char));
  if (new_description == NULL) return;
  sprintf (new_description, "%s through %s", exdesc1, exdesc2);
  MemFree (exdesc1);
  MemFree (exdesc2);
  if (fcp->feature_label_data.description != NULL)
  {
    MemFree (fcp->feature_label_data.description);
  }
  fcp->feature_label_data.description = new_description;
}

/* This function recursively searches for lists of consecutive exons
 * and calls ReplaceExonClauseList to consolidate the exons into a list
 * clause.
 */
static void RenameExonSequences (
  ValNodePtr PNTR list,
  BioseqPtr       bsp,
  Boolean         delete_now
)
{
  ValNodePtr       clause;
  Int4             num_exons;
  FeatureClausePtr fcp;
 
  if (list == NULL) return; 
  clause = *list;
  while (clause != NULL)
  {
    if (clause->choice == DEFLINE_CLAUSEPLUS
      && clause->data.ptrvalue != NULL)
    {
      fcp = clause->data.ptrvalue;
      if ( ! fcp->delete_me)
      {
        num_exons = GetNumberOfConsecutiveExons (clause, bsp);
        if (num_exons > 2)
        {
          ReplaceExonClauseList (fcp, clause, num_exons, bsp);
        }
        else
        {
          RenameExonSequences (&fcp->featlist, bsp, FALSE);
        }
      }
    }
    clause = clause->next;
  }
  if (delete_now) DeleteFeatureClauses (list);
}

static CharPtr organelleByGenome [] = {
  NULL,
  NULL,
  "chloroplast",
  "chromoplast",
  "kinetoplast",
  "mitochondrial",
  "plastid",
  "",
  "",
  "",
  "",
  "",
  "cyanelle",
  "",
  "",
  "",
  "apicoplast",
  "leucoplast",
  "proplastid",
  NULL
};

static CharPtr organelleByPopup [] = {
  NULL,
  "mitochondrial",
  "chloroplast",
  "kinetoplast",
  "plastid",
  "chromoplast",
  "cyanelle",
  "apicoplast",
  "leucoplast",
  "proplastid",
  NULL
};
#define DEFAULT_ORGANELLE_CLAUSE 10

/*---------------------------------------------------------------------*/
/*                                                                     */
/* AutoDef_AddEnding () -- Add an ending on to the definition line     */
/*                         after the last feature.                     */
/*                                                                     */
/*---------------------------------------------------------------------*/

static void AutoDef_AddEnding (
  ValNodePtr   clause_list,
  ValNodePtr PNTR strings,
  BioseqPtr    bsp,
  Int2         mitochloroflag,
  Boolean      alternate_splice_flag
)
{
  Char orgnelle [80];
  Char str [200];
  BioSourcePtr  biop;
  ValNodePtr last_string;
  Int4 new_data_len;
  CharPtr new_data;
  Int4 num_genes;
  SubSourcePtr  ssp;

  str[0] = 0;
  num_genes = 0;
  biop = GetBiopForBsp (bsp);

  if (biop != NULL) {
    if (FindStringInStrings (*strings, "genes"))
    {
      num_genes = 2;
    }
    else if ((last_string = FindStringInStrings (*strings, "gene")) != NULL
      && last_string->next != NULL
      && (last_string = FindStringInStrings (last_string->next, "gene")) != NULL)
    {
      num_genes = 2;
    }
    else
    {
      num_genes = 1;
    }

    orgnelle [0] = '\0';
  
    switch (biop->genome) {
    case GENOME_macronuclear :
      StringCat (str, "; macronuclear");
      break;
    case GENOME_nucleomorph :
      StringCat (str, "; nucleomorph");
      break;
    case GENOME_apicoplast :
    case GENOME_chloroplast :
    case GENOME_chromoplast :
    case GENOME_kinetoplast :
    case GENOME_mitochondrion :
    case GENOME_plastid :
    case GENOME_cyanelle :
    case GENOME_leucoplast :
    case GENOME_proplastid :
      sprintf (orgnelle, "; %s", organelleByGenome [biop->genome]);
      StringCat (str, orgnelle);
      break;
    default :
      ssp = biop->subtype;
      while (ssp != NULL && ssp->subtype != 255)
      {
        ssp = ssp->next;
      }
      if (ssp != NULL
        && ssp->name != NULL
        && StringStr (ssp->name, "micronuclear"))
      {
        StringCat (str, "; micronuclear");
      }
      else if (mitochloroflag > 0) {
        if (mitochloroflag > 9) {
          /* beyond list */
        }
        else {
          if (num_genes > 1)
          {
            sprintf (orgnelle, "; nuclear genes for %s products",
                     organelleByPopup [mitochloroflag]);
          }
          else 
          {
            sprintf (orgnelle, "; nuclear gene for %s product",
                     organelleByPopup [mitochloroflag]);
          }
          StringCat (str, orgnelle);
        }
      }
      break;
    }
  }
  if (alternate_splice_flag) {
    StringCat (str, "; alternatively spliced");
  }

  StringCat (str, ".");

  last_string = *strings;
  if (last_string == NULL)
  {
    ValNodeAddStr (strings, 0, StringSave ( str));
  }
  else
  {
    while (last_string->next != NULL) last_string = last_string->next;
    new_data_len = StringLen (last_string->data.ptrvalue) + StringLen (str) + 1;
    new_data = (CharPtr) MemNew (new_data_len);
    if (new_data == NULL) return;
    StringCpy (new_data, last_string->data.ptrvalue);
    StringCat (new_data, str);
    MemFree (last_string->data.ptrvalue);
    last_string->data.ptrvalue = new_data;
  }
}

static Boolean LastIntervalChangeBeforeEnd (
  FeatureClausePtr onebefore,
  FeatureClausePtr thisclause,
  ValNodePtr rest_of_list
)
{
  ValNodePtr       vnp;
  FeatureClausePtr fcp;

  if (onebefore == NULL || rest_of_list == NULL) return FALSE;
 
  if (StringCmp (onebefore->interval, thisclause->interval) == 0) return FALSE;
  
  for (vnp = rest_of_list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice == DEFLINE_CLAUSEPLUS && vnp->data.ptrvalue != NULL)
    {
      fcp = vnp->data.ptrvalue;
      if (StringCmp (thisclause->interval, fcp->interval) != 0) return FALSE;
    }
  }
  return TRUE;
    
}

static void PluralizeClauseIntervals (
  FeatureClausePtr fcp
)
{
  CharPtr new_interval, cp;

  if (fcp->interval != NULL
    && (cp = StringStr (fcp->interval, "gene, ")) != NULL)
  {
    new_interval = MemNew (StringLen (fcp->interval) + 2);
    if (new_interval == NULL) return;
    StringCpy (new_interval, fcp->interval);
    new_interval [ cp - fcp->interval + 4] = 's';
    StringCpy (new_interval + (cp - fcp->interval) + 5,
               cp + 4);
    MemFree (fcp->interval);
    fcp->interval = new_interval;
  }
}  

static Boolean DisplayAlleleName (FeatureClausePtr thisclause)
{
  if (thisclause == NULL) return FALSE;
  if (StringCmp (thisclause->feature_label_data.typeword, "gene") == 0
    || StringCmp (thisclause->feature_label_data.typeword, "pseudogene") == 0
    || StringCmp (thisclause->feature_label_data.typeword, "mRNA") == 0
    || StringCmp (thisclause->feature_label_data.typeword, "pseudogene mRNA") == 0
    || StringCmp (thisclause->feature_label_data.typeword, "precursor RNA") == 0
    || StringCmp (thisclause->feature_label_data.typeword, "pseudogene precursor RNA") == 0)
  {
    return TRUE;
  }
  return FALSE;
}

static void ListClauses (
  ValNodePtr clauselist,
  ValNodePtr PNTR strings,
  Boolean    allow_semicolons,
  Boolean    suppress_final_and
)
{
  FeatureClausePtr thisclause, onebefore, twobefore, oneafter, twoafter;
  Boolean print_typeword;
  Boolean print_and;
  Boolean print_comma;
  Boolean print_semicolon;
  Boolean print_comma_between_description_and_typeword;
  Boolean typeword_is_plural;
  size_t clause_len;
  CharPtr clause_string;
  Boolean oneafter_has_detail_change;
  Boolean oneafter_has_interval_change;
  Boolean oneafter_has_typeword_change;
  Boolean onebefore_has_detail_change;
  Boolean onebefore_has_interval_change;
  Boolean onebefore_has_typeword_change;
  SeqFeatPtr main_feat;
  CharPtr new_interval;
  ValNodePtr voneafter, vtwoafter;

  while (clauselist != NULL && clauselist->choice != DEFLINE_CLAUSEPLUS)
  {
    clauselist = clauselist->next;
  }
  if (clauselist == NULL) return;

  thisclause = clauselist->data.ptrvalue;
  onebefore = NULL;
  twobefore = NULL;
  
  while (thisclause != NULL)
  {
    oneafter_has_detail_change = FALSE;
    oneafter_has_interval_change = FALSE;
    oneafter_has_typeword_change = FALSE;
    onebefore_has_detail_change = FALSE;
    onebefore_has_interval_change = FALSE;
    onebefore_has_typeword_change = FALSE;
    if (onebefore != NULL)
    {
      if (StringCmp (onebefore->interval, thisclause->interval) != 0)
        onebefore_has_interval_change = TRUE;
      if (StringCmp (onebefore->feature_label_data.typeword,
                     thisclause->feature_label_data.typeword) != 0)
      {
        onebefore_has_typeword_change = TRUE;
      }
      if (onebefore_has_typeword_change || onebefore_has_interval_change
          || (DisplayAlleleName (onebefore) && StringLen (onebefore->allelename) != 0)
          || (DisplayAlleleName (thisclause) && StringLen (thisclause->allelename) != 0))
     {
        onebefore_has_detail_change = TRUE;  
      }
    }
    voneafter = clauselist->next;
    while (voneafter != NULL && voneafter->choice != DEFLINE_CLAUSEPLUS)
    {
      voneafter = voneafter->next;
    }
    if (voneafter == NULL)
    {
      vtwoafter = NULL;
    }
    else
    {
      vtwoafter = voneafter->next;
      while (vtwoafter != NULL && vtwoafter->choice != DEFLINE_CLAUSEPLUS)
      {
        vtwoafter = vtwoafter->next;
      }
    }

    if (voneafter != NULL)
    {
      oneafter = voneafter->data.ptrvalue;
      if (StringCmp (oneafter->interval, thisclause->interval) != 0)
        oneafter_has_interval_change = TRUE;
      if (StringCmp (oneafter->feature_label_data.typeword,
                     thisclause->feature_label_data.typeword) != 0)
      {
        oneafter_has_typeword_change = TRUE;
      }
      if (oneafter_has_typeword_change  || oneafter_has_interval_change
          || (DisplayAlleleName (thisclause) && StringLen (thisclause->allelename) != 0)
          || (DisplayAlleleName (oneafter) && StringLen (oneafter->allelename) != 0))
      {
        oneafter_has_detail_change = TRUE;
      }
      if (vtwoafter != NULL)
      {
        twoafter = vtwoafter->data.ptrvalue;
      }
      else
      {
        twoafter = NULL;
      }
    }
    else
    {
      oneafter = NULL;
      twoafter = NULL;
    }
    print_typeword = FALSE;
    typeword_is_plural = FALSE;
    print_and = FALSE;
    print_comma = FALSE;
    print_semicolon = FALSE;

    if (thisclause->feature_label_data.is_typeword_first)
    {
      if (onebefore == NULL || onebefore_has_detail_change)
      {
        print_typeword = TRUE;
        if (oneafter != NULL && ! oneafter_has_detail_change)
        {
          typeword_is_plural = TRUE;
        }
        else if (StringStr (thisclause->feature_label_data.description, " through ") != NULL
          && StringCmp (thisclause->feature_label_data.typeword, "exon") == 0)
        {
          typeword_is_plural = TRUE;
        }
      }
    }
    else
    {
      if (oneafter == NULL || oneafter_has_detail_change)
      {
        print_typeword = TRUE;
        if (onebefore != NULL && ! onebefore_has_detail_change)
        {
          typeword_is_plural = TRUE;
        }
      }
    }

    /* when to print and before this section */
    if ( onebefore != NULL
         && ! onebefore_has_detail_change
         && (oneafter == NULL || oneafter_has_detail_change))
    {
      print_and = TRUE;
    }
    else if (oneafter == NULL && onebefore != NULL)
    {
      print_and = TRUE;
    }
    else if (onebefore != NULL
         && ! onebefore_has_interval_change
         && oneafter_has_interval_change)
    {
      print_and = TRUE;
    }
    else if ( LastIntervalChangeBeforeEnd ( onebefore, 
                                            thisclause,
                                            clauselist->next))
    {
      print_and = TRUE;
    }

    if (suppress_final_and && oneafter == NULL)
    {
      print_and = FALSE;
    }
    if (suppress_final_and && oneafter != NULL && twoafter == NULL)
    {
      print_comma = TRUE;
    }
    
    /* when to print semicolon after this section */
    /* after every interval change except when exons change "interval" */
    /* exons changing interval are going from alt-spliced to not */
    /* or vice versa, in either case we don't want a semicolon or comma */
    if (oneafter != NULL && oneafter_has_interval_change
      && (StringCmp (thisclause->feature_label_data.typeword, "exon") != 0
         || StringCmp (oneafter->feature_label_data.typeword, "exon") != 0))
    {
      print_semicolon = TRUE;
    }

    /* when to print comma after this section */
    if (onebefore != NULL && oneafter != NULL
      && ! onebefore_has_detail_change
      && ! oneafter_has_detail_change )
    {
      print_comma = TRUE;
    }
    else if (oneafter != NULL && onebefore != NULL
      && ! onebefore_has_interval_change && ! oneafter_has_interval_change
      &&  onebefore_has_typeword_change &&  oneafter_has_typeword_change)
    {
      print_comma = TRUE;
    }
    else if (oneafter != NULL && twoafter != NULL
      && ! oneafter_has_detail_change
      && StringCmp (twoafter->feature_label_data.typeword,
                    thisclause->feature_label_data.typeword) == 0
      && StringCmp (twoafter->interval,
                    thisclause->interval) == 0)
    {
      print_comma = TRUE;
    }
    else if (oneafter != NULL  && twoafter != NULL
      && oneafter_has_typeword_change
      && StringCmp (twoafter->feature_label_data.typeword,
                    oneafter->feature_label_data.typeword) == 0
      && StringCmp (twoafter->interval,
                    oneafter->interval) == 0
      && ! print_and)
    {
      print_comma = TRUE;
    }
    else if (((oneafter_has_interval_change || oneafter == NULL)
      && StringDoesHaveText (thisclause->interval))
      || (oneafter_has_interval_change && oneafter != NULL && ! print_semicolon))
    {
      print_comma = TRUE;
    }
    else if (oneafter != NULL && twoafter != NULL
      && !oneafter_has_interval_change
      && StringCmp (thisclause->interval, twoafter->interval) == 0
      && oneafter_has_typeword_change
      && StringCmp (thisclause->feature_label_data.typeword,
                    twoafter->feature_label_data.typeword) != 0)
    {
      print_comma = TRUE;
    }
    else if (oneafter != NULL && onebefore != NULL && twoafter != NULL
      && ! oneafter_has_interval_change && ! onebefore_has_interval_change
      && StringCmp (thisclause->interval, twoafter->interval) == 0
      && oneafter_has_typeword_change)
    {
      print_comma = TRUE;
    }
    else if (oneafter != NULL && twoafter != NULL
      && oneafter_has_typeword_change
      && StringCmp (oneafter->feature_label_data.typeword,
                    twoafter->feature_label_data.typeword) != 0
      && ! oneafter_has_interval_change
      && StringCmp (oneafter->interval, twoafter->interval) == 0)
    {
      /* spacer 1, foo RNA gene, and spacer2, complete sequence */
      /*         ^ */
      print_comma = TRUE;
    }
    else if (oneafter != NULL && twoafter != NULL 
      && ! oneafter_has_interval_change && StringCmp (thisclause->interval, twoafter->interval) == 0
      && ((DisplayAlleleName (oneafter) && StringLen (oneafter->allelename) > 0)
        || (DisplayAlleleName (thisclause) && StringLen (thisclause->allelename) > 0)))
    {
      print_comma = TRUE;  	
    }
    else if (oneafter != NULL && onebefore != NULL
      && ! oneafter_has_interval_change && ! onebefore_has_interval_change
      && ((DisplayAlleleName (oneafter) && StringLen (oneafter->allelename) > 0)
        || (DisplayAlleleName (thisclause) && StringLen (thisclause->allelename) > 0)))
    {
      print_comma = TRUE;  	
    }

    if (thisclause->featlist != NULL
      && thisclause->featlist->data.ptrvalue != NULL
      && StringDoesHaveText (thisclause->interval)
      && StringNCmp (thisclause->interval, "partial", 7) != 0
      && StringNCmp (thisclause->interval, "complete", 8) != 0)
    {
      main_feat = thisclause->featlist->data.ptrvalue;
      if (IsTransposon (main_feat) || IsInsertionSequence (main_feat)
        || IsEndogenousVirusSourceFeature (main_feat) )
      {
        print_comma = FALSE;
      }
    }

    if (onebefore != NULL
      && ! onebefore_has_interval_change
      && (oneafter_has_interval_change || oneafter == NULL))
    {
      PluralizeClauseIntervals (thisclause);
    }

    if ( thisclause->make_plural )
    {
      if ((onebefore != NULL && ! onebefore_has_detail_change)
        || (oneafter != NULL && !oneafter_has_detail_change))
      {
        PluralizeConsolidatedClauseDescription (thisclause);
      }
      else
      {
        typeword_is_plural = TRUE;
      }
    }

    clause_len = StringLen (thisclause->feature_label_data.description) + 1;

    /* we need to place a comma between the description and the type word 
     * when the description ends with "precursor" and the type word
     * is "gene" or "mRNA"
     */
    if ( thisclause->feature_label_data.description != NULL
      && ! thisclause->feature_label_data.is_typeword_first
      && print_typeword
      && ( StringCmp (thisclause->feature_label_data.typeword,
                      "gene") == 0
        || StringCmp (thisclause->feature_label_data.typeword,
                      "pseudogene")==0
        || StringCmp (thisclause->feature_label_data.typeword,
                      "mRNA")==0
        || StringCmp (thisclause->feature_label_data.typeword,
                      "pseudogene mRNA")==0)
      && clause_len > StringLen ("precursor")
      && StringCmp ( thisclause->feature_label_data.description
                     + clause_len - StringLen ("precursor"),
                     "precursor") == 0)
    {
      print_comma_between_description_and_typeword = TRUE;
      clause_len += 1;
    }
    else
    {
      print_comma_between_description_and_typeword = FALSE;
    }

    if (print_typeword)
      clause_len += StringLen (thisclause->feature_label_data.typeword) + 1;
    if (typeword_is_plural)
      clause_len += 1;
    if (print_and)
      clause_len += 4;
    if (print_comma)
      clause_len += 2;
    if (DisplayAlleleName (thisclause))
    {
      clause_len += StringLen (thisclause->allelename) + 10;
      if (StringLen (thisclause->allelename) > 0) 
      {
        clause_len += StringLen (thisclause->allelename) + StringLen ("allele ");
      }
    }
    
    clause_string = (CharPtr) MemNew (clause_len);
    if (clause_string == NULL)
      return;
    clause_string[0] = 0;
    if (print_and)
      StringCat (clause_string, "and ");
    if (thisclause->feature_label_data.is_typeword_first && print_typeword
      && thisclause->feature_label_data.typeword != NULL
      && ! StringHasNoText (thisclause->feature_label_data.typeword))
    {
      StringCat (clause_string, thisclause->feature_label_data.typeword);
      if (typeword_is_plural)
        StringCat (clause_string, "s");
      if (thisclause->feature_label_data.description != NULL)
        StringCat (clause_string, " ");
    }
    if (thisclause->feature_label_data.description != NULL)
    {
      StringCat (clause_string, thisclause->feature_label_data.description);
      if (print_comma_between_description_and_typeword)
      {
        StringCat (clause_string, ",");
      }
    }
    if (! thisclause->feature_label_data.is_typeword_first && print_typeword
      && thisclause->feature_label_data.typeword != NULL
      && ! StringHasNoText (thisclause->feature_label_data.typeword))
    {
      if (thisclause->feature_label_data.description != NULL)
        StringCat (clause_string, " ");
      StringCat (clause_string, thisclause->feature_label_data.typeword);
      if (typeword_is_plural)
        StringCat (clause_string, "s");
      if (DisplayAlleleName (thisclause)
        && thisclause->allelename != NULL)
      {
        StringCat (clause_string, ", ");
        StringCat (clause_string, thisclause->allelename);
        StringCat (clause_string, " allele");
      }
    }
    if (StringLen (clause_string) > 0 ) 
    {
      if (print_comma)
        StringCat (clause_string, ",");
      ValNodeAddStr (strings, 0, clause_string);
    }
    else 
    {
    	MemFree (clause_string);
    	clause_string = NULL;
    }
 
    if (oneafter == NULL || oneafter_has_interval_change)
    {
      if (print_semicolon
        && (thisclause->interval == NULL
          || StringHasNoText(thisclause->interval)
          || thisclause->interval[StringLen (thisclause->interval)] != ';'))
      {
        new_interval = MemNew (StringLen (thisclause->interval) + 2);
        if (new_interval == NULL) return;
        StringCpy (new_interval, thisclause->interval);
        if (allow_semicolons) 
        {
          StringCat (new_interval, ";");
        }
        else
        {
          StringCat (new_interval, ",");
        }
        MemFree (thisclause->interval);
        thisclause->interval = new_interval;
      } 
      if (thisclause->interval != NULL
        && !StringHasNoText (thisclause->interval))
      {
        ValNodeAddStr (strings, 0, StringSave (thisclause->interval));
      }
    }
    twobefore = onebefore;
    onebefore = thisclause;   
    thisclause = oneafter;
    clauselist = voneafter;
  }
}

static Uint1 GetMoleculeType 
(BioseqPtr bsp,
 Uint2     entityID)
{
  SeqDescPtr         sdp;
  MolInfoPtr         mip;
  SeqMgrDescContext  dcontext;

  if (bsp == NULL) return MOLECULE_TYPE_GENOMIC;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, &dcontext);
  if (sdp == NULL) return MOLECULE_TYPE_GENOMIC;
  mip = (MolInfoPtr) sdp->data.ptrvalue;
  if (mip == NULL) return MOLECULE_TYPE_GENOMIC;
  return mip->biomol;
}

static Boolean SpecialHandlingForSpecialTechniques (
  BioseqPtr bsp
)
{
  SeqDescPtr sdp;
  MolInfoPtr mip;
  ValNodePtr vnp;

  if (bsp == NULL) return MOLECULE_TYPE_GENOMIC;
  sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_molinfo, NULL);
  if (sdp == NULL)
  {
    for (sdp = bsp->descr;
         sdp != NULL && sdp->choice != Seq_descr_molinfo;
         sdp = sdp->next)
    {}
  }
  if (sdp == NULL) return FALSE;
  mip = (MolInfoPtr) sdp->data.ptrvalue;
  if (mip == NULL) return FALSE;
  if (mip->tech == MI_TECH_htgs_0 ||
      mip->tech == MI_TECH_htgs_1 ||
      mip->tech == MI_TECH_htgs_2 ||
      mip->tech == MI_TECH_est ||
      mip->tech == MI_TECH_sts ||
      mip->tech == MI_TECH_survey ||
      mip->tech == MI_TECH_wgs) {
    vnp = ValNodeExtract (&(bsp->descr), Seq_descr_title);
    if (vnp != NULL)
      vnp = ValNodeFreeData (vnp);
    return TRUE;
  }

  return FALSE;
}

static Boolean LIBCALLBACK ShouldRemoveExon (
  SeqFeatPtr sfp,
  FeatureClausePtr parent_fcp,
  FeatureClausePtr this_fcp,
  BioseqPtr bsp,
  Boolean isLonely,
  Boolean isRequested,
  Boolean isSegment
)
{
  Boolean partial3, partial5;
  SeqFeatPtr main_feat;

  if (isSegment || isLonely || isRequested) return FALSE;
  if (parent_fcp == NULL
    || parent_fcp->featlist == NULL
    || parent_fcp->featlist->data.ptrvalue == NULL)
  {
    return TRUE;
  }
  
  main_feat = parent_fcp->featlist->data.ptrvalue;
  if ( IsCDS (main_feat))
  {
    CheckSeqLocForPartial (main_feat->location, &partial5, &partial3);
    if (partial5 || partial3) return FALSE;
  }
  return TRUE;
}

static Boolean LIBCALLBACK ShouldRemoveCDS (
  SeqFeatPtr sfp,
  FeatureClausePtr parent_fcp,
  FeatureClausePtr this_fcp,
  BioseqPtr bsp,
  Boolean isLonely,
  Boolean isRequested,
  Boolean isSegment
)
{
  CharPtr description;
  Boolean retval = FALSE;

  description = GetGeneProtDescription (this_fcp, bsp);
  if (StringHasNoText (description))
  {
    retval = TRUE;
  }
  if (description != NULL) MemFree (description);
  return retval;
}

static Boolean LIBCALLBACK ShouldRemoveNoncodingProductFeat (
  SeqFeatPtr sfp,
  FeatureClausePtr parent_fcp,
  FeatureClausePtr this_fcp,
  BioseqPtr bsp, Boolean isLonely,
  Boolean isRequested,
  Boolean isSegment
)
{
  if (isRequested) return FALSE;
  return TRUE;
}

static Boolean LIBCALLBACK ShouldRemovePromoter (
  SeqFeatPtr sfp,
  FeatureClausePtr parent_fcp,
  FeatureClausePtr this_fcp,
  BioseqPtr bsp, Boolean isLonely,
  Boolean isRequested,
  Boolean isSegment
)
{
  if (isLonely || isRequested) return FALSE;
  else return TRUE;
}

static Boolean LIBCALLBACK ShouldRemoveLTR (
  SeqFeatPtr sfp,
  FeatureClausePtr parent_fcp,
  FeatureClausePtr this_fcp,
  BioseqPtr bsp,
  Boolean isLonely,
  Boolean isRequested,
  Boolean isSegment
)
{
  if (isLonely || isRequested)
    return FALSE;
  else
    return TRUE;
}

static Boolean LIBCALLBACK ShouldRemove3UTR (
  SeqFeatPtr sfp,
  FeatureClausePtr parent_fcp,
  FeatureClausePtr this_fcp,
  BioseqPtr bsp,
  Boolean isLonely,
  Boolean isRequested,
  Boolean isSegment
)
{
  if (isLonely || isRequested)
    return FALSE;
  else
    return TRUE;
}

static Boolean LIBCALLBACK ShouldRemove5UTR (
  SeqFeatPtr sfp,
  FeatureClausePtr parent_fcp,
  FeatureClausePtr this_fcp,
  BioseqPtr bsp,
  Boolean isLonely,
  Boolean isRequested,
  Boolean isSegment
)
{
  if (isLonely || isRequested)
    return FALSE;
  else
    return TRUE;
}

static Boolean LIBCALLBACK ShouldRemoveIntron (
  SeqFeatPtr sfp,
  FeatureClausePtr parent_fcp,
  FeatureClausePtr this_fcp,
  BioseqPtr bsp, Boolean isLonely,
  Boolean isRequested,
  Boolean isSegment
)
{
  if (isLonely || isRequested) return FALSE;
  else return TRUE;
}

static Boolean LIBCALLBACK ShouldRemoveGeneric (
  SeqFeatPtr sfp,
  FeatureClausePtr parent_fcp,
  FeatureClausePtr this_fcp,
  BioseqPtr bsp,
  Boolean isLonely,
  Boolean isRequested,
  Boolean isSegment
)
{
  CharPtr productname;
  Boolean rval;

  rval = FALSE;
  if (IsMiscRNA (sfp) && ( productname = GetProductName (sfp, bsp)) != NULL)
  {
    if (StringStr (productname, "trans-spliced leader") != NULL)
    {
      rval = TRUE;
    }
    MemFree (productname);
  }
    
  return rval;
}


typedef struct removableitemglobal {
  matchFunction  itemmatch;
  ShouldRemoveFunction ShouldRemove;
  CharPtr  group_name;
} RemovableItemGlobalData, PNTR RemovableItemGlobalPtr;

typedef struct removableitemlocal {
  ButtoN  keep_request;
  Boolean  keep;
} RemovableItemLocalData, PNTR RemovableItemLocalPtr;

static RemovableItemGlobalData remove_items[] = {
  { IsExon, ShouldRemoveExon, "Exons" },
  { IsIntron, ShouldRemoveIntron, "Introns" },
  { IsCDS,  ShouldRemoveCDS, "CDSs" },
  { IsNoncodingProductFeat,  ShouldRemoveNoncodingProductFeat, "Misc feats with comments" },
  { IsPromoter, ShouldRemovePromoter, "Promoters" },
  { IsLTR, ShouldRemoveLTR, "LTRs" },
  { Is3UTR, ShouldRemove3UTR, "3' UTRs" },
  { Is5UTR, ShouldRemove5UTR, "5' UTRs" }
};

typedef enum {
  RemovableExon = 0,
  RemovableIntron,
  RemovableCDS,
  RemovableNoncodingProductFeat,
  RemovablePromoter,
  RemovableLTR,
  Removable3UTR,
  Removable5UTR,
  NumRemovableItems
} RemovableList;

typedef enum {
  DEFLINE_USE_FEATURES = 1,
  DEFLINE_COMPLETE_SEQUENCE,
  DEFLINE_COMPLETE_GENOME
} DefLineType;

typedef struct deflinefeaturerequestlist {
  RemovableItemLocalData items[NumRemovableItems];
  Boolean                suppress_alt_splice_phrase;
  Boolean                remove_subfeatures;
  DefLineType            feature_list_type;
  Int4                   misc_feat_parse_rule;
} DeflineFeatureRequestList, PNTR DeflineFeatureRequestListPtr;

static void InitFeatureRequests (
  DeflineFeatureRequestListPtr feature_requests
)
{
  Int4 i;
  for (i=0; i < NumRemovableItems; i++)
  {
    feature_requests->items[i].keep_request = NULL;
    feature_requests->items[i].keep = FALSE;
  }
  feature_requests->suppress_alt_splice_phrase = FALSE;
  feature_requests->remove_subfeatures = FALSE;
  feature_requests->feature_list_type = DEFLINE_USE_FEATURES;
  feature_requests->misc_feat_parse_rule = 2;
}

static Boolean RemoveCondition (
  SeqFeatPtr sfp,
  FeatureClausePtr parent_fcp,
  FeatureClausePtr this_fcp,
  BioseqPtr bsp,
  Boolean isLonely,
  Boolean isSegment,
  DeflineFeatureRequestList *feature_requests
)
{
  Int4 i;
  if (sfp == NULL) return TRUE;
  for (i=0; i < NumRemovableItems; i++)
  {
    if (remove_items[i].itemmatch (sfp))
      return remove_items[i].ShouldRemove (sfp, parent_fcp, this_fcp, bsp,
      isLonely, feature_requests->items[i].keep, isSegment);
  }
  return ShouldRemoveGeneric(sfp, parent_fcp, this_fcp, bsp, isLonely, FALSE,
                             isSegment);
}

static Boolean FindOtherGeneClause (
  ValNodePtr feature_list,
  ValNodePtr me,
  GeneRefPtr grp
)
{
  ValNodePtr vnp;
  FeatureClausePtr fcp;

  if (grp == NULL) return FALSE;

  for (vnp = feature_list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp == me) continue;
    if (vnp->choice == DEFLINE_CLAUSEPLUS && vnp->data.ptrvalue != NULL)
    {
      fcp = vnp->data.ptrvalue;
      if (fcp->delete_me) continue;
      if ( fcp->grp == grp
        || (fcp->grp != NULL && DoGenesMatch (fcp->grp, grp)))
      {
        return TRUE;
      }
      if ( FindOtherGeneClause (fcp->featlist, me, grp))
      {
        return TRUE;
      }
    }
  }
  return FALSE;
}
 
static void RemoveGenesMentionedElsewhere (
  ValNodePtr PNTR feature_list,
  ValNodePtr search_list,
  Boolean    delete_now
)
{
  ValNodePtr vnp;
  FeatureClausePtr fcp;

  for (vnp = *feature_list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice == DEFLINE_CLAUSEPLUS && vnp->data.ptrvalue != NULL)
    {
      fcp = vnp->data.ptrvalue;
      if (fcp->featlist == NULL)
      {
        continue;
      }
      if ( IsGene (fcp->featlist->data.ptrvalue)
        && fcp->featlist->next == NULL
        && FindOtherGeneClause ( search_list, vnp, fcp->grp))
      {
        fcp->delete_me = TRUE;
      }
      else
      {
        RemoveGenesMentionedElsewhere ( &(fcp->featlist), search_list, FALSE);
      }
    }
  }
  if (delete_now)
  {
    DeleteFeatureClauses (feature_list);
  }
}

static void MarkUnwantedFeatureClauseForRemoval (
  ValNodePtr clause,
  BioseqPtr  bsp,
  Boolean    isLonely,
  FeatureClausePtr parent_fcp,
  Boolean    isSegment,
  DeflineFeatureRequestList PNTR feature_requests
)
{
  FeatureClausePtr fcp;
  ValNodePtr       featlist;
  ValNodePtr       firstfeat;
  Int4             clause_count;

  if (clause == NULL
    || clause->choice != DEFLINE_CLAUSEPLUS
    || clause->data.ptrvalue == NULL)
  {
    return;
  }

  fcp = clause->data.ptrvalue;
  firstfeat = fcp->featlist;
  clause_count = 0;
  for (featlist = firstfeat;
       featlist != NULL && isLonely;
       featlist = featlist->next)
  {
    if (featlist->choice == DEFLINE_CLAUSEPLUS)
    {
      clause_count ++;
      if (clause_count > 1)
      {
        isLonely = FALSE;
      }
    }
  }
    
  featlist = firstfeat;
  while (featlist != NULL)
  {  
    if (featlist->choice == DEFLINE_FEATLIST
      && featlist->data.ptrvalue != NULL
      && RemoveCondition (featlist->data.ptrvalue, parent_fcp, fcp, bsp, 
                          isLonely, isSegment, feature_requests))
    {
      fcp->delete_me = TRUE;
    }
    else if (featlist->choice == DEFLINE_CLAUSEPLUS 
      && featlist->data.ptrvalue != NULL)
    {
      MarkUnwantedFeatureClauseForRemoval (featlist, bsp, isLonely, fcp,
                                           isSegment,
                                           feature_requests);
    }
    featlist = featlist->next;
  }
}
  
static void RemoveUnwantedFeatures (
  ValNodePtr PNTR list,
  BioseqPtr bsp,
  Boolean   isSegment,
  DeflineFeatureRequestList PNTR feature_requests
)
{
  ValNodePtr       vnp;
  Boolean          isLonely;

  if (list == NULL) return;

  isLonely = TRUE;

  for (vnp = *list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->next != NULL) isLonely = FALSE;
    if (vnp->choice == DEFLINE_CLAUSEPLUS)
    {
      MarkUnwantedFeatureClauseForRemoval (vnp, bsp, isLonely, NULL,
                                           isSegment, feature_requests);
    }
  }
  DeleteFeatureClauses (list);
}

static Boolean LIBCALLBACK IsMasterClause (
  SeqFeatPtr sfp
)
{
  if ( IsTransposon (sfp) || IsInsertionSequence (sfp)) return TRUE;
  return FALSE;
}

static void DeleteSubfeatures (
  ValNodePtr PNTR feature_list,
  Boolean         delete_now
)
{
  ValNodePtr       clause, featlist;
  FeatureClausePtr clause_fcp, fcp;

  if (feature_list == NULL) return;
  for (clause = *feature_list; clause != NULL; clause = clause->next)
  {
    if (clause->choice != DEFLINE_CLAUSEPLUS
      || (clause_fcp = clause->data.ptrvalue) == NULL
      || clause_fcp->featlist == NULL)
    {
      continue;
    }
    if (clause_fcp->featlist->choice == DEFLINE_FEATLIST
      && IsMasterClause (clause_fcp->featlist->data.ptrvalue))
    {
      for (featlist = clause_fcp->featlist->next;
           featlist != NULL;
           featlist = featlist->next)
      {
        if (featlist->choice == DEFLINE_CLAUSEPLUS
          && (fcp = featlist->data.ptrvalue) != NULL)
        {
          fcp->delete_me = TRUE;
        }
      }
    }
    else
    {
      DeleteSubfeatures ( &(clause_fcp->featlist), FALSE);
    }
  }
  if (delete_now) 
  {
    DeleteFeatureClauses (feature_list);
  }
}

static void DeleteOperonSubfeatures (
  ValNodePtr PNTR feature_list,
  Boolean         delete_now
)
{
  ValNodePtr       clause, featlist;
  FeatureClausePtr clause_fcp, fcp;

  if (feature_list == NULL) return;
  for (clause = *feature_list; clause != NULL; clause = clause->next)
  {
    if (clause->choice != DEFLINE_CLAUSEPLUS
      || (clause_fcp = clause->data.ptrvalue) == NULL
      || clause_fcp->featlist == NULL)
    {
      continue;
    }
    if (clause_fcp->featlist->choice == DEFLINE_FEATLIST
      && IsOperon (clause_fcp->featlist->data.ptrvalue))
    {
      for (featlist = clause_fcp->featlist->next;
           featlist != NULL;
           featlist = featlist->next)
      {
        if (featlist->choice == DEFLINE_CLAUSEPLUS
          && (fcp = featlist->data.ptrvalue) != NULL)
        {
          fcp->delete_me = TRUE;
        }
      }
    }
    else
    {
      DeleteSubfeatures ( &(clause_fcp->featlist), FALSE);
    }
  }
  if (delete_now) 
  {
    DeleteFeatureClauses (feature_list);
  }
}

static void RemoveFeats (
  ValNodePtr    list,
  matchFunction itemmatch
)
{
  ValNodePtr vnp;
  FeatureClausePtr fcp;
  
  if (list == NULL) return;

  for (vnp = list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice == DEFLINE_FEATLIST
      && itemmatch (vnp->data.ptrvalue))
    {
      vnp->choice = DEFLINE_REMOVEFEAT;
    }
    else if (vnp->choice == DEFLINE_CLAUSEPLUS
      && (fcp = vnp->data.ptrvalue) != NULL)
    {
      RemoveFeats (fcp->featlist, itemmatch);
    }
  }
}

/* A clause is "tall" if it has only one clause at any level */
static Boolean IsClauseTall (
  FeatureClausePtr fcp
)
{
  ValNodePtr featlist;
  Int4       num_clauses;
  FeatureClausePtr subclause;

  num_clauses = 0;
  if (fcp == NULL) return FALSE;
  subclause = NULL;
  if (fcp->featlist == NULL) return FALSE;
  for (featlist = fcp->featlist;
       featlist != NULL;
       featlist = featlist->next)
  {
    if (featlist->choice == DEFLINE_CLAUSEPLUS)
    {
      subclause = featlist->data.ptrvalue;
      if (subclause == NULL || ! IsClauseTall (subclause))
      {
        return FALSE;
      }
      num_clauses ++;
      if (num_clauses > 1) return FALSE;
    }
  }
  if (subclause == NULL || ! subclause->feature_label_data.is_typeword_first)
  {
    return TRUE;
  }
  return FALSE;
}

static void SmashOneTallClause (
  FeatureClausePtr fcp
)
{
  FeatureClausePtr subclause;
  ValNodePtr       featlist;
  ValNodePtr       subclause_featlist;
  ValNodePtr       subclause_firstclause;
  CharPtr          new_description;
  Int4             new_description_len;
  SeqFeatPtr       main_feat;

  if (fcp == NULL || fcp->featlist == NULL) return;

  /* move features up */
  featlist = fcp->featlist;
  if (featlist->choice == DEFLINE_FEATLIST)
  {
    main_feat = fcp->featlist->data.ptrvalue;
  }
  else
  {
    main_feat = NULL;
  }
  
  while (featlist != NULL && featlist->choice != DEFLINE_CLAUSEPLUS)
  {
    featlist = featlist->next;
  }
  if (featlist == NULL) return;
  subclause = featlist->data.ptrvalue;
  if (subclause == NULL) return;
 
  /* move subclause feats to top of list */
  if (subclause->featlist != NULL
    && subclause->featlist->choice == DEFLINE_FEATLIST)
  {
    subclause_featlist = subclause->featlist;
    while (subclause->featlist != NULL
           && subclause->featlist->next != NULL
           && subclause->featlist->next->choice == DEFLINE_FEATLIST)
    {
      subclause->featlist = subclause->featlist->next;
    }
    if (subclause->featlist != NULL)
    {
      subclause_firstclause = subclause->featlist->next;
      subclause->featlist->next = fcp->featlist;
      fcp->featlist = subclause->featlist;
      subclause->featlist = subclause_firstclause;
    }
  }

  /* create new description */
  new_description_len = StringLen (subclause->feature_label_data.description)
                   + StringLen (fcp->feature_label_data.description)
                   + StringLen (fcp->feature_label_data.typeword)
                   + 4;
  new_description = (CharPtr) MemNew (new_description_len);
  if (new_description == NULL) return;
  new_description [0] = 0;
  if ( fcp->feature_label_data.is_typeword_first)
  {
    StringCat (new_description, fcp->feature_label_data.typeword);
    StringCat (new_description, " ");
  }
  StringCat (new_description, fcp->feature_label_data.description);
  if ( ! fcp->feature_label_data.is_typeword_first)
  {
    StringCat (new_description, fcp->feature_label_data.typeword);
  }

  if ( ! IsTransposon (main_feat)
    && ! IsInsertionSequence (main_feat)
    && ! IsEndogenousVirusSourceFeature (main_feat) )
  {
    StringCat (new_description, ",");
  }
  StringCat (new_description, " ");
  StringCat (new_description, subclause->feature_label_data.description);

  if (fcp->feature_label_data.description != NULL)
  {
    MemFree (fcp->feature_label_data.description);
  }
  fcp->feature_label_data.description = new_description;
 
  /* move interval up */
  if (fcp->interval != NULL)
  {
    MemFree (fcp->interval);
  }
  fcp->interval = subclause->interval;
  subclause->interval = NULL;

  /* move typeword up */
  fcp->feature_label_data.typeword = subclause->feature_label_data.typeword;
  fcp->feature_label_data.is_typeword_first = 
               subclause->feature_label_data.is_typeword_first;
  subclause->feature_label_data.typeword = NULL;
  subclause->delete_me = TRUE;

}

static void SmashTallClauses (
  ValNodePtr PNTR clause_list,
  Boolean         delete_now
)
{
  ValNodePtr clause;
  FeatureClausePtr fcp;

  if (clause_list == NULL) return;
  for (clause = *clause_list; clause != NULL; clause = clause->next)
  {
    if (clause->choice != DEFLINE_CLAUSEPLUS || clause->data.ptrvalue == NULL)
    {
      continue;
    }
    fcp = clause->data.ptrvalue;
    if ( IsClauseTall (fcp))
    {
      SmashOneTallClause (fcp);
    }
    else
    {
      SmashTallClauses (& (fcp->featlist), FALSE);
    }
  }
  if (delete_now) 
  {
    DeleteFeatureClauses (clause_list);
  }
}

static ValNodePtr RemoveAllButLastCDS (
  ValNodePtr list,
  ValNodePtr last_cds
)
{
  ValNodePtr vnp;
  FeatureClausePtr fcp;

  /* now remove all CDSs except the last one */
  for (vnp = list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice == DEFLINE_FEATLIST
      && IsCDS (vnp->data.ptrvalue))
    {
      if (last_cds != NULL)
      {
        last_cds->choice = DEFLINE_REMOVEFEAT;
      }
      last_cds = vnp;
    }
    else if (vnp->choice == DEFLINE_CLAUSEPLUS
      && (fcp = vnp->data.ptrvalue) != NULL)
    {
      last_cds = RemoveAllButLastCDS (fcp->featlist, last_cds);
    }
  }
  return last_cds;
}

static Boolean OkToConsolidate (
  CharPtr last_desc,
  CharPtr new_desc,
  Boolean last_partial,
  Boolean new_partial,
  FeatureClausePtr last_fcp,
  FeatureClausePtr fcp
)
{
  if (StringCmp (last_desc, new_desc) != 0) return FALSE;
  if (new_partial != last_partial) return FALSE;
  if ( ( fcp->is_alt_spliced && ! last_fcp->is_alt_spliced)
      || (! fcp->is_alt_spliced && last_fcp->is_alt_spliced))
  {
    return FALSE;
  }
  if (fcp->featlist == NULL || last_fcp->featlist == NULL) return FALSE;
  if ( fcp->featlist->choice != DEFLINE_FEATLIST) return FALSE;
  if ( last_fcp->featlist->choice != DEFLINE_FEATLIST) return FALSE;
  if ( (IsCDS (fcp->featlist->data.ptrvalue)
        && ! IsCDS (last_fcp->featlist->data.ptrvalue)
        && ! IsGene (last_fcp->featlist->data.ptrvalue))
      || (! IsCDS (fcp->featlist->data.ptrvalue)
        && ! IsGene (fcp->featlist->data.ptrvalue)
        && IsCDS (last_fcp->featlist->data.ptrvalue)))
  {
    return FALSE;
  }
  return TRUE;
}

static void RemoveRedundantGeneFeatureFromConsolidatedClause (
  FeatureClausePtr fcp
)
{
  ValNodePtr featlist, prevfeat, tmpfeat;
  SeqFeatPtr feat1, feat2;

  prevfeat = NULL;
  featlist = fcp->featlist;
  while ( featlist != NULL
         && featlist->choice == DEFLINE_FEATLIST
         && featlist->next != NULL
         && featlist->next->choice == DEFLINE_FEATLIST)
  {
    feat1 = featlist->data.ptrvalue;
    feat2 = featlist->next->data.ptrvalue;
    if (feat1 == NULL || feat2 == NULL) return;
    if (IsGene (feat1) && ! IsGene (feat2))
    {
      if (prevfeat == NULL)
      {
        fcp->featlist = featlist->next;
        featlist->next = NULL;
        FreeListElement (featlist);
        featlist = fcp->featlist->next;
      }
      else
      {
        prevfeat->next = featlist->next;
        featlist->next = NULL;
        FreeListElement (featlist);
        featlist = prevfeat->next;
      }
    }
    else if ( !IsGene (feat1) && IsGene (feat2))
    {
      tmpfeat = featlist->next;
      featlist->next = tmpfeat->next;
      tmpfeat->next = NULL;
      FreeListElement (tmpfeat);
    }
    else
    {
      featlist = featlist->next;
    }
  }
}

static void PluralizeConsolidatedClauseDescription (
  FeatureClausePtr fcp
)
{
  CharPtr new_desc;

  new_desc = MemNew (StringLen (fcp->feature_label_data.description) + 2);
  if (new_desc == NULL) return;

  StringCpy (new_desc, fcp->feature_label_data.description);
  StringCat (new_desc, "s");
  MemFree (fcp->feature_label_data.description);
  fcp->feature_label_data.description = new_desc;
}

static void ConsolidateClauses (
  ValNodePtr PNTR list,
  BioseqPtr  bsp,
  Uint1      biomol,
  Boolean    delete_now
)
{
  ValNodePtr       vnp;
  FeatureClausePtr fcp;
  FeatureClausePtr last_cds_fcp;
  CharPtr          last_desc, new_desc;
  Boolean          last_partial, new_partial, partial3, partial5;

  if (list == NULL || *list == NULL) return;
  last_cds_fcp = NULL;
  last_desc = NULL;
  for (vnp = *list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice != DEFLINE_CLAUSEPLUS
      || (fcp = vnp->data.ptrvalue) == NULL
      || fcp->featlist == NULL
      || fcp->featlist->choice != DEFLINE_FEATLIST)
    {
      continue;
    }

    ConsolidateClauses (&(fcp->featlist), bsp, biomol, FALSE);

    if (last_cds_fcp == NULL)
    {
      last_cds_fcp = fcp;
      if (fcp->feature_label_data.description == NULL)
      {
        last_desc = GetGeneProtDescription (fcp, bsp);
      }
      else
      {
        last_desc = StringSave (fcp->feature_label_data.description);
      }
      CheckSeqLocForPartial (fcp->slp, &partial5, &partial3);
      if (partial5 || partial3) 
      {
        last_partial = TRUE;
      }
      else
      {
        last_partial = FALSE;
      }
    }
    else 
    {
      if (fcp->feature_label_data.description == NULL)
      {
        new_desc = GetGeneProtDescription (fcp, bsp);
      }
      else
      {
        new_desc = StringSave (fcp->feature_label_data.description);
      }
      CheckSeqLocForPartial (fcp->slp, &partial5, &partial3);
      if (partial5 || partial3) 
      {
        new_partial = TRUE;
      }
      else
      {
        new_partial = FALSE;
      }
      if ( OkToConsolidate (last_desc, new_desc,
                            last_partial, new_partial,
                            last_cds_fcp, fcp))
      {
        /* two clauses have identical descriptions - combine them */
        MoveSubclauses (last_cds_fcp, fcp);
        RemoveRedundantGeneFeatureFromConsolidatedClause (last_cds_fcp);
        fcp->featlist = NULL;
        fcp->delete_me = TRUE;
        last_cds_fcp->slp = SeqLocMerge (bsp, last_cds_fcp->slp, fcp->slp,
                                         FALSE, TRUE, FALSE);
        /* if we have two clauses that are really identical instead of
         * just sharing a "prefix", make the description plural
         */
        if (StringCmp (last_cds_fcp->interval, fcp->interval) == 0)
        {
          last_cds_fcp->make_plural = TRUE;
/*          PluralizeConsolidatedClauseDescription (last_cds_fcp); */
        }

        /* Recalculate the interval */
        if (last_cds_fcp->interval != NULL)
        {
          MemFree (last_cds_fcp->interval);
        }
        last_cds_fcp->interval =
                  GetGenericInterval (last_cds_fcp, biomol, bsp);
        MemFree (new_desc);
      }
      else
      {
        MemFree (last_desc);
        last_desc = new_desc;
        last_cds_fcp = fcp;
        last_partial = new_partial;
      }
    }  
  }   
  if (delete_now) 
  {
    DeleteFeatureClauses (list);
  }
} 

static void CountUnknownGenes (
  ValNodePtr PNTR clause_list,
  BioseqPtr bsp
)
{
  FeatureClausePtr fcp, new_fcp;
  ValNodePtr vnp, new_vnp;
  CharPtr gene_name;
  Int4 num_unknown_genes;
  
  num_unknown_genes = 0;
  vnp = *clause_list;
  new_vnp = NULL;
  new_fcp = NULL;
  while (vnp != NULL)
  {
    if (vnp->choice == DEFLINE_CLAUSEPLUS
      && (fcp = vnp->data.ptrvalue) != NULL
      && ! fcp->is_unknown) 
    {
      CountUnknownGenes (&(fcp->featlist), bsp);
      gene_name = GetGeneProtDescription (fcp, bsp);
      if (StringCmp (gene_name, "unknown") == 0
        && fcp->featlist != NULL
        && fcp->featlist->choice == DEFLINE_FEATLIST)
      {
        if (new_fcp == NULL)
        {
          new_vnp = ValNodeNew (*clause_list);
          if (new_vnp == NULL) return;
          new_fcp = NewFeatureClause (fcp->featlist->data.ptrvalue, bsp);
          new_fcp->is_unknown = TRUE;
          new_vnp->choice = DEFLINE_CLAUSEPLUS;
          new_vnp->data.ptrvalue = new_fcp;
        }
        else
        {
          new_vnp = ValNodeNew (new_fcp->featlist);
          if (new_vnp == NULL) return;
          new_vnp->choice = DEFLINE_FEATLIST;
          new_vnp->data.ptrvalue = fcp->featlist->data.ptrvalue;
        }  
        num_unknown_genes ++;
        fcp->delete_me = TRUE;
      }
    }
    vnp = vnp->next;
  }
   
  if (num_unknown_genes > 0)
  {
    DeleteFeatureClauses (clause_list);
    if (num_unknown_genes > 1)
    {
      new_fcp->feature_label_data.typeword = StringSave ("genes");
    }
  }
}

static void ReplaceDefinitionLine (
  SeqEntryPtr sep,
  CharPtr defline
)
{
  ValNodePtr ttl;
  if (sep == NULL || defline == NULL) return;

  ttl = SeqEntryGetSeqDescr (sep, Seq_descr_title, NULL);
  if (ttl == NULL)
    ttl = CreateNewDescriptor (sep, Seq_descr_title);
  if (ttl != NULL) {
    MemFree (ttl->data.ptrvalue);
    ttl->data.ptrvalue = defline;
    defline = NULL;
  }
  MemFree (defline);
}

FeatureClausePtr NewFeatureClause (
  SeqFeatPtr sfp,
  BioseqPtr  bsp
)
{
  FeatureClausePtr fcp;
  Boolean          partial5, partial3;

  fcp = (FeatureClausePtr) MemNew (sizeof (FeatureClauseData));
  if (fcp == NULL) return NULL;

  fcp->feature_label_data.typeword = NULL;
  fcp->feature_label_data.description = NULL;
  fcp->feature_label_data.productname = NULL;
  fcp->feature_label_data.pluralizable = FALSE;
  fcp->feature_label_data.is_typeword_first = FALSE;
  fcp->allelename = NULL;
  fcp->interval = NULL;
  fcp->featlist = NULL;
  fcp->delete_me = FALSE;
  fcp->clause_info_only = FALSE;
  fcp->make_plural = FALSE;
  fcp->is_unknown = FALSE;
  fcp->grp = NULL;
  if (sfp == NULL) return fcp;
  CheckSeqLocForPartial (sfp->location, &partial5, &partial3);
  fcp->slp = SeqLocMerge (bsp, sfp->location, NULL,
                                 FALSE, TRUE, FALSE);
  SetSeqLocPartial (fcp->slp, partial5, partial3);
  
  if (sfp->data.choice == SEQFEAT_GENE)
  {
    fcp->grp = sfp->data.value.ptrvalue;
  }
  else
  { 
    fcp->grp = SeqMgrGetGeneXref (sfp);
  }
  if (( IsCDS (sfp) || IsExon (sfp) || IsNoncodingProductFeat (sfp))
    && StringStr (sfp->comment, "alternatively spliced") != NULL) 
  {
    fcp->is_alt_spliced = TRUE;
  }
  else
  {
    fcp->is_alt_spliced = FALSE;
  }
  if (IsCDS (sfp))
  {
    fcp->feature_label_data.productname = GetProductName (sfp, bsp);
  }
  fcp->featlist = ValNodeNew (NULL);
  if (fcp->featlist == NULL)
  {
    MemFree (fcp);
    return NULL;
  }

  fcp->featlist->data.ptrvalue = sfp;
  fcp->featlist->choice = DEFLINE_FEATLIST;
  
  return fcp;
}

static ValNodePtr GetFeatureList (
  BioseqPtr bsp
)
{
  ValNodePtr        head, vnp;
  SeqFeatPtr        sfp;
  FeatureClausePtr  fcp;
  SeqMgrFeatContext fcontext;

  if (bsp == NULL) return NULL;

  /* get list of all recognized features */
  head = NULL;
  sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
  while (sfp != NULL)
  {
    if (IsRecognizedFeature (sfp))
    {
      fcp = NewFeatureClause (sfp, bsp);
      if (fcp == NULL) return NULL;
      fcp->numivals = fcontext.numivals;
      fcp->ivals = fcontext.ivals;
      vnp = ValNodeNew (head);
      if (head == NULL) head = vnp;
      if (vnp == NULL) return NULL;
      vnp->data.ptrvalue = fcp;
      vnp->choice = DEFLINE_CLAUSEPLUS;
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
  }
  return head;
}

static void ExtractSegmentClauses (
  ValNodePtr segment_features,
  ValNodePtr parent_features,
  ValNodePtr PNTR segment_clauses
);

static Boolean FeatureIsOnSegment (
  SeqFeatPtr sfp, 
  ValNodePtr segment_features
)
{
  ValNodePtr vnp, featclause;
  FeatureClausePtr fcp;

  for (vnp = segment_features; vnp != NULL; vnp = vnp->next)
  {
    fcp = vnp->data.ptrvalue;
    if (fcp != NULL)
    {
      for (featclause = fcp->featlist;
           featclause != NULL;
           featclause = featclause->next)
      {
        if (featclause->data.ptrvalue == sfp) return TRUE;
      }
    }
  }
  return FALSE;
}

static Boolean FeatureClauseIsOnSegment (
  FeatureClausePtr fcp,
  ValNodePtr segment_features
)
{
  ValNodePtr vnp;

  if (fcp == NULL || fcp->featlist == NULL) return FALSE;
  for (vnp = fcp->featlist; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->choice == DEFLINE_FEATLIST 
      && FeatureIsOnSegment (vnp->data.ptrvalue, segment_features))
    {
      return TRUE;
    }
    else if (vnp->choice == DEFLINE_CLAUSEPLUS)
    {
      if (FeatureClauseIsOnSegment (vnp->data.ptrvalue, segment_features))
      {
        return TRUE;
      }     
    }
  }
  return FALSE;
}

static FeatureClausePtr CopyMatchingClauses (
  FeatureClausePtr fcp,
  ValNodePtr segment_features
)
{
  FeatureClausePtr new_fcp, copy_clause;
  ValNodePtr       featlist, new_feat;
  Boolean          found_feat_on_segment;
  Boolean          partial5, partial3;

  new_fcp = (FeatureClausePtr) MemNew (sizeof (FeatureClauseData));
  if (new_fcp == NULL) return NULL;
  new_fcp->feature_label_data.pluralizable = 
    fcp->feature_label_data.pluralizable;
  new_fcp->feature_label_data.is_typeword_first = 
    fcp->feature_label_data.is_typeword_first;
  new_fcp->feature_label_data.typeword =
    StringSave (fcp->feature_label_data.typeword);
  new_fcp->feature_label_data.description = 
    StringSave (fcp->feature_label_data.description);
  new_fcp->feature_label_data.productname =
    StringSave (fcp->feature_label_data.productname);
  new_fcp->allelename = StringSave (fcp->allelename);
  new_fcp->interval = StringSave (fcp->interval);
  new_fcp->is_alt_spliced = fcp->is_alt_spliced;
  CheckSeqLocForPartial (fcp->slp, &partial5, &partial3);
  new_fcp->slp = SeqLocMerge (BioseqFindFromSeqLoc (fcp->slp), fcp->slp, NULL,
                                 FALSE, TRUE, FALSE);
  SetSeqLocPartial (new_fcp->slp, partial5, partial3);
  new_fcp->grp = fcp->grp;
  new_fcp->delete_me = FALSE;
  new_fcp->clause_info_only = fcp->clause_info_only;
  new_fcp->featlist = NULL;
  found_feat_on_segment = FALSE;
  for (featlist = fcp->featlist; featlist != NULL; featlist = featlist->next)
  {
    new_feat = NULL;
    if (featlist->choice == DEFLINE_FEATLIST)
    {
      new_feat = ValNodeNew (new_fcp->featlist);
      if (new_feat == NULL) return NULL;
      new_feat->data.ptrvalue = featlist->data.ptrvalue;
      new_feat->choice = DEFLINE_FEATLIST;

      /* some portions of the clause are present for product and gene info */
      /* if they aren't actually on the segment */
      if ( segment_features == NULL
        || FeatureIsOnSegment (new_feat->data.ptrvalue, segment_features))
      {
        found_feat_on_segment = TRUE;
      }
    }
    else if (featlist->choice == DEFLINE_CLAUSEPLUS)
    {
      copy_clause = featlist->data.ptrvalue;
      if ( segment_features == NULL
        || FeatureClauseIsOnSegment ( copy_clause, segment_features))
      {
        new_feat = ValNodeNew (new_fcp->featlist);
        if (new_feat == NULL) return NULL;
        new_feat->data.ptrvalue = CopyMatchingClauses ( copy_clause,
                                                        segment_features);
        new_feat->choice = DEFLINE_CLAUSEPLUS;
      }
    }
    if (new_feat != NULL && new_fcp->featlist == NULL) 
    {
      new_fcp->featlist = new_feat;
    }
  }
  if (found_feat_on_segment)
  {
    new_fcp->clause_info_only = FALSE;
  }
  else
  {
    new_fcp->clause_info_only = TRUE;
  }
  return new_fcp; 
}

static void CopyFeatureList (
  ValNodePtr match_features,
  ValNodePtr parent_features,
  ValNodePtr PNTR new_list
)
{
  ValNodePtr vnp, addvnp;

  for (vnp = parent_features;
       vnp != NULL;
       vnp = vnp->next)
  {
    if (vnp->choice == DEFLINE_FEATLIST 
      && (match_features == NULL
        || FeatureIsOnSegment ( vnp->data.ptrvalue, match_features)))
    {
      addvnp = ValNodeNew (*new_list);
      if (addvnp == NULL) return;
      addvnp->data.ptrvalue = vnp->data.ptrvalue;
      addvnp->choice = DEFLINE_FEATLIST;
      if (*new_list == NULL) *new_list = addvnp;
    }
    else if (vnp->choice == DEFLINE_CLAUSEPLUS
      && (match_features == NULL
        || FeatureClauseIsOnSegment ( vnp->data.ptrvalue, match_features)))
    {
      addvnp = ValNodeNew (*new_list);
      if (addvnp == NULL) return;
      addvnp->data.ptrvalue = CopyMatchingClauses ( vnp->data.ptrvalue,
                                                    match_features);
      addvnp->choice = DEFLINE_CLAUSEPLUS;
      if (*new_list == NULL) *new_list = addvnp;
    }
  }
    
}

static void ExtractSegmentClauses (
  ValNodePtr segment_features,
  ValNodePtr parent_features,
  ValNodePtr PNTR segment_clauses
)
{
  CopyFeatureList (segment_features, parent_features, segment_clauses);
}

typedef struct segmentdeflinedata {
  BioseqPtr  parent_bsp;
  ValNodePtr parent_feature_list;
  Uint1      molecule_type;
  DeflineFeatureRequestList PNTR feature_requests;
  ModifierCombinationPtr m;
  ModifierItemLocalPtr modList;
  OrganismDescriptionModifiersPtr odmp;
  Int2 product_flag;
} SegmentDefLineData, PNTR SegmentDefLinePtr;

typedef struct segmentdeflinefeatureclausedata {
  BioseqPtr  parent_bsp;
  ValNodePtr parent_feature_list;
  Uint1      molecule_type;
  DeflineFeatureRequestList PNTR feature_requests;
  Int2 product_flag;
  Boolean         alternate_splice_flag;
  ValNodePtr PNTR list;
} SegmentDefLineFeatureClauseData, PNTR SegmentDefLineFeatureClausePtr;

typedef struct deflinefeatclause {
  SeqEntryPtr sep;
  BioseqPtr   bsp;
  CharPtr     clauselist;
} DefLineFeatClauseData, PNTR DefLineFeatClausePtr;

static void DefLineFeatClauseListFree (ValNodePtr vnp)
{
  DefLineFeatClausePtr deflist;

  if (vnp == NULL) return;
  DefLineFeatClauseListFree (vnp->next);
  vnp->next = NULL;
  deflist = vnp->data.ptrvalue;
  if (deflist != NULL)
  {
    MemFree (deflist->clauselist);
    MemFree (deflist);
  }
  ValNodeFree (vnp);
}


static Boolean IntervalIntersectsIvals 
(Int2    numivals,
 Int4Ptr ivals,
 SeqMgrSegmentContextPtr context)
{
  Int2 idx;
  Int4 start, stop;

  if (numivals == 0 || ivals == NULL || context == NULL) return FALSE;

  for (idx = 0; idx < numivals; idx ++) {
    start = ivals [idx * 2];
    stop = ivals [idx * 2 + 1];
    if ( start <= context->cumOffset + context->to - context->from
         && stop >= context->cumOffset)
    {
      return TRUE;
    }
  }
  return FALSE;
}


/* if there are no features at all on this segment, select the genes that 
 * traverse the segment.
 */
static ValNodePtr GrabTraversingGenes 
(ValNodePtr              parent_feature_list,
 SeqMgrSegmentContextPtr context,
 BioseqPtr               parent_bsp)
{
  FeatureClausePtr  fcp, new_fcp;
  ValNodePtr        clause;
  SeqFeatPtr        sfp;
  ValNodePtr        segment_feature_list;
  ValNodePtr        vnp;

  segment_feature_list = NULL;
  for (clause = parent_feature_list;
       clause != NULL;
       clause = clause->next)
  {
    fcp = clause->data.ptrvalue;

    if (fcp != NULL  &&  fcp->featlist != NULL
        &&  (sfp = fcp->featlist->data.ptrvalue) != NULL
        &&  sfp->idx.subtype == FEATDEF_GENE
        &&  fcp->ivals != NULL && fcp->numivals > 0)
    {
      if (IntervalIntersectsIvals (fcp->numivals, fcp->ivals, context)) {
        new_fcp = NewFeatureClause (fcp->featlist->data.ptrvalue, parent_bsp);
        if (new_fcp == NULL) return FALSE;
        vnp = ValNodeNew (segment_feature_list);
        if (vnp == NULL) return FALSE;
        vnp->data.ptrvalue = new_fcp;
        vnp->choice = DEFLINE_CLAUSEPLUS;
        if (segment_feature_list == NULL) segment_feature_list = vnp;
      }
    } 
  }
  return segment_feature_list;
}


static CharPtr BuildFeatureClauses (
  BioseqPtr bsp,
  Uint1      molecule_type,
  SeqEntryPtr sep,
  ValNodePtr  PNTR feature_list,
  Boolean     isSegment,
  ValNodePtr  PNTR seg_feature_list,
  Int2 product_flag,
  Boolean     alternate_splice_flag,
  DeflineFeatureRequestList PNTR feature_requests
);

static Boolean LIBCALLBACK GetFeatureClauseForSeg (
  SeqLocPtr slp,
  SeqMgrSegmentContextPtr context
)
{
  SegmentDefLineFeatureClausePtr sdlp;
  ValNodePtr        clause, tmp_parent_list;
  FeatureClausePtr  fcp, new_fcp;
  Int2              idx;
  Int4              start, stop;
  ValNodePtr        segment_feature_list, vnp;
  SeqIdPtr          sip;
  BioseqPtr         bsp;
  Uint2             entityID;
  SeqLocPtr         loc;
  DefLineFeatClausePtr deflist;

  if (slp == NULL || context == NULL) return FALSE;
  sdlp = (SegmentDefLineFeatureClausePtr) context->userdata;

  sip = SeqLocId (slp);
  
  if (sip == NULL) {
    loc = SeqLocFindNext (slp, NULL);
    if (loc != NULL) {
      sip = SeqLocId (loc);
    }
  }
  if (sip == NULL) return TRUE;

  bsp = BioseqFind (sip);

  if (bsp == NULL) return TRUE;


  segment_feature_list = NULL;
  for (clause = sdlp->parent_feature_list;
       clause != NULL;
       clause = clause->next)
  {
    fcp = clause->data.ptrvalue;

    if (fcp != NULL && fcp->ivals != NULL && fcp->numivals > 0)
    {
      idx = (fcp->numivals - 1) * 2;
      start = fcp->ivals [idx];
      stop = fcp->ivals [idx + 1];
      if ( stop <= context->cumOffset + context->to - context->from
           && stop >= context->cumOffset)
      {
        new_fcp = NewFeatureClause (fcp->featlist->data.ptrvalue,
                                    sdlp->parent_bsp);
        if (new_fcp == NULL) return FALSE;
        vnp = ValNodeNew (segment_feature_list);
        if (vnp == NULL) return FALSE;
        vnp->data.ptrvalue = new_fcp;
        vnp->choice = DEFLINE_CLAUSEPLUS;
        if (segment_feature_list == NULL) segment_feature_list = vnp;
      }
    } 
  }

  if (segment_feature_list == NULL) {
    segment_feature_list = GrabTraversingGenes (sdlp->parent_feature_list,
                                                context, sdlp->parent_bsp);
  }

  entityID = ObjMgrGetEntityIDForPointer (bsp);

  tmp_parent_list = NULL;
  CopyFeatureList (NULL, sdlp->parent_feature_list, &tmp_parent_list);

  deflist = (DefLineFeatClausePtr) MemNew (sizeof (DefLineFeatClauseData));
  if (deflist == NULL) return TRUE;
  deflist->sep = SeqMgrGetSeqEntryForData (bsp);
  deflist->bsp = bsp;
  deflist->clauselist = BuildFeatureClauses (sdlp->parent_bsp,
                            sdlp->molecule_type,
                            SeqMgrGetSeqEntryForData (bsp),
                            &tmp_parent_list,
                            TRUE,
                            &segment_feature_list,
                            sdlp->product_flag,
							sdlp->alternate_splice_flag,
                            sdlp->feature_requests);
  vnp = ValNodeNew (*(sdlp->list));
  if (vnp == NULL) return TRUE;
  if (*(sdlp->list) == NULL) *(sdlp->list) = vnp;
  vnp->data.ptrvalue = deflist;

  FreeListElement (tmp_parent_list);
  FreeListElement (segment_feature_list);
  return TRUE;
}

static CharPtr BuildFeatureClauses (
  BioseqPtr bsp,
  Uint1      molecule_type,
  SeqEntryPtr sep,
  ValNodePtr  PNTR feature_list,
  Boolean     isSegment,
  ValNodePtr  PNTR seg_feature_list,
  Int2 product_flag,
  Boolean     alternate_splice_flag,
  DeflineFeatureRequestList PNTR feature_requests
)
{
  ValNodePtr strings = NULL;
  ValNodePtr clause;
  CharPtr    str = NULL;
  ValNodePtr tmp_feat_list;

  if (feature_requests->feature_list_type == DEFLINE_USE_FEATURES
    && ( ! isSegment || (seg_feature_list != NULL && *seg_feature_list != NULL)))
  {
    GroupmRNAs (feature_list, bsp);

    /* genes are added to other clauses */
    GroupGenes (feature_list);

    if (! feature_requests->suppress_alt_splice_phrase)
    {
      /* find alt-spliced CDSs */
      FindAltSplices (*feature_list, bsp);
    }

    GroupAltSplicedExons (feature_list, bsp, TRUE);

    /* now group clauses */
    GroupAllClauses ( feature_list, bsp );

    ExpandAltSplicedExons (*feature_list, bsp);

    FindGeneProducts (*feature_list, bsp);

    if (seg_feature_list != NULL && *seg_feature_list != NULL)
    {
      tmp_feat_list = NULL; 
      ExtractSegmentClauses ( *seg_feature_list, *feature_list, &tmp_feat_list);
      FreeListElement (*feature_list);
      *feature_list = tmp_feat_list;
    }
   
    /* remove exons and other unwanted features */
    RemoveUnwantedFeatures (feature_list, bsp, isSegment, feature_requests);

    RemoveGenesMentionedElsewhere (feature_list, *feature_list, TRUE);

    if (feature_requests->remove_subfeatures)
    {
      DeleteSubfeatures (feature_list, TRUE);
    }

    DeleteOperonSubfeatures (feature_list, TRUE);

    CountUnknownGenes (feature_list, bsp);

    if (feature_requests->misc_feat_parse_rule == 1)
    {
      RenameMiscFeats (*feature_list, molecule_type);
    }
    else
    {
      RemoveUnwantedMiscFeats (feature_list, TRUE);
    }

    ReplaceRNAClauses (feature_list, bsp);

    /* take any exons on the minus strand */
    /* and reverse their order within the clause */
    ReverseClauses (feature_list, IsExon);

    RenameExonSequences ( feature_list, bsp, TRUE);

    LabelClauses (*feature_list, molecule_type, bsp);

    ConsolidateClauses (feature_list, bsp, molecule_type, TRUE);

    /* this allows genes to be listed together even if they are from */
    /* separate sequences */
/*    SmashTallClauses (feature_list, TRUE); */

    clause = *feature_list;
    ListClauses (clause, &strings, TRUE, FALSE);

    AutoDef_AddEnding (clause, &strings, bsp, 
                       product_flag, alternate_splice_flag);

    str = MergeValNodeStrings (strings, FALSE);
	ValNodeFreeData (strings);
  }
  else if ( feature_requests->feature_list_type == DEFLINE_COMPLETE_SEQUENCE)
  {
    str = StringSave (", complete sequence.");
  }
  else if ( feature_requests->feature_list_type == DEFLINE_COMPLETE_GENOME)
  {
    str = StringSave (", complete genome.");
  }
  else
  {
    str = StringSave ("");
  }
  return str;
}

/* This function looks at the product names for the CDSs on the Bioseq,
 * and sets the flag for the "nuclear genes for X products" ending
 * based on the contents of the CDS products. */
static Int2 GetProductFlagFromCDSProductNames (BioseqPtr bsp)
{
  SeqMgrFeatContext context;
  SeqFeatPtr        cds = NULL;
  Int2              product_flag;
  Int2              i;

  product_flag = 0;
  for (cds = SeqMgrGetNextFeature (bsp, cds, SEQFEAT_CDREGION, 0, &context); cds != NULL && product_flag == 0; cds = cds->next)
  {
	for (i = 1; organelleByPopup[i] != NULL && product_flag == 0; i++)
	{
      if (StringStr (context.label, organelleByPopup[i]))
	  {
	    product_flag = i;
	  }
	}
  }

  return product_flag;
}

static void BuildDefLineFeatClauseList (
  SeqEntryPtr sep,
  Uint2 entityID,
  DeflineFeatureRequestList PNTR feature_requests,
  Int2 product_flag,
  Boolean alternate_splice_flag,
  ValNodePtr PNTR list
)
{
  BioseqSetPtr    bssp;
  BioseqPtr    bsp;
  ValNodePtr    head;
  Uint1      molecule_type;
  SeqEntryPtr   nsep;
  SegmentDefLineFeatureClauseData sdld;
  DefLineFeatClausePtr deflist;
  ValNodePtr    vnp;

  if (sep == NULL || list == NULL) return;

  if ( IS_Bioseq_set (sep))
  {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp == NULL) return;
    if ( bssp->_class == 7 || IsPopPhyEtcSet (bssp->_class))
    {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next)
      {
        BuildDefLineFeatClauseList (sep, entityID, feature_requests,
                                    product_flag, alternate_splice_flag, list);
      }
      return;
    }
  }
    
  nsep = FindNucSeqEntry (sep);
  if (nsep != NULL)
  {
    bsp = (BioseqPtr) nsep->data.ptrvalue;
    if ( SpecialHandlingForSpecialTechniques (bsp)) return;
    if (bsp != NULL && bsp->repr == Seq_repr_seg &&
      bsp->seq_ext != NULL && bsp->seq_ext_type == 1) 
    {
      /* get default product flag if necessary */
	  if (product_flag == -1 || product_flag == DEFAULT_ORGANELLE_CLAUSE) {
	    product_flag = GetProductFlagFromCDSProductNames (bsp);
	  }

      sdld.parent_bsp = bsp;
      sdld.molecule_type = GetMoleculeType (bsp, entityID);
      sdld.parent_feature_list = GetFeatureList (bsp);

      sdld.feature_requests =  feature_requests;
      sdld.product_flag = product_flag;
	  sdld.alternate_splice_flag = alternate_splice_flag;
      sdld.list = list;
      SeqMgrExploreSegments (bsp, (Pointer) &sdld, GetFeatureClauseForSeg);
      deflist = (DefLineFeatClausePtr) MemNew (sizeof (DefLineFeatClauseData));
      if (deflist == NULL) return;
      deflist->sep = SeqMgrGetSeqEntryForData (bsp),
      deflist->bsp = bsp;


      deflist->clauselist = BuildFeatureClauses (bsp,
                            sdld.molecule_type,
                            SeqMgrGetSeqEntryForData (bsp),
                            &sdld.parent_feature_list,
                            FALSE,
                            NULL,
                            product_flag,
							alternate_splice_flag,
                            feature_requests);
      vnp = ValNodeNew (*list);
      if (vnp == NULL) return;
      if (*list == NULL) *list = vnp;
      vnp->data.ptrvalue = deflist;
      FreeListElement (sdld.parent_feature_list);
      return;
    }
  }

  if (nsep != NULL && nsep != sep)
    sep = nsep;

  /* get list of all recognized features */
  bsp = (BioseqPtr) sep->data.ptrvalue;
  if (bsp == NULL) return;
  if ( SpecialHandlingForSpecialTechniques (bsp)) return;
  molecule_type = GetMoleculeType (bsp, entityID);
  head = GetFeatureList (bsp);

  /* get default product flag if necessary */
  if (product_flag == -1 || product_flag == DEFAULT_ORGANELLE_CLAUSE) {
    product_flag = GetProductFlagFromCDSProductNames (bsp);
  }

  deflist = (DefLineFeatClausePtr) MemNew (sizeof (DefLineFeatClauseData));
  if (deflist == NULL) return;
  deflist->sep = SeqMgrGetSeqEntryForData (bsp),
  deflist->bsp = bsp;
  deflist->clauselist = BuildFeatureClauses (bsp,
                                             molecule_type,
                                             SeqMgrGetSeqEntryForData (bsp),
                                             &head,
                                             FALSE,
                                             NULL,
                                             product_flag,
											 alternate_splice_flag,
                                             feature_requests);
  vnp = ValNodeNew (*list);
  if (vnp == NULL) return;
  if (*list == NULL) *list = vnp;
  vnp->data.ptrvalue = deflist;
  FreeListElement (head);

}

static Boolean AreFeatureClausesUnique (ValNodePtr list)
{
  ValNodePtr vnp1, vnp2;
  DefLineFeatClausePtr deflist1, deflist2;

  for (vnp1 = list; vnp1 != NULL && vnp1->next != NULL; vnp1 = vnp1->next)
  {
    deflist1 = vnp1->data.ptrvalue;
    if (deflist1 == NULL || deflist1->clauselist == NULL) return FALSE;
    
    for (vnp2 = vnp1->next; vnp2 != NULL; vnp2 = vnp2->next)
    {
      deflist2 = vnp2->data.ptrvalue;
      if (deflist2 == NULL || deflist2->clauselist == NULL
        || StringCmp (deflist1->clauselist, deflist2->clauselist) == 0)
      {
        return FALSE;
      }
    }
  }
  return TRUE;
}

static void BuildDefinitionLinesFromFeatureClauseLists (
  ValNodePtr list,
  ModifierItemLocalPtr modList,
  ModifierCombinationPtr m,
  OrganismDescriptionModifiers odmp
);

typedef struct deflineformdata {
  FEATURE_FORM_BLOCK

  ModifierItemLocalPtr modList;
  DeflineFeatureRequestList feature_requests;
  GrouP     sourceListGrp;
  PopuP     customGrp;
  ButtoN    use_labels;
  ButtoN    keep_paren;
  ButtoN    exclude_sp;
  GrouP     clone_isolate_HIV_rule_num;
  PopuP     modLimit;
  PopuP     organelle_popup;
  BioseqPtr target_bsp;
  ButtoN    modify_only_target;
  ButtoN    suppress_alt_splice_phrase;
  ButtoN    remove_subfeatures;
  PopuP     featurePopup;
  GrouP     featureOptsGrp;
  PopuP     misc_feat_parse_rule;
  ButtoN    alternate_splice_flag;
} DefLineFormData, PNTR DefLineFormPtr;

static void DefLineFormMessageProc (ForM f, Int2 mssg)

{
  DefLineFormPtr  dlfp;

  dlfp = (DefLineFormPtr) GetObjectExtra (f);
  if (dlfp != NULL) {
    if (dlfp->appmessage != NULL) {
      dlfp->appmessage (f, mssg);
    }
  }
}

static void CleanupDefLineForm (
  GraphiC g,
  VoidPtr data
)

{
  DefLineFormPtr  dlfp;
  Int4            i;

  dlfp = (DefLineFormPtr) data;
  if (dlfp != NULL) {
    if (dlfp->modList != NULL)
    {
      for (i=0; i < NumDefLineModifiers; i++)
      {
        ValNodeFree (dlfp->modList[i].values_seen);
      }
      MemFree (dlfp->modList);
    }
  }
  StdCleanupFormProc (g, data);
}

static void ChangeCustomPopup (PopuP p)

{
  DefLineFormPtr  dlfp;

  dlfp = (DefLineFormPtr) GetObjectExtra (p);
  if (dlfp == NULL) return;
  if (GetValue (p) == 1) {
    SafeDisable (dlfp->sourceListGrp);
  } else {
    SafeEnable (dlfp->sourceListGrp);
  }
}

static void ChangeFeaturePopup (PopuP p)
{
  DefLineFormPtr  dlfp;

  dlfp = (DefLineFormPtr) GetObjectExtra (p);
  if (dlfp == NULL) return;
  if (GetValue (p) == 2 || GetValue (p) == 3)
  {
    SafeDisable (dlfp->featureOptsGrp);
  }
  else
  {
    SafeEnable (dlfp->featureOptsGrp);
  }
}

static void DoAutoDefLine (ButtoN b)
{
  DefLineFormPtr dlfp;
  SeqEntryPtr sep;
  ModifierCombinationPtr m;
  Int2 feature_index;
  OrganismDescriptionModifiers odmp;
  Int2 product_flag, feature_list_type;
  Int4 i;
  ValNodePtr defline_clauses = NULL;
  Boolean alternate_splice_flag;

  dlfp = GetObjectExtra (b);
  if (b == NULL) return;
  Hide (dlfp->form);
  WatchCursor ();
  Update ();

  odmp.use_labels = GetStatus (dlfp->use_labels);
  odmp.keep_paren = GetStatus (dlfp->keep_paren);
  odmp.exclude_sp = GetStatus (dlfp->exclude_sp);
  odmp.use_modifiers = TRUE;

  if (dlfp->clone_isolate_HIV_rule_num == NULL)
  {
    odmp.clone_isolate_HIV_rule_num = clone_isolate_HIV_rule_want_both;
  }
  else
  {
    odmp.clone_isolate_HIV_rule_num = GetValue (dlfp->clone_isolate_HIV_rule_num);
  }

  /* create combo with the specified modifiers */
  m = NewModifierCombo (NULL);
  if (m == NULL) return;
  if (GetValue (dlfp->customGrp) == 1)
  {
    /* take all features */
    for (feature_index = 0; feature_index < NumDefLineModifiers; feature_index++)
    {
      if (dlfp->modList[feature_index].any_present)
      {
        AddQualToModifierCombo (m, dlfp->modList + feature_index, feature_index);
      }
    }
  }
  else
  {
    /* take selected features */
    for (feature_index = 0; feature_index < NumDefLineModifiers; feature_index++)
    {
      if (GetStatus (dlfp->modList[feature_index].button))
      {
        AddQualToModifierCombo (m, dlfp->modList + feature_index, feature_index);
      }
    }
  }

  feature_list_type = GetValue (dlfp->featurePopup);
  switch (feature_list_type)
  {
    case DEFLINE_USE_FEATURES :
      dlfp->feature_requests.feature_list_type = DEFLINE_USE_FEATURES;
      break;
    case DEFLINE_COMPLETE_SEQUENCE :
      dlfp->feature_requests.feature_list_type = DEFLINE_COMPLETE_SEQUENCE;
      break;
    case DEFLINE_COMPLETE_GENOME :
      dlfp->feature_requests.feature_list_type = DEFLINE_COMPLETE_GENOME;
      break;
    default:
      dlfp->feature_requests.feature_list_type = DEFLINE_USE_FEATURES;
      break;
  }

  for (i=0; i< NumRemovableItems; i++)
  {
    dlfp->feature_requests.items[i].keep 
              = GetStatus (dlfp->feature_requests.items[i].keep_request);
  }

  dlfp->feature_requests.suppress_alt_splice_phrase = 
                 GetStatus (dlfp->suppress_alt_splice_phrase);

  dlfp->feature_requests.remove_subfeatures = 
                 GetStatus (dlfp->remove_subfeatures);

  dlfp->feature_requests.misc_feat_parse_rule = 
                 GetValue (dlfp->misc_feat_parse_rule);

  odmp.max_mods = GetValue (dlfp->modLimit);
  if (odmp.max_mods > 1)
  {
    odmp.max_mods = odmp.max_mods - 1;
  }
  else
  {
    odmp.max_mods = -99;
  }

  product_flag = GetValue (dlfp->organelle_popup) - 1;
  alternate_splice_flag = GetStatus (dlfp->alternate_splice_flag);
 
  if (dlfp->target_bsp != NULL && GetStatus (dlfp->modify_only_target))
  {
    sep = GetBestTopParentForData (dlfp->input_entityID, dlfp->target_bsp);
  }
  else
  { 
    sep = GetTopSeqEntryForEntityID (dlfp->input_entityID);
  }
  if (sep == NULL) return;

  SeqEntrySetScope (sep);

  BuildDefLineFeatClauseList (sep, dlfp->input_entityID,
                              &dlfp->feature_requests,
                              product_flag, alternate_splice_flag,
                              &defline_clauses);

  BuildDefinitionLinesFromFeatureClauseLists (defline_clauses, dlfp->modList,
                                                m, odmp);
  DefLineFeatClauseListFree (defline_clauses);
  ClearProteinTitlesInNucProts (dlfp->input_entityID, NULL);
  InstantiateProteinTitles (dlfp->input_entityID, NULL);
  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (dlfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, dlfp->input_entityID, 0, 0);
  Remove (dlfp->form);
  SeqEntrySetScope (NULL);
}

static void IsTricky (
  BioSourcePtr biop,
  Pointer userdata
)
{
  Boolean PNTR pTricky;
  static CharPtr long_name = "Human immunodeficiency virus";
  static CharPtr short_name = "HIV-";
  OrgModPtr     mod;
  OrgNamePtr    onp;
  SubSourcePtr  ssp;

  pTricky = (Boolean PNTR) userdata;
  if (pTricky == NULL
    || *pTricky
    || biop == NULL
    || biop->org == NULL
    || biop->org->taxname == NULL
    || biop->org->orgname == NULL
    || biop->subtype == NULL)
  {
    return;
  }
  if (StringNICmp (biop->org->taxname, long_name, StringLen (long_name)) != 0
    && StringNICmp (biop->org->taxname, short_name, StringLen (short_name)) != 0)
  {
    return;
  }

  onp = biop->org->orgname;
  if (onp == NULL) return;
  mod = onp->mod;
  while (mod != NULL
    && mod->subtype != ORGMOD_isolate)
  {
    mod = mod->next;
  }
  if (mod == NULL || mod->subname == NULL)
  {
    return;
  }

  ssp = biop->subtype;
  while (ssp != NULL && ssp->subtype != SUBSRC_clone)
  {
    ssp = ssp->next;
  }
  if (ssp == NULL || ssp->name == NULL)
  {
    return;
  }
  *pTricky = TRUE;
  return;
}

static Boolean HasTrickyHIVRecords (
  BaseFormPtr bfp
)
{
  SeqEntryPtr sep;
  Boolean     has_tricky;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return FALSE;
  
  has_tricky = FALSE;
  VisitBioSourcesInSep (sep, &has_tricky, IsTricky);
  return has_tricky;
}

static GrouP CreateDefLineFormHIVRule (
  GrouP h
)
{
  GrouP hiv_rule;

  hiv_rule = NormalGroup (h, 3, 0, "HIV rule", programFont, NULL);
  RadioButton (hiv_rule, "Prefer Clone");
  RadioButton (hiv_rule, "Prefer Isolate");
  RadioButton (hiv_rule, "Want Both Isolate and Clone");
  SetValue (hiv_rule, clone_isolate_HIV_rule_want_both);
  return hiv_rule;
}

static PopuP CreateDefLineFormModLimitPopup (
  GrouP q,
  Int2 count
)
{
  PopuP limit;
  Int2  i;
  Char  str[10];

  StaticPrompt (q, "Maximum modifiers per line", 0, popupMenuHeight, programFont, 'l');
  limit = PopupList (q, TRUE, NULL);
  PopupItem (limit, "no limit");
  for (i = 1; i <= count; i++) {
    sprintf (str, "%d", (int) i);
    PopupItem (limit, str);
  }

  SetValue (limit, 1);
  
  return limit;
}

static PopuP CreateDefLineFormModifierListPopuP (
  GrouP          q,
  DefLineFormPtr dlfp
)
{
  PopuP popup;
  StaticPrompt (q, "Modifier List", 0, popupMenuHeight, programFont, 'l');
  popup = PopupList (q, TRUE, ChangeCustomPopup);
  SetObjectExtra (popup, dlfp, NULL);
  PopupItem (popup, "All");
  PopupItem (popup, "Custom");
  SetValue (popup, 2);
  
  return popup;
}
  
static void SetHIVRuleEnable (Handle a)
{
  DefLineFormPtr dlfp;
 
  dlfp = GetObjectExtra (a);
  if (dlfp == NULL) return;
  if (dlfp->clone_isolate_HIV_rule_num == NULL) return;
  if (dlfp->modList == NULL) return;
  if (dlfp->modList [DEFLINE_POS_Clone].button != NULL
    && GetStatus (dlfp->modList [DEFLINE_POS_Clone].button)
    && dlfp->modList [DEFLINE_POS_Isolate].button != NULL
    && GetStatus (dlfp->modList [DEFLINE_POS_Isolate].button))
  {
    SetValue (dlfp->clone_isolate_HIV_rule_num, 
              clone_isolate_HIV_rule_want_both);
    Disable (dlfp->clone_isolate_HIV_rule_num);
  }
  else
  {
    Enable (dlfp->clone_isolate_HIV_rule_num);
  }
}

static GrouP CreateDefLineFormModifierClassGroup (
  GrouP          h,
  DefLineFormPtr dlfp,
  Int2 PNTR      count
)
{
  GrouP p, q;
  Int2    item_index, num_label_columns, num_item_rows;
  Int2    num_item_columns;

  p = HiddenGroup (h, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);
  q = HiddenGroup (p, 3, 0, NULL);
  SetGroupSpacing (q, 10, 10);
  dlfp->customGrp = CreateDefLineFormModifierListPopuP (q, dlfp);

  (*count) = 0;
  for (item_index=0; item_index < NumDefLineModifiers; item_index++)
  {
    if (dlfp->modList[item_index].any_present)
    {
      (*count)++;
    }
  }

  num_item_rows = 6;
  if (*count > 18)
    num_item_rows = 8;
  if (*count > 24)
    num_item_rows = 10;
  if (*count > 30)
    num_item_rows = 12;

  num_item_columns = (*count) / num_item_rows;
  if ((*count) % num_item_rows != 0) num_item_columns ++;

  num_label_columns = 3;
  if (*count > 6)
    num_label_columns --;
  if (*count > 12)
    num_label_columns --;

  dlfp->sourceListGrp = NormalGroup (p,
                                     num_item_columns * num_label_columns,
                                     0,
                                     "Available Modifiers",
                                     programFont, NULL);

  for (item_index=0; item_index < NumDefLineModifiers; item_index++)
  {
    if (dlfp->modList[item_index].any_present)
    {
      dlfp->modList[item_index].button = CheckBox (dlfp->sourceListGrp,
                                             DefLineModifiers[item_index].name,
                                             (BtnActnProc) SetHIVRuleEnable);
      SetObjectExtra (dlfp->modList[item_index].button, dlfp, NULL);
      SetStatus (dlfp->modList[item_index].button,
                 dlfp->modList[item_index].required);
      
      if (num_label_columns > 1)
      {
        StaticPrompt (dlfp->sourceListGrp,
                      dlfp->modList[item_index].status,
                      0, popupMenuHeight, programFont, 'l');
        if (num_label_columns > 2)
        {
          StaticPrompt (dlfp->sourceListGrp,
                        dlfp->modList[item_index].first_value_seen,
                        0, popupMenuHeight, programFont, 'l');
        }
      }
    }
    else
    {
      dlfp->modList[item_index].button = NULL;
      dlfp->modList[item_index].required = FALSE;
    }
  }
  Enable (dlfp->sourceListGrp);

  AlignObjects (ALIGN_CENTER, (HANDLE) q,
                 (HANDLE) dlfp->sourceListGrp, NULL); 
  return p;
}

static PopuP CreateDefLineFormOrganellePopup (
  GrouP h
)
{
  PopuP o;

  o = PopupList (h, FALSE, NULL);
  PopupItem (o, "No mitochondrial or chloroplast suffix");
  PopupItem (o, "Nuclear gene(s) for mitochondrial product(s)");
  PopupItem (o, "Nuclear gene(s) for chloroplast product(s)");
  PopupItem (o, "Nuclear gene(s) for kinetoplast product(s)");
  PopupItem (o, "Nuclear gene(s) for plastid product(s)");
  PopupItem (o, "Nuclear gene(s) for chromoplast product(s)");
  PopupItem (o, "Nuclear gene(s) for cyanelle product(s)");
  PopupItem (o, "Nuclear gene(s) for apicoplast product(s)");
  PopupItem (o, "Nuclear gene(s) for leucoplast product(s)");
  PopupItem (o, "Nuclear gene(s) for proplastid product(s)");
  PopupItem (o, "Nuclear genes based on CDS product names");
  SetValue (o, DEFAULT_ORGANELLE_CLAUSE + 1);
  return o;
}

static GrouP CreateDefLineFormFeatureListPopuP (
  GrouP          g,
  DefLineFormPtr dlfp
)
{
  GrouP q;

  q = HiddenGroup (g, 2, 0, NULL);
  StaticPrompt (q, "Feature List", 0, popupMenuHeight, programFont, 'l');
  dlfp->featurePopup = PopupList (q, TRUE, ChangeFeaturePopup);
  SetObjectExtra (dlfp->featurePopup, dlfp, NULL);
  PopupItem (dlfp->featurePopup, "List Features");
  PopupItem (dlfp->featurePopup, "Complete Sequence");
  PopupItem (dlfp->featurePopup, "Complete Genome");
  SetValue (dlfp->featurePopup, 1);
  
  return q;
}

static void SetMiscFeatRuleEnable (Handle a)
{
  DefLineFormPtr dlfp;

  if (a == NULL || ( dlfp = (DefLineFormPtr) GetObjectExtra (a)) == NULL)
  {
    return;
  }
  if (dlfp->feature_requests.items != NULL
    && GetStatus (dlfp->feature_requests.items[RemovableNoncodingProductFeat].keep_request))
  {
    Enable (dlfp->misc_feat_parse_rule);
  }
  else
  {
    Disable (dlfp->misc_feat_parse_rule);
  }
}
  
static GrouP CreateDefLineFormFeatureOptionsGroup (
  GrouP          h,
  DefLineFormPtr dlfp
)
{
  GrouP p, q, g, r;
  Int4  i;

  p = HiddenGroup (h, -1, 0, NULL);
  SetGroupSpacing (p, 10, 10);

  q = CreateDefLineFormFeatureListPopuP (p, dlfp);

  dlfp->featureOptsGrp = HiddenGroup (p, -1, 0, NULL);
  dlfp->organelle_popup = CreateDefLineFormOrganellePopup (dlfp->featureOptsGrp);
  dlfp->alternate_splice_flag = CheckBox (dlfp->featureOptsGrp,
	                "Append 'alternatively spliced' to end of defline", NULL);
  dlfp->suppress_alt_splice_phrase = CheckBox (dlfp->featureOptsGrp, 
                    "Suppress alternative splice phrase for features", NULL);
  SetStatus (dlfp->suppress_alt_splice_phrase, FALSE);

  dlfp->remove_subfeatures = CheckBox (dlfp->featureOptsGrp, 
            "Suppress transposon and insertion sequence subfeatures", NULL);
  SetStatus (dlfp->remove_subfeatures, FALSE);

  g = NormalGroup (dlfp->featureOptsGrp, 3, 0,
                   "Optional Features", programFont, NULL);
  
  for (i=0; i < NumRemovableItems; i++)
  {
    if (i != RemovableCDS)
    {
      dlfp->feature_requests.items[i].keep_request = 
                     CheckBox (g, remove_items[i].group_name, 
                               (BtnActnProc) SetMiscFeatRuleEnable);
      SetObjectExtra (dlfp->feature_requests.items[i].keep_request, dlfp, NULL);
      SetStatus (dlfp->feature_requests.items[i].keep_request, FALSE);
    }
  }
  r = HiddenGroup (p, 2, 0, NULL);
  StaticPrompt (r, "Misc Feat Parse Rule", 0, dialogTextHeight, programFont, 'l');
  dlfp->misc_feat_parse_rule = PopupList (r, TRUE, NULL);
  PopupItem (dlfp->misc_feat_parse_rule, "Product is comment before first semicolon");
  PopupItem (dlfp->misc_feat_parse_rule, "Look for Noncoding Product Features");
  SetValue (dlfp->misc_feat_parse_rule, 2);
  Disable (dlfp->misc_feat_parse_rule);

  AlignObjects (ALIGN_CENTER, (HANDLE) q,
                 (HANDLE) dlfp->featureOptsGrp, NULL); 
  AlignObjects (ALIGN_CENTER, (HANDLE) dlfp->organelle_popup,
                 (HANDLE) dlfp->alternate_splice_flag,
                 (HANDLE) dlfp->suppress_alt_splice_phrase,
                 (HANDLE) dlfp->remove_subfeatures,
                 (HANDLE) g,
                 (HANDLE) r,
                 NULL);

  return p;
}

static void CreateDefLineForm (
  BaseFormPtr bfp,
  ModifierItemLocalPtr modList,
  DeflineFeatureRequestList *feature_requests
)
{
  DefLineFormPtr dlfp;
  WindoW         w;
  GrouP          h, k, p, q, c, feat_opts;
  ButtoN         b;
  Int2           count;
  Boolean        has_tricky;

  dlfp = MemNew (sizeof (DefLineFormData));
  if (dlfp == NULL) return;
  dlfp->input_entityID = bfp->input_entityID;
  dlfp->input_itemID = bfp->input_itemID;
  dlfp->input_itemtype = bfp->input_itemtype;

  w = FixedWindow (-50, -33, -10, -10, "Automatic Definition Line",
  StdCloseWindowProc);
  SetObjectExtra (w, dlfp, CleanupDefLineForm);
  dlfp->form = (ForM) w;
  dlfp->formmessage = DefLineFormMessageProc;

  dlfp->modList = modList;
  dlfp->feature_requests = *feature_requests;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  k = HiddenGroup (h, -1, 0, NULL);
  SetGroupSpacing (k, 10, 10);
  dlfp->use_labels = CheckBox (k, "Use labels (e.g., 'strain BALB/c'", NULL);
  SetStatus (dlfp->use_labels, TRUE);

  p = CreateDefLineFormModifierClassGroup (h, dlfp, &count);

  has_tricky = HasTrickyHIVRecords (bfp);
  if (has_tricky)
  {
    dlfp->clone_isolate_HIV_rule_num = CreateDefLineFormHIVRule (h);
    SetObjectExtra (dlfp->clone_isolate_HIV_rule_num, dlfp, NULL);
    SetHIVRuleEnable (dlfp->clone_isolate_HIV_rule_num);
  }
  else
  {
    dlfp->clone_isolate_HIV_rule_num = NULL;
  }
  q = HiddenGroup (h, 4, 0, NULL);
  dlfp->modLimit = CreateDefLineFormModLimitPopup (q, count);
  dlfp->keep_paren = CheckBox (h, "Leave in parenthetical organism info", NULL);
  SetStatus (dlfp->keep_paren, TRUE);
  dlfp->exclude_sp = CheckBox (h, "Do not apply modifier to 'sp.' organisms",
         NULL);
  SetStatus (dlfp->exclude_sp, TRUE);

  feat_opts = CreateDefLineFormFeatureOptionsGroup (h, dlfp);

  dlfp->target_bsp = GetBioseqGivenIDs (dlfp->input_entityID,
          dlfp->input_itemID,
          dlfp->input_itemtype);
  dlfp->modify_only_target = CheckBox (h, "Only modify targeted record", NULL);
  SetStatus (dlfp->modify_only_target, FALSE);
  if (dlfp->target_bsp == NULL)
  {
    Disable (dlfp->modify_only_target);
  }

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoAutoDefLine);
  SetObjectExtra (b, dlfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  if (has_tricky)
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) p,
                  (HANDLE) dlfp->clone_isolate_HIV_rule_num,
                  (HANDLE) q,
                  (HANDLE) dlfp->keep_paren,
                  (HANDLE) dlfp->exclude_sp,
                  (HANDLE) feat_opts,
                  (HANDLE) dlfp->modify_only_target,
                  (HANDLE) c, NULL);
  }
  else
  {
    AlignObjects (ALIGN_CENTER, (HANDLE) k, (HANDLE) p,
                  (HANDLE) q,
                  (HANDLE) dlfp->keep_paren,
                  (HANDLE) dlfp->exclude_sp,
                  (HANDLE) feat_opts,
                  (HANDLE) dlfp->modify_only_target,
                  (HANDLE) c, NULL);
  }
  RealizeWindow (w);
  Show (w);
  Update ();
}

static void BuildDefinitionLinesFromFeatureClauseLists (
  ValNodePtr list,
  ModifierItemLocalPtr modList,
  ModifierCombinationPtr m,
  OrganismDescriptionModifiers odmp
)
{
  ValNodePtr vnp;
  DefLineFeatClausePtr defline_featclause;
  CharPtr    org_desc, tmp_str;

  for (vnp = list; vnp != NULL; vnp = vnp->next)
  {
    if (vnp->data.ptrvalue != NULL)
    {
      defline_featclause = vnp->data.ptrvalue;
      org_desc = GetOrganismDescription (defline_featclause->bsp,
                                         modList, m, &odmp);
      tmp_str = (CharPtr) MemNew ( StringLen (org_desc) + StringLen (defline_featclause->clauselist) + 2);
      if (tmp_str == NULL) return;
      StringCpy (tmp_str, org_desc);
      if (defline_featclause->clauselist != NULL
        && defline_featclause->clauselist [0] != ','
        && defline_featclause->clauselist [0] != '.'
        && defline_featclause->clauselist [0] != 0)
      {
        StringCat (tmp_str, " ");
      }
      StringCat (tmp_str, defline_featclause->clauselist);
      tmp_str [0] = toupper (tmp_str [0]);
      ReplaceDefinitionLine (defline_featclause->sep, tmp_str);
      MemFree (org_desc);
    }
  }
}

extern void AutoDefBaseFormCommon (
  BaseFormPtr bfp,
  Boolean use_form,
  Boolean use_modifiers
)
{
  SeqEntryPtr sep;
  DeflineFeatureRequestList feature_requests;
  ModifierItemLocalPtr modList;
  ModifierCombinationPtr m;
  OrganismDescriptionModifiers odmp;
  Int4 index;
  ValNodePtr defline_clauses = NULL;

  if (bfp == NULL) return;
  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  InitFeatureRequests (&feature_requests);

  modList = MemNew (NumDefLineModifiers * sizeof (ModifierItemLocalData));
  if (modList == NULL) return;
  SetRequiredModifiers (modList);
  CountModifiers (modList, sep);
  if (use_form)
  {
    m = FindBestCombo (sep, modList);
    AddBestComboModifiersToModList ( m, modList);
    CreateDefLineForm (bfp, modList, &feature_requests);
  }
  else
  {
    SeqEntrySetScope (sep);
    WatchCursor ();
    Update ();
    odmp.use_labels = TRUE;
    odmp.max_mods = -99;
    odmp.keep_paren = TRUE;
    odmp.exclude_sp = TRUE;
    odmp.clone_isolate_HIV_rule_num = clone_isolate_HIV_rule_want_both;
    odmp.use_modifiers = use_modifiers;
    ClearProteinTitlesInNucProts (bfp->input_entityID, NULL);
    BuildDefLineFeatClauseList (sep, bfp->input_entityID, &feature_requests,
                                DEFAULT_ORGANELLE_CLAUSE, FALSE, &defline_clauses);
    if ( AreFeatureClausesUnique (defline_clauses))
    {
      m = CreateComboFromModList (modList);
    }
    else
    {
      m = FindBestCombo (sep, modList);
    }

    BuildDefinitionLinesFromFeatureClauseLists (defline_clauses, modList,
                                                m, odmp);
    DefLineFeatClauseListFree (defline_clauses);
    if (modList != NULL)
    {
      for (index=0; index < NumDefLineModifiers; index++)
      {
        ValNodeFree (modList[index].values_seen);
      }
      MemFree (modList);
    }
    ArrowCursor ();
    Update ();
    ObjMgrSetDirtyFlag (bfp->input_entityID, TRUE);  
    ObjMgrSendMsg (OM_MSG_UPDATE, bfp->input_entityID, 0, 0);
    SeqEntrySetScope (NULL);
  }
  FreeModifierCombo (m);
}

static void AutoDefCommon (IteM i, Boolean use_form, Boolean use_modifiers)
{
  BaseFormPtr  bfp;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL || bfp->input_entityID == 0) return;
  AutoDefBaseFormCommon (bfp, use_form, use_modifiers);
}

extern void testAutoDef (IteM i)
{
  AutoDefCommon (i, FALSE, TRUE);
}

extern void testAutoDefWithOptions (IteM i)
{
  AutoDefCommon (i, TRUE, TRUE);
}

extern void AutoDefWithoutModifiers (IteM i)
{
  AutoDefCommon (i, FALSE, FALSE);
}

extern void AutoDefToolBtn (ButtoN b)
{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;
  AutoDefBaseFormCommon (bfp, FALSE, TRUE);
}

extern void AutoDefOptionsToolBtn (ButtoN b)
{
  BaseFormPtr  bfp;

  bfp = (BaseFormPtr) GetObjectExtra (b);
  if (bfp == NULL) return;
  AutoDefBaseFormCommon (bfp, TRUE, TRUE);
}

typedef struct tablelinedata {
  Int4 num_parts;
  ValNodePtr parts;
} TableLineData, PNTR TableLinePtr;

static TableLinePtr MakeTableData (CharPtr line)
{
  TableLinePtr tlp;
  CharPtr p_start, p_end;
  Int4    plen;
  Boolean found_end;

  if (line == NULL) return NULL;
  tlp = MemNew (sizeof (TableLineData));
  if (tlp == NULL) return NULL;
  p_start = line;
  found_end = FALSE;
  while (*p_start != 0 && !found_end)
  {
    plen = StringCSpn (p_start, "\t\n");
    if (plen == 0)
    {
      if (tlp->num_parts > 0)
      {
        ValNodeAddStr (&tlp->parts, 0, StringSave (""));
        tlp->num_parts ++;
      }
      p_start++;
      if (*p_start == 0) {
        if (tlp->num_parts > 0)
        {
          ValNodeAddStr (&tlp->parts, 0, StringSave (""));
          tlp->num_parts ++;
        }
      }
      continue;
    }
    if (plen == StringLen (p_start))
    {
      found_end = TRUE;
    }
    else
    {
      p_end = p_start + plen;
      *p_end = 0;
    }
    TrimSpacesAroundString (p_start);
    ValNodeAddStr (&tlp->parts, 0, StringSave (p_start));
    tlp->num_parts ++;
    if (!found_end)
    {
      p_start = p_end + 1;
    }
  }
  if (tlp->num_parts == 0)
  {
    MemFree (tlp);
    return NULL;
  }
  return tlp;  
}

static void CleanUpTableData (ValNodePtr vnp)
{
  TableLinePtr tlp;

  if (vnp == NULL) return;
  CleanUpTableData (vnp->next);
  vnp->next = NULL;
  tlp = vnp->data.ptrvalue;
  if (tlp != NULL)
  {
    ValNodeFreeData (tlp->parts);
  }
  ValNodeFree (vnp);
}

typedef struct orgmodlineformdata {
  Int4    column_number;
  CharPtr value_string;
  GrouP   action_choice;
  PopuP   match_choice;
  LisT    apply_choice;
  GrouP   container;
} OrgModLineFormData, PNTR OrgModLineFormPtr;

typedef struct orgmodloadformdata {
  FEATURE_FORM_BLOCK

  ValNodePtr        line_list;
  OrgModLineFormPtr line_forms;
  Int4              num_line_forms;
  Uint2             entityID;
  Boolean           replace_with_blank;
  ButtoN            replace_with_blank_btn;
  ButtoN            accept_button;
  
} OrgModLoadFormData, PNTR OrgModLoadFormPtr;
 
static void SetFormModsAcceptButton (Handle a)
{
  OrgModLoadFormPtr form_data;
  Boolean           have_apply;
  Boolean           have_action;
  Int4              column_index, apply_choice;

  form_data = GetObjectExtra (a);
  if (form_data == NULL) return;
 
  have_apply = FALSE;
  have_action = FALSE;
  for (column_index = 0;
       column_index < form_data->num_line_forms;
       column_index ++)
  {
    if ( GetValue (form_data->line_forms [ column_index].action_choice) == 2)
    {
      apply_choice = GetValue (form_data->line_forms[column_index].apply_choice);
      if ( apply_choice > 0 && apply_choice < NumDefLineModifiers)
      {
        have_apply = TRUE;
      }
      else
      {
        Disable (form_data->accept_button);
        return;
      }
    }
    else if (GetValue (form_data->line_forms [column_index].action_choice) == 1
      && GetValue (form_data->line_forms [column_index].match_choice) > 0
      && GetValue (form_data->line_forms [column_index].match_choice) < 6)
    {
      have_action = TRUE;
    }
  }

  /* must have an action selected if there is more than one line */
  if ( ! have_action
    && form_data->line_list != NULL
    && form_data->line_list->next != NULL)
  {
    Disable (form_data->accept_button);
    return;
  }

  if (have_apply)
  {
    Enable (form_data->accept_button);
  }
  else
  {
    Disable (form_data->accept_button);
  }
}

static void BuildOrgModLineForm (
  CharPtr           value_string,
  Int4              column_number,
  GrouP             parent_group,
  OrgModLineFormPtr omlfp,
  OrgModLoadFormPtr parent_form
)
{
  GrouP g;
  Int4  index;

  omlfp->column_number = column_number;
  omlfp->value_string = value_string;
  omlfp->container = NormalGroup (parent_group, 3, 0, value_string,
                                  programFont, NULL);
  omlfp->action_choice = HiddenGroup (omlfp->container, 0, 3,
                                      (GrpActnProc) SetFormModsAcceptButton);
  SetObjectExtra (omlfp->action_choice, parent_form, NULL);
  SetGroupSpacing (omlfp->action_choice, 10 , 10);
  RadioButton (omlfp->action_choice, "Match to");
  RadioButton (omlfp->action_choice, "Apply to");

  /* list of matchable items */
  g = HiddenGroup (omlfp->container, 0, 2, NULL);
  omlfp->match_choice = PopupList (g, TRUE,
                                   (PupActnProc) SetFormModsAcceptButton);
  SetObjectExtra (omlfp->match_choice, parent_form, NULL);
  PopupItem (omlfp->match_choice, "Accession");
  PopupItem (omlfp->match_choice, "Local ID");
  PopupItem (omlfp->match_choice, "Organism Taxonomy Name");
  PopupItem (omlfp->match_choice, "TMSMART ID");
  PopupItem (omlfp->match_choice, "BankIt ID");
  PopupItem (omlfp->match_choice, "None");
  SetValue (omlfp->match_choice, 6);

  /* list of organism modifiers */
  omlfp->apply_choice = SingleList (g, 5, 8,
                                    (LstActnProc) SetFormModsAcceptButton);
  SetObjectExtra (omlfp->apply_choice, parent_form, NULL);
  for (index = 0; index < NumDefLineModifiers; index++)
  {
    ListItem (omlfp->apply_choice, DefLineModifiers[index].name);
  }

  SetValue (omlfp->action_choice, 1);
}

static void CleanupOrgModLoadForm (
  GraphiC g,
  VoidPtr data
)
{
  OrgModLoadFormPtr form_data;

  form_data = (OrgModLoadFormPtr)data;
  if (form_data == NULL) return;
  MemFree (form_data->line_forms);
  CleanUpTableData (form_data->line_list);
  StdCleanupFormProc (g, data);
} 

static void AddOneQualToOrg (
  BioSourcePtr biop,
  CharPtr      value_string,
  Int4         modifier_index
)
{
  OrgRefPtr     orp;
  OrgModPtr     mod, last_mod;
  OrgNamePtr    onp;
  SubSourcePtr  ssp, last_ssp;

  if (biop == NULL || value_string == NULL) return;

  orp = biop->org;
  if (orp == NULL) return;

  if (DefLineModifiers[modifier_index].isOrgMod)
  {
    onp = orp->orgname;
    if (onp == NULL) {
      onp = OrgNameNew ();
      if (onp == NULL) return;
      orp->orgname = onp;
    }
    mod = onp->mod;
    last_mod = NULL;
    while (mod != NULL
      && mod->subtype != DefLineModifiers[modifier_index].subtype)
    {
      last_mod = mod;
      mod = mod->next;
    }
    if (mod != NULL)
    {
      MemFree (mod->subname);
      mod->subname = StringSave (value_string);
    }
    else
    {
      mod = OrgModNew ();
      mod->subtype = DefLineModifiers[modifier_index].subtype;
      mod->subname = StringSave (value_string);
      if (last_mod == NULL)
      {
        onp->mod = mod;
      }
      else
      {
        last_mod->next = mod;
      }
    }
  } else {
    ssp = biop->subtype;
    last_ssp = NULL;
    while (ssp != NULL && ssp->subtype != DefLineModifiers[modifier_index].subtype)
    {
      last_ssp = ssp;
      ssp = ssp->next;
    }
    if (ssp != NULL)
    {
      MemFree (ssp->name);
      ssp->name = StringSave (value_string);
    }
    else
    {
      ssp = SubSourceNew ();
      ssp->subtype = DefLineModifiers[modifier_index].subtype;
      ssp->name = StringSave (value_string);
      if (last_ssp == NULL)
      {
        biop->subtype = ssp;
      }
      else
      {
        last_ssp->next = ssp;
      }
    }
  }
}
  
static void ApplyQualsToOrg (
  BioSourcePtr biop, 
  ValNodePtr parts,
  OrgModLoadFormPtr form_data
)
{
  Int4       column_index;
  ValNodePtr part;
  Int4       apply_choice;

  if (biop == NULL || parts == NULL || form_data == NULL) return;

  part = parts;
  for (column_index = 0;
       column_index < form_data->num_line_forms && part != NULL;
       column_index ++)
  {
    apply_choice = GetValue (form_data->line_forms[column_index].apply_choice);
    if (GetValue (form_data->line_forms[column_index].action_choice) == 2
      && apply_choice > 0 && apply_choice < NumDefLineModifiers)
    {
      if (form_data->replace_with_blank || ! StringHasNoText (part->data.ptrvalue)) {
        AddOneQualToOrg (biop, part->data.ptrvalue, 
          GetValue (form_data->line_forms[column_index].apply_choice) - 1);
      }
    }
    part = part->next;
  }
}

static void ApplyTableModsByTaxName (BioSourcePtr biop, Pointer userdata)
{
  OrgModLoadFormPtr form_data;
  Int4 column_index, part_index;
  ValNodePtr line, part;
  TableLinePtr tlp;
  Boolean found_org;
  
  form_data = (OrgModLoadFormPtr) userdata;
  if (form_data == NULL) return;
  if (biop == NULL) return;
  if (biop->org == NULL) return;
  if (biop->org->taxname == NULL) return;

  for (column_index = 0;
       column_index < form_data->num_line_forms;
       column_index ++)
  {
    if (GetValue (form_data->line_forms[column_index].action_choice) == 1
      && GetValue (form_data->line_forms[column_index].match_choice) == 3)
    {
      for (line = form_data->line_list; line != NULL; line = line->next)
      {
        tlp = line->data.ptrvalue;
        if (tlp == NULL) continue;
        part = tlp->parts;
        for (part_index = 0;
             part_index < column_index && part != NULL;
             part_index ++)
        {
          part = part->next;
        }
        if (part != NULL
          && StringCmp (part->data.ptrvalue, biop->org->taxname) == 0)
        {
          ApplyQualsToOrg (biop, tlp->parts, form_data);
          found_org = TRUE;
        }
      }
    }
  }
}

static Boolean HasExtraAccession (
  CharPtr acc_str,
  GBBlockPtr gbp)
{
  ValNodePtr vnp;
  if (acc_str == NULL || gbp == NULL) return FALSE;
  for (vnp = gbp->extra_accessions;
       vnp != NULL && StringCmp (acc_str, vnp->data.ptrvalue) != 0;
       vnp = vnp->next)
  {}
  if (vnp != NULL)
    return TRUE;
  else
    return FALSE;
}

static Boolean
IDIsInTextList 
(CharPtr id,
 CharPtr acc_str,
 Boolean look_for_tmsmart)
{
  CharPtr wanted, found;
  Int4    len;

  if (id == NULL || acc_str == NULL) return FALSE;

  if (StringNCmp (acc_str, "TMSMART:", 8) == 0) {
    if (look_for_tmsmart) {
      wanted = acc_str + 8;
    } else {
      return FALSE;
    }
  } else {
    if (look_for_tmsmart) {
      return FALSE;
    } else {
      wanted = acc_str;
    }
  }
  len = StringLen (wanted);
  found = StringStr (id, wanted);
  if (found == NULL) {
    return FALSE;
  } else if (*(found + len) != 0 
             && *(found + len) != ','
             && ! isspace ((Int4)*(found + len))) {
    return FALSE;
  } else if (found != id
             && * (found - 1) != ','
             && ! isspace ((Int4)*(found - 1))) {
    return FALSE;
  } else {
    return TRUE;
  }
}

static Boolean IDListHasValue (
  CharPtr id,
  SeqIdPtr list,
  Boolean only_local,
  Boolean look_for_tmsmart)
{
  SeqIdPtr sip;
  Char     acc_str [256];
  Int4     match_len, match_len2;

  for (sip = list; sip != NULL; sip = sip->next)
  {
    if ((! only_local && sip->choice != SEQID_LOCAL)
      || (only_local && sip->choice == SEQID_LOCAL))
    {
      SeqIdWrite (sip, acc_str, PRINTID_REPORT, sizeof (acc_str));
      if (look_for_tmsmart)
      {
        if (StringNCmp (acc_str, "TMSMART:", 8) == 0
          && StringCmp (id, acc_str + 8) == 0)
        {
          return TRUE;
        } else if (IDIsInTextList (id, acc_str, TRUE)) {
          return TRUE;
        }
      } else {
        match_len = StringCSpn (acc_str, ".");
        match_len2 = StringCSpn (id, ".");
        if (match_len == match_len2
          && match_len > 0 && StringNCmp (id, acc_str, match_len) == 0)
        {
          return TRUE;
        }
      }
    }
  }
  return FALSE;
}

static void ApplyTableModsByAccessionNumber (BioseqPtr bsp, Pointer userdata)
{
  OrgModLoadFormPtr form_data;
  SeqDescrPtr       sdp;
  GBBlockPtr        gbp;
  Int4              column_index, part_index;
  BioSourcePtr      biop;
  ValNodePtr        line, part;
  TableLinePtr      tlp;
  Boolean           use_local_id;
  Boolean           look_for_tmsmart;
  Int4              match_choice;
  
  form_data = (OrgModLoadFormPtr) userdata;
  if (form_data == NULL || bsp == NULL) return;

  biop = GetBiopForBsp (bsp);
  if (biop == NULL) return;

  gbp = NULL;
  sdp = BioseqGetSeqDescr (bsp, Seq_descr_genbank, NULL);
  if (sdp != NULL)
  {
    gbp = sdp->data.ptrvalue;
  }

  for (column_index = 0;
       column_index < form_data->num_line_forms;
       column_index ++)
  {
    match_choice = GetValue (form_data->line_forms[column_index].match_choice);
    if (GetValue (form_data->line_forms[column_index].action_choice) == 1
      && (match_choice == 1 || match_choice == 2 || match_choice == 4))
    {
      if (match_choice == 1 || match_choice == 4) {
        use_local_id = FALSE;
      } else {
        use_local_id = TRUE;
      }
      if (match_choice == 4) {
        look_for_tmsmart = TRUE;
      } else {
        look_for_tmsmart = FALSE;
      }

      for (line = form_data->line_list; line != NULL; line = line->next)
      {
        tlp = line->data.ptrvalue;
        if (tlp == NULL) continue;
        part = tlp->parts;
        for (part_index = 0;
             part_index < column_index && part != NULL;
             part_index ++)
        {
          part = part->next;
        }
        if (part != NULL
          && ( IDListHasValue ( part->data.ptrvalue,
                                bsp->id, use_local_id, look_for_tmsmart)
            || (! use_local_id && 
                HasExtraAccession ( part->data.ptrvalue, gbp))))
        {
          ApplyQualsToOrg (biop, tlp->parts, form_data);
        }
      }
    }
  }
}

static void ApplyTableModsByBankitID (BioseqPtr bsp, Pointer userdata)
{
  OrgModLoadFormPtr form_data;
  ObjectIdPtr       oip;
  SeqIdPtr          sip;
  DbtagPtr          dp;
  Char              match_str [30];
  Int4              column_index, part_index;
  BioSourcePtr      biop;
  ValNodePtr        line, part;
  TableLinePtr      tlp;
  Int4              match_choice;
  
  form_data = (OrgModLoadFormPtr) userdata;
  if (form_data == NULL || bsp == NULL) return;

  biop = GetBiopForBsp (bsp);
  if (biop == NULL) return;

  match_str [0] = 0;
  for(sip = bsp->id; sip != NULL; sip = sip->next) {
    if(sip->choice == SEQID_GENERAL) {
      dp = (DbtagPtr) sip->data.ptrvalue;
      if(StringICmp(dp->db, "BankIt") == 0) {
        oip = dp->tag;
        sprintf (match_str, "bankit%d", oip->id);
        break;
      }
    }
  }
  if (match_str [0] == 0) return;

  for (column_index = 0;
       column_index < form_data->num_line_forms;
       column_index ++)
  {
    match_choice = GetValue (form_data->line_forms[column_index].match_choice);
    if (GetValue (form_data->line_forms[column_index].action_choice) == 1 && match_choice == 5) {
      for (line = form_data->line_list; line != NULL; line = line->next) {
        tlp = line->data.ptrvalue;
        if (tlp == NULL) continue;
        part = tlp->parts;
        for (part_index = 0; part_index < column_index && part != NULL; part_index ++) {
          part = part->next;
        }
        if (part != NULL && StringCmp (part->data.ptrvalue, match_str) == 0) {
          ApplyQualsToOrg (biop, tlp->parts, form_data);
        }
      }
    }
  }
}


static void DoAcceptFormMods (ButtoN b)
{
  OrgModLoadFormPtr form_data;
  SeqEntryPtr   sep;

  form_data = GetObjectExtra (b);
  if (form_data == NULL) return;

  form_data->replace_with_blank = GetStatus (form_data->replace_with_blank_btn);

  sep = GetTopSeqEntryForEntityID (form_data->entityID);
  if (sep == NULL) return;
  
  VisitBioSourcesInSep (sep, form_data, ApplyTableModsByTaxName);
  VisitBioseqsInSep (sep, form_data, ApplyTableModsByAccessionNumber);
  VisitBioseqsInSep (sep, form_data, ApplyTableModsByBankitID);
  Update ();
  ObjMgrSetDirtyFlag (form_data->entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, form_data->entityID, 0, 0);
  Remove (form_data->form);
}

static ValNodePtr ReadTableData (void)
{
  Char          path [PATH_MAX];
  FILE          *fp;
  Char          str [8192];
  size_t        len = 8192;
  Int4          max_columns;
  ValNodePtr    header_line;
  ValNodePtr    line_list;
  TableLinePtr  tlp;
  ValNodePtr    vnp;

  path [0] = '\0';
  if (! GetInputFileName (path, sizeof (path), NULL, "TEXT")) return NULL;
  
  fp = FileOpen (path, "r");
  if (fp == NULL) return NULL;

  line_list = NULL;
  max_columns = 0;
  header_line = NULL;
  ReadLine (fp, str, len);
  while (Nlm_fileDone) 
  {
    tlp = MakeTableData (str);
    if (tlp != NULL)
    {
      vnp = ValNodeNew (line_list);
      if (vnp == NULL) return NULL;
      if (line_list == NULL) line_list = vnp;
      vnp->data.ptrvalue = tlp;
      if (tlp->num_parts > max_columns)
      {
        max_columns = tlp->num_parts;
        header_line = vnp;
      }
    }
    ReadLine (fp, str, len);
  }
  FileClose (fp);
  /* throw out all lines before header line */
  if (header_line != line_list)
  {
    vnp = line_list;
    while (vnp != NULL && vnp->next != header_line)
    {
      vnp = vnp->next;
    }
    vnp->next = NULL;
    ValNodeFreeData (line_list);
    line_list = NULL;
  }
  return header_line;
}

extern void LoadOrganismModifierTable (IteM i)
{
  BaseFormPtr   bfp;
  ValNodePtr    header_line;
  ValNodePtr    vnp;
  TableLinePtr  tlp;
  WindoW        w;
  GrouP         h, g, k, c;
  OrgModLoadFormPtr form_data;
  Int4          index;
  Int4          max_columns;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  header_line = ReadTableData ();
  if (header_line == NULL || header_line->data.ptrvalue == NULL) return;
  tlp = header_line->data.ptrvalue;
  max_columns = 0;
  for (vnp = tlp->parts; vnp != NULL; vnp = vnp->next)
  {
    max_columns ++;
  }
  
  form_data = MemNew (sizeof (OrgModLoadFormData));
  if (form_data == NULL) return;
  form_data->entityID = bfp->input_entityID;
  form_data->line_list = header_line;
  form_data->num_line_forms = max_columns;
  form_data->line_forms = MemNew (max_columns * sizeof (OrgModLineFormData));
  if (form_data->line_forms == NULL) return;

  /* now create a dialog to display values */
  w = FixedWindow (-50, -33, -10, -10, "Table Conversion", StdCloseWindowProc);
  SetObjectExtra (w, form_data, CleanupOrgModLoadForm);
  form_data->form = (ForM) w;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  g = HiddenGroup (h, 3, 0, NULL);
  tlp = header_line->data.ptrvalue;
  vnp = tlp->parts;
  for (index = 0; index < max_columns; index++)
  {
    BuildOrgModLineForm (vnp->data.ptrvalue, index, g,
                         form_data->line_forms + index, form_data);
    vnp = vnp->next;
  }
  k = HiddenGroup (h, 1, 0, NULL);
  form_data->replace_with_blank_btn = CheckBox (k, "Erase current value when blank found in table", NULL);

  c = HiddenGroup (h, 4, 0, NULL);
  form_data->accept_button = DefaultButton (c, "Accept", DoAcceptFormMods);
  SetObjectExtra (form_data->accept_button, form_data, NULL);
  Disable (form_data->accept_button);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) k, (HANDLE) c, NULL);

  RealizeWindow (w);
  Show (w);
  Update ();
}

typedef struct qualifierselect {
  LisT gene_quals;
  LisT cds_quals;
  LisT import_quals;
} QualifierSelectData, PNTR QualifierSelectPtr;

typedef struct featurequalloadformdata {
  FEATURE_FORM_BLOCK

  ValNodePtr             line_list;
  ValNodePtr             featlist;
  Int4                   num_feats;
  Uint2                  entityID;
  LisT                   feature_type;
  QualifierSelectData    match_qual;
  QualifierSelectData    apply_qual;
  PopuP                  sequence_id_type;
  PopuP                  sequence_column;
  PopuP                  feature_column;
  PopuP                  apply_column;
  GrouP                  seq_group;
  GrouP                  match_group;
  GrouP                  apply_group;
  Boolean                asked_about_replace;
  Boolean                do_replace;
  Boolean                use_semicolon;
  ButtoN                 replace_with_blank_btn;
  Boolean                replace_with_blank;
  ButtoN                 accept_button;
} FeatureQualLoadFormData, PNTR FeatureQualLoadFormPtr;
 
static void CleanupFeatureQualLoadForm (
  GraphiC g,
  VoidPtr data
)
{
  FeatureQualLoadFormPtr form_data;

  form_data = (FeatureQualLoadFormPtr)data;
  if (form_data == NULL) return;
  CleanUpTableData (form_data->line_list);
  StdCleanupFormProc (g, data);
} 

typedef struct qualifierlistdata {
  CharPtr item_name;
} QualifierListData, PNTR QualifierListPtr;

static Int4 GetSubtypeFromOffsetInFeatureList (Int4 list_offset,
                                               ValNodePtr list)
{
  ValNodePtr vnp;

  if (list == NULL || list_offset < 1) return -1;

  vnp = list;
  while (vnp != NULL && list_offset > 1)
  {
    list_offset --;
    vnp = vnp->next;
  }
  if (list_offset > 1) return -1;
  return vnp->choice;
}

static void ShowQualifierListBySubtype (QualifierSelectPtr qsp, Int4 feature_subtype)
{
  if (feature_subtype == -1) {
    Hide (qsp->gene_quals);
    Hide (qsp->cds_quals);
    Hide (qsp->import_quals);
  } else if (feature_subtype == FEATDEF_GENE) {
    Show (qsp->gene_quals);
    Hide (qsp->cds_quals);
    Hide (qsp->import_quals);
  } else if (feature_subtype == FEATDEF_CDS) {
    Hide (qsp->gene_quals);
    Show (qsp->cds_quals);
    Hide (qsp->import_quals);
  } else {
    Hide (qsp->gene_quals);
    Hide (qsp->cds_quals);
    Show (qsp->import_quals);
  }
}

static void ShowQualifierLists (FeatureQualLoadFormPtr form_data)
{
  Int4 feature_offset, feature_subtype;

  if (form_data == NULL) return;

  feature_offset = GetValue (form_data->feature_type);
  feature_subtype = GetSubtypeFromOffsetInFeatureList (feature_offset,
                                                       form_data->featlist);
  ShowQualifierListBySubtype (&(form_data->match_qual), feature_subtype);
  ShowQualifierListBySubtype (&(form_data->apply_qual), feature_subtype);
}

static QualifierListData GeneTableQualifiers [] = {
  { "allele"},
  { "locus" },
  { "locus_tag"},
  { "description"},
  { "product"}
};

#define NUM_GENE_TABLE_QUALIFIERS 5

static QualifierListData ProtTableQualifiers [] = {
  { "product"},
  { "description"},
  { "comment"}
};

#define NUM_PROT_TABLE_QUALIFIERS 3

/* check the appropriate list box for qualifier type,
 * keeping in mind that the first item is always "none"
 */
static Int4 GetQualSelectBySubtype (QualifierSelectPtr qsp, Int4 feature_subtype)
{
  Int4 qual_choice = -1;

  if (feature_subtype == -1) {
    qual_choice = -1;
  } else if (feature_subtype == FEATDEF_GENE) {
    qual_choice = GetValue (qsp->gene_quals);
    if (qual_choice < 2 
      || qual_choice > NUM_GENE_TABLE_QUALIFIERS + 2) {
      qual_choice = -1;
    } else {
      qual_choice = qual_choice - 1;
    }
  } else if (feature_subtype == FEATDEF_CDS) {
    qual_choice = GetValue (qsp->cds_quals);
    if (qual_choice < 2 
      || qual_choice > NUM_PROT_TABLE_QUALIFIERS + 2) {
      qual_choice = -1;
    } else {
      qual_choice = qual_choice - 1;
    }
  } else {
    qual_choice = GetValue (qsp->import_quals);
    if (qual_choice < 2
      || qual_choice > ParFlat_TOTAL_GBQUAL + 2) {
      qual_choice = -1;
    } else {
      qual_choice = qual_choice - 1;
    }
  }
  return qual_choice;

}

static void SetFeatureQualAcceptButton (Handle a)
{
  FeatureQualLoadFormPtr form_data;
  Boolean                have_feature_select_column;
  Boolean                have_feature_qual_select_column;
  Int4                   feature_select_choice;
  Int4                   apply_qual_choice;
  Int4                   match_qual_choice;
  Int4                   feature_subtype;
  Boolean                seqid_select_ok;
  Int4                   seqid_select_type;
  Int4                   seqid_select_column;

  form_data = GetObjectExtra (a);
  if (form_data == NULL) return;

  ShowQualifierLists (form_data);
  have_feature_qual_select_column = FALSE;
  have_feature_select_column = FALSE;
  seqid_select_ok = FALSE;

  feature_select_choice = GetValue (form_data->feature_type);
  if ( feature_select_choice > 0 && feature_select_choice < form_data->num_feats)
  {
    have_feature_select_column = TRUE;
  }
  
  feature_subtype = GetSubtypeFromOffsetInFeatureList (feature_select_choice,
                                                       form_data->featlist);
  
  apply_qual_choice = GetQualSelectBySubtype (&(form_data->apply_qual),
                                              feature_subtype);
  if (apply_qual_choice != -1)
  {
    have_feature_qual_select_column = TRUE;
  }

  /* if a column is selected for a sequence identifier, must select type */
  /* if a column is not selected for a sequence identifier, must not select type */
  seqid_select_type = GetValue (form_data->sequence_id_type);
  seqid_select_column = GetValue (form_data->sequence_column);
  if (seqid_select_type == 1 || seqid_select_type == 2) {
    if (seqid_select_column > 0) {
      seqid_select_ok = TRUE;
    }
  } else {
    seqid_select_ok = TRUE;
  }
 
  /* if the feature column is not specified, must not have match qualifier */
  /* and must have sequence ID */
  /* if the feature column is specified, must have match qualifier */
  match_qual_choice = GetQualSelectBySubtype (&(form_data->match_qual),
                                              feature_subtype);
  if (GetValue (form_data->feature_column) < 1)
  {
    if (match_qual_choice != -1)
    {
      have_feature_select_column = FALSE;
    }
    else if (seqid_select_column < 1)
    {
      have_feature_select_column = FALSE;
    }
  } else {
    if (match_qual_choice == -1)
    {
      have_feature_select_column = FALSE;
    }
  }

  if (GetValue (form_data->apply_column) < 1) {
    have_feature_qual_select_column = FALSE;
  }

  if ( have_feature_select_column && have_feature_qual_select_column
    && seqid_select_ok)
  {
    Enable (form_data->accept_button);
  }
  else
  {
    Disable (form_data->accept_button);
  }
}

static Boolean ProductNameMatches (
  SeqLocPtr slp,
  BioseqPtr  bsp,
  CharPtr   pString
)
{
  SeqFeatPtr        sfp;
  SeqMgrFeatContext fcontext;

  if (slp == NULL || bsp == NULL || pString == NULL) return FALSE;

  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &fcontext);
  while (sfp != NULL)
  {
    if ( IsLocAInBonSameStrand (sfp->location, slp)
      && StringStr ( fcontext.label, pString) != NULL)
    {
      return TRUE;
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_CDREGION, 0, &fcontext);
  }
  return FALSE;
}

static Boolean GeneHasThisQualifier (
  SeqFeatPtr sfp,
  Uint2      entityID,
  CharPtr    pString,
  Int2       popup_offset
)
{
  BioseqPtr  bsp;
  GeneRefPtr grp;

  if (sfp == NULL || pString == NULL) return FALSE;
  if (popup_offset < 1 || popup_offset > NUM_GENE_TABLE_QUALIFIERS) return FALSE;

  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (grp == NULL) return FALSE;

  if ( StringCmp (GeneTableQualifiers [popup_offset - 1].item_name, "allele") == 0) {
    if (StringCmp (pString, grp->allele) == 0) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else if ( StringCmp (GeneTableQualifiers [popup_offset - 1].item_name, "locus") == 0) {
    if (StringCmp (pString, grp->locus) == 0) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else if ( StringCmp (GeneTableQualifiers [popup_offset - 1].item_name, "locus_tag") == 0) {
    if (StringCmp (pString, grp->locus_tag) == 0) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else if ( StringCmp (GeneTableQualifiers [popup_offset - 1].item_name, "description") == 0) {
    if (StringCmp (pString, grp->desc) == 0) {
      return TRUE;
    } else {
      return FALSE;
    }
  } else if ( StringCmp (GeneTableQualifiers [popup_offset - 1].item_name, "product") == 0)
  {
    bsp = GetBioseqGivenSeqLoc (sfp->location, entityID);
    return ProductNameMatches (sfp->location, bsp, pString);
  }
  return FALSE;
}

static Boolean ProteinHasThisQualifier (
  SeqFeatPtr sfp,
  CharPtr    pString,
  Int2       popup_offset
)
{
  ProtRefPtr prp;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_PROT || pString == NULL
    || popup_offset < 1 || popup_offset > NUM_PROT_TABLE_QUALIFIERS)
  {
    return FALSE;
  }

  prp = sfp->data.value.ptrvalue;
  if (prp == NULL) return FALSE;

  if (StringCmp (ProtTableQualifiers [ popup_offset - 1].item_name, "product") == 0)
  {
    if (prp->name != NULL
      && StringStr (prp->name->data.ptrvalue, pString) != NULL)
    {
      return TRUE;
    }
  } else if (StringCmp (ProtTableQualifiers [popup_offset - 1].item_name, "description") == 0) {
    if (StringStr (prp->desc, pString) != NULL)
    {
      return TRUE;
    }
  } else if (StringCmp (ProtTableQualifiers [popup_offset - 1].item_name, "comment") == 0) {
    if (StringStr (sfp->comment, pString) != NULL)
    {
      return TRUE;
    }
  }
  return FALSE;
}

static Boolean CDSHasThisQualifier (
  SeqFeatPtr sfp,
  Uint2      entityID,
  CharPtr    pString,
  Int2       popup_offset
)
{
  SeqFeatPtr protein;

  if (sfp == NULL || pString == NULL) return FALSE;

  if (StringCmp (ProtTableQualifiers [popup_offset - 1].item_name, "comment") == 0) {
    if (StringStr (sfp->comment, pString) != NULL)
    {
      return TRUE;
    }
  }
  else
  {
    protein = FindBestProtein (entityID, sfp->product);
    if (protein == NULL) return FALSE;
    return ProteinHasThisQualifier (protein, pString, popup_offset);
  }
  return FALSE;
}
  
static Boolean FeatureHasThisQualifier (
  SeqFeatPtr sfp,
  Uint2      entityID,
  CharPtr    pString,
  Int2       popup_offset
)
{
  Int2      qualval;
  GBQualPtr gbqual;

  if (popup_offset < 1) return FALSE;
  if (sfp == NULL || pString == NULL) return FALSE;

  if (sfp->idx.subtype == FEATDEF_GENE)
  {
    return GeneHasThisQualifier (sfp, entityID, pString, popup_offset);
  }
  else if (sfp->idx.subtype == FEATDEF_CDS)
  {
    return CDSHasThisQualifier (sfp, entityID, pString, popup_offset);
  }
  else
  {
    for (gbqual = sfp->qual; gbqual != NULL; gbqual = gbqual->next)
    {
      qualval = GBQualNameValid (gbqual->qual);
      if (qualval > -1 && qualval == popup_offset -1)
      {
        return TRUE;
      }
    }
  }
  return FALSE; 
}
 
static CharPtr GetDataForColumnOffset (ValNodePtr column_data, Int4 offset)
{
  ValNodePtr vnp;

  for (vnp = column_data; vnp != NULL && offset > 1; vnp = vnp->next)
  {
    offset --;
  }
  if (offset > 1 || vnp == NULL) {
    return NULL;
  } else {
    return vnp->data.ptrvalue;
  }
}

static Boolean ApplyQualToThisFeature (
  SeqFeatPtr sfp,
  ValNodePtr parts,
  FeatureQualLoadFormPtr form_data
)
{
  Int4       column_index;
  CharPtr    val;
  Int4       subtype;
  Int4       popup_offset;

  if (sfp == NULL || parts == NULL || form_data == NULL) return FALSE;

  column_index = GetValue (form_data->feature_column);
  subtype = GetSubtypeFromOffsetInFeatureList (
                     GetValue (form_data->feature_type), form_data->featlist);
  val = GetDataForColumnOffset (parts, column_index);

  if (val != NULL && sfp->idx.subtype == subtype)
  {
    popup_offset = GetQualSelectBySubtype (&(form_data->match_qual),
                                              subtype);
    if (popup_offset < 1
      || FeatureHasThisQualifier (sfp, form_data->entityID, val, popup_offset))
    {
      return TRUE;
    }
  }
  return FALSE;
}

typedef struct featurequaltabledata {
  ValNodePtr parts;
  FeatureQualLoadFormPtr form_data;
} FeatureQualTableData, PNTR FeatureQualTablePtr;

static void ApplyOneQualToProt (
  SeqFeatPtr sfp,
  Int2       popup_offset,
  CharPtr    pString,
  Boolean PNTR asked_about_replace,
  Boolean PNTR do_replace,
  Boolean PNTR use_semicolon)
{
  ProtRefPtr prp;
  CharPtr    cp;

  if (sfp == NULL || sfp->data.choice != SEQFEAT_PROT || pString == NULL
    || popup_offset < 1 || popup_offset > NUM_PROT_TABLE_QUALIFIERS
    || asked_about_replace == NULL
    || do_replace == NULL
    || use_semicolon == NULL)
  {
    return;
  }

  prp = sfp->data.value.ptrvalue;
  if (prp == NULL) return;

  if (StringCmp (ProtTableQualifiers [ popup_offset - 1].item_name, "product") == 0)
  {
    if (prp->name == NULL)
    {
      ValNodeAddStr (&prp->name, 0, StringSave (pString));
    }
    else
    {
      cp = prp->name->data.ptrvalue;
      AppendOrReplaceString (&cp, pString,
                            asked_about_replace, do_replace, use_semicolon);
      prp->name->data.ptrvalue = cp;
    }
  } else if (StringCmp (ProtTableQualifiers [popup_offset - 1].item_name, "description") == 0) {
    AppendOrReplaceString (&(prp->desc), pString,
                            asked_about_replace, do_replace, use_semicolon);
  } else if (StringCmp (ProtTableQualifiers [popup_offset - 1].item_name, "comment") == 0) {
    AppendOrReplaceString (&(sfp->comment), pString, 
                            asked_about_replace, do_replace, use_semicolon);
  }
}

static void ApplyOneQualToCDS (
  SeqFeatPtr sfp,
  Uint2      entityID,
  Int2       feat_qual_choice,
  CharPtr    pString,
  Boolean PNTR asked_about_replace,
  Boolean PNTR do_replace,
  Boolean PNTR use_semicolon)
{
  SeqFeatPtr protein;

  if (sfp == NULL || pString == NULL
    || asked_about_replace == NULL
    || do_replace == NULL
    || use_semicolon == NULL)
  {
    return;
  }

  if (StringCmp (ProtTableQualifiers [feat_qual_choice - 1].item_name, "comment") == 0) {
    AppendOrReplaceString (&(sfp->comment), pString, 
                            asked_about_replace, do_replace, use_semicolon);
  }
  else
  {
    protein = FindBestProtein (entityID, sfp->product);
    if (protein == NULL) return;
    ApplyOneQualToProt (protein, feat_qual_choice, pString, 
                            asked_about_replace, do_replace, use_semicolon);
  }
}

static void ApplyGeneProductName (
  SeqLocPtr slp,
  Uint2     entityID,
  CharPtr   pString,
  Boolean PNTR asked_about_replace,
  Boolean PNTR do_replace,
  Boolean PNTR use_semicolon)
{
  SeqFeatPtr        sfp;
  SeqMgrFeatContext fcontext;
  Int4              prot_popup_offset;
  BioseqPtr         bsp;

  if (slp == NULL || pString == NULL
    || asked_about_replace == NULL
    || do_replace == NULL
    || use_semicolon == NULL)
  {
    return;
  }

  bsp = GetBioseqGivenSeqLoc (slp, entityID);
  if (bsp == NULL) return;

  for (prot_popup_offset = 0;
       prot_popup_offset < NUM_PROT_TABLE_QUALIFIERS
         && StringCmp (ProtTableQualifiers [prot_popup_offset].item_name, "product") != 0;
       prot_popup_offset ++)
  {}
  if (prot_popup_offset >= NUM_PROT_TABLE_QUALIFIERS) return;

  sfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_CDREGION, 0, &fcontext);
  while (sfp != NULL)
  {
    if ( IsLocAInBonSameStrand (sfp->location, slp))
    {
      ApplyOneQualToCDS (sfp, entityID, prot_popup_offset + 1, pString, 
                            asked_about_replace, do_replace, use_semicolon);
    }
    sfp = SeqMgrGetNextFeature (bsp, sfp, SEQFEAT_CDREGION, 0, &fcontext);
  }
}

static void ApplyOneQualToGene (
  SeqFeatPtr sfp,
  Uint2      entityID,
  Int2       feat_qual_choice,
  CharPtr    pString, 
  Boolean PNTR asked_about_replace,
  Boolean PNTR do_replace,
  Boolean PNTR use_semicolon)
{
  GeneRefPtr grp;

  if (sfp == NULL
    || sfp->idx.subtype != FEATDEF_GENE
    || feat_qual_choice < 1
    || feat_qual_choice > NUM_GENE_TABLE_QUALIFIERS
    || pString == NULL
    || asked_about_replace == NULL
    || do_replace == NULL
    || use_semicolon == NULL)
  {
    return;
  }

  grp = (GeneRefPtr) sfp->data.value.ptrvalue;
  if (grp == NULL) return;

  if (StringCmp ( GeneTableQualifiers [feat_qual_choice - 1].item_name,
                 "allele") == 0) {
    AppendOrReplaceString (&(grp->allele), pString, 
                            asked_about_replace, do_replace, use_semicolon);
  } else if (StringCmp ( GeneTableQualifiers [feat_qual_choice - 1].item_name,
                        "locus") == 0) {
    AppendOrReplaceString (&(grp->locus), pString, 
                            asked_about_replace, do_replace, use_semicolon);
  } else if (StringCmp ( GeneTableQualifiers [feat_qual_choice - 1].item_name,
                        "locus_tag") == 0) {
    AppendOrReplaceString (&(grp->locus_tag), pString, 
                            asked_about_replace, do_replace, use_semicolon);
  } else if (StringCmp ( GeneTableQualifiers [feat_qual_choice - 1].item_name,
                        "description") == 0) {
    AppendOrReplaceString (&(grp->desc), pString, 
                            asked_about_replace, do_replace, use_semicolon);
  } else if (StringCmp (GeneTableQualifiers [feat_qual_choice - 1].item_name,
                        "product") == 0) {
    ApplyGeneProductName (sfp->location, entityID, pString, 
                            asked_about_replace, do_replace, use_semicolon);
  }
}

static void ApplyOneQualToImportFeature (
  SeqFeatPtr sfp,
  Int2 feat_qual_choice,
  CharPtr pString,
  Boolean PNTR asked_about_replace,
  Boolean PNTR do_replace,
  Boolean PNTR use_semicolon)
{
  GBQualPtr           gbqual;

  if (sfp == NULL
    || feat_qual_choice < 1
    || feat_qual_choice > ParFlat_TOTAL_GBQUAL
    || pString == NULL
    || asked_about_replace == NULL
    || do_replace == NULL
    || use_semicolon == NULL)
  {
    return;
  }

  gbqual = sfp->qual;
  while (gbqual != NULL
    && StringCmp (gbqual->qual,
                  ParFlat_GBQual_names [feat_qual_choice - 1].name) != 0)
  {
    gbqual = gbqual->next;
  }
  
  if (gbqual == NULL)
  {
    gbqual = GBQualNew ();
    if (gbqual == NULL) return;
    gbqual->qual = StringSave ( ParFlat_GBQual_names [feat_qual_choice - 1].name);
    gbqual->val = StringSave ( pString );
    gbqual->next = sfp->qual;
    sfp->qual = gbqual;
  }
  else
  {
    AppendOrReplaceString (&(gbqual->val), pString, 
                            asked_about_replace, do_replace, use_semicolon);
  }
}

static void ApplyOneQualToFeature (
  SeqFeatPtr sfp,
  Uint2      entityID,
  Int2       feat_qual_choice,
  CharPtr    pString,
  Boolean PNTR asked_about_replace,
  Boolean PNTR do_replace,
  Boolean PNTR use_semicolon)
{
  if (sfp == NULL || pString == NULL 
    || asked_about_replace == NULL
    || do_replace == NULL
    || use_semicolon == NULL)
  {
    return;
  }

  switch (sfp->idx.subtype)
  {
    case FEATDEF_GENE :
      ApplyOneQualToGene (sfp, entityID, feat_qual_choice, pString,
                            asked_about_replace, do_replace, use_semicolon);
      break;
    case FEATDEF_CDS :
      ApplyOneQualToCDS (sfp, entityID, feat_qual_choice, pString,
                            asked_about_replace, do_replace, use_semicolon);
      break;
    default :
      ApplyOneQualToImportFeature (sfp, feat_qual_choice, pString,
                            asked_about_replace, do_replace, use_semicolon);
      break;
  }
}

static CharPtr GetImpFeatKeyFromSubtype (Uint2 subtype)
{
  FeatDefPtr  curr;
  Uint1       key;
  CharPtr     label;

  curr = FeatDefFindNext (NULL, &key, &label, FEATDEF_ANY, TRUE);
  while (curr != NULL) {
    if (key == subtype)
    {
      return curr->typelabel;
    }
    curr = FeatDefFindNext (curr, &key, &label, FEATDEF_ANY, TRUE);
  }
  return NULL;
}

static void ApplyQualsToFeaturesOnBsp (BioseqPtr bsp,
 FeatureQualLoadFormPtr form_data,
 ValNodePtr             parts)
{
  SeqFeatPtr sfp;
  SeqMgrFeatContext fcontext;
  Boolean found_feature = FALSE;
  Int4                subtype;
  Int4                column_index;
  Int2                qualval;
  Int2                matchval;
  CharPtr             val;
  ImpFeatPtr          ifp;

  subtype = GetSubtypeFromOffsetInFeatureList (
                          GetValue (form_data->feature_type), 
                          form_data->featlist);
  column_index = GetValue (form_data->apply_column);
  val = GetDataForColumnOffset (parts, column_index);
  /* if the value to be applied is blank and we're not replacing with blanks, skip */
  if (! form_data->replace_with_blank && StringHasNoText (val)) {
    return;
  }
  qualval = GetQualSelectBySubtype (&(form_data->apply_qual), subtype);
  if (subtype == -1 || val == NULL || column_index < 1 || qualval < 1) {
    /* do nothing */
  }
  else
  {
    sfp = SeqMgrGetNextFeature (bsp, NULL, 0, 0, &fcontext);
    while (sfp != NULL)
    {
      if ( ApplyQualToThisFeature (sfp, parts, form_data))
      {
        found_feature = TRUE;
        ApplyOneQualToFeature (sfp, form_data->entityID, qualval, val,
                               &(form_data->asked_about_replace),
                               &(form_data->do_replace),
                               &(form_data->use_semicolon));
      }
      sfp = SeqMgrGetNextFeature (bsp, sfp, 0, 0, &fcontext);
    }

    /* only want to create new features if no feature matching qualifier
     * was specified and no feature was found */
    matchval = GetQualSelectBySubtype (&(form_data->match_qual), subtype);

    if (! found_feature && matchval == -1)
    {
      if (subtype == FEATDEF_CDS)
      {
        sfp = CreateNewFeatureOnBioseq (bsp, SEQFEAT_CDREGION, NULL);
        if (sfp == NULL) return;
        sfp->data.value.ptrvalue = CdRegionNew ();
      } else if (subtype == FEATDEF_GENE) {
        sfp = CreateNewFeatureOnBioseq (bsp, SEQFEAT_GENE, NULL);
        if (sfp == NULL) return;
        sfp->data.value.ptrvalue = GeneRefNew ();
      } else {
        ifp = ImpFeatNew ();
        if (ifp == NULL) return;
        sfp = CreateNewFeatureOnBioseq (bsp, SEQFEAT_IMP, NULL);
        if (sfp == NULL) return;
        ifp->key = StringSave ( GetImpFeatKeyFromSubtype (subtype));
        sfp->data.value.ptrvalue = ifp;
      }
      SeqMgrIndexFeatures (0, (Pointer) bsp);
      ApplyOneQualToFeature (sfp, form_data->entityID, qualval, val,
                               &(form_data->asked_about_replace),
                               &(form_data->do_replace),
                               &(form_data->use_semicolon));
    }
  }
}

static void ApplyTableQuals (BioseqPtr bsp, Pointer userdata)
{
  FeatureQualLoadFormPtr form_data;
  SeqDescrPtr            sdp;
  GBBlockPtr             gbp;
  Int4                   column_index;
  ValNodePtr             line;
  TableLinePtr           tlp;
  Boolean                use_local_id;
  FeatureQualTableData   fqtd;
  Int2                   seq_match_choice;
  CharPtr                idval;
  
  form_data = (FeatureQualLoadFormPtr) userdata;
  if (form_data == NULL || bsp == NULL) return;

  gbp = NULL;
  sdp = BioseqGetSeqDescr (bsp, Seq_descr_genbank, NULL);
  if (sdp != NULL)
  {
    gbp = sdp->data.ptrvalue;
  }

  seq_match_choice = GetValue (form_data->sequence_id_type);
  if (seq_match_choice == 1) {
    use_local_id = FALSE;
  }

  fqtd.form_data = form_data;
  if (seq_match_choice == 1 || seq_match_choice == 2)
  {
    column_index = GetValue (form_data->sequence_column);

    for (line = form_data->line_list; line != NULL; line = line->next)
    {
      tlp = line->data.ptrvalue;
      if (tlp == NULL) continue;
      idval = GetDataForColumnOffset (tlp->parts, column_index);
      if (idval != NULL
        && ( IDListHasValue ( idval,
                              bsp->id, use_local_id, FALSE)
          || (! use_local_id && 
              HasExtraAccession ( idval, gbp))))
      {
        fqtd.parts = tlp->parts;
        ApplyQualsToFeaturesOnBsp (bsp, form_data, tlp->parts);
      }
    }
  } else {
    for (line = form_data->line_list; line != NULL; line = line->next)
    {
      tlp = line->data.ptrvalue;
      if (tlp == NULL) continue;
      fqtd.parts = tlp->parts;
      ApplyQualsToFeaturesOnBsp (bsp, form_data, tlp->parts);
    }
  }
}

static void DoAcceptFeatureQuals (ButtoN b)
{
  FeatureQualLoadFormPtr form_data;
  SeqEntryPtr   sep;

  form_data = GetObjectExtra (b);
  if (form_data == NULL) return;

  form_data->replace_with_blank = GetStatus (form_data->replace_with_blank_btn);

  sep = GetTopSeqEntryForEntityID (form_data->entityID);
  if (sep == NULL) return;
  
  VisitBioseqsInSep (sep, form_data, ApplyTableQuals);
  Update ();
  ObjMgrSetDirtyFlag (form_data->entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, form_data->entityID, 0, 0);
  Remove (form_data->form);
}

static PopuP BuildColumnSelector ( GrouP g,
                                  FeatureQualLoadFormPtr parent_form,
                                  ValNodePtr parts )
{
  PopuP      p;
  ValNodePtr this_part;

  if (g == NULL || parts == NULL) return NULL;

  p = PopupList (g, TRUE, (PupActnProc) SetFeatureQualAcceptButton);
  for (this_part = parts; this_part != NULL; this_part = this_part->next)
  {
    if (this_part->data.ptrvalue != NULL)
    {
      PopupItem (p, this_part->data.ptrvalue);
    }
  }
  SetObjectExtra (p, parent_form, NULL);
  return p;
}

static void BuildQualifierSelect ( GrouP parent, 
                                  FeatureQualLoadFormPtr parent_form,
                                  ValNodePtr parts,
                                  QualifierSelectPtr qsp )
{
  GrouP g;
  Int4  listheight = 4;
  Int4  listwidth  = 13;
  Int4  j;

  if (parent_form == NULL || parts == NULL || qsp == NULL)
  {
    return;
  }

  g = HiddenGroup (parent, 0, 0, NULL);

  qsp->gene_quals = SingleList (g, listwidth, listheight,
                                   (LstActnProc) SetFeatureQualAcceptButton);
  SetObjectExtra (qsp->gene_quals, parent_form, NULL);
  ListItem (qsp->gene_quals, "None");
  for (j=0; j < NUM_GENE_TABLE_QUALIFIERS; j++) {
    ListItem (qsp->gene_quals, GeneTableQualifiers [j].item_name);
  }
  SetValue (qsp->gene_quals, 1);

  qsp->cds_quals = SingleList (g, listwidth, listheight,
                                   (LstActnProc) SetFeatureQualAcceptButton);
  SetObjectExtra (qsp->cds_quals, parent_form, NULL);
  ListItem (qsp->cds_quals, "None");
  for (j=0; j < NUM_PROT_TABLE_QUALIFIERS; j++) {
    ListItem (qsp->cds_quals, ProtTableQualifiers [j].item_name);
  }
  SetValue (qsp->cds_quals, 1);

  qsp->import_quals = SingleList (g, listwidth, listheight,
                                   (LstActnProc) SetFeatureQualAcceptButton);
  SetObjectExtra (qsp->import_quals, parent_form, NULL);
  ListItem (qsp->import_quals, "None");
  for (j = 0; j < ParFlat_TOTAL_GBQUAL; j++) {
    ListItem (qsp->import_quals, ParFlat_GBQual_names [j].name);
  }
  SetValue (qsp->import_quals, 1);
}

static void BuildSequenceSelect ( GrouP parent,
                                  FeatureQualLoadFormPtr parent_form,
                                  ValNodePtr parts )
{
  if (parent == NULL || parent_form == NULL || parts == NULL) return;

  parent_form->seq_group = NormalGroup ( parent, 0, 2, "Sequence", programFont, NULL);
  StaticPrompt (parent_form->seq_group, "Identifier Type", 0, popupMenuHeight, programFont, 'l');
  parent_form->sequence_id_type = PopupList (parent_form->seq_group, TRUE, (PupActnProc) SetFeatureQualAcceptButton);
  PopupItem (parent_form->sequence_id_type, "Accession");
  PopupItem (parent_form->sequence_id_type, "Local ID");
  PopupItem (parent_form->sequence_id_type, "None");
  SetObjectExtra (parent_form->sequence_id_type, parent_form, NULL);
  SetValue (parent_form->sequence_id_type, 3);

  StaticPrompt (parent_form->seq_group, "Column", 0, popupMenuHeight, programFont, 'l');
  parent_form->sequence_column = BuildColumnSelector ( parent_form->seq_group, parent_form, parts);
}

static void BuildFeatureSelect ( GrouP parent,
                                 FeatureQualLoadFormPtr parent_form,
                                 ValNodePtr parts )
{
  Int4  listheight = 4;
  ValNodePtr vnp;

  if (parent == NULL || parent_form == NULL || parts == NULL) return;

  parent_form->match_group = NormalGroup ( parent, 0, 2,
                                    "Feature to Edit", programFont, NULL);
  StaticPrompt (parent_form->match_group, "Feature type", 0, 
                                   popupMenuHeight, programFont, 'l');
  parent_form->feature_type = SingleList (parent_form->match_group, 
                                   9, listheight,
                   (LstActnProc) SetFeatureQualAcceptButton);
  SetObjectExtra (parent_form->feature_type, parent_form, NULL);
  for (vnp = parent_form->featlist; vnp != NULL; vnp = vnp->next)
  {
    ListItem (parent_form->feature_type, (CharPtr) vnp->data.ptrvalue);
  }

  StaticPrompt (parent_form->match_group, "Qualifier to match", 0, popupMenuHeight, programFont, 'l');
  BuildQualifierSelect ( parent_form->match_group, parent_form, parts, &(parent_form->match_qual));

  StaticPrompt (parent_form->match_group, "Column", 0, popupMenuHeight, programFont, 'l');
  parent_form->feature_column = BuildColumnSelector ( parent_form->match_group, parent_form, parts);
  
}

static void BuildFeatureApply ( GrouP parent,
                               FeatureQualLoadFormPtr parent_form,
                               ValNodePtr parts )
{
  if (parent == NULL || parent_form == NULL || parts == NULL) return;

  parent_form->apply_group = NormalGroup ( parent, 0, 2, "Qualifier to Edit", programFont, NULL);
  
  StaticPrompt (parent_form->apply_group, "Qualifier Name", 0, popupMenuHeight, programFont, 'l');
  BuildQualifierSelect ( parent_form->apply_group, parent_form, parts, &(parent_form->apply_qual));

  StaticPrompt (parent_form->apply_group, "Column", 0, popupMenuHeight, programFont, 'l');
  parent_form->apply_column = BuildColumnSelector ( parent_form->apply_group, parent_form, parts);
  
}

extern void LoadFeatureQualifierTable (IteM i)
{
  BaseFormPtr   bfp;
  ValNodePtr    header_line;
  ValNodePtr    vnp;
  TableLinePtr  tlp;
  WindoW        w;
  GrouP         h, g, c;
  FeatureQualLoadFormPtr form_data;
  Int4          max_columns;

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL) return;

  header_line = ReadTableData ();
  if (header_line == NULL || header_line->data.ptrvalue == NULL) return;
  tlp = header_line->data.ptrvalue;
  max_columns = 0;
  for (vnp = tlp->parts; vnp != NULL; vnp = vnp->next)
  {
    max_columns ++;
  }
  
  form_data = MemNew (sizeof (FeatureQualLoadFormData));
  if (form_data == NULL) return;
  form_data->asked_about_replace = FALSE;
  form_data->do_replace = FALSE;
  form_data->use_semicolon = FALSE;
  form_data->entityID = bfp->input_entityID;
  form_data->line_list = header_line;
  form_data->featlist = BuildFeatureValNodeList (TRUE, NULL, 0, TRUE, FALSE);
  form_data->num_feats = 0;
  for (vnp = form_data->featlist; vnp != NULL; vnp = vnp->next)
  {
    form_data->num_feats ++;
  }

  /* now create a dialog to display values */
  w = FixedWindow (-50, -33, -10, -10, "Table Conversion", StdCloseWindowProc);
  SetObjectExtra (w, form_data, CleanupFeatureQualLoadForm);
  form_data->form = (ForM) w;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);

  BuildSequenceSelect ( h, form_data, tlp->parts);
  BuildFeatureSelect (h, form_data, tlp->parts);
  BuildFeatureApply (h, form_data, tlp->parts);
  ShowQualifierLists (form_data);

  g = HiddenGroup (h, 1, 0, NULL);
  form_data->replace_with_blank_btn = CheckBox (g, "Erase current value when blank found in table", NULL);
  c = HiddenGroup (h, 4, 0, NULL);
  form_data->accept_button = DefaultButton (c, "Accept", DoAcceptFeatureQuals);
  SetObjectExtra (form_data->accept_button, form_data, NULL);
  Disable (form_data->accept_button);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER,
                (HANDLE) form_data->seq_group, 
                (HANDLE) form_data->match_group,
                (HANDLE) form_data->apply_group,
                (HANDLE) c, NULL);

  RealizeWindow (w);
  Show (w);
  Update ();
}

typedef struct prefixformdata {
  FEATURE_FORM_BLOCK

  ModifierItemLocalPtr modList;
  PopuP PNTR           popup_list;
  ButtoN               add_org_name;
  ButtoN               use_labels;
} PrefixFormData, PNTR PrefixFormPtr;

static void CleanupPrefixForm (
  GraphiC g,
  VoidPtr data
)

{
  PrefixFormPtr  pfp;
  Int4           i;

  pfp = (PrefixFormPtr) data;
  if (pfp != NULL) {
    if (pfp->modList != NULL)
    {
      for (i=0; i < NumDefLineModifiers; i++)
      {
        ValNodeFree (pfp->modList[i].values_seen);
      }
      MemFree (pfp->modList);
    }
    if (pfp->popup_list != NULL)
    {
      MemFree (pfp->popup_list);
    }
  }
  StdCleanupFormProc (g, data);
}

static Int4 GetDefLineModifierPopupIndex (
  Int4 popup_index, 
  ModifierItemLocalPtr modList
)
{
  Int4 index;
  Int4 want_index;
  
  want_index = 0;
  for (index = 0;
       index < NumDefLineModifiers && want_index < popup_index;
       index++)
  {
    if (modList [index].any_present)
    {
      want_index ++;
    }
  }
  if (index >= NumDefLineModifiers || index == 0
    || want_index < popup_index)
  {
    return -1;
  }
  return index - 1;
}

static void AddPrefixesToOneDefLine (
  PrefixFormPtr pfp,
  BioseqPtr bsp,
  SeqEntryPtr sep
)
{
  BioSourcePtr biop;
  SeqDescrPtr  sdp;
  ValNodePtr   strings;
  Int4         index;
  Int4         qual_index;
  Int4         popup_choice;
  CharPtr      new_defline;
  Char         taxName [196];
  Char         modifier_text [256];
  OrgRefPtr    orp;
  OrgNamePtr   onp;
  OrgModPtr    mod;
  SubSourcePtr ssp;
  CharPtr  org_desc;
  CharPtr  cp;
  SeqMgrDescContext  dcontext;
  Boolean      use_labels;
  Int4         no_semicolon_len;
  Int4         prefix_len;

  if (bsp == NULL || pfp == NULL
    || pfp->popup_list == NULL
    || pfp->modList == NULL
    || (biop = GetBiopForBsp (bsp)) == NULL)
  {
    return;
  }

  if (biop->org == NULL) return;
  if (biop->org->taxname == NULL) return;
  StringNCpy (taxName, biop->org->taxname, sizeof (taxName) - 1);
  taxName [ sizeof (taxName) - 1] = 0;

  strings = NULL;
  CleanUpTaxName (taxName, TRUE);
  if (GetStatus (pfp->add_org_name))
  {
    ValNodeCopyStr( &strings, 0, taxName);
  }

  use_labels = GetStatus (pfp->use_labels);

  orp = biop->org;
  if (orp == NULL) return;
  onp = orp->orgname;
  if (onp == NULL) return;
  for (index = 0; index < NumDefLineModifiers; index ++)
  {
    if (pfp->popup_list [ index] != NULL
      && (popup_choice = GetValue (pfp->popup_list [ index])) > 0)
    {
      qual_index = GetDefLineModifierPopupIndex (popup_choice, pfp->modList);
      if (qual_index < 0 || qual_index >= NumDefLineModifiers) continue;
      if (DefLineModifiers[qual_index].isOrgMod)
      {
        mod = onp->mod;
        while (mod != NULL
          && mod->subtype != DefLineModifiers[qual_index].subtype)
        {
          mod = mod->next;
        }
        if ( UseOrgModifier (mod, taxName))
        {
          no_semicolon_len = StringCSpn (mod->subname, ";");
          if (mod->subtype == ORGMOD_nat_host)
          {
            sprintf (modifier_text, "from ");
            if (no_semicolon_len > sizeof (modifier_text) - 6) {
			  prefix_len = sizeof (modifier_text) - 1;
              no_semicolon_len = sizeof (modifier_text) - 6;
			} else {
			  prefix_len = no_semicolon_len + 5;
			}
            StringNCpy (modifier_text + 5, mod->subname, no_semicolon_len);
            modifier_text[prefix_len] = 0;
          }
          else
          {
            AddModifierLabel (use_labels, qual_index, modifier_text);
            if (modifier_text[0] != 0)
              StringCat (modifier_text, " ");
            if (no_semicolon_len > sizeof (modifier_text) - StringLen (modifier_text) - 1) {
              no_semicolon_len = sizeof (modifier_text) - StringLen (modifier_text) - 1;
			  prefix_len = sizeof (modifier_text) - 1;
			} else {
			  prefix_len = no_semicolon_len + StringLen (modifier_text);
			}

            StringNCat (modifier_text, mod->subname, no_semicolon_len);
            modifier_text[prefix_len] = 0;
          }
          ValNodeCopyStr( &strings, 0, modifier_text);
        }
      } else {
        ssp = biop->subtype;
        while (ssp != NULL
            && ssp->subtype != DefLineModifiers[qual_index].subtype)
        {
          ssp = ssp->next;
        }
        if (ssp != NULL)
        {
          no_semicolon_len = StringCSpn (ssp->name, ";");
          AddModifierLabel (use_labels, qual_index, modifier_text);
          if (ssp->subtype == SUBSRC_country)
          {
            sprintf (modifier_text, "from ");
            if (no_semicolon_len > sizeof (modifier_text) - 6) {
              no_semicolon_len = sizeof (modifier_text) - 6;
			  prefix_len = sizeof(modifier_text);
			} else {
			  prefix_len = StringLen (modifier_text) + no_semicolon_len;
			}
            StringNCpy (modifier_text + 5, ssp->name, no_semicolon_len);
            modifier_text[prefix_len] = 0;
            cp = StringChr (modifier_text, ':');
            if (cp != NULL) *cp = 0;
          }
          else if (ssp->name != NULL
            && (ssp->subtype != SUBSRC_plasmid_name
              || StringCmp (ssp->name, "unnamed") != 0))
          {
            if (modifier_text[0] != 0)
              StringCat (modifier_text, " ");
            if (no_semicolon_len > sizeof (modifier_text) - StringLen (modifier_text) - 1) {
              no_semicolon_len = sizeof (modifier_text) - StringLen (modifier_text) - 1;
			  prefix_len = sizeof (modifier_text);
			} else {
			  prefix_len = StringLen (modifier_text) + no_semicolon_len;
			}
 
            StringNCat (modifier_text, ssp->name, no_semicolon_len);
            modifier_text[prefix_len] = 0;
          }
          ValNodeCopyStr( &strings, 0, modifier_text);
        }
      }
    }
  }
  org_desc = MergeValNodeStrings (strings, FALSE);
  ValNodeFreeData (strings);

  sdp = SeqEntryGetSeqDescr (sep, Seq_descr_title, NULL);
  if (sdp == NULL) {
    sdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_title, &dcontext);
    if (sdp == NULL) return;
    cp = (CharPtr) sdp->data.ptrvalue;
    if (cp == NULL) return;
    sdp = SeqDescrAdd (&(bsp->descr));
    if (sdp == NULL) return;
    sdp->choice = Seq_descr_title;
    sdp->data.ptrvalue = StringSave (cp);
  }
  if (sdp == NULL) return;
  if (StringHasNoText (sdp->data.ptrvalue)) return;
  new_defline = MemNew ((StringLen (org_desc)
                        + StringLen (sdp->data.ptrvalue) + 4) * sizeof (Char));
  if (new_defline == NULL) return;
  StringCpy (new_defline, org_desc);
  StringCat (new_defline, " ");
  StringCat (new_defline, sdp->data.ptrvalue);
  MemFree (sdp->data.ptrvalue);
  sdp->data.ptrvalue = new_defline;
  ObjMgrSetDirtyFlag (pfp->input_entityID, TRUE);
}

static void AddPrefixesToDefLines (PrefixFormPtr pfp, SeqEntryPtr sep)
{
  BioseqSetPtr bssp;
  if (pfp == NULL || sep == NULL) return;
  if (IS_Bioseq_set (sep)) {
    bssp = (BioseqSetPtr) sep->data.ptrvalue;
    if (bssp != NULL) {
      for (sep = bssp->seq_set; sep != NULL; sep = sep->next) {
        AddPrefixesToDefLines (pfp, sep);
      }
      return;
    }
  }
  if (! IS_Bioseq (sep)) return;
  AddPrefixesToOneDefLine (pfp, sep->data.ptrvalue, sep);
}

static void DoPrefixDefLines (ButtoN b)
{
  PrefixFormPtr pfp;
  SeqEntryPtr   sep;

  pfp = GetObjectExtra (b);
  if (pfp == NULL) return;

  Hide (pfp->form);
  WatchCursor ();
  Update ();
  sep = GetTopSeqEntryForEntityID (pfp->input_entityID);
  if (sep == NULL) return;
  AddPrefixesToDefLines (pfp, sep);

  ArrowCursor ();
  Update ();
  ObjMgrSetDirtyFlag (pfp->input_entityID, TRUE);
  ObjMgrSendMsg (OM_MSG_UPDATE, pfp->input_entityID, 0, 0);
}

extern void PrefixDefLines (IteM i)
{
  BaseFormPtr  bfp;
  SeqEntryPtr  sep;
  ModifierItemLocalPtr modList;
  Int4         index;
  WindoW       w;
  GrouP        h, g, k;
  Int4         popup_index, item_index, listed_index;
  GrouP        c;
  ButtoN       b;
  PrefixFormPtr pfp;
  Char         label [256];

#ifdef WIN_MAC
  bfp = currentFormDataPtr;
#else
  bfp = GetObjectExtra (i);
#endif
  if (bfp == NULL || bfp->input_entityID == 0) return;

  sep = GetTopSeqEntryForEntityID (bfp->input_entityID);
  if (sep == NULL) return;

  modList = MemNew (NumDefLineModifiers * sizeof (ModifierItemLocalData));
  if (modList == NULL) return;
  CountModifiers (modList, sep);

  pfp = MemNew (sizeof (PrefixFormData));
  if (pfp == NULL) return;
  pfp->input_entityID = bfp->input_entityID;
  pfp->modList = modList;
  pfp->popup_list = MemNew (sizeof (PopuP) * NumDefLineModifiers);
  if (pfp->popup_list == NULL) return;

  w = FixedWindow (-50, -33, -10, -10, "Definition Line Prefixes",
                   StdCloseWindowProc);
  SetObjectExtra (w, pfp, CleanupPrefixForm);
  pfp->form = (ForM) w;

  h = HiddenGroup (w, -1, 0, NULL);
  SetGroupSpacing (h, 10, 10);
  g = HiddenGroup (h, 4, 0, NULL);
  for (index = 0; index < NumDefLineModifiers; index++)
  {
    pfp->popup_list [index] = NULL;
  }
  popup_index = 0;
  for (index = 0; index < NumDefLineModifiers; index++)
  {
    if (modList [ index].any_present)
    {
      k = HiddenGroup (g, 2, 0, NULL);
      sprintf (label, "%d", popup_index + 1);
      StaticPrompt (k, label, 0, popupMenuHeight, programFont, 'l');
      pfp->popup_list[popup_index] = PopupList (k, TRUE, NULL);
      listed_index = 0;
      for (item_index = 0; item_index < NumDefLineModifiers; item_index ++)
      {
        if (modList [item_index].any_present)
        {
          PopupItem (pfp->popup_list[popup_index], 
                   DefLineModifiers[item_index].name);
          listed_index ++;
        }
      }
      PopupItem (pfp->popup_list[popup_index], "Ignore");
      listed_index++;
      SetValue (pfp->popup_list[popup_index], listed_index);
      popup_index ++;
    }
  }

  pfp->add_org_name = CheckBox (h, "Prefix with taxonomy name", NULL);
  SetStatus (pfp->add_org_name, TRUE);
  pfp->use_labels = CheckBox (h, "Use modifier labels", NULL);
  SetStatus (pfp->use_labels, TRUE);

  c = HiddenGroup (h, 4, 0, NULL);
  b = DefaultButton (c, "Accept", DoPrefixDefLines);
  SetObjectExtra (b, pfp, NULL);
  PushButton (c, "Cancel", StdCancelButtonProc);

  AlignObjects (ALIGN_CENTER, (HANDLE) g, (HANDLE) pfp->add_org_name,
                (HANDLE) pfp->use_labels,
                (HANDLE) c, NULL);
  RealizeWindow (w);
  Show (w);
  Update ();

}

#ifndef WIN16
CharPtr objPrtMemStr = "PrintTemplateSet ::= {\n" \
"{ name \"StdSeqDesc\" ,\n" \
"format { asn1 \"Seqdesc\" , form block {\n" \
"components {\n" \
"{ asn1 \"Seqdesc.mol-type\" , label \"Molecule type\" , prefix \"\\n\" , form enum { } } ,\n" \
"{ asn1 \"Seqdesc.modif\" , label \"Modifiers\" , prefix \"\\n\" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Seqdesc.modif.E\" , form enum { } } } } } ,\n" \
"{ asn1 \"Seqdesc.method\" , label \"Method\" , prefix \"\\n\" , form enum { } } ,\n" \
"{ asn1 \"Seqdesc.name\" , label \"Name\" , prefix \"\\n\" , form text { } } ,\n" \
"{ asn1 \"Seqdesc.title\" , label \"Title\" , prefix \"\\n\" , form text { } } ,\n" \
"{ asn1 \"Seqdesc.org\" , label \"Organism\" , prefix \"\\n\" , form use-template \"StdOrgRef\" } ,\n" \
"{ asn1 \"Seqdesc.comment\" , label \"Comment\" , prefix \"\\n\" , form text { } } ,\n" \
"{ asn1 \"Seqdesc.num\" , label \"Numbering\" , prefix \"\\n\" , form use-template \"StdNumbering\" } ,\n" \
"{ asn1 \"Seqdesc.maploc\" , label \"Map location\" , prefix \"\\n\" , form use-template \"StdDbtag\" } ,\n" \
"{ asn1 \"Seqdesc.pir\" , label \"PIR block\" , prefix \"\\n\" , form null NULL } ,\n" \
"{ asn1 \"Seqdesc.genbank\" , label \"GenBank block\" , prefix \"\\n\" , form use-template \"StdGBBlock\" } ,\n" \
"{ asn1 \"Seqdesc.pub\" , label \"Citation\" , prefix \"\\n\" , form use-template \"StdPubdesc\" } ,\n" \
"{ asn1 \"Seqdesc.region\" , label \"Region\" , prefix \"\\n\" , form text { } } ,\n" \
"{ asn1 \"Seqdesc.user\" , label \"User Type\" , prefix \"\\n\" , form use-template \"StdUserObj\" } ,\n" \
"{ asn1 \"Seqdesc.sp\" , label \"SWISS-PROT block\" , prefix \"\\n\" , form null NULL } ,\n" \
"{ asn1 \"Seqdesc.dbxref\" , label \"Cross reference\" , prefix \"\\n\" , form use-template \"StdDbtag\"  } ,\n" \
"{ asn1 \"Seqdesc.embl\" , label \"EMBL block\" , prefix \"\\n\" , form null NULL } ,\n" \
"{ asn1 \"Seqdesc.create-date\" , label \"Create date\" , prefix \"\\n\" , form user { printfunc \"StdDatePrint\" } } ,\n" \
"{ asn1 \"Seqdesc.update-date\" , label \"Update date\" , prefix \"\\n\" , form user { printfunc \"StdDatePrint\" } } ,\n" \
"{ asn1 \"Seqdesc.prf\" , label \"PRF block\" , prefix \"\\n\" , form null NULL } ,\n" \
"{ asn1 \"Seqdesc.pdb\" , label \"PDB block\" , prefix \"\\n\" , form null NULL } ,\n" \
"{ asn1 \"Seqdesc.het\" , label \"Heterogen\" , prefix \"\\n\" , form text { } } ,\n" \
"{ asn1 \"Seqdesc.source\" , label \"Biological Source\" , prefix \"\\n\" , form use-template \"StdBioSource\" } ,\n" \
"{ asn1 \"Seqdesc.molinfo\" , label \"Molecule Information\" , prefix \"\\n\" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"MolInfo.biomol\" , form enum { } } ,\n" \
"{ asn1 \"MolInfo.tech\" , form enum { } } ,\n" \
"{ asn1 \"MolInfo.completeness\" , form enum { } } } } } } } } } ,\n" \
"{ name \"StdSeqFeatLocation\" ,\n" \
"format { asn1 \"Seq-feat.location\" , label \"Location\" , prefix \"\\t\" , form user { printfunc \"StdSeqLocPrint\" } } } ,\n" \
"{ name \"StdSeqFeatProduct\" ,\n" \
"format { asn1 \"Seq-feat.product\" , label \"Product\" , prefix \"\\t\" , form user { printfunc \"StdSeqLocPrint\" } } } ,\n" \
"{ name \"EntrySeqFeatData\" ,\n" \
"labelfrom \"Seq-feat.data\" ,\n" \
"format { asn1 \"Seq-feat.data\" , prefix \"\\t\" , form use-template \"StdSeqFeatData\" } } ,\n" \
"{ name \"StdSeqFeat\" ,\n" \
"labelfrom \"Seq-feat.data\" ,\n" \
"format { asn1 \"Seq-feat\" , prefix \"\\n\" , suffix \"\\n\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Seq-feat.data\" , form use-template \"StdSeqFeatData\" } ,\n" \
"{ asn1 \"Seq-feat\" , form use-template \"StdSeqFeatCommon\" } ,\n" \
"{ asn1 \"Seq-feat.product\" , label \"Product\" , prefix \" \" , form user { printfunc \"StdSeqLocPrint\" } } ,\n" \
"{ asn1 \"Seq-feat.location\" , label \"Location\" , prefix \" \" , form user { printfunc \"StdSeqLocPrint\" } } ,\n" \
"{ asn1 \"Seq-feat.cit\" , label \"Citations\" , prefix \"\\n\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Seq-feat.cit.pub.E\" , form use-template \"StdPub\" } } } } ,\n" \
"{ asn1 \"Seq-feat.xref\" , label \"Cross-reference\" , prefix \"\\n\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Seq-feat.xref.E\" , form use-template \"StdSeqFeatXref\" } } } } } } } } ,\n" \
"{ name \"StdSeqFeatData\" ,\n" \
"format { asn1 \"SeqFeatData\" , form block {\n" \
"components {\n" \
"{ asn1 \"SeqFeatData.gene\" , label \"Gene\" , form use-template \"StdGeneRef\" } ,\n" \
"{ asn1 \"SeqFeatData.org\" , label \"Organism\" , form use-template \"StdOrgRef\" } ,\n" \
"{ asn1 \"SeqFeatData.cdregion\" , label \"Coding Region\" , form use-template \"StdCdRegion\" } ,\n" \
"{ asn1 \"SeqFeatData.prot\" , label \"Protein\" , form use-template \"StdProtRef\" } ,\n" \
"{ asn1 \"SeqFeatData.rna\" , label \"RNA\" , form use-template \"StdRNARef\" } ,\n" \
"{ asn1 \"SeqFeatData.pub\" , label \"Citation\" , form use-template \"StdPubdesc\" } ,\n" \
"{ asn1 \"SeqFeatData.seq\" , label \"Sequence\" , form user { printfunc \"StdSeqLocPrint\" } } ,\n" \
"{ asn1 \"SeqFeatData.imp.key\" , label \"Import\" , form use-template \"StdImpFeat\" } ,\n" \
"{ asn1 \"SeqFeatData.region\" , label \"Region\" , form text { } } ,\n" \
"{ asn1 \"SeqFeatData.comment\" , label \"Comment\" , form null NULL } ,\n" \
"{ asn1 \"SeqFeatData.bond\" , label \"Bond\" , form enum { } } ,\n" \
"{ asn1 \"SeqFeatData.site\" , label \"Site\" , form enum { } } ,\n" \
"{ asn1 \"SeqFeatData.rsite\" , label \"Rest. Site\" , form use-template \"StdRsiteRef\" } ,\n" \
"{ asn1 \"SeqFeatData.user\" , label \"User Type\" , form use-template \"StdUserObj\" } ,\n" \
"{ asn1 \"SeqFeatData.txinit\" , label \"TxInit\" , form use-template \"StdTxInit\" } ,\n" \
"{ asn1 \"SeqFeatData.num\" , label \"Numbering\" , form use-template \"StdNumbering\" } ,\n" \
"{ asn1 \"SeqFeatData.psec-str\" , label \"Sec. Struct\" , form enum { } } ,\n" \
"{ asn1 \"SeqFeatData.non-std-residue\" , label \"NonStd Residue\" , form text { } } ,\n" \
"{ asn1 \"SeqFeatData.het\" , label \"Heterogen\" , form text { } } ,\n" \
"{ asn1 \"SeqFeatData.biosrc\" , label \"Biological Source\" , prefix \"\\n\" , form use-template \"StdBioSource\" } } } } } ,\n" \
"{ name \"StdGeneRef\" ,\n" \
"format { asn1 \"Gene-ref\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Gene-ref\" , form block {\n" \
"components {\n" \
"{ asn1 \"Gene-ref.locus\" , form text { } } ,\n" \
"{ asn1 \"Gene-ref.allele\" , prefix \" \" , form text { } } } } } ,\n" \
"{ asn1 \"Gene-ref.desc\" , prefix \"[\" , suffix \"]\" , form text { } } ,\n" \
"{ asn1 \"Gene-ref.pseudo\" , form boolean {\n" \
"true \"This is a pseudogene.\" } } ,\n" \
"{ asn1 \"Gene-ref.syn\" , label \"Synonyms\" , prefix \" (\" , suffix \")\" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Gene-ref.syn.E\" , form text { } } } } } ,\n" \
"{ asn1 \"Gene-ref.maploc\" , label \"Map Location\" , prefix \" \" , form text { } } ,\n" \
"{ asn1 \"Gene-ref.db\" , label \"Cross Reference\" , prefix \" \" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Gene-ref.db.E\" , prefix \"(\" , suffix \")\" , form use-template \"StdDbtag\" } } } } } } } } ,\n" \
"{ name \"StdUserObj\" ,\n" \
"format { asn1 \"User-object\" , label \"User-object\" , prefix \"\\n\" , form block {\n" \
"separator \": \" ,\n" \
"components {\n" \
"{ asn1 \"User-object.class\" , form text { } } ,\n" \
"{ asn1 \"User-object.type\" , form use-template \"StdObjectId\" } } } } } ,\n" \
"{ name \"StdPubOnFeat\" ,\n" \
"format { asn1 \"Pub\" , label \"Citation\" , prefix \"\\n\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Pub\" , form use-template \"StdPub\" } } } } } ,\n" \
"{ name \"StdPub\" ,\n" \
"format { asn1 \"Pub\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Pub.gen\" , form use-template \"StdCitGen\" } ,\n" \
"{ asn1 \"Pub.sub\" , form use-template \"StdCitSub\" } ,\n" \
"{ asn1 \"Pub.medline\" , form use-template \"StdMedlineEntry\" } ,\n" \
"{ asn1 \"Pub.muid\" , label \"MEDLINE uid: \" , form text { } } ,\n" \
"{ asn1 \"Pub.pmid\" , label \"PubMed id: \" , form text { } } ,\n" \
"{ asn1 \"Pub.article\" , form use-template \"StdCitArt\" } ,\n" \
"{ asn1 \"Pub.journal\" , form use-template \"StdCitJour\" } ,\n" \
"{ asn1 \"Pub.book\" , form use-template \"StdCitBook\" } ,\n" \
"{ asn1 \"Pub.proc\" , form use-template \"StdCitProc\" } ,\n" \
"{ asn1 \"Pub.patent\" , form use-template \"StdCitPat\" } ,\n" \
"{ asn1 \"Pub.pat-id\" , form use-template \"StdIdPat\" } ,\n" \
"{ asn1 \"Pub.man\" , form use-template \"StdCitLet\" } ,\n" \
"{ asn1 \"Pub.equiv\" , form use-template \"StdPubEquiv\" } } } } } ,\n" \
"{ name \"StdCitGen\" ,\n" \
"format { asn1 \"Cit-gen\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Cit-gen.serial-number\" , prefix \"[\" , suffix \"]\" , form text { } } ,\n" \
"{ asn1 \"Cit-gen.authors\" , form use-template \"StdAuthList\" } ,\n" \
"{ asn1 \"Cit-gen.date\" , prefix \"(\" , suffix \")\" , form user { printfunc \"StdDatePrint\" } } ,\n" \
"{ asn1 \"Cit-gen.title\" , form text { } } ,\n" \
"{ asn1 \"Cit-gen.cit\" , form text { } } ,\n" \
"{ asn1 \"Cit-gen\" , form block {\n" \
"separator \" \" ,\n" \
"components {\n" \
"{ asn1 \"Cit-gen.journal\" , suffix \":\" , form use-template \"StdTitle\" } ,\n" \
"{ asn1 \"Cit-gen.issue\" , suffix \";\" , form text { } } ,\n" \
"{ asn1 \"Cit-gen.pages\" , form text { } } } } } } } } } ,\n" \
"{ name \"StdCitSub\" ,\n" \
"format { asn1 \"Cit-sub\" , prefix \"Data Submission \" , form block {\n" \
"components {\n" \
"{ asn1 \"Cit-sub.medium\" , prefix \"on \" , suffix \" \" , form enum { } } ,\n" \
"{ asn1 \"Cit-sub.imp.date\" , prefix \"(\" , suffix \")\" , form user { printfunc \"StdDatePrint\" } } ,\n" \
"{ asn1 \"Cit-sub.authors\" , prefix \"\\n\" , form use-template \"StdAuthList\" } } } } } ,\n" \
"{ name \"StdMedlineEntry\" ,\n" \
"format { asn1 \"Medline-entry\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Medline-entry\" , form block {\n" \
"separator \"   \" ,\n" \
"components {\n" \
"{ asn1 \"Medline-entry.uid\" , label \"uid\" , prefix \": \" , form text { } } ,\n" \
"{ asn1 \"Medline-entry.em\" , label \"entry month\" , prefix \": \" , form user { printfunc \"StdDatePrint\" } } } } } ,\n" \
"{ asn1 \"Medline-entry.cit\" , form use-template \"StdCitArt\" } ,\n" \
"{ asn1 \"Medline-entry.abstract\" , label \"abstract\" , prefix \": \" , form text { } } ,\n" \
"{ asn1 \"Medline-entry.mesh\" , label \"Mesh Terms\" , prefix \"\\n\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Medline-entry.mesh.E\" , form block {\n" \
"components {\n" \
"{ asn1 \"Medline-mesh.term\" , form text { } } ,\n" \
"{ asn1 \"Medline-mesh.mp\" , form boolean {\n" \
"true \" (Main Point)\" } } ,\n" \
"{ asn1 \"Medline-mesh.qual\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Medline-mesh.qual.E\" , form block {\n" \
"components {\n" \
"{ asn1 \"Medline-qual.subh\" , form text { } } ,\n" \
"{ asn1 \"Medline-qual.mp\" , form boolean {\n" \
"true \" (Main Point)\" } } } } } } } } } } } } } } ,\n" \
"{ asn1 \"Medline-entry.substance\" , label \"Substance\" , prefix \"\\n\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Medline-entry.substance.E\" , form block {\n" \
"components {\n" \
"{ asn1 \"Medline-rn.name\" , form text { } } ,\n" \
"{ asn1 \"Medline-rn.type\" , form enum {\n" \
"values {\n" \
"\"\" ,\n" \
"\" CAS: \" ,\n" \
"\"EC \" } } } ,\n" \
"{ asn1 \"Medline-rn.cit\" , form text { } } } } } } } } ,\n" \
"{ asn1 \"Medline-entry.xref\" , label \"Cross Reference\" , prefix \"\\n\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Medline-entry.xref.E\" , form block {\n" \
"separator \": \" ,\n" \
"components {\n" \
"{ asn1 \"Medline-si.type\" , form enum { } } ,\n" \
"{ asn1 \"Medline-si.cit\" , form text { } } } } } } } } ,\n" \
"{ asn1 \"Medline-entry.gene\" , label \"Possible Gene Symbols\" , prefix \": \" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Medline-entry.gene.E\" , form text { } } } } } ,\n" \
"{ asn1 \"Medline-entry.idnum\" , label \"Support\" , prefix \": \" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Medline-entry.idnum.E\" , form text { } } } } } } } } } ,\n" \
"{ name \"StdCitArt\" ,\n" \
"format { asn1 \"Cit-art\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Cit-art.title\" , form use-template \"StdTitle\" } ,\n" \
"{ asn1 \"Cit-art.authors\" , form use-template \"StdAuthList\" } ,\n" \
"{ asn1 \"Cit-art.from.journal\" , form use-template \"StdCitJour\" } ,\n" \
"{ asn1 \"Cit-art.from.book\" , prefix \"(in) \" , form use-template \"StdCitBook\" } ,\n" \
"{ asn1 \"Cit-art.from.proc\" , prefix \"(in) \" , form use-template \"StdCitProc\" } } } } } ,\n" \
"{ name \"StdCitJour\" ,\n" \
"format { asn1 \"Cit-jour\" , form block {\n" \
"separator \" \" ,\n" \
"components {\n" \
"{ asn1 \"Cit-jour.title\" , form use-template \"StdTitle\" } ,\n" \
"{ asn1 \"Cit-jour.imp\" , form use-template \"StdImprint\" } } } } } ,\n" \
"{ name \"StdCitBook\" ,\n" \
"format { asn1 \"Cit-book\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Cit-book.title\" , form use-template \"StdTitle\" } ,\n" \
"{ asn1 \"Cit-book.coll\" , prefix \"Collection: \" , form use-template \"StdTitle\" } ,\n" \
"{ asn1 \"Cit-book.authors\" , form use-template \"StdAuthList\" } ,\n" \
"{ asn1 \"Cit-book.imp\" , form use-template \"StdImprint\" } } } } } ,\n" \
"{ name \"StdCitProc\" ,\n" \
"format { asn1 \"Cit-proc\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Cit-proc.book\" , form use-template \"StdCitBook\" } ,\n" \
"{ asn1 \"Cit-proc.meet\" , label \"Meeting \" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Meeting.number\" , form text { } } ,\n" \
"{ asn1 \"Meeting.date\" , form user { printfunc \"StdDatePrint\" } } ,\n" \
"{ asn1 \"Meeting.place\" , form use-template \"StdAffil\" } } } } } } } } ,\n" \
"{ name \"StdCitPat\" ,\n" \
"format { asn1 \"Cit-pat\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Cit-pat.title\" , form text { } } ,\n" \
"{ asn1 \"Cit-pat.authors\" , form use-template \"StdAuthList\" } ,\n" \
"{ asn1 \"Cit-pat\" , form block {\n" \
"components {\n" \
"{ asn1 \"Cit-pat.country\" , suffix \" \" , form text { } } ,\n" \
"{ asn1 \"Cit-pat.doc-type\" , form text { } } ,\n" \
"{ asn1 \"Cit-pat.number\" , form text { } } ,\n" \
"{ asn1 \"Cit-pat.date-issue\" , prefix \" (\" , suffix \")\" , form user { printfunc \"StdDatePrint\" } } ,\n" \
"{ asn1 \"Cit-pat.app-number\" , prefix \" Appl: \" , form text { } } ,\n" \
"{ asn1 \"Cit-pat.app-date\" , prefix \" (\" , suffix \")\" , form user { printfunc \"StdDatePrint\" } } } } } } } } } ,\n" \
"{ name \"StdIdPat\" ,\n" \
"format { asn1 \"Id-pat\" , form block {\n" \
"components {\n" \
"{ asn1 \"Id-pat.country\" , suffix \" \" , form text { } } ,\n" \
"{ asn1 \"Id-pat.id.number\" , form text { } } ,\n" \
"{ asn1 \"Id-pat.id.app-number\" , prefix \"Appl: \" , form text { } } } } } } ,\n" \
"{ name \"StdCitLet\" ,\n" \
"format { asn1 \"Cit-let\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Cit-let.type\" , prefix \"[\" , suffix \"]\" , form enum { } } ,\n" \
"{ asn1 \"Cit-let.man-id\" , form text { } } ,\n" \
"{ asn1 \"Cit-let.cit\" , form use-template \"StdCitBook\" } } } } } ,\n" \
"{ name \"StdPubEquiv\" ,\n" \
"format { asn1 \"Pub-equiv\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Pub-equiv.E\" , form use-template \"StdPub\" } } } } } ,\n" \
"{ name \"StdTitle\" ,\n" \
"format { asn1 \"Title\" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Title.E.trans\" , prefix \"[\" , suffix \"]\" , form text { } } ,\n" \
"{ asn1 \"Title.E.name\" , form text { } } ,\n" \
"{ asn1 \"Title.E.tsub\" , form text { } } ,\n" \
"{ asn1 \"Title.E.abr\" , form text { } } ,\n" \
"{ asn1 \"Title.E.iso-jta\" , form text { } } ,\n" \
"{ asn1 \"Title.E.ml-jta\" , label \"MEDLINE\" , prefix \": \" , form text { } } ,\n" \
"{ asn1 \"Title.E.jta\" , label \"jta\" , prefix \": \" , form text { } } ,\n" \
"{ asn1 \"Title.E.issn\" , label \"ISSN\" , prefix \": \" , form text { } } ,\n" \
"{ asn1 \"Title.E.coden\" , label \"CODEN\" , prefix \": \" , form text { } } ,\n" \
"{ asn1 \"Title.E.isbn\" , label \"ISBN\" , prefix \": \" , form text { } } } } } } ,\n" \
"{ name \"StdAuthList\" ,\n" \
"format { asn1 \"Auth-list\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Auth-list\" , form user { printfunc \"StdAuthListNamesPrint\" } } ,\n" \
"{ asn1 \"Auth-list.affil\" , form use-template \"StdAffil\" } } } } } ,\n" \
"{ name \"StdAffil\" ,\n" \
"format { asn1 \"Affil\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Affil.str\" , form text { } } ,\n" \
"{ asn1 \"Affil.std.affil\" , form text { } } ,\n" \
"{ asn1 \"Affil.std.div\" , form text { } } ,\n" \
"{ asn1 \"Affil.std.street\" , form text { } } ,\n" \
"{ asn1 \"Affil.std\" , form block {\n" \
"separator \" \" ,\n" \
"components {\n" \
"{ asn1 \"Affil.std.city\" , form text { } } ,\n" \
"{ asn1 \"Affil.std.sub\" , form text { } } ,\n" \
"{ asn1 \"Affil.std.country\" , form text { } } } } } } } } } ,\n" \
"{ name \"StdImprint\" ,\n" \
"format { asn1 \"Imprint\" , form block {\n" \
"components {\n" \
"{ asn1 \"Imprint.date\" , prefix \"(\" , suffix \") \" , form user { printfunc \"StdDatePrint\" } } ,\n" \
"{ asn1 \"Imprint.volume\" , form text { } } ,\n" \
"{ asn1 \"Imprint.issue\" , prefix \" (\" , suffix \")\" , form text { } } ,\n" \
"{ asn1 \"Imprint.section\" , prefix \" (\" , suffix \")\" , form text { } } ,\n" \
"{ asn1 \"Imprint.part-sup\" , prefix \" (\" , suffix \")\" , form text { } } ,\n" \
"{ asn1 \"Imprint.pages\" , prefix \": \" , form text { } } ,\n" \
"{ asn1 \"Imprint.prepub\" , prefix \" (\" , suffix \")\" , form enum { } } ,\n" \
"{ asn1 \"Imprint.pub\" , label \"\nPublisher: \" , form use-template \"StdAffil\" } ,\n" \
"{ asn1 \"Imprint.cprt\" , label \" Copyright: \" , form user { printfunc \"StdDatePrint\" } } } } } } ,\n" \
"{ name \"StdSeqFeatXref\" ,\n" \
"format { asn1 \"SeqFeatXref\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"SeqFeatXref.id\" , label \"Id=\" , form use-template \"StdFeatId\" } ,\n" \
"{ asn1 \"SeqFeatXref.data\" , form use-template \"StdSeqFeatData\" } } } } } ,\n" \
"{ name \"StdOrgRef\" ,\n" \
"format { asn1 \"Org-ref\" , label \"Org-ref\" , prefix \"\\n\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Org-ref\" , form block {\n" \
"separator \" \" ,\n" \
"components {\n" \
"{ asn1 \"Org-ref.taxname\" , form text { } } ,\n" \
"{ asn1 \"Org-ref.common\" , prefix \"(\" , suffix \")\" , form text { } } } } } ,\n" \
"{ asn1 \"Org-ref.mod\" , label \"Modifiers\" , prefix \" (\" , suffix \")\" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Org-ref.mod.E\" , form text { } } } } } ,\n" \
"{ asn1 \"Org-ref.db\" , label \"Cross Reference\" , prefix \" \" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Org-ref.db.E\" , prefix \"(\" , suffix \")\" , form use-template \"StdDbtag\" } } } } ,\n" \
"{ asn1 \"Org-ref.syn\" , label \"Synonyms\" , prefix \" (\" , suffix \")\" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Org-ref.syn.E\" , form text { } } } } } } } } } ,\n" \
"{ name \"StdBioSource\" ,\n" \
"format { asn1 \"BioSource\" , label \"BioSource\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"BioSource.genome\" , form enum { } } ,\n" \
"{ asn1 \"BioSource.org\" , label \"Organism\" , form use-template \"StdOrgRef\" } } } } } ,\n" \
"{ name \"StdCdRegion\" ,\n" \
"format { asn1 \"Cdregion\" , label \"Cdregion\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Cdregion.orf\" , form boolean {\n" \
"true \"Uncharacterized Open Reading Frame\" } } ,\n" \
"{ asn1 \"Cdregion.frame\" , label \"Reading Frame = \" , form enum { } } ,\n" \
"{ asn1 \"Cdregion.code\" , label \"Genetic Code: \" , suffix \";\" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Genetic-code.E.name\" , form text { } } ,\n" \
"{ asn1 \"Genetic-code.E.id\" , label \"id= \" , form text { } } } } } ,\n" \
"{ asn1 \"Cdregion.conflict\" , form boolean {\n" \
"true \"Translation conflicts with protein sequence\" } } ,\n" \
"{ asn1 \"Cdregion.stops\" , prefix \"Translation contains \" , suffix \" stop codons\" , form text { } } ,\n" \
"{ asn1 \"Cdregion.gaps\" , prefix \"Translation contains \" , suffix \" gaps when aligned to protein\" , form text { } } ,\n" \
"{ asn1 \"Cdregion.mismatch\" , prefix \"Translation contains \" , suffix \" mismatches when aligned to protein\" , form text { } } } } } } ,\n" \
"{ name \"StdProtRef\" ,\n" \
"format { asn1 \"Prot-ref\" , label \"Prot-ref\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Prot-ref.name\" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Prot-ref.name.E\" , form text { } } } } } ,\n" \
"{ asn1 \"Prot-ref.desc\" , prefix \"[\" , suffix \"]\" , form text { } } ,\n" \
"{ asn1 \"Prot-ref.processed\" , form enum { } } ,\n" \
"{ asn1 \"Prot-ref.ec\" , label \"ec\" , prefix \": \" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Prot-ref.ec.E\" , form text { } } } } } ,\n" \
"{ asn1 \"Prot-ref.activity\" , label \"activity\" , prefix \": \" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Prot-ref.activity.E\" , form text { } } } } } ,\n" \
"{ asn1 \"Prot-ref.db\" , label \"Cross Reference\" , prefix \" \" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Prot-ref.db.E\" , prefix \"(\" , suffix \")\" , form use-template \"StdDbtag\" } } } } } } } } ,\n" \
"{ name \"StdRNARef\" ,\n" \
"format { asn1 \"RNA-ref\" , label \"RNA-ref\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"RNA-ref.type\" , form enum { } } ,\n" \
"{ asn1 \"RNA-ref.pseudo\" , form boolean {\n" \
"true \"This is an RNA pseudogene.\" } } ,\n" \
"{ asn1 \"RNA-ref.ext.name\" , form text { } } } } } } ,\n" \
"{ name \"StdPubdesc\" ,\n" \
"format { asn1 \"Pubdesc\" , label \"Pubdesc\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Pubdesc.pub\" , form use-template \"StdPubEquiv\" } ,\n" \
"{ asn1 \"Pubdesc\" , prefix \"In this article:\n\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Pubdesc.name\" , label \"name=\" , form text { } } ,\n" \
"{ asn1 \"Pubdesc.fig\" , label \"figure=\" , form text { } } ,\n" \
"{ asn1 \"Pubdesc.poly-a\" , form boolean {\n" \
"true \"poly(A) shown\" } } ,\n" \
"{ asn1 \"Pubdesc.maploc\" , label \"map location=\" , form text { } } ,\n" \
"{ asn1 \"Pubdesc.num\" , form use-template \"StdNumbering\" } ,\n" \
"{ asn1 \"Pubdesc.numexc\" , form boolean {\n" \
"true \"numbering inconsistent\" } } } } } ,\n" \
"{ asn1 \"Pubdesc.comment\" , form text { } } } } } } ,\n" \
"{ name \"StdImpFeat\" ,\n" \
"format { asn1 \"Imp-feat.key\" , label \"Imp-feat\" , form text { } } } ,\n" \
"{ name \"StdRsiteRef\" ,\n" \
"format { asn1 \"Rsite-ref\" , label \"Rsite-ref\" , form block {\n" \
"components {\n" \
"{ asn1 \"Rsite-ref.str\" , form text { } } ,\n" \
"{ asn1 \"Rsite-ref.std\" , form use-template \"StdDbtag\" } } } } } ,\n" \
"{ name \"StdTxInit\" ,\n" \
"format { asn1 \"Txinit\" , label \"TxInit\" , form block {\n" \
"components {\n" \
"{ asn1 \"Txinit.name\" , form text { } } } } } } ,\n" \
"{ name \"StdNumbering\" ,\n" \
"format { asn1 \"Numbering\" , label \"Numbering\" , form null NULL } } ,\n" \
"{ name \"StdGBBlock\" ,\n" \
"format { asn1 \"GB-block\" , label \"GenBank-block\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"GB-block.extra-accessions\" , label \"Extra accessions\" , prefix \" (\" , suffix \")\" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"GB-block.extra-accessions.E\" , form text { } } } } } ,\n" \
"{ asn1 \"GB-block.keywords\" , label \"Keywords\" , prefix \" (\" , suffix \")\" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"GB-block.keywords.E\" , form text { } } } } } ,\n" \
"{ asn1 \"GB-block.source\" , label \"Source: \" , form text { } } ,\n" \
"{ asn1 \"GB-block.origin\" , label \"Origin: \" , form text { } } ,\n" \
"{ asn1 \"GB-block.div\" , label \"Division: \" , form text { } } ,\n" \
"{ asn1 \"GB-block.taxonomy\" , label \"Taxonomy: \" , form text { } } ,\n" \
"{ asn1 \"GB-block.date\" , label \"Date: \" , form text { } } ,\n" \
"{ asn1 \"GB-block.entry-date\" , label \"Entry date: \" , form user { printfunc \"StdDatePrint\" } } } } } } ,\n" \
"{ name \"StdFeatId\" ,\n" \
"format { asn1 \"Feat-id\" , form block {\n" \
"components {\n" \
"{ asn1 \"Feat-id.gibb\" , label \"GenInfo Backbone: \" , form text { } } ,\n" \
"{ asn1 \"Feat-id.giim.id\" , label \"GenInfo Import Id: \" , form text { } } ,\n" \
"{ asn1 \"Feat-id.local\" , label \"Local: \" , form use-template \"StdObjectId\" } ,\n" \
"{ asn1 \"Feat-id.general\" , form use-template \"StdDbtag\" } } } } } ,\n" \
"{ name \"StdSeqFeatCommon\" ,\n" \
"format { asn1 \"Seq-feat\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Seq-feat.id\" , label \"Id=\" , form use-template \"StdFeatId\" } ,\n" \
"{ asn1 \"Seq-feat.title\" , form text { } } ,\n" \
"{ asn1 \"Seq-feat\" , suffix \";\" , form block {\n" \
"separator \", \" ,\n" \
"components {\n" \
"{ asn1 \"Seq-feat.partial\" , form boolean {\n" \
"true \"Partial\" } } ,\n" \
"{ asn1 \"Seq-feat.except\" , form boolean {\n" \
"true \"Biological Exception\" } } ,\n" \
"{ asn1 \"Seq-feat.exp-ev\" , label \"Evidence\" , prefix \" is \" , form enum { } } } } } ,\n" \
"{ asn1 \"Seq-feat.comment\" , form text { } } ,\n" \
"{ asn1 \"Seq-feat.ext\" , form use-template \"StdUserObj\" } ,\n" \
"{ asn1 \"Seq-feat.qual\" , label \"Qualifiers\" , prefix \"\\n\" , form block {\n" \
"separator \"\\n\" ,\n" \
"components {\n" \
"{ asn1 \"Seq-feat.qual.E\" , prefix \"/\" , form block {\n" \
"separator \"= \" ,\n" \
"components {\n" \
"{ asn1 \"Gb-qual.qual\" , form text { } } ,\n" \
"{ asn1 \"Gb-qual.val\" , form text { } } } } } } } } } } } } ,\n" \
"{ name \"StdDbtag\" ,\n" \
"format { asn1 \"Dbtag\" , form block {\n" \
"components {\n" \
"{ asn1 \"Dbtag.db\" , suffix \": \" , form text { } } ,\n" \
"{ asn1 \"Dbtag.tag\" , form use-template \"StdObjectId\" } } } } } ,\n" \
"{ name \"StdObjectId\" ,\n" \
"format { asn1 \"Object-id\" , form block {\n" \
"components {\n" \
"{ asn1 \"Object-id.id\" , form text { } } ,\n" \
"{ asn1 \"Object-id.str\" , form text { } } } } } } };\n";
#else
CharPtr objPrtMemStr = "";
#endif

