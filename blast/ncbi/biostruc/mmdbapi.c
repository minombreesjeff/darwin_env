/*   mmdbapi.c
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
* Author:  Christopher Hogue 
*
* Version Creation Date:   08/19/95
*
* $Revision: 6.12 $
*
* File Description: MMDBAPI wrappers. 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
*
* $Log: mmdbapi.c,v $
* Revision 6.12  1999/08/04 21:21:25  lewisg
* made ClearStructure turn off neighbor mode in non-cn3d mode
*
* Revision 6.11  1999/03/05 22:43:59  vakatov
* Made ClearStructures() function be NLM_EXTERN -- for the DLL build on PC
*
* Revision 6.10  1999/03/05 20:22:31  kans
* ClearStructures clears neighbor bit
*
* Revision 6.9  1998/08/26 18:02:38  kans
* fixed -v -fd warnings
*
* Revision 6.8  1998/08/12 21:54:37  lewisg
* pick correct master in SetNeighborOn
*
* Revision 6.7  1998/05/06 16:25:27  lewisg
* get rid of clear all structures bug
*
* Revision 6.6  1998/04/16 00:30:00  lewisg
* get rid of dead code
*
* Revision 6.5  1998/04/05 04:29:01  lewisg
* make SetNeighborOn set the right slaves in cn3d mode
*
* Revision 6.4  1998/04/04 00:57:52  lewisg
* corrected several functions to behave properly in neighbor mode
*
* Revision 6.3  1998/03/13 22:34:18  lewisg
* fix neighbor mode
*
* Revision 6.2  1998/03/06 01:16:00  lewisg
* merge
*
* Revision 6.0  1997/08/25 18:10:48  madden
* Revision changed to 6.0
*
* Revision 5.0  1996/05/28 14:02:09  ostell
* Set to revision 5.0
*
 * Revision 1.6  1996/05/15  17:01:35  hogue
 * Fixed compiler warnings.
 *
 * Revision 1.5  1996/04/18  17:05:49  hogue
 * Added neighbor mode functionality, setting Master & Slave structures.  GetFirstModelstruc gets first slave now when in neighbor mode.  Backing out of neighbor mode resets pdnmsSelected to the Master.
 *
 * Revision 1.4  1996/01/31  21:30:49  hogue
 * Major changes to accomodate multiple structures, added mmdbAPI
 * data structure, list pointers for traversing structures,
 * list pointers for PFB selection holders, etc.
 *
 * Revision 1.3  1995/10/07  15:24:19  hogue
 * Counting biostrucs from 0 to n.
 *
 * Revision 1.2  1995/10/05  20:22:42  hogue
 * Fixed some structure tracking bugs, added ClearStructures()
 *
 * Revision 1.1  1995/09/20  13:45:47  kans
 * Initial revision
 *
*
* ==========================================================================
*/


/*

  WARNING!  The way some of these functions work is dependent on the setting of 
  pmmdbapi->fCn3D, which is set to TRUE when being run by Cn3D.  This is to
  eliminate the modality of those functions when running in Neighbor mode.

  */

#include <mmdbapi.h>



static pmmdbAPI pmmdbapi = NULL;

Boolean LIBCALL IsMMDBAPIOpen(void)
{
  return (Boolean) (pmmdbapi != NULL);
} 

void LIBCALL fnMMDBCn3Dmode (void)  /* may I never live this down */
{
  pmmdbapi->fCn3D = TRUE;
}

pmmdbAPI LIBCALL NewMMDBAPI(void)
{
  pmmdbAPI papi = NULL;
  
  papi = (pmmdbAPI)MemNew((size_t)(sizeof(mmdbAPI)));
  return papi;
}

Int2 LIBCALL OpenMMDBAPI(Byte bExtent, CharPtr pcDictName)
{
 
  if (pmmdbapi != NULL) return 1;
  else pmmdbapi = NewMMDBAPI();
  if (bExtent == 0)
    pmmdbapi->bExtent = (Byte) POWER_VIEW;
  else  
    pmmdbapi->bExtent = (Byte) bExtent;
 if (pcDictName == NULL)
    pmmdbapi->prgdDict =  LoadDict(DICT_DEFAULT);
  else 
    pmmdbapi->prgdDict =  LoadDict(pcDictName);
  if (!pmmdbapi->prgdDict)
    {
	ErrClear(); 
	ErrPostEx(SEV_FATAL,0,0,  "Unable to Load Residue Dictionary");
	ErrShow();
	return 0;
    } 
  if (! (objmmdb1AsnLoad() && objmmdb2AsnLoad() && objmmdb3AsnLoad() ))
     {
	ErrClear(); 
	ErrPostEx(SEV_FATAL,0,0, "Internal objmmdbNAsnLoad() Failure");
	ErrShow();
	return 0;
    }
  pmmdbapi->iLocalCount = 0;
  pmmdbapi->iFeatureSet = 0;
  pmmdbapi->iFeatureId = 0;
  pmmdbapi->pvnsffHead = NULL; 
  pmmdbapi->pColorFunc = NULL;
  pmmdbapi->Neighbor = FALSE;
  pmmdbapi->fCn3D = FALSE;
  return 1;
}


BiostrucPtr LIBCALL FetchBiostrucPDB(CharPtr pcAcession, Int4 iModelLevel, Int4 iMaxModels)
{
  BiostrucPtr pbsTemp;
  pbsTemp = FetchBS(pcAcession, INP_PDB_ACCESSION, iModelLevel, 
		iMaxModels, pmmdbapi->bExtent);
  return pbsTemp;
}

PDNMS LIBCALL MakeAModelstruc(BiostrucPtr pbsThis)
{
    PDNMS pdnmsTemp = NULL;
    PDNMS pdnmsCount = NULL;
    PMSD  pmsdThis = NULL;
    PMSD  pmsdMaster = NULL;
    Int4  iCount = 0;
    
    if (!pmmdbapi) 
      {
         ErrClear(); 
	 ErrPostEx(SEV_FATAL,0,0, "MMDBAPI Not Initialized\n  Use InitMMDBAPI() \n");
	 ErrShow();
         return pdnmsTemp;
      }
      
    if (!pbsThis) return pdnmsTemp;
    if (pmmdbapi->Neighbor && pmmdbapi->pdnmsMaster) 
      {
         pmsdMaster = (PMSD) pmmdbapi->pdnmsMaster->data.ptrvalue;
         pmmdbapi->iLocalCount++;
         pdnmsTemp = Biostruc2Modelstruc(&pmsdMaster->pdnmsSlaves,  pbsThis,  pmmdbapi->prgdDict, 
				     pmmdbapi->bExtent, pmmdbapi->iLocalCount, NULL, (Int2)0);
      } 
    else
      {
        pmmdbapi->iLocalCount++;
        pdnmsTemp = Biostruc2Modelstruc(&pmmdbapi->pdnmsMain,  pbsThis,  pmmdbapi->prgdDict, 
				     pmmdbapi->bExtent, pmmdbapi->iLocalCount, NULL, (Int2)0);
      }
    if (!pdnmsTemp)
       {
	 pmmdbapi->iLocalCount--;  /* back it off the counter */
         return pdnmsTemp;  /* NOTE THE BIOSTRUC pbsThis is FREED if this fails!!*/
       } 
  
    if (pmmdbapi->Neighbor) 
      {
         pmmdbapi->pdnmsSlave  = pdnmsTemp;
                  pmmdbapi->pdnmsSelected = pmmdbapi->pdnmsMaster;
      }
    else  pmmdbapi->pdnmsSelected = pdnmsTemp;
     
    pmsdThis = (PMSD) pdnmsTemp->data.ptrvalue;
    pmsdThis->iHashChange = MakeHashChange(pdnmsTemp);
    AssignBackBone(pdnmsTemp); 
    AssignVirtual(pdnmsTemp);
    AssignIons(pdnmsTemp);

    return pdnmsTemp;
}


PDNMS LIBCALL GetPDNMSMain(void)  
{
    return pmmdbapi->pdnmsMain;
}

PRGD LIBCALL GetPRGDDictionary(void)
{
    return pmmdbapi->prgdDict;
}


void LIBCALL ChangeMMDBAPIbExtent(Byte bExtent)
{
    pmmdbapi->bExtent = bExtent;
}


Byte LIBCALL GetMMDBAPIbExtent(void)
{
    return pmmdbapi->bExtent;
}




Int4 LIBCALL MakeHashChange(PDNMS pdnmsThis)
{
  PMSD pmsdThis = NULL;
  Int4 iHashCalc = 0;
  Int4 iAtomLocs = 0;  

  if (!pdnmsThis) return iHashCalc;
  pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
  iAtomLocs =  AssignAtomLocId(pdnmsThis);  
  if (!(pmsdThis->bUpdate & (Byte) AM_CHANGED))
       return pmsdThis->iMMDBid;

  /* only changed if dirty bit set ... */
  iHashCalc = pmsdThis->iMMDBid + pmsdThis->iModels + pmsdThis->iFeatures + 
	    pmsdThis->iObjCount + pmsdThis->iDensCount + 100000 + iAtomLocs;
  iHashCalc++; /* inc by one... for incremental changes (eg transform'ns) */
  return iHashCalc; 
    
}

Int4 LIBCALL CountModelstrucs(void)
{
    Int4 iCount = 0;
    PDNMS pdnmsCount = NULL;
    
    if (!pmmdbapi) return iCount;
    pdnmsCount = pmmdbapi->pdnmsMain;
    while (pdnmsCount)
      {
	  iCount++;
	  pdnmsCount = pdnmsCount->next;
      }
    return iCount;
}


void LIBCALL FreeAModelstruc(PDNMS pdnmsThis)
{
    Int2 iLocalId;
    ValNodePtr pvnKillSelectList = NULL;
    

    iLocalId = (Int2) pdnmsThis->choice;
    pvnKillSelectList = ValNodeExtractList(&pmmdbapi->pvnPFBSelect, iLocalId);
    ValNodeFree(pvnKillSelectList);  /* selection list purged for this Modelstruc */
    if (pdnmsThis == pmmdbapi->pdnmsSelected)
	  pmmdbapi->pdnmsSelected = NULL;
    if (pdnmsThis == pmmdbapi->pdnmsPrevious)
   	  pmmdbapi->pdnmsPrevious = NULL;
    if (pdnmsThis == pmmdbapi->pdnmsSlave)
          pmmdbapi->pdnmsSlave = NULL;
    if (pdnmsThis == pmmdbapi->pdnmsHolder)
          pmmdbapi->pdnmsHolder = pdnmsThis->last;
    if(AreNeighborsOn() && pdnmsThis != pmmdbapi->pdnmsMaster) 
      FreeDNMS(((PMSD)(pmmdbapi->pdnmsMaster->data.ptrvalue))->pdnmsSlaves, pdnmsThis);
      else 
      {
         if (pdnmsThis == pmmdbapi->pdnmsMaster) pmmdbapi->pdnmsMaster = NULL;	
         pmmdbapi->pdnmsMain = FreeDNMS(pmmdbapi->pdnmsMain, pdnmsThis);
      }
}


NLM_EXTERN void LIBCALL ClearStructures(void)
{
  PDNMS pdnmsThis = NULL;

     if (!pmmdbapi) return;
     pdnmsThis = GetFirstModelstruc();
     if (!pdnmsThis) return;
     FreeListDNMS(pdnmsThis);
     if (!pmmdbapi->Neighbor || pmmdbapi->fCn3D)
       {
        pmmdbapi->pdnmsMain = NULL;
        pmmdbapi->pdnmsMaster = NULL;
       }
       pmmdbapi->Neighbor = 0;
       pmmdbapi->pdnmsSlave  = NULL;
       pmmdbapi->pdnmsHolder = NULL;
       pmmdbapi->pdnmsSelected = NULL;
       pmmdbapi->pdnmsPrevious = NULL;
       ValNodeFree(pmmdbapi->pvnPFBSelect);
       pmmdbapi->pvnPFBSelect = NULL;
       pmmdbapi->pvnUndoMark = NULL;
/* lyg.  no freeing of feature data in pmmdbapi.  Is this a good idea? */
}


void LIBCALL CloseMMDBAPI(void)
{
    if (!pmmdbapi) return;
    if (pmmdbapi->pdnmsMain) FreeListDNMS(pmmdbapi->pdnmsMain);
    if (pmmdbapi->pvnPFBSelect) ValNodeFree(pmmdbapi->pvnPFBSelect);
    if (pmmdbapi->prgdDict) BiostrucResidueGraphSetFree(pmmdbapi->prgdDict);
        if (pmmdbapi->pvnsffHead) FreeListVNSFF(pmmdbapi->pvnsffHead);
    if (pmmdbapi) MemFree(pmmdbapi);
    return;
}



CharPtr LIBCALL GetStrucStrings(PDNMS pdnmsThis, Int4 iPickType)
{
    PMSD pmsdThis;
    
    if (!pdnmsThis) pdnmsThis = pmmdbapi->pdnmsMain;
    if (!pdnmsThis) return NULL;
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    if (pmsdThis->bMe != (Byte) AM_MSD) return NULL;
    if (iPickType == LONG_NAME) return (pmsdThis->pcChemName);
    if (iPickType == PDB_ACC) return (pmsdThis->pcPDBName);
    if (iPickType == PDB_CLASS) return (pmsdThis->pcPdbClass);
    if (iPickType == PDB_SOURCE) return (pmsdThis->pcPdbSource);
    return NULL;	
} 



void LIBCALL SetSelectedModelstruc(PDNMS pdnmsThis)
{

/* doesn't check to see if structure is a really a slave if Neighbor mode on */
  
    if (pmmdbapi->Neighbor == TRUE && !(pmmdbapi->fCn3D))
      {
         pmmdbapi->pdnmsSlave = pdnmsThis;
         pmmdbapi->pdnmsSelected = pdnmsThis;
      }
    else
       pmmdbapi->pdnmsSelected = pdnmsThis;
}

void LIBCALL SetNeighborOn(void)
{
    
   if (!pmmdbapi->pdnmsMaster)
       pmmdbapi->pdnmsMaster = GetSelectedModelstruc();
   
   if ( pmmdbapi->pdnmsMaster && !(pmmdbapi->fCn3D))
     {
      pmmdbapi->Neighbor = TRUE;
      if (!pmmdbapi->pdnmsSlave)  
        {
          pmmdbapi->pdnmsSlave = GetFirstModelstruc(); /* NULL if none loaded */
        }
      else pmmdbapi->Neighbor = FALSE;
     }
  else
    {
      pmmdbapi->pdnmsMaster = GetSelectedModelstruc();
      if (pmmdbapi->pdnmsMaster)
      {
        pmmdbapi->pdnmsSlave = ((PMSD)(pmmdbapi->pdnmsMaster->data.ptrvalue))->pdnmsSlaves;
        pmmdbapi->Neighbor = TRUE;
      }
    }
}


Boolean LIBCALL AreNeighborsOn(void)
{
  return pmmdbapi->Neighbor;
}

void LIBCALL SetNeighborOff(void)
{
   pmmdbapi->pdnmsSelected = pmmdbapi->pdnmsMaster;
   pmmdbapi->Neighbor = FALSE;
   pmmdbapi->pdnmsSlave = NULL;
}

PDNMS LIBCALL GetSelectedModelstruc(void)
{
     if (pmmdbapi->Neighbor == TRUE && !(pmmdbapi->fCn3D))
     return pmmdbapi->pdnmsSlave;
   else
    return pmmdbapi->pdnmsSelected;
}

PDNMS LIBCALL SwapModelstruc(void)
{
    PDNMS pdnmsTemp;
    pdnmsTemp = pmmdbapi->pdnmsSelected;
    pmmdbapi->pdnmsSelected = pmmdbapi->pdnmsPrevious;
    pmmdbapi->pdnmsPrevious = pdnmsTemp;
    return pmmdbapi->pdnmsSelected;
}


void LIBCALL SetMasterModelstruc(PDNMS pdnmsThis)
{
    pmmdbapi->pdnmsMaster = pdnmsThis;
}

PDNMS LIBCALL GetMasterModelstruc(void)
{
    return pmmdbapi->pdnmsMaster;
}

void LIBCALL SetSlaveModelstruc(PDNMS pdnmsThis)
{
    pmmdbapi->pdnmsSlave = pdnmsThis;
}

PDNMS LIBCALL GetSlaveModelstruc(void)
{
   return pmmdbapi->pdnmsSlave;
}

/**************************************************/
/*  GetFirstModelstruc, GetNextModelstruc and     */
/*  GetPreviousModelstruc are provided to walk    */
/*  the doubly-linked list of structures that     */
/*  MMDBAPI has in memory...                      */
/*  SetHolderModelstruc allows one to set the     */
/*  pointer to any of the structures              */
/**************************************************/

PDNMS LIBCALL GetFirstModelstruc(void)
{

   PMSD pmsdThis = NULL;
   PDNMS pdnmsThis = NULL;

   if (pmmdbapi->Neighbor && !(pmmdbapi->fCn3D))
     {
       pdnmsThis = GetMasterModelstruc();
       if (pdnmsThis)
         {
           pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
           pmmdbapi->pdnmsHolder = pmsdThis->pdnmsSlaves;
           return pmsdThis->pdnmsSlaves;
         }
        else return NULL;
     }
    pmmdbapi->pdnmsHolder = pmmdbapi->pdnmsMain;
    return pmmdbapi->pdnmsMain;
}


void LIBCALL SetHolderModelstruc(PDNMS pdnmsThis)
{
    pmmdbapi->pdnmsHolder = pdnmsThis;
}

PDNMS LIBCALL GetNextModelstruc(void)
{
    if (pmmdbapi->pdnmsHolder)
      {
	 pmmdbapi->pdnmsHolder = pmmdbapi->pdnmsHolder->next;
	 return pmmdbapi->pdnmsHolder;
      }
    return NULL;
}

PDNMS LIBCALL GetPreviousModelstruc(void)
{  /* node which came before this one */
    if (pmmdbapi->pdnmsHolder)
      {
	pmmdbapi->pdnmsHolder = pmmdbapi->pdnmsHolder->last;
    	return pmmdbapi->pdnmsHolder;
      }
    return NULL;
}

ValNodePtr LIBCALL AddPFBSelect(PFB pfbThis)
{
    PDNMS pdnmsStruc = NULL;
    PMSD  pmsdStruc = NULL;
    
    if (!pfbThis) return NULL;
    pdnmsStruc = DNFromPFB((PFB)ToMSDParent(pfbThis)); 
    pmmdbapi->pvnUndoMark = ValNodeAddPointer(&pmmdbapi->pvnPFBSelect, 
		       pdnmsStruc->choice, (VoidPtr) pfbThis);   
    /* the choice field allows us to extract pointers 
     * to different structures from the list,  esp. when freeing 
     * specific structures.
     */
    return pmmdbapi->pvnUndoMark;
}


void LIBCALL ClearPFBSelectList(void)
{
    ValNodeFree(pmmdbapi->pvnPFBSelect);
    pmmdbapi->pvnPFBSelect = NULL;
    return;
}

void LIBCALL UndoPFBSelectList(void)
{
    ValNodeFree(pmmdbapi->pvnUndoMark);    
    return;
}



PSFF LIBCALL DoesFeatureTypeHaveFuncs(Uint1 iCode)
{
  ValNodePtr pvn = NULL;
  PSFF psffThis = NULL;

  ValNodeFindNext(pmmdbapi->pvnsffHead, NULL, iCode);
  if (pvn)
    {
      psffThis = (PSFF) pvn->data.ptrvalue;
      return psffThis;
    }
  return NULL;
}

Uint1 LIBCALL Call3DColorProc(ColorPropPtr pcpThis)
{
  Uint1 i = 0;
  if (pmmdbapi->pColorFunc)
     (*(pmmdbapi->pColorFunc)) (pcpThis, &i);
 return i;
}

