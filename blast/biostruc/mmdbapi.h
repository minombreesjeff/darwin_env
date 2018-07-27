/* ===========================================================================
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
* File Name:  mmdbapi.h
*
* Author:  Christopher Hogue
*
* Version Creation Date:   09/18/95
*
* $Revision: 6.5 $
*
* File Description: General Header with Macros for MMDBAPI.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
* $Log: mmdbapi.h,v $
* Revision 6.5  2000/03/01 16:16:45  thiessen
* modified backbone definitions, added AM_PARTIALBACKBONE
*
* Revision 6.4  1999/03/05 22:44:00  vakatov
* Made ClearStructures() function be NLM_EXTERN -- for the DLL build on PC
*
* Revision 6.3  1998/04/16 00:30:01  lewisg
* get rid of dead code
*
* Revision 6.2  1998/03/13 22:34:26  lewisg
* fix neighbor mode
*
* Revision 6.1  1998/03/06 01:16:13  lewisg
* merge
*
* Revision 6.0  1997/08/25 18:10:53  madden
* Revision changed to 6.0
*
* Revision 5.2  1997/06/19 18:40:01  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.1  1997/03/13 22:07:20  vakatov
* In the preprocessor macros, surrounded macro argument("pfb") by parenthesis
*
 * Revision 5.0  1996/05/28  14:02:09  ostell
 * Set to revision 5.0
 *
 * Revision 1.5  1996/04/18  17:09:23  hogue
 * Modifications for Neighbor mode with Master and Slave structures.
 *
 * Revision 1.4  1996/02/02  21:25:53  hogue
 * changed to include mmdbapi4.h
 *
 * Revision 1.3  1996/01/31  21:32:24  hogue
 * Major changes to accomodate multiple structures, added _mmdbAPI
 *
 * Revision 1.2  1995/10/05  20:22:16  hogue
 * move mmdbapi struc definition to mmdbapi.h
 *
 * Revision 1.1  1995/09/20  13:45:47  kans
 * Initial revision
 *
*
* ==========================================================================
*/

#ifndef _MMDBAPI_
#define _MMDBAPI_


#ifndef _MMDBAPI1_
#include <mmdbapi1.h>   /* Type definitions, New/Free Functions */
#endif

#ifndef _MMDBAPI2_
#include <mmdbapi2.h>   /* Traversers, & Utility functions */
#endif

#ifndef _MMDBAPI3_
#include <mmdbapi3.h>   /* File generators */
#endif

#ifndef _MMDBAPI4_
#include <mmdbapi4.h>   /* ASN.1 savers & future feature handling code */
#endif



#define PDB_ACC    1
#define LONG_NAME  2
#define PDB_CLASS  3
#define PDB_SOURCE 4

/* this struct may be later expanded with a ->next */
/* pointer for separate instances of programs */
/* running to isolate data ... */

typedef struct NLM_mmdbapi
  {
    PDNMS pdnmsMain;	    /* the list of structures for viewing */
    PDNMS pdnmsMaster;	    /* pointer to master of an alignment */
    PDNMS pdnmsSlave;       /* pointer to slave of an alignment */
    PDNMS pdnmsHolder;	    /* used for internal indexing First/next */
    PDNMS pdnmsSelected;     /* pointer to the active structure */
    PDNMS pdnmsPrevious;     /* pointer to the last active structure */
    ValNodePtr pvnPFBSelect; /* linked-list of selected PFB pointers */ 
    ValNodePtr pvnUndoMark; /* the undo marker for the Select list  */
    PRGD  prgdDict;    	    /* Global dictionary pointer */
    Byte  bExtent;   
    Boolean Neighbor;       /* for child structures aligned to parent */
    Int4  iLocalCount;	    /* assigns each new Biostruc a new number */
			    /* this is choice field in pvnPFBSelect */
			    /* and in the pdnms */
			    /* this may become a history list of MMDB */
			    /* ID's assoicated with each localcount   */
    Boolean fCn3D;  /* oh gosh, a mode bit.  Necessary for backward compatibility */
        Int2  iFeatureSet;
    Int2  iFeatureId;  
    PVNSFF pvnsffHead;      /* ValNode list of registered feature functions */ 
    PColor3DFunc pColorFunc;    

  } mmdbAPI, *pmmdbAPI;
  
/* MMDBAPI MACRO FUNCTIONS */

/* use these with any PFB node */
#define IsAtomNode(pfb)      ((int) (pfb)->bMe == AM_MAD)
#define IsBondNode(pfb)      ((int) (pfb)->bMe == AM_MBD)
#define IsGraphNode(pfb)     ((int) (pfb)->bMe == AM_MGD)
#define IsMoleculeNode(pfb)  ((int) (pfb)->bMe == AM_MMD)
#define IsObjectNode(pfb)    ((int) (pfb)->bMe == AM_MOD)
#define IsDensityNode(pfb)   ((int) (pfb)->bMe == AM_MDD)
#define IsStructureNode(pfb) ((int) (pfb)->bMe == AM_MSD)
#define IsAtomLocNode(pfb)   ((int) (pfb)->bMe == AM_ALD)

/* use these when IsStructureNode(pfb) OR IsMoleculeNode(pfb) == TRUE */
#define IsIon(pfb)		((pfb)->bWhat & (Byte) AM_ION)
#define IsRNA(pfb)		((pfb)->bWhat & (Byte) AM_RNA)
#define IsDNA(pfb)		((pfb)->bWhat & (Byte) AM_DNA)
#define IsNA(pfb)	       (((pfb)->bWhat & (Byte) AM_RNA) || ((pfb)->bWhat & (Byte) AM_DNA))
#define IsProtein(pfb)		((pfb)->bWhat & (Byte) AM_PROT)
#define IsNAorProtein(pfb)     (((pfb)->bWhat & (Byte) AM_RNA) || ((pfb)->bWhat & (Byte) AM_DNA)\
 || ((pfb)->bWhat & (Byte) AM_PROT))
#define IsWater(pfb)		((pfb)->bWhat & (Byte) AM_WAT)  
#define IsSolvent(pfb)	       (((pfb)->bWhat & (Byte) AM_SOL) || ((pfb)->bWhat & (Byte) AM_WAT))
#define IsHeterogen(pfb)	((pfb)->bWhat & (Byte) AM_HET)
#define IsOtherPolymer(pfb)	((pfb)->bWhat & (Byte) AM_POLY)

/* use these when IsGraphNode(pfb) == TRUE */
#define IsGraphRNABase(pfb)	((pfb)->bWhat & (Byte) RES_RNA)
#define IsGraphDNABase(pfb)	((pfb)->bWhat & (Byte) RES_DNA)
#define IsGraphNABase(pfb)     (((pfb)->bWhat & (Byte) RES_RNA) || ((pfb)->bWhat & (Byte) RES_DNA))
#define IsGraphAminoAcid(pfb)	((pfb)->bWhat & (Byte) RES_AA)
#define IsGraphNAorAA(pfb)     (((pfb)->bWhat & (Byte) RES_RNA) || ((pfb)->bWhat & (Byte) RES_DNA)\
 || ((pfb)->bWhat & (Byte) RES_AA))
 
/* use these when IsGraphNode(pfb) AND IsGraphAminoAcid(pfb) == TRUE */
#define IsAAInPDBSS(pfb)      ((pfb)->bPDBSecStru)
#define IsAAInPDBHelix(pfb)   ((pfb)->bPDBSecStru & (Byte) SS_HELIX)
#define IsAAInPDBSheet(pfb)   ((pfb)->bPDBSecStru & (Byte) SS_SHEET)
#define IsAAInPDBStrand(pfb)  ((pfb)->bPDBSecStru & (Byte) SS_STRAND)
#define IsAAInPDBTurn(pfb)    ((pfb)->bPDBSecStru & (Byte) SS_TURN)
#define IsAAInNCBISS(pfb)     ((pfb)->bNCBISecStru)
#define IsAAInNCBIHelix(pfb)  ((pfb)->bNCBISecStru & (Byte) SS_HELIX)
#define IsAAInNCBISheet(pfb)  ((pfb)->bNCBISecStru & (Byte) SS_SHEET)
#define IsAAInNCBIStrand(pfb) ((pfb)->bNCBISecStru & (Byte) SS_STRAND)
#define IsAAInNCBITurn(pfb)   ((pfb)->bNCBISecStru & (Byte) SS_TURN)


/* use these when IsAtomNode(pfb) == TRUE */
#define IsAtomBackBone(pfb)     ((pfb)->bWhat & (Byte) AM_BACKBONE)
#define IsAtomPartialBackBone(pfb)    ((pfb)->bWhat & (Byte) AM_PARTIALBACKBONE)
#define IsAtomCAlpha(pfb)       ((pfb)->bWhat & (Byte) AM_CALPHA)
#define IsAtomPAlpha(pfb)       ((pfb)->bWhat & (Byte) AM_PALPHA)
#define IsAtomAlpha(pfb)       (((pfb)->bWhat & (Byte) AM_CALPHA)  ||  ((pfb)->bWhat & (Byte) AM_PALPHA))
#define IsAtomCBeta(pfb)        ((pfb)->bWhat & (Byte) AM_CBETA)
#define IsAtomNBeta(pfb)        ((pfb)->bWhat & (Byte) AM_NBETA)
#define IsAtomBeta(pfb)	       (((pfb)->bWhat & (Byte) AM_CBETA) || ((pfb)->bWhat & (Byte) AM_NBETA))
#define IsAtomC1Ribose(pfb)     ((pfb)->bWhat & (Byte) AM_C1RIBOSE)
#define IsAtomC4Ribose(pfb)     ((pfb)->bWhat & (Byte) AM_C4RIBOSE)
#define AtomPDBName(pfb)         (pfb)->pcAName
#define AtomicNumber(pfb)        (pfb)->pvnmaLink->choice 


/* use these when IsBondNode(pfb) == TRUE */
#define IsBondVirtual(pfb)      ((pfb)->bWhat & (Byte) BOND_VIRTUAL)
#define IsBondPDouble(pfb)      ((pfb)->bWhat & (Byte) BOND_PDOUBLE)
#define IsBondVanderWaal(pfb)   ((pfb)->bWhat & (Byte) BOND_VDW)
#define IsBondIonic(pfb)        ((pfb)->bWhat & (Byte) BOND_IONIC)
#define IsBondHydrogen(pfb)     ((pfb)->bWhat & (Byte) BOND_H)
#define IsBondTriple(pfb)       ((pfb)->bWhat & (Byte) BOND_TRIPLE)
#define IsBondDouble(pfb)       ((pfb)->bWhat & (Byte) BOND_DOUBLE)
#define IsBondSingle(pfb)       ((pfb)->bWhat & (Byte) BOND_SINGLE)
#define IsBondAromatic(pfb)    (((pfb)->bWhat & (Byte) BOND_PDOUBLE) && ((pfb)->bWhat & (Byte) BOND_DOUBLE))

/* use these when IsObjectNode(pfb) == TRUE */
#define IsObjectSphere(pfb)     ((pfb)->bWhat & (Byte) OBJ_SPHERE)
#define IsObjectCylinder(pfb)   ((pfb)->bWhat & (Byte) OBJ_CYLINDER)
#define IsObjectBrick(pfb)      ((pfb)->bWhat & (Byte) OBJ_BRICK)
#define IsObjectTMesh(pfb)      ((pfb)->bWhat & (Byte) OBJ_TMESH)
#define IsObjectTriangles(pfb)  ((pfb)->bWhat & (Byte) OBJ_TRIANGLES)
#define IsObjectCone(pfb)       ((pfb)->bWhat & (Byte) OBJ_CONE)

/* use these when IsAtomLocNode(pfb) == TRUE */
#define AtomLocX(pfb)  (pfb)->pflvData[0]
#define AtomLocY(pfb)  (pfb)->pflvData[1]
#define AtomLocZ(pfb)  (pfb)->pflvData[2]

/* use when IsStructureNode(pfb) == TRUE */
#define PDBAccession(pfb)  (pfb)->pcPDBName
 


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

PDNMS LIBCALL GetPDNMSMain(void);  
NLM_EXTERN void LIBCALL ClearStructures(void);
void LIBCALL fnMMDBCn3Dmode (void); 
NLM_EXTERN PRGD LIBCALL GetPRGDDictionary(void);
NLM_EXTERN void LIBCALL ChangeMMDBAPIbExtent(Byte bExtent);
NLM_EXTERN Byte LIBCALL GetMMDBAPIbExtent(void);
NLM_EXTERN Int2 LIBCALL OpenMMDBAPI(Byte bExtent, CharPtr pcDictName);
Boolean LIBCALL IsMMDBAPIOpen(void);
pmmdbAPI LIBCALL NewMMDBAPI(void);
BiostrucPtr LIBCALL FetchBiostrucPDB(CharPtr pcAcession, Int4 iModelLevel, Int4 iMaxModels);
Int4 LIBCALL MakeHashChange(PDNMS pdnmsThis);
Int4 LIBCALL CountModelstrucs(void);
NLM_EXTERN PDNMS LIBCALL MakeAModelstruc(BiostrucPtr pbsThis);
NLM_EXTERN void LIBCALL FreeAModelstruc(PDNMS pdnmsThis);
void LIBCALL SetNeighborOn(void);
Boolean LIBCALL AreNeighborsOn(void); 
void LIBCALL SetNeighborOff(void);
void LIBCALL SetMasterModelstruc(PDNMS pdnmsThis);
void LIBCALL SetSlaveModelstruc(PDNMS pdnmsThis);
void LIBCALL SetHolderModelstruc(PDNMS pdnmsThis);
void LIBCALL SetSelectedModelstruc(PDNMS pdnmsThis);
PDNMS LIBCALL GetSelectedModelstruc(void);
PDNMS LIBCALL GetMasterModelstruc(void);
PDNMS LIBCALL GetSlaveModelstruc(void);
PDNMS LIBCALL GetFirstModelstruc(void);
PDNMS LIBCALL GetNextModelstruc(void);
PDNMS LIBCALL GetPreviousModelstruc(void);
PDNMS LIBCALL SwapModelstruc(void);
NLM_EXTERN void LIBCALL CloseMMDBAPI(void);
CharPtr LIBCALL GetStrucStrings(PDNMS pdnmsThis, Int4 iPickType);
ValNodePtr LIBCALL AddPFBSelect(PFB pfbThis);
void LIBCALL ClearPFBSelectList(void);
void LIBCALL UndoPFBSelectList(void);

Uint1 LIBCALL Call3DColorProc(ColorPropPtr pcpThis);
PSFF LIBCALL DoesFeatureTypeHaveFuncs(Uint1 iCode);

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

