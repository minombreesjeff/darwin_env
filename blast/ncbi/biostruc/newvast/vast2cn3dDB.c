/* 
 * $Id: vast2cn3dDB.c,v 1.3 2003/01/15 16:21:47 chenj Exp $
 *
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
 *
 * Author: Jie Chen, Lewis Geer, Chris Hogue, Siqian He
 *
 *
 * $Log: vast2cn3dDB.c,v $
 * Revision 1.3  2003/01/15 16:21:47  chenj
 * add construcMaxSdi()
 *
 * Revision 1.2  2003/01/14 20:42:32  chenj
 * change VastToCn3D() to VastToCn3DAndAli()
 *
 * Revision 1.1.1.1  2002/12/06 20:17:21  chenj
 * Imported Scouces
 *
 *
 *
 * This file fetches data from pubvast database, but for Vast Search job, it 
 * still reads data from files.
 *
 * =========================================================================
 *
 */


#define NLM_GENERATED_CODE_PROTO
#undef  DOWNLOAD_DEBUG_MODE

#include <stdio.h> 
#include <ncbi.h> 
#include <accentr.h>
#include <objalign.h>
#include <objseq.h>
#include <objmgr.h>
#include <lsqfetch.h>
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
#include "mkbioseq.h"


/* #include <asnmime.h> */
#include <objmime.h>
#include <strimprt.h>
#include "CAV_lib_new/cddalignview.h"
#include "/home/he/proj/VastSrv/PubVastApi.h"
/* #include "./VastHe/PubVastApi.h" */
#include "vast2cn3d.h"
#include "/home/he/proj/MmdbSrv/PubStructApi.h"
#include "vastuti.h"

#define CPUTIME_MAX 120

#define VIEW_STR_SUBBUT		0
#define VIEW_ALI_SUBBUT		1
#define VIEW_IN_Cn3DCACHE	'a'
#define VIEW_IN_Cn3D_GEN	'c'
#define VIEW_IN_HTML  		'h'
#define VIEW_IN_FASTA 		'f'
#define VIEW_IN_TEXT 		't'
#define VIEW_IN_HTML_PAGE	'H'
#define VIEW_IN_FASTA_PAGE	'F'
#define VIEW_IN_TEXT_PAGE	'T'


#define ALL_NBR 	0
#define CHECKED_NBR  	1

extern  void    WWWPrintFileData(CharPtr FName,  FILE *pFile);
extern 	Char 	MAILto[PATH_MAX], Database[PATH_MAX];
extern 	Int4	aSdi;

typedef struct
{
  unsigned BsfId;
  unsigned gi;
} GiBsfIdData, *GiBsfIdDataPtr;

static void AddMMDBIdToBioseq(BioseqPtr bsp, Int4 mmdbid)
{
    DbtagPtr    dbtp;
    SeqIdPtr    sip_new = NULL;
    SeqAnnotPtr sap_new = NULL;

    if (bsp == NULL) return;

    sap_new= SeqAnnotNew();
    sap_new->type = 4;
    sip_new = (SeqIdPtr)ValNodeNew(NULL);
    sip_new->choice = SEQID_GENERAL;
    dbtp = (DbtagPtr)DbtagNew();
    dbtp->db = (CharPtr) MemNew (10);
    sprintf(dbtp->db, "mmdb");
    dbtp->tag = (ObjectIdPtr) ObjectIdNew();
    dbtp->tag->id = mmdbid;
    sip_new->data.ptrvalue = dbtp;
    sap_new->data = sip_new;
    if (bsp->annot == NULL) bsp->annot = sap_new;
    else {
            sap_new->next = bsp->annot;
            bsp->annot = sap_new;
    }

    return;

}	/* AddMMDBIdToBioseq */



static Int4 GetMGPIdx(GiBsfIdDataPtr gbp, Int4 BsfId)
{
   Int4  i=0;

   while (BsfId != gbp[i].BsfId) i++;

   return (i);	

}	/* GetMGPIdx */



/* Display a structural alignment in Cn3D and in HTML*/

Boolean LIBCALL VastToCn3DAndAli(WWWInfoPtr www_info)
{
  Char 		szName[5], AsnPath[PATH_MAX], AsnName[10]; 
  Char		cViewType, pdbname_m[5], chain_m=' ';
  CharPtr 	Name, www_arg, szTemp, JobID=NULL, pcPass;
  Int2 		iPDB = 0, ret, complexity;
  Int2 		nbr_complexity;
  Int4 		iFidCount = 0, domNo_m, i, hits_num=0, iSubBut;
  Int4 		aMmdbId, aChain, indx, Fid, Fsid, NumLabels, *BsfId;
  /* Fsid = aMmdbId * 10000 + aChnNo * 100 + aDomCumulNo */
  /* Fid = bMmdbId * 100000 + bChnNo * 1000 + bDomCumulNo * 10 + bAlignId */
  AsnIoPtr 		paiFile, aipr;
  BioseqPtr		biosp;
  BiostrucAnnotSetPtr 	pbsa = NULL, pbsaShort = NULL;
  BiostrucAlignSeqPtr	basp = NULL;
  BiostrucFeatureSetPtr pbsfs = NULL;
  BiostrucFeaturePtr 	pbsf = NULL;
  BiostrucResidueGraphSetPtr 	stdDictionary;
  BiostrucPtr 		pbsMaster=NULL, pbsSlave=NULL, pbsSlaveHead = NULL;
  BiostrucPtr		pbsSlaveTail;
  BiostrucAlignPtr 	pbsaStruct;
  BundleSeqsAlignsPtr 	bsap = NULL;
  GiBsfIdDataPtr  	gbp;
  NcbiMimeAsn1Ptr 	pvnNcbi;
  ObjectIdPtr		objid;
  SeqAnnotPtr 		psaAlignHead = NULL, psaAlignTail;
  SeqAlignPtr 		salpHead, salpTail;
  SeqEntryPtr 		sep;
  SeqIdPtr		sid;
  ValNodePtr 		pvnFids = NULL;
  VastPageDataPtr	vpp;

  if ((indx = WWWFindName(www_info, "chaindom")) >= 0) {
      www_arg = WWWGetValueByIndex(www_info, indx);
      if (isdigit(www_arg[0])) Fsid = atoi(www_arg);
      else
           PrtMes(MAILto, "VASTSRV",
                "Invalid Biostruc-feature-set-id: chaindom = ", www_arg, FALSE);
  }
  else if ((indx = WWWFindName(www_info, "sdid")) >=0) {
	int maxsdi;

        www_arg = WWWGetValueByIndex(www_info, indx);
        if (isdigit(www_arg[0]) || www_arg[0] == '-') 
		aSdi= (long) atol(www_arg);
        else 
           PrtMes(MAILto, "VASTSRV",
                "Non-numeric Structure Domain Identifier: sdid = ",
                 www_arg,FALSE);

        maxsdi = constructMaxSdi();
        if (aSdi > maxsdi || !aSdi)
            PrtMes(NULL, "VASTSRV",
                "Vast neighbor data for this domain is not yet available. Please try later again.", NULL, FALSE);

	if (aSdi < 0)
 	   PrtMes(NULL, "VASTSRV",
	      "This is an obsolete domain, no VAST information.", NULL, FALSE);

        Fsid = SdiToLongDomId(aSdi);
        if (!Fsid) {
           Char str[10];

           sprintf(str, "%d", aSdi);
           PrtMes(MAILto, "VASTSRV",
               "Error: No domain exists for this identifier: sdid = ", str,FALSE);
 
        }
  }
  else 
      PrtMes(NULL, "VASTSRV (Vast2Cn3D)",
	"No chaindom or sdid -- please input chaindom or Structure Domain identifier .", NULL, FALSE);


  aMmdbId = Fsid / 10000;
  aChain = (Fsid % 10000) / 100;

  /* vsid and pass are to look at alignments from VAST Search */
  if ((indx = WWWFindName(www_info, "vsid")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    JobID = StringSave(www_arg);

    if ((indx = WWWFindName(www_info, "pass")) < 0)  {
	PrtMes(NULL, "VASTSRV (Vast2Cn3D)", 
	  "Please input the password of the Vast Search job.",
	   NULL, TRUE);
	return 0;
    }
    else {
      www_arg = WWWGetValueByIndex(www_info, indx);
      pcPass = StringSave(www_arg);

      if ((ret = Check_VastSearch_Password(pcPass, JobID)) != 1) {
        if (ret == 2) return 0;
	PrtMes(NULL, "VASTSRV (Vast2Cn3D)", "Incorrect password.", 
		NULL, TRUE); 
	return 0;
      }
    }
  }
  else {
      aSdi = LongDomIdToSdi(Fsid);
      if (!aSdi) {
        char    str[20];

        sprintf(str, "%d", Fsid);

        PrtMes(NULL, "VASTSRV (Vast2Cn3D)",
         "VAST structure neighbor calculations for this entry are in progress or an incorrect chaindom = ", str, FALSE);
      }
  }

  /* action == 0 indicates MIME; action == 1 is text; action == 2 is save */
  if ((indx = WWWFindName(www_info, "action")) < 0) iPDB = 0;
  else {
    www_arg = WWWGetValueByIndex(www_info, indx);

    if (isdigit(www_arg[0])) iPDB = (Int4) atoi(www_arg);
    else iPDB = 0;
  }


  if ((indx=WWWFindName(www_info, "viewstr")) >=0) 
		iSubBut=VIEW_STR_SUBBUT;  
  else if ((indx=WWWFindName(www_info, "viewali")) >=0) 
		iSubBut=VIEW_ALI_SUBBUT;
  else {
	PrtMes(NULL, "VASTSRV (Vast2Cn3D)", 
	    "Please click either \"View 3D Structure\" or \"View Alignment\".", 
	     NULL, TRUE); 
	return 0;
  }

  if (iSubBut == VIEW_STR_SUBBUT) {
  	if ((indx = WWWFindName(www_info, "calltype")) >=0) {
		www_arg = WWWGetValueByIndex(www_info, indx);
		cViewType = www_arg[0];
  	}
	else {
	   PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", 
		"No calltype code -- Please choose either Cn3D or Cn3D/Cache.\n", NULL, TRUE); 
		return 0;
	}
  }
  else if ((indx = WWWFindName(www_info, "alitype")) >=0) {
	CharPtr www_arg2;

	www_arg = WWWGetValueByIndex(www_info, indx);
        
  	if ((indx = WWWFindName(www_info, "nbr_complexity")) >=0) {
	    www_arg2 = WWWGetValueByIndex(www_info, indx);
	    if (isdigit(www_arg2[0])) nbr_complexity = (Int2)atoi(www_arg2);
            else nbr_complexity = ALL_NBR;
	}
	else nbr_complexity = ALL_NBR;

   	cViewType = www_arg[0];
	if (nbr_complexity == ALL_NBR) {
	    if (cViewType == VIEW_IN_HTML) cViewType = VIEW_IN_HTML_PAGE;
	    else if (cViewType == VIEW_IN_FASTA) 
			cViewType = VIEW_IN_FASTA_PAGE;
	    else if (cViewType == VIEW_IN_TEXT) 
			cViewType = VIEW_IN_TEXT_PAGE;
	}
  }
  else {
    PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", 
	  "No alitype code -- Please choose Hypertext, Plain text or mFASTA.", 
	   NULL, TRUE); 
    return 0; 
  }


  if ((indx = WWWFindName(www_info, "atm_complexity")) < 0)
    	complexity = ONECOORDRES; /* select alpha Carbons only by default */
  else {
    www_arg = WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) complexity = (Int2) atoi(www_arg);
    else complexity = ONECOORDRES;
  }
  if ((complexity != ONECOORDRES) && (complexity != ONECOORDATOM))
    complexity = ONECOORDRES; 
		/* bizarre value, but default to alpha-Carbons only */

/*
  if ((indx = WWWFindName(www_info, "asdi")) < 0) {
	PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", "No asdi was submitted.", NULL, TRUE); 
	return 0;
  }
  www_arg = WWWGetValueByIndex(www_info, indx);
  aSdi = (Int4) atol (www_arg);

*/

  if (cViewType == 'H' || cViewType == 'F' || cViewType == 'T') {

	nbr_complexity = ALL_NBR;
        if ((indx = WWWFindName(www_info, "allbsfid")) < 0)  {
              PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", 
			"No \"allbsfid\" was submitted!", NULL, TRUE);
	      return 0;
        }

        www_arg = WWWGetValueByIndex(www_info, indx);
        MakeNbrList(www_arg, NULL, &BsfId,  &hits_num, 0);
        gbp = (GiBsfIdDataPtr) MemNew (hits_num *sizeof(GiBsfIdData));
	gbp[0].BsfId = BsfId[0];
        for (i=1; i< hits_num; i++) {
	    Fid = gbp[i].BsfId = BsfId[i];
	    if (JobID) ValNodeAddInt(&pvnFids, 0, Fid);
	}
	iFidCount = i-1;
  }
  else {
     CharPtr ptr = NULL;

     nbr_complexity = CHECKED_NBR;

     if ((indx = WWWFindName(www_info, "hit")) <0) {
	if ((indx = WWWFindName(www_info, "defhit")) <0) {
	    PrtMes(MAILto, "VASTSRV (Vast2Cn3D)",
		"No defhit was submitted!", NULL, TRUE);
	    return 0;
	}
	else {
	   www_arg = WWWGetValueByIndex(www_info, indx);
		
	   hits_num = 2;
           gbp = (GiBsfIdDataPtr) MemNew (2*sizeof(GiBsfIdData)); 
	   BsfId = (Int4Ptr) MemNew (2*sizeof(Int4));
           gbp[0].BsfId = BsfId[0] = Fsid * 10;
	
	   if (isdigit(www_arg[0]))
		BsfId[1] = Fid = gbp[1].BsfId = atol(www_arg);
           else {
                   PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", 
			"Non-numeric defhit: defhit = ", ptr+1, TRUE);
                   return 0;
           }

           if (cViewType == VIEW_IN_Cn3D_GEN || JobID) {
                 	ValNodeAddInt(&pvnFids, 0, Fid);
			iFidCount=1;
	   }
	}
     }
     else {	 /* loop over all the "hit" values in the list */

	 NumLabels = WWWGetNumEntries(www_info);

	 for (indx = 0; indx < NumLabels; indx++) {
		Name = WWWGetNameByIndex(www_info, indx);
		if (!StrICmp(Name, "hit")) hits_num++;
	 }

	 gbp = (GiBsfIdDataPtr) MemNew (++hits_num * sizeof(GiBsfIdData));
	 BsfId = (Int4Ptr) MemNew(hits_num * sizeof(Int4));
   	 BsfId[0] = gbp[0].BsfId = Fsid * 10;
      
	 for (indx = 0; indx < NumLabels; indx++) {

	     Name = WWWGetNameByIndex(www_info, indx);
	     
	     if (!StrICmp(Name, "hit")) {
		 www_arg = WWWGetValueByIndex(www_info, indx);

		 if (isdigit(www_arg[0])) Fid =atoi(www_arg);
		 else {
		   PrtMes(MAILto,  "VASTSRV (Vast2Cn3D)",
			"Non-numeric hit: hit = .", ptr+1, TRUE);
	           return 0;
		 }
	
		 if (++iFidCount > 10 && cViewType == VIEW_IN_Cn3D_GEN) 
			break; 	/* up to 10 boxes, ignore the rest. */
		 ValNodeAddInt(&pvnFids, 0, Fid);
		 BsfId[iFidCount] = gbp[iFidCount].BsfId = Fid;
	     }
	 }
     }
  }

  if (VASTInit(JobID) != TRUE) {
	PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", "Can't find VAST data on server.",
		NULL, TRUE);
	return 0;
  }

  /* Get BiostrucAnnotSet and SeqAnnot */

  if (JobID) {			/* VastSearch */
    OpenMMDBAPI((POWER_VIEW  /* ^ FETCH_ENTREZ */), NULL);
    pbsa = LocalGetFeatureSet(aMmdbId, Fsid, JobID);
  }
  else {
    VastPageDataPtr vpptmp;

    vpptmp = (VastPageDataPtr) MemNew ((hits_num-1) * sizeof(VastPageData));
    vpp = (VastPageDataPtr) MemNew ((hits_num-1) * sizeof(VastPageData));
    for (i=0; i< hits_num+1; i++)
	vpptmp[i].bBsfId = BsfId[i+1];
    i =constructVastPagesByNbrs(vpptmp, (unsigned *)(BsfId+1),hits_num-1, aSdi); 
    if (i != hits_num-1) {
	PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", "Error in getting VAST data.", 
		NULL, TRUE); 
	return 0;
    }

    OrderCopyVpp(vpptmp, vpp, hits_num-1, &(BsfId[1]));
    MemFree(vpptmp);

    pbsa = constructBASPFromVastPagePtr(vpp, hits_num-1);

    MemFree(vpp);
  }


  if (pbsa == NULL) {
    printf("Content-type: text/html\n\n");
    printf("<br><center><h2>VASTSERV Error (VastToCn3D):</h2><p>\n");
    printf("No alignment record exists for master mmdb_id = %d.</h3>\n",
                        aMmdbId);
    printf("Please alert \"%s\" of this problem.</center>\n",MAILto);
    return 0;
  }

  if (JobID) { 		/* VastSearch */
        for (i=1; i< hits_num; i++) {
	     Int4 bsfid;
	
	     bsfid = gbp[i].BsfId;
	     gbp[i].gi = constructGi(bsfid/100000, (bsfid % 100000) / 1000);
	}
		
        pbsfs = pbsa->features;
	pbsf = pbsfs->features;
  }
  else 
     for (i=0; i< hits_num; i++) {
      	Int4 bsfid;

       	bsfid = gbp[i].BsfId;
       	gbp[i].gi = constructGi(bsfid/100000, (bsfid % 100000) / 1000);
     }

  if (JobID) { 		/* VastSearch */
    if (iFidCount == 1) 
		pbsaShort = BiostrucAnnotSetGetByFid(pbsa, Fid, Fsid);
    else pbsaShort = PruneBiostrucAnnotHits(pbsa, Fsid, pvnFids);
  }
  else pbsaShort = pbsa;


  if (pbsaShort == NULL) {
	PrtMes(NULL, "VASTSRV (Vast2Cn3D)", "Can't find alignment record.", 
		NULL, TRUE); 
	return 0;
  }

  pbsfs = pbsaShort->features;
  if (pbsfs) {
      pbsf = pbsfs->features;
      szTemp = pbsf->name;
      StrCut(szName, pbsf->name, 1, 4);
      sprintf(pdbname_m, szName);
      chain_m = szTemp[4];
      domNo_m = (Int4) atol (szTemp+5);
  }



  if (cViewType == VIEW_IN_Cn3DCACHE) bsap = BundleSeqsAlignsNew();
  else if (cViewType != VIEW_IN_Cn3D_GEN) basp = BiostrucAlignSeqNew();


  /* Get Biostruc for Cn3d, and SeqEntry (Bioseq)  */

  if (cViewType == VIEW_IN_Cn3D_GEN || JobID) {
    Int4 idx;

    pbsaStruct = BiostrucAlignNew();

    if (JobID == NULL) {
	Boolean psok;
	pbsMaster = openBSP(aMmdbId, complexity, 1, TRUE, FALSE, FALSE, 
							&psok, Database);
    }
    else {
      AsnName[0]='\0';
      StringCpy(AsnName, "/b");
      StringCat(AsnName, szName);
 
      AsnPath[0]='\0';
      StringCpy(AsnPath, VSPATH);
      StringCat(AsnPath, JobID);
      StringCat(AsnPath, AsnName);
  
      pbsMaster = FetchBS(AsnPath, 0, complexity, 1, POWER_VIEW);
    }

    if (pbsMaster == NULL) {
      PrtMes(NULL, "VASTSERV (Vast2Cn3D)", 
		"Unable to load master structure from a Vast Search result.", 
		 NULL, TRUE); 
	return 0;
    }


    aipr = NULL;
    aipr = AsnIoOpen("bstdt", "rb");
       
    if (aipr == NULL) {
      PrtMes(MAILto, "VASTSERV (Vast2Cn3D)", 
		"Can't find standard dictionary.", NULL, TRUE);
      return 0;
    }

    stdDictionary = BiostrucResidueGraphSetAsnRead(aipr, NULL);
    AsnIoFlush(aipr);
    aipr = AsnIoClose(aipr);

    idx = GetMGPIdx(gbp, BsfId[0]);
      
    if (JobID == NULL) 
	sep = (SeqEntryPtr) constructSeqEntryForGi(gbp[idx].gi, TRUE, Database);
    else {
	sep = (SeqEntryPtr) MakeBioseqs (pbsMaster, stdDictionary);
        if (sep->choice ==2) {
            Int2   i=1;
            BioseqSetPtr        bssp = NULL;

            bssp = (BioseqSetPtr)sep->data.ptrvalue;
            sep = (SeqEntryPtr) bssp->seq_set;
            while (i< aChain) {
                sep = sep->next;
                i++;
            }
            if (sep->next) sep->next= NULL;
        }
    }

    if ( sep == NULL )  {
      PrtMes(MAILto, "VASTSERV (Vast2Cn3D)", "Unable to get SeqEntry.",
		NULL, TRUE); 
      return 0; 
    }

    if (cViewType == VIEW_IN_Cn3D_GEN) { 
	ValNodeLink(&(pbsaStruct->sequences), sep);
	if (JobID) {
		biosp = (BioseqPtr)sep->data.ptrvalue;
		sid = biosp->id;
		objid = (ObjectIdPtr) sid->data.ptrvalue;
 	}	
	
    }
    else if (cViewType != VIEW_IN_Cn3DCACHE) 
    		ValNodeLink(&(basp->sequences), sep);
    else {
	PrtMes(NULL, "VASTSRV (Vast2Cn3D)", 
		"Can't work in Cn3D/Cache mode -- please try Cn3D mode.",
		 NULL, TRUE); 
        return 0;
    }

	
  /* Make a linked list of Biostrucs of the slave structures */
    for (i=1; i< hits_num; i++) {
      idx = GetMGPIdx(gbp, BsfId[i]);

      if (!pbsSlaveHead) {
	Boolean psok; 

	pbsSlaveHead = openBSP(gbp[idx].BsfId/100000, complexity, 1, TRUE, 
						FALSE, FALSE, &psok, Database);
	if (!pbsSlaveHead)  {
	  PrtMes(NULL, "VASTSRV (Vast2Cn3D)", "Unable to load slave structure.",
		NULL, TRUE);
	  return 0;
	}

      /* Make Bioseq for Slaves */
	sep = (SeqEntryPtr) constructSeqEntryForGi(gbp[idx].gi, TRUE, Database);
	if ( sep == NULL ) {
	  PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", "Unable to get SeqEntry.",
			NULL, TRUE); 
	  return 0;
	}

        if (cViewType == VIEW_IN_Cn3D_GEN) 
                ValNodeLink(&(pbsaStruct->sequences), sep);
    	else if (cViewType != VIEW_IN_Cn3DCACHE) 
                ValNodeLink(&(basp->sequences), sep);
    	else {
        	PrtMes(NULL, "VASTSRV (Vast2Cn3D)", 
		      "Can't work in Cn3D/Cache mode -- please try Cn3D mode.",
		       NULL, TRUE);
		return 0;
    	}

	pbsSlaveTail = pbsSlaveHead;
      }
      else { 
	Boolean psok;

	pbsSlave =openBSP(gbp[idx].BsfId/100000, complexity, 1, TRUE, FALSE, 
							FALSE, &psok, Database);
	if (!pbsSlave) { 
		PrtMes(NULL, "VASTSERV (Vast2Cn3D)", 
			"Unable to load slave structure.",
			 NULL, TRUE);
		return 0;
	}

	sep = (SeqEntryPtr) constructSeqEntryForGi(gbp[idx].gi, TRUE, Database);
	if ( sep == NULL ) {
		PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", 
		    "Unable to get SeqEntry.", NULL, TRUE);
		return 0;
	}

        if (cViewType == VIEW_IN_Cn3D_GEN) 
                ValNodeLink(&(pbsaStruct->sequences), sep);
        else if (cViewType != VIEW_IN_Cn3DCACHE) 
                ValNodeLink(&(basp->sequences), sep);
        else {
                PrtMes(NULL, "VASTSRV (Vast2Cn3D)", 
		    "Can't work in Cn3d/Cache mode -- please try Cn3D mode.",
			NULL, TRUE);
		return 0;
        }

	pbsSlaveTail->next = pbsSlave;
	pbsSlaveTail = pbsSlaveTail->next;
	pbsSlaveTail->next = NULL;
      }
    }
  }
  else {     /* cViewType != Cn3D_IN_GEN */

    BioseqPtr 		bsp;

/* Get SeqEntryPtr */

    for (i=0; i< hits_num; i++) {

      Int4 idx;

      idx = GetMGPIdx(gbp, BsfId[i]);

      sep = constructSeqEntryForGi(gbp[idx].gi, TRUE, Database);

      if (sep == NULL) {
	PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", "Unable to get Bioseq.",
		NULL, TRUE); 
	return 0;
      }

      bsp = sep->data.ptrvalue;
      AddMMDBIdToBioseq(bsp, gbp[idx].BsfId/100000);      

      if (sep == NULL) {
	PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", "Unable to get Bioseq.", 
		NULL, TRUE); 
	return 0;
      }
      if (cViewType == VIEW_IN_Cn3DCACHE) ValNodeLink(&(bsap->sequences), sep);
      else ValNodeLink(&(basp->sequences), sep);
    }
  }

  MemFree(gbp);

  /* Make a linked list of sequence alignments of master and slaves */
  pbsf=pbsfs->features;
  while (pbsf) {
    if (!psaAlignHead) {
      psaAlignHead = fnPBSFtoPSA (pbsf); /* get the sequence alignments */

      if (psaAlignHead == NULL || psaAlignHead->data == NULL) {
	PrtMes(MAILto, "VASTSERV (Vast2Cn3D)", "Unable to create SeqAnnot.", NULL, TRUE);
	return 0;
      }
      salpHead = (SeqAlignPtr)(psaAlignHead->data);
      salpTail = salpHead;

      if (JobID && cViewType == VIEW_IN_Cn3D_GEN) {
       	DenseDiagPtr dendiag;

      	for (dendiag=(DenseDiagPtr)salpHead->segs;dendiag;dendiag=dendiag->next)
 	{
      		sid = dendiag->id;
      		sid->choice = 1; 
      		sid->data.ptrvalue = (ObjectIdPtr)objid;
        }
      }

    }
    else {
      psaAlignTail = fnPBSFtoPSA (pbsf);

      salpTail->next = (SeqAlignPtr)(psaAlignTail->data);
      if (psaAlignTail == NULL || psaAlignTail->data == NULL) {
	PrtMes(NULL, "VASTSRV (Vast2Cn3D)", "Unable to create SeqAnnot.", NULL, TRUE);
	return 0;
      }
      salpTail = salpTail->next;
      if (JobID && cViewType == VIEW_IN_Cn3D_GEN) {
	DenseDiagPtr dendiag;

	for (dendiag=(DenseDiagPtr)salpTail->segs;dendiag;dendiag=dendiag->next)
	{
        	sid = dendiag->id;
        	sid->choice = 1;
        	sid->data.ptrvalue = (ObjectIdPtr)objid;
	}
     }
      salpTail->next = NULL;
    }
    pbsf = pbsf->next;
  }

  /* assemble pvnNcbi */
  
  pvnNcbi = ValNodeNew(NULL);
  if (cViewType == VIEW_IN_Cn3D_GEN) {

    pbsaStruct->master = pbsMaster;
    pbsaStruct->slaves = pbsSlaveHead;
    pbsaStruct->alignments = pbsaShort;
    pbsaStruct->seqalign = psaAlignHead;

    pvnNcbi->choice =  NcbiMimeAsn1_alignstruc;
    pvnNcbi->data.ptrvalue = pbsaStruct;
/*    pvnNcbi = (NcbiMimeAsn1Ptr) CheckId(pvnNcbi, JobID); */
  	/* to check identity, yanli  */
  }
  else if (cViewType == VIEW_IN_HTML || cViewType == VIEW_IN_FASTA 
	|| cViewType == VIEW_IN_HTML_PAGE || cViewType == VIEW_IN_FASTA_PAGE
	|| cViewType == VIEW_IN_TEXT || cViewType == VIEW_IN_TEXT_PAGE) {

    basp->seqalign = psaAlignHead;

    pvnNcbi->choice = NcbiMimeAsn1_alignseq;
    pvnNcbi->data.ptrvalue = basp; 

    if (JobID) {
	SeqEntryPtr sep=NULL;
	BioseqPtr   bsp=NULL;
	SeqIdPtr    sip=NULL, sip_next=NULL;
	ObjectIdPtr oip=NULL;
	PDBSeqIdPtr psip=NULL;

        sep=basp->sequences;
	bsp = (BioseqPtr)sep->data.ptrvalue;
	sip = bsp->id;
        if (sip->choice == SEQID_LOCAL) {
	    sip_next = sip->next;
	    oip = (ObjectIdPtr)sip->data.ptrvalue;
	    psip = PDBSeqIdNew();
	    psip->mol= (CharPtr) MemNew (5);
	    StrCut(psip->mol, oip->str, 1, 4);
	    psip->chain = (Uint1)(oip->str)[5];
            oip = ObjectIdFree(oip);
	    sip->choice = SEQID_PDB;
	    sip->next = sip_next;
	    sip->data.ptrvalue = psip;
	}	    
    }
  }
  else { 	/*if (cViewType == VIEW_IN_Cn3dCache) */

    BiostrucSeqsAlignsCddPtr bsacp;
    SeqAlignData_seq_align_dataPtr sadp;

    bsap->seqaligns = psaAlignHead; 
    bsap->strucaligns = pbsaShort;

    sadp = ValNodeNew(NULL);
    sadp->data.ptrvalue = bsap;
    sadp->choice = SeqAlignData_seq_align_data_bundle;
    bsacp = BiostrucSeqsAlignsCddNew();
    bsacp->SeqAlignData_seq_align_data = sadp;
    if (complexity == ONECOORDRES)     
		bsacp->structure_type = 2;	/* ncbi_backbone */
    else bsacp->structure_type = 3;		/* ncbi_all_atoms */

    pvnNcbi->choice = NcbiMimeAsn1_general;
    pvnNcbi->data.ptrvalue = bsacp;

  } 


/*
{AsnIoPtr aipr;
aipr=AsnIoOpen("pvnNcbi.out", "w");
NcbiMimeAsn1AsnWrite(pvnNcbi, aipr, NULL);
AsnIoClose(aipr);
}
*/


  if (cViewType == VIEW_IN_Cn3D_GEN || cViewType == VIEW_IN_Cn3DCACHE) {
    if (iPDB == 0)    /* cn3d MIME */
      printf ("Content-type: chemical/ncbi-asn1-binary\n\n");

    else if (iPDB == 1) {    /* "See File" */
      printf ("Content-type: text/html\n\n");
      printf ("<HTML><body><pre>\n");
    }
    else        /* "Save File" */
      printf ("Content-type: application/octet-stream\n\n");

    if (iPDB != 1) paiFile = AsnIoNew(ASNIO_BIN_OUT, stdout, NULL, NULL, NULL); 
    else paiFile = AsnIoNew(ASNIO_TEXT_OUT, stdout, NULL, NULL, NULL);

    NcbiMimeAsn1AsnWrite(pvnNcbi, paiFile, NULL); 
    AsnIoFlush(paiFile); 
    AsnIoClose(paiFile);
  }
  else {

    Uint4      	size;
    BytePtr 	buf;
    Nlm_ByteStorePtr bsp = NULL;
    AsnIoBSPtr	aibp = NULL;

    bsp = Nlm_BSNew(1024);
    aibp = AsnIoBSOpen("wb", bsp);
    if (!bsp || !aibp) {
        PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", "AsnIoBS creation failed.",
		NULL, TRUE); 
	return 0;
    }

    if (!(NcbiMimeAsn1AsnWrite(pvnNcbi, aibp->aip, NULL)))  {
        PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", "C object -> AsnInBS failed.",
		NULL, TRUE);
	return 0;
    }

    AsnIoFlush(aibp->aip);
    AsnIoBSClose(aibp); aibp = NULL;

    size = Nlm_BSLen(bsp);
    buf = MemNew(size);
    if (!buf) {
        PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", "buf allocation failed",
		NULL, TRUE); 
	return 0;
    }
       
    Nlm_BSSeek(bsp, 0, 0);
    if (Nlm_BSRead(bsp, buf, size) != size) {
        PrtMes(MAILto, "VASTSRV (Vast2Cn3D)", "AsnIoBS -> buf failed",
		NULL, TRUE); 
	return 0;
    }

    Nlm_BSFree(bsp); bsp = NULL;

    printf("Content-type: text/html\n\n");
    WWWPrintFileData("sshead.txt", stdout);
    printf("<TABLE width=800 BORDER=0 CELLPADDING=3 CELLSPACING=0 bgcolor=#FFFFCC>\n\n");
    PrintQueryInfo(stdout, pdbname_m, chain_m, domNo_m, aMmdbId, JobID);
    printf("</TABLE>\n");
    printf("<br>\n");
    if (cViewType == VIEW_IN_HTML || cViewType == VIEW_IN_HTML_PAGE)
      CAV_DisplayMultiple(buf, CAV_HTML|CAV_SHOW_IDENTITY, 60, 2.0, NULL, 0, NULL); 
    else if (cViewType == VIEW_IN_FASTA || cViewType == VIEW_IN_FASTA_PAGE) {
      printf("<pre>\n");
      CAV_DisplayMultiple(buf, 
		CAV_FASTA|CAV_LEFTTAILS|CAV_RIGHTTAILS|CAV_FASTA_LOWERCASE, 
		60, 2.0, NULL, 0, NULL);
      printf("</pre>\n");
    }
    else if (cViewType == VIEW_IN_TEXT || cViewType == VIEW_IN_TEXT_PAGE) {
	printf("<pre>\n");
	CAV_DisplayMultiple(buf, CAV_TEXT, 60, 2.0, NULL, 0, NULL);
	printf("</pre>\n");
    }

    printf("<HR SIZE=5 NOSHADE>\n");
    printf("</body></html>\n");

    MemFree(buf);
  }

  if (cViewType==VIEW_IN_Cn3D_GEN) {
  	CloseMMDBAPI();
  	MMDBFini();
  }
  VASTFini();

  NcbiMimeAsn1Free(pvnNcbi); 

  return 0; 
} 	/* end of VastToCn3DAndAli */
