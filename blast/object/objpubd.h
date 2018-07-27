/*  objpubd.h
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
* File Name:  objpubd.h
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.4 $
*
* File Description:  Object manager interface for type Pubdesc from
*                    NCBI-Sequence.  This is separate to avoid typedef
*                    order problems with NCBI-Sequence and NCBI-Seqfeat
*                    which both reference Pubdesc
*   				   Numbering and Heterogen have now also been added
*   				for the same reason.  Heterogen is just a string,
*   				so no special typedefs are required.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: objpubd.h,v $
* Revision 6.4  2000/09/28 11:53:13  ostell
* added Pubdesc.reftype = 3
*
* Revision 6.3  2000/05/26 19:50:23  ostell
* added MI_TECH_htc
*
* Revision 6.2  1999/02/12 18:45:22  ostell
* added MI_TECH_htgs_0
*
* Revision 6.1  1998/06/12 18:02:26  kans
* added MI_TECH_fli_cdna 17 define
*
* Revision 6.0  1997/08/25 18:50:32  madden
* Revision changed to 6.0
*
* Revision 4.3  1997/08/20 19:17:13  madden
* Version 6 of ASN.1
*
* Revision 4.2  1997/06/19 18:41:50  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.1  1996/06/13 21:09:08  ostell
* added defines for MI_TECH
*
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.2  1995/07/11  18:49:20  ostell
 * added reftype
 *
 * Revision 3.1  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
*
* ==========================================================================
*/

#ifndef _NCBI_Pubdesc_
#define _NCBI_Pubdesc_

#ifndef _ASNTOOL_
#include <asn.h>
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
*   Pubdesc
*
*****************************************************************************/
typedef struct pd {
    ValNodePtr pub;          /* points to Pub-equiv */
    CharPtr name,
        fig;
    ValNodePtr num;          /* points to Numbering */
    Boolean numexc,
        poly_a;
    Uint1 align_group;       /* 0 = not part of a group */
    CharPtr maploc,
        seq_raw,
		comment;
	Uint1 reftype;     /* 0=seq, 1=sites, 2=feats,
                              3=seq with no explicit span, for GenBank ref */
} Pubdesc, PNTR PubdescPtr;

NLM_EXTERN PubdescPtr LIBCALL PubdescNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL PubdescAsnWrite PROTO((PubdescPtr pdp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN PubdescPtr LIBCALL PubdescAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN PubdescPtr LIBCALL PubdescFree PROTO((PubdescPtr pdp));

typedef ValNode Numbering, FAR *NumberingPtr;

/*****************************************************************************
*
*   Numbering uses an ValNode with choice = 
    1 = cont Num-cont ,              -- continuous numbering
    2 = enum Num-enum ,              -- enumerated names for residues
    3 = ref Num-ref, type 1 sources  -- by reference to another sequence
    4 = ref Num-ref, type 2 aligns  (SeqAlign in data.ptrvalue)
    5 = real Num-real     -- for maps etc
*
*****************************************************************************/

#define Numbering_cont 1
#define Numbering_enum 2
#define Numbering_ref_source 3
#define Numbering_ref_align 4
#define Numbering_real 5

NLM_EXTERN Boolean      LIBCALL NumberingAsnWrite PROTO((NumberingPtr anp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN NumberingPtr LIBCALL NumberingAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN NumberingPtr LIBCALL NumberingFree PROTO((NumberingPtr anp));

/*****************************************************************************
*
*   NumCont - continuous numbering system
*
*****************************************************************************/
typedef struct numcont {
    Int4 refnum;
    Boolean has_zero,
        ascending;
} NumCont, PNTR NumContPtr;

NLM_EXTERN NumContPtr LIBCALL NumContNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL NumContAsnWrite PROTO((NumContPtr ncp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN NumContPtr LIBCALL NumContAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN NumContPtr LIBCALL NumContFree PROTO((NumContPtr ncp));

/*****************************************************************************
*
*   NumEnum - enumerated numbering system
*
*****************************************************************************/
typedef struct numenum {
    Int4 num;               /* number of names */
    CharPtr buf;            /* a buffer for the names */
    CharPtr PNTR names;     /* array of pointers to names */
} NumEnum, PNTR NumEnumPtr;

NLM_EXTERN NumEnumPtr LIBCALL NumEnumNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL NumEnumAsnWrite PROTO((NumEnumPtr nep, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN NumEnumPtr LIBCALL NumEnumAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN NumEnumPtr LIBCALL NumEnumFree PROTO((NumEnumPtr nep));

/*****************************************************************************
*
*   NumReal - float type numbering system
*
*****************************************************************************/
typedef struct numreal {
    FloatHi a, b;        /* number in "units" = ax + b */
    CharPtr units;
} NumReal, PNTR NumRealPtr;

NLM_EXTERN NumRealPtr LIBCALL NumRealNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL NumRealAsnWrite PROTO((NumRealPtr ncp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN NumRealPtr LIBCALL NumRealAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN NumRealPtr LIBCALL NumRealFree PROTO((NumRealPtr ncp));

/*****************************************************************************
*
*   MolInfo - Info about mols
*
*****************************************************************************/
typedef struct molinfo {
	Uint1 biomol,
		tech;
	CharPtr techexp;
	Uint1 completeness;
} MolInfo, PNTR MolInfoPtr;

#define MI_TECH_unknown 0
#define MI_TECH_standard 1
#define MI_TECH_est 2        /* EST division */
#define MI_TECH_sts 3        /* STS division */
#define MI_TECH_survey 4     /* GSS division */
#define MI_TECH_genemap 5    /* Bioseq is a genetic map */
#define MI_TECH_physmap 6    /* Bioseq is physical map */
#define MI_TECH_derived 7    /* Bioseq is a computed inference */
#define MI_TECH_concept_trans 8  /* conceptual translation */
#define MI_TECH_seq_pept 9       /* peptide sequencing used */
#define MI_TECH_both 10          /* combination of 8 and 9 used */
#define MI_TECH_seq_pept_overlap 11 /* peptides ordered by overlap */
#define MI_TECH_seq_pept_homol 12   /* peptides ordered by homology */
#define MI_TECH_concept_trans_a 13  /* concept trans supplied by author */
#define MI_TECH_htgs_1 14  /* unordered High Throughput Sequence Contig */
#define MI_TECH_htgs_2 15  /* ordered High Throughput sequence contig */
#define MI_TECH_htgs_3 16  /* finished High Throughut sequence */
#define MI_TECH_fli_cdna 17  /* full-length insert cDNA sequence */
#define MI_TECH_htgs_0 18    /* unordered single pass reads */
#define MI_TECH_htc 19       /* High Throughput CDNA.. unfinished */
#define MI_TECH_other 255           /* doesnt' fit anything */

NLM_EXTERN MolInfoPtr LIBCALL MolInfoNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL MolInfoAsnWrite PROTO((MolInfoPtr mip, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN MolInfoPtr LIBCALL MolInfoAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN MolInfoPtr LIBCALL MolInfoFree PROTO((MolInfoPtr mip));


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
