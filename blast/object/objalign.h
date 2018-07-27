/*  objalign.h
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
* File Name:  objalign.h
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.6 $
*
* File Description:  Object manager interface for module NCBI-Seqalign
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: objalign.h,v $
* Revision 6.6  1999/09/28 12:09:56  kans
* added alignID field
*
* Revision 6.5  1999/09/27 17:48:37  kans
* using GatherIndex structure
*
* Revision 6.4  1999/09/24 23:09:23  kans
* adds EXTRA_OBJMGR_FIELDS to several objects
*
* Revision 6.3  1999/09/07 17:00:26  kans
* added entityID, itemID, itemtype fields for new Alignment Indexing functions
*
* Revision 6.2  1999/07/29 15:49:58  ostell
* added pointer and free for a SeqAlignIndex
*
* Revision 6.1  1999/07/26 20:41:01  ostell
* added SAT_ and SAS_ defines, added master to SeqAlign
*
* Revision 6.0  1997/08/25 18:49:14  madden
* Revision changed to 6.0
*
* Revision 4.1  1997/06/19 18:40:41  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.0  1995/07/26 13:48:06  ostell
* force revision to 4.0
*
 * Revision 3.5  1995/07/22  21:59:13  ostell
 * added support for ASN.1 spec 4.0
 *
 * Revision 3.4  1995/06/21  17:17:50  epstein
 * *** empty log message ***
 *
 * Revision 3.3  95/05/15  21:22:00  ostell
 * added Log line
 * 
*
*
*
* ==========================================================================
*/

#ifndef _NCBI_Seqalign_
#define _NCBI_Seqalign_

#ifndef _ASNTOOL_
#include <asn.h>
#endif
#ifndef _NCBI_General_
#include <objgen.h>
#endif
#ifndef _NCBI_Seqloc_
#include <objloc.h>
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
NLM_EXTERN Boolean LIBCALL SeqAlignAsnLoad PROTO((void));

/*****************************************************************************
*
*   internal structures for NCBI-Seqalign objects
*
*****************************************************************************/

/*****************************************************************************
*
*   Score
*     NOTE: read, write, and free always process GROUPS of scores
*
*****************************************************************************/
typedef struct score {
    ObjectIdPtr id;
    Uint1 choice;          /* 0=not set, 1=int, 2=real */
    DataVal value;
    struct score PNTR next;    /* for sets of scores */
} Score, PNTR ScorePtr;

NLM_EXTERN ScorePtr LIBCALL ScoreNew PROTO((void));
NLM_EXTERN Boolean  LIBCALL ScoreSetAsnWrite PROTO((ScorePtr sp, AsnIoPtr aip, AsnTypePtr settype));
NLM_EXTERN ScorePtr LIBCALL ScoreSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr settype));
NLM_EXTERN ScorePtr LIBCALL ScoreSetFree PROTO((ScorePtr anp));

/****************************************************************************
*
*  SeqAlignIndex
*
*    This structure is the public face of a data structure which is
*    extended in AlignMgr for alignment indexing, alignment features
*    and other utilities.
*
*    It is left as a limited structure "stub" here
*
****************************************************************************/
/** the VoidPtr below should really be a SeqAlignIndexPtr **/
typedef Boolean (LIBCALLBACK * SeqAlignIndexFreeFunc)(VoidPtr);

typedef struct seqalignindex {
	Uint1 indextype;
	SeqAlignIndexFreeFunc freefunc;
} SeqAlignIndex, PNTR SeqAlignIndexPtr;

NLM_EXTERN SeqAlignIndexPtr LIBCALL SeqAlignIndexFree (SeqAlignIndexPtr saip);
    
/*****************************************************************************
*
*   SeqAlign
*   type =  type of alignment
        not-set (0) ,
        global (1) ,
        diags (2) ,
        partial (3) ,           -- mapping pieces together
        other (255) } ,
    segtype = type of segs structure
        not-set 0
        dendiag 1
        denseq 2
        std 3
		packed 4
		disc 5      SeqAlignSet is used
	compseq 6       used by colombe 
*   
*
*****************************************************************************/
/** SeqAlign.type values ***/
#define SAT_GLOBAL 1    /* ordered segments, over full length of seqs */
#define SAT_DIAGS 2     /* unordered, possibly overlapping segments */
#define SAT_PARTIAL 3   /* ordered segments, over part of sequence */
#define SAT_MASTERSLAVE 4   /* set of SeqAligns, all of which have one common */
                            /* sequence. Not in ASN.1 yet */

/** SeqAlign.segtype values ***/
#define SAS_DENDIAG 1
#define SAS_DENSEG 2
#define SAS_STD 3
#define SAS_PACKED 4
#define SAS_DISC 5
#define SAS_COMPSEQ 6


typedef struct seqalign {
    Uint1 type,
        segtype;
    Int2 dim;
    ScorePtr score;
    Pointer segs;
    struct seqalign PNTR next;
	SeqLocPtr bounds;      /* sequence of SeqLocPtr */
    SeqIdPtr master;   /* for SAT_MASTERSLAVE */
    SeqAlignIndexPtr saip;  /* for added Alignment Indexing structures */
	GatherIndex idx;      /* internal gather/objmgr tracking fields */
	Uint2 alignID;        /* unique number assigned to alignment */
} SeqAlign, PNTR SeqAlignPtr;

NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignNew PROTO((void));
NLM_EXTERN Boolean     LIBCALL SeqAlignAsnWrite PROTO((SeqAlignPtr anp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignFree PROTO((SeqAlignPtr anp));
NLM_EXTERN Int2 LIBCALL SeqAlignLabel PROTO((SeqAlignPtr sap, CharPtr buffer, Int2 buflen, Uint1 content));

/*****************************************************************************
*
*   SeqAlignSet
*
*****************************************************************************/
NLM_EXTERN Boolean     LIBCALL SeqAlignSetAsnWrite PROTO((SeqAlignPtr anp, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignSetFree PROTO((SeqAlignPtr sap));
NLM_EXTERN SeqAlignPtr LIBCALL SeqAlignSetNew PROTO((void));
NLM_EXTERN SeqAlignPtr LIBCALL SpecialSeqAlignSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr set));
NLM_EXTERN Boolean LIBCALL SpecialSeqAlignSetAsnWrite PROTO((SeqAlignPtr sap, AsnIoPtr aip, AsnTypePtr set));


/*****************************************************************************
*
*   DenseDiag
*   
*
*****************************************************************************/
typedef struct dendiag {
    Int2 dim;                   /* this is a convenience, not in asn1 */
    SeqIdPtr id;
    Int4Ptr starts;
    Int4 len;
    Uint1Ptr strands;
    ScorePtr scores;
    struct dendiag PNTR next;
} DenseDiag, PNTR DenseDiagPtr;

NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagNew PROTO((void));
NLM_EXTERN Boolean      LIBCALL DenseDiagAsnWrite PROTO((DenseDiagPtr ddp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN DenseDiagPtr LIBCALL DenseDiagFree PROTO((DenseDiagPtr ddp));

/*****************************************************************************
*
*   DenseSeg
*   
*
*****************************************************************************/
typedef struct denseg {
    Int2 dim,
        numseg;
    SeqIdPtr ids;           /* dimension is dim */
    Int4Ptr starts;			/* dimension is dim * numseg */
    Int4Ptr lens;			/* dimension is numseg */
    Uint1Ptr strands;		/* dimension is dim * numseg */
    ScorePtr scores;		/* dimension is numseg */
} DenseSeg, PNTR DenseSegPtr;

NLM_EXTERN DenseSegPtr LIBCALL DenseSegNew PROTO((void));
NLM_EXTERN Boolean     LIBCALL DenseSegAsnWrite PROTO((DenseSegPtr dsp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN DenseSegPtr LIBCALL DenseSegAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN DenseSegPtr LIBCALL DenseSegFree PROTO((DenseSegPtr dsp));

/*****************************************************************************
*
*   PackSeg
*   
*
*****************************************************************************/
typedef struct packseg {
    Int2 dim,
        numseg;
    SeqIdPtr ids;			/* dimension is dim */
    Int4Ptr starts;			/* dimension is dim */
	ByteStorePtr present;	/* dimension is dim * numseg booleans */
    Int4Ptr lens;			/* dimension is numseg */
    Uint1Ptr strands;		/* dimension is dim */
    ScorePtr scores;		/* dimension is numseg */
} PackSeg, PNTR PackSegPtr;

NLM_EXTERN PackSegPtr LIBCALL PackSegNew PROTO((void));
NLM_EXTERN Boolean     LIBCALL PackSegAsnWrite PROTO((PackSegPtr psp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN PackSegPtr LIBCALL PackSegAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN PackSegPtr LIBCALL PackSegFree PROTO((PackSegPtr psp));

/*****************************************************************************
*
*   StdSeg
*   
*
*****************************************************************************/
typedef struct stdseg {
    Int2 dim;
    SeqIdPtr ids;    /* SeqId s */
    SeqLocPtr loc;    /* SeqLoc s */
    ScorePtr scores;
    struct stdseg PNTR next;
} StdSeg, PNTR StdSegPtr;

NLM_EXTERN StdSegPtr LIBCALL StdSegNew PROTO((void));
NLM_EXTERN Boolean   LIBCALL StdSegAsnWrite PROTO((StdSegPtr ssp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN StdSegPtr LIBCALL StdSegAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN StdSegPtr LIBCALL StdSegFree PROTO((StdSegPtr ssp));

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
