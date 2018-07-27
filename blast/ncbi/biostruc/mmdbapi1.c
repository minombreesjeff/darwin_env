/*   mmdbapi1.c
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
* File Name:  mmdbapi1.c
*
* Author:  Christopher Hogue,  Hitomi Ohkawa
*
* Version Creation Date:   03/14/95
*
* $Revision: 6.47 $
*
* File Description:
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 95/08/28 C. Hogue    Major change to new Biostruc ASN.1 v4.0,
*                      Addition of Features, Secondary Structure, Ensembles,
*		       Model nodes and Dictionaries altered...
* 95/08/28 C. Hogue    Put model id into its DValnode choice as it should be.
* 95/08/30 C. Hogue    Minor changes.
*
* $Log: mmdbapi1.c,v $
* Revision 6.47  2005/01/25 15:39:07  thiessen
* tweak LoadDict again
*
* Revision 6.46  2005/01/24 20:42:58  thiessen
* fix memory leak; check current dir for dictionary if ProgramPath returns empty string
*
* Revision 6.45  2004/05/06 19:31:09  chenj
* fixed the bug in fnPBSFtoPSA() to use 2 chars for domain id
*
* Revision 6.44  2003/12/03 02:11:28  kans
* added defines missing from Mac OS 10.3 headers
*
* Revision 6.43  2002/05/10 16:33:18  chenj
* fix bugs in fnPBSFtoPSA
*
* Revision 6.41  2001/01/26 15:06:39  lewisg
* use entrez2 to retrieve structures
*
* Revision 6.40  2000/12/05 22:13:06  ywang
* fix bugs for MakePDBSeqId2
*
* Revision 6.39  2000/07/21 18:56:41  thiessen
* allow dynamic slave->master transformation
*
* Revision 6.38  2000/06/09 14:35:16  lewisg
* fix freeing bugs
*
* Revision 6.37  2000/04/20 23:27:44  lewisg
* misc bug fixes
*
* Revision 6.36  2000/03/13 15:51:25  thiessen
* added storage for PARS to each residue for Cn3D; removed unused fields
*
* Revision 6.35  2000/03/09 17:52:56  thiessen
* changes to MSD, MGD for feature implementation in Cn3D
*
* Revision 6.34  2000/03/08 21:46:12  lewisg
* cn3d saves viewport, misc bugs
*
* Revision 6.33  2000/02/14 19:31:03  lewisg
* fix occupancy bug when temperature factors present
*
* Revision 6.32  1999/11/02 23:06:08  lewisg
* fix cn3d to launch correctly if there is no seqentry associated with bioseq
*
* Revision 6.31  1999/10/05 23:18:17  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.30  1999/06/07 21:23:36  ywang
* add iUserDefinedFeatureOld, FeatureOnOld to MGD
*
* Revision 6.29  1999/05/07 20:48:59  kans
* including new name of prunebsc.h
*
* Revision 6.28  1999/05/07 14:07:07  zimmerma
*  new code to call prunebiostruc (lines starting at 3807)
*
*  InstBSAnnotSet - added params: Boolean Chain, CharPtr Path and
* 		  modified to call PruneBiostruc (KA)
*
*  InstallAlignedSlave -  added params: Boolean Chain, CharPtr Path and
* 			modified to call PruneBiostruc (KA)
*
*  InstallStrucFeature -  added params: Boolean Chain, CharPtr Path and
* 			propagate to InstallAlignedSlave (KA)
*
*  BiostrucAddFeature -  added params: Boolean Chain, CharPtr Path and
* 			propagate to InstallStrucFeature (KA)
*
*  Moved isBiopoly and isHet from mkbioseqB.c - DZ
*
* Revision 6.27  1999/04/26 20:49:59  lewisg
* changed arguments named list to fix visual c++ bug
*
* Revision 6.26  1999/03/18 21:04:29  lewisg
* reverse transform for saving
*
* Revision 6.25  1999/03/01 20:22:10  ywang
* add bTurnedOff flag on MGD
*
* Revision 6.24  1999/02/25 23:11:32  ywang
* keep ASN.1 features intact
*
* Revision 6.23  1999/02/24 22:58:09  ywang
* add iMimeType to MSD node and other minor name change
*
* Revision 6.22  1999/02/02 22:27:00  ywang
* add bJustHighlighted flag to MGD for feature edit
*
 * Revision 6.21  1998/12/16  19:30:12  ywang
 * add flag for highlight status to MGD
 *
 * Revision 6.20  1998/11/06  23:02:01  ywang
 * add FeatureOn to MGD for feature on/off control
 *
 * Revision 6.19  1998/11/04  00:53:39  ywang
 * add iFeature to MGD for modeling control
 *
 * Revision 6.18  1998/10/21  15:43:21  ywang
 * attach the whole vast alignment data to master structure
 *
 * Revision 6.17  1998/10/01  21:56:33  ywang
 * set display flag for object
 *
 * Revision 6.16  1998/09/22  17:52:20  ywang
 * add flag for display control on MM and MG level
 *
 * Revision 6.15  1998/08/26  18:02:39  kans
 * fixed -v -fd warnings
 *
* Revision 6.14  1998/08/14 20:24:05  lewisg
* fix fault when retrieving structure by PDB name and database is unavailable
*
* Revision 6.13  1998/07/01 22:22:08  ywang
* set getgi flag as FALSE in function fnPBSFtoPSA when mastersip and slavesip is made by MakePDBSeqId2
*
 * Revision 6.12  1998/05/12  21:46:16  lewisg
 * stricter conservation coloring
 *
* Revision 6.11  1998/05/08 21:19:30  ywang
* make salp->segs as DenseDiag
*
* Revision 6.9  1998/04/28 22:51:13  lewisg
* created separate pointer to master in MGD
*
* Revision 6.8  1998/04/21 22:58:46  lewisg
* eliminate dead code
*
* Revision 6.7  1998/04/10 22:41:42  lewisg
* eliminated dead code and added a visible bit to MSD
*
* Revision 6.6  1998/04/04 06:01:24  lewisg
* got rid of dos line breaks
*
* Revision 6.5  1998/04/04 00:56:46  lewisg
* corrected freeing of MSD
*
* Revision 6.4  1998/03/10 04:30:59  lewisg
* change name of MakePDBSeqId to MakePDBSeqId2 to avoid name conflict
*
* Revision 6.3  1998/03/06 23:17:15  lewisg
* codewarrior fixes
*
* Revision 6.2  1998/03/06 01:15:43  lewisg
* merge
*
* Revision 6.0  1997/08/25 18:10:56  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/04/18 19:18:56  hogue
* Int2 counters for atoms upgraded to Int4's, made ppAsnOrder alloc optional.
*
 * Revision 5.2  1997/01/15  18:15:04  hogue
 * Changed to abstraction of Entrez calls requiring mmdbentr or equivalent.  This abstracts the Entrez (CD/Network) calls from MMDB-API,
 * in preparation to fetching MMDB records in alternate ways
 * (local filesystem or http GET with MIME-typed Biostrucs) and for
 * providing a slot for an MMDB-API caching system.
 *
 * Revision 5.1  1996/11/18  19:58:51  hogue
 * eliminate ALLSIMPLEMDL to obtain performance improvement, Update
 * of MMDB data makes ALLSIMPLEMDL obsolete as vectors now stored in
 * the ONECOORDATOM model.
 *
 * Revision 5.0  1996/05/28  14:02:09  ostell
 * Set to revision 5.0
 *
 * Revision 1.25  1996/05/15  17:01:53  hogue
 * Fixed compiler warnings
 *
 * Revision 1.24  1996/05/01  14:39:12  hogue
 * Enhanced feature handling, added MakeChemGraphNodeList, added support for domains.
 *
 * Revision 1.23  1996/04/18  17:40:05  hogue
 * Added FETCH_ENTREZ bExtent test to FetchBS to mask EntrezInit/Fini; added mdlLvl BSEVERYTHING support, added a freeer call for pdnmsSlaves.
 *
 * Revision 1.22  1996/03/29  19:28:02  hogue
 * Added pExtra pointer to MSD, fixed PMOD parent pointer
 *
 * Revision 1.21  1996/01/31  21:35:00  hogue
 * Changes to accomodate the free-ing of models, most dramatically is the
 * encapuslation of interim free-ers to FreeSurfaceModelAsnList and
 * FreeAtomicModelAsnLists.  KEEP_ASN no longer supported.  PDNSM and its
 * kin renamed to PDNML.
 *
 * Revision 1.20  1995/12/12  23:51:26  kans
 * removed include vibrant.h
 *
 * Revision 1.19  1995/11/30  20:31:24  hogue
 * Asn order vector in PMLD contains PALD not PMAD's.
 *
 * Revision 1.18  1995/11/15  19:15:59  hogue
 * Traversers now handle solids on a per-model basis.
 *
 * Revision 1.17  1995/11/09  22:53:15  hogue
 * Links between models and locations added, some new free-ers
 *
 * Revision 1.16  1995/10/05  17:41:52  hogue
 * Added rot/translation matrix free-ers
 *
 * Revision 1.15  1995/09/21  20:27:00  hogue
 * Took out call to StrToLong - inserted it longhand for non-vibrant apps
 *
 * Revision 1.14  1995/09/19  21:07:22  hogue
 * Added to FetchBS() Entrez access via PDB or GI
 *
 * Revision 1.13  1995/09/18  21:24:37  hogue
 * Het names added, and ProgMon calls
 *
 * Revision 1.12  1995/09/14  14:21:48  hogue
 * Fix for Element No's 254 (other) and 255 (unk) put into Int1
 *
 * Revision 1.11  1995/09/05  19:09:32  hogue
 * Changed error severity in conversion routine to SEV_ERROR
 *
 * Revision 1.10  1995/08/30  19:36:44  kans
 * numerous changes
 *
 * Revision 1.9  1995/08/29  20:03:06  kans
 * Entrez-like loading now enabled, models fixed
 *
 * Revision 1.6  1995/08/28  22:46:40  kans
 * moved BiostrucAvail to mmdbapi1.[ch]
 *
 * Revision 1.5  1995/08/28  20:06:37  kans
 * *** empty log message ***
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
*  mmdbapi1.c
*
*   UNDER CONSTRUCTION NOTICE.
*	    SUBJECT TO RADICAL CHANGES...
*
*  programmer C.W.V. Hogue
*  created: 14 Mar 95
*  last mod: 28 AUGUST 95
*****************************************************************************/
#include <ncbi.h>
#include <mmdbdata.h>
#include <sequtil.h>
NLM_EXTERN void VnpHeapSort PROTO ((ValNodePtr PNTR vnp, int (LIBCALLBACK *compar )PROTO ((Nlm_VoidPtr, Nlm_VoidPtr ))));
  /* this should be #include <utilpub.h> but this conflicts with mmdbdata.h. fix this. lyg */
#include <mmdbapi.h>
#include <mmdbapi1.h>
#include <matrix.h>
#include "prunebsc.h"

/* Missing from /usr/include/gcc/darwin/3.3/machine/limits.h */
#ifdef __MWERKS__
#ifdef OS_UNIX_DARWIN
#ifndef __SCHAR_MAX__
#define __SCHAR_MAX__ 127
#endif
#endif
#endif
/* End missing from /usr/include/gcc/darwin/3.3/machine/limits.h */

PMBD LIBCALL NewMBD(void)
{
    PMBD pmbdNew = NULL;
    pmbdNew = (PMBD)MemNew((size_t)(sizeof(MBD)));
    if (pmbdNew)
     {
      pmbdNew->bMe = (Byte) AM_MBD;
      pmbdNew->bUpdate = (Byte) (REG_STYLE | SHOW_ME);
     }
    return pmbdNew;
}


PMOD LIBCALL NewMOD(void)
{
    PMOD pmodNew = NULL;
    pmodNew = (PMOD)MemNew((size_t)(sizeof(MOD)));
    if (pmodNew)
     {
      pmodNew->bMe = (Byte) AM_MOD;
      pmodNew->bUpdate = (Byte) (REG_STYLE | SHOW_ME );
      pmodNew->bVisible = 1;
                   /* turn on object by default -- Yanli */
      }
    return pmodNew;
}


PMDD LIBCALL NewMDD(void)
{
    PMDD pmddNew = NULL;
    pmddNew = (PMDD)MemNew((size_t)(sizeof(MDD)));
    if (pmddNew)
     {
      pmddNew->bMe = (Byte) AM_MDD;
      pmddNew->bUpdate = (Byte) (REG_STYLE | SHOW_ME );
      }
    return pmddNew;
}

PMAD LIBCALL NewMAD(void)
{
    PMAD pmadNew = NULL;
      pmadNew = (PMAD)MemNew((size_t)(sizeof(MAD)));
    if (pmadNew)
     {
      pmadNew->bMe = (Byte) AM_MAD;
      pmadNew->bUpdate = (Byte) (REG_STYLE | SHOW_ME );
      }
    return pmadNew;
}

PALD LIBCALL NewALD(void)
{
    PALD paldNew = NULL;
    paldNew = (PALD)MemNew((size_t)(sizeof(ALD)));
    if (paldNew)
      {
        paldNew->cAltConf = ' ';
	paldNew->bMe = (Byte) AM_ALD;
	paldNew->bUpdate = (Byte) (REG_STYLE | SHOW_ME );
      }
    return paldNew;
}

PMLD LIBCALL NewMLD(void)
{
    PMLD pmldNew = NULL;
    pmldNew = (PMLD)MemNew((size_t)(sizeof(MLD)));
    return pmldNew;
}



PMGD LIBCALL NewMGD(void)
{
    PMGD pmgdNew = NULL;
    pmgdNew = (PMGD)MemNew((size_t)(sizeof(MGD)));
    if (pmgdNew)
     {
      pmgdNew->bMe = (Byte) AM_MGD;
      pmgdNew->bUpdate = (Byte) (REG_STYLE | SHOW_ME );
      pmgdNew->bReserved = 0;
      pmgdNew->bVisible = 1;   /* be displayed by default */
      pmgdNew->bHighlighted = 0;
      pmgdNew->pbMasterReserved = NULL;
      pmgdNew->pvnPARSList = NULL;
     }
    return pmgdNew;
}

PMMD LIBCALL NewMMD(void)
{
    PMMD pmmdNew = NULL;
    pmmdNew = (PMMD)MemNew((size_t)(sizeof(MMD)));
    if (pmmdNew)
     {
      pmmdNew->bMe = (Byte) AM_MMD;
      pmmdNew->bUpdate = (Byte) (REG_STYLE | SHOW_ME );
      pmmdNew->bVisible = 1;
      pmmdNew->iTargetRow = -1;
      }
      return pmmdNew;
}

PMSD LIBCALL NewMSD(void)
{
    PMSD pmsdNew = NULL;
    pmsdNew = (PMSD)MemNew((size_t)(sizeof(MSD)));
    if (pmsdNew)
     {
      pmsdNew->bMe = (Byte) AM_MSD;
      pmsdNew->bUpdate = (Byte) (REG_STYLE | SHOW_ME );
      pmsdNew->pfbParent = NULL;
      pmsdNew->pGlobalPARS = NULL;
      pmsdNew->pdnSlaveToMasterTransforms = NULL;
     }
    return pmsdNew;
}



PSFF LIBCALL NewSFF(void)
{
  PSFF psffNew = NULL;
  psffNew = (PSFF) MemNew((size_t)(sizeof(SFF)));
  return psffNew;
}

PSFD LIBCALL NewSFD(void)
{
    PSFD psfdNew = NULL;
    psfdNew = (PSFD) MemNew((size_t)(sizeof(SFD)));
    return psfdNew;
}


PSFS LIBCALL NewSFS(void)
{
    PSFS psfsNew = NULL;
    psfsNew = (PSFS)MemNew((size_t)(sizeof(SFS)));
    return psfsNew;
}

PDNSFS LIBCALL NewDNSFS(PDNSFS PNTR ppdnsfsList,  Int2 choice)
{
    PDNSFS pdnsfsNew = NULL;
    PSFS psfsNew = NULL;
    psfsNew = NewSFS();
    if (!psfsNew) return NULL;
    pdnsfsNew = DValNodeAddPointer(ppdnsfsList, choice, (Nlm_VoidPtr) psfsNew);
    psfsNew->pdnsfsSet = pdnsfsNew; /* link to parent */
    return pdnsfsNew;
}

void LIBCALL FreeSFS(PSFS psfsThis)
{

  if (psfsThis)
    {
  	if (psfsThis->pcName) MemFree(psfsThis->pcName);
        if (psfsThis->psfdFeats) FreeSFD(psfsThis->psfdFeats);
        if (psfsThis->pvnDescr) BiostrucFeatureSetDescrFree(psfsThis->pvnDescr);
        if (psfsThis->pbsfsFeatSet)
             MsgAlert(KEY_NONE,SEV_ERROR, "Dangling Feature Set",
		"\nMemory leaking at FeatureSet\n.");
            BiostrucFeatureSetFree(psfsThis->pbsfsFeatSet);
   	MemFree(psfsThis);
    }
  return;
}



PVNSFF LIBCALL NewVNSFF(PVNSFF PNTR ppvnsffList,  Int2 choice)
{
    PVNSFF pvnsffNew = NULL;
    PSFF psffNew = NULL;

    psffNew = NewSFF();
    if (!psffNew) return NULL;
    pvnsffNew = ValNodeAddPointer(ppvnsffList, choice, (Nlm_VoidPtr) psffNew);
    return pvnsffNew;
}


void LIBCALL FreeListVNSFF(PVNSFF pvnsffList)
{
  FreeVNDataFn(pvnsffList,  (pFreeFunc)FreeSFF);
  return;
}


void LIBCALL FreeSFF(PSFF psffThis)
{
  if (psffThis)
    {
     MemFree(psffThis);
    }
  return;
}

void LIBCALL FreeSFD(PSFD psfdThis)
{
  PSFD psfdNext = NULL;
  PSFF psffThis = NULL;
  ValNodePtr vnp = NULL;

  if (psfdThis)
    {
     if (psfdThis->next) psfdNext = psfdThis->next;
     if (psfdThis->pcName) MemFree(psfdThis->pcName);
     if (psfdThis->pData)
       {
        psffThis = DoesFeatureTypeHaveFuncs(psfdThis->iType);
        if (psffThis)
         {  /* this uses the installed function to free pData */
             (*(psffThis->pFeatToCFn)) ( (Pointer PNTR) &(psfdThis->pData), NULL);
         }
        else
        switch (psfdThis->iType)
          {
           case Feature_type_transform:
	      FreeDNTRN((PDNTRN) vnp->data.ptrvalue);
	     break;
           case Feature_type_camera:
	      vnp = (ValNodePtr) psfdThis->pData;
	      CameraFree((CameraPtr) vnp->data.ptrvalue);
	      ValNodeFree(vnp);
	     break;
           case Feature_type_script:
/* lygmerge	      vnp = (ValNodePtr) psfdThis->pData;
	      BiostrucScriptFree((BiostrucScriptPtr) vnp->data.ptrvalue);
	      ValNodeFree(vnp); */
	     break;

           case Feature_type_multalign:
           case Feature_type_alignment:

	      ChemGraphAlignmentFree((ChemGraphAlignmentPtr)  psfdThis->pData);
	      ValNodeFree(vnp);
	     break;
           case Feature_type_similarity:
	      vnp = (ValNodePtr) psfdThis->pData;
	      RegionSimilarityFree((RegionSimilarityPtr) vnp->data.ptrvalue);
	      ValNodeFree(vnp);
	     break;
           case Feature_type_region:
	      vnp = (ValNodePtr) psfdThis->pData;
	      RegionPntrsFree((RegionPntrsPtr) vnp->data.ptrvalue);
	      ValNodeFree(vnp);
             break;

              /* these don't have sub-alloced parts hanging off pData */
         /*  case Feature_type_indirect:
             case Feature_type_color:
             case Feature_type_render:
             case Feature_type_helix:
             case Feature_type_strand:
             case Feature_type_sheet:
             case Feature_type_turn:
             case Feature_type_site:
             case Feature_type_footnote:
             case Feature_type_subgraph:
             case Feature_type_core:
             case Feature_type_supercore:
	     case Feature_type_other: */

	     default: ;

          } /* switch type */
	 psfdThis->pData = NULL;
       } /* if pData */

     if (psfdThis->pbsfFeat)
        {
            MsgAlert(KEY_NONE,SEV_ERROR, "Dangling Feature",
		"\nMemory leaks at Feature\n.");
          BiostrucFeatureFree(psfdThis->pbsfFeat);
        }

     MemFree(psfdThis);
     FreeSFD(psfdNext);
    }
}



void LIBCALL FreeMBD(PMBD pmbdThis)
{
#ifdef _DEBUG_2
printf("in FreeMBD ");
#endif
    if (pmbdThis)
       MemFree(pmbdThis);
}

void LIBCALL FreeMOD(PMOD pmodThis)
{
#ifdef _DEBUG_2
printf("in FreeMOD ");
#endif
    if (pmodThis->pvnContains) ValNodeFree(pmodThis->pvnContains);
    if (pmodThis->ppflObject) FLMatrixFree(pmodThis->ppflObject, 0, 0);
    if (pmodThis->pucSwap) UCVectorFree(pmodThis->pucSwap, 0);
    if (pmodThis->ppi4Triangles) I4MatrixFree(pmodThis->ppi4Triangles, 0, 0);
    if (pmodThis->pi2vColor) I2VectorFree(pmodThis->pi2vColor,0);
    if (pmodThis) MemFree(pmodThis);
}

void LIBCALL FreeMDD(PMDD pmddThis)
{
#ifdef _DEBUG_2
printf("in FreeMDD ");
#endif
    if (pmddThis->pvnContains) ValNodeFree(pmddThis->pvnContains);
    if (pmddThis)
       MemFree(pmddThis);
}


void LIBCALL FreeMAD(PMAD pmadThis)
{
#ifdef _DEBUG_2
printf("in FreeMAD ");
#endif
    if (pmadThis)
     {
        if (pmadThis->pvnContainedBy) ValNodeFree(pmadThis->pvnContainedBy);
       if (pmadThis->pvnBonds) ValNodeFree(pmadThis->pvnBonds);
       if (pmadThis->pvnalLocate) FreeListVNAL(pmadThis->pvnalLocate);
       MemFree(pmadThis);
     }

}

void LIBCALL FreeALD(PALD paldThis)
{
  PALD paldNext;
#ifdef _DEBUG_2
printf("in FreeALD ");
#endif
  while (paldThis)
    {
        paldNext = paldThis->next;
        if ((paldThis->iFloatNo) && (paldThis->pflvData))
	  FLVectorFree(paldThis->pflvData, 0);
	MemFree(paldThis);
	paldThis = paldNext;
    }
}

void LIBCALL FreeMLD(PMLD pmldThis)
{
  PMLD pmldNext;
#ifdef _DEBUG_2
printf("in FreeMLD ");
#endif
  while (pmldThis)
    {
        pmldNext = pmldThis->next;
         /* models are owned by the Biostruc ptr in pmsd->pbsBS */
	if (pmldThis->ppAsnOrder) PTRVectorFree(pmldThis->ppAsnOrder, 0);
	if (pmldThis->pcAltConf) MemFree(pmldThis->pcAltConf);
	if (pmldThis->pcModelName) MemFree(pmldThis->pcModelName);
	MemFree(pmldThis);
	pmldThis = pmldNext;
    }
}



void LIBCALL FreeKeptFeature(BiostrucFeatureSetPtr pbsfsThis)
{
#ifdef _DEBUG_2
printf("in FreeKeptFeature ");
#endif
    if (pbsfsThis) BiostrucFeatureSetFree(pbsfsThis);
}


void LIBCALL FreeMGD(PMGD pmgdThis)
{
#ifdef _DEBUG_2
printf("in FreeMGD ");
#endif
   if (pmgdThis)
     {
      if (pmgdThis->pvnContainedBy) ValNodeFree(pmgdThis->pvnContainedBy);
      if (pmgdThis->pcGraphName) MemFree(pmgdThis->pcGraphName);
      if (pmgdThis->pcPDBComment) MemFree(pmgdThis->pcPDBComment);
      if (pmgdThis->pvnmaAHead) FreeListVNMA(pmgdThis->pvnmaAHead);
      if (pmgdThis->pvnmbBHead) FreeListVNMB(pmgdThis->pvnmbBHead);
      if (pmgdThis->ppflBoundBox) FLMatrixFree(pmgdThis->ppflBoundBox,0,0);
      if (pmgdThis->pcIUPAC) MemFree(pmgdThis->pcIUPAC);
      if (pmgdThis->pcGraphNum) MemFree(pmgdThis->pcGraphNum);
      MemFree(pmgdThis);
     }
}

void LIBCALL FreeMMD(PMMD pmmdThis)
{
#ifdef _DEBUG_2
printf("in FreeMMD ");
#endif
    if (pmmdThis)
      {
        if (pmmdThis->pvnContainedBy) ValNodeFree(pmmdThis->pvnContainedBy);
    	if (pmmdThis->pcMolName) MemFree(pmmdThis->pcMolName);
	if (pmmdThis->pcSeqId) MemFree(pmmdThis->pcSeqId);
	if (pmmdThis->pcSequence) MemFree(pmmdThis->pcSequence);
    	if (pmmdThis->pdnmgHead) FreeListDNMG(pmmdThis->pdnmgHead);
        if (pmmdThis->pvnmbIRBHead) FreeListVNMB(pmmdThis->pvnmbIRBHead);
        if (pmmdThis->ppflBoundBox) FLMatrixFree(pmmdThis->ppflBoundBox,0,0);
        if (pmmdThis->pMolDescr)
            AsnGenericChoiceSeqOfFree(pmmdThis->pMolDescr,(AsnOptFreeFunc)BiomolDescrFree);
	if (pmmdThis->pSeqId) SeqIdFree(pmmdThis->pSeqId);
        MemFree(pmmdThis);
      }
}

void LIBCALL FreeMSD(PMSD pmsdThis)
{
#ifdef _DEBUG_2
printf("in FreeMSD ");
#endif
    if (pmsdThis)
      {
/* ASN.1 parts */
	if (pmsdThis->pbsBS) BiostrucFree(pmsdThis->pbsBS);
  /* BiostrucFree kills models, features % saved ASN.1 parts ... */
        if (pmsdThis->pGraphDescr)
            AsnGenericChoiceSeqOfFree(pmsdThis->pGraphDescr,(AsnOptFreeFunc)BiomolDescrFree);
    if (pmsdThis->pDictLocal)
        AsnGenericUserSeqOfFree(pmsdThis->pDictLocal,(AsnOptFreeFunc)ResidueGraphFree);
/* Structure Strings */
  	if (pmsdThis->pcSName) MemFree(pmsdThis->pcSName);
  	if (pmsdThis->pcPDBName) MemFree(pmsdThis->pcPDBName);
        if (pmsdThis->pcChemName) MemFree(pmsdThis->pcChemName);
    	if (pmsdThis->pcPdbClass) MemFree(pmsdThis->pcPdbClass);
    	if (pmsdThis->pcPdbSource) MemFree(pmsdThis->pcPdbSource);
/* Modelstruc parts */
    	if (pmsdThis->pdnmmHead) FreeListDNMM(pmsdThis->pdnmmHead);
  	if (pmsdThis->pvnmbIMBHead) FreeListVNMB(pmsdThis->pvnmbIMBHead);
	if (pmsdThis->pdnmlModels) FreeListDNML(pmsdThis->pdnmlModels);
	if (pmsdThis->pvnmoHead) FreeListVNMO(pmsdThis->pvnmoHead);
	if (pmsdThis->pvnmdHead) FreeListVNMD(pmsdThis->pvnmdHead);
/*      if (pmsdThis->pdnsfFeatures) FreeListDNSF(pmsdThis->pdnsfFeatures); */
        if (pmsdThis->psaStrucAlignment) BiostrucAnnotSetFree(pmsdThis->psaStrucAlignment);
        if (pmsdThis->ppflRotate) FLMatrixFree(pmsdThis->ppflRotate,0,0);
        if (pmsdThis->pdnsfsHead) FreeListDNSFS(pmsdThis->pdnsfsHead);
        if (pmsdThis->pflTranslate) FLVectorFree(pmsdThis->pflTranslate,0);
        if (pmsdThis->ppflBoundBox) FLMatrixFree(pmsdThis->ppflBoundBox,0,0);
        if (pmsdThis->pdnmsSlaves) FreeListDNMS(pmsdThis->pdnmsSlaves);
 	MemFree(pmsdThis);

       }
}

/* adds after last node in list if *head not NULL
 * otherwise head is new node
 */

PVNMB LIBCALL NewVNMB(PVNMB PNTR ppvnmbList,  Int2 choice)
{
    PVNMB pvnmbNew = NULL;
    PMBD pmbdNew = NULL;
    pmbdNew = NewMBD();
    if (!pmbdNew) return NULL;
    pvnmbNew = ValNodeAddPointer(ppvnmbList, choice, (Nlm_VoidPtr) pmbdNew);
    pmbdNew->pvnmbLink = pvnmbNew;  /* back link */
    return pvnmbNew;
}

PVNMO LIBCALL NewVNMO(PVNMO PNTR ppvnmoList,  Int2 choice)
{
    PVNMO pvnmoNew = NULL;
    PMOD pmodNew = NULL;
    pmodNew = NewMOD();
    if (!pmodNew) return NULL;
    pvnmoNew = ValNodeAddPointer(ppvnmoList, choice, (Nlm_VoidPtr) pmodNew);
    pmodNew->pvnmoLink = pvnmoNew;  /* back link */
    return pvnmoNew;
}


PVNMD LIBCALL NewVNMD(PVNMD PNTR ppvnmdList,  Int2 choice)
{
    PVNMD pvnmdNew = NULL;
    PMDD pmddNew = NULL;
    pmddNew = NewMDD();
    if (!pmddNew) return NULL;
    pvnmdNew = ValNodeAddPointer(ppvnmdList, choice, (Nlm_VoidPtr) pmddNew);
    pmddNew->pvnmdLink = pvnmdNew;  /* back link */
    return pvnmdNew;
}


PVNMA LIBCALL NewVNMA(PVNMA PNTR ppvnmaList,  Int2 choice)
{
    PVNMA pvnmaNew = NULL;
    PMAD pmadNew = NULL;
    pmadNew = NewMAD();
    if (!pmadNew) return NULL;
    pvnmaNew = ValNodeAddPointer(ppvnmaList,  choice, (Nlm_VoidPtr) pmadNew);
    pmadNew->pvnmaLink = pvnmaNew;
    return pvnmaNew;
}

PVNAL LIBCALL NewVNAL(PVNAL PNTR ppvnalList, Int2 choice)
{
 /* picks either paldlNew or paldNew as data node */
 /* choice is model number - which will end up as a Uint1 */
    PVNAL pvnalNew = NULL;
    PALD paldNew = NULL;
    paldNew = NewALD();
    if (!paldNew) return NULL;
    pvnalNew = ValNodeAddPointer(ppvnalList, choice, (Nlm_VoidPtr) paldNew);
    paldNew->pvnalLink = pvnalNew; /* link to ValNode */
    return pvnalNew;
}

PDNMG LIBCALL NewDNMG(PDNMG PNTR ppdnmgList,  Int2 choice)
{
    PDNMG pdnmgNew = NULL;
    PMGD pmgdNew = NULL;
    pmgdNew = NewMGD();
    if (!pmgdNew) return NULL;
    pdnmgNew = DValNodeAddPointer(ppdnmgList, choice, (Nlm_VoidPtr) pmgdNew);
    pmgdNew->pdnmgLink = pdnmgNew;
    return pdnmgNew;
}

PDNMM LIBCALL NewDNMM(PDNMM PNTR ppdnmmList,  Int2 choice)
{
    PDNMM pdnmmNew = NULL;
    PMMD pmmdNew = NULL;
    pmmdNew = NewMMD();
    if (!pmmdNew) return NULL;
    pdnmmNew = DValNodeAddPointer(ppdnmmList, choice,  (Nlm_VoidPtr) pmmdNew);
    pmmdNew->pdnmmLink = pdnmmNew;
    return pdnmmNew;
}

PDNMS LIBCALL NewDNMS(PDNMS PNTR ppdnmsList,  Int2 choice)
{
    PDNMS pdnmsNew = NULL;
    PMSD pmsdNew = NULL;
    pmsdNew = NewMSD();
    if (!pmsdNew) return NULL;
    pdnmsNew = DValNodeAddPointer(ppdnmsList,  choice, (Nlm_VoidPtr) pmsdNew);
    pmsdNew->pdnmsLink = pdnmsNew;
    return pdnmsNew;
}



/* need to make equivalent function ptr call in a ValNodeFreeDataFn */
ValNodePtr LIBCALL FreeVNDataFn (ValNodePtr vnp,  pFreeFunc freefn)
{
	ValNodePtr next;

	while (vnp)
	{
		if (freefn)
		  (*freefn)(vnp->data.ptrvalue);
		else
		   Nlm_MemFree(vnp->data.ptrvalue);
		next = vnp->next;
		Nlm_MemFree(vnp);
		vnp = next;
	}
	return NULL;
}

void LIBCALL FreeListVNAL(PVNAL pvnalList)
{
#ifdef _DEBUG_2
printf("in FreeListVNAL ");
#endif
    FreeVNDataFn(pvnalList,  (pFreeFunc)FreeALD);
}

void LIBCALL FreeListDNML(PDNML pdnmlList)
{
#ifdef _DEBUG_2
printf("in FreeListDNML ");
#endif
   DValNodeFreeData(pdnmlList, (pFreeFunc)FreeMLD);
}

void LIBCALL FreeListDNSFS(PDNSFS pdnsfsList)
{
#ifdef _DEBUG_2
printf("in FreeListDNSF ");
#endif
   DValNodeFreeData(pdnsfsList, (pFreeFunc)FreeSFS);  /* don't free Biostruc-dependent features */

}

void LIBCALL FreeListDNSF(PDNSF pdnsfList)
{
#ifdef _DEBUG_2
printf("in FreeListDNSF ");
#endif
   DValNodeFree(pdnsfList);  /* don't free Biostruc-dependent features */

}

void LIBCALL FreeListVNMB(PVNMB pvnmbList) /* takes out the whole singly-linked list */
{
#ifdef _DEBUG_2
printf("in FreeListVNMB ");
#endif
    FreeVNDataFn(pvnmbList, (pFreeFunc)FreeMBD);
}


void LIBCALL FreeListVNMO(PVNMO pvnmoList) /* takes out the whole singly-linked list */
{
#ifdef _DEBUG_2
printf("in FreeListVNMO ");
#endif
    FreeVNDataFn(pvnmoList, (pFreeFunc)FreeMOD);
}


void LIBCALL FreeListVNMD(PVNMD pvnmdList) /* takes out the whole singly-linked list */
{
#ifdef _DEBUG_2
printf("in FreeListVNMD ");
#endif
    FreeVNDataFn(pvnmdList, (pFreeFunc)FreeMDD);
}

void LIBCALL FreeListVNMA(PVNMA pvnmaList) /* takes out the whole singly-linked list */
{
#ifdef _DEBUG_2
printf("in FreeListVNMA ");
#endif
   FreeVNDataFn(pvnmaList, (pFreeFunc)FreeMAD);
}

/* takes out one - at a time ! */
PDNMG LIBCALL FreeDNMG(PDNMG pdnmgList, PDNMG pdnmgThis)
{
#ifdef _DEBUG_2
printf("in FreeDNMG ");
#endif
   return DValNodeListDelNode(pdnmgList,  pdnmgThis, (pFreeFunc)FreeMGD);
}


void LIBCALL FreeListDNMG(PDNMG pdnmgList)
{
    PDNMG pdnmgTemp;
#ifdef _DEBUG_2
printf("in FreeListDNMG ");
#endif

    pdnmgTemp = pdnmgList;
    while (pdnmgTemp)
    {
	pdnmgList = FreeDNMG(pdnmgTemp, pdnmgTemp);
	pdnmgTemp = pdnmgList;
    }
}


PDNMM LIBCALL FreeDNMM(PDNMM pdnmmList,  PDNMM pdnmmThis)
{
#ifdef _DEBUG_2
printf("in FreeDNMM ");
#endif
   return DValNodeListDelNode(pdnmmList, pdnmmThis, (pFreeFunc)FreeMMD);
}

void LIBCALL FreeListDNMM(PDNMM pdnmmList)
{
    PDNMM pdnmmTemp;
#ifdef _DEBUG_2
printf("in FreeListDNMM ");
#endif
    pdnmmTemp = pdnmmList;
    while (pdnmmTemp)
    {
	pdnmmList = FreeDNMM(pdnmmTemp, pdnmmTemp);
	pdnmmTemp = pdnmmList;
    }
}

PDNMS LIBCALL FreeDNMS(PDNMS pdnmsList, PDNMS pdnmsThis)
{
#ifdef _DEBUG_2
printf("in FreeDNMS ");
#endif
    return DValNodeListDelNode(pdnmsList,  pdnmsThis, (pFreeFunc)FreeMSD);
}


void LIBCALL FreeListDNMS(PDNMS pdnmsList)
{
    PDNMS pdnmsTemp;
    pdnmsTemp = pdnmsList;
#ifdef _DEBUG_2
printf("in FreeListDNMS ");
#endif
    while (pdnmsTemp)
    {
	pdnmsList = FreeDNMS(pdnmsTemp, pdnmsTemp);
	pdnmsTemp = pdnmsList;
    }
}


void LIBCALL FreeAtomicModelAsnLists(AtomicCoordinatesPtr pacThis)
{
   AlternateConformationIdsPtr 		paciThis = NULL;
   IsotropicTemperatureFactorsPtr 	pitfThis = NULL;
   AnisotropicTemperatureFactorsPtr 	patfThis = NULL;
   AtomicOccupanciesPtr 		pocThis = NULL;
   ModelSpacePointsPtr 			pmspThis = NULL;
   AtomPntrsPtr				pappThis = NULL;
   CoordinatesPtr			pcoThis = NULL;

    /* Free data off AtomicCoordinatesPtr pacThis */

    if (!pacThis) return;
#ifdef _DEBUG_1
printf("in FreeAtomicModelAsnList\n");
#endif
    pappThis = pacThis->atoms; /*  AtomPntrsPtr */
    pmspThis =  pacThis->sites; /* ModelSpacePointsPtr */
    pocThis = pacThis->occupancies; /*  AtomicOccupanciesPtr */
    paciThis = pacThis->alternate_conf_ids; /* AlternateConformationIdsPtr */
    if (pacThis->temperature_factors)  /* choice of temp factor types */
      {
       if (pacThis->temperature_factors->choice == AtomicTemperatureFactors_isotropic)
        {
          pitfThis = (IsotropicTemperatureFactorsPtr) pacThis->temperature_factors->data.ptrvalue;
        }
       if (pacThis->temperature_factors->choice == AtomicTemperatureFactors_anisotropic)
        {
          patfThis = (AnisotropicTemperatureFactorsPtr) pacThis->temperature_factors->data.ptrvalue;
        }
      }
    if (pappThis)
	 {
	  AtomPntrsFree(pappThis);
	  pacThis->atoms = NULL;
	 }
    if (pmspThis)
	 {
	  ModelSpacePointsFree(pmspThis);
	  pacThis->sites = NULL;
	 }
    if (pocThis)
	 {
	  AtomicOccupanciesFree(pocThis);
	  pacThis->occupancies = NULL;
	 }
    if (paciThis)
	 {
	  AlternateConformationIdsFree(paciThis);
	  pacThis->alternate_conf_ids = NULL;
	 }
    if (pitfThis)
	 {
	  IsotropicTemperatureFactorsFree (pitfThis);
	  pacThis->temperature_factors->data.ptrvalue = NULL;
	 }
    if (patfThis)
	 {
	  AnisotropicTemperatureFactorsFree(patfThis);
	  pacThis->temperature_factors->data.ptrvalue = NULL;
	 }
    if (pacThis->temperature_factors)
         {
	   ValNodeFree(pacThis->temperature_factors);
	   pacThis->temperature_factors = NULL;
	 }
   return;
}





PMAD LIBCALL AtomFromMMDBIndex(PDNMS pdnmsList, Int2 iStru, Int2 iMol, Int2 iGraph, Int4 iAtom)
{
    PDNMS pdnmsThis;
    PMSD  pmsdThis;
    PDNMM pdnmmThis;
    PMMD  pmmdThis;
    PDNMG pdnmgThis;
    PMGD  pmgdThis;
    PVNMA pvnmaThis;
    PMAD  pmadThis;

    pdnmsThis = (PDNMS) DValNodeFindNext((DValNodePtr)pdnmsList, NULL, iStru);
    if (pdnmsThis) pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    else return NULL;
    pdnmmThis = (PDNMM) DValNodeFindNext((DValNodePtr)pmsdThis->pdnmmHead, NULL, iMol);
    if (pdnmmThis) pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
    else return NULL;
    pdnmgThis = (PDNMG) DValNodeFindNext((DValNodePtr)pmmdThis->pdnmgHead,  NULL,  iGraph);
    if (pdnmgThis) pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
    else return NULL;
    pvnmaThis = (PVNMA) pmgdThis->pvnmaAHead;
    if (pvnmaThis) pmadThis = (PMAD) pvnmaThis->data.ptrvalue;
    else return NULL;
    while (pvnmaThis)
     {
         if (pmadThis->iIndex == iAtom)
           {
	     return pmadThis;
	   }
         pvnmaThis = pvnmaThis->next;
         if (pvnmaThis) pmadThis = (PMAD) pvnmaThis->data.ptrvalue;
     }
    return NULL;
}



PMGD LIBCALL GraphFromMMDBIndex(PDNMS pdnmsList, Int2 iStru, Int2 iMol, Int2 iGraph )
{
    PDNMS pdnmsThis;
    PMSD  pmsdThis;
    PDNMM pdnmmThis;
    PMMD  pmmdThis;
    PDNMG pdnmgThis;
    PMGD  pmgdThis;


    pdnmsThis = (PDNMS) DValNodeFindNext((DValNodePtr)pdnmsList, NULL, iStru);
    if (pdnmsThis) pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    else return NULL;
    pdnmmThis = (PDNMM) DValNodeFindNext((DValNodePtr)pmsdThis->pdnmmHead, NULL, iMol);
    if (pdnmmThis) pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
    else return NULL;
    pdnmgThis = (PDNMG) DValNodeFindNext((DValNodePtr)pmmdThis->pdnmgHead,  NULL,  iGraph);
    if (pdnmgThis)
       {
         pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
	 return pmgdThis;
       }
    else return NULL;
}


PMMD LIBCALL MolFromMMDBIndex(PDNMS pdnmsList, Int2 iStru, Int2 iMol)
{
    PDNMS pdnmsThis;
    PMSD  pmsdThis;
    PDNMM pdnmmThis;
    PMMD  pmmdThis;


    pdnmsThis = (PDNMS) DValNodeFindNext((DValNodePtr)pdnmsList, NULL, iStru);
    if (pdnmsThis) pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    else return NULL;
    pdnmmThis = (PDNMM) DValNodeFindNext((DValNodePtr)pmsdThis->pdnmmHead, NULL, iMol);
    if (pdnmmThis)
       {
         pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
         return pmmdThis;
       }
    else return NULL;
}

PRGD LIBCALL LoadDict(CharPtr pcDictName)
{

   AsnIoPtr aipr=NULL;
   FILE *f;
   Char fullpath [PATH_MAX];
   CharPtr ptr;
   PRGD rgdp;

   aipr = NULL;
   rgdp = NULL;

   ProgramPath (fullpath, sizeof (fullpath) - 1);
   ptr = StringRChr (fullpath, DIRDELIMCHR);
   if (ptr != NULL) {
      *ptr = '\0';
   } else {
      StrCpy(fullpath, ".");
   }
   FileBuildPath (fullpath, NULL, pcDictName);

   if ((f = FileOpen (fullpath, "rb")) == NULL) {
      if (FindPath ("NCBI", "NCBI", "DATA", fullpath, sizeof (fullpath) - 1)) {
         FileBuildPath (fullpath, NULL, pcDictName);
         aipr = AsnIoOpen (fullpath, "rb");
         if (aipr == NULL) {
            ErrPost(CTX_NCBIOBJ, 1, "Couldn't open [%s] in LoadDict", fullpath);
            return NULL;
         }
      } else {
         ErrPost(CTX_NCBIOBJ, 1, "FindPath failed in LoadDict");
         return NULL;
      }
   } else {
      FileClose (f);
      aipr = AsnIoOpen(fullpath, "rb");
      if (aipr == NULL) {
         ErrPost(CTX_NCBIOBJ, 1, "Couldn't open [%s] in LoadDict", fullpath);
         return NULL;
      }
   }

   rgdp = BiostrucResidueGraphSetAsnRead(aipr, NULL);

   AsnIoFlush(aipr);
   aipr = AsnIoClose(aipr);

   return rgdp;
}



BiostrucPtr LIBCALL FetchBS(CharPtr pcFetch,  Int2 iType, Int4 mdlLvl,
		Int4 maxModels, Byte bExtent)
{   /* mdlLvl = BSEVERYTHING used to denote fetch everything!  */
    AsnIoPtr paioAIP = NULL; /* aip */
    BiostrucPtr pbsThe = NULL;
    Boolean bIsNetwork = FALSE;
    /* LinkSetPtr plsLink = NULL; */
    DocUid duUID = 0;
  /* StrToLong stuff */
  Nlm_Char     ch;
  Nlm_Int2     i;
  Nlm_Int2     len;
  Nlm_Char     local [64];
  Nlm_Boolean  nodigits;
  Nlm_Boolean  rsult;
  long int     val;


#ifdef _DEBUG_0
printf("MaxModels=%d ModelLevel=%d\n",(int) maxModels,(int) mdlLvl);
#endif
	/* load the parse trees */
	if (! (objmmdb1AsnLoad() && objmmdb2AsnLoad() && objmmdb3AsnLoad() ))
	  {
	    	ErrPostEx(SEV_FATAL,0,0, "Internal - objmmdbNAsnLoad() Failure");
		return NULL;
	  } /* DON'T load any more than necessary */

 	/* handle the different types of input data */
	switch (iType)
	{
	case INP_ASCII_FILE:
		/* open the ASN.1 input file in the right mode */
		if ((paioAIP = AsnIoOpen (pcFetch, "r")) == NULL)
		{
			ErrClear(); /* toolkit fatal */
	    		ErrPostEx(SEV_ERROR,0,0, "Unable to open ASCII ASN.1 input file:\n %s\n Check if it exists."
			          ,pcFetch);
			ErrShow();
			return NULL;
		}
		/* get the pointer to the seq entry */

		if (mdlLvl == BSEVERYTHING)
		{
		  if ((pbsThe =  BiostrucAsnRead(paioAIP, NULL)) == NULL)
		    {
			ErrClear(); /* toolkit fatal */
			ErrPostEx(SEV_ERROR,0,0, "Unable to read Biostruc in:\n%s\nConfused Binary/ASCII or Wrong File Type!"
			,pcFetch);
			ErrShow();
			return NULL;
		    }}
		else
		 {  if ((pbsThe =  BiostrucAsnGet(paioAIP, NULL, mdlLvl, maxModels)) == NULL)
		    {
			ErrClear(); /* toolkit fatal */
			ErrPostEx(SEV_ERROR,0,0, "Unable to read Biostruc in:\n%s\nConfused Binary/ASCII or Wrong File Type!"
			,pcFetch);
			ErrShow();
			return NULL;
		    }}
		AsnIoClose(paioAIP);
		/* printf("Got the ascii biostruc\n"); */
		break;
	case INP_BINARY_FILE:
        /*	printf("in Binary input\n"); */

                /* open the ASN.1 input file in the right mode */
		if ((paioAIP = AsnIoOpen (pcFetch, "rb")) == NULL)
		{
			ErrClear(); /* toolkit fatal */
		      	ErrPostEx(SEV_ERROR,0,0, "Unable to open BINARY ASN.1 input file:\n %s\n Check if it exists.",pcFetch);
			ErrShow();
			return NULL;
		}
		/* get the pointer to the seq entry */
		if (mdlLvl == BSEVERYTHING)
		{
		  if ((pbsThe =  BiostrucAsnRead(paioAIP, NULL)) == NULL)
		    {
			ErrClear(); /* toolkit fatal */
			ErrPostEx(SEV_ERROR,0,0, "Unable to read Biostruc in:\n%s\nConfused Binary/ASCII or Wrong File Type!"
			,pcFetch);
			ErrShow();
			return NULL;
		    }}
		else
		 {  if ((pbsThe =  BiostrucAsnGet(paioAIP, NULL, mdlLvl, maxModels)) == NULL)
		    {
			ErrClear(); /* toolkit fatal */
			ErrPostEx(SEV_ERROR,0,0, "Unable to read Biostruc in:\n%s\nConfused Binary/ASCII or Wrong File Type!"
			,pcFetch);
			ErrShow();
			return NULL;
		    }}

		AsnIoClose(paioAIP);
		/* printf("Got the binary biostruc\n"); */
		break;
	case INP_GI:
                if (bExtent & (Byte) FETCH_ENTREZ)
		if (!MMDBInit())
		  {
		        ErrClear(); /* toolkit fatal */
		      	ErrPostEx(SEV_ERROR,0,0, "Unable to open MMDB input.");
			ErrShow();
			return NULL;
		  }

/* this is code from Nlm StrToLong */
  rsult = FALSE;
  len = (Nlm_Int2) Nlm_StringLen (pcFetch);
  if (len != 0) {
    rsult = TRUE;
    nodigits = TRUE;
    for (i = 0; i < len; i++) {
      ch = pcFetch [i];
      if (ch == ' ' || ch == '+' || ch == '-') {
      } else if (ch < '0' || ch > '9') {
        rsult = FALSE;
      } else {
        nodigits = FALSE;
      }
    }
    if (nodigits) {
      rsult = FALSE;
    }
    if (rsult) {
      Nlm_StringNCpy (local, pcFetch, sizeof (local));
      if (sscanf (local, "%ld", &val) == 1) {
        duUID = val;
      }
    }
  }
 		if (duUID == 0)
		   {
		        ErrClear(); /* toolkit fatal */
		      	ErrPostEx(SEV_ERROR,0,0, "UID -  %s - not in structure database, \n Check your input.", pcFetch);
			ErrShow();
			if (bExtent & (Byte) FETCH_ENTREZ) MMDBFini();
		        return NULL;
		  }
		if ((pbsThe = MMDBBiostrucGet(duUID,  mdlLvl, maxModels)) == NULL)
		  {
		        ErrClear(); /* toolkit fatal */
		      	ErrPostEx(SEV_ERROR,0,0, "Unable to find UID = %ld \nin database.", (long)duUID);
			ErrShow();
			if (bExtent & (Byte) FETCH_ENTREZ) MMDBFini();
			return NULL;
		  }
		 if (bExtent & (Byte) FETCH_ENTREZ) MMDBFini();
		break;
	case INP_PDB_ACCESSION:
	         if (bExtent & (Byte) FETCH_ENTREZ)
                 if (!MMDBInit())
		  {
		        ErrClear(); /* toolkit fatal */
		      	ErrPostEx(SEV_ERROR,0,0, "Unable to open MMDB input.");
			ErrShow();
			return NULL;
		  }
	        duUID = MMDBEvalPDB(pcFetch);
       	        if (duUID != 0)
		  {
		      if ((pbsThe = MMDBBiostrucGet(duUID,  mdlLvl, maxModels)) == NULL)
			{
			    ErrClear(); /* toolkit fatal */
		      	    ErrPostEx(SEV_ERROR,0,0, "Unable to find UID = %ld \nin database.", (long)duUID);
			    ErrShow();
			    /* LinkSetFree(plsLink); */
			    if (bExtent & (Byte) FETCH_ENTREZ) MMDBFini();
			    return NULL;
			}
		  }
		else
		  {
		        ErrClear(); /* toolkit fatal */
		      	ErrPostEx(SEV_ERROR,0,0, "Accession -  %s - not in structure database, \n Check your input.", pcFetch);
			ErrShow();
			if (bExtent & (Byte) FETCH_ENTREZ) MMDBFini();
		        return NULL;
		  }
		if (bExtent & (Byte) FETCH_ENTREZ) MMDBFini();
		break;

	  default:
	       return NULL;
 	} /* switch iTYPE */
/* here pbsThe should be valid in-memory Biostruc */
    return pbsThe;
}








/* make a GetBSRefictiona  */










/**************************/
/*version 4.1 ASN.1 code **/
/**************************/


 /* makes ValNode list on MAD, MGD, MDD pvnContainedBy
  * and ValNode list on MOD MDD pvnContains.
  * ranges are explicitly instantiated,
  * but not children.
  */

Boolean LIBCALL AssignSurfaceContents(PDNMS pdnmsList, Int2 iModel, PFB pfbThis, ChemGraphPntrsPtr pcgpContents)
{

   AtomPntrsPtr ppaThis = NULL;
   ResiduePntrsPtr pprThis = NULL;
   ResidueExplicitPntrsPtr ppreThis = NULL;
   ResidueIntervalPntrPtr ppriThis = NULL;
   MoleculePntrsPtr ppmThis = NULL;
   ValNodePtr pvnAtoms = NULL;
   ValNodePtr pvnMol = NULL;
   ValNodePtr pvnRes = NULL;
   ValNodePtr pvnThis = NULL;
   PMAD pmadThis = NULL;
   PMGD pmgdThis = NULL;
   PMMD pmmdThis = NULL;
   PMOD pmodThis = NULL;
   PMDD pmddThis = NULL;
   Int4 iStart = 0;
   Int4 iEnd = 0;
#ifdef _DEBUG_1
printf("in AssignSurfaceContents\n ");
#endif
   switch (pcgpContents->choice)
     {
	 case ChemGraphPntrs_atoms:
	   ppaThis = (AtomPntrsPtr) pcgpContents->data.ptrvalue;
	   pvnMol = ppaThis->molecule_ids;
	   pvnRes = ppaThis->residue_ids;
	   pvnAtoms = ppaThis->atom_ids;
	   while (pvnAtoms)
	     {
		 pmadThis = AtomFromMMDBIndex(pdnmsList, (Int2) pdnmsList->choice,
		          (Int2) pvnMol->data.intvalue, (Int2) pvnRes->data.intvalue,
			  (Int4) pvnAtoms->data.intvalue);
		 if (pmadThis)
		   {
		       pvnThis = ValNodeAddPointer(&pmadThis->pvnContainedBy,  iModel, (Pointer) pfbThis);
		       if (!pvnThis) return FALSE;
		   }
		 else return FALSE;
		 if (pfbThis->bMe == AM_MOD)
		   {
		       pmodThis = (PMOD) pfbThis;
		       pvnThis = ValNodeAddPointer(&pmodThis->pvnContains,  ChemGraphPntrs_atoms, (Pointer) pmadThis);
		   }
		 if (pfbThis->bMe == AM_MDD)
		   {
		       pmddThis = (PMDD) pfbThis;
		       pvnThis = ValNodeAddPointer(&pmddThis->pvnContains, ChemGraphPntrs_atoms, (Pointer) pmadThis);
		   }
		 pvnAtoms = pvnAtoms->next;
		 pvnMol = pvnMol->next;
		 pvnRes = pvnRes->next;
	     }
	  return TRUE;
	 case ChemGraphPntrs_residues:
	   pprThis = (ResiduePntrsPtr) pcgpContents->data.ptrvalue;
	   if (pprThis->choice == ResiduePntrs_explicit)
	     {

		 ppreThis = (ResidueExplicitPntrsPtr) pprThis->data.ptrvalue;
		 pvnMol = ppreThis->molecule_ids;
		 pvnRes = ppreThis->residue_ids;

		 while (pvnRes)
		   {
		    pmgdThis = GraphFromMMDBIndex(pdnmsList, (Int2) pdnmsList->choice,
		          (Int2) pvnMol->data.intvalue, (Int2) pvnRes->data.intvalue);
		    if (pmgdThis)
			{
			    pvnThis = ValNodeAddPointer(&pmgdThis->pvnContainedBy,  iModel, (Pointer) pfbThis);
			    if (!pvnThis) return FALSE;
			}
		    else return FALSE;
		    if (pfbThis->bMe == AM_MOD)
			{
			    pmodThis = (PMOD) pfbThis;
			    pvnThis = ValNodeAddPointer(&pmodThis->pvnContains,  ChemGraphPntrs_residues, (Pointer) pmgdThis);
			}
		    if (pfbThis->bMe == AM_MDD)
			{
			    pmddThis = (PMDD) pfbThis;
			    pvnThis = ValNodeAddPointer(&pmddThis->pvnContains, ChemGraphPntrs_residues, (Pointer) pmgdThis);
			}
		       pvnMol = pvnMol->next;
		       pvnRes = pvnRes->next;
		   }

		 return TRUE;
	     }
	   if (pprThis->choice == ResiduePntrs_interval)
	     {
	        ppriThis = (ResidueIntervalPntrPtr) pprThis->data.ptrvalue;
		   while (ppriThis)
		     {
			iStart = ppriThis->from;
			iEnd = ppriThis->to;
			if (iStart > ppriThis->to)
			  {
			      iStart = ppriThis->to;
			      iEnd = ppriThis->from;
			  }
			for (;iStart <= iEnd; iStart++)
			  {
			    pmgdThis = GraphFromMMDBIndex(pdnmsList, (Int2) pdnmsList->choice,
				(Int2) ppriThis->molecule_id, (Int2) iStart);
		            if (pmgdThis)
			      {
				pvnThis = ValNodeAddPointer(&pmgdThis->pvnContainedBy,  iModel, (Pointer) pfbThis);
				if (!pvnThis) return FALSE;
			      }
			    else return FALSE;
			    if (pfbThis->bMe == AM_MOD)
			      {
				pmodThis = (PMOD) pfbThis;
				pvnThis = ValNodeAddPointer(&pmodThis->pvnContains,  ChemGraphPntrs_residues, (Pointer) pmgdThis);
			      }
			    if (pfbThis->bMe == AM_MDD)
			      {
				pmddThis = (PMDD) pfbThis;
				pvnThis = ValNodeAddPointer(&pmddThis->pvnContains, ChemGraphPntrs_residues, (Pointer) pmgdThis);
			      }
			  } /* for start to end of interval */

		       ppriThis = ppriThis->next;
		     }	/* while intervals */
		return TRUE;
	     }
	  return FALSE;
	 case ChemGraphPntrs_molecules:
	  ppmThis = (MoleculePntrsPtr) pcgpContents->data.ptrvalue;
	  pvnMol = ppmThis->molecule_ids;
	    while (pvnMol)
	      {
	         pmmdThis = MolFromMMDBIndex(pdnmsList, (Int2) pdnmsList->choice,
		          (Int2) pvnMol->data.intvalue);
		 if (pmmdThis)
		   {
		       pvnThis = ValNodeAddPointer(&pmmdThis->pvnContainedBy,  iModel, (Pointer) pfbThis);
		       if (!pvnThis) return FALSE;
		   }
		 else return FALSE;
		 if (pfbThis->bMe == AM_MOD)
		   {
		       pmodThis = (PMOD) pfbThis;
		       pvnThis = ValNodeAddPointer(&pmodThis->pvnContains,  ChemGraphPntrs_molecules, (Pointer) pmmdThis);
		   }
		 if (pfbThis->bMe == AM_MDD)
		   {
		       pmddThis = (PMDD) pfbThis;
		       pvnThis = ValNodeAddPointer(&pmddThis->pvnContains, ChemGraphPntrs_molecules, (Pointer) pmmdThis);
		   }
		  pvnMol = pvnMol->next;
	      }
	  return TRUE;
     }
   return FALSE;
}





void LIBCALL FreeSurfaceModelAsnList(SurfaceCoordinatesPtr pscThis)
{

    TMeshPtr ptmshThis = NULL;
    TrianglesPtr ptriThis = NULL;
    ValNodePtr pvnX = NULL;

 if (!pscThis) return;
 if (!pscThis->Surface_surface) return;
#ifdef _DEBUG_1
printf("in FreeSurfaceModelAsnList\n");
#endif
 switch (pscThis->Surface_surface->choice)
      {
	   case Surface_surface_cylinder:
 	   case Surface_surface_brick:
 	   case Surface_surface_sphere:
 	   case Surface_surface_cone:
	      return;  /* nothing to free */
 	   case Surface_surface_tmesh:
	     ptmshThis = (TMeshPtr) pscThis->Surface_surface->data.ptrvalue;
	     pvnX = ValNodeFree(ptmshThis->x);
	     ptmshThis->x = NULL;
	     pvnX = ValNodeFree(ptmshThis->y);
	     ptmshThis->y = NULL;
	     pvnX = ValNodeFree(ptmshThis->z);
	     ptmshThis->z = NULL;
	     pvnX = ValNodeFree(ptmshThis->swap);
	     ptmshThis->swap = NULL;
	     return;
	   case Surface_surface_triangles:
 	     ptriThis = (TrianglesPtr) pscThis->Surface_surface->data.ptrvalue;
	     pvnX = ValNodeFree(ptriThis->x);
	     ptriThis->x = NULL;
	     pvnX = ValNodeFree(ptriThis->y);
	     ptriThis->y = NULL;
	     pvnX = ValNodeFree(ptriThis->z);
	     ptriThis->z = NULL;
	     pvnX = ValNodeFree(ptriThis->v1);
	     ptriThis->v1 = NULL;
	     pvnX = ValNodeFree(ptriThis->v2);
	     ptriThis->v2 = NULL;
	     pvnX = ValNodeFree(ptriThis->v3);
	     ptriThis->v3 = NULL;
	     return;
	   default:
	     return;
       }
}


Boolean LIBCALL FillSurface(SurfaceCoordinatesPtr pscThis, PMOD pmodThis)
{

    CylinderPtr pcylThis = NULL;
    BrickPtr pbrkThis = NULL;
    ConePtr pconThis = NULL;
    TMeshPtr ptmshThis = NULL;
    TrianglesPtr ptriThis = NULL;
    SpherePtr psphThis = NULL;
    ModelSpacePointPtr pmspTop = NULL;
    ModelSpacePointPtr pmspBottom = NULL;
    ModelSpacePointPtr pmspTemp = NULL;
    RealValuePtr prvRadius = NULL;
    ValNodePtr pvnSwap = NULL;
    ValNodePtr pvnX = NULL;
    ValNodePtr pvnY = NULL;
    ValNodePtr pvnZ = NULL;
    Int4 iCount = 0;
    Boolean rsult = FALSE;

#ifdef _DEBUG_1
printf("in FillSurface=");
#endif
    if (!pscThis) return FALSE;
    if (!pscThis->Surface_surface) return FALSE;

    ProgMon("Instantiating Surface");
    switch (pscThis->Surface_surface->choice)
      {
	   case Surface_surface_cylinder:
#ifdef _DEBUG_2
printf("cylinder\n ");
#endif
	     pmodThis->bWhat = (Byte)(pmodThis->bWhat | (Byte) OBJ_CYLINDER);
	     pcylThis = (CylinderPtr) pscThis->Surface_surface->data.ptrvalue;
	     pmodThis->ppflObject = FLMatrix(0, 1, 0, 2); /* 2 by 3 matrix */
	     if (!pmodThis->ppflObject) return FALSE;
	     pmodThis->iCoordNo = 2;
	     pmspTop = pcylThis->axis_top;
             pmspBottom = pcylThis->axis_bottom;
	     prvRadius = pcylThis->radius;
 	     pmodThis->ppflObject[0][0] = (FloatLo)((FloatLo)pmspTop->x/pmspTop->scale_factor);
	     pmodThis->ppflObject[0][1] = (FloatLo)((FloatLo)pmspTop->y/pmspTop->scale_factor);
	     pmodThis->ppflObject[0][2] = (FloatLo)((FloatLo)pmspTop->z/pmspTop->scale_factor);
	     pmodThis->ppflObject[1][0] = (FloatLo)((FloatLo)pmspBottom->x/pmspBottom->scale_factor);
	     pmodThis->ppflObject[1][1] = (FloatLo)((FloatLo)pmspBottom->y/pmspBottom->scale_factor);
	     pmodThis->ppflObject[1][2] = (FloatLo)((FloatLo)pmspBottom->z/pmspBottom->scale_factor);
	     pmodThis->flRadius = (FloatLo)((FloatLo)prvRadius->scaled_integer_value/prvRadius->scale_factor);
	     return TRUE;
	   case Surface_surface_brick:
#ifdef _DEBUG_2
printf("brick\n ");
#endif 	
	     pmodThis->bWhat = (Byte)(pmodThis->bWhat | (Byte) OBJ_BRICK);
	     pbrkThis = (BrickPtr) pscThis->Surface_surface->data.ptrvalue;
	     pmodThis->ppflObject = FLMatrix(0, 7, 0, 2); /* 8 by 3 matrix */
	     if (!pmodThis->ppflObject) return FALSE;
	     pmodThis->iCoordNo = 8;
	     pmspTemp = pbrkThis->corner_000;
	     pmodThis->ppflObject[0][0] = (FloatLo)((FloatLo)pmspTemp->x/pmspTemp->scale_factor);
	     pmodThis->ppflObject[0][1] = (FloatLo)((FloatLo)pmspTemp->y/pmspTemp->scale_factor);
	     pmodThis->ppflObject[0][2] = (FloatLo)((FloatLo)pmspTemp->z/pmspTemp->scale_factor);
	     pmspTemp = pbrkThis->corner_001;
	     pmodThis->ppflObject[1][0] = (FloatLo)((FloatLo)pmspTemp->x/pmspTemp->scale_factor);
	     pmodThis->ppflObject[1][1] = (FloatLo)((FloatLo)pmspTemp->y/pmspTemp->scale_factor);
	     pmodThis->ppflObject[1][2] = (FloatLo)((FloatLo)pmspTemp->z/pmspTemp->scale_factor);
	     pmspTemp = pbrkThis->corner_010;
	     pmodThis->ppflObject[2][0] = (FloatLo)((FloatLo)pmspTemp->x/pmspTemp->scale_factor);
	     pmodThis->ppflObject[2][1] = (FloatLo)((FloatLo)pmspTemp->y/pmspTemp->scale_factor);
	     pmodThis->ppflObject[2][2] = (FloatLo)((FloatLo)pmspTemp->z/pmspTemp->scale_factor);
	     pmspTemp = pbrkThis->corner_011;
	     pmodThis->ppflObject[3][0] = (FloatLo)((FloatLo)pmspTemp->x/pmspTemp->scale_factor);
	     pmodThis->ppflObject[3][1] = (FloatLo)((FloatLo)pmspTemp->y/pmspTemp->scale_factor);
	     pmodThis->ppflObject[3][2] = (FloatLo)((FloatLo)pmspTemp->z/pmspTemp->scale_factor);
	     pmspTemp = pbrkThis->corner_100;
	     pmodThis->ppflObject[4][0] = (FloatLo)((FloatLo)pmspTemp->x/pmspTemp->scale_factor);
	     pmodThis->ppflObject[4][1] = (FloatLo)((FloatLo)pmspTemp->y/pmspTemp->scale_factor);
	     pmodThis->ppflObject[4][2] = (FloatLo)((FloatLo)pmspTemp->z/pmspTemp->scale_factor);
	     pmspTemp = pbrkThis->corner_101;
	     pmodThis->ppflObject[5][0] = (FloatLo)((FloatLo)pmspTemp->x/pmspTemp->scale_factor);
	     pmodThis->ppflObject[5][1] = (FloatLo)((FloatLo)pmspTemp->y/pmspTemp->scale_factor);
	     pmodThis->ppflObject[5][2] = (FloatLo)((FloatLo)pmspTemp->z/pmspTemp->scale_factor);
	     pmspTemp = pbrkThis->corner_110;
	     pmodThis->ppflObject[6][0] = (FloatLo)((FloatLo)pmspTemp->x/pmspTemp->scale_factor);
	     pmodThis->ppflObject[6][1] = (FloatLo)((FloatLo)pmspTemp->y/pmspTemp->scale_factor);
	     pmodThis->ppflObject[6][2] = (FloatLo)((FloatLo)pmspTemp->z/pmspTemp->scale_factor);
	     pmspTemp = pbrkThis->corner_111;
	     pmodThis->ppflObject[7][0] = (FloatLo)((FloatLo)pmspTemp->x/pmspTemp->scale_factor);
	     pmodThis->ppflObject[7][1] = (FloatLo)((FloatLo)pmspTemp->y/pmspTemp->scale_factor);
	     pmodThis->ppflObject[7][2] = (FloatLo)((FloatLo)pmspTemp->z/pmspTemp->scale_factor);
	     return TRUE;
	   case Surface_surface_sphere:
#ifdef _DEBUG_2
printf("sphere\n ");
#endif 		     
	     pmodThis->bWhat = (Byte)(pmodThis->bWhat | (Byte) OBJ_SPHERE);
	     psphThis = (SpherePtr) pscThis->Surface_surface->data.ptrvalue;
	     pmodThis->ppflObject = FLMatrix(0, 1, 0, 1); /* 1 by 3 matrix */
	     if (!pmodThis->ppflObject) return FALSE;
	     pmodThis->iCoordNo = 1;
	     pmspTop = psphThis->center;
             prvRadius = psphThis->radius;
 	     pmodThis->ppflObject[0][0] = (FloatLo)((FloatLo)pmspTop->x/pmspTop->scale_factor);
	     pmodThis->ppflObject[0][1] = (FloatLo)((FloatLo)pmspTop->y/pmspTop->scale_factor);
	     pmodThis->ppflObject[0][2] = (FloatLo)((FloatLo)pmspTop->z/pmspTop->scale_factor);
	     pmodThis->flRadius = (FloatLo)((FloatLo)prvRadius->scaled_integer_value/prvRadius->scale_factor);
	     return TRUE;
	   case Surface_surface_cone:
#ifdef _DEBUG_2
printf("cone\n ");
#endif 	
	     pmodThis->bWhat = (Byte)(pmodThis->bWhat | (Byte) OBJ_CONE);
	     pconThis = (ConePtr) pscThis->Surface_surface->data.ptrvalue;
	     pmodThis->ppflObject = FLMatrix(0, 1, 0, 2); /* 2 by 3 matrix */
	     if (!pmodThis->ppflObject) return FALSE;
	     pmodThis->iCoordNo = 2;
	     pmspTop = pconThis->axis_top;
             pmspBottom = pconThis->axis_bottom;
	     prvRadius = pconThis->radius_bottom;
 	     pmodThis->ppflObject[0][0] = (FloatLo)((FloatLo)pmspTop->x/pmspTop->scale_factor);
	     pmodThis->ppflObject[0][1] = (FloatLo)((FloatLo)pmspTop->y/pmspTop->scale_factor);
	     pmodThis->ppflObject[0][2] = (FloatLo)((FloatLo)pmspTop->z/pmspTop->scale_factor);
	     pmodThis->ppflObject[1][0] = (FloatLo)((FloatLo)pmspBottom->x/pmspBottom->scale_factor);
	     pmodThis->ppflObject[1][1] = (FloatLo)((FloatLo)pmspBottom->y/pmspBottom->scale_factor);
	     pmodThis->ppflObject[1][2] = (FloatLo)((FloatLo)pmspBottom->z/pmspBottom->scale_factor);
	     pmodThis->flRadius = (FloatLo)((FloatLo)prvRadius->scaled_integer_value/prvRadius->scale_factor);
	     return TRUE;
	   case Surface_surface_tmesh:
#ifdef _DEBUG_2
printf("tmesh\n ");
#endif 	
	     pmodThis->bWhat = (Byte)(pmodThis->bWhat | (Byte) OBJ_TMESH);
	     ptmshThis = (TMeshPtr) pscThis->Surface_surface->data.ptrvalue;
	     pmodThis->ppflObject = FLMatrix(0, ptmshThis->number_of_points, 0, 2); /* N by 3 matrix */
	     if (!pmodThis->ppflObject) return FALSE;
	     pmodThis->pucSwap = UCVector(0, ptmshThis->number_of_points);  /* N vector for swap flags */
	     if (!pmodThis->pucSwap) return FALSE;
	     pvnX = ptmshThis->x;
	     pvnY = ptmshThis->y;
	     pvnZ = ptmshThis->z;
	     pvnSwap = ptmshThis->swap;
	     iCount = 0;
	     while (pvnX)
	        {
 		    ProgMon("Instantiating TMesh");
		    pmodThis->ppflObject[iCount][0] = (FloatLo)
				((FloatLo)pvnX->data.intvalue/ptmshThis->scale_factor);
		    pmodThis->ppflObject[iCount][1] = (FloatLo)
				((FloatLo)pvnY->data.intvalue/ptmshThis->scale_factor);
		    pmodThis->ppflObject[iCount][2] = (FloatLo)
				((FloatLo)pvnZ->data.intvalue/ptmshThis->scale_factor);
		    if (pvnSwap->data.boolvalue) pmodThis->pucSwap[iCount] = (Uchar) 1;
		    iCount++;
		    pvnX = pvnX->next;
		    pvnY = pvnY->next;
		    pvnZ = pvnZ->next;
		    pvnSwap = pvnSwap->next;
		}
	     pmodThis->iCoordNo = iCount;
	     FreeSurfaceModelAsnList(pscThis);
	     return TRUE;
	   case Surface_surface_triangles:
#ifdef _DEBUG_2
printf("triangles\n ");
#endif 	
	     pmodThis->bWhat = (Byte)(pmodThis->bWhat | (Byte) OBJ_TRIANGLES);
	     ptriThis = (TrianglesPtr) pscThis->Surface_surface->data.ptrvalue;
	     pmodThis->ppflObject = FLMatrix(0, ptriThis->number_of_points, 0, 2); /* N by 3 matrix */
	     if (!pmodThis->ppflObject) return FALSE;
	     pmodThis->ppi4Triangles = I4Matrix(0, ptriThis->number_of_triangles,0,2);  /* N by 3 matrix */
	     if (!pmodThis->ppi4Triangles) return FALSE;
	     pvnX = ptriThis->x;
	     pvnY = ptriThis->y;
	     pvnZ = ptriThis->z;
	     iCount = 0;
	     while (pvnX)
	        {
		    ProgMon("Instantiating Triangles");
		    pmodThis->ppflObject[iCount][0] = (FloatLo)
				((FloatLo)pvnX->data.intvalue/ptriThis->scale_factor);
		    pmodThis->ppflObject[iCount][1] = (FloatLo)
				((FloatLo)pvnY->data.intvalue/ptriThis->scale_factor);
		    pmodThis->ppflObject[iCount][2] = (FloatLo)
				((FloatLo)pvnZ->data.intvalue/ptriThis->scale_factor);
		    iCount++;
		    pvnX = pvnX->next;
		    pvnY = pvnY->next;
		    pvnZ = pvnZ->next;
		}
	     pmodThis->iCoordNo = iCount;
	     iCount = 0;
	     pvnX = ptriThis->v1;
	     pvnY = ptriThis->v2;
	     pvnZ = ptriThis->v3;
	     while (pvnX)
	        {
		    pmodThis->ppi4Triangles[iCount][0] = (Int4) pvnX->data.intvalue;   
		    pmodThis->ppi4Triangles[iCount][1] = (Int4) pvnY->data.intvalue;   
		    pmodThis->ppi4Triangles[iCount][2] = (Int4) pvnZ->data.intvalue;   
		    iCount++;
		    pvnX = pvnX->next;
		    pvnY = pvnY->next;
		    pvnZ = pvnZ->next;  
		}
	    pmodThis->iTriNo = iCount;
	    FreeSurfaceModelAsnList(pscThis);
	    return TRUE;
	
         default:
	     rsult = FALSE;
       }
    return rsult;   
}


 
  


Int2 LIBCALL BiostrucModel2ModelStruc(BiostrucModelPtr pbsmThis, PDNMS pdnmsThis, 
				Int2 iChoice, Int2 iModelCount, Byte bExtent)
{
    PMAD pmadThis = NULL;
    PVNMO pvnmoThis = NULL;
    PVNMD pvnmdThis = NULL;
    PVNAL pvnalThis = NULL;
    PVNAL pvnalTemp = NULL;
    PALD paldThis = NULL;
    PALD paldTemp = NULL;
    PMOD pmodThis = NULL;
    PMDD pmddThis = NULL;
    PMLD pmldThis = NULL;
    PMLD pmldFirst = NULL;
    PDNML pdnmlModel = NULL;
    PMSD pmsdThis = NULL;
    
    
    	    
    IsotropicTemperatureFactorsPtr pitfThis = NULL;
    AnisotropicTemperatureFactorsPtr patfThis = NULL;
    AlternateConformationIdsPtr paciThis = NULL;
    AtomicOccupanciesPtr pocThis = NULL;
    ModelSpacePointsPtr pmspThis = NULL;
		
    AtomicCoordinatesPtr pacThis = NULL;
    AtomPntrsPtr pappThis = NULL;
    CoordinatesPtr pcoThis = NULL;
    ModelCoordinateSetPtr pmcsThis = NULL;
    SurfaceCoordinatesPtr  pscThis = NULL;
    DensityCoordinatesPtr pdcThis = NULL; 
     
    CharPtr pcThis = NULL;
    CharPtr pcTemp = NULL;
    ValNodePtr pvnX = NULL;
    ValNodePtr pvnY = NULL;
    ValNodePtr pvnZ = NULL;
    ValNodePtr pvnB11 = NULL;
    ValNodePtr pvnB12 = NULL;
    ValNodePtr pvnB13 = NULL;
    ValNodePtr pvnB22 = NULL;
    ValNodePtr pvnB23 = NULL;
    ValNodePtr pvnB33 = NULL;
    ValNodePtr pvnOcc = NULL;
    ValNodePtr pvnAlt = NULL;
    ValNodePtr pvnMid = NULL;
    ValNodePtr pvnRid = NULL;
    ValNodePtr pvnAid = NULL;
    ValNodePtr pmdrThis = NULL;
    Int4 iCountAtoms = 0;
    Int4 iCounter = 0;
    Int4 iVectLen = 0;
    Int4 iCoordScale = 0;
    Int4 iCoordSets = 0;
    Int4 iOccScale = 0;
    Int4 iTempScale = 0;
    CharPtr pcTest = NULL;
    Byte bAnisotropy = FALSE;
    Byte bAltConf = FALSE;
    Boolean bFirstCoordSet = TRUE;
    	   
#ifdef _DEBUG_1
     printf("Making Model #%d %d \n",iModelCount,  (Int2)bExtent);
#endif    
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    pmldFirst = NewMLD();
    if (!pmldFirst) goto allocerr;
    pmldFirst->iType = pbsmThis->type;
    pmldFirst->iId = pbsmThis->id;
    pmldFirst->bSelected = (Byte) 0x01; /* for saving/conversion */
    pmldFirst->pbsmModel = pbsmThis;  /* point to the entire model */
    pmldFirst->pmsdParent = (PMSD) pdnmsThis->data.ptrvalue;
    pmdrThis = ValNodeFindNext(pbsmThis->descr, NULL, ModelDescr_name);
    if (pmdrThis)
      {
	  pmldFirst->pcModelName = StringSave((CharPtr) pmdrThis->data.ptrvalue); 
      }
    
    pmcsThis = pbsmThis->model_coordinates;
    while  (pmcsThis)  /* walk the model-coordinate sets - flattened and instantiated */
      {  
	 bAnisotropy = FALSE;
         bAltConf = FALSE;
         iCoordSets++;
#ifdef _DEBUG_1
printf("Model coord set %d\n ", iCoordSets);
#endif 	  	
	if (!bFirstCoordSet)
	  {
	      pmldThis->next = NewMLD();
	      if (!pmldThis->next) goto allocerr;
	      pmldThis = pmldThis->next;
	      pmldThis->iType = pbsmThis->type;
	      pmldThis->iId = pbsmThis->id;
	      pmldThis->pbsmModel = pbsmThis;  /* point to the entire model */
	      pmldThis->iNoCoordSet = pmcsThis->id;
	      pmldThis->pmsdParent = (PMSD) pdnmsThis->data.ptrvalue;
   
	  }
	else
	  {
	      pmldThis = pmldFirst;
	      pmldThis->iNoCoordSet = pmcsThis->id;
	  }
#ifdef _DEBUG_1
printf("Model Type %d\n ", pmldThis->iType);
#endif 	 
	bFirstCoordSet = FALSE;
	/* NOT BY REFERENCE YET */
      	if (pmcsThis->Coordinates_coordinates->choice == Coordinates_coordinates_reference) goto skipnext;
	/* rest are literals */
   	
	pcoThis = (CoordinatesPtr) pmcsThis->Coordinates_coordinates->data.ptrvalue;

 	/* read in Surface coordinate sets MOD */ 
        if (pcoThis->choice == Coordinates_surface)
	  {
#ifdef _DEBUG_3
printf("surface\n");
#endif 
	     pmsdThis->iObjCount += 1;
	     pvnmoThis = NewVNMO(NULL, iModelCount);
	     if (!pvnmoThis) goto allocerr;
	     pmodThis = (PMOD) pvnmoThis->data.ptrvalue;
	     pmodThis->pfbParent = (PFB) pmsdThis;
	     pscThis = (SurfaceCoordinatesPtr) pcoThis->data.ptrvalue;
	     if (!(FillSurface(pscThis, pmodThis))) goto allocerr;
	     if (!(AssignSurfaceContents(pdnmsThis, iModelCount, (PFB) pmodThis, pscThis->contents))) goto allocerr;     
 	     if (pmsdThis->pvnmoHead == NULL)
	       {
		   pmsdThis->pvnmoHead = pvnmoThis;
	       }
	     else 
	       ValNodeListCat(pmsdThis->pvnmoHead, pvnmoThis);
	     pmodThis->pmldCoordSet = pmldThis;
	     pmldThis->pmodObject = pmodThis;
	     goto skipnext;
	  }
	  

	/* read in DENSITY coordinate sets MDD */

        if (pcoThis->choice == Coordinates_density)
	  {
#ifdef _DEBUG_3
printf("density\n");
#endif 	     
	     pmsdThis->iDensCount += 1;
	     pvnmdThis = NewVNMD(NULL, iModelCount);
	     if (!pvnmdThis) goto allocerr;
	     pmddThis = (PMDD) pvnmdThis->data.ptrvalue;
	     pdcThis = (DensityCoordinatesPtr) pcoThis->data.ptrvalue;
	       /* @@@@@@@@@@@@@@@@@@@@@@@convert the density object */
	     if (!(AssignSurfaceContents(pdnmsThis, iModelCount,  (PFB) pmddThis, pdcThis->contents))) goto allocerr; 
	      if (pmsdThis->pvnmdHead == NULL)
	       {
		   pmsdThis->pvnmdHead = pvnmdThis;
	       }
	     else 
	       ValNodeListCat(pmsdThis->pvnmdHead, pvnmdThis);
	     pmddThis->pmldCoordSet = pmldThis;
	     pmldThis->pmddDensity = pmddThis;
	 
	     goto skipnext;
	  }


        /* NOTE that coordinates by REFERENCE are not yet supported */

	pacThis = NULL;
	pappThis = NULL;
	pmspThis = NULL;
	pocThis = NULL;
	paciThis = NULL;
	if ((pcoThis->choice == Coordinates_atomic) && (bExtent & (Byte) COORDS_DATA))
          {
   	     pacThis = (AtomicCoordinatesPtr) pcoThis->data.ptrvalue;
	     pappThis = pacThis->atoms; /*  AtomPntrsPtr */        
	     pmspThis =  pacThis->sites; /* ModelSpacePointsPtr */
	     pocThis = pacThis->occupancies; /*  MolOccPtr */
	     paciThis = pacThis->alternate_conf_ids; /* AlternateConformationIdsPtr */
	     if ((pacThis->temperature_factors) && (bExtent & (Byte) TEMP_DATA))
  	        {
#ifdef _DEBUG_2
printf("Isotropic Temperature factors \n");
#endif	
	           if (pacThis->temperature_factors->choice == AtomicTemperatureFactors_isotropic)
    		     {         
		      pitfThis = (IsotropicTemperatureFactorsPtr) pacThis->temperature_factors->data.ptrvalue;
		      if (pitfThis) iTempScale = pitfThis->scale_factor;
		      pvnB11 = pitfThis->b;
	      	     }
                   if (pacThis->temperature_factors->choice == AtomicTemperatureFactors_anisotropic)
 	             {
		      patfThis = (AnisotropicTemperatureFactorsPtr) pacThis->temperature_factors->data.ptrvalue;
                      bAnisotropy = TRUE;     
		      if (patfThis) 
			{ 
#ifdef _DEBUG_2
printf("Anisotropic Temperature factors \n");
#endif				 
			  iTempScale = patfThis->scale_factor;
			  pvnB11 = patfThis->b_11;
			  pvnB12 = patfThis->b_12;
			  pvnB13 = patfThis->b_13;
		 	  pvnB22 = patfThis->b_22;
			  pvnB23 = patfThis->b_23;
			  pvnB33 = patfThis->b_33;
			}
		     }
	        }
	     /* else deal with anisotropic ones */
  	  } /* if pcoThis */
	else
	  {
	      goto skipnext;
	  }

     
        if (pmspThis)
          {
	   pvnX = pmspThis->x;
	   pvnY = pmspThis->y;
	   pvnZ = pmspThis->z;
	   iCoordScale = pmspThis->scale_factor;
#ifdef _DEBUG_2
printf("Coordinates / = %d\n", iCoordScale);
#endif	
          } else goto erret;  
	if (pappThis)
	  {
   	   pvnMid = pappThis->molecule_ids;
	   pvnRid = pappThis->residue_ids;
	   pvnAid = pappThis->atom_ids;
	   iCountAtoms = pappThis->number_of_ptrs;
	   
	   
#ifdef _DEBUG_2
printf("%d ATOMS In Model\n", iCountAtoms);
#endif		   
	  } else goto erret;   
	if ((pocThis) && (bExtent &  (Byte) OCC_DATA))
	  {
	   pvnOcc = pocThis->o;
	   iOccScale = pocThis->scale_factor;
#ifdef _DEBUG_2
printf("Occupancies / %d\n", iOccScale);
#endif	 
	  }
        if ((paciThis) && (bExtent & (Byte) ALTCNF_DATA))
          {
 	   pvnAlt = paciThis;
	   bAltConf = TRUE;
#ifdef _DEBUG_3
printf("Alt conformations found \n");
#endif	 	
	  }
        if (iCoordScale) iVectLen = 2; /* 3 elements 0=X,1=Y,2=Z */
        if (iOccScale) iVectLen = 3; 
        if (iTempScale) iVectLen = 4; 
        if ((iTempScale) && bAnisotropy) iVectLen = 9;
#ifdef _DEBUG_2
printf("iVectLen = %d\n", iVectLen);
#endif	 	
        if ((iCoordScale) && (pappThis && pmspThis)) /* minimum to do the atomic locations */
          {
             /* do all data collect in one walk-through the vectors to preserve atom loc data */
	     /* the vector pmdlThis->ppAsnOrder has PALD pointers in it, which begin at       */
	     /* ppAsnOrder[1] and end at ppAsnOrder[iCountAtoms], so ASN.1 indexing is not    */
	     /* shifted by (-1) to match C indexing                                           */
	     /* ppAsnOrder[iCountAtoms+1] is set to NULL as an end-of matrix indicator        */
	   pmldThis->ppAsnOrder = PTRVector(0, iCountAtoms + 1);
	   if (pmldThis->ppAsnOrder) /* this should not be fatal - only affects features */
	     pmldThis->ppAsnOrder[iCountAtoms+1] = NULL;
   	   while (iCounter < iCountAtoms) 
            {
		iCounter++;
		ProgMon("Instantiating Atom Locations");
#ifdef _DEBUG_3
printf("[%d] ",iCounter);
#endif			
		pmadThis = AtomFromMMDBIndex(pdnmsThis, iChoice, (Int2) pvnMid->data.intvalue,
			    (Int2) pvnRid->data.intvalue, (Int4) pvnAid->data.intvalue);
		if (!pmadThis) goto erret;		
		pvnalThis = NewVNAL(NULL, iModelCount); 
		if (!pvnalThis) goto allocerr;
	        paldThis = (PALD) pvnalThis->data.ptrvalue;
		paldThis->pfbParent = (PFB) pmadThis; /* back-link to pmad */
	        paldThis->pflvData = FLVector(0,(Int4) iVectLen); /* alloc the data vector */
		if (!paldThis->pflvData) goto allocerr;
		paldThis->iCoordSet = (Int1) pmcsThis->id;
		paldThis->iFloatNo = (Int1) iVectLen; 
		if (pmldThis->ppAsnOrder) pmldThis->ppAsnOrder[iCounter] = (Pointer) paldThis; /* add to order array */
		if (pmadThis->pvnalLocate)  
		  {  /* the case where models are already present */
#ifdef _DEBUG_3
printf("{%d}", iModelCount);
#endif			
		   pvnalTemp = (PVNAL) ValNodeFindNext(pmadThis->pvnalLocate,NULL,iModelCount);
		   if (pvnalTemp)
		     { /* the case where THIS model has a location already */
#ifdef _DEBUG_3
printf("DISORDER!!\n");
#endif			 
		        paldTemp = (PALD) pvnalTemp->data.ptrvalue;  
		        while (paldTemp->next)
			   {   /* find end of pald list */
			       paldTemp = paldTemp->next;
			   }
			paldTemp->next = (PALD)pvnalThis->data.ptrvalue; /* attach the new location */
			pvnalThis->data.ptrvalue = NULL;
			pvnalThis = ValNodeFree(pvnalThis); /* lose the ValNode baggage */   
		     }
		   else
		   ValNodeListCat(pmadThis->pvnalLocate, pvnalThis); /* attach to pmadThis */
		  }
		else
		  pmadThis->pvnalLocate = pvnalThis;
	
		paldThis->pflvData[0] = (FloatLo)((FloatLo)pvnX->data.intvalue/iCoordScale);  
		paldThis->pflvData[1] = (FloatLo)((FloatLo)pvnY->data.intvalue/iCoordScale); 
		paldThis->pflvData[2] = (FloatLo)((FloatLo)pvnZ->data.intvalue/iCoordScale);
#ifdef _DEBUG_3
printf("%s  X= %f, Y= %f, Z= %f ", pmadThis->pcAName, paldThis->pflvData[0], 
                     paldThis->pflvData[1], paldThis->pflvData[2]);
#endif			

		if (iOccScale) 
		   {
		     paldThis->pflvData[3] = (FloatLo)((FloatLo)pvnOcc->data.intvalue/iOccScale) ;
		     pvnOcc = pvnOcc->next;
#ifdef _DEBUG_3
printf(" Occ = %f  ", paldThis->pflvData[3]);
#endif
		
	           }
        else if(iVectLen >=3 ) paldThis->pflvData[3]  = 1.0;
	        if (bAltConf)
                   {
		     pcThis = (CharPtr) pvnAlt->data.ptrvalue;
		     if (pcThis)
		        {
		          paldThis->cAltConf = (Char) pcThis[0];
			  if (!pmldThis->pcAltConf)
  			    {
			      pmldThis->pcAltConf = StringSave(pcThis);
			    }
			  else 
			    { /* no alt conf string yet */
			      pcTest = NULL;
			      pcTest = StringChr(pmldThis->pcAltConf,(Char) pcThis[0]); 
			      if (!(pcTest))
			        { /* char not yet in pmldThis->pcAltConf */
#ifdef _DEBUG_3		      
printf("\nNEW STRING\n");
#endif				
			          pcTemp = (CharPtr) MemNew((size_t)(StringLen(pmldThis->pcAltConf)+2)); /* more room */
			          StringCpy(pcTemp, pmldThis->pcAltConf);  /* copy old string */
			          StringCat(pcTemp, pcThis); /* new letter added */
			          MemFree(pmldThis->pcAltConf); /* lose old string */
			         pmldThis->pcAltConf = pcTemp; /* new string */
				}
 			    }
			}
#ifdef _DEBUG_3		      
printf(" <%s>pmldThis \n",pmldThis->pcAltConf);
printf("%s-\n",pcThis);
printf(" <%c>paldThis \n",(char) paldThis->cAltConf );
#endif
 		     pvnAlt = pvnAlt->next;
	     	   }
		if (iTempScale)
		  {
                   if (bAnisotropy)
		    {
#ifdef _DEBUG_3
printf("%d %d %d %d %d %d", pvnB11->data.intvalue,  pvnB12->data.intvalue, 
			pvnB13->data.intvalue, pvnB22->data.intvalue, 
			pvnB23->data.intvalue, pvnB33->data.intvalue);
#endif			      
 			paldThis->pflvData[5] = (FloatLo)((FloatLo)pvnB12->data.intvalue/iTempScale);
			paldThis->pflvData[6] = (FloatLo)((FloatLo)pvnB13->data.intvalue/iTempScale);
			paldThis->pflvData[7] = (FloatLo)((FloatLo)pvnB22->data.intvalue/iTempScale);
			paldThis->pflvData[8] = (FloatLo)((FloatLo)pvnB23->data.intvalue/iTempScale);
			paldThis->pflvData[9] = (FloatLo)((FloatLo)pvnB33->data.intvalue/iTempScale);
#ifdef _DEBUG_3
printf("B12=%f,  B13=%f,  B22=%f, B23=%f, B33=%f ", 
			paldThis->pflvData[5], paldThis->pflvData[6], paldThis->pflvData[7], 
			paldThis->pflvData[8], paldThis->pflvData[9] );
#endif			
			pvnB12 = pvnB12->next;
			pvnB13 = pvnB13->next;
			pvnB22 = pvnB22->next;
			pvnB23 = pvnB23->next;
			pvnB33 = pvnB33->next;
		    }
		   paldThis->pflvData[4] = (FloatLo)((FloatLo)pvnB11->data.intvalue/iTempScale);
#ifdef _DEBUG_3
printf("B11=%f ", paldThis->pflvData[4]);
#endif		
		   pvnB11 = pvnB11->next;
 		  } /* if iTempScale */
#ifdef _DEBUG_3
printf("\n");
#endif     	   
		pvnX = pvnX->next;
		pvnY = pvnY->next;
		pvnZ = pvnZ->next;
		pvnMid = pvnMid->next;
		pvnRid = pvnRid->next;
		pvnAid = pvnAid->next;
           }   /* while (iCounter < iCountAtoms) */

   	 } /* if pmspThis && pappThis */
	 
    skipnext:	
#ifdef _DEBUG_3
printf("skipnext");
#endif 
	
         /* free the model data leaving the ASN.1 stub */
       if (pacThis) FreeAtomicModelAsnLists(pacThis);

       pmcsThis = pmcsThis->next;	
    } /* while pmcsThis - do next coordinate set*/	
      
    pdnmlModel = DValNodeAddPointer(&pmsdThis->pdnmlModels, iModelCount, (Nlm_VoidPtr) pmldFirst);
    if (!pdnmlModel) goto allocerr;
	 
   return 1;

   allocerr:
 	     ErrClear(); 
	     ErrPostEx(SEV_ERROR,0,0, "Out Of Memory While Loading Model\nTry loading smaller or fewer Models");
	     ErrShow();
	     FreeMLD(pmldFirst);
 	     return 0;
 
   erret: 
 	ErrClear(); 
	ErrPostEx(SEV_ERROR,0,0, "Internal Indexing Failure in ASN.1\nCheck ASN.1 Input for integrity");
	ErrShow();
  	FreeMLD(pmldFirst);
 	return -1;
}



ValNodePtr LIBCALL MakeChemGraphNodeList(PDNMS pdnmsThis,  ChemGraphPntrsPtr pcgpThis)
{
  
    ValNodePtr pvnThis = NULL;
    ValNodePtr pvnLocList = NULL;
    PFB pfbThis = NULL;
    ResiduePntrsPtr prpsThis = NULL;
    ResidueExplicitPntrsPtr prepThis = NULL;
    ResidueIntervalPntrPtr pripThis = NULL;
    AtomPntrsPtr pAtoms = NULL;
    MoleculePntrsPtr pMolecules = NULL;
    ValNodePtr pvnAtom = NULL;
    ValNodePtr pvnMol = NULL;
    ValNodePtr pvnRes = NULL;
    Int4 iCountPtrs = 0;
    Int4 iStart = 0;
    Int4 iEnd = 0;
 
    if (!pdnmsThis) return NULL;
    if (!pcgpThis) return NULL;
    if (pcgpThis && pdnmsThis)
     {
       switch (pcgpThis->choice) 
         {
	    case ChemGraphPntrs_atoms:
	     pAtoms = (AtomPntrsPtr) pcgpThis->data.ptrvalue;
	     if (!pAtoms) return NULL;
	     pvnMol = pAtoms->molecule_ids;
	     pvnRes = pAtoms->residue_ids;
	     pvnAtom = pAtoms->atom_ids; 	
	     if (pAtoms->number_of_ptrs == 0) return NULL;
	     for (iCountPtrs = 0; iCountPtrs < pAtoms->number_of_ptrs; iCountPtrs++)
	       {
	           pfbThis = NULL;
		   pfbThis = (PFB) AtomFromMMDBIndex(pdnmsThis, (Int2) pdnmsThis->choice, 
		                      (Int2) pvnMol->data.intvalue, 
			              (Int2) pvnRes->data.intvalue, 
				      (Int4) pvnAtom->data.intvalue);
		   if (pfbThis)
		       {
		          pvnThis = ValNodeAddPointer(&pvnLocList, 
						0, (Nlm_VoidPtr) pfbThis);
		       }		      
		   pvnMol = pvnMol->next;
		   pvnRes = pvnRes->next;
		   pvnAtom = pvnAtom->next;
	       }   
            break;
	    
          case ChemGraphPntrs_residues:
	     prpsThis = (ResiduePntrsPtr) pcgpThis->data.ptrvalue;
	     if (!prpsThis) return NULL;
	     if (prpsThis->choice == ResiduePntrs_explicit)
	       {
		  prepThis = (ResidueExplicitPntrsPtr) prpsThis->data.ptrvalue; 
		  pvnMol = prepThis->molecule_ids;
		  pvnRes = prepThis->residue_ids;		 
		  while (pvnRes)
		   {
		     pfbThis = NULL;
		     pfbThis = (PFB) GraphFromMMDBIndex(pdnmsThis, (Int2) pdnmsThis->choice, 
		          (Int2) pvnMol->data.intvalue, (Int2) pvnRes->data.intvalue);
		     if (pfbThis)
		       {
		          pvnThis = ValNodeAddPointer(&pvnLocList, 
						0, (Nlm_VoidPtr) pfbThis);
		       }
		     else return NULL;
		     pvnMol = pvnMol->next;
		     pvnRes = pvnRes->next;
		   }
	        }  /* explicit */
	     if (prpsThis->choice == ResiduePntrs_interval)
	       {
		   pripThis = (ResidueIntervalPntrPtr) prpsThis->data.ptrvalue;
		   while (pripThis)
		     {
		        iStart = pripThis->from;
			iEnd = pripThis->to;
			if (iStart > pripThis->to)
			  {
			      iStart = pripThis->to;
			      iEnd = pripThis->from;
			  }
			for (;iStart <= iEnd; iStart++)
			  {  
			     pfbThis = NULL;
			     pfbThis = (PFB) GraphFromMMDBIndex(pdnmsThis, (Int2) pdnmsThis->choice, 
				(Int2) pripThis->molecule_id, (Int2) iStart);
		             if (pfbThis)
			       {
				  pvnThis = ValNodeAddPointer(&pvnLocList, 
						0, (Nlm_VoidPtr) pfbThis);
			       }
			      else return NULL;
			  }
			pripThis = pripThis->next;
		     } /* while pripThis */
	       } /* interval */		     
	    break;
        
	  case ChemGraphPntrs_molecules:
             pMolecules = (MoleculePntrsPtr) pcgpThis->data.ptrvalue;
	     if (!pMolecules)  return NULL;
	     pvnMol = pMolecules->molecule_ids;
	     if (pMolecules->number_of_ptrs == 0) return NULL;
	     for (iCountPtrs = 0; iCountPtrs < pMolecules->number_of_ptrs; iCountPtrs++)
	       {
	           pfbThis = NULL;
		   pfbThis = (PFB) MolFromMMDBIndex(pdnmsThis, (Int2) pdnmsThis->choice, 
		                      (Int2) pvnMol->data.intvalue);
		   if (pfbThis)
		       {
		          pvnThis = ValNodeAddPointer(&pvnLocList, 
						0, (Nlm_VoidPtr) pfbThis);
		       }		      
		   pvnMol = pvnMol->next;
	       }      
	    break;
	    
          default: return NULL;
	 }

    }  /* if pcgpThis && pmsdThis*/

  return pvnLocList;

}
     
 

Int2 SortOn=0;

PDNTRN LIBCALL NewDNTRN(PDNTRN PNTR ppdntrnList,  Int2 choice)
{
    PDNTRN pdntrnNew = NULL;
    FloatLoPtr pflvNew = NULL;
    FloatLoPtrPtr ppflmNew = NULL;
    
    if (!choice) return NULL;
    if ((int) choice == Move_translate)
      {
	  pflvNew = FLVector(0, 2); 
	  if (!pflvNew) return NULL;
	  pdntrnNew = DValNodeAddPointer(ppdntrnList, choice, (Nlm_VoidPtr) pflvNew);
      }
    if ((int) choice == Move_rotate)
      {
          ppflmNew = FLMatrix(0, 2, 0, 2);
	  if (!ppflmNew) return NULL;
	  pdntrnNew = DValNodeAddPointer(ppdntrnList, choice, (Nlm_VoidPtr) ppflmNew);
      }
    return pdntrnNew;  
}


void LIBCALL FreeDNTRN(PDNTRN pdntrnList)
{
    PDNTRN pdntrnThis;
    PDNTRN pdntrnNext;
    FloatLoPtr pflv = NULL;
    FloatLoPtrPtr ppflm = NULL;
  
    pdntrnThis = pdntrnList;
    while(pdntrnThis)
     {
       pdntrnNext = pdntrnThis->next;
       switch(pdntrnThis->choice)  
         {
	    case Move_rotate:
	         ppflm = (FloatLoPtrPtr) pdntrnThis->data.ptrvalue;
		 FLMatrixFree(ppflm,  0,  0);
	      break;
	    case Move_translate: 
	         pflv = (FloatLoPtr) pdntrnThis->data.ptrvalue;
		 FLVectorFree(pflv,  0);
              break;
	 }
         pdntrnThis->next = NULL;
         DValNodeFree(pdntrnThis);
       pdntrnThis = pdntrnNext;
     }
}

void LIBCALL TransformToDNTRN(PDNTRN PNTR pplist,  TransformPtr transform)
{
  PDNTRN pdntrnThis = NULL;
  PDNTRN pdntrnFirst = NULL;
  MovePtr pm = NULL;
  RotMatrixPtr prm = NULL;
  TransMatrixPtr ptm = NULL;
  FloatLoPtr pflv = NULL;
  FloatLoPtrPtr ppflm = NULL;
   
  if (!transform) return;
  
  pm = transform->moves;
  while (pm)
    {
  	pdntrnThis = NewDNTRN(pplist, (Int2) pm->choice);
	if (!pdntrnThis) return ;
	if (!pdntrnFirst) pdntrnFirst = pdntrnThis;
	if ((int) pm->choice == Move_rotate)
	  {  /* copy * scale from RotMatrix */
	   ppflm = (FloatLoPtrPtr) pdntrnThis->data.ptrvalue;
	   prm = (RotMatrixPtr) pm->data.ptrvalue;
	   ppflm[0][0] = (FloatLo) ((FloatLo)prm->rot_11 / prm->scale_factor);
	   ppflm[0][1] = (FloatLo) ((FloatLo)prm->rot_12 / prm->scale_factor);
	   ppflm[0][2] = (FloatLo) ((FloatLo)prm->rot_13 / prm->scale_factor);
	   ppflm[1][0] = (FloatLo) ((FloatLo)prm->rot_21 / prm->scale_factor);
	   ppflm[1][1] = (FloatLo) ((FloatLo)prm->rot_22 / prm->scale_factor);
	   ppflm[1][2] = (FloatLo) ((FloatLo)prm->rot_23 / prm->scale_factor);
	   ppflm[2][0] = (FloatLo) ((FloatLo)prm->rot_31 / prm->scale_factor);
	   ppflm[2][1] = (FloatLo) ((FloatLo)prm->rot_32 / prm->scale_factor);
	   ppflm[2][2] = (FloatLo) ((FloatLo)prm->rot_33 / prm->scale_factor);
 	  }
	if ((int) pm->choice == Move_translate)
	  {
	    pflv = (FloatLoPtr) pdntrnThis->data.ptrvalue;
	    ptm = (TransMatrixPtr) pm->data.ptrvalue;
	    pflv[0] = (FloatLo) ((FloatLo)ptm->tran_1 / ptm->scale_factor);
	    pflv[1] = (FloatLo) ((FloatLo)ptm->tran_2 / ptm->scale_factor);
	    pflv[2] = (FloatLo) ((FloatLo)ptm->tran_3 / ptm->scale_factor);
 	  }
	pm = pm->next;
    } 
}
 
void LIBCALLBACK DoApplyTransform(PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr)
{
  PMAD pmadAtom = NULL;
  PMOD pmodThis = NULL;
  PALD paldLoc = NULL;
  PMSD pmsdThis = NULL;
  PDNTRN pdntrnThis = NULL;
  FloatLoPtr pflv = NULL;
  FloatLoPtr pflvData = NULL;
  FloatLoPtrPtr ppflmData = NULL;
  FloatLoPtrPtr ppflm = NULL;  
  FloatLo flX,  flY,  flZ;
  Int2 i,  j;
  
  if (!ptr) return;
  
  if (IsAtomNode(pfbThis))
      {
          pmadAtom = (PMAD) pfbThis;
          paldLoc = GetAtomLocs(pmadAtom, iModel);
          pmsdThis = (PMSD) ptr;
          /* go through each location for this particular atom and model */ 
          while (paldLoc) 
            {    
	       pdntrnThis = (PDNTRN) ptr;  /* reset to head of transformations */
	       pflvData = paldLoc->pflvData;
               while (pdntrnThis)
	         {
		   if (pdntrnThis->choice == Move_translate)
		     {
		      pflv = (FloatLoPtr) pdntrnThis->data.ptrvalue;
	              pflvData[0] = pflvData[0] + pflv[0];
	              pflvData[1] = pflvData[1] + pflv[1];
	              pflvData[2] = pflvData[2] + pflv[2];
                     }
                  if (pdntrnThis->choice == Move_rotate)
	             {
		       ppflm = (FloatLoPtrPtr) pdntrnThis->data.ptrvalue;
	               flX =(pflvData[0] * ppflm[0][0] +
			     pflvData[1] * ppflm[1][0] +
			     pflvData[2] * ppflm[2][0]);
	               flY =(pflvData[0] * ppflm[0][1] +
			     pflvData[1] * ppflm[1][1] +
			     pflvData[2] * ppflm[2][1]);
	               flZ = (pflvData[0] * ppflm[0][2] +
			     pflvData[1]* ppflm[1][2] +
			     pflvData[2] * ppflm[2][2]);
		       pflvData[0] = flX;
		       pflvData[1] = flY;
		       pflvData[2] = flZ;	
        	     } 
	          pdntrnThis = pdntrnThis->next; /* get next transform */
               }
	      paldLoc = paldLoc->next; /* get next location */
          }  /* while paldLoc */
      }  /* if IsAtomNode */
      
    if (IsObjectNode(pfbThis))
      {
         /* deal with the objects too */
      pmodThis = (PMOD) pfbThis;
      pdntrnThis = (PDNTRN) ptr;  /* reset to head of transformations */
      ppflmData = pmodThis->ppflObject;
      while (pdntrnThis)
      {
        switch (pmodThis->bWhat)
        {
          
        case OBJ_CYLINDER:
		        i = 2;
            break;
        case OBJ_BRICK:
		        i = 8;
            break;
        default: i=0;
        }
        
        if (pdntrnThis->choice == Move_translate)
        {
          pflv = (FloatLoPtr) pdntrnThis->data.ptrvalue;
          for (j = 0; j<i; j++)
		        {		        
            pflvData = ppflmData[j];
            pflvData[0] = pflvData[0] + pflv[0];
            pflvData[1] = pflvData[1] + pflv[1];
            pflvData[2] = pflvData[2] + pflv[2];
		        }
        }
        if (pdntrnThis->choice == Move_rotate)
        {
          ppflm = (FloatLoPtrPtr) pdntrnThis->data.ptrvalue;
          for (j = 0; j<i; j++)
		        {	
            flX =(ppflmData[j][0] * ppflm[0][0] +
              ppflmData[j][1] * ppflm[1][0] +
              ppflmData[j][2] * ppflm[2][0]);
            flY =(ppflmData[j][0] * ppflm[0][1] +
              ppflmData[j][1] * ppflm[1][1] +
              ppflmData[j][2] * ppflm[2][1]);
            flZ = (ppflmData[j][0] * ppflm[0][2] +
              ppflmData[j][1]* ppflm[1][2] +
              ppflmData[j][2] * ppflm[2][2]);
            ppflmData[j][0] = flX;
            ppflmData[j][1] = flY;
            ppflmData[j][2] = flZ;	
            
          }
        }  
        pdntrnThis = pdntrnThis->next; /* get next transform */
      } /* while pdntrnThis */
   }  /* if IsObjectNode */     
} 


static void TransformMultiply(FloatLoPtr pflvData, FloatLoPtrPtr ppflm)
{
    Nlm_Matrix Matrix, Vector, Result, Invert;
    Nlm_Uint4 m, n;  /* matrix indices */

    Matrix = Nlm_MatrixNew(3, 3);
    if (!Matrix) return;
    Vector = Nlm_MatrixNew(1, 3);
    if (!Vector) return;

    for(m = 0; m < 3; m++) {
	for(n = 0; n < 3; n++) {
	    Nlm_MatrixSetNode(Matrix, m, n, (Nlm_FloatHi) ppflm[m][n]);
	}
    }
    for(m = 0; m < 3; m++) Nlm_MatrixSetNode(Vector, 0, m, (Nlm_FloatHi) pflvData[m]);
    Invert = Nlm_MatrixInvert(Matrix);
    Result = Nlm_MatrixMultiply(Vector, Invert);
    for(m = 0; m < 3; m++) pflvData[m] = (Nlm_FloatLo) Nlm_MatrixNode(Result, 0, m);
    Nlm_MatrixDelete(Matrix);
    Nlm_MatrixDelete(Vector);
    Nlm_MatrixDelete(Result);
    Nlm_MatrixDelete(Invert);
}


void LIBCALLBACK DoReverseTransform(PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr)
{
  PMAD pmadAtom = NULL;
  PMOD pmodThis = NULL;
  PALD paldLoc = NULL;
  PMSD pmsdThis = NULL;
  PDNTRN pdntrnThis = NULL;
  FloatLoPtr pflv = NULL;
  FloatLoPtr pflvData = NULL;
  FloatLoPtrPtr ppflmData = NULL;
  FloatLoPtrPtr ppflm = NULL;  
  /* FloatLo flX,  flY,  flZ; */
  Int2 i,  j;
  
  if (IsAtomNode(pfbThis))
      {
          pmadAtom = (PMAD) pfbThis;
          paldLoc = GetAtomLocs(pmadAtom, iModel);
          pmsdThis = (PMSD) ptr;
          /* go through each location for this particular atom and model */ 
          while (paldLoc) 
            {    
	       pdntrnThis = (PDNTRN) ptr;  /* reset to head of transformations */
	       /* then go to the end */
	       while (pdntrnThis->next)  pdntrnThis = pdntrnThis->next;
	       pflvData = paldLoc->pflvData;
               while (pdntrnThis)
	         {
		   if (pdntrnThis->choice == Move_translate)
		     {
		      pflv = (FloatLoPtr) pdntrnThis->data.ptrvalue;
	              pflvData[0] = pflvData[0] - pflv[0];
	              pflvData[1] = pflvData[1] - pflv[1];
	              pflvData[2] = pflvData[2] - pflv[2];
                     }
                  if (pdntrnThis->choice == Move_rotate)
	             {
		       ppflm = (FloatLoPtrPtr) pdntrnThis->data.ptrvalue;
		       TransformMultiply(pflvData, ppflm);
		       /*
	               flX =(pflvData[0] * ppflm[0][0] +
			     pflvData[1] * ppflm[1][0] +
			     pflvData[2] * ppflm[2][0]);
	               flY =(pflvData[0] * ppflm[0][1] +
			     pflvData[1] * ppflm[1][1] +
			     pflvData[2] * ppflm[2][1]);
	               flZ = (pflvData[0] * ppflm[0][2] +
			     pflvData[1]* ppflm[1][2] +
			     pflvData[2] * ppflm[2][2]);
		       pflvData[0] = flX;
		       pflvData[1] = flY;
		       pflvData[2] = flZ;
		       */
        	     } 
	          pdntrnThis = pdntrnThis->last; /* get previous transform */
               }
	      paldLoc = paldLoc->next; /* get next location */
          }  /* while paldLoc */
      }  /* if IsAtomNode */
      
    if (IsObjectNode(pfbThis))
      {
         /* deal with the objects too */
      pmodThis = (PMOD) pfbThis;
      pdntrnThis = (PDNTRN) ptr;  /* reset to head of transformations */
      /* then go to the end */
      while (pdntrnThis->next)  pdntrnThis = pdntrnThis->next;
      ppflmData = pmodThis->ppflObject;
      while (pdntrnThis)
      {
        switch (pmodThis->bWhat)
        {
          
        case OBJ_CYLINDER:
		        i = 2;
            break;
        case OBJ_BRICK:
		        i = 8;
            break;
        default: i=0;
        }
        
        if (pdntrnThis->choice == Move_translate)
        {
          pflv = (FloatLoPtr) pdntrnThis->data.ptrvalue;
          for (j = 0; j<i; j++)
		        {		        
            pflvData = ppflmData[j];
            pflvData[0] = pflvData[0] - pflv[0];
            pflvData[1] = pflvData[1] - pflv[1];
            pflvData[2] = pflvData[2] - pflv[2];
		        }
        }
        if (pdntrnThis->choice == Move_rotate)
        {
          ppflm = (FloatLoPtrPtr) pdntrnThis->data.ptrvalue;
          for (j = 0; j<i; j++)
		        {
	TransformMultiply(ppflmData[j], ppflm);
	/*
            flX =(ppflmData[j][0] * ppflm[0][0] +
              ppflmData[j][1] * ppflm[1][0] +
              ppflmData[j][2] * ppflm[2][0]);
            flY =(ppflmData[j][0] * ppflm[0][1] +
              ppflmData[j][1] * ppflm[1][1] +
              ppflmData[j][2] * ppflm[2][1]);
            flZ = (ppflmData[j][0] * ppflm[0][2] +
              ppflmData[j][1]* ppflm[1][2] +
              ppflmData[j][2] * ppflm[2][2]);
            ppflmData[j][0] = flX;
            ppflmData[j][1] = flY;
            ppflmData[j][2] = flZ;
	*/
            
          }
        }  
        pdntrnThis = pdntrnThis->last; /* get next transform */
      } /* while pdntrnThis */
   }  /* if IsObjectNode */     
} 



PDNMS LIBCALL FindLoadedBiostruc(CharPtr pcPDBID, Int4 iId)
{
 /* replace this with caching mechanism */
 PDNMS pdnmsThis = NULL;
 PMSD pmsdThis = NULL;
 
 if ((pcPDBID == NULL) && (iId == 0)) return NULL;

 pdnmsThis = GetFirstModelstruc( );
 while (pdnmsThis)
  {
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    if (pmsdThis->iMMDBid == iId) return pdnmsThis;    
    if (!(StringCmp(pcPDBID,pmsdThis->pcPDBName))) return pdnmsThis;
    pdnmsThis = pdnmsThis->next;
  }
 return NULL; 
}

Boolean InstBSAnnotSet(BiostrucAnnotSetPtr pbsasThis, CharPtr JobID,
		       Boolean Chain, CharPtr Path) {
  Int2 iTest;
  Int4 iMMDBId;
  PDNMS pdnmsThis = NULL;
  PMSD  pmsdThis = NULL;
  BiostrucPtr pbsThis = NULL, pbsTemp = NULL;
  BiostrucIdPtr pbsidThis = NULL;
  BiostrucFeatureSetPtr pbsfsThis = NULL;
  BiostrucFeaturePtr pbsfThis = NULL;
  PSFS psfsThis = NULL;

  CharPtr szTemp;		/* NEW - K.A. 5/4/99 for pruning master/slave */
  Char szName[5];
  Char AsnPath[PATH_MAX];
  Char AsnName[10];
  Char MasterChain[2];
  Char SlaveChain[2];

 /* the Feature set either attaches to an in-memory Modelstruc */
 /* or, if it specifies a new Modelstruc - attempts a retrieval */
 
  if (pbsasThis == NULL) return FALSE;
  MasterChain[0] = '\0';
  SlaveChain[0] = '\0';

/* grab the id out of the Biostruc-descr */

  pbsidThis = ValNodeFindNext(pbsasThis->id,NULL,BiostrucId_mmdb_id);
  if (pbsidThis)  /* only deal with the first one */
    {
      iMMDBId = (Int4) pbsidThis->data.intvalue;
      /*printf("iMMDBID=%ld\n", iMMDBId);*//*master MMDB_ID*/
    }

  pdnmsThis = FindLoadedBiostruc(NULL, iMMDBId);

  if (!pdnmsThis) {
    szTemp = pbsasThis->features->features->name;  
    szName[0] = szTemp[0];
    szName[1] = szTemp[1];
    szName[2] = szTemp[2];
    szName[3] = szTemp[3];
    szName[4] = '\0';
    /* try to load it using all NCBI models */
    /* JobID points to dir containing biostruc of a query protein from VAST Search */  

    /* First get the structure using MMDBBiostrucGet or FetchBS, then prune if needed */

    if (JobID == NULL)
      pbsThis = MMDBBiostrucGet(iMMDBId, ALLSIMPLEMDL, 3);
    else {
      AsnName[0]='\0';
      StringCpy(AsnName, "/b");
      StringCat(AsnName, szName);
      AsnPath[0]='\0';
      StringCpy(AsnPath, Path);
      StringCat(AsnPath, JobID);
      StringCat(AsnPath, AsnName);
      pbsThis = FetchBS(AsnPath, 0, ALLSIMPLEMDL, 3, POWER_VIEW);
    }

    if (Chain) {
      if (szTemp[4] != ' ') {
        MasterChain[0] = szTemp[4];
        MasterChain[1] = '\0';
        pbsTemp = (BiostrucPtr)PruneBiostruc(pbsThis, MasterChain);
        pbsThis = NULL;
        pbsThis = pbsTemp;
      }
      SlaveChain[0] = szTemp[11];
      SlaveChain[1] = '\0';
    } 

    if (!pbsThis) goto nogomem;
    pdnmsThis =  MakeAModelstruc(pbsThis);
    /* side effect is that this is now the selected modelstruc too */
    if (!pdnmsThis) goto nogomem;  
  }
  pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;

  /****** WARNING: fix this if this code is ever used to recognize Neighbor mode *********/
  SetSelectedModelstruc(pdnmsThis);  /* set it to selected one */
  /* at this point we have annot-set, and its associated Modelstruc */

  pbsfsThis = pbsasThis->features;

  while (pbsfsThis)  /* walk through each feature-set */
    {
    iTest = BiostrucAddFeature(pbsfsThis,pdnmsThis, Chain, SlaveChain);  /* new slave args passed */
    if (iTest < 0) goto nogomem; /* a malloc error */
      if (!iTest) goto nogo;  /* bad feature table error - fatal */
      pbsfsThis = pbsfsThis->next;
    }
  /* given a sucessfully registered Biostruc-annot-set
     we can now free it */
  BiostrucAnnotSetFree(pbsasThis);
  return TRUE;
  
nogomem:
nogo:
  BiostrucAnnotSetFree(pbsasThis);
  return FALSE;
}   

/************************************************************
produce a sequence alignment from a structure-alignment
:)bug fixed: need two loops to go through master and slave selection with
BiostrucAnnotSetPtr->features (master selection), and 
BiostrucAnnotSetPtr->features->features (slave selection) 
in order to pick both master and slave with defination
of molecule name, chain, and domain
***********************************************************/
SeqAnnotPtr LIBCALL BiosToSeq (BiostrucAnnotSetPtr set, Boolean usePValue, 
                               Char* pdbname_master, Char* pdbname_slave)
{
  BiostrucFeatureSetPtr pbsfs=NULL;
  BiostrucFeaturePtr pbsf=NULL;
  SeqAnnotPtr     sap = NULL;
  SeqAlignPtr     salp;
  ScorePtr        scp;
  Int2 		  count;
  ObjectIdPtr     oid;
  DenseSegPtr     dsp;
  SeqIdPtr        mastersip = NULL, slavesip;
  SeqIdPtr        pdbsip = NULL;
  BiostrucFeaturePtr feature;
  ChemGraphAlignmentPtr pcga1 = NULL;
  ValNodePtr      loc1 = NULL;
  ValNodePtr      vnp = NULL;
  SeqAlignPtr     tail = NULL;
  ChemGraphPntrsPtr cgpp;
  ResidueIntervalPntrPtr master;
  ResidueIntervalPntrPtr slave;
  ResidueIntervalPntrPtr masterseg;
  ResidueIntervalPntrPtr slaveseg;
  CharPtr         pcPDB;
  int             iDomain;
  ValNodePtr      pvn;
  Char            cChain;
  Char            buf[100];
  Char            buf2[50];
  Int4 nextmasterstart, nextslavestart;

  if (set == NULL || set->features == NULL || set->features->features == NULL)
    return NULL;
  pbsfs=set->features;
  while(pbsfs)
  {
     feature=pbsfs->features;
     /*if master name matched*/
    if(strcmp((CharPtr)(pbsfs->descr->data.ptrvalue), pdbname_master) == 0)
    {
     while(feature)
    {

    /* construct the master SeqId if it hasn't already been constructed */
    if (mastersip == NULL) {
      pcPDB = StringSave (PDBNAME_DEFAULT);
      iDomain = 0;
      cChain = ' ';
      pcPDB[0] = feature->name[0];
      pcPDB[1] = feature->name[1];
      pcPDB[2] = feature->name[2];
      pcPDB[3] = feature->name[3];
      cChain = feature->name[4];
      iDomain = atoi ((char *) &feature->name[5]);
      mastersip = MakePDBSeqId2 (pcPDB, cChain, iDomain, TRUE);
      pdbsip = MakePDBSeqId2 (pcPDB, cChain, iDomain, FALSE);
   
      MemFree (pcPDB);
    }
    /* get the embedded PDB code of the hit */
    pcPDB = StringSave (PDBNAME_DEFAULT);
    iDomain = 0;
    cChain = '-';

    if (StringLen (feature->name) >= 13) {
      pcPDB[0] = feature->name[7];
      pcPDB[1] = feature->name[8];
      pcPDB[2] = feature->name[9];
      pcPDB[3] = feature->name[10];
      cChain = feature->name[11];
      iDomain = atoi ((char *) &feature->name[12]);
      slavesip = MakePDBSeqId2 (pcPDB, cChain, iDomain, TRUE);
    
      MemFree (pcPDB);
    }

pcPDB[4] = feature->name[11];
pcPDB[5] = feature->name[12];
pcPDB[6]='\0';	
  
 if (strcmp(pcPDB, pdbname_slave) == 0)
{
    pvn = ValNodeFindNext (feature->Location_location, NULL, Location_location_alignment);
    if (pvn)
      pcga1 = (ChemGraphAlignmentPtr) pvn->data.ptrvalue;

    vnp = pcga1->alignment;

    if (vnp->choice != ChemGraphPntrs_residues)
      continue;

    cgpp = (ChemGraphPntrsPtr) vnp->data.ptrvalue;

    if (cgpp->choice != ResiduePntrs_interval)
      continue;
    master = (ResidueIntervalPntrPtr) cgpp->data.ptrvalue;
    cgpp = (ChemGraphPntrsPtr) vnp->next->data.ptrvalue;
    slave = (ResidueIntervalPntrPtr) cgpp->data.ptrvalue; 
    
    for (nextmasterstart = 1, nextslavestart = 1, count = 0, masterseg = master, 
          slaveseg = slave; masterseg != NULL && slaveseg != NULL; masterseg = masterseg->next,
           slaveseg = slaveseg->next) {
      if (count > 0 && nextmasterstart < masterseg->from)
	  count++;
      if (count > 0 && nextslavestart < slaveseg->from)
	  count++;
      count++;
      nextmasterstart = masterseg->to + 1;
      nextslavestart = slaveseg->to + 1;
    } 


    salp = SeqAlignNew ();
    if (sap == NULL) {
      sap = SeqAnnotNew ();
      sap->type = 2;		/* align */
      sap->data = salp;
      sap->desc = ValNodeNew(NULL);
      sap->desc->choice = 2; /* title */
      SeqIdWrite (pdbsip, buf2, PRINTID_FASTA_LONG, sizeof(buf2));
      sprintf (buf, "VAST structural alignment for %s, mapped to sequence alignment", buf2);
      sap->desc->data.ptrvalue = StringSave(buf);
    } else {
      tail->next = salp;
    }
    tail = salp;
    salp->type = 3 /* partial */ ;
    salp->dim = 2;

    scp = ScoreNew ();
    salp->score = scp;
    salp->segtype = 2 /* denseg */ ;
    scp->choice = 2;		/* real */
    scp->value.realvalue = ((FloatHi) pcga1->aligndata->vast_score) / pcga1->aligndata->scale_factor;
    oid = ObjectIdNew ();
    scp->id = oid;
    oid->str = StringSave ("VAST score");
    scp->next = ScoreNew ();
    scp = scp->next;
    scp->choice = 2;		/* real */
    scp->value.realvalue = ((FloatHi) pcga1->aligndata->vast_mlogp) / pcga1->aligndata->scale_factor;
    oid = ObjectIdNew ();
    scp->id = oid;
    oid->str = StringSave ("VAST p-value");
    scp->next = ScoreNew ();
    scp = scp->next;
    scp->choice = 2;		/* real */
    scp->value.realvalue = ((FloatHi) pcga1->aligndata->rmsd) / pcga1->aligndata->scale_factor;
    oid = ObjectIdNew ();
    scp->id = oid;
    oid->str = StringSave ("VAST root mean square deviation, measured in angstroms");
    scp->next = ScoreNew ();
    scp = scp->next;
    scp->choice = 1;		/* int */
    scp->value.intvalue = pcga1->aligndata->align_res;
    oid = ObjectIdNew ();
    scp->id = oid;
    oid->str = StringSave ("VAST aligned residue count");

    dsp = DenseSegNew ();
    salp->segs = (Pointer) dsp;
    dsp->dim = 2;
    dsp->numseg = count;
    dsp->ids = SeqIdDup (mastersip);
    dsp->ids->next = slavesip;
    dsp->starts = (Int4Ptr) MemNew (count * sizeof (Int4) * 2);
    dsp->lens = (Int4Ptr) MemNew (count * sizeof (Int4));

    for (nextmasterstart = 1, nextslavestart = 1, count = 0, masterseg = master, slaveseg = slave; masterseg != NULL && slaveseg != NULL; masterseg = masterseg->next, slaveseg = slaveseg->next) {
      if (count > 0 && nextmasterstart < masterseg->from)
      { /* add a gap */
        dsp->starts[count * 2] = nextmasterstart - 1;
        dsp->starts[count * 2 + 1] = -1;
        dsp->lens[count] = masterseg->from - nextmasterstart;
	count++;
      }
      if (count > 0 && nextslavestart < slaveseg->from)
      { /* add a gap */
        dsp->starts[count * 2 + 1] = nextslavestart - 1;
        dsp->starts[count * 2] = -1;
        dsp->lens[count] = slaveseg->from - nextslavestart;
	count++;
      }
      dsp->starts[count * 2] = masterseg->from-1;
      dsp->starts[count * 2 + 1] = slaveseg->from-1;
      dsp->lens[count] = masterseg->to - masterseg->from + 1;
      count++;
      nextmasterstart = masterseg->to + 1;
      nextslavestart = slaveseg->to + 1;
       }
      } /*if pdbname matched*/
      feature=feature->next;
    }/*while feature (pbsf)*/
    }/*if pcMaster matches pdbname_master*/
    pbsfs=pbsfs->next;
  }/*while pbsfs*/
  SeqIdFree (mastersip);
  SeqIdFree (pdbsip);
  return sap;
}



SeqAnnotPtr LIBCALL fnPBSFtoPSA (BiostrucFeaturePtr pbsfSelected)
{
  SeqAnnotPtr     sap = NULL;
  SeqAlignPtr     salp;
  ScorePtr        scp;
  Int2 		  count;
  ObjectIdPtr     oid;
  DenseSegPtr     dsp;
  ValNodePtr      loc1 = NULL;
  SeqAlignPtr     tail = NULL;
  ChemGraphPntrsPtr cgpp;
  ResidueIntervalPntrPtr master;
  ResidueIntervalPntrPtr slave;
  ResidueIntervalPntrPtr masterseg;
  ResidueIntervalPntrPtr slaveseg;
  Char            buf[100];
  Char            buf2[50];
  Int4 nextmasterstart, nextslavestart;
  SeqIdPtr        mastersip = NULL, slavesip;
  SeqIdPtr        pdbsip = NULL;
  int             iDomain;
  CharPtr         pcPDB;
  Char            cChain;
  ValNodePtr pvnAlignment;
  ChemGraphAlignmentPtr pcga1;

  Boolean choose_densediag = TRUE;
    
    pvnAlignment = ValNodeFindNext(pbsfSelected->Location_location,NULL,
    Location_location_alignment);
     pcga1 = (ChemGraphAlignmentPtr) pvnAlignment->data.ptrvalue;

     if (pcga1->alignment->choice != ChemGraphPntrs_residues)
      return NULL;

    cgpp = (ChemGraphPntrsPtr) pcga1->alignment->data.ptrvalue;
  
  pcPDB = StringSave (PDBNAME_DEFAULT);
  iDomain = 0;
  cChain = '-';
  pcPDB[0] = pbsfSelected->name[0];
  pcPDB[1] = pbsfSelected->name[1];
  pcPDB[2] = pbsfSelected->name[2];
  pcPDB[3] = pbsfSelected->name[3];
  cChain = pbsfSelected->name[4];
  iDomain = atoi ((char *) &pbsfSelected->name[5]);
/*mastersip = MakePDBSeqId2 (pcPDB, cChain, iDomain, TRUE); */
  mastersip = MakePDBSeqId2 (pcPDB, cChain, iDomain, FALSE);
  pdbsip = MakePDBSeqId2 (pcPDB, cChain, iDomain, FALSE);
  
  MemFree (pcPDB);
  
  /* get the embedded PDB code of the hit */
 if (iDomain < 10) {
  pcPDB = StringSave (PDBNAME_DEFAULT);
  iDomain = 0;
  cChain = '-';
  
  pcPDB[0] = pbsfSelected->name[7];
  pcPDB[1] = pbsfSelected->name[8];
  pcPDB[2] = pbsfSelected->name[9];
  pcPDB[3] = pbsfSelected->name[10];
  cChain = pbsfSelected->name[11];
  iDomain = atoi ((char *) &pbsfSelected->name[12]);
 }
 else {		/* have at least 10 domains in 1 str., added by J. Chen */
  pcPDB = StringSave (PDBNAME_DEFAULT);
  iDomain = 0;
  cChain = '-';
 
  pcPDB[0] = pbsfSelected->name[8];
  pcPDB[1] = pbsfSelected->name[9];
  pcPDB[2] = pbsfSelected->name[10];
  pcPDB[3] = pbsfSelected->name[11];
  cChain = pbsfSelected->name[12];
  iDomain = atoi ((char *) &pbsfSelected->name[13]);
 }

/*slavesip = MakePDBSeqId2 (pcPDB, cChain, iDomain, TRUE);  */
  slavesip = MakePDBSeqId2 (pcPDB, cChain, iDomain, FALSE);
  
  MemFree (pcPDB);
  
  if (cgpp->choice != ResiduePntrs_interval)
    return NULL;
  master = (ResidueIntervalPntrPtr) cgpp->data.ptrvalue;
  cgpp = (ChemGraphPntrsPtr) pcga1->alignment->next->data.ptrvalue;
  slave = (ResidueIntervalPntrPtr) cgpp->data.ptrvalue; 
  
  for (nextmasterstart = 1, nextslavestart = 1, count = 0, masterseg = master, 
    slaveseg = slave; masterseg != NULL && slaveseg != NULL; masterseg = masterseg->next,
    slaveseg = slaveseg->next) {
    if (count > 0 && nextmasterstart < masterseg->from)
      count++;
    if (count > 0 && nextslavestart < slaveseg->from)
      count++;
    count++;
    nextmasterstart = masterseg->to + 1;
    nextslavestart = slaveseg->to + 1;
  } 
  
  salp = SeqAlignNew ();
  if (sap == NULL) {
    sap = SeqAnnotNew ();
    sap->type = 2;		/* align */
    sap->data = salp;
    sap->desc = ValNodeNew(NULL);
    sap->desc->choice = 2; /* title */
    SeqIdWrite (pdbsip, buf2, PRINTID_FASTA_LONG, sizeof(buf2));
    sprintf (buf, "VAST structural alignment for %s, mapped to sequence alignment", buf2);
    sap->desc->data.ptrvalue = StringSave(buf);
  } else {
    tail->next = salp;
  }
  tail = salp;
  salp->type = 3 /* partial */ ;
  salp->dim = 2;
  
  scp = ScoreNew ();
  salp->score = scp;
  salp->segtype = 2 /* denseg */ ;
  scp->choice = 2;		/* real */
  scp->value.realvalue = ((FloatHi) pcga1->aligndata->vast_score) / pcga1->aligndata->scale_factor;
  oid = ObjectIdNew ();
  scp->id = oid;
  oid->str = StringSave ("VAST score");
  scp->next = ScoreNew ();
  scp = scp->next;
  scp->choice = 2;		/* real */
  scp->value.realvalue = ((FloatHi) pcga1->aligndata->vast_mlogp) / pcga1->aligndata->scale_factor;
  oid = ObjectIdNew ();
  scp->id = oid;
  oid->str = StringSave ("VAST p-value");
  scp->next = ScoreNew ();
  scp = scp->next;
  scp->choice = 2;		/* real */
  scp->value.realvalue = ((FloatHi) pcga1->aligndata->rmsd) / pcga1->aligndata->scale_factor;
  oid = ObjectIdNew ();
  scp->id = oid;
  oid->str = StringSave ("VAST root mean square deviation, measured in angstroms");
  scp->next = ScoreNew ();
  scp = scp->next;
  scp->choice = 1;		/* int */
  scp->value.intvalue = pcga1->aligndata->align_res;
  oid = ObjectIdNew ();
  scp->id = oid;
  oid->str = StringSave ("VAST aligned residue count");
  
 if(!choose_densediag){
  dsp = DenseSegNew ();
  salp->segs = (Pointer) dsp;
  dsp->dim = 2;
  dsp->numseg = count;
  dsp->ids = SeqIdDup (mastersip);
 /* dsp->ids->next = slavesip;  could be a bug, J. Chen */
  dsp->ids->next = SeqIdDup(slavesip);
  dsp->starts = (Int4Ptr) MemNew (count * sizeof (Int4) * 2);
  dsp->lens = (Int4Ptr) MemNew (count * sizeof (Int4));
  
  for (nextmasterstart = 1, nextslavestart = 1, count = 0, masterseg = master, slaveseg = slave; masterseg != NULL && slaveseg != NULL; masterseg = masterseg->next, slaveseg = slaveseg->next) {
    if (count > 0 && nextmasterstart < masterseg->from)
    { /* add a gap */
      dsp->starts[count * 2] = nextmasterstart - 1;
      dsp->starts[count * 2 + 1] = -1;
      dsp->lens[count] = masterseg->from - nextmasterstart;
      count++;
    }
    if (count > 0 && nextslavestart < slaveseg->from)
    { /* add a gap */
      dsp->starts[count * 2 + 1] = nextslavestart - 1;
      dsp->starts[count * 2] = -1;
      dsp->lens[count] = slaveseg->from - nextslavestart;
      count++;
    }
    dsp->starts[count * 2] = masterseg->from-1;
    dsp->starts[count * 2 + 1] = slaveseg->from-1;
    dsp->lens[count] = masterseg->to - masterseg->from + 1;
    count++;
    nextmasterstart = masterseg->to + 1;
    nextslavestart = slaveseg->to + 1;
  }
 }
 else {
 DenseDiagPtr ddp, ddp_tmp;

  salp->segs = NULL;
  salp->type = 3;
  salp->segtype = 1  /* densediag */ ;

  for(count = 0, masterseg = master, slaveseg = slave; masterseg != NULL && slaveseg != NULL; masterseg = masterseg->next, slaveseg = slaveseg->next) {
     ddp = DenseDiagNew();
     ddp->dim = 2;

     ddp->id = SeqIdDup (mastersip); 
/*     ddp->id->next = slavesip;    bug! fixed by J.Chen */
     ddp->id->next = SeqIdDup(slavesip);

     ddp->starts = (Int4Ptr) MemNew ((size_t) 4 * sizeof (Int4));
     ddp->starts[0] = masterseg->from-1;
     ddp->starts[1] = slaveseg->from -1 ;
     ddp->len = masterseg->to - masterseg->from  + 1;
     count++;
     if(salp->segs == NULL) salp->segs = (Pointer) ddp;
     else {
        ddp_tmp = (DenseDiagPtr) salp->segs;
        while(ddp_tmp->next) ddp_tmp = ddp_tmp->next;
        ddp_tmp->next = ddp;
     }
  }

 }

  
  SeqIdFree (mastersip);
  SeqIdFree (pdbsip);
  slavesip->next = NULL;

  return sap;
}

/************************************************************
Produce sequence annotation set of a master (name, chain, domain)
with all its slaves according to structure alignments.
************************************************************/

/* please don't use this function, use the function above */

SeqAnnotPtr LIBCALL BiostrToSeqAnnotSet (BiostrucAnnotSetPtr set, 
                               Boolean usePValue, Char* pdbname_master)
{
  BiostrucFeatureSetPtr pbsfs=NULL;
  BiostrucFeaturePtr pbsf=NULL;
  SeqAnnotPtr     sap = NULL;
  SeqAlignPtr     salp;
  ScorePtr        scp;
  Int2 		  count;
  ObjectIdPtr     oid;
  DenseSegPtr     dsp;
  SeqIdPtr        mastersip = NULL, slavesip;
  SeqIdPtr        pdbsip = NULL;
  BiostrucFeaturePtr feature;
  ChemGraphAlignmentPtr pcga1 = NULL;
  ValNodePtr      loc1 = NULL;
  ValNodePtr      vnp = NULL;
  SeqAlignPtr     tail = NULL;
  ChemGraphPntrsPtr cgpp;
  ResidueIntervalPntrPtr master;
  ResidueIntervalPntrPtr slave;
  ResidueIntervalPntrPtr masterseg;
  ResidueIntervalPntrPtr slaveseg;
  CharPtr         pcPDB;
  Char  pcMaster[10];  /* fixed lyg */
  int             iDomain;
  ValNodePtr      pvn;
  Char            cChain;
  Char            buf[100];
  Char            buf2[50];
  Int4 nextmasterstart, nextslavestart;

  if (set == NULL || set->features == NULL || set->features->features == NULL)
    return NULL;
  pbsfs=set->features;
  while(pbsfs)
  {
     feature=pbsfs->features;
     strcpy(pcMaster, (char *)pbsfs->descr->data.ptrvalue);
     /*if master name matched*/
    if(strcmp(pcMaster, pdbname_master) == 0) /* I had this commented out */
    {
     while(feature)
    {

    /* construct the master SeqId if it hasn't already been constructed */
    if (mastersip == NULL) {
      pcPDB = StringSave (PDBNAME_DEFAULT);
      iDomain = 0;
      cChain = '-';
      pcPDB[0] = feature->name[0];
      pcPDB[1] = feature->name[1];
      pcPDB[2] = feature->name[2];
      pcPDB[3] = feature->name[3];
      cChain = feature->name[4];
      iDomain = atoi ((char *) &feature->name[5]);
      mastersip = MakePDBSeqId2 (pcPDB, cChain, iDomain, TRUE);
      pdbsip = MakePDBSeqId2 (pcPDB, cChain, iDomain, FALSE);
   
      MemFree (pcPDB);
    }
    /* get the embedded PDB code of the hit */
    pcPDB = StringSave (PDBNAME_DEFAULT);
    iDomain = 0;
    cChain = '-';

    if (StringLen (feature->name) >= 13) {
      pcPDB[0] = feature->name[7];
      pcPDB[1] = feature->name[8];
      pcPDB[2] = feature->name[9];
      pcPDB[3] = feature->name[10];
      cChain = feature->name[11];
      iDomain = atoi ((char *) &feature->name[12]);
      slavesip = MakePDBSeqId2 (pcPDB, cChain, iDomain, TRUE);
    
      MemFree (pcPDB);
    }

pcPDB[4] = feature->name[11];
pcPDB[5] = feature->name[12];
pcPDB[6]='\0';	

    pvn = ValNodeFindNext (feature->Location_location, NULL, Location_location_alignment);
    if (pvn)
      pcga1 = (ChemGraphAlignmentPtr) pvn->data.ptrvalue;

    vnp = pcga1->alignment;

    if (vnp->choice != ChemGraphPntrs_residues)
      continue;

    cgpp = (ChemGraphPntrsPtr) vnp->data.ptrvalue;

    if (cgpp->choice != ResiduePntrs_interval)
      continue;
    master = (ResidueIntervalPntrPtr) cgpp->data.ptrvalue;
    cgpp = (ChemGraphPntrsPtr) vnp->next->data.ptrvalue;
    slave = (ResidueIntervalPntrPtr) cgpp->data.ptrvalue; 
    
    for (nextmasterstart = 1, nextslavestart = 1, count = 0, masterseg = master, 
          slaveseg = slave; masterseg != NULL && slaveseg != NULL; masterseg = masterseg->next,
           slaveseg = slaveseg->next) {
      if (count > 0 && nextmasterstart < masterseg->from)
	  count++;
      if (count > 0 && nextslavestart < slaveseg->from)
	  count++;
      count++;
      nextmasterstart = masterseg->to + 1;
      nextslavestart = slaveseg->to + 1;
    } 


    salp = SeqAlignNew ();
    if (sap == NULL) {
      sap = SeqAnnotNew ();
      sap->type = 2;		/* align */
      sap->data = salp;
      sap->desc = ValNodeNew(NULL);
      sap->desc->choice = 2; /* title */
      SeqIdWrite (pdbsip, buf2, PRINTID_FASTA_LONG, sizeof(buf2));
      buf2[10]=pdbname_master[5];
      buf2[11]='\0';
      sprintf (buf, "VAST structural alignment for %s, mapped to sequence alignment", buf2);
      sap->desc->data.ptrvalue = StringSave(buf);
    } else {
      tail->next = salp;
    }
    tail = salp;
    salp->type = 3 /* partial */ ;
    salp->dim = 2;

    scp = ScoreNew ();
    salp->score = scp;
    salp->segtype = 2 /* denseg */ ;
    scp->choice = 2;		/* real */
    scp->value.realvalue = ((FloatHi) pcga1->aligndata->vast_score) / pcga1->aligndata->scale_factor;
    oid = ObjectIdNew ();
    scp->id = oid;
    oid->str = StringSave ("VAST score");
    scp->next = ScoreNew ();
    scp = scp->next;
    scp->choice = 2;		/* real */
    scp->value.realvalue = ((FloatHi) pcga1->aligndata->vast_mlogp) / pcga1->aligndata->scale_factor;
    oid = ObjectIdNew ();
    scp->id = oid;
    oid->str = StringSave ("VAST p-value");
    scp->next = ScoreNew ();
    scp = scp->next;
    scp->choice = 2;		/* real */
    scp->value.realvalue = ((FloatHi) pcga1->aligndata->rmsd) / pcga1->aligndata->scale_factor;
    oid = ObjectIdNew ();
    scp->id = oid;
    oid->str = StringSave ("VAST root mean square deviation, measured in angstroms");
    scp->next = ScoreNew ();
    scp = scp->next;
    scp->choice = 1;		/* int */
    scp->value.intvalue = pcga1->aligndata->align_res;
    oid = ObjectIdNew ();
    scp->id = oid;
    oid->str = StringSave ("VAST aligned residue count");

    dsp = DenseSegNew ();
    salp->segs = (Pointer) dsp;
    dsp->dim = 2;
    dsp->numseg = count;
    dsp->ids = SeqIdDup (mastersip);
    dsp->ids->next = slavesip;
    dsp->starts = (Int4Ptr) MemNew (count * sizeof (Int4) * 2);
    dsp->lens = (Int4Ptr) MemNew (count * sizeof (Int4));

    for (nextmasterstart = 1, nextslavestart = 1, count = 0, masterseg = master, slaveseg = slave; masterseg != NULL && slaveseg != NULL; masterseg = masterseg->next, slaveseg = slaveseg->next) {
      if (count > 0 && nextmasterstart < masterseg->from)
      { /* add a gap */
        dsp->starts[count * 2] = nextmasterstart - 1;
        dsp->starts[count * 2 + 1] = -1;
        dsp->lens[count] = masterseg->from - nextmasterstart;
	count++;
      }
      if (count > 0 && nextslavestart < slaveseg->from)
      { /* add a gap */
        dsp->starts[count * 2 + 1] = nextslavestart - 1;
        dsp->starts[count * 2] = -1;
        dsp->lens[count] = slaveseg->from - nextslavestart;
	count++;
      }
      dsp->starts[count * 2] = masterseg->from-1;
      dsp->starts[count * 2 + 1] = slaveseg->from-1;
      dsp->lens[count] = masterseg->to - masterseg->from + 1;
      count++;
      nextmasterstart = masterseg->to + 1;
      nextslavestart = slaveseg->to + 1;
       }
      feature=feature->next;
    }/*while feature (pbsf)*/
    }/*if pcMaster matches pdbname_master*/
    pbsfs=pbsfs->next;
  }/*while pbsfs*/
  SeqIdFree (mastersip);
  SeqIdFree (pdbsip);
  return sap;
}

void LIBCALL ResolveAlignChain(PDNMS pdnmsThis)
{  /* moves the reserved byte value up to pmmd to indicate 
     chain is in alignment */
    PMSD pmsdThis = NULL;
    PDNMM pdnmmThis = NULL;
    PMMD pmmdThis = NULL;
    PDNMG pdnmgThis = NULL;
    PMGD pmgdThis = NULL;
    int i, j;
 
    if (!pdnmsThis) return;
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;

    pdnmmThis = pmsdThis->pdnmmHead;
    /*printf("iMMDBid=%ld, iModels=%i\n", pmsdThis->iMMDBid, pmsdThis->iModels);*/
    i=1;
    while (pdnmmThis)
      {
         pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
	 /*printf("I am at #358, pmmdThis->bReserved=%x\n", pmmdThis->bReserved);*/
         pdnmgThis = pmmdThis->pdnmgHead;
	 j=1;
         while(pdnmgThis)
	  {
	      /*printf("loop pdnmg=%i, loop pdnmm=%i\n", j, i);*/
	      pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
	     /* printf("I am at #360, %x\n", pmgdThis->bReserved);*/
              if (pmgdThis->bReserved)
                pmmdThis->bReserved = pmgdThis->bReserved;
/*I added the following two lines

	      pmmdThis->bReserved = 1;
	      pmgdThis->bReserved = 1;
*/
	      
	      pdnmgThis = pdnmgThis->next;
	      j++;
  	  } 
         pdnmmThis = pdnmmThis->next;
	 i++;
      }
}


int LIBCALLBACK CompareScores(VoidPtr vp1, VoidPtr vp2)
{
        BiostrucFeaturePtr pbf1, pbf2;
	ChemGraphAlignmentPtr pcga1 = NULL;
	ChemGraphAlignmentPtr pcga2 = NULL;
	ValNodePtr loc1 = NULL;
	ValNodePtr loc2 = NULL;
  	ValNodePtr vnp1, vnp2;
	ValNodePtr PNTR vnpp1;
	ValNodePtr PNTR vnpp2;

	vnpp1 = (ValNodePtr PNTR) vp1;
	vnpp2 = (ValNodePtr PNTR) vp2;
	vnp1 = *vnpp1;
	vnp2 = *vnpp2;

	pbf1 = (BiostrucFeaturePtr) vnp1->data.ptrvalue;
	pbf2 = (BiostrucFeaturePtr) vnp2->data.ptrvalue;

/* First, three cases that should not be, need to be checked for. */
	if (pbf1 == NULL && pbf2 != NULL)
		return -1;
	if (pbf1 != NULL && pbf2 == NULL)
		return 1;
	if (pbf1 == NULL && pbf2 == NULL)
		return 0;


        /* get at the appropriate data element */
	
        loc1 = pbf1->Location_location;
	loc2 = pbf2->Location_location;
	if ((loc1) && (loc1->choice == Location_location_alignment))
	    pcga1 = (ChemGraphAlignmentPtr) loc1->data.ptrvalue;
	if ((loc2) && (loc2->choice == Location_location_alignment))
	    pcga2 = (ChemGraphAlignmentPtr) loc2->data.ptrvalue;
 	
 	if (pcga1 == NULL && pcga2 != NULL)
		return -1;
	if (pcga1 != NULL && pcga2 == NULL)
		return 1;
	if (pcga1 == NULL && pcga2 == NULL)
		return 0;
		
	if (pcga1->aligndata == NULL && pcga2->aligndata != NULL)
		return -1;
	if (pcga1->aligndata != NULL && pcga2->aligndata == NULL)
		return 1;
	if (pcga1->aligndata == NULL && pcga2->aligndata == NULL)
		return 0;	
        switch(SortOn)
	  {
              case 0:
	      case 1:  /* Score */
	           if (pcga1->aligndata->vast_score  > pcga2->aligndata->vast_score)
		      return -1;
	           else if (pcga1->aligndata->vast_score < pcga2->aligndata->vast_score)
		      return 1;
	           else 
		      return 0;
	      case 2:  /* P value (inverse sorting) */
	           if (pcga1->aligndata->vast_mlogp  > pcga2->aligndata->vast_mlogp)
		      return -1;
	           else if (pcga1->aligndata->vast_mlogp < pcga2->aligndata->vast_mlogp)
		      return 1;
	           else 
		      return 0;
	      case 3:  /* RMSD */
	           if (pcga1->aligndata->rmsd  < pcga2->aligndata->rmsd)
		      return -1;
	           else if (pcga1->aligndata->rmsd > pcga2->aligndata->rmsd)
		      return 1;
	           else 
		      return 0;
	      case 4:  /* Aligned residues */
	           if (pcga1->aligndata->align_res  > pcga2->aligndata->align_res)
		      return -1;
	           else if (pcga1->aligndata->align_res < pcga2->aligndata->align_res)
		      return 1;
	           else 
		      return 0;
	      case 5: /* Identity */
	           if (pcga1->aligndata->other_score  > pcga2->aligndata->other_score)
		      return -1;
	           else if (pcga1->aligndata->other_score < pcga2->aligndata->other_score)
		      return 1;
	           else 
		      return 0;
	      default: 
	         return 0;
	  }
} 

void LIBCALL VastTableSort(BiostrucFeatureSetPtr pbsfs,  Int2 iSort)
{
    ValNodePtr pvnSort = NULL;
    ValNodePtr pvnList = NULL;
    BiostrucFeaturePtr pbsf = NULL;
    BiostrucFeaturePtr pbsfThis = NULL;
    
    if ((!pbsfs)) return;
    
    pbsf = pbsfs->features;
    while(pbsf)
      {  /* chop out features into a ValNode list */
          pvnSort = ValNodeAddPointer(&pvnList, 0,  (VoidPtr) pbsf);	  
          pbsfThis = pbsf;
	  pbsf = pbsf->next;
	  pbsfThis->next = NULL;  /* detach feature */
      }
    SortOn = iSort;

    VnpHeapSort(&pvnList,  CompareScores);
    
      /* reassemble in sorted order */
    pbsfs->features = (BiostrucFeaturePtr) pvnList->data.ptrvalue;
    pbsf = pbsfs->features;
    pvnSort = pvnList->next;
    while (pvnSort)
      {
          pbsf->next = (BiostrucFeaturePtr) pvnSort->data.ptrvalue;
	  pbsf = pbsf->next;
	  pvnSort = pvnSort->next;
      }
    ValNodeFree(pvnList);        
    return;
}
 



Boolean LIBCALL SetNodeFeatureData(PFB pfbThis, PSFD  psfdThis)
{
  Int4Ptr pI4vFeat = NULL;
  PointerPtr ppvOld = NULL;
  int iNum,  i;

  /* Broadcasts the feature into the node that maps to it.
   * This sets the indicator in the PFB that there is a feature
   * associated with it.  The variable pI4vFeatID in the pfb is
   * a vector of pairs of id's where the first id is the feature-set
   * ordinal (not its true id),  and the second id is the 
   * true feature-id.
   * 
   * If there is ->pData associated with the node,  that pointer
   * is put into a parallel vector of pointers
   * to the pairs of id's (feature-set,  feature), 
   * ppFeatData.  This should be NULL for every feature
   * that has no associated data structures.
   * 
   */
   
  if (!pfbThis) return FALSE;
  if (!psfdThis) return FALSE;
  
  iNum = (int) pfbThis->iNumFeats;
  if ((pfbThis->pI4vFeatID) &&(iNum > 0))  
     pI4vFeat = pfbThis->pI4vFeatID;
  pfbThis->pI4vFeatID = NULL;
  pfbThis->pI4vFeatID = (Int4Ptr) MemNew((size_t) (iNum+1)*2*sizeof(Int4));
  if (!pfbThis->pI4vFeatID)
    {
      pfbThis->pI4vFeatID = pI4vFeat;
      return FALSE;
    }
  if (pI4vFeat)
    {  /* copy other id's and destroy */
	for (i = 0; i<(iNum*2); i++)
	  {
	      pfbThis->pI4vFeatID[i] = pI4vFeat[i];
	  }
	MemFree(pI4vFeat);	  
    }  
  pfbThis->pI4vFeatID[iNum*2] = (Int4) psfdThis->pdnsfsSet->choice;  
  pfbThis->pI4vFeatID[(iNum*2)+1] = psfdThis->iId; 
 
 
  if (psfdThis->pData) 
   {
     if (pfbThis->ppvFeatData)
       ppvOld = pfbThis->ppvFeatData;
     pfbThis->ppvFeatData = NULL;
     pfbThis->ppvFeatData = (PointerPtr) MemNew((size_t) (iNum+1)*sizeof(Pointer));
     if (!pfbThis->ppvFeatData)
       {
	   pfbThis->ppvFeatData = ppvOld;
	   return FALSE;
       }
     if (ppvOld)
       {  /* copy other pointers, and free old vector */
          for (i = 0; i < (iNum); i++)
	    pfbThis->ppvFeatData[i] = ppvOld[i];
          MemFree(ppvOld);
       }   
      /* assign the data pointer */ 
     pfbThis->ppvFeatData[iNum] = psfdThis->pData;
   } 
 
  pfbThis->iNumFeats++;
  return TRUE;
}
	  
ValNodePtr LIBCALL MakeRegionNodeList(PDNMS pdnmsThis, RegionPntrsPtr prgpThis)
{
 
   
    /* 

    This traverses the ASN.1 RegionPntrs list and makes a list of PFB's  
    prgpThis->model_id;
    ValNodeFindNext(prgpThis->region ... );
    
    RegionCoordinates ...  (model coord set, - does the lookup )
    


    RegionBoundary... (sphere,  cone,  cylinder,  brick - does the bounding calc 
                       and returns entities within )
     */
     
   return NULL;  
}

void LIBCALL InstallAlignedSlave(PDNMS pdnmsMaster, ChemGraphAlignmentPtr pcgaAlign,
				 PSFD psfdThis, Boolean Chain, CharPtr SlaveChain) {
   Int4 iId = 0;
   BiostrucPtr pbsSlave = NULL, pbsTemp = NULL;
   PDNMS pdnmsSlave = NULL;
   PMSD  pmsdSlave = NULL;
   PFB   pfbThis = NULL;
   ValNodePtr pvnThis = NULL;
   ValNodePtr pvnList = NULL;
   ValNodePtr pvnLoc = NULL;
   BiostrucIdPtr pbsidThis = NULL;
   ChemGraphPntrsPtr pcgpThis = NULL;
   PSFS psfsThis = NULL;
   Pointer ptr = NULL;
   Boolean bTest;
   Int2 iTest;

#ifdef _DEBUG_9
printf("in InstallAlignedSlave \n");
#endif 	

   if (!pcgaAlign) return;
   if (!pdnmsMaster) return;
   
   SetSlaveModelstruc(NULL);
   SetSelectedModelstruc(pdnmsMaster);
   SetNeighborOn(); /* sets master to selected */
   /* sets slave to first on master's list if slave is null */

   if (pcgaAlign->dimension != 2) return;

#ifdef _DEBUG_9
printf("still in InstallAlignedSlave \n");
#endif 	 

  /* the second id is the slave */
  pbsidThis = ValNodeFindNext(pcgaAlign->biostruc_ids,NULL,BiostrucId_mmdb_id);
  if (pbsidThis)
    {
  	pbsidThis = ValNodeFindNext(pcgaAlign->biostruc_ids,pbsidThis,BiostrucId_mmdb_id);
        if (pbsidThis) iId = (Int4) pbsidThis->data.intvalue;  
    }   
   
   if (iId == 0) 
     {
#ifdef _DEBUG_9
printf("Biostruc Slave Fetch failed %d \n", (int) iId);
#endif 	
      return; 
    }

   pbsSlave = MMDBBiostrucGet(iId, ALLSIMPLEMDL, 3);
/*****************************************************************************/
/* This is code added by KA so that we only have to call one MMDBBiostrucGet */   
/*****************************************************************************/
   if (Chain == TRUE) {
     if (SlaveChain[0] != ' ') {
       pbsTemp = (BiostrucPtr)PruneBiostruc(pbsSlave, SlaveChain);
       pbsSlave = NULL;
       pbsSlave = pbsTemp;
      }
   }  
/*****************************************************************************/
   if (!pbsSlave) { 
          SetNeighborOff(); 

#ifdef _DEBUG_9
printf("Biostruc Slave Fetch failed %d \n", (int) iId);
#endif 	
          return; }
   else
    {
#ifdef _DEBUG_9
printf("Biostruc Slave Fetched! %d \n", (int) iId);
#endif 
    }
   pdnmsSlave = MakeAModelstruc(pbsSlave);
   /* hangs off of master and sets slave and sets selected=master */
   SetNeighborOff();
   /* resets selected to master only */
   if (!pdnmsSlave) {
#ifdef _DEBUG_9
printf("Biostruc Slave Convert failed %d \n", (int) iId);
#endif 
     return;
    }

   SetSlaveModelstruc(pdnmsSlave);  /* defaults *on* next time SetNeighborOn called */
  
   /* get the master data - > apply alignment CGP to master structure */
   /* using SetNodeFeatureData */

   pvnThis = NULL;
   pvnThis = pcgaAlign->alignment;
   if (pvnThis)
                   {
    /********TEMPORARY******/
                     pcgpThis = ( ChemGraphPntrsPtr ) pvnThis;
                     pvnList = MakeChemGraphNodeList(pdnmsMaster, pcgpThis);
                     if (!pvnList)  {
#ifdef _DEBUG_9
printf("Master MakeChemGraphNodeList failed \n");
#endif 
			return; }
                     pvnLoc = pvnList;
                     while (pvnLoc)
                       {
		           pfbThis = (PFB) pvnLoc->data.ptrvalue;
                           pfbThis->bReserved++;
	                   pvnLoc = pvnLoc->next;
                       }  /* while pvnLoc */
                      ValNodeFree(pvnList);
                      pvnList = NULL;

                     pcgpThis = ( ChemGraphPntrsPtr ) pvnThis->next;
                     pvnList = MakeChemGraphNodeList(pdnmsSlave, pcgpThis);
                     if (!pvnList)  {
#ifdef _DEBUG_9
printf("Master MakeChemGraphNodeList failed \n");
#endif 
			return; }
                     pvnLoc = pvnList;
                     while (pvnLoc)
                       {
		         pfbThis = (PFB) pvnLoc->data.ptrvalue;
                         /* register feature in each location */
                           bTest = SetNodeFeatureData( pfbThis,  psfdThis); 
                           psfdThis->bPFBtype = (psfdThis->bPFBtype | pfbThis->bMe); 
                      /********TEMPORARY******/
                           pfbThis->bReserved++;
		           if (bTest == FALSE)  {
#ifdef _DEBUG_9
printf("Master SetNodeFeatureData failed \n");
#endif 
			return; } 
                         pvnLoc = pvnLoc->next;
                       }  /* while pvnLoc */
                      ValNodeFree(pvnList);
                      pvnList = NULL;
                   }  /* if pvnThis */
  
	       
   TransformToDNTRN((PDNTRN PNTR) &ptr, pcgaAlign->transform ); 
   
   /* apply the transform to the slave */
   
   iTest = TraverseModels( pdnmsSlave,
                                TRAVERSE_ATOM, 
                                0, ptr,
                                (pNodeFunc)(DoApplyTransform));
   iTest = TraverseModels( pdnmsSlave, 
                                TRAVERSE_SOLID, 
                                0, ptr,
                                (pNodeFunc)(DoApplyTransform));
#ifdef _DEBUG_9
printf("Transformed Models \n");
#endif 
 				
   /* free the transform data */
   
   FreeDNTRN((PDNTRN) ptr);
  

   /* set dirty bit */ 
   pmsdSlave = (PMSD) pdnmsSlave->data.ptrvalue;
   pmsdSlave->bUpdate = (Byte) (pmsdSlave->bUpdate | (Byte) AM_CHANGED);
   /* enforce an MMDB-ID change */
   pmsdSlave->iHashChange = MakeHashChange(pdnmsSlave);
 
  /* Do nothing with the domain data... */
  /* that's all folks, now just use SetNeighborOn()... */
  /* selected = master after this is through */
  return;
}
            

PSFD  LIBCALL InstallStrucFeature(PDNMS pdnmsThis, PDNSFS pdnsfsThis, BiostrucFeaturePtr pbsfThis,
				  Boolean Chain, CharPtr SlaveChain)
{

/* This procedure adds a new Feature to the Feature-Set registry
 * specified.  If there is "global" data to be kept (e.g. color, 
 * render,  transform values,  camera values, label values
 */
  PSFF psffFunc = NULL;
  PSFS psfsThis = NULL;
  PSFD psfdTemp = NULL;
  Uint1 iColor = 0;
  ColorPropPtr pcpThis = NULL;
   
  
  if (!(pdnsfsThis || pbsfThis)) return NULL;
 
#ifdef _DEBUG_9
printf("in InstallStrucFeature \n");
#endif 	 

  psfsThis = (PSFS) pdnsfsThis->data.ptrvalue;
  psfdTemp = psfsThis->psfdFeats;
  psfsThis->psfdFeats = NewSFD(); /* insert new feature at head LIFO */
  if (!psfsThis->psfdFeats) 
    {
      psfsThis->psfdFeats = psfdTemp;
      return NULL;
    }
  psfsThis->psfdFeats->next = psfdTemp; /* attach rest of features */
  psfsThis->psfdFeats->iType = pbsfThis->type;
  psfsThis->psfdFeats->pdnsfsSet = pdnsfsThis;
  psfsThis->psfdFeats->iId = pbsfThis->id;
  if (pbsfThis->name) psfsThis->psfdFeats->pcName = StringSave(pbsfThis->name);
 

  psffFunc = DoesFeatureTypeHaveFuncs(pbsfThis->type);
  if (psffFunc)
    {  /* this uses the installed functions associated with the feature type */
       /* to copy from the User-object into a newly-allocated data object */
       /* which is owned by psfsThis->psfdFeats */

	(*(psffFunc->pFeatToCFn)) ( (Pointer PNTR) &(psfsThis->psfdFeats->pData),  
	                  (UserObjectPtr PNTR) &pbsfThis->Property_property->data.ptrvalue);
    }
  
  
/* this is to instantiate the property data */     
  switch(pbsfThis->type)
     {
        case Feature_type_color:
	  /* register color - gets back index into palette */
	  pcpThis = (ColorPropPtr) pbsfThis->Property_property->data.ptrvalue;
	  iColor = Call3DColorProc(pcpThis);
	  psfsThis->psfdFeats->iColor = iColor;	  
	  break;
	  
        case Feature_type_render:
          /* register render code */
	  psfsThis->psfdFeats->iRender = (Int4) pbsfThis->Property_property->data.intvalue;
	  break;

        case Feature_type_transform:
	  TransformToDNTRN((PDNTRN PNTR) &psfsThis->psfdFeats->pData, 
	                       (TransformPtr) pbsfThis->Property_property->data.ptrvalue);
	  break;
	  
        case Feature_type_camera:
        case Feature_type_script:
           /* detach & save the property data in ASN.1 format */
/*	  psfsThis->psfdFeats->pData = (Pointer) pbsfThis->Property_property; */
/*	  pbsfThis->Property_property = NULL;*/
          break;
	  
	case Feature_type_cn3dstate:
        case Feature_type_comment:
	case Feature_type_label:	
           /* globally applied user-object in use */
	    /* the ASN.1 has been converted to the C by registered function */   	       	 
	  break;
	  
        case Feature_type_other:  
	  break;  /* other not allowed to have data... */  
	  
        case Feature_type_multalign:
        case Feature_type_alignment:

#ifdef _DEBUG_9
printf("in  alignment case \n");
#endif 	             
            /* cast property to alignment ASN.1 type */
	    /* detach location & save it in ASN.1 format  */
	     psfsThis->psfdFeats->pData = (Pointer) pbsfThis->Location_location->data.ptrvalue;
	     pbsfThis->Location_location = NULL; 
 	     InstallAlignedSlave(pdnmsThis, 
                                 (ChemGraphAlignmentPtr) psfsThis->psfdFeats->pData, 
                                  psfsThis->psfdFeats, Chain, SlaveChain);
          break;
        case Feature_type_similarity:
        case Feature_type_region:
          break;   
        case Feature_type_indirect:
	    /* deal with mapping the chem-graph-ptrs and "prototypes" */ 
	  break;
	 
      /* these don't get put into flag bytes */
      /* but they all have subgraph locations */
     /*   case Feature_type_helix:
        case Feature_type_strand:
        case Feature_type_sheet:
        case Feature_type_turn:
        case Feature_type_site:              
        case Feature_type_footnote:
        case Feature_type_subgraph:
        case Feature_type_core:
        case Feature_type_supercore: */
        default: ;
	  /* these have no special indication other than Type */
     }  /* switch generic feature-type */
  
  return psfsThis->psfdFeats;
}

Int2 LIBCALL BiostrucAddFeature(BiostrucFeatureSetPtr pbsfsThis, PDNMS pdnmsThis,
				Boolean Chain, CharPtr SlaveChain)
{
  BiostrucFeaturePtr pbsfThis = NULL;
  PDNSFS pdnsfsThis = NULL;
  PSFS psfsThis = NULL;
  PSFD psfdThis = NULL;
  PMSD pmsdThis = NULL;
  PMGD pmgdThis = NULL;
  PFB pfbThis = NULL;
  ValNodePtr pvnThis = NULL;
  DValNodePtr pdvnThis = NULL;
  ChemGraphPntrsPtr pcgpThis = NULL;
  RegionPntrsPtr prgpThis = NULL;
  CharPtr pcThis = NULL;
  ValNodePtr pvnList = NULL;
  ValNodePtr pvnLoc = NULL;
  Byte bSet = 0;
  Boolean ncbiSS = FALSE;
  Boolean pdbSS = FALSE;
  Boolean pdbSite = FALSE;
  Boolean pdbFootnote  = FALSE;
  Boolean ncbiDomain = FALSE;
  Boolean ncbiVast = FALSE;
  Boolean bTest = FALSE;


#ifdef _DEBUG_9
printf("in BiostrucAddFeature \n");
#endif 	    
  if (!pbsfsThis) return 0;
  if (!pdnmsThis) return 0;
  
  pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
 
/* first chore is to make a new feature-set node off this structure */

  pdnsfsThis = NewDNSFS(&pmsdThis->pdnsfsHead, pmsdThis->iFeatures+1); 
  if (!pdnsfsThis) return 0;
  pmsdThis->iFeatures++;
  psfsThis = (PSFS) pdnsfsThis->data.ptrvalue;
  psfsThis->iId = pbsfsThis->id;
  
/*psfsThis->pvnDescr = (ValNodePtr) pbsfsThis->descr; */ /* link stub for descr */
/*pbsfsThis->descr = NULL; */ /* detach and save from free-ing descr  */
               /* yanli comment the above two line out, instead do the following */
  psfsThis->pvnDescr = (ValNodePtr)AsnIoMemCopy((ValNodePtr) pbsfsThis->descr, (AsnReadFunc)BiostrucFeatureSetDescrAsnRead, (AsnWriteFunc)BiostrucFeatureSetDescrAsnWrite); 

  pvnThis = NULL;
  pvnThis = ValNodeFindNext(psfsThis->pvnDescr, NULL,  BiostrucFeatureSetDescr_name); 
  if (pvnThis)
    {
     pcThis = (CharPtr) pvnThis->data.ptrvalue;
     psfsThis->pcName = StringSave(pcThis);

  /* these strings categorize known feature sets that come bundled
   with MMDB data */

     if (!(StringCmp(pcThis, STRING_NCBI_SS)))
       {
         ncbiSS = TRUE;
       }
     if (!(StringCmp(pcThis, STRING_PDB_SS)))
       {
         pdbSS = TRUE;
       }  
     if (!(StringCmp(pcThis, STRING_PDB_SITE)))
       {
         pdbSite = TRUE;
       }       
     if (!(StringCmp(pcThis, STRING_PDB_FOOTNOTE)))
       {
         pdbFootnote = TRUE;
       }  
     if (!(StringCmp(pcThis, STRING_NCBI_DOMAIN)))
       {
         ncbiDomain = TRUE;
       }
     if (!(StringCmp(pcThis, STRING_NCBI_VAST)))
       {
       /* lyg: this no longer works.  The id is simply the mmdb # hash of the 2 proteins */
         ncbiVast = TRUE;
       }
    }
  

  pbsfThis = pbsfsThis->features;
  while (pbsfThis)
    {  /* walk through each feature */

      if (ncbiSS || pdbSS)
        {  /* set bytes for secondary strucutre flags */
	switch (pbsfThis->type)
	  {
	    case Feature_type_helix:
 	      bSet = (Byte) SS_HELIX;
 	      break;
	    case Feature_type_strand:
 	      bSet =  (Byte) SS_STRAND;
 	      break;
	    case Feature_type_sheet:
 	      bSet = (Byte) (SS_STRAND | SS_SHEET);
 	      break;
	    case Feature_type_turn:
 	      bSet = (Byte) SS_TURN;
 	      break;
	    default:
	     return -1;
	  }      
       }  /* if bSet - Sec stru to set */

     /* Installs feature entry in feature registry */   
     /* any data is attached and owned by the psfdThis node */

     psfdThis = InstallStrucFeature(pdnmsThis, pdnsfsThis, pbsfThis, Chain, SlaveChain);

     if (!psfdThis) return 0;
     
     /* this registers the installed feature into the appropriate PFB node(s) */
     switch(psfdThis->iType)
           {
 
             case Feature_type_alignment:
	     case Feature_type_multalign:
	       /* these are already registered  */
                break;
	     case Feature_type_similarity:
             case Feature_type_region:
             case Feature_type_cn3dstate:
             case Feature_type_camera:
             case Feature_type_script:
	              /*  locations are assumed MSD */
                bTest = SetNodeFeatureData((PFB) pmsdThis,  psfdThis );
                psfdThis->bPFBtype = pmsdThis->bMe;  /* set to AM_MSD */

	       break;
 /*
             case Feature_type_helix:
             case Feature_type_strand:
             case Feature_type_sheet:
             case Feature_type_turn:
             case Feature_type_site:              
             case Feature_type_footnote:
             case Feature_type_subgraph:
             case Feature_type_color:
             case Feature_type_render:
             case Feature_type_comment:
             case Feature_type_label:
             case Feature_type_transform:
             case Feature_type_core:
             case Feature_type_supercore:
             case Feature_type_other:
   */
             default:
	          /* do chem-graph-pointers */
                 pvnThis = NULL;
                 pvnThis = ValNodeFindNext(pbsfThis->Location_location, NULL, Location_location_subgraph);
                 if (pvnThis)
                   {
                     pcgpThis = (ChemGraphPntrsPtr) pvnThis->data.ptrvalue;
                     pvnList = MakeChemGraphNodeList(pdnmsThis, pcgpThis);
                     if (!pvnList) return -1;
                     pvnLoc = pvnList;
                     while (pvnLoc)
                       {
                         pfbThis = (PFB) pvnLoc->data.ptrvalue;
		         
			 /* register feature in each location node */ 
                         bTest = SetNodeFeatureData( pfbThis,  psfdThis );
                         if (bTest == FALSE) return 0; 
                         psfdThis->bPFBtype = (psfdThis->bPFBtype | pfbThis->bMe);
			   /* special case for Sec Stru - set flag bits & strings */
			 if (ncbiSS || pdbSS || ncbiDomain)
			   {
			      pmgdThis = (PMGD) pfbThis;
			      if (pmgdThis->bMe != AM_MGD) return -1;
			      if (ncbiSS)
				    { 
					pmgdThis->pcNCBISS = psfdThis->pcName;
					pmgdThis->bNCBISecStru = pmgdThis->bNCBISecStru | bSet;
				    }
				if (pdbSS) 
				    {
					pmgdThis->pcPDBSS = psfdThis->pcName;
					pmgdThis->bPDBSecStru = pmgdThis->bPDBSecStru |  bSet;
				    } 
				if (ncbiDomain)
			            {
 				        pmgdThis->iDomain = pbsfThis->id;
			            }
		            }	  /* special cases */
                         pvnLoc = pvnLoc->next;
                       }  /* while pvnLoc */
                      ValNodeFree(pvnList);
                      pvnList = NULL;
                   }  /* if pvnThis */
		   
		   
		   /* do regions */
		   
		 pvnThis = NULL;
                 pvnThis = ValNodeFindNext(pbsfThis->Location_location, NULL, Location_location_region);
                 if (pvnThis)
                   {
                     prgpThis = (RegionPntrsPtr) pvnThis->data.ptrvalue;
                     pvnList = MakeRegionNodeList(pdnmsThis, prgpThis);
                     if (!pvnList) return -1;
                     pvnLoc = pvnList;
                     while (pvnLoc)
                       {
		         pfbThis = (PFB) pvnLoc->data.ptrvalue;
                         /* register feature in each location */
                         bTest = SetNodeFeatureData( pfbThis,  psfdThis);  
                         psfdThis->bPFBtype = (psfdThis->bPFBtype | pfbThis->bMe); 
		         if (bTest == FALSE) return 0;
                         pvnLoc = pvnLoc->next;
                       }  /* while pvnLoc */
                      ValNodeFree(pvnList);
                      pvnList = NULL;
                   }  /* if pvnThis */
  
            }  /* switch generic feature-type */
  
     pbsfThis = pbsfThis->next;
   } /* while pbsfThis : next feature in feature-set */
  
  /* now that they are instantiated - the caller can delete the ASN.1 forms */
  /* of the features - we have kept the descr and other non-volatile elements */
  /* like camera's and tranforms, and null-ed out their pointers */  
  
  return TRUE;
}
 

static Int4 FindGiSeq(ValNodePtr pvnSeqId)
{
    ValNodePtr pvn;
    pvn = ValNodeFindNext(pvnSeqId, NULL, SEQID_GI);
    if (pvn)
      return (Int4) pvn->data.intvalue;
    else
      return 0;
}




Byte LIBCALL SetBondOrder(Byte bThis,  Int2 iType)
{
       switch (iType)
	{
	 case Bond_order_partial_double:
		return (Byte)(bThis | BOND_PDOUBLE);
	 case Bond_order_aromatic:
		return (Byte)(bThis | BOND_PDOUBLE | BOND_DOUBLE);
	 case Bond_order_double:
		return (Byte)(bThis | BOND_DOUBLE);
	 case Bond_order_triple:
		return (Byte)(bThis | BOND_TRIPLE);
	 case Bond_order_other:
	 case Bond_order_unknown:
		return bThis;
	 case Bond_order_single:
	 default:
		bThis = (Byte)(bThis | BOND_SINGLE);
	}
  return bThis;	
}


/*
 * Biostruc2Modelstruc is the main routine to fetch and convert a 
 * Biostruc into Modelstruc form - which is a hybrid ASN.1 / hand
 * made data structure.
 * 
 */



 

PDNMS LIBCALL Biostruc2Modelstruc(PDNMS PNTR ppdnmsList, BiostrucPtr pbsBS, PRGD prgdDict, 
                         Byte  bExtent, Int2 iChoice,  
                         CharPtr pcFetch, Int2 iType)
/* pdnmsList is the master list of structures or NULL for first */
/* if pbsBS = NULL - use the pcFetch functions.. */
{
	BiostrucModelPtr pbsmThis = NULL;
	IntraResidueBondPtr pirbThis = NULL;
	InterResidueBondPtr pierbThis = NULL;
	ValNodePtr pvnThis = NULL;
	AtomPntrPtr papIndex = NULL;
	
	PDNMS pdnmsThis = NULL; 
 	PMSD pmsdThis = NULL; 
	PDNMM pdnmmThis = NULL; 
	PDNMM pdnmmPrev = NULL; 
	PMMD pmmdThis = NULL; 
	PDNMG pdnmgThis = NULL;
	PDNMG pdnmgPrev = NULL; 
	PMGD pmgdThis = NULL;  
	PVNMA pvnmaThis = NULL;
	PVNMA pvnmaPrev = NULL;
	PMAD pmadThis = NULL;
	PVNMB pvnmbThis = NULL;
	PVNMB pvnmbPrev = NULL;
	PMBD pmbdThis = NULL;
	Int2 iTest = TRUE;
	Int4 iGraphId = 0;
					 

	Int4 iMolCount = 0; 
	Int4 iResCount = 0;
	Int4 iAtomCount = 0;
	Int4 iBondCount = 0;
	Int4 iModelCount = 0;

 	/* version 4.0 additions */
	BiostrucIdPtr pbsidThis = NULL;
	BiostrucDescrPtr pbsdrThis = NULL;
	BiomolDescrPtr pbmdrThis = NULL;
        MoleculeGraphPtr pmgThis = NULL;
	ResiduePtr prThis = NULL; 
	ValNodePtr pvnrgThis = NULL; /* a ValNodePtr */
	ResidueGraphPtr prgDict = NULL;  /* a ValNodePtr */
	ValNodePtr pvnStrings = NULL;
	AtomPtr  pAtom;
	BiostrucFeatureSetPtr pbsfsThis = NULL;
	BiostrucResidueGraphSetPntrPtr ppdDict = NULL;

	Int4 iMolGraphType = 0; 

	
	 /* ErrSetFatalLevel - allow malloc failure to return */
#ifdef _DEBUG_0
	printf("Biostruc2Modelstruc\n");
#endif
	if (!pbsBS) pbsBS = FetchBS(pcFetch,iType, ALLMDL,3000,bExtent);

        
	/* traverse the chemical graph to make C memory
	 * strucures */

	if (pbsBS)
	  {

	   pdnmsThis = NewDNMS(ppdnmsList, iChoice); 
           if (!pdnmsThis) goto allocerror;
	   pmsdThis = (PMSD) pdnmsThis->data.ptrvalue; /*set the pointer to the data */

     pmsdThis->pdnmsSlaves = NULL;  
     pmsdThis->bAligned = 0;  /* nothing aligned to it */
     pmsdThis->pbAligned = NULL;
     pmsdThis->bMaster = TRUE; /* default to being a master structure */
     pmsdThis->iMimeType = 0;

		   /* non-MMDB dependent info */ 
	   pmsdThis->pcSName = StringSave(pcFetch);
	   pmsdThis->iType = iType;  /* copy iType also used in fetch for future ref */ 

		  /* MMDB dependent stuff */
   

	   pbsidThis = ValNodeFindNext(pbsBS->id,NULL,BiostrucId_mmdb_id);
	   if (pbsidThis)
 	     {
	   	pmsdThis->iMMDBid = (Int4) pbsidThis->data.intvalue;  /* Get MMDB id no (only the first one) */ 
	     }
	   else 
	     {
		;  /* assign it a hashed ID number based on iChoice?? */
	     }
	   pbsdrThis = ValNodeFindNext(pbsBS->descr,NULL,BiostrucDescr_name);
	     /* just want the first one */
	   pmsdThis->pcPDBName = StringSave(PDBNAME_DEFAULT);
	   if (pbsdrThis)
	     {
		StringNCpy(pmsdThis->pcPDBName,(CharPtr) pbsdrThis->data.ptrvalue, sizeof(pmsdThis->pcPDBName));
	     }
#ifdef _DEBUG_0
printf("[%d]id [%s] pdb\n",pmsdThis->iMMDBid,pmsdThis->pcPDBName); 
printf("<%s>\n",pmsdThis->pcSName);
#endif		     
	    /* ATTACH THE ENTIRE Biostruc */ 
	   pmsdThis->pbsBS = pbsBS;  
	   
	   /* the chemical_graph and model_structure portions are erased at the end  */
	   
	   pmsdThis->pGraphDescr = pbsBS->chemical_graph->descr; /* save the chem-graph descr (ValNode) */
	   pmsdThis->bVisible = 1;  /* can be seen */
	   pbmdrThis = ValNodeFindNext(pbsBS->chemical_graph->descr, NULL, BiomolDescr_name);
	   if (pbmdrThis)
	     {
		 pmsdThis->pcChemName = StringSave((CharPtr)pbmdrThis->data.ptrvalue); 
	     }
	   else 
	     {
		 pmsdThis->pcChemName = StringSave(STRING_UNK_PDB);
	     }
	   
	   pbmdrThis = ValNodeFindNext(pbsBS->chemical_graph->descr, NULL, BiomolDescr_pdb_class);
	   if (pbmdrThis)
	     {
	      pmsdThis->pcPdbClass = StringSave((CharPtr)pbmdrThis->data.ptrvalue);
	     } 
	   else 
	     {
		pmsdThis->pcPdbClass = StringSave(STRING_UNK_PDB); 
	     }
	     
	   pbmdrThis = ValNodeFindNext(pbsBS->chemical_graph->descr, NULL, BiomolDescr_pdb_source);
  	   if (pbmdrThis)
	     {
	      pmsdThis->pcPdbSource = StringSave((CharPtr)pbmdrThis->data.ptrvalue);
	     } 
	   else 
	     {
		pmsdThis->pcPdbSource = StringSave(STRING_UNK_PDB); 
	     }
	     
	   pbmdrThis = ValNodeFindNext(pbsBS->chemical_graph->descr, NULL, BiomolDescr_assembly_type);
	   if (pbmdrThis)
	     {
	       pmsdThis->iPDBType = (Int2) pbmdrThis->data.intvalue;
	     }
	   else 
	     {
	       pmsdThis->iPDBType = Assembly_Type_Other;
	     }  
	   
	  pbsBS->chemical_graph->descr = NULL;  /* stops the free-er from killing it prematurely */
	  
	   
#ifdef _DEBUG_0	     
	    printf("{%s} pcChemName \n", pmsdThis->pcChemName); 
	    printf("{%s} pcPdbClass \n", pmsdThis->pcPdbClass); 
	    printf("{%s} pcPdbSource\n", pmsdThis->pcPdbSource);
#endif 	   

/* flesh-out the chemical graph */
	    pmgThis = pbsBS->chemical_graph->molecule_graphs;
	    ProgMon("Instantiating Chemical Graph");
	    while (pmgThis) /* walk up the molecule_graphs */
	     {
		iMolCount++; 
		iResCount = 0; 
		pdnmmPrev = pdnmmThis;
		pdnmmThis = NewDNMM(NULL, (Int2) pmgThis->id); 
		if (!pdnmmThis) goto allocerror;
		pmmdThis = (PMMD) pdnmmThis->data.ptrvalue; /*set the pointer to the data */
		pmmdThis->pfbParent = (PFB) pmsdThis;
    
    /* turn off the selection.  lyg */
    pmmdThis->pdnmgStartSelect = NULL;
    pmmdThis->pdnmgEndSelect = NULL;

		/* get the sequence for each one */ 
		pmmdThis->iChainId = pmgThis->id; 
#ifdef _DEBUG_0
printf("\n<%d> ChainId ", pmmdThis->iChainId); 
#endif
		/* this gets chain names, like A,B etc... */
		
		pbmdrThis = ValNodeFindNext(pmgThis->descr, NULL, BiomolDescr_name);
		if (pbmdrThis)
		  {
		   pmmdThis->pcMolName = StringSave((CharPtr)pbmdrThis->data.ptrvalue); 
		  }
		else 
		  {
		    pmmdThis->pcMolName = StringSave(STRING_UNK_MOL);
		  }
		
#ifdef _DEBUG_0	
printf("'%s' name \n", pmmdThis->pcMolName); 
#endif
		
	
		pbmdrThis = ValNodeFindNext(pmgThis->descr, NULL, BiomolDescr_molecule_type);
		if (pbmdrThis)
		  {
		    iMolGraphType = (Int4) pbmdrThis->data.intvalue;
		  } 
		else 
		  {  
		    iMolGraphType = Molecule_type_other;
		  }
		  
		pmmdThis->pMolDescr = pmgThis->descr; /* save molecule descr */
	    	pmgThis->descr = NULL; /* stops from premature free-ing when chem-graph turfed */
		  
		switch (iMolGraphType)
		  {
		   case Molecule_type_dna:
#ifdef _DEBUG_2
printf("dna ");
#endif
		      pmmdThis->bWhat = (Byte) AM_DNA;
		      pmsdThis->bWhat = (Byte) (pmsdThis->bWhat | (Byte) AM_DNA);
		      pmmdThis->iGi = FindGiSeq(pmgThis->seq_id);
		      break;
		   case Molecule_type_rna:
#ifdef _DEBUG_2
printf("rna ");
#endif 
		      pmmdThis->bWhat = (Byte) AM_RNA; 
		      pmsdThis->bWhat = (Byte) (pmsdThis->bWhat | (Byte) AM_RNA);
		      pmmdThis->iGi = FindGiSeq(pmgThis->seq_id);
		    
	 	      break;
		   case Molecule_type_protein:
#ifdef _DEBUG_2
printf("protein ");
#endif

		      pmmdThis->bWhat = (Byte) AM_PROT;
		      pmsdThis->bWhat = (Byte) (pmsdThis->bWhat | (Byte) AM_PROT);
		      pmmdThis->iGi = FindGiSeq(pmgThis->seq_id);
		    
		      break; 
		   case Molecule_type_other_biopolymer:
#ifdef _DEBUG_2
printf("other biopolymer ");
#endif
		      pmmdThis->bWhat = (Byte) AM_POLY; 
		      pmsdThis->bWhat = (Byte) (pmsdThis->bWhat | (Byte)AM_POLY);
		      break; 
 		   case Molecule_type_solvent:
#ifdef _DEBUG_2
printf("solvent ");
#endif		   
		      pmmdThis->bWhat = (Byte) AM_SOL;
		      pmsdThis->bWhat = (pmsdThis->bWhat | (Byte) AM_SOL);
		      break; 
		   case Molecule_type_other_nonpolymer:
#ifdef _DEBUG_2
printf("other ");
#endif			 
		      pmmdThis->bWhat = (Byte) AM_HET;
		      pmsdThis->bWhat = (pmsdThis->bWhat | (Byte) AM_HET);
		      break; 
		   case Molecule_type_other:
		   default:
#ifdef _DEBUG_2
printf("unknown ");
#endif			   
 		      pmmdThis->bWhat = (Byte) AM_UNK;
		      pmsdThis->bWhat = (pmsdThis->bWhat | (Byte) AM_UNK);
		  } /*switch*/
 
 		  pmmdThis->pSeqId = pmgThis->seq_id;
		  pmgThis->seq_id = NULL;  /* copies & protects the ASN.1 seq_id */


		    
		  prThis = pmgThis->residue_sequence; 

		  while (prThis)
	  	    {  /* flesh - out the molecule graphs residue-by-residue */
			 ProgMon("Instantiating Chemical Graph");
			 iResCount++; 
			 iAtomCount = 0;
			 iBondCount = 0;
			 pdnmgPrev = pdnmgThis; 
			 pdnmgThis  = NewDNMG(NULL,(Int2) prThis->id); 
			 if (!pdnmgThis) goto allocerror;
			 pmgdThis = (PMGD)  pdnmgThis->data.ptrvalue; /*set the pointer to the data */
			 pmgdThis->pfbParent = (PFB) pmmdThis;
			 pmgdThis->pcGraphNum = StringSave(prThis->name);
#ifdef _DEBUG_1
printf(":%s: PDB No ", pmgdThis->pcGraphNum);
#endif				 


			 pvnrgThis = prThis->residue_graph;
		         /* USE ENTREZ-FETCH TO GET REF-ED BIOSTRUCS */
			 switch (pvnrgThis->choice)
			  {
			     case ResidueGraphPntr_local:
			        prgDict = pbsBS->chemical_graph->residue_graphs;
				iGraphId = (Int4) pvnrgThis->data.intvalue;
				pmsdThis->pDictLocal = prgDict;
				pmgdThis->bWhat = (Byte) (pmgdThis->bWhat | (Byte) DICT_LOCAL);
				break;
			     case ResidueGraphPntr_standard:
			       /* get the id of the graph to lookup */
			        ppdDict = (BiostrucResidueGraphSetPntrPtr) pvnrgThis->data.ptrvalue;
				iGraphId = (Int4) ppdDict->residue_graph_id;
			 	pmgdThis->bWhat = (Byte) (pmgdThis->bWhat | (Byte) DICT_GLOBAL);
			       /* ****use the dictionary id to get the right global dictionary */
			 	prgDict = prgdDict->residue_graphs;  /* the one passed as a parameter */
				break;
			     case ResidueGraphPntr_biostruc:
			     default:
			        prgDict = NULL;
				pmgdThis->bWhat = (Byte) (pmgdThis->bWhat | (Byte) DICT_REF);

	 	 	  }
			 while (prgDict)
			  {
			  
			    if (prgDict->id == iGraphId)
			     {
			       /* found the right dictionary entry */
			        pbmdrThis = ValNodeFindNext(prgDict->descr, NULL, BiomolDescr_name);
			        if (pbmdrThis)
				  {
				    pmgdThis->pcGraphName = StringSave((CharPtr)pbmdrThis->data.ptrvalue); 
				  }
				else 
				  {
				     pmgdThis->pcGraphName = StringSave(STRING_UNK_RES);
				  }
				pbmdrThis = ValNodeFindNext(prgDict->descr, NULL, BiomolDescr_pdb_comment);
				if (pbmdrThis)
				  {
   				    pmgdThis->pcPDBComment = StringSave((CharPtr) pbmdrThis->data.ptrvalue);
				  }
				pvnStrings = prgDict->iupac_code;
				if (pvnStrings)  /* only interpret the first one!!! */
				  {
				    pmgdThis->pcIUPAC = StringSave((CharPtr)pvnStrings->data.ptrvalue);
				  }
				  
#ifdef _DEBUG_1
printf("@%s@%s@",pmgdThis->pcIUPAC, pmgdThis->pcGraphName);
#endif				

				switch(prgDict->residue_type)
				   {
					case  Residue_deoxyribonucleotide:
					    pmgdThis->bWhat = (Byte) (pmgdThis->bWhat | (Byte) RES_DNA);
					    break;
					case Residue_ribonucleotide:
					    pmgdThis->bWhat = (Byte) (pmgdThis->bWhat | (Byte) RES_RNA);
					    break;
					case Residue_amino_acid:
					    pmgdThis->bWhat = (Byte) (pmgdThis->bWhat | (Byte) RES_AA);
					    break;
					case Residue_other:
					default:
					    ;
				   }
	 		       pmgdThis->iIDict = iGraphId;
			       pAtom = prgDict->atoms;
			       while (pAtom)
			        {
				    iAtomCount++;
				    pvnmaPrev = pvnmaThis;
 				    if (pAtom->element < 254) 
				       pvnmaThis = NewVNMA(NULL, (Int2) pAtom->element);
				    else 
				      {
				       if (pAtom->element == 254)
				          pvnmaThis = NewVNMA(NULL, 0);
				       else /* bigger usually is 255 */
				 	  pvnmaThis = NewVNMA(NULL, (Int2) INT1_MAX);
				      }
				    if (!pvnmaThis) goto allocerror;
				    pmadThis = (PMAD) pvnmaThis->data.ptrvalue;
				    pmadThis->pfbParent = (PFB) pmgdThis;
				    pmadThis->iIndex = (Int2) pAtom->id;
	/**/			    pmadThis->pcAName = pAtom->name;
				    /* note the name is still in the dictionary !!*/
				    /* no link back to dictionary source and 
				     * no link back to the iupac_code valnode list
				     * as this is redundant with the element lookup table
				     * and more compact to save an Int1 element number
				     * for each atom
				     */
#ifdef _DEBUG_1
printf(" ^%s^ ", pmadThis->pcAName);
#endif			         
				   
				    pAtom = pAtom->next;
				    if (iAtomCount == 1) pmgdThis->pvnmaAHead = pvnmaThis; /* first case */
				    else ValNodeListCat(pvnmaPrev, pvnmaThis);	
					/* faster linking with pvnmaPrev - dosen't have to walk the chain... */
			        }
			     
			     
			     
			        /* do intra-residue bonds here */	
			        pirbThis = prgDict->bonds;
				while (pirbThis)
				 {
				     iBondCount++;
				     pvnmbPrev = pvnmbThis;
				     pvnmbThis = NewVNMB(NULL, (Int2) pirbThis->bond_order);
				    
				     if (!pvnmbThis) goto allocerror;
				     pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
				     pmbdThis->pfbParent = (PFB) pmgdThis;
				     pmbdThis->bWhat = SetBondOrder(pmbdThis->bWhat,
				                                (Int2)pirbThis->bond_order);
				     /* register the bond in both the MBD and MAD */
				     pvnmaThis = pmgdThis->pvnmaAHead;
				     
				     while (pvnmaThis)
				       {
 					   pmadThis = (PMAD) pvnmaThis->data.ptrvalue;
				           if (pmadThis->iIndex == (Int2) pirbThis->atom_id_1)
					     {
						pmbdThis->pmadFrom = pmadThis;
						pvnThis = ValNodeAddPointer(&pmadThis->pvnBonds, 
						(Int2) pirbThis->bond_order, (Nlm_VoidPtr) pmbdThis);
						if (!pvnThis) goto allocerror;
						 
					     }
					   if (pmadThis->iIndex == (Int2) pirbThis->atom_id_2)
					     {
					       pmbdThis->pmadTo = pmadThis;
					       pvnThis = ValNodeAddPointer(&pmadThis->pvnBonds, 
						(Int2) pirbThis->bond_order, (Nlm_VoidPtr) pmbdThis);
					       if (!pvnThis) goto allocerror;
					     }    
					   pvnmaThis = pvnmaThis->next;
					   
				       }  /* while (pvnmaThis) */
				   /* check that pmadFrom and pmadTo are filled */
#ifdef _DEBUG_1
printf("\nbond from %s to %s ", pmbdThis->pmadFrom->pcAName, 
				    pmbdThis->pmadTo->pcAName);
#endif					   
				    pirbThis = pirbThis->next;
				    if (iBondCount == 1) pmgdThis->pvnmbBHead = pvnmbThis; /* first case */
				    else ValNodeListCat(pvnmbPrev, pvnmbThis);	
					/* faster linking with pvnmbPrev - dosen't have to walk the chain... */
				 }  /* while (pirbThis) */
			       break; /*  out of while (prgDict) */
			     }
			    prgDict = prgDict->next; 
			  } /* while (prgDict) */
                         if (!pmgdThis->iIDict) goto erret;  /* dictionary failed to resolve an ID */
	 		 pmgdThis->iAtomCount = iAtomCount;
			 pmgdThis->iBondCount = iBondCount;
			 if (iResCount == 1) pmmdThis->pdnmgHead = pdnmgThis;  /* link first case */ 
			 else DValNodeListCat(pdnmgPrev, pdnmgThis);  /* link it on */
			 prThis = prThis->next; 
	  	     }  /* while (prThis) */
		 /* put in error messages when not got sequence
		  */  
		pmmdThis->iResCount = iResCount;
		if (iMolCount == 1) pmsdThis->pdnmmHead = pdnmmThis;
		/* link first case */ 
		else DValNodeListCat(pdnmmPrev,pdnmmThis);  /* link it on */
		/* the molecule must be linked onto the structure 
		 * or the AtomFromMMDBIndex won't find it...
		 */
		iBondCount = 0;
		pvnmbPrev = NULL;
		pierbThis = pmgThis->inter_residue_bonds;
  		while (pierbThis)
		     { /*  ...  fill in the inter_residue_bonds list... */
		         iBondCount++;
			 pvnmbPrev = pvnmbThis;
			 pvnmbThis = NewVNMB(NULL, (Int2) pierbThis->bond_order);
			 
			 if (!pvnmbThis) goto allocerror;
			 pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
			 pmbdThis->pfbParent = (PFB) pmmdThis;
			 pmbdThis->bWhat = SetBondOrder(pmbdThis->bWhat,
			                           (Int2)pierbThis->bond_order);
			  /* register the bond in both the MBD and MAD */
				   
		         papIndex = pierbThis->atom_id_1;
			 pmadThis = AtomFromMMDBIndex(pdnmsThis, iChoice, papIndex->molecule_id, 
			              papIndex->residue_id,  papIndex->atom_id);
		         if (pmadThis) pmbdThis->pmadFrom = pmadThis;
			 else goto erret;
			 pvnThis = ValNodeAddPointer(&pmadThis->pvnBonds, 
					(Int2) pierbThis->bond_order, (Nlm_VoidPtr) pmbdThis);
			 if (!pvnThis) goto allocerror;
			 
			 papIndex = pierbThis->atom_id_2;
			 pmadThis = AtomFromMMDBIndex(pdnmsThis, iChoice, papIndex->molecule_id, 
			              papIndex->residue_id,  papIndex->atom_id);
		         if (pmadThis) pmbdThis->pmadTo = pmadThis;
			 else goto erret;
			 pvnThis = ValNodeAddPointer(&pmadThis->pvnBonds, 
					(Int2) pierbThis->bond_order, (Nlm_VoidPtr) pmbdThis);
			 if (!pvnThis) goto allocerror;
		
			 /* check that pmadFrom and pmadTo are filled */
#ifdef _DEBUG_1
printf("\nINTER-RES bond from %s to %s ", pmbdThis->pmadFrom->pcAName, 
				    pmbdThis->pmadTo->pcAName);
#endif			
			 if (iBondCount == 1) pmmdThis->pvnmbIRBHead = pvnmbThis; /* first case */
			 else ValNodeListCat(pvnmbPrev, pvnmbThis);	
		         pierbThis = pierbThis->next;
		       } /* while (pierbThis) */
		pmgThis = pmgThis->next; 
		pmmdThis->iIRBCount = iBondCount;       
	     } /*while (pmgThis) */
	   pierbThis = pbsBS->chemical_graph->inter_molecule_bonds;
  	   iBondCount = 0;
	   pvnmbPrev = NULL;
	   while (pierbThis)
   	     { /*  ...  fill in the inter_molecule_bonds list... */
	         iBondCount++;
		 pvnmbPrev = pvnmbThis;
		 pvnmbThis = NewVNMB(NULL, (Int2) pierbThis->bond_order);
		  
		 if (!pvnmbThis) goto allocerror;
		 pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
		 pmbdThis->pfbParent = (PFB) pmsdThis;
		 pmbdThis->bWhat = SetBondOrder(pmbdThis->bWhat,
				                 (Int2)pierbThis->bond_order);
		  /* register the bond in both the MBD and MAD */
			   
	         papIndex = pierbThis->atom_id_1;
		 pmadThis = AtomFromMMDBIndex(pdnmsThis, iChoice, papIndex->molecule_id, 
		              papIndex->residue_id,  papIndex->atom_id);
	         if (pmadThis) pmbdThis->pmadFrom = pmadThis;
		 else goto erret;
		 pvnThis = ValNodeAddPointer(&pmadThis->pvnBonds, 
				(Int2) pierbThis->bond_order, (Nlm_VoidPtr) pmbdThis);
		 if (!pvnThis) goto allocerror;
	 	 
		 papIndex = pierbThis->atom_id_2;
		 pmadThis = AtomFromMMDBIndex(pdnmsThis, iChoice, papIndex->molecule_id, 
		              papIndex->residue_id,  papIndex->atom_id);
	         if (pmadThis) pmbdThis->pmadTo = pmadThis;
		 else goto erret;
		 pvnThis = ValNodeAddPointer(&pmadThis->pvnBonds, 
				(Int2) pierbThis->bond_order, (Nlm_VoidPtr) pmbdThis);
		 if (!pvnThis) goto allocerror;
		
	 	 /* check that pmadFrom and pmadTo are filled */
#ifdef _DEBUG_1
printf("\nINTER-MOL bond from %s to %s ", pmbdThis->pmadFrom->pcAName, 
			    pmbdThis->pmadTo->pcAName);
#endif		 	
	 	        
		 if (iBondCount == 1) pmsdThis->pvnmbIMBHead = pvnmbThis; /* first case */
		 else ValNodeListCat(pvnmbPrev, pvnmbThis);	
	         pierbThis = pierbThis->next;
	     } /* while (pierbThis) */	
			
  
	   pmsdThis->iMolCount = iMolCount; 
	   pmsdThis->iIMBCount = iBondCount;
	      
	   /* REMOVE ALL BUT THE {id, descr, history} PART OF pbsBS */
	   /* TURF THE CHEMICAL GRAPH (NOT A SEQUENCE) */
	   /* keep Local Dict from destruction when I turf the chem graph */
	   if (pbsBS->chemical_graph->residue_graphs) 
	       pbsBS->chemical_graph->residue_graphs = NULL; 
	   BiostrucGraphFree(pbsBS->chemical_graph);
	   pbsBS->chemical_graph = NULL;
	   
	   /* Set up the Internal Feature Set */
	   pbsfsThis = pbsBS->features;
	   while (pbsfsThis)
    	     {
	        iTest = BiostrucAddFeature(pbsfsThis, pdnmsThis, FALSE, NULL);
          if (iTest < 0) goto erret;
	        if (!iTest) goto allocerror;
		pbsfsThis=pbsfsThis->next;
	     }
	              /* blow away the internal Feature Set ASN.1 */
/*         BiostrucFeatureSetFree(pbsfsThis);
           pbsBS->features = NULL;      */
           /* stubs to reconstruc Feature-set descr fields remain */
           /* as well as feature-contents */

	   /* CONVERT all Models one at a time */
	    pbsmThis = pbsBS->model; 
	    while (pbsmThis) 
	     {
	       iModelCount++;
 	       iTest = BiostrucModel2ModelStruc(pbsmThis,pdnmsThis,iChoice, iModelCount, bExtent);
	       if (iTest < 0) goto erret;
               if (iTest == 0) goto allocerror;
	       pmsdThis->iModels++;
 	       pbsmThis = pbsmThis->next;
	     } 
	    /* The Biostruc owns the models - they are always kept and are always freed at pbsBS level */ 
	    /* if not keeping ASN.1 the coordinates are all stripped out in the next routine */
	    /* but the model stubs still remain */
	  } /* if pbsBS */

  
    return pdnmsThis; 
    
    allocerror:
             ErrClear(); 
	     ErrPostEx(SEV_ERROR,0,0, "Not enough memory to handle Biostruc\nFreeing Biostruc");
	     ErrShow();
	     FreeListDNMS(pdnmsThis);
	     MsgAlert(KEY_NONE,SEV_ERROR, "Biostruc to Modelstruc Failed", 
		  "\nBiostruc was removed from memory.\nTry again with smaller or fewer Models.");
	     return NULL;
		
    erret:
	     ErrClear(); 
	     ErrPostEx(SEV_ERROR,0,0, "Biostruc ASN.1 Internal Indexing Failure\nFreeing Biostruc");
	     ErrShow();
	     FreeListDNMS(pdnmsThis);   
	     return NULL;
    
}


/* TraverseAll
 *
 * Arguments:
 *     pdnModel = a DValNode pointer USED for MSD, MMD, MGD.
 *                You cannot begin traversal at terminal nodes 
 *                MAD,MBD,MOD,MDD.
 *     iIndex	= any index to be passed to pfnCallme
 *                
 *     ptr      = any pointer to be passed to pfnCallme
 *     pfnCallMe = The callback to be performed at every node.
 *
 */


Int2 LIBCALL TraverseAll( DValNodePtr pdnModel, Int4 iModel, Int4 iIndex, 
			  Pointer ptr, pNodeFunc pfnCallMe)
/*	 Walks the structure children from the current pointer.  Performs
	 (*pfnCallMe)((PFB) pxxxxThis,  Int4 iMode, Int4 iIndex, Pointer ptr) at each data node 
	 (as Flags Block). callbacks are void return types.
 
*/
{
    PFB pfbThis;
    PDNMS pdnmsThis = NULL; 
    PMSD pmsdThis = NULL; 
    PDNMM pdnmmThis = NULL; 
    PMMD pmmdThis = NULL; 
    PDNMG pdnmgThis = NULL;
    PMGD pmgdThis = NULL;  
    PVNMA pvnmaThis = NULL;
    PMAD pmadThis = NULL;
    PVNMB pvnmbThis = NULL;
    PMBD pmbdThis = NULL;
    PVNMO pvnmoThis = NULL;
    PMOD pmodThis = NULL;
    PVNMD pvnmdThis = NULL;
    PMDD pmddThis = NULL;
    if (pdnModel)
      pfbThis = (PFB) pdnModel->data.ptrvalue;  /* get the data block */
    else return FALSE;
    switch ((int)pfbThis->bMe)  /* set the proper type */
      {
	  case AM_MSD: 
	      pdnmsThis = (PDNMS) pdnModel;
	    break;
	  case AM_MMD:
	      pdnmmThis = (PDNMM) pdnModel;
	    break;
	  case AM_MGD:
	      pdnmgThis = (PDNMM) pdnModel;
	    break;
          default:
            return FALSE;
      }
   
    if (pdnmgThis)
       {
        while(pdnmgThis)
		  {   /*graph*/
		      pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
		      (*pfnCallMe)((PFB) pmgdThis, iModel, iIndex, ptr); /* do the callback */
		      pvnmaThis = pmgdThis->pvnmaAHead;
		      while (pvnmaThis)
		        {   /*atom*/
			    pmadThis = (PMAD) pvnmaThis->data.ptrvalue;
			    (*pfnCallMe)((PFB) pmadThis, iModel, iIndex, ptr); /* do the callback */
			    
			    pvnmaThis = pvnmaThis->next;
			}   /*atom*/
		      pvnmbThis = pmgdThis->pvnmbBHead;
		      while (pvnmbThis)
		        {   /*bond*/
			   pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
			   (*pfnCallMe)((PFB) pmbdThis,  iModel, iIndex, ptr); /* do the callback */
			   
			   
			   pvnmbThis = pvnmbThis->next;
			} /*bond*/	
		      
		      pdnmgThis = pdnmgThis->next;
		  }/*graph*/
        return TRUE;
       } /* if pdnmgThis */
    if (pdnmmThis) 
       {
          while(pdnmmThis)
	    {   /*molecule*/
		pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
		(*pfnCallMe)((PFB) pmmdThis, iModel, iIndex, ptr); /* do the callback */
	        pdnmgThis =  pmmdThis->pdnmgHead;
		while(pdnmgThis)
		  {   /*graph*/
		      pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
		      (*pfnCallMe)((PFB) pmgdThis, iModel, iIndex, ptr); /* do the callback */
		      pvnmaThis = pmgdThis->pvnmaAHead;
		      while (pvnmaThis)
		        {   /*atom*/
			    pmadThis = (PMAD) pvnmaThis->data.ptrvalue;
			    (*pfnCallMe)((PFB) pmadThis, iModel, iIndex, ptr); /* do the callback */
			    
			    pvnmaThis = pvnmaThis->next;
			}   /*atom*/
		      pvnmbThis = pmgdThis->pvnmbBHead;
		      while (pvnmbThis)
		        {   /*bond*/
			   pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
			   (*pfnCallMe)((PFB) pmbdThis, iModel, iIndex, ptr); /* do the callback */
			   
			   
			   pvnmbThis = pvnmbThis->next;
			} /*bond*/	
		      
		      pdnmgThis = pdnmgThis->next;
		  }/*graph*/
		/* traverse inter-residue bonds */
		pvnmbThis = pmmdThis->pvnmbIRBHead;
		while (pvnmbThis)
		  { /* inter-residue bonds */
		    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
		    (*pfnCallMe)((PFB) pmbdThis, iModel, iIndex, ptr);  /* do the callback */
		      
		    pvnmbThis = pvnmbThis->next;
		  }
		pdnmmThis = pdnmmThis->next;
	    }   /*molecule*/
        return TRUE;
       } /* if pdnmmThis */
    if (pdnmsThis)  
     {
         pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
	  (*pfnCallMe)((PFB) pmsdThis, iModel, iIndex, ptr);  /* do the callback */
          pdnmmThis = pmsdThis->pdnmmHead;
	  while(pdnmmThis)
	    {   /*molecule*/
		pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
		(*pfnCallMe)((PFB) pmmdThis, iModel, iIndex, ptr); /* do the callback */
	        pdnmgThis =  pmmdThis->pdnmgHead;
		while(pdnmgThis)
		  {   /*graph*/
		      pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
		      (*pfnCallMe)((PFB) pmgdThis, iModel, iIndex, ptr); /* do the callback */
		      pvnmaThis = pmgdThis->pvnmaAHead;
		      while (pvnmaThis)
		        {   /*atom*/
			    pmadThis = (PMAD) pvnmaThis->data.ptrvalue;
			    (*pfnCallMe)((PFB) pmadThis, iModel, iIndex, ptr); /* do the callback */
			    
			    pvnmaThis = pvnmaThis->next;
			}   /*atom*/
		      pvnmbThis = pmgdThis->pvnmbBHead;
		      while (pvnmbThis)
		        {   /*bond*/
			   pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
			   (*pfnCallMe)((PFB) pmbdThis, iModel, iIndex, ptr); /* do the callback */
			   
			   
			   pvnmbThis = pvnmbThis->next;
			} /*bond*/	
		      
		      pdnmgThis = pdnmgThis->next;
		  }/*graph*/
		/* traverse inter-residue bonds */
		pvnmbThis = pmmdThis->pvnmbIRBHead;
		while (pvnmbThis)
		  { /* inter-residue bonds */
		    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
		    (*pfnCallMe)((PFB) pmbdThis, iModel, iIndex, ptr);  /* do the callback */
		      
		    pvnmbThis = pvnmbThis->next;
		  }
		pdnmmThis = pdnmmThis->next;
	    }   /*molecule*/
	  /* traverse inter-molecule bonds */
	  pvnmbThis = pmsdThis->pvnmbIMBHead;
	  while (pvnmbThis)
	    { /* inter-molecule bonds */
		pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
		(*pfnCallMe)((PFB) pmbdThis, iModel, iIndex, ptr); /* do the callback */
		
		pvnmbThis = pvnmbThis->next;
	    }
          /* traverse Objects */
	  pvnmoThis = pmsdThis->pvnmoHead;
	  while (pvnmoThis)
	    { 
		if (pvnmoThis->choice == iModel)
  		  {	/* only those models stated */
			pmodThis = (PMOD) pvnmoThis->data.ptrvalue;
			(*pfnCallMe)((PFB) pmodThis, iModel, iIndex, ptr);
		  }
		pvnmoThis = pvnmoThis->next;
	    }
	 
          /* traverse Densities */
	  pvnmdThis = pmsdThis->pvnmdHead;
	  while (pvnmdThis)
	    { 
		if (pvnmdThis->choice == iModel)
		  {	/* only those models stated */
			pmddThis = (PMDD) pvnmdThis->data.ptrvalue;
			(*pfnCallMe)((PFB) pmddThis, iModel, iIndex, ptr);
		  }
		pvnmdThis = pvnmdThis->next;
	    }
     return TRUE;
    }
  return FALSE;
}

/*  Function to retrieve Biostrucs from ASN.1 files  */
/************************************************************/
/*							    */
/*	BiostrucAsnGet()				    */
/*							    */
/*	Retrieves Biostruc with a structure model of 	    */
/*	choice by ignoring portions of the I/O stream	    */
/*							    */
/************************************************************/

BiostrucPtr LIBCALL BiostrucAsnGet(AsnIoPtr aip, AsnTypePtr atp, Int4 mdlLvl, Int4 maxModels)
{
    BiostrucPtr retval;
    DataVal av;
    AsnOptionPtr aop;


    av.intvalue = mdlLvl;
    aop = AsnIoOptionNew(aip, OP_NCBIOBJSTR, BIOSTRUC_CHECK_MODEL, av, NULL);
    av.intvalue = maxModels;
    aop = AsnIoOptionNew(aip, OP_NCBIOBJSTR, BIOSTRUC_MAX_MODELS, av, NULL);
    
    if (mdlLvl == ALLSIMPLEMDL)
    { /* this is a reaction to an earlier kludge, when ONECOORDATOM didn't
	 contain all of the models.  Now they do contain all the models, so
	 by forcing this value here we avoid the unnecessary fetching of
	 multiple models.  J. Epstein per C. Hogue, 11/18/96 */
	mdlLvl = ONECOORDATOM;
    }

    retval = BiostrucAsnRead(aip, atp);

    AsnIoOptionFree(aip, OP_NCBIOBJSTR, BIOSTRUC_CHECK_MODEL);
    AsnIoOptionFree(aip, OP_NCBIOBJSTR, BIOSTRUC_MAX_MODELS);
    if (mdlLvl == ONECOORDATOM || mdlLvl == ONECOORDRES || mdlLvl == VECMODEL)
    { /* we have left AsnIoStream in a hopelessly messed-up state, so reset it */
        AsnIoReset(aip);
    }

    return retval;
}

/* The accmmdbs.c stub has BiostrucAvail returning FALSE,
   and empty BiostrucAsn Read and Write functions.
   It should not be added to project libraries (Mac), or
   will be be overridden on linking (PC or UNIX). */

Boolean LIBCALL BiostrucAvail (void)

{
  return TRUE;
}

/*	These 2 Bool funs were moved from mkbioseqB.c - DZ 4/27/99    */

/***********************************************************************/
/*  isBiopoly()					                       */
/*							               */
/*  Given a molecule id, checks whether the corresponding molecule is  */
/*  either protein, DNA, or RNA and has coordinates.		       */
/*							               */
/***********************************************************************/ 

Boolean LIBCALL isBiopoly(Int4 molecule_id, MoleculeGraphPtr currentbp)
{
  Boolean ret=FALSE;
  while (currentbp) {
    if (currentbp->id == molecule_id) {
      ret = TRUE;
      break;
    }
    currentbp = currentbp->next;
  }
  return ret;
}
/*************************************************************************/
/*	isHet()							         */
/*								         */
/*  Given a molecule id, checks whether the corresponding molecule is    */
/*  a heterogen but not solvent or of unknown type, and has coordinates. */
/*									 */
/*************************************************************************/ 

Boolean LIBCALL isHet(Int4 molecule_id, MoleculeGraphPtr currenthet)
{
  Boolean ret=FALSE;
  while (currenthet) {
    if (currenthet->id == molecule_id) {
      ret = TRUE;
      break;
    }
    currenthet = currenthet->next;
  }
  return ret;
}
