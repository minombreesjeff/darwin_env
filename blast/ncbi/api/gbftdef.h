/*****************************************************************************
*   gbfeatdfn.h:
*   -- GenBank Feature table define file
*
* $Log: gbftdef.h,v $
* Revision 6.18  2004/08/17 15:50:52  kans
* added GBQUAL_old_locus_tag and GBQUAL_compare
*
* Revision 6.17  2003/10/07 13:50:36  kans
* added gap, operon, oriT features and ecotype, estimated_length and operon qualifiers
*
* Revision 6.16  2003/08/19 15:18:37  kans
* added GBQUAL_segment, increased ParFlat_TOTAL_GBQUAL and opt_qual array size
*
* Revision 6.15  2003/05/07 22:03:31  kans
* added GBQUAL_mol_type, raised opt_qual array to 51 elements
*
* Revision 6.14  2003/02/22 21:20:05  kans
* added GBQUAL_locus_tag, legal for now in gene features
*
* Revision 6.13  2002/04/17 14:41:08  kans
* added GBQUAL_serovar to source feature
*
* Revision 6.12  2002/03/26 16:06:30  kans
* added transgenic, environmental_sample, and isolation_source
*
* Revision 6.11  2002/02/13 18:45:52  kans
* increased ParFlat_TOTAL_GBFEAT, added snoRNA
*
* Revision 6.10  2001/04/10 22:17:03  tatiana
* GBQUAL_endogenous_virus backed off to /note
*
* Revision 6.9  2001/04/10 21:59:17  tatiana
* GBQUAL_endogenous_virus added
*
* Revision 6.8  2000/11/29 20:34:29  tatiana
* virion key removed, GBQUAL_transcript_id added
*
* Revision 6.7  2000/02/02 21:02:41  tatiana
* new type for /number added
*
* Revision 6.6  2000/01/21 21:06:44  kans
* reverted qualifiers, just added organelle, so parser can deal with old and new form during transition
*
* Revision 6.5  2000/01/21 20:48:19  kans
* changes to merge several source qualifiers under new organelle qualifier
*
* Revision 6.4  1999/02/05 15:40:39  tatiana
* GBQUAL_country added
*
* Revision 6.3  1999/01/29 16:18:58  tatiana
* protein_id qualifier added to CDS
*
* Revision 6.2  1998/04/30 21:43:11  tatiana
* *** empty log message ***
*
* Revision 6.1  1997/12/23 23:39:00  kans
* raised ParFlat_TOTAL_GBQUAL, added GBQUAL_focus and GBQUAL_specimen_voucher
*
* Revision 6.0  1997/08/25 18:05:59  madden
* Revision changed to 6.0
*
* Revision 5.7  1997/07/29 20:59:52  vakatov
* Encapsulated 'ParFlat_GBQual_names' and 'ParFlat_GBFeat'(formerly
* global) variables into access functions. Made other global variables
* be "extern" instead of "NLM_EXTERN"(i.e. local within the PC DLL).
*
* Revision 5.6  1997/06/19 18:37:59  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.5  1996/09/17 14:50:43  tatiana
* 'virion' added, number of quals increased to 50
*
 * Revision 5.4  1996/08/02  16:50:30  tatiana
 * a typo fixed
 *
 * Revision 5.3  1996/07/30  17:28:07  kans
 * ParFlat_... arrays now external in header file
 *
 * Revision 5.2  1996/07/29  19:44:57  tatiana
 * GBQual_names changed to use a structure
 *
 * Revision 5.1  1996/07/25  14:18:15  tatiana
 * added qualifiers: allele, exception, replace
 *
 * Revision 4.4  1995/11/13  15:53:11  tatiana
 * serotype added
 *
 * Revision 4.3  1995/11/08  22:58:20  tatiana
 * serotype added
 *
 * Revision 4.2  1995/08/19  03:08:06  tatiana
 * *** empty log message ***
 *
 * Revision 4.1  1995/08/15  22:06:29  tatiana
 * db_xref added
 *
 * Revision 1.2  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*                                                                  10-14-93
******************************************************************************/
#ifndef _GBFEATDFN_
#define _GBFEATDFN_


#define GBQUAL_allele             0
#define GBQUAL_anticodon          1
#define GBQUAL_bound_moiety       2
#define GBQUAL_cell_line          3
#define GBQUAL_cell_type          4
#define GBQUAL_chromosome         5
#define GBQUAL_chloroplast        6
#define GBQUAL_chromoplast        7
#define GBQUAL_citation           8
#define GBQUAL_clone              9
#define GBQUAL_clone_lib         10
#define GBQUAL_codon             11
#define GBQUAL_codon_start       12
#define GBQUAL_cons_splice       13
#define GBQUAL_cultivar          14
#define GBQUAL_cyanelle          15
#define GBQUAL_db_xref           16
#define GBQUAL_dev_stage         17
#define GBQUAL_direction         18
#define GBQUAL_EC_number         19
#define GBQUAL_evidence          20
#define GBQUAL_exception         21
#define GBQUAL_frequency         22
#define GBQUAL_function          23
#define GBQUAL_gene              24
#define GBQUAL_gdb_xref          25
#define GBQUAL_germline          26
#define GBQUAL_haplotype         27
#define GBQUAL_insertion_seq     28
#define GBQUAL_isolate           29
#define GBQUAL_kinetoplast       30
#define GBQUAL_label             31
#define GBQUAL_lab_host          32
#define GBQUAL_map               33
#define GBQUAL_macronuclear      34
#define GBQUAL_mitochondrion     35
#define GBQUAL_mod_base          36
#define GBQUAL_note              37
#define GBQUAL_number            38
#define GBQUAL_organism          39
#define GBQUAL_partial           40
#define GBQUAL_PCR_conditions    41
#define GBQUAL_pop_variant       42
#define GBQUAL_phenotype         43
#define GBQUAL_plasmid           44
#define GBQUAL_product           45
#define GBQUAL_proviral          46
#define GBQUAL_pseudo            47          
#define GBQUAL_rearranged        48
#define GBQUAL_replace           49
#define GBQUAL_rpt_family        50
#define GBQUAL_rpt_type          51
#define GBQUAL_rpt_unit          52
#define GBQUAL_sex               53
#define GBQUAL_sequenced_mol     54
#define GBQUAL_serotype		     55
#define GBQUAL_specific_host     56
#define GBQUAL_standard_name     57
#define GBQUAL_strain            58
#define GBQUAL_sub_clone         59
#define GBQUAL_sub_species       60
#define GBQUAL_sub_strain        61
#define GBQUAL_tissue_lib        62
#define GBQUAL_tissue_type       63
#define GBQUAL_translation       64
#define GBQUAL_transl_except     65
#define GBQUAL_transl_table      66
#define GBQUAL_transposon        67
#define GBQUAL_usedin            68
#define GBQUAL_variety           69
#define GBQUAL_virion            70
#define GBQUAL_focus             71
#define GBQUAL_specimen_voucher  72
#define GBQUAL_protein_id        73
#define GBQUAL_country           74
#define GBQUAL_organelle         75
#define GBQUAL_transcript_id     76
#define GBQUAL_transgenic        77
#define GBQUAL_environmental_sample 78
#define GBQUAL_isolation_source  79
#define GBQUAL_serovar           80
#define GBQUAL_locus_tag         81
#define GBQUAL_mol_type          82
#define GBQUAL_segment           83
#define GBQUAL_ecotype           84
#define GBQUAL_estimated_length  85
#define GBQUAL_operon            86
#define GBQUAL_old_locus_tag     87
#define GBQUAL_compare           88

#define ParFlat_TOTAL_GBQUAL     89
#define ParFlat_TOTAL_IntOr       3
#define ParFlat_TOTAL_LRB         3
#define ParFlat_TOTAL_Exp         2
#define ParFlat_TOTAL_Rpt         7
#define ParFlat_TOTAL_GBFEAT     67

#define  Class_pos_aa             1
#define  Class_text               2
#define  Class_bracket_int        3
#define  Class_seq_aa             4
#define  Class_int_or             5
#define  Class_site               6
#define  Class_L_R_B              7
#define  Class_ecnum              8
#define  Class_exper              9
#define  Class_none              10
#define  Class_token             11
#define  Class_int               12
#define  Class_rpt               13
#define  Class_flabel_base       14
#define  Class_flabel_dbname     15
#define  Class_note              16
#define  Class_number            17


#define  ParFlat_Stoken_type            1
#define  ParFlat_BracketInt_type        2
#define  ParFlat_Integer_type           3      
#define  ParFlat_Number_type            4      

/*********************************************************************/

typedef struct sematic_gbfeature {
    CharPtr  key;
    Int2     mand_num;
    Int2     mand_qual[5];
    Int2     opt_num;
    Int2     opt_qual[55];
} SematicFeat, PNTR SematicFeatPtr;

typedef struct gbfeat_name {
    CharPtr  name;
    Uint1     gbclass;
} GbFeatName, PNTR GbFeatNamePtr;

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

NLM_EXTERN GbFeatNamePtr x_ParFlat_GBQual_names PROTO((void));
#define ParFlat_GBQual_names x_ParFlat_GBQual_names()

NLM_EXTERN SematicFeatPtr x_ParFlat_GBFeat PROTO((void));
#define ParFlat_GBFeat x_ParFlat_GBFeat()

extern CharPtr     ParFlat_IntOrString [ParFlat_TOTAL_IntOr];
extern CharPtr     ParFlat_LRBString   [ParFlat_TOTAL_LRB];
extern CharPtr     ParFlat_ExpString   [ParFlat_TOTAL_Exp];
extern CharPtr     ParFlat_RptString   [ParFlat_TOTAL_Rpt];

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
