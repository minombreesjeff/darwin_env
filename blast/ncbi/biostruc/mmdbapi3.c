/*   mmdbapi3.c
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
* File Name:  mmdbapi3.c
*
* Author:  Christopher Hogue, Hitomi Ohkawa
*
* Version Creation Date:   08/06/95
*
* $Revision: 6.20 $
*
* File Description: FlatFile Generators for PDB & Kinemage
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 08/30/95  C. Hogue    Changed the element & amino acid lookups to functions
*                       Fixed ions, hets and x-links in WriteKinAnimate,
*                       Moved globals from mmdbapi3.h here, checked over
*                       memory usage.
*
*
* $Log: mmdbapi3.c,v $
* Revision 6.20  2000/05/19 21:53:37  lewisg
* fix header formatting errors
*
* Revision 6.19  2000/05/15 23:39:33  lewisg
* shred cblast, add menu items for gapped/ungapped, fix pdbheaders
*
* Revision 6.18  2000/05/09 19:51:01  lewisg
* add new blast header to file>properties
*
* Revision 6.17  2000/05/08 20:33:29  lewisg
* get rid of fault because vastsrch returns null date
*
* Revision 6.16  2000/05/08 16:50:47  lewisg
* fix formatting bugs in pdb format
*
* Revision 6.15  2000/05/06 00:05:05  lewisg
* rework pdb dumpers to use ncbi data
*
* Revision 6.14  2000/04/27 22:21:57  lewisg
* misc bugs/features
*
* Revision 6.13  2000/03/31 22:30:47  lewisg
* fix output of CONECT, create intrabond traverser, misc bugs
*
* Revision 6.12  2000/01/31 19:59:12  lewisg
* fix output of date in pdb header
*
* Revision 6.11  2000/01/18 22:49:16  lewisg
* send OM_MSG_FLUSH to ddv/udv, tweak CPK coloration, misc bugs
*
* Revision 6.10  2000/01/14 21:40:39  lewisg
* add translucent spheres, ion labels, new cpk, fix misc bugs
*
* Revision 6.9  1999/12/22 22:49:20  lewisg
* fix color bugs: no color in color by domain, selection overrides cpk and temperature
*
* Revision 6.8  1999/10/06 16:47:47  addess
* added line feeds and space filling to END and CONECT reocrds in write pdb function
*
* Revision 6.7  1998/08/26 18:02:42  kans
* fixed -v -fd warnings
*
* Revision 6.6  1998/06/17 18:01:01  lewisg
* orange secondary structure
*
* Revision 6.5  1998/04/15 12:21:26  kans
* needed to include prtutil.h
*
* Revision 6.4  1998/04/15 00:01:00  lewisg
* fixed AuthorListPDB to work on std author names
*
* Revision 6.3  1998/03/06 23:17:24  lewisg
* codewarrior fixes
*
* Revision 6.2  1998/03/06 01:14:40  lewisg
* merge
*
* Revision 6.1  1997/12/02 16:37:41  lewisg
* added %5ld to line 498
*
* Revision 6.0  1997/08/25 18:11:09  madden
* Revision changed to 6.0
*
* Revision 5.5  1997/02/03 16:35:23  hogue
* Changed Traverser in FASTA dumper to traverse when no models.
*
 * Revision 5.4  1997/01/27  21:43:48  hogue
 * Added Code to write FASTA reports and FASTA files from biopolymer molecules
 *
 * Revision 5.3  1996/08/19  21:07:26  vakatov
 * Removed redundant(and dangerous) function castings to "pNodeFunc"
 *
 * Revision 5.2  1996/07/22  16:16:34  hogue
 * Removed an excess MemFree from WritePDBRemarks.
 *
 * Revision 5.1  1996/07/22  00:29:32  hogue
 * Added a WritePDBRemarks function for PDB file generation.
 *
 * Revision 5.0  1996/05/28  14:02:09  ostell
 * Set to revision 5.0
 *
 * Revision 1.23  1996/05/22  20:44:28  hogue
 * Changed color scheme of hydrophobes from brown to grey.
 *
 * Revision 1.22  1996/05/01  21:49:23  hogue
 * Fixed spacing problem in some PDB files HELIX records.
 *
 * Revision 1.21  1996/04/12  20:27:40  hogue
 * Added AM_POLY cases to Kinemage generators.  Altered the color scheme for structure rendering, added inter-residue bond walking on heterogen rendering passes for polymers.
 *
 * Revision 1.20  1996/03/29  19:39:30  hogue
 * Changed color schemes for NA's, temp factors and sec. structure.
 *
 * Revision 1.19  1996/02/28  17:32:12  hogue
 * Fixes to string literals being altered causing LINUX crashes...
 *
 * Revision 1.18  1996/01/31  21:38:35  hogue
 * Changed PDNSM and kin to PDNML, fixed bugs in Kinemage and PDB file generation given subsets of model lists.
 *
 * Revision 1.17  1995/10/12  17:08:57  hogue
 * Made Au & Hg the right colors.
 *
 * Revision 1.16  1995/10/05  17:42:11  hogue
 * Made Kinemage color tables public
 *
 * Revision 1.15  1995/09/21  20:27:35  hogue
 * Added code for B. Brylawski to make HTML output summary forms.
 *
 * Revision 1.14  1995/09/20  14:04:03  hogue
 * Return value for WriteStrucSummary
 *
 * Revision 1.13  1995/09/19  21:08:52  hogue
 * Added WriteStrucSummary(), Cleaned up Kinemage Text Header.
 *
 * Revision 1.12  1995/09/18  21:25:59  hogue
 * ProgMon() calls, Het names in Kin files, PDB disclaimer added.
 *
 * Revision 1.11  1995/09/05  19:12:31  hogue
 * Separated fn call out of fprintf, fixed for NULL strings, unknown elements.
 *
 * Revision 1.10  1995/09/05  15:56:08  hogue
 * Kinemage Atom-Type rendering Ion fix.
 *
 * Revision 1.9  1995/09/05  14:17:42  hogue
 * Logic fix for multiple vs single Kinemage models
 *
 * Revision 1.8  1995/09/01  14:02:09  hogue
 * NMR multiple model display bug fix.
 *
 * Revision 1.7  1995/08/31  18:50:37  hogue
 * Removed @thinline, <A> Kinemage bugs, Forced NMR animate off if 1 model only.
 *
 * Revision 1.6  1995/08/30  19:36:44  kans
 * numerous changes
 *
 * Revision 1.5  1995/08/29  20:45:54  kans
 * default bKinRender to KIN_VIRTUAL
 *
 * Revision 1.4  1995/08/29  20:03:06  kans
 * Entrez-like loading now enabled, models fixed
 *
 * Revision 1.1  1995/08/28  19:30:14  kans
 * Initial revision
 *
*
* ==========================================================================
*/
/*****************************************************************************
*
*  mmdbapi3.c
*
*  UNDER CONSTRUCTION NOTICE.
*	SUBJECT TO RADICAL CHANGES...
*
*  programmer C.W.V. Hogue
*  created: 6 AUGUST 95
*  last mod: 28 AUGUST 95
*****************************************************************************/


#include <ncbi.h>
#include <asn.h>
#include <mmdbapi.h>
#include <prtutil.h>
/*#include <mmdbapi1.h>
#include <mmdbapi2.h>
#include <mmdbapi3.h> */


/* these are declared and initialized in mmdbapi2.c */
extern CharPtr NCBIstdaaUC;
extern CharPtr NCBI4naUC;
extern CharPtr NCBI4naLC;
extern CharPtr NCBIstdaaLC;



/* numbers are addresses into KineColors */
/* this assigns a color to a residue type */
Int1 KinAAColor[MAX_NCBIstdaa] = {21,20,11,
	9,12,12,3,20,4,20,4,20,9,6,2,6,4,8,8,20,3,20,3,11,10,1 };
Int2 KinNAColor[MAX_NCBI4na] = {0,5,7,17,8,2,12,0,6,17,18,0,0,0,0,1};


Int1 KinAtoms[MAX_KIN_ATOMS] = {1,6,7,8,15,16};


/* this assigns a color to a temperature scale */
Int1 ThermKine[KIN_COLOR_THERM] = {4,4,5,6,6,7,8,9,10,11,11,13,1,12,2,0};
Int4 TempsKine[KIN_COLOR_THERM] = {0,500,1000,1500,2000,2500,3000,3500,4000,5000,
					  6000,7000,8000,9000,10000,99900};



/* this assigns a color pair for color-by-molecule number */
Int1 ColorNumKinBB[10] = {14,6,8,4,12,5,9,11,12};
Int1 ColorNumKinSC[10] = {14,7,18,3,13,17,19,10,14,20};
Int1 ColorNumKinAC[10] = {0,1,2,14,17,6,8,4,1};


CharPtr KineColors[KIN_COLOR_MAX] = {
	"default", /*0*/
	"hotpink",
	"magenta",
	"purple",
	"blue",
	"sky",   /*5*/
	"cyan",
	"sea",
	"green",
	"yellow",
	"gold", /*10*/
	"orange",
	"red",
	"pink",
	"pinktint",
	"white", /*15*/
	"black",
	"bluetint",
	"greentint",
	"yellowtint",
	"gray", /*20*/
	"brown" };

Int1 ElementKinColors[MAX_ELEMENTS]  = {
 	16,
	15, /* H */
	15, /* He */
	15, /* Li */
	15, /* Be */
	34, /* B */
	20, /* C */
	17, /* N */
	12, /* O */
	8, /* F */
	34, /* Ne */
	36, /* Na */
	15, /* Mg */
	15, /* Al */
	34, /* Si */
	11, /* P */
	19, /* S */
	8, /* Cl */
	34, /* Ar */
	35, /* K */
	15, /* Ca */
	15, /* Sc */
	15, /* Ti */
	15, /* V */
	15, /* Cr */
	15, /* Mn */
	33, /* Fe */
	15, /* Co */
	15, /* Ni */
	4, /* Cu */
	15, /* Zn */
	15, /* Ga */
	34, /* Ge */
	34, /* As */
	34, /* Se */
	21, /* Br */
	34, /* Kr */
	15, /* Rb */
	15, /* Sr */
	15, /* Y */
	15, /* Zr */
	15, /* Nb */
	15, /* Mo */
	15, /* Tc */
	15, /* Ru */
	15, /* Rh */
	15, /* Pd */
	15, /* Ag */
	15, /* Cd */
	15, /* In */
	15, /* Sn */
	15, /* Sb */
	34, /* Te */
	21, /* I */
	34, /* Xe */
	15, /* Cs */
	15, /* Ba */
	15, /* La */
	15, /* Ce */
	15, /* Pr */
	15, /* Nd */
	15, /* Pm */
	15, /* Sm */
	15, /* Eu */
	15, /* Gd */
	15, /* Tb */
	15, /* Dy */
	15, /* Ho */
	15, /* Er */
	15, /* Tm */
	15, /* Yb */
	15, /* Lu */
	15, /* Hf */
	15, /* Ta */
	15, /* W */
	15, /* Re */
	15, /* Os */
	15, /* Ir */
	15, /* Pt */
	19, /* Au */
	15, /* Hg */
	15, /* Tl */
	15, /* Pb */
	15, /* Bi */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1, /*  */
	1 /*  */};
    
    
static void MMDB_writeres(Char *pcResNum, Int4 choice)
{
    if (choice < 10000)
        sprintf(pcResNum, "%4d ", choice);
    else sprintf(pcResNum, "%5d", choice);
}


void LIBCALLBACK WriteAtomOrHet(PFB pfbThis, Int4 iModel,  Int4 iIndex,  Pointer ptr)
{  /* iIndex used to flag use for constructing ATOM or HETATM (False) records */
  PMAD pmadThis = NULL;
  PALD paldThis = NULL;
  PMGD pmgdThis = NULL;
  PMMD pmmdThis = NULL;
  CharPtr pcAlt = NULL;
  CharPtr pcChem = NULL;
  CharPtr pcRes = NULL;
  CharPtr pcTemp = NULL;
  CharPtr pcResNum = NULL;
  CharPtr pcChain = NULL;
  CharPtr pcStruc = NULL;
  FILE *pFile = NULL;
  FloatLo fIsotropic = 0.0;
  if (ptr) pFile = (FILE *) ptr;
  if (pfbThis->bMe == (Byte) AM_MAD)
    {
	  pmadThis = (PMAD) pfbThis;
	  paldThis = GetAtomLocs(pmadThis, iModel);
	  if (paldThis) /* make ATOM or HETATOM lines for all locations */
 	     {
	        /* get pointer to parent molecule and graph */
		pmgdThis = GetParentGraph((PFB) pmadThis);
		if (!pmgdThis) return;
		pmmdThis = GetParentMol((PFB) pmadThis);
		if (!pmmdThis) return;
		if (iIndex)
		   switch ((int)pmmdThis->bWhat)
		      { case AM_POLY:
			case AM_SOL:   /* make HETATMs */
			case AM_UNK:
			case AM_HET:
			case AM_ION:
			case AM_WAT:
			  return;
			default:
			;
		       }
	        else switch ((int)pmmdThis->bWhat)
		       {
		        case AM_DNA:
			case AM_RNA:     /* make ATOMS */
			case AM_PROT:
			  return;
			default:
			;
		       }

		pcResNum = (CharPtr)MemNew((size_t)6*sizeof(char));
        if(iIndex) MMDB_writeres(pcResNum, pmgdThis->pdnmgLink->choice);
        else MMDB_writeres(pcResNum, pmmdThis->pdnmmLink->choice);

 		pcChem = StringSave(pmadThis->pcAName);
		if (!pcChem) 
		  {
		    pcChem = StringSave("    ");
		    pcTemp = ElementName(pmadThis->pvnmaLink->choice);
		    if (pcTemp)
		      {
			pcChem[1] = pcTemp[0];
			if (pcTemp[1] != '\0')
			 {
			   pcChem[0] = pcTemp[0];
			   pcChem[1] = pcTemp[1];
			 }
		      }
		  }
		pcTemp = NULL;
 		pcRes = StringSave(ParentGraphPDBName((PFB) pmadThis));
	 	if (!pcRes)
		    pcRes = StringSave("UNK");
		pcTemp = pcRes;
		if ((pmgdThis->bWhat & (Byte) DICT_LOCAL) &&
		    ((pcRes[0]=='D') || (pcRes[0]=='R')) &&
		    (pcRes[1]=='N') &&
		    (pcRes[2]=='A'))
		   {
		      pcTemp = (CharPtr)&pcRes[3];
		   }
		if (StringLen(pcTemp) > 3) pcTemp[3] = '\0'; /* truncate SER COOH dict. */
		pcChain = ParentMolName((PFB) pmadThis);
		if (!pcChain) pcChain = " ";
		pcStruc = ParentStrucName((PFB) pmadThis);
		if (!pcStruc) pcStruc = "1UNK";
		while (paldThis) /* print for each location */
		   {

		     switch((int)pmmdThis->bWhat)
		       {
		       	case AM_POLY:
			case AM_SOL:
			case AM_UNK:
			case AM_HET:
			case AM_ION:
			case AM_WAT:
			  fprintf(pFile,"%6s","HETATM");
			  fflush(pFile);
			  break;
		        case AM_DNA:
			case AM_RNA:
			case AM_PROT:
			  fprintf(pFile,"%6s","ATOM  ");
			  fflush(pFile);
			}
		    pcAlt = StringSave(" ");
		    pcAlt[0] = paldThis->cAltConf;
	            if (paldThis->iFloatNo)
			{
			  fprintf(pFile,"%5ld %4s%s%3s %1s%5s   %8.3f%8.3f%8.3f",
				(long) paldThis->iUniqueId, /* Atom serial No */
				pcChem, /* Atom name "cccc" */
				pcAlt, /* Alternat loc id char "c" */
				pcTemp, /* Res name "ccc" */
				pcChain, /* Chain id char "c"*/
				pcResNum, /* Residue Sequence String "iiiic" */
				(float) paldThis->pflvData[0],
				(float) paldThis->pflvData[1],
				(float) paldThis->pflvData[2]);
			  fflush(pFile);
			}
		    else
			{  /* No locations - use 0.0 so as not to wreck the PDB file */
			  fprintf(pFile,"%5ld %4s%s%3s %1s%5s   %8.3f%8.3f%8.3f",
				(long) paldThis->iUniqueId, /* Atom serial No */
				pcChem, /* Atom name "cccc" */
				pcAlt, /* Alternat loc id char "c" */
				pcTemp, /* Res name "ccc" */
				pcChain, /* Chain id char "c"*/
				pcResNum, /* Residue Sequence String "iiiic" */
				(float) 0.0,
				(float) 0.0,
				(float) 0.0);
			   fflush(pFile);
		    	}
		    if (paldThis->iFloatNo >= 3) /* occupancy */
			fprintf(pFile,"%6.2f",(float)paldThis->pflvData[3]);
		    else
			fprintf(pFile, "  1.00");
		    fflush(pFile);
		    if (paldThis->iFloatNo == 4) /* isotropic temp */
		        {
			 fprintf(pFile,"%6.2f          %2.2s\n",
				 (float)paldThis->pflvData[4],
				 ElementName(pmadThis->pvnmaLink->choice));
		    	 fflush(pFile);
			}
		    else
		        {
	 		 if (paldThis->iFloatNo == 9) /* Anisotropic Temp factors */
			   {
			      fIsotropic = (FloatLo) (((paldThis->pflvData[4] +
						    paldThis->pflvData[5] +
						    paldThis->pflvData[6]) / 3) * 10000);
			      fprintf(pFile,"%6.2f          %2.2s\n",
				 (float)fIsotropic, /* put calculated isotropic */
		        	  ElementName(pmadThis->pvnmaLink->choice));  /* finish the ATOM string */
			      fflush(pFile);
			      fprintf(pFile,"ANISOU%5ld %4s%s%3s %1s%5s  %6d %6d %6d %6d %6d %6d\n",
				    (long) paldThis->iUniqueId, /* Atom serial No */
				    pcChem, /* Atom name "cccc" */
				    pcAlt, /* Alternat loc id char "c" */
				    pcRes, /* Res name "ccc" */
				    pcChain, /* Chain id char "c"*/
				    pcResNum, /* Residue Sequence String "iiiic" */
				    (int) (paldThis->pflvData[4] * 10000),
				    (int) (paldThis->pflvData[5] * 10000),
				    (int) (paldThis->pflvData[6] * 10000),
				    (int) (paldThis->pflvData[7] * 10000),
				    (int) (paldThis->pflvData[8] * 10000),
				    (int) (paldThis->pflvData[9] * 10000));
			      fflush(pFile);
			   }
			 else
		           { /* neither temp factors */
			      fprintf(pFile,"%6.2f          %2.2s\n",(float)0.0,
                      ElementName(pmadThis->pvnmaLink->choice));
		  	      fflush(pFile);
		           }
			} /* else from iFloatNo == 4 */

		     /* note the future PDB spec has segment id, element and charge
		      * at the end
		      */

		    paldThis = paldThis->next;
	           }
		 if (pcResNum) MemFree(pcResNum);
		 if (pcChem) MemFree(pcChem);
		 if (pcRes) MemFree(pcRes);
		 if (pcAlt) MemFree(pcAlt);
	     }
      }
}




Int2 LIBCALL WritePDBModel(Int2 iModel, Int2 iManyModels,  PDNMS pdnmsThis, FILE *pFile)
{
   Int2 iTest = 1;
   Int2 iHold = 0;
   PMSD pmsdThis = NULL;
   PDNMM pdnmmThis = NULL;
   PMMD pmmdThis = NULL;
   PDNMG pdnmgThis = NULL;


    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    iHold = SetActiveModel(PFBFromDN(pdnmsThis),iModel);
    if (iManyModels)
    {
	fprintf(pFile, "MODEL     %4d\n", (int)iModel);
	fflush(pFile);
    }
    pdnmmThis = pmsdThis->pdnmmHead; /* the top of the molecule list */
    while (pdnmmThis) /* walk the molecules individually */
      {
	ProgMon("Writing PDB");
	pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
	switch((int)pmmdThis->bWhat)
		       {
		        case AM_DNA:
			case AM_RNA:
			case AM_PROT:
			  pdnmgThis = pmmdThis->pdnmgHead;
			  iTest = TraverseOneModel(pdnmgThis, TRAVERSE_ATOM,
			      iModel,  TRUE, (Pointer) pFile,
                  WriteAtomOrHet); /* make ATOM records */
			  fprintf(pFile,"TER\n");
			  fflush(pFile);
			default:
			  ;
			}
	 pdnmmThis = pdnmmThis->next;
      }
    /* get all the Hets in this model */
    pdnmmThis = pmsdThis->pdnmmHead;
    iTest = TraverseOneModel(pdnmmThis, TRAVERSE_ATOM, iModel, FALSE, (Pointer) pFile,
    WriteAtomOrHet); /* make HET records */
    if (iManyModels)
	{
	    fprintf(pFile, "ENDMDL\n");
	}
    iHold = SetActiveModel(PFBFromDN(pdnmsThis),iHold);
    return iTest;
}




CharPtr LIBCALL AuthorListPDB(BiostrucPtr pbsThis)

/* this function gives a pretty print list of the authors of the biostruc */

{

   BiostrucDescrPtr pbsdrThis = NULL;
   BiostrucDescrPtr pbsdrLast = NULL;
   ValNodePtr pvnPub = NULL;
   CharPtr pcAuthors = NULL, pcNameString = NULL;
   CitSubPtr pcsThis = NULL;
   Boolean bFirst = TRUE;
   DataVal dvAuthors;

   pcAuthors = StringSave("");
   pbsdrThis = ValNodeFindNext(pbsThis->descr,pbsdrLast,BiostrucDescr_attribution);
   while (pbsdrThis)
   {
     pvnPub = (ValNodePtr) pbsdrThis->data.ptrvalue;
     if (pvnPub->choice == PUB_Sub)
     {
       pcsThis = (CitSubPtr) pvnPub->data.ptrvalue;
       dvAuthors.ptrvalue = pcsThis->authors; /* get the pointer to the author id's */
       pcNameString = StdAuthListNamesPrint(&dvAuthors);  /* pretty print them */
       if (pcNameString == NULL) break;
       pcAuthors = (CharPtr)MemMore( (void *) pcAuthors,(size_t)(StringLen(pcAuthors) +
         StringLen(pcNameString) + 3 ));  /* reallocate to add room for new authors */
       if (pcAuthors == NULL) break;
       if (!bFirst) StringCat(pcAuthors,"\n");   /* add a carriage return in the list */
       else bFirst = FALSE;
       StringCat(pcAuthors, pcNameString);
       MemFree(pcNameString);
     }
     pbsdrLast = pbsdrThis;
     pbsdrThis = ValNodeFindNext(pbsThis->descr,pbsdrLast,BiostrucDescr_attribution);
   }
   return pcAuthors;
}



void LIBCALL WritePDBHeader(PDNMS pdnmsThis,  FILE *pFile)
{
    
    Int4 i, depyear, depday, depmon, chrcnt, prevcnt, linecnt, back, first, adjust;
    CharPtr cmpnd, src;
    Char lastchr, nextchr;
    Boolean lastok, nextok;
    PMSD pmsdThis = NULL;
    BiostrucSourcePtr pbssThis = NULL;
    BiostrucHistoryPtr pbshThis = NULL;
    ValNodePtr pvnThis = NULL;
    ValNodePtr descr;
    Char orgstring[1024];
    OrgRefPtr org;
    
    if (!pdnmsThis) return;
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    pvnThis = ValNodeFindNext(pmsdThis->pbsBS->descr,NULL,BiostrucDescr_history);
    if (pvnThis)
    {
        pbshThis = (BiostrucHistoryPtr) pvnThis->data.ptrvalue;
        pbssThis = pbshThis->data_source;
    }
    
    /* Output a HEADER record. */
    
    fprintf(pFile,"HEADER    %s", pmsdThis->pcPdbClass);
    
    for (i = 0; i < (40 - StringLen( pmsdThis->pcPdbClass )); i++)
    {
        fprintf(pFile," ");
    }
    fflush(pFile);
    if(pbssThis) {
        depyear = pbssThis->database_entry_date->data[1];
        depday = pbssThis->database_entry_date->data[3];
        depmon = pbssThis->database_entry_date->data[2];
    } else {
        depyear = depday = 0;
        depmon = 1;
    }
    if (!pmsdThis->pcPDBName) pmsdThis->pcPDBName = StringSave("1UNK");
    
    fprintf(pFile,"%2d-%3s-%02d   %s\n",
        (int) depday,
        NCBI_months[depmon-1],
        (int) depyear%100,
        pmsdThis->pcPDBName);
    
    /* Output a COMPND record. */
    
    cmpnd = pmsdThis->pcChemName;
    chrcnt = StringLen(cmpnd);
    prevcnt = 0;
    
    linecnt = 1;
    
    while (chrcnt > 0)
    {
        fprintf(pFile, "COMPND");
        back = 0;
        if (linecnt > 1)
        {
            fprintf(pFile,"  %2d ",(int) linecnt++);
            first = 1;
        }
        else
        {
            fprintf(pFile,"    ");
            first = 0;
            linecnt++;
        }
        
        if (chrcnt > (60-first))
        {
            lastchr = cmpnd[prevcnt + 59 - first];
            nextchr = cmpnd[prevcnt + 59 - first + 1];
            lastok = (lastchr==' ') || (lastchr=='.') || (lastchr==',') || (lastchr=='\n') || (lastchr=='\t');
            nextok = (nextchr==' ') || (nextchr=='.') || (nextchr==',') || (nextchr=='\n') || (nextchr=='\t');
            if (lastok || nextok)
            {
                adjust = 0;
                for (i = 0; i < (60-first); i++)
                {
                    if (i==0 && cmpnd[prevcnt]==' ')
                    {
                        adjust = 1;
                        continue;
                    }
                    
                    fprintf(pFile,"%c",cmpnd[prevcnt+i]);
                }
                
                if (adjust == 1)
                    fprintf(pFile, " ");
                
                chrcnt = chrcnt - 60 + first;
                prevcnt = prevcnt + 60 - first;
            }
            else
            {
                i = prevcnt + 59 - first;
                
                do
                {
                    i--;
                    back++;
                }
                while ((cmpnd[i]!=' ') && (cmpnd[i]!='.') && (cmpnd[i]!=',') && (i>=0));
                
                adjust = 0;
                for (i=0; i<(60-first-back); i++)
                {
                    if (i==0 && cmpnd[prevcnt]==' ')
                    {
                        adjust = 1;
                        continue;
                    }
                    
                    fprintf(pFile,"%c",cmpnd[prevcnt+i]);
                }
                for (i=0; i<(back+adjust); i++)
                    fprintf(pFile," ");
                
                chrcnt = chrcnt - 60 + first + back;
                prevcnt = prevcnt + 60 - first - back;
            }
        }
        else
        {
            adjust = 0;
            for (i=0; i<chrcnt; i++)
            {
                if (i==0 && cmpnd[prevcnt]==' ')
                {
                    adjust = 1;
                    continue;
                }
                
                fprintf(pFile,"%c",cmpnd[prevcnt+i]);
            }
            
            for (i=0; i<(60-first-chrcnt+adjust); i++)
                fprintf(pFile," ");
            chrcnt = 0;
        }
        fprintf(pFile,"\n");
        
    }
    
    /* Output a SOURCE record. */
    
    prevcnt = 0;
    linecnt = 1;
    orgstring[0] = '\0';
    org = NULL;
    
    for (descr = ((PMMD)(pmsdThis->pdnmmHead->data.ptrvalue))->pMolDescr; descr != NULL; descr = descr->next) {
        if (descr->choice == BiomolDescr_organism)
            org = (OrgRefPtr)((BioSourcePtr)descr->data.ptrvalue)->org;           
    }
    
    if(org) 
        if(org->common) sprintf(orgstring, "ORGANISM_SCIENTIFIC: %s; ORGANISM_COMMON: %s",
            org->taxname, org->common);
        else if (org->taxname) sprintf(orgstring, "ORGANISM_SCIENTIFIC: %s",
            org->taxname);
        src = orgstring;
        chrcnt = StringLen(src);
        
        while (chrcnt > 0)
        {
            fprintf(pFile, "SOURCE");
            back = 0;
            
            if (linecnt > 1)
            {
                fprintf(pFile,"  %2d ",(int)linecnt++);
                first = 1;
            }
            else
            {
                fprintf(pFile,"    ");
                first = 0;
                linecnt++;
            }
            
            if (chrcnt > (60-first))
            {
                lastchr = src[prevcnt + 59 - first];
                nextchr = src[prevcnt + 59 - first + 1];
                lastok = (lastchr==' ') || (lastchr=='.') || (lastchr==',') || (lastchr=='\n') || (lastchr=='\t');
                nextok = (nextchr==' ') || (nextchr=='.') || (nextchr==',') || (nextchr=='\n') || (nextchr=='\t');
                if (lastok || nextok)
                {
                    adjust = 0;
                    for (i = 0; i < (60-first); i++)
                    {
                        if (i==0 && src[prevcnt]==' ')
                        {
                            adjust = 1;
                            continue;
                        }
                        
                        fprintf(pFile,"%c",src[prevcnt+i]);
                    }
                    
                    if (adjust==1)
                        fprintf(pFile, " ");
                    
                    chrcnt = chrcnt - 60 + first;
                    prevcnt = prevcnt + 60 - first;
                }
                else
                {
                    i = prevcnt + 59 - first;
                    
                    do
                    {
                        i--;
                        back++;
                    }
                    while ((src[i]!=' ') && (src[i]!='.') && (src[i]!=',') && (i>=0));
                    
                    adjust = 0;
                    for (i=0; i<(60-first-back); i++)
                    {
                        if (i==0 && src[prevcnt]==' ')
                        {
                            adjust = 1;
                            continue;
                        }
                        
                        fprintf(pFile,"%c",src[prevcnt+i]);
                    }
                    for (i=0; i<(back+adjust); i++)
                        fprintf(pFile," ");
                    
                    chrcnt = chrcnt - 60 + first + back;
                    prevcnt = prevcnt + 60 - first - back;
                }
            }
            else
            {
                adjust = 0;
                for (i=0; i<chrcnt; i++)
                {
                    if (i==0 && src[prevcnt]==' ')
                    {
                        adjust = 1;
                        continue;
                    }
                    
                    fprintf(pFile,"%c",src[prevcnt+i]);
                }
                
                for (i=0; i<(60-first-chrcnt+adjust); i++)
                    fprintf(pFile," ");
                
                chrcnt = 0;
            }
            fprintf(pFile,"\n");
            
        }
        fflush(pFile);
        /* output an AUTHOR record */
        src =  AuthorListPDB(pmsdThis->pbsBS);
        if (src) chrcnt = StringLen(src);
        else return;
        
        prevcnt = 0;
        linecnt = 1;
        
        while (chrcnt > 0)
        {
            fprintf(pFile, "AUTHOR");
            back = 0;
            
            if (linecnt > 1)
            {
                fprintf(pFile,"  %2d ",(int)linecnt++);
                first = 1;
            }
            else
            {
                fprintf(pFile,"    ");
                first = 0;
                linecnt++;
            }
            
            if (chrcnt > (60-first))
                
            {
                lastchr = src[prevcnt + 59 - first];
                nextchr = src[prevcnt + 59 - first + 1];
                lastok = (lastchr==' ') || (lastchr=='.') || (lastchr==',') || (lastchr=='\n') || (lastchr=='\t');
                nextok = (nextchr==' ') || (nextchr=='.') || (nextchr==',') || (nextchr=='\n') || (nextchr=='\t');
                if (lastok || nextok)
                {
                    adjust = 0;
                    for (i = 0; i < (60-first); i++)
                    {
                        if (i==0 && src[prevcnt]==' ')
                        {
                            adjust = 1;
                            continue;
                        }
                        
                        fprintf(pFile,"%c",src[prevcnt+i]);
                    }
                    
                    if (adjust==1)
                        fprintf(pFile, " ");
                    
                    chrcnt = chrcnt - 60 + first;
                    prevcnt = prevcnt + 60 - first;
                }
                else
                {
                    i = prevcnt + 59 - first;
                    
                    do
                    {
                        i--;
                        back++;
                    }
                    while ((src[i]!=' ') && (src[i]!='.') && (src[i]!=',') && (i>=0));
                    
                    adjust = 0;
                    for (i=0; i<(60-first-back); i++)
                    {
                        if (i==0 && src[prevcnt]==' ')
                        {
                            adjust = 1;
                            continue;
                        }
                        
                        fprintf(pFile,"%c",src[prevcnt+i]);
                    }
                    for (i=0; i<(back+adjust); i++)
                        fprintf(pFile," ");
                    
                    chrcnt = chrcnt - 60 + first + back;
                    prevcnt = prevcnt + 60 - first - back;
                }
            }
            else
            {
                adjust = 0;
                for (i=0; i<chrcnt; i++)
                {
                    if (i==0 && src[prevcnt]==' ')
                    {
                        adjust = 1;
                        continue;
                    }
                    
                    fprintf(pFile,"%c",src[prevcnt+i]);
                }
                
                for (i=0; i<(60-first-chrcnt+adjust); i++)
                    fprintf(pFile," ");
                chrcnt = 0;
            }
            fprintf(pFile,"\n");
   
        }
        fflush(pFile);
        if (src) MemFree(src);
        return;
}






void LIBCALL WritePDBMotifs(PDNMS pdnmsThis,  FILE *pFile)
{
    CharPtr pcMolnam = NULL;
    CharPtr pcResnambeg = NULL;
    CharPtr pcResnamend = NULL;
    CharPtr pcResnumbeg = NULL;
    CharPtr pcResnumend = NULL;
    CharPtr pcSSName = NULL;
    PMSD pmsdThis = NULL;
    PDNMM pdnmmThis = NULL;
    PMMD pmmdThis = NULL;
    PDNMG pdnmgThis = NULL;
    PMGD pmgdThis = NULL;
    Int2 iHlxcnt = 0;
    Int2 iStcnt = 0;
    Int2 iTrncnt = 0;
    Boolean bInHelix = FALSE;
    Boolean bInSheet = FALSE;
    Boolean bInTurn = FALSE;
    PMGD pmgdStart = NULL;
    PMGD pmgdEnd = NULL;
    
    
    if (!pdnmsThis) return;
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    pdnmmThis = pmsdThis->pdnmmHead; /* the top of the molecule list */
#ifdef _DEBUG_3
    printf("Helix scan...\n");
#endif
    /* scan for helices */
    while (pdnmmThis) /* walk the molecules individually */
    {
        pmgdEnd = NULL;
        pmgdStart = NULL;
        pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
        if ((int)pmmdThis->bWhat == AM_PROT)
        {
            bInHelix = FALSE;
            pdnmgThis = pmmdThis->pdnmgHead;
            while (pdnmgThis)
            {
                pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
                if (pmgdThis->bNCBISecStru & (Byte) SS_HELIX)
                {
                    if (bInHelix != TRUE)
                    {
#ifdef _DEBUG_3
                        printf("Helix found...\n");
#endif
                        pmgdStart = pmgdThis;
                        bInHelix = TRUE;
                    } /* not in a helix */
                } /* this residue is in a helix */
                if ((!(pmgdThis->bNCBISecStru & (Byte) SS_HELIX)) || (pdnmgThis->next == NULL))
                {
                    if (bInHelix == TRUE)
                    {
                        bInHelix =  FALSE;
                        if (pmgdStart)
                        {
#ifdef _DEBUG_3
                            printf("Writing Helix record...\n");
#endif
                            if ((pmgdThis->pdnmgLink->last) && (!(pmgdThis->bNCBISecStru & (Byte) SS_HELIX)))
                                pmgdEnd = (PMGD) pmgdThis->pdnmgLink->last->data.ptrvalue;
                            if ((pdnmgThis->next == NULL) && (pmgdThis->bNCBISecStru & (Byte) SS_HELIX))
                                pmgdEnd = pmgdThis;
                            /* write out the helix record... */
                            pcSSName = StringSave("   ");
                            if (pmgdStart->pcNCBISS)
                            {
                                sprintf(pcSSName, "%3.3s", pmgdStart->pcNCBISS);;
                            }
                            if (StringLen(pcSSName)> 3) pcSSName[3] = '\0';
                            pcResnambeg = StringSave(pmgdStart->pcGraphName);
                            if (!pcResnambeg)
                                pcResnambeg = StringSave("UNK");
                            if (StringLen(pcResnambeg) > 3) pcResnambeg[3] = '\0';
                            pcResnumbeg = StringSave("     ");
                            if (pmgdStart->pdnmgLink)
                            {
                                MMDB_writeres(pcResnumbeg, pmgdStart->pdnmgLink->choice);
                            }
                            
                            pcResnamend = StringSave(pmgdEnd->pcGraphName);
                            if (!pcResnamend) pcResnamend = StringSave("UNK");
                            if (StringLen(pcResnamend) > 3) pcResnamend[3] = '\0';
                            pcResnumend = StringSave("     ");
                            if (pmgdEnd->pdnmgLink)
                            {
                                MMDB_writeres(pcResnumend, pmgdEnd->pdnmgLink->choice);
                            }
                            pcMolnam = pmmdThis->pcMolName;
                            if (!pcMolnam) pcMolnam = " ";
                            fprintf(pFile,  "HELIX  %3d %3s %3s %1s %5s %3s %1s %5s\n",
                                (int) (++iHlxcnt),
                                pcSSName,
                                pcResnambeg,
                                pcMolnam,
                                pcResnumbeg,
                                pcResnamend,
                                pcMolnam,
                                pcResnumend);
                            fflush(pFile);
                            if (pcResnambeg) MemFree(pcResnambeg);
                            if (pcResnumbeg) MemFree(pcResnumbeg);
                            if (pcSSName) MemFree(pcSSName);
                            if (pcResnamend) MemFree(pcResnamend);
                            if (pcResnumend) MemFree(pcResnumend);
                            pmgdEnd =  NULL;
                            pmgdStart =  NULL;
                            
                        } /* there was a last record */
                        else
                        { /* reset - some wacky error */
                            bInHelix =  FALSE;
                            pmgdEnd =  NULL;
                            pmgdStart =  NULL;
                        }
                    } /* helix already started */
                }  /* this res not in a helix */
                pdnmgThis = pdnmgThis->next;
            }
        } /* if protien */
        pdnmmThis = pdnmmThis->next;
    } /* next molecule */
    pmgdStart = NULL;
    pmgdEnd = NULL;
    pdnmmThis = pmsdThis->pdnmmHead; /* top of the molecules */
    /* scan for sheets */
    while (pdnmmThis) /* walk the molecules individually */
    {
        pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
        if ((int)pmmdThis->bWhat == AM_PROT)
        {
            bInSheet = FALSE;
            pdnmgThis = pmmdThis->pdnmgHead;
            while (pdnmgThis)
            {
                pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
                if (pmgdThis->bNCBISecStru & (Byte) SS_STRAND)
                {
                    if (bInSheet != TRUE)
                    {
                        pmgdStart = pmgdThis;
                        bInSheet = TRUE;
                    } /* not in a sheet */
                } /* this residue is in a sheet */
                if ( (!(pmgdThis->bNCBISecStru & (Byte) SS_STRAND))
                    || (pdnmgThis->next == NULL))
                {
                    if (bInSheet == TRUE)
                    {
                        bInSheet =  FALSE;
                        if (pmgdStart)
                        {
#ifdef _DEBUG_3
                            printf("Writing sheet record...\n");
#endif
                            if ((pmgdThis->pdnmgLink->last) &&
                                (!(pmgdThis->bNCBISecStru & (Byte) SS_STRAND)) )
                                pmgdEnd = (PMGD) pmgdThis->pdnmgLink->last->data.ptrvalue;
                            if ((pdnmgThis->next == NULL) &&
                                (pmgdThis->bNCBISecStru & (Byte) SS_STRAND) )
                                pmgdEnd = pmgdThis;
                            
                            /* write out the sheet record... */
                            pcSSName = StringSave("   ");
                            if (pmgdStart->pcNCBISS)
                            {
                                sprintf(pcSSName, "%3.3s", pmgdStart->pcNCBISS);
                                
                            }
                            if (StringLen(pcSSName)> 3) pcSSName[3] = '\0';
                            pcResnambeg = StringSave(pmgdStart->pcGraphName);
                            if (!pcResnambeg)
                                pcResnambeg = StringSave("UNK");
                            if (StringLen(pcResnambeg) > 3) pcResnambeg[3] = '\0';
                            pcResnumbeg = StringSave("     ");
                            if (pmgdStart->pdnmgLink)
                            {
                                MMDB_writeres(pcResnumbeg, pmgdStart->pdnmgLink->choice);
                            }

                            pcResnamend = StringSave(pmgdEnd->pcGraphName);
                            if (!pcResnamend)
                                pcResnamend = StringSave("UNK");
                            if (StringLen(pcResnamend) > 3) pcResnamend[3] = '\0';
                            pcResnumend = StringSave("     ");
                            if (pmgdEnd->pdnmgLink)
                            {
                                MMDB_writeres(pcResnumend, pmgdEnd->pdnmgLink->choice);
                            }
                            pcMolnam = pmmdThis->pcMolName;
                            if (!pcMolnam) pcMolnam = " ";
                            fprintf(pFile,  "SHEET  %3d %3s%2s %3s %1s%5s %3s %1s%5s\n",
                                (int) (++iStcnt),
                                pcSSName,
                                "  ", /* number of strands in the sheet */
                                pcResnambeg,
                                pcMolnam,
                                pcResnumbeg,
                                pcResnamend,
                                pcMolnam,
                                pcResnumend);
                            fflush(pFile);
                            if (pcResnambeg) MemFree(pcResnambeg);
                            if (pcResnumbeg) MemFree(pcResnumbeg);
                            if (pcResnamend) MemFree(pcResnamend);
                            if (pcResnumend) MemFree(pcResnumend);
                            if (pcResnamend) MemFree(pcSSName);
                            pmgdEnd = NULL;
                            pmgdStart = NULL;
                            
                        } /* there was a last record */
                        else
                        { /* reset - some wacky error */
                            bInSheet = FALSE;
                            pmgdEnd = NULL;
                            pmgdStart = NULL;
                        }
                    } /* sheet already started */
                }  /* this res not in a sheet */
                pdnmgThis = pdnmgThis->next;
            }
        } /* if protien */
        pdnmmThis = pdnmmThis->next;
    } /* next molecule */
    pmgdStart = NULL;
    pmgdEnd = NULL;
    pdnmmThis = pmsdThis->pdnmmHead; /* top of the molecules */
    /* scan for turns */
    while (pdnmmThis) /* walk the molecules individually */
    {
        pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
        if ((int)pmmdThis->bWhat == AM_PROT)
        {
            bInTurn = FALSE;
            pdnmgThis = pmmdThis->pdnmgHead;
            while (pdnmgThis)
            {
                pmgdThis = (PMGD) pdnmgThis->data.ptrvalue;
                if (pmgdThis->bNCBISecStru & (Byte) SS_TURN)
                {
                    if (bInTurn != TRUE)
                    {
                        pmgdStart = pmgdThis;
                        bInTurn = TRUE;
                    } /* not in a turn */
                } /* this residue is in a turn */
                if ((!(pmgdThis->bNCBISecStru & (Byte) SS_TURN)) || (pdnmgThis->next == NULL))
                {
                    if (bInTurn == TRUE)
                    {
                        bInTurn =  FALSE;
                        if (pmgdStart)
                        {
#ifdef _DEBUG_3
                            printf("Writing turn record...\n");
#endif
                            if ((pmgdThis->pdnmgLink->last) && (!(pmgdThis->bNCBISecStru &(Byte) SS_TURN)))
                                pmgdEnd = (PMGD) pmgdThis->pdnmgLink->last->data.ptrvalue;
                            if ((pdnmgThis->next == NULL) &&  (pmgdThis->bNCBISecStru & (Byte) SS_TURN))
                                pmgdEnd = pmgdThis;
                            /* write out the turn record... */
                            pcSSName = StringSave("   ");
                            if (pmgdStart->pcNCBISS)
                            {
                                sprintf(pcSSName, "%3.3s", pmgdStart->pcNCBISS);
                            }
                            if (StringLen(pcSSName)> 3) pcSSName[3] = '\0';
                            pcResnambeg = StringSave(pmgdStart->pcGraphName);
                            if (!pcResnambeg)
                                pcResnambeg = StringSave("UNK");
                            if (StringLen(pcResnambeg) > 3) pcResnambeg[3] = '\0';
                            pcResnumbeg = StringSave("     ");
                            if (pmgdStart->pdnmgLink)
                            {
                                MMDB_writeres(pcResnumbeg, pmgdStart->pdnmgLink->choice);
                            }
                            pcResnamend = StringSave(pmgdEnd->pcGraphName);
                            if (!pcResnamend)
                                pcResnamend = StringSave("UNK");
                            if (StringLen(pcResnamend) > 3) pcResnamend[3] = '\0';
                            pcResnumend = StringSave("     ");
                            if (pmgdEnd->pdnmgLink)
                            {
                                MMDB_writeres(pcResnumend, pmgdEnd->pdnmgLink->choice);
                            }
                            pcMolnam = pmmdThis->pcMolName;
                            if (!pcMolnam) pcMolnam = " ";
                            fprintf(pFile, "TURN   %3d %3s %3s %1s%5s %3s %1s%5s\n",
                                (int) (++iTrncnt),
                                pcSSName,
                                pcResnambeg,
                                pcMolnam,
                                pcResnumbeg,
                                pcResnamend,
                                pcMolnam,
                                pcResnumend);
                            fflush(pFile);
                            if (pcResnambeg) MemFree(pcResnambeg);
                            if (pcResnumbeg) MemFree(pcResnumbeg);
                            if (pcResnamend) MemFree(pcResnamend);
                            if (pcResnumend) MemFree(pcResnumend);
                            if (pcSSName) MemFree(pcSSName);
                            pmgdEnd = NULL;
                            pmgdStart = NULL;
                            
                        } /* there was a last record */
                        else
                        { /* reset - some wacky error */
                            bInTurn = FALSE;
                            pmgdEnd = NULL;
                            pmgdStart = NULL;
                        }
                    } /* turn already started */
                }  /* this res not in a turn */
                pdnmgThis = pdnmgThis->next;
            } /* next graph */
        } /* if protien */
        pdnmmThis = pdnmmThis->next;
    } /* next molecule */
    return;
}


void LIBCALL WritePDBRemarks(PDNMS pdnmsThis,  FILE *pFile)
{

   PMSD pmsdThis = NULL;
   BiostrucDescrPtr pbsdrThis = NULL;
   BiostrucDescrPtr pbsdrLast = NULL;
   Char pcThis[81];
   int len;

   if (!pdnmsThis) return;
   if (!pFile) return;

   pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;

   if (!pmsdThis->pbsBS) return;
   if (!pmsdThis->pbsBS->descr) return;
    fprintf(pFile,"REMARK  00 NCBI PDB FORMAT VERSION 5.0\n");
    fprintf(pFile,"REMARK  00 NOTE:  NCBI-MMDB PDB-Format File derived from ASN.1\n");
    fprintf(pFile,"REMARK  00 Refer to original ASN.1 file or PDB file for data records\n");
    fprintf(pFile,"REMARK  00 not included.  Changes from original PDB files include:\n");
    fprintf(pFile,"REMARK  00 - residues are derived by graph matching algorithms.\n");
    fprintf(pFile,"REMARK  00 - residue numbers always increase numerically (e.g. no 8A).\n");
    fprintf(pFile,"REMARK  00 - taxonomic information reannotated using NCBI Taxonomy.\n");
    fprintf(pFile,"REMARK  00 - element names are made consistent.\n");
    fprintf(pFile,"REMARK  00 - CONECT records are derived algorithmically.\n");
    fprintf(pFile,"REMARK  00 - Secondary structure is derived algorithmically.\n");
    fflush(pFile);
#if 0
    /* we decided not to print out comments since they may refer to residues that
       have been renumbered */
    pbsdrThis = ValNodeFindNext(pmsdThis->pbsBS->descr,
         pbsdrLast, BiostrucDescr_pdb_comment);
    while (pbsdrThis)
     {
        StringNCpy(pcThis, (CharPtr) pbsdrThis->data.ptrvalue,80);
        StringUpper(pcThis);
	len = StringLen(pcThis);
	while (len < 72)
	  {
	      len++;
	      StringCat(pcThis," ");
	  }
	if (pcThis[10] == ':') pcThis[10] = ' ';  /* how'd these horrible colons get there! */
        fprintf(pFile,"%s%s\n",pcThis, pmsdThis->pcPDBName);
        fflush(pFile);
	pbsdrLast = pbsdrThis;
	pbsdrThis = ValNodeFindNext(pmsdThis->pbsBS->descr,pbsdrLast,
                   BiostrucDescr_pdb_comment);
     }
#endif /* 0 */
  return;
}


void LIBCALL WritePDBSeqRes(PDNMS pdnmsThis,  FILE *pFile)
{
   Int4 resnum,linecnt,rest,curResCnt,i;
   CharPtr resnam = NULL;
   PDNMG pdnmgRes = NULL;
   PMSD pmsdThis = NULL;
   PDNMM pdnmmThis = NULL;
   PMMD pmmdThis = NULL;
   PMGD pmgdThis = NULL;


    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    pdnmmThis = pmsdThis->pdnmmHead; /* the top of the molecule list */

    while (pdnmmThis) /* walk the molecules individually */
      {
	pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
	switch((int)pmmdThis->bWhat)
	 {
	  case AM_DNA:
	  case AM_RNA:
	  case AM_PROT:
		resnum = pmmdThis->iResCount;
		linecnt = 1;
 	 	rest = resnum;
	 	pdnmgRes = pmmdThis->pdnmgHead;
	 	while ((rest > 0) && (pdnmgRes != NULL))
	 	  {
	    		fprintf(pFile,"SEQRES  %2d %s %4d  ",
				(int) linecnt,
				pmmdThis->pcMolName,
				(int)resnum);
			fflush(pFile);
			linecnt++;
			if (rest > 13)
	       		    curResCnt = 13;
			else
			    curResCnt = rest;
			i = 0;
			while ((i < curResCnt) && (pdnmgRes != NULL))
			 {
			    pmgdThis = (PMGD) pdnmgRes->data.ptrvalue;
			    resnam = StringSave(pmgdThis->pcGraphName);
			    if (!resnam)
				resnam = StringSave("UNK");
			    if ((pmgdThis->bWhat & (Byte) DICT_LOCAL) &&
			        ((resnam[0]=='D') || (resnam[0]=='R')) &&
				(resnam[1]=='N') &&
				(resnam[2]=='A'))
				{
				    fprintf(pFile,"%3s ",(CharPtr)&resnam[3]);
				    fflush(pFile);
				}
			    else
			        {
				    if (StringLen(resnam) > 3) resnam[3] = '\0';
					/* truncate SER COOH dict. */
				    fprintf(pFile,"%3s ",resnam);
				    fflush(pFile);
				}
			    if (resnam) MemFree(resnam);
			    i++;
			    pdnmgRes = pdnmgRes->next;
			  }
			rest -= curResCnt;
            fprintf(pFile,"\n");
			fflush(pFile);
		  } /* while pdnmgRes */
		break;
	  default:
	    ;
	 } /* switch */
	 pdnmmThis = pdnmmThis->next;
      } /* while pdnmmThis */
    return;
}




void LIBCALLBACK PDBConnect(PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr)
{
/* when iIndex == 1 it looks for HETs */
/* when iIndex == 0 it does all bonds it encounters */
/* FALSE used with TraverseIBonds  */
  PMBD pmbdThis = NULL;
  PMMD pmmdFrom = NULL;
  PMMD pmmdTo = NULL;
  PALD paldFrom = NULL;
  PMLD pmldThis = NULL;
  PALD paldTo = NULL;
  PMSD pmsdThis = NULL;
  PDNML pdnmlThis = NULL;
  FILE *pFile = NULL;
  CharPtr pcThis = NULL;
  if (ptr) pFile = (FILE *) ptr;
  else return;
  if (pfbThis->bMe == (Byte) AM_MBD)
      {
	  pmbdThis = (PMBD) pfbThis;
	  pmmdFrom = (GetParentMol((PFB) pmbdThis->pmadFrom));
	  pmmdTo = (GetParentMol((PFB) pmbdThis->pmadTo));
	  if (iIndex == 1)  /* do the HETS only */
	   {
	    if (!
	        ((pmmdFrom->bWhat == (Byte) AM_HET) ||
                 (pmmdFrom->bWhat == (Byte) AM_POLY) ||
		(pmmdFrom->bWhat == (Byte) AM_SOL) ||
		(pmmdFrom->bWhat == (Byte) AM_ION) ||
		(pmmdFrom->bWhat == (Byte) AM_WAT) ||
		(pmmdTo->bWhat == (Byte) AM_HET) ||
                (pmmdTo->bWhat == (Byte) AM_POLY) ||
		(pmmdTo->bWhat == (Byte) AM_SOL) ||
		(pmmdTo->bWhat == (Byte) AM_WAT) ||
		(pmmdTo->bWhat == (Byte) AM_ION)) ) return;
	   }
      /* get rid of solvent solvent bonds */
      if((pmmdFrom->bWhat == (Byte) AM_SOL || pmmdFrom->bWhat == (Byte) AM_WAT)
          && (pmmdFrom->bWhat == (Byte) AM_SOL || pmmdFrom->bWhat == (Byte) AM_WAT)) return;
	  if (iIndex == 0) /* don't connect the backbone stupid */
	    {
  	     if ((pmbdThis->pmadFrom->bWhat & (Byte) AM_BACKBONE) ||
	         (pmbdThis->pmadTo->bWhat & (Byte) AM_BACKBONE))
		  {
		       return;
		  }
	    }
	  pmsdThis = ToMSDParent(pfbThis);
	  pdnmlThis = DValNodeFindNext(pmsdThis->pdnmlModels, NULL, iModel); /* find the model */
 	  if (pdnmlThis)
   	    {  /* this connects all alternative conformers recorded */
	      pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
	      pcThis = pmldThis->pcAltConf;
	      if (pcThis)
	      while (StringLen(pcThis))
	        {
		     paldFrom = GetLocation(GetAtomLocs(pmbdThis->pmadFrom, iModel), 0, (Char) pcThis[0]);
		     paldTo = GetLocation(GetAtomLocs(pmbdThis->pmadTo, iModel), 0, (Char) pcThis[0] );
		     if (paldFrom && paldTo)
			{  /* only does the first one in a model */
			    /* have to match up above stuff */
			    fprintf(pFile, "CONECT%5ld%5ld                  \n",
				(long) paldFrom->iUniqueId,
				(long) paldTo->iUniqueId);
			    fflush(pFile);
			}
		    paldFrom = NULL;
		    paldTo = NULL;
		    pcThis = (CharPtr) &pcThis[1];
		}
	     else
  		{
		     paldFrom = GetLocation(GetAtomLocs(pmbdThis->pmadFrom, iModel), 0, ' ');
		     paldTo = GetLocation(GetAtomLocs(pmbdThis->pmadTo, iModel), 0, ' ');
		     if (paldFrom && paldTo)
			{  /* only does the first one in a model */
			    /* have to match up above stuff */
			    fprintf(pFile, "CONECT%5ld%5ld                  \n",
				(long) paldFrom->iUniqueId,
				(long) paldTo->iUniqueId);
			    fflush(pFile);
			}
		}

	    } /* does the model have a location? */
    }
}




void LIBCALL WritePDBConnect(PDNMS pdnmsThis,  Int2 iModel, FILE *pFile)
{

   Int2 iTest;
   /* inter-res and inter-molecule bond pass */
   iTest = TraverseOneModel(pdnmsThis, TRAVERSE_IBOND, iModel,
         0, pFile, PDBConnect);

  /* HET pass */
   iTest = TraverseOneModel(pdnmsThis, TRAVERSE_INTRABOND, iModel,
         1, pFile,  PDBConnect);
   return;
}



Int2 LIBCALL WritePDBModelList(PDNMS pdnmsThis,  FILE *pFile,  Int2 iNumModels,  Int2Ptr i2Vec )
{
    Int4 iCount;
    Int2 iIndex;
    Int2 iTest;
    Int2 iManyModels = TRUE;
    iCount = AssignAtomLocId(pdnmsThis);  /* !!!MOVE THESE OUT TO WRAPPER */
    AssignBackBone(pdnmsThis);
	/* printf("Location count = %ld\n", (long) iCount); */
    ProgMon("Writing PDB");
    WritePDBHeader(pdnmsThis, pFile);
    ProgMon("Writing PDB");
    WritePDBRemarks(pdnmsThis, pFile);
    WritePDBSeqRes(pdnmsThis, pFile);
    WritePDBMotifs(pdnmsThis, pFile);
    if (iNumModels == 1) iManyModels = FALSE;
    for (iIndex = 0; iIndex < iNumModels; iIndex++)
       {
	   ProgMon("Writing PDB");
	   iTest = WritePDBModel(i2Vec[iIndex], iManyModels,  pdnmsThis, pFile);
	   if (!iTest) return 0;
       }
    for (iIndex = 0; iIndex < iNumModels; iIndex++)
       {
	   WritePDBConnect(pdnmsThis, i2Vec[iIndex], pFile);
       }
    fprintf(pFile, "END\n");
    fflush(pFile);
    return 1;
}


Int2 LIBCALL WritePDBOneModel(PDNMS pdnmsThis,  FILE *pFile,  Int2 iModel)
{
    Int2 iRet = FALSE;
    iRet = WritePDBModelList(pdnmsThis, pFile, 1, (Int2Ptr) &iModel);
    return iRet;
}

Int2 LIBCALL WritePDBAllModel(PDNMS pdnmsThis,  FILE *pFile)
{
    Int2Ptr i2Vec = NULL;
    PDNML pdnmlThis = NULL;
    PMLD pmldThis = NULL;
    PMSD pmsdThis = NULL;
    Int2 iCount = 0;
    Int2 iRet = FALSE;

    pmsdThis = (PMSD)  PFBFromDN(pdnmsThis);
    if (!(pmsdThis->bMe == (Byte) AM_MSD)) return iRet;

    pdnmlThis = pmsdThis->pdnmlModels; /* the linked-list of models */
    if (pdnmlThis)
      while (pdnmlThis)
       {
	 pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
	 if (pmldThis)
 	 if (pmldThis->iType != Model_type_ncbi_vector)
	   {
         	iCount++;
           }
	 pdnmlThis = pdnmlThis->next;

       }
    else return 0;  /* no models */
    i2Vec = I2Vector(0, iCount);
    if (!i2Vec) return iRet;
    iCount = 0;
    pdnmlThis = pmsdThis->pdnmlModels;
    while (pdnmlThis)
	{  /* copy the model id's into the vector */
	   pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
	   if (pmldThis)
 	   if (pmldThis->iType != Model_type_ncbi_vector) /* keep out vector models */
	     {
     	  	i2Vec[iCount] = pdnmlThis->choice;
	    	iCount++;
	     }
	    pdnmlThis = pdnmlThis->next;
	}
    iRet = WritePDBModelList(pdnmsThis, pFile, iCount, i2Vec);
    I2VectorFree(i2Vec, 0);
    return iRet;

}





/**************/
/* Kinemage   */
/**************/

CharPtr LIBCALL GetKinMolName(PMMD pmmdThis)
{
  CharPtr rsult = "mol";

  switch ((int) pmmdThis->bWhat)
	    {
	      case AM_RNA:
	         return "rna";
 	      case AM_DNA:
	         return "dna";
  	      case AM_PROT:
	         return "pep";
 	      case AM_HET:
              case AM_POLY:
	        return "het";
 	      case AM_ION:
	        return "ion";
 	      default:
	        rsult = "mol";
	    }
  return rsult;
}









CharPtr LIBCALL WriteKinSeq(PDNMS pdnmsThis,  FILE *pFile)
{  /* writes out the sequence and also generates the composition string  */
   Int4 resnum,linecnt,rest,curResCnt,i;
   CharPtr resnam;
   PDNMG pdnmgRes;
   PMSD pmsdThis = NULL;
   PDNMM pdnmmThis = NULL;
   PMMD pmmdThis = NULL;
   PMGD pmgdThis = NULL;
   CharPtr pcComposition = NULL;
   CharPtr pcKinMolName = NULL;
   CharPtr pcTest = NULL;
   CharPtr pcComp = NULL;
   Int4 iLen = 0;

    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    pdnmmThis = pmsdThis->pdnmmHead; /* the top of the molecule list */
    pcComposition = (CharPtr)MemNew((size_t) (MAX_NCBIstdaa + MAX_NCBI4na + 2));
    pcComposition[0] = '\0';
    fprintf(pFile, "\nSequence(s) as they appear in the PDB file.\n");
    fflush(pFile);
    while (pdnmmThis) /* walk the molecules individually */
      {
	pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
	switch((int)pmmdThis->bWhat)
	 {
	  case AM_HET:
                pdnmgRes = pmmdThis->pdnmgHead;
		pmgdThis = (PMGD) pdnmgRes->data.ptrvalue;
		if (pmgdThis->pcPDBComment)
		  {
		    fprintf(pFile,"\nHeterogen {%d het} Name: %s\n",
				(int)pdnmmThis->choice,
				pmgdThis->pcPDBComment);
		  }
	        break;
         case AM_POLY:
	        pdnmgRes = pmmdThis->pdnmgHead;
		pmgdThis = (PMGD) pdnmgRes->data.ptrvalue;
		if (pmgdThis->pcPDBComment)
		  {
		    fprintf(pFile,"\nBiopolymer {%d het} Name: %s\n",
				(int)pdnmmThis->choice,
				pmgdThis->pcPDBComment);
		  }
	        break;
	  case AM_DNA:
	  case AM_RNA:
         case AM_PROT:
		resnum = pmmdThis->iResCount;
		linecnt = 1;
 	 	rest = resnum;
	 	pdnmgRes = pmmdThis->pdnmgHead;
	        pcKinMolName = GetKinMolName(pmmdThis);
		fprintf(pFile,"\nSequence {%d %s %s} has %d residues:\n",
				(int)pdnmmThis->choice,
				pcKinMolName,
				pmmdThis->pcMolName,
				(int)resnum);

			fflush(pFile);
	 	while ((rest > 0) && (pdnmgRes != NULL))
	 	  {

			linecnt++;
			if (rest > 10)
	       		    curResCnt = 10;
			else
			    curResCnt = rest;
			i = 0;
			while ((i < curResCnt) && (pdnmgRes != NULL))
			 {
			    pmgdThis = (PMGD) pdnmgRes->data.ptrvalue;
			    resnam = StringSave(pmgdThis->pcGraphName);
			    if (!resnam)
				resnam = StringSave("UNK");
			    pcTest = NULL;
			    if (pmgdThis->bWhat & (Byte) RES_AA)
  			     {
				pcTest = StringChr(NCBIstdaaUC,pmgdThis->pcIUPAC[0]);

			     }
			    if ((pmgdThis->bWhat & (Byte) RES_RNA) || (pmgdThis->bWhat & (Byte) RES_DNA))
			     {
				pcTest = StringChr(NCBI4naUC,pmgdThis->pcIUPAC[0]);
				if (pcTest)
				  {
				      pcComp = NCBI4naUC;
				      iLen = 0;
				      while ((pcComp != pcTest) && (pcComp))
				        {
					    iLen++;
					    pcComp = (CharPtr) &pcComp[1];
					}
				      pcTest = (CharPtr) &NCBI4naLC[iLen];
				  }
			     }
			    if (!pcTest) pcTest = "?";
			    if (!StringChr(pcComposition, pcTest[0]))
			        {
				  StringNCat(pcComposition, pcTest, 1);
#ifdef _DEBUG_4
printf("%s\n", pcComposition);
#endif
			        }
		            if ((pmgdThis->bWhat & (Byte) DICT_LOCAL) &&
			        ((resnam[0]=='D') || (resnam[0]=='R')) &&
				(resnam[1]=='N') &&
				(resnam[2]=='A'))
				{
				    fprintf(pFile,"%3s ",(CharPtr)&resnam[3]);
				    fflush(pFile);
				}
			    else
			        {
				    if (StringLen(resnam) > 3) resnam[3] = '\0';
					/* truncate SER COOH dict. */
				    fprintf(pFile,"%3s ",resnam);
				    fflush(pFile);
				}
			    if (resnam) MemFree(resnam);
			    i++;
			    pdnmgRes = pdnmgRes->next;
			  }
			rest -= curResCnt;
 			fprintf(pFile," \n");
			fflush(pFile);
		  } /* while pdnmgRes */
		break;
	  default:
	    ;
	 } /* switch */
	 pdnmmThis = pdnmmThis->next;
      } /* while pdnmmThis */
    return pcComposition;
}


void LIBCALL WriteKinHeader(PDNMS pdnmsThis,  FILE *pFile)
{

   Int4 depyear, depday;
   PMSD pmsdThis = NULL;
   BiostrucSourcePtr pbssThis = NULL;
   ValNodePtr pvnThis = NULL;
   BiostrucHistoryPtr pbshThis = NULL;
   CharPtr pcAuthors = NULL;

   if (!pdnmsThis) return;
   pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
   pvnThis = ValNodeFindNext(pmsdThis->pbsBS->descr,NULL,BiostrucDescr_history);
   if (pvnThis)
     {
      pbshThis = (BiostrucHistoryPtr) pvnThis->data.ptrvalue;
      pbssThis = pbshThis->data_source;
     }
   if (!pmsdThis->pcPDBName) pmsdThis->pcPDBName = StringSave("1UNK");
   fprintf(pFile,"Structure Summary\n\n");
   fprintf(pFile,"Contents:\n%s\n\n", pmsdThis->pcChemName);
   fflush(pFile);
   fprintf(pFile,"MMDB Accession:\n%ld\n\n", (long) pmsdThis->iMMDBid);
   fprintf(pFile,"PDB Accession:\n%s\n\n", pmsdThis->pcPDBName);
   fflush(pFile);
   if (pbssThis)
     {
   	depyear = pbssThis->database_entry_date->data[1];
   	depday = pbssThis->database_entry_date->data[3];
    if(pbssThis->database_entry_date->data[2] >= 1)
   	fprintf(pFile,"PDB Deposition:\n%2d-%3s-%02d\n\n",
                   (int) depday,
                   NCBI_months[pbssThis->database_entry_date->data[2]-1],
                   (int) depyear%100);
     }
  fprintf(pFile,"Class:\n%s\n\n", pmsdThis->pcPdbClass);
  fflush(pFile);
  fprintf(pFile,"Source:\n%s\n\n", pmsdThis->pcPdbSource);
  fflush(pFile);
  pcAuthors =  AuthorListPDB(pmsdThis->pbsBS) ;
  if (pcAuthors)
    {
      fprintf(pFile,"Authors:\n%s\n\n", (CharPtr) &pcAuthors[1]);
      fflush(pFile);
      MemFree(pcAuthors);
    }

  return;
}




void LIBCALL AddLineKin(FILE *pFile,  PALD paldFrom,  PALD paldTo)
{
   CharPtr pcAtomFrom = NULL;
   CharPtr pcAtomTo = NULL;
   CharPtr pcGraphFrom = NULL;
   CharPtr pcGraphTo = NULL;
   CharPtr pcTempFrom = NULL;
   CharPtr pcTempTo = NULL;
   PMGD pmgdFrom = NULL;
   PMGD pmgdTo = NULL;
   PMAD pmadFrom = NULL;
   PMAD pmadTo = NULL;

   pmadFrom = (PMAD) paldFrom->pfbParent;
   pmadTo = (PMAD) paldTo->pfbParent;
   if ((!pmadFrom) || (!pmadTo)) return;
   pcAtomFrom = StringSave(pmadFrom->pcAName);
   if (!pcAtomFrom)
     pcAtomFrom = StringSave(ElementName((Int1)pmadFrom->pvnmaLink->choice));
   pcAtomTo = StringSave(pmadTo->pcAName);
   if (!pcAtomTo)
     pcAtomTo = StringSave(ElementName((Int1)pmadTo->pvnmaLink->choice));
   pcTempFrom = StringSave(ParentGraphPDBName((PFB)pmadFrom));
   if (!pcTempFrom)
	pcTempFrom = StringSave("UNK");
   pmgdFrom = GetParentGraph((PFB)pmadFrom);
   pcGraphFrom = pcTempFrom;
   if (pmgdFrom)
   if ((pmgdFrom->bWhat & (Byte) DICT_LOCAL) &&
       ((pcTempFrom[0]=='D') || (pcTempFrom[0]=='R')) &&
       (pcTempFrom[1]=='N') &&
       (pcTempFrom[2]=='A'))
     {
       pcGraphFrom = (CharPtr)&pcTempFrom[3];
     }
   if (StringLen(pcGraphFrom) > 3) pcGraphFrom[3] = '\0'; /* truncate SER COOH dict. */

   pcTempTo= StringSave(ParentGraphPDBName((PFB)pmadTo));
   if (!pcTempTo)
	pcTempTo = StringSave("UNK");
   pmgdTo = GetParentGraph((PFB)pmadTo);
   pcGraphTo = pcTempTo;
   if (pmgdTo)
   if ((pmgdTo->bWhat & (Byte) DICT_LOCAL) &&
       ((pcTempTo[0]=='D') || (pcTempTo[0]=='R')) &&
       (pcTempTo[1]=='N') &&
       (pcTempTo[2]=='A'))
     {
       pcGraphTo = (CharPtr)&pcTempTo[3];
     }
   if (StringLen(pcGraphTo) > 3) pcGraphTo[3] = '\0'; /* truncate SER COOH dict. */

   fprintf(pFile,"{%s %s %d} P%8.3f,%8.3f,%8.3f {%s %s %d} L%8.3f,%8.3f,%8.3f\n",
		    pcAtomFrom,
		    pcGraphFrom,
		    (int) IndexFromNode((PFB)pmgdFrom),
		    (float) paldFrom->pflvData[0],
		    (float) paldFrom->pflvData[1],
		    (float) paldFrom->pflvData[2],
		    pcAtomTo,
		    pcGraphTo,
		    (int) IndexFromNode((PFB)pmgdTo),
		    (float) paldTo->pflvData[0],
		    (float) paldTo->pflvData[1],
		    (float) paldTo->pflvData[2]);
   fflush(pFile);
   if (pcAtomFrom) MemFree(pcAtomFrom);
   if (pcAtomTo) MemFree(pcAtomTo);
   if (pcTempTo) MemFree(pcTempTo);
   if (pcTempFrom) MemFree(pcTempFrom);
   return;
}


void LIBCALLBACK WriteKinVirt(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr)
{

  PMBD pmbdThis = NULL;
  PALD paldFrom = NULL;
  PALD paldTo = NULL;
  FILE *pFile = NULL;

  if (!pfbThis) return;
  if (ptr) pFile = (FILE *) ptr;
  if (pfbThis->bMe == (Byte) AM_MBD)
      {
        pmbdThis = (PMBD) pfbThis;
	if (!(pmbdThis->bWhat & (Byte) BOND_VIRTUAL)) return;
	paldFrom = GetLocation(GetAtomLocs(pmbdThis->pmadFrom, iModel), 0, ' ');
	paldTo = GetLocation(GetAtomLocs(pmbdThis->pmadTo, iModel), 0, ' ');
	 if (paldFrom && paldTo)
	    {
              AddLineKin(pFile, paldFrom, paldTo);
           }
      }
  return;
}

void LIBCALLBACK WriteKinHet(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr)
{

  PMBD pmbdThis = NULL;
  PALD paldFrom = NULL;
  PMMD pmmdFrom = NULL;
  PMMD pmmdTo = NULL;
  PALD paldTo = NULL;
  FILE *pFile = NULL;


  if (!pfbThis) return;
  if (ptr) pFile = (FILE *) ptr;
    else return;
  if (pfbThis->bMe == (Byte) AM_MBD)
      {
        pmbdThis = (PMBD) pfbThis;
	pmmdFrom = GetParentMol((PFB)pmbdThis->pmadFrom);
	pmmdTo = GetParentMol((PFB)pmbdThis->pmadTo);
	if (!((pmmdFrom->bWhat & (Byte) AM_HET) ||
	   (pmmdFrom->bWhat & (Byte) AM_ION) ||
           (pmmdFrom->bWhat & (Byte) AM_POLY) ||
	   (pmmdTo->bWhat & (Byte) AM_HET) ||
           (pmmdTo->bWhat & (Byte) AM_POLY) ||
	   (pmmdTo->bWhat & (Byte) AM_ION))) return;
	paldFrom = GetLocation(GetAtomLocs(pmbdThis->pmadFrom, iModel), 0, ' ');
	paldTo = GetLocation(GetAtomLocs(pmbdThis->pmadTo, iModel), 0, ' ');
		/* these should call GetLocation and match AltConf/CoordSet */
	 if (paldFrom && paldTo)
	    {
              AddLineKin(pFile, paldFrom, paldTo);
           }
      }
  return;
}



void LIBCALLBACK WriteKinBB(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr)
{

  PMBD pmbdThis = NULL;
  PALD paldFrom = NULL;
  PALD paldTo = NULL;
  FILE *pFile = NULL;


  if (!pfbThis) return;
  if (ptr) pFile = (FILE *) ptr;
  if (pfbThis->bMe == (Byte) AM_MBD)
      {
        pmbdThis = (PMBD) pfbThis;
	if (!((pmbdThis->pmadFrom->bWhat & (Byte) AM_BACKBONE)
	      && (pmbdThis->pmadTo->bWhat & (Byte) AM_BACKBONE))) return;
	      /* both have to be backbone atoms */
	if (pmbdThis->bWhat & (Byte) BOND_VIRTUAL) return;
	     /* discard the virtual ones */
	paldFrom = GetLocation(GetAtomLocs(pmbdThis->pmadFrom, iModel), 0, ' ');
	paldTo = GetLocation(GetAtomLocs(pmbdThis->pmadTo, iModel), 0, ' ');
	 if (paldFrom && paldTo)
	    {
              AddLineKin(pFile, paldFrom, paldTo);
           }
      }
  return;
}


void LIBCALLBACK WriteKinRes(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr)
{

  PMBD pmbdThis = NULL;
  PALD paldFrom = NULL;
  PALD paldTo = NULL;
  FILE *pFile = NULL;


  if (!pfbThis) return;
  if (ptr) pFile = (FILE *) ptr;
  if (pfbThis->bMe == (Byte) AM_MBD)
      {
        pmbdThis = (PMBD) pfbThis;
	if (((pmbdThis->pmadFrom->bWhat & (Byte) AM_BACKBONE)
	      && (pmbdThis->pmadTo->bWhat & (Byte) AM_BACKBONE))) return;
	      /* no bonds connecting two backbone residues */
	if (pmbdThis->bWhat & (Byte) BOND_VIRTUAL) return;
	     /* discard the virtual ones */
	paldFrom = GetLocation(GetAtomLocs(pmbdThis->pmadFrom, iModel), 0, ' ');
	paldTo = GetLocation(GetAtomLocs(pmbdThis->pmadTo, iModel), 0, ' ');
		/* these should call GetLocation and match AltConf/CoordSet */
	 if (paldFrom && paldTo)
	    {
              AddLineKin(pFile, paldFrom, paldTo);
           }
      }
  return;
}

void LIBCALLBACK WriteKinAAResType(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr)
{
   PMGD pmgdThis = NULL;
   PVNMB pvnmbThis = NULL;
   FILE *pFile = NULL;


   if (!pfbThis) return;
   if (ptr) pFile = (FILE *) ptr;

   if (pfbThis->bMe == (Byte) AM_MGD)
     {
       pmgdThis = (PMGD) pfbThis;
       if (!(pmgdThis->bWhat & (Byte) RES_AA)) return;
       if (pmgdThis->pcIUPAC[0] == NCBIstdaaUC[iIndex])
         {
	   pvnmbThis = pmgdThis->pvnmbBHead;
	   while (pvnmbThis)
	     {
		 WriteKinRes((PFB) pvnmbThis->data.ptrvalue, iModel,  0,  pFile);
		 pvnmbThis = pvnmbThis->next;
	     }
	 }
     }
}


void LIBCALLBACK WriteKinNAResType(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr)
{
   PMGD pmgdThis = NULL;
   PVNMB pvnmbThis = NULL;
   FILE *pFile = NULL;


   if (!pfbThis) return;
   if (ptr) pFile = (FILE *) ptr;
   if (pfbThis->bMe == (Byte) AM_MGD)
     {
       pmgdThis = (PMGD) pfbThis;
       if (!((pmgdThis->bWhat & (Byte) RES_RNA) || (pmgdThis->bWhat & (Byte) RES_DNA))) return;
       if (pmgdThis->pcIUPAC[0] == NCBI4naUC[iIndex])
         {
           pvnmbThis = pmgdThis->pvnmbBHead;
	   while (pvnmbThis)
	     {
		 WriteKinRes((PFB) pvnmbThis->data.ptrvalue, iModel,  0,  pFile);
		 pvnmbThis = pvnmbThis->next;
	     }
	 }
     }
}


void LIBCALLBACK WriteKinAlt(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr)
{
  PMBD pmbdThis = NULL;
  PALD paldFrom = NULL;
  PALD paldTo = NULL;
  PMMD pmmdFrom = NULL;
  PMMD pmmdTo = NULL;
  FILE *pFile = NULL;


  if (!pfbThis) return;
  if (ptr) pFile = (FILE *) ptr;
  if (pfbThis->bMe == (Byte) AM_MBD)
      {
        pmbdThis = (PMBD) pfbThis;
	if (pmbdThis->bWhat & (Byte) BOND_VIRTUAL) return;

	paldFrom = GetLocation(GetAtomLocs(pmbdThis->pmadFrom, iModel), 0, (char) iIndex);
	paldTo = GetLocation(GetAtomLocs(pmbdThis->pmadTo, iModel), 0, (char) iIndex);
	if  ((paldFrom) && (paldTo == NULL))
	   paldTo =  GetAtomLocs(pmbdThis->pmadTo, iModel);
	else
	if  ((paldTo) && (paldFrom == NULL))
	   paldFrom =  GetAtomLocs(pmbdThis->pmadFrom, iModel);
	if (paldFrom && paldTo)
	    {
	      pmmdFrom = GetParentMol((PFB) pmbdThis->pmadFrom);
	      pmmdTo = GetParentMol((PFB) pmbdThis->pmadTo);
	      if (pmmdFrom->bWhat & (Byte) AM_SOL) return; /* do no solvent */
	      if (pmmdTo->bWhat & (Byte) AM_SOL) return; /* do no solvent */
	      AddLineKin(pFile, paldFrom, paldTo);
            }
      }  /* is the node a bond */
  return;
}






void LIBCALLBACK WriteKinIon(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr)
{

  PMAD pmadThis = NULL;
  PALD paldFrom = NULL;
  FILE *pFile = NULL;
  CharPtr pcElName = NULL;


  if (!pfbThis) return;
  if (ptr) pFile = (FILE *) ptr;
  if (pfbThis->bMe == (Byte) AM_MAD)
      {
        pmadThis = (PMAD) pfbThis;
	if (!(pmadThis->bUpdate & (Byte) AM_ION)) return;
	if (pmadThis->pvnmaLink->choice > MAX_ELEMENTS) return;
	paldFrom = GetAtomLocs(pmadThis, iModel);
	while (paldFrom)
	  {
	    pcElName = NULL;
	    pcElName = ElementName((Int1)pmadThis->pvnmaLink->choice);
	    if (!pcElName) pcElName = "?";
	    fprintf(pFile, "@ballist {%s} color= %s radius= %3.1f nobutton master= {%s}\n",
		    pcElName,
		    KineColors[(int)ElementKinColors[(int)pmadThis->pvnmaLink->choice]],
		    (float) (ElementSize((Int1)pmadThis->pvnmaLink->choice)),
		    pcElName);
	    fflush(pFile);
   	    fprintf(pFile, "{%s}  %8.3f, %8.3f, %8.3f \n",
			pcElName,
			paldFrom->pflvData[0],
			paldFrom->pflvData[1],
			paldFrom->pflvData[2]);
	    fflush(pFile);
	    paldFrom = paldFrom->next;
	  }
      }
  return;
}


void LIBCALLBACK WriteKinAtom(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr)
{
   /* this does atomic coloring using lines from XYZ to midpoint of bond */
   /* deals with atomic type iIndex <0 or temperature iIndex >=0 */
  PMAD pmadThis = NULL;
  PMAD pmadTo = NULL;
  PMAD pmadFrom = NULL;
  PMBD pmbdThis = NULL;
  PALD paldFrom = NULL;
  PALD paldTo = NULL;
  PALD paldMid = NULL;
  PVNMB pvnmbThis = NULL;
  PMMD pmmdFrom = NULL;
  FILE *pFile = NULL;
  Int4 iTemp;
  FloatLo flAtemp;
  Int1 iElem;
  CharPtr pcElName = NULL;


  if (!pfbThis) return;
  if (ptr) pFile = (FILE *) ptr;
  if (pfbThis->bMe == (Byte) AM_MAD)
      {
        pmadThis = (PMAD) pfbThis;
	pmmdFrom = GetParentMol((PFB)pmadThis);
	if (pmmdFrom->bWhat & (Byte) AM_SOL) return; /* do no solvent */
	paldFrom = GetLocation(GetAtomLocs(pmadThis, iModel),0,' ');
	if (paldFrom)
	  {
           if (iIndex >= 0) /* index of temp range 1 to (KIN_COLOR_THERM-1) */
             {
               if (paldFrom->iFloatNo == 4) /* istotropic */
                 {
		    iTemp = (Int4) paldFrom->pflvData[4] * 100;
		 }
	       if (paldFrom->iFloatNo == 9) /* anisotropic */
	         {  /* calculate the isotropic temp factor */
		     flAtemp = (FloatLo) (((paldFrom->pflvData[4] +
					    paldFrom->pflvData[5] +
					    paldFrom->pflvData[6]) / 3));
		     iTemp = (Int4) flAtemp * 100;
		 }
	       if (iTemp > TempsKine[iIndex]) return;  /* too high */
	       if (iTemp < TempsKine[(iIndex-1)]) return; /* too low */
	     }
	   if (iIndex < 0) /* we are coloring by atom type */
	     {  /* we cannily stuffed the element type in as a negative index */
		 if (pmadThis->bUpdate & (Byte) AM_ION) return; /* done in a later pass */
	         iElem = (Int1) -iIndex;
		 if (pmadThis->pvnmaLink->choice != iElem) return;  /* too easy! */
	     }

	   pvnmbThis = pmadThis->pvnBonds;
	    /* no bonds, not solvent atom needs a Temp sphere - color set in group */
	   /*
	   if ((pvnmbThis == NULL) && (iIndex >= 0))
	     {
		    pcElName = NULL;
		    pcElName = ElementName((Int1)pmadThis->pvnmaLink->choice);
		    if (!pcElName) pcElName = "?";
		    fprintf(pFile, "@ballist {%s} radius= %3.1f nobutton master= {%s}\n",
			pcElName,
			(float) (ElementSize((Int1)pmadThis->pvnmaLink->choice)),
			pcElName);
		    fflush(pFile);
   		    fprintf(pFile, "{%s}  %8.3f, %8.3f, %8.3f \n",
			pcElName,
			paldFrom->pflvData[0],
			paldFrom->pflvData[1],
			paldFrom->pflvData[2]);
		    fflush(pFile);
		    return;
	     } */
	   while (pvnmbThis)
	     {     /* walk the atom's bond list - all non-virtual ones */
		 pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
		 if (!(pmbdThis->bWhat & (Byte) BOND_VIRTUAL))
		   { /* make a half-way bond */
		     pmadTo = pmbdThis->pmadTo;
		     pmadFrom = pmadThis;
		     if (pmadFrom == pmadTo) /* not the right one ? */
		         pmadTo = pmbdThis->pmadFrom; /* use the other one */
		     paldTo = GetAtomLocs(pmadTo,  iModel); /* just the first one */
		     if (paldTo)
		       {
		         paldMid = NewALD(); /* allocate a phoney location */
		         if (!paldMid) return;
		         paldMid->pfbParent = (PFB) pmadFrom; /* link to the "from" atom */
		         paldMid->iFloatNo = (Int1) 3;
		         paldMid->cAltConf = ' ';
		         paldMid->pflvData = FLVector(0,3);
		         if (!paldMid->pflvData) return;
			  /* midpoint between the two atoms */
		         paldMid->pflvData[0] = (paldTo->pflvData[0] + paldFrom->pflvData[0]) / 2.0;
		         paldMid->pflvData[1] = (paldTo->pflvData[1] + paldFrom->pflvData[1]) / 2.0;
		         paldMid->pflvData[2] = (paldTo->pflvData[2] + paldFrom->pflvData[2]) / 2.0;
		         if (paldMid && paldFrom)
			   {
			    AddLineKin(pFile, paldFrom, paldMid);
			   }
		         fflush(pFile);
		         FreeALD(paldMid);
		      }
		    }
		 pvnmbThis = pvnmbThis->next;
	     }
	  } /* if paldFrom */
      }  /* if AM_MAD */
  return;
}



Int2 LIBCALL WriteKinAnimate(Int2 iModel,  PDNMS pdnmsThis,
		FILE *pFile,  Int2 iColor, Byte bKinRender, CharPtr pcComposition)
{
   Int2 iTest = 1;
   Int2 iHold = 0;
   PMSD pmsdThis = NULL;
   PDNMM pdnmmThis = NULL;
   PMMD pmmdThis = NULL;
   PMBD pmbdThis = NULL;
   PVNMB pvnmbThis = NULL;
   CharPtr pcColor = NULL;
   Boolean bFirst = TRUE;
   CharPtr pcKinMolName = NULL;

    /* this ignores ensembles - only animates models */
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    iHold = SetActiveModel(PFBFromDN(pdnmsThis),iModel);


 	  fprintf(pFile, "@group {model %d} animate dominant\n",iModel);
	  fflush(pFile);
	  if (bKinRender & (Byte) KIN_VIRTUAL)
	    {
		 fprintf(pFile, "@subgroup {virtual} master= {Virtual}\n");
		 fflush(pFile);
		 /* walk the molecules */
		 pdnmmThis = pmsdThis->pdnmmHead;
		 while (pdnmmThis)
		   {
		       ProgMon("Writing Kinemage");
		       pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
		       if ((pmmdThis->bWhat == AM_RNA) ||
			    (pmmdThis->bWhat == AM_DNA) ||
			    (pmmdThis->bWhat == AM_PROT))
			  {
			     pcKinMolName = GetKinMolName(pmmdThis);
			     pcColor = KineColors[ColorNumKinBB[(pdnmmThis->choice % KIN_COLOR_NUM)]];
			     fprintf(pFile, "@vectorlist {%s %s %d} color= %s\n",pcKinMolName,
					 pmmdThis->pcMolName, (int)pdnmmThis->choice , pcColor);
			     fflush(pFile);
			     iTest = TraverseOneModel(pmmdThis->pdnmgHead,TRAVERSE_BOND,iModel,0,pFile,
                        WriteKinVirt);
			     pvnmbThis = pmmdThis->pvnmbIRBHead;
			     while (pvnmbThis) /* walk the IRBonds by hand */
				{
				    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
				    WriteKinVirt((PFB) pmbdThis, iModel,  0,  pFile);
				    pvnmbThis = pvnmbThis->next;
				}

			  }

		       pdnmmThis = pdnmmThis->next;
		   }

	    }
	  pdnmmThis = pmsdThis->pdnmmHead;
	  if (bKinRender & (Byte) KIN_BACKBONE)
	    {
		fprintf(pFile, "@subgroup {backbone}");
		if (bKinRender & (Byte) KIN_VIRTUAL) fprintf(pFile, " master= {Real} off\n");
		else fprintf(pFile, "\n");
		fflush(pFile);
		/* walk the molecules */
		 while (pdnmmThis)
		   {
		       pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
		       if ((pmmdThis->bWhat == AM_RNA) ||
			    (pmmdThis->bWhat == AM_DNA) ||
			    (pmmdThis->bWhat == AM_PROT))
			  {
			     pcKinMolName = GetKinMolName(pmmdThis);
			     pcColor = KineColors[ColorNumKinBB[(pdnmmThis->choice % KIN_COLOR_NUM)]];
			     fprintf(pFile, "@vectorlist {%s %s %d} color= %s\n",pcKinMolName,
					 pmmdThis->pcMolName, (int)pdnmmThis->choice , pcColor);
			     fflush(pFile);
			     iTest = TraverseOneModel(pmmdThis->pdnmgHead,TRAVERSE_BOND,iModel,0,pFile,
                        WriteKinBB);
			     pvnmbThis = pmmdThis->pvnmbIRBHead;
			     while (pvnmbThis) /* walk the IRBonds by hand */
				{
				    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
				    WriteKinBB((PFB) pmbdThis, iModel,  0,  pFile);
				    pvnmbThis = pvnmbThis->next;
				}

			  }

		       pdnmmThis = pdnmmThis->next;
		   }
	    }
	  pdnmmThis = pmsdThis->pdnmmHead;
	  if (bKinRender & (Byte) KIN_RESIDUE)
	    {
		fprintf(pFile, "@subgroup {residues}");
		if (bKinRender & (Byte) KIN_VIRTUAL) fprintf(pFile, " master= {Residues} off\n");
		else fprintf(pFile, "\n");
		fflush(pFile);
		/* walk the molecules */
		 while (pdnmmThis)
		   {
		       ProgMon("Writing Kinemage");
		       pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
		       if ((pmmdThis->bWhat == AM_RNA) ||
			    (pmmdThis->bWhat == AM_DNA) ||
			    (pmmdThis->bWhat == AM_PROT))
			  {
			     pcKinMolName = GetKinMolName(pmmdThis);
			     pcColor = KineColors[ColorNumKinSC[(pdnmmThis->choice % KIN_COLOR_NUM)]];
			     fprintf(pFile, "@vectorlist {%s %s %d} color= %s\n",pcKinMolName,
					 pmmdThis->pcMolName, (int)pdnmmThis->choice , pcColor);
			     fflush(pFile);
			     iTest = TraverseOneModel(pmmdThis->pdnmgHead,TRAVERSE_BOND,iModel,0,pFile,
                        WriteKinRes);
			     fprintf(pFile, "@vectorlist {x-links} nobutton color= yellow master= {X-links} \n");
			     fflush(pFile);
			     pvnmbThis = pmmdThis->pvnmbIRBHead;
			     while (pvnmbThis) /* walk the IMBonds by hand */
				{
				    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
				    WriteKinRes((PFB) pmbdThis, iModel,  0,  pFile);
				    pvnmbThis = pvnmbThis->next;
				}
			  }

		       pdnmmThis = pdnmmThis->next;
		   }
	    }
	  pdnmmThis = pmsdThis->pdnmmHead;
	  if (bKinRender & (Byte) KIN_HET)
	    {
	          bFirst = TRUE;
		 /* walk the molecules */
		  while (pdnmmThis)
		   {
		        pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
			if (pmmdThis)
		        if ((pmmdThis->bWhat == AM_HET) ||
                            (pmmdThis->bWhat == AM_POLY) ||
			    (pmmdThis->bWhat == AM_ION))
			  {
			     if (bFirst)
			       {
				    fprintf(pFile, "@subgroup {hets} master= {Hets}\n");
				    fflush(pFile);
				    bFirst = FALSE;
			       }
			     pcColor = KineColors[ColorNumKinBB[(pdnmmThis->choice % KIN_COLOR_NUM)]];
			     pcKinMolName = GetKinMolName(pmmdThis);
			     fprintf(pFile, "@vectorlist {%s %d} color= %s\n",pcKinMolName,
					(int)pdnmmThis->choice , pcColor);
			     fflush(pFile);
			     iTest = TraverseOneModel(pmmdThis->pdnmgHead,TRAVERSE_BOND,iModel,0,pFile,
                        WriteKinHet);
			     pvnmbThis = pmmdThis->pvnmbIRBHead;
			     while (pvnmbThis) /* walk the IRBonds by hand */
				{
				    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
				    WriteKinHet((PFB) pmbdThis, iModel,  0,  pFile);
				    pvnmbThis = pvnmbThis->next;
				}
			  }

		       pdnmmThis = pdnmmThis->next;
		   }
	    }
	 if ((bKinRender & (Byte) KIN_HET) && (pmsdThis->bWhat & (Byte) AM_ION))
           {
	     fprintf(pFile,  "@subgroup {Ions} \n");
	     fflush(pFile);
      	     iTest = TraverseOneModel(pmsdThis->pdnmmHead, TRAVERSE_ATOM,
					iModel, iColor, (Pointer) pFile,
                    WriteKinIon);
	     fflush(pFile);
	   }
	    /* do the rest of the inter-molecule bonds */
	  pvnmbThis = pmsdThis->pvnmbIMBHead;
	  if (((bKinRender & (Byte) KIN_HET)  ||
	      (bKinRender & (Byte) KIN_RESIDUE)) && (pvnmbThis))
	    {
	         fprintf(pFile, "@subgroup {bonds}\n");
		 fflush(pFile);
		 if ((bKinRender & (Byte) KIN_HET)  ||  (bKinRender & (Byte) KIN_RESIDUE))
		   {
		     fprintf(pFile, "@subgroup {x-links} off\n");
		     fprintf(pFile, "@vectorlist {x-links} color= yellow master= {X-links}");
		     fflush(pFile);
		     while (pvnmbThis) /* walk the IMBonds by hand */
		     {
		      pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
		      if (bKinRender & (Byte) KIN_HET) WriteKinHet((PFB) pmbdThis, iModel,  0,  pFile);
		      if (bKinRender & (Byte) KIN_RESIDUE) WriteKinRes((PFB) pmbdThis, iModel,  0,  pFile);
	 	      pvnmbThis = pvnmbThis->next;
		     }
		   }
	    }

    iHold = SetActiveModel(PFBFromDN(pdnmsThis),iHold);
    return iTest;
}

Int2 LIBCALL KinColorFromSS(PMGD pmgdThis)
{
	Int2 rsult = 0;

      switch ((int) (pmgdThis->bNCBISecStru))
       {
	  case SS_HELIX:
	     return 8;
	  case SS_SHEET:
	  case SS_STRAND:
	     return 10;
	  case SS_TURN:
	     return 11;
	  default:
	     rsult = 6;
	}
   return rsult;
}

CharPtr LIBCALL NameFromSS(PMGD pmgdThis)
{
	CharPtr rsult = NULL;

      switch ((int) (pmgdThis->bNCBISecStru))
       {
	  case SS_HELIX:
	     return "Helix";
	  case SS_SHEET:
	  case SS_STRAND:
	     return "Sheet";
	  case SS_TURN:
	     return "Turn";
	  default:
	     rsult = "Prot";
	}
   return rsult;
}

Int2 LIBCALL WriteKinModelByType(Int2 iModel,  PDNMS pdnmsThis,
		FILE *pFile,  Int2 iColor, Byte bKinRender, CharPtr pcComposition)

{  /* this does coloring by secondary structure and by residue type */

    PMSD pmsdThis = NULL;
    PDNMM pdnmmThis = NULL;
    PDNMG pdnmgThis = NULL;
    PMMD pmmdThis = NULL;
    PMGD pmgdFrom = NULL;
    PMBD pmbdThis = NULL;
    PVNMA pvnmaThis = NULL;
    PVNMB pvnmbThis = NULL;
    PMAD pmadFrom = NULL;
    PMAD pmadThis = NULL;
    PMAD pmadTo = NULL;
    PALD paldMid = NULL;
    PALD paldTo = NULL;
    PALD paldFrom = NULL;
    Int2 iHold = 0;
    Int4 iIndex;
    Int2 iTest = 0;
    Boolean bFirst = TRUE;
    Byte bOldSS = 0x00;
    CharPtr pcColor = NULL;
    CharPtr pcKinMolName = NULL;

    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    iHold = SetActiveModel(PFBFromDN(pdnmsThis),iModel);

    pdnmmThis = pmsdThis->pdnmmHead; /* the top of the molecule list */

      /* individual models... */

    while (pdnmmThis)  /* walk the molecules individually - each has a group */
      {
	   ProgMon("Writing Kinemage");
	   bFirst = TRUE;
	   pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;

	   if ((pmmdThis->bWhat == AM_RNA) ||
		(pmmdThis->bWhat == AM_DNA) ||
		(pmmdThis->bWhat == AM_PROT))
	    {
	        pcKinMolName = GetKinMolName(pmmdThis);
		fprintf(pFile, "@group {%d %s %s}\n",(int)pdnmmThis->choice,
		  pcKinMolName,  pmmdThis->pcMolName);
		fflush(pFile);
	   	if (bKinRender & (Byte) KIN_VIRTUAL)
		  {
		    bOldSS = (Byte) 0x00;
		    fprintf(pFile, "@subgroup {BB Virtual} dominant\n");
 		    pdnmgThis = pmmdThis->pdnmgHead;
		    while (pdnmgThis)
		      {   /* walk the graphs */
			   pmgdFrom = (PMGD) pdnmgThis->data.ptrvalue;
			   if ((bOldSS != (Byte) (pmgdFrom->bNCBISecStru)) || bFirst)
			      {  /* change in sec stru detected */
			        bFirst = FALSE;
			        if (pmmdThis->bWhat == AM_RNA)
				     fprintf(pFile, "@vectorlist {rna} color= %s master= {NA}\n",
				         KineColors[KinColorFromSS(pmgdFrom)]);
				if (pmmdThis->bWhat == AM_DNA)
				      fprintf(pFile, "@vectorlist {dna} color= %s master= {NA}\n",
					 KineColors[KinColorFromSS(pmgdFrom)]);
				if (pmmdThis->bWhat == AM_PROT)
				    fprintf(pFile, "@vectorlist {%s} color= %s master= {%s}\n",
					NameFromSS(pmgdFrom),  KineColors[KinColorFromSS(pmgdFrom)],
					NameFromSS(pmgdFrom));
			        fflush(pFile);
			      }
			   bOldSS = (Byte) (pmgdFrom->bNCBISecStru);
		           pvnmaThis = pmgdFrom->pvnmaAHead;
			   while (pvnmaThis)
			     {  /* find the Virtual bonds for each graph */
			         pmadThis = (PMAD) pvnmaThis->data.ptrvalue;
				 pvnmbThis = pmadThis->pvnBonds;
				 while (((pmadThis->bWhat & (Byte) AM_CALPHA) || (pmadThis->bWhat & (Byte) AM_PALPHA))
					    && (pvnmbThis))
				   {
				    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
				    if (pmbdThis->bWhat & (Byte) BOND_VIRTUAL)
				    {
					pmadFrom = pmbdThis->pmadFrom;
					if (pmgdFrom != GetParentGraph((PFB) pmadFrom))
					    {
						pmadFrom = pmbdThis->pmadTo;
						pmadTo = pmbdThis->pmadFrom;
					    }
					else
					    pmadTo = pmbdThis->pmadTo;
					if ((pmadTo) && (pmadFrom))
					{
					    paldTo = NULL;
					    paldMid = NULL;
					    paldFrom = NULL;
					    paldTo = GetLocation(GetAtomLocs(pmadTo,  iModel),0,' ');
					    paldFrom = GetLocation(GetAtomLocs(pmadFrom, iModel), 0,' ');
					    paldMid = NewALD(); /* allocate a phoney location */
					    if (!paldMid) return -1;
					    if (paldTo && paldFrom)
					     {
						paldMid->pfbParent =(PFB) pmadFrom; /* link to the "from" atom */
						paldMid->iFloatNo = (Int1) 3;
						paldMid->cAltConf = ' ';
						paldMid->pflvData = FLVector(0, 3);
						if (!paldMid->pflvData) return -2;
						/* midpoint between the two atoms */
						paldMid->pflvData[0] = (paldTo->pflvData[0] + paldFrom->pflvData[0]) / 2.0;
						paldMid->pflvData[1] = (paldTo->pflvData[1] + paldFrom->pflvData[1]) / 2.0;
						paldMid->pflvData[2] = (paldTo->pflvData[2] + paldFrom->pflvData[2]) / 2.0;
						AddLineKin(pFile, paldFrom, paldMid);
						fflush(pFile);
					      }

					    FreeALD(paldMid);
					  }

					}  /* if virtual bond */
				      pvnmbThis = pvnmbThis->next;
				    }  /* while alpha carbon or P and bonds */
				 pvnmaThis = pvnmaThis->next;
			     } /* while atom */
			   pmadFrom = NULL;
			   pdnmgThis = pdnmgThis->next;
		      }  /* while graphs */
		  }  /* if virtual render */
		bFirst = TRUE;
		if (bKinRender & (Byte) KIN_BACKBONE)
		  {
		    bOldSS = (Byte) 0x00;
		    fprintf(pFile, "@subgroup {BB Real} dominant ");
		    if (bKinRender & (Byte) KIN_VIRTUAL) fprintf(pFile,  "off\n");
		    else fprintf(pFile, "\n");
		    fflush(pFile);
 		    pdnmgThis = pmmdThis->pdnmgHead;
		    while (pdnmgThis)
		      {   /* walk the graphs */
			   pmgdFrom = (PMGD) pdnmgThis->data.ptrvalue;
			   if ((bOldSS != (Byte) (pmgdFrom->bNCBISecStru)) || bFirst)
			      {  /* change in sec stru detected */
			        bFirst = FALSE;
			        if (pmmdThis->bWhat == AM_RNA)
				     fprintf(pFile, "@vectorlist {rna} color= %s master= {NA}\n",
				         KineColors[KinColorFromSS(pmgdFrom)]);
				if (pmmdThis->bWhat == AM_DNA)
				      fprintf(pFile, "@vectorlist {dna} color= %s master= {NA}\n",
					 KineColors[KinColorFromSS(pmgdFrom)]);
				if (pmmdThis->bWhat == AM_PROT)
				    fprintf(pFile, "@vectorlist {%s} color= %s master= {%s}\n",
					NameFromSS(pmgdFrom),  KineColors[KinColorFromSS(pmgdFrom)],
					NameFromSS(pmgdFrom));
			        fflush(pFile);
			      }
			   bOldSS = (Byte) (pmgdFrom->bNCBISecStru);
			   pvnmbThis = pmgdFrom->pvnmbBHead;
			   while(pvnmbThis)  /* walk the bond list */
			     {
				pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
				WriteKinBB((PFB) pmbdThis, iModel,  0,  pFile);
				pvnmbThis = pvnmbThis->next;
			     }
			   pdnmgThis = pdnmgThis->next;
		      }  /* while graphs */
		      /* do the rest of the backbone - coloring sucks, but... */
		     pvnmbThis = pmmdThis->pvnmbIRBHead;
		     if (pmmdThis->bWhat == AM_RNA)
			  fprintf(pFile, "@vectorlist {bb} color= default master= {NA}\n");
		     if (pmmdThis->bWhat == AM_DNA)
			  fprintf(pFile, "@vectorlist {bb} color= default master= {NA}\n");
		     if (pmmdThis->bWhat == AM_PROT)
			  fprintf(pFile, "@vectorlist {bb} color= default master= {Prot}\n");
		     while (pvnmbThis) /* walk the IMBonds by hand */
			{
			    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
			    WriteKinBB((PFB) pmbdThis, iModel,  0,  pFile);
			    pvnmbThis = pvnmbThis->next;
			}
		  } /* if Backbone */
	    } /* if DNA or protein or RNA */
	  pdnmmThis = pdnmmThis->next;
      } /* while molecule */

    if (bKinRender & (Byte) KIN_RESIDUE)
      {
	 /* traverse all the graphs - strip out all the residues one at a time */
	fprintf(pFile, "@group {Residues} off\n");
	fflush(pFile);
 	for (iIndex = 0;  iIndex < MAX_NCBIstdaa;  iIndex++)
	{
	    if (StringChr(pcComposition, NCBIstdaaUC[iIndex]))
	      {  /* indicates the amino acid is indeed in the structrure */
		ProgMon("Writing Kinemage");
		fprintf(pFile, "@subgroup {%s} dominant \n",  AminoAcidNameFromIdx(iIndex,  USE_MIXCASE));
		fprintf(pFile,  "@vectorlist {aa}  color= %s master= {Prot}\n", KineColors[KinAAColor[iIndex]] );
    		iTest = TraverseOneModel(pmsdThis->pdnmmHead, TRAVERSE_GRAPH,
					iModel, (Int4) iIndex, (Pointer) pFile,
                    WriteKinAAResType);
	      }
	}
       for (iIndex = 0;  iIndex < MAX_NCBI4na; iIndex++)
	{
	    if (StringChr(pcComposition, NCBI4naLC[iIndex]))
	      {  /* indicates the residue is indeed in the structrure */
		ProgMon("Writing Kinemage");
		fprintf(pFile, "@subgroup {%c} dominant \n",  NCBI4naLC[iIndex]);
		fprintf(pFile,  "@vectorlist {na} color= %s master = {NA}\n", KineColors[KinNAColor[iIndex]]);
    		iTest = TraverseOneModel(pmsdThis->pdnmmHead, TRAVERSE_GRAPH,
					iModel, (Int4) iIndex, (Pointer) pFile,
                    WriteKinNAResType);
	      }
	}
      }

    pdnmmThis = pmsdThis->pdnmmHead; /* the top of the molecule list */
    while (pdnmmThis)  /* walk the molecules for hets or ions */
      {
	  pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;

	  if ((pmmdThis->bWhat == (Byte) AM_HET) ||
              (pmmdThis->bWhat == (Byte) AM_POLY) ||
              (pmmdThis->bWhat == (Byte) AM_ION))
	    {
	        fprintf(pFile, "@group {%d het} dominant\n",(int)pdnmmThis->choice);
		fflush(pFile);
		pcColor = KineColors[ColorNumKinBB[(pdnmmThis->choice % KIN_COLOR_NUM)]];
		fprintf(pFile, "@vectorlist {het} color= %s master= {Hets}\n",  pcColor);
		fflush(pFile);
		iTest = TraverseOneModel(pmmdThis->pdnmgHead, TRAVERSE_BOND,
					iModel, iColor, (Pointer) pFile,
                    WriteKinHet);
		if (pmmdThis->bWhat == AM_ION)
		  {
		     iTest = TraverseOneModel(pmmdThis->pdnmgHead, TRAVERSE_ATOM,
					iModel, iColor, (Pointer) pFile,
                    WriteKinIon);
		  }
		 pvnmbThis = pmmdThis->pvnmbIRBHead;
		 while (pvnmbThis) /* walk the IRBonds by hand */
		   {
		    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
		    WriteKinHet((PFB) pmbdThis, iModel,  0,  pFile);
		    pvnmbThis = pvnmbThis->next;
		   }

		fflush(pFile);
	    }
	  pdnmmThis = pdnmmThis->next;
      }  /* while pdnmmThis - traverses the molecules */

    /* traverse the inter-molecule bonds */
    pvnmbThis = pmsdThis->pvnmbIMBHead;
    if (((bKinRender & (Byte) KIN_HET) ||
	      (bKinRender & (Byte) KIN_RESIDUE)) && (pvnmbThis))
      {

		 fprintf(pFile, "@group {X-links} dominant master= {X-links}\n");
		 fflush(pFile);
		 if ((bKinRender & (Byte) KIN_HET)  ||  (bKinRender & (Byte) KIN_RESIDUE))
		   {
		     fprintf(pFile, "@vectorlist {real} color= yellow ");
		     if (bKinRender & (Byte) KIN_VIRTUAL) fprintf(pFile, "master= {X-links} off\n");
		     else fprintf(pFile, "\n");
		     fflush(pFile);
		   }
	        while (pvnmbThis) /* walk the IMBonds by hand */
		  {
		     pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
		     if (bKinRender & (Byte) KIN_HET) WriteKinHet((PFB) pmbdThis, iModel,  0,  pFile);
		     if (bKinRender & (Byte) KIN_RESIDUE) WriteKinRes((PFB) pmbdThis, iModel,  0,  pFile);
	 	     pvnmbThis = pvnmbThis->next;
		  }
      }

    iHold = SetActiveModel(PFBFromDN(pdnmsThis),iHold);
    return iTest;
}


Int2 LIBCALL WriteKinModel(Int2 iModel,  PDNMS pdnmsThis,
		FILE *pFile,  Int2 iColor, Byte bKinRender, CharPtr pcComposition)
{
   Int2 iTest = 1;
   Int2 iHold = 0;
   Int2 iTemp = 0;
   PMSD pmsdThis = NULL;
   PDNMM pdnmmThis = NULL;
   PVNMB pvnmbThis = NULL;
   PMMD pmmdThis = NULL;
   PMBD pmbdThis = NULL;
   PMLD pmldThis = NULL;
   PDNML pdnmlThis = NULL;
   CharPtr pcColor = NULL;
   CharPtr pcThis = NULL;
   CharPtr pcElName = NULL;


    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    iHold = SetActiveModel(PFBFromDN(pdnmsThis),iModel);
    if (bKinRender == KIN_DEFAULT)
      {
        bKinRender = (Byte) (KIN_VIRTUAL | KIN_HET); /* lowest settings */
	iColor = KIN_COLOR_NUMBER;
      }
    if (iColor == KIN_COLOR_DEFAULT) iColor = KIN_COLOR_NUMBER;


    if (iColor == KIN_COLOR_TEMP)
      {  /* all the bonds are binned into buttons according to temp */
         /* non-solvent lone atoms are spheres */
        for (iTemp = 1; iTemp < KIN_COLOR_THERM ; iTemp++)
           {
                  pcColor = KineColors[ThermKine[iTemp]];
		  fprintf(pFile,  "@group {T %d} dominant\n",(int) (TempsKine[iTemp]/100) );
		  fflush(pFile);
		  fprintf(pFile,  "@vectorlist {atom} color= %s\n", pcColor);
		  fflush(pFile);
		  ProgMon("Writing Kinemage");
		  iTest = TraverseOneModel(pmsdThis->pdnmmHead, TRAVERSE_ATOM,
					iModel, (Int4) iTemp, (Pointer) pFile,
                    WriteKinAtom);
		  fflush(pFile);
           }
         goto donetype;
      }
    if (iColor == KIN_COLOR_ATOM)
      {  /* all the atoms are binned into atom groups */
         for (iTemp = 0;  iTemp < MAX_KIN_ATOMS;  iTemp++)
	    {
	        pcElName =  ElementName(KinAtoms[iTemp]);
		if (!pcElName) pcElName = "?";
		pcColor = KineColors[ElementKinColors[KinAtoms[iTemp]]];
		fprintf(pFile, "@group {%s} dominant\n",pcElName);
		fflush(pFile);
		fprintf(pFile,  "@vectorlist {atom} color= %s\n", pcColor);
		fflush(pFile);
	 	ProgMon("Writing Kinemage");
		iTest = TraverseOneModel(pmsdThis->pdnmmHead, TRAVERSE_ATOM,
					iModel, (Int4) -KinAtoms[iTemp], (Pointer) pFile,
                    WriteKinAtom);
		fflush(pFile);
	    }
	  /* then do all the ions */
	if (pmsdThis->bWhat & (Byte) AM_ION)
          {
 	    fprintf(pFile,  "@group {Ions} \n");
	    fflush(pFile);
      	    iTest = TraverseOneModel(pmsdThis->pdnmmHead, TRAVERSE_ATOM,
					iModel, iColor, (Pointer) pFile,
                    WriteKinIon);
	    fflush(pFile);
	  }
	goto donetype;
      }

    if (iColor == KIN_COLOR_TYPE)
      {
        iTest = WriteKinModelByType(iModel,  pdnmsThis,  pFile, iColor, bKinRender,  pcComposition);
	goto donetype;
      }

    pdnmmThis = pmsdThis->pdnmmHead; /* the top of the molecule list */

      /* individual models... */

    while (pdnmmThis)  /* walk the molecules individually - each has a group */
      {
	  pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;

	   if ((pmmdThis->bWhat == AM_RNA) ||
		(pmmdThis->bWhat == AM_DNA) ||
		(pmmdThis->bWhat == AM_PROT))
	    {
		ProgMon("Writing Kinemage");
		fprintf(pFile, "@group {%d %s %s}\n",(int)pdnmmThis->choice,
		  GetKinMolName(pmmdThis),  pmmdThis->pcMolName);
		fflush(pFile);
		if ((bKinRender & (Byte) KIN_VIRTUAL) || (bKinRender & (Byte) KIN_BACKBONE))
		  { /* make a backbone subgroup */
		    fprintf(pFile, "@subgroup {backbone} dominant\n");
		    fflush(pFile);
		  }
	   	if (bKinRender & (Byte) KIN_VIRTUAL)
		  {
		    if (iColor == KIN_COLOR_NUMBER)
			{
			     pcColor = KineColors[ColorNumKinBB[(pdnmmThis->choice % KIN_COLOR_NUM)]];
			     fprintf(pFile, "@vectorlist {virtual} color= %s master= {Virtual}\n", pcColor);
			     fflush(pFile);
			     iTest = TraverseOneModel(pmmdThis->pdnmgHead, TRAVERSE_BOND,
					iModel, iColor, (Pointer) pFile,
                    WriteKinVirt);
			}

			fflush(pFile);
		  }
		if (bKinRender & (Byte) KIN_BACKBONE)
		  {
		      if (iColor == KIN_COLOR_NUMBER)
			  {
			      pcColor =  KineColors[ColorNumKinBB[(pdnmmThis->choice % KIN_COLOR_NUM)]];
			      fprintf(pFile, "@vectorlist {real} color= %s", pcColor );
			      if (bKinRender & (Byte) KIN_VIRTUAL) fprintf(pFile, " master= {Real} off\n");
			      else fprintf(pFile, "\n");
			      fflush(pFile);
			      iTest = TraverseOneModel(pmmdThis->pdnmgHead, TRAVERSE_BOND,
					iModel, iColor, (Pointer) pFile,
                    WriteKinBB);
			      pvnmbThis = pmmdThis->pvnmbIRBHead;
			      while (pvnmbThis) /* walk the IMBonds by hand */
				{
				    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
				    WriteKinBB((PFB) pmbdThis, iModel,  0,  pFile);
				    pvnmbThis = pvnmbThis->next;
				}
			  }
		 	else {/*do the KIN_COLOR_TYPE with SecStru */ ;}
			fflush(pFile);
		  }
		   /* color-by number is one-pass residues */
		if  ((iColor == KIN_COLOR_NUMBER) &&
	         (bKinRender & (Byte) KIN_RESIDUE))
		  {
		    pcColor =  KineColors[ColorNumKinSC[(pdnmmThis->choice % KIN_COLOR_NUM)]];
		    fprintf(pFile, "@subgroup {residues} dominant master= {Residues} off\n");
		    fflush(pFile);
		    if (pmmdThis->bWhat == AM_RNA)
		       fprintf(pFile, "@vectorlist {nucleic acids} color= %s\n", pcColor);
		    if (pmmdThis->bWhat == AM_DNA)
		       fprintf(pFile, "@vectorlist {nucleic acids} color= %s\n", pcColor);
		    if (pmmdThis->bWhat == AM_PROT)
		        fprintf(pFile, "@vectorlist {amino acids} color= %s\n", pcColor);
		    fflush(pFile);
		    iTest = TraverseOneModel(pmmdThis->pdnmgHead, TRAVERSE_BOND,
					iModel, iColor, (Pointer) pFile,
                    WriteKinRes);
		    fflush(pFile);

		     /* make all the molecule's residue in one pass */
		  }  /* if making residues */
	       if (bKinRender & (Byte) KIN_RESIDUE)
	            {  /* do inter-res bonds by hand */
			fprintf(pFile, "@subgroup {x-links} dominant\n");
			fflush(pFile);
			if (bKinRender & (Byte) KIN_RESIDUE)
			   {
			     fprintf(pFile, "@vectorlist {real}  color= yellow master= {Real} off\n");
			     fflush(pFile);
			    }
			pvnmbThis = pmmdThis->pvnmbIRBHead;
			while (pvnmbThis) /* walk the IMBonds by hand */
			    {
				pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
	 			if (bKinRender & (Byte) KIN_RESIDUE) WriteKinRes((PFB) pmbdThis, iModel,  0,  pFile);
	 		 	pvnmbThis = pvnmbThis->next;
			    }
		    }

	     } /* if making DNA, RNA or PROT */

	  if ((pmmdThis->bWhat == (Byte) AM_HET) ||
               (pmmdThis->bWhat == (Byte) AM_POLY) ||
               (pmmdThis->bWhat == (Byte) AM_ION))
	    {
	        fprintf(pFile, "@group {%d het} dominant\n",(int)pdnmmThis->choice);
		fflush(pFile);
		pcColor = KineColors[ColorNumKinBB[(pdnmmThis->choice % KIN_COLOR_NUM)]];
		fprintf(pFile, "@vectorlist {het} color= %s master= {Hets}\n",  pcColor);
		fflush(pFile);
		iTest = TraverseOneModel(pmmdThis->pdnmgHead, TRAVERSE_BOND,
					iModel, iColor, (Pointer) pFile,
                    WriteKinHet);
		if (pmmdThis->bWhat == AM_ION)
		  {
		     iTest = TraverseOneModel(pmmdThis->pdnmgHead, TRAVERSE_ATOM,
					iModel, iColor, (Pointer) pFile,
                    WriteKinIon);
		  }
		pvnmbThis = pmmdThis->pvnmbIRBHead;
	        while (pvnmbThis) /* walk the IRBonds by hand */
		  {
		    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
		    WriteKinHet((PFB) pmbdThis, iModel,  0,  pFile);
		    pvnmbThis = pvnmbThis->next;
		  }
		fflush(pFile);
	    }
	  pdnmmThis = pdnmmThis->next;
      }  /* while pdnmmThis - traverses the molecules */

      /* do the rest of the inter-molecule bonds */
    pvnmbThis = pmsdThis->pvnmbIMBHead;
    if (((bKinRender & (Byte) KIN_HET) ||
	      (bKinRender & (Byte) KIN_RESIDUE)) && (pvnmbThis))
      {

		 fprintf(pFile, "@group {X-links} dominant \n");
		 fflush(pFile);
		 if ((bKinRender & (Byte) KIN_HET)  ||  (bKinRender & (Byte) KIN_RESIDUE))
		   {
		     fprintf(pFile, "@vectorlist {real}  color= yellow ");
		     if (bKinRender & (Byte) KIN_VIRTUAL) fprintf(pFile, "master= {Real} off\n");
		     else fprintf(pFile, "\n");
		     fflush(pFile);
		   }
	        while (pvnmbThis) /* walk the IMBonds by hand */
		  {
		     pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
		     if (bKinRender & (Byte) KIN_HET) WriteKinHet((PFB) pmbdThis, iModel,  0,  pFile);
		     if (bKinRender & (Byte) KIN_RESIDUE) WriteKinRes((PFB) pmbdThis, iModel,  0,  pFile);
	 	     pvnmbThis = pvnmbThis->next;
		  }
      }
   donetype:
     if (bKinRender & (Byte) KIN_ALTCONF)
       {
	pdnmlThis = DValNodeFindNext(pmsdThis->pdnmlModels, NULL, iModel); /* find the model */
 	if (pdnmlThis)
   	  {
	     /* this connects all alternative conformers recorded regardless of what they are */
	     /* they are colored by ocurrence, never by type */
	     pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
	     pcThis = pmldThis->pcAltConf;
	     if (pcThis) /* model has to have alt-confs */
	     while (StringLen(pcThis))
	        {
		     if (pcThis[0] != ' ')  /* is it really an alt-conf */
		       {
		           fprintf(pFile, "@group {Alt %c} animate dominant\n",(char) pcThis[0]);
			   fprintf(pFile, "@vectorlist {%c} color= %s\n", (char) pcThis[0],
			           KineColors[ColorNumKinAC[StringLen(pcThis) % KIN_COLOR_NUM]]);
			   iTest = TraverseOneModel(pdnmsThis, TRAVERSE_BOND,
						    iModel,(Int4) pcThis[0], (Pointer) pFile,
                            WriteKinAlt);
			   fflush(pFile);
		       }
		     pcThis = (CharPtr) &pcThis[1]; /* move the string up */
		}
	  } /* does the model have a location? */
       } /* if alt confs */

    iHold = SetActiveModel(PFBFromDN(pdnmsThis),iHold);
    return iTest;
}



Int2 LIBCALL WriteKinModelList(PDNMS pdnmsThis,  FILE *pFile,  Int2 iColor, Byte bKinRender,
				 Int2 iNumModels,  Int2Ptr i2Vec )
{
    Int2 iIndex;
    Int2 iTest = 0;
    CharPtr pcComposition = NULL;
    PMSD pmsdThis = NULL;
    PDNML pdnmlThis = NULL;
    PMLD  pmldThis = NULL;
    Int2 iPDBCount = 0;

/*	if (bKinRender == KIN_DEFAULT) bKinRender = KIN_VIRTUAL;  DONE ELSEWHERE */

    fprintf(pFile, "@text\n");
    fprintf(pFile, "NCBI - Generated Kinemage File From MMDB\n\n");
    fflush(pFile);

    if (!pdnmsThis) return iTest;
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    if (!(pmsdThis->bMe == (Byte) AM_MSD)) return iTest;

    WriteKinHeader(pdnmsThis, pFile);  /* puts @text with compound, source */
    pcComposition = WriteKinSeq(pdnmsThis, pFile); /* puts sequence, returns composition string */

    for (iIndex = 0; iIndex < iNumModels; iIndex++)
       {
        pdnmlThis = NULL;
	pdnmlThis = DValNodeFindNext(pmsdThis->pdnmlModels, NULL, i2Vec[iIndex]);
      	if (pdnmlThis)
	{
	 pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
	 if (pmldThis)
	 if (pmldThis->iType !=  Model_type_ncbi_vector) /* skip vector models */
	   {
	       if (pmldThis->iType == Model_type_pdb_model) iPDBCount++;
	   }
 	}
       }
    if ((iPDBCount < 2) && (bKinRender & (Byte) KIN_MULTANIM))
      {  /* force KIN_MULTANIM OFF if too few PDB models */
        bKinRender =  (Byte) (bKinRender - (Byte) KIN_MULTANIM);
      }
    if (bKinRender & (Byte) KIN_MULTANIM) /* Do as an animation series */
       {
          iColor = KIN_COLOR_NUMBER;  /* no choices here */
 	  fprintf(pFile, "@kinemage 1\n");
	  fprintf(pFile, "@caption\n");
	  fprintf(pFile, "PDB Models - Animation Series\n\n\n Try the A Key\n");
	  fflush(pFile);
	  for (iIndex = 0; iIndex < iNumModels; iIndex++)
	    {
		ProgMon("Writing Kinemage Animation");
/*	        printf("model %d\n", i2Vec[iIndex]); */
	       iTest = WriteKinAnimate(i2Vec[iIndex],  pdnmsThis, pFile,  iColor,  bKinRender, pcComposition);
	    }
       }
    else
    for (iIndex = 0; iIndex < iNumModels; iIndex++)
       {  /* otherwise separate kinemages */
	  ProgMon("Writing Kinemage");
	  fprintf(pFile, "@kinemage %d\n", (int)iIndex);
	  fprintf(pFile, "@caption\n");
	  fprintf(pFile, "\nMMDB Model Number %d. \n", (int)i2Vec[iIndex]);
	  fflush(pFile);
	  iTest = WriteKinModel(i2Vec[iIndex], pdnmsThis, pFile,  iColor,  bKinRender, pcComposition);
       }
    fflush(pFile);
    if (pcComposition) MemFree(pcComposition);
    return iTest;
}


Int2 LIBCALL WriteKinOneModel(PDNMS pdnmsThis,  FILE *pFile, Int2 iColor, Byte bKinRender, Int2 iModel)
{
    Int2 iRet = FALSE;
    iRet = WriteKinModelList(pdnmsThis, pFile,  iColor,  bKinRender, 1, (Int2Ptr) &iModel);
    return iRet;
}


Int2 LIBCALL WriteKinAllModel(PDNMS pdnmsThis, FILE *pFile, Int2 iColor, Byte bKinRender)
{
   Int2Ptr i2Vec = NULL;
    PDNML pdnmlThis = NULL;
    PMSD pmsdThis = NULL;
    PMLD pmldThis = NULL;
    Int2 iCount = 0;
    Int2 iRet = 0;
    Int2 iPDBCount = 0;

    pmsdThis = (PMSD)  PFBFromDN(pdnmsThis);
    if (!(pmsdThis->bMe == (Byte) AM_MSD)) return iRet;

    pdnmlThis = pmsdThis->pdnmlModels; /* the linked-list of models */
    if (pdnmlThis)
      while (pdnmlThis)
       {
	 pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
	 if (pmldThis)
	 if (pmldThis->iType !=  Model_type_ncbi_vector) /* skip vector models */
	   {
	       if (pmldThis->iType == Model_type_pdb_model) iPDBCount++;
	       iCount++;
	   }
         pdnmlThis = pdnmlThis->next;
       }
    else return 0;  /* no models */
    if ((iPDBCount < 2) && (bKinRender & (Byte) KIN_MULTANIM))
      {  /* force KIN_MULTANIM OFF if too few models */
        bKinRender =  (Byte) (bKinRender - (Byte) KIN_MULTANIM);
      }
    if (bKinRender & (Byte) KIN_MULTANIM)
       i2Vec = I2Vector(0, iCount);
    else
       i2Vec = I2Vector(0, iPDBCount);
    if (!i2Vec) return iRet;
    iCount = 0;
    pdnmlThis = pmsdThis->pdnmlModels;
    while (pdnmlThis)
	{  /* copy the model id's into the vector */
 	 pmldThis = (PMLD) pdnmlThis->data.ptrvalue;
	 if (pmldThis)
	 if (pmldThis->iType != Model_type_ncbi_vector) /* skip vector models */
	   {
	     if ((bKinRender & (Byte) KIN_MULTANIM))
	       {
		 if (pmldThis->iType == Model_type_pdb_model)
		   { /* just do the PDB models */
		    i2Vec[iCount] = pdnmlThis->choice;
		    iCount++;
		   }
	       }
	     else
	       {  /* pick the rest */
		i2Vec[iCount] = pdnmlThis->choice;
	        iCount++;
	       }
	   }

	    pdnmlThis = pdnmlThis->next;
	}
#ifdef _DEBUG_0
   printf("%d models to do \n", iCount);
#endif

    iRet = WriteKinModelList(pdnmsThis, pFile,  iColor,  bKinRender, iCount, i2Vec);
    I2VectorFree(i2Vec, 0);
    return iRet;
}




/* BRANDON - Here's your code - Go Crazy! */

static void WriteStrucHTMLHeader(PDNMS pdnmsThis,  FILE *pFile)
{

   Int4 depyear, depday;
   PMSD pmsdThis = NULL;
   BiostrucSourcePtr pbssThis = NULL;
   ValNodePtr pvnThis = NULL;
   BiostrucHistoryPtr pbshThis = NULL;
   CharPtr pcAuthors = NULL;

   if (!pdnmsThis) return;
   pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
   pvnThis = ValNodeFindNext(pmsdThis->pbsBS->descr,NULL,BiostrucDescr_history);
   if (pvnThis)
     {
      pbshThis = (BiostrucHistoryPtr) pvnThis->data.ptrvalue;
      pbssThis = pbshThis->data_source;
     }
   if (!pmsdThis->pcPDBName) pmsdThis->pcPDBName = StringSave("1UNK");
   fprintf(pFile,"Structure Summary\n\n");
   fprintf(pFile,"Contents:\n%s\n\n", pmsdThis->pcChemName);
   fflush(pFile);
   fprintf(pFile,"MMDB Accession:\n%ld\n\n", (long) pmsdThis->iMMDBid);
   fprintf(pFile,"PDB Accession:\n%s\n\n", pmsdThis->pcPDBName);
   fflush(pFile);
   if (pbssThis)
     {
   	depyear = pbssThis->database_entry_date->data[1];
   	depday = pbssThis->database_entry_date->data[3];

    if(pbssThis->database_entry_date->data[2] >= 1)
   	fprintf(pFile,"PDB Deposition:\n%2d-%3s-%02d\n\n",
                   (int) depday,
                   NCBI_months[pbssThis->database_entry_date->data[2]-1],
                   (int) depyear%100);
     }
  fprintf(pFile,"Class:\n%s\n\n", pmsdThis->pcPdbClass);
  fflush(pFile);
  fprintf(pFile,"Source:\n%s\n\n", pmsdThis->pcPdbSource);
  fflush(pFile);
  pcAuthors =  AuthorListPDB(pmsdThis->pbsBS) ;
  if (pcAuthors)
    {
      fprintf(pFile,"Authors:\n%s\n\n", pcAuthors);
      fflush(pFile);
      MemFree(pcAuthors);
    }

  return;
}

NLM_EXTERN void WriteStrucHTMLSeq(PDNMS pdnmsThis,  FILE *pFile)
{  /* writes out the sequence in UPPERCASE 3 Letter Code PDB Style, 10 per line  */
   /* some of this is from "legacy code" from Hitomi's stuff */
   Int4 resnum,linecnt,rest,curResCnt,i;
   CharPtr resnam;
   PDNMG pdnmgRes;
   PMSD pmsdThis = NULL;
   PDNMM pdnmmThis = NULL;
   PMMD pmmdThis = NULL;
   PMGD pmgdThis = NULL;
   CharPtr pcKinMolName = NULL;
   CharPtr pcTest = NULL;
   CharPtr pcComp = NULL;
   Int4 iLen = 0;

    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    pdnmmThis = pmsdThis->pdnmmHead; /* the top of the molecule list */

    fprintf(pFile, "\nSequence(s) as they appear in the PDB file.\n");
    fflush(pFile);
    while (pdnmmThis) /* walk the molecules individually */
      {
	pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
	switch((int)pmmdThis->bWhat)
	 {
	  case AM_HET:
      case AM_POLY:
      case AM_ION:
		pdnmgRes = pmmdThis->pdnmgHead;
		pmgdThis = (PMGD) pdnmgRes->data.ptrvalue;
		if (pmgdThis->pcPDBComment)
		  {
            if(StrCmp(pmgdThis->pcPDBComment, "") != 0)
		        fprintf(pFile,"\nHeterogen Name: %s\n",
				    pmgdThis->pcPDBComment);
		  }
	        break;
	  case AM_DNA:
	  case AM_RNA:
         case AM_PROT:
		resnum = pmmdThis->iResCount;
		linecnt = 1;
 	 	rest = resnum;
	 	pdnmgRes = pmmdThis->pdnmgHead;
	        pcKinMolName = GetKinMolName(pmmdThis);
		fprintf(pFile,"\nSequence {%d %s %s} has %d residues:\n",
				(int)pdnmmThis->choice,
				pcKinMolName,
				pmmdThis->pcMolName,
				(int)resnum);

			fflush(pFile);
	 	while ((rest > 0) && (pdnmgRes != NULL))
	 	  {

			linecnt++;
			if (rest > 10)
	       		    curResCnt = 10;
			else
			    curResCnt = rest;
			i = 0;
			while ((i < curResCnt) && (pdnmgRes != NULL))
			 {
			    pmgdThis = (PMGD) pdnmgRes->data.ptrvalue;
			    resnam = StringSave(pmgdThis->pcGraphName);
			    if (!resnam)
				resnam = StringSave("UNK");
			    pcTest = NULL;
			    if (pmgdThis->bWhat & (Byte) RES_AA)
  			     {
				pcTest = StringChr(NCBIstdaaUC,pmgdThis->pcIUPAC[0]);

			     }
			    if ((pmgdThis->bWhat & (Byte) RES_RNA) || (pmgdThis->bWhat & (Byte) RES_DNA))
			     {
				pcTest = StringChr(NCBI4naUC,pmgdThis->pcIUPAC[0]);
				if (pcTest)
				  {
				      pcComp = NCBI4naUC;
				      iLen = 0;
				      while ((pcComp != pcTest) && (pcComp))
				        {
					    iLen++;
					    pcComp = (CharPtr) &pcComp[1];
					}
				      pcTest = (CharPtr) &NCBI4naLC[iLen];
				  }
			     }
			    if (!pcTest) pcTest = "?";
		            if ((pmgdThis->bWhat & (Byte) DICT_LOCAL) &&
			        ((resnam[0]=='D') || (resnam[0]=='R')) &&
				(resnam[1]=='N') &&
				(resnam[2]=='A'))
				{
				    fprintf(pFile,"%3s ",(CharPtr)&resnam[3]);
				    fflush(pFile);
				}
			    else
			        {
				    if (StringLen(resnam) > 3) resnam[3] = '\0';
					/* truncate SER COOH dict. */
				    fprintf(pFile,"%3s ",resnam);
				    fflush(pFile);
				}
			    if (resnam) MemFree(resnam);
			    i++;
			    pdnmgRes = pdnmgRes->next;
			  }
			rest -= curResCnt;
 			fprintf(pFile," \n");
			fflush(pFile);
		  } /* while pdnmgRes */
		break;
	  default:
	    ;
	 } /* switch */
	 pdnmmThis = pdnmmThis->next;
      } /* while pdnmmThis */
    return;
}


Int2 LIBCALL WriteStructSummary(PDNMS pdnmsThis,  FILE *pFile)
{
   WriteStrucHTMLHeader(pdnmsThis, pFile);
   WriteStrucHTMLSeq(pdnmsThis, pFile);
   return 1;
}



/**********************************************/
/***Model to FASTA file ***********************/


static void LIBCALLBACK DoSeq(PFB pfbThis, Int4 iModel, Int4 iIndex, Pointer ptr)
{
  FILE *pFile = NULL;
  PMGD pmgdRes = NULL;

  if (!pfbThis) return;
  if (ptr) pFile = (FILE *) ptr;
    else return;
  
    if (pfbThis->bMe == AM_MGD)
      {
          /* cast to the correct PMGD pointer type */
          pmgdRes = (PMGD) pfbThis;
          /* prints 1 letter code  */
          if (pmgdRes->pcIUPAC)
            fprintf(pFile, "%c", pmgdRes->pcIUPAC[0]);        
          if (pmgdRes->pdnmgLink->choice % iIndex == 0)
          fprintf(pFile,"\n");
      }
}


void LIBCALL WriteFASTAMolecule(PDNMM pdnmmThis, FILE *File, Int4 LineLen)
  {
   PMMD pmmdThis = NULL; 
   PMSD pmsdThis = NULL;
   Int4 iTest =0;
  
   if (!pdnmmThis) return;
   if (!File) return;
   
   if (LineLen <= 0)
     LineLen = 60;
   pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
   pmsdThis = ToMSDParent((PFB) pmmdThis);
    
   if (pmmdThis->iResCount > 1) 
     {
       if (pmmdThis->pcMolName[0] != ' ')
          fprintf(File, ">gi|%ld|pdb|%s|%c Chain %c, %s, %s\n",
                 (long) pmmdThis->iGi, pmsdThis->pcPDBName, (char) pmmdThis->pcMolName[0], 
		 (char) pmmdThis->pcMolName[0],  
                  pmsdThis->pcChemName ? pmsdThis->pcChemName : "No name",  
                  pmsdThis->pcPdbSource ? pmsdThis->pcPdbSource : "Unknown source");
       else 
          fprintf(File, ">gi|%ld|pdb|%s|%c %s, %s\n",
                 (long) pmmdThis->iGi, pmsdThis->pcPDBName, (char) pmmdThis->pcMolName[0], 
		  pmsdThis->pcChemName ? pmsdThis->pcChemName : "No name",   
                  pmsdThis->pcPdbSource ? pmsdThis->pcPdbSource : "Unknown source");
       fflush(File);
       iTest = TraverseGraphs(pmmdThis->pdnmgHead,0, 
                                LineLen,File,
                                (pNodeFunc)(DoSeq));
    
      fflush(File);
      fprintf(File, "\n");
      fflush(File);
  
     }
   return;
  }


void LIBCALL WriteFASTASeqHet(PDNMS pdnmsThis,  FILE *pFile)
{  /* writes out the sequences and heterogen names   */
   PDNMG pdnmgRes;
   PMSD pmsdThis = NULL;
   PDNMM pdnmmThis = NULL;
   PMMD pmmdThis = NULL;
   PMGD pmgdThis = NULL;
   
    pmsdThis = (PMSD) pdnmsThis->data.ptrvalue;
    pdnmmThis = pmsdThis->pdnmmHead; /* the top of the molecule list */
    fprintf(pFile, "Validated Sequence(s) in FASTA format; Heterogen Names\n\n");
    fflush(pFile);
    while (pdnmmThis) /* walk the molecules individually */
      {
	pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
	switch((int)pmmdThis->bWhat)
	 {
	  case AM_HET:
                pdnmgRes = pmmdThis->pdnmgHead;
		pmgdThis = (PMGD) pdnmgRes->data.ptrvalue;
		if (pmgdThis->pcPDBComment)
		  {
		    fprintf(pFile,"Heterogen Name: %s\n",
				pmgdThis->pcPDBComment);
		  }
	        break;
         case AM_POLY:
	        pdnmgRes = pmmdThis->pdnmgHead;
		pmgdThis = (PMGD) pdnmgRes->data.ptrvalue;
		if (pmgdThis->pcPDBComment)
		  {
		    fprintf(pFile,"Biopolymer {%d het} Name: %s\n",
				(int)pdnmmThis->choice,
				pmgdThis->pcPDBComment);
		  }
	        break;
	  case AM_DNA:
	  case AM_RNA:
         case AM_PROT:
		 WriteFASTAMolecule(pdnmmThis, pFile, 60);
                 fflush(pFile);
		 fprintf(pFile, "\n");
		break;
	  default:
	    ;
	 } /* switch */
	 pdnmmThis = pdnmmThis->next;
      } /* while pdnmmThis */
    return ;
}
