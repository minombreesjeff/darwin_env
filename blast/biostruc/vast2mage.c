/* vast2mage.c
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
 * File Name: vast2mage.c
 *
 * Author: Tom Madej
 *
 * Version Creation Date: 03/12/98
 *
 * $Log: vast2mage.c,v $
 * Revision 6.10  2000/01/31 19:59:12  lewisg
 * fix output of date in pdb header
 *
 * Revision 6.9  1999/10/13 20:19:35  zimmerma
 * DZ: Removed use of temporary files - html output redirected to stdout
 *
 * Revision 6.8  1999/05/07 13:53:02  zimmerma
 * Changed call to InstBSAnnotSet() to pass Chain and SlaveChain params
 *
 * Revision 6.7  1999/02/10 14:42:53  addess
 * fixed problem with not being able to read new files
 *
 * Revision 6.6  1998/10/14  17:17:35  addess
 * sending aligned chains to MAGE
 *
 * Revision 6.5  1998/07/17  18:47:15  madej
 * Allow Mage to work with Vast Search.
 *
 * Revision 6.4  1998/06/11  19:11:20  madej
 * Part of a fix for a problem in Vast search.
 *
 * Revision 6.3  1998/05/19  20:21:17  madej
 * Add general WWW routines for running on Sun servers.
 *
 * Revision 6.2  1998/03/30  19:13:25  madej
 * Changes by Ken Addess.
 *
 * Revision 6.1  1998/03/12  17:10:06  madej
 * First official version of vast2mage.c
 *
 */

#include <stdio.h>
#include <ncbi.h>
#include <accentr.h>
#include <netentr.h>
#include <www.h>
#include <sys/resource.h>
#include <mmdbapi.h>
#include <mmdbapi1.h>
#include "vastlocl.h"
#include "mmdblocl.h"
#include "mmdbdata.h"
#include "vast2mage.h"
#include "vastsrv.h"

static char *BaseURL;
static FILE *OutputFile = NULL;
static char OutputName[200];
#define CPUTIME_MAX 120

extern CharPtr KineColors[];
extern Int1 ColorNumKinBB[];
extern Int1 ColorNumKinSC[];
extern Int1 ColorNumKinAC[];
 

void LIBCALLBACK AlignKinVirt(PFB pfbThis, Int4 iModel,  Int4 iIndex, Pointer ptr)
{  

  PMBD pmbdThis = NULL;
  PALD paldFrom = NULL;
  PALD paldTo = NULL;
  FILE *pFile = NULL;
  PMAD pmadFrom = NULL;
  PMGD pmgdFrom = NULL;
  
  if (!pfbThis) return;
  if (ptr) pFile = (FILE *) ptr; 
  if (pfbThis->bMe == (Byte) AM_MBD)
      {
        pmbdThis = (PMBD) pfbThis;
	if (!(pmbdThis->bWhat & (Byte) BOND_VIRTUAL)) return;
	paldFrom = GetLocation(GetAtomLocs(pmbdThis->pmadFrom, iModel), 0, ' ');
	paldTo = GetLocation(GetAtomLocs(pmbdThis->pmadTo, iModel), 0, ' ');
	pmadFrom = pmbdThis->pmadFrom;
	pmgdFrom = (PMGD) pmadFrom->pfbParent;
	if (paldFrom && paldTo)
	 {
	  if (iIndex)
           {  /* do aligned region */
             if (pmgdFrom->bReserved)
	       {
                AddLineKin(pFile, paldFrom, paldTo);
               }
           } 
          else
            if (!pmgdFrom->bReserved)
              {
                AddLineKin(pFile, paldFrom, paldTo);
              } 
	 }
	   
      }
  return;  
}

 
 


void LIBCALL AlignKinHeader(PDNMS pdnmsThis,  FILE *pFile, Boolean master)
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
   fprintf(pFile,"Alignment Summary\n\n");
   if (master)
      fprintf(pFile,"Master:\n%s\n\n", pmsdThis->pcChemName);
   else 
      fprintf(pFile,"Slave:\n%s\n\n", pmsdThis->pcChemName);
   fflush(pFile);
   fprintf(pFile,"MMDB Accession:\n%ld\n\n", (long) pmsdThis->iMMDBid);
   fprintf(pFile,"PDB Accession:\n%s\n\n", pmsdThis->pcPDBName);
   fflush(pFile);
   if (pbssThis) 
     {
   	depyear = pbssThis->database_entry_date->data[1];
   	depday = pbssThis->database_entry_date->data[3];
	
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


static Int2 WriteKinAlign(Int2 iStructure, Int2 iModel,  PDNMS pdnmsThis, 
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
    
       
 	  fprintf(pFile, "@group {%s %d} animate dominant\n",pmsdThis->pcPDBName, iStructure);
	  fflush(pFile);
	  if (bKinRender & (Byte) KIN_VIRTUAL)
	    {
		 fprintf(pFile, "@subgroup {aligned %s} master= {Aligned V}\n", pmsdThis->pcPDBName);
		 fflush(pFile);
		 /* walk the molecules - do the aligned portions */
		 pdnmmThis = pmsdThis->pdnmmHead;
		 while (pdnmmThis)
		   {
		       ProgMon("Writing Kinemage");
		       pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
		       if (pmmdThis->bReserved)
			  {
			     pcKinMolName = GetKinMolName(pmmdThis);
			     pcColor = KineColors[ColorNumKinAC[(iStructure % KIN_COLOR_NUM)]];
			     fprintf(pFile, "@vectorlist {%s %s %d} color= %s\n",pcKinMolName, 
					 pmmdThis->pcMolName, (int)pdnmmThis->choice , pcColor);
			     fflush(pFile); 
			     iTest = TraverseOneModel(pmmdThis->pdnmgHead,TRAVERSE_BOND,iModel,1,pFile,
						(pNodeFunc)AlignKinVirt);
			     pvnmbThis = pmmdThis->pvnmbIRBHead;
			     while (pvnmbThis) /* walk the IRBonds by hand */
				{
				    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
				    AlignKinVirt((PFB) pmbdThis, iModel,  1,  pFile);
				    pvnmbThis = pvnmbThis->next;
				}
				
			  }
			  
		       pdnmmThis = pdnmmThis->next;
		   }
                 fprintf(pFile, "@subgroup {unaligned %s} master= {Unaligned V}\n", pmsdThis->pcPDBName);
		 fflush(pFile);
		 /* walk the molecules - do the unaligned portions */
		 pdnmmThis = pmsdThis->pdnmmHead;
		 while (pdnmmThis)
		   {
		       ProgMon("Writing Kinemage");
		       pmmdThis = (PMMD) pdnmmThis->data.ptrvalue;
		       if (pmmdThis->bReserved)
			  {
			     pcKinMolName = GetKinMolName(pmmdThis);
			     pcColor = KineColors[ColorNumKinBB[(iStructure % KIN_COLOR_NUM)]];
			     fprintf(pFile, "@vectorlist {%s %s %d} color= %s\n",pcKinMolName, 
					 pmmdThis->pcMolName, (int)pdnmmThis->choice , pcColor);
			     fflush(pFile); 
			     iTest = TraverseOneModel(pmmdThis->pdnmgHead,TRAVERSE_BOND,iModel,0,pFile,
						(pNodeFunc)AlignKinVirt);
			     pvnmbThis = pmmdThis->pvnmbIRBHead;
			     while (pvnmbThis) /* walk the IRBonds by hand */
				{
				    pmbdThis = (PMBD) pvnmbThis->data.ptrvalue;
				    AlignKinVirt((PFB) pmbdThis, iModel,  0,  pFile);
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
						(pNodeFunc)WriteKinHet);
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
					(pNodeFunc)WriteKinIon);
	     fflush(pFile);
	   }
    return iTest;
}



static Int2 WriteKinPairAlign(PDNMS pdnmsMaster,
                           PDNMS pdnmsSlave,
                           FILE *pFile, 
                           Int2 iColor, Byte bKinRender)
{

    Int2 iIndex = 1;
    Int2 iTest = 0;
    CharPtr pcComposition1 = NULL;
    CharPtr pcComposition2 = NULL;
   
    PMSD pmsdThis = NULL;
    PDNML pdnmlThis = NULL;
    PMLD  pmldThis = NULL;
    Int2 iPDBCount = 0;

    if (pdnmsSlave == NULL)
	return 0;

    fprintf(pFile, "@text\n");
    fprintf(pFile, "NCBI - Generated Kinemage File Alignment From MMDB\n\n");
    fflush(pFile);
   
    if (!pdnmsMaster) return iTest;
    pmsdThis = (PMSD) pdnmsMaster->data.ptrvalue;
    if (!(pmsdThis->bMe == (Byte) AM_MSD)) return iTest;

    ResolveAlignChain(pdnmsMaster);
    ResolveAlignChain(pdnmsSlave);  /* moves up bReserved value into pmmd->bReserved to flag mols for align */
    
    AlignKinHeader(pdnmsMaster, pFile, TRUE);   
    AlignKinHeader(pdnmsSlave, pFile, FALSE);
    pcComposition1 = WriteKinSeq(pdnmsMaster, pFile); /* puts sequence, returns composition string */
    pcComposition2 = WriteKinSeq(pdnmsSlave, pFile); /* puts sequence, returns composition string */
  
    fprintf(pFile, "@kinemage %d\n", (int)iIndex);
    fprintf(pFile, "@caption\n");
    fprintf(pFile, "\nVAST derived alignment \n" );
    fflush(pFile);

    iTest = WriteKinAlign(1, 0,  pdnmsMaster, pFile,  iColor,  bKinRender, pcComposition1);
    fflush(pFile);
    iTest = WriteKinAlign(2, 0,  pdnmsSlave, pFile, iColor, bKinRender, pcComposition2);
    fflush(pFile);
    if (pcComposition1) MemFree(pcComposition1);
    if (pcComposition2) MemFree(pcComposition2);
   
    return iTest;
}



/* Display a structural alignment by a direct call to Kinemage. */

Boolean LIBCALL VastToMage(WWWInfoPtr www_info)
{
	FILE *pFile = NULL;
	FILE *pIn = NULL;
	Char pcBuf[100], pcLine[256], giBuf[20], URL[200];
	CharPtr pcTest, pcL1 = NULL, www_arg;
	CharPtr JobID = NULL, pcPass;
	Int4 GetGi, Fid, Fsid, iFileExists = 0, indx, complexity;
	BiostrucAnnotSetPtr pbsa = NULL; 
	BiostrucAnnotSetPtr pbsaShort = NULL;
	PDNMS pdnmsMaster = NULL, pdnmsSlave = NULL;
	Int2 iTest = 0, iPDB = 0, iColor, ret;
	AsnIoPtr aip = NULL; 
	Byte bRender;
	Char *IPAddress = getenv("REMOTE_HOST");
        Boolean Chain;

	if ((indx = WWWFindName(www_info, "uid")) < 0) {
		printf("Content-type: text/html\n\n");
		printf("<h2>VASTSERV Error (VastToMage)</h2>\n");
		printf("<h3>No accession (PDB ID) was input - nothing to report.</h3>\n"); 
		return 0;
	}

	www_arg = WWWGetValueByIndex(www_info, indx);

	if (isdigit(www_arg[0]))
		GetGi = (Int4) atoi(www_arg);
	else {
		printf("Content-type: text/html\n\n");
		printf("<h2>VASTSERV Error (VastToMage)</h2>\n");
		printf("<h3>Non-numeric MMDB-id input - no results.</h3>\n");
		return 0;
	}

	/* vsid and pass are to look at alignments from VAST Search */
	if ((indx = WWWFindName(www_info, "vsid")) >= 0) {
		www_arg = WWWGetValueByIndex(www_info, indx);
		JobID = StringSave(www_arg);

		if ((indx = WWWFindName(www_info, "pass")) < 0) {
			printf("Content-type: text/html\n\n");
			printf("<body bgcolor = \"#f0f0f0\">\n");
			printf("<h2>VAST SEARCH</h2>\n");
			printf("<h3>Password required.</h3>\n");
			return 0;
		}
		else {
			www_arg = WWWGetValueByIndex(www_info, indx);
			pcPass = StringSave(www_arg);

			if ((ret = Check_VastSearch_Password(pcPass, JobID)) != 1) {
				if (ret == 2) return 0;
				printf("Content-type: text/html\n\n");
				printf("<body bgcolor = \"#f0f0f0\">\n");
				printf("<h2>VAST SEARCH</h2>\n");
				printf("<h3>Incorrect password.</h3>\n"); 
				return 0;
			}
		}
	}

	if ((indx = WWWFindName(www_info, "hit")) < 0) {
		printf("Content-type: text/html\n\n");
		printf("<body bgcolor = \"#f0f0f0\">\n");
		printf("<br>\n<h2>No alignment was selected!</h2>\n");
		printf("<h3>Please click on a box in the leftmost column of the table.</h3>\n");
		return 0;
	}

	www_arg = WWWGetValueByIndex(www_info, indx);

	if (isdigit(www_arg[0]))
		Fid = (Int4) atoi(www_arg);
	else {
		printf("Content-type: text/html\n\n");
		printf("<h2>VASTSERV Error (VastToMage)</h2>\n");
		printf("<h3>Non-numeric slave alignment code - no results.</h3>\n");
		return 0;
	}

	if ((indx = WWWFindName(www_info, "chaindom")) < 0) {
		printf("Content-type: text/html\n\n");
		printf("<h2>VASTSERV Error (VastToMage)</h2>\n");
		printf("<h3>No feature set ID (master alignment code) - nothing to report.</h3>\n");
		return 0;
	}

	www_arg = WWWGetValueByIndex(www_info, indx);

	if (isdigit(www_arg[0]))
		Fsid = (Int4) atoi(www_arg);
	else {
		printf("Content-type: text/html\n\n");
		printf("<h2>VASTSERV Error (VastToMage)</h2>\n");
		printf("<h3>Non-numeric master alignment code - no results.</h3>\n");
		return 0;
	}
        
        if ((indx = WWWFindName(www_info, "chn_complexity")) < 0)
            Chain = TRUE;
        else
        {
          www_arg = WWWGetValueByIndex(www_info, indx);
          complexity =(Int2)atoi(www_arg);

          if (complexity) Chain = TRUE;
          else Chain = FALSE;
        }
	
        /* action == 0 indicates MIME; action == 1 is text; action == 2 is save */
	if ((indx = WWWFindName(www_info, "action")) < 0)
		iPDB = 0;
	else {
		www_arg = WWWGetValueByIndex(www_info, indx);

		if (isdigit(www_arg[0]))
			iPDB = (Int4) atoi(www_arg);
		else
			iPDB = 0;
	}

	if (VASTInit() != TRUE) {
		printf("Content-type: text/html\n\n");
		printf("<h2>VASTSERV Error (VastToMage)</h2>\n");
		printf("<h3>Can't find VAST data on server.\n");
		printf("Contact info@ncbi.nlm.nih.gov</h3>\n"); 
		return 0;
	}
 
	OpenMMDBAPI((POWER_VIEW /* ^ FETCH_ENTREZ */), NULL);

	if (JobID == NULL)
		pbsa = VASTBsAnnotSetGet(Fsid);
	else
		pbsa = LocalGetFeatureSet(GetGi, Fsid, JobID);

	if (pbsa == NULL) {
		printf("Content-type: text/html\n\n");
		printf("<h2>VASTSERV Error (VastToMage)</h2>\n");
		printf("<h3>No master alignment record exists for %ld.</h3>\n", (long) GetGi);
		return 0; 
	}

	pbsaShort = BiostrucAnnotSetGetByFid(pbsa, Fid, Fsid);

	if (pbsaShort == NULL) {
		printf("Content-type: text/html\n\n");
		printf("<h2>VASTSERV Error (VastToMage)</h2>\n");
		printf("<h3>Can't find alignment record.</h3>\n");
		return 0; 
	}
  
	if (MMDBInit() == FALSE) {
		printf("Content-type: text/html\n\n");
		printf("<h2>VASTSERV Error (VastToMage)</h2>\n");
		printf("<h3>MMDBInit failed.</h3>\n");
		return 0;
	}
 
	/* A note to myself for when I get back from Xmas break.  This is the situation:
	 * The call to VastToMage is failing on the check where pdnmsSlave == NULL.  For
	 * some reason the slave structure is not getting loaded into the global data
	 * structure.  It appears that the next function call to InstBSAnnotSet() loads
	 * the master structure.  However, I do not see where the slave structure gets
	 * loaded.  Absurdly enough, though, the locally compiled vastsrv works fine, and
	 * the slave is indeed getting properly loaded there.
	 *
	 * I think I see it.  There are massive differences between the local mmdbapi1.c
	 * and the checked-in one.  In particular, BiostrucAddFeature is grossly different
	 * between the two.  It looks like the slaves are loaded in the latter routine.
	 * InstBSAnnotSet calls BiostrucAddFeature.
	 */

        InstBSAnnotSet(pbsaShort, JobID, Chain, VSPATH); /* now uses mmdbapi1 version */
	pdnmsMaster = GetMasterModelstruc();
	/***** This is causing a problem at line 126, where pmsdThis->pbsBS does not exist!
	pdnmsMaster = GetSelectedModelstruc();
	*****/

	if (pdnmsMaster == NULL) {
		printf("Content-type: text/html\n\n");
		printf("<h2>VASTSERV Error (VastToMage)</h2>\n");
		printf("<h3>Unable to load master structure.</h3>\n");
		return 0;
	}

	pdnmsSlave = GetSlaveModelstruc();

	if (pdnmsSlave == NULL) {
		printf("Content-type: text/html\n\n");
		printf("<h2>VASTSERV Error (VastToMage)</h2>\n");
		printf("<h3>Unable to load slave structure.</h3>\n");
		return 0;
	}
 
	/* Kinemage file generation */
     
     if (iPDB == 2)
     {
       fprintf(stdout, "Content-type: application/octet-stream\n\n");
     }
     else if (iPDB == 1)
     {
       fprintf(stdout, "Content-type: text/html\n\n");  
       fprintf(stdout, "<HTML><body><pre>\n");
     }
     else
       fprintf(stdout, "Content-type: chemical/x-kinemage\n\n"); /* MIME */
  
	bRender = (Byte) (KIN_VIRTUAL | KIN_HET);
	iColor = KIN_COLOR_NUMBER;
	iTest = WriteKinPairAlign(pdnmsMaster, pdnmsSlave, stdout, iColor, bRender);

  if (!iTest)
    {
      printf("Content-type: text/html\n\n");
      printf("<h2>Error</h2>\n");
      printf("Kinemage File write failed on Server.<p>\n");
      RemoveTempFiles();   
      CloseMMDBAPI();
      MMDBFini();
      VASTFini();
      exit(1);
    }
 
  fflush(stdout);
    
  CloseMMDBAPI();
  MMDBFini();
  VASTFini();
  RemoveTempFiles();   
  return 0;

} /* end of VastToMage */

