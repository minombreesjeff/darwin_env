/**************************************************************************
*   gbftglob.c:
*   -- all the globally variables for gbfeat.c
*   -- all the defined variables in the gbfeat.h
*
* $Log: gbftglob.c,v $
* Revision 6.41  2004/10/21 14:37:35  kans
* gap feature has note and map qualifiers
*
* Revision 6.40  2004/08/19 20:56:02  kans
* changed opt_num (SB caught this oversight)
*
* Revision 6.39  2004/08/17 15:50:52  kans
* added GBQUAL_old_locus_tag and GBQUAL_compare
*
* Revision 6.38  2004/03/02 17:15:44  kans
* added GBQUAL_allele to -10_signal and -35_signal
*
* Revision 6.37  2003/10/07 17:05:58  kans
* added allele and operon to many features
*
* Revision 6.36  2003/10/07 13:50:36  kans
* added gap, operon, oriT features and ecotype, estimated_length and operon qualifiers
*
* Revision 6.35  2003/10/06 16:19:45  kans
* rpt_unit went from Class_token to Class_text
*
* Revision 6.34  2003/10/03 15:16:14  bazhin
* Numeric value "opt_num" in STATIC__ParFlat_GBFeat array changed
* from 13 to 15 for feature "allele" to cover "usedin" and "locus_tag"
* qualifiers.
*
* Revision 6.33  2003/10/03 12:44:27  bazhin
* Numeric value "opt_num" in STATIC__ParFlat_GBFeat array changed
* from 13 to 14 for feature "precursor_RNA" to cover "locus_tag"
* qualifier.
*
* Revision 6.32  2003/08/19 15:19:04  kans
* added GBQUAL_segment, increased ParFlat_TOTAL_GBQUAL and opt_qual array size
*
* Revision 6.31  2003/05/07 22:03:31  kans
* added GBQUAL_mol_type, raised opt_qual array to 51 elements
*
* Revision 6.30  2003/03/06 16:23:13  kans
* when gene qualifier was removed as required on gene feature, it was not put back as optional qualifier
*
* Revision 6.29  2003/02/26 19:30:19  kans
* GBQUAL_gene is no longer required for gene features, since locus_tag will be able to suffice
*
* Revision 6.28  2003/02/24 20:09:04  kans
* added GBQUAL_locus_tag to all remaining features
*
* Revision 6.27  2003/02/22 21:20:05  kans
* added GBQUAL_locus_tag, GBQUAL_old_locus_tag, legal for now in gene features
*
* Revision 6.26  2002/04/17 19:11:48  kans
* added serovar to GBQual_names
*
* Revision 6.25  2002/04/17 14:41:08  kans
* added GBQUAL_serovar to source feature
*
* Revision 6.24  2002/04/02 21:10:24  kans
* restored organelle and transcript_id, which accidentally got deleted
*
* Revision 6.23  2002/03/26 16:06:31  kans
* added transgenic, environmental_sample, and isolation_source
*
* Revision 6.22  2002/02/13 18:45:52  kans
* increased ParFlat_TOTAL_GBFEAT, added snoRNA
*
* Revision 6.21  2001/12/11 17:56:42  kans
* allele and phenotype added to several features
*
* Revision 6.20  2001/11/28 15:34:38  kans
* added /transposon and /insertion_seq to repeat_region
*
* Revision 6.19  2001/08/30 18:29:39  kans
* added db_xref to prim_transcript, count already correct
*
* Revision 6.18  2001/08/20 13:55:58  kans
* -35_signal has standard_name
*
* Revision 6.17  2001/04/10 22:17:11  tatiana
* GBQUAL_endogenous_virus backed off to /note
*
* Revision 6.16  2001/04/10 21:59:28  tatiana
* GBQUAL_endogenous_virus added
*
* Revision 6.15  2001/01/30 16:20:02  kans
* precursor_RNA allows /product
*
* Revision 6.14  2000/11/29 20:37:00  tatiana
* virion key removed, GBQUAL_transcript_id added
*
* Revision 6.13  2000/10/20 16:59:47  tatiana
* evidence qualifier is legal on all features
*
* Revision 6.12  2000/05/17 16:21:10  kans
* allele is legal on variation features
*
* Revision 6.11  2000/04/03 17:37:20  tatiana
* evidence added to STS qualifiers
*
* Revision 6.10  2000/02/02 21:02:33  tatiana
* new type for /number added
*
* Revision 6.8  2000/01/21 21:06:44  kans
* reverted qualifiers, just added organelle, so parser can deal with old and new form during transition
*
* Revision 6.7  2000/01/21 20:48:19  kans
* changes to merge several source qualifiers under new organelle qualifier
*
* Revision 6.6  1999/12/22 21:50:56  kans
* when frequency was added to source qual legal list, the count number was not raised, bumping country off of the recognized list - my fault
*
* Revision 6.5  1999/12/15 23:28:37  kans
* frequency is legal qualifier for source feature
*
* Revision 6.4  1999/02/05 15:40:47  tatiana
* GBQUAL_country added
*
* Revision 6.3  1999/01/29 16:18:37  tatiana
* protein_id qualifier added to CDS
*
* Revision 6.2  1998/01/15 20:28:08  tatiana
* increased the size of optional quals array for source feature
*
* Revision 6.1  1997/12/23 22:01:07  tatiana
* focus and specimen_voucher
*
* Revision 6.0  1997/08/25 18:06:02  madden
* Revision changed to 6.0
*
* Revision 5.10  1997/07/29 20:59:54  vakatov
* Encapsulated 'ParFlat_GBQual_names' and 'ParFlat_GBFeat'(formerly
* global) variables into access functions. Made other global variables
* be "extern" instead of "NLM_EXTERN"(i.e. local within the PC DLL).
*
* Revision 5.9  1997/07/11 17:00:58  tatiana
* in misc_recomb GBQUAL_organism changed from mandatory ro optional
*
 * Revision 5.8  1997/01/08  21:08:56  tatiana
 * /clone added to misc_difference
 *
 * Revision 5.7  1996/09/17  14:52:17  tatiana
 * 'virion' added
 *
 * Revision 5.6  1996/08/02  16:50:45  tatiana
 * a typo fixed
 *
 * Revision 5.5  1996/08/01  17:15:47  kans
 * fixed a typo
 *
 * Revision 5.4  1996/07/30  17:28:07  kans
 * ParFlat_... arrays now external in header file
 *
 * Revision 5.3  1996/07/29  19:45:34  tatiana
 * GBQual_names changed to use a structure
 *
 * Revision 5.2  1996/07/25  19:35:34  kans
 * ParFlat_GBQual_class array was missing an item at the cultivar position
 *
 * Revision 5.1  1996/07/25  14:17:34  tatiana
 * added qualifiers: allele, exception replace
 *
 * Revision 4.5  1995/11/15  18:03:32  tatiana
 * a bug fixed.
 *
 * Revision 4.4  1995/11/13  15:53:50  tatiana
 * serotype added
 *
 * Revision 4.3  1995/11/08  22:55:24  tatiana
 * /serotype added
 *
 * Revision 4.2  1995/08/16  22:02:08  tatiana
 * changes for db_xref
 *
 * Revision 4.1  1995/08/15  22:07:16  tatiana
 * db_xref added
 *
 * ..
 *
 * Revision 1.7  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*                                                                 10-14-93
***************************************************************************/

#include <stdio.h>
#include <ncbi.h>
#include <gbftdef.h>

static GbFeatName STATIC__ParFlat_GBQual_names[ParFlat_TOTAL_GBQUAL] = { 
 {"allele", Class_text}, {"anticodon", Class_pos_aa}, 
 {"bound_moiety", Class_text}, {"cell_line", Class_text}, 
 {"cell_type", Class_text}, {"chromosome", Class_text}, 
 {"chloroplast", Class_none}, {"chromoplast", Class_none}, 
 {"citation", Class_bracket_int}, {"clone", Class_text}, 
 {"clone_lib", Class_text}, {"codon", Class_seq_aa}, 
 {"codon_start", Class_int_or}, {"cons_splice", Class_site}, 
 {"cultivar", Class_text}, {"cyanelle", Class_none},
 {"db_xref", Class_text}, {"dev_stage", Class_text},
 {"direction", Class_L_R_B}, {"EC_number", Class_ecnum}, 
 {"evidence", Class_exper}, {"exception", Class_text},
 {"frequency", Class_text}, {"function", Class_text}, 
 {"gene", Class_text}, {"gdb_xref", Class_text}, 
 {"germline", Class_none}, {"haplotype", Class_text},
 {"insertion_seq", Class_text}, {"isolate", Class_text}, 
 {"kinetoplast", Class_none}, {"label", Class_token}, 
 {"lab_host", Class_text}, {"map", Class_text}, 
 {"macronuclear", Class_none}, {"mitochondrion", Class_none}, 
 {"mod_base", Class_token}, {"note", Class_note},
 {"number", Class_number}, {"organism", Class_text}, 
 {"partial", Class_none}, {"PCR_conditions", Class_text}, 
 {"pop_variant", Class_text}, {"phenotype", Class_text},
 {"plasmid", Class_text}, {"product", Class_text}, 
 {"proviral", Class_none}, {"pseudo", Class_none},
 {"rearranged", Class_none}, { "replace", Class_text}, 
 {"rpt_family", Class_text}, {"rpt_type", Class_rpt},
 { "rpt_unit", Class_text}, { "sex", Class_text},
 {"sequenced_mol", Class_text}, { "serotype", Class_text},
 {"specific_host", Class_text}, {"standard_name", Class_text}, 
 {"strain", Class_text}, {"sub_clone", Class_text}, 
 {"sub_species", Class_text}, {"sub_strain", Class_text}, 
 {"tissue_lib", Class_text},  {"tissue_type", Class_text}, 
 {"translation", Class_text}, {"transl_except", Class_pos_aa}, 
 {"transl_table", Class_int}, {"transposon", Class_text}, 
 {"usedin", Class_token}, {"variety", Class_text}, {"virion", Class_none},
 {"focus", Class_none}, { "specimen_voucher", Class_text},
 {"protein_id", Class_text}, { "country", Class_text},
 {"organelle", Class_text}, {"transcript_id", Class_text},   
 {"transgenic", Class_none}, {"environmental_sample", Class_none},
 {"isolation_source", Class_text}, {"serovar", Class_text},
 {"locus_tag", Class_text}, {"mol_type", Class_text},
 {"segment", Class_text},{"ecotype", Class_text},
 {"estimated_length", Class_text}, {"operon", Class_text},
 {"old_locus_tag", Class_text}, {"compare", Class_text}
};

NLM_EXTERN GbFeatNamePtr x_ParFlat_GBQual_names(void) {
  return STATIC__ParFlat_GBQual_names;
}

CharPtr ParFlat_IntOrString[ParFlat_TOTAL_IntOr] = {"1", "2", "3"};

CharPtr ParFlat_LRBString[ParFlat_TOTAL_LRB] = {"LEFT", "RIGHT", "BOTH"};

CharPtr ParFlat_ExpString[ParFlat_TOTAL_Exp] = {
                    "EXPERIMENTAL", "NOT_EXPERIMENTAL"};

CharPtr ParFlat_RptString[ParFlat_TOTAL_Rpt] = {
       "tandem", "inverted", "flanking", "terminal", "direct",
       "dispersed", "other"};

static SematicFeat STATIC__ParFlat_GBFeat[ParFlat_TOTAL_GBFEAT] = {
   {"allele", 0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_frequency, 
     GBQUAL_gene,GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, 
      GBQUAL_phenotype, GBQUAL_product, GBQUAL_replace, GBQUAL_standard_name, 
      GBQUAL_usedin, GBQUAL_locus_tag, GBQUAL_old_locus_tag,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"attenuator", 0, {-1, -1, -1, -1, -1}, 14,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_label, 
     GBQUAL_gene, GBQUAL_map,
      GBQUAL_note, GBQUAL_partial, GBQUAL_phenotype, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele, GBQUAL_operon,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"C_region", 0, {-1, -1, -1, -1, -1}, 15,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, 
     GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, 
     GBQUAL_product, GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"CAAT_signal", 0, {-1, -1, -1, -1, -1}, 12,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_label, 
     GBQUAL_gene, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"CDS", 0, {-1, -1, -1, -1, -1}, 27,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_codon, GBQUAL_codon_start, GBQUAL_db_xref, 
     GBQUAL_EC_number,GBQUAL_evidence, GBQUAL_exception, GBQUAL_function, 
     GBQUAL_gdb_xref, GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, 
     GBQUAL_number, GBQUAL_partial, GBQUAL_product, GBQUAL_pseudo, 
     GBQUAL_standard_name, GBQUAL_translation, GBQUAL_transl_except, 
     GBQUAL_transl_table, GBQUAL_usedin, GBQUAL_protein_id,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_operon,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"conflict",  1, {GBQUAL_citation, -1, -1, -1, -1}, 11,
     {GBQUAL_db_xref,GBQUAL_evidence, GBQUAL_map, GBQUAL_note, GBQUAL_gene, GBQUAL_usedin,
     GBQUAL_replace, GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele, GBQUAL_compare,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"D-loop",  0, {-1, -1, -1, -1}, 12,
     {GBQUAL_evidence, GBQUAL_citation, GBQUAL_label, GBQUAL_gene,
     GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_usedin, GBQUAL_db_xref, 
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"D_segment", 0, {-1, -1, -1, -1, -1}, 15,
     {GBQUAL_citation, GBQUAL_evidence, GBQUAL_gene, 
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_product,
      GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin, GBQUAL_db_xref, 
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"enhancer",  0, {-1, -1, -1, -1, -1}, 13,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_label, 
     GBQUAL_gene, GBQUAL_map, 
      GBQUAL_note, GBQUAL_partial, GBQUAL_standard_name,  GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"exon", 0, {-1, -1, -1, -1, -1}, 18,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_db_xref, GBQUAL_EC_number,
      GBQUAL_evidence, GBQUAL_function, GBQUAL_gene, GBQUAL_label,
      GBQUAL_map, GBQUAL_note, GBQUAL_number, GBQUAL_partial,
      GBQUAL_product, GBQUAL_pseudo, GBQUAL_standard_name, 
      GBQUAL_usedin,GBQUAL_locus_tag, GBQUAL_old_locus_tag,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"gap",  1, {GBQUAL_estimated_length, -1, -1, -1, -1}, 2,
     {GBQUAL_map, GBQUAL_note,  -1,  -1, -1,  -1, -1, -1,  -1,  -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"GC_signal", 0, {-1, -1, -1, -1, -1}, 12,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_label, 
     GBQUAL_gene, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"gene", 0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, 
     GBQUAL_function, GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, 
     GBQUAL_pseudo, GBQUAL_phenotype, GBQUAL_usedin, GBQUAL_locus_tag, GBQUAL_old_locus_tag,
     GBQUAL_operon, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
	{"iDNA", 0, {-1, -1, -1, -1, -1}, 15,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function, 
     GBQUAL_label,
      GBQUAL_gene, GBQUAL_map, GBQUAL_note, GBQUAL_number, GBQUAL_partial,
      GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"intron", 0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_cons_splice, GBQUAL_db_xref,
     GBQUAL_evidence,  GBQUAL_function, 
      GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_number, 
      GBQUAL_partial, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"J_segment", 0, {-1, -1, -1, -1, -1}, 15,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene, 
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_product, 
      GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"LTR", 0, {-1, -1, -1, -1, -1}, 13,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function, 
     GBQUAL_gene, GBQUAL_label, GBQUAL_note, GBQUAL_partial, GBQUAL_standard_name,
      GBQUAL_usedin, GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"mat_peptide",  0, {-1, -1, -1, -1, -1}, 18,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_EC_number,
      GBQUAL_evidence, GBQUAL_function, GBQUAL_gene, GBQUAL_label,
      GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_pseudo, 
      GBQUAL_product, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"misc_binding",  1, {GBQUAL_bound_moiety, -1, -1, -1, -1}, 13,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function, 
      GBQUAL_gene,
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial,
      GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"misc_difference", 0, {-1, -1, -1, -1}, 17,
     {GBQUAL_phenotype, GBQUAL_citation, GBQUAL_clone, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene, 
     GBQUAL_label, GBQUAL_map, GBQUAL_partial, GBQUAL_replace, 
      GBQUAL_note, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele, GBQUAL_compare,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"misc_feature",  0, {-1, -1, -1, -1, -1}, 18,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function, 
     GBQUAL_gene,
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_number, 
      GBQUAL_phenotype, GBQUAL_product, GBQUAL_pseudo, GBQUAL_standard_name,
      GBQUAL_usedin, GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"misc_recomb", 0, {-1, -1, -1, -1, -1}, 14,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene, 
     GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_organism, 
     GBQUAL_partial, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"misc_RNA",  0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function, 
     GBQUAL_gene,
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_product, 
      GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele, GBQUAL_operon,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"misc_signal",  0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function, 
     GBQUAL_gene,
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_phenotype, 
      GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele, GBQUAL_operon,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"misc_structure",  0, {-1, -1, -1, -1, -1}, 14,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function, 
     GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial,
      GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"modified_base",  1, {GBQUAL_mod_base, -1, -1, -1, -1}, 12,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_frequency, 
     GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"mRNA",  0, {-1, -1, -1, -1, -1}, 18,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function, 
     GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, 	
     GBQUAL_product, GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_transcript_id, GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_operon,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, 
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"mutation",  0, {-1, -1, -1, -1}, 15,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_frequency,
     GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_phenotype,
     GBQUAL_product, GBQUAL_replace, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"N_region",  0, {-1, -1, -1, -1, -1}, 13,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene,
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_product, 
      GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"old_sequence",   1, {GBQUAL_citation, -1, -1, -1}, 12,
     {GBQUAL_db_xref, GBQUAL_gene, GBQUAL_evidence, GBQUAL_map, GBQUAL_note, 
     GBQUAL_partial, GBQUAL_replace, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele, GBQUAL_compare,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"operon", 1, {GBQUAL_operon, -1, -1, -1, -1}, 12,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, 
     GBQUAL_function, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, 
     GBQUAL_pseudo, GBQUAL_phenotype, GBQUAL_usedin,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"oriT", 0, {-1, -1, -1, -1, -1}, 17,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_direction, GBQUAL_evidence, 
     GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, 
     GBQUAL_rpt_family, GBQUAL_rpt_type, GBQUAL_rpt_unit, GBQUAL_standard_name,
     GBQUAL_usedin, GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"polyA_signal", 0, {-1, -1, -1, -1, -1}, 12,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene,
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"polyA_site", 0, {-1, -1, -1, -1, -1}, 11,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene,
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"precursor_RNA",  0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial,
      GBQUAL_product, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_operon,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"prim_transcript",  0, {-1, -1, -1, -1, -1}, 15,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial,
      GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_operon,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"primer_bind",  0, {-1, -1, -1, -1, -1}, 14,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene,
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_standard_name,
      GBQUAL_PCR_conditions,GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"promoter",  0, {-1, -1, -1, -1, -1}, 17,
     {GBQUAL_citation,GBQUAL_db_xref,  GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial,
      GBQUAL_phenotype, GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele, GBQUAL_operon,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"protein_bind",  1, {GBQUAL_bound_moiety, -1, -1, -1, -1}, 14,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, 
      GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"RBS",  0, {-1, -1, -1, -1, -1}, 13,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene,
      GBQUAL_label, GBQUAL_map,GBQUAL_note, GBQUAL_partial, GBQUAL_standard_name,
      GBQUAL_usedin, GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"repeat_region",  0, {-1, -1, -1, -1, -1}, 19,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene, GBQUAL_insertion_seq, GBQUAL_label, GBQUAL_map, GBQUAL_note,
      GBQUAL_partial, GBQUAL_rpt_type, GBQUAL_rpt_family, GBQUAL_rpt_unit,
      GBQUAL_standard_name,
      GBQUAL_transposon, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"repeat_unit",  0, {-1, -1, -1, -1, -1}, 15,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene, 
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_rpt_family,
      GBQUAL_rpt_type, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"rep_origin",  0, {-1, -1, -1, -1, -1}, 14,
     {GBQUAL_citation, GBQUAL_direction, GBQUAL_db_xref, GBQUAL_evidence,
      GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial,
      GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"rRNA",  0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function, 
     GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_product,
      GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"S_region",  0, {-1, -1, -1, -1, -1}, 15,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene,
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_product,
      GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"satellite",  0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene,
      GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, GBQUAL_rpt_type,
      GBQUAL_rpt_family,GBQUAL_rpt_unit, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"scRNA",  0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene,GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial,
      GBQUAL_product, GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"sig_peptide", 0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
     GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial,
     GBQUAL_product, GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"snoRNA",  0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene,GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial,
      GBQUAL_product, GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"snRNA",  0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene,GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial,
      GBQUAL_product, GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"source", 1, {GBQUAL_organism, -1, -1, -1, -1}, 53,
     {GBQUAL_cell_line, GBQUAL_cell_type, GBQUAL_chloroplast, 
      GBQUAL_chromoplast, GBQUAL_chromosome,
      GBQUAL_citation, GBQUAL_clone, GBQUAL_clone_lib, GBQUAL_country, GBQUAL_cultivar,
      GBQUAL_cyanelle, GBQUAL_db_xref, GBQUAL_dev_stage, GBQUAL_focus,
      GBQUAL_frequency, GBQUAL_germline, GBQUAL_haplotype,
      GBQUAL_lab_host, GBQUAL_insertion_seq, GBQUAL_isolate, GBQUAL_kinetoplast,
      GBQUAL_label, GBQUAL_macronuclear, GBQUAL_map, GBQUAL_mitochondrion,
      GBQUAL_note, GBQUAL_plasmid, GBQUAL_pop_variant, 
      GBQUAL_proviral, GBQUAL_rearranged, GBQUAL_sex, GBQUAL_sequenced_mol, 
      GBQUAL_serotype, GBQUAL_specific_host, GBQUAL_strain, GBQUAL_sub_clone, 
      GBQUAL_sub_species, GBQUAL_sub_strain, GBQUAL_tissue_lib, 
      GBQUAL_tissue_type, GBQUAL_transposon, GBQUAL_usedin, 
      GBQUAL_specimen_voucher, GBQUAL_variety, GBQUAL_virion,
      GBQUAL_organelle, GBQUAL_transgenic, GBQUAL_environmental_sample,
      GBQUAL_isolation_source, GBQUAL_serovar, GBQUAL_mol_type, GBQUAL_segment,
      GBQUAL_ecotype, -1, -1}},
   {"stem_loop",  0, {-1, -1, -1, -1, -1}, 15,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, 
      GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele, GBQUAL_operon,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"STS",  0, {-1, -1, -1, -1, -1}, 13,
     {GBQUAL_citation, GBQUAL_standard_name, GBQUAL_db_xref, GBQUAL_gene,
      GBQUAL_label, GBQUAL_usedin, GBQUAL_note, GBQUAL_partial, GBQUAL_map, 
      GBQUAL_evidence, GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"TATA_signal",  0, {-1, -1, -1, -1, -1}, 12,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene,
      GBQUAL_label, GBQUAL_map,
      GBQUAL_note, GBQUAL_partial, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"terminator",  0, {-1, -1, -1, -1, -1}, 14,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene,
      GBQUAL_label, GBQUAL_map,GBQUAL_note, GBQUAL_partial,
       GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele, GBQUAL_operon,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"transit_peptide",  0, {-1, -1, -1, -1, -1}, 16,
     {GBQUAL_citation, GBQUAL_db_xref,
      GBQUAL_evidence, GBQUAL_function, GBQUAL_gene, GBQUAL_label, GBQUAL_map,
       GBQUAL_note,GBQUAL_partial, GBQUAL_product, GBQUAL_pseudo, 	
       GBQUAL_standard_name, GBQUAL_usedin,
       GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
       -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"tRNA",  0, {-1, -1, -1, -1, -1}, 17,
     {GBQUAL_anticodon, GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence,
      GBQUAL_function, GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, 
      GBQUAL_partial, GBQUAL_product, GBQUAL_pseudo, GBQUAL_standard_name,
       GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"unsure",  0, {-1, -1, -1, -1}, 12,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene, GBQUAL_usedin, 
     GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_replace, 
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"V_region",  0, {-1, -1, -1, -1, -1}, 15,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence,
      GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, 
      GBQUAL_product, GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"V_segment",  0, {-1, -1, -1, -1, -1}, 15,
     {GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence,
      GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, 
      GBQUAL_product, GBQUAL_pseudo, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_allele,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"variation",  0, {-1, -1, -1, -1}, 18,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, 
      GBQUAL_frequency, GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note,
      GBQUAL_partial, GBQUAL_replace, GBQUAL_phenotype, GBQUAL_product,
      GBQUAL_standard_name, GBQUAL_usedin, GBQUAL_locus_tag, GBQUAL_old_locus_tag,
      GBQUAL_compare, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"3'clip",  0, {-1, -1, -1, -1, -1}, 14,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, 
      GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"3'UTR",  0, {-1, -1, -1, -1, -1}, 14,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene, GBQUAL_label, GBQUAL_map, GBQUAL_note, GBQUAL_partial, 
      GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"5'clip",  0, {-1, -1, -1, -1, -1}, 14,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_db_xref, GBQUAL_function, GBQUAL_gene,
      GBQUAL_evidence, GBQUAL_label, GBQUAL_map,  
      GBQUAL_note, GBQUAL_partial, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"5'UTR",  0, {-1, -1, -1, -1, -1}, 14,
     {GBQUAL_allele, GBQUAL_citation, GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_function,
      GBQUAL_gene, GBQUAL_label, GBQUAL_map, 
      GBQUAL_note, GBQUAL_partial, GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"-10_signal",  0, {-1, -1, -1, -1, -1}, 14,
     {GBQUAL_allele, GBQUAL_citation,GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene,
     GBQUAL_label, GBQUAL_map,GBQUAL_note,GBQUAL_partial, 
     GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_operon,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1}},
   {"-35_signal",  0, {-1, -1, -1, -1, -1}, 14,
     {GBQUAL_allele, GBQUAL_citation,GBQUAL_db_xref, GBQUAL_evidence, GBQUAL_gene,
     GBQUAL_label, GBQUAL_map,GBQUAL_note,GBQUAL_partial, 
     GBQUAL_standard_name, GBQUAL_usedin,
      GBQUAL_locus_tag, GBQUAL_old_locus_tag, GBQUAL_operon,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
      -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 -1, -1, -1}}
      };

NLM_EXTERN SematicFeatPtr x_ParFlat_GBFeat(void) {
  return STATIC__ParFlat_GBFeat;
}



