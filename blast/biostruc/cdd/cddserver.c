/* $Id: cddserver.c,v 1.26 2001/11/13 19:46:53 bauer Exp $
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
* File Name:  cddserver.c
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 2/10/2000
*
* $Revision: 1.26 $
*
* File Description:
*         CD WWW-Server, Cd summary pages and alignments directly from the
*         CD ASN.1 data
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cddserver.c,v $
* Revision 1.26  2001/11/13 19:46:53  bauer
* Biostrucs now read from file, support for new mmdbsrv
*
* Revision 1.25  2001/10/01 18:14:24  bauer
* minor changes in logic
*
* Revision 1.24  2001/06/20 20:50:50  bauer
* fixed a problem with gi's for PDB-derived sequences
*
* Revision 1.23  2001/06/19 16:02:37  bauer
* fixed URL for linking to SMART by accession
*
* Revision 1.22  2001/05/31 22:04:45  bauer
* changes to accomodate new type of Smart accessions
*
* Revision 1.21  2001/05/23 21:19:02  bauer
* fix a problem with displaying CDs without consensus
*
* Revision 1.20  2001/03/07 20:29:19  bauer
* cddserver.c
*
* Revision 1.19  2001/03/07 16:31:49  bauer
* bullet-proof against service failures?
*
* Revision 1.18  2001/03/02 23:35:46  bauer
* FASTA format now done by CddAlignView
*
* Revision 1.17  2001/02/28 19:45:11  bauer
* changed options menu
*
* Revision 1.16  2001/02/16 03:11:38  bauer
* more support for CddAlignView
*
* Revision 1.15  2001/02/14 17:10:41  bauer
* added bit-threshold to CddAlignView call
*
* Revision 1.14  2001/02/13 23:11:57  bauer
* integrated CddAlignView
*
* Revision 1.13  2001/02/10 01:57:26  bauer
* use of CddAlignView library conditional
*
* Revision 1.12  2001/02/06 22:54:52  bauer
* make Paul's CddAlignView the default
*
* Revision 1.11  2001/02/05 23:02:11  bauer
* changes for consensus CDs
*
* Revision 1.10  2001/01/24 03:08:47  bauer
* changes for consensus CDs
*
* Revision 1.9  2001/01/11 21:50:45  bauer
* fixed typo
*
* Revision 1.8  2000/12/08 20:17:32  bauer
* changed status text for oAsIs CDs
*
* Revision 1.7  2000/08/03 22:02:23  bauer
* added support for 3D-structure link highlighting
*
* Revision 1.6  2000/08/01 21:24:25  bauer
* changed [DC] label to [CD]
*
* Revision 1.5  2000/07/28 17:58:50  bauer
* added DomainComposition hotlinks in CD summary pages
*
* Revision 1.4  2000/07/24 15:44:50  bauer
* Changed Get Cn3D message for query-added alignment displays
*
* Revision 1.3  2000/07/20 17:50:44  bauer
* fixed bug in manual sequence selection
*
* Revision 1.2  2000/07/19 19:23:12  bauer
* added modification logging
*
*
* ==========================================================================
*/

#include <ncbi.h>
#include <stdio.h>
#include <lsqfetch.h>
#include <netentr.h>
#include <www.h>
#include <sys/resource.h>
#include <asn.h>
#include <accpubseq.h>
#include <accutils.h>
#include <mmdbapi.h>
#include <mmdbapi1.h>
#include <objmime.h>
#include <strimprt.h>
#include <objcdd.h>
#include "cddsrv.h"
#include <pgppop.h> 
#include <alignmgr.h>
#include <taxutil.h>
#include <txcommon.h>
#include <cdd.h>
#ifdef  USE_CAV
#include "cddalignview.h"
#endif
#include "cddutil.h"

#undef DEBUG
#ifdef  USE_CAV
#define CAV_DEFAULT
#endif

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* report Errors in processing and exit immediately                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddHtmlError(CharPtr cErrTxt) 
{
  printf("Content-type: text/html\n\n");
  printf("<h2>CDDsrv Error:</h2>\n");
  printf("<h3>%s</h3>\n",cErrTxt);
  exit(1);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* read parameters from configuration file                                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean CddGetParams()
{
  URLBase[0] = URLcgi[0] = ENTREZurl[0] = DOCSUMurl[0] = MAILto[0] = '\0';
  MMDBpath[0] = gunzip[0] = '\0';

  GetAppParam("cdd", "CDDSRV", "URLBase", "", URLBase, PATH_MAX);
  if (URLBase[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no URLBase...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "URLcgi", "", URLcgi, PATH_MAX);
  if (URLcgi[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no URLcgi...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "PFAMcgiUS", "", PFAMcgiUS, PATH_MAX);
  if (PFAMcgiUS[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no PFAMcgiUS...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "PFAMcgiUK", "", PFAMcgiUK, PATH_MAX);
  if (PFAMcgiUK[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no PFAMcgiUK...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "SMARTcgi", "", SMARTcgi, PATH_MAX);
  if (SMARTcgi[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no SMARTcgi...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "SMACCcgi", "", SMACCcgi, PATH_MAX);
  if (SMACCcgi[0] == '\0') {
                ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no SMACCcgi...\n");
                return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "ENTREZurl", "", ENTREZurl, PATH_MAX);
  if (ENTREZurl[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no ENTREZurl...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "DOCSUMurl", "", DOCSUMurl, PATH_MAX);
  if (DOCSUMurl[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no DOCSUMurl...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "Gunzip", "", gunzip, (size_t) 256*(sizeof(Char)));
  if (gunzip[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no Gunzip...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "Database", "", MMDBpath, PATH_MAX);
  if (MMDBpath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"MMDB config file\nMMDBSRV section has no Database...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "MAILto", "", MAILto, PATH_MAX);
  if (MAILto[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no MAILto...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "DATApath", "", DATApath, PATH_MAX);
  if (DATApath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no VAST Data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDatabase", "", CDDdpath, PATH_MAX);
  if (CDDdpath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDD data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CVDatabase", "", CDDvpath, PATH_MAX);
  if (CDDvpath[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDD/VAST data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDextens", "", CDDextens, PATH_MAX);
  if (CDDextens[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no CDD file name extension...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "RAWextens", "", RAWextens, PATH_MAX);
  if (RAWextens[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no RAW file name extension...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDdescr", "", CDDdescr, PATH_MAX);
  if (CDDdescr[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no description file name extension...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "Database", "", database, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no VAST data path...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "PUBcgi", "", PUBcgi, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no PUBcgi ...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "TAXcgi", "", TAXcgi, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nCDDSRV section has no TAXcgi ...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDPrefix", "", CDDPrefix, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDPrefix...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDPost_O", "", CDDPost_O, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDPost_O...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDPost_C", "", CDDPost_C, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDPost_C...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDefault", "", CDDefault, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDefault...\n");
    return FALSE;
  }
  GetAppParam("cdd", "CDDSRV", "CDDdbtype", "", CDDdbtype, PATH_MAX);
  if (database[0] == '\0') {
    ErrPostEx(SEV_FATAL,0,0,"CDD config file\nWRPSB section has no CDDdbtype...\n");
    return FALSE;
  }

  return TRUE;
}                                                       /* end CddGetParams */

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
  pcds->bIs3dRep    = FALSE;
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
/* Check whether a particular gi has occured previously in the CddSum lnklst */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean CDSUniqueUid(Int4 uid, CddSumPtr pcds) {
  
  CddSumPtr    pcdsThis;

  if (!pcds) return TRUE;
  pcdsThis = pcds;
  while (pcdsThis) {
    if (pcdsThis->uid == uid) return FALSE;
    pcdsThis = pcdsThis->next;
  }
  return TRUE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Check whether a particular gi has occured previously in the CddSum lnklst */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean UniqueSip(SeqIdPtr sip, CddSumPtr pcds) {
  
  CddSumPtr    pcdsThis;

  if (!pcds) return TRUE;
  pcdsThis = pcds;
  while (pcdsThis) {
    if (CddSameSip(pcdsThis->sip, sip)) return FALSE;
    pcdsThis = pcdsThis->next;
  }
  return TRUE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Check whether a particular gi has been supplied as cgi-argument           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean UseThisGi(Int4 Gi, ValNodePtr pvnGis) 
{
  ValNodePtr    pvnGi;

  pvnGi = pvnGis;
  while (pvnGi) {
    if (Gi == pvnGi->data.intvalue) return TRUE;
    pvnGi = pvnGi->next;
  }
  return FALSE;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Check whether a sequence falls into a taxonomy group                      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean CddSameTaxLineage(SeqIdPtr sip, Int4 taxid, SeqEntryPtr sep)
{
  Int4            inLineage;
  TXC_TreeNodePtr *txtnpp;
  Boolean         bMatch = FALSE;
  CddSumPtr       pcdsThis;
  Int4            i,j;
  Int4            thistaxid;
  BioseqPtr       bsp;
  ValNodePtr      descr;
  BioSourcePtr    bsop;
  ObjectIdPtr     oidp;
  DbtagPtr        dbtp;
  OrgRefPtr       pOrgRef;

  bsp = (BioseqPtr) CddFindSip(sip,sep);
  if (bsp) {
    descr = bsp->descr;
    while (descr) {
      if (descr->choice == Seq_descr_source) {
        bsop = descr->data.ptrvalue;
        pOrgRef = bsop->org;
        if (pOrgRef->db) {
          dbtp = pOrgRef->db->data.ptrvalue;
          oidp = dbtp->tag;
          thistaxid = oidp->id;
        } else {
          thistaxid = tax_getIdByName(pOrgRef->taxname,NULL,0);
        }
        break;
      }
      descr = descr->next;
    }
    txtnpp = (TXC_TreeNodePtr *)txc_getLineage(thistaxid,&inLineage);
    if (txtnpp && inLineage > 0) {
      for (i=0; i<inLineage;i++) if (txtnpp[i]->tax_id == taxid) {
        bMatch = TRUE; break;
      }
    }
  } else return(FALSE);
  return(bMatch);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Count how many sequences in the Cdd fall into a certain taxonomy group    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddCountTaxLineage(TXC_TreeNodePtr *tnpp, Int4 iLen, CddSumPtr pcds)
{
  Int4            inLineage;
  TXC_TreeNodePtr *txtnpp;
  Int4            iCount = 0;
  CddSumPtr       pcdsThis;
  Int4            i,j;
  Int4            thistaxid;
  BioseqPtr       bsp;
  ValNodePtr      descr;
  BioSourcePtr    bsop;
  ObjectIdPtr     oidp;
  DbtagPtr        dbtp;
  OrgRefPtr       pOrgRef;
 

  for (i=0;i<iLen;i++) tnpp[i]->flags = 0;
  pcdsThis = pcds; 
  while (pcdsThis) {
    bsp = BioseqLockById(pcdsThis->sip);
    if (bsp) {
      descr = bsp->descr; thistaxid = 0;
      while (descr) {
        if (descr->choice == Seq_descr_source) {
          bsop = descr->data.ptrvalue;
          pOrgRef = bsop->org;
	  if (NULL != pOrgRef) {
	    if (NULL != pOrgRef->taxname) {
	      pcdsThis->cTaxName = strdup(pOrgRef->taxname);
	    }
	  }
          if (pOrgRef->db) {
            dbtp = pOrgRef->db->data.ptrvalue;
            oidp = dbtp->tag;
            thistaxid = oidp->id;
          } else {
            thistaxid = tax_getIdByName(pOrgRef->taxname,NULL,0);
          }
          break;
        }
        descr = descr->next;
      }
      pcdsThis->iTaxId = thistaxid;
      txtnpp = (TXC_TreeNodePtr*)txc_getLineage(thistaxid,&inLineage);
      if (txtnpp && inLineage > 0) {
        for (i=0; i<inLineage;i++) {
          for (j=0;j<iLen;j++) {
            if (txtnpp[i]->tax_id == tnpp[j]->tax_id) tnpp[j]->flags++;
          }
        }
      }
      BioseqUnlock(bsp);
    }
    pcdsThis = pcdsThis->next;
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* dumps the HTML to show the list of CD sequences (tracks according to the  */
/* juke-box analogy                                                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddServerShowTracks(CddSumPtr pcds, CddPtr pcdd, Int4 thisTax,
                                Boolean bHasPdb, CharPtr dbversion, Boolean bHasConsensus,
				Boolean bShowTax)
{
  CddDescrPtr       pCddesc;
  CddSumPtr         pcdsThis;
  CharPtr           cParentTaxName;
  CharPtr           cCurrDesc;
  DatePtr           dtp;
  DbtagPtr          dbtp;
  ObjectIdPtr       oidp;
  OrgRefPtr         orp;
  SeqIntPtr         sintp;
  SeqPortPtr        spp;
  Taxon1DataPtr     t1dp;
  TaxNamePtr        *tnpp;
  TXC_TreeNodePtr   *txtnpp; 
  ValNodePtr        pub;
  Boolean           bRefOpen = FALSE;
  Boolean           bProfileOnly = FALSE;
  Boolean           bIsOasis = FALSE;
  Char              buf[61];
  Char              cTax[PATH_MAX];
  Char              descript[CDD_MAX_DESCR];
  Char              tableName[PATH_MAX]; 
  Char              source[PATH_MAX];
  Int4              i;
  Int4              iCount = 0;
  Int4              inChildren;
  Int4              iParent;
  Int4              muid;
  Int4              *piChildren;
  FILE              *table = NULL;

  if (strcmp(CDDdbtype,"oasis")==0) bIsOasis = TRUE;

  pcdsThis = pcds;

  strcpy(tableName,GetTempName("cddsrv")); 
  if  (!(table = FileOpen(tableName,WRITE))) CddHtmlError("Temp File Open Failed on Server");

  fprintf(table, "Content-type: text/html\n\n");
  fprintf(table, "<HTML><TITLE>CD summary for %s</title>\n", cCDDid);
  fprintf(table, "<BODY BGCOLOR = \"ffffff\">\n");
  fprintf(table, "<A HREF=\"cdd_form.map\">\n");
  fprintf(table, "<IMG SRC=\"cdbrowse.gif\" BORDER=0 ISMAP></A>\n");

  fprintf(table, "<TABLE BORDER=0 CELLPADDING=3 CELLSPACING=0 WIDTH=600>\n");
  fprintf(table, "\n");
  if (thisTax > 0) {
    fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><H2><FONT COLOR=#000000>CD:</FONT></H2></TD><TD VALIGN=TOP NOWRAP BGCOLOR=#FFFFCC><H2><A HREF=\"%scddsrv.cgi?uid=%s&version=%s\">%s, selected subset</A></TD></TR></H2>\n",
                    URLBase,cCDDid,dbversion,cCDDid);

  } else {
    fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><H2><FONT COLOR=#000000>CD:</FONT></H2></TD><TD VALIGN=TOP NOWRAP BGCOLOR=#FFFFCC><H2><A HREF=\"%scddsrv.cgi?uid=%s&version=%s\">%s</A></TD></TR></H2>\n",
                    URLBase,cCDDid,dbversion,cCDDid);
  }
  pCddesc = pcdd->description;
  while (pCddesc) {
    if (pCddesc->choice == CddDescr_reference) {
      if (!bRefOpen) {
        if (MedArchInit()) {
          bRefOpen = TRUE;
          fprintf(table, "<TR><TD ALIGN=RIGHT BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>References:</FONT></STRONG></TD><TD BGCOLOR=#FFFFCC><A HREF=\"%s",PUBcgi);
          pub = pCddesc->data.ptrvalue; muid=0;
          muid = MedArchCitMatch(pub);
          if (muid) fprintf(table, "%d,",muid);
        }
      } else {
        pub = pCddesc->data.ptrvalue; muid=0;
        muid = MedArchCitMatch(pub);
        if (muid) fprintf(table, "%d,",muid);
      }
    } else {
      if (bRefOpen) {
        fprintf(table, "\">PubMed</A></TD></TR>\n");
        bRefOpen = FALSE; MedArchFini();
      }
      switch(pCddesc->choice) {
        case CddDescr_status:
          fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>CD status:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>");
          switch(pCddesc->data.intvalue) {
            case 0:
              fprintf(table,"unassigned</TD></TR>\n");
              break;        
            case 1:
              fprintf(table,"finished CDD</TD></TR>\n");
              break;        
            case 2:
              fprintf(table,"CDD pending release</TD></TR>\n");
              break;        
            case 3:
              fprintf(table,"Full-length sequences, including 3D structure if known. Alignment from source, reindexed to representative</TD></TR>\n");
              break;        
            case 4:
              fprintf(table,"checkpoint/PSSM only</TD></TR>\n");
	      bProfileOnly = TRUE;
              break;        
            default:
              fprintf(table,"unknown</TD></TR>\n");
              break;        
          }          
          break;
        case CddDescr_create_date:
          dtp = (DatePtr) pCddesc->data.ptrvalue;
          fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Created:</FONT></STRONG></TD>");
          fprintf(table, "<TD BGCOLOR=#FFFFCC>%2d-%3s-%4d</TD></TR>\n",
                         (int)dtp->data[3],
                         NCBI_months[dtp->data[2]-1],
                         (int)dtp->data[1]+1900);
          break;
        case CddDescr_tax_source:
          if (TaxArchInit()) {
            orp  = pCddesc->data.ptrvalue;
            dbtp = orp->db->data.ptrvalue;
            oidp = dbtp->tag;
            iParent = oidp->id;
            cParentTaxName = orp->taxname;
            if (thisTax > 0) {
              iParent = thisTax;
              t1dp = tax1_getbyid(iParent);
              cParentTaxName = t1dp->org->taxname;
            }
            txtnpp = NULL;
            txtnpp = (TXC_TreeNodePtr *)txc_getChildren(iParent,&inChildren);
            if (txtnpp && inChildren > 0) {
              CddCountTaxLineage(txtnpp,inChildren,pcds);
            }
            if (iParent > 1) {
              sprintf(cTax,"%s%d",TAXcgi,iParent);
            } else {
              sprintf(cTax,"%s%d",TAXcgi,0);
            }
            fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Taxonomy spanned:</FONT></STRONG></TD>");
            fprintf(table, "<TD BGCOLOR=#FFFFCC><TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0><TR><TD VALIGN=TOP>");
            fprintf(table, "<A HREF=\"%s\">%s&nbsp</A>->&nbsp</TD>\n",cTax,cParentTaxName);
            if (txtnpp && inChildren > 0) {
              fprintf(table, "<TD><TABLE BORDER=0 CELLPADDING=0 CELLSPACING=0>");
              for (i=0;i<inChildren;i++) {
                if (txtnpp[i]->flags > 0) {
                  t1dp = tax1_getbyid(txtnpp[i]->tax_id);
                  sprintf(cTax,"%s%d",TAXcgi,txtnpp[i]->tax_id);
                  fprintf(table, "<TR><TD ALIGN=RIGHT>");
                  fprintf(table, "<A HREF=\"%scddsrv.cgi?uid=%s&tax=%d&version=%s\">[%3d]&nbsp</A> from&nbsp</TD>",
                          URLBase,cCDDid,txtnpp[i]->tax_id,dbversion,(Int4)txtnpp[i]->flags);
                  fprintf(table, "<TD><A HREF=\"%s\">%s</A></TD></TR>\n",cTax,t1dp->org->taxname);
                }            
              }
              fprintf(table, "</TABLE></TD>\n");
            }
            fprintf(table, "</TR></TABLE></TD></TR>\n");
            TaxArchFini();
          }
          break;

        case CddDescr_othername:
          fprintf(table, "\n");
          fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Alternative Name:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>%s</TD></TR>\n",pCddesc->data.ptrvalue);
          break;
        case CddDescr_category:
          fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Category:</FONT></STRONG></TD><TD>%s</TD VALIGN=TOP BGCOLOR=#FFFFCC></TR>\n",pCddesc->data.ptrvalue);
          break;
        case CddDescr_comment:
	  cCurrDesc = pCddesc->data.ptrvalue;
	  if (strcmp(cCurrDesc,"linked to 3D-structure") != 0) {
            fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Description:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>%s</TD></TR>\n",pCddesc->data.ptrvalue);
          }
          break;
        case CddDescr_source:
          strcpy(source,pCddesc->data.ptrvalue);
          if (strcmp("Smart",source) == 0) {
	    if (StrNCmp(cCDDid,"smart0",6) == 0) {
              fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Source:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC><A HREF=\"%s%s\">%s</A></TD></TD>\n",
                      SMACCcgi,&cCDDid[5],source);
	    } else {
              fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Source:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC><A HREF=\"%s%s\">%s</A></TD></TD>\n",
                      SMARTcgi,cCDDid,source);
            }
          } else if (strcmp("Pfam",source) == 0) {
            fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Source:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC><A HREF=\"%s%s\">%s[US]</A>, <A HREF=\"%s%s\">%s[UK]</A></TD></TD>\n",
                    PFAMcgiUS,&cCDDid[4],source,PFAMcgiUK,&cCDDid[4],source);
          } else {
            fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Source:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>%s</TD></TD>\n",source);
          }
          break;
        default:
          break;
      }
    }
    pCddesc = pCddesc->next;
  }
  if (bRefOpen) {
    fprintf(table, "\">PubMed</A></TD></TR>\n");
    bRefOpen = FALSE; MedArchFini();
  }

  while (pcdsThis) {
    iCount++;
    pcdsThis = pcdsThis->next;
  }
  fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Aligned sequences:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>\n");
  fprintf(table, "%d</TD></TR>\n",iCount);

  pcdsThis = pcds;
  if (pcdsThis) {
    fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Representative:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>\n");
    if (pcdsThis->bIsPdb) {
      fprintf(table, "<FONT COLOR=#660000>pdb</FONT>|<A HREF=\"%s%s\">%s %s</A>|", MMDBCALL, pcdsThis->cPdbId,
              pcdsThis->cPdbId, pcdsThis->cChainId);
      if (pcdsThis->cChainId[0] == '\0' || pcdsThis->cChainId[0] == ' ') {
        fprintf(table,"<FONT COLOR=#660000>%s</FONT> [<A HREF=\"%s%s\">CD</A>]\n",pcdsThis->cDefLine,WRPSBCALL,pcdsThis->cPdbId);
      } else {
        fprintf(table,"<FONT COLOR=#660000>%s</FONT> [<A HREF=\"%s%s%s\">CD</A>]\n",pcdsThis->cDefLine,WRPSBCALL,pcdsThis->cPdbId,pcdsThis->cChainId);
      }
    } else if (!bHasConsensus) {
      fprintf(table, "gi|<A HREF=\"%s%d\">%d</A>|", ENTREZCALL, pcdsThis->uid,pcdsThis->uid);
      fprintf(table,"%s [<A HREF=\"%s%d\">CD</A>]\n",pcdsThis->cDefLine,WRPSBCALL,pcdsThis->uid);
    } else {
      fprintf(table,"Consensus sequence:");
    }
    fprintf(table, "</TD></TR>\n");
  }
  sintp = (SeqIntPtr) pcdd->profile_range;
  if (sintp) {
    fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Aligned range:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>%d-%d</TD></TR>\n",
            sintp->from+1,sintp->to+1);
  } 
  if (bHasConsensus || bProfileOnly) {
    fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Sequence:</FONT></STRONG></TD><TD VALIGN=TOP NOWRAP BGCOLOR=#FFFFCC><PRE>\n");
    spp = FastaSeqPort((BioseqPtr)pcdd->trunc_master,FALSE,FALSE,Seq_code_ncbieaa);
    while (FastaSeqLine(spp,buf,60,FALSE)) {
      fprintf(table, "%s\n",buf);
    }
    fprintf(table, "</TD></TR></PRE>\n");
    SeqPortFree(spp);
  }
  fprintf(table, "</TABLE>\n");

  if (pcdsThis) {
    if (bHasPdb) fprintf(table, "<BR><IMG SRC=\"pinkb.gif\" ALIGN=TOP>&nbsp;<FONT COLOR=#660000>This CD alignment includes 3D structure.</FONT> To display structure, download <A HREF=\"http://www.ncbi.nlm.nih.gov/Structure/CN3D/cn3d.shtml\"><STRONG>Cn3D v3.0</STRONG></A>!\n");

/*    fprintf(table, "<FORM METHOD=\"POST\" TARGET=\"_new\" ACTION=\"%scddsrv.cgi\">\n",URLBase); */
    fprintf(table, "<FORM METHOD=\"POST\" ACTION=\"%scddsrv.cgi\">\n",URLBase);
    fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"uid\" VALUE=\"%s\">\n", cCDDid);
    fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"version\" VALUE=\"%s\">\n",dbversion);
    if (thisTax > 0) fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"tax\" VALUE=\"%d\">\n",thisTax);
    fprintf(table, "<INPUT TYPE=\"SUBMIT\" VALUE=\"View Alignment\">&nbsp; showing\n");
    fprintf(table, "<SELECT name=\"maxaln\">\n");
    fprintf(table, "<OPTION VALUE=\"5\">up to 5\n");
    fprintf(table, "<OPTION SELECTED VALUE=\"10\">up to 10\n");
    fprintf(table, "<OPTION VALUE=\"25\">up to 25\n");
    fprintf(table, "<OPTION VALUE=\"50\">up to 50\n");
    fprintf(table, "<OPTION VALUE=\"100\">up to 100\n");
    fprintf(table, "<OPTION VALUE=\"-1\">all\n");
    fprintf(table, "</SELECT>\n");
    fprintf(table, "<SELECT name=\"seltype\">\n");
    fprintf(table, "<OPTION VALUE=\"0\">sequences selected below\n");
    fprintf(table, "<OPTION SELECTED VALUE=\"1\">top listed sequences\n");
    fprintf(table, "<OPTION VALUE=\"2\">of the most diverse sequences\n");
    fprintf(table, "</SELECT>\n");
    fprintf(table, "<INPUT TYPE=\"RESET\">\n");
    fprintf(table, "<TABLE BORDER=0 CELLPADDING=3 CELLSPACING=3>\n");
    if (bHasPdb) {
      fprintf(table, "<TD VALIGN=TOP NOWRAP>\n");
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"chn\" VALUE=1 CHECKED> Aligned chains<BR>\n");
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"chn\" VALUE=0> All chains<BR>\n");  
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ato\" VALUE=0 CHECKED> Virtual Bonds<BR>\n");
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ato\" VALUE=1> All Atoms</TD>\n");  
      fprintf(table, "<TD VALIGN=TOP NOWRAP>\n");
      if (CddGetStatus(pcdd)!=3) {
        fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"mode\" VALUE=\"v\" CHECKED> All in 3D<BR>\n");
        fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"mode\" VALUE=\"s\"> Representative in 3D</TD>\n");
/*      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"mode\" VALUE=\"a\"> None in 3D</TD>\n"); */
      }
    }
    fprintf(table, "<TD VALIGN=TOP NOWRAP>\n");
    if (bHasPdb) {
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=0 CHECKED> Launch Cn3D<BR>\n");
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=2> HTML Display<BR>\n");
    } else {
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=2 CHECKED> HTML Display<BR>\n");
    }
    fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=3> Text Display</TD>\n");
    fprintf(table, "<TD VALIGN=TOP NOWRAP>\n");
    fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=4> FASTA with gaps<BR>\n");
    fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=5> Phylip format<BR>\n");
    if (bHasPdb) fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=1> See ASN.1 file\n");
    fprintf(table, "</TD>\n");
    fprintf(table, "<TD VALIGN=TOP NOWRAP>\n");
    fprintf(table, "Conservation color threshold:&nbsp\n");
    fprintf(table, "<SELECT NAME=\"bit\">\n");
    fprintf(table, "<OPTION VALUE=\"0.5\">0.5 bits\n");
    fprintf(table, "<OPTION VALUE=\"1.0\">1.0 bits\n");
    fprintf(table, "<OPTION VALUE=\"1.5\">1.5 bits\n");
    fprintf(table, "<OPTION SELECTED VALUE=\"2.0\">2.0 bits\n");
    fprintf(table, "<OPTION VALUE=\"2.5\">2.5 bits\n");
    fprintf(table, "<OPTION VALUE=\"3.0\">3.0 bits\n");
    fprintf(table, "<OPTION VALUE=\"3.5\">3.5 bits\n");
    fprintf(table, "<OPTION VALUE=\"4.0\">4.0 bits\n");
    fprintf(table, "<OPTION VALUE=\"0.0\">Identity\n");
    fprintf(table, "</SELECT><BR>\n");
    fprintf(table, "Alignment width:&nbsp\n");
    fprintf(table, "<SELECT NAME=\"pwidth\">\n");
    fprintf(table, "<OPTION VALUE=\"40\">40\n");
    fprintf(table, "<OPTION SELECTED VALUE=\"60\">60\n");
    fprintf(table, "<OPTION VALUE=\"80\">80\n");
    fprintf(table, "<OPTION VALUE=\"100\">100\n");
    fprintf(table, "</SELECT>\n");
    fprintf(table, "</TD>\n");
    fprintf(table, "</TR></TABLE>\n");
    fprintf(table, "<TABLE BORDER=\"1\" CELLPADDING=\"2\" CELLSPACING=\"2\" WIDTH=100%%>\n");  
    if ((bHasPdb && bHasConsensus && bIsOasis) && bShowTax)
      fprintf(table, "<TR><TD COLSPAN=5 ALIGN=CENTER VALIGN=TOP NOWRAP>pick aligned sequences (will be added to selection above)</TD></TR>");
    else if ((bHasPdb && bHasConsensus && bIsOasis) || bShowTax)
      fprintf(table, "<TR><TD COLSPAN=4 ALIGN=CENTER VALIGN=TOP NOWRAP>pick aligned sequences (will be added to selection above)</TD></TR>");
    else fprintf(table, "<TR><TD COLSPAN=3 ALIGN=CENTER VALIGN=TOP NOWRAP>pick aligned sequences (will be added to selection above)</TD></TR>");
    pcdsThis = pcdsThis->next;

    fprintf(table, "<TR><TH>&nbsp;</TH>");
    if (bHasConsensus && bHasPdb && bIsOasis) fprintf(table,"<TH>3D</TH>");
    fprintf(table, "<TH>PDB-Id/gi</TH>");
    if (bShowTax) fprintf(table,"<TH>Organism</TH>");
    fprintf(table, "<TH ALIGN=\"LEFT\">Definition</TH></TR>\n");
    
    while (pcdsThis) {
      fprintf(table, "<TR>\n");
      fprintf(table, "<TD>\n");
      fprintf(table, "<INPUT TYPE=\"CHECKBOX\" NAME=\"gi\" VALUE=\"%d\">\n",pcdsThis->iCddIdx);
      fprintf(table, "</TD>\n");
      if (pcdsThis->bIsPdb && bHasConsensus && bIsOasis) {
        if (pcdsThis->iCddIdx == 1) {
          fprintf(table,"<TD><INPUT TYPE=\"RADIO\" NAME=\"Tdrep\" VALUE=%d CHECKED></TD>\n",pcdsThis->iCddIdx);
	} else {
          fprintf(table,"<TD><INPUT TYPE=\"RADIO\" NAME=\"Tdrep\" VALUE=%d></TD>\n",pcdsThis->iCddIdx);
        }
      }
      if (pcdsThis->bIsPdb) {
        fprintf(table, "<TD NOWRAP>\n");
        fprintf(table, "<a href=\"%s%s\">%s %s</a>\n", MMDBCALL, pcdsThis->cPdbId, 
                        pcdsThis->cPdbId, pcdsThis->cChainId);
      } else {
        if (bHasConsensus && bIsOasis && bHasPdb) fprintf(table,"<TD NOWRAP COLSPAN=2>\n");
	else fprintf(table,"<TD NOWRAP>\n");    
        fprintf(table, "<A HREF=\"%s%d\">%d</a>\n", ENTREZCALL, pcdsThis->uid, pcdsThis->uid);
        fprintf(table, "</TD>\n");
      }
      if (bShowTax) {
        fprintf(table,"<TD NOWRAP><A HREF=\"%s%d\">%s</A></TD>\n",TAXcgi,pcdsThis->iTaxId,pcdsThis->cTaxName);
      }
      fprintf(table, "<TD>\n");
      if (pcdsThis->bIsPdb) {
        if (pcdsThis->cChainId[0] == '\0' || pcdsThis->cChainId[0] == ' ') {
          fprintf(table, "<FONT COLOR=#660000>%s</FONT> [<a href=\"%s%s\">CD</a>]\n",pcdsThis->cDefLine,WRPSBCALL,pcdsThis->cPdbId);
        } else {
          fprintf(table, "<FONT COLOR=#660000>%s</FONT> [<a href=\"%s%s%s\">CD</a>]\n",pcdsThis->cDefLine,WRPSBCALL,pcdsThis->cPdbId,pcdsThis->cChainId);
	}
      } else {
        fprintf(table, "%s [<a href=\"%s%d\">CD</a>]\n",pcdsThis->cDefLine,WRPSBCALL,pcdsThis->uid);
      }
      fprintf(table, "</TD>\n");
      fprintf(table, "</TR>\n");
      pcdsThis = pcdsThis->next;
    }
    fprintf(table, "</TABLE>\n");
    fprintf(table, "</FORM>\n");
    fprintf(table, "<FORM METHOD=\"POST\" ACTION=\"%scddfind.cgi\">\n",URLBase);
    fprintf(table, "<INPUT TYPE=\"SUBMIT\" VALUE=\"Search CDD\"> for \n");
    fprintf(table, "<INPUT TYPE=\"TEXT\" NAME=\"query\" SIZE=\"40\">\n");
    fprintf(table, "<INPUT TYPE=\"RESET\">\n");
    fprintf(table, "</FORM>\n");
  }
  fprintf(table,"</BODY></HTML>\n");
	fflush(table);
	if (table != stdout) {
	  fclose(table);
	  PrintFile(tableName);
  }
  RemoveTempFiles();
  exit(0);
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
	iUID = MMDBEvalPDB(pcTemp);
  MemFree(pcTemp);
  return iUID; 
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Check if  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static Boolean CddUseThisMMDBid(ValNodePtr location, CddSumPtr pcds)
{
  ChemGraphAlignmentPtr cgap;
  ValNodePtr            vnp;
  Int4                  iThisId;
  CddSumPtr             pcdsThis;

  cgap = (ChemGraphAlignmentPtr) location->data.ptrvalue;
  vnp = cgap->biostruc_ids;
  vnp = vnp->next;
  iThisId = vnp->data.intvalue;
  pcdsThis = pcds;
  while (pcdsThis) {
    if (iThisId == pcdsThis->iMMDBId) {
      pcdsThis->iMMDBId = -1;
      return TRUE ;
    }
    pcdsThis = pcdsThis->next;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Uses Patrick's DDV function to generate HTML-formatted alignment display  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddDumpAlignAsHtml(SeqAlignPtr salp, CddPtr pcdd, CharPtr QuerySeq,
                               CharPtr QueryAlign, Boolean bHasPdb,
			             CharPtr dbversion, Int2 iPDB, Uint2 pwidth,
                               Int4 iQueryGi, CharPtr QueryName)
{
  Uint4                disp_format;
  Char                 tableName[PATH_MAX]; 
  FILE                 *table;
  DDVOptionsBlockPtr   pdob;
  DDV_Disp_Opt         ddo;
  ObjectIdPtr          oidp;
  SeqIdPtr             sip;
  BioseqPtr            bsp;
  CddDescrPtr          pCddesc;
  Char                 source[PATH_MAX];
  CharPtr              cCurrDesc;


  disp_format  = RULER_TOP;
  disp_format |= RULER_TICK;
  switch (iPDB) {
    case 2: 
      disp_format |= DISP_FULL_HTML;
      break;
    case 3: 
      disp_format |= DISP_FULL_TXT;
      break;
    case 4: 
      disp_format |= DISP_FASTA_GAP;
      break;
    case 5: 
      disp_format |= DISP_PHYLIP_TXT;
      break;
    default:
      disp_format |= DISP_FULL_TXT;
      break;
  }
  disp_format |= DISPE_COLOR;
  disp_format |= DISPE_TABLE;
  disp_format |= VIEW_FULLSEQ;
  disp_format |= DISP_BSP_COORD;

  SeqMgrLinkSeqEntry(pcdd->sequences,0,NULL);

  SAM_ReplaceGI(salp);

  if (!AlnMgrNeatlyIndex(salp))
    CddHtmlError("AlnMgr failure: could not neatly index SeqAlign!");
  if (!AlnMgrMakeMultByIntersectOnMaster(salp,FALSE))
    CddHtmlError("AlnMgr failure: could not run IBM on this SeqAlign!"); 

  pdob = MemNew(sizeof(DDVOptionsBlock));
  MemSet(pdob,0,sizeof(DDVOptionsBlock));
	MemSet(&ddo,0,sizeof(DDV_Disp_Opt));
  ddo.bUseColors        = FALSE;
  ddo.ShowLeftTail      = FALSE;
  ddo.ShowRightTail     = FALSE;
  ddo.DispDiscStyle     = MSA_TXT_STYLE_2;
  ddo.SpacerSize        = 0;
  ddo.DiscJustification = DISP_JUST_SPLIT;
  ddo.UAGapStyle        = MSA_TXT_STYLE_GAP;
  ddo.AGapStyle         = MSA_TXT_STYLE_GAP;
/* kludge to avoid including ddvcreate.h               */
/*  ddo.RulerStyle        = RulerStyle_Continue_Start; */
  ddo.RulerStyle        = (Uint1)2;
  pdob->LineSize   = pwidth;
  pdob->bUseLayout = TRUE;
  pdob->byteSpp = NULL;
  pdob->matrix = NULL;
  pdob->ddop = &ddo;


  strcpy(tableName,GetTempName("cddsrv")); 
  if  (!(table = FileOpen(tableName,WRITE))) CddHtmlError("Temp File Open Failed on Server");
  fprintf(table, "Content-type: text/html\n\n");
  fprintf(table, "<HTML><TITLE>CD multiple alignment for %s</TITLE><BODY BGCOLOR=\"ffffff\">\n",
          cCDDid);
  fprintf(table, "<A HREF=\"cdd_form.map\">\n");
  fprintf(table, "<IMG SRC=\"cdbrowse.gif\" BORDER=0 ISMAP></A>\n");

/*---------------------------------------------------------------------------*/
/* if necessary, write out menu for alignment redrawing                      */
/*---------------------------------------------------------------------------*/
  if ((QuerySeq || iQueryGi != -1) && QueryAlign) {

    fprintf(table, "<TABLE BORDER=0 CELLPADDING=3 CELLSPACING=0>\n");
    fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><H2><FONT COLOR=#000000>CD:</FONT></H2></TD><TD VALIGN=TOP NOWRAP BGCOLOR=#FFFFCC><H2><A HREF=\"%scddsrv.cgi?uid=%s&version=%s\">%s</A><FONT size=-1>, &nbsp; CD-Search result with query-sequence added</FONT></H2></TD></TR>\n",
            URLBase,cCDDid,dbversion,cCDDid);

    pCddesc = (CddDescrPtr) pcdd->description;
    while (pCddesc) {
      switch(pCddesc->choice) {
        case CddDescr_othername:
          fprintf(table, "\n");
          fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Alternative Name:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>%s</TD></TR>\n",pCddesc->data.ptrvalue);
          break;
        case CddDescr_category:
          fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Category:</FONT></STRONG></TD><TD>%s</TD VALIGN=TOP BGCOLOR=#FFFFCC></TR>\n",pCddesc->data.ptrvalue);
          break;
        case CddDescr_comment:
	  cCurrDesc = pCddesc->data.ptrvalue;
	  if (strcmp(cCurrDesc,"linked to 3D-structure") != 0) 
            fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Description:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>%s</TD></TR>\n",pCddesc->data.ptrvalue);
          break;
        case CddDescr_source:
          strcpy(source,pCddesc->data.ptrvalue);
          if (strcmp("Smart",source) == 0) {
	    if (StrNCmp(cCDDid,"smart0",6) == 0) {
              fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Source:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC><A HREF=\"%s%s\">%s</A></TD></TD>\n",
                      SMACCcgi,&cCDDid[5],source);
	    } else {
              fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Source:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC><A HREF=\"%s%s\">%s</A></TD></TD>\n",
                      SMARTcgi,cCDDid,source);
            }
          } else if (strcmp("Pfam",source) == 0) {
            fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Source:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC><A HREF=\"%s%s\">%s[US]</A>, <A HREF=\"%s%s\">%s[UK]</A></TD></TD>\n",
                    PFAMcgiUS,&cCDDid[4],source,PFAMcgiUK,&cCDDid[4],source);
          } else {
            fprintf(table, "<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Source:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>%s</TD></TD>\n",source);
          }
          break;
        default:
          break;
      }
      pCddesc = pCddesc->next;
    }
    fprintf(table, "</TABLE>\n");

    if (bHasPdb) fprintf(table, "<BR><IMG SRC=\"pinkb.gif\" ALIGN=TOP>&nbsp;<FONT COLOR=#660000>This CD alignment includes 3D structure.</FONT> To display structure, download <A HREF=\"http://www.ncbi.nlm.nih.gov/Structure/CN3D/cn3d.shtml\"><STRONG>Cn3D v3.0</STRONG></A>!\n");
    fprintf(table, "<FORM METHOD=\"POST\" ACTION=\"%scddsrv.cgi\">\n",URLBase);
    fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"uid\" VALUE=\"%s\">\n", cCDDid);
    fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"version\" VALUE=\"%s\">\n", dbversion);
    if (QuerySeq) fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"query\" VALUE=\"%s\">\n", QuerySeq);
    if (QueryAlign) fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"aln\" VALUE=\"%s\">\n", QueryAlign);
    if (QueryName) fprintf(table, "<INPUT TYPE=\"HIDDEN\" NAME=\"querynm\" VALUE=\"%s\">\n", QueryName);
    if (iQueryGi != -1) fprintf(table, "<INPUT TYPE=\"HIDDEN\",NAME=\"qerygi\" VALUE=\"%d\">\n",iQueryGi);
    fprintf(table, "<INPUT TYPE=\"SUBMIT\" VALUE=\"Redisplay Alignment\"> showing\n");
    fprintf(table, "<SELECT NAME=\"maxaln\">\n");
    fprintf(table, "<OPTION VALUE=\"5\"> up to   5\n");
    fprintf(table, "<OPTION SELECTED VALUE=\"10\">up to  10\n");
    fprintf(table, "<OPTION VALUE=\"25\">up to  25\n");
    fprintf(table, "<OPTION VALUE=\"50\">up to  50\n");
    fprintf(table, "<OPTION VALUE=\"100\">up to 100\n");
    fprintf(table, "<OPTION VALUE=\"-1\">all\n");
    fprintf(table, "</SELECT>sequences\n");
    fprintf(table, "<SELECT NAME=\"seltype\">\n");
    fprintf(table, "<OPTION VALUE=\"1\">from the top of the CD alignment\n"); 
    fprintf(table, "<OPTION VALUE=\"2\">from the most diverse subset\n"); 
    fprintf(table, "<OPTION SELECTED VALUE=\"3\">most similar to the query \n"); 
    fprintf(table, "</SELECT>\n");
    
    fprintf(table, "<TABLE BORDER=0 CELLPADDING=3 CELLSPACING=3> <TR><TD VALIGN=TOP NOWRAP>\n");
    if (bHasPdb) {
      fprintf(table, "<TD VALIGN=TOP NOWRAP>\n");
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"chn\" VALUE=1 CHECKED> Aligned chains<BR>\n");
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"chn\" VALUE=0> All chains</TD>\n");  
      fprintf(table, "<TD VALIGN=TOP NOWRAP>\n");
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ato\" VALUE=0 CHECKED> Virtual Bonds<BR>\n");
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ato\" VALUE=1> All Atoms</TD>\n");
    }
    fprintf(table, "<TD VALIGN=TOP NOWRAP>\n");
    if (bHasPdb) {
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=0 CHECKED> Launch Cn3D<BR>\n");
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=2> HTML Display<BR>\n");
    } else {
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=2 CHECKED> HTML Display<BR>\n");
    }
    fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=3> Text Display</TD>\n");
    fprintf(table, "<TD VALIGN=TOP NOWRAP>\n");
    fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=4> FASTA with gaps<BR>\n");
    fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=5> Phylip format</TD>\n");
    if (CddGetStatus(pcdd)!=3) {
      fprintf(table, "<TD VALIGN=TOP NOWRAP>\n");
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"mode\" VALUE=\"v\" CHECKED> All in 3D<BR>\n");
      fprintf(table, "<INPUT TYPE=\"RADIO\" NAME=\"mode\" VALUE=\"s\"> Representative in 3D</TD>\n");
    }
    fprintf(table, "<TD VALIGN=TOP NOWRAP>\n");
    fprintf(table, "Conservation color threshold:&nbsp\n");
    fprintf(table, "<SELECT NAME=\"bit\">\n");
    fprintf(table, "<OPTION VALUE=\"0.5\">0.5 bits\n");
    fprintf(table, "<OPTION VALUE=\"1.0\">1.0 bits\n");
    fprintf(table, "<OPTION VALUE=\"1.5\">1.5 bits\n");
    fprintf(table, "<OPTION SELECTED VALUE=\"2.0\">2.0 bits\n");
    fprintf(table, "<OPTION VALUE=\"2.5\">2.5 bits\n");
    fprintf(table, "<OPTION VALUE=\"3.0\">3.0 bits\n");
    fprintf(table, "<OPTION VALUE=\"3.5\">3.5 bits\n");
    fprintf(table, "<OPTION VALUE=\"4.0\">4.0 bits\n");
    fprintf(table, "<OPTION VALUE=\"0.0\">Identity\n");
    fprintf(table, "</SELECT><BR>\n");
    fprintf(table, "Alignment width:&nbsp\n");
    fprintf(table, "<SELECT NAME=\"pwidth\">\n");
    fprintf(table, "<OPTION VALUE=\"40\">40\n");
    fprintf(table, "<OPTION SELECTED VALUE=\"60\">60\n");
    fprintf(table, "<OPTION VALUE=\"80\">80\n");
    fprintf(table, "<OPTION VALUE=\"100\">100\n");
    fprintf(table, "</SELECT>\n");
    fprintf(table, "</TD>\n");
    fprintf(table, "</TR></TABLE>\n");
    fprintf(table, "</FORM>\n");
  }

/* Test code inserted to check whether query sequence is accessible at
   this point

  oidp = ObjectIdNew();
  sip = (SeqIdPtr) ValNodeNew(NULL);
  sip->choice = 1;
  oidp->str = MemNew(6*sizeof(Char));
  strcpy(oidp->str,"query");
  oidp->str[5]='\0';
  sip->data.ptrvalue = oidp;
  bsp = BioseqLockById(sip);
*/

  if (iPDB != 2) fprintf(table,"<PRE>\n");
  if(!DDV_DisplayDefaultAlign(salp,0,-1,-1,disp_format,pdob,table))
    CddHtmlError("DDV failure: could not convert alignment to HTML!\n");
  if (iPDB != 2) fprintf(table,"</PRE>\n");
  fprintf(table, "</BODY></HTML>\n");
	fflush(table);
	if (table != stdout) {
	  fclose(table);
	  PrintFile(tableName);
  }
  RemoveTempFiles();
  exit(0);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* use Paul's function to output HTML or Text-formatted alignments           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean CddInvokeAlignView(NcbiMimeAsn1Ptr pvnNcbi, CharPtr CDDalign, Int2 iPDB,
                           CharPtr QuerySeq, CharPtr QueryAlign, CharPtr dbversion,
			         CddPtr pcdd, Boolean bHasPdb, FloatHi tbit, Uint2 pwidth,
                           Int4 iQueryGi, CharPtr QueryName)
{
  Uint4       size = FileLength(CDDalign);
  Uint4       uCAVoptions = 0;
  BytePtr     buf;
  AsnIoMemPtr aimp;
  CddDescrPtr pCddesc;
  CharPtr     cCurrDesc;
  Char        source[PATH_MAX];
    
#ifdef USE_CAV
  buf = MemNew(size);
  aimp = (AsnIoMemPtr) AsnIoMemOpen("wb",buf,size);
  if (NULL == aimp) return(FALSE);
  if (!NcbiMimeAsn1AsnWrite(pvnNcbi,aimp->aip,NULL)) return(FALSE);
  AsnIoFlush(aimp->aip);
  AsnIoMemClose(aimp);

  uCAVoptions = CAV_TEXT;
  if (iPDB == 2) uCAVoptions = CAV_HTML;
  else if (iPDB == 4) uCAVoptions = CAV_FASTA;
  if (tbit <= 0.0) uCAVoptions |= CAV_SHOW_IDENTITY;
  printf("Content-type: text/html\n\n");
  printf("<HTML><TITLE>CD multiple alignment for %s</TITLE><BODY BGCOLOR=\"ffffff\">\n",cCDDid);
  printf("<A HREF=\"cdd_form.map\">\n");
  printf("<IMG SRC=\"cdbrowse.gif\" BORDER=0 ISMAP></A>\n");
/*---------------------------------------------------------------------------*/
/* if necessary, write out menu for alignment redrawing                      */
/*---------------------------------------------------------------------------*/
  if ((QuerySeq || iQueryGi != -1) && QueryAlign) {

    printf("<TABLE BORDER=0 CELLPADDING=3 CELLSPACING=0>\n");
    printf("<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><H2><FONT COLOR=#000000>CD:</FONT></H2></TD><TD VALIGN=TOP NOWRAP BGCOLOR=#FFFFCC><H2><A HREF=\"%scddsrv.cgi?uid=%s&version=%s\">%s</A><FONT size=-1>, &nbsp; CD-Search result with query-sequence added</FONT></H2></TD></TR>\n",
            URLBase,cCDDid,dbversion,cCDDid);

    pCddesc = (CddDescrPtr) pcdd->description;
    while (pCddesc) {
      switch(pCddesc->choice) {
        case CddDescr_othername:
          printf("\n");
          printf("<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Alternative Name:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>%s</TD></TR>\n",pCddesc->data.ptrvalue);
          break;
        case CddDescr_category:
          printf("<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Category:</FONT></STRONG></TD><TD>%s</TD VALIGN=TOP BGCOLOR=#FFFFCC></TR>\n",pCddesc->data.ptrvalue);
          break;
        case CddDescr_comment:
	  cCurrDesc = pCddesc->data.ptrvalue;
	  if (strcmp(cCurrDesc,"linked to 3D-structure") != 0) 
            printf("<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Description:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>%s</TD></TR>\n",pCddesc->data.ptrvalue);
          break;
        case CddDescr_source:
          strcpy(source,pCddesc->data.ptrvalue);
          if (strcmp("Smart",source) == 0) {
	    if (StrNCmp(cCDDid,"smart0",6) == 0) {
              printf("<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Source:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC><A HREF=\"%s%s\">%s</A></TD></TD>\n",
                      SMACCcgi,&cCDDid[5],source);
	    } else {
              printf("<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Source:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC><A HREF=\"%s%s\">%s</A></TD></TD>\n",
                      SMARTcgi,cCDDid,source);
            }
          } else if (strcmp("Pfam",source) == 0) {
            printf("<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Source:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC><A HREF=\"%s%s\">%s[US]</A>, <A HREF=\"%s%s\">%s[UK]</A></TD></TD>\n",
                    PFAMcgiUS,&cCDDid[4],source,PFAMcgiUK,&cCDDid[4],source);
          } else {
            printf("<TR><TD VALIGN=TOP ALIGN=RIGHT NOWRAP BGCOLOR=#99CCFF><STRONG><FONT COLOR=#000000>Source:</FONT></STRONG></TD><TD VALIGN=TOP BGCOLOR=#FFFFCC>%s</TD></TD>\n",source);
          }
          break;
        default:
          break;
      }
      pCddesc = pCddesc->next;
    }
    printf("</TABLE>\n");

    if (bHasPdb) printf("<BR><IMG SRC=\"pinkb.gif\" ALIGN=TOP>&nbsp;<FONT COLOR=#660000>This CD alignment includes 3D structure.</FONT> To display structure, download <A HREF=\"http://www.ncbi.nlm.nih.gov/Structure/CN3D/cn3d.shtml\"><STRONG>Cn3D v3.0</STRONG></A>!\n");
    printf("<FORM METHOD=\"POST\" ACTION=\"%scddsrv.cgi\">\n",URLBase);
    printf("<INPUT TYPE=\"HIDDEN\" NAME=\"uid\" VALUE=\"%s\">\n", cCDDid);
    printf("<INPUT TYPE=\"HIDDEN\" NAME=\"version\" VALUE=\"%s\">\n", dbversion);
    if (QuerySeq) printf("<INPUT TYPE=\"HIDDEN\" NAME=\"query\" VALUE=\"%s\">\n", QuerySeq);
    if (QueryAlign) printf("<INPUT TYPE=\"HIDDEN\" NAME=\"aln\" VALUE=\"%s\">\n", QueryAlign);
    if (QueryName) printf("<INPUT TYPE=\"HIDDEN\" NAME=\"querynm\" VALUE=\"%s\">\n", QueryName);
    if (iQueryGi != -1) printf("<INPUT TYPE=\"HIDDEN\" NAME=\"querygi\" VALUE=\"%d\">\n",iQueryGi);
    printf("<INPUT TYPE=\"SUBMIT\" VALUE=\"Redisplay Alignment\"> showing\n");
    printf("<SELECT NAME=\"maxaln\">\n");
    printf("<OPTION VALUE=\"5\"> up to   5\n");
    printf("<OPTION SELECTED VALUE=\"10\">up to  10\n");
    printf("<OPTION VALUE=\"25\">up to  25\n");
    printf("<OPTION VALUE=\"50\">up to  50\n");
    printf("<OPTION VALUE=\"100\">up to 100\n");
    printf("<OPTION VALUE=\"-1\">all\n");
    printf("</SELECT>sequences\n");
    printf("<SELECT NAME=\"seltype\">\n");
    printf("<OPTION VALUE=\"1\">from the top of the CD alignment\n"); 
    printf("<OPTION VALUE=\"2\">from the most diverse subset\n"); 
    printf("<OPTION SELECTED VALUE=\"3\">most similar to the query \n"); 
    printf("</SELECT>\n");
    
    printf("<TABLE BORDER=0 CELLPADDING=3 CELLSPACING=3> <TR><TD VALIGN=TOP NOWRAP>\n");
    if (bHasPdb) {
      printf("<TD VALIGN=TOP NOWRAP>\n");
      printf("<INPUT TYPE=\"RADIO\" NAME=\"chn\" VALUE=1 CHECKED> Aligned chains<BR>\n");
      printf("<INPUT TYPE=\"RADIO\" NAME=\"chn\" VALUE=0> All chains</TD>\n");  
      printf("<TD VALIGN=TOP NOWRAP>\n");
      printf("<INPUT TYPE=\"RADIO\" NAME=\"ato\" VALUE=0 CHECKED> Virtual Bonds<BR>\n");
      printf("<INPUT TYPE=\"RADIO\" NAME=\"ato\" VALUE=1> All Atoms</TD>\n");
    }
    printf("<TD VALIGN=TOP NOWRAP>\n");
    if (bHasPdb) {
      printf("<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=0 CHECKED> Launch Cn3D<BR>\n");
      printf("<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=2> HTML Display<BR>\n");
    } else {
      printf("<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=2 CHECKED> HTML Display<BR>\n");
    }
    printf("<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=3> Text Display</TD>\n");
    printf("<TD VALIGN=TOP NOWRAP>\n");
    printf("<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=4> FASTA with gaps<BR>\n");
    printf("<INPUT TYPE=\"RADIO\" NAME=\"ascbin\" VALUE=5> Phylip format</TD>\n");
    if (CddGetStatus(pcdd)!=3) {
      printf("<TD VALIGN=TOP NOWRAP>\n");
      printf("<INPUT TYPE=\"RADIO\" NAME=\"mode\" VALUE=\"v\" CHECKED> All in 3D<BR>\n");
      printf("<INPUT TYPE=\"RADIO\" NAME=\"mode\" VALUE=\"s\"> Representative in 3D</TD>\n");
    }
    printf("<TD VALIGN=TOP NOWRAP>\n");
    printf("Conservation color threshold:&nbsp\n");
    printf("<SELECT NAME=\"bit\">\n");
    printf("<OPTION VALUE=\"0.5\">0.5 bits\n");
    printf("<OPTION VALUE=\"1.0\">1.0 bits\n");
    printf("<OPTION VALUE=\"1.5\">1.5 bits\n");
    printf("<OPTION SELECTED VALUE=\"2.0\">2.0 bits\n");
    printf("<OPTION VALUE=\"2.5\">2.5 bits\n");
    printf("<OPTION VALUE=\"3.0\">3.0 bits\n");
    printf("<OPTION VALUE=\"3.5\">3.5 bits\n");
    printf("<OPTION VALUE=\"4.0\">4.0 bits\n");
    printf("<OPTION VALUE=\"0.0\">Identity\n");
    printf("</SELECT><BR>\n");
    printf("Alignment width:&nbsp\n");
    printf("<SELECT NAME=\"pwidth\">\n");
    printf("<OPTION VALUE=\"40\">40\n");
    printf("<OPTION SELECTED VALUE=\"60\">60\n");
    printf("<OPTION VALUE=\"80\">80\n");
    printf("<OPTION VALUE=\"100\">100\n");
    printf("</SELECT>\n");
    printf("</TD>\n");
    printf("</TR></TABLE>\n");
    printf("</FORM>\n");
  }


  if (iPDB != 2) printf("<PRE>\n");
  CAV_DisplayMultiple(buf, uCAVoptions, pwidth, tbit, NULL);
  MemFree(buf);
  if (iPDB != 2) printf("</PRE>\n");
  printf("</BODY></HTML>\n");
#endif
  return(TRUE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* MAIN Function for cddserver                                               */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int2 Main()
{
  AsnIoPtr                 paiFile;
  AsnIoMemPtr              aimp;
  BioseqPtr                bsp, bspQuery      = NULL;
  BioseqPtr                bspRet;
  BioseqSetPtr             bssp;
  BiostrucAlignPtr         pbsaStruct;
  BiostrucAlignSeqPtr      pbsaSeq;
  BiostrucAnnotSetPtr      pbsa = NULL;
  BiostrucAnnotSetPtr      pbsaShort = NULL;
  BiostrucFeaturePtr       pbsf, pbsfCopy, pbsfTail;
  BiostrucIdPtr            pbsi;
  BiostrucPtr              pbsMaster, pbsSlave;
  BiostrucPtr              pbsSlaveHead       = NULL;
  BiostrucPtr              pbsSlaveTail, pbsTemp;
  BiostrucSeqsPtr          pbsaStrSeq;
  BytePtr                  buf;
  CddPtr                   pcdd               = NULL;
  CddSumPtr                pcds               = NULL;
  CddSumPtr                pcdsCopy, pcdsTail, pcdsHead;
  CddSumPtr                pcdsThis           = NULL;
  CharPtr                  Name;
  CharPtr                  outptr             = NULL;
  CharPtr                  www_arg, cPart;
  CharPtr                  QuerySeq           = NULL;
  CharPtr                  QueryAlign         = NULL;
  CharPtr                  QueryName          = NULL;
  DenseDiagPtr             ddp;
  DenseDiagPtr             ddpQuery, ddpQTail;
  Int4Ptr                  iGiList            = NULL;
  NcbiMimeAsn1Ptr          pvnNcbi;
  ObjectIdPtr              oidp;
  PDBSeqIdPtr              pdb_seq_id;
  ScorePtr                 psc;
  SeqAlignPtr              salpHead;
  SeqAlignPtr              salpTail;
  SeqAlignPtr              salpCopy, salpFlat;
  SeqAlignPtr              salpQuery          = NULL;
  SeqAnnotPtr              psaCAlignHead      = NULL;
  SeqEntryPtr              sep, sequences     = NULL;
  SeqEntryPtr              sepQuery           = NULL;
  SeqIdPtr                 sip, sipNew, sipQuery, sipRet;
  SeqIdPtr                 sipMaster          = NULL;
  SeqIntPtr                sintp;
  ValNodePtr               pvnGi              = NULL;
  ValNodePtr               pvnGis             = NULL;
  WWWInfoPtr               www_info;
  Boolean                  bSelect            = FALSE;
  Boolean                  bAtom              = FALSE;
  Boolean                  bChain             = TRUE;
  Boolean                  bFix               = TRUE;
  Boolean                  bHaveMaster        = FALSE;
  Boolean                  bHave3dRep         = FALSE;
  Boolean                  bMode              = CDDSUMMARY;
  Boolean                  bTax               = FALSE;
  Boolean                  bBlast             = FALSE;
  Boolean                  bIsOasis           = FALSE;
  Boolean                  bWantedPick        = FALSE;
  Boolean                  bHasPdb            = FALSE;
  Boolean                  is_network, bRemove;
  Boolean                  bHasConsensus      = FALSE;
#ifdef CAV_DEFAULT
  Boolean                  bUseCddAlignView   = TRUE;
#else
  Boolean                  bUseCddAlignView   = FALSE;
#endif
  Boolean                  bShowTax           = FALSE;
  Char                     CDDalign[PATH_MAX];
  Char                     CDDfile[PATH_MAX];
  Char                     chain[2], cChain;
  Char                     cMode;
  Char                     dbversion[6];
  Char                     szName[5];
  Int2                     iPDB               = 0;
  Int2                     iSeqStrMode        = NOALIGN; 
  Int4                     i3dRepIndex        = 1;
  Int4                     Gi, i, iQueryGi    = -1;
  Int4                     iCddSize           = 0;
  Int4                     iIndex;
  Int4                     iMaxAln            = 0;
  Int4                     iSelType           = 0;
  Int4                     iModelComplexity   = ONECOORDRES;
  Int4                     iNsegments;
  Int4                     indx;
  Int4                     iTaxId             = 0;
  Int4                     NumLabels          = 0;
  Int4                     nGi                = 0;
  Int4                     nPdb               = 0;
  struct rlimit            rl;\
  FILE                    *fp;
  FloatHi                  tbit               = 2.0;
  Uint2                    pwidth             = 60;

/*---------------------------------------------------------------------------*/
/* Yanli's fix for making binary reading work                                */
/*---------------------------------------------------------------------------*/
  objmmdb1AsnLoad(); objmmdb2AsnLoad(); objmmdb3AsnLoad();

/*---------------------------------------------------------------------------*/
/* this sets up the unix time limit                                          */
/*---------------------------------------------------------------------------*/
  getrlimit(RLIMIT_CPU, &rl);
  rl.rlim_max = rl.rlim_cur = CPUTIME_MAX;
  setrlimit(RLIMIT_CPU, &rl);

/*---------------------------------------------------------------------------*/
/* retrieve names for directories etc.                                       */
/*---------------------------------------------------------------------------*/
  if (!CddGetParams()) CddHtmlError("Couldn't read from config file...");

/*---------------------------------------------------------------------------*/
/* Get ready for sequence retrieval                                          */
/*---------------------------------------------------------------------------*/
  if (!PUBSEQBioseqFetchEnable("cddsrv", TRUE))
    CddHtmlError("Unable to enable PUBSEQBioseqFetch");

/*---------------------------------------------------------------------------*/
/* Begin processing www information block                                    */
/*---------------------------------------------------------------------------*/
  if (WWWGetArgs(&www_info) != WWWErrOk)
    CddHtmlError("Failed to process posting - check your get/post syntax.");    
  if ((NumLabels = WWWGetNumEntries(www_info)) == 0) 
    CddHtmlError("No input - nothing to report.");

/*---------------------------------------------------------------------------*/
/* retrieve the Cdd unique identifier                                        */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "uid")) < 0) 
    CddHtmlError("No accession (CDD-ID) was input - nothing to report.");
  www_arg =  WWWGetValueByIndex(www_info, indx);
  strcpy(cCDDid,www_arg);
#ifdef DEBUG
  printf(" DEBUG: CDD-Id passed on to cddsrv is: %s\n",cCDDid);
#endif 

/*---------------------------------------------------------------------------*/
/* retrieve the database version string                                      */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "version")) < 0) {
    strcpy(dbversion,CDDefault);
  } else {
    www_arg = WWWGetValueByIndex(www_info, indx);
    strcpy(dbversion,www_arg);
  }
/*---------------------------------------------------------------------------*/
/* kludge to reset version number to 1.51 if an OLD Smart accession is used  */
/*---------------------------------------------------------------------------*/
  if (StringNCmp(cCDDid,"smart0",6)) {
    if (StringNCmp(cCDDid,"pfam",4)) {
      if (StringNCmp(cCDDid,"LOAD_",5)) {
        if (StringCmp(dbversion,"v1.52") == 0) {
          strcpy(dbversion,"v1.51"); 
        }
      }
    }
  }

/*---------------------------------------------------------------------------*/
/* read in the CDD data structure and fill in the pcds linked list           */
/*---------------------------------------------------------------------------*/
/* access the information contained in the master CDD . No need to continue  */
/* if this file can not be found/accessed                                    */
/*---------------------------------------------------------------------------*/
  if (strcmp(CDDdbtype,"oasis")==0 || strcmp(CDDdbtype,"OASIS")==0 ||
      strcmp(CDDdbtype,"Oasis")==0 || strcmp(CDDdbtype,"oAsIs")==0) {
    bIsOasis = TRUE;
  }
  strcpy(CDDalign,CDDPrefix);
  strcat(CDDalign,dbversion);
  if (bIsOasis) strcat(CDDalign,CDDPost_O); else strcat(CDDalign, CDDPost_C);
  strcat(CDDalign,"/");
  strcat(CDDalign,cCDDid);
  strcat(CDDalign,CDDextens);
#ifdef DEBUG
  printf(" DEBUG: CDD data searched in: %s\n",CDDalign);
#endif 

/*---------------------------------------------------------------------------*/
/* changed reading of the CD asn.1 to BINARY, 6/12/00, as v1.00 is near      */
/*---------------------------------------------------------------------------*/
  pcdd = (CddPtr) CddReadFromFile(CDDalign,TRUE);
  if (!pcdd) CddHtmlError("Could not access CDD data!");
  bHasConsensus = CddHasConsensus(pcdd);

/*---------------------------------------------------------------------------*/
/* retrieve data required for using cddserver as a rps-blast output formatter*/
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"querynm")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    QueryName = strdup(www_arg);
  }
  if ((indx = WWWFindName(www_info,"querygi")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    iQueryGi = (Int4) atoi(www_arg);
    sipRet = (SeqIdPtr) ValNodeNew(NULL);
    sipRet->choice = SEQID_GI;
    sipRet->data.intvalue = iQueryGi;
    bspRet = BioseqLockById(sipRet);
    if (!bspRet) CddHtmlError("Could not retrieve query sequence!");
    sipQuery = (SeqIdPtr) ValNodeNew(NULL);
    sipQuery->choice = SEQID_LOCAL;
    oidp = ObjectIdNew();
    if (QueryName) {
      oidp->str = strdup(QueryName);
    } else {
      oidp->str = strdup("query");
    }
    sipQuery->data.ptrvalue = oidp;
    sipQuery->next = NULL;
    bspQuery = (BioseqPtr) BioseqCopy(sipQuery,sipRet,0,bspRet->length-1,0,FALSE);
    BioseqUnlock(bspRet);
    sepQuery = SeqEntryNew();
    sepQuery->choice = 1;
    sepQuery->data.ptrvalue = bspQuery;    
  } else if ((indx = WWWFindName(www_info,"query")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    if((sepQuery=FastaToSeqBuffEx(www_arg, &outptr,FALSE,NULL,FALSE))==NULL)
      CddHtmlError("Can not convert FASTA formatted sequence!");
    if(sepQuery->choice != 1) CddHtmlError("Conversion from FASTA failed!");
    QuerySeq = strdup(www_arg);
    bspQuery = (BioseqPtr) sepQuery->data.ptrvalue;
    sipQuery = bspQuery->id;
    oidp = ObjectIdNew();
    if (QueryName) {
      oidp->str = strdup(QueryName);
    } else {
      oidp->str = strdup("query");
    }
    MemFree(sipQuery->data.ptrvalue);
    sipQuery->choice = SEQID_LOCAL;
    sipQuery->data.ptrvalue = oidp;
    sipQuery->next = NULL;
  }
  if (bspQuery) {
/*---------------------------------------------------------------------------*/
/* add query sequence to the linked list of bioseqs in the CD                */
/*---------------------------------------------------------------------------*/
    bssp = (BioseqSetPtr) pcdd->sequences->data.ptrvalue;
    ValNodeLink(&(bssp->seq_set),sepQuery);
    SeqMgrAdd(OBJ_BIOSEQ,bspQuery);
    bBlast = TRUE;
  } else {
    bBlast = FALSE;
  }
  if ((indx = WWWFindName(www_info,"aln")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    QueryAlign = strdup(www_arg);
    sintp = (SeqIntPtr) pcdd->profile_range;
    salpQuery = SeqAlignNew();
    salpQuery->dim = 2;
    salpQuery->type = SAT_PARTIAL;
    salpQuery->segtype = SAS_DENDIAG;
    salpQuery->next = NULL;
    cPart = strtok(www_arg,",");
    iNsegments = (Int4) atoi(cPart);
    for (i=0;i<iNsegments;i++) {
      ddpQuery = DenseDiagNew();
      ddpQuery->dim = 2;
      ddpQuery->starts = MemNew(2*sizeof(Int4));
      cPart = strtok(NULL,",");
      ddpQuery->starts[0] = sintp->from + (Int4)atoi(cPart);
      cPart = strtok(NULL,",");
      ddpQuery->starts[1] = (Int4)atoi(cPart);
      cPart = strtok(NULL,",");
      ddpQuery->len = (Int4)atoi(cPart);
      ddpQuery->next = NULL;
      if (!salpQuery->segs) {
        salpQuery->segs = ddpQuery;
        ddpQTail = ddpQuery;
      } else {
        ddpQTail->next = ddpQuery;
        ddpQTail = ddpQuery;
      }      
    }
    bBlast = TRUE;
  } else {
    bBlast = FALSE;
  }
  if (bBlast) {
    bMode = CDDALIGNMENT;
    iSeqStrMode = CDDSEQUONLY;
  }
/*---------------------------------------------------------------------------*/
/* retrieve parameters describing which kind of selection to make on the     */
/* sequences in the set                                                      */
/* iSelType:   0 .... do not use any of the selection mechanisms             */
/*             1 .... use first "maxaln" sequences, as listed in the CD      */
/*             2 .... use the most divergent subset size maxaln              */
/*             3 .... use the maxaln sequences most similar to query         */
/* if iMaxAln is 0 don't use any of these mechanisms (selection must be      */
/* "everything" or single sequences picked from the table)                   */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info,"maxaln")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    iMaxAln = (Int4) atoi(www_arg);
    bMode = CDDALIGNMENT;
  } else iMaxAln = 0;
  if ((indx = WWWFindName(www_info,"seltype")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info,indx);
    iSelType = (Int4) atoi(www_arg);
    if (iSelType == 0) bWantedPick = TRUE;
    if (iSelType < 0) iSelType = 0;
    if (iSelType > 3) iSelType = 3;
    if (!bBlast && iSelType==3) iSelType = 0;
    bMode = CDDALIGNMENT;
  } else iSelType = 0;

/*---------------------------------------------------------------------------*/
/* retrieve the list of gi's selected for visualization                      */
/*---------------------------------------------------------------------------*/
  if (bWantedPick) {
    if (!UseThisGi(0,pvnGis)) {
      bMode = CDDALIGNMENT;
      nGi++;
      pvnGi = ValNodeAddInt(&pvnGis,0,0);
      bSelect = TRUE;
    }
  }
  NumLabels = WWWGetNumEntries(www_info);
  for (indx = 0; indx < NumLabels; indx++) {
    Name = WWWGetNameByIndex(www_info, indx);
    if (StrICmp(Name, "gi") == 0) {
      www_arg = WWWGetValueByIndex(www_info, indx);
      if (isdigit(www_arg[0])) {
        Gi     = (Int4) atol(www_arg);
        if (!UseThisGi(Gi,pvnGis)) {
          bMode  = CDDALIGNMENT;
          nGi++;
          pvnGi  = ValNodeAddInt(&pvnGis, 0, Gi);
	  bSelect = TRUE;
	}
      } else CddHtmlError("Non-numeric identifier - no results.");
    }
  }

/*---------------------------------------------------------------------------*/
/* retrieve, if present, the indicator for which structure to use as master  */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "Tdrep")) >= 0) {
    www_arg = WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      i3dRepIndex = (Int4) atoi(www_arg);
    } else CddHtmlError("Non numerical parameter supplied for \"3drep\"");
  }

/*---------------------------------------------------------------------------*/
/* retrieve the boolean specifying whether the aligned chain only should be  */
/* displayed                                                                 */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "chn")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      if (atoi(www_arg)) bChain = TRUE; 
      else bChain = FALSE;
    } else CddHtmlError("Non numerical parameter supplied for \"chn\"");
  }

/*---------------------------------------------------------------------------*/
/* retrieve the boolean specifying whether an all-atom model should be       */
/* displayed                                                                 */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "ato")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      if (atoi(www_arg)) bAtom = TRUE; 
      else bAtom = FALSE;
    } else CddHtmlError("Non numerical parameter supplied for \"chn\"");
  }
  if (bAtom) iModelComplexity = ONECOORDATOM;

/*---------------------------------------------------------------------------*/
/* retrieve the taxonomy subtree node                                        */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "tax")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      iTaxId = (Int4) atoi(www_arg);  
      bTax = TRUE;
    } else CddHtmlError("Non numerical parameter supplied for \"tax\"");
  }

/*---------------------------------------------------------------------------*/
/* retrieve the informativeness threshold for alignment column coloring      */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "bit")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      tbit = (FloatHi) atof(www_arg);  
    } else CddHtmlError("Non numerical parameter supplied for \"bit\"");
  }

/*---------------------------------------------------------------------------*/
/* retrieve the alignment display width                                      */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "pwidth")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      pwidth = (Uint2) atoi(www_arg);  
    } else CddHtmlError("Non numerical parameter supplied for \"pwidth\"");
  }

/*---------------------------------------------------------------------------*/
/* retrieve the boolean specifying whether the results shall be dumped as    */
/* binary or ascii data                                                      */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "ascbin")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      iPDB = (Int2) atoi(www_arg); 
      if (iPDB < 0) iPDB=0;
      if (iPDB > 5) iPDB=5;
/*---------------------------------------------------------------------------*/
/* kludge to use Paul's CddAlignView by default, if possible                 */
/*---------------------------------------------------------------------------*/
#ifdef CAV_DEFAULT
      if (iPDB == 2 || iPDB == 3 || iPDB == 4) bUseCddAlignView = TRUE;
#endif
    } else CddHtmlError("Non numerical parameter supplied for \"ascbin\"");
  }
  
/*---------------------------------------------------------------------------*/
/* retrieve the boolean specifying whether HTML/Text results shall be dumped */
/* by CddAlignView, to explicitly reset to using DDV if iPDB is 2 or 3       */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "usecav")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      bUseCddAlignView = (Boolean) atoi(www_arg); 
    } else CddHtmlError("Non numerical parameter supplied for \"usecav\"");
  }
  
/*---------------------------------------------------------------------------*/
/* retrieve the boolean specifying whether tax. should be displayed explic.  */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "showtax")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    if (isdigit(www_arg[0])) {
      bShowTax = (Boolean) atoi(www_arg); 
    } else CddHtmlError("Non numerical parameter supplied for \"showtax\"");
  }
  
  
/*---------------------------------------------------------------------------*/
/* end of WWW-parameter parsing block                                        */
/* the only parameter retrieved later is cMode, overrides default alignment  */
/* display settings when dealing with multiple structure CD's (CDDproper..)  */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* if the Cdd contains an alignment, fill the pcds data structure            */
/*---------------------------------------------------------------------------*/
  if (pcdd->seqannot) {
    bssp = (BioseqSetPtr) pcdd->sequences->data.ptrvalue;
    salpHead = (SeqAlignPtr) pcdd->seqannot->data;
    if (salpHead->dim !=2)  CddHtmlError("Wrong dimension in Cdd alignment!");
    while (salpHead) {
      iIndex = 0;
      ddp = (DenseDiagPtr) salpHead->segs; sip = ddp->id;
      while(sip) {
        if (iIndex > 0 || !bHaveMaster) {
          pcdsThis = CddSumNew();
	  if (!bHaveMaster) pcdsThis->sip = SeqIdDup(sip);
          else pcdsThis->sip = sip;
	  pcdsThis->salp = salpHead;
	  pcdsThis->iCddIdx = iCddSize++;
          if (sip->choice == SEQID_PDB) {
	    bHasPdb = TRUE;
            pcdsThis->bIsPdb = TRUE;
            pdb_seq_id = (PDBSeqIdPtr) GetPdbSeqId(sip);
            pcdsThis->cChainId[0] = pdb_seq_id->chain; pcdsThis->cChainId[1] = '\0';
            pcdsThis->cPdbId[0] = pdb_seq_id->mol[0]; pcdsThis->cPdbId[1] = pdb_seq_id->mol[1];
            pcdsThis->cPdbId[2] = pdb_seq_id->mol[2]; pcdsThis->cPdbId[3] = pdb_seq_id->mol[3];
            pcdsThis->cPdbId[4] = '\0';
	    if (!bHave3dRep && pcdsThis->iCddIdx == i3dRepIndex) {
	      pcdsThis->bIs3dRep = TRUE; /* first 3D-seq is 3D representative*/
	      bHave3dRep = TRUE;
	    }
          } else {
            pcdsThis->bIsPdb = FALSE;
            if (pcdsThis->sip->choice == SEQID_GI)
	      pcdsThis->uid = sip->data.intvalue;
	    else pcdsThis->uid = -1;
          }
          if (!bHaveMaster && iIndex == 0) {
            bHaveMaster = TRUE;
            pcdsThis->bIsMaster = TRUE;
          }
          CddSumLink(&(pcds),pcdsThis);
        } 
        iIndex++;
        sip = sip->next;
      }
      salpHead = salpHead->next;
    }
/* for Blast alignment visualization, add pcds */
    if (bBlast) {      
      pcdsThis = CddSumNew();
      pcdsThis->sip = SeqIdDup(sipQuery);
      pcdsThis->bIsPdb = FALSE;
      pcdsThis->uid = 0;
      pcdsThis->bIsMaster = FALSE;
      pcdsThis->iCddIdx = iCddSize + 1;
      pcdsThis->next = NULL;
      CddSumLink(&(pcds),pcdsThis);
    }
  } else bMode = FALSE;            /* can't show alignment if none available */

/*---------------------------------------------------------------------------*/
/* fix problem with displaying CDs that have no consensus, 3D structures,    */
/* but a regular sequence as the representative/master                       */
/*---------------------------------------------------------------------------*/
  if (!bHave3dRep) bHasPdb = FALSE;
  if (!bHasConsensus && !pcds->bIsPdb) bHasPdb=FALSE;

/*---------------------------------------------------------------------------*/
/* in case we're dealing with Blast output formatting, add alignment on top  */
/* Must take care of query sequence later                                    */
/*---------------------------------------------------------------------------*/
  if (bBlast) {
    ddp = salpQuery->segs;
    while (ddp) {                     /* need to properly allocate SeqIdPtrs */
      sipMaster = SeqIdDup(pcds->sip);
      sipNew = SeqIdDup(sipQuery);
      ASSERT(FALSE == CddSameSip(sipMaster,sipNew));
      sipMaster->next = sipNew;
      sipNew->next = NULL;
      ddp->id = sipMaster;
      ddp = ddp->next; 
    }
    salpQuery->next = (SeqAlignPtr) pcdd->seqannot->data;
    pcdsThis->salp = salpQuery;
  }

/*---------------------------------------------------------------------------*/
/* reduce number of sequences pulled out if the query needs to be included   */
/*---------------------------------------------------------------------------*/
  if (iMaxAln>iCddSize  || iMaxAln<0) iMaxAln = iCddSize;
  if (iMaxAln==iCddSize && iSelType > 1) iSelType = 1; 
  if (bBlast && iMaxAln && iMaxAln < iCddSize) iMaxAln--;

/*---------------------------------------------------------------------------*/
/* generate a list of "GI's" in case the most divergent set is selected      */
/*---------------------------------------------------------------------------*/
  if (iMaxAln && iSelType == 2) {
    iGiList = (Int4Ptr) CddMostDiverse(pcdd->distance,iMaxAln);
    if (iGiList) {
      for (indx =0; indx < iMaxAln; indx++) {
        if (!UseThisGi(iGiList[indx],pvnGis)) {
	  nGi ++;
	  pvnGi  = ValNodeAddInt(&pvnGis, 0, iGiList[indx]);
	  bMode  = CDDALIGNMENT;
	}
      }
      if (!UseThisGi(0,pvnGis)) {
	pvnGi  = ValNodeAddInt(&pvnGis, 0, 0);
        bMode  = CDDALIGNMENT;
      }
      free(iGiList);
    } 
  }

/*---------------------------------------------------------------------------*/
/* generate a list of "GI's" in case the most similar set is selected        */
/*---------------------------------------------------------------------------*/
  if (iMaxAln && iSelType == 3 && bBlast) {
    psc = (ScorePtr) CddCalculateQuerySim(pcdd,salpQuery);
    if (psc) {
      iGiList = (Int4Ptr) CddMostSimilarToQuery(psc,iMaxAln);
      ScoreSetFree(psc);
      if (iGiList) {
        for (indx =0; indx < iMaxAln; indx++) {
          if (!UseThisGi(iGiList[indx],pvnGis)) {
	    nGi ++;
  	    pvnGi  = ValNodeAddInt(&pvnGis, 0, iGiList[indx]);
	    bMode  = CDDALIGNMENT;
	  }
        }
        if (!UseThisGi(0,pvnGis)) {
	  pvnGi  = ValNodeAddInt(&pvnGis, 0, 0);
	  bMode  = CDDALIGNMENT;
        }
        free(iGiList);
      }
    }
  }
/*---------------------------------------------------------------------------*/
/* generate a list of "GI's" in case a simple max. number is selected        */
/*---------------------------------------------------------------------------*/
  if (iMaxAln && iSelType == 1) {
    for (indx = 0; indx < iMaxAln; indx++) {
      if (!UseThisGi(indx,pvnGis)) {
        nGi ++;
	pvnGi  = ValNodeAddInt(&pvnGis, 0, indx);
	bMode  = CDDALIGNMENT;
      }
    }
  }

/*---------------------------------------------------------------------------*/
/* if no GI's are selected for alignment display reset the mode to listing   */
/*---------------------------------------------------------------------------*/
  if ((nGi==0 && !bBlast) || bMode==CDDSUMMARY) {
    iSeqStrMode = NOALIGN;
    bMode = CDDSUMMARY;
  }

/*---------------------------------------------------------------------------*/
/* this should happen if no slave is selected (only master gi listed)        */
/*---------------------------------------------------------------------------*/
  if ((bMode && nGi <= 1)) {
    CddHtmlError("Select at least one sequence from the table!");
  }
  
/*---------------------------------------------------------------------------*/
/* check whether the 3D representative is needed and included in the Gi list */
/*---------------------------------------------------------------------------*/
  if (i < 2 && !UseThisGi(i3dRepIndex,pvnGis)) {
    nGi ++;
    pvnGi = ValNodeAddInt(&pvnGis, 0, i3dRepIndex);
  }

/*---------------------------------------------------------------------------*/
/* default mode if more than one gi present is to show alignments, at least  */
/*---------------------------------------------------------------------------*/
  if ((nGi >= 2 || bBlast) && bMode) {
    iSeqStrMode = CDDSEQUONLY;
    bMode = CDDALIGNMENT;
  }

/*---------------------------------------------------------------------------*/
/* The pcds data structure has been filled already. Now subset the alignments*/
/* selected for visualization and create the copy SeqAlign, pile up sequences*/
/*---------------------------------------------------------------------------*/
  pcdsThis = pcds; pcdsTail = NULL; pcdsHead = NULL; 
  salpCopy = NULL; salpTail = NULL; 
  nPdb = 0; iCddSize = 0;
  if (pcdd->seqannot) {
    if (bTax) {
      if (!TaxArchInit()) bTax = FALSE;
    }
    while (pcdsThis) {
      if (((UseThisGi(pcdsThis->iCddIdx,pvnGis) || !bMode) && 
          (!bTax || CddSameTaxLineage(pcdsThis->sip,iTaxId,bssp->seq_set))) ||
	  pcdsThis->bIsMaster || pcdsThis->bIs3dRep) {
	if (iCddSize < iMaxAln || bSelect || iMaxAln==0) {
          iCddSize++;
	  if (pcdsThis->bIsPdb) nPdb++;
          if (!pcdsHead) {
	    pcdsHead = pcdsThis;
	  } else {
	    pcdsTail->next = pcdsThis;
	  }
	  pcdsTail = pcdsThis;
          bsp = (BioseqPtr) CddFindSip(pcdsThis->sip, bssp->seq_set);
	  if (bMode) {
            if (!pcdsThis->bIsMaster) {
	      if (!salpCopy) {
	        salpCopy = pcdsThis->salp;
	      } else {
	        salpTail->next = pcdsThis->salp;
	      }
	      salpTail = pcdsThis->salp;
	    }
            if (bsp) {
	      if (bsp!= (BioseqPtr) CddFindSip(pcdsThis->sip,sequences)) {
                sep = SeqEntryNew();
                sep->choice = 1;
                sep->data.ptrvalue = bsp;
                ValNodeLink(&(sequences),sep);
	      }
	    }
          } else {
            if (bsp) CreateDefLine(NULL,bsp,pcdsThis->cDefLine,256,0,NULL,NULL);
	  }
        }
      }
      pcdsThis = pcdsThis->next;
    }
    if (bTax) TaxArchFini();
  }
  if (pcdsHead) pcds = pcdsHead;
  if (pcdsTail) pcdsTail->next = NULL;
  if (salpTail) salpTail->next = NULL;
#ifdef DEBUG
  printf(" DEBUG: Number of sequences counted in CDD: %4d\n",iCddSize);
  printf(" DEBUG: Number of pdb-derived sequences   : %4d\n",nPdb);
#endif

/*---------------------------------------------------------------------------*/
/* again, add the query (alignment) in case this is a BLAST result           */
/*---------------------------------------------------------------------------*/
  if (bBlast) {
    salpQuery->next = salpCopy;
    salpCopy = salpQuery;
    sep = SeqEntryNew();
    sep->choice = 1;
    sep->data.ptrvalue = bspQuery;
    ValNodeLink(&(sequences), sep); 
  }
  
/*---------------------------------------------------------------------------*/
/* default action: Output a list of gi's and PDB-Id's for subset selection   */
/* CddServerShowTracks ends program  - all code after this assumes that bMode*/
/* is TRUE and that alignments need to be shown                              */
/*---------------------------------------------------------------------------*/
  if (!bMode) {
    if (pcds) {
      CddServerShowTracks(pcds,pcdd,iTaxId,bHasPdb,dbversion,bHasConsensus,bShowTax);
    } else {
      CddServerShowTracks(pcds,pcdd,iTaxId,FALSE,dbversion,bHasConsensus,bShowTax);
    }
  }

/*---------------------------------------------------------------------------*/
/* If HTML display is selected, call Patrick's Function and exit             */
/*---------------------------------------------------------------------------*/
  if ((iPDB > 1 && !bUseCddAlignView) || iPDB == 5) {
    CddDumpAlignAsHtml(salpCopy, pcdd, QuerySeq, QueryAlign, bHasPdb,dbversion,iPDB,pwidth,iQueryGi,QueryName);
  }

/*---------------------------------------------------------------------------*/
/* Initialize the data structure needed to collect alignments and sequences  */
/*---------------------------------------------------------------------------*/
  pbsaSeq    = BiostrucAlignSeqNew();
  pbsaStrSeq = BiostrucSeqsNew();
  pbsaStruct = BiostrucAlignNew();

  pbsaSeq->sequences = sequences;
  pbsaStruct->sequences = sequences;
  pbsaStrSeq->sequences = sequences;
  psaCAlignHead = SeqAnnotNew();
  psaCAlignHead->data = salpCopy;
  psaCAlignHead->desc = pcdd->seqannot->desc;
  psaCAlignHead->type = pcdd->seqannot->type;
 
  OpenMMDBAPI((POWER_VIEW /* ^ FETCH_ENTREZ */), NULL);
  if (!MMDBInit()) CddHtmlError("MMDB Initialization failed");

  pcdsThis = pcds;
  while (pcdsThis) {
    if (pcdsThis->bIsPdb && iPDB < 2) {
      pcdsThis->iMMDBId = ConvertMMDBUID(pcdsThis->cPdbId);
    }
    pcdsThis=pcdsThis->next;
  }

/*---------------------------------------------------------------------------*/
/* automatically change the iSeqStrMode settings if a structure is available */
/*---------------------------------------------------------------------------*/
  if (bHasPdb) {
    if (iSeqStrMode==CDDSEQUONLY) iSeqStrMode = CDDONESTRUC;
    if (nPdb > 1 && CddGetStatus(pcdd)!=3) {
      if (iSeqStrMode==CDDONESTRUC) iSeqStrMode = CDDSEVSTRUC;
    }
  } else if (!bUseCddAlignView) {
/*---------------------------------------------------------------------------*/
/* if no PDB structure is available, the alignment is dumped as HTML by def. */
/*---------------------------------------------------------------------------*/
    iPDB = 2;
    CddDumpAlignAsHtml(salpCopy, pcdd, QuerySeq, QueryAlign, bHasPdb, dbversion, iPDB, pwidth,iQueryGi,QueryName);
  }

/*---------------------------------------------------------------------------*/
/* retrieve Master structure in the case of a single-structure CD            */
/*---------------------------------------------------------------------------*/
  if (bMode && bHasPdb && iPDB < 2) { 
    if (!bHasConsensus) {
      strcpy(szName,pcds->cPdbId);
    } else {
      pcdsThis = pcds; while (pcdsThis) {
        if (pcdsThis->bIsPdb && pcdsThis->bIs3dRep) {
          strcpy(szName,pcdsThis->cPdbId);
	  break;
	}
        pcdsThis = pcdsThis->next;
      }
    }
    pbsMaster = (BiostrucPtr) MMDBBiostrucGet(ConvertMMDBUID(szName),iModelComplexity,1);
    if (!pbsMaster) CddHtmlError("Unable to load Master structure!");
    if (bChain) {
      if (!bHasConsensus) strcpy(chain,pcds->cChainId);
      else strcpy(chain,pcdsThis->cChainId);
      if (chain[0] != ' ') {
        pbsTemp = (BiostrucPtr)PruneBiostruc(pbsMaster,chain);
        pbsMaster = NULL;
        pbsMaster = pbsTemp;
      }
    }
    pbsaStrSeq->structure = pbsMaster;    
    pbsaStruct->master = pbsMaster;
  }

/*---------------------------------------------------------------------------*/
/* retrieve slave structures in the case of a multi-structure CD             */
/*---------------------------------------------------------------------------*/
  if (iSeqStrMode == CDDSEVSTRUC && iPDB < 2) {
    pcdsThis = pcds->next;
    while (pcdsThis) {
      if (pcdsThis->bIsPdb && !pcdsThis->bIs3dRep) {
        strcpy(szName,pcdsThis->cPdbId);
        if (!pbsSlaveHead) {
          pbsSlaveHead = (BiostrucPtr) MMDBBiostrucGet(ConvertMMDBUID(szName),iModelComplexity,1);
          if (!pbsSlaveHead) CddHtmlError("Unable to load slave structure!");
          if (bChain) {
            strcpy(chain,pcdsThis->cChainId);
            if (chain[0] != ' ') {
              pbsTemp = (BiostrucPtr)PruneBiostruc(pbsSlaveHead,chain);
              pbsSlaveHead = NULL;
              pbsSlaveHead = pbsTemp;
            }
          }
          pbsSlaveTail = pbsSlaveHead;
        } else {
          pbsSlave = (BiostrucPtr) MMDBBiostrucGet(ConvertMMDBUID(szName),iModelComplexity, 1);
          if (!pbsSlave) CddHtmlError("Unable to load slave structure!");
          if (bChain) {
            strcpy(chain,pcdsThis->cChainId);
            if (chain[0] != ' ') {
              pbsTemp = (BiostrucPtr)PruneBiostruc(pbsSlave,chain);
              pbsSlave = NULL;
              pbsSlave = pbsTemp;
            }
          }
          pbsSlaveTail->next = pbsSlave;
          pbsSlaveTail = pbsSlaveTail->next;
          pbsSlaveTail->next = NULL;
        }
      }
      pcdsThis = pcdsThis->next;
    }
    pbsaStruct->slaves = pbsSlaveHead;
  }

/*---------------------------------------------------------------------------*/
/* retrieve the program mode if present in the input, it may override        */
/* automatically generated settings                                          */
/*  options for mode are:  'c' .... A CDD summary is generated               */
/*                         'a' .... alignseq generated, single alignment     */
/*                         's' .... strucseqs generated, single alignment    */
/*                         'v' .... alignstruc generated, single alignment   */
/*---------------------------------------------------------------------------*/
  if ((indx = WWWFindName(www_info, "mode")) >= 0) {
    www_arg =  WWWGetValueByIndex(www_info, indx);
    cMode = www_arg[0];
    if (cMode == 'a') {
      bMode = CDDALIGNMENT;
      iSeqStrMode = CDDSEQUONLY;
    } else if (cMode == 's') {
      if (bHasPdb){ bMode = CDDALIGNMENT;
        iSeqStrMode = CDDONESTRUC;
      }
    } else if (cMode == 'v') {
      if (bHasPdb && nPdb > 1) { bMode = CDDALIGNMENT;
        iSeqStrMode = CDDSEVSTRUC;
      }
    } else if (cMode == 'c' || cMode == 'C') bMode = CDDSUMMARY;       
#ifdef DEBUG
    printf(" DEBUG: mode selected as %c, interpreted as %d\n",cMode,bMode);
#endif
  }
  if (bMode != CDDSUMMARY && iPDB > 1) {
    bMode = CDDALIGNMENT;
    iSeqStrMode = CDDSEQUONLY;
  }

/*---------------------------------------------------------------------------*/
/* if more than one structure present, VAST results have to be retrieved     */
/*---------------------------------------------------------------------------*/
  if (bHasPdb && nPdb > 1 && iSeqStrMode != CDDSEQUONLY) {
    pbsa = (BiostrucAnnotSetPtr) pcdd->features;
    pbsfCopy = NULL;
    pbsfTail = NULL;
    if (pbsa) {
      pbsf = pbsa->features->features;
      while (pbsf) {
        if (CddUseThisMMDBid(pbsf->Location_location,pcds)) {
          if (!pbsfCopy) {
            pbsfCopy = pbsf;
            pbsfTail = pbsf;
          } else {
            pbsfTail->next = pbsf;
            pbsfTail = pbsf;
          }
        }  
        pbsf = pbsf->next;
      }
      if (pbsfTail) pbsfTail->next = NULL;
      pbsa->features->features = pbsfCopy;
    }
/*---------------------------------------------------------------------------*/
/* need to remove slave structures not aligned by VAST, otherwise Cn3D may   */
/* behave weird.                                                             */
/*---------------------------------------------------------------------------*/
    pcdsThis = pcds; nPdb = 1;
    while (pcdsThis) {
      if (pcdsThis->bIsPdb && !pcdsThis->bIsMaster && !pcdsThis->bIs3dRep) {
        if (pcdsThis->iMMDBId != -1) {
          pbsSlaveHead = pbsaStruct->slaves;
          pbsSlave = pbsSlaveHead;
          pbsSlaveTail = NULL;
          while (pbsSlaveHead) {
            bRemove = FALSE;
            pbsi=pbsSlaveHead->id;
            while (pbsi) {
              if (pbsi->choice == BiostrucId_mmdb_id) {
                if (pbsi->data.intvalue == pcdsThis->iMMDBId) {
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
        } else nPdb++;
      }
      pcdsThis = pcdsThis->next;
    }
    if (pbsfCopy) pbsaStruct->alignments = pbsa;
    if (nPdb == 1) {
      if (iSeqStrMode == CDDSEVSTRUC) iSeqStrMode = CDDONESTRUC;
    }
  }

/*---------------------------------------------------------------------------*/
/* need to add gi's to Seq-Id's for PDB-derived sequences in order to make   */
/* Cn3D happy                                                                */
/*---------------------------------------------------------------------------*/
  sequences = pbsaSeq->sequences;
  sep = sequences;
  while (sep) {
    bsp = sep->data.ptrvalue;
    sip = bsp->id;
    if (sip->choice == SEQID_PDB && NULL == sip->next) {
      sipNew = ValNodeNew(NULL);
      sipNew->choice = SEQID_GI;
      sipNew->data.intvalue = EntrezFindSeqId(sip);
      ValNodeLink(&(sip),sipNew);
    }
    sep = sep->next;
  }

/*---------------------------------------------------------------------------*/
/* if CD has consensus, and 3D visualization is selected, need to reindex    */
/* alignment to use the 3D representative as the master!                     */
/*---------------------------------------------------------------------------*/
  if (bHasConsensus && iPDB < 2) {
    salpCopy = psaCAlignHead->data;
    pcdsThis = pcds; while (pcdsThis) {
      if (pcdsThis->bIs3dRep) {
        sipMaster = pcdsThis->sip;
        break;
      }
      pcdsThis = pcdsThis->next;
    }
    psaCAlignHead->data = CddReindexSeqAlign(salpCopy, sipMaster, bssp);
  }
  
/*---------------------------------------------------------------------------*/
/* prepare data for output                                                   */
/*---------------------------------------------------------------------------*/
  pvnNcbi=ValNodeNew(NULL);
  switch(iSeqStrMode) {
    case CDDSEQUONLY: pvnNcbi->choice=NcbiMimeAsn1_alignseq; break;
    case CDDONESTRUC: pvnNcbi->choice=NcbiMimeAsn1_strucseqs; break;
    case CDDSEVSTRUC: pvnNcbi->choice=NcbiMimeAsn1_alignstruc; break;
    default: CddHtmlError("Could not interpret alignment mode!"); break;
  }
  switch(iSeqStrMode) {
    case CDDSEQUONLY: pbsaSeq->seqalign    = psaCAlignHead; break;
    case CDDONESTRUC: pbsaStrSeq->seqalign = psaCAlignHead; break;
    case CDDSEVSTRUC: pbsaStruct->seqalign = psaCAlignHead; break;
  }
  switch(iSeqStrMode) {
    case CDDSEQUONLY: pvnNcbi->data.ptrvalue = pbsaSeq;    break;
    case CDDONESTRUC: pvnNcbi->data.ptrvalue = pbsaStrSeq; break;
    case CDDSEVSTRUC: pvnNcbi->data.ptrvalue = pbsaStruct; break;
  }

/*---------------------------------------------------------------------------*/
/* use Paul's CddAlignView, write data into a buffer                         */
/*---------------------------------------------------------------------------*/
#ifdef USE_CAV
  if (iSeqStrMode == CDDSEQUONLY && bUseCddAlignView) {
    if (!CddInvokeAlignView(pvnNcbi,CDDalign,iPDB,QuerySeq,QueryAlign,dbversion,pcdd,bHasPdb,tbit,pwidth,iQueryGi,QueryName)) 
      CddHtmlError("Could not display alignment");
    return 0;  
  }
#endif

/*---------------------------------------------------------------------------*/
/* Insert local Id's instead of duplicate sequence Id's                      */ 
/*---------------------------------------------------------------------------*/
/*  pvnNcbi = (NcbiMimeAsn1Ptr) CheckId(pvnNcbi,NULL);                       */
/*---------------------------------------------------------------------------*/
/* Generate temporary file                                                   */
/*---------------------------------------------------------------------------*/
  strcpy(OutputName,GetTempName("cddsrv"));
  if(!(OutputFile = FileOpen(OutputName,WRITE))) {
    CddHtmlError("Temp File Open Failed at CDD WWW-Server");
  }
/*---------------------------------------------------------------------------*/
/* cn3d file generation                                                      */
/*---------------------------------------------------------------------------*/
  if (iPDB == 0)
    fprintf(OutputFile, "Content-type: chemical/ncbi-asn1-binary\n\n");
  else if (iPDB == 1) {                         /* corresponds to "See File" */
    fprintf(OutputFile, "Content-type: text/html\n\n");
    fprintf(OutputFile, "<HTML><body><pre>\n");
  } else fprintf(OutputFile, "Content-type: application/octet-stream\n\n");
  fflush(OutputFile);
  if (OutputFile != stdout) {
    if (iPDB == 0)
      paiFile = AsnIoNew(ASNIO_BIN_OUT, OutputFile, NULL, NULL, NULL);
    else
      paiFile = AsnIoNew(ASNIO_TEXT_OUT, OutputFile, NULL, NULL, NULL);
    NcbiMimeAsn1AsnWrite(pvnNcbi, paiFile, NULL);
    AsnIoFlush(paiFile);
    AsnIoClose(paiFile);
    if (iPDB == 1) fprintf(OutputFile, "</pre></body></HTML>\n");
    PrintFile(OutputName);    
  }
  CloseMMDBAPI();
  MMDBFini();
  VASTFini();
  PUBSEQFini();
  RemoveTempFiles();   
  return 0;
}
