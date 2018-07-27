/*  objfeat.h
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
* File Name:  objfeat.h
*
* Author:  James Ostell
*   
* Version Creation Date: 4/1/91
*
* $Revision: 6.11 $
*
* File Description:  Object manager interface for module NCBI-SeqFeat
*
* Modifications:  
* --------------------------------------------------------------------------
* Date	   Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* $Log: objfeat.h,v $
* Revision 6.11  2002/06/03 17:43:48  kans
* added locus_tag to GeneRef
*
* Revision 6.10  2001/10/02 21:47:54  kans
* added hooks for writing extra descriptors or features (JO)
*
* Revision 6.9  2000/09/28 11:40:19  ostell
* added endogenous_virus (19) to MolInfo.genome
*
* Revision 6.8  2000/01/16 19:18:32  kans
* added specialCleanupFlag field to SeqFeatXref structure
*
* Revision 6.7  1999/12/14 17:30:39  ostell
* changed protoplast to proplastid
*
* Revision 6.6  1999/10/06 18:45:27  kans
* fixed nested comment warning
*
* Revision 6.5  1999/10/05 20:23:17  kans
* fixed typo
*
* Revision 6.4  1999/10/05 19:05:45  ostell
* added comments on BioSource.genome
*
* Revision 6.3  1999/09/27 17:48:37  kans
* using GatherIndex structure
*
* Revision 6.2  1999/09/24 23:09:23  kans
* adds EXTRA_OBJMGR_FIELDS to several objects
*
* Revision 6.1  1998/06/29 20:31:30  kans
* explicit defines for FEATDEF_MAX, SEQFEAT_MAX, and SEQDESCR_MAX Boolean array sizes
*
* Revision 6.0  1997/08/25 18:49:53  madden
* Revision changed to 6.0
*
* Revision 4.3  1997/08/20 19:17:06  madden
* Version 6 of ASN.1
*
* Revision 4.2  1997/06/19 18:41:18  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 4.1  1996/05/20 19:43:50  ostell
* added prot-ref.processed, virion, div
*
 * Revision 4.0  1995/07/26  13:48:06  ostell
 * force revision to 4.0
 *
 * Revision 3.6  1995/07/12  22:02:07  ostell
 * added comment on OrgRef.db
 *
 * Revision 3.5  1995/07/11  18:46:25  ostell
 * added dbxref to Seqfeat
 * added anticodon to tRNA
 *
 * Revision 3.4  1995/05/15  21:22:00  ostell
 * added Log line
 *
*
*
* ==========================================================================
*/

#ifndef _NCBI_Seqfeat_
#define _NCBI_Seqfeat_

#ifndef _ASNTOOL_
#include <asn.h>
#endif
#ifndef _NCBI_General_
#include <objgen.h>
#endif
#ifndef _NCBI_Seqloc_
#include <objloc.h>
#endif
#ifndef _NCBI_Pub_
#include <objpub.h>
#endif
#ifndef _NCBI_Pubdesc_
#include <objpubd.h>
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
NLM_EXTERN Boolean LIBCALL SeqFeatAsnLoad PROTO((void));

/*****************************************************************************
*
*   GBQual
*
*****************************************************************************/
typedef struct gbqual {
    CharPtr qual,
        val;
    struct gbqual PNTR next;
} GBQual, PNTR GBQualPtr;

NLM_EXTERN GBQualPtr LIBCALL GBQualNew PROTO((void));
NLM_EXTERN Boolean   LIBCALL GBQualAsnWrite PROTO((GBQualPtr gbp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN GBQualPtr LIBCALL GBQualAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN GBQualPtr LIBCALL GBQualFree PROTO((GBQualPtr gbp));

/*****************************************************************************
*
*   SeqFeatXref
*   	cross references between features
*
*****************************************************************************/
typedef struct seqfeatxref {
    Choice id;      
    Choice data;
    struct seqfeatxref PNTR next;
    Boolean specialCleanupFlag; /* marks converted gbquals for extra checking within SeriousSeqEntryCleanup */
} SeqFeatXref, PNTR SeqFeatXrefPtr;

NLM_EXTERN SeqFeatXrefPtr LIBCALL SeqFeatXrefNew PROTO((void));
NLM_EXTERN Boolean        LIBCALL SeqFeatXrefAsnWrite PROTO((SeqFeatXrefPtr sfxp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqFeatXrefPtr LIBCALL SeqFeatXrefAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqFeatXrefPtr LIBCALL SeqFeatXrefFree PROTO((SeqFeatXrefPtr sfxp));
				   /* free frees whole chain of SeqFeatXref */
/*****************************************************************************
*
*   SeqFeat
*     Feat-id is built into idtype/id
*       1=gibb (in id.intvalue)
*       2=gimm (id.ptrvalue)
*       3=local (id.ptrvalue to Object-id)
*       4=general (id.ptrvalue to Dbtag)
*     SeqFeatData is built into datatype/data
*       datatype gives type of SeqFeatData:
*   0 = not set
    1 = gene, data.value.ptrvalue = Gene-ref ,
    2 = org , data.value.ptrvalue = Org-ref ,
    3 = cdregion, data.value.ptrvalue = Cdregion ,
    4 = prot , data.value.ptrvalue = Prot-ref ,
    5 = rna, data.value.ptrvalue = RNA-ref ,
    6 = pub, data.value.ptrvalue = Pubdesc , -- publication applies to this seq
    7 = seq, data.value.ptrvalue = Seq-loc ,  -- for tracking source of a seq.
    8 = imp, data.value.ptrvalue = Imp-feat ,
    9 = region, data.value.ptrvalue= VisibleString,      -- for a name
    10 = comment, data.value.ptrvalue= NULL ,             -- just a comment
    11 = bond, data.value.intvalue = ENUMERATED {
        disulfide (1) ,
        thiolester (2) ,
        xlink (3) ,
        other (255) } ,
    12 = site, data.value.intvalue = ENUMERATED {
        active (1) ,
        binding (2) ,
        cleavage (3) ,
        inhibit (4) ,
        modified (5),
        other (255) } ,
    13 = rsite, data.value.ptrvalue = Rsite-ref
    14 = user, data.value.ptrvalue = UserObjectPtr
    15 = txinit, data.value.ptrvalue = TxinitPtr
	16 = num, data.value.ptrvalue = NumberingPtr   -- a numbering system
	17 = psec-str data.value.intvalue = ENUMERATED {   -- protein secondary structure
		helix (1) ,         -- any helix
		sheet (2) ,         -- beta sheet
		turn  (3) } ,       -- beta or gamma turn
	18 = non-std-residue data.value.ptrvalue = VisibleString ,  -- non-standard residue here in seq
	19 = het data.value.ptrvalue=CharPtr Heterogen   -- cofactor, prosthetic grp, etc, bound to seq
	20 = biosrc, data.value.ptrvalue = BioSource
*   
*
*****************************************************************************/
typedef struct seqfeat {
    Choice id;      
    Choice data;
    Boolean partial ,
        excpt;         /* except changed to excpt (Win32 reserved word) */
    CharPtr comment;
    ValNodePtr product ,
        location;
    GBQualPtr qual;
    CharPtr title;
    UserObjectPtr ext;
    ValNodePtr cit;       /* citations (Pub-set)  */
	Uint1 exp_ev;
	SeqFeatXrefPtr xref;
	ValNodePtr dbxref;    /* each vnp->data.ptrvalue is a DbtagPtr */
	Boolean pseudo;      /* pseudogene feature ? */
	CharPtr except_text;   /* explanation of biological exception */
    struct seqfeat PNTR next;
	GatherIndex idx;      /* internal gather/objmgr tracking fields */
} SeqFeat, PNTR SeqFeatPtr;

NLM_EXTERN SeqFeatPtr LIBCALL SeqFeatNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL SeqFeatAsnWrite PROTO((SeqFeatPtr anp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqFeatPtr LIBCALL SeqFeatAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SeqFeatPtr LIBCALL SeqFeatFree PROTO((SeqFeatPtr anp));

/**********************************************
*
*  the SeqFeat Labelling functions are quite complex and are all in
*   objfdef.[ch]. See FeatDefLabel()
*
************************************************/

     /* get a SeqFeatXref from a feature.  Currently only Prot-ref and */
     /* Gene-ref are supported */

NLM_EXTERN SeqFeatXrefPtr LIBCALL SeqFeatToXref PROTO((SeqFeatPtr sfp));

#define SEQFEAT_GENE	1
#define SEQFEAT_ORG		2
#define SEQFEAT_CDREGION	3
#define SEQFEAT_PROT	4
#define SEQFEAT_RNA		5
#define SEQFEAT_PUB		6
#define SEQFEAT_SEQ		7
#define SEQFEAT_IMP		8
#define SEQFEAT_REGION	9
#define SEQFEAT_COMMENT	10
#define SEQFEAT_BOND	11
#define SEQFEAT_SITE	12
#define SEQFEAT_RSITE	13
#define SEQFEAT_USER	14
#define SEQFEAT_TXINIT	15
#define SEQFEAT_NUM		16
#define SEQFEAT_PSEC_STR	17
#define SEQFEAT_NON_STD_RESIDUE	18
#define SEQFEAT_HET		19
#define SEQFEAT_BIOSRC	20

#define SEQFEAT_MAX 21 /* size of array needed for seqfeat filter parameters */

/*****************************************************************************
*
*   SeqFeatId - used as parts of other things, so is not allocated itself
*
*****************************************************************************/
NLM_EXTERN void    LIBCALL SeqFeatIdFree PROTO((ChoicePtr cp));  /* does NOT free cp itself */
NLM_EXTERN Boolean LIBCALL SeqFeatIdAsnWrite PROTO((ChoicePtr cp, AsnIoPtr aip, AsnTypePtr orig));
NLM_EXTERN Boolean LIBCALL SeqFeatIdAsnRead PROTO((AsnIoPtr aip, AsnTypePtr orig, ChoicePtr cp));
       /** NOTE: SeqFeatIdAsnRead() does NOT allocate cp ***/
NLM_EXTERN Boolean LIBCALL SeqFeatIdDup PROTO((ChoicePtr dest, ChoicePtr src));

/*****************************************************************************
*
*   SeqFeatData - used as parts of other things, so is not allocated itself
*
*****************************************************************************/
NLM_EXTERN void    LIBCALL SeqFeatDataFree PROTO((ChoicePtr cp));  /* does NOT free cp itself */
NLM_EXTERN Boolean LIBCALL SeqFeatDataAsnWrite PROTO((ChoicePtr cp, AsnIoPtr aip, AsnTypePtr orig));
NLM_EXTERN Boolean LIBCALL SeqFeatDataAsnRead PROTO((AsnIoPtr aip, AsnTypePtr orig, ChoicePtr cp));
       /** NOTE: SeqFeatDataAsnRead() does NOT allocate cp ***/

/*****************************************************************************
*
*   SeqFeatSet - sets of seqfeats
*
*****************************************************************************/
NLM_EXTERN Boolean    LIBCALL SeqFeatSetAsnWrite PROTO((SeqFeatPtr anp, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN SeqFeatPtr LIBCALL SeqFeatSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));

/** used only SeqAnnotAsnWriteExtra **/
NLM_EXTERN Boolean    LIBCALL SeqFeatSetAsnWriteExtra PROTO((SeqFeatPtr anp, AsnIoPtr aip,
					AsnTypePtr set, AsnTypePtr element, ValNodePtr extras));
/*****************************************************************************
*
*   CodeBreak
*
*****************************************************************************/
typedef struct cb {
    SeqLocPtr loc;          /* the Seq-loc */
    Choice aa;              /* 1=ncbieaa, 2=ncbi8aa, 3=ncbistdaa */
    struct cb PNTR next;
} CodeBreak, PNTR CodeBreakPtr;

NLM_EXTERN CodeBreakPtr LIBCALL CodeBreakNew PROTO((void));
NLM_EXTERN Boolean      LIBCALL CodeBreakAsnWrite PROTO((CodeBreakPtr cbp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN CodeBreakPtr LIBCALL CodeBreakAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN CodeBreakPtr LIBCALL CodeBreakFree PROTO((CodeBreakPtr cbp));

/*****************************************************************************
*
*   CdRegion
*
*****************************************************************************/
typedef struct cdregion {
    Boolean orf;
    Uint1 frame;
    Boolean conflict;
    Uint1 gaps,                         /* 255 = any number > 254 */
        mismatch,
        stops;
    ValNodePtr genetic_code;                 /* NULL = not set */
    CodeBreakPtr code_break;
} CdRegion, PNTR CdRegionPtr;

NLM_EXTERN CdRegionPtr LIBCALL CdRegionNew PROTO((void));
NLM_EXTERN Boolean     LIBCALL CdRegionAsnWrite PROTO((CdRegionPtr cdp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN CdRegionPtr LIBCALL CdRegionAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN CdRegionPtr LIBCALL CdRegionFree PROTO((CdRegionPtr cdp));

/*****************************************************************************
*
*   GeneticCode
*
*      ncbieaa, ncbi8aa, ncbistdaa
*      are arrays 64 cells long, where each cell gives the aa produced
*      by triplets coded by T=0, C=1, A=2, G=3
*      TTT = cell[0]
*      TTC = cell[1]
*      TTA = cell[2]
*      TTG = cell[3]
*      TCT = cell[4]
*      ((base1 * 16) + (base2 * 4) + (base3)) = cell in table
*
*      sncbieaa, sncbi8aa, sncbistdaa
*   	are arrays same as above, except the AA's they code for are only for
*   	the first AA of a peptide.  This accomdates alternate start codes.
*       If a codon is not a valid start, the cell contains the "gap" symbol
*       instead of an AA.
*
*      in both cases, IUPAC cannot be used because it has no symbol for
*       stop.
*   	
*
*   GeneticCode is a ValNodePtr so variable numbers of elements are
*   	easily accomodated.  A ValNodePtr with choice = 254 is the head
*       of the list.  It's elements are a chain of ValNodes beginning with
*       the data.ptrvalue of the GeneticCode (head).  GeneticCodeNew()
*       returns the head.
*   
*   Types in ValNodePtr->choice are:
*   	0 = not set
*   	1 = name (CharPtr in ptrvalue)
*   	2 = id	(in intvalue)
*   	3 = ncbieaa (CharPtr in ptrvalue)
*   	4 = ncbi8aa (ByteStorePtr in ptrvalue)
*   	5 = ncbistdaa (ByteStorePtr in ptrvalue)
*   	6 = sncbieaa (CharPtr in ptrvalue)
*   	7 = sncbi8aa (ByteStorePtr in ptrvalue)
*   	8 = sncbistdaa (ByteStorePtr in ptrvalue)
*   	255 = read unrecognized type, but passed ASN.1
*   
*****************************************************************************/
typedef ValNode GeneticCode, FAR *GeneticCodePtr;

NLM_EXTERN GeneticCodePtr LIBCALL GeneticCodeNew PROTO((void));
NLM_EXTERN Boolean        LIBCALL GeneticCodeAsnWrite PROTO((GeneticCodePtr gcp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN GeneticCodePtr LIBCALL GeneticCodeAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN GeneticCodePtr LIBCALL GeneticCodeFree PROTO((GeneticCodePtr gcp));

NLM_EXTERN Boolean        LIBCALL GeneticCodeTableAsnWrite PROTO((GeneticCodePtr gcp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN GeneticCodePtr LIBCALL GeneticCodeTableAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));

NLM_EXTERN GeneticCodePtr LIBCALL GeneticCodeFind PROTO((Int4 id, CharPtr name));
NLM_EXTERN GeneticCodePtr LIBCALL GeneticCodeTableLoad PROTO((void));

/*****************************************************************************
*
*   ImpFeat
*
*****************************************************************************/
typedef struct impfeat {
    CharPtr key,
        loc,
        descr;
} ImpFeat, PNTR ImpFeatPtr;

NLM_EXTERN ImpFeatPtr LIBCALL ImpFeatNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL ImpFeatAsnWrite PROTO((ImpFeatPtr ifp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN ImpFeatPtr LIBCALL ImpFeatAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN ImpFeatPtr LIBCALL ImpFeatFree PROTO((ImpFeatPtr ifp));

/*****************************************************************************
*
*   RnaRef
*    Choice used for extensions
*      0 = no extension
*      1 = name, ext.value.ptrvalue = CharPtr
*      2 = trna, ext.value.ptrvalue = tRNA
*
*****************************************************************************/
typedef struct rnaref {
    Uint1 type;
    Boolean pseudo;
    Choice ext;
} RnaRef, PNTR RnaRefPtr;

NLM_EXTERN RnaRefPtr LIBCALL RnaRefNew PROTO((void));
NLM_EXTERN Boolean   LIBCALL RnaRefAsnWrite PROTO((RnaRefPtr rrp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN RnaRefPtr LIBCALL RnaRefAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN RnaRefPtr LIBCALL RnaRefFree PROTO((RnaRefPtr rrp));

/*****************************************************************************
*
*   tRNA
*
*****************************************************************************/
typedef struct trna {
    Uint1 aatype,  /* 0=not set, 1=iupacaa, 2=ncbieaa, 3=ncbi8aa 4=ncbistdaa */
        aa;        /* the aa transferred in above code */
    Uint1 codon[6];    /* codons recognized, coded as for Genetic-code */
	SeqLocPtr anticodon;  /* location of anticodon */
} tRNA, PNTR tRNAPtr;   /*  0-63 = codon,  255=no data in cell */

/*****************************************************************************
*
*   GeneRef
*
*****************************************************************************/
typedef struct generef {
    CharPtr locus,
        allele,
        desc,
        maploc;
    Boolean pseudo;
    ValNodePtr db;          /* ids in other databases */
    ValNodePtr syn;         /* synonyms for locus */
    CharPtr locus_tag;      /* systematic gene name */
} GeneRef, PNTR GeneRefPtr;

NLM_EXTERN GeneRefPtr LIBCALL GeneRefNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL GeneRefAsnWrite PROTO((GeneRefPtr grp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN GeneRefPtr LIBCALL GeneRefAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN GeneRefPtr LIBCALL GeneRefFree PROTO((GeneRefPtr grp));
NLM_EXTERN GeneRefPtr LIBCALL GeneRefDup PROTO((GeneRefPtr grp));

/*****************************************************************************
*
*   OrgRef
*
*****************************************************************************/
typedef struct taxelement {
	Uint1 fixed_level;  /* controlled levels, 0=other,1=family,2=order,3=class */
	CharPtr level,		/* level name, if "other" */
		name;			/* tax name at this level */
	struct taxelement PNTR next;
} TaxElement, PNTR TaxElementPtr;

NLM_EXTERN TaxElementPtr LIBCALL TaxElementNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL TaxElementAsnWrite PROTO((TaxElementPtr tep, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN TaxElementPtr LIBCALL TaxElementAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN TaxElementPtr LIBCALL TaxElementFree PROTO((TaxElementPtr tep));

NLM_EXTERN Boolean    LIBCALL TaxElementSetAsnWrite PROTO((TaxElementPtr tep, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN TaxElementPtr LIBCALL TaxElementSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN TaxElementPtr LIBCALL TaxElementSetFree PROTO((TaxElementPtr tep));

typedef struct binomialorgname {
	CharPtr genus,
		species,
		subspecies;
} BinomialOrgName, PNTR BinomialOrgNamePtr;

NLM_EXTERN BinomialOrgNamePtr LIBCALL BinomialOrgNameNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL BinomialOrgNameAsnWrite PROTO((BinomialOrgNamePtr bop, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN BinomialOrgNamePtr LIBCALL BinomialOrgNameAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN BinomialOrgNamePtr LIBCALL BinomialOrgNameFree PROTO((BinomialOrgNamePtr bop));

typedef struct orgmod {
	Uint1 subtype;
	CharPtr subname,
		attrib;
	struct orgmod PNTR next;
} OrgMod, PNTR OrgModPtr;

NLM_EXTERN OrgModPtr LIBCALL OrgModNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL OrgModAsnWrite PROTO((OrgModPtr omp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN OrgModPtr LIBCALL OrgModAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN OrgModPtr LIBCALL OrgModFree PROTO((OrgModPtr omp));

NLM_EXTERN Boolean    LIBCALL OrgModSetAsnWrite PROTO((OrgModPtr omp, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN OrgModPtr LIBCALL OrgModSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN OrgModPtr LIBCALL OrgModSetFree PROTO((OrgModPtr omp));

typedef struct orgname {
	Uint1 choice;  /* 1=binomial, 2=virus, 3=hybrid, 4=namedhybrid, 5=partial */
	Pointer data;  /* depends on choice */
	CharPtr attrib;  /* attribution for this name */
	OrgModPtr mod;   /* OrgMods */
	CharPtr lineage;  /* lineage to this org */
	Uint1 gcode,      /* genetic code using GenBank keys */
		  mgcode;      /* mitochondrial genetic code using GenBank keys..0=none */
	CharPtr div;       /* GenBank division code */
	struct orgname PNTR next;   /* for MultiOrgName */
} OrgName, PNTR OrgNamePtr;

NLM_EXTERN OrgNamePtr LIBCALL OrgNameNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL OrgNameAsnWrite PROTO((OrgNamePtr onp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN OrgNamePtr LIBCALL OrgNameAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN OrgNamePtr LIBCALL OrgNameFree PROTO((OrgNamePtr onp));

NLM_EXTERN Boolean    LIBCALL OrgNameSetAsnWrite PROTO((OrgNamePtr onp, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN OrgNamePtr LIBCALL OrgNameSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN OrgNamePtr LIBCALL OrgNameSetFree PROTO((OrgNamePtr onp));


typedef struct orgref {
    CharPtr taxname,		/* preferred full formal name */
        common;				/* preferred common name */
    ValNodePtr mod;			/* unstructured modifiers (Obsolete) */
    ValNodePtr db;          /* ids in other databases (set of Dbtag pointers)*/
    ValNodePtr syn;         /* synonyms for taxname and/or common */
	OrgNamePtr orgname;     /* structured names and components */
} OrgRef, PNTR OrgRefPtr;

NLM_EXTERN OrgRefPtr LIBCALL OrgRefNew PROTO((void));
NLM_EXTERN Boolean   LIBCALL OrgRefAsnWrite PROTO((OrgRefPtr orp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN OrgRefPtr LIBCALL OrgRefAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN OrgRefPtr LIBCALL OrgRefFree PROTO((OrgRefPtr orp));

/*****************************************************************************
*
*   BioSource
*
*****************************************************************************/

typedef struct subsource {
	Uint1 subtype;
	CharPtr name,
		attrib;
	struct subsource PNTR next;
} SubSource, PNTR SubSourcePtr;

NLM_EXTERN SubSourcePtr LIBCALL SubSourceNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL SubSourceAsnWrite PROTO((SubSourcePtr ssp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SubSourcePtr LIBCALL SubSourceAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN SubSourcePtr LIBCALL SubSourceFree PROTO((SubSourcePtr ssp));

NLM_EXTERN Boolean    LIBCALL SubSourceSetAsnWrite PROTO((SubSourcePtr ssp, AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN SubSourcePtr LIBCALL SubSourceSetAsnRead PROTO((AsnIoPtr aip, AsnTypePtr set, AsnTypePtr element));
NLM_EXTERN SubSourcePtr LIBCALL SubSourceSetFree PROTO((SubSourcePtr ssp));

/******************************
* BioSource
    current values for genome are: */
    /* 0 unknown */
    /* 1 genomic */
    /* 2 chloroplast */
    /* 3 chromoplast */
    /* 4 kinteoplast */
    /* 5 mitochondrion */
    /* 6 plastid */
    /* 7 macronuclear */
    /* 8 extrachrom */
    /* 9 plasmid */
    /* 10 transposon */
    /* 11 insertion_seq */
    /* 12 cyanelle */
    /* 13 proviral */
    /* 14 virion */
        /* Below are new in ASN.1 spec */
    /* 15 nucleomorph */
    /* 16 apicoplast */
    /* 17 leucoplast */
    /* 18 proplastid */
    /* 19 endogenous_virus */

typedef struct biosource {
	Uint1 genome,
		origin;
	OrgRefPtr org;
	SubSourcePtr subtype;
	Boolean is_focus;         /* is main BioSource for molecule (when multiple) */
} BioSource, PNTR BioSourcePtr;

NLM_EXTERN BioSourcePtr LIBCALL BioSourceNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL BioSourceAsnWrite PROTO((BioSourcePtr bsp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN BioSourcePtr LIBCALL BioSourceAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN BioSourcePtr LIBCALL BioSourceFree PROTO((BioSourcePtr bsp));

/*****************************************************************************
*
*   ProtRef
*
*****************************************************************************/
typedef struct protref {
    ValNodePtr name;
    CharPtr desc;
    ValNodePtr ec,
        activity;
    ValNodePtr db;          /* ids in other databases */
	Uint1 processed;        /* 0=not-set, 1=preprotein, 2=mature protein, 3=signal peptide,
	                            4=transit peptide */
} ProtRef, PNTR ProtRefPtr;

NLM_EXTERN ProtRefPtr LIBCALL ProtRefNew PROTO((void));
NLM_EXTERN Boolean    LIBCALL ProtRefAsnWrite PROTO((ProtRefPtr orp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN ProtRefPtr LIBCALL ProtRefAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN ProtRefPtr LIBCALL ProtRefFree PROTO((ProtRefPtr orp));
NLM_EXTERN ProtRefPtr LIBCALL ProtRefDup PROTO((ProtRefPtr orp));

/*****************************************************************************
*
*   RsiteRef
*       uses an ValNode
*       choice = 1 = str
*                2 = db
*
*****************************************************************************/
typedef ValNode RsiteRef, FAR *RsiteRefPtr;

NLM_EXTERN Boolean     LIBCALL RsiteRefAsnWrite PROTO((RsiteRefPtr orp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN RsiteRefPtr LIBCALL RsiteRefAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN RsiteRefPtr LIBCALL RsiteRefFree PROTO((RsiteRefPtr orp));

/*****************************************************************************
*
*   Txinit
*       Transcription initiation site
*
*****************************************************************************/
typedef struct txevidence {
    Uint1 exp_code ,
        exp_sys ;
    Boolean low_prec_data ,
        from_homolog;
    struct txevidence PNTR next;
} TxEvidence, PNTR TxEvidencePtr;

typedef struct txinit {
    CharPtr name;
    ValNodePtr syn ,
        gene ,
        protein ,
        rna ;
    CharPtr expression;
    Uint1 txsystem;
    CharPtr txdescr;
    OrgRefPtr txorg;
    Boolean mapping_precise,
        location_accurate;
    Uint1 inittype;              /* 255 if not set */
    TxEvidencePtr evidence;
} Txinit, PNTR TxinitPtr;

NLM_EXTERN TxinitPtr LIBCALL TxinitNew PROTO((void));
NLM_EXTERN Boolean   LIBCALL TxinitAsnWrite PROTO((TxinitPtr txp, AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN TxinitPtr LIBCALL TxinitAsnRead PROTO((AsnIoPtr aip, AsnTypePtr atp));
NLM_EXTERN TxinitPtr LIBCALL TxinitFree PROTO((TxinitPtr txp));


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

