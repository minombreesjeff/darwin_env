#ifndef _TOASN3_
#define _TOASN3_

/*  $Id: toasn3.h,v 6.10 2001/10/23 12:48:31 kans Exp $
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
 * Author:  
 *
 * File Description:
 *   
 *
 * ===========================================================================
 */


#include <ncbi.h>
#include <seqport.h>


#ifdef __cplusplus
extern "C" {
#endif


#define INFO_ASNOLD 0
#define INFO_ASNNEW 1
#define ERR_REJECT  2
#define ERR_INPUT   4


typedef struct {
	ValNodePtr	pept;
	ValNodePtr	cds;
} SeqFeatArr, PNTR SeqFeatArrPtr;

typedef struct {
	CharPtr name;
	Uint1   num;
} ORGMOD;

typedef struct {
	BioseqSetPtr bioset;
	ValNodePtr   list;
} PubList, PNTR PubListPtr;

typedef struct {
	Boolean first;
	ValNodePtr   list;
} PubSetList, PNTR PubSetListPtr;

typedef struct qualmap{
	CharPtr name;
	Boolean same;
} QualMap, PNTR QualMapPtr;

typedef struct bsmap{
	BioSourcePtr bsp;
	Boolean same;
} BSMap, PNTR BSMapPtr;

typedef struct orgfix {
	SeqEntryPtr 	contains;
	Boolean 		desc;               /* descriptor containing org-ref */
	SeqFeatPtr 		sfp;                /* or feature containing the org-ref */
	SeqFeatPtr 		imp;                /* ImpFeat source */
	OrgRefPtr 		orp;
	ValNodePtr		modif;
	BioSourcePtr 	bsp;
	Int4 			index;
	struct orgfix PNTR next;
} OrgFix, PNTR OrgFixPtr;

typedef struct molfix {
	SeqEntryPtr		contains;
	Uint1 			mol;               		/*  mol_type */
	ValNodePtr 		modif;               /* descriptor containing modif */
	Uint1 			method;               	/*  method */
	MolInfoPtr 		molinfo;
	Int4 			index;
	struct molfix PNTR next;
} MolFix, PNTR MolFixPtr;

typedef struct toasn3 {
	OrgFixPtr ofp;
	MolFixPtr mfp;
	Boolean had_biosource;
	Boolean had_molinfo;
} ToAsn3, PNTR ToAsn3Ptr;

Int4 ToAsn4(SeqEntryPtr sep);
Int4 SeqEntryPubsAsn4(SeqEntryPtr sep);
Int4 SeqEntryPubsAsn3(SeqEntryPtr sep);
Int4 SeqEntryToAsn3(SeqEntryPtr sep, Boolean strip, Boolean correct, Boolean taxserver, SeqEntryFunc taxfun);
Int4 SeqEntryToAsn3Ex(SeqEntryPtr sep, Boolean strip, Boolean correct, Boolean taxserver, SeqEntryFunc taxfun, SeqEntryFunc taxmerge);
Int2 seq_loc_compare( SeqLocPtr a, SeqLocPtr b);
void compare_quals(GBQualPtr PNTR qual1, GBQualPtr PNTR qual2);
Boolean feat_join(SeqFeatPtr f1, SeqFeatPtr f2, SeqFeatPtr head);
void count_join(SeqFeatPtr f1, SeqFeatPtr f2);
void CountSourceFeat(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void CorrectSourceFeat(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);

Boolean CheckLocWhole(BioseqPtr bsp, SeqLocPtr slp);
void FindOrg(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void CheckQuals(BioSourcePtr bsp, GBQualPtr sfp);
void CheckQualsWithComm(BioSourcePtr bsp, SeqFeatPtr sfp);
MolInfoPtr new_info(MolInfoPtr mfi);
MolInfoPtr ModToMolInfo(MolInfoPtr mfi, Uint1 mod);
void ModToBiosource(BioSourcePtr bsp, Uint1 mod);
void StripOld(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void CkOrg(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
void MergeBSinDescr(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
Int4 FixNucProtSet(SeqEntryPtr sep);
void CheckBS(SeqEntryPtr sep, Pointer data, Int4 index, Int2 indent);
Int4 BSComparison(BioSourcePtr one, BioSourcePtr two);
Int4 BSComparisonEx(BioSourcePtr one, BioSourcePtr two, Boolean clone);
Int2 BioSourceToGeneticCode(BioSourcePtr biop);
ValNodePtr GetMultBiosource(SeqEntryPtr sep);

/****************************************************
* Does a SeqEntryExplore, calling ImpFeatToCdregion() on
*  each feature table found
*
****************************************************/
void EntryChangeImpFeat(SeqEntryPtr sep);

/****************************************************
*  Changes a SeqFeat of type Imp-feat CDS to a real
*  CdRegion in place (sfp does not change)
*  returns TRUE if the change happened
*  returns FALSE if no changes were made
*    (so also returns FALSE if not an Imp-feat of type CDS)
*
*****************************************************/
Boolean ImpFeatToCdregion(SeqFeatPtr sfp);

void CdCheck(SeqEntryPtr sep, FILE *fp);

/****************************************************
*  Creates source string from BioSource structure
*  Compare with GBBlock.source
*  deletes GBBlock.source if it's the same as from BioSource
*
*****************************************************/
void EntryChangeGBSource(SeqEntryPtr sep);

/****************************************************
*  Finds multiple biosource descriptors on the same chain with
*  the same taxonomic name, moves subsource, orgmod, and some
*  other blocks, conservatively, deletes second biosource
*
*****************************************************/
void EntryMergeDupBioSources(SeqEntryPtr sep);

/****************************************************
*  Checks GBBlock.source, .taxonomy, and .div, removes any empty
*  GBBlock descriptors, and returns TRUE if information (other
*  than PAT or SYN division) exists in the these fields
*
*****************************************************/
Boolean EntryCheckGBBlock(SeqEntryPtr sep);

void EntryChangeImpFeatToProt(SeqEntryPtr sep);

void CombineBSFeat(SeqEntryPtr sep);
void ChangeReplaceToQual(SeqFeatPtr sfp);
void AddReplaceQual(SeqFeatPtr sfp, CharPtr p);
Boolean SeqEntryMoveDbxrefs (SeqEntryPtr sep);

/* functions moved from Sequin */

void NormalizePeriodsOnInitials (SeqEntryPtr sep);
void MoveRnaGBQualProductToName (SeqEntryPtr sep);
void MoveProtGBQualProductToName (SeqEntryPtr sep);
void MoveCdsGBQualProductToName (SeqEntryPtr sep);
void MoveFeatGBQualsToFields (SeqEntryPtr sep);
void StripTitleFromProtsInNucProts (SeqEntryPtr sep);

void GetRidOfEmptyFeatsDescCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent);

/* from move_cds (S. Bazhin) */

Uint2 move_cds(SeqEntryPtr sep);
Uint2 move_cds_ex (SeqEntryPtr sep, Boolean doPseudo);

/* more functions moved from Sequin, placed in toporg.c */

extern void CleanUpPseudoProducts (Uint2 entityID, SeqEntryPtr sep);
extern void CleanupGenbankCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent);
extern void MergeAdjacentAnnotsCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent);
extern void CleanupEmptyFeatCallback (SeqEntryPtr sep, Pointer mydata, Int4 index, Int2 indent);
extern void RemoveBioSourceOnPopSet (SeqEntryPtr sep, OrgRefPtr master);
extern Boolean NoBiosourceOrTaxonId (SeqEntryPtr sep);
extern void ExtendGeneFeatIfOnMRNA (Uint2 entityID, SeqEntryPtr sep);
extern void ConvertFullLenSourceFeatToDesc (SeqEntryPtr sep);
extern void ConvertFullLenPubFeatToDesc (SeqEntryPtr sep);

/* SeriousSeqEntryCleanup combines many of the above cleanups */

extern void SeriousSeqEntryCleanup (SeqEntryPtr sep, SeqEntryFunc taxfun, SeqEntryFunc taxmerge);

/* SeriousSeqEntryCleanupBulk does not clean up pseudo products, instead posting a message */

extern void SeriousSeqEntryCleanupBulk (SeqEntryPtr sep);


#ifdef __cplusplus
}
#endif

#endif
