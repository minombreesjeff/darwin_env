/*  objfdef.h
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
* File Name:  objfdef.h
*
* Author:  James Ostell
*   
* Version Creation Date: 9/94
*
* $Revision: 6.2 $
*
* File Description:  Object manager for feature definitions
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: objfdef.h,v $
* Revision 6.2  2001/05/29 22:46:03  kans
* added FEATDEF_snoRNA, increased FEATDEF_MAX
*
* Revision 6.1  1998/06/29 20:31:28  kans
* explicit defines for FEATDEF_MAX, SEQFEAT_MAX, and SEQDESCR_MAX Boolean array sizes
*
* Revision 6.0  1997/08/25 18:49:41  madden
* Revision changed to 6.0
*
* Revision 4.2  1997/06/19 18:41:06  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.1  1996/07/17 15:00:12  ostell
* added preprotein, mat_peptide_aa, sig_peptide_aa, tran sit_peptide_aa
*
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 1.4  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_FeatDef_
#define _NCBI_FeatDef_

#ifndef _NCBI_SeqFeat_
#include <objfeat.h>
#endif

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   loader
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL FeatDefAsnLoad PROTO((void));

/*****************************************************************************
*
*   FeatDef
*
*****************************************************************************/
typedef struct featdef {
    CharPtr typelabel,
        menulabel;
	Uint1 featdef_key,
		seqfeat_key,
		entrygroup,
		displaygroup,
		molgroup;
    struct featdef PNTR next;
} FeatDef, PNTR FeatDefPtr;

NLM_EXTERN FeatDefPtr LIBCALL FeatDefNew PROTO((void));
NLM_EXTERN Boolean   LIBCALL FeatDefAsnWrite PROTO((FeatDefPtr fdp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN FeatDefPtr LIBCALL FeatDefAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN FeatDefPtr LIBCALL FeatDefFree PROTO((FeatDefPtr fdp));

NLM_EXTERN Boolean   LIBCALL FeatDefSetAsnWrite PROTO((FeatDefPtr fdp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN FeatDefPtr LIBCALL FeatDefSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN FeatDefPtr LIBCALL FeatDefSetFree PROTO((FeatDefPtr fdp));


/*****************************************************************************
*
*   FeatDispGroup
*
*****************************************************************************/
typedef struct featdispgroup {
	Uint1 groupkey;
    CharPtr groupname;
    struct featdispgroup PNTR next;
} FeatDispGroup, PNTR FeatDispGroupPtr;

NLM_EXTERN FeatDispGroupPtr LIBCALL FeatDispGroupNew PROTO((void));
NLM_EXTERN Boolean   LIBCALL FeatDispGroupAsnWrite PROTO((FeatDispGroupPtr fdp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN FeatDispGroupPtr LIBCALL FeatDispGroupAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN FeatDispGroupPtr LIBCALL FeatDispGroupFree PROTO((FeatDispGroupPtr fdp));

NLM_EXTERN Boolean   LIBCALL FeatDispGroupSetAsnWrite PROTO((FeatDispGroupPtr fdp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN FeatDispGroupPtr LIBCALL FeatDispGroupSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN FeatDispGroupPtr LIBCALL FeatDispGroupSetFree PROTO((FeatDispGroupPtr fdp));


/*****************************************************************************
*
*   Data Loader Function
*   	Loads default FeatDefSet and FeatDispGroupSet from "featdef.val"
*       located in /ncbi/data directory
*       returns head of linked list
*
*****************************************************************************/

NLM_EXTERN FeatDefPtr LIBCALL FeatDefSetLoad PROTO((void));

/*****************************************************************************
*
*   FindFeatDefType(sfp)
*   	Finds the featdef_type for a SeqFeat
*       returns FEATDEF_BAD if can't find it
*
*****************************************************************************/
NLM_EXTERN Uint1 LIBCALL FindFeatDefType PROTO((SeqFeatPtr sfp));

/*****************************************************************************
*
*   FeatDefLabel(sfp, buf, buflen, type)
*   	fills in buf with a content based label
*   	if longer than buflen, makes the last visible char >
*   	guarantees a '\0' at the end of the string
*   
*   NOTE: buf MUST be (buflen+1) long
*   
*   	This function makes nicer labels since it can combine elements
*   	returns length of string or 0 on failure
*
*   	type is OM_LABEL_TYPE_ defined in objmgr.h
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL FeatDefLabel PROTO((SeqFeatPtr sfp, CharPtr buf, Int2 buflen, Uint1 labeltype));

/*****************************************************************************
*
*   FeatDefTypeLabel(sfp)
*   	returns a short string with the feature type
*       returns NULL if can't find one
*
*****************************************************************************/
NLM_EXTERN const char * LIBCALL FeatDefTypeLabel PROTO((SeqFeatPtr sfp));

/*****************************************************************************
*
*   DispGroupNum()
*   	returns number of display groups
*   	returns 0 on failure
*       loads featdef.val if not already loaded
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL DispGroupNum PROTO((void));

/*****************************************************************************
*
*   DispGroupFindNext(curr, groupptr, groupname)
*     returns display groups in order
*     start with curr=NULL, then return current in curr until function
*       returns NULL
*     loads featdef.val if necessary
*     groupptr is filled in with the key for the group, used
*       in FeatDefFindNext() below.
*     groupname points to the string naming the group
*
*****************************************************************************/
NLM_EXTERN FeatDispGroupPtr LIBCALL DispGroupFindNext PROTO((FeatDispGroupPtr curr, Uint1Ptr groupptr, CharPtr PNTR groupname));

/*****************************************************************************
*
*   FeatDefNum()
*   	returns total number of FeatDef
*       loads featdef.val if necessary
*
*****************************************************************************/
NLM_EXTERN Int2 LIBCALL FeatDefNum PROTO((void));


/*****************************************************************************
*
*   FeatDefFindNext(curr, keyptr, menulabel, group, for_display)
*   	returns next FeatDef within display group
*       if group == FEATDEF_ANY returns all
*       start with curr = NULL and return current in curr until function
*         returns NULL
*       keyptr is filled in with featdef-key
*       menulabel is filled in with menulabel
*       if for_display == TRUE then group must match display group
*         else group must match entrygroup
*       loads featdef.val if necessary
*
*****************************************************************************/
NLM_EXTERN FeatDefPtr LIBCALL FeatDefFindNext PROTO((FeatDefPtr curr, Uint1Ptr keyptr, CharPtr PNTR menulabel, Uint1 group, Boolean for_display));

/*****************************************************************************
*
*   #defines for FeatDef types
*
*****************************************************************************/

#define FEATDEF_BAD  0
#define FEATDEF_ANY  255

#define FEATDEF_GENE  1
#define FEATDEF_ORG   2
#define FEATDEF_CDS   3
#define FEATDEF_PROT  4
#define FEATDEF_preRNA 5
#define FEATDEF_mRNA  6
#define FEATDEF_tRNA  7
#define FEATDEF_rRNA  8
#define FEATDEF_snRNA 9
#define FEATDEF_scRNA 10
#define FEATDEF_otherRNA 11
#define FEATDEF_PUB   12
#define FEATDEF_SEQ   13
#define FEATDEF_IMP   14
#define FEATDEF_allele 15
#define FEATDEF_attenuator 16
#define FEATDEF_C_region 17
#define FEATDEF_CAAT_signal 18
#define FEATDEF_Imp_CDS 19
#define FEATDEF_conflict 20
#define FEATDEF_D_loop 21
#define FEATDEF_D_segment 22
#define FEATDEF_enhancer 23
#define FEATDEF_exon 24
#define FEATDEF_GC_signal 25
#define FEATDEF_iDNA 26
#define FEATDEF_intron 27
#define FEATDEF_J_segment 28
#define FEATDEF_LTR 29
#define FEATDEF_mat_peptide 30
#define FEATDEF_misc_binding 31
#define FEATDEF_misc_difference 32
#define FEATDEF_misc_feature 33
#define FEATDEF_misc_recomb 34
#define FEATDEF_misc_RNA 35
#define FEATDEF_misc_signal 36
#define FEATDEF_misc_structure 37
#define FEATDEF_modified_base 38
#define FEATDEF_mutation 39
#define FEATDEF_N_region 40
#define FEATDEF_old_sequence 41
#define FEATDEF_polyA_signal 42
#define FEATDEF_polyA_site 43
#define FEATDEF_precursor_RNA 44
#define FEATDEF_prim_transcript 45
#define FEATDEF_primer_bind 46
#define FEATDEF_promoter 47
#define FEATDEF_protein_bind 48
#define FEATDEF_RBS 49
#define FEATDEF_repeat_region 50
#define FEATDEF_repeat_unit 51
#define FEATDEF_rep_origin 52
#define FEATDEF_S_region 53
#define FEATDEF_satellite 54
#define FEATDEF_sig_peptide 55
#define FEATDEF_source 56
#define FEATDEF_stem_loop 57
#define FEATDEF_STS 58
#define FEATDEF_TATA_signal 59
#define FEATDEF_terminator 60
#define FEATDEF_transit_peptide 61
#define FEATDEF_unsure 62
#define FEATDEF_V_region 63
#define FEATDEF_V_segment 64
#define FEATDEF_variation 65
#define FEATDEF_virion 66
#define FEATDEF_3clip 67
#define FEATDEF_3UTR 68
#define FEATDEF_5clip 69
#define FEATDEF_5UTR 70
#define FEATDEF_10_signal 71
#define FEATDEF_35_signal 72
#define FEATDEF_site_ref 73
#define FEATDEF_REGION 74
#define FEATDEF_COMMENT 75
#define FEATDEF_BOND 76
#define FEATDEF_SITE 77
#define FEATDEF_RSITE 78
#define FEATDEF_USER 79
#define FEATDEF_TXINIT 80
#define FEATDEF_NUM 81
#define FEATDEF_PSEC_STR 82
#define FEATDEF_NON_STD_RESIDUE 83
#define FEATDEF_HET 84
#define FEATDEF_BIOSRC 85
#define FEATDEF_preprotein 86
#define FEATDEF_mat_peptide_aa 87
#define FEATDEF_sig_peptide_aa 88
#define FEATDEF_transit_peptide_aa 89
#define FEATDEF_snoRNA 90

#define FEATDEF_MAX 91 /* size of array needed for featdef filter parameters */


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
