/*  objmime.h
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name:  objmime.h
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: objmime.h,v $
* Revision 6.14  2002/03/14 01:29:11  thiessen
* hand changes from struct_Seq_annot to seqannot
*
* Revision 6.13  2002/02/27 15:53:37  thiessen
* fix typo
*
* Revision 6.12  2002/02/26 13:21:21  thiessen
* add structure type to new general mime
*
* Revision 6.11  2001/11/21 16:38:23  thiessen
* move cn3d stuff into bundle
*
* Revision 6.10  2001/11/21 14:25:41  thiessen
* remove BisotrucOrId
*
* Revision 6.9  2001/11/20 15:59:34  thiessen
* add imports to BundleSeqsAligns
*
* Revision 6.8  2001/11/16 14:54:26  thiessen
* add new general type
*
* Revision 6.7  2001/06/21 14:44:29  thiessen
* add new user annotations
*
* Revision 6.6  2001/06/14 14:21:33  thiessen
* add style dictionary to mime blobs
*
* Revision 6.5  1998/12/07 17:41:05  kans
* restored hand changes from struct_Seq_annot to seqannot - needed because of limitation of asncode
*
* Revision 6.4  1998/12/07 16:29:29  ywang
* add object loaded for mime type Biostruc-seqs
*
* ==========================================================================
*/
#ifndef _objmime_
#define _objmime_

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
*    Generated objects for Module NCBI-Mime
*    Generated using ASNCODE Revision: 6.5 at Dec 4, 1998  2:11 PM
*
**************************************************/

NLM_EXTERN Boolean LIBCALL
objmimeAsnLoad PROTO((void));
typedef ValNodePtr NcbiMimeAsn1Ptr;
typedef ValNode NcbiMimeAsn1;
#define NcbiMimeAsn1_entrez 1
#define NcbiMimeAsn1_alignstruc 2
#define NcbiMimeAsn1_alignseq 3     /* yanli added */
#define NcbiMimeAsn1_strucseq 4     /* yanli added */
#define NcbiMimeAsn1_strucseqs 5    /* yanli added */
#define NcbiMimeAsn1_general 6      /* paul */


NLM_EXTERN NcbiMimeAsn1Ptr LIBCALL NcbiMimeAsn1Free PROTO ((NcbiMimeAsn1Ptr ));
NLM_EXTERN NcbiMimeAsn1Ptr LIBCALL NcbiMimeAsn1AsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL NcbiMimeAsn1AsnWrite PROTO (( NcbiMimeAsn1Ptr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    EntrezGeneral
*
**************************************************/
typedef struct struct_Entrez_general {
   CharPtr   title;
   ValNodePtr   Data_data;
   Uint2   style;
   CharPtr   location;
} EntrezGeneral, PNTR EntrezGeneralPtr;


NLM_EXTERN EntrezGeneralPtr LIBCALL EntrezGeneralFree PROTO ((EntrezGeneralPtr ));
NLM_EXTERN EntrezGeneralPtr LIBCALL EntrezGeneralNew PROTO (( void ));
NLM_EXTERN EntrezGeneralPtr LIBCALL EntrezGeneralAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL EntrezGeneralAsnWrite PROTO (( EntrezGeneralPtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr Data_dataPtr;
typedef ValNode Data_data;

#endif /* NLM_GENERATED_CODE_PROTO */

#define Data_data_ml 1
#define Data_data_prot 2
#define Data_data_nuc 3
#define Data_data_genome 4
#define Data_data_structure 5
#define Data_data_strucAnnot 6

#ifdef NLM_GENERATED_CODE_PROTO

static Data_dataPtr LIBCALL Data_dataFree PROTO ((Data_dataPtr ));
static Data_dataPtr LIBCALL Data_dataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL Data_dataAsnWrite PROTO (( Data_dataPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    BiostrucAlign
*
**************************************************/
typedef struct struct_Biostruc_align {
   struct struct_Biostruc PNTR   master;
   struct struct_Biostruc PNTR   slaves;
   struct struct_Biostruc_annot_set PNTR   alignments;
   ValNodePtr   sequences;
   struct seqannot PNTR   seqalign;   /* hand change -- lyg */
   struct struct_Cn3d_style_dictionary PNTR   style_dictionary; /* paul */
   struct struct_Cn3d_user_annotations PNTR   user_annotations;
} BiostrucAlign, PNTR BiostrucAlignPtr;


NLM_EXTERN BiostrucAlignPtr LIBCALL BiostrucAlignFree PROTO ((BiostrucAlignPtr ));
NLM_EXTERN BiostrucAlignPtr LIBCALL BiostrucAlignNew PROTO (( void ));
NLM_EXTERN BiostrucAlignPtr LIBCALL BiostrucAlignAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BiostrucAlignAsnWrite PROTO (( BiostrucAlignPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucAlignSeq
*
**************************************************/
typedef struct struct_Biostruc_align_seq {
   ValNodePtr   sequences;
  struct seqannot PNTR   seqalign;   /* hand change struct -- lyg */
   struct struct_Cn3d_style_dictionary PNTR   style_dictionary; /* paul */
   struct struct_Cn3d_user_annotations PNTR   user_annotations;
} BiostrucAlignSeq, PNTR BiostrucAlignSeqPtr;


NLM_EXTERN BiostrucAlignSeqPtr LIBCALL BiostrucAlignSeqFree PROTO ((BiostrucAlignSeqPtr ));
NLM_EXTERN BiostrucAlignSeqPtr LIBCALL BiostrucAlignSeqNew PROTO (( void ));
NLM_EXTERN BiostrucAlignSeqPtr LIBCALL BiostrucAlignSeqAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BiostrucAlignSeqAsnWrite PROTO (( BiostrucAlignSeqPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucSeq
*
**************************************************/
typedef struct struct_Biostruc_seq {
   struct struct_Biostruc PNTR   structure;
   ValNodePtr   sequences;
   struct struct_Cn3d_style_dictionary PNTR   style_dictionary; /* paul */
   struct struct_Cn3d_user_annotations PNTR   user_annotations;
} BiostrucSeq, PNTR BiostrucSeqPtr;


NLM_EXTERN BiostrucSeqPtr LIBCALL BiostrucSeqFree PROTO ((BiostrucSeqPtr ));
NLM_EXTERN BiostrucSeqPtr LIBCALL BiostrucSeqNew PROTO (( void ));
NLM_EXTERN BiostrucSeqPtr LIBCALL BiostrucSeqAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BiostrucSeqAsnWrite PROTO (( BiostrucSeqPtr , AsnIoPtr, AsnTypePtr));



/**************************************************
*
*    BiostrucSeqs
*
**************************************************/
typedef struct struct_Biostruc_seqs {
   struct struct_Biostruc PNTR   structure;
   ValNodePtr   sequences;
   struct seqannot PNTR   seqalign;   /* hand change struct -- lyg */
   struct struct_Cn3d_style_dictionary PNTR   style_dictionary; /* paul */
   struct struct_Cn3d_user_annotations PNTR   user_annotations;
} BiostrucSeqs, PNTR BiostrucSeqsPtr;


NLM_EXTERN BiostrucSeqsPtr LIBCALL BiostrucSeqsFree PROTO ((BiostrucSeqsPtr ));
NLM_EXTERN BiostrucSeqsPtr LIBCALL BiostrucSeqsNew PROTO (( void ));
NLM_EXTERN BiostrucSeqsPtr LIBCALL BiostrucSeqsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BiostrucSeqsAsnWrite PROTO (( BiostrucSeqsPtr , AsnIoPtr, AsnTypePtr));

/* following #defines are for enumerated type, not used by object loaders */
#define Entrez_style_docsum 1
#define Entrez_style_genbank 2
#define Entrez_style_genpept 3
#define Entrez_style_fasta 4
#define Entrez_style_asn1 5
#define Entrez_style_graphic 6
#define Entrez_style_alignment 7
#define Entrez_style_globalview 8
#define Entrez_style_report 9
#define Entrez_style_medlars 10
#define Entrez_style_embl 11
#define Entrez_style_pdb 12
#define Entrez_style_kinemage 13


/**************************************************
*
*    BiostrucSeqsAlignsCdd
*
**************************************************/
typedef struct struct_Biostruc_seqs_aligns_cdd {
   ValNodePtr   SeqAlignData_seq_align_data;
   struct struct_Biostruc PNTR   structures;
   Uint2   structure_type;
} BiostrucSeqsAlignsCdd, PNTR BiostrucSeqsAlignsCddPtr;

/* following #defines are for enumerated type, not used by object loaders */
#define Biostruc_seqs_aligns_cdd_structure_type_ncbi_backbone 2
#define Biostruc_seqs_aligns_cdd_structure_type_ncbi_all_atom 3
#define Biostruc_seqs_aligns_cdd_structure_type_pdb_model 4

NLM_EXTERN BiostrucSeqsAlignsCddPtr LIBCALL BiostrucSeqsAlignsCddFree PROTO ((BiostrucSeqsAlignsCddPtr ));
NLM_EXTERN BiostrucSeqsAlignsCddPtr LIBCALL BiostrucSeqsAlignsCddNew PROTO (( void ));
NLM_EXTERN BiostrucSeqsAlignsCddPtr LIBCALL BiostrucSeqsAlignsCddAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BiostrucSeqsAlignsCddAsnWrite PROTO (( BiostrucSeqsAlignsCddPtr , AsnIoPtr, AsnTypePtr));


#ifdef NLM_GENERATED_CODE_PROTO

typedef ValNodePtr SeqAlignData_seq_align_dataPtr;
typedef ValNode SeqAlignData_seq_align_data;

#endif /* NLM_GENERATED_CODE_PROTO */

#define SeqAlignData_seq_align_data_bundle 1
#define SeqAlignData_seq_align_data_cdd 2

#ifdef NLM_GENERATED_CODE_PROTO

static SeqAlignData_seq_align_dataPtr LIBCALL SeqAlignData_seq_align_dataFree PROTO ((SeqAlignData_seq_align_dataPtr ));
static SeqAlignData_seq_align_dataPtr LIBCALL SeqAlignData_seq_align_dataAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
static Boolean LIBCALL SeqAlignData_seq_align_dataAsnWrite PROTO (( SeqAlignData_seq_align_dataPtr , AsnIoPtr, AsnTypePtr));

#endif /* NLM_GENERATED_CODE_PROTO */



/**************************************************
*
*    BundleSeqsAligns
*
**************************************************/
typedef struct struct_Bundle_seqs_aligns {
   ValNodePtr   sequences;
   struct seqannot PNTR   seqaligns;    /* manual change */
   struct struct_Biostruc_annot_set PNTR   strucaligns;
   struct seqannot PNTR   imports;
   struct struct_Cn3d_style_dictionary PNTR   style_dictionary;
   struct struct_Cn3d_user_annotations PNTR   user_annotations;
} BundleSeqsAligns, PNTR BundleSeqsAlignsPtr;


NLM_EXTERN BundleSeqsAlignsPtr LIBCALL BundleSeqsAlignsFree PROTO ((BundleSeqsAlignsPtr ));
NLM_EXTERN BundleSeqsAlignsPtr LIBCALL BundleSeqsAlignsNew PROTO (( void ));
NLM_EXTERN BundleSeqsAlignsPtr LIBCALL BundleSeqsAlignsAsnRead PROTO (( AsnIoPtr, AsnTypePtr));
NLM_EXTERN Boolean LIBCALL BundleSeqsAlignsAsnWrite PROTO (( BundleSeqsAlignsPtr , AsnIoPtr, AsnTypePtr));


#ifdef __cplusplus
/* { */ }
#endif

#endif /* _objmime_ */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

