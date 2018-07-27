/*   mmdbapi3.h
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
* File Name:  mmdbapi3.h
*
* Author:  Christopher Hogue
*
* Version Creation Date:   08/06/95
*
* $Revision: 6.1 $
*
* File Description: Contains FlatFile Generators for PDB & Kinemage
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 08/30/95 C. Hogue    Moved globals out - into mmdbapi3.c
*
* $Log: mmdbapi3.h,v $
* Revision 6.1  2000/05/09 19:51:02  lewisg
* add new blast header to file>properties
*
* Revision 6.0  1997/08/25 18:11:13  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/06/19 18:40:07  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.2  1997/01/27 21:43:48  hogue
* Added Code to write FASTA reports and FASTA files from biopolymer molecules
*
 * Revision 5.1  1996/07/22  00:29:32  hogue
 * Added a WritePDBRemarks function for PDB file generation.
 *
 * Revision 5.0  1996/05/28  14:02:09  ostell
 * Set to revision 5.0
 *
 * Revision 1.3  1995/09/19  21:08:32  hogue
 * Added WriteStructSummary()
 *
 * Revision 1.2  1995/08/30  19:36:44  kans
 * numerous changes
 *
 * Revision 1.1  1995/08/28  19:30:14  kans
 * Initial revision
 *

*
* ==========================================================================
*/
/*****************************************************************************
*
*  mmdbapi3.h
*   
*  UNDER CONSTRUCTION NOTICE.  
*	SUBJECT TO RADICAL CHANGES...
*   
*  programmer C.W.V. Hogue
*  created: 6 AUGUST 95
*  last mod: 28 AUGUST 95
*****************************************************************************/

#ifndef _MMDBAPI3_
#define _MMDBAPI3_

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* GLOBAL VARIABLES */

/*#define            0x80 */ 
#define KIN_MULTANIM    0x20   
#define KIN_ALTCONF     0x10  
#define KIN_HET         0x08   /* default */
#define KIN_RESIDUE     0x04 
#define KIN_BACKBONE    0x02   
#define KIN_VIRTUAL     0x01   /* default */
#define KIN_DEFAULT     0x00
#define KIN_MASK_ALT    0xEF  /* not with MULTANIM */

#define KIN_COLOR_DEFAULT 0
#define KIN_COLOR_NUMBER  1  /* default & forced with MULTANIM */
#define KIN_COLOR_TYPE    2 
#define KIN_COLOR_TEMP    3  /* forces Temp groups only */ 
#define KIN_COLOR_ATOM    4 /* forces Element groups only */

 
#define KIN_COLOR_THERM 16
#define KIN_COLOR_MAX 22
#define KIN_COLOR_NUM 9
#define MAX_KIN_ATOMS 6


/* FUNCTION PROTOTYPES */


void LIBCALLBACK WriteAtomOrHet(PFB pfbThis, Int4 iModel,  Int4 iIndex,  Pointer ptr);
CharPtr LIBCALL AuthorListPDB(BiostrucPtr pbsThis);
Int2 LIBCALL WritePDBModel(Int2 iModel, Int2 iManyModels,  PDNMS pdnmsThis, FILE *pFile);
void LIBCALL WritePDBHeader(PDNMS pdnmsThis,  FILE *pFile);
void LIBCALL WritePDBMotifs(PDNMS pdnmsThis,  FILE *pFile);
NLM_EXTERN void LIBCALL WritePDBRemarks(PDNMS pdnmsThis,  FILE *pFile);
void LIBCALL WritePDBSeqRes(PDNMS pdnmsThis,  FILE *pFile);
void LIBCALLBACK PDBConnect(PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr);
void LIBCALL WritePDBConnect(PDNMS pdnmsThis,  Int2 iModel, FILE *pFile);
Int2 LIBCALL WritePDBModelList(PDNMS pdnmsThis,  FILE *pFile,  Int2 iNumModels,  Int2Ptr i2Vec );
Int2 LIBCALL WritePDBOneModel(PDNMS pdnmsThis,  FILE *pFile,  Int2 iModel);
NLM_EXTERN Int2 LIBCALL WritePDBAllModel(PDNMS pdnmsThis,  FILE *pFile);

CharPtr LIBCALL GetKinMolName(PMMD pmmdThis);
CharPtr LIBCALL WriteKinSeq(PDNMS pdnmsThis,  FILE *pFile);
void LIBCALL WriteKinHeader(PDNMS pdnmsThis,  FILE *pFile);
void LIBCALL AddLineKin(FILE *pFile,  PALD paldFrom,  PALD paldTo);
void LIBCALLBACK WriteKinVirt(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr);
void LIBCALLBACK WriteKinHet(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr);
void LIBCALLBACK WriteKinBB(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr);
void LIBCALLBACK WriteKinRes(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr);
void LIBCALLBACK WriteKinAAResType(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr);
void LIBCALLBACK WriteKinNAResType(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr);
void LIBCALLBACK WriteKinAlt(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr);
void LIBCALLBACK WriteKinIon(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr);
void LIBCALLBACK WriteKinAtom(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr);
Int2 LIBCALL WriteKinAnimate(Int2 iModel,  PDNMS pdnmsThis, 
		FILE *pFile,  Int2 iColor, Byte bKinRender, CharPtr pcComposition);
Int2 LIBCALL KinColorFromSS(PMGD pmgdThis);
CharPtr LIBCALL NameFromSS(PMGD pmgdThis);
Int2 LIBCALL WriteKinModelByType(Int2 iModel,  PDNMS pdnmsThis, 
		FILE *pFile,  Int2 iColor, Byte bKinRender, CharPtr pcComposition);
Int2 LIBCALL WriteKinModel(Int2 iModel,  PDNMS pdnmsThis, 
		FILE *pFile,  Int2 iColor, Byte bKinRender, CharPtr pcComposition);
Int2 LIBCALL WriteKinModelList(PDNMS pdnmsThis,  FILE *pFile,  Int2 iColor, Byte bKinRender,
				 Int2 iNumModels,  Int2Ptr i2Vec );
NLM_EXTERN Int2 LIBCALL WriteKinOneModel(PDNMS pdnmsThis,  FILE *pFile, Int2 iColor, Byte bKinRender, Int2 iModel);
NLM_EXTERN Int2 LIBCALL WriteKinAllModel(PDNMS pdnmsThis, FILE *pFile, Int2 iColor, Byte bKinRender);
NLM_EXTERN Int2 LIBCALL WriteStructSummary(PDNMS pdnmsThis,  FILE *pFile);
void LIBCALL WriteFASTAMolecule(PDNMM pdnmmThis, FILE *File, Int4 LineLen);
void LIBCALL WriteFASTASeqHet(PDNMS pdnmsThis,  FILE *pFile);
NLM_EXTERN void WriteStrucHTMLSeq(PDNMS pdnmsThis,  FILE *pFile);


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
