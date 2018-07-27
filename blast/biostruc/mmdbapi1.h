/*   mmdbapi1.h
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
* File Name:  mmdbapi1.h
*
* Author:  Christopher Hogue
*
* Version Creation Date:   03/14/95
*
* $Revision: 6.32 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 95/08/28 C. Hogue    Major change to new Biostruc ASN.1 v4.0,
*                      Addition of Features, Secondary Structure, Ensembles,
*                      PDB file generators, Kinemage file renderers.
*		       Model nodes and Dictionaries altered...
*
* $Log: mmdbapi1.h,v $
* Revision 6.32  2000/12/05 21:03:05  hurwitz
* made it neat
*
* Revision 6.31  2000/07/24 22:29:45  thiessen
* fix header conflict
*
* Revision 6.30  2000/07/21 18:56:41  thiessen
* allow dynamic slave->master transformation
*
* Revision 6.29  2000/03/13 15:51:25  thiessen
* added storage for PARS to each residue for Cn3D; removed unused fields
*
* Revision 6.28  2000/03/09 17:52:57  thiessen
* changes to MSD, MGD for feature implementation in Cn3D
*
* Revision 6.27  2000/03/01 16:16:45  thiessen
* modified backbone definitions, added AM_PARTIALBACKBONE
*
* Revision 6.26  1999/10/05 23:18:17  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.25  1999/06/07 21:23:36  ywang
* add iUserDefinedFeatureOld, FeatureOnOld to MGD
*
* Revision 6.24  1999/05/07 14:11:25  zimmerma
* - Modified InstallAlignedSlave(), InstallStrucFeature(), BiostrucAddFeature() and
* Boolean InstBSAnnotSet() for additional args: Boolean Chain, and CharPtr SlaveChain;
*
* - InstBSAnnotSet now also expects argument CharPtr JobID
*
* - Added forward DCLs for LIBCALL isBiopoly() and LIBCALL isHet()
*
* Revision 6.23  1999/04/26 20:49:59  lewisg
* changed arguments named list to fix visual c++ bug
*
* Revision 6.22  1999/04/15 21:36:18  ywang
* move iMimeType to data block for MSD
*
* Revision 6.21  1999/04/13 23:07:17  ywang
* move position for Byte bVisible for MOD
*
* Revision 6.20  1999/03/18 21:04:30  lewisg
* reverse transform for saving
*
* Revision 6.19  1999/03/17 20:11:10  lewisg
* fix memory overwrite in pfd
*
* Revision 6.18  1999/03/01 20:22:11  ywang
* add bTurnedOff flag on MGD
*
* Revision 6.17  1999/02/24 22:58:10  ywang
* add iMimeType to MSD node and other minor name change
*
* Revision 6.16  1999/02/02 22:27:01  ywang
* add bJustHighlighted flag to MGD for feature edit
*
 * Revision 6.15  1998/12/16  19:30:14  ywang
 * add flag for highlight status to MGD
 *
 * Revision 6.14  1998/11/06  23:02:02  ywang
 * add FeatureOn to MGD for feature on/off control
 *
 * Revision 6.13  1998/11/04  00:53:41  ywang
 * add iFeature to MGD for modeling control
 *
 * Revision 6.12  1998/10/21  15:43:22  ywang
 * attach the whole vast alignment data to master structure
 *
 * Revision 6.11  1998/10/01  21:56:37  ywang
 * set display flag for object
 *
 * Revision 6.10  1998/09/22  17:52:21  ywang
 * add flag for display control on MM and MG level
 *
 * Revision 6.9  1998/05/12  21:46:24  lewisg
 * stricter conservation coloring
 *
* Revision 6.8  1998/04/28 22:51:20  lewisg
* created separate pointer to master in MGD
*
* Revision 6.7  1998/04/21 22:58:53  lewisg
* eliminate dead code
*
* Revision 6.6  1998/04/10 22:41:48  lewisg
* eliminated dead code and added a visible bit to MSD
*
* Revision 6.5  1998/03/26 22:43:30  lewisg
* added seqentry and seq annot to msd
*
* Revision 6.4  1998/03/10 04:31:05  lewisg
* change name of MakePDBSeqId to MakePDBSeqId2 to avoid name conflict
*
* Revision 6.3  1998/03/06 23:17:12  lewisg
* codewarrior fixes
*
* Revision 6.2  1998/03/06 01:15:28  lewisg
* merge
*
* Revision 6.0  1997/08/25 18:11:00  madden
* Revision changed to 6.0
*
* Revision 5.5  1997/06/19 18:40:03  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.4  1997/04/18 19:18:56  hogue
* Int2 counters for atoms upgraded to Int4's, made ppAsnOrder alloc optional.
*
 * Revision 5.3  1997/04/09  18:59:33  hogue
 * removed redundant include of objall.h
 *
 * Revision 5.2  1997/01/08  21:07:51  vakatov
 * Removed redundant declaration of NCBI_months[12]
 *
 * Revision 5.1  1996/07/31  18:38:22  hogue
 * Structure feature handling structs added, and pointer from ald to graphic for composing groups of segments.
 *
 * Revision 5.0  1996/05/28  14:02:09  ostell
 * Set to revision 5.0
 *
 * Revision 1.17  1996/05/01  14:41:26  hogue
 * Added  MakeChemGraphNodeList, and Domain support
 *
 * Revision 1.16  1996/04/18  17:38:36  hogue
 * FETCH_ENTREZ flag use altered, added pdnmsSlaves to MSD for slave structures in multiple alignments
 *
 * Revision 1.15  1996/03/29  19:28:49  hogue
 * Added pExtra pointer to MSD
 *
 * Revision 1.14  1996/01/31  21:35:00  hogue
 * Changes to accomodate the free-ing of models, most dramatically is the
 * encapuslation of interim free-ers to FreeSurfaceModelAsnList and
 * FreeAtomicModelAsnLists.  KEEP_ASN no longer supported.  PDNSM and its
 * kin renamed to PDNML.
 *
 * Revision 1.13  1995/11/09  22:54:56  hogue
 * Added new pointers, variables to node data structures for Feature tables,
 * rendering, coloring; added linking pointers between models and locations/objects
 *
 * Revision 1.12  1995/09/19  21:08:08  hogue
 * Removed unnecessary typedefs.
 *
 * Revision 1.11  1995/09/18  21:25:11  hogue
 * Het name field added in MGD
 *
 * Revision 1.10  1995/08/29  20:03:06  kans
 * Entrez-like loading now enabled, models fixed
 *
 * Revision 1.8  1995/08/28  22:46:40  kans
 * moved BiostrucAvail to mmdbapi1.[ch]
 *
 * Revision 1.7  1995/08/28  20:06:37  kans
 * *** empty log message ***
 *
 * Revision 1.5  1995/08/28  19:31:53  kans
 * version 4.1
 *
 * Revision 1.3  1995/08/11  20:23:09  epstein
 * add support for VECMODEL
 *
 * Revision 1.2  1995/08/03  21:44:33  kans
 * numerous changes
 *
 * Revision 1.1  1995/08/02  17:07:15  kans
 * Initial revision
 *
*
* ==========================================================================
*/

/*****************************************************************************
*
*  mmdbapi1.c
*   
*   UNDER CONSTRUCTION NOTICE. 
*	EVERYTHING SUBJECT TO RADICAL CHANGES!
*   
*  programmer C.W.V. Hogue
*  created: 14 Mar 95
*  last mod: 28 AUGUST 95
*****************************************************************************/

#ifndef _MMDBAPI1_
#define _MMDBAPI1_

/* GLOBAL VARIABLES */

#include <asn.h>
#include <objall.h>
#include <strimprt.h>
#include <objmmdb1.h>
#include <objmmdb2.h>
#include <objmmdb3.h>
#include <dvncode.h>
#include <corematx.h>
#include <objseq.h>
#include <objsset.h>
/*#include <mmdbapi.h>*/

#define INP_ASCII_FILE 1
#define INP_BINARY_FILE 0
#define INP_GI 2
#define INP_PDB_ACCESSION 3

#define DICT_DEFAULT "bstdt.val"
/* new version 4 ASN.1 defines */
#define PDBNAME_DEFAULT "1AAA"
#define STRING_UNK_PDB "Unknown/Non-PDB"
#define STRING_UNK_RES "UNK"
#define STRING_UNK_MOL " "
#define STRING_NCBI_SS "NCBI assigned secondary structure"
#define STRING_PDB_SS "PDB secondary structure"
#define STRING_NCBI_VAST "VAST Alignment"
#define STRING_NCBI_DOMAIN "NCBI Domains"
#define STRING_PDB_SITE "PDB SITE"
#define STRING_PDB_FOOTNOTE "PDB FOOTNOTE"
#define Assembly_Type_Physiological_Form 1
#define Assembly_Type_Crystallographic_Cell 2
#define Assembly_Type_Other 255

#define Molecule_type_dna 1
#define Molecule_type_rna 2
#define Molecule_type_protein 3
#define Molecule_type_other_biopolymer 4
#define Molecule_type_solvent 5
#define Molecule_type_other_nonpolymer 6
#define Molecule_type_other 255

#define Residue_deoxyribonucleotide 1
#define Residue_ribonucleotide 2
#define Residue_amino_acid 3
#define Residue_other 255

#define Feature_type_helix 1
#define Feature_type_strand 2
#define Feature_type_sheet 3
#define Feature_type_turn 4
#define Feature_type_site 5
#define Feature_type_footnote 6
#define Feature_type_subgraph 100
#define Feature_type_region 101
#define Feature_type_alignment 200
#define Feature_type_similarity 201
#define Feature_type_other 255  

#define Feature_type_transform 153
#define Feature_type_camera 154
#define Feature_type_script 155
#define Feature_type_multalign 202
#define Feature_type_color 150
#define Feature_type_render 151
#define Feature_type_label 152
#define Feature_type_indirect 203
#define Feature_type_cn3dstate 254
#define Feature_type_comment 7

#define MAX_FEATURES 10  
#define VIEW_CODE 1 /* undef to remove display specific stuff in Modelstruc*/
/* #define BIG_CODE 1  define to add "user" variables to a Modelstruc PFB nodes */

#define INTSTRLEN 30  /* characters in MAXINT */

#define Model_type_ncbi_vector  1
#define Model_type_ncbi_backbone 2
#define Model_type_ncbi_all_atom 3 
#define Model_type_pdb_model 4 
#define Model_type_other 255
 
#define BSEVERYTHING 5

/*bExtent settings:*/
/* used by Biostruc2Modelstruc */
/* determines how much is converted */
#define COORDS_DATA  0x01
#define TEMP_DATA    0x02 /*must have coords too*/
#define OCC_DATA     0x04 /* must have coords too */
#define ALTCNF_DATA  0x08 /* other locations */ 
#define FETCH_ENTREZ 0x20 /* Need to call EntrezInit/EntrezFini   */
#define BD_TREE      0x40
/* #define FEATURE_TAB  0x80 not used */
#define POWER_VIEW   0xFF
#define CONVERT_ALL  0xFF

/*
 * Flags for the bMe of the MBD,MAD,MGD,MMD, MSD hierarchy of structures.
 * SHOW_ME byte is for a viewer to see if the node and its children are to be shown
 * REG_STYLE says display using the default style palette for the node and children 
 * AM_CHANGED is a flag saying the node has been changed by last operation.
 * AM_MDB,  AM_MAD, AM_MGD, AM_MMD, AM_MSD say what kind of sub-struc this is
 * and can be used to typecast appropriately.
 * 
 */

/* Flags Block bUpdate byte */
/* these bits ARE stacked - i.e. 
 * don't set with:
 *  byte = AM_CHANGED, instead use
 *  byte = (byte | AM_CHANGED)
 *  
 */
#define AM_CHANGED 0x01
#define SHOW_ME    0x02
#define REG_STYLE  0x04
/* define AM_ION   0x80 declared below */


/* Flags Block bMe byte */
/* these bits are NOT EVER stacked 
 * and are set by MMDBAPI 
 * you can use a switch statement
 * on these...
 */
#define AM_ALD     0x80  /* Atom Location   Data */
#define AM_MDD     0x40  /* Model Density   Data */
#define AM_MOD     0x20  /* Model Object    Data */
#define AM_MBD     0x10  /* Model Bond      Data */
#define AM_MAD     0x08  /* Model Atom      Data */
#define AM_MGD     0x04  /* Model Graph     Data */
#define AM_MMD     0x02  /* Model Molecule  Data */
#define AM_MSD     0x01  /* Model Structure Data */
#define AM_UNK     0x00  

/*
 * Flags for the bWhat of MBD, MAD, MGD, MMD, MDS hierarchy.
 * This says what is contained in this branch of hierarchy, i.e.
 * a full MSD structure can be (no Nucleic acids)
 * e.g. pMSD->bWhat = AM_POLY | AM_PROT | AM_HET | AM_SOL
 * or, in a MMD molecule
 * pMMD->bWhat = AM_POLY | AM_PROT
 * etc...
 * This way if the structure has NO HET groups,  the don't invoke
 * them from a menu...
 */

/* Flags Block bWhat byte  For Struc, Molec */
/* these bits ARE stacked in Struc, but NOT stacked in Molec */
#define  AM_ION   0x80 /* not set in PDB derived data - do lookup */
#define  AM_RNA   0x40
#define  AM_WAT   0x20
#define  AM_SOL   0x10
#define  AM_HET   0x08
#define  AM_DNA   0x04
#define  AM_PROT  0x02
#define  AM_POLY  0x01  

/* Flags Block bWhat byte for Graph */
/* these bits ARE stacked */
#define  DICT_REF     0x80
#define  DICT_GLOBAL  0x40
#define  DICT_LOCAL   0x20
#define  RES_RNA      0x10
#define  RES_DNA      0x08
#define  RES_AA       0x04

/* Flags for bNCBISecStru or pPDBSecStru in PMGD of type RES_AA */
#define   SS_HELIX     0x80
#define   SS_SHEET     0x40
#define   SS_STRAND    0x20
#define   SS_TURN      0x10
 
/* Flags Block bWhat byte for MAD atom */
/* all these are  set by traversers */
#define  AM_BACKBONE            0x01  /* PROT & NA */
#define  AM_CALPHA              0x02  /* PROT */
#define  AM_PALPHA              0x04  /* RNA & DNA */
#define  AM_PARTIALBACKBONE     0x08  /* PROT & NA - all atoms off linear backbone */
#define  AM_CBETA               0x10  /* PROT */
#define  AM_NBETA               0x20  /* RNA & DNA */
#define  AM_C1RIBOSE            0x40  /* RNA & DNA */
#define  AM_C4RIBOSE            0x80  /* RNA & DNA */

/* shorter names from the ASN.1 defines */
#define Bond_order_partial_double 2
#define Bond_order_aromatic 3
#define Bond_order_double 4
#define Bond_order_triple 5
#define Bond_order_other 6
#define Bond_order_unknown 255
#define Bond_order_single 1

/* Flags Block bWhat byte for MBD bond */
/* these bits are stacked */
#define  BOND_VIRTUAL  0x80  /* set by traversers */
#define  BOND_PDOUBLE  0x40
#define  BOND_VDW      0x20  /* not set for PDB */
#define  BOND_IONIC    0x10  /* not set for PDB */
#define  BOND_H        0x08  /* not set for PDB */
#define  BOND_TRIPLE   0x04
#define  BOND_DOUBLE   0x02
#define  BOND_SINGLE   0x01
#define  BOND_NONE     0x00
 
/* if BOND_PDOUBLE & BOND_DOUBLE then aromatic */

/* Flags Block bWhat byte  For Object */
/* these are not stacked */
#define  OBJ_SPHERE     0x80
#define  OBJ_CYLINDER   0x40
#define  OBJ_BRICK      0x20
#define  OBJ_TMESH      0x10
#define  OBJ_TRIANGLES  0x08
#define  OBJ_CONE       0x04
 
/* Pointers to the hierarchical types for Biostruc data
 * Note the terse struct names MMx, PMMx, MMxD are used to limit the
 * length of long pointer casts when coding and for Hungarian Notation
 * coding.  The names are broken down
 * here in the header file for each declaration,  so you can see how they
 * are logically constructed.
 * a PDN is a Pointer to DValNode
 * a PVN is a Pointer to ValNode
 */

typedef DValNodePtr PDNMS; /* Model Structure (MS) */
/* Node contains a PMSD - Pointer to Model Structure Data */
/* choice = structure no */

typedef DValNodePtr PDNMM; /*  Model Molecule (MM) */
/* Node contains a PMMD - Pointer to Model Molecule Data */
/* choice = molecule id */

typedef DValNodePtr PDNMG; /* Model  Residue (MG)*/
/* Node contains a PMGD - Pointer to Model Residue Data */
/* choice = residue id */

typedef ValNodePtr PVNMA; /* Model Atom (MA) */
/* Node contains a PMAD - Pointer to Model Atom Data */
/* choice = element */

typedef ValNodePtr PVNMB; /* Model Bond (MB)*/
/* Node contains a PMBD - Pointer to Model Bond Data */
/* choice = bond order */

typedef ValNodePtr PVNAL; /* Atom Location (AL) */
/* Node contains a PALD - Pointer to Atom Location Data */
/* choice = model id */

typedef ValNodePtr PVNMO; /* Model Object */
/* Node contains a PMOD - Pointer to Model Object Data */
/* choice = model id */

typedef ValNodePtr PVNMD; /* Model Density */
/* Node contains a PMDD - Pointer to Model Density Data */
/* choice = model id */

typedef DValNodePtr PDNML; /* Structure Model */
/* Node contains a BiostrucModelPtr */
/* choice = model id */

typedef DValNodePtr PDNSF; /* Structure Feature */
/* Node contains a BiostrucFeatureSetPtr */
/* choice = feature number */

/***************************/
/*MMDBAPI NODE DECLARATIONS*/
/***************************/

/**************/
/* Flag Block */
/**************/

/* COMMON TO MSD, MMD, MGD, MAD, MOD, MDD and MBD: */
/* if you have a PMxD, can cast as a "PFB" and get the correct type */
 
typedef struct nlm_flagblock { 
  struct nlm_flagblock PNTR pfbParent; 
  /* pointer to parent Data node (for MSD it is NULL) */
  Byte bMe; /* Me byte - identifies typecast and display triage */
  Byte bWhat; /* What is in me? */
  Byte bUpdate; /* byte to indicate status */
  Byte bReserved;
  Int4Ptr pI4vFeatID;  /* Feature Id */ 
  PointerPtr ppvFeatData; /* vector containing pointers */
  Int2 iNumFeats;  /* number of Features registered in node */
#ifdef BIG_CODE
  Byte bReserved1;  /*  */
  Byte bUser1;
  Int2 i2Reserved;
  Int2 i2User;
#endif
}FB,  *PFB, **PPFB;


/*************************/
/*  Feature Handlers     */
/*************************/

typedef DValNodePtr PDNTRN;

typedef DValNodePtr PDNSFS; /* Structure Feature Set*/
/* Node contains a PSFS - Pointer to Structure Feature Set */
/* choice = Feature Set Id  - each "session" is a new feature set */
 
typedef void (LIBCALLBACK *PColor3DFunc)(ColorPropPtr pcpThis, Uint1  PNTR pIndex);

typedef void (LIBCALLBACK *PStrFeatFunc)(Pointer PNTR ppCUserData, 
				         UserObjectPtr PNTR ppASNUserObj);

/*  New uses nothing, returns NEW pointer value in pCUserData */
/*  Free frees what is passed in ppCUserData */
/*  ToC  uses ppASNUserObj, allocates anc copies, creating NEW pointer 
    in ppCUserData  - does the allocation using the New fn. if necessary */
/*  ToASN uses ppCUserData, returns NEW pointer 
    in pASNUserObj  - does the allocation of the UserData  */
/*  ToASN is automatically used upon writing to stream. - its object is Free-ed */
/*  with UserObjectFree(*ppASNUserObj) */
 
/* if the feature type has registered functions, it is automatically   */
/* appended as a C structure on each appropriate node, and the ASN.1   */
/* is deleted.  When a call is made to save the Biostruc, it is        */
/* reconstructed, and the features are reconstructed from the feature  */
/* registry.  Afterward, the feature ASN.1 structures are freed.       */
 
typedef ValNodePtr PVNSFF;  /* linked list of SFF */
/* choice = feature type code */   
 
typedef struct Nlm_sff
    { /* structure feature functions */
      PStrFeatFunc pFeatNewFn;
      PStrFeatFunc pFeatFreeFn;
      PStrFeatFunc pFeatToASNFn;
      PStrFeatFunc pFeatToCFn;
    }SFF, *PSFF;

typedef struct Nlm_sfd  
{
  struct Nlm_sfd PNTR next;
  PDNSFS pdnsfsSet;
  Int4 iId;
  Int4 iType;
  CharPtr pcName;
  Int2 iColor;  /* table integer index */
  Int2 iRender;  /* render code from spec */
  Byte bPFBtype;  /* type of node feature applies to */
  Pointer pData; /* transform, camera, script, align in ASN.1 format  */
  BiostrucFeaturePtr pbsfFeat; /* stub for ASN.1 used only for write */        
}SFD,  *PSFD;       

typedef struct Nlm_sfs  
{
  Int4 iId;
  PDNSFS pdnsfsSet;
  Boolean SetisOn;
  CharPtr pcName;
  PSFD   psfdFeats;
  ValNodePtr pvnDescr;
  BiostrucFeatureSetPtr pbsfsFeatSet;  /* ASN.1 stub only for write */
}SFS,  *PSFS;       


/************************/
/* Model Structure Data */
/************************/
typedef struct Nlm_msd  
{
  /* flag block */
  PFB pfbParent; 
  Byte bMe; 
  Byte bWhat;  
  Byte bUpdate; 
  Byte bReserved; 
  Int4Ptr pI4vFeatID;  
  PointerPtr ppvFeatData; 
  Int2 iNumFeats;  
#ifdef BIG_CODE
  Byte bReserved1;  /*  */
  Byte bUser1;
  Int2 i2Reserved;
  Int2 i2User;
#endif
  /* bookkeeping block */
  Byte bVisible; /* can it be seen? */ 
  Byte bMaster; /* am I a master struct? */
  Byte bAligned; /* master: how many slaves? */
  BytePtr pbAligned;  /* slave: pointer to master bAligned */
  PDNMS pdnmsLink; 
  CharPtr pcSName; /* the "retrieval" string */
  Int2 iType; /* encodes pcSName as net id or filename for retrieve */
  BiostrucPtr pbsBS;  /* the ASN.1 Biostruc is kept here, usually */ 
  /* "header" with {id, desc, history} only */
  /* but during write operations it is to be */
  /* fully reconstructed. */
  ResidueGraphPtr pDictLocal;  /* the local ASN.1 dictionary */
  /* make this into a linked-list of dictionaries with id's */
  BiomolDescrPtr pGraphDescr;  /* The chemical graph ASN.1 descr (ValNode) */
  CharPtr pcPDBName;  /* from ASN.1 Biostruc descr field */
  Int4 iMMDBid;  /* MMDB unique number */
  CharPtr pcChemName; /* from ASN.1 chem graph descr field */
  CharPtr pcPdbClass; 
  CharPtr pcPdbSource; 
  Int2 iModels;  /* Number of Models present */
  Int2 iActiveModel; /* Index of Active Model */
  Int2 iFeatures;
  Int2 iPDBType;  /* enum from ASN.1 */
  Int4 iMolCount; /* number of molecules */
  Int4 iObjCount; /* number of objects */
  Int4 iDensCount; /* number of densities */
  Int4 iIMBCount; /* number of inter-molecule bonds */
  Int4 iHashChange; /* iModels+iFeatures+iObjCount+iDensCount + #ALD's */
  /* data block */
  Int2 iMimeType;
  PDNML pdnmlModels; /* contains PMLD with Models that are */ 
  /*   PDNSF pdnsfFeatures; */  /*  BiostrucFeatureSetPtrs */ 
  /* moved to psastrucAlignment */
  PDNSFS pdnsfsHead;  /* the feature registry for this structure */ 
  PDNMM pdnmmHead; /* the list of molecules (children) */          
  PVNMB pvnmbIMBHead;  /* Inter-molecule bonds in structure (children) */
  PVNMO pvnmoHead; /* the list of model objects */
  PVNMD pvnmdHead; /* the list of model densities */
  FloatLoPtr PNTR ppflBoundBox; /* matrix 2x3 with max & min XYZs */     
  /*  When needed, Allocated with:  ppflBoundBox = FLMatrix(0,2,0,3); */
  FloatLoPtr PNTR ppflRotate; /* matrix 3x3 with rotations for structure */
  FloatLoPtr pflTranslate; /* 3 - vector with translations for structure */
  PDNMS pdnmsSlaves;  /* slave structures in multiple alignments */
  BiostrucAnnotSetPtr psaStrucAlignment;
  VoidPtr pGlobalPARS; /* will hold pointer to default "global" PARS for this structure */
  ValNodePtr pdnSlaveToMasterTransforms; /* store transformation for slave onto master */
} MSD,  *PMSD;
 

/***********************/
/* Model Molecule Data */
/***********************/
typedef struct Nlm_mmd 
{
  /* flag block */
  PFB pfbParent; 
  Byte bMe; 
  Byte bWhat;  
  Byte bUpdate;
  Byte bReserved;   
  Int4Ptr pI4vFeatID;  
  PointerPtr ppvFeatData; 
  Int2 iNumFeats;  
#ifdef BIG_CODE
  Byte bReserved1;  /*  */
  Byte bUser1;
  Int2 i2Reserved;
  Int2 i2User;
#endif
  /* bookkeeping block */
  Byte bVisible;  /* to control show/off on MM level -- Yanli  */
  PDNMM pdnmmLink;
  ValNodePtr pMolDescr; /* the ASN.1 molecule descr */
  ValNodePtr pSeqId;
  CharPtr pcMolName; /* from ASN.1 "chain" descr field */
  Int4 iChainId;
  Int4 iGi;
  Int4 iResCount; /* number of graphs */
  Int4 iIRBCount; /* number of inter-res bonds */
  CharPtr pcSeqId;
  CharPtr pcSequence;
  PDNMG pdnmgStartSelect, pdnmgEndSelect;  /* used to select a range of residues in the molecule.  lyg */
  Int4 iTargetRow; /* corresponding row in the original ASN1 alignment */
  /* data block */
  PDNMG pdnmgHead;  /* the list of model graphs (children)  */
  PVNMB pvnmbIRBHead;  /* Inter-residue bonds in molecule (children) */
  FloatLoPtr PNTR ppflBoundBox; /* matrix 2x3 with max & min XYZs */     
  ValNodePtr pvnContainedBy;
} MMD, *PMMD;


/********************/    
/* Model Graph Data */
/********************/
typedef struct Nlm_mgd  
{
  /* flag block */
  PFB pfbParent; 
  Byte bMe; 
  Byte bWhat;  
  Byte bUpdate;  
  Byte bReserved;
 	Int4Ptr pI4vFeatID;  
  PointerPtr ppvFeatData; 
  Int2 iNumFeats;  
#ifdef BIG_CODE
  Byte bReserved1;  /*  */
  Byte bUser1;
  Int2 i2Reserved;
  Int2 i2User;
#endif
  /* bookkeeping block */
  Byte bVisible;     /* control display at residue level */
  Byte bHighlighted;
  Byte bNCBISecStru;
  Byte bPDBSecStru;
  PDNMG pdnmgLink;
  Int2 iDomain;   /* NCBI assigned domain number */
  CharPtr pcGraphName;  /* PDB 3-letter code */
  CharPtr pcGraphNum; /* the PDB numbering string e.g . 38A */
  Int4 iIDict;  /* index into dictionary */
  CharPtr pcIUPAC; /* IUPAC name X = invalid graph code */ 
  CharPtr pcNCBISS; /* pointer into the NCBI ss features string */
  CharPtr pcPDBSS; /* pointer into the PDB ss features string */
  CharPtr pcPDBComment; /* pointer into name of heterogen in dict */
  Int4 iAtomCount;
  Int4 iBondCount;
  /* data block */
  PVNMA pvnmaAHead; /* the atom list (children) */
  PVNMB pvnmbBHead; /* the bond list (children) */
  FloatLoPtr PNTR ppflBoundBox; /* matrix 2x3 with max & min XYZs */     
  ValNodePtr pvnContainedBy;
  BytePtr pbMasterReserved;
  /* will contain list of pointers to PARS structures - "global" has choice = 0,
  * "special style" has choice >= 1 */
  ValNodePtr pvnPARSList;
} MGD, *PMGD;


/*******************/
/* Model Atom Data */
/*******************/
typedef struct Nlm_mad  
{
  /* flag block */
  PFB pfbParent; 
  Byte bMe; 
  Byte bWhat;  
  Byte bUpdate;  
  Byte bReserved; 
  Int4Ptr pI4vFeatID;  
  PointerPtr ppvFeatData; 
  Int2 iNumFeats;  
#ifdef BIG_CODE
  Byte bReserved1;  /*  */
  Byte bUser1;
  Int2 i2Reserved;
  Int2 i2User;
#endif
  /* bookkeeping block */
  PVNMA pvnmaLink; /* element number is pvnmaLink->choice */
  CharPtr pcAName; /* PDB name in the DICTIONARY!! */
  Int2 iIndex; /* MMDB index */
  /* data block */
  ValNodePtr pvnBonds; 
  ValNodePtr pvnContainedBy;
  PVNAL pvnalLocate;
}MAD,  *PMAD;


/**********************/   
/* Atom Location Data */
/**********************/
typedef struct Nlm_ald
{
  /* flag block */
  PFB pfbParent; 
  Byte bMe; 
  Byte bWhat;  
  Byte bUpdate;  
  Byte bReserved;
  Int4Ptr pI4vFeatID;  
  PointerPtr ppvFeatData; 
  Int2 iNumFeats;  
#ifdef BIG_CODE
  Byte bReserved1;  /*  */
  Byte bUser1;
  Int2 i2Reserved;
  Int2 i2User;
#endif
  struct  Nlm_ald PNTR next;
  PVNAL pvnalLink; /* link to get model id */ 
  Int4 iUniqueId;  
  Char  cAltConf; /* if = " " */
  Int1 iCoordSet;  /* index from ASN.1 model coord set */
  Int1 iFloatNo;
  FloatLoPtr pflvData;
  Pointer pGraphic;  /* pointer to graphic segment */
} ALD, *PALD;


/*  pflvData is a vector containing...
  its size is dependent on the amount of data 
i.e. 3 = XYZ only
     4 = XYZ + occupancy
     5 = XYZ + occupancy + isotropic Temp
    10 = XYZ + occupancy + anisotropic Temp
Indexed as follows...
   [0]    X;
   [1]    Y;
   [2]    Z;
   [3]    Occ;
   [4]    B11;
   [5]    B12;
   [6]    B13;
   [7]    B22;
   [8]    B23;
   [9]    B33;
*/


/*******************/
/* Model Bond Data */
/*******************/
typedef struct Nlm_mbd  
{
  /* flag block */
  PFB pfbParent; 
  Byte bMe; 
  Byte bWhat;  
  Byte bUpdate;  
  Byte bReserved;
  Int4Ptr pI4vFeatID;  
  PointerPtr ppvFeatData; 
  Int2 iNumFeats;  
#ifdef BIG_CODE
  Byte bReserved1;  /*  */
  Byte bUser1;
  Int2 i2Reserved;
  Int2 i2User;
#endif
  /* bookkeeping block */
  PVNMB pvnmbLink;  /*  ASN.1 defined types is in pvnmbLink->choice */
  /* data block */
  PMAD pmadFrom;
  PMAD pmadTo;
}MBD,  *PMBD;       


/*********************/
/* Model Object Data */
/*********************/
typedef struct Nlm_mod  
{
  /* flag block */
  PFB pfbParent; 
  Byte bMe; 
  Byte bWhat;  
  Byte bUpdate;  
  Byte bReserved;
  Int4Ptr pI4vFeatID;  
  PointerPtr ppvFeatData; 
  Int2 iNumFeats;  
#ifdef BIG_CODE
  Byte bReserved1;  /*  */
  Byte bUser1;
  Int2 i2Reserved;
  Int2 i2User;
#endif
  /* bookkeeping block */
  PVNMO pvnmoLink;
  ValNodePtr pvnContains;
  struct  Nlm_mld PNTR pmldCoordSet; 
  Byte bVisible;   /* control object display to synchronize with its partner display -- Yanli  */
  /* data block */
  Int4  	   iCoordNo;
  FloatLoPtrPtr ppflObject;  /* coordinate lists */
  UcharPtr pucSwap;          /* same length as coord list */
  Int4	   iTriNo;
  Int4PtrPtr ppi4Triangles;  /* triangles list */
  FloatLo flRadius;     
  Int2Ptr pi2vColor;  /* colors for each triangle */
  /* cast a matrix Floating pt N x 3 for data */
  /* length 0 for sphere */
  /*        1 for cylinder */
  /*        7 for brick */
  /*        n for surface */
#ifdef VIEW_CODE
  /* logic to pass surface as optimized t-mesh */
  /* regardless of order of points */
  /* can view surface as dots or as t-mesh */
  /* display block */
#endif
}MOD,  *PMOD;       


/**********************/
/* Model Density Data */
/**********************/
typedef struct Nlm_mdd  
{
  /* flag block */
  PFB pfbParent; 
  Byte bMe; 
  Byte bWhat;  
  Byte bUpdate;  
  Byte bReserved;
  Int4Ptr pI4vFeatID;  
  PointerPtr ppvFeatData; 
  Int2 iNumFeats;  
#ifdef BIG_CODE
  Byte bReserved1;  /*  */
  Byte bUser1;
  Int2 i2Reserved;
  Int2 i2User;
#endif
  /* bookkeeping block */
  PVNMD pvnmdLink;
  struct  Nlm_mld PNTR pmldCoordSet;
  /* pointer to the ASN.1 data */
  /* data block */
  /* put the floating point Brick values into a matrix */
  /* cast the density matrix into XYZ points in a n x 3 matrix */
  /* make a connectivity matrix m x 2 of integer array indices */
  /* cast the contents into a ValNode matrix containing PFB's */
  ValNodePtr pvnContains;
#ifdef VIEW_CODE
  /* display the Brick as lines */
  /* display the intensity as dots or connected lines */
  /* cast the intensity value into colors in an Int2 n-vector  */
  /* cast the intensities into display layers - and Int2 n-vector */

  /* display block */
#endif
}MDD,  *PMDD;       


/**********************/   
/*Model Location Data */
/**********************/
/* these hold each coordset's data 
 * can be either atomic, object or density data
 * if Atomic, there needs to be a vector of 
 * pointers to each and every ALD in the model
 * This vector allows indexing features to atom locs
 */
typedef struct Nlm_mld
{  
  /* tiny flag block */
  PMSD    pmsdParent;  /* Parent Structure*/
  struct  Nlm_mld PNTR next;
  Byte    bSelected; /* for saving/conversion to other file formats */
  Byte    bReserved;
  Int2 iType;  
  Int2 iId;
  CharPtr pcModelName;
  CharPtr pcAltConf; /* A string of all AltConfs in model */
  Int2 iNoCoordSet;  /* index from ASN.1 model coord set */   
  PointerPtr ppAsnOrder; /* vector containing all PALD's if atomic data */
  PMOD pmodObject;  /* if model coord set is object */
  PMDD pmddDensity;  /* if model coord set is density */
  BiostrucModelPtr pbsmModel; 
} MLD, *PMLD;


typedef BiostrucResidueGraphSetPtr PRGD;
typedef void (LIBCALLBACK * pNodeFunc) PROTO((PFB pfbThis,Int4 iModel, Int4 iIndex, Pointer ptr));

/***********************/
/*       MMDBAPI       */  
/* Function Prototypes */
/***********************/

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

PMBD LIBCALL NewMBD(void);
PMAD LIBCALL NewMAD(void);
PMLD LIBCALL NewMLD(void);
PALD LIBCALL NewALD(void);
PMGD LIBCALL NewMGD(void);
PMMD LIBCALL NewMMD(void);
PMSD LIBCALL NewMSD(void);
PMOD LIBCALL NewMOD(void);
PMDD LIBCALL NewMDD(void);
PSFF LIBCALL NewSFF PROTO((void));
PSFD LIBCALL NewSFD PROTO((void));
PSFS LIBCALL NewSFS PROTO((void));
void LIBCALL FreeSFS PROTO((PSFS psfsThis));
void LIBCALL FreeSFF PROTO((PSFF psffThis));
void LIBCALL FreeSFD PROTO((PSFD psfdThis));void LIBCALL FreeMBD(PMBD pmbdThis);
void LIBCALL FreeMAD(PMAD pmadThis);
void LIBCALL FreeALD(PALD paldThis);
void LIBCALL FreeMGD(PMGD pmgdThis);
void LIBCALL FreeMMD(PMMD pmmdThis);
void LIBCALL FreeMSD(PMSD pmsdThis);
void LIBCALL FreeMOD(PMOD pmodThis);
void LIBCALL FreeMDD(PMDD pmddThis);
void LIBCALL FreeMLD(PMLD pmldThis);
void LIBCALL FreeKeptModel(PMLD pmldThis);
void LIBCALL FreeKeptFeature(BiostrucFeatureSetPtr pbsfsThis);
PVNMB LIBCALL NewVNMB(PVNMB PNTR ppvnmbList,  Int2 choice);
PVNMO LIBCALL NewVNMO(PVNMO PNTR ppvnmoList,  Int2 choice);
PVNMD LIBCALL NewVNMD(PVNMD PNTR ppvnmdList,  Int2 choice);
PVNMA LIBCALL NewVNMA(PVNMA PNTR ppvnmaList,  Int2 choice);
PVNAL LIBCALL NewVNAL(PVNAL PNTR ppvnalList, Int2 choice);
PDNMG LIBCALL NewDNMG(PDNMG PNTR ppdnmgList,  Int2 choice);
PDNMM LIBCALL NewDNMM(PDNMM PNTR ppdnmmList,  Int2 choice);
PDNMS LIBCALL NewDNMS(PDNMS PNTR ppdnmsList,  Int2 choice);
ValNodePtr LIBCALL FreeVNDataFn (ValNodePtr vnp,  pFreeFunc freefn);
void LIBCALL FreeListVNAL(PVNAL pvnalList);
void LIBCALL FreeListDNML(PDNML pdnmlList);

PVNSFF LIBCALL NewVNSFF(PVNSFF PNTR ppvnsffList,  Int2 choice);
PDNSFS LIBCALL NewDNSFS(PDNSFS PNTR ppdnsfsList,  Int2 choice);
void LIBCALL FreeListDNSFS(PDNSFS pdnsfsList);
void LIBCALL FreeListVNSFF(PVNSFF pvnsffList);

void LIBCALL FreeListDNSF(PDNSF pdnsfList);
void LIBCALL FreeListVNMB(PVNMB pvnmbList);
void LIBCALL FreeListVNMO(PVNMO pvnmoList);
void LIBCALL FreeListVNMD(PVNMD pvnmdList);
void LIBCALL FreeListVNMA(PVNMA pvnmaList); 
PDNMG LIBCALL FreeDNMG(PDNMG pdnmgList, PDNMG pdnmgThis);
void LIBCALL FreeListDNMG(PDNMG pdnmgList);
PDNMM LIBCALL FreeDNMM(PDNMM pdnmmList,  PDNMM pdnmmThis);
void LIBCALL FreeListDNMM(PDNMM pdnmmList);
PDNMS LIBCALL FreeDNMS(PDNMS pdnmsList, PDNMS pdnmsThis);
NLM_EXTERN void LIBCALL FreeListDNMS(PDNMS pdnmsList);
void LIBCALL FreeAtomicModelAsnLists(AtomicCoordinatesPtr pacThis);
NLM_EXTERN PRGD LIBCALL LoadDict(CharPtr pcDictName);

PMAD LIBCALL AtomFromMMDBIndex(PDNMS pdnmsList, Int2 iStru, Int2 iMol, Int2 iGraph, Int4 iAtom);
CharPtr MyType(PFB pfbThis);
PMGD LIBCALL GraphFromMMDBIndex(PDNMS pdnmsList, Int2 iStru, Int2 iMol, Int2 iGraph );
PMMD LIBCALL MolFromMMDBIndex(PDNMS pdnmsList, Int2 iStru, Int2 iMol);

BiostrucPtr LIBCALL FetchBS(CharPtr pcFetch,  Int2 iType, Int4 mdlLvl, 
		Int4 maxModels, Byte bExtent);
Boolean LIBCALL AssignSurfaceContents(PDNMS pdnmsList, Int2 iModel, PFB pfbThis, ChemGraphPntrsPtr pcgpContents);
void LIBCALL FreeSurfaceModelAsnList(SurfaceCoordinatesPtr pscThis); 
Boolean LIBCALL FillSurface(SurfaceCoordinatesPtr pscThis, PMOD pmodThis);
Int2 LIBCALL BiostrucModel2ModelStruc( BiostrucModelPtr pbsmThis, PDNMS pdnmsThis, 
		Int2 iChoice, Int2 iModelCount, Byte bExtent);
ValNodePtr LIBCALL MakeChemGraphNodeList(PDNMS pdnmsThis,  ChemGraphPntrsPtr pcgpThis);

ValNodePtr LIBCALL MakeRegionNodeList(PDNMS pdnmsThis, RegionPntrsPtr prgpThis);
void LIBCALL ResolveAlignChain(PDNMS pdnmsThis);
Boolean LIBCALL SetNodeFeatureData(PFB pfbThis, PSFD  psfdThis);

/***************** modified 4/26/99 KA to handle VAST chains **************/
void LIBCALL InstallAlignedSlave(PDNMS pdnmsMaster, ChemGraphAlignmentPtr pcgaAlign,
				 PSFD psfdThis, Boolean Chain, CharPtr SlaveChain);

PSFD LIBCALL InstallStrucFeature(PDNMS pdnmsThis, PDNSFS pdnsfsThis, BiostrucFeaturePtr pbsfThis,
				 Boolean Chain, CharPtr SlaveChain);

Int2 LIBCALL BiostrucAddFeature(BiostrucFeatureSetPtr pbsfsThis, PDNMS pdnmsThis,
				Boolean Chain, CharPtr SlaveChain);
/*************************************************************************/

PDNTRN LIBCALL NewDNTRN(PDNTRN PNTR ppdntrnList,  Int2 choice);
void LIBCALL FreeDNTRN(PDNTRN pdntrnList);
void LIBCALL TransformToDNTRN(PDNTRN PNTR pplist,  TransformPtr transform);
void LIBCALLBACK DoApplyTransform(PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr);
void LIBCALLBACK DoReverseTransform(PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr);
BiostrucAnnotSetPtr LIBCALL  BiostrucAnnotSetGetByFid (BiostrucAnnotSetPtr basp, Int4 feature_id, Int4 feature_set_id);
PDNMS LIBCALL FindLoadedBiostruc(CharPtr pcPDBID, Int4 iId);

/***************** modified 4/26/99 KA to handle VAST chains **************/
Boolean InstBSAnnotSet(BiostrucAnnotSetPtr pbsasThis, CharPtr JobID, Boolean Chain, CharPtr Path);

/****** DZ: moved from mkbioseqB.c 4/27/99 KA to support PruneBiostruc ******/

Boolean LIBCALL isBiopoly(Int4 molecule_id, MoleculeGraphPtr currentbp);
Boolean LIBCALL isHet(Int4 molecule_id, MoleculeGraphPtr currenthet);

/*************************************************************************/

SeqAnnotPtr LIBCALL BiosToSeq (BiostrucAnnotSetPtr set, Boolean usePValue, 
                               Char* pdbname_master, Char* pdbname_slave);
SeqAnnotPtr LIBCALL BiostrToSeqAnnotSet (BiostrucAnnotSetPtr set, 
                               Boolean usePValue, Char* pdbname_master);
int LIBCALLBACK CompareScores(VoidPtr vp1, VoidPtr vp2);
void LIBCALL VastTableSort(BiostrucFeatureSetPtr pbsfs,  Int2 iSort);
SeqAnnotPtr LIBCALL fnPBSFtoPSA (BiostrucFeaturePtr pbsfSelected);

Byte LIBCALL SetBondOrder(Byte bThis,  Int2 iType);
NLM_EXTERN PDNMS LIBCALL Biostruc2Modelstruc(PDNMS PNTR ppdnmsList, BiostrucPtr pbsBS, 
                     PRGD prgdDict, Byte  bExtent, Int2 iChoice,  
                         CharPtr pcFetch, Int2 iType );
Int2 LIBCALL TraverseAll( DValNodePtr pdnModel, Int4 iModel, Int4 iIndex, 
			  Pointer ptr, pNodeFunc pfnCallMe);
AlternateConformationIdsPtr LIBCALL AlternateConformationIdsFree (AlternateConformationIdsPtr );

/*  Function to retrieve Biostrucs from ASN.1 files  */
NLM_EXTERN BiostrucPtr LIBCALL BiostrucAsnGet(AsnIoPtr aip, AsnTypePtr atp, Int4 mdlLvl, Int4 maxModels);

NLM_EXTERN Boolean LIBCALL BiostrucAvail (void);

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
