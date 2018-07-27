/*   mmdbapi4.c
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
* File Name:  mmdbapi4.c
*
* Author:  Christopher Hogue
*
* Version Creation Date:   10/18/95
*
* $Revision: 6.8 $
*
* File Description: Code for Modelstruc -> Biostruc conversion
*                   And User-feature handling.
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* $Log: mmdbapi4.c,v $
* Revision 6.8  2003/05/13 13:48:20  bauer
* changed iCountAtoms in RefreshModelAsnMem from Int2 to Int4
*
* Revision 6.7  2000/12/05 22:13:07  ywang
* fix bugs for MakePDBSeqId2
*
* Revision 6.6  1999/03/12 18:37:08  kans
* fixed ErrPostEx problem
*
* Revision 6.5  1999/02/25 23:11:53  ywang
* synchronize with cn3d to save out mime data
*
* Revision 6.4  1998/09/02 16:25:27  kans
* removed comments around debug ifdef to suppress printf statement
*
* Revision 6.3  1998/08/26 18:02:43  kans
* fixed -v -fd warnings
*
* Revision 6.2  1998/03/10 04:31:08  lewisg
* change name of MakePDBSeqId to MakePDBSeqId2 to avoid name conflict
*
* Revision 6.1  1998/03/06 23:17:29  lewisg
* codewarrior fixes
*
* Revision 6.0  1997/08/25 18:11:16  madden
* Revision changed to 6.0
*
* Revision 5.3  1996/11/26 19:15:44  epstein
* represent gaps correctly in sequence alignments
*
 * Revision 5.2  1996/11/25  16:14:41  epstein
 * fix off-by-one error since VAST alignments are indexed beginning at position 1 rather than 0
 *
 * Revision 5.1  1996/11/22  21:16:59  epstein
 * add BiostrucAnnotSetToSeqAnnot()
 *
 * Revision 5.0  1996/05/28  14:02:09  ostell
 * Set to revision 5.0
 *
 * Revision 1.4  1996/02/07  23:05:56  hogue
 * fixed a type cast in FreeSingleModel
 *
 * Revision 1.3  1996/02/02  21:26:26  hogue
 * changed to include only mmdbapi.h (brings in all 1-4)
 *
 * Revision 1.2  1996/02/02  19:45:01  hogue
 * Initial Revision
 *
*
* ==========================================================================
*/

/*****************************************************************************
*
*  mmdbapi4.c
*   
*   UNDER CONSTRUCTION NOTICE. 
*	EVERYTHING SUBJECT TO RADICAL CHANGES!
*   
*  programmer C.W.V. Hogue
*  created: 18 Oct 95
*  last mod: 
*****************************************************************************/

#include <mmdbapi.h>
#include <sequtil.h>






static Int4 ScaleToInt(FloatLo f)  
{

  Int4 iI;
  FloatHi fh;
  fh = f * SAVE_XYZ_SCALE_FACTOR;  /* scale floating point portion */
  if (fh > 0)  
    fh += 0.5;   /* this will push a 2.99999 to 3.099999 */
  else  fh -= 0.5;  /* or a -2172.9999 to a -2173.0999  */
  iI = (Int4) fh;  /* which is truncated here */
  return iI;
}		


Boolean LIBCALL  RefreshSurface(SurfaceCoordinatesPtr  pscThis, PMOD pmodThis)
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
    Int4 iCoords = 0;
    Int4 iVal = 0;
    Boolean rsult = FALSE;

    switch (pscThis->Surface_surface->choice)
      {
	   case Surface_surface_cylinder:
 	     if (!(pmodThis->bWhat & (Byte) OBJ_CYLINDER)) return FALSE;
	     pcylThis = (CylinderPtr) pscThis->Surface_surface->data.ptrvalue;
	     if (!pmodThis->ppflObject) return FALSE;
	     pmspTop = pcylThis->axis_top;
             pmspBottom = pcylThis->axis_bottom;
	     prvRadius = pcylThis->radius;
 	     pmspTop->x = ScaleToInt(pmodThis->ppflObject[0][0]); 
	     pmspTop->y = ScaleToInt(pmodThis->ppflObject[0][1]);
	     pmspTop->z = ScaleToInt(pmodThis->ppflObject[0][2]); 
	     pmspTop->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     pmspBottom->x = ScaleToInt(pmodThis->ppflObject[1][0]);  
	     pmspBottom->y = ScaleToInt(pmodThis->ppflObject[1][1]);
	     pmspBottom->z = ScaleToInt(pmodThis->ppflObject[1][2]); 
	     pmspBottom->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     prvRadius->scaled_integer_value = ScaleToInt(pmodThis->flRadius);
	     prvRadius->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     return TRUE;
	   case Surface_surface_sphere:
 	     if (!(pmodThis->bWhat & (Byte) OBJ_SPHERE)) return FALSE;
	     psphThis = (SpherePtr) pscThis->Surface_surface->data.ptrvalue;
	     if (!pmodThis->ppflObject) return FALSE;
	     pmspTop = psphThis->center;
             prvRadius = psphThis->radius;
 	     pmspTop->x = ScaleToInt(pmodThis->ppflObject[0][0]);
	     pmspTop->y = ScaleToInt(pmodThis->ppflObject[0][1]);
	     pmspTop->z = ScaleToInt(pmodThis->ppflObject[0][2]);
	     pmspTop->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     prvRadius->scaled_integer_value = ScaleToInt(pmodThis->flRadius);
	     prvRadius->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     return TRUE;
	   case Surface_surface_brick:
 	     if (!(pmodThis->bWhat &(Byte) OBJ_BRICK)) return FALSE;
	     pbrkThis = (BrickPtr) pscThis->Surface_surface->data.ptrvalue;
	     if (!pmodThis->ppflObject) return FALSE;
	     pmspTemp = pbrkThis->corner_000;
	     pmspTemp->x = ScaleToInt(pmodThis->ppflObject[0][0]);  
	     pmspTemp->y = ScaleToInt(pmodThis->ppflObject[0][1]); 
	     pmspTemp->z = ScaleToInt(pmodThis->ppflObject[0][2]);  
	     pmspTemp->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     pmspTemp = pbrkThis->corner_001;
	     pmspTemp->x = ScaleToInt(pmodThis->ppflObject[1][0]);  
	     pmspTemp->y = ScaleToInt(pmodThis->ppflObject[1][1]); 
	     pmspTemp->z = ScaleToInt(pmodThis->ppflObject[1][2]); 
	     pmspTemp->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     pmspTemp = pbrkThis->corner_010;
	     pmspTemp->x =  ScaleToInt(pmodThis->ppflObject[2][0]); 
	     pmspTemp->y =  ScaleToInt(pmodThis->ppflObject[2][1]); 
	     pmspTemp->z =  ScaleToInt(pmodThis->ppflObject[2][2]); 
	     pmspTemp->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     pmspTemp = pbrkThis->corner_011;
	     pmspTemp->x =  ScaleToInt(pmodThis->ppflObject[3][0]); 
	     pmspTemp->y =  ScaleToInt(pmodThis->ppflObject[3][1]);  
	     pmspTemp->z =  ScaleToInt(pmodThis->ppflObject[3][2]);  
	     pmspTemp->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     pmspTemp = pbrkThis->corner_100;
	     pmspTemp->x =  ScaleToInt(pmodThis->ppflObject[4][0]); 
	     pmspTemp->y =  ScaleToInt(pmodThis->ppflObject[4][1]); 
	     pmspTemp->z =  ScaleToInt(pmodThis->ppflObject[4][2]); 
	     pmspTemp->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     pmspTemp = pbrkThis->corner_101;
	     pmspTemp->x =  ScaleToInt(pmodThis->ppflObject[5][0]); 
	     pmspTemp->y =  ScaleToInt(pmodThis->ppflObject[5][1]); 
	     pmspTemp->z =  ScaleToInt(pmodThis->ppflObject[5][2]); 
	     pmspTemp->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     pmspTemp = pbrkThis->corner_110;
	     pmspTemp->x =  ScaleToInt(pmodThis->ppflObject[6][0]); 
	     pmspTemp->y =  ScaleToInt(pmodThis->ppflObject[6][1]); 
	     pmspTemp->z =  ScaleToInt(pmodThis->ppflObject[6][2]); 
	     pmspTemp->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     pmspTemp = pbrkThis->corner_111;
	     pmspTemp->x =  ScaleToInt(pmodThis->ppflObject[7][0]);
	     pmspTemp->y =  ScaleToInt(pmodThis->ppflObject[7][1]);
	     pmspTemp->z =  ScaleToInt(pmodThis->ppflObject[7][2]);
	     pmspTemp->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     return TRUE;	   
	   case Surface_surface_cone:
 	     if (!(pmodThis->bWhat & (Byte) OBJ_CONE)) return FALSE;
	     pconThis = (ConePtr) pscThis->Surface_surface->data.ptrvalue;
	     if (!pmodThis->ppflObject) return FALSE;
	     pmspTop = pconThis->axis_top;
             pmspBottom = pconThis->axis_bottom;
	     prvRadius = pconThis->radius_bottom;
 	     pmspTop->x = ScaleToInt(pmodThis->ppflObject[0][0]);
	     pmspTop->y = ScaleToInt(pmodThis->ppflObject[0][1]);
	     pmspTop->z = ScaleToInt(pmodThis->ppflObject[0][2]);
	     pmspTop->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     pmspBottom->x = ScaleToInt(pmodThis->ppflObject[1][0]);
	     pmspBottom->y = ScaleToInt(pmodThis->ppflObject[1][1]);
	     pmspBottom->z=  ScaleToInt(pmodThis->ppflObject[1][2]);
	     pmspBottom->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     prvRadius->scaled_integer_value = ScaleToInt(pmodThis->flRadius);
	     prvRadius->scale_factor = SAVE_XYZ_SCALE_FACTOR;;
	     return TRUE;
	   case Surface_surface_tmesh:
	     if (!(pmodThis->bWhat & (Byte) OBJ_TMESH)) return FALSE;
	     ptmshThis = (TMeshPtr) pscThis->Surface_surface->data.ptrvalue;
	     if (!pmodThis->ppflObject) return FALSE;
	     if (!pmodThis->pucSwap) return FALSE; 
	     ptmshThis->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     iCoords = pmodThis->iCoordNo;
	     for (iCount = 0; iCount < iCoords; iCount++) 
	        {
		    pvnX = ValNodeAddInt(&pvnX, 0, ScaleToInt(pmodThis->ppflObject[iCount][0]));
		    pvnY = ValNodeAddInt(&pvnY, 0, ScaleToInt(pmodThis->ppflObject[iCount][1]));
		    pvnZ = ValNodeAddInt(&pvnZ, 0, ScaleToInt(pmodThis->ppflObject[iCount][2]));
		    pvnSwap = ValNodeAddBoolean(&pvnSwap, 0, (Boolean)(pmodThis->pucSwap[iCount] == 
					    (Uchar) 1)); 
		    if (ptmshThis->x == NULL)
		       {
			 ptmshThis->x = pvnX;  /* these have been set to null */
		         ptmshThis->y = pvnY;
			 ptmshThis->z = pvnZ;
			 ptmshThis->swap = pvnSwap;
		       } 
		    if ((pvnX == NULL) || (pvnY == NULL) || (pvnZ == NULL) || (pvnSwap == NULL))
			goto objallocerr;
		} 
	     ptmshThis->number_of_points = iCoords;
	     return TRUE;
	   case Surface_surface_triangles:
 	     if (!(pmodThis->bWhat & (Byte) OBJ_TRIANGLES)) return FALSE;
	     ptriThis = (TrianglesPtr) pscThis->Surface_surface->data.ptrvalue;
	     if (!pmodThis->ppflObject) return FALSE;
	     if (!pmodThis->ppi4Triangles) return FALSE;
	     ptriThis->scale_factor = SAVE_XYZ_SCALE_FACTOR;
	     iCoords = pmodThis->iCoordNo;
	     for (iCount = 0; iCount < iCoords; iCount++) 
	        {
		    pvnX = ValNodeAddInt(&pvnX, 0, ScaleToInt(pmodThis->ppflObject[iCount][0]));
		    pvnY = ValNodeAddInt(&pvnY, 0, ScaleToInt(pmodThis->ppflObject[iCount][1]));
		    pvnZ = ValNodeAddInt(&pvnZ, 0, ScaleToInt(pmodThis->ppflObject[iCount][2]));
		    if (ptmshThis->x == NULL)
		       {
			 ptriThis->x = pvnX;  /* these have been set to null */
		         ptriThis->y = pvnY;
			 ptriThis->z = pvnZ;
		       } 
		    if ((pvnX == NULL) || (pvnY == NULL) || (pvnZ == NULL))
			goto objallocerr;
		} 
	     ptriThis->number_of_points = iCoords;  
	     iCoords = pmodThis->iTriNo;
	     pvnX = NULL; 
	     pvnY = NULL;
	     pvnZ = NULL;
	     for (iCount = 0; iCount < iCoords; iCount++) 
	        {
		    pvnX = ValNodeAddInt(&pvnX, 0, pmodThis->ppi4Triangles[iCount][0]);
		    pvnY = ValNodeAddInt(&pvnY, 0, pmodThis->ppi4Triangles[iCount][1] );
		    pvnZ = ValNodeAddInt(&pvnZ, 0, pmodThis->ppi4Triangles[iCount][2] );
		    if (ptmshThis->x == NULL)
		       {
			 ptriThis->v1 = pvnX;  /* these have been set to null */
		         ptriThis->v2 = pvnY;
			 ptriThis->v3 = pvnZ;
		       } 
		    if ((pvnX == NULL) || (pvnY == NULL) || (pvnZ == NULL))
			goto objallocerr;
		 
		}  
	     ptriThis->number_of_triangles = iCoords;
	     return TRUE;
	   default:
	     rsult = FALSE;
       }
    return rsult;   
    
    objallocerr:
   
      FreeSurfaceModelAsnList(pscThis);
      return FALSE;
}



		
			

static void FreeCoordinatesAsnLists(CoordinatesPtr pcoThis)
{
    AtomicCoordinatesPtr 		pacThis = NULL;
    SurfaceCoordinatesPtr		pscThis = NULL;
    DensityCoordinatesPtr		pdcThis = NULL; 
    
  
    switch (pcoThis->choice)
     {
      case Coordinates_surface:
        pscThis = (SurfaceCoordinatesPtr) pcoThis->data.ptrvalue;
	FreeSurfaceModelAsnList(pscThis);
        return;
      case Coordinates_density:
	pdcThis = (DensityCoordinatesPtr) pcoThis->data.ptrvalue;
/*	FreeDensityModelAsnList(pdcThis); */
        return;
      case Coordinates_atomic:
	pacThis = (AtomicCoordinatesPtr) pcoThis->data.ptrvalue;
	FreeAtomicModelAsnLists(pacThis);
	return;
      default:
        return;
     }
}	
	
	
void LIBCALL   FreeRedundantAsn(PDNMS pdnmsThis)
{
   /* does un-hooking of dict from chemical graph; frees chem-graph */
   /* Recoverable - out of memory errors are handled with this */
   /* frees model data ASN.1 & parts of chemical graph  */

    PMLD pmldThis = NULL;
    PMSD pmsdThis = NULL;
    PDNML pdnmlThis = NULL;
    BiostrucModelPtr pbsmThis = NULL;
    ModelCoordinateSetPtr pmcsThis = NULL;
    CoordinatesPtr pcoThis = NULL;
    PDNMM pdnmmThis = NULL;
    PMMD pmmdThis = NULL;
    MoleculeGraphPtr pmgThis = NULL;

    /* get rid of model redundant stuff */
    if (!pdnmsThis) return;
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    if (!pmsdThis) return;
    
    /* remove redundant parts of Chemical Graph */
    /* keep, dictionary, descrs and seq_id's */
    
    if (pmsdThis->pbsBS->chemical_graph)
      {
	/* unhook dict from chem graph */
	if (pmsdThis->pbsBS->chemical_graph->residue_graphs)
	pmsdThis->pbsBS->chemical_graph->residue_graphs = NULL;
	/* these are pointed to also by pmsdThis->pDictLocal */

	/* unhook the Descr's from chem graph */
	if (pmsdThis->pbsBS->chemical_graph->descr)
	  pmsdThis->pbsBS->chemical_graph->descr = NULL;
	/* pointed to by pmsdThis->pGraphDescr */
    
	/* walk the molecules in the chem graph, unhook the pMolDescr, pSeqId */
	pmgThis = pmsdThis->pbsBS->chemical_graph->molecule_graphs;
	while (pmgThis) /* walk up the molecule_graphs */
	{
	    pmgThis->descr = NULL;
	    pmgThis->seq_id = NULL;
	    pmgThis = pmgThis->next;
	}
      
	/* free chem-graph */
	BiostrucGraphFree(pmsdThis->pbsBS->chemical_graph);
	pmsdThis->pbsBS->chemical_graph = NULL;
      }
  
    /* remove redundant parts of models */ 
    
    pdnmlThis = pmsdThis->pdnmlModels;
    while (pdnmlThis)
      {
	pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
	if (!pmldThis) return;	
	while (pmldThis) 
	{  
	    pmcsThis = pmldThis->pbsmModel->model_coordinates;
	    while (pmcsThis)
		{  /* match the two together */  
		    if (pmcsThis->id == pmldThis->iNoCoordSet) break;
		    pmcsThis = pmcsThis->next;
		}  /* find the Asn.1 stub */
	    pcoThis = (CoordinatesPtr) pmcsThis->Coordinates_coordinates->data.ptrvalue;
	    if (pcoThis) FreeCoordinatesAsnLists(pcoThis);  /* pare down to stubs */
	    pmldThis = pmldThis->next;
	}
       pdnmlThis = pdnmlThis->next;
     }
  return;  
}	


 
Boolean LIBCALL RefreshModelAsnMem(PDNMS pdnmsThis,  Int2 iModel)
{

/* Does a direct copy of all Modelstruc coordinates into Biostruc Model      */
/* data structures. Used to populate coordinates before saving               */
/* The ASN.1 stub is validated - and it must be present and complete, or     */ 
/* the routine will fail */
/* The stub would be absent under conditions where a model was               */
/* constructed by hand without using the NewModel() function 	             */


    PMAD	pmadThis = NULL;
    PALD	paldThis = NULL;
    PMOD	pmodThis = NULL;
    PMDD	pmddThis = NULL;
    PMLD	pmldThis = NULL;
    PDNML	pdnmlThis = NULL;
    PMSD	pmsdThis = NULL;
    PDNMM	pdnmmThis = NULL; 
    PMMD	pmmdThis = NULL; 
    PDNMG	pdnmgThis = NULL;
    PMGD	pmgdThis = NULL;  
    PVNMA	pvnmaThis = NULL;
    
    AlternateConformationIdsPtr 	paciThis = NULL;
    BiostrucModelPtr 			pbsmThis = NULL;
    AtomicCoordinatesPtr 		pacThis = NULL;
    IsotropicTemperatureFactorsPtr 	pitfThis = NULL;
    AnisotropicTemperatureFactorsPtr 	patfThis = NULL;
    AtomicOccupanciesPtr 		pocThis = NULL;
    ModelSpacePointsPtr 		pmspThis = NULL;
    AtomPntrsPtr			pappThis = NULL;
    CoordinatesPtr			pcoThis = NULL;
    ModelCoordinateSetPtr		pmcsThis = NULL;
    SurfaceCoordinatesPtr		pscThis = NULL;
  /*  DensityCoordinatesPtr		pdcThis = NULL; 
  */   
    CharPtr				pcTemp = NULL;
    Int4				iCountAtoms = 0;
    Int4				iX,  iY,  iZ;
    Int4				iCoordSets = 0;
    Boolean				bFirst = TRUE;
    Boolean				bGotoCalled = FALSE;
    ValNodePtr	pvnX = NULL;
    ValNodePtr	pvnY = NULL;
    ValNodePtr	pvnZ = NULL;
    ValNodePtr	pvnB11 = NULL;
    ValNodePtr	pvnB12 = NULL;
    ValNodePtr	pvnB13 = NULL;
    ValNodePtr	pvnB22 = NULL;
    ValNodePtr	pvnB23 = NULL;
    ValNodePtr	pvnB33 = NULL;
    ValNodePtr	pvnOcc = NULL;
    ValNodePtr	pvnAlt = NULL;
    ValNodePtr	pvnMid = NULL;
    ValNodePtr	pvnRid = NULL;
    ValNodePtr	pvnAid = NULL;
    ValNodePtr	pmdrThis = NULL;       
    int		iBailno = 0;
   
 
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
 /* find the right model in the doubly-linked list */
    if (!pmsdThis->pdnmlModels) 
      {

#ifdef _DEBUG_5 
printf("No models in structure...\n");
#endif    
   
	 return FALSE;
      }
    pdnmlThis = DValNodeFindNext(pmsdThis->pdnmlModels, NULL, iModel); 
    if (!pdnmlThis)
      {
#ifdef _DEBUG_5 
printf("No such model in RefreshModelAsn \n");
   
         pdnmlThis = pmsdThis->pdnmlModels;
 	 while (pdnmlThis)
	  {
 	    printf("but found model %d\n", (int) pdnmlThis->choice);
	    pdnmlThis = pdnmlThis->next;
	  }
	 return FALSE;  /* no such model */
#endif 
      }
 /* cast to the model node */
    pmldThis = (PMLD) pdnmlThis->data.ptrvalue;  
    if (!pmldThis->pbsmModel) { iBailno = 1; goto errorbail; }
 /* there must be a pbsmModel at this point */
    pbsmThis = pmldThis->pbsmModel;
    pbsmThis->id =  pmldThis->iId;   /* refresh the id, type */
    pbsmThis->type = pmldThis->iType;
    
    /* refresh  name string   */  
    /* find the old string */
    pmdrThis = ValNodeFindNext(pbsmThis->descr, NULL, ModelDescr_name);
    if (pmdrThis)
      {
         /* free it */
	 if (pmdrThis->data.ptrvalue) 
	   MemFree((CharPtr) pmdrThis->data.ptrvalue);
	 /* replace it */
	 if (pmldThis->pcModelName) 
	    pmdrThis->data.ptrvalue = StringSave(pmldThis->pcModelName);
	 else 
	    pmdrThis->data.ptrvalue = NULL;
      }  
    else
      {
	  ValNodeCopyStr(&pmldThis->pbsmModel->descr, ModelDescr_name, pmldThis->pcModelName);
	  /* if no string, this does nothing */
      }
      /* conditionally rebuild the ASN.1 stub */
    if (!pbsmThis->model_coordinates)
      {
	iBailno = 1;
	goto errorbail;
      }
      
    /* model_space is optional - if it wasn't already there we don't fill it in here... */
      
    if (!pbsmThis->model_coordinates) { iBailno = 2; goto errorbail; }
       /* there must be a model_coordinates node */
      
    /* Do each coord set for this one model */
    while (pmldThis) 
     {  
     
      /*
       * each pmldThis contains either one chemical graph's worth of ald's or
       * one object or one density  pointer 
       * There must be one pmcsThis for each pmldThis. 
       */   
	
	/* find the matching id in the ASN.1 stub model_coordinates list */
	pmcsThis = pbsmThis->model_coordinates;
	while (pmcsThis)
	  {
	    if (pmcsThis->id == pmldThis->iNoCoordSet) break;
	    pmcsThis = pmcsThis->next;
	  }  
	if (!pmcsThis){ iBailno = 3; goto errorbail; } /* not found - ASN.1 stub error condition */
	
	
	
	/* find all the heads of pointers we'll need to be updating */
        /* from within the ASN.1 stub */
    
	  
	if (!pmcsThis->Coordinates_coordinates) { iBailno = 4; goto errorbail; }  
	  /* this part of the ASN.1 stub must be present */
	if (pmcsThis->Coordinates_coordinates->choice == Coordinates_coordinates_reference) goto skipnext;
    	    /* cannot handle coordinates by reference yet */
		
	pcoThis = (CoordinatesPtr) pmcsThis->Coordinates_coordinates->data.ptrvalue;
	if (!pcoThis) { iBailno = 5; goto errorbail; } /* this part of the ASN.1 stub must be present */
   
          /* see what kind of data pmldThis contains, surface, density or atoms */
	  
        pmodThis = pmldThis->pmodObject;
	if (pmodThis) /* is it a surface object ? */
	  {
	    if (pcoThis->choice != Coordinates_surface){ iBailno = 6; goto errorbail; } /* ASN.1 stub mismatch */
	      /* rebuild or refresh ASN.1 object */
	      pscThis = (SurfaceCoordinatesPtr) pcoThis->data.ptrvalue;
	      FreeSurfaceModelAsnList(pscThis); 
	      if (!RefreshSurface(pscThis, pmodThis)) { iBailno = 6; goto memnewbail; } /* memory error */;
	      goto skipnext;
	  }
 
	pmddThis = pmldThis->pmddDensity;
	if (pmddThis)  /* is it a density object ?  */
	  {
	    if (pcoThis->choice != Coordinates_density) { iBailno = 7; goto errorbail; }  /* ASN.1 stub mismatch */
	      /* rebuild or refresh ASN.1 density */
	      
	/* 	FreeDensityAsnList();
		RefreshDensity();
	*/
	      goto skipnext;
	  }
	  
	/* still here - MUST be atoms */
	if (pcoThis->choice != Coordinates_atomic) { iBailno = 8; goto errorbail; }  /* ASN.1 stub mismatch */
	
	pacThis = NULL;
	pacThis = (AtomicCoordinatesPtr) pcoThis->data.ptrvalue;
	if (!pacThis) { iBailno = 9; goto errorbail; }  /* ASN.1 stub not complete */
	
	/* Free all remaining data off AtomicCoordinatesPtr pacThis */
	
	FreeAtomicModelAsnLists(pacThis);
	
	/* at this point pacThis is clean excepting any !!!CONF-ENSEMBLES!! that may be present */
	
	/* go ahead and rebuild the pacThis model linked-lists from the ALD node data */

	iCountAtoms = 0;
	bFirst = TRUE;
	bGotoCalled = FALSE;
	pdnmmThis = pmsdThis->pdnmmHead;
	if (pmldThis->ppAsnOrder)
	  {   /* if the order matrix is present, then     */
	      /* traverse nodes in order they were loaded */
	      /* This matrix begins with element #1 for ASN.1 indexing purposes */
	      /* and ends with a null pointer */
	    for (iCountAtoms = 1; pmldThis->ppAsnOrder[iCountAtoms] != NULL; iCountAtoms++)
	      {
		  paldThis = pmldThis->ppAsnOrder[iCountAtoms];
		  bGotoCalled = TRUE;
		  pmadThis = (PMAD) paldThis->pfbParent;
		  goto aldsaveblockstart;
		   /* goto used to avoid  code duplication                         */
		   /* making a separate function is impractical in this case owing */
		   /* to the large number of linked-lists kept as overhead         */
		   /* This way, everything stays in scope.                         */
		  aldsaveblockreturn: ;
		  		 
	      }  /* for */
	    iCountAtoms--;  /* back it up one to the correct count */
	  }  /* if ppAsnOrder */
	else  
	  /* traverse the hierachical structure to find paldThis */
	while(pdnmmThis)
	    {   /*molecule*/
		pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
		pdnmgThis =  pmmdThis->pdnmgHead;
		while(pdnmgThis)
		{   /*graph*/
		    pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
		    pvnmaThis = pmgdThis->pvnmaAHead;
		    while (pvnmaThis)
			{   /*atom*/
			    pmadThis = (PMAD) pvnmaThis->data.ptrvalue;
			    paldThis = GetAtomLocs(pmadThis,  iModel);
			    while (paldThis)
			      {
				iCountAtoms++;
	aldsaveblockstart:   /* goto used in the for loop to avoid code duplication */
				if ((!paldThis->pflvData)  || (paldThis->iFloatNo <  2)) goto errorbail; /* coords missing */
				iX =  ScaleToInt(paldThis->pflvData[0]);
				iY =  ScaleToInt(paldThis->pflvData[1]);
				iZ =  ScaleToInt(paldThis->pflvData[2]);
				   
				if (bFirst)
				  {
				     /* initialize lists off of pacThis */
				     /* note that each ALD must have same amt of data as first */
				    pacThis->atoms = AtomPntrsNew();
				    if (!pacThis->atoms) { iBailno = 10; goto memnewbail; } 
				       /* number_of_ptrs, molecule_ids, resiude_ids, atom_ids */
				    pacThis->atoms->molecule_ids = 
					ValNodeAddInt(	NULL, 
							0, 
							IndexFromNode((PFB)GetParentMol((PFB)pmadThis)));
				    if (!pacThis->atoms->molecule_ids) { iBailno = 11; goto memnewbail; } 
				    pvnMid = pacThis->atoms->molecule_ids;
				    pacThis->atoms->residue_ids = 
					ValNodeAddInt(	NULL, 
							0, 
							IndexFromNode((PFB)GetParentGraph((PFB)pmadThis)));
				    if (!pacThis->atoms->residue_ids){ iBailno = 12; goto memnewbail; } 	
				    pvnRid = pacThis->atoms->residue_ids;					
				    pacThis->atoms->atom_ids =
					ValNodeAddInt(	NULL, 
							0, 
							IndexFromNode((PFB)pmadThis));
				    if (!pacThis->atoms->atom_ids) { iBailno = 13; goto memnewbail; } 	  
				    pvnAid = pacThis->atoms->atom_ids; 
				    pacThis->sites = ModelSpacePointsNew();
				    if (!pacThis->sites){ iBailno = 14; goto memnewbail; } 
				      /* scale_factor, x, y, z */
				    pacThis->sites->x = ValNodeAddInt(NULL, 0, iX);
				    if (!pacThis->sites->x) { iBailno = 15; goto memnewbail; } 
				    pvnX = pacThis->sites->x;
				    pacThis->sites->y = ValNodeAddInt(NULL, 0, iY);
				    if (!pacThis->sites->y) { iBailno = 16; goto memnewbail; } 
				    pvnY = pacThis->sites->y;
				    pacThis->sites->z = ValNodeAddInt(NULL, 0, iZ); 
				    if (!pacThis->sites->z) { iBailno = 17; goto memnewbail; } 
				    pvnZ = pacThis->sites->z;
				    pacThis->sites->scale_factor = SAVE_XYZ_SCALE_FACTOR;
				    if (paldThis->iFloatNo >= 3)
				      {
					  pacThis->occupancies = AtomicOccupanciesNew();
					  if (!pacThis->occupancies) { iBailno = 18; goto memnewbail; } 
					     /* scale_factor, o */
					  iZ =  ScaleToInt(paldThis->pflvData[3]);
					  pacThis->occupancies->o = ValNodeAddInt(NULL, 0, iZ);
					  if (!pacThis->occupancies->o) { iBailno = 19; goto memnewbail; } 
					  pvnOcc = pacThis->occupancies->o;
					  pacThis->occupancies->scale_factor = SAVE_XYZ_SCALE_FACTOR;
				      }
				    if (paldThis->iFloatNo == 4) /* Isotropic Temp Factors */
				      {
					  pitfThis = IsotropicTemperatureFactorsNew();
					  if (!pitfThis) { iBailno = 20; goto memnewbail; } 
					  pacThis->temperature_factors = ValNodeAddPointer(NULL, 
										    AtomicTemperatureFactors_isotropic, 
										    pitfThis);
					  if (!pacThis->temperature_factors) { iBailno = 21; goto memnewbail; } 
					  iZ =  ScaleToInt(paldThis->pflvData[4]);
					  pitfThis->b = ValNodeAddInt(NULL, 0,  iZ);
					  if (!pitfThis->b){ iBailno = 22; goto memnewbail; } 
					  pvnB11 = pitfThis->b;
					  pitfThis->scale_factor = SAVE_XYZ_SCALE_FACTOR;
				      }
				    if (paldThis->iFloatNo == 9) /* Anisotropic Temp Factors */
				      {
					patfThis = AnisotropicTemperatureFactorsNew();
					if (!patfThis) { iBailno = 23; goto memnewbail; } 
					pacThis->temperature_factors = ValNodeAddPointer(NULL, 
										    AtomicTemperatureFactors_anisotropic, 
										    patfThis);
					if (!pacThis->temperature_factors) { iBailno = 24; goto memnewbail; } 
					patfThis->scale_factor = SAVE_XYZ_SCALE_FACTOR;
					iZ = ScaleToInt(paldThis->pflvData[4]);
					patfThis->b_11 = ValNodeAddInt(NULL, 0,  iZ);
					if (!patfThis->b_11) { iBailno = 25; goto memnewbail; } 
					pvnB11 = patfThis->b_11;
					iZ =  ScaleToInt(paldThis->pflvData[5]);
					patfThis->b_12 = ValNodeAddInt(NULL, 0,  iZ);
					if (!patfThis->b_12) { iBailno = 26; goto memnewbail; } 
					pvnB12 = patfThis->b_12;
					iZ =  ScaleToInt(paldThis->pflvData[6]);
					patfThis->b_13 = ValNodeAddInt(NULL, 0,  iZ);
					if (!patfThis->b_13) { iBailno = 27; goto memnewbail; } 
					pvnB13 = patfThis->b_13;
					iZ =  ScaleToInt(paldThis->pflvData[7]);
					patfThis->b_22 = ValNodeAddInt(NULL, 0,  iZ);
					if (!patfThis->b_22) { iBailno = 28; goto memnewbail; } 
					pvnB22 = patfThis->b_22;
					iZ =  ScaleToInt(paldThis->pflvData[8]);
					patfThis->b_23 = ValNodeAddInt(NULL, 0,  iZ);
					if (!patfThis->b_23) { iBailno = 29; goto memnewbail; } 
					pvnB23 = patfThis->b_23;
					iZ =  ScaleToInt(paldThis->pflvData[9]);
					patfThis->b_33 = ValNodeAddInt(NULL, 0,  iZ);
					if (!patfThis->b_33) { iBailno = 30; goto memnewbail; } 
					pvnB33 = patfThis->b_33;
				      }   
				    if (pmldThis->pcAltConf) /* is there an alt-conf string for this model ? */
				      {
				 	pacThis->alternate_conf_ids = ValNodeCopyStr(NULL, 0, STRING_UNK_MOL);
					if (!pacThis->alternate_conf_ids) { iBailno = 31; goto memnewbail; } 
					pcTemp = (CharPtr) pacThis->alternate_conf_ids->data.ptrvalue;
					pcTemp[0] = paldThis->cAltConf;
				        pvnAlt = pacThis->alternate_conf_ids;
				      }
				  }  /* first-case */
				else
				  {
				    /* otherwise the parent pointers are already initialized */
				    /* Allocate new nodes; x,y,z,o,m_ids,r_ids,a_ids,temp(s) */
				    
				    pvnMid->next = ValNodeAddInt(   NULL, 
								    0, 
								    IndexFromNode((PFB)GetParentMol((PFB)pmadThis)));
				    if (!pvnMid->next) { iBailno = 32; goto memnewbail; } 
				    pvnMid = pvnMid->next;
				    pvnRid->next = ValNodeAddInt(   NULL, 
								    0, 
								    IndexFromNode((PFB)GetParentGraph((PFB)pmadThis)));
				    if (!pvnRid->next) { iBailno = 33; goto memnewbail; } 
				    pvnRid = pvnRid->next;
				    pvnAid->next = ValNodeAddInt(   NULL, 
								    0, 
								    IndexFromNode((PFB)pmadThis));
				    if (!pvnAid->next) { iBailno = 34; goto memnewbail; } 
				    pvnAid = pvnAid->next;
				    pvnX->next = ValNodeAddInt(NULL, 0, iX);
				    if (!pvnX->next){ iBailno = 35; goto memnewbail; } 
				    pvnX = pvnX->next;
				    pvnY->next = ValNodeAddInt(NULL, 0, iY);
				    if (!pvnY->next) { iBailno = 36; goto memnewbail; } 
				    pvnY = pvnY->next;
				    pvnZ->next = ValNodeAddInt(NULL, 0, iZ); 
				    if (!pvnZ->next) { iBailno = 37; goto memnewbail; } 
				    pvnZ = pvnZ->next;
				    if (paldThis->iFloatNo == 4)
				      {
					iZ =  ScaleToInt(paldThis->pflvData[4]);
					pvnB11->next = ValNodeAddInt(NULL, 0,  iZ);
					if (!pvnB11->next) { iBailno = 38; goto memnewbail; } 
					pvnB11 = pvnB11->next;
				      }	
				    if (paldThis->iFloatNo == 9)
				      {
					iZ =  ScaleToInt(paldThis->pflvData[4]);
					pvnB11->next = ValNodeAddInt(NULL, 0,  iZ);
					if (!pvnB11->next) { iBailno = 39; goto memnewbail; } 
					pvnB11 = pvnB11->next;
					iZ =  ScaleToInt(paldThis->pflvData[5]);
					pvnB12->next = ValNodeAddInt(NULL, 0,  iZ);
					if (!pvnB12->next) { iBailno = 40; goto memnewbail; } 
					pvnB12 = pvnB12->next;
					iZ =  ScaleToInt(paldThis->pflvData[6]);
					pvnB13->next = ValNodeAddInt(NULL, 0,  iZ);
					if (!pvnB13->next) { iBailno = 41; goto memnewbail; } 
					pvnB13 = pvnB13->next;
					iZ =  ScaleToInt(paldThis->pflvData[7]);
					pvnB22->next = ValNodeAddInt(NULL, 0,  iZ);
					if (!pvnB22->next) { iBailno = 42; goto memnewbail; } 
					pvnB22 = pvnB22->next;
					iZ =  ScaleToInt(paldThis->pflvData[8]);
					pvnB23->next = ValNodeAddInt(NULL, 0,  iZ);
					if (!pvnB23->next) { iBailno = 43; goto memnewbail; } 
					pvnB23 = pvnB23->next;
					iZ =  ScaleToInt(paldThis->pflvData[9]);
					pvnB33->next = ValNodeAddInt(NULL, 0,  iZ);
					if (!pvnB33->next) { iBailno = 44; goto memnewbail; } 
					pvnB33 = pvnB33->next;
				      }
				    if (pvnOcc)
				      {  
					iZ =  ScaleToInt(paldThis->pflvData[3]);
					pvnOcc->next =   ValNodeAddInt(NULL, 0, iZ);
					if (!pvnOcc->next) { iBailno = 45; goto memnewbail; } 
					pvnOcc = pvnOcc->next;
				      }
				    if (pvnAlt)
				      {
					pvnAlt->next = ValNodeCopyStr(NULL, 0, STRING_UNK_MOL);
					if (!pvnAlt->next) { iBailno = 46; goto memnewbail; } 
					pcTemp = (CharPtr) pvnAlt->next->data.ptrvalue;
					pcTemp[0] = paldThis->cAltConf;
				        pvnAlt = pvnAlt->next;
				      }
 				  }  /* continuation of linked-lists */
				  
				  bFirst = FALSE; 
	if (bGotoCalled) goto aldsaveblockreturn;  /* ends the for loop block goto */
				  paldThis = paldThis->next;  /* traverse alt-confs */
			      }  /* while paldThis */
			    pvnmaThis = pvnmaThis->next;
			}   /*atom*/
		    pdnmgThis = pdnmgThis->next;
		}/*graph*/
		pdnmmThis = pdnmmThis->next;
	    }   /*molecule*/

    /* copy COUNTER into appropriate places */
 	    
	    
	pacThis->number_of_points = iCountAtoms;
	pacThis->atoms->number_of_ptrs = iCountAtoms;
   
    skipnext:	 
       pmldThis = pmldThis->next;  /* next coordinate set in same model */
     
     } /* while pmldThis */

return TRUE;
    
    
    errorbail:
#ifdef _DEBUG_5
printf("errorbail out %d - ASN.1 stub mismatch \n ",iBailno );
#endif    
      /* Unrecoverable stub mismach or Modelstruc indexing errors */
      /* fatal */
 	ErrClear(); 
	ErrPostEx(SEV_FATAL,0,0, "Save operation found corrupt data structures\n Error at %d.",iBailno);
	ErrShow();
	  
      return FALSE;
      
    memnewbail:
#ifdef _DEBUG_5
printf("memnewbail out %d - out of memory error \n ", iBailno );
#endif 
      /* Recoverable out of memory error */
      /* clear all ASN data out - must start save process from scratch */
  
  
    FreeRedundantAsn(pdnmsThis);
    ErrClear(); 
    ErrPostEx(SEV_ERROR,0,0, "Out of memory (at %d) while trying to save.\n Try again after closing other windows/programs.",iBailno);
    ErrShow();
    return FALSE;

}


	


Boolean LIBCALL RebuildChemGraphAsn(PDNMS pdnmsThis)
{
   /* looks for kept chemical graps - if not ASN.1 minimum
    * or if it was not kept,  it reconstructs the chemical graph.
    * Upon TRUE,  pdnmsThis' Chemical Graph ASN.1 is repopulated.
    * FASE means it cannot be repopulated.
    */

    Boolean			bOK = TRUE;
    Boolean			bFirstMol = TRUE;
    Boolean			bFirstRes = TRUE;
    Boolean     		bFirstIRB = TRUE;
    Int1    			iDictChoice;
    PMSD 			pmsdThis = NULL;
    PDNMM 			pdnmmThis = NULL;
    PMMD 			pmmdThis = NULL;
    MoleculeGraphPtr 		pmgNew = NULL;
    MoleculeGraphPtr 		pmgLast = NULL;
    ResiduePtr  		prNew = NULL;
    ResiduePtr  		prLast = NULL;
    PDNMG 			pdnmgThis = NULL;
    PMGD  			pmgdThis = NULL;
    BiostrucResidueGraphSetPntrPtr 	pbrgsNew = NULL;
    DbtagPtr 			pdbtNew = NULL;
    PVNMB 			pvnmbThis = NULL;
    PMBD 			pmbdThis = NULL;
    InterResidueBondPtr 	pirbNew = NULL;
    InterResidueBondPtr 	pirbLast = NULL;
	   
    
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    if (!(pmsdThis->bMe == (Byte) AM_MSD)) return FALSE;
    
    /* affix a new chemical graph node to the Biostruc */ 
    pmsdThis->pbsBS->chemical_graph = BiostrucGraphNew();
   
    if (!pmsdThis->pbsBS->chemical_graph) goto allocerr;
    /* re-attach the main descr (ASN optional) */
    pmsdThis->pbsBS->chemical_graph->descr = pmsdThis->pGraphDescr;

    /* re-attach the local dictionary (ASN optional) */
    pmsdThis->pbsBS->chemical_graph->residue_graphs = pmsdThis->pDictLocal;

    /* rebuild molecule_graphs */
    /* here the seq-id's and descr's have been preserved in MMD's */
    pdnmmThis = pmsdThis->pdnmmHead;
    bFirstMol = TRUE;
    while(pdnmmThis)  /* walk the molecule list */
      {
	pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
	pmgNew = MoleculeGraphNew();
	if (!pmgNew) goto allocerr;
	if (bFirstMol) 
	  { 
	    pmsdThis->pbsBS->chemical_graph->molecule_graphs = pmgNew;
	    pmgLast = pmgNew;
	    bFirstMol = FALSE;
	  }
	else
	  {
	    pmgLast->next = pmgNew;
	    pmgLast = pmgNew;
	  }
	   
        /* Fill pmgNew's id, descr, seq_id */ 
	
	pmgNew->id = pdnmmThis->choice;
	pmgNew->descr = pmmdThis->pMolDescr;
	pmgNew->seq_id = pmmdThis->pSeqId;
	
	
	/* residue_sequence */
	
	pdnmgThis = pmmdThis->pdnmgHead; /* top of sequence */
	bFirstRes = TRUE;
	while (pdnmgThis)  /* walk the residues */
	  {
	    pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
	    prNew = ResidueNew();
	    if (!prNew) goto allocerr;
	    if  (bFirstRes)
	      {
		  pmgNew->residue_sequence = prNew;
		  prLast = prNew;
		  bFirstRes = FALSE;
	      }
	    else
	      {
		  prLast->next = prNew;
		  prLast = prNew;
	      }
	    /* Fill in prNew's id, name, residue    */
	        
	    prNew->id = pdnmgThis->choice;
	    prNew->name = StringSave(pmgdThis->pcGraphNum);  /* this gets copied */
	    
	    if (pmgdThis->bWhat & (Byte) DICT_GLOBAL) 
	      {
	         iDictChoice = ResidueGraphPntr_standard;
		 pbrgsNew = BiostrucResidueGraphSetPntrNew();
		 if (!pbrgsNew) goto allocerr;
		 pdbtNew = DbtagNew();
		 if (!pdbtNew) goto allocerr;
		 pdbtNew->db = StringSave(DICT_STANDARD_STRING);
		 pdbtNew->tag = ObjectIdNew();
		 if (!pdbtNew->tag) goto allocerr;
		 pdbtNew->tag->id = DICT_STANDARD_ID;
		 pbrgsNew->biostruc_residue_graph_set_id =  
		 ValNodeAddPointer(NULL, BiostrucId_other_database, pdbtNew);
		 pbrgsNew->residue_graph_id = pmgdThis->iIDict;
		 prNew->residue_graph = ValNodeAddPointer(NULL, iDictChoice, pbrgsNew);
	      }
	    else  
	    if (pmgdThis->bWhat & (Byte) DICT_LOCAL) 
	      {
	         iDictChoice = ResidueGraphPntr_local;
		 prNew->residue_graph = ValNodeAddInt(NULL, iDictChoice, pmgdThis->iIDict);
		 if (!prNew->residue_graph) goto allocerr;
	      }
	    else  
	   /* if (pmgdThis->bWhat & (Byte) DICT_REF)  */
	      {
	      
	            ErrClear(); 
		    ErrPostEx(SEV_FATAL,0,0, "Cannot handle Dictionary \nCannot Reference to Another Biostruc");
		    ErrShow();
		    return FALSE;
	      }
	      
	    pdnmgThis = pdnmgThis->next;
	  }
		
	 /* inter_residue_bonds */
       
        
	pvnmbThis = pmmdThis->pvnmbIRBHead;
	bFirstIRB = TRUE;
	while(pvnmbThis)
	  {
	    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
	    pirbNew = InterResidueBondNew();
	    if (!pirbNew) goto allocerr;
	    if  (bFirstIRB)
	      {
		  pmgNew->inter_residue_bonds = pirbNew;
		  pirbLast = pirbNew;
		  bFirstIRB = FALSE;
	      }
	    else
	      {
		  pirbLast->next = pirbNew;
		  pirbLast = pirbNew;
	      }
	    /* fill in pirbNew's atom_id_1, atom_id_2, bond_order */
	    pirbNew->atom_id_1 = AtomPntrNew();
	    if (! pirbNew->atom_id_1) goto allocerr;
	    pirbNew->atom_id_2 = AtomPntrNew();
	    if (! pirbNew->atom_id_2) goto allocerr;
	    
	    pirbNew->atom_id_1->molecule_id = IndexFromNode((PFB)pmmdThis);
	    pirbNew->atom_id_1->residue_id = IndexFromNode((PFB)GetParentGraph((PFB)pmbdThis->pmadFrom));
	    pirbNew->atom_id_1->atom_id = IndexFromNode((PFB)pmbdThis->pmadFrom);
	    
	    pirbNew->atom_id_2->molecule_id = IndexFromNode((PFB)pmmdThis);
	    pirbNew->atom_id_2->residue_id = IndexFromNode((PFB)GetParentGraph((PFB)pmbdThis->pmadTo));
	    pirbNew->atom_id_2->atom_id = IndexFromNode((PFB)pmbdThis->pmadTo);
	    
	    pirbNew->bond_order = pvnmbThis->choice;  /* bond order held here */
	    
	    pvnmbThis = pvnmbThis->next;
	  }  /* inter-residue bonds */	 	
	
	pdnmmThis = pdnmmThis->next;
      }  /* molecule */
         
   /* rebuild inter_molecule_bonds */ 	
	
   pvnmbThis = pmsdThis->pvnmbIMBHead;
   bFirstIRB = TRUE;
   while(pvnmbThis)
      {
        pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
        pirbNew = InterResidueBondNew();
        if (!pirbNew) goto allocerr;
        if  (bFirstIRB)
          {
    	    pmsdThis->pbsBS->chemical_graph->inter_molecule_bonds = pirbNew;
    	    pirbLast = pirbNew;
    	    bFirstIRB = FALSE;
          }
        else
          {
    	    pirbLast->next = pirbNew;
    	    pirbLast = pirbNew;
          }
        /* fill in pirbNew's atom_id_1, atom_id_2, bond_order */
        pirbNew->atom_id_1 = AtomPntrNew();
        if (! pirbNew->atom_id_1) goto allocerr;
        pirbNew->atom_id_2 = AtomPntrNew();
        if (! pirbNew->atom_id_2) goto allocerr;
        
        pirbNew->atom_id_1->molecule_id = IndexFromNode((PFB)GetParentMol((PFB)pmbdThis->pmadFrom));
        pirbNew->atom_id_1->residue_id = IndexFromNode((PFB)GetParentGraph((PFB)pmbdThis->pmadFrom));
        pirbNew->atom_id_1->atom_id = IndexFromNode((PFB)pmbdThis->pmadFrom);
        
        pirbNew->atom_id_2->molecule_id = IndexFromNode((PFB)GetParentMol((PFB)pmbdThis->pmadTo));
        pirbNew->atom_id_2->residue_id = IndexFromNode((PFB)GetParentGraph((PFB)pmbdThis->pmadTo));
        pirbNew->atom_id_2->atom_id = IndexFromNode((PFB)pmbdThis->pmadTo);
        
        pirbNew->bond_order = pvnmbThis->choice;  /* bond order held here */
        
        pvnmbThis = pvnmbThis->next;
    }  /* inter-molecule bonds */	 
  
   
   if (pmsdThis->pbsBS->chemical_graph) return TRUE;

  allocerr: 
    /* Recoverable out of memory error */
      /* clear all ASN data out - must start save process from scratch */
    FreeRedundantAsn(pdnmsThis);
    ErrClear(); 
    ErrPostEx(SEV_ERROR,0,0, "Out of memory while trying to save.\n Try again after closing other windows/programs.");
    ErrShow();
    return FALSE;
  
}




Boolean LIBCALL WriteOutBiostruc(BiostrucPtr pbsThis, CharPtr pcSave,  Byte bSave)
{   

    AsnIoPtr paioAIP = NULL;
    Boolean bWriteOK = FALSE;
    CharPtr pcSaveMode = NULL;
    BiostrucFeatureSetPtr pfsThis = NULL; 

    if (!pcSave) return FALSE;
    if (!pbsThis) return FALSE;    
    if (bSave & (Byte) SAVE_BINARY) pcSaveMode = SAVE_BINARY_STRING; 
    else pcSaveMode = SAVE_ASCII_STRING;  
  
    if (bSave & (Byte) NOT_FEATURES)
      {
        pfsThis = pbsThis->features;
	pbsThis->features = NULL;
      } /* unlink features */
   
    paioAIP = AsnIoOpen(pcSave, pcSaveMode);  
    bWriteOK = BiostrucAsnWrite(pbsThis, paioAIP, NULL);
    AsnIoClose (paioAIP); 
   
    if (bSave & (Byte) NOT_FEATURES)
     {
       pbsThis->features = pfsThis;
     } /* re-link features */
      	
    return bWriteOK;
}


Boolean LIBCALL WriteASNChemGraphOnly(PDNMS pdnmsThis, CharPtr pcSave,  Byte bSave)
{
    /* writes out chemical graph only - models are not destroyed */
    AsnIoPtr paioAIP = NULL;
    Boolean bWriteOK = FALSE;
    CharPtr pcSaveMode = NULL;
    Int4 iHash = 0;
    BiostrucFeatureSetPtr pfsThis = NULL; 
    BiostrucModelPtr pbsmThis = NULL;
    PMSD pmsdThis = NULL;
    
    if (!pdnmsThis) return FALSE;
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
      
   if (RebuildChemGraphAsn(pdnmsThis) == FALSE)
     {
	bWriteOK = FALSE;
	goto seeyalater;
     }
#ifdef _DEBUG_5
printf("Chem Graph rebuilt \n");
#endif  
    
    if (bSave & (Byte) SAVE_BINARY) pcSaveMode = SAVE_BINARY_STRING; 
    else pcSaveMode = SAVE_ASCII_STRING;  
      
    if (bSave & (Byte) NOT_FEATURES)
      {
        pfsThis = pmsdThis->pbsBS->features;
	pmsdThis->pbsBS->features = NULL;
      } /* unlink features */
   
   
    pbsmThis = pmsdThis->pbsBS->model;
    pmsdThis->pbsBS->model = NULL;  
    /* unlink models */
   
    paioAIP = AsnIoOpen(pcSave, pcSaveMode);  
    bWriteOK = BiostrucAsnWrite(pmsdThis->pbsBS, paioAIP, NULL);
    AsnIoClose (paioAIP); 
   
    if (bSave & (Byte) NOT_FEATURES)
     {
       pmsdThis->pbsBS->features = pfsThis;
     } /* re-link features */
   
   
    pmsdThis->pbsBS->model = pbsmThis;
    /* relink models */
  seeyalater:    
    FreeRedundantAsn(pdnmsThis);
    /* remove the chem graph */
   
    return bWriteOK;
    
}




Boolean LIBCALL WriteAsnLocalDict(PDNMS pdnmsThis, CharPtr pcSave,  Byte bSave,   Boolean SaveId)
{
    CharPtr pcSaveMode = NULL;
    BiostrucResidueGraphSetPtr pbrgsDict = NULL;
    PMSD pmsdThis = NULL;
    AsnIoPtr paioAIP = NULL;
    Boolean bRet;
    
    if (bSave & (Byte) SAVE_BINARY) pcSaveMode = SAVE_BINARY_STRING; 
    else pcSaveMode = SAVE_ASCII_STRING;  
            
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    if (pmsdThis->bMe == (Byte) AM_MSD)
	if (pmsdThis->pDictLocal)
	    {
	      pbrgsDict =  BiostrucResidueGraphSetNew();
	      if (!pbrgsDict) return FALSE;
	      pbrgsDict->residue_graphs = pmsdThis->pDictLocal; /* attach dict */
	      if (SaveId) pbrgsDict->id = pmsdThis->pbsBS->id;  /* point at id */
	      paioAIP = AsnIoOpen(pcSave, pcSaveMode); 
	      bRet = BiostrucResidueGraphSetAsnWrite(pbrgsDict, paioAIP, NULL);
	      AsnIoClose(paioAIP);
	      pbrgsDict->residue_graphs = NULL;  /* detach dict */
	      pbrgsDict->id = NULL; /* detach id */
	      BiostrucResidueGraphSetFree(pbrgsDict);
	      return bRet;
	    }
    return FALSE;	
}


static void LIBCALLBACK FreeModelChemGraphLoc(PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr)
{   /* callback for FreeSingleModel - frees the model-specific data from a chem-graph node */
 
   PMAD pmadThis = NULL;
   PALD paldThis = NULL;
   PMGD pmgdThis = NULL;
   PMMD pmmdThis = NULL;
   PVNAL pvnalThis = NULL;
   ValNodePtr *ppvn  = NULL;
   ValNodePtr pvnKillList = NULL;

   
   if (pfbThis->bMe == (Byte) AM_MAD)
     {  /* iIndex & ptr not used */
	pmadThis = (PMAD) pfbThis;
	/* look up the cointainedBy list for model ocurrences */
	paldThis = GetAtomLocs(pmadThis, iModel); 
	if (paldThis) 
 	  {
	    pvnalThis = paldThis->pvnalLink;
 	    FreeALD(paldThis);
	    pvnalThis->data.ptrvalue = NULL;
	    pvnalThis = ValNodeExtract(&pmadThis->pvnalLocate, iModel); 
	    if (pvnalThis) ValNodeFree(pvnalThis);
	  }
	ppvn = &pmadThis->pvnContainedBy;
     }  
     
   if (pfbThis->bMe == (Byte) AM_MGD)
        
     {
	 /* kill the pvnContainedBy entries for this model in choice field */
	 pmgdThis = (PMGD) pfbThis;
	 ppvn = &pmgdThis->pvnContainedBy;
     }
 
   if (pfbThis->bMe == (Byte) AM_MMD)
     {
	 /* kill the pvnContainedBy entries for this model in choice field */
	 pmmdThis = (PMMD) pfbThis;
	 ppvn = &pmmdThis->pvnContainedBy;
     }
   if (!ppvn) return;  
   pvnKillList = ValNodeExtractList(ppvn, iModel);
   if (pvnKillList) ValNodeFree(pvnKillList);  
  
}

Boolean LIBCALL FreeSingleModel(PDNMS pdnmsThis, Int2 iModel)
{
    PMLD pmldThis = NULL;
    PMLD pmldTemp = NULL;
    PMSD pmsdThis = NULL;
    PDNML pdnmlThis = NULL;
    PDNML pdnmlTemp = NULL;
    BiostrucModelPtr pbsmTemp = NULL;
    BiostrucModelPtr pbsmLast = NULL;
    BiostrucModelPtr pbsmHead = NULL;
    ValNodePtr pvn = NULL;
    PDNML pdnml = NULL;
    
    if (!pdnmsThis) return FALSE;
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    
  
   pdnmlTemp = pmsdThis->pdnmlModels;
   while(pdnmlTemp)
     {
        if (pdnmlTemp->choice == iModel) 
	    break;
	pdnmlTemp = pdnmlTemp->next;
     }
     
   if (!pdnmlTemp) 
     {
#ifdef _DEBUG_5
printf("Cannot find iModel %d to delete it\n",(int) iModel);
#endif  
       return FALSE;
     }
   
   pdnmlThis = pdnmlTemp;
   pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
    
	/* call a traverser callback which removes the model's pvnal lists */
	/* and pvnContainedBy instances from MMD, MGD and MAD. */
#ifdef _DEBUG_5
printf("Calling Traverser iModel %d\n",(int) iModel);
#endif  

    TraverseOneModel(pdnmsThis, TRAVERSE_ALL, 
			iModel, 0,   NULL, (pNodeFunc)FreeModelChemGraphLoc);

       /* unlink all the models' ASN.1 portions */
#ifdef _DEBUG_5
printf("Traverser Done\n");
#endif  
       
    pdnmlTemp =  pmsdThis->pdnmlModels;
    while (pdnmlTemp)  
      { 
	 pmldTemp = (PMLD) pdnmlTemp->data.ptrvalue;
	 if (pmldTemp->pbsmModel)
	    pmldTemp->pbsmModel->next = NULL;  /* unlinked */	   
	 else
	   {
	    ErrClear(); 
	    ErrPostEx(SEV_ERROR,0,0, "Model Lacks ASN.1 Stub\n pmldTemp->pbsmModel");
	    ErrShow();
	    return FALSE;
	   }
	 pdnmlTemp = pdnmlTemp->next;
      }	
      
	/* free the Biostruc model stub off first node's pdnmlThis>>pmldThis->pbsmModel */
	/* these are now all detached from each other and can be deleted */
	/*   without causing a chain reaction */
#ifdef _DEBUG_5
printf("Freeing Biostruc Models\n");
#endif  

    if (pmldThis->pbsmModel) BiostrucModelFree(pmldThis->pbsmModel);
    pmldThis->pbsmModel = NULL;		
   
	 /* extract the MOD's and MDD's from the lists off of pmsdThis and free them */
    pvn = ValNodeExtractList(&pmsdThis->pvnmoHead, iModel);
    if (pvn) FreeListVNMO((PVNMO) pvn);
    pmsdThis->iObjCount = 0;
    pvn = pmsdThis->pvnmoHead;
    while (pvn)
      {
	  (pmsdThis->iObjCount)++;
	  pvn = pvn->next;
      }
    
    pvn = NULL;
    pvn = ValNodeExtractList(&pmsdThis->pvnmdHead, iModel);
    if (pvn) FreeListVNMD((PVNMD) pvn);
    pmsdThis->iDensCount = 0;
    pvn = pmsdThis->pvnmdHead;
    while (pvn)
      {
	  (pmsdThis->iDensCount)++;
	  pvn = pvn->next;
      }
 
   /* free the chain of mld's off of pdnmlThis */
  
  
	/* unhook the structure model node to delete */
#ifdef _DEBUG_5 
        pdnml  = pmsdThis->pdnmlModels;
 	 while (pdnml )
	  {
 	    printf("Before found model %d\n", (int) pdnml->choice);
	    pdnml = pdnml->next;
	  }
#endif	    

    pdnmlThis = DValNodeExtract(&pmsdThis->pdnmlModels, iModel);

#ifdef _DEBUG_5 
        pdnml  = pmsdThis->pdnmlModels;
 	 while (pdnml )
	  {
 	    printf(" After Unlink - found model %d\n", (int) pdnml->choice);
	    pdnml = pdnml->next;
	  }
#endif	  
  
#ifdef _DEBUG_5
printf("Freeing MLD's\n");
#endif  

   if (pdnmlThis) FreeListDNML(pdnmlThis);

#ifdef _DEBUG_5
printf("MLDs freed\n");
#endif      

 
#ifdef _DEBUG_5
printf("Re-linking pbsmModels\n");
#endif  

    	/* re-link the remaining list of pbsmModel's... */
	
    pdnmlTemp = pmsdThis->pdnmlModels;
    while (pdnmlTemp)  /* link the model list ASN.1 portions */
      { 
	 pmldThis = (PMLD) pdnmlTemp->data.ptrvalue;
         if (!pbsmHead) pbsmHead = pmldThis->pbsmModel; /* sets the first one */
	 if (pbsmLast) pbsmLast->next = pmldThis->pbsmModel; /* point to this one */
	 
	 pbsmLast = pmldThis->pbsmModel;
	 pdnmlTemp = pdnmlTemp->next;
      }	
   pmsdThis->pbsBS->model = pbsmHead;  /* reset the Biostruc main model pointer to new list */
   pmsdThis->iModels--;
   return TRUE;
}



Boolean LIBCALL WriteAsnModelList(PDNMS pdnmsThis,   Int2 iNumModels,  Int2Ptr i2Vec,  
				    CharPtr pcSave,  Byte bSave, Boolean iCn3d)
{
    /* The master ASN.1 writer routine */
    /* MODELS not specified for writing are ERASED for congruency */
    /* the User-interface needs to notify the user of this if some models are not saved */
    /* takes a vector containing model numbers as input */
    /* one can save any arbitrary collection of models */
    /* or one can save no models - just features, or dictionaries */
    /* use bSave Byte to  pick binary/ascii modes */
   
    Int2 iIndex = 0;
    Int2 iTest = 0;
    Boolean bFirst = TRUE;
    BiostrucPtr pBSThis = NULL;
    BiostrucModelPtr pbsmThis = NULL;
    PMSD pmsdThis = NULL;
    PDNML pdnmlThis = NULL;
    ValNodePtr pvnDelList = NULL;
    ValNodePtr pvnDelTemp = NULL;
    PDNML pdnmlTemp = NULL;
    PMLD  pmldThis = NULL;
    Boolean bRet;
    ValNodePtr pvn = NULL;  
    AsnIoPtr paioAIP = NULL;
    Int4 iHash = 0;
    Boolean bFound;
   
 
 /*FIRST-PASS VALIDATION*/
    
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    if (!(pmsdThis->bMe == (Byte) AM_MSD)) return FALSE;
    /* A pbsBS must exist or be created! */
    if (!(pmsdThis->pbsBS))
      {
          ErrClear(); 
	  ErrPostEx(SEV_ERROR,0,0, "Modelstruc Lacks Biostruc ASN.1 Stub\n pmsdThis->pbsBS");
	  ErrShow();
	  return FALSE;
      } 
   
  
    
 /*CHEMICAL-GRAPH*/
    
    if (RebuildChemGraphAsn(pdnmsThis) == FALSE)
      {
	bRet = FALSE;
#ifdef _DEBUG_5
printf("Chem Graph rebuild failed \n");
#endif
        goto outofhere;
      } 
    
#ifdef _DEBUG_5 
printf("Chem Graph rebuilt \n");
#endif    
    


/*MODELS*/   
      
  
    /* walk through models, free the ones not on the list (ALD, MLD, and ASN stubs) */
    pdnmlTemp = pmsdThis->pdnmlModels;
 
    while(pdnmlTemp)
      {
          bFound = FALSE;
	  for (iIndex = 0; iIndex < iNumModels; iIndex++)
	     {
		 if ((Int2)pdnmlTemp->choice == i2Vec[iIndex])
		   {
		     bFound = TRUE;
		   }  
	     }
	  if (!bFound) pvnDelTemp = ValNodeAddPointer(&pvnDelTemp, 0, (VoidPtr) pdnmlTemp); 
	  if (!pvnDelList) pvnDelList = pvnDelTemp;  /* set the head pointer */
	  pdnmlTemp = pdnmlTemp->next;  
      }
    
    pvnDelTemp = pvnDelList;
    while(pvnDelTemp)
     {
        pdnmlTemp = (PDNML) pvnDelTemp->data.ptrvalue;
	if (FreeSingleModel(pdnmsThis, (Int2)pdnmlTemp->choice) == FALSE)
	  {
	     ErrClear(); 
	     ErrPostEx(SEV_FATAL,0,0, "Model Not Correctly Freed. Possibly Corrupt Memory Data\n");
	     ErrShow();
	     goto outofhere;
	  }
	pvnDelTemp = pvnDelTemp->next; 
     }
   
   if (pvnDelList) ValNodeFree(pvnDelList);
   
    /* now pmsdThis->pbsBS->model contains only the models for saving */   
    for (iIndex = 0; iIndex < iNumModels; iIndex++)
       {  
        /* for each model, refresh or rebuild its data */
#ifdef _DEBUG_5
printf("Refreshing Model %d\n ", (int) i2Vec[iIndex]);
#endif
  	
	 if (RefreshModelAsnMem(pdnmsThis, i2Vec[iIndex]) == FALSE)
	   {
 	     bRet = FALSE;
#ifdef _DEBUG_5
printf("Refreshing Model failed %d\n ", (int) i2Vec[iIndex]);
#endif   
  		     goto outofhere;
	   } 
       
       }
      
       
    /* it is up to the bSave parameter to specify whether there is a new ID to  */
    /* be hashed automatically */
 
     

    if (bSave & (Byte) NEW_MMDB_ID) 
      {
        iHash = MakeHashChange(pdnmsThis);
	if (iHash != pmsdThis->iHashChange)
	  { 
	      pmsdThis->iMMDBid = iHash;
	      pvn = pmsdThis->pbsBS->id;
	      pmsdThis->pbsBS->id = NULL;
	      pmsdThis->pbsBS->id = ValNodeAddInt(&pmsdThis->pbsBS->id, BiostrucId_mmdb_id, iHash);
	      if (pvn) ValNodeFree(pvn);
	  }
      }
     
     if(iCn3d){
        if(pmsdThis->pbsBS) bRet = TRUE;
        return bRet;
               /* leave cn3d handle FreeRedundantAsn - yanli */
     }

     bRet = WriteOutBiostruc(pmsdThis->pbsBS, pcSave, bSave);

#ifdef _DEBUG_5
if (!bRet) printf("WriteOutBiostruc Failed\n");
#endif    
  	   
  outofhere:
     FreeRedundantAsn(pdnmsThis);
     
     return bRet;
   
}


Boolean LIBCALL WriteAsnOneModel(PDNMS pdnmsThis,  Int2 iModel,  CharPtr pcSave,  
		Byte bSave)
{
   /* call to save a single model */

    Boolean bRet;
    PMSD pmsdThis = NULL;
    
    pmsdThis = (PMSD)  PFBFromDN(pdnmsThis);
    if (!(pmsdThis->bMe == (Byte) AM_MSD)) return FALSE;
   
    bRet = WriteAsnModelList(pdnmsThis,  1, (Int2Ptr) &iModel, 
				pcSave, bSave, 0);
    return bRet;
}

Boolean LIBCALL WriteAsnAllModel(PDNMS pdnmsThis,  CharPtr pcSave,  Byte bSave)
{

    Int2Ptr i2Vec = NULL;
    PDNML pdnmlThis = NULL;
    PMSD pmsdThis = NULL;
    PMLD pmldThis = NULL;
    Int2 iCount = 0;
    Boolean bRet = FALSE;
 
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    if (!(pmsdThis->bMe == (Byte) AM_MSD)) return FALSE;
    
    pdnmlThis = pmsdThis->pdnmlModels; /* the linked-list of models */
    if (pdnmlThis)
      while (pdnmlThis)
       {
	 pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
	 if (pmldThis)
	   {
	        iCount++;
	   } 
         pdnmlThis = pdnmlThis->next;
       }
    else return bRet;  /* no models */
    i2Vec = I2Vector(0, iCount);  /* allocate vector */
    if (!i2Vec) return bRet; 
    iCount = 0;
    pdnmlThis = pmsdThis->pdnmlModels;
    while (pdnmlThis)
	{  /* copy the model id's into the vector */
 	  pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
	  if (pmldThis)
	   {
	     i2Vec[iCount] = pdnmlThis->choice;
	     iCount++;
	   }
	  pdnmlThis = pdnmlThis->next;
	}
    bRet = WriteAsnModelList(pdnmsThis, iCount, i2Vec, pcSave, bSave, 0);
    if (i2Vec) I2VectorFree(i2Vec, 0);  /* free vector */
    return bRet;	  
}




SeqIdPtr MakePDBSeqId2 (CharPtr pcPDB, Char cChain, int iDomain, Boolean getgi)
{
  SeqIdPtr        sip;
  PDBSeqIdPtr     psip;
  Int4 gi;

  psip = PDBSeqIdNew ();

  psip->chain = cChain;
  psip->rel = NULL;
  psip->mol = StringSave (pcPDB);
  sip = ValNodeNew (NULL);
  sip->choice = SEQID_PDB;
  sip->data.ptrvalue = psip;

  if (getgi)
  {
    if ((gi = GetGIForSeqId (sip)) > 0) {
      SeqIdFree (sip);
      sip = ValNodeNew (NULL);
      sip->choice = SEQID_GI;
      sip->data.intvalue = gi;
    }
  }

  return sip;
}

/* produce a sequence alignment from a structure-alignment */
SeqAnnotPtr LIBCALL BiostrucAnnotSetToSeqAnnot (BiostrucAnnotSetPtr set, Boolean usePValue)
{
  SeqAnnotPtr     sap = NULL;
  SeqAlignPtr     salp;
  ScorePtr        scp;
  Int2 count;
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

  for (feature = set->features->features; feature != NULL; feature = feature->next) {

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
    
    for (nextmasterstart = 1, nextslavestart = 1, count = 0, masterseg = master, slaveseg = slave; masterseg != NULL && slaveseg != NULL; masterseg = masterseg->next, slaveseg = slaveseg->next) {
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

  }

  SeqIdFree (mastersip);
  SeqIdFree (pdbsip);
  return sap;
}
