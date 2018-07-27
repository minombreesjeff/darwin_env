/* mkbioseqA.c
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
 * File Name: mkbioseqA.c
 *
 * Author: Ken Addess
 *
 * $Log: mkbioseqA.c,v $
 * Revision 1.1.1.1  2002/12/06 20:17:21  chenj
 * Imported Scouces
 *
 * Revision 1.3  2001/04/25 22:40:43  he
 * Added vastsrvDB.c as database version of vastsrv.c
 *
 * Revision 1.2  2001/04/17 18:27:57  he
 * Added vastdtbs.c as database version of vastlocl
 *
 * Revision 6.2  1998/12/01 15:13:29  addess
 * cleaned up code to remove memory leaks
 *
 * Revision 6.1  1998/07/17 18:54:26  madej
 * Created by Ken Addess.
 *
 */

#include "mkbioseq.h"

/*****************************************************************************
*
*   Begin an entry, adding mol and chain ids, descriptions
*   	This makes a Seq-entry of type PDB-entry. 
*
*****************************************************************************/


SeqEntryPtr LIBCALL CreateSeqEntry
(BiostrucSourcePtr bssp, BiostrucGraphPtr bsgp, BiostrucModelPtr bsmp, ValNodePtr descr, Int4 nchn)
{
	SeqEntryPtr pdb_entry, sephead = NULL, prev, sep;
	BioseqPtr biosp;
	BioseqSetPtr biossp;
	Int4 i;
	Int2 month, day, year;
	PdbBlockPtr pbp;
	ValNodePtr vnp, tmp, pubcat, pubd, pubs;
	CharPtr com, compound = NULL, source = NULL;
	PubdescPtr pdp;
	CitSubPtr citsub;
	CitArtPtr citart;
	CitBookPtr citbook;
	CitGenPtr citgen;

	/*entry_name = StringSave(*pdbid);*/
       /* save entry name */

	/*** get the PdbBlock data ***************/

	pbp = PdbBlockNew();
	pbp->deposition = bssp->database_entry_date;
        
        if (vnp = ValNodeFindNext(bsgp->descr, NULL, BiomolDescr_pdb_class))
          pbp->pdbclass = StringSave(vnp->data.ptrvalue);
     
        if (vnp = ValNodeFindNext(bsgp->descr, NULL, BiomolDescr_name))
        {
          compound = StringSave(vnp->data.ptrvalue);
          tmp = ValNodeNew(NULL);
          pbp->compound = tmp;
          tmp->data.ptrvalue = (Pointer)StringSave(compound);
        }

        if (vnp = ValNodeFindNext(bsgp->descr, NULL, BiomolDescr_pdb_source))
        {
          source = StringSave(vnp->data.ptrvalue);
          tmp = ValNodeNew(NULL);
          pbp->source = tmp;
          tmp->data.ptrvalue = (Pointer)StringSave(source);
        }
        
        MemFree(compound);
        MemFree(source);

	if (vnp = ValNodeFindNext(bsmp->descr, NULL, ModelDescr_pdb_method))
          pbp->exp_method = StringSave(vnp->data.ptrvalue);
        
        
	/*** make a seq_set of bioseqs *****************/

	for (i = 0; i < nchn; i++)
	{
          sep = SeqEntryNew();
          if (sephead == NULL) sephead = sep;
          else prev->next = sep;
	  prev = sep;
          sep->choice = 1;    /* Bioseq */
	  biosp = BioseqNew();
	  sep->data.ptrvalue = (Pointer) biosp;
          biosp->repr = Seq_repr_raw;    /* always raw */
	}

	/***** make pbp into a Seq-descr *******/

        vnp = ValNodeNew(NULL);
	vnp->choice = Seq_descr_pdb;
	vnp->data.ptrvalue = (Pointer) pbp;

	/***** is it a set or a single sequence *********/

	if (nchn == 1)    /* one sequence */
	{
	  pdb_entry = sephead;
	  biosp->descr = vnp;	
	}
	else                   /* multiple sequences */
	{
	  pdb_entry = SeqEntryNew();
          pdb_entry->choice = 2;   /* Bioseq-set */
	  biossp = BioseqSetNew();
	  pdb_entry->data.ptrvalue = (Pointer) biossp;
	  biossp->descr = vnp;
	  biossp->seq_set = sephead;
	  biossp->_class = 12;   /* pdb-entry */
	}

        if (tmp = ValNodeFindNext(bsmp->descr, NULL, ModelDescr_pdb_comment))
        {
          com = (CharPtr)MemNew(10 * sizeof(Char)); 
          StringNCpy(com, tmp->data.ptrvalue, 9);
          com[9] = '\0';
          if (StringICmp("Alternate", com) && StringICmp("A complet", com)) 
          {
            vnp =  ValNodeNew(vnp);
            vnp->choice = Seq_descr_comment;
            vnp->data.ptrvalue = MemNew((size_t)(StringLen(tmp->data.ptrvalue) +19));
            StringCpy(vnp->data.ptrvalue, "Revision History:~");
            StringCat(vnp->data.ptrvalue, tmp->data.ptrvalue);
          }
          MemFree(com);
        }
        
        while (descr)
        {
          if (descr->choice == BiostrucDescr_attribution)
          {
             pubs = descr->data.ptrvalue;

            if (nchn == 1)
              pubd = ValNodeNew(((BioseqPtr)(pdb_entry->data.ptrvalue))->descr);
            else 
              pubd = ValNodeNew(((BioseqSetPtr)(pdb_entry->data.ptrvalue))->descr);
          
            pubd->choice = Seq_descr_pub;
 	    pdp = PubdescNew();
  	    pubd->data.ptrvalue = pdp;

  	    pdp->pub = ValNodeNew(NULL);
  
  	    switch(pubs->choice)
  	    {
    	      case PUB_Gen:
                citgen = pubs->data.ptrvalue;
                pdp->pub->choice = 1;
                pdp->pub->data.ptrvalue = citgen;

                break;
              case PUB_Sub:
                citsub = pubs->data.ptrvalue;
                pdp->pub->choice = 2;
                pdp->pub->data.ptrvalue = citsub;

                break;
              case PUB_Article:
                citart = pubs->data.ptrvalue;
                pdp->pub->choice = 5;
                pdp->pub->data.ptrvalue = citart;

                break;
              case PUB_Book:
                citbook = pubs->data.ptrvalue;
                pdp->pub->choice = 7;
                pdp->pub->data.ptrvalue = citbook;

                break;
              case PUB_Equiv:
                pdp->pub = ValNodeFree(pdp->pub);
                pubcat = pubs->data.ptrvalue;

                while (pubcat)
                {
                  vnp = ValNodeNew(pdp->pub);
                  if (pdp->pub == NULL)
                    pdp->pub = vnp;

                  vnp->choice = pubcat->choice;
                  if (pubcat->choice==4)
                    vnp->data.intvalue = pubcat->data.intvalue;
                  else
                    vnp->data.ptrvalue = pubcat->data.ptrvalue;

                  pubcat = pubcat->next;
                }  
                break;
              } 
            }
            descr = descr->next;
          }  
      
      return pdb_entry;
}


/*****************************************************************************
*
*   Make SeqIDPtr
*
*****************************************************************************/
SeqIdPtr LIBCALL MakePDBId(BiostrucSourcePtr bssp, MoleculeGraphPtr mgp, DbtagPtr dtp)
{
  ValNodePtr vnp;
  SeqIdPtr sip;
  PDBSeqIdPtr psp;
  CharPtr chnid;
        
  if (vnp = ValNodeFindNext(mgp->descr, NULL, BiomolDescr_name))
    chnid = vnp->data.ptrvalue;
	
  sip = ValNodeNew(NULL);
  sip->choice = SEQID_PDB;

  psp = PDBSeqIdNew();
  sip->data.ptrvalue=psp;
  psp->mol = StringSave(dtp->tag->str);
  psp->chain = (Uint1) *chnid;
  psp->rel = DateNew();
  psp->rel = bssp->database_entry_date;
  return sip;
}

SeqIdPtr LIBCALL MakeLocalID(Int4 mid, MoleculeGraphPtr mgp, DbtagPtr dtp)
{
  ValNodePtr vnp;
  SeqIdPtr sip;
  ObjectIdPtr oip;
  CharPtr chnid;
  Char string_name[9];
  CharPtr dom_name = {"0"};
  
  
  if (vnp = ValNodeFindNext(mgp->descr, NULL, BiomolDescr_name))
    chnid = vnp->data.ptrvalue;
   
  string_name[0] = '\0';
  StringCpy(string_name, dtp->tag->str);
  StringCat(string_name, " ");
  StringCat(string_name, chnid);
  StringCat(string_name, " ");
  StringCat(string_name, dom_name);

  sip = ValNodeNew(NULL);
  sip->choice = SEQID_LOCAL;
	
  oip = ObjectIdNew();
  sip->data.ptrvalue = oip;
  oip->id = mid;
  oip->str = StringSave(string_name);

  return sip;
}

SeqIdPtr LIBCALL MakeGId(Int4 gi)
{
	SeqIdPtr sip;

	sip = ValNodeNew(NULL);
	sip->choice = SEQID_GI;
	sip->data.intvalue=gi;

	return sip;
}
/*****************************************************************************
*
*   Make Bioseq Descriptors
*
*****************************************************************************/

ValNodePtr LIBCALL MakeBioseqDescr(MoleculeGraphPtr mgp, ValNodePtr bioseq_descr)
{
  ValNodePtr descr = NULL, vnp;
  BioSourcePtr biossp = NULL;
  ResiduePtr rs;
  ResidueGraphPntrPtr rgpp;
  CharPtr *resnam = NULL, rn, tmp;
  Int4 resnum, index, len;
  NumEnumPtr nep;

  /* For taxonomy */
  if (vnp = ValNodeFindNext(mgp->descr, NULL, BiomolDescr_organism))
      biossp = (BioSourcePtr) vnp->data.ptrvalue; 
  
  if (biossp)
  {
    descr = ValNodeNew(bioseq_descr);
    if (!bioseq_descr) bioseq_descr = descr;
    descr->choice = Seq_descr_source;
    descr->data.ptrvalue = (Pointer) biossp;
  }       
  /* For comments */
  vnp = ValNodeFindNext(mgp->descr, NULL, BiomolDescr_pdb_comment);
  
  if (!vnp)
  {
    descr = ValNodeNew(bioseq_descr);
    if (!bioseq_descr) bioseq_descr = vnp;
    descr->choice = Seq_descr_comment;
    descr->data.ptrvalue = StringSave("Sequence derived from ATOM co-ordinates.");
  }
  /* For descr->enum */
  resnum = CountNumOfResidues(mgp);
  resnam = (CharPtr PNTR)MemNew((resnum + 1) * sizeof(CharPtr));
  
  for (rs = mgp->residue_sequence, index = 0 ; rs, index < resnum ; rs = rs->next, index++)
  {
    if (rs->name == NULL)
      resnam[index] = (CharPtr)StringSave("");
    else
    {
      rn = rmvSpace(rs->name);
      if (!StringICmp(rn," "))
      {
        MemFree(rn);
	resnam[index] = StringSave("");
      }
      else
        resnam[index] = rn;
    }
  }
  
  descr = ValNodeNew(bioseq_descr);
  if (bioseq_descr == NULL) bioseq_descr = descr;
  descr->choice = Seq_descr_num;
  descr->data.ptrvalue = ValNodeNew(NULL);
  descr = descr->data.ptrvalue;
  descr->choice = Numbering_enum;
  nep = NumEnumNew();
  descr->data.ptrvalue = nep;
  nep->num = resnum;
  len = 0;
  for (index = 0; index < nep->num; index++)
      len += StrLen(resnam[index]) + 1;
  nep->buf = MemNew((size_t)len);
  nep->names = MemNew((size_t)(sizeof(CharPtr) * nep->num));
  tmp = nep->buf;
  for (index = 0; index < nep->num; index++)
  {
    nep->names[index] = tmp;
    StringMove(tmp, resnam[index]);
    tmp += StrLen(resnam[index]) + 1;
  }
  
  MemFree(resnam);
  return bioseq_descr;
}

Uint1 LIBCALL MakeBioseqMol(MoleculeGraphPtr mgp)
{
  ValNodePtr vnp; 
  Int4 mtype;  
  Uint1 mol;
 
  if (vnp = ValNodeFindNext(mgp->descr, NULL, BiomolDescr_molecule_type))
     mtype = vnp->data.intvalue;
  
  switch(mtype)
  {
    case 1:
      mol = Seq_mol_dna;
      break;
    case 2:
      mol = Seq_mol_rna;
      break;
    case 3:
      mol = Seq_mol_aa;
      break;
  }
  
  return mol;
}

Int4 LIBCALL CountNumOfResidues(MoleculeGraphPtr mgp)
{
  Int4 resnum = 0;
  ResiduePtr rs;
  
  rs = mgp->residue_sequence;
  
  while (rs)
  {
    rs = rs->next;
    resnum++;
  }
  
  return resnum;
}

ByteStorePtr LIBCALL 
AddSeqData(MoleculeGraphPtr mgp, Uint1 mol, Int4 length, BiostrucGraphPtr bsgp, BiostrucResidueGraphSetPtr stdDictionary)
{
  ByteStorePtr seq_data;
  CharPtr olcode;
  BiostrucResidueGraphSetPntrPtr brgsp;
  DbtagPtr dtp;
  AsnIoPtr aipr=NULL;
  ResidueGraphPtr newrg;
  ResidueGraphPntrPtr rgpp;
  ResiduePtr rs;
  Int2 residue;
  Int4 i = 0;
  
  seq_data = BSNew(length);
 
  for (i = 0, rs = mgp->residue_sequence; i < length, rs; i++, rs = rs->next)
  {
    rgpp = rs->residue_graph;

    switch(rgpp->choice)
    {
      case ResidueGraphPntr_local:
        newrg = getNstdResGraph(rgpp->data.intvalue, bsgp);
        olcode = getNstdOlcode(newrg);
        break;
      case ResidueGraphPntr_biostruc:
        break;
      case ResidueGraphPntr_standard:
        brgsp = (BiostrucResidueGraphSetPntrPtr)rgpp->data.ptrvalue;
        dtp = (DbtagPtr) brgsp->biostruc_residue_graph_set_id->data.ptrvalue;
        olcode = getStdOlcode(dtp->tag->id, brgsp->residue_graph_id, stdDictionary);
        break;
    }
    residue = (Int2)(*olcode);
    if ((mol != Seq_mol_aa) && (residue == 'U')) residue = 'T';
    BSPutByte(seq_data, residue); 
  }
  
  return seq_data;
}

SeqAnnotPtr LIBCALL AddNstdSeqAnnot(MoleculeGraphPtr mgp, SeqIdPtr id, BiostrucGraphPtr bsgp)
{
  ValNodePtr head = NULL, current, prev = NULL, vnp;
  ResiduePtr rs;
  ResidueGraphPtr newrg, currentrg, nstdrg = NULL;
  ResidueGraphPntrPtr rgpp;
  SeqAnnotPtr sap = NULL;
  SeqFeatPtr sfp, prev_seqfeat;
  SeqLocPtr slp;
  SeqPntPtr spp;
  Int4 nstdnum = 0, i, len, k, resnum;
  CharPtr rname, pdbcomm, resnam, chp;
  Boolean nucacid;
 
  resnum = 0;
  for (rs = mgp->residue_sequence; rs; rs = rs->next)
  {
    rgpp = rs->residue_graph;
    
    if (rgpp->choice == ResidueGraphPntr_local)
    {
      newrg = getNstdResGraph(rgpp->data.intvalue, bsgp);
      
      if (isNstd(newrg))
      {
        if (nstdrg == NULL)
          nstdrg = newrg;
        else
          currentrg->next = newrg;
        currentrg = newrg;

        current = ValNodeNew(prev);
        current->data.intvalue = resnum;

        if (head == NULL)
          head = current;
        else
          prev->next = current;
        prev = current;
        nstdnum++;
      } 
      else
        MemFree(newrg);
    }
    resnum++;
  }
  
  if (nstdnum > 0)
  {
    if (sap == NULL)
    {
      sap = SeqAnnotNew();
      sap->type = 1;
    }
    
    prev_seqfeat = (SeqFeatPtr)sap->data;
    if (prev_seqfeat != NULL)
    {
      while (prev_seqfeat->next != NULL)
	prev_seqfeat = prev_seqfeat->next;
    }
    
    for (currentrg = nstdrg, current = head, i = 0; currentrg, current, i < nstdnum; 
         currentrg = currentrg->next, current = current->next, i++)
    {
      if (vnp = ValNodeFindNext(currentrg->descr, NULL, BiomolDescr_name))
        rname = vnp->data.ptrvalue;
      
      if (vnp = ValNodeFindNext(currentrg->descr, NULL, BiomolDescr_pdb_comment))
        pdbcomm = vnp->data.ptrvalue;
      
      nucacid = FALSE;
      chp = rname; 
      if ((*chp++=='D')&&(*chp++=='N')&&(*chp++=='A'))
        nucacid = TRUE;
      chp = rname;
      if ((*chp++=='R')&&(*chp++=='N')&&(*chp++=='A'))
        nucacid = TRUE;
      
      if (nucacid)
      {
        chp = rname;
	chp+=3;
	len = strlen(rname);
	resnam = (CharPtr)MemNew((len-2) * sizeof(char));
	k = 0;
	while (*chp != '\0')
	{
          resnam[k++] = *chp++;
        }
	resnam[k] = '\0';
      }
      else resnam = StringSave(rname);
        
      sfp = SeqFeatNew();
      if (prev_seqfeat == NULL)
        sap->data = sfp;
      else
        prev_seqfeat->next = sfp;
      prev_seqfeat = sfp;
      sfp->data.choice = 18;   /* non-std residue */
      sfp->data.value.ptrvalue = StringSave(resnam);
			
      if (*pdbcomm != '\0') 
        sfp->comment = StringSave(pdbcomm);
      slp = ValNodeNew(NULL);
      sfp->location = slp;
      slp->choice = SEQLOC_PNT;
      spp = SeqPntNew();
      slp->data.ptrvalue = spp;
      spp->point = current->data.intvalue;
      spp->id = SeqIdDup(id);
    }
  
  return sap;
  }
  else return NULL;

} 

SeqAnnotPtr LIBCALL AddSecDomToSeqAnnot(BiostrucFeaturePtr bsfp, CharPtr name, SeqAnnotPtr seq_annot, SeqIdPtr id, Int4 num)
{
  SeqAnnotPtr sap;
  SeqFeatPtr sfp, prev;
  SeqLocPtr slp, prevloc;
  SeqIntPtr sip;
  CharPtr tmp;
  Char domnum[4];  	/* changed from 3 to 4 by J. Chen 3/7/02 */
  PDBSeqIdPtr psp;
  ChemGraphPntrsPtr cgpp;
  ResiduePntrsPtr rpp;
  ResidueIntervalPntrPtr ripp = NULL;
  Int4 rippcount = 0;
  
  if (bsfp == NULL) return seq_annot;
  
  sap = seq_annot;
  if (sap == NULL)
  {
    sap = SeqAnnotNew();
    seq_annot = sap;
    sap->type = 1;
  }
  prev = (SeqFeatPtr)sap->data;
  if (prev != NULL)
  {
    while (prev->next != NULL)
     prev = prev->next;
  }
  
  cgpp = (ChemGraphPntrsPtr)bsfp->Location_location->data.ptrvalue;
  rpp = (ResiduePntrsPtr)cgpp->data.ptrvalue;
  ripp = (ResidueIntervalPntrPtr)rpp->data.ptrvalue;
  
  while (ripp)
  {
    rippcount++;
    ripp = ripp->next;
  }
   
  sfp = SeqFeatNew();
  if (prev == NULL)
    sap->data = sfp;
  else
    prev->next = sfp;
  prev = sfp;

  if (!StringICmp("NCBI assigned secondary structure", name))
  {
    sfp->data.choice = 17;   /* protein secondary structure */
    sfp->data.value.intvalue = bsfp->type;
    sfp->comment = StringSave(bsfp->name);
  }
  else
  {
    sfp->data.choice = 9;
    tmp = MemNew(StringLen(bsfp->name) + sizeof(domnum));
    *tmp = '\0';
    StringCpy(tmp, bsfp->name);
    sprintf(domnum, " %d", num);
    StringCat(tmp, domnum);  
    sfp->data.value.ptrvalue = StringSave(tmp);
    sfp->comment = StringSave(name);
  } 
  slp = ValNodeNew(NULL);
  sfp->location = slp;
  
  if (rippcount <= 1)
  {
    ripp = (ResidueIntervalPntrPtr)rpp->data.ptrvalue;
    slp->choice = SEQLOC_INT;
    sip = SeqIntNew();
    slp->data.ptrvalue = sip;
    sip->from = ripp->from - 1;
    sip->to = ripp->to - 1;
    sip->id = SeqIdDup(id);
  }
  else
  {
    prevloc = NULL;
    slp->choice = SEQLOC_MIX;
    for (ripp = (ResidueIntervalPntrPtr)rpp->data.ptrvalue; ripp; ripp = ripp->next) 
    {
      slp = ValNodeNew(prevloc);
      if (prevloc == NULL)
        sfp->location->data.ptrvalue = slp;
      prevloc = slp;
      slp->choice = SEQLOC_INT;
      sip = SeqIntNew();
      slp->data.ptrvalue = sip;
      sip->from = ripp->from - 1;
      sip->to = ripp->to - 1;
      sip->id = SeqIdDup(id);
    }
  }
  
  return sap;
}

ValNodePtr LIBCALL 
MakeHetValNode(MoleculeGraphPtr nhet, BiostrucResidueGraphSetPtr stdDictionary, ResidueGraphPtr rg)
{
  ValNodePtr hetval = NULL;
  CharPtr hettype, hetnum, hetdesc, tmp;
  Int4 len;
  
  hettype = StringSave(getResNam(nhet->residue_sequence->residue_graph, stdDictionary, rg));
  hetnum = StringSave(nhet->residue_sequence->name);
  hetdesc = StringSave(getResComm(nhet->residue_sequence->residue_graph, stdDictionary, rg));
  
  hetval = ValNodeNew(NULL);
  hetval->choice = Seq_descr_het;
  tmp = MemNew(StringLen(hettype) + StringLen(hetnum) + StringLen(hetdesc) + 5);
  hetval->data.ptrvalue = tmp;
  tmp = StringMove(tmp, "(");
  tmp = StringMove(tmp, hettype);
  tmp = StringMove(tmp, ",");
  tmp = StringMove(tmp, hetnum);
  tmp = StringMove(tmp, ")");

  if (*hetdesc != '\0')
  {
    tmp = StringMove(tmp, " ");
    tmp = StringMove(tmp, hetdesc);
  }
 
  return hetval;
}

SeqAnnotPtr LIBCALL AddHetToSeqAnnot
(SeqAnnotPtr seq_annot, SeqIdPtr id, ValNodePtr hetval, ValNodePtr pvnThePoints, Int4 rescount)
{
  SeqAnnotPtr sap;
  SeqFeatPtr sfp, prev;
  SeqLocPtr slp, prevloc;
  SeqBondPtr sbp;
  SeqPntPtr spp;
  ValNodePtr pvnPoint;
  
  
  sap = seq_annot;
  
  if (sap == NULL)
  {
    sap = SeqAnnotNew();
    seq_annot = sap;
    sap->type = 1;
  }
 
  prev = (SeqFeatPtr)sap->data;
  if (prev != NULL)
  {
    while (prev->next != NULL)
     prev = prev->next;
  }
		
  sfp = SeqFeatNew();
  if (prev == NULL)
    sap->data = sfp;
  else
    prev->next = sfp;

  sfp->data.choice = 19;   /* heterogen */
  sfp->data.value.ptrvalue = hetval->data.ptrvalue;
  slp = ValNodeNew(NULL);
  sfp->location = slp;
  
  if (rescount == 1)
  {
    slp->choice = SEQLOC_BOND;
    sbp = SeqBondNew();
    slp->data.ptrvalue = sbp;
    spp = SeqPntNew();
    sbp->a = spp;
    spp->id = SeqIdDup(id);
    spp->point = (Int4) pvnThePoints->data.intvalue;
  }
  else
  {
    prevloc = NULL;
    slp->choice = SEQLOC_MIX;
    for (pvnPoint = pvnThePoints; pvnPoint; pvnPoint = pvnPoint->next) 
    {
      slp = ValNodeNew(prevloc);
      if (prevloc == NULL)
        sfp->location->data.ptrvalue = slp;
      prevloc = slp;
      slp->choice = SEQLOC_BOND;
      sbp = SeqBondNew();
      slp->data.ptrvalue = sbp;
      spp = SeqPntNew();
      sbp->a = spp;
      spp->id = SeqIdDup(id);
      spp->point = (Int4) pvnPoint->data.intvalue;
    }
  }
			  
  return sap;
}
			
SeqAnnotPtr LIBCALL AddDisulToSeqAnnot(SeqAnnotPtr seq_annot, Int4 residx1, Int4 residx2, SeqIdPtr id1, SeqIdPtr id2)
{
  SeqAnnotPtr sap;
  SeqFeatPtr sfp, prev;
  SeqLocPtr slp;
  SeqBondPtr sbp;
  SeqPntPtr spp;
  
  sap = seq_annot;
  
  if (sap == NULL)
  {
    sap = SeqAnnotNew();
    seq_annot = sap;
    sap->type = 1;
  }
 
  prev = (SeqFeatPtr)sap->data;
  if (prev != NULL)
  {
    while (prev->next != NULL)
     prev = prev->next;
  }
		
  sfp = SeqFeatNew();
  if (prev == NULL)
    sap->data = sfp;
  else
    prev->next = sfp;

  sfp->data.choice = 11;   /* bond */
  sfp->data.value.intvalue = 1;   /* disulfide */
  slp = ValNodeNew(NULL);
  sfp->location = slp;
  slp->choice = SEQLOC_BOND;
  sbp = SeqBondNew();
  slp->data.ptrvalue = sbp;
  spp = SeqPntNew();
  sbp->a = spp;
  spp->id = SeqIdDup(id1);
  spp->point = residx1;
  spp = SeqPntNew();
  sbp->b = spp;
  spp->id = SeqIdDup(id2);
  spp->point = residx2;
  
  return sap;

}
