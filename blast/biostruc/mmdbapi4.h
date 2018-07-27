/*   mmdbapi4.h
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
* File Name:  mmdbapi4.h
*
* Author:  Christopher Hogue
*
* Version Creation Date:   10/18/95
*
* $Revision: 6.2 $
*
* File Description:  Code for Modelstruc -> Biostruc conversion 
*                    And User-feature handling.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* $Log: mmdbapi4.h,v $
* Revision 6.2  1999/02/26 14:17:52  beloslyu
* fixed declaration of WriteAsnModelList function to synchronize with cn3d to save out mime data
*
* Revision 6.1  1998/03/10 04:31:11  lewisg
* change name of MakePDBSeqId to MakePDBSeqId2 to avoid name conflict
*
* Revision 6.0  1997/08/25 18:11:19  madden
* Revision changed to 6.0
*
* Revision 5.1  1996/11/22 21:18:21  epstein
* add BiostrucAnnotSetToSeqAnnot()
*
 * Revision 5.0  1996/05/28  14:02:09  ostell
 * Set to revision 5.0
 *
 * Revision 1.4  1996/02/07  23:06:41  hogue
 * fixed a type cast in FreeSingleModel
 *
 * Revision 1.3  1996/02/02  20:19:50  hogue
 * *** empty log message ***
 *
 * Revision 1.2  1996/02/02  19:45:10  hogue
 * Initial Revision
 *
*
* ==========================================================================
*/

/*****************************************************************************
*
*  mmdbapi4.h
*   
*   UNDER CONSTRUCTION NOTICE. 
*	EVERYTHING SUBJECT TO RADICAL CHANGES!
*   
*  programmer C.W.V. Hogue
*  created: 18 Oct 95
*  last mod: 
*****************************************************************************/


#ifndef _MMDBAPI4_
#define _MMDBAPI4_

#ifdef __cplusplus
extern "C" {
#endif


/* bSave Byte for WriteASN functions */
/* use these to subset portions of the in-memory structure to save */
#define    ANNOT_ONLY     0x80  /* Save Features in Biostruc-annot-set.  */
#define    DICT_ONLY	  0x40  /* Save Local Dictionary in Biostruc-residue-graph-set */
#define    NEW_MMDB_ID   0x20  /* assigns a new ID with a hashed-random function */ 
#define    NOT_MODELS     0x04  /* omits all models */
#define    NOT_FEATURES   0x02  /* omits all features */
#define    SAVE_BINARY    0x01  /* writes ASN.1 binaries */
#define    SAVE_ASCII     0x00  /* default - saves all in one ascii file */
 
#define SAVE_ASCII_STRING "w"
#define SAVE_BINARY_STRING "wb"
         
#define SAVE_XYZ_SCALE_FACTOR 1000.0
#define DICT_STANDARD_STRING "Standard residue dictionary"
#define DICT_STANDARD_ID 1

Boolean LIBCALL  RefreshSurface(SurfaceCoordinatesPtr  pscThis, PMOD pmodThis);
void LIBCALL   FreeRedundantAsn(PDNMS pdnmsThis);
Boolean LIBCALL RefreshModelAsnMem(PDNMS pdnmsThis,  Int2 iModel);
Boolean LIBCALL RebuildChemGraphAsn(PDNMS pdnmsThis);
Boolean LIBCALL WriteOutBiostruc(BiostrucPtr pbsThis, CharPtr pcSave,  Byte bSave);
Boolean LIBCALL WriteAsnChemGraphOnly(PDNMS pdnmsThis, CharPtr pcSave,  Byte bSave);
Boolean LIBCALL WriteAsnLocalDict(PDNMS pdnmsThis, CharPtr pcSave,  Byte bSave,  
	Boolean SaveId);
Boolean LIBCALL FreeSingleModel(PDNMS pdnmsThis, Int2 iModel);
Boolean LIBCALL WriteAsnModelList(PDNMS pdnmsThis,   Int2 iNumModels,  Int2Ptr i2Vec,  
				    CharPtr pcSave,  Byte bSave, Boolean iCn3d);
Boolean LIBCALL WriteAsnOneModel(PDNMS pdnmsThis,  Int2 iModel,  CharPtr pcSave,  
					Byte bSave);
Boolean LIBCALL WriteAsnAllModel(PDNMS pdnmsThis,  CharPtr pcSave,  Byte bSave);
Boolean LIBCALL WriteASNChemGraphOnly(PDNMS pdnmsThis, CharPtr pcSave,  Byte bSave);

SeqIdPtr MakePDBSeqId2 (CharPtr pcPDB, Char cChain, int iDomain, Boolean getgi);
SeqAnnotPtr LIBCALL BiostrucAnnotSetToSeqAnnot(BiostrucAnnotSetPtr set, Boolean usePValue);










#ifdef __cplusplus
}
#endif

#endif


/*  Minimum allowed ASN.1 ver 4.1 Biostruc...
* Biostruc ::= {
*  id {
*    mmdb-id 0 } ,
*  chemical-graph {
*    molecule-graphs {
*      {
*        id 1 ,
*        residue-sequence {
*          {
*            id 1 ,
*            residue-graph
*              local 1 } } } } ,
*    residue-graphs {
*      {
*        id 1 ,
*        atoms {
*          {
*            id 1 ,
*            element unknown } } ,
*        bonds {
*           } } } } }
*/
