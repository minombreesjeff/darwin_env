/************************************************************/
/*							    */
/*	prunebiostruc.c 				    */
/*							    */
/*	pulls out a single biopolymer chain from a complete */
/*	biostruc object  and writes it out to a file.       */
/*	Kenneth J. Addess August 25, 1998		    */
/************************************************************/ 
/*
* $Log: prunebsc.c,v $
* Revision 6.2  1999/11/06 15:56:15  addess
* got rid of two useless lines of code
*
* Revision 6.1  1999/05/07 20:01:09  kans
* renaming prunebiostruc to prunebsc
*
* Revision 6.4  1998/12/01 15:13:47  addess
* cleaned up code to remove memory leaks
*
 * Revision 6.3  1998/11/28  20:43:53  addess
 * clean up some code that leaked memory
 *
 * Revision 6.2  1998/10/14  17:33:52  addess
 * *** empty log message ***
 *
*/

#include "prunebsc.h"
	
/*static void My_StringNCpy(CharPtr str1, CharPtr str2, Int4 len)
{
  StringNCpy(str1,str2,len);
  str1[len] = '\0';
}*/ 


/*static Boolean IsLayerOne(ValNodePtr pvn)
{
  Boolean Layer = FALSE;
  CharPtr comment_txt;
  CharPtr LayerOne = {"This is Layer 1 Release"};
  CharPtr temp_string;
  Int4 len;
  
  while (pvn)
  {
     if (pvn->choice == BiostrucDescr_pdb_comment)
     {
        comment_txt = (CharPtr)pvn->data.ptrvalue;
        temp_string = StringSave(LayerOne);
        len = StringLen(LayerOne);
        My_StringNCpy(temp_string, &comment_txt[18], len);
        if (!StringICmp(temp_string, LayerOne))
        {
          Layer = TRUE;
          break;
        }
     }
     pvn = pvn->next;
   }
   
   return Layer;
}
*/

/*static Int4 NumberOfBioChains(MoleculeGraphPtr mgp)
{
  ValNodePtr vnp;
  Int4 mtype, nbp = 0;

  while (mgp != NULL)
  {
      vnp = ValNodeFindNext(mgp->descr, NULL, BiomolDescr_molecule_type);

      if (vnp) mtype = vnp->data.intvalue;

     switch(mtype)
     {
	 case 1:
	 case 2:
	 case 3:
	    nbp++;
	    break;
     }
        
      mgp = mgp->next;
  }
  
  return nbp;
}*/

static MoleculeGraphPtr MakeBioGraphPtr(MoleculeGraphPtr mgp)
{
  MoleculeGraphPtr newbp, bp = NULL, currentbp;
  ValNodePtr vnp;
  Int4 mtype;

  while (mgp != NULL)
   {
      vnp = ValNodeFindNext(mgp->descr, NULL, BiomolDescr_molecule_type);

      if (vnp) mtype = vnp->data.intvalue;

      switch(mtype)
      {
	 case 1:
	 case 2:
	 case 3:
	    newbp = MoleculeGraphNew();
	    newbp->id = mgp->id;
	    newbp->descr = mgp->descr;
	    newbp->seq_id = mgp->seq_id;
            newbp->residue_sequence = mgp->residue_sequence;
	    newbp->inter_residue_bonds = mgp->inter_residue_bonds;
	    if (bp == NULL)
	    {
	       bp = newbp;
            }
	    else
	    {
	       currentbp->next = newbp;
            }
	    currentbp = newbp;
	    break;
      }

      mgp = mgp->next;
   }
   
  return bp;
}

/*static Int4 NumberOfHetChains(MoleculeGraphPtr mgp, MoleculeGraphPtr bp)
{
  ValNodePtr vnp;
  Int4 mtype, molecule_id, nhet = 0;
  CharPtr mname;

  while (mgp != NULL)
  {
    vnp = ValNodeFindNext(mgp->descr, NULL, BiomolDescr_molecule_type);

    if (vnp) mtype = vnp->data.intvalue;

    switch(mtype)
    {
      case 6:
      if (vnp = ValNodeFindNext(mgp->descr, NULL, BiomolDescr_name))
      mname = vnp->data.ptrvalue;
      molecule_id = atoi(mname);
	    
      if (isBiopoly(molecule_id, bp))
      {
        nhet++;
      }
      break;
    }
   mgp = mgp->next;
  }
  return nhet;
}
*/
static MoleculeGraphPtr MakeHetGraphPtr(MoleculeGraphPtr mgp, MoleculeGraphPtr bp)
{
  MoleculeGraphPtr newhet = NULL, het = NULL, currenthet;
  ValNodePtr vnp;
  Int4 mtype, molecule_id;
  CharPtr mname;

  while (mgp != NULL)
  {
      vnp = ValNodeFindNext(mgp->descr, NULL, BiomolDescr_molecule_type);

      if (vnp) mtype = vnp->data.intvalue;

      switch(mtype)
      {
        case 6:
	  vnp = ValNodeFindNext(mgp->descr, NULL, BiomolDescr_name);

	  if (vnp) mname = vnp->data.ptrvalue;
		  
          molecule_id = atoi(mname);
	  if (isBiopoly(molecule_id, bp))
	  {
	    newhet = MoleculeGraphNew();
	    newhet->id = mgp->id;
	    newhet->descr = mgp->descr;
	    newhet->seq_id = mgp->seq_id;
            newhet->residue_sequence = mgp->residue_sequence;
	    newhet->inter_residue_bonds = mgp->inter_residue_bonds;
	    if (het == NULL)
	    {
	      het = newhet;
            }
	    else
	    {
	      currenthet->next = newhet;
            }
	    currenthet = newhet;
	  }
	  break;
      }

   mgp = mgp->next;
  }
   
  return het;
}

static BiostrucFeatureSetPtr PruneFeatureSet(BiostrucFeatureSetPtr bsfsp, Int4 id)
{
  BiostrucFeatureSetPtr pbsfs;
  BiostrucFeaturePtr bsfpDie, bsfp, bsfpnew = NULL;
  Int4 i, j, cnt, cnt2, zero;
  ValNodePtr vnp;
  ResidueExplicitPntrsPtr repp;
  ResidueIntervalPntrPtr ripp,current;
  ResiduePntrsPtr rpp;
  AtomPntrsPtr app;
  ChemGraphPntrsPtr cgpp;
   
  pbsfs = BiostrucFeatureSetNew();
  pbsfs->id = bsfsp->id;
  bsfsp->id = 0;
  pbsfs->descr = bsfsp->descr;
  pbsfs->features = NULL;
  bsfp = bsfsp->features;
  
  while (bsfp)
  {
    cgpp = (ChemGraphPntrsPtr)bsfp->Location_location->data.ptrvalue;
    rpp = (ResiduePntrsPtr)cgpp->data.ptrvalue;
    ripp = (ResidueIntervalPntrPtr)rpp->data.ptrvalue;
    
    if (ripp->molecule_id == id)
    {
      if (pbsfs->features == NULL)
      {
        pbsfs->features = bsfp;
        bsfpnew = pbsfs->features;
        bsfp = bsfp->next;
        bsfpnew->next = NULL;
      }
      else
      {
        bsfpnew->next = bsfp;
        bsfpnew = bsfp;
        bsfp = bsfp->next;
        bsfpnew->next = NULL;
      }
    }
    else
    {
      bsfpDie = bsfp;
      bsfp = bsfp->next;
      bsfpDie = NULL;
    }
  }
  return pbsfs;
}

static BiostrucModelPtr PruneModel(BiostrucModelPtr bsmp, ValNodePtr pvnIds, Int4 id)
{
  BiostrucModelPtr bsmp2;
  ValNodePtr pvnCoordinates, pvnLiteral, pvnCoordinatesNew, pvnLiteralNew; 
  ValNodePtr pvnMolId, pvnResId, pvnAtmId, pvnId;
  ValNodePtr pvnX, pvnY, pvnZ; 
  ValNodePtr prev = NULL, current = NULL; 
  ModelCoordinateSetPtr mcs, mcsDie, mcsnew = NULL;
  SurfaceCoordinatesPtr scp;
  AtomicCoordinatesPtr acp, acpnew;
  ResidueIntervalPntrPtr ripp;
  ResiduePntrsPtr rpp;
  AtomPntrsPtr atmp;
  ModelSpacePointsPtr stmp;
  Int4 numatoms, index, new_numatoms = 0;
  Boolean *molidx, found;
 
  bsmp2 = BiostrucModelNew();
  bsmp2->id = bsmp->id;
  bsmp2->type = bsmp->type;
  bsmp2->descr = bsmp->descr;
  bsmp->descr = NULL;
  bsmp2->model_space = bsmp->model_space;
  bsmp->model_space = NULL;
  bsmp2->model_coordinates = NULL;
  
  mcs = bsmp->model_coordinates;
    
  while (mcs)
  {
    if (mcs->descr)
    {
      pvnCoordinates = mcs->Coordinates_coordinates;
      pvnLiteral = pvnCoordinates->data.ptrvalue;
      scp = (SurfaceCoordinatesPtr)pvnLiteral->data.ptrvalue;
      rpp = (ResiduePntrsPtr)scp->contents->data.ptrvalue;
      ripp = (ResidueIntervalPntrPtr)rpp->data.ptrvalue; 
      
      if (ripp->molecule_id == id)
      {
        if (!bsmp2->model_coordinates)
        {
          bsmp2->model_coordinates = mcs;
          mcsnew = bsmp2->model_coordinates;
          mcs = mcs->next;
          mcsnew->next = NULL;
        }
        else
        {
          mcsnew->next = mcs;
          mcsnew = mcs;
          mcs = mcs->next;
          mcsnew->next = NULL;
        }
      }
      else
      {
        mcsDie = mcs;
        mcs = mcs->next;
        mcsDie = NULL;
      }
    }
    else
    {
      pvnCoordinates = mcs->Coordinates_coordinates;
      pvnLiteral = pvnCoordinates->data.ptrvalue;
      acp = (AtomicCoordinatesPtr)pvnLiteral->data.ptrvalue;
      atmp = acp->atoms;
      stmp = acp->sites;
      numatoms = atmp->number_of_ptrs;
      molidx = (Boolean *)MemNew(numatoms * sizeof(Boolean));
     
      for (index = 0, pvnMolId = atmp->molecule_ids;
           pvnMolId != NULL; index++, pvnMolId = pvnMolId->next)
      {
        pvnId = pvnIds;
        found = FALSE;
        while (pvnId)
        {
          if (pvnMolId->data.intvalue == pvnId->data.intvalue)
          { 
            found = TRUE;
            break;
          }
          pvnId = pvnId->next;
        }
        if (found)
        {   
          molidx[index] = TRUE;
          new_numatoms++;
        }
          else molidx[index] = FALSE;
      } 
      
      acpnew = AtomicCoordinatesNew();
      acpnew->number_of_points = new_numatoms;
      acpnew->atoms = AtomPntrsNew();
      acpnew->atoms->number_of_ptrs = new_numatoms;
      pvnMolId = atmp->molecule_ids;
      for (index = 0; index < numatoms; index++)
      {
        if (molidx[index] == TRUE)
        { 
          current = pvnMolId;
          pvnMolId = pvnMolId->next;
          current->next = NULL;
        }
        else
        {
          pvnMolId = pvnMolId->next;
          continue;
        }  
        if (acpnew->atoms->molecule_ids == NULL)
	  acpnew->atoms->molecule_ids = current;
        else
	  prev->next = current;
        prev = current;
      }
      prev = NULL;
      current = NULL;
      pvnResId = atmp->residue_ids;
      for (index = 0; index < numatoms; index++)
      {
        if (molidx[index] == TRUE)
        { 
          current = pvnResId;
          pvnResId = pvnResId->next;
          current->next = NULL;
        }
        else
        {
          pvnResId = pvnResId->next;
          continue;
        }       
        if (acpnew->atoms->residue_ids == NULL)
	  acpnew->atoms->residue_ids = current;
        else
	  prev->next = current;
        prev = current;
      }
      prev = NULL;
      current = NULL;
      pvnAtmId = atmp->atom_ids;
      for (index = 0; index < numatoms; index++)
      {
        if (molidx[index] == TRUE)
        {
          current = pvnAtmId;
          pvnAtmId = pvnAtmId->next;
          current->next = NULL;
        }
        else
        {
          pvnAtmId = pvnAtmId->next;
          continue;
        }
        if (acpnew->atoms->atom_ids == NULL)
	  acpnew->atoms->atom_ids = current;
        else
	  prev->next = current;
        prev = current;
      }
      prev = NULL;
      current = NULL;
      acpnew->sites = ModelSpacePointsNew();
      acpnew->sites->scale_factor = 1000;
      pvnX = stmp->x;
      for (index = 0; index < numatoms; index++)
      {
        if (molidx[index] == TRUE)
        { 
          current = pvnX;
          pvnX = pvnX->next;
          current->next = NULL;
        }
        else
        {
          pvnX = pvnX->next;
          continue;
        }
        if (acpnew->sites->x == NULL)
	  acpnew->sites->x = current;
        else
	  prev->next = current;
        prev = current;
      } 
      prev = NULL;
      current = NULL;
      pvnY = stmp->y;
      for (index = 0; index < numatoms; index++)
      {
        if (molidx[index] == TRUE)
        { 
          current = pvnY;
          pvnY = pvnY->next;
          current->next = NULL;
        }
        else
        {
          pvnY = pvnY->next;
          continue;
        }
        if (acpnew->sites->y == NULL)
	  acpnew->sites->y = current;
        else
	  prev->next = current;
        prev = current;
      } 
      prev = NULL;
      current = NULL;
      pvnZ = stmp->z;
      for (index = 0; index < numatoms; index++)
      {
        if (molidx[index] == TRUE)
        { 
          current = pvnZ;
          pvnZ = pvnZ->next;
          current->next = NULL;
        }
        else
        {
          pvnZ = pvnZ->next;
          continue;
        }
        if (acpnew->sites->z == NULL)
	  acpnew->sites->z = current;
        else
	  prev->next = current;
        prev = current;
      } 
      
      pvnLiteralNew = ValNodeNew(NULL);
      pvnLiteralNew->choice = 1;
      pvnLiteralNew->data.ptrvalue = (Pointer)acpnew;
      pvnCoordinatesNew = ValNodeNew(NULL);
      pvnCoordinatesNew->choice = 1;
      pvnCoordinatesNew->data.ptrvalue = (Pointer)pvnLiteralNew;
      mcsnew = ModelCoordinateSetNew();
      mcsnew->id = mcs->id;
      mcsnew->descr = NULL;
      mcsnew->Coordinates_coordinates = pvnCoordinatesNew;
      
      if (!bsmp2->model_coordinates)
      {
        bsmp2->model_coordinates = mcsnew;
        mcs = mcs->next;
      }
    }
  }
  MemFree(molidx);
  return bsmp2;
}

BiostrucPtr LIBCALL PruneBiostruc(BiostrucPtr bsp, CharPtr chain)
{
  ValNodePtr vnp, pvnId = NULL, pvnIds = NULL;
  CharPtr chnid, hetid, feature_name;
  Int4 mol_id, hetnum, molId1, molId2;
  BiostrucPtr bsp2;
  BiostrucGraphPtr bsgp;
  BiostrucModelPtr bsmp, bsmp2, bsmpHead = NULL, bsmpTail;
  BiostrucFeatureSetPtr bsfsp, bsfsp2, bsfspHead = NULL, bsfspTail;
  BiostrucFeaturePtr bsfp;
  MoleculeGraphPtr bp, het, currenthet, currentbp;
  InterResidueBondPtr pirb, pirbnew;
  Boolean found1, found2;

  if (!bsp)
  {
    return NULL;
  }
      
   bsgp = bsp->chemical_graph; 
  /*bsmp = bsp->model;*/
    
  bp =  MakeBioGraphPtr(bsgp->molecule_graphs);

/* nhet = NumberOfHetChains(bsgp->molecule_graphs, bp);*/
  
  het = MakeHetGraphPtr(bsgp->molecule_graphs, bp);

  bsp2 = BiostrucNew();
  bsp2->id = bsp->id;
  bsp->id = NULL;
  vnp = ValNodeNew(NULL);
  vnp = bsp->descr;
  bsp2->descr = vnp;
  bsp->descr = NULL;
  bsp2->chemical_graph = BiostrucGraphNew();

/* Pull out the relevant parts of the molecule_graphs */
  
  for (currentbp = bp; currentbp != NULL; currentbp = currentbp->next)
  {
    if (vnp = ValNodeFindNext(currentbp->descr, NULL, BiomolDescr_name))
      chnid = vnp->data.ptrvalue;
    
    if (*chnid == *chain)
    {
      bsp2->chemical_graph->molecule_graphs = currentbp;
      mol_id = currentbp->id;
      ValNodeAddInt(&pvnIds, 0, mol_id);
      currentbp->next = NULL;
      break;
    }
  } 

  while (het)
  {
    if (vnp = ValNodeFindNext(het->descr, NULL, BiomolDescr_name))
      hetid = vnp->data.ptrvalue;
    
    hetnum = atoi(hetid);
    
    if (hetnum == mol_id)
    {
      if (!bsp2->chemical_graph->molecule_graphs->next)
      {
        bsp2->chemical_graph->molecule_graphs->next = het;
        currenthet = bsp2->chemical_graph->molecule_graphs->next;
        ValNodeAddInt(&pvnIds, 0, currenthet->id);
        het = het->next;
        currenthet->next = NULL;
      }
      else
      {
        currenthet->next = het;
        currenthet = het;
        ValNodeAddInt(&pvnIds, 0, currenthet->id);
        het = het->next;
        currenthet->next = NULL;
      }
    }
    else het = het->next;
  }
     
  bsp->chemical_graph->molecule_graphs = NULL;
  bsp2->chemical_graph->descr = bsp->chemical_graph->descr;
  bsp->chemical_graph->descr = NULL;
  bsp2->chemical_graph->residue_graphs = bsp->chemical_graph->residue_graphs;
  bsp->chemical_graph->residue_graphs = NULL;
  
  pirb = bsp->chemical_graph->inter_molecule_bonds;
  
  while (pirb)
  {
    molId1 = pirb->atom_id_1->molecule_id;
    molId2 = pirb->atom_id_2->molecule_id;
    found1 = FALSE;
    found2 = FALSE;
    pvnId = pvnIds;
    
    while (pvnId)
    {
      if (pvnId->data.intvalue == molId1)
      {
        found1 = TRUE;
        break;
      }
      pvnId = pvnId->next;
    }
    
    pvnId = pvnIds;
    while (pvnId)
    {
      if (pvnId->data.intvalue == molId2)
      {
        found2 = TRUE;
        break;
      }
      pvnId = pvnId->next;
    }

    if ((found1) && (found2))
    {
      if (!bsp2->chemical_graph->inter_molecule_bonds)
      { 
        bsp2->chemical_graph->inter_molecule_bonds = pirb;
        pirbnew = bsp2->chemical_graph->inter_molecule_bonds;
        pirb = pirb->next;
        pirbnew->next = NULL;
      }
      else
      {
        pirbnew->next = pirb;
        pirbnew = pirb;
        pirb = pirb->next;
        pirbnew->next = NULL;
      }
    }
    else pirb = pirb->next;
  } 
  
  bsp->chemical_graph->inter_molecule_bonds = NULL;

  bsfsp = bsp->features;
  while (bsfsp)
  {
    if (vnp = ValNodeFindNext(bsfsp->descr, NULL, BiostrucFeatureSetDescr_name))
       feature_name = vnp->data.ptrvalue;
     
    if ((!StringICmp("NCBI assigned secondary structure", feature_name)) ||
       (!StringICmp("NCBI Domains", feature_name)) || 
       (!StringICmp("PDB secondary structure", feature_name)))
    {
      if (!bsfspHead)
      {
        bsfspHead = PruneFeatureSet(bsfsp, mol_id);
        bsfspTail = bsfspHead;
        bsfspTail->next = NULL;
      }
      else
      {
        bsfspTail->next = PruneFeatureSet(bsfsp, mol_id); 
        bsfspTail = bsfspTail->next;
        bsfspTail->next = NULL;
      }
    }  
    
    bsfsp = bsfsp->next;
  }   
  bsp2->features = bsfspHead;
  bsp->features = NULL;
  
  bsmp = bsp->model;
  
  while (bsmp)
  {
    if (!bsmpHead)
    {
      bsmpHead = PruneModel(bsmp, pvnIds, mol_id);
      bsmpTail = bsmpHead;
      bsmpTail->next = NULL;
    }
    else
    {
      bsmpTail->next = PruneModel(bsmp, pvnIds, mol_id);
      bsmpTail = bsmpTail->next;
      bsmpTail->next = NULL;
    }
    
    bsmp = bsmp->next;
  }   
  
  bsp2->model = bsmpHead;
  bsp->model = NULL;
  bsp->chemical_graph = NULL;
  BiostrucFree(bsp);
  ValNodeFree(pvnIds);
  return bsp2;
}

