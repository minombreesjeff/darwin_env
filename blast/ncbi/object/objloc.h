/*  objloc.h
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE                          
*               National Center for Biotechnology Information
*                                                                          
*  This software/database is a "United States Government Work" under the   
*  terms of the United States Copyright Act.  It was written as part of    
*  the author's official duties as a United States Government employee and 
*  thus cannot be copyrighted.  This software/database is freely available 
*  to the loclic for use. The National Library of Medicine and the U.S.    
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
* File Name:  objloc.h
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.3 $
*
* File Description:  Object manager interface for module NCBI-Seqloc
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: objloc.h,v $
* Revision 6.3  2001/08/06 20:07:26  ostell
* added SEQID_TPG, TPE, TPD types
*
* Revision 6.2  2001/01/31 15:24:20  kans
* PatentSeqId.seqid is now an Int4 (JO)
*
* Revision 6.1  2000/04/05 18:11:41  dondosha
* Moved SeqIdSetDup from mblast.h
*
* Revision 6.0  1997/08/25 18:50:07  madden
* Revision changed to 6.0
*
* Revision 4.1  1997/06/19 18:41:32  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.0  1995/07/26 13:48:06  ostell
* force revision to 4.0
*
 * Revision 3.3  1995/05/09  18:04:44  ostell
 * added NUM_SEGID to define how many SeqId classes there are
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_Seqloc_
#define _NCBI_Seqloc_

#ifndef _ASNTOOL_
#include <asn.h>
#endif
#ifndef _NCBI_General_
#include <objgen.h>
#endif
#ifndef _NCBI_Biblio_
#include <objbibli.h>
#endif

typedef ValNode SeqId, FAR *SeqIdPtr;
typedef ValNode SeqLoc, FAR *SeqLocPtr;

#ifndef _NCBI_Seqfeat_
#include <objfeat.h>      /* after Seqloc to avoid cycles */
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
*   Seqloc loader
*
*****************************************************************************/
NLM_EXTERN Boolean	LIBCALL SeqLocAsnLoad PROTO((void));

/*****************************************************************************
*
*   internal structures for NCBI-Seqloc objects
*
*****************************************************************************/

/*****************************************************************************
*
*   SeqId is a choice using an ValNode, most types in data.ptrvalue
*      except integers, in data.intvalue
*   choice:
*   0 = not set
    1 = local Object-id ,      -- local use
    2 = gibbsq INTEGER ,         -- Geninfo backbone seqid
    3 = gibbmt INTEGER ,         -- Geninfo backbone moltype
    4 = giim Giimport-id ,       -- Geninfo import id
    5 = genbank Textseq-id ,
    6 = embl Textseq-id ,
    7 = pir Textseq-id ,
    8 = swissprot Textseq-id ,
    9 = patent Patent-seq-id ,
    10 = other Textseq-id ,       -- catch all
    11 = general Dbtag          -- for other databases
    12 = gi  INTEGER          -- GenInfo Integrated Database
    13 = ddbj Textseq-id
	14 = prf Textseq-id ,         -- PRF SEQDB
	15 = pdb PDB-seq-id          -- PDB sequence
    16 = tpg Textseq-id ,         -- Third Party Annot/Seq Genbank
    17 = tpe Textseq-id ,         -- Third Party Annot/Seq EMBL
    18 = tpd Textseq-id }         -- Third Party Annot/Seq DDBJ


	WARNING: Some routines in sequtil.c (e.g. SeqIdPrint, SeqIdParse)
        and tofasta.c  use
	arrays of size NUM_SEQID to set certain behaviors. They should be changed
	when SeqIds are added or changed.
*
*****************************************************************************/

#define NUM_SEQID 19     /* total number of SeqId types */

#define SEQID_NOT_SET ( (Uint1)0)
#define SEQID_LOCAL ( (Uint1)1)
#define SEQID_GIBBSQ ( (Uint1)2)
#define SEQID_GIBBMT ( (Uint1)3)
#define SEQID_GIIM ( (Uint1)4)

#define SEQID_GENBANK ( (Uint1)5)
#define SEQID_EMBL ( (Uint1)6)
#define SEQID_PIR ( (Uint1)7)
#define SEQID_SWISSPROT ( (Uint1)8)


#define SEQID_PATENT ( (Uint1)9)
#define SEQID_OTHER ( (Uint1)10)
#define SEQID_GENERAL ( (Uint1)11)
#define SEQID_GI ( (Uint1)12)
#define SEQID_DDBJ ((Uint1)13)
#define SEQID_PRF ((Uint1)14)
#define SEQID_PDB ((Uint1)15)

#define SEQID_TPG ((Uint1)16)
#define SEQID_TPE ((Uint1)17)
#define SEQID_TPD ((Uint1)18)

NLM_EXTERN Boolean	 LIBCALL SeqIdAsnWrite PROTO((SeqIdPtr anp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqIdPtr LIBCALL SeqIdAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqIdPtr LIBCALL SeqIdFree PROTO((SeqIdPtr anp));
NLM_EXTERN SeqIdPtr LIBCALL SeqIdDup PROTO((SeqIdPtr oldid));
NLM_EXTERN SeqIdPtr LIBCALL SeqIdSetDup PROTO((SeqIdPtr seqid));
NLM_EXTERN Int2 LIBCALL SeqIdLabel PROTO(( SeqIdPtr sip, CharPtr buffer, Int2 buflen, Uint1 content));

/*****************************************************************************
*
*   These routines process sets or sequences of SeqId's
*
*****************************************************************************/
NLM_EXTERN Boolean  LIBCALL SeqIdSetAsnWrite PROTO((SeqIdPtr anp, AsnIoPtr aip, AsnTypePtr settype, AsnTypePtr elementtype));
NLM_EXTERN SeqIdPtr LIBCALL SeqIdSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr settype, AsnTypePtr elementtype));
NLM_EXTERN SeqIdPtr LIBCALL SeqIdSetFree PROTO((SeqIdPtr anp));


/*****************************************************************************
*
*   PatentSeqId
*
*****************************************************************************/
typedef struct patentseqid {
    Int4 seqid;
    IdPatPtr cit;
} PatentSeqId, PNTR PatentSeqIdPtr;

NLM_EXTERN PatentSeqIdPtr LIBCALL PatentSeqIdNew PROTO((void));
NLM_EXTERN Boolean        LIBCALL PatentSeqIdAsnWrite PROTO((PatentSeqIdPtr psip, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN PatentSeqIdPtr LIBCALL PatentSeqIdAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN PatentSeqIdPtr LIBCALL PatentSeqIdFree PROTO((PatentSeqIdPtr psip));

/*****************************************************************************
*
*   TextSeqId
*
*****************************************************************************/
typedef struct textseqid {
    CharPtr name,
        accession,
        release;
	Int2 version;             /* INT2_MIN (ncbilcl.h) = not set */
} TextSeqId, PNTR TextSeqIdPtr;

NLM_EXTERN TextSeqIdPtr LIBCALL TextSeqIdNew PROTO((void));
NLM_EXTERN Boolean      LIBCALL TextSeqIdAsnWrite PROTO((TextSeqIdPtr tsip, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN TextSeqIdPtr LIBCALL TextSeqIdAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN TextSeqIdPtr LIBCALL TextSeqIdFree PROTO((TextSeqIdPtr tsip));

/*****************************************************************************
*
*   Giim
*
*****************************************************************************/
typedef struct giim {
    Int4 id;
    CharPtr db,
        release;
} Giim, PNTR GiimPtr;

NLM_EXTERN GiimPtr LIBCALL GiimNew PROTO((void));
NLM_EXTERN Boolean LIBCALL GiimAsnWrite PROTO((GiimPtr gip, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN GiimPtr LIBCALL GiimAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN GiimPtr LIBCALL GiimFree PROTO((GiimPtr gip));


/*****************************************************************************
*
*   PDBSeqId
*
*****************************************************************************/
typedef struct pdbseqid {
    CharPtr mol;
	Uint1 chain;        /* 0 = no chain set.  default = 32 */
	DatePtr rel;
} PDBSeqId, PNTR PDBSeqIdPtr;

NLM_EXTERN PDBSeqIdPtr LIBCALL PDBSeqIdNew PROTO((void));
NLM_EXTERN Boolean     LIBCALL PDBSeqIdAsnWrite PROTO((PDBSeqIdPtr tsip, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN PDBSeqIdPtr LIBCALL PDBSeqIdAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN PDBSeqIdPtr LIBCALL PDBSeqIdFree PROTO((PDBSeqIdPtr tsip));

/*****************************************************************************
*
*   SeqLoc
*   SeqLoc is a choice using an ValNode, most types in data.ptrvalue
*      except integers, in data.intvalue
*   choice:
    1 = null NULL ,           -- not placed
    2 = empty Seq-id ,        -- to NULL one Seq-id in a collection
    3 = whole Seq-id ,        -- whole sequence
    4 = int Seq-interval ,    -- from to
    5 = packed-int Packed-seqint ,
    6 = pnt Seq-point ,
    7 = packed-pnt Packed-seqpnt ,
    8 = mix SEQUENCE OF Seq-loc ,
    9 = equiv SET OF Seq-loc ,  -- equivalent sets of locations
    10 = bond Seq-bond
    11 = feat Feat-id    -- indirect through a feature
*
*****************************************************************************/
#define SEQLOC_NULL ( (Uint1)1)
#define SEQLOC_EMPTY ( (Uint1)2)
#define SEQLOC_WHOLE ( (Uint1)3)
#define SEQLOC_INT ( (Uint1)4)
#define SEQLOC_PACKED_INT ( (Uint1)5)
#define SEQLOC_PNT ( (Uint1)6)
#define SEQLOC_PACKED_PNT ( (Uint1)7)
#define SEQLOC_MIX ( (Uint1)8)
#define SEQLOC_EQUIV ( (Uint1)9)
#define SEQLOC_BOND ( (Uint1)10)
#define SEQLOC_FEAT ( (Uint1)11)

NLM_EXTERN Boolean   LIBCALL SeqLocAsnWrite PROTO((SeqLocPtr anp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqLocPtr LIBCALL SeqLocAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqLocPtr LIBCALL SeqLocFree PROTO((SeqLocPtr anp));
NLM_EXTERN Int2 LIBCALL SeqLocLabel PROTO((SeqLocPtr slp, CharPtr buffer, Int2 buflen, Uint1 content));

/*****************************************************************************
*
*   these routines work on set/seq of SeqLoc
*
*****************************************************************************/
NLM_EXTERN Boolean   LIBCALL SeqLocSetAsnWrite PROTO((SeqLocPtr anp, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN SeqLocPtr LIBCALL SeqLocSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr orig, AsnTypePtr element));
NLM_EXTERN SeqLocPtr LIBCALL SeqLocSetFree PROTO((SeqLocPtr anp));

/*****************************************************************************
*
*   SeqInt
*
*****************************************************************************/
typedef struct seqint {
    Int4 from,
        to;
    Uint1 strand;
    SeqIdPtr id;    /* seq-id */
    IntFuzzPtr if_from,
               if_to;
} SeqInt, PNTR SeqIntPtr;

NLM_EXTERN SeqIntPtr LIBCALL SeqIntNew PROTO((void));
NLM_EXTERN Boolean   LIBCALL SeqIntAsnWrite PROTO((SeqIntPtr sip, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqIntPtr LIBCALL SeqIntAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqIntPtr LIBCALL SeqIntFree PROTO((SeqIntPtr sip));

/*****************************************************************************
*
*   Packed-int
*
*****************************************************************************/

NLM_EXTERN Boolean   LIBCALL PackSeqIntAsnWrite PROTO((SeqLocPtr sip, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqLocPtr LIBCALL PackSeqIntAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   SeqLocMix
*
*****************************************************************************/

NLM_EXTERN Boolean   LIBCALL SeqLocMixAsnWrite PROTO((SeqLocPtr anp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqLocPtr LIBCALL SeqLocMixAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   SeqLocEquiv
*
*****************************************************************************/

NLM_EXTERN Boolean   LIBCALL SeqLocEquivAsnWrite PROTO((SeqLocPtr anp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqLocPtr LIBCALL SeqLocEquivAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));

/*****************************************************************************
*
*   SeqPnt
*
*****************************************************************************/
typedef struct seqpoint {
    Int4 point;
    Uint1 strand;
    SeqIdPtr id;    /* seq-id */
    IntFuzzPtr fuzz;
} SeqPnt, PNTR SeqPntPtr;

NLM_EXTERN SeqPntPtr LIBCALL SeqPntNew PROTO((void));
NLM_EXTERN Boolean   LIBCALL SeqPntAsnWrite PROTO((SeqPntPtr spp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqPntPtr LIBCALL SeqPntAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqPntPtr LIBCALL SeqPntFree PROTO((SeqPntPtr spp));

/*****************************************************************************
*
*   PackSeqPnt
*
*****************************************************************************/
#define PACK_PNT_NUM 100     /* number of points per block */

typedef struct packseqpnt {
    SeqIdPtr id;    /* seq-id */
    IntFuzzPtr fuzz;
    Uint1 strand,
          used;       /* number of pnts used */
    Int4 pnts[PACK_PNT_NUM];
    struct packseqpnt PNTR next;   /* builds up chain of points */
} PackSeqPnt, PNTR PackSeqPntPtr;

NLM_EXTERN PackSeqPntPtr LIBCALL PackSeqPntNew PROTO((void));
NLM_EXTERN Boolean       LIBCALL PackSeqPntAsnWrite PROTO((PackSeqPntPtr pspp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN PackSeqPntPtr LIBCALL PackSeqPntAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN PackSeqPntPtr LIBCALL PackSeqPntFree PROTO((PackSeqPntPtr pspp));
NLM_EXTERN Int4          LIBCALL PackSeqPntGet PROTO((PackSeqPntPtr pspp, Int4 index));
NLM_EXTERN Boolean       LIBCALL PackSeqPntPut PROTO((PackSeqPntPtr pspp, Int4 point));
NLM_EXTERN Int4          LIBCALL PackSeqPntNum PROTO((PackSeqPntPtr pspp));

/*****************************************************************************
*
*   SeqBond
*
*****************************************************************************/
typedef struct seqbond {
    SeqPntPtr a,
                b;
} SeqBond, PNTR SeqBondPtr;

NLM_EXTERN SeqBondPtr LIBCALL SeqBondNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL SeqBondAsnWrite PROTO((SeqBondPtr sbp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqBondPtr LIBCALL SeqBondAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqBondPtr LIBCALL SeqBondFree PROTO((SeqBondPtr sbp));


/*****************************************************************************
*
*   strand types
*
*****************************************************************************/
#define Seq_strand_unknown 0
#define Seq_strand_plus 1
#define Seq_strand_minus 2
#define Seq_strand_both 3
#define Seq_strand_both_rev 4
#define Seq_strand_other 255

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
