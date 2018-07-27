/*   mmdbapi2.h
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*            National Center for Biotechnology Information (NCBI)
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government do not place any restriction on its use or reproduction.
*  We would, however, appreciate having the NCBI and the author cited in
*  any work or product based on this material
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
* ===========================================================================
*
* File Name:  mmdbapi2.h
*
* Author:  Christopher Hogue
*
* Version Creation Date:   07/24/95
*
* $Revision: 6.2 $
*
* File Description:  Traversers & Data Lookup
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 95/08/28 C. Hogue    Major change to new Biostruc ASN.1 v4.0,
*                      PDB file generators, Kinemage file moved to 
*                      mmdbapi3.h
*		       Model nodes and Dictionaries altered...
*
*  95/08/30 C. Hogue   Moved globals into mmdbapi2.c
*
*
* $Log: mmdbapi2.h,v $
* Revision 6.2  2000/03/31 22:30:47  lewisg
* fix output of CONECT, create intrabond traverser, misc bugs
*
* Revision 6.1  1999/03/30 22:31:12  ywang
* add GetMGFromMM & GetMMFromMSDBySeqId
*
* Revision 6.0  1997/08/25 18:11:06  madden
* Revision changed to 6.0
*
* Revision 5.4  1997/06/19 18:40:05  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.3  1997/03/19 16:48:40  hogue
* Added fn SeqStringFromMol.
*
 * Revision 5.2  1997/01/31  21:41:12  hogue
 * removed a static function that Denis complained about.
 *
 * Revision 5.1  1997/01/27  21:41:10  hogue
 * Added function to find a chain based on PDB chain code (char)
 *
 * Revision 5.0  1996/05/28  14:02:09  ostell
 * Set to revision 5.0
 *
 * Revision 1.7  1996/05/22  20:43:48  hogue
 * Added new API functions GetFirstGraph, GetNextGraph, GetPrevGraph, GetMainAtom
 *
 * Revision 1.6  1995/10/07  15:31:31  hogue
 * Selenium not an ion...
 *
 * Revision 1.5  1995/09/05  19:11:08  hogue
 * Enlarged virtual bond size for old 1HDS sizes, unknown elements fix.
 *
 * Revision 1.4  1995/08/30  19:36:44  kans
 * numerous changes
 *
 * Revision 1.3  1995/08/28  19:31:53  kans
 * version 4.1
 *
 * Revision 1.1  1995/08/02  17:07:15  kans
 * Initial revision
 *
*
* ==========================================================================
*/
/*****************************************************************************
*
*  mmdbapi2.h
*   
*  UNDER CONSTRUCTION NOTICE.  
*	SUBJECT TO RADICAL CHANGES...
*   
*  programmer C.W.V. Hogue
*  created: 24 JULY 95
*  last mod: 28 AUGUST 95
*****************************************************************************/

#ifndef _MMDBAPI2_
#define _MMDBAPI2_


#define TRAVERSE_ALL 0
#define TRAVERSE_MOLECULE 1
#define TRAVERSE_GRAPH 2
#define TRAVERSE_ATOM 3
#define TRAVERSE_BOND 4
#define TRAVERSE_SOLID  5
#define TRAVERSE_IBOND 6
/* traverse all intraresidue bonds for a structure */
#define TRAVERSE_INTRABOND 7

#define VIRTUAL_BOND_CHOICE 7 

#define VIRTUAL_BOND_AA_MAX 4.5
#define VIRTUAL_BOND_AA_MAX_SQ 20.25

#define VIRTUAL_BOND_NA_MAX 7.8
#define VIRTUAL_BOND_NA_MAX_SQ 60.84
 
#define MAX_NCBIstdaa 26
#define MAX_NCBI4na 17
#define USE_UPPERCASE 1
#define USE_LOWERCASE 2
#define USE_MIXCASE   3
#define USE_FULLNAME  4

#define MAX_ELEMENTS 105
#define MAX_NOT_ION 17
 


/* FUNCTION PROTOTYPES */

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

Int2 LIBCALL TraverseSolids( DValNodePtr pdnModel, Int4 iModel, Int4 iIndex, 
			  Pointer ptr, pNodeFunc pfnCallMe);
Int2 LIBCALL TraverseIntraBonds( DValNodePtr pdnModel, Int4 iModel, Int4 iIndex,
			  Pointer ptr, pNodeFunc pfnCallMe);
Int2 LIBCALL TraverseBonds( DValNodePtr pdnModel, Int4 iModel, Int4 iIndex, 
			  Pointer ptr, pNodeFunc pfnCallMe);
Int2 LIBCALL TraverseIBonds( DValNodePtr pdnModel, Int4 iModel, Int4 iIndex, 
			  Pointer ptr, pNodeFunc pfnCallMe);
Int2 LIBCALL TraverseAtoms( DValNodePtr pdnModel, Int4 iModel, Int4 iIndex, 
			  Pointer ptr, pNodeFunc pfnCallMe);
Int2 LIBCALL TraverseGraphs( DValNodePtr pdnModel, Int4 iModel, Int4 iIndex, 
			  Pointer ptr, pNodeFunc pfnCallMe);
Int2 LIBCALL TraverseMolecules( DValNodePtr pdnModel, Int4 iModel, Int4 iIndex, 
			  Pointer ptr, pNodeFunc pfnCallMe);
Int2 LIBCALL TraverseModels(DValNodePtr pdnModel, Int2 iTraverse, 
     Int4 iIndex, Pointer ptr,  pNodeFunc pfnCallMe);
Int2 LIBCALL TraverseOneModel(DValNodePtr pdnModel, Int2 iTraverse, Int2 iModel, 
			Int4 iIndex, Pointer ptr, pNodeFunc pfnCallMe);
Int2 LIBCALL IndexFromNode(PFB pfbNode);
PMSD LIBCALL ToMSDParent(PFB pfbThis);
CharPtr LIBCALL ParentStrucName(PFB pfbThis);
PMGD LIBCALL GetParentGraph(PFB pfbThis);
CharPtr LIBCALL ParentGraphPDBNo(PFB pfbThis);
CharPtr LIBCALL ParentGraphPDBName(PFB pfbThis);
CharPtr LIBCALL ParentGraphIUPAC1(PFB pfbThis);
Int2 LIBCALL ParentGraphNum(PFB pfbThis);
PMMD LIBCALL GetParentMol(PFB pfbThis);
CharPtr LIBCALL ParentMolName(PFB pfbThis);
Int2 LIBCALL ParentMolNum(PFB pfbThis);
PFB LIBCALL PFBFromVN(ValNodePtr pvnModel);
PFB LIBCALL PFBFromDN(DValNodePtr pdnModel);
DValNodePtr DNFromPFB(PFB pfbThis);
Int2 LIBCALL SetActiveModel(PFB pfbThis, Int2 iModel);
Int2 LIBCALL GetActiveModel(PFB pfbThis);
PALD LIBCALL GetAtomLocs(PMAD pmadThis,  Int2 iModel);
PALD LIBCALL GetLocation(PALD paldList,  Int2 iCoord,  Char cAlt);
NLM_EXTERN Int4 LIBCALL AssignAtomLocId(PDNMS pdnmsThis);
CharPtr LIBCALL ElementName(Int1 iAtomicNo);
FloatLo LIBCALL ElementSize(Int1 iAtomicNo);
CharPtr LIBCALL AminoAcidName(Char cThis, Int2 iRetCode);
CharPtr LIBCALL AminoAcidNameFromIdx(Int2 iIndex, Int2 iRetCode);
void LIBCALLBACK SetBackBone(PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr);
NLM_EXTERN void LIBCALL AssignBackBone(PDNMS pdnmsThis);
FloatLo LIBCALL AtomDistanceSq(PMAD pmadFrom, PMAD pmadTo, Int2 iModel, Char cAlt);
void LIBCALLBACK LinkAlpha(PFB pfbThis,  Int4 iModel,  Int4 iIndex,  Pointer ptr);
void LIBCALLBACK SetVirtualBB(PFB pfbThis,  Int4 iModel,  Int4 iIndex,  Pointer ptr);
NLM_EXTERN void LIBCALL AssignVirtual(PDNMS pdnmsThis);
void LIBCALLBACK SetIons(PFB pfbThis,  Int4 iModel,  Int4 iIndex,  Pointer ptr);
NLM_EXTERN void LIBCALL AssignIons(PDNMS pdnmsThis);
PDNMG LIBCALL GetFirstGraph(PMMD pmmdThis);
PDNMG LIBCALL GetLastGraph(PMMD pmmdThis);
PDNMG LIBCALL GetNextGraph(PDNMG pdnmgThis);
PDNMG LIBCALL GetPrevGraph(PDNMG pdnmgThis);
PMAD LIBCALL GetMainAtom(PDNMG pdnmgThis);
void LIBCALLBACK FindMolByChar(PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr);
PDNMM LIBCALL MolFromPDBChain(PDNMS pdnmsThis,  char c);
CharPtr LIBCALL SeqStringFromMol(PDNMM pdnmmThis);
PMGD LIBCALL GetMGFromMM(PMMD pmmdThis, Int4 iRes);
PMMD LIBCALL GetMMFromMSDBySeqId(PMSD pmsdThis, SeqIdPtr sip);


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
