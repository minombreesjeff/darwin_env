/*  objsset.h
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
* File Name:  objsset.h
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.5 $
*
* File Description:  Object manager interface for module NCBI-Seqset
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: objsset.h,v $
* Revision 6.5  2001/05/14 19:54:20  kans
* redefine BioseqseqSet_class_gen_prod_set to 17, now in ASN.1 spec
*
* Revision 6.4  2001/02/14 23:57:28  kans
* added define for BioseqseqSet_class_eco_set
*
* Revision 6.3  1999/09/27 17:48:38  kans
* using GatherIndex structure
*
* Revision 6.2  1999/09/24 23:09:24  kans
* adds EXTRA_OBJMGR_FIELDS to several objects
*
* Revision 6.1  1998/05/29 20:49:26  kans
* added BioseqseqSet_class_gen_prod_set, currently same as _other (255)
*
* Revision 6.0  1997/08/25 18:50:58  madden
* Revision changed to 6.0
*
* Revision 4.2  1997/06/19 18:42:09  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.1  1996/03/06 20:44:45  ostell
* added defines for bioseqset classes
*
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.3  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_Seqset_
#define _NCBI_Seqset_

#ifndef _ASNTOOL_
#include <asn.h>
#endif
#ifndef _NCBI_General_
#include <objgen.h>
#endif
#ifndef _NCBI_Seq_
#include <objseq.h>
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
NLM_EXTERN Boolean LIBCALL SeqSetAsnLoad PROTO((void));

/*****************************************************************************
*
*   internal structures for NCBI-Seqset objects
*
*****************************************************************************/
/*****************************************************************************
*
*   BioseqSet - a collection of sequences
*
*****************************************************************************/
typedef struct seqset {
    ObjectIdPtr id;
    DbtagPtr coll;
    Int2 level;            /* set to INT2_MIN (ncbilcl.h) for not used */
    Uint1 _class;
    CharPtr release;
    DatePtr date;
    ValNodePtr descr;
    SeqEntryPtr seq_set;
    SeqAnnotPtr annot;
	GatherIndex idx;        /* internal gather/objmgr tracking fields */
	SeqEntryPtr seqentry;   /* internal seqentry that points to this bioseqset */
} BioseqSet, PNTR BioseqSetPtr;

NLM_EXTERN BioseqSetPtr LIBCALL BioseqSetNew PROTO((void));
NLM_EXTERN Boolean      LIBCALL BioseqSetAsnWrite PROTO((BioseqSetPtr bsp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN BioseqSetPtr LIBCALL BioseqSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN BioseqSetPtr LIBCALL BioseqSetFree PROTO((BioseqSetPtr bsp));
NLM_EXTERN Int2 LIBCALL BioseqSetLabel PROTO((BioseqSetPtr bssp, CharPtr buffer, Int2 buflen, Uint1 content));

/*****************************************************************************
*
*   defines for BioseqSet._class
        not-set (0) ,
        nuc-prot (1) ,              -- nuc acid and coded proteins
        segset (2) ,                -- segmented sequence + parts
        conset (3) ,                -- constructed sequence + parts
        parts (4) ,                 -- parts for 2 or 3
        gibb (5) ,                  -- geninfo backbone
        gi (6) ,                    -- geninfo
        genbank (7) ,               -- converted genbank
        pir (8) ,                   -- converted pir
        pub-set (9) ,               -- all the seqs from a single publication
        equiv (10) ,                -- a set of equivalent maps or seqs
        swissprot (11) ,            -- converted SWISSPROT
        pdb-entry (12) ,            -- a complete PDB entry
        mut-set (13) ,              -- set of mutations
        pop-set (14) ,              -- population study
        phy-set (15) ,              -- phylogenetic study
        other (255) } DEFAULT not-set ,

*
******************************************************************************/
#define BioseqseqSet_class_not_set 0
#define BioseqseqSet_class_nuc_prot 1
#define BioseqseqSet_class_segset 2
#define BioseqseqSet_class_conset 3
#define BioseqseqSet_class_parts 4
#define BioseqseqSet_class_gibb 5
#define BioseqseqSet_class_gi 6
#define BioseqseqSet_class_genbank 7
#define BioseqseqSet_class_pir 8
#define BioseqseqSet_class_pub_set 9
#define BioseqseqSet_class_equiv 10
#define BioseqseqSet_class_swissprot 11
#define BioseqseqSet_class_pdb_entry 12
#define BioseqseqSet_class_mut_set 13
#define BioseqseqSet_class_pop_set 14
#define BioseqseqSet_class_phy_set 15
#define BioseqseqSet_class_eco_set 16
#define BioseqseqSet_class_gen_prod_set 17
#define BioseqseqSet_class_other 255

/*****************************************************************************
*
*   BioseqSetFreeComponents(bsp, parts)
*       Frees associated data of a BioseqSet
*   	if (parts == FALSE)
*   	      Calls SeqEntryFreeComponents() for seq-set
*       else
*             Calls SeqEntryFree()
*   	Does not free the BioseqSet itself
*   	Called by BioseqSetFree
*       Used by SeqMgr for caching out
*
*****************************************************************************/
NLM_EXTERN BioseqSetPtr LIBCALL BioseqSetFreeComponents PROTO((BioseqSetPtr bsp, Boolean parts));

/*****************************************************************************
*
*   SeqEntry - implemented as an ValNode
*     choice:
*       1 = Bioseq
*       2 = Bioseq-set
*
*****************************************************************************/

NLM_EXTERN SeqEntryPtr LIBCALL SeqEntryNew PROTO((void));
NLM_EXTERN Boolean     LIBCALL SeqEntryAsnWrite PROTO((SeqEntryPtr sep, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqEntryPtr LIBCALL SeqEntryAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqEntryPtr LIBCALL SeqEntryFree PROTO((SeqEntryPtr sep));
NLM_EXTERN Int2 LIBCALL SeqEntryLabel PROTO((SeqEntryPtr sep, CharPtr buffer, Int2 buflen, Uint1 content));

/*****************************************************************************
*
*   SeqEntryFreeComponents(sep)
*       Frees components of elements associated with SeqEntry
*   	used by SeqMgr for caching out
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr LIBCALL SeqEntryFreeComponents PROTO((SeqEntryPtr sep));

/*****************************************************************************
*
*   Options for SeqEntryAsnRead()
*
*****************************************************************************/
NLM_EXTERN SeqEntryPtr LIBCALL SeqEntryAsnGet PROTO((AsnIoPtr aip, AsnTypePtr atp, SeqIdPtr sip, Int2 retcode));

#define SEQENTRY_OPTION_MAX_COMPLEX 1   /* option type to use with OP_NCBIOBJSSET */

    /* values for retcode, implemented with AsnIoOptions */
#define SEQENTRY_READ_BIOSEQ 1    /* read only Bioseq identified by sip */
#define SEQENTRY_READ_SEG_SET 2   /* read any seg-set it may be part of */
#define SEQENTRY_READ_NUC_PROT 3   /* read any nuc-prot set it may be in */
#define SEQENTRY_READ_PUB_SET 4    /* read pub-set it may be part of */

typedef struct objsset_option {
	SeqIdPtr sip;              /* seq-id to find */
	Int2 retcode;              /* type of set/seq to return */
	Boolean in_right_set;
	Uint1 working_on_set;      /* 2, if in first set of retcode type */
	                           /* 1, if found Bioseq, but not right set */
	                           /* 0, if Bioseq not yet found */
} Op_objsset, PNTR Op_objssetPtr;


#define IS_Bioseq(a) ((a)->choice == 1)
#define IS_Bioseq_set(a)  ((a)->choice == 2)

/*****************************************************************************
*
*   loader for ObjSeqSet and Sequence Codes
*
*****************************************************************************/
NLM_EXTERN Boolean LIBCALL SeqEntryLoad PROTO((void));


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
