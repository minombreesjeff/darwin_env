#ifndef _objmmdb1_ 
#define _objmmdb1_ 

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" { /* } */
#endif


/**************************************************
*
*    Generated objects for Module MMDB
*    Generated using ASNCODE Revision: 4.2 at Aug 1, 1996  4:27 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objmmdb1AsnLoad PROTO((void));


/**************************************************
*
*    Biostruc
*
**************************************************/
typedef struct struct_Biostruc {
   struct struct_Biostruc PNTR next;
   Uint4 OBbits__;
   ValNodePtr   id;
   ValNodePtr   descr;
   struct struct_Biostruc_graph PNTR   chemical_graph;
   struct struct_Biostruc_feature_set PNTR   features;
   struct struct_Biostruc_model PNTR   model;
} Biostruc, PNTR BiostrucPtr;


NLM_EXTERN BiostrucPtr LIBCALL BiostrucFree PROTO ((BiostrucPtr ));
BiostrucPtr LIBCALL BiostrucNew PROTO (( void ));
NLM_EXTERN BiostrucPtr LIBCALL BiostrucAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BiostrucAsnWrite PROTO (( BiostrucPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr BiostrucIdPtr;
typedef ValNode BiostrucId;
#define BiostrucId_mmdb_id 1
#define BiostrucId_other_database 2
#define BiostrucId_local_id 3


BiostrucIdPtr LIBCALL BiostrucIdFree PROTO ((BiostrucIdPtr ));
BiostrucIdPtr LIBCALL BiostrucIdAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL BiostrucIdAsnWrite PROTO (( BiostrucIdPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucSet
*
**************************************************/
typedef struct struct_Biostruc_set {
   Uint4 OBbits__;
   ValNodePtr   id;
   ValNodePtr   descr;
   struct struct_Biostruc PNTR   biostrucs;
} BiostrucSet, PNTR BiostrucSetPtr;


BiostrucSetPtr LIBCALL BiostrucSetFree PROTO ((BiostrucSetPtr ));
BiostrucSetPtr LIBCALL BiostrucSetNew PROTO (( void ));
BiostrucSetPtr LIBCALL BiostrucSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL BiostrucSetAsnWrite PROTO (( BiostrucSetPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucAnnotSet
*
**************************************************/
typedef struct struct_Biostruc_annot_set {
   Uint4 OBbits__;
   ValNodePtr   id;
   ValNodePtr   descr;
   struct struct_Biostruc_feature_set PNTR   features;
} BiostrucAnnotSet, PNTR BiostrucAnnotSetPtr;


NLM_EXTERN BiostrucAnnotSetPtr LIBCALL BiostrucAnnotSetFree PROTO ((BiostrucAnnotSetPtr ));
NLM_EXTERN BiostrucAnnotSetPtr LIBCALL BiostrucAnnotSetNew PROTO (( void ));
NLM_EXTERN BiostrucAnnotSetPtr LIBCALL BiostrucAnnotSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BiostrucAnnotSetAsnWrite PROTO (( BiostrucAnnotSetPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucResidueGraphSet
*
**************************************************/
typedef struct struct_Biostruc_residue_graph_set {
   Uint4 OBbits__;
   ValNodePtr   id;
   ValNodePtr   descr;
   struct struct_Residue_graph PNTR   residue_graphs;
} BiostrucResidueGraphSet, PNTR BiostrucResidueGraphSetPtr;


NLM_EXTERN BiostrucResidueGraphSetPtr LIBCALL BiostrucResidueGraphSetFree PROTO ((BiostrucResidueGraphSetPtr ));
BiostrucResidueGraphSetPtr LIBCALL BiostrucResidueGraphSetNew PROTO (( void ));
BiostrucResidueGraphSetPtr LIBCALL BiostrucResidueGraphSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL BiostrucResidueGraphSetAsnWrite PROTO (( BiostrucResidueGraphSetPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr BiostrucDescrPtr;
typedef ValNode BiostrucDescr;
#define BiostrucDescr_name 1
#define BiostrucDescr_pdb_comment 2
#define BiostrucDescr_other_comment 3
#define BiostrucDescr_history 4
#define BiostrucDescr_attribution 5


BiostrucDescrPtr LIBCALL BiostrucDescrFree PROTO ((BiostrucDescrPtr ));
BiostrucDescrPtr LIBCALL BiostrucDescrAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL BiostrucDescrAsnWrite PROTO (( BiostrucDescrPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucHistory
*
**************************************************/
typedef struct struct_Biostruc_history {
   Uint4 OBbits__;
   struct struct_Biostruc_replace PNTR   replaces;
   struct struct_Biostruc_replace PNTR   replaced_by;
   struct struct_Biostruc_source PNTR   data_source;
} BiostrucHistory, PNTR BiostrucHistoryPtr;


BiostrucHistoryPtr LIBCALL BiostrucHistoryFree PROTO ((BiostrucHistoryPtr ));
BiostrucHistoryPtr LIBCALL BiostrucHistoryNew PROTO (( void ));
BiostrucHistoryPtr LIBCALL BiostrucHistoryAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL BiostrucHistoryAsnWrite PROTO (( BiostrucHistoryPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucReplace
*
**************************************************/
typedef struct struct_Biostruc_replace {
   Uint4 OBbits__;
   ValNodePtr   id;
   DatePtr   date;
} BiostrucReplace, PNTR BiostrucReplacePtr;


BiostrucReplacePtr LIBCALL BiostrucReplaceFree PROTO ((BiostrucReplacePtr ));
BiostrucReplacePtr LIBCALL BiostrucReplaceNew PROTO (( void ));
BiostrucReplacePtr LIBCALL BiostrucReplaceAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL BiostrucReplaceAsnWrite PROTO (( BiostrucReplacePtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucSource
*
**************************************************/
typedef struct struct_Biostruc_source {
   Uint4 OBbits__;
   CharPtr   name_of_database;
   ValNodePtr   VersionOfDatabase_version_of_database;
   ValNodePtr   database_entry_id;
   DatePtr   database_entry_date;
   ValNodePtr   database_entry_history;
} BiostrucSource, PNTR BiostrucSourcePtr;


BiostrucSourcePtr LIBCALL BiostrucSourceFree PROTO ((BiostrucSourcePtr ));
BiostrucSourcePtr LIBCALL BiostrucSourceNew PROTO (( void ));
BiostrucSourcePtr LIBCALL BiostrucSourceAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL BiostrucSourceAsnWrite PROTO (( BiostrucSourcePtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr VersionOfDatabase_version_of_databasePtr;
typedef ValNode VersionOfDatabase_version_of_database;

#endif /* NLM_GENERATED_CODE_PROTO */

#define VersionOfDatabase_version_of_database_release_date 1
#define VersionOfDatabase_version_of_database_release_code 2

#ifdef NLM_GENERATED_CODE_PROTO

static VersionOfDatabase_version_of_databasePtr LIBCALL VersionOfDatabase_version_of_databaseFree PROTO ((VersionOfDatabase_version_of_databasePtr ));
static VersionOfDatabase_version_of_databasePtr LIBCALL VersionOfDatabase_version_of_databaseAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL VersionOfDatabase_version_of_databaseAsnWrite PROTO (( VersionOfDatabase_version_of_databasePtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    Generated objects for Module MMDB-Chemical-graph
*    Generated using ASNCODE Revision: 4.2 at Aug 1, 1996  4:27 PM
*
**************************************************/


/**************************************************
*
*    BiostrucGraph
*
**************************************************/
typedef struct struct_Biostruc_graph {
   Uint4 OBbits__;
   ValNodePtr   descr;
   struct struct_Molecule_graph PNTR   molecule_graphs;
   struct struct_Inter_residue_bond PNTR   inter_molecule_bonds;
   struct struct_Residue_graph PNTR   residue_graphs;
} BiostrucGraph, PNTR BiostrucGraphPtr;


BiostrucGraphPtr LIBCALL BiostrucGraphFree PROTO ((BiostrucGraphPtr ));
BiostrucGraphPtr LIBCALL BiostrucGraphNew PROTO (( void ));
BiostrucGraphPtr LIBCALL BiostrucGraphAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL BiostrucGraphAsnWrite PROTO (( BiostrucGraphPtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr BiomolDescrPtr;
typedef ValNode BiomolDescr;
#define BiomolDescr_name 1
#define BiomolDescr_pdb_class 2
#define BiomolDescr_pdb_source 3
#define BiomolDescr_pdb_comment 4
#define BiomolDescr_other_comment 5
#define BiomolDescr_organism 6
#define BiomolDescr_attribution 7
#define BiomolDescr_assembly_type 8
#define BiomolDescr_molecule_type 9


BiomolDescrPtr LIBCALL BiomolDescrFree PROTO ((BiomolDescrPtr ));
BiomolDescrPtr LIBCALL BiomolDescrAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL BiomolDescrAsnWrite PROTO (( BiomolDescrPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ResidueGraph
*
**************************************************/
typedef struct struct_Residue_graph {
   struct struct_Residue_graph PNTR next;
   Uint4 OBbits__;
   Int4   id;
   ValNodePtr   descr;
#define OB__Residue_graph_residue_type 0

   Int4   residue_type;
   ValNodePtr   iupac_code;
   struct struct_Atom PNTR   atoms;
   struct struct_Intra_residue_bond PNTR   bonds;
   struct struct_Chiral_center PNTR   chiral_centers;
} ResidueGraph, PNTR ResidueGraphPtr;


ResidueGraphPtr LIBCALL ResidueGraphFree PROTO ((ResidueGraphPtr ));
ResidueGraphPtr LIBCALL ResidueGraphNew PROTO (( void ));
ResidueGraphPtr LIBCALL ResidueGraphAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL ResidueGraphAsnWrite PROTO (( ResidueGraphPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    MoleculeGraph
*
**************************************************/
typedef struct struct_Molecule_graph {
   struct struct_Molecule_graph PNTR next;
   Uint4 OBbits__;
   Int4   id;
   ValNodePtr   descr;
   ValNodePtr   seq_id;
   struct struct_Residue PNTR   residue_sequence;
   struct struct_Inter_residue_bond PNTR   inter_residue_bonds;
} MoleculeGraph, PNTR MoleculeGraphPtr;


MoleculeGraphPtr LIBCALL MoleculeGraphFree PROTO ((MoleculeGraphPtr ));
MoleculeGraphPtr LIBCALL MoleculeGraphNew PROTO (( void ));
MoleculeGraphPtr LIBCALL MoleculeGraphAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL MoleculeGraphAsnWrite PROTO (( MoleculeGraphPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    InterResidueBond
*
**************************************************/
typedef struct struct_Inter_residue_bond {
   struct struct_Inter_residue_bond PNTR next;
   Uint4 OBbits__;
   struct struct_Atom_pntr PNTR   atom_id_1;
   struct struct_Atom_pntr PNTR   atom_id_2;
#define OB__Inter_residue_bond_bond_order 0

   Int4   bond_order;
} InterResidueBond, PNTR InterResidueBondPtr;


InterResidueBondPtr LIBCALL InterResidueBondFree PROTO ((InterResidueBondPtr ));
InterResidueBondPtr LIBCALL InterResidueBondNew PROTO (( void ));
InterResidueBondPtr LIBCALL InterResidueBondAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL InterResidueBondAsnWrite PROTO (( InterResidueBondPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Residue
*
**************************************************/
typedef struct struct_Residue {
   struct struct_Residue PNTR next;
   Uint4 OBbits__;
   Int4   id;
   CharPtr   name;
   ValNodePtr   residue_graph;
} Residue, PNTR ResiduePtr;


ResiduePtr LIBCALL ResidueFree PROTO ((ResiduePtr ));
ResiduePtr LIBCALL ResidueNew PROTO (( void ));
ResiduePtr LIBCALL ResidueAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL ResidueAsnWrite PROTO (( ResiduePtr , AsnIoPtr, AsnTypePtr));

typedef ValNodePtr ResidueGraphPntrPtr;
typedef ValNode ResidueGraphPntr;
#define ResidueGraphPntr_local 1
#define ResidueGraphPntr_biostruc 2
#define ResidueGraphPntr_standard 3


ResidueGraphPntrPtr LIBCALL ResidueGraphPntrFree PROTO ((ResidueGraphPntrPtr ));
ResidueGraphPntrPtr LIBCALL ResidueGraphPntrAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL ResidueGraphPntrAsnWrite PROTO (( ResidueGraphPntrPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucGraphPntr
*
**************************************************/
typedef struct struct_Biostruc_graph_pntr {
   Uint4 OBbits__;
   ValNodePtr   biostruc_id;
   Int4   residue_graph_id;
} BiostrucGraphPntr, PNTR BiostrucGraphPntrPtr;


BiostrucGraphPntrPtr LIBCALL BiostrucGraphPntrFree PROTO ((BiostrucGraphPntrPtr ));
BiostrucGraphPntrPtr LIBCALL BiostrucGraphPntrNew PROTO (( void ));
BiostrucGraphPntrPtr LIBCALL BiostrucGraphPntrAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL BiostrucGraphPntrAsnWrite PROTO (( BiostrucGraphPntrPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucResidueGraphSetPntr
*
**************************************************/
typedef struct struct_Biostruc_residue_graph_set_pntr {
   Uint4 OBbits__;
   ValNodePtr   biostruc_residue_graph_set_id;
   Int4   residue_graph_id;
} BiostrucResidueGraphSetPntr, PNTR BiostrucResidueGraphSetPntrPtr;


BiostrucResidueGraphSetPntrPtr LIBCALL BiostrucResidueGraphSetPntrFree PROTO ((BiostrucResidueGraphSetPntrPtr ));
BiostrucResidueGraphSetPntrPtr LIBCALL BiostrucResidueGraphSetPntrNew PROTO (( void ));
BiostrucResidueGraphSetPntrPtr LIBCALL BiostrucResidueGraphSetPntrAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL BiostrucResidueGraphSetPntrAsnWrite PROTO (( BiostrucResidueGraphSetPntrPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    Atom
*
**************************************************/
typedef struct struct_Atom {
   struct struct_Atom PNTR next;
   Uint4 OBbits__;
   Int4   id;
   CharPtr   name;
   ValNodePtr   iupac_code;
   Uint2   element;
   /* following #defines are for enumerated type, not used by object loaders */
#define Atom_element_h 1
#define Atom_element_he 2
#define Atom_element_li 3
#define Atom_element_be 4
#define Atom_element_b 5
#define Atom_element_c 6
#define Atom_element_n 7
#define Atom_element_o 8
#define Atom_element_f 9
#define Atom_element_ne 10
#define Atom_element_na 11
#define Atom_element_mg 12
#define Atom_element_al 13
#define Atom_element_si 14
#define Atom_element_p 15
#define Atom_element_s 16
#define Atom_element_cl 17
#define Atom_element_ar 18
#define Atom_element_k 19
#define Atom_element_ca 20
#define Atom_element_sc 21
#define Atom_element_ti 22
#define Atom_element_v 23
#define Atom_element_cr 24
#define Atom_element_mn 25
#define Atom_element_fe 26
#define Atom_element_co 27
#define Atom_element_ni 28
#define Atom_element_cu 29
#define Atom_element_zn 30
#define Atom_element_ga 31
#define Atom_element_ge 32
#define Atom_element_as 33
#define Atom_element_se 34
#define Atom_element_br 35
#define Atom_element_kr 36
#define Atom_element_rb 37
#define Atom_element_sr 38
#define Atom_element_y 39
#define Atom_element_zr 40
#define Atom_element_nb 41
#define Atom_element_mo 42
#define Atom_element_tc 43
#define Atom_element_ru 44
#define Atom_element_rh 45
#define Atom_element_pd 46
#define Atom_element_ag 47
#define Atom_element_cd 48
#define Atom_element_in 49
#define Atom_element_sn 50
#define Atom_element_sb 51
#define Atom_element_te 52
#define Atom_element_i 53
#define Atom_element_xe 54
#define Atom_element_cs 55
#define Atom_element_ba 56
#define Atom_element_la 57
#define Atom_element_ce 58
#define Atom_element_pr 59
#define Atom_element_nd 60
#define Atom_element_pm 61
#define Atom_element_sm 62
#define Atom_element_eu 63
#define Atom_element_gd 64
#define Atom_element_tb 65
#define Atom_element_dy 66
#define Atom_element_ho 67
#define Atom_element_er 68
#define Atom_element_tm 69
#define Atom_element_yb 70
#define Atom_element_lu 71
#define Atom_element_hf 72
#define Atom_element_ta 73
#define Atom_element_w 74
#define Atom_element_re 75
#define Atom_element_os 76
#define Atom_element_ir 77
#define Atom_element_pt 78
#define Atom_element_au 79
#define Atom_element_hg 80
#define Atom_element_tl 81
#define Atom_element_pb 82
#define Atom_element_bi 83
#define Atom_element_po 84
#define Atom_element_at 85
#define Atom_element_rn 86
#define Atom_element_fr 87
#define Atom_element_ra 88
#define Atom_element_ac 89
#define Atom_element_th 90
#define Atom_element_pa 91
#define Atom_element_u 92
#define Atom_element_np 93
#define Atom_element_pu 94
#define Atom_element_am 95
#define Atom_element_cm 96
#define Atom_element_bk 97
#define Atom_element_cf 98
#define Atom_element_es 99
#define Atom_element_fm 100
#define Atom_element_md 101
#define Atom_element_no 102
#define Atom_element_lr 103
#define Atom_element_other 254
#define Atom_element_unknown 255

#define OB__Atom_ionizable_proton 0

   Uint2   ionizable_proton;
   /* following #defines are for enumerated type, not used by object loaders */
#define Atom_ionizable_proton_true 1
#define Atom_ionizable_proton_false 2
#define Atom_ionizable_proton_unknown 255

} Atom, PNTR AtomPtr;


AtomPtr LIBCALL AtomFree PROTO ((AtomPtr ));
AtomPtr LIBCALL AtomNew PROTO (( void ));
AtomPtr LIBCALL AtomAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL AtomAsnWrite PROTO (( AtomPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    IntraResidueBond
*
**************************************************/
typedef struct struct_Intra_residue_bond {
   struct struct_Intra_residue_bond PNTR next;
   Uint4 OBbits__;
   Int4   atom_id_1;
   Int4   atom_id_2;
#define OB__Intra_residue_bond_bond_order 0

   Int4   bond_order;
} IntraResidueBond, PNTR IntraResidueBondPtr;


IntraResidueBondPtr LIBCALL IntraResidueBondFree PROTO ((IntraResidueBondPtr ));
IntraResidueBondPtr LIBCALL IntraResidueBondNew PROTO (( void ));
IntraResidueBondPtr LIBCALL IntraResidueBondAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL IntraResidueBondAsnWrite PROTO (( IntraResidueBondPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    ChiralCenter
*
**************************************************/
typedef struct struct_Chiral_center {
   struct struct_Chiral_center PNTR next;
   Uint4 OBbits__;
   Int4   c;
   Int4   n1;
   Int4   n2;
   Int4   n3;
   Uint2   sign;
   /* following #defines are for enumerated type, not used by object loaders */
#define Chiral_center_sign_positive 1
#define Chiral_center_sign_negative 2

} ChiralCenter, PNTR ChiralCenterPtr;


ChiralCenterPtr LIBCALL ChiralCenterFree PROTO ((ChiralCenterPtr ));
ChiralCenterPtr LIBCALL ChiralCenterNew PROTO (( void ));
ChiralCenterPtr LIBCALL ChiralCenterAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL ChiralCenterAsnWrite PROTO (( ChiralCenterPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AtomPntr
*
**************************************************/
typedef struct struct_Atom_pntr {
   struct struct_Atom_pntr PNTR next;
   Uint4 OBbits__;
   Int4   molecule_id;
   Int4   residue_id;
   Int4   atom_id;
} AtomPntr, PNTR AtomPntrPtr;


AtomPntrPtr LIBCALL AtomPntrFree PROTO ((AtomPntrPtr ));
AtomPntrPtr LIBCALL AtomPntrNew PROTO (( void ));
AtomPntrPtr LIBCALL AtomPntrAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL AtomPntrAsnWrite PROTO (( AtomPntrPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    AtomPntrSet
*
**************************************************/
typedef struct struct_AtomPntr AtomPntrSet;
typedef struct struct_AtomPntr PNTR AtomPntrSetPtr;
#define AtomPntrSetNew() AtomPntrNew() 

#ifdef NLM_GENERATED_CODE_PROTO

AtomPntrSetPtr LIBCALL AtomPntrSetFree PROTO ((AtomPntrSetPtr ));
AtomPntrSetPtr LIBCALL AtomPntrSetNew PROTO (( void ));
AtomPntrSetPtr LIBCALL AtomPntrSetAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
Boolean LIBCALL AtomPntrSetAsnWrite PROTO (( AtomPntrSetPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */

#ifdef __cplusplus
/* { */ }
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /* _objmmdb1_ */
