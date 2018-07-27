/* $Id: cddumper.c,v 1.26 2004/04/01 13:43:05 lavr Exp $
*===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name:  cddumper.c
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 10/30/2000
*
* $Revision: 1.26 $
*
* File Description: CD-dumper, rebuilt from scrap parts       
*         
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cddumper.c,v $
* Revision 1.26  2004/04/01 13:43:05  lavr
* Spell "occurred", "occurrence", and "occurring"
*
* Revision 1.25  2003/05/21 17:24:28  bauer
* fixes to Blast DB access
*
* Revision 1.24  2002/10/10 20:38:19  bauer
* changes to accomodate new spec items
* - old-root node
* - curation-status
*
* Revision 1.23  2002/10/02 17:32:21  bauer
* avoid merging blocks when reindexing alignments
*
* Revision 1.22  2002/08/06 12:54:25  bauer
* fixes to accomodate COGs
*
* Revision 1.21  2002/07/31 14:58:27  bauer
* BLAST DB Sequence Retrieval
*
* Revision 1.20  2002/07/31 03:21:35  bauer
* PUBSEQ instead of ID1 access
*
* Revision 1.19  2002/06/25 21:25:31  bauer
* fixed binary ASN.1 reading problem
*
* Revision 1.18  2002/05/06 17:16:16  bauer
* fixed bug in defline processing (conditional prepend of short name)
*
* Revision 1.17  2002/05/06 16:58:47  bauer
* changed default behaviour in tax branch merge
*
* Revision 1.16  2002/03/07 19:12:14  bauer
* major revisions to cgi-bins and the CD-dumper
*
* Revision 1.15  2002/02/20 22:26:33  bauer
* fixed bug in recalculation of diversity ranks for finished CDs
*
* Revision 1.14  2002/01/28 14:10:40  bauer
* add short name to trunc_master defline
*
* Revision 1.13  2002/01/04 19:49:16  bauer
* support for CDTrack production
*
* Revision 1.12  2001/11/13 19:52:56  bauer
* biostrucs from file
*
* Revision 1.11  2001/05/23 21:18:06  bauer
* added functions for alignment block structure control
*
* Revision 1.10  2001/04/10 20:28:45  bauer
* optional flag for not saving out the CD
*
* Revision 1.9  2001/03/07 20:28:38  bauer
* CddFree commented out
*
* Revision 1.8  2001/02/28 19:43:01  bauer
* resort sequences when reindexing to new master
*
* Revision 1.7  2001/02/16 03:09:30  bauer
* added ability to reindex to new master
*
* Revision 1.6  2001/02/13 20:56:21  bauer
* added capability to read in DenSeg alignments
*
* Revision 1.5  2001/02/06 22:46:46  bauer
* Scoring Matrix as program parameter
*
* Revision 1.4  2001/02/05 22:45:54  bauer
* added support for split sets
*
* Revision 1.3  2001/01/24 03:08:08  bauer
* fixed memory leaks
*
* Revision 1.2  2001/01/17 21:32:01  bauer
* changes to PSSM calculation
*
* Revision 1.1  2001/01/17 20:58:36  bauer
* add cddumper, the new version of cddump
*
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <asn.h>
#include <mmdbapi.h>
#include <objmmdb1.h>
#include <objmime.h>
#include <taxutil.h>
#include <txcommon.h>
#include <objcdd.h>
#include "cddsrv.h"
#include "cddutil.h"
#include <readdb.h>

#ifdef DBNTWIN32
#include <edutil.h>
#include <prunebsc.h>
#include <PubVastApi.h>
#include <PubStructApi.h>
#endif

#define NUMARGS 26
static Args myargs[NUMARGS] = {
  {"Cd-Accession",                                           /*0*/
   "cd00000",  NULL,NULL,FALSE,'c',ARG_STRING, 0.0,0,NULL},
  {"Extension for ASN.1 output file name",                   /*1*/
   "acd",      NULL,NULL,FALSE,'a',ARG_STRING, 0.0,0,NULL},
  { "Binary output CD",                                      /*2*/
   "F",        NULL,NULL,FALSE,'b',ARG_BOOLEAN,0.0,0,NULL},
  { "Store PSSM in CD",                                      /*3*/
   "F",        NULL,NULL,FALSE,'k',ARG_BOOLEAN,0.0,0,NULL},
  { "Source Identifier",                                     /*4*/
    "Cdd",     NULL,NULL,TRUE, 's',ARG_STRING, 0.0,0,NULL},
  { "Convert Dense Diag to true multiple Alignment",         /*5*/
   "F",        NULL,NULL,FALSE,'m',ARG_BOOLEAN,0.0,0,NULL},
  { "File extension for tree file",                          /*6*/
   "act",      NULL,NULL,FALSE,'t',ARG_STRING, 0.0,0,NULL},
  { "Reference file extension",                              /*7*/
   "REF",      NULL,NULL,FALSE,'r',ARG_STRING, 0.0,0,NULL},
  { "Status flag for CDD (1=finished,2=pending,3=oAsIs",     /*8*/
    "2",       NULL,NULL,FALSE,'f',ARG_INT,    0.0,0,NULL},
  { "Calculate a consensus sequence",                        /*9*/
   "F",        NULL,NULL,FALSE,'C',ARG_BOOLEAN,0.0,0,NULL},
  { "Pseudocount constant (integers 1-10)",                 /*10*/
   "-1",       NULL,NULL,FALSE,'p',ARG_INT,    0.0,0,NULL},
  { "Use this real-ind file to import alignment",           /*11*/
    NULL,      NULL,NULL,TRUE, 'i',ARG_STRING, 0.0,0,NULL},
  { "Update this existing CD (file name, will overwrite)",  /*12*/
   NULL,       NULL,NULL,TRUE, 'u',ARG_STRING, 0.0,0,NULL},
  { "Binary input CD",                                      /*13*/
    "F",       NULL,NULL,FALSE,'e',ARG_BOOLEAN,0.0,0,NULL},
  { "New accession for CD in case of update",               /*14*/
    NULL,      NULL,NULL,TRUE, 'n',ARG_STRING, 0.0,0,NULL},
  { "Parse new name from real-ind file",                    /*15*/
    "T",       NULL,NULL,FALSE,'N',ARG_BOOLEAN,0.0,0,NULL},
  { "Scoring Matrix (PAM30, BLOSUM62, ..)",                 /*16*/
    "BLOSUM62",NULL,NULL,FALSE,'M',ARG_STRING, 0.0,0,NULL},
  { "Make this PDB-derived sequence the new master",        /*17*/
    NULL,      NULL,NULL,TRUE, 'R',ARG_STRING, 0.0,0,NULL},
  { "Output Checkpoint file/Sequence only (no CD/tree)",    /*18*/
    "F",       NULL,NULL,FALSE,'o',ARG_BOOLEAN,0.0,0,NULL},
  { "trim Bioseqs",                                         /*19*/
    "T",       NULL,NULL,FALSE,'T',ARG_BOOLEAN,0.0,0,NULL},
  { "trim SeqAligns if not an oAsIs CD",                    /*20*/
    "T",       NULL,NULL,FALSE,'A',ARG_BOOLEAN,0.0,0,NULL},
  { "retrieve superposition data from PubVast",             /*21*/
    "T",       NULL,NULL,FALSE,'v',ARG_BOOLEAN,0.0,0,NULL},
  { "Parse short name from description, move to name field",/*22*/
    "F",       NULL,NULL,TRUE, 'P',ARG_BOOLEAN,0.0,0,NULL},
  { "Write out Checkpoint File, Sequence, and Tree File",   /*23*/
    "T",       NULL,NULL,TRUE, 'K',ARG_BOOLEAN,0.0,0,NULL},
  { "Verbose",                                              /*24*/
    "F",       NULL,NULL,TRUE, 'V',ARG_BOOLEAN,0.0,0,NULL},
  { "BLAST Sequence DB retrieval",                          /*25*/
    "T",       NULL,NULL,TRUE, 'B',ARG_BOOLEAN,0.0,0,NULL}
};

ReadDBFILEPtr rdfp;

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* read parameters from configuration file                                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean CddGetParams()
{
  MMDBpath[0] = gunzip[0] = '\0';

#ifndef DBNTWIN32
  GetAppParam("cdd", "CDDSRV", "Gunzip", "", gunzip, (size_t) 256*(sizeof(Char)));
  if (gunzip[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file has no Gunzip...\n");
    return FALSE;
  }
#endif
  GetAppParam("cdd", "CDDSRV", "CDDatabase", "", CDDdpath, PATH_MAX);
  if (CDDdpath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file has no CDD data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDextens", "", CDDextens, PATH_MAX);
  if (CDDextens[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file has no CDD file name extension...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDdescr", "", CDDdescr, PATH_MAX);
  if (CDDdescr[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file has no description file name extension...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "REFpath", "", REFpath, PATH_MAX);
  if (REFpath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file has no REFpath...\n");
    return FALSE;
  }
  return TRUE;
}                                                       /* end GetVastParams */

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Read in Sequence Alignment Data formatted as ASN.1                        */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAlignPtr CddReadSeqAln() {
  
  SeqAnnotPtr              psaCAlignHead = NULL;
  AsnIoPtr                 aip;
  Char                     CDDalign[PATH_MAX];
  SeqAlignPtr              salp;

  Nlm_StrCpy(CDDalign,CDDdpath);
  Nlm_StrCat(CDDalign,cCDDid);
  Nlm_StrCat(CDDalign,CDDextens);
  aip = AsnIoOpen(CDDalign,"r");
  psaCAlignHead = SeqAnnotAsnRead(aip, NULL);
  AsnIoClose(aip);
  if (!psaCAlignHead) CddSevError("Could not access CDD alignment!");
  salp = psaCAlignHead->data;
  if (salp->segtype == SAS_DENDIAG) {
    return (salp);
  } else if (salp->segtype == SAS_DENSEG) {
    return (CddMSLDenSegToMSLDenDiag(salp));
  }
  return (NULL);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Check whether a particular gi has occurred previously in the CddSum lnklst*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean UniqueUid(Int4 uid, Boolean bIsPdb, CddSumPtr pcds) {
  
  CddSumPtr    pcdsThis;

  if (!pcds) return TRUE;
  pcdsThis = pcds;
  while (pcdsThis) {
    if (pcdsThis->uid == uid && pcdsThis->bIsPdb == bIsPdb) return FALSE;
    pcdsThis = pcdsThis->next;
  }
  return TRUE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Stolen from mmdbsrv.c - converts PDB-Id to numerical mmdb-id              */
/* modified - does no longer check whether the string corresponds to an      */
/* integer in the first place, so that PDB-Id's like "1914" can be read as   */
/* well ..                                                                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Int4 ConvertMMDBUID(CharPtr pcString)  
{
  Int4    iUID;
  CharPtr pcTemp = NULL;
        
  if (pcString == NULL) return 0;
  iUID = 0;
  pcTemp = StringSave(pcString);
  CleanSpaces(pcTemp);
#ifndef DBNTWIN32
  iUID = MMDBEvalPDB(pcTemp);
#else
  iUID = constructLiveOrDeadMmdbIdForPdbId(pcTemp);
#endif
  MemFree(pcTemp);
  return iUID; 
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* allocate a new CddSum linked list entry                                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CddSumPtr CddSumNew()
{
  CddSumPtr   pcds;
  
  pcds=(CddSumPtr)MemNew(sizeof(CddSum));
  if (pcds==NULL) return(pcds);
  pcds->bIsPdb      = FALSE;
  pcds->bIsMaster   = FALSE;
  pcds->cPdbId[0]   = '\0';
  pcds->cChainId[0] = '\0';
  pcds->cPKBMDom[0] = '\0';
  pcds->cPKBDom[0]  = '\0';
  pcds->cDefLine[0] = '\0';
  pcds->iFsid       = -1;
  pcds->iFid        = -1;
  pcds->iMMDBId     = -1;
  pcds->iCddIdx     = -1;
  pcds->uid         = 0;
  pcds->sip         = NULL;
  pcds->next        = NULL;
  return pcds;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Free a CddSum linked list                                                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CddSumPtr CddSumFree(CddSumPtr pcds)
{
  CddSumPtr    next;
  
  while (pcds) {
    next = pcds->next;
    Nlm_MemFree(pcds);
    pcds = next;
  }
  return NULL;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* adds a to a linked list of CddSumPtr, always returns the beginning of the */
/* list and always adds to the end of the list!!                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CddSumPtr CddSumLink(CddSumPtr PNTR head, CddSumPtr newnode)
{
  CddSumPtr     pcds;
 
  if (head == NULL) return newnode;
  pcds = *head;
  if (pcds != NULL) {
    while(pcds->next != NULL) pcds = pcds->next;
    pcds->next = newnode;
  } else *head = newnode;
  return *head;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* retrieve a SeqEntry from the BLAST database - or use PUBSEQ if not found  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static SeqEntryPtr CddumperSeqEntryGet(Int4 uid, Int2 retcode)
{
  SeqIdPtr    sip;
  BioseqPtr   bsp;
  SeqEntryPtr sep;

  if (myargs[25].intvalue) {
    sip = ValNodeAddInt(NULL, SEQID_GI, uid);
    bsp = CddReadDBGetBioseq(sip, -1, rdfp);
    if (bsp) {
      if (bsp->seq_data_type != Seq_code_ncbieaa)
        BioseqRawConvert(bsp, Seq_code_ncbieaa);
      sep = SeqEntryNew();
      sep->choice = 1;
      sep->data.ptrvalue = bsp;
      return(sep);
    }
  }
#ifdef OS_UNIX
  return (SeqEntryPtr) PUBSEQSeqEntryGet(uid,retcode);
#else 
  return NULL;
#endif
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Process Sequence Alignment                                                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
CddSumPtr CddProcessSeqAln(SeqAlignPtr* salpInOut, Int4* n_Pdb, BiostrucAlignSeqPtr pbsaSeq,
                           Boolean bTrimBioseq, Boolean bVerbose)
{
  BioseqPtr      bspNew;
  CddSumPtr      pcdsThis, pcds = NULL;
  DbtagPtr       dbtp;
  DenseDiagPtr   ddp;
  ObjectIdPtr    oidp;
  PDBSeqIdPtr    pdb_seq_id;
  SeqAlignPtr    salpCopy, salpTail, salpHead;
  SeqAnnotPtr    annot, thisAnnot;
  SeqEntryPtr    sep, sepNew;
  SeqIdPtr       sip, sip_master = NULL, sipXtra;
  Int2           retcode = 2;
  Int4           iPcount, uid, uidmaster = 0;
  Int4           iCddSize = 0;
  Int4           nPdb = 0;

  salpHead = *salpInOut;
  salpCopy = NULL; salpTail = NULL;

  while (salpHead) {
    iPcount = 1;
    ddp = (DenseDiagPtr) salpHead->segs;
    sip = ddp->id;
    while (sip) {
      if (sip->choice == SEQID_GI) uid = sip->data.intvalue;
#ifdef OS_UNIX
      else uid = PUBSEQFindSeqId(sip);
#else
      else CddSevError("Can not process all SeqAligns on NT platform");
#endif
      if (uid) {
        pcdsThis = CddSumNew();
        pcdsThis->uid = uid;
        pcdsThis->iCddIdx = iCddSize;
        pcdsThis->sip = sip;
        if (sip->choice == 15) {
          pcdsThis->bIsPdb = TRUE;
          pdb_seq_id = (PDBSeqIdPtr) CddGetPdbSeqId(sip);
          pcdsThis->cChainId[0] = pdb_seq_id->chain;
          pcdsThis->cChainId[1] = '\0';
          pcdsThis->cPdbId[0] = pdb_seq_id->mol[0];
          pcdsThis->cPdbId[1] = pdb_seq_id->mol[1];
          pcdsThis->cPdbId[2] = pdb_seq_id->mol[2];
          pcdsThis->cPdbId[3] = pdb_seq_id->mol[3];
          pcdsThis->cPdbId[4] = '\0';
          pcdsThis->iMMDBId = ConvertMMDBUID(pcdsThis->cPdbId);
        } else pcdsThis->bIsPdb = FALSE;
        if (!uidmaster && iPcount == 1) {
          if (!sip_master) sip_master = sip;
          uidmaster = uid;
          pcdsThis->bIsMaster = TRUE;
          if (pcdsThis->bIsPdb) nPdb++;
          iCddSize++;
          if (UniqueUid(uid,pcdsThis->bIsPdb,pcds)) {
            sep = (SeqEntryPtr) CddumperSeqEntryGet(uid,retcode);
            if (sep == NULL) CddSevError("Unable to get MasterSeqEntry from PUBSEQ");
            bspNew = (BioseqPtr) CddExtractBioseq(sep,sip);
	    if (bTrimBioseq) CddShrinkBioseq(bspNew);
/*---------------------------------------------------------------------------*/
/* Add Additional SequenceIds in case this is a 3D structure                 */
/*---------------------------------------------------------------------------*/
	    if (pcdsThis->bIsPdb) {
	      sipXtra = ValNodeNew(NULL);
	      sipXtra->data.intvalue = pcdsThis->uid;
	      sipXtra->choice = SEQID_GI;
	      bspNew->id->next = sipXtra;
              sipXtra = ValNodeNew(NULL);
	      sipXtra->choice = SEQID_GENERAL;
	      dbtp = (DbtagPtr) DbtagNew();
	      dbtp->db = (CharPtr) StringSave("mmdb");
	      oidp= ObjectIdNew();
	      oidp->id = pcdsThis->iMMDBId;
	      dbtp->tag = oidp;
	      sipXtra->data.ptrvalue = dbtp;
              annot = SeqAnnotNew();
	      annot->type = (Uint1) 4;
	      annot->data = sipXtra;
              if (!bspNew->annot) {
	        bspNew->annot = annot;
              } else {
	        thisAnnot = bspNew->annot;
		while (thisAnnot->next) thisAnnot = thisAnnot->next;
	        thisAnnot->next = annot;
	      }
	    }
            sepNew = SeqEntryNew();
            sepNew->data.ptrvalue = bspNew;
            sepNew->choice = 1;
            ValNodeLink(&(pbsaSeq->sequences), sepNew);
            SeqEntryFree(sep);
          }
          CddSumLink(&(pcds),pcdsThis);
        } else if (uid == uidmaster && iPcount == 1) {
          sip = sip->next;
          iPcount++;
          CddSumFree(pcdsThis);
        } else {
          sep = (SeqEntryPtr) CddumperSeqEntryGet(uid,retcode);
          if (sep == NULL) {
            if (bVerbose) 
	      printf("Unable to get SeqEntry %d from PUBSEQ, skipping ..\n",uid);
          } else {
            if (pcdsThis->bIsPdb) nPdb++;
            iCddSize++;
            if (!salpCopy) {
              salpCopy = salpHead;
              salpTail = salpCopy;
            } else {
              salpTail->next = salpHead;
              salpTail = salpTail->next;
            }
            if (UniqueUid(uid,pcdsThis->bIsPdb,pcds)) {
              bspNew = (BioseqPtr) CddExtractBioseq(sep,sip);
	      if (bTrimBioseq) CddShrinkBioseq(bspNew);
/*---------------------------------------------------------------------------*/
/* Add Additional SequenceIds in case this is a 3D structure                 */
/*---------------------------------------------------------------------------*/
	      if (pcdsThis->bIsPdb) {
	        sipXtra = ValNodeNew(NULL);
	        sipXtra->data.intvalue = pcdsThis->uid;
	        sipXtra->choice = SEQID_GI;
	        bspNew->id->next = sipXtra;
                sipXtra = ValNodeNew(NULL);
	        sipXtra->choice = SEQID_GENERAL;
	        dbtp = (DbtagPtr) DbtagNew();
	        dbtp->db = (CharPtr) StringSave("mmdb");
	        oidp= ObjectIdNew();
	        oidp->id = pcdsThis->iMMDBId;
	        dbtp->tag = oidp;
	        sipXtra->data.ptrvalue = dbtp;
		annot = SeqAnnotNew();
	        annot->type = (Uint1) 4;
	        annot->data = sipXtra;
                if (!bspNew->annot) {
	          bspNew->annot = annot;
                } else {
	          thisAnnot = bspNew->annot;
		  while (thisAnnot->next) thisAnnot = thisAnnot->next;
	          thisAnnot->next = annot;
	        }
	        /* bspNew->id->next->next = sipXtra; */
	      }
	      /* bspNew = BioseqLockById(sip); */
              sepNew = SeqEntryNew();
              sepNew->data.ptrvalue = bspNew;
              sepNew->choice = 1;
              ValNodeLink(&(pbsaSeq->sequences), sepNew);
            }
            SeqEntryFree(sep);
          }
          sip = sip->next;
          CddSumLink(&(pcds),pcdsThis);
        }
      }
      else {
        if (bVerbose)
          printf("Warning: %s - could not find uid #%d[%d] in PUBSEQ, removed from Cdd\n",
	         cCDDid,iCddSize,sip->data.intvalue);
        sip=sip->next;
        iPcount++;
      }
    }
    salpHead = salpHead->next;
  }
  salpTail->next = NULL;
  *n_Pdb = nPdb;
  *salpInOut = salpCopy;
  return(pcds);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* move alignments to PDB-derived sequences up in a list                     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static SeqAlignPtr CddAlignSort(SeqAlignPtr salp, CddSumPtr pcds)
{
  CddSumPtr     pcdsThis;
  DenseDiagPtr  ddp;
  DenseSegPtr   dsp;
  SeqIdPtr      sip;
  SeqAlignPtr   salpThis;
  SeqAlignPtr   salpStruct = NULL;
  SeqAlignPtr   salpStructHead = NULL;
  SeqAlignPtr   salpSeq = NULL;
  SeqAlignPtr   salpSeqHead = NULL;
  Boolean       bCont;

  salpThis = salp;
  while (salpThis) {
    ddp = (DenseDiagPtr) salpThis->segs;
    sip = ddp->id;
    sip = sip->next;
    
    bCont = FALSE;
    pcdsThis = pcds;
    while (pcdsThis) {
      if (pcdsThis->bIsPdb && CddSameSip(pcdsThis->sip,sip)) {
        bCont = TRUE; break;
      }
      pcdsThis = pcdsThis->next;
    }
    if (bCont && sip->choice == 15) {
      if (!salpStruct) {
        salpStruct = salpThis; salpStructHead = salpThis;
      } else {
        salpStruct->next = salpThis;
        salpStruct = salpStruct->next;
      }
    } else {
      if (!salpSeq) {
        salpSeq = salpThis; salpSeqHead = salpThis;
      } else {
        salpSeq->next = salpThis;
        salpSeq = salpSeq->next;
      }
    }
    salpThis = salpThis->next;
  }
  if (salpSeqHead) {
    if (!salpStructHead) return(salpSeqHead);
    salpStruct->next = salpSeqHead;
    salpSeq->next = NULL;
  } else {
    salpStruct->next = NULL;
  }
  return(salpStructHead);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* move all pdb-derived sequences up in the list                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CddSumPtr CddSumSort(CddSumPtr pcds)
{
  CddSumPtr pcdsThis;
  CddSumPtr pcdsStruct = NULL;
  CddSumPtr pcdsStructHead = NULL;
  CddSumPtr pcdsSeq = NULL;
  CddSumPtr pcdsSeqHead = NULL;

  pcdsThis = pcds;
  while (pcdsThis) {
    if (pcdsThis->bIsPdb) {
      if (!pcdsStruct) {
        pcdsStruct = pcdsThis; pcdsStructHead = pcdsThis;
      } else {
        pcdsStruct->next = pcdsThis;
        pcdsStruct = pcdsStruct->next;
      }
    } else {
      if (!pcdsSeq) {
        pcdsSeq = pcdsThis; pcdsSeqHead = pcdsThis;
      } else {
        pcdsSeq->next = pcdsThis;
        pcdsSeq = pcdsSeq->next;
      }
    }
    pcdsThis = pcdsThis->next;
  }
  if (pcdsSeq) {
    pcdsStruct->next = pcdsSeqHead;
    pcdsSeq->next = NULL;
  } else {
    pcdsStruct->next = NULL;
  }
  return(pcdsStructHead);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Read information about which VAST data have to be retrieved               */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddReadVASTInfo(CddSumPtr pcds)
{
  FILE             *fp;
  Char             pcBuf[100];
  Char             path[PATH_MAX];
  Char             cSlave[7];
  Char             cMaster[7];
  Char             cPKBMDom[7];
  Char             cPKBDom[9];
  CharPtr          pcTest;
  CddSumPtr        pcdsThis;

  Nlm_StrCpy(path,CDDvpath);
  Nlm_StrCat(path,cCDDid);
  Nlm_StrCat(path,".VASTinfo");
  if (!(fp=FileOpen(path, "r"))) return;
  do {
    pcBuf[0]='\0';
    pcTest = fgets(pcBuf, (size_t)100,fp);
    if (pcTest) {
      Nlm_StrCpy(cMaster,strtok(pcTest,"\t"));  cMaster[6]  = '\0';
      Nlm_StrCpy(cSlave,strtok(NULL,"\t"));     cSlave[6]   = '\0';
      Nlm_StrCpy(cPKBMDom,strtok(NULL,"\t"));   cPKBMDom[6] = '\0';
      strncpy(cPKBDom,strtok(NULL,"\t"),8); cPKBDom[8]  = '\0';
      pcdsThis = pcds;
      while (pcdsThis) {
        if (pcdsThis->bIsPdb && !(pcdsThis->bIsMaster)) {
          if (strncmp(cSlave,pcdsThis->cPdbId,4)==0) {
            if (pcdsThis->cChainId[0]==cSlave[5]) {
              Nlm_StrCpy(pcdsThis->cPKBMDom,cPKBMDom);
              Nlm_StrCpy(pcdsThis->cPKBDom,cPKBDom);
            }
          } 
        }
        pcdsThis = pcdsThis->next;
      }
    }      
  } while (pcTest);
  FileClose(fp);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* checks for general overlap (i.e. two intervals have at least one position */
/* in common                                                                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean OverlapInterval(Int4 from1, Int4 to1, Int4 from2, Int4 to2)
{
  if (from1 <= to2 && to1 >= from2) return (TRUE);
  else return (FALSE);
}

/*----------------------------------------------------------------------------*/
/* The callback routine - for sorting aligned blocks from the real.ind file   */
/*----------------------------------------------------------------------------*/
static int LIBCALLBACK CompareBlocks(VoidPtr vp1, VoidPtr vp2)
{
  RealIndPtr      pri1 = NULL;
  RealIndPtr      pri2 = NULL;
  ValNodePtr      vnp1, vnp2;
  ValNodePtr PNTR vnpp1;
  ValNodePtr PNTR vnpp2;
  int             bigr = 1;
  int             smlr = -1;

  vnpp1 = (ValNodePtr PNTR) vp1;
  vnpp2 = (ValNodePtr PNTR) vp2;
  vnp1 = *vnpp1;
  vnp2 = *vnpp2;
  pri1 = (RealIndPtr) vnp1->data.ptrvalue;
  pri2 = (RealIndPtr) vnp2->data.ptrvalue;

  if (pri1 == NULL && pri2 != NULL) return bigr;
  if (pri1 != NULL && pri2 == NULL) return smlr;
  if (pri1 == NULL && pri2 == NULL) return 0;

  if      (pri1->aid > pri2->aid) return bigr;
  else if (pri1->aid < pri2->aid) return smlr;
  
  if      (pri1->mstart > pri2->mstart) return bigr;
  else if (pri1->mstart < pri2->mstart) return smlr;
  
  if      (pri1->mstop > pri2->mstop) return bigr;
  else if (pri1->mstop < pri2->mstop) return smlr;
  
  if      (pri1->sstart > pri2->sstart) return bigr;
  else if (pri1->sstart < pri2->sstart) return smlr;
  
  if      (pri1->sstop > pri2->sstop) return bigr;
  else if (pri1->sstop < pri2->sstop) return smlr;
  
  return 0;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* heapsort wrapper, stolen from utilpub.c, to deal with larger lists        */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CDDVnpHeapSort (ValNodePtr PNTR vnp, int (LIBCALLBACK *compar )PROTO ((Nlm_VoidPtr, Nlm_VoidPtr )))	

{
  Int4 index, total;
  ValNodePtr vnp1;
  ValNodePtr PNTR temp;

  if (vnp == NULL  ||  *vnp == NULL)  return;  /* do nothing */
  total=0;
  for (vnp1 = *vnp; vnp1; vnp1=vnp1->next) total++;
  temp = (ValNodePtr PNTR) MemNew(total*sizeof(ValNodePtr));
  index=0;
  for (vnp1 = *vnp; vnp1; vnp1=vnp1->next) {
    temp[index] = vnp1;
    index++;
  }
  HeapSort ((VoidPtr) temp, (size_t) index, sizeof(ValNodePtr), compar);
  *vnp = temp[0];
  for (vnp1 = *vnp, index=0; index<(total-1); vnp1=vnp1->next, index++) {
    vnp1->next = temp[index+1];
  }
  vnp1 = temp[total-1];
  vnp1->next = NULL;
  temp = MemFree(temp);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* read in an alignment formatted in the "real.ind" style, i.e. in a white-  */
/* space delimited 6 column format: ID of master, ID of slave, start/stop    */
/* pairs in master and slave. IDs are gi's, and contain PDB-Ids if structure */
/* derived which are separated from the gi's with a vertical bar             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAlignPtr CddReadRealInd(CharPtr name)
{
  FILE             *infile;
  SeqAlignPtr       salp, salpHead = NULL, salpTail = NULL;
  DenseDiagPtr      ddp, ddpTail;
  RealIndPtr        priThat, priThis, priHead = NULL, priTail = NULL, priTmp;
  PDBSeqIdPtr       pdb_seq_id;
  SeqIdPtr          sip;
  Char              mid[15], sid[15];
  CharPtr           nxtid;
  CharPtr           pdbid;
  Int4              mstart, mstop, sstart, sstop;
  Int4              i, lastid = -1;
  Int4             *conflict;
  ValNodePtr        vnp = NULL;
  
  if ((infile = FileOpen(name,"r"))==NULL)
    CddSevError("Could not open real.ind-file!");
/*---------------------------------------------------------------------------*/
/* scan in file and fill the RealInd data structures                         */
/*---------------------------------------------------------------------------*/
  while (fscanf(infile,"%s %s %d %d %d %d\n",mid,sid,&mstart,&mstop,&sstart,&sstop)!=EOF) {
    priThis = (RealIndPtr) MemNew(sizeof(RealInd));
    priThis->mstart = mstart;
    priThis->mstop = mstop;
    priThis->sstart = sstart;
    priThis->sstop = sstop;
    priThis->aid = -1;
    nxtid = strstr(mid,"|");
    if (nxtid) {
      pdbid = nxtid + 1;
      pdb_seq_id = (PDBSeqIdPtr) PDBSeqIdNew();
      if (strlen(pdbid) > 4) pdb_seq_id->chain = (Uint1) *(pdbid+4);
      pdbid[4]='\0';
      pdb_seq_id->mol = StringSave(pdbid);
      strtok(mid,"|");
      priThis->mgi = atoi(mid);
      sip = ValNodeNew(NULL);
      sip->choice = SEQID_PDB;
      sip->data.ptrvalue = pdb_seq_id;
      priThis->msip = sip;
      priThis->mIsPdb = TRUE;
    } else {
      priThis->mgi = atoi(mid);
      sip = ValNodeNew(NULL);
      sip->choice = SEQID_GI;
      sip->data.intvalue = priThis->mgi;
      priThis->msip = sip;
      priThis->mIsPdb = FALSE;
    }
    nxtid = strstr(sid,"|");
    if (nxtid) {
      pdbid = nxtid + 1;
      pdb_seq_id = (PDBSeqIdPtr) PDBSeqIdNew();
      if (strlen(pdbid) > 4) pdb_seq_id->chain = (Uint1) *(pdbid+4);
      pdbid[4]='\0';
      pdb_seq_id->mol = StringSave(pdbid);
      strtok(sid,"|");
      priThis->sgi = atoi(sid);
      sip = ValNodeNew(NULL);
      sip->choice = SEQID_PDB;
      sip->data.ptrvalue = pdb_seq_id;
      priThis->ssip = sip;
      priThis->sIsPdb = TRUE;
    } else {
      priThis->sgi = atoi(sid);
      sip = ValNodeNew(NULL);
      sip->choice = SEQID_GI;
      sip->data.intvalue = priThis->sgi;
      priThis->ssip = sip;
      priThis->sIsPdb = FALSE;
    }
    if (priHead) {
      priTail->next = priThis;
      priTail = priThis;
      priThis->next = NULL;
    } else {
      priHead = priThis;
      priTail = priThis;
      priThis->next = NULL;      
    }
  }
  priThis = priHead;
  while (priThis) {
    if (lastid < 0) {
      priThis->aid = 0;
      lastid = 0;
    } else {
      if (priThis->mgi != priHead->mgi)
	CddSevError("Inconsistent master-id's in real.ind file!");
      conflict = MemNew((lastid+1)*sizeof(Int4));
      for (i=0;i<=lastid;i++) conflict[i] = 0;  
      priThat = priHead;
      while (priThat != priThis) {
        if (priThat->sgi != priThis->sgi) {
          conflict[priThat->aid] = 1;
        } else {
	  if (OverlapInterval(priThis->mstart,priThis->mstop,priThat->mstart,priThat->mstop))
	    conflict[priThat->aid] = 1;
	  if (OverlapInterval(priThis->sstart,priThis->sstop,priThat->sstart,priThat->sstop))
	    conflict[priThat->aid] = 1;
	  if (priThis->mstart < priThat->mstart &&
	      priThis->sstart > priThat->sstart) 
	    conflict[priThat->aid] = 1;
	  if (priThis->mstart > priThat->mstart &&
	      priThis->sstart < priThat->sstart) 
	    conflict[priThat->aid] = 1;
	}
        priThat = priThat->next;
      }    
    
      for (i=0;i<=lastid;i++) {
        if (conflict[i] == 0) {
	  priThis->aid = i;
          break;
	}
      }
      if (priThis->aid == -1) priThis->aid = ++lastid;
      MemFree(conflict);
    }
    priThis = priThis->next;
  }
  priThis = priHead;
  while (priThis) {
    ValNodeAddPointer(&vnp,0,priThis);
    priThis = priThis->next;
  }
  CDDVnpHeapSort(&vnp,CompareBlocks);
  priHead = NULL;
  priTail = NULL;
  while (vnp) {
    priThis = vnp->data.ptrvalue;
    if (priHead) {
      priTail->next = priThis;
      priTail = priThis;
      priThis->next = NULL;
    } else {
      priHead = priThis;
      priTail = priThis;
      priThis->next = NULL;
    }
    vnp = vnp->next;
  }
  priThis = priHead; priThat = NULL;
  while (priThis) {
    if (priThis == priHead || (priThat && priThis->aid != priThat->aid)) {
      salp = (SeqAlignPtr) SeqAlignNew();
      salp->type =    (Uint1) SAT_PARTIAL;
      salp->segtype = (Uint1) SAS_DENDIAG;
      salp->dim = 2;
      ddpTail = NULL;
      if (!salpHead) {
        salpHead = salp;
	salpTail = salp;
        salpTail->next = NULL;
      } else {
	salpTail->next = salp;
        salpTail = salp;        
        salpTail->next = NULL;
      }
    }
    ddp = DenseDiagNew();
    ddp->next = NULL;
    ddp->dim = 2;
    ddp->id = priThis->msip;
    ddp->id->next = priThis->ssip;
    ddp->len = priThis->mstop - priThis->mstart + 1;
    ddp->starts = MemNew(2*sizeof(Int4));
    ddp->starts[0] = priThis->mstart-1;
    ddp->starts[1] = priThis->sstart-1;
    if (ddpTail) {
      ddpTail->next = ddp;
      ddpTail = ddp;
    } else {
      salp->segs = ddp;
      ddpTail = ddp;
    }
    priThat = priThis;
    priThis = priThis->next;
  }

  FileClose(infile);
  return (salpHead);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* From the VAST info get the feature set id's required for slaves           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddDetermineFsids(CddSumPtr pcds, PDNMS pModelStruc)
{
  CddSumPtr       pcdsThis;
  PDNMG           pdnmg;
  PDNMM           pdnmm;
  PMGD            pmgd;
  PMMD            pmmd;
  PMSD            pmsd;
  Int4            uid;
  Int4            ichn, idom, domcnt;
  Int4            chndom0, chndom;
  Int2            cnt;
  Char            cDid[3];

  pmsd = (PMSD) pModelStruc->data.ptrvalue;
  uid = (Int4) pmsd->iMMDBid;

  for (pdnmm=pmsd->pdnmmHead,cnt=ichn=0;pdnmm!=NULL;pdnmm=pdnmm->next) {
    pmmd = (PMMD) pdnmm->data.ptrvalue;
/*---------------------------------------------------------------------------*/
/* getting the chain id from pmmd seems to be a safer way than to increment  */
/*---------------------------------------------------------------------------*/
    ichn = pmmd->iChainId;
    if ((pmmd->bWhat) & AM_PROT) {
      if ((pmmd->iResCount <= 1) || (pmmd->iGi <= 0)) continue;
      chndom0 = 10000 * uid + 100 * (Int4) ichn;
      pcdsThis = pcds;
      while (pcdsThis) {
        if (pcdsThis->bIsPdb && !pcdsThis->bIsMaster) {
          if (pcdsThis->cPKBMDom[4]==pmmd->pcMolName[0]) { 
            if (pcdsThis->cPKBMDom[5]=='0') pcdsThis->iFsid = chndom0;
          }
        }
        pcdsThis = pcdsThis->next;
      }
      idom = 0; domcnt = 0;
      for (pdnmg=pmmd->pdnmgHead; pdnmg != NULL; pdnmg=pdnmg->next) {
        pmgd = pdnmg->data.ptrvalue;
        if (pmgd->iDomain > idom) {
          idom = (Int4) pmgd->iDomain;
          chndom = chndom0+idom;
          domcnt++;
          pcdsThis = pcds;
          while (pcdsThis) {
            if (pcdsThis->bIsPdb && !pcdsThis->bIsMaster) {
              if (pcdsThis->cPKBMDom[4]==pmmd->pcMolName[0]) { 
                Nlm_StrCpy(cDid,&(pcdsThis->cPKBMDom[5]));
                if (atoi(cDid)==domcnt) pcdsThis->iFsid = chndom;
              }
            }
            pcdsThis = pcdsThis->next;
          }
        }
      }
    }
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* get the Feature id's identifying the correct slaves in the VAST data      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddDetermineFids(CddSumPtr pcds, PDNMS pModelStruc, CharPtr szName)
{
  CddSumPtr       pcdsThis;
  PDNMG           pdnmg;
  PDNMM           pdnmm;
  PMGD            pmgd;
  PMMD            pmmd;
  PMSD            pmsd;
  Int4            uid;
  Int4            ichn, idom, domcnt;
  Int4            chndom0, chndom;
  Int2            cnt;
  Char            cDid[3];

  pmsd = (PMSD) pModelStruc->data.ptrvalue;
  uid = (Int4) pmsd->iMMDBid;

  for (pdnmm=pmsd->pdnmmHead,cnt=ichn=0;pdnmm!=NULL;pdnmm=pdnmm->next) {
    pmmd = (PMMD) pdnmm->data.ptrvalue;
    ichn = pmmd->iChainId;
    if ((pmmd->bWhat) & AM_PROT) {
      if ((pmmd->iResCount <= 1) || (pmmd->iGi <= 0)) continue;
      chndom0 = 100000 * uid + 1000 * (Int4) ichn;
      pcdsThis = pcds;
      while (pcdsThis) {
        if (pcdsThis->bIsPdb && !pcdsThis->bIsMaster) {
          if (strncmp(pcdsThis->cPdbId,szName,4)==0) {
            if (pcdsThis->cPKBDom[5]==pmmd->pcMolName[0]) { 
              if (pcdsThis->cPKBDom[7]==' ') {
                chndom = chndom0 + 1;
                if (pcdsThis->iFid == -1) pcdsThis->iFid = chndom;
              }
            }
          }
        }
        pcdsThis = pcdsThis->next;
      }
      idom = 0; domcnt = 0;
      for (pdnmg=pmmd->pdnmgHead; pdnmg != NULL; pdnmg=pdnmg->next) {
        pmgd = pdnmg->data.ptrvalue;
        if (pmgd->iDomain > idom) {
          idom = (Int4) pmgd->iDomain;
          chndom = chndom0 + idom * 10;
          domcnt++;
          pcdsThis = pcds;
          while (pcdsThis) {
            if (pcdsThis->bIsPdb && !pcdsThis->bIsMaster) {
              if (strncmp(pcdsThis->cPdbId,szName,4)==0) {
                if (pcdsThis->cPKBDom[5]==pmmd->pcMolName[0]) { 
                  Nlm_StrCpy(cDid,&(pcdsThis->cPKBDom[7]));
                  chndom = chndom + 1;
                  if (atoi(cDid)==domcnt) {
                     if (pcdsThis->iFid == -1) pcdsThis->iFid = chndom;
                  }
                }
              }
            }
            pcdsThis = pcdsThis->next;
          }
        }
      }
    }
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Identify Master Feature Set Id's that go with each aligned slave.         */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddIdentifyFsids(CddSumPtr pcds)
{
  Int4            iMMDBid;
  Char            chain[2], szName[5];
  BiostrucPtr     pbsXtra, pbsTemp;
  Int4            iModelComplexity = ONECOORDRES;
  CddSumPtr       pcdsThis;
  PDNMS           pModelStruc;
  
  iMMDBid = pcds->iMMDBId;
  Nlm_StrCpy(szName,pcds->cPdbId);
#ifndef DBNTWIN32
  pbsXtra = (BiostrucPtr) MMDBBiostrucGet(ConvertMMDBUID(szName),iModelComplexity,1);
#else
  pbsXtra = (BiostrucPtr) openBSP(constructLiveOrDeadMmdbIdForPdbId(szName),iModelComplexity,1,TRUE,TRUE,FALSE,NULL,NULL);
#endif
  Nlm_StrCpy(chain,pcds->cChainId);
  if (chain[0] != ' ') {
    pbsTemp = (BiostrucPtr)PruneBiostruc(pbsXtra,chain);
    pbsXtra = NULL;
    pbsXtra = pbsTemp;
  }
  pModelStruc = MakeAModelstruc(pbsXtra);
  CddDetermineFsids(pcds, pModelStruc);
  ClearStructures();
  pcdsThis = pcds->next;
  while (pcdsThis) {
    if (pcdsThis->bIsPdb) {
      Nlm_StrCpy(szName,pcdsThis->cPdbId);
#ifndef DBNTWIN32
      pbsXtra = (BiostrucPtr) MMDBBiostrucGet(ConvertMMDBUID(szName),iModelComplexity,1);
#else
      pbsXtra = (BiostrucPtr) openBSP(constructLiveOrDeadMmdbIdForPdbId(szName),iModelComplexity,1,TRUE,TRUE,FALSE,NULL,NULL);
#endif
      Nlm_StrCpy(chain,pcdsThis->cChainId);
      if (chain[0] != ' ') {
        pbsTemp = (BiostrucPtr)PruneBiostruc(pbsXtra,chain);
        pbsXtra = NULL;
        pbsXtra = pbsTemp;
      }
      iMMDBid = pcdsThis->iMMDBId;
      pModelStruc = MakeAModelstruc(pbsXtra);
      if (pModelStruc) CddDetermineFids(pcds,pModelStruc,szName);
      ClearStructures();
    }
    pcdsThis = pcdsThis->next;
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* stolen from vastlocl.c                                                    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
#ifndef DBNTWIN32
static BiostrucAnnotSetPtr CddVASTBsAnnotSetGet (Int4 uid)
{
  AsnIoPtr            aip = NULL;
  AsnTypePtr          atp = NULL;
  Char                path[PATH_MAX];
  Char                compath[PATH_MAX];
  Char                tempfile[PATH_MAX];
  Char                pcId[20];    
  Int2                iFileExists = 0;
  BiostrucAnnotSetPtr pbsa = NULL;
  int                 iAvail = 1;
  FILE                *pipe;

  sprintf(pcId, "%ld", (long) uid);
  path[0] = '\0';
  StringCpy(path, database);
  StringCat(path, pcId);
  StringCat(path, ".bas");

#ifdef MMDB_UNIXCOMPRESSED
  compath[0] = '\0';
  sprintf(compath, "%s -c %s.gz ", gunzip, path);
  pipe = popen(compath, "rb");
  if (pipe == NULL) {
    ErrPostEx(SEV_FATAL,0,0, "VASTBsAnnotSetGet failed: Can't find gunzip in path.\n");
    return NULL;
  }
  aip = AsnIoNew(ASNIO_BIN_IN, pipe, NULL, NULL, NULL);
#else
  iFileExists = FileLength(path);
  if (iFileExists == 0) {
    return NULL;
  }
  aip = AsnIoOpen(path, "rb");
#endif
  if (aip) {
    pbsa = BiostrucAnnotSetAsnRead(aip, NULL);
    AsnIoClose (aip);
  }
#ifdef MMDB_UNIXCOMPRESSED 
   pclose(pipe);
#endif
   if (!pbsa) return NULL;  
   return pbsa;
}
#endif // #ifndef DBNTWIN32

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* stolen from vastlocl.c                                                    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean CddIsVASTData(Int4 uid)
{
  AsnIoPtr   aip = NULL;
  AsnTypePtr atp = NULL;
  Char       path[PATH_MAX];
  Char       pcId[30];

  sprintf(pcId, "%ld", (long) uid);
  path[0] = '\0';
  StringCpy(path, database);
  StringCat(path, pcId);
  StringCat(path, ".bas");

#ifdef MMDB_UNIXCOMPRESSED 
  StringCat(path, ".gz");
  if (FileLength(path) != 0) return TRUE;
#else
  if (FileLength(path) != 0) return TRUE;
#endif
   return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* stolen from vastsrv.c                                                     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static BiostrucAnnotSetPtr CddLocalGetFeatureSet(Int4 mmdbid, Int4 feature_set_id)
{
  BiostrucAnnotSetPtr   basp = NULL;
  BiostrucAnnotSetPtr   basp2 = NULL;
  BiostrucFeatureSetPtr pbsfs = NULL;
  BiostrucFeatureSetPtr pbsfsLast = NULL;
    
  if (CddIsVASTData(mmdbid))
#ifndef DBNTWIN32
    basp = (BiostrucAnnotSetPtr) CddVASTBsAnnotSetGet(mmdbid);
#else
    basp = (BiostrucAnnotSetPtr) constructBASPForLongDomId(mmdbid);
#endif
  else if (CddIsVASTData(feature_set_id)) {
#ifndef DBNTWIN32
    basp = (BiostrucAnnotSetPtr) CddVASTBsAnnotSetGet(feature_set_id);
#else
    basp = (BiostrucAnnotSetPtr) constructBASPForLongDomId(feature_set_id);
#endif
    if (basp != NULL) return basp;
  } 

  if (basp == NULL) return NULL;
 
  pbsfs = basp->features;
  pbsfsLast  = NULL;
  basp2 = NULL;
  while (pbsfs) {
    if (pbsfs->id == feature_set_id) {
      basp2 = BiostrucAnnotSetNew();
      basp2->id = basp->id;
      basp->id = NULL; /* unlink the id valnode from basp object */
      basp2->descr = basp->descr; 
      basp->descr = NULL;  /* unlink the descr from basp object */
      basp2->features = pbsfs;
      if (pbsfsLast) /* relink next to prev */
        pbsfsLast->next = pbsfs->next;
      else basp->features = pbsfs->next;         
      basp2->features->next = NULL;
      BiostrucAnnotSetFree(basp);
      return basp2;
    }
    pbsfsLast = pbsfs;
    pbsfs = pbsfs->next;
  }   
  BiostrucAnnotSetFree(basp);
  return basp2;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* stolen from vastsrv.c                                                     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static BiostrucAnnotSetPtr CddBiostrucAnnotSetGetByFid (BiostrucAnnotSetPtr basp, Int4 feature_id, Int4 feature_set_id)
{
  BiostrucAnnotSetPtr   basp2 = NULL;
  BiostrucFeatureSetPtr pbsfs = NULL;
  BiostrucFeaturePtr    pbsf = NULL;

  if (basp == NULL) return NULL;
 
  pbsfs = basp->features;
  while (pbsfs) {
    if (pbsfs->id == feature_set_id) {
      pbsf =  pbsfs->features;
      while(pbsf) {
        if (pbsf->id == feature_id) {  /* found it */
          basp2 = BiostrucAnnotSetNew();
          basp2->id = basp->id;
          basp->id = NULL; /* unlink the id valnode from basp object */
          basp2->descr = basp->descr; 
          basp->descr = NULL;  /* unlink the descr from basp object */
          basp2->features = BiostrucFeatureSetNew();
          basp2->features->id = pbsfs->id;
          basp2->features->descr = pbsfs->descr;
          pbsfs->descr = NULL; /* unlink the feature-set descr from basp  object */
          basp2->features->features = BiostrucFeatureNew();
          basp2->features->features->id = pbsf->id;
          basp2->features->features->name = StringSave(pbsf->name);
          basp2->features->features->type = pbsf->type;
          basp2->features->features->Property_property = pbsf->Property_property;
          pbsf->Property_property = NULL; /* unlink the property from basp  object */
          basp2->features->features->Location_location = pbsf->Location_location;
          pbsf->Location_location = NULL; /* unlink the location from basp  object */ 
          BiostrucAnnotSetFree(basp);
          return basp2;
        }
        pbsf = pbsf->next;
      }
    }
    pbsfs = pbsfs->next;
  }
  BiostrucAnnotSetFree(basp);
  return basp2;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Get the BiostrucAnnotSets from the VAST neighbor data                     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddLoadBSAnnotSets(CddSumPtr pcds, Int4 *nPdb, Int2 *iSeqStrMode,
                               SeqAlignPtr salp, BiostrucAlignPtr pbsaStruct)
{
  CddSumPtr              pcdsThis;
  BiostrucAnnotSetPtr    pbsa = NULL;
  Int4                   iMMDBid, iDomain, iMmid, iSmid;
  Boolean                bShrunk = FALSE;
  Boolean                bRemove = FALSE;
  BiostrucAnnotSetPtr    pbsaShort = NULL;
  BiostrucFeaturePtr     pbsf;
  BiostrucPtr            pbsSlaveHead = NULL;
  BiostrucPtr            pbsSlaveTail, pbsSlave;
  BiostrucIdPtr          pbsi;
  SeqAlignPtr            salpCopy, salpHead;
  SeqIdPtr               mastersip, pdbsip, slavesip, sip;
  CharPtr                pcPDB;
  Char                   cChain;
  ValNodePtr             pvnAlignment;
  ChemGraphAlignmentPtr  cgap;
  ChemGraphPntrsPtr      cgpp;
  ResidueIntervalPntrPtr mrip, srip, masterrip, mtailrip, stailrip, slaverip;
  DenseDiagPtr           ddp;


  pcdsThis = pcds;
  while (pcdsThis) {
    if (pcdsThis->bIsPdb && !pcdsThis->bIsMaster) {
      pbsa = (BiostrucAnnotSetPtr) CddLocalGetFeatureSet(pcds->iMMDBId,pcdsThis->iFsid);
      pcdsThis->pbsaShort = CddBiostrucAnnotSetGetByFid(pbsa, pcdsThis->iFid, pcdsThis->iFsid);
      if (pcdsThis->pbsaShort) {
        if (pbsaShort == NULL) {
          pbsaShort = pcdsThis->pbsaShort;
          pbsf = pbsaShort->features->features;
          pbsf->next = NULL;
        } else {
          pbsf->next = pcdsThis->pbsaShort->features->features;
          pbsf = pbsf->next;
          pbsf->next = NULL;
        }
      } else {
        bShrunk = TRUE;
        pcdsThis->bIsPdb = FALSE;
        *nPdb--;
/*---------------------------------------------------------------------------*/
/* if a particular slave is not a VAST neighbor, the slave structure must be */
/* removed - otherwise Cn3D will barf                                        */
/*---------------------------------------------------------------------------*/
        iMMDBid = pcdsThis->iMMDBId;
        pbsSlaveHead = pbsaStruct->slaves;
        pbsSlave = pbsSlaveHead;
        pbsSlaveTail = NULL;
        while (pbsSlaveHead) {
          bRemove = FALSE;
          pbsi=pbsSlaveHead->id;
          while (pbsi) {
            if (pbsi->choice == BiostrucId_mmdb_id) {
              if (pbsi->data.intvalue == iMMDBid) {
                bRemove = TRUE;
                if (!pbsSlaveTail) {
                  pbsSlave = pbsSlaveHead->next;
                  pbsSlaveTail = pbsSlave;
                } else {
                  pbsSlaveTail->next = pbsSlaveHead->next;
                  pbsSlaveTail = pbsSlaveTail->next;
                }
                break;
              }
            }
            pbsi = pbsi->next;
          }
          if (!bRemove) pbsSlaveTail = pbsSlaveHead;
          pbsSlaveHead = pbsSlaveHead->next;
        }
        pbsaStruct->slaves = pbsSlave;
      }
    }
    pcdsThis = pcdsThis->next;
  }
/*---------------------------------------------------------------------------*/
/* need to reorder the SeqAligns if some PDB-derived seqs. are not VAST ngb. */
/*---------------------------------------------------------------------------*/
  if (bShrunk) {
    if (*nPdb > 1) {
      salpCopy = CddAlignSort(salp,pcds);
      salp = salpCopy;
    }
  }
  if (pbsaShort) pbsaStruct->alignments = pbsaShort;  
  if (*nPdb == 1) {
    if (*iSeqStrMode == SEVSTRUC) *iSeqStrMode = ONESTRUC;
    if (*iSeqStrMode == CDDSEVSTRUC) *iSeqStrMode = CDDONESTRUC;
  }
/*---------------------------------------------------------------------------*/
/* if required, change structure alignment to reflect SMART sequence align-  */
/*  ment, i.e. "fix coloring" in the Cn3D display                            */
/*---------------------------------------------------------------------------*/
  if (pbsaStruct->alignments) {
    pbsf = pbsaStruct->alignments->features->features;
    while (pbsf) {
      pcPDB=StringSave(PDBNAME_DEFAULT);
      iDomain = 0; cChain = '-';
      pcPDB[0] = pbsf->name[0]; pcPDB[1] = pbsf->name[1];
      pcPDB[2] = pbsf->name[2]; pcPDB[3] = pbsf->name[3];
      cChain = pbsf->name[4];
      iDomain = atoi ((char *) &pbsf->name[5]);
      mastersip = MakePDBSeqId2(pcPDB,cChain,iDomain,FALSE);
      pdbsip = MakePDBSeqId2(pcPDB,cChain,iDomain,FALSE);
      pcPDB[0] = pbsf->name[7]; pcPDB[1] = pbsf->name[8];
      pcPDB[2] = pbsf->name[9]; pcPDB[3] = pbsf->name[10];
      cChain = pbsf->name[11];
      iDomain = atoi ((char *) &pbsf->name[12]);
      slavesip = MakePDBSeqId2(pcPDB,cChain,iDomain,FALSE);
      MemFree(pcPDB);
      pvnAlignment = ValNodeFindNext(pbsf->Location_location,NULL,Location_location_alignment);
      cgap = (ChemGraphAlignmentPtr) pvnAlignment->data.ptrvalue;
      cgpp = (ChemGraphPntrsPtr) cgap->alignment->data.ptrvalue;
      masterrip = (ResidueIntervalPntrPtr) cgpp->data.ptrvalue;
      iMmid = masterrip->molecule_id;
      cgpp = (ChemGraphPntrsPtr) cgap->alignment->next->data.ptrvalue;
      slaverip = (ResidueIntervalPntrPtr) cgpp->data.ptrvalue;
      iSmid = slaverip->molecule_id;
/*---------------------------------------------------------------------------*/
/* now identify the CDD alignment corresponding to the current structure ali.*/
/*---------------------------------------------------------------------------*/
      salpHead = (SeqAlignPtr) salp;
      while (salpHead) {
        ddp = (DenseDiagPtr) salpHead->segs;
        sip = ddp->id;
        if (CddSameSip(sip,mastersip)) {
          sip = sip->next; if (CddSameSip(sip,slavesip)) {
/*---------------------------------------------------------------------------*/
/* if the alignment is found, start building up a new set of residue interv. */
/*---------------------------------------------------------------------------*/
            masterrip = NULL; slaverip = NULL;
            while (ddp) {
              mrip = ResidueIntervalPntrNew();
              srip = ResidueIntervalPntrNew();
              mrip->from = ddp->starts[0]+1;
              srip->from = ddp->starts[1]+1;
              mrip->to = ddp->starts[0]+ddp->len;
              srip->to = ddp->starts[1]+ddp->len;
              mrip->molecule_id = iMmid;
              srip->molecule_id = iSmid;
              if (!masterrip) {
                masterrip = mrip;
                slaverip = srip;
                mtailrip = masterrip;
                stailrip = slaverip;
              } else {
                mtailrip->next = mrip;
                stailrip->next = srip;
                mtailrip = mtailrip->next;
                stailrip = stailrip->next;
              }
              ddp = ddp->next;
            }
          }
        }
        salpHead = salpHead->next;
      }
      cgpp = (ChemGraphPntrsPtr) cgap->alignment->data.ptrvalue;
      cgpp->data.ptrvalue = masterrip;
      cgpp = (ChemGraphPntrsPtr) cgap->alignment->next->data.ptrvalue;
      cgpp->data.ptrvalue = slaverip;
      pbsf = pbsf->next;
    }
  }
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* allocate a new CddDesc linked list entry                                  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CddDescPtr CddDescNew()
{
  CddDescPtr pcdd;

  pcdd = (CddDescPtr)MemNew(sizeof(CddDesc));
  if (pcdd == NULL) return pcdd;
  pcdd->cCddId[0] = '\0';
  pcdd->cDescr[0] = '\0';
  pcdd->cSourc[0] = '\0';
  pcdd->next = NULL;
  return(pcdd);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Free a CddDesc linked list                                                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CddDescPtr CddDescFree(CddDescPtr pcdd)
{
  CddDescPtr    next;
  
  while (pcdd) {
    next = pcdd->next;
    MemFree(pcdd);
    pcdd = next;
  }
  return NULL;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* adds a to a linked list of CddDescPtr, always returns the beginning of the*/
/* list and always adds to the end of the list!!                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CddDescPtr CddDescLink(CddDescPtr PNTR head, CddDescPtr newnode)
{
  CddDescPtr     pcdd;
 
  if (head == NULL) return newnode;
  pcdd = *head;
  if (pcdd != NULL) {
    while(pcdd->next != NULL) pcdd = pcdd->next;
    pcdd->next = newnode;
  } else *head = newnode;
  return *head;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Read the table of Cdd Names and descriptions                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CddDescPtr CddReadDescr() {
  FILE             *fp;
  Char              pcBuf[CDD_MAX_DESCR];
  CharPtr           pcTest;
  CddDescPtr        pcdd = NULL;
  CddDescPtr        pcddThis;

  if (!(fp = FileOpen(CDDdescr, "r"))) 
    CddSevError("Can not read description file!");
  do {
    pcBuf[0]='\0';
    pcTest = fgets(pcBuf, (size_t)CDD_MAX_DESCR, fp);
    if (pcTest) if (pcTest[0] != ' ') {
      pcddThis = CddDescNew();
      Nlm_StrCpy(pcddThis->cCddId,strtok(pcTest,"\t"));
      Nlm_StrCpy(pcddThis->cDescr,strtok(NULL,"\t"));
      Nlm_StrCpy(pcddThis->cSourc,strtok(NULL,"\t"));
      CddDescLink(&(pcdd),pcddThis);
    }
  } while (pcTest);
  FileClose(fp);
  return(pcdd);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return a pointer to the master sequence id                                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static SeqIdPtr CddDetermineMasterSip(CddPtr pcdd)
{
  SeqAnnotPtr    psa;
  SeqAlignPtr    salp;
  DenseDiagPtr   ddp;
  SeqIdPtr       sip;
  
  if (!pcdd) return NULL;
  psa = (SeqAnnotPtr) pcdd->seqannot;
  if (!psa) return NULL;
  salp = (SeqAlignPtr) psa->data;
  if (!salp) return NULL;
  ddp = salp->segs;
  if (!ddp) return NULL;
  sip = ddp->id;
  return sip;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* local version of tax1_join                                                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Int4 Cdd_tax1_join(Int4 taxid1, Int4 taxid2)
{
  TXC_TreeNodePtr *txtnpp1;
  TXC_TreeNodePtr *txtnpp2;
  Int4             inLineage1, inLineage2;
  Int4             i, j;
  Int4             retid = -1;
  

  if (taxid1 == taxid2) return (taxid1);
  if (taxid1 <= 1 || taxid2 <=1) return(1);
  txtnpp1 = (TXC_TreeNodePtr *)txc_getLineage(taxid1,&inLineage1);
  txtnpp2 = (TXC_TreeNodePtr *)txc_getLineage(taxid2,&inLineage2);
/*---------------------------------------------------------------------------*/
/* kludge to deal with taxonomies extinct in the NCBI tax tree               */  
/*---------------------------------------------------------------------------*/
  if (!txtnpp1 && txtnpp2) return (taxid2);
  if (!txtnpp2 && txtnpp1) return (taxid1);

  if (txtnpp1 && txtnpp2 && inLineage1 > 0 && inLineage2 > 0) {
    for (i=0;i<inLineage1;i++) {
      for (j=0;j<inLineage2;j++) {
        if (txtnpp1[i]->tax_id == txtnpp2[j]->tax_id) {
          retid = txtnpp1[i]->tax_id;
	  break;
	}
      }
      if (retid != -1) break;
    }
  }
  

  for (i=0;i<inLineage1;i++) MemFree(txtnpp1[i]);
  MemFree(txtnpp1);
  for (i=0;i<inLineage2;i++) MemFree(txtnpp2[i]);
  MemFree(txtnpp2);

  if (retid != -1) return (retid);
  return (1);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* put taxonomy root node in the CD                                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static void CddAddTax(CddPtr pcdd, Boolean bVerbose) 
{
  BioseqPtr               bsp;
  BioseqSetPtr            bssp;
  BioSourcePtr            bsop;
  CharPtr                 orgname;
  DbtagPtr                dbtp;
  Int4Ptr                 Ids;
  ObjectIdPtr             oidp;
  OrgRefPtr               pOrgRef;
  SeqEntryPtr             sep;
  SeqIdPtr                sip;
  Taxon1DataPtr           t1dp;
  ValNodePtr              descr, vnpTail = NULL, vnp;
  Int4                    iTxid1 = -1, iTxid2 = -1, gi;
  
  descr = pcdd->description; /* assumes that Tax-node is not 1st description*/
  while (descr) {            /* and that there's no 2 consecutive Tax-nodes */
    if (descr->choice == CddDescr_tax_source) {
      if (bVerbose) printf("Updating existing taxonomy assignments...\n");
      if (vnpTail) vnpTail->next = descr->next;
    }
    vnpTail = descr;
    descr = descr->next;
  }
  
  if (!TaxArchInit()) CddSevError("Can't initialize taxonomy services\n");
  bssp = (BioseqSetPtr) pcdd->sequences->data.ptrvalue;
  sep = bssp->seq_set;
  while (sep) {
    if (sep->choice != 1) {
      if (bVerbose) printf("Warning: Error in Seq-entry!\n");
    } else {
      bsp = sep->data.ptrvalue;
      descr = bsp->descr;
      while (descr) {
        if (descr->choice == Seq_descr_source) {
          bsop = descr->data.ptrvalue;
          pOrgRef = bsop->org;
	  if (pOrgRef->db) {
            dbtp = pOrgRef->db->data.ptrvalue;
            oidp = dbtp->tag;
            iTxid2 = oidp->id;
	  } else {
	    sip = bsp->id;
	    gi = GetGIForSeqId (sip);
	    iTxid2 = tax1_getTaxId4GI(gi);
	  }
        }
        descr = descr->next;
      }
      if (iTxid2 >= 1) {
        if (iTxid1 == -1) {
          iTxid1 = iTxid2;
        } else {
          iTxid1 = Cdd_tax1_join(iTxid2, iTxid1);
        }
      }
    }
    sep = sep->next;
  }
  if (iTxid1 >= 1) {
    t1dp = (Taxon1DataPtr) tax1_getbyid(iTxid1);
    pOrgRef = t1dp->org;
    if (pOrgRef) CddAssignDescr(pcdd,pOrgRef,CddDescr_tax_source,0);   
  }
  
  if (!TaxArchFini()) CddSevError("Can't disconnect from taxonomy services\n");
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static ValNodePtr SeqDescCopy(ValNodePtr vnp)
{
  ValNodePtr vnpNew;
  
  vnpNew = ValNodeNew(NULL);
  vnpNew->choice = vnp->choice;
  vnpNew->data.ptrvalue = StringSave(vnp->data.ptrvalue);
  return(vnpNew);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static CharPtr CddTitlePrepend(CharPtr cTitle, CharPtr cPrefix)
{
  CharPtr  cNewTitle;
  Int4     cLen;
  
  cLen = Nlm_StrLen(cTitle) + Nlm_StrLen(cPrefix) + 3;
  cNewTitle = MemNew(cLen * sizeof(Char));
  sprintf(cNewTitle,"%s, %s",cPrefix, cTitle);
  MemFree(cTitle);
  return (cNewTitle);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Main Function;    name of the CD to be dumped is a command-line parameter */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int2 Main()
{
  BioseqPtr                bspTrunc, bspCons, bspFake;
  BioseqPtr                bspThis;
  BioseqSetPtr             bssp;
  BiostrucAlignPtr         pbsaStruct              = NULL;
  BiostrucAlignSeqPtr      pbsaSeq;
  BiostrucSeqsPtr          pbsaStrSeq;
  CddDescPtr               pcddesc                 = NULL;
  CddDescPtr               pcddeschead             = NULL;
  CddExpAlignPtr           pCDea, pCDea1, pCDeaR;
  CddPtr                   pcdd;
  CddSumPtr                pcds                    = NULL;
  CddTreePtr               pcddt;
  CharPtr                  cCategory, pcTest;
  CharPtr                  cDescStr, cDescNew;
  DbtagPtr                 dbtp;
  GlobalIdPtr              pGid                    = NULL;
  ObjectIdPtr              oidp;
  PDBSeqIdPtr              pdbsip;
  SeqAlignPtr              salpHead, salpNew, salpCons, salpCopy, salpTail;
  SeqAlignPtr              salpThis;
  SeqAnnotPtr              psaCAlignHead;
  SeqEntryPtr              sepNew;
  SeqEntryPtr              oldhead, newhead, septemp, oldtail, newtail, remaind;
  SeqIdPtr                 sip_master, sipTrunc, sipNew;
  SeqIntPtr                sintp;
  TrianglePtr              pTri                    = NULL;
  ValNodePtr               pvnId, pub, vnp, vnpHead, pubNew;
  ValNodePtr               desc                    = NULL;
  Int4                     pmid, muid, i, nPdb     = 0;
  Int4*                    iGiList;
  Int2                     iSeqStrMode             = CDDSEVSTRUC;
  Boolean                  is_network, bHaveSource = FALSE;
  Char                     CDDref[PATH_MAX];
  Char                     cLink[23]               = "linked to 3D-structure";
  Char                     pcBuf[100], cTmp[PATH_MAX], checkstring[PATH_MAX];
  Char                     cOutFile[PATH_MAX];
  FILE                    *fp;
  Boolean                  bIsFinished             = FALSE;
  CharPtr                  pch;

#ifdef DBNTWIN32
  if (! MmdbSrvInitialize()) CddSevError("MMDB Initialization failed");
  if (! VastSrvInitialize()) CddSevError("VAST Initialization failed");
#endif

/*---------------------------------------------------------------------------*/
/* Yanli's fix for making binary reading work                                */
/*---------------------------------------------------------------------------*/
  objmmdb1AsnLoad();
  objmmdb2AsnLoad();
  objmmdb3AsnLoad();

/*---------------------------------------------------------------------------*/
/* retrieve command-line parameters                                          */
/*---------------------------------------------------------------------------*/
  if (! GetArgs ("cddump", NUMARGS, myargs)) return (1);

/*---------------------------------------------------------------------------*/
/* retrieve names for directories etc.                                       */
/*---------------------------------------------------------------------------*/
  if (!CddGetParams()) CddSevError("Couldn't read from config file...");

/*---------------------------------------------------------------------------*/
/* assign CD-Id                                                              */
/*---------------------------------------------------------------------------*/
  CddRegularizeFileName(myargs[0].strvalue,cCDDid,cCDDfname,myargs[1].strvalue);

/*---------------------------------------------------------------------------*/
/* read in an existing CD if necessary/possible                              */
/*---------------------------------------------------------------------------*/
  if (myargs[12].strvalue) {
    if (myargs[13].intvalue > 0) {
           pcdd = (CddPtr) CddReadFromFile(myargs[12].strvalue, TRUE);
    } else pcdd = (CddPtr) CddReadFromFile(myargs[12].strvalue, FALSE);
    if (!pcdd) CddSevError("Could not read CD from file!");
    iSeqStrMode = CDDUPDATE;
    if (CddHasConsensus(pcdd)) {
      bIsFinished = TRUE;
      if (myargs[24].intvalue) printf("CD is finished, will dump Checkpoint only\n"); 
    } else {
      psaCAlignHead = (SeqAnnotPtr)  pcdd->seqannot;
      salpHead      = (SeqAlignPtr)  pcdd->seqannot->data;
      bssp          = (BioseqSetPtr) pcdd->sequences->data.ptrvalue;
      pbsaSeq       = BiostrucAlignSeqNew();
      pbsaSeq->sequences = bssp->seq_set;
      if (myargs[14].strvalue) {
        Nlm_StrCpy(cCDDid, myargs[14].strvalue);
      }
    }
  }

/*---------------------------------------------------------------------------*/
/* Initialize the data structures needed to collect alignments and sequences */
/*---------------------------------------------------------------------------*/
  if (iSeqStrMode != CDDUPDATE) {
/*---------------------------------------------------------------------------*/
/* initialize PUBSEQ interface - this is needed to retrieve sequences        */
/*---------------------------------------------------------------------------*/
 #ifdef OS_UNIX
    if (!PUBSEQBioseqFetchEnable("cddump",TRUE))
      CddSevError("Unable to initialize PUBSEQ");
 #endif
    if (myargs[25].intvalue) {
      if(!(rdfp = readdb_new_ex2("nr",READDB_DB_IS_PROT,READDB_NEW_DO_TAXDB,NULL,NULL)))
      CddSevError("Readdb init failed");
    }
#ifndef DBNTWIN32
    if (!MMDBInit()) CddSevError("MMDB Initialization failed");
#endif
    pbsaSeq = BiostrucAlignSeqNew();
/*---------------------------------------------------------------------------*/
/* read in the ASN.1-formatted sequence alignment corresponding to this CD   */
/*---------------------------------------------------------------------------*/
    if (!myargs[11].strvalue) {
      salpHead = (SeqAlignPtr) CddReadSeqAln();
    } else {
      salpHead = (SeqAlignPtr) CddReadRealInd(myargs[11].strvalue);
    }
/*---------------------------------------------------------------------------*/
/* process the Seqalign                                                      */
/*---------------------------------------------------------------------------*/
    pcds = (CddSumPtr) CddProcessSeqAln(&salpHead,&nPdb,pbsaSeq,
                                        myargs[19].intvalue,
					(Boolean) myargs[24].intvalue);
    if (nPdb > 1) {
      salpHead = CddAlignSort(salpHead,pcds);
      pcds = CddSumSort(pcds);
    } else if (nPdb == 1) {
      iSeqStrMode = CDDONESTRUC;
    } else {
      iSeqStrMode = CDDSEQUONLY;
    }
/*---------------------------------------------------------------------------*/
/* additional pointers to list of sequences                                  */
/*---------------------------------------------------------------------------*/
    if (nPdb == 1) {
      pbsaStrSeq = BiostrucSeqsNew();
      pbsaStrSeq->sequences = pbsaSeq->sequences;
    } else if (nPdb > 1) {
      pbsaStruct = BiostrucAlignNew();
      pbsaStruct->sequences = pbsaSeq->sequences;
    }
/*---------------------------------------------------------------------------*/
/* if more than one structure present, VAST results have to be retrieved     */
/*---------------------------------------------------------------------------*/
    if (nPdb > 1) {
     OpenMMDBAPI((POWER_VIEW /* ^ FETCH_ENTREZ */), NULL);
/*---------------------------------------------------------------------------*/
/* read the information contained in the VAST info file. This is used to     */
/* figure out which biostruc annot sets have to be retrieved from the VAST   */
/* data base                                                                 */
/*---------------------------------------------------------------------------*/
      CddReadVASTInfo(pcds);
/*---------------------------------------------------------------------------*/
/* Now identify the feature set id's that go with each structurally aligned  */
/* slave - these need not be the same                                        */
/*---------------------------------------------------------------------------*/
      CddIdentifyFsids(pcds);
/*---------------------------------------------------------------------------*/
/* load the biostruc annot sets (and trim them) for each aligned slave struc.*/
/*---------------------------------------------------------------------------*/
      CddLoadBSAnnotSets(pcds, &nPdb, &iSeqStrMode, salpHead, pbsaStruct);    
      CloseMMDBAPI();
#ifndef DBNTWIN32
      VASTFini();
#else

#endif
    }

/*---------------------------------------------------------------------------*/
/* Disable BioseqAccess                                                      */
/*---------------------------------------------------------------------------*/
#ifdef OS_UNIX
     if (iSeqStrMode != CDDUPDATE) PUBSEQFini();
#endif 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* allocate the CDD data structure                                           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
    pcdd = (CddPtr) CddNew();

/*---------------------------------------------------------------------------*/
/* add the alignment data to the new CD                                      */
/*---------------------------------------------------------------------------*/
    psaCAlignHead = SeqAnnotNew();
    psaCAlignHead->type = 2;
    psaCAlignHead->data = salpHead;
    pcdd->seqannot = (SeqAnnotPtr) psaCAlignHead;

/*---------------------------------------------------------------------------*/
/* Assign pointer to bioseqs                                                 */
/*---------------------------------------------------------------------------*/
    bssp = BioseqSetNew();
    bssp->seq_set = pbsaSeq->sequences;
    pcdd->sequences = ValNodeNew(NULL);
    pcdd->sequences->choice = 2;
    pcdd->sequences->data.ptrvalue = bssp;

/*---------------------------------------------------------------------------*/
/* Assign pointer to BiostrucFeatureSet (holding the VAST alignments)        */
/*---------------------------------------------------------------------------*/
    if (nPdb > 1) { 
      pcdd->features = (BiostrucAnnotSetPtr) pbsaStruct->alignments;
    }

/*---------------------------------------------------------------------------*/
/* fill in as much of the descriptive information as available               */
/* use a set of generic calls to CddAssignDescr                              */
/*---------------------------------------------------------------------------*/
    pcddesc = CddReadDescr();
    pcddeschead = pcddesc;
    while (pcddesc) {
      if (strcmp(cCDDid,pcddesc->cCddId)==0) {
        CddAssignDescr(pcdd,(CharPtr) StringSave(pcddesc->cDescr),CddDescr_comment,0);
        if (strncmp(pcddesc->cSourc," ",1)!=0) {
          cCategory = StringSave(pcddesc->cSourc);
          cCategory[strlen(cCategory)-1]='\0';
          CddAssignDescr(pcdd, (CharPtr) StringSave(cCategory),CddDescr_category,0);
        }
        break;
      }  
      pcddesc = pcddesc->next;
    }

/*---------------------------------------------------------------------------*/
/* note whether CD has link to 3D-Structure                                  */
/*---------------------------------------------------------------------------*/
    if (nPdb) {
      CddAssignDescr(pcdd, StringSave(cLink), CddDescr_comment,0);
    }

/*---------------------------------------------------------------------------*/
/* assign the source identifier if present                                   */
/*---------------------------------------------------------------------------*/
    if (NULL == myargs[4].strvalue) {
      if (strncmp(myargs[0].strvalue,"pfam",4) == 0) {
        myargs[4].strvalue = StringSave("Pfam");
      } else if (strncmp(myargs[0].strvalue,"LOAD",4) == 0) {
        myargs[4].strvalue = StringSave("Load");
      } else {
        myargs[4].strvalue = StringSave("Smart");
      }
    
    }
    CddAssignDescr(pcdd,StringSave(myargs[4].strvalue),CddDescr_source,0);

/*---------------------------------------------------------------------------*/
/* assign create date as the current date                                    */
/*---------------------------------------------------------------------------*/
    CddAssignDescr(pcdd,(DatePtr) DateCurr(),CddDescr_create_date,0);

/*---------------------------------------------------------------------------*/
/* Assign references if they can be found                                    */
/*---------------------------------------------------------------------------*/
    Nlm_StrCpy(CDDref,REFpath);
    Nlm_StrCat(CDDref,cCDDid);
    Nlm_StrCat(CDDref,".");
    Nlm_StrCat(CDDref,myargs[7].strvalue);
    fp = FileOpen(CDDref,"r");
    if (fp) {
      MedArchInit();
      do {
        pcBuf[0]='\0';
        pcTest = fgets(pcBuf, (size_t)100, fp);
        if (pcTest) {
          pcTest[strlen(pcTest)-1]='\0';
          muid = (Int4) atoi(pcTest);
          pub  = (ValNodePtr) FetchPub(muid);
	  if (!pub)  pub = (ValNodePtr) FetchPubPmId(muid);
          if (pub) CddAssignDescr(pcdd, pub, CddDescr_reference, 0);
        }
      } while (pcTest);
      FileClose(fp);
      MedArchFini();
    }
  }                                       /* end if iSeqStrMode != CDDUPDATE */

/*---------------------------------------------------------------------------*/
/* remove status if present and set to current flag - but only if unfinished */
/*---------------------------------------------------------------------------*/
  if (!bIsFinished) {
    if (CddGetStatus(pcdd) != 8) {
      while (CddKillDescr(pcdd,NULL, CddDescr_status, 0)) {};
      CddAssignDescr(pcdd, NULL, CddDescr_status , myargs[8].intvalue);
    }
/*---------------------------------------------------------------------------*/
/* assign a CD-name automatically if not yet present in the CD               */
/*---------------------------------------------------------------------------*/
      if (!pcdd->name) {
        pcdd->name = StringSave(cCDDid);
      }
      if (myargs[22].intvalue) {
        if (Nlm_StrNCmp(pcdd->name,"pfam0",5) == 0 ||
            Nlm_StrNCmp(pcdd->name,"smart0",6) == 0 ||
	    Nlm_StrNCmp(pcdd->name,"COG",3) == 0 || 
	    Nlm_StrNCmp(pcdd->name,"LOAD_",5) == 0) {
          vnp = pcdd->description;
          while (vnp) {
            if (vnp->choice == CddDescr_comment) {
	      if (Nlm_StrCmp(vnp->data.ptrvalue,"linked to 3D-structure") != 0) {
	        cDescStr = StringSave(vnp->data.ptrvalue);
                break;
	      }
            }
            vnp = vnp->next;
          }
          cDescNew = Nlm_StrStr(cDescStr,",");
          if (cDescNew) {
            if (Nlm_StrLen(cDescNew) > 2) {
	      vnp->data.ptrvalue = cDescNew + 2;
              Nlm_StrTok(cDescStr,",");
              pcdd->name = cDescStr;
            }
          }
        }
      }

/*---------------------------------------------------------------------------*/
/* if parsed from a real.ind-file, extract the CDD id from the input file    */
/* name. This is a kludge to deal with the setup of "split sets" for testing */
/* with RPS-Blast. The CDD-id is only chaged at this point so that the refs  */
/* and descriptions are parsed according to the id of the 'mother' CD        */
/*---------------------------------------------------------------------------*/
    if (myargs[15].intvalue > 0 && myargs[11].strvalue) {
      strncpy(cCDDid,myargs[11].strvalue,strlen(myargs[11].strvalue)-9);
    }

/*---------------------------------------------------------------------------*/
/* this part includes modifications which are part of a CD update as well    */
/*---------------------------------------------------------------------------*/
    if (!pcdd->id || myargs[14].strvalue) {
      pvnId = ValNodeNew(NULL);
      pGid = (GlobalIdPtr) GlobalIdNew();
      pGid->accession = StringSave(cCDDid);
      pvnId->choice = CddId_gid;
      pvnId->data.ptrvalue = (GlobalIdPtr) pGid;
      pcdd->id = pvnId;
    }
    
/*---------------------------------------------------------------------------*/
/* if requested, reindex the CD-Alignment to a new master. The argument must */
/* be given as a PDB-derived Sequence ID, in the form of "1ABCX" or "1ABC"   */
/*---------------------------------------------------------------------------*/
    if (myargs[17].strvalue) {
      if (strlen(myargs[17].strvalue) < 4) CddSevError("option -R needs a valid PDB-Id");
      if (strlen(myargs[17].strvalue) > 5) CddSevError("option -R needs a valid PDB-Id");
      pdbsip = PDBSeqIdNew();
      if (strlen(myargs[17].strvalue) == 5) {
        pdbsip->chain = (Uint1) *(myargs[17].strvalue+4);
        myargs[17].strvalue[4]='\0';
      }
      pdbsip->mol = StringSave(myargs[17].strvalue);
      sip_master = ValNodeNew(NULL);
      sip_master->choice = SEQID_PDB;
      sip_master->data.ptrvalue = pdbsip;
      if (!CddFindSip(sip_master,bssp->seq_set)) CddSevError("Could not find new master in CD");

      psaCAlignHead = SeqAnnotNew();
      psaCAlignHead->type = 2;
      psaCAlignHead->data = CddReindexSeqAlign(salpHead,sip_master,bssp);
      salpHead = psaCAlignHead->data;
      pcdd->seqannot = (SeqAnnotPtr) psaCAlignHead;
      if (NULL != pcdd->features) pcdd->features = NULL;
/*---------------------------------------------------------------------------*/
/* for compliance with Cn3D++, the bioseqs need to be reordered so that the  */
/* new master appears on top                                                 */
/*---------------------------------------------------------------------------*/
      septemp = bssp->seq_set;
      oldhead = septemp;
      oldtail = septemp;
      while (septemp) {
        if (septemp->choice == 1) {
          bspThis = (BioseqPtr) septemp->data.ptrvalue;
	  if (CddSameSip(bspThis->id, sip_master)) {
	    newhead = septemp;
	    newtail = septemp;
	    remaind = septemp->next;
            break;	
	  }
	  else oldtail = septemp;
        } else (CddSevError("Invalid SeqEntry encountered while reindexing to new master"));
        septemp = septemp->next;
      }
      bssp->seq_set = newhead;
      newtail->next = oldhead;
      oldtail->next = remaind;    
    }

/*---------------------------------------------------------------------------*/
/* assuming this is a set of pairwise master-slave dendiag alignments,       */
/* calculate the interval on the master that is aligned                      */
/*---------------------------------------------------------------------------*/
    sip_master = CddDetermineMasterSip(pcdd);
    CddAssignProfileRange(pcdd, sip_master);
    if (sip_master->choice == SEQID_PDB) {
      pcdd->master3d = (SeqIdPtr) SeqIdDup(sip_master);
    }

/*---------------------------------------------------------------------------*/
/* create identifier and description for the truncated master                */
/*---------------------------------------------------------------------------*/
    oidp = (ObjectIdPtr) ObjectIdNew();
    oidp->str = StringSave(cCDDid);
    dbtp = DbtagNew();
    dbtp->tag = oidp;
    if (myargs[4].strvalue) {
      dbtp->db = myargs[4].strvalue;
    } else {
      dbtp->db = StringSave("Cdd");
    }
    vnp = pcdd->description;
    while (vnp) {
      if (vnp->choice == CddDescr_comment) {
        desc = ValNodeNew(NULL);
        desc->choice = Seq_descr_title;
        desc->data.ptrvalue = StringSave((CharPtr)vnp->data.ptrvalue);
        break;
      }
      vnp = vnp->next;
    }

/*---------------------------------------------------------------------------*/
/* and create the truncated master                                           */
/*---------------------------------------------------------------------------*/
    sintp = (SeqIntPtr) pcdd->profile_range;
    sipTrunc = (SeqIdPtr) ValNodeNew(NULL);
    sipTrunc->choice = 11;
    sipTrunc->data.ptrvalue = dbtp;
    bspTrunc = (BioseqPtr) BioseqCopy(sipTrunc,sip_master,sintp->from,sintp->to,0,FALSE);
    if (desc) bspTrunc->descr = SeqDescCopy(desc);
    if (myargs[9].intvalue == 0) {
/*---------------------------------------------------------------------------*/
/* add to cdd if no consensus is calculated                                  */
/*---------------------------------------------------------------------------*/
      pcdd->trunc_master = (struct bioseq PNTR) bspTrunc;
/*---------------------------------------------------------------------------*/
/* now calculate the PSSM and write the checkpoint file and corresponding    */
/* sequence out to disk                                                      */
/*---------------------------------------------------------------------------*/
      salpCopy = (SeqAlignPtr) CddCopyMSLDenDiag(pcdd->seqannot->data);
      CddReindexMSLDenDiagMaster(salpCopy, sintp->from);
      bspFake  = (BioseqPtr) BioseqCopy(NULL,sip_master,sintp->from,sintp->to,0,FALSE);
/*---------------------------------------------------------------------------*/
/* Kludge to tell CddDenDiagCposComp2 whether to write out the matrix or not */
/*---------------------------------------------------------------------------*/
      if (myargs[23].intvalue) {
        CddAssignDescr(pcdd,"Write out matrix",CddDescr_comment,0);
      }
      CddDenDiagCposComp2(bspFake,myargs[10].intvalue,salpCopy,pcdd,bspTrunc,0.5,1.0,myargs[16].strvalue);
      if (myargs[23].intvalue) {
        CddKillDescr(pcdd,"Write out matrix",CddDescr_comment,0);
      }
    } else {
/*---------------------------------------------------------------------------*/
/* calculate consensus sequence using the 50/50 algorithm                    */
/*---------------------------------------------------------------------------*/
      salpNew = (SeqAlignPtr) CddConsensus(salpHead,bssp->seq_set,
                                           bspTrunc,pcdd->profile_range,
                                           &bspCons, &salpCons);
      if (desc) bspCons->descr = SeqDescCopy(desc);
      sepNew = SeqEntryNew();
      sepNew->data.ptrvalue = bspCons;
      sepNew->choice = 1;
      ValNodeLink(&(pbsaSeq->sequences), sepNew);
      sintp->from = 0;
      sintp->to = bspCons->length - 1;
      sintp->id = SeqIdDup(bspCons->id);
      sip_master = sintp->id;
      BioseqFree(bspTrunc);
      bspTrunc = (BioseqPtr) BioseqCopy(sipTrunc,sip_master,sintp->from,sintp->to,0,FALSE);
      if (desc) bspTrunc->descr = SeqDescCopy(desc);
/*---------------------------------------------------------------------------*/
/* also add short name to trunc_master defline - if there's a consensus seq. */
/*---------------------------------------------------------------------------*/
      vnp = bspTrunc->descr;
      while (vnp) {
        if (vnp->choice == Seq_descr_title) {
	  sprintf(cTmp,"%s,",pcdd->name);
          if (Nlm_StrNCmp(vnp->data.ptrvalue,cTmp,Nlm_StrLen(cTmp))) {
	    vnp->data.ptrvalue = CddTitlePrepend(vnp->data.ptrvalue, pcdd->name);
	  }
          break;      
        }
        vnp = vnp->next;
      }
      bspFake  = (BioseqPtr) BioseqCopy(NULL,sip_master,sintp->from,sintp->to,0,FALSE);
/*---------------------------------------------------------------------------*/
/* Kludge to tell CddDenDiagCposComp2 whether to write out the matrix or not */
/*---------------------------------------------------------------------------*/
      if (myargs[23].intvalue) {
        CddAssignDescr(pcdd,"Write out matrix",CddDescr_comment,0);
      }
      CddDenDiagCposComp2(bspFake,myargs[10].intvalue,salpNew,pcdd,bspTrunc,0.5,1.0,myargs[16].strvalue);
      if (myargs[23].intvalue) {
        CddKillDescr(pcdd,"Write out matrix",CddDescr_comment,0);
      }
    
      pcdd->trunc_master = (struct bioseq PNTR) bspTrunc;
      salpNew = SeqAlignSetFree(salpNew);
/*---------------------------------------------------------------------------*/
/* Now reindex the seqalign to make the consensus the new master             */
/*---------------------------------------------------------------------------*/
      pCDea1 = (CddExpAlignPtr) SeqAlignToCddExpAlign(salpCons,bssp->seq_set);
      pCDeaR = (CddExpAlignPtr) InvertCddExpAlign(pCDea1, bssp->seq_set);
      salpNew = (SeqAlignPtr) CddExpAlignToSeqAlign(pCDeaR,NULL); 
      pCDeaR = CddExpAlignFree(pCDeaR);   
      salpTail = salpNew;
      salpCopy = salpHead;
      salpHead = salpCopy;
      i = 0;
      while (salpHead) {
        i++;
        pCDea = (CddExpAlignPtr) SeqAlignToCddExpAlign(salpHead, bssp->seq_set);
        pCDeaR = (CddExpAlignPtr) CddReindexExpAlign(pCDea1,bspCons->length,pCDea,0,i);
        salpThis = (SeqAlignPtr) CddExpAlignToSeqAlign(pCDeaR,NULL);
        pCDeaR = CddExpAlignFree(pCDeaR);
        pCDea = CddExpAlignFree(pCDea);
        if (salpThis) {
          salpTail->next = salpThis;
          salpTail = salpThis;
        }
        salpHead = salpHead->next;
      }
      psaCAlignHead->data = salpNew;
      pCDea1 = CddExpAlignFree(pCDea1);
      salpCopy = SeqAlignSetFree(salpCopy);
/*---------------------------------------------------------------------------*/
/* transfer CD annotation from the old master to the consensus sequence      */
/*---------------------------------------------------------------------------*/
      if (pcdd->alignannot) 
        CddTransferAlignAnnot(pcdd->alignannot,sip_master,salpNew,bssp);
    }
  } else {                                            /* END if !bIsFinished */
    vnp = pcdd->id; while (vnp) {
      if (vnp->choice == CddId_uid) {
        sipNew = (SeqIdPtr) ValNodeNew(NULL);
	sipNew->choice = SEQID_GENERAL;
	dbtp = (DbtagPtr) DbtagNew();
	dbtp->db = StringSave("CDD");
	oidp = ObjectIdNew();
	oidp->id = vnp->data.intvalue;
	dbtp->tag = oidp;
	sipNew->data.ptrvalue = dbtp;
	pcdd->trunc_master->id = sipNew;
        break;
      }
      vnp = vnp->next;
    }
    vnp = pcdd->trunc_master->descr;
    while (vnp) {
      if (vnp->choice == Seq_descr_title) {
        sprintf(checkstring,"%s, \0",pcdd->name);
        if (Nlm_StrNCmp(vnp->data.ptrvalue,checkstring,Nlm_StrLen(checkstring))) {
	  vnp->data.ptrvalue = CddTitlePrepend(vnp->data.ptrvalue, pcdd->name);
	}
	pvnId = pcdd->id; while (pvnId) {
	  if (pvnId->choice == CddId_gid) {
	    pGid = pvnId->data.ptrvalue;
	    vnp->data.ptrvalue = CddTitlePrepend(vnp->data.ptrvalue,pGid->accession);
	  }
	  pvnId = pvnId->next;
	}
        break;      
      }
      vnp = vnp->next;
    }
    salpCopy = SeqAlignSetDup(pcdd->seqannot->data);
    CddDegapSeqAlign(salpCopy);
    if (myargs[23].intvalue) {
      CddAssignDescr(pcdd,"Write out matrix",CddDescr_comment,0);
    }
    CddDenDiagCposComp2(pcdd->trunc_master,myargs[10].intvalue,salpCopy,pcdd,pcdd->trunc_master,0.5,1.0,myargs[16].strvalue);
    if (myargs[23].intvalue) {
      CddKillDescr(pcdd,"Write out matrix",CddDescr_comment,0);
    }
    SeqAlignSetFree(salpCopy);
  }
/*---------------------------------------------------------------------------*/
/* if the CD needs to be written to disk, fill in the missing data items     */
/*---------------------------------------------------------------------------*/
  if (myargs[18].intvalue == 0) {
    if (!bIsFinished) {
/*---------------------------------------------------------------------------*/
/* do the following steps only if the CD-status is not 2, pending release    */
/*---------------------------------------------------------------------------*/
      if (myargs[8].intvalue != 2) {

/*---------------------------------------------------------------------------*/
/* Calculate pairwise percent identities between the sequences, for alignment*/
/* formatting purposes                                                       */
/*---------------------------------------------------------------------------*/
        pTri = (TrianglePtr) CddCalculateTriangle(pcdd);
        if (myargs[24].intvalue) printf("\n");
        if (pTri) pcdd->distance = pTri;
      } else {
        pcdd->posfreq = NULL;
        pcdd->scoremat = NULL;
      }

/*---------------------------------------------------------------------------*/
/* create/update the CD taxonomy data item                                   */
/*---------------------------------------------------------------------------*/
      CddAddTax(pcdd, (Boolean) myargs[24].intvalue);

/*---------------------------------------------------------------------------*/
/* check if CD is a proper CD, and trim the seqaligns if necessary           */
/*---------------------------------------------------------------------------*/
      if (myargs[8].intvalue !=3 && myargs[20].intvalue > 0) {
        if (CddTrimSeqAligns(pcdd)) CddSevError("Crashed while trimming seqaligns!");
      }

/*---------------------------------------------------------------------------*/
/* for output, convert the pairwise master-slave densediag to a multiple     */
/* dense-diag alignment if the block-structure is consistent - and if        */
/* specified in the command line                                             */
/*---------------------------------------------------------------------------*/
      if (myargs[5].intvalue > 0) {
        pcdd->seqannot->data = (SeqAlignPtr)CddMSLDenDiagToMULDenDiag(pcdd->seqannot->data);
      }
    }
/*---------------------------------------------------------------------------*/
/* unhook the PSSM if not required but still present in the CD               */
/*---------------------------------------------------------------------------*/
    if (myargs[3].intvalue == 0) {
      if (pcdd->scoremat) {    
        pcdd->scoremat = NULL;           /* using MatrixFree corrupts memory */
      }
    }

/*---------------------------------------------------------------------------*/
/* convert list of pairwise distances into a sorted list of diversity        */
/*---------------------------------------------------------------------------*/
    if (!bIsFinished) {
      if (pcdd->distance) {
        pcdd->distance->div_ranks = NULL;
        iGiList = CddMostDiverse(pcdd->distance, pcdd->distance->nelements-1,MAXDIV);
        vnp = NULL; vnpHead = NULL;
        for (i=0;i<MIN(MAXDIV,(pcdd->distance->nelements-1));i++) {
          vnp = ValNodeAddInt(&vnpHead,0,iGiList[i]);
        }
        MemFree(iGiList);
        pcdd->distance->div_ranks = vnpHead;
        pcdd->distance->scores = ScoreSetFree(pcdd->distance->scores);
      }
/*---------------------------------------------------------------------------*/
/* check for repeats in the CD using default values for detection            */
/*---------------------------------------------------------------------------*/
      if (CddCheckForRepeats(pcdd,10,15,2,.875,FALSE)) {
        if (myargs[24].intvalue) {
          printf("..found repeats in %s\n",pcdd->name);
        }
      }

/*---------------------------------------------------------------------------*/
/* do a final check of citations listed in description, avoid duplicates     */
/*---------------------------------------------------------------------------*/
      MedArchInit();
      vnp = pcdd->description;
      while (vnp) {
        if (vnp->choice == CddDescr_reference) {
          pub = vnp->data.ptrvalue;
	  if (pub->choice != PUB_PMid) {
	    pmid = 0;
	    pmid = MedArchCitMatchPmId(pub);
            if (pmid > 0) {
              pubNew = ValNodeNew(NULL);
              pubNew->choice = PUB_PMid;
              pubNew->data.intvalue = pmid;
              vnp->data.ptrvalue = pubNew;
	      PubFree(pub);
	      pub = pubNew;
            }
	  }
          if (pub->choice == PUB_PMid) {
	    vnpHead = pcdd->description;
	    while (vnpHead) {
              if (vnpHead == vnp) break; 
	      else {
	        if (vnpHead->choice == CddDescr_reference) {
                  pubNew = vnpHead->data.ptrvalue;
		  if (pubNew->choice == PUB_PMid) {
                    if (pubNew->data.intvalue == pub->data.intvalue) {
                      CddKillDescr(pcdd,pubNew,CddDescr_reference,pubNew->data.intvalue);
		    }		
		  }	      
	        }
	      }
	      vnpHead = vnpHead->next;
	    }
	  }
        }
        vnp = vnp->next;
      }
      MedArchFini();
    
/*---------------------------------------------------------------------------*/
/* record the original "source-id" in the CD - for compat. with curated set  */
/*---------------------------------------------------------------------------*/
      vnp = pcdd->description;
      while (vnp) {
        if (vnp->choice == CddDescr_source_id) {
          bHaveSource = TRUE; break;      
        }
        vnp = vnp->next;
      }
      if (!bHaveSource && pGid) {
        vnp = ValNodeNew(NULL);
        vnp->choice = CddId_gid;
        vnp->data.ptrvalue = pGid;
        CddAssignDescr(pcdd,vnp,CddDescr_source_id,0);
      }
    }
/*---------------------------------------------------------------------------*/
/* Write the finished CD out to disk                                         */
/*---------------------------------------------------------------------------*/
    if (myargs[12].strvalue) {
      Nlm_StrCpy(cOutFile,myargs[12].strvalue);
      Nlm_StrCpy(cCDDid,myargs[12].strvalue);
      for (i=0;i<Nlm_StrLen(cCDDid);i++) {
        if (cCDDid[i] == '.') cCDDid[i] = '\0';
      }
    } else {
      Nlm_StrCpy(cOutFile,cCDDid);
      Nlm_StrCat(cOutFile,".");
      Nlm_StrCat(cOutFile,myargs[1].strvalue);
    }
    if (!CddWriteToFile(pcdd,cOutFile,(Boolean) myargs[2].intvalue))
      CddSevError("Could not write ASN.1 output / CDD to file!");

/*---------------------------------------------------------------------------*/
/* output the corresponding Cdd tree-file                                    */
/*---------------------------------------------------------------------------*/
    if (myargs[23].intvalue) {
      Nlm_StrCpy(cOutFile,cCDDid);
      Nlm_StrCat(cOutFile,".");
      Nlm_StrCat(cOutFile,myargs[6].strvalue);
  
      pcddt              = CddTreeNew();
      pcddt->name        = pcdd->name;
      pcddt->id          = pcdd->id;
      pcddt->description = pcdd->description;

      if (!CddTreeWriteToFile(pcddt,cOutFile,(Boolean) myargs[2].intvalue))
        CddSevError("Could not write CDD-tree");
    }
  }

/*---------------------------------------------------------------------------*/
/* Cleanup                                                                   */
/*---------------------------------------------------------------------------*/
/*  pcdd        = (CddPtr) CddFreeCarefully(pcdd); */
  pcddeschead = CddDescFree(pcddeschead);
  pcds        = CddSumFree(pcds);
  if (iSeqStrMode != CDDUPDATE)
#ifndef DBNTWIN32
    MMDBFini();
#endif
  if (myargs[25].intvalue) rdfp = readdb_destruct(rdfp);
#ifdef DBNTWIN32
  MmdbSrvFinish();
  VastSrvFinish();
#endif
  return 0;
}
