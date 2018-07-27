/* mkbioseqB.c
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
 * File Name: mkbioseqB.c
 *
 * Author: Ken Addess
 *
 * $Log: mkbioseqB.c,v $
 * Revision 6.3  1999/05/07 14:03:21  zimmerma
 * Moved LIBCALL isBiopoly() and LIBCALL isHet() into mmdbapi1.c
 *
 * Revision 6.2  1998/12/01 15:13:36  addess
 * cleaned up code to remove memory leaks
 *
 * Revision 6.1  1998/07/17 18:54:37  madej
 * Created by Ken Addess.
 *
 */

/************************************************************
/*							    */
/*	readBiostruc()					    */
/*							    */
/*	Opens a file, reads data for a PDB entry from it,   */
/*	and closes it.					    */
/*							    */
/************************************************************/ 

#include "mkbioseq.h"
#include <mmdbapi.h>

BiostrucPtr LIBCALL readBiostruc(CharPtr filename, Int2 mode, Int4 mdlLvl)
{
   BiostrucPtr lbsp;   
 
   if (filename == NULL) {
     return NULL;
   }

   /* Read in a Biostruc object. */

   /* lbsp = BiostrucAsnRead(aipr, NULL); */
  
   lbsp = FetchBS(filename, mode, mdlLvl, 1000, POWER_VIEW);

   return lbsp;
}

Int4 LIBCALL getHetIdx(Int4 molecule_id, MoleculeGraphPtr currenthet)
{
   Int4 i;

   for (i = 0; currenthet; i++, currenthet = currenthet->next)
   {
      if (currenthet->id == molecule_id)
      {
	 return i;
      }
   }

   return -1;
}

/************************************************************/
/*							    */
/*	getStdOlcode()					    */
/*							    */
/*	Returns a one-letter iupac code for a standard      */
/*	residue.		    			    */
/*							    */
/************************************************************/ 

CharPtr LIBCALL 
getStdOlcode(Int4 standard_graphs_id, Int4 residue_graph_id, BiostrucResidueGraphSetPtr std)
{
   ResidueGraphPtr rg;

   rg = std->residue_graphs;

   while (rg)
   {
      if (rg->id == residue_graph_id)
      {
	 return(rg->iupac_code->data.ptrvalue);
      }

      rg = rg->next;
   }

   return NULL; 
}

/************************************************************/
/*							    */
/*	getNstdOlcode()					    */
/*							    */
/*	Returns a one-letter iupac code for a non-standard  */
/*	residue.		    			    */
/*							    */
/************************************************************/ 

CharPtr LIBCALL getNstdOlcode(ResidueGraphPtr nrg)
{
   return(nrg->iupac_code->data.ptrvalue);
}

/************************************************************/
/*							    */
/*	getNstdResGraph()				    */
/*							    */
/*	Given a graph id, returns a corresponding residue   */
/*	graph. 						    */
/*							    */
/************************************************************/ 

ResidueGraphPtr LIBCALL getNstdResGraph(Int4 rgid, BiostrucGraphPtr bsgp)
{
   ResidueGraphPtr rgp, new;

   rgp = bsgp->residue_graphs;

   while (rgp)
   {
      if (rgp->id == rgid)
      {
	 new = ResidueGraphNew();
	 new->id = rgp->id;
	 new->descr = rgp->descr;
	 new->residue_type = rgp->residue_type;
	 new->iupac_code = rgp->iupac_code;
	 new->atoms = rgp->atoms;
	 new->bonds = rgp->bonds;
	 new->chiral_centers = rgp->chiral_centers;

	 return new;
      }

      rgp = rgp->next;
   }

   return NULL;
}

/************************************************************/
/*							    */
/*	rmvSpace()					    */
/*							    */
/*	Removes white space from a give string.		    */
/*							    */
/************************************************************/ 

CharPtr LIBCALL rmvSpace(CharPtr str)
{
   CharPtr newstr = NULL;
   Int4 len, i, j;
   Boolean allBlank = TRUE;

   len = strlen(str);
   for (i=0; i<len; i++)
   {
      if (str[i] != ' ')
      {
         allBlank = FALSE;
         break;
      }
   }

   if (!allBlank)
   {
      newstr = (CharPtr)MemNew((len+1) * sizeof(CharPtr));

      for (i=0, j=0; i<len; i++)
      {
         if (str[i] != ' ')
            newstr[j++] = str[i]; 
      }
      newstr[j] = '\0';

      return newstr;
   }
   
   MemFree(newstr);
   return StringSave(" ");
}

Boolean LIBCALL isNstd(ResidueGraphPtr rgp)
{
   ValNodePtr vnp;
   CharPtr rname;

   vnp = rgp->descr;

   while (vnp)
   {
      if (vnp->choice == BiomolDescr_name)
      {
	 rname = vnp->data.ptrvalue;
	 break;
      }

      vnp = vnp->next; 
   }

   if (!StringICmp(rname, "ALA"))
      return(FALSE);
   else if (!StringICmp(rname, "ARG"))
      return(FALSE);
   else if (!StringICmp(rname, "ASN"))
      return(FALSE);
   else if (!StringICmp(rname, "ASP"))
      return(FALSE);
   else if (!StringICmp(rname, "CYS"))
      return(FALSE);
   else if (!StringICmp(rname, "GLN"))
      return(FALSE);
   else if (!StringICmp(rname, "GLU"))
      return(FALSE);
   else if (!StringICmp(rname, "GLY"))
      return(FALSE);
   else if (!StringICmp(rname, "HIS"))
      return(FALSE);
   else if (!StringICmp(rname, "ILE"))
      return(FALSE);
   else if (!StringICmp(rname, "LEU"))
      return(FALSE);
   else if (!StringICmp(rname, "LYS"))
      return(FALSE);
   else if (!StringICmp(rname, "MET"))
      return(FALSE);
   else if (!StringICmp(rname, "PHE"))
      return(FALSE);
   else if (!StringICmp(rname, "PRO"))
      return(FALSE);
   else if (!StringICmp(rname, "SER"))
      return(FALSE);
   else if (!StringICmp(rname, "THR"))
      return(FALSE);
   else if (!StringICmp(rname, "TRP"))
      return(FALSE);
   else if (!StringICmp(rname, "TYR"))
      return(FALSE);
   else if (!StringICmp(rname, "VAL"))
      return(FALSE);
   else if (!StringICmp(rname, "DNA  A"))
      return(FALSE);
   else if (!StringICmp(rname, "DNA  C"))
      return(FALSE);
   else if (!StringICmp(rname, "DNA  G"))
      return(FALSE);
   else if (!StringICmp(rname, "DNA  T"))
      return(FALSE);
   else if (!StringICmp(rname, "RNA  A"))
      return(FALSE);
   else if (!StringICmp(rname, "RNA  C"))
      return(FALSE);
   else if (!StringICmp(rname, "RNA  G"))
      return(FALSE);
   else if (!StringICmp(rname, "RNA  U"))
      return(FALSE);
   else
      return(TRUE);
}

Int4 LIBCALL findChnidx(Int4 mol_id, Int4 nbp, MoleculeGraphPtr bp)
{
   Int4 i;
   MoleculeGraphPtr currentbp;

   currentbp = bp;

   for (i=1; i<=nbp; i++)
   {
      if (currentbp->id == mol_id)
      {
         return i;
      }

      currentbp = currentbp->next;
   }

   return -1;
}

/************************************************************/
/*							    */
/*	getResNam()					    */
/*							    */
/*	Given a graph id, returns a name of corresponding   */
/*	residue.					    */
/*							    */
/************************************************************/ 

CharPtr LIBCALL getResNam(ResidueGraphPntrPtr rgpp, BiostrucResidueGraphSetPtr stdDictionary, ResidueGraphPtr rg)
{
   BiostrucResidueGraphSetPntrPtr brgsp;
   ValNodePtr vnp;
   ResidueGraphPtr rgStd;

   switch(rgpp->choice)
   {
      case ResidueGraphPntr_local:
	 
         while (rg)
	 {
	    if (rg->id == rgpp->data.intvalue)
	    {
	       vnp = rg->descr;

	       while (vnp)
	       {
		  if (vnp->choice == BiomolDescr_name)
		  {
		     return vnp->data.ptrvalue;
		  }
	       }
            }

	    rg = rg->next;
         }
	 break;
      case ResidueGraphPntr_standard:
	 rgStd = stdDictionary->residue_graphs;
	 brgsp = (BiostrucResidueGraphSetPntrPtr)rgpp->data.ptrvalue;

	 while (rgStd)
	 {
	    if (rgStd->id == brgsp->residue_graph_id)
	    {
	       vnp = rgStd->descr;

	       while (vnp)
	       {
		  if (vnp->choice == BiomolDescr_name)
		  {
		     return vnp->data.ptrvalue;
		  }
	       }
            }

	    rgStd = rgStd->next;
         }
	 break;
   }

   return NULL;
}

/************************************************************/
/*							    */
/*	getResComm()					    */
/*							    */
/*	Given a residue graph pointer, returns its comment. */
/*							    */
/************************************************************/ 

CharPtr LIBCALL getResComm(ResidueGraphPntrPtr rgpp, BiostrucResidueGraphSetPtr stdDictionary, ResidueGraphPtr rg)
{

   BiostrucResidueGraphSetPntrPtr brgsp;
   ValNodePtr vnp;
   ResidueGraphPtr rgStd;

   switch(rgpp->choice)
   {
      case ResidueGraphPntr_local:
	 while (rg)
	 {
	    if (rg->id == rgpp->data.intvalue)
	    {
	       vnp = rg->descr;

	       while (vnp)
	       {
		  if (vnp->choice == BiomolDescr_pdb_comment)
		  {
		     return vnp->data.ptrvalue;
		  }

		  vnp = vnp->next;
	       }
            }

	    rg = rg->next;
         }
	 break;
      case ResidueGraphPntr_standard:
	 rgStd = stdDictionary->residue_graphs;
	 brgsp = (BiostrucResidueGraphSetPntrPtr)rgpp->data.ptrvalue;

	 while (rgStd)
	 {
	    if (rgStd->id == brgsp->residue_graph_id)
	    {
	       vnp = rgStd->descr;

	       while (vnp)
	       {
		  if (vnp->choice == BiomolDescr_name)
		  {
		     /* Standard residue graph dictionary doesn't have
		     pdb comments as for nonstandard cases - return a name. */

		     return vnp->data.ptrvalue;
		  }

		  vnp = vnp->next;
	       }
            }

	    rgStd = rgStd->next;
         }
	 break;
   }

   return NULL;
}

Int4 LIBCALL 
getAtomElementIdx(Int4 molecule_id, Int4 residue_id, Int4 atom_id, BiostrucGraphPtr bsgp, BiostrucResidueGraphSetPtr stdDictionary)
{
   MoleculeGraphPtr mgp;
   ResiduePtr rs;
   ResidueGraphPtr rgp;
   AtomPtr atmp;
   BiostrucResidueGraphSetPntrPtr brgsp;

   mgp = bsgp->molecule_graphs;

   while (mgp)
   {
      if (mgp->id == molecule_id)
	 break;

      mgp = mgp->next;
   }

   rs = mgp->residue_sequence;

   while (rs)
   {
      if (rs->id == residue_id)
	 break;

      rs = rs->next;
   }

   if (rs->residue_graph->choice == ResidueGraphPntr_local)
   {
      rgp = bsgp->residue_graphs;

      while (rgp)
      {
	 if (rgp->id == rs->residue_graph->data.intvalue)
	 {
	    atmp = rgp->atoms;

            while (atmp != NULL)
	    {
	       if (atmp->id == atom_id)
	       {
                  return atmp->element;
               }

	       atmp = atmp->next;
            }
         }

	 rgp = rgp->next;
      }
   }
   else if (rs->residue_graph->choice == ResidueGraphPntr_standard)
   {
      rgp = stdDictionary->residue_graphs;
      brgsp = (BiostrucResidueGraphSetPntrPtr)rs->residue_graph->data.ptrvalue;

      while (rgp)
      {
	 if (rgp->id == brgsp->residue_graph_id)
	 {
	    atmp = rgp->atoms;

	    while (atmp != NULL)
	    {
	       if (atmp->id == atom_id) 
	       {
		  return atmp->element;
               }

	       atmp = atmp->next;
            }

         }

	 rgp = rgp->next;
      }
   }

   return(255);
}

/************************************************************/
/*							    */
/*	getResGraph()					    */
/*							    */
/*	Given a graph id, returns corresponding residue	    */
/*	graph.						    */
/*							    */
/************************************************************/ 

ResidueGraphPtr LIBCALL getResGraph(ResidueGraphPntrPtr rgpp, BiostrucGraphPtr bsgp, BiostrucResidueGraphSetPtr stdDictionary)
{
   ResidueGraphPtr rg;
   BiostrucResidueGraphSetPntrPtr brgsp;

   switch(rgpp->choice)
   {
      case ResidueGraphPntr_local:
	 rg = bsgp->residue_graphs;

	 while (rg)
	 {
	    if (rg->id == rgpp->data.intvalue)
	    {
	       return rg;
            }

	    rg = rg->next;
         }
	 break;
      case ResidueGraphPntr_standard:
	 rg = stdDictionary->residue_graphs;
	 brgsp = (BiostrucResidueGraphSetPntrPtr)rgpp->data.ptrvalue;

	 while (rg)
	 {
	    if (rg->id == brgsp->residue_graph_id)
	    {
	       return rg;
            }

	    rg = rg->next;
         }
	 break;
   }

   return NULL;
}
