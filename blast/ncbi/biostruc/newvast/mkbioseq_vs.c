/* mkbioseq_vs.c
 *
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
 * File Name: mkbioseq_vs.c
 *
 * Author: Ken Addess
 *
 * $Log: mkbioseq_vs.c,v $
 * Revision 1.1.1.1  2002/12/06 20:17:21  chenj
 * Imported Scouces
 *
 * Revision 6.2  1999/06/15 18:12:53  addess
 * fixed some lines related to BioseqPtr
 *
 * Revision 6.2  1999/06/15 18:12:53  addess
 * fixed some lines related to BioseqPtr
 *
 * Revision 6.1  1998/07/17 18:59:57  madej
 * Created by Ken Addess.
 *
 */

/************************************************************/
/*							    */
/*	mkBioseqs()					    */
/*							    */
/*	Creates a Bioseq/BioseqSet object from a Biostruc   */
/*	object and writes it out to a file.		    */
/*							    */
/************************************************************/ 


#include "mkbioseq.h"

static Int4 NumberOfBioChains(MoleculeGraphPtr mgp)
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
} 

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

static Int4 NumberOfHetChains(MoleculeGraphPtr mgp, MoleculeGraphPtr bp)
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

static MoleculeGraphPtr MakeHetGraphPtr(MoleculeGraphPtr mgp, MoleculeGraphPtr bp)
{
  MoleculeGraphPtr newhet, het = NULL, currenthet;
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


SeqEntryPtr LIBCALL MakeBioseqs(BiostrucPtr bsp, BiostrucResidueGraphSetPtr stdDictionary)
{
  ValNodePtr vnp, seq_set, hetval, pvnThePoints;
  BiostrucHistoryPtr bhp;
  BiostrucSourcePtr bssp;
  BiostrucGraphPtr bsgp;
  BiostrucModelPtr bsmp;
  BiostrucFeatureSetPtr bsfsp;
  BiostrucFeaturePtr bsfp;
  ChemGraphPntrsPtr cgpp;
  ResiduePtr rs;
  ResidueGraphPtr rgp;
  ResiduePntrsPtr rpp;
  ResidueIntervalPntrPtr ripp;
  ResidueExplicitPntrsPtr rpp1=NULL, rpp2=NULL;
  MoleculeGraphPtr bp, het, currenthet, currentbp, mgp;
  InterResidueBondPtr currentabp, abp;
  DbtagPtr dtp;
  SeqEntryPtr pdb_entry;
  BioseqSetPtr biossp;
  BioseqPtr bioseqs[MAXNUM], current_bioseq;
  Int4 DomainNum, molId1, resId1, atmId1, molId2, resId2, atmId2;
  Int4 nbp, nhet, num_chain, index = 0, chnidx, bpchnidx, bpresidx, hetidx, rescount, bioseq_idx;
  Int4 ssresidx1, ssresidx2, ssmolidx1, ssmolidx2;
  CharPtr feature_name, rname;
  Boolean interchain, bonds, found1, found2;
  SeqAnnotPtr sap = NULL;
  SeqIdPtr sip;

  if (!bsp)
  {
    return NULL;
  }
      
  vnp = ValNodeFindNext(bsp->descr, NULL, BiostrucDescr_history);

  if (vnp)
  { 
    bhp = (BiostrucHistoryPtr) vnp->data.ptrvalue; 
    bssp = bhp->data_source;
  }

  bsgp = bsp->chemical_graph; 
  bsmp = bsp->model;
    
  nbp = NumberOfBioChains(bsgp->molecule_graphs);
   
  bp =  MakeBioGraphPtr(bsgp->molecule_graphs);

  nhet = NumberOfHetChains(bsgp->molecule_graphs, bp);
  
  het = MakeHetGraphPtr(bsgp->molecule_graphs, bp);
   
  pdb_entry = CreateSeqEntry(bssp, bsgp, bsmp, bsp->descr, nbp);
   
  if (IS_Bioseq(pdb_entry))
  {
    vnp = ValNodeFindNext(pdb_entry, NULL, 1);
    bioseqs[index] = (BioseqPtr) vnp->data.ptrvalue;
  }
  else
  {
    vnp = ValNodeFindNext(pdb_entry, NULL, 2);
    biossp = (BioseqSetPtr) vnp->data.ptrvalue;
    seq_set = biossp->seq_set;
      
    for (num_chain = 0; num_chain < nbp, seq_set; seq_set = seq_set->next, num_chain++, index++)
      bioseqs[index] = (BioseqPtr) seq_set->data.ptrvalue;
  }  
  
  dtp = (DbtagPtr)bssp->database_entry_id->data.ptrvalue;
   
  for (index = 0, currentbp = bp; index < nbp, currentbp != NULL; currentbp = currentbp->next, index++)
  {
     current_bioseq = bioseqs[index];
     if (currentbp->seq_id->choice == '\f')
     {
       current_bioseq->id = MakePDBId(bssp, currentbp, dtp);   
       sip = ValNodeNew(NULL);
       sip->choice = SEQID_GI;
       sip->data.intvalue = currentbp->seq_id->data.intvalue;
       current_bioseq->id->next = sip;
     }
     else if (currentbp->seq_id->choice == SEQID_LOCAL)
       current_bioseq->id = MakeLocalID(-99999, currentbp, dtp);
     current_bioseq->descr = MakeBioseqDescr(currentbp, current_bioseq->descr);
     current_bioseq->mol = MakeBioseqMol(currentbp);
     current_bioseq->length = CountNumOfResidues(currentbp);
     
     if (current_bioseq->mol == Seq_mol_aa)
	current_bioseq->seq_data_type = Seq_code_iupacaa;
     else
        current_bioseq->seq_data_type = Seq_code_iupacna;
     
     current_bioseq->seq_data = AddSeqData(currentbp, current_bioseq->mol, current_bioseq->length, bsgp, stdDictionary);
     current_bioseq->annot = AddNstdSeqAnnot(currentbp, current_bioseq->id, bsgp);
   }  
   /* Add information about Secondary Structure and Domains */
  for (bsfsp = bsp->features, DomainNum = 0; bsfsp; bsfsp = bsfsp->next)
  {
    if (vnp = ValNodeFindNext(bsfsp->descr, NULL, BiostrucFeatureSetDescr_name))
       feature_name = vnp->data.ptrvalue;
     
    if ((!StringICmp("NCBI assigned secondary structure", feature_name)) ||
       (!StringICmp("NCBI Domains", feature_name)))
    {
      for (bsfp = bsfsp->features; bsfp; bsfp = bsfp->next)
      {
        cgpp = (ChemGraphPntrsPtr)bsfp->Location_location->data.ptrvalue;
        rpp = (ResiduePntrsPtr)cgpp->data.ptrvalue;
        ripp = (ResidueIntervalPntrPtr)rpp->data.ptrvalue;
        chnidx = findChnidx(ripp->molecule_id, nbp, bp);
        current_bioseq = bioseqs[chnidx-1];
        
        if (!StringICmp("NCBI Domains", feature_name)) DomainNum++;
        
        if (current_bioseq->annot)
          current_bioseq->annot = AddSecDomToSeqAnnot(bsfp, feature_name, current_bioseq->annot, current_bioseq->id, DomainNum);
        else
          current_bioseq->annot = AddSecDomToSeqAnnot(bsfp, feature_name, NULL, current_bioseq->id, DomainNum); 
      }
    }
  }
  for (index = 0, currenthet = het; index < nhet, currenthet; currenthet = currenthet->next, index++)
  {
    hetval = MakeHetValNode(currenthet, stdDictionary, bsgp->residue_graphs);
    bioseq_idx = 0;
    interchain = FALSE;

    for (abp = bsgp->inter_molecule_bonds, bonds = FALSE, rescount = 0; abp; abp = abp->next)
    {
      molId1 = abp->atom_id_1->molecule_id;
      molId2 = abp->atom_id_2->molecule_id;
      resId1 = abp->atom_id_1->residue_id;
      resId2 = abp->atom_id_2->residue_id;
      atmId1 = abp->atom_id_1->atom_id;
      atmId2 = abp->atom_id_2->atom_id;
      
      if (isBiopoly(molId1, bp) && isHet(molId2, het))
      {
        bpchnidx = molId1 - 1;
	bpresidx = resId1 - 1;
	hetidx = getHetIdx(molId2, het);
        bonds = TRUE;
      }
      else if (isBiopoly(molId2, bp) && isHet(molId1, het))
      {
        bpchnidx = molId2 - 1;
	bpresidx = resId2 - 1;
	hetidx = getHetIdx(molId1, het);
        bonds = TRUE;
      }
      
      if (bonds)
      {
        if (hetidx == index)
        {
          if (!rescount) pvnThePoints = NULL;
          ValNodeAddInt(&pvnThePoints, 0, bpresidx);
          rescount++;
          if (bioseq_idx >= 0)
          {
             if (bioseq_idx != bpchnidx) interchain = TRUE;
          }
          bioseq_idx = bpchnidx;
        }
      }
    }
    
    if (rescount)
    {  
      if (!interchain)
      {
        current_bioseq = bioseqs[bioseq_idx];
        if (current_bioseq->annot)
          current_bioseq->annot = AddHetToSeqAnnot(current_bioseq->annot, current_bioseq->id, hetval, pvnThePoints, rescount);
        else
          current_bioseq->annot = AddHetToSeqAnnot(NULL, current_bioseq->id, hetval, pvnThePoints, rescount); 
      }
      if (interchain)
      {
        if (IS_Bioseq(pdb_entry))
	  sap = ((BioseqPtr)(pdb_entry->data.ptrvalue))->annot;
	else
          sap = ((BioseqSetPtr)(pdb_entry->data.ptrvalue))->annot;
	if (sap == NULL)
	{
	  sap = SeqAnnotNew();
	  sap->type = 1;
	  if (IS_Bioseq(pdb_entry))
	    ((BioseqPtr)(pdb_entry->data.ptrvalue))->annot = sap;
          else
	    ((BioseqSetPtr)(pdb_entry->data.ptrvalue))->annot = sap;
         }
         sap = AddHetToSeqAnnot(sap, bioseqs[bioseq_idx]->id, hetval, pvnThePoints, rescount);
      }
    }
    else
    {
      current_bioseq = bioseqs[bioseq_idx];
      vnp = current_bioseq->descr;
      if (vnp != NULL)
      { 
        while (vnp->next != NULL) vnp = vnp->next;
	vnp->next = hetval;
      }
      else current_bioseq->descr = hetval;
    }
  }      
  
  mgp = bsgp->molecule_graphs;
  abp = bsgp->inter_molecule_bonds;
  
  while(1)
  {
    if (mgp != NULL)
    { 
      currentabp = mgp->inter_residue_bonds;
      mgp = mgp->next;
    }
    
    while (currentabp != NULL)
    {
      molId1 = currentabp->atom_id_1->molecule_id;
      molId2 = currentabp->atom_id_2->molecule_id;
      resId1 = currentabp->atom_id_1->residue_id;
      resId2 = currentabp->atom_id_2->residue_id;
      atmId1 = currentabp->atom_id_1->atom_id;
      atmId2 = currentabp->atom_id_2->atom_id;

      interchain = FALSE;
      found1 = FALSE;
      found2 = FALSE;

      if ((getAtomElementIdx(molId1, resId1, atmId1, bsgp, stdDictionary)==16) 
         && (getAtomElementIdx(molId2, resId2, atmId2, bsgp, stdDictionary)==16))
      { 
	 /* Found possible disulfide bonds. */

        if (isBiopoly(molId1, bp) && isBiopoly(molId2, bp))  
	{
	  currentbp = bp;
	  for (index=0; index<findChnidx(molId1, nbp, bp)-1; index++)
	    currentbp = currentbp->next;

          rs = currentbp->residue_sequence;

	   while (rs)
	   {
	     if (rs->id == resId1)
	     {
               rgp = getResGraph(rs->residue_graph, bsgp, stdDictionary);
	       break;
             }
           
            rs = rs->next;
           }
           
          if (vnp = ValNodeFindNext(rgp->descr, NULL, BiomolDescr_name))
             rname = vnp->data.ptrvalue;
		 
          if (!StringICmp(rname, "CYS")) found1 = TRUE;

	  currentbp = bp;
	  for (index = 0; index < findChnidx(molId2, nbp, bp)-1; index++)
	     currentbp = currentbp->next;

          rs = currentbp->residue_sequence;

	  while (rs)
	  {
	    if (rs->id == resId2)
	    {
	      rgp = getResGraph(rs->residue_graph, bsgp, stdDictionary);
	      break;
             }

	    rs = rs->next;
          }

	  if (vnp = ValNodeFindNext(rgp->descr, NULL, BiomolDescr_name))
            rname = vnp->data.ptrvalue;
 
	  if (!StringICmp(rname, "CYS")) found2 = TRUE;

	  if (found1 && found2)
	  {
            ssresidx1 = resId1 - 1;
            ssresidx2 = resId2 - 1;
            ssmolidx1 = molId1 - 1;
            ssmolidx2 = molId2 - 1;
            chnidx = findChnidx(molId1, nbp, bp);
            
            if (ssmolidx1 == ssmolidx2) 
            {
              current_bioseq = bioseqs[chnidx - 1];
              if (current_bioseq->annot)
                current_bioseq->annot = AddDisulToSeqAnnot(current_bioseq->annot, ssresidx1, ssresidx2, current_bioseq->id, current_bioseq->id);
              else
                current_bioseq->annot = AddDisulToSeqAnnot(NULL, ssresidx1, ssresidx2, current_bioseq->id, current_bioseq->id);
            }
            else
            {
              if (IS_Bioseq(pdb_entry))
	        sap = ((BioseqPtr)(pdb_entry->data.ptrvalue))->annot;
	      else
                sap = ((BioseqSetPtr)(pdb_entry->data.ptrvalue))->annot;
	      if (sap == NULL)
	      {
	        sap = SeqAnnotNew();
	        sap->type = 1;
	        if (IS_Bioseq(pdb_entry))
	          ((BioseqPtr)(pdb_entry->data.ptrvalue))->annot = sap;
                else
	          ((BioseqSetPtr)(pdb_entry->data.ptrvalue))->annot = sap;
              }
              sap = AddDisulToSeqAnnot(sap, ssresidx1, ssresidx2, bioseqs[ssmolidx1]->id, bioseqs[ssmolidx2]->id);
            }   
          }
        }
      }   
      
      currentabp = currentabp->next;
    }
    
    if ((currentabp == NULL) && (mgp == NULL) && (abp == NULL)) break;
    
    else if((currentabp == NULL) && (mgp == NULL) && (abp != NULL))
    { 
      currentabp = abp;
      abp = NULL;
    }
  }

  return pdb_entry;
}


