/* $Id: cddutil.c,v 1.91 2003/12/09 22:21:35 bauer Exp $
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
* File Name:  cddutil.c
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 10/18/1999
*
* $Revision: 1.91 $
*
* File Description: CDD utility routines
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cddutil.c,v $
* Revision 1.91  2003/12/09 22:21:35  bauer
* added CddCountResTypes
*
* Revision 1.90  2003/12/09 18:48:05  bauer
* commented out deprecated fields in BlastKarlinBlkCopy
*
* Revision 1.89  2003/10/07 21:19:39  bauer
* made CddMasterIs3D more general
*
* Revision 1.88  2003/08/25 19:09:47  bauer
* added SeqAlignReadFromFile
*
* Revision 1.87  2003/05/22 18:39:14  bauer
* list txids in Cdd XML dumper
*
* Revision 1.86  2003/05/21 17:25:21  bauer
* optional ObjMgr in CddReadDBGetBioseq
*
* Revision 1.85  2003/05/09 20:38:12  bauer
* report invalid intervals in CddRelocateSeqLoc
*
* Revision 1.84  2003/05/08 14:40:09  bauer
* bugfix in CddConsensus
*
* Revision 1.83  2003/04/25 17:17:32  thiessen
* fix return value type
*
* Revision 1.82  2003/04/25 14:36:20  bauer
* impalaScaling now returns boolean value
*
* Revision 1.81  2003/02/06 21:04:27  bauer
* fixed bug in reindexing to consensus
*
* Revision 1.80  2002/12/03 14:36:31  bauer
* added CddMSLMixedToMSLDenDiag
*
* Revision 1.79  2002/11/22 21:35:23  bauer
* added SeqAnnotReadFromFile and preservation of scores in DenseSeg to DenseDiag conversion
*
* Revision 1.78  2002/10/22 14:54:27  bauer
* fixed date format in XML dumper
*
* Revision 1.77  2002/10/10 20:38:19  bauer
* changes to accomodate new spec items
* - old-root node
* - curation-status
*
* Revision 1.76  2002/10/02 17:32:21  bauer
* avoid merging blocks when reindexing alignments
*
* Revision 1.75  2002/08/17 11:55:07  bauer
* backed out changes
*
* Revision 1.74  2002/08/16 19:51:45  bauer
* added Ben's CddSrvGetStyle2
*
* Revision 1.73  2002/08/06 12:54:26  bauer
* fixes to accomodate COGs
*
* Revision 1.72  2002/07/31 17:15:29  kans
* added prototype for BlastDefLineSetFree for Mac compiler
*
* Revision 1.71  2002/07/31 14:58:58  bauer
* BLAST DB Sequence Retrieval
*
* Revision 1.70  2002/07/17 18:54:10  bauer
* CddFeaturesAreConsistent now returns explicit error messages
*
* Revision 1.69  2002/07/11 14:43:43  bauer
* column 21(X) is now always -1 in PSSMs
*
* Revision 1.68  2002/07/10 22:51:10  bauer
* changed score for 26th pssm column to match whats done by makemat
*
* Revision 1.67  2002/07/10 22:06:07  bauer
* replaced posScaling by impalaScaling
*
* Revision 1.66  2002/07/10 15:34:31  bauer
* made SipIsConsensus public
*
* Revision 1.65  2002/07/09 21:12:39  bauer
* added CddDenDiagCposComp2KBP to return Karlin-Altschul parameters together with PSSM
*
* Revision 1.64  2002/07/05 21:09:25  bauer
* added GetAlignmentSize
*
* Revision 1.63  2002/06/12 15:22:51  bauer
* 6/11/02 update
*
* Revision 1.62  2002/05/31 17:54:31  thiessen
* fix for read-only string literal (e.g. on Mac)
*
* Revision 1.61  2002/05/24 17:49:01  bauer
* Unlock Bioseqs again in SeqAlignInform
*
* Revision 1.60  2002/05/16 22:37:49  bauer
* free seqalign in CddExpAlignToSeqAlign if empty
*
* Revision 1.59  2002/05/06 16:59:50  bauer
* remove blanks in inferred CD short names
*
* Revision 1.58  2002/04/25 14:30:19  bauer
* fixed CddFindMMDBIdInBioseq
*
* Revision 1.57  2002/04/22 16:37:31  bauer
* added check for missing structure alignments
*
* Revision 1.56  2002/04/18 21:00:26  bauer
* added check CddFeaturesAreConsistent
*
* Revision 1.55  2002/04/12 14:02:43  bauer
* added update_date case to CddAssignDescr
*
* Revision 1.54  2002/04/11 14:34:25  bauer
* added CddRemoveConsensus
*
* Revision 1.53  2002/03/28 15:55:00  bauer
* fixed bug in CddFindBioseqInSeqEntry, thanks to Ben
*
* Revision 1.52  2002/02/20 22:27:28  bauer
* utility functions for the CD-Server
*
* Revision 1.51  2002/02/12 23:00:46  bauer
* added missing break in CddRelocateSeqLoc
*
* Revision 1.50  2002/02/06 19:35:37  bauer
* some more CddGet.. functionality
*
* Revision 1.49  2002/02/05 23:15:40  bauer
* added a few CddGet.. utility functions, changes to CddAddDescr allow to extend scrapbook line by line
*
* Revision 1.48  2002/01/28 14:11:29  bauer
* add score to entrez neighbor lists
*
* Revision 1.47  2002/01/05 14:49:44  bauer
* made SeqAlignDup a local function
*
* Revision 1.46  2002/01/04 19:46:56  bauer
* added functions to interconvert PSSM-Ids and accessions
*
* Revision 1.45  2001/11/15 15:35:13  kans
* changed strdup to StringSave for Mac
*
* Revision 1.44  2001/11/13 19:51:52  bauer
* support for annotation transfer in alignment reindexing
*
* Revision 1.43  2001/05/24 15:02:22  kans
* included salpacc for Mac compiler
*
* Revision 1.42  2001/05/23 21:18:05  bauer
* added functions for alignment block structure control
*
* Revision 1.41  2001/04/11 19:49:56  kans
* include profiles.h for Mac compiler
*
* Revision 1.40  2001/04/10 20:26:23  bauer
* *** empty log message ***
*
* Revision 1.39  2001/04/10 20:18:09  bauer
* write out ascii-formatted mtx-Files for copymat
*
* Revision 1.38  2001/03/07 20:27:37  bauer
* fixed bug in CddBioseqCopy
*
* Revision 1.37  2001/03/07 16:30:33  bauer
* fixed alignment reindexing bug
*
* Revision 1.36  2001/03/05 20:25:09  bauer
* fixed gap-penalty selection when changing scoring matrix
*
* Revision 1.35  2001/02/15 15:41:41  shoemake
* fixed small memory leak
*
* Revision 1.34  2001/02/14 17:11:03  bauer
* relaced calls to BioseqCopy with CddBioseqCopy
*
* Revision 1.33  2001/02/13 20:55:10  bauer
* fixed bug when comparing local ids
*
* Revision 1.32  2001/02/07 11:36:51  thiessen
* fix minor memory leak
*
* Revision 1.31  2001/02/06 22:55:35  bauer
* Scoring Matrix now a function parameter in CddDenDiagCposComp2
*
* Revision 1.30  2001/02/06 20:56:10  hurwitz
* get rid of problematic BioSeqNew
*
* Revision 1.29  2001/02/05 22:58:46  bauer
* added alignment reindexing function
*
* Revision 1.28  2001/02/01 17:15:15  bauer
* removed SeqIdDup in CddReadBlastOptions
*
* Revision 1.27  2001/01/26 15:06:40  lewisg
* use entrez2 to retrieve structures
*
* Revision 1.26  2001/01/24 03:08:09  bauer
* fixed memory leaks
*
* Revision 1.25  2001/01/22 21:25:06  hurwitz
* bioseq id shouldn't be freed
*
* Revision 1.24  2001/01/19 23:34:55  bauer
* fixed memory leaks
*
* Revision 1.23  2001/01/19 21:43:44  bauer
* removed dependency from NR for threading PSSM calculations
*
* Revision 1.22  2001/01/17 21:32:02  bauer
* changes to PSSM calculation
*
* Revision 1.21  2001/01/12 01:33:32  bauer
* removed CddGetBlastOptions
*
* Revision 1.20  2001/01/12 00:17:01  bauer
* added routines for information content calculation
*
* Revision 1.19  2001/01/11 23:48:24  bauer
* added check for consensus-Cd
*
* Revision 1.18  2000/12/29 00:52:51  hurwitz
* cleaning memory leaks
*
* Revision 1.17  2000/12/20 18:56:40  hurwitz
* new random num gen, more debug printing
*
* Revision 1.16  2000/12/01 19:36:57  hurwitz
* added scale factor to PSSM calcs
*
* Revision 1.15  2000/11/22 22:34:49  hurwitz
* changed pssm scale factor to match contact potential scale factor
*
* Revision 1.14  2000/11/14 22:08:44  hurwitz
* added weighting for pssm calculation
*
* Revision 1.13  2000/10/10 18:55:06  shoemake
* fixed memory error in CddAssignProfileRange
*
* Revision 1.12  2000/09/08 21:43:51  hurwitz
* adding PSSM calculation to DDE
*
* Revision 1.11  2000/09/01 21:59:04  hurwitz
* re-order columns from PSSM of CDs to column order expected in threading
*
* Revision 1.10  2000/08/30 21:33:55  hurwitz
* added new and free functions for Seq_Mtf and Qry_Seq
*
* Revision 1.9  2000/08/30 16:03:57  bauer
* fixed GCC compiler warning fo CddGetPairId
*
* Revision 1.8  2000/08/14 21:52:04  hurwitz
* added CddCalcPSSM
*
* Revision 1.7  2000/08/14 19:36:26  hurwitz
* got CddCposComp working and tested
*
* Revision 1.6  2000/08/11 19:54:00  hurwitz
* restored CddDenDiagCposComputation and CddCposComputation to original, added CddCposComp which combines the 2
*
* Revision 1.5  2000/08/10 18:18:59  kans
* commented out direct calls to ID1 services
*
* Revision 1.4  2000/08/10 16:50:06  kans
* use StringSave instead of unavailable strdup
*
* Revision 1.3  2000/08/09 21:29:08  hurwitz
* adding cddutil.c to VC++ build
*
* Revision 1.2  2000/07/19 19:32:55  bauer
* added modification logging
*
*
* ==========================================================================
*/


#include <stdio.h>
#include <ncbi.h>
/*#include <accentr.h>*/
#include <lsqfetch.h>
/* #include <netentr.h> */
/* #include <www.h> */
/* #include <sys/resource.h> */
/*#include <accutils.h>*/
#include <mmdbapi.h>
#include <mmdbapi1.h>
/* #include <asnmime.h> */
#include <objseq.h>
#include <objmime.h>
#include <strimprt.h>
#include <cdd.h>
/* #include <objcdd.h> */
#include <readdb.h>
#include <fdlobj.h>
#include <cddutil.h>
#include <edutil.h>
#include <posit.h>
#include <cddposutil.h>
#include <blastpri.h>
#include <accid1.h>
#include <thrddecl.h>
#include <profiles.h>
#include <salpacc.h>

static void CddCposCompPart1(SeqAlignPtr listOfSeqAligns, BioseqPtr bsp,
                             compactSearchItems* compactSearch, ValNodePtr* LetterHead,
                             posSearchItems* posSearch);

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Cdd asn1 reader and writer wrappers                                       */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddWriteToFile(CddPtr pcdd, CharPtr cFile, Boolean bBin)
{
  AsnIoPtr aip = NULL;
  Boolean  bWriteOK;
  
  if (!pcdd) return(FALSE);
  if (bBin) {
     aip = AsnIoOpen(cFile,"wb");
  } else {
     aip = AsnIoOpen(cFile,"w");
  }
  bWriteOK = CddAsnWrite(pcdd,aip,NULL);
  AsnIoClose(aip);
  return(bWriteOK);
}


CddPtr LIBCALL CddReadFromFile(CharPtr cFile, Boolean bBin)
{
  AsnIoPtr aip = NULL;
  CddPtr   pcdd;
  
  if (bBin) {
    aip = AsnIoOpen(cFile,"rb");
  } else {
    aip = AsnIoOpen(cFile,"r");
  }
  pcdd = CddAsnRead(aip,NULL);
  AsnIoClose(aip);
  return(pcdd);
}

SeqAnnotPtr LIBCALL SeqAnnotReadFromFile(CharPtr cFile, Boolean bBin)
{
  AsnIoPtr    aip = NULL;
  SeqAnnotPtr sap;

  if (bBin) {
    aip = AsnIoOpen(cFile,"rb");
  } else {
    aip = AsnIoOpen(cFile,"r");
  }
  sap = SeqAnnotAsnRead(aip, NULL);
  AsnIoClose(aip);
  return(sap);
}

SeqAlignPtr LIBCALL SeqAlignReadFromFile(CharPtr cFile, Boolean bBin)
{
  AsnIoPtr    aip = NULL;
  SeqAlignPtr salp;

  if (bBin) {
    aip = AsnIoOpen(cFile,"rb");
  } else {
    aip = AsnIoOpen(cFile,"r");
  }
  salp = SeqAlignAsnRead(aip, NULL);
  AsnIoClose(aip);
  return(salp);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Cdd-tree asn1 reader and writer wrappers                                  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddTreeWriteToFile(CddTreePtr pcddt, CharPtr cFile, Boolean bBin)
{
  AsnIoPtr aip = NULL;
  Boolean  bWriteOK;
  
  if (!pcddt) return(FALSE);
  if (bBin) {
     aip = AsnIoOpen(cFile,"wb");
  } else {
     aip = AsnIoOpen(cFile,"w");
  }
  bWriteOK = CddTreeAsnWrite(pcddt,aip,NULL);
  AsnIoClose(aip);
  return(bWriteOK);
}


CddTreePtr LIBCALL CddTreeReadFromFile(CharPtr cFile, Boolean bBin)
{
  AsnIoPtr     aip = NULL;
  CddTreePtr   pcddt;
  
  if (bBin) {
     aip = AsnIoOpen(cFile,"rb");
     pcddt = (CddTreePtr) CddTreeAsnRead(aip,NULL);
  } else {
     aip = AsnIoOpen(cFile,"r");
     pcddt = (CddTreePtr) CddTreeAsnRead(aip,NULL);
  }

  AsnIoClose(aip);
  return(pcddt);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Functions to add to a Cdd data structure                                  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddAssignDescr(CddPtr pcdd, Pointer pThis, Int4 iWhat, Int4 iIval)
{
  ValNodePtr  vnp, vnp2;

  vnp = ValNodeNew(NULL);
  vnp->choice = iWhat;
  switch (iWhat) {
    case CddDescr_othername:
      vnp->data.ptrvalue = (CharPtr) pThis;
      break;
    case CddDescr_category:
      vnp->data.ptrvalue = (CharPtr) pThis;
      break;
    case CddDescr_comment:
      vnp->data.ptrvalue = (CharPtr) pThis;
      break;
    case CddDescr_reference:
      vnp->data.ptrvalue = (ValNodePtr) pThis;
      break;
    case CddDescr_create_date:
    case CddDescr_update_date:
      vnp->data.ptrvalue = (DatePtr) pThis;
      break;
    case CddDescr_tax_source:
      vnp->data.ptrvalue = (OrgRefPtr) pThis;
      break;
    case CddDescr_source:
      vnp->data.ptrvalue = (CharPtr) pThis;
      break;
    case CddDescr_status:
      vnp->data.intvalue = (Int4) iIval;
      break;
    case CddDescr_curation_status:
      vnp->data.intvalue = (Int4) iIval;
      break;
    case CddDescr_old_root:
      vnp->data.ptrvalue = (ValNodePtr) pThis;
      break;
    case CddDescr_scrapbook:
      vnp = ValNodeExtractList(&pcdd->description, CddDescr_scrapbook);
      vnp2 = ValNodeCopyStr(NULL,0,(CharPtr)pThis);
      if(vnp) {
	if(vnp->next) ErrPostEx(SEV_WARNING,0,0,"Multiple scrapbooks");
	ValNodeLink((ValNodePtr *)&vnp->data.ptrvalue, vnp2);
      } else ValNodeAddPointer(&vnp, CddDescr_scrapbook, vnp2);
      break;
    default:
      vnp->data.ptrvalue = pThis;
      break;
  }
  ValNodeLink(&(pcdd->description),vnp);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* remove an item from the CD description. Works for a limited set of types. */
/* status and repeats are removed no matter what. For comments, references   */
/* etc. the content of the description is compared to the input              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddKillDescr(CddPtr pcdd, Pointer pThis, Int4 iWhat, Int4 iIval) 
{
  ValNodePtr   vnp, vnpold = NULL, vnpthis = NULL, vnpbefore = NULL, pub;
  
  vnp = pcdd->description;
  while (vnp) {
    if (vnp->choice == iWhat) {
      switch (iWhat) {
        case CddDescr_comment:
        case CddDescr_othername:
        case CddDescr_category:
        case CddDescr_source:
          if (Nlm_StrCmp((CharPtr)pThis,(CharPtr)vnp->data.ptrvalue) == 0) {
            if (!vnpthis) {
	      vnpthis = vnp;
	      vnpbefore = vnpold;
            }
	  }
          break;
	case CddDescr_status:
	case CddDescr_curation_status:
	case CddDescr_old_root:
	case CddDescr_repeats:
          if (!vnpthis) {
	    vnpthis = vnp;
	    vnpbefore = vnpold;
          }
	  break;
        case CddDescr_reference:
	  pub = vnp->data.ptrvalue;
	  if ((pThis && pThis == pub) || 
	      (iIval>0 && pub->choice==PUB_PMid && pub->data.intvalue==iIval)) {
	    if (!vnpthis) {
	      vnpthis = vnp;
	      vnpbefore = vnpold;
	    }
	  }
        default:
	  break;
      }    
    }
    vnpold = vnp;
    vnp = vnp->next;
  }
  if (vnpthis) {
    if (vnpbefore) {
      vnpbefore->next = vnpthis->next;    
    } else {
      pcdd->description = vnpthis->next;
    }
    return TRUE;
  } else return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* series of functions to extract info from CDs                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int4 LIBCALL CddGetVersion(CddPtr pcdd) 
{
  CddIdPtr    cid;
  GlobalIdPtr pGid;

  cid = pcdd->id; while (cid) {
    if (cid->choice == CddId_gid) {
      pGid = cid->data.ptrvalue;
      return pGid->version;
    }
    cid = cid->next;
  }
  return 0;
}

OrgRefPtr LIBCALL CddGetOrgRef(CddPtr pcdd)
{
  CddDescrPtr   pcdsc;

  pcdsc = pcdd->description;
  while (pcdsc) {
    if (pcdsc->choice == CddDescr_tax_source) {
      return (OrgRefPtr) pcdsc->data.ptrvalue;
    }
    pcdsc = pcdsc->next;
  }
  return NULL;
}

Int4 LIBCALL CddGetPssmId(CddPtr pcdd) 
{
  CddIdPtr cid;
  
  cid = pcdd->id; while (cid) {
    if (cid->choice == CddId_uid) {
      return cid->data.intvalue;
    }
    cid = cid->next;
  }
  return 0;
}

Int4 LIBCALL CddGetPmIds(CddPtr pcdd, Int4Ptr iPMids)
{
  Int4          count = 0;
  CddDescrPtr   pcdsc;
  ValNodePtr    pub;
  Boolean       bRefOpen = FALSE;

  if (iPMids) iPMids = MemNew(250*sizeof(Int4));
  pcdsc = pcdd->description;
  while (pcdsc) {
    if (pcdsc->choice == CddDescr_reference) {
      pub = pcdsc->data.ptrvalue;
      if (pub->choice == PUB_PMid) {
        iPMids[count] = pub->data.intvalue;
        count++;
        if (count >= 250) return count;
      }
    }
    pcdsc = pcdsc->next;
  }
  return count;
}


CharPtr LIBCALL CddGetDescr(CddPtr pcdd)
{
  CddDescrPtr   pcdsc;

  pcdsc = pcdd->description;
  while (pcdsc) {
    if (pcdsc->choice == CddDescr_comment) {
      if (Nlm_StrCmp(pcdsc->data.ptrvalue,"linked to 3D-structure") != 0) {
        return (CharPtr) pcdsc->data.ptrvalue;
      }
    }
    pcdsc = pcdsc->next;
  }
  return NULL;
}

DatePtr LIBCALL CddGetCreateDate(CddPtr pcdd)
{
  CddDescrPtr pcdsc;

  pcdsc = pcdd->description;
  while (pcdsc) {
    if (pcdsc->choice == CddDescr_create_date) {
      return (DatePtr) pcdsc->data.ptrvalue;
    }
    pcdsc = pcdsc->next;
  }
  return NULL;
}

DatePtr LIBCALL CddGetUpdateDate(CddPtr pcdd)
{
  DatePtr pcd = NULL, pcdthis;
  CddDescrPtr pcdsc;

  pcdsc = pcdd->description;
  while (pcdsc) {
    if (pcdsc->choice == CddDescr_update_date) {
      pcdthis = (DatePtr) pcdsc->data.ptrvalue;
      if (!pcd) {
	pcd = pcdthis; 
      } else {
	if (pcdthis->data[1] > pcd->data[1]) {
	  pcd = pcdthis;
	} else {
	  if (pcdthis->data[2] > pcd->data[2]) {
	    pcd = pcdthis;
	  } else {
	    if (pcdthis->data[3] > pcd->data[3]) pcd = pcdthis;
	  }
	}
      }
    }
    pcdsc = pcdsc->next;
  }
  return pcd;
}

CharPtr LIBCALL CddGetSource(CddPtr pcdd)
{
  CddDescrPtr pcdsc;

  pcdsc = pcdd->description;
  while (pcdsc) {
    if (pcdsc->choice == CddDescr_source) {
      return (CharPtr) pcdsc->data.ptrvalue;
    }
    pcdsc = pcdsc->next;
  }
  return NULL;
}

CharPtr LIBCALL CddGetSourceId(CddPtr pcdd)
{
  CddDescrPtr pcdsc;
  GlobalIdPtr pGid;
  ValNodePtr  vnp;

  pcdsc = pcdd->description;
  while (pcdsc) {
    if (pcdsc->choice == CddDescr_source_id) {
      vnp = pcdsc->data.ptrvalue;
      if (vnp->choice == CddId_gid) {
        pGid = vnp->data.ptrvalue;
        return (CharPtr) pGid->accession;
      }
    }
    pcdsc = pcdsc->next;
  }
  return NULL;
}

Int4 LIBCALL CddGetAlignmentLength(CddPtr pcdd)
{
  SeqAnnotPtr sap;
  SeqAlignPtr salp;
  Int4        iLength = 0;

  sap = pcdd->seqannot;
  if (sap) {
    salp = (SeqAlignPtr) sap->data;
    while (salp) {
      iLength++;
      salp = salp->next;
    }
  }
  return iLength + 1;
}

/*---------------------------------------------------------------------------*/
/* return number of rows and average number of blocks per row                */
/*---------------------------------------------------------------------------*/
Int4Ptr LIBCALL GetAlignmentSize(SeqAlignPtr salp)
{
  Int4Ptr       pRet;
  Int4          iLength = 0;
  Int4          iBlocks = 0;
  DenseDiagPtr  ddp;
  SeqAlignPtr   salpThis;

  salpThis = salp;
  while (salpThis) {
    iLength++;
    ddp = salpThis->segs;
    while (ddp) {
      iBlocks++;
      ddp = ddp->next;
    }
    salpThis = salpThis->next;
  }
  pRet = MemNew(2*sizeof(Int4));
  pRet[0] = iLength + 1;
  pRet[1] = iBlocks / iLength;
  return pRet;
}

ValNodePtr LIBCALL CddGetAnnotNames(CddPtr pcdd)
{
  AlignAnnotPtr   aap;
  ValNodePtr      vnp, vnpHead = NULL;
 
  aap = pcdd->alignannot;
  while (aap) {
    vnp = ValNodeCopyStr(&(vnpHead),0,aap->description);
    aap = aap->next;  
  }
  return (vnpHead);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Query the status of a CD                                                  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int4 LIBCALL CddGetStatus(CddPtr pcdd)
{
  CddDescrPtr    pCddesc;
  
  pCddesc = pcdd->description;
  while (pCddesc) {
    if (pCddesc->choice == CddDescr_status) {
      return(pCddesc->data.intvalue);
    }
    pCddesc = pCddesc->next;
  }
  return 0;
}

/*---------------------------------------------------------------------------*/
/* find a particular descriptive string in a CD                              */
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddHasDescription(CddPtr pcdd, CharPtr pc)
{
  CddDescrPtr    pCddesc;
  
  if (!pcdd) return FALSE;
  pCddesc = pcdd->description;
  while (pCddesc) {
    if (pCddesc->choice == CddDescr_comment) {
      if (Nlm_StrCmp((CharPtr)pCddesc->data.ptrvalue,pc) == 0) return TRUE;
    }
    pCddesc = pCddesc->next;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------*/
/* Is the alignment decorated with feature annotation?                       */
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddHasAnnotation(CddPtr pcdd)
{
  AlignAnnotPtr aap;
  
  aap = pcdd->alignannot;
  if (aap) {
    if (aap->description) return TRUE;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Is the CD master a 3D Structure                                           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddMasterIs3D(CddPtr pcdd)
{
  SeqAlignPtr     salp;
  DenseDiagPtr    ddp;
  SeqIdPtr        sip;

  if (!pcdd) return FALSE;
  if (!pcdd->seqannot) return FALSE;
  salp = pcdd->seqannot->data;
  ddp = salp->segs;
  sip = ddp->id;
  if (sip->choice == SEQID_PDB) return TRUE;
  if (sip->choice == SEQID_LOCAL) {
    sip = ddp->id->next;
    if (sip->choice == SEQID_PDB) return TRUE;
  }
  return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* How many structure-related alignments in a CDD?                           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int4 LIBCALL CddCount3DAlignments(CddPtr pcdd) 
{
  Int4              n3dali = 0;
  Boolean           bHasConsensus;
  Boolean           bHas3DMaster;
  SeqAlignPtr       salp;
  DenseDiagPtr      ddp;
  SeqIdPtr          sip;
  
  
  bHasConsensus = CddHasConsensus(pcdd);
  bHas3DMaster = CddMasterIs3D(pcdd);

  if (!bHas3DMaster) return(0);
  salp = pcdd->seqannot->data;
  while (salp) {
    ddp = salp->segs;
    sip = ddp->id->next;
    if (sip->choice == SEQID_PDB) n3dali++;
    salp = salp->next;
  }
  if (n3dali && bHasConsensus) n3dali--;
  return (n3dali);
}

/*---------------------------------------------------------------------------*/
/* Check if alignment master is a consensus Sequence                         */
/*---------------------------------------------------------------------------*/
Boolean LIBCALL SeqAlignHasConsensus(SeqAlignPtr salp)
{
  DenseDiagPtr      ddp;
  SeqIdPtr          sip;
  ObjectIdPtr       oidp;

  if (!salp) return(FALSE);
  ASSERT(salp->segtype == SAS_DENDIAG);
  ddp = salp->segs; if (!ddp) return (FALSE);
  sip = ddp->id;    if (!sip) return (FALSE);
  if (sip->choice != SEQID_LOCAL) return (FALSE);
  oidp = sip->data.ptrvalue; if (!oidp) return(FALSE);
  if (StringCmp(oidp->str,"consensus")==0) return (TRUE);
  return(FALSE);
}

/*---------------------------------------------------------------------------*/
/* Check if Cdd has a consensus Sequence                                     */
/*---------------------------------------------------------------------------*/
Boolean LIBCALL SipIsConsensus(SeqIdPtr sip)
{
  ObjectIdPtr   oidp;
  
  if (!sip) return (FALSE);
  if (sip->choice != SEQID_LOCAL) return (FALSE);
  oidp = sip->data.ptrvalue; if (!oidp) return(FALSE);
  if (StringCmp(oidp->str,"consensus")==0) return (TRUE);
  return(FALSE);
}

Boolean LIBCALL CddHasConsensus(CddPtr pcdd)
{
  SeqIntPtr     sintp;
  SeqIdPtr      sip;
  SeqAlignPtr   salp;
  DenseDiagPtr  ddp;
  
  if (!pcdd) return (FALSE);
  sintp = (SeqIntPtr) pcdd->profile_range;
  if (!sintp) {
    salp = pcdd->seqannot->data;
    ddp = salp->segs;
    sip = ddp->id;
    return (SipIsConsensus(sip));
  }
  sip = (SeqIdPtr) sintp->id;
  return(SipIsConsensus(sip));
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* fix CD file names and accessions / Character Arrays must be allocated     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddRegularizeFileName(CharPtr cIn, CharPtr cAcc, CharPtr cFn,
                                   CharPtr cEx)
{
  if (Nlm_StrStr(cIn, cEx) != 0) {
    Nlm_StrCpy(cFn,cIn);
    Nlm_StrNCpy(cAcc,cIn,Nlm_StrLen(cIn)-Nlm_StrLen(cEx));
    cAcc[Nlm_StrLen(cIn)-Nlm_StrLen(cEx)] = '\0';
  } else {
    Nlm_StrCpy(cAcc,cIn);
    Nlm_StrCpy(cFn,cIn);
    if (cEx[0] == '.') {
      Nlm_StrCat(cFn,cEx);
    }
    else {
      Nlm_StrCat(cFn,".");
      Nlm_StrCat(cFn,cEx);
    }
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* repeat detection - find repeated subsequences in a CD consensus Sequence  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddCheckForRepeats(CddPtr pcdd, Int4 width, Int4 GapI, Int4 GapE,
                                   Nlm_FloatHi rthresh, Boolean bOutput)
{
  CddRepeatPtr        pcdr;
  BLAST_ScoreBlkPtr   sbp;
  BioseqPtr           bsp;
  Int4                len, winner = 1;
  Int4                **iscore, repct = 0;
  Int4                **fscore, bst, pnum, plen;
  Int2                iStatus;
  Int4                i, j, k, l, m, n, t1, t2, laststart;
  SeqPortPtr          spp;
  Uint1Ptr            buffer;
  Boolean             done = FALSE;
  Boolean             tbdone;
  Nlm_FloatHi         cfac, wplaus = 0.0;
  SeqLocPtr           slp, slpHead;
  SeqIntPtr           sintp;
  
  typedef struct repstep {
    Int4            score;
    Int4            start;
    Int4            npred;
    Int4            lpred;
    Int4            lremn;
    Nlm_FloatHi     plaus;
  } RepStep;

  RepStep            rep[100];

  if (!CddHasConsensus) return FALSE;
  bsp = pcdd->trunc_master;
  len = bsp->length;
  sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa,1);
  sbp->read_in_matrix = TRUE;
  sbp->protein_alphabet = TRUE;
  sbp->posMatrix = NULL;
  sbp->number_of_contexts = 1;
  iStatus = BlastScoreBlkMatFill(sbp,"BLOSUM62");
  
  spp = SeqPortNew(bsp, 0, LAST_RESIDUE, Seq_strand_unknown,Seq_code_ncbistdaa);
  buffer = MemNew((len)*sizeof(Uint1));
  for (i=0; i<len; i++) buffer[i] = SeqPortGetResidue(spp);
  spp = SeqPortFree(spp);

  iscore = (Int4 **) MemNew(len * sizeof(Int4 *));
  for (i=0;i<len;i++) iscore[i] = (Int4 *) MemNew(len * sizeof(Int4));
  fscore = (Int4 **) MemNew(len * sizeof(Int4 *));
  for (i=0;i<len;i++) fscore[i] = (Int4 *) MemNew(len * sizeof(Int4));

  for (i=0;i<len;i++) {
    t1 = buffer[i];
    iscore[i][i] = sbp->matrix[t1][t1];
    for (j=i+1;j<len;j++) {
      t2 = buffer[j];
      iscore[i][j] = sbp->matrix[t1][t2];
      fscore[j][i] = 0;
    }  
  }

  laststart = 0;
  while (!done) {
    for (i=0;i<len;i++) for (j=i;j<len;j++) fscore[i][j] = iscore[i][j];
    for (i=len-2;i>=0;i--) {
      for (j=len-2;j>=i;j--) {
        bst = fscore[i+1][j+1];
        for (k = i+2;k<i+2+width && k<len;k++) bst = MAX(bst,fscore[k][j+1]-GapI-GapE*(k-i-1));
        for (l = j+2;l<j+2+width && l<len;l++) bst = MAX(bst,fscore[i+1][l]-GapI-GapE*(l-j-1));
        bst = MAX(bst, 0);
        fscore[i][j] += bst;
      }
    }

    bst = 0; l = 0;
    for (j=laststart;j<len;j++) if (fscore[0][j] >= bst) {
      bst = fscore[0][j]; l = j;
    }
    if (bst <= 0.0) {
      done = TRUE;
      break;
    }
    laststart = l;
/*    printf("Next best alignment scores %4d at %4d (of %4d: %5.1f%%)\n",bst,l,len, 100.0*l/len); */
    rep[repct].start = l;
    rep[repct].score = bst;
    rep[repct].npred = 1;
    rep[repct].plaus = 0.0;
    rep[repct].lpred = len;
    rep[repct].lremn = len - rep[repct].start;
    if (repct > 0) {
      rep[repct].lpred = (Int4) (0.5 + ((Nlm_FloatHi)   l / (Nlm_FloatHi) repct));
      rep[repct].npred = (Int4) (0.5 + ((Nlm_FloatHi) len / (Nlm_FloatHi) rep[repct].lpred)); 
    }
    repct++;
    i = 0; j = l;
    tbdone = FALSE;
    if (l >= len-1) {
      tbdone = TRUE;
      done = TRUE;
    }
    while (!tbdone) {
      m = i+1; n = j+1;
      bst = fscore[m][n];
      for (k=i+2;k<i+2+width && k<len;k++) {
        if (fscore[k][j+1]-GapI-GapE*(k-i-1) > bst) {
	  bst = fscore[k][j+1]-GapI-GapE*(k-i-1);
	  m = k; n = j+1;
	}
      }
      for (l=j+2;l<j+2+width && l<len;l++) {
        if (fscore[i+1][l]-GapI-GapE*(l-j-1) > bst) {
	  bst = fscore[i+1][l]-GapI-GapE*(l-j-1);
	  m = i+1; n = l;
	}
      }
      iscore[i][j] = -10000;
      if (bst < 0) tbdone = TRUE;
      i = m; j = n;
      if (i >= len-1 || j >= len-1) {
	tbdone = TRUE;
	iscore[i][j] = -10000;
      }
    }
  }
  
  for (i=0;i<len;i++) MemFree(fscore[i]);
  MemFree(fscore);
  for (i=0;i<len;i++) MemFree(iscore[i]);
  MemFree(iscore);
  rep[0].plaus = (Nlm_FloatHi) (rep[0].score - rep[1].score) / (Nlm_FloatHi) rep[0].score;
  if (rep[0].plaus > rthresh) rep[0].plaus = 1.0;
  winner = 1; wplaus = rep[0].plaus;

  for (i=1;i<repct;i++) {
    pnum = i+1;
    plen = (Int4) (((Nlm_FloatHi)len/(Nlm_FloatHi)pnum)+.5);
    cfac = 0.0;
    for (j=i;j>0;j--) {
      cfac += (Nlm_FloatHi)(abs(rep[j].start-(j*plen)))/(Nlm_FloatHi)plen;
    }
    cfac /= (Nlm_FloatHi) i;
    rep[i].plaus = 1.0 - cfac;

    if (rep[i].plaus > wplaus) {
      wplaus = rep[i].plaus;
      winner = pnum;
    }
  }
  for (i=0;i<winner;i++) {
    if (rep[i].lpred < 3 || (i>0 && rep[i].score < 17)) {
      winner = 1; break;
    }
  }

  if (bOutput) {
    for (i=0;i<repct;i++) {
      printf("%s %3d %4d %8.5f %4d %8.5f %2d %3d %8.5f\n",pcdd->name,
             i,rep[i].score,(Nlm_FloatHi)rep[i].score/(Nlm_FloatHi)rep[0].score,
	     rep[i].start, (Nlm_FloatHi)rep[i].start/(Nlm_FloatHi)len,
	     rep[i].npred,rep[i].lpred, rep[i].plaus);
    }
    printf("%s: predicted %d repeats\n",pcdd->name,winner);
  }

  CddKillDescr(pcdd,NULL,CddDescr_repeats,0);
  if (winner > 1) {
    pcdr = CddRepeatNew(); pcdr->avglen = 0;  
    slp = NULL; slpHead = NULL;
    for (i=0;i<winner;i++) {
      k = rep[i].start;
      if (i < (winner-1)) l = rep[i+1].start - 1; else l = len - 1;
      pcdr->avglen += l-k+1;
      slp = (SeqLocPtr) ValNodeNew(NULL);
      slp->choice = SEQLOC_INT;
      sintp = SeqIntNew();
      sintp->from = k;
      sintp->to = l;
      sintp->id = SeqIdDup(pcdd->profile_range->id);
      slp->data.ptrvalue = sintp;
      if (!slpHead) slpHead = slp; else ValNodeLink(&(slpHead),slp);
    }
    pcdr->avglen /= winner;
    pcdr->count = winner;
    pcdr->location = (SeqLocPtr) ValNodeNew(NULL);
    pcdr->location->choice = SEQLOC_PACKED_INT;
    pcdr->location->data.ptrvalue = slpHead;
    CddAssignDescr(pcdd,pcdr,CddDescr_repeats,0);
  }
  
  sbp = BLAST_ScoreBlkDestruct(sbp);
  MemFree(buffer);
  if (winner > 1) return TRUE;
  return (FALSE);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* retrieve the accession of a CD as a character string                      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
CharPtr LIBCALL CddGetAccession(CddPtr pcdd)
{
  CharPtr     cAccession = NULL;
  ValNodePtr  vnp;
  GlobalIdPtr pGid;

  if (!pcdd) return NULL;
  vnp = pcdd->id;
  while (vnp) {
    if (vnp->choice == CddId_gid) {
      pGid = (GlobalIdPtr) vnp->data.ptrvalue;
      cAccession = StringSave (pGid->accession);
    }
    vnp = vnp->next;
  }
  return cAccession;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* report Errors in processing and exit immediately                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddSimpleHtmlError(CharPtr cErrTxt) 
{
  printf("Content-type: text/html\n\n");
  printf("<h2>Error:</h2>\n");
  printf("<h3>%s</h3>\n",cErrTxt);
  exit(1);
}

void LIBCALL CddSevError(CharPtr cErrTxt) 
{
  printf(" Error: %s\n",cErrTxt);
  exit(1);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Find bioseqptr in Cdd internal sequence set                               */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
BioseqPtr LIBCALL CddFindSip(SeqIdPtr sip, SeqEntryPtr sep)
{
  BioseqPtr    bsp = NULL;
  BioseqSetPtr bssp;
  SeqEntryPtr  sepThis;

  sepThis = sep;
  
  while (sepThis) {
    if (sepThis->choice == 2) {
      bssp = sepThis->data.ptrvalue;
      bsp = CddFindSip(sip, bssp->seq_set);
    } else if (sepThis->choice == 1) {
      bsp = (BioseqPtr) sepThis->data.ptrvalue;
      if (CddSameSip(bsp->id, sip)) return(bsp);
    }
    sepThis = sepThis->next;
  }
  return(NULL);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* version of BioseqCopy that doesn't use BioseqFind and expects the old bsp */
/* adapted by Ben                                                            */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
BioseqPtr LIBCALL CddBioseqCopy (SeqIdPtr newid, BioseqPtr oldbsp, Int4 from,
                                 Int4 to, Uint1 strand, Boolean do_feat)
{
  BioseqPtr   newbsp=NULL, tmpbsp;
  Uint1       seqtype;
  ValNodePtr  tmp;
  ObjectIdPtr oid;
  Int4        len;
  ValNode     fake;
  SeqLocPtr   the_segs, head, curr;
  Boolean     handled = FALSE, split;
  SeqFeatPtr  sfp, newsfp, lastsfp;
  DeltaSeqPtr dsp;
  SeqEntryPtr oldscope;


  if (from < 0) return FALSE;
  if (oldbsp == NULL) return NULL;
  len = to - from + 1;
  if (len <= 0) return NULL;
  newbsp = BioseqNew();
  if (newid != NULL) newbsp->id = SeqIdDup(newid);
  else {
    tmp = ValNodeNew(NULL);
    tmp->choice = SEQID_LOCAL;
    oid = ObjectIdNew();
    tmp->data.ptrvalue = (Pointer)oid;
    oid->str = StringSave("Clipboard");
    tmpbsp = BioseqFind(tmp);   /* old clipboard present? */
    if (tmpbsp == NULL) {
      oldscope = SeqEntrySetScope (NULL);
      if (oldscope != NULL) {
        tmpbsp = BioseqFind(tmp);
        SeqEntrySetScope (oldscope);
      }
    }
    if (tmpbsp != NULL) BioseqFree(tmpbsp);
    newbsp->id = tmp;
  }

  newbsp->repr = oldbsp->repr;
  newbsp->mol = oldbsp->mol;
  newbsp->length = len;
  newbsp->seq_ext_type = oldbsp->seq_ext_type;

  if (newbsp->repr == Seq_repr_virtual)
    handled = TRUE;               /* no more to do */

  if ((newbsp->repr == Seq_repr_raw) ||
    (newbsp->repr == Seq_repr_const)) {
    if (ISA_aa(newbsp->mol)) {
      seqtype = Seq_code_ncbieaa;
    } else {
      seqtype = Seq_code_iupacna;
    }
/*---------------------------------------------------------------------------*/
/* need to check whether seqtypes agree, otherwise the ByteStore is invalid! */
/*---------------------------------------------------------------------------*/
    if (seqtype != oldbsp->seq_data_type) {
      seqtype = oldbsp->seq_data_type;
    }
    newbsp->seq_data_type = seqtype;
    newbsp->seq_data = BSDup(oldbsp->seq_data);
    handled = TRUE;
  }

  if ((newbsp->repr == Seq_repr_seg) ||
    (newbsp->repr == Seq_repr_ref) ||
    (newbsp->repr == Seq_repr_delta)) {
    if (newbsp->repr == Seq_repr_seg) { /* segmented */
      fake.choice = SEQLOC_MIX;   /* make SEQUENCE OF Seq-loc, into one */
      fake.data.ptrvalue = oldbsp->seq_ext;
      fake.next = NULL;
      the_segs = (SeqLocPtr)&fake;
      head = (SeqLocPtr)SeqLocCopyPart (the_segs, from, to, strand, FALSE, NULL, NULL);
    } else if (newbsp->repr == Seq_repr_ref) { /* reference: is a Seq-loc */
      head = (SeqLocPtr)SeqLocCopyPart ((SeqLocPtr)(oldbsp->seq_ext), from, to,
                               strand, TRUE, NULL, NULL);
    } else if (newbsp->repr == Seq_repr_delta) {
      dsp = (DeltaSeqPtr)(oldbsp->seq_ext);  /* real data is here */
      the_segs = (SeqLocPtr)DeltaSeqsToSeqLocs(dsp);
      head = (SeqLocPtr)SeqLocCopyPart (the_segs, from, to, strand, FALSE, NULL, NULL);
      SeqLocFree (the_segs);
    }
    newbsp->seq_ext = (Pointer)head;
    handled = TRUE;
  }

  if (newbsp->repr == Seq_repr_map) {
    lastsfp = NULL;
    for (sfp = (SeqFeatPtr)(oldbsp->seq_ext); sfp != NULL; sfp = sfp->next) {
      split = FALSE;
      curr = (SeqLocPtr)SeqLocCopyRegion(newbsp->id, sfp->location, oldbsp, from, to, strand, &split);
      if (curr != NULL) {  /* got one */
        newsfp = (SeqFeatPtr)AsnIoMemCopy((Pointer)sfp, (AsnReadFunc)SeqFeatAsnRead, (AsnWriteFunc)SeqFeatAsnWrite);
        SeqLocFree(newsfp->location);
        newsfp->location = curr;
        if (split)
          newsfp->partial = TRUE;
        if (lastsfp == NULL)  /* first one */
          newbsp->seq_ext = (Pointer)newsfp;
        else
          lastsfp->next = newsfp;
        lastsfp = newsfp;
      }
    }
    handled = TRUE;
  }
  if (! handled) goto erret;
  if (do_feat)
    SeqFeatsCopy (newbsp, oldbsp, from, to, strand);

  return newbsp;

erret:
  BioseqFree(newbsp);
  return NULL;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* extract from a SeqEntryPtr a "minimum bioseq" with a bit of description   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
BioseqPtr LIBCALL CddExtractBioseq(SeqEntryPtr sep, SeqIdPtr sip)
{
  BioseqPtr    bsp, bspNew, bspTemp;
  BioseqSetPtr bssp;
  SeqIdPtr     sipNew;
  ValNodePtr   vnpThis, vnp = NULL;
  SeqAnnotPtr  sanp;

  bsp = BioseqFindInSeqEntry(sip,sep);
  sipNew = SeqIdDup(sip);
  bspNew = (BioseqPtr) CddBioseqCopy(sipNew,bsp,0,bsp->length-1,0,FALSE);
  sipNew = SeqIdFree(sipNew);
  if (sep->choice == 2) {
    bssp = sep->data.ptrvalue;
    vnp = bssp->descr;
    bssp->descr = NULL;
  } else if (sep->choice == 1) {
    bspTemp = sep->data.ptrvalue;
    vnp = bspTemp->descr;
    bspTemp->descr = NULL;
  }
  sanp = bsp->annot;
  vnpThis = bsp->descr;
  bsp->descr = NULL;
  ValNodeLink(&(vnp),vnpThis);
  bspNew->descr = vnp;
  if (sanp) {
    bspNew->annot = sanp;
    bsp->annot = NULL; 
  }
  return(bspNew);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Cdd specific alignment format converters                                  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Create a Dense-Diag seqalign from a Discontinuous SeqAlign                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAlignPtr LIBCALL CddMSLMixedToMSLDenDiag(SeqAlignPtr salp)
{
  SeqAlignPtr         salpSub, salpnew, salphead, salptail;
  DenseDiagPtr        ddp, ddphead, ddptail;
  DenseSegPtr         dsp;
  Int4                i, s1, s2;
  ScorePtr            scp, scpHead, scpTail, scpThis;

  if (!salp) return(NULL);
  salphead = NULL;
  salptail = NULL;
  while (salp) {
    if (salp->segtype) {
      if (salp->segtype != SAS_DISC && salp->segtype != SAS_DENSEG) {
        CddSevError("CddMSLDiscToMSLDenDiag: Wrong segtype specified!");
      }
    }
    if (salp->dim) {
      if (salp->dim != 2) {
        CddSevError("CddMSLDiscToMSLDenDiag: Expect alignments of dimension 2!");
      }
    }
    ddphead = NULL;
    ddptail = NULL;

    if (salp->segtype == SAS_DISC) {
      salpSub = salp->segs;
      while (salpSub) {
        if (salpSub->segtype != SAS_DENSEG)
          CddSevError("CddMSLDiscToMSLDenDiag: Wrong segtype in Sub-Alignment!");
        if (salpSub->dim && salpSub->dim !=2)
          CddSevError("CddMSLDiscToMSLDenDiag: Wrong dimension in Sub-Alignment!");
        dsp = salpSub->segs;
        for (i=0;i<dsp->numseg;i++) {
          s1 = dsp->starts[i*2];
          s2 = dsp->starts[i*2+1]; 
          if (s1 >=0 && s2>= 0) {
            ddp = DenseDiagNew();
	    ddp->starts = MemNew(2*sizeof(Int4));
	    ddp->starts[0] = s1;
	    ddp->starts[1] = s2;
	    ddp->len=dsp->lens[i];
            ddp->id = SeqIdDupList(dsp->ids);
	    ddp->dim = 2;
            if (!ddphead) {
	      ddphead = ddp;
	      ddptail = ddp;
	      ddptail->next = NULL;
	    } else {
	      ddptail->next = ddp;
	      ddptail = ddp;
	      ddptail->next = NULL;
	    }
          }
        }
        salpSub = salpSub->next;  
      }
    } else { /* segtype is plain SAS_DENSEG */
      dsp = salp->segs;
      ddphead = NULL;
      ddptail = NULL;
      for (i=0;i<dsp->numseg;i++) {
        s1 = dsp->starts[i*2];
        s2 = dsp->starts[i*2+1]; 
        if (s1 >=0 && s2>= 0) {
          ddp = DenseDiagNew();
	  ddp->starts = MemNew(2*sizeof(Int4));
	  ddp->starts[0] = s1;
	  ddp->starts[1] = s2;
	  ddp->len=dsp->lens[i];
          ddp->id = SeqIdDupList(dsp->ids);
	  ddp->dim = 2;
          if (!ddphead) {
	    ddphead = ddp;
	    ddptail = ddp;
	    ddptail->next = NULL;
	  } else {
	    ddptail->next = ddp;
	    ddptail = ddp;
	    ddptail->next = NULL;
	  }
        }
      }
    }
    salpnew = SeqAlignNew();
    salpnew->type = SAT_PARTIAL;
    salpnew->segtype = SAS_DENDIAG;
    salpnew->dim = 2;
    salpnew->segs = ddphead;
    scp = salp->score; scpHead = NULL;
    while (scp) {
      scpThis = ScoreNew();
      scpThis->id = ObjectIdDup(scp->id);
      scpThis->choice = scp->choice;
      scpThis->value = scp->value;
      if (!scpHead) {
        scpHead = scpThis;
      } else {
        scpTail->next = scpThis;
      }
      scpTail = scpThis;
      scp = scp->next;
    }
    salpnew->score = scpHead;
    if (!salphead) {
      salphead = salpnew;
      salptail = salpnew;
      salptail->next = NULL;
    } else {
      salptail->next = salpnew;
      salptail = salpnew;
      salptail->next = NULL;
    }
    salp = salp->next;
  }
  return(salphead);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Create a Dense-Diag seqalign from a Dense-Seg SeqAlign                    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAlignPtr LIBCALL CddMSLDenSegToMSLDenDiag(SeqAlignPtr salp)
{
  SeqAlignPtr         salpnew, salphead, salptail;
  DenseDiagPtr        ddp, ddphead, ddptail;
  DenseSegPtr         dsp;
  Int4                i, s1, s2;
  ScorePtr            scp, scpHead, scpTail, scpThis;


  if (!salp) return(NULL);
  salphead = NULL;
  salptail = NULL;
  while (salp) {
    if (salp->segtype) {
      if (salp->segtype != SAS_DENSEG) {
        CddSevError("CddMSLDenSegToMSLDenDiag: Wrong segtype specified!");
      }
    }
    if (salp->dim) {
      if (salp->dim != 2) {
        CddSevError("CddMSLDenSegToMSLDenDiag: Expect alignments of dimension 2!");
      }
    }
    dsp = salp->segs;
    ddphead = NULL;
    ddptail = NULL;
    for (i=0;i<dsp->numseg;i++) {
      s1 = dsp->starts[i*2];
      s2 = dsp->starts[i*2+1]; 
      if (s1 >=0 && s2>= 0) {
        ddp = DenseDiagNew();
	ddp->starts = MemNew(2*sizeof(Int4));
	ddp->starts[0] = s1;
	ddp->starts[1] = s2;
	ddp->len=dsp->lens[i];
        ddp->id = SeqIdDupList(dsp->ids);
	ddp->dim = 2;
        if (!ddphead) {
	  ddphead = ddp;
	  ddptail = ddp;
	  ddptail->next = NULL;
	} else {
	  ddptail->next = ddp;
	  ddptail = ddp;
	  ddptail->next = NULL;
	}
      }
    }
    salpnew = SeqAlignNew();
    salpnew->type = SAT_PARTIAL;
    salpnew->segtype = SAS_DENDIAG;
    salpnew->dim = 2;
    salpnew->segs = ddphead;
    scp = salp->score; scpHead = NULL;
    while (scp) {
      scpThis = ScoreNew();
      scpThis->id = ObjectIdDup(scp->id);
      scpThis->choice = scp->choice;
      scpThis->value = scp->value;
      if (!scpHead) {
        scpHead = scpThis;
      } else {
        scpTail->next = scpThis;
      }
      scpTail = scpThis;
      scp = scp->next;
    }
    salpnew->score = scpHead;
    if (!salphead) {
      salphead = salpnew;
      salptail = salpnew;
      salptail->next = NULL;
    } else {
      salptail->next = salpnew;
      salptail = salpnew;
      salptail->next = NULL;
    }
    salp = salp->next;
  }
  return(salphead);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* make a DenseSeg Alignment-Set turning each diag into a separate alignment */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAlignPtr LIBCALL CddMSLDenDiagToMSLDenSeg(SeqAlignPtr salp)
{
  SeqAlignPtr         salpnew, salphead, salptail;
  DenseDiagPtr        ddp;
  DenseSegPtr         dsp;
  Int4                lastm, lasts;

  if (!salp) return(NULL);
  salphead = NULL;
  salptail = NULL;
  while (salp) {
    if (salp->segtype) {
      if (salp->segtype != SAS_DENDIAG) {
        CddSevError("CddMSLDenDiagToMSLDenSeg: Wrong segtype specified!");
      }
    }
    if (salp->dim) {
      if (salp->dim != 2) {
        CddSevError("CddMSLDenDiagToMSLDenSeg: Expect alignments of dimension 2!");
      }
    }
    ddp = salp->segs;
    lastm = -1;
    lasts = -1;
    while (ddp) {
      salpnew = SeqAlignNew();
      if (!salphead) salphead = salpnew;
      salpnew->type = SAT_PARTIAL;
      salpnew->segtype = SAS_DENSEG;
      salpnew->dim = 2;
      salpnew->score = ddp->scores;
      dsp = DenseSegNew();
      salpnew->segs = dsp;
      dsp->dim = 2;
      dsp->numseg = 1;
      dsp->ids = SeqIdDupList(ddp->id);
      dsp->starts = ddp->starts;
      dsp->lens = &(ddp->len);
      dsp->strands = ddp->strands;
      dsp->scores = ddp->scores;
      if (salptail) salptail->next = salpnew;
      salptail = salpnew;
      ddp = ddp->next;
    }
    salp = salp->next;
  }
  return(salphead);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* convert a dendiag pairwise alignment set into a multiple dendiag alignment*/
/* if possible - i.e. check that the number of segments and their extents on */
/* the common master is the same for all pairwise alignments                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAlignPtr LIBCALL CddMSLDenDiagToMULDenDiag(SeqAlignPtr salp)
{
  SeqAlignPtr     salpHead;
  SeqAlignPtr     salpNew;
  Int4            iNumSegs, iCnt, iDim;
  Int4            *iSegStart;
  Int4            *iSegLen;
  DenseDiagPtr    ddp, ddpNew = NULL, ddpTail = NULL;
  Boolean         bIsOk = TRUE;
  SeqIdPtr        sipNew, sipTail;

  if (!salp) return NULL;
  if (salp->dim && salp->dim != 2) {
    printf(" CddMSLDenDiagToMulDenDiag Warning: Can't convert alignment of dim!=2\n");
    return(salp);
  }
  iNumSegs = 0;
  ddp = (DenseDiagPtr) salp->segs;
  while (ddp) {
    iNumSegs++;
    ddp = ddp->next;
  }
  iSegStart = calloc(iNumSegs,sizeof(Int4));
  iSegLen = calloc(iNumSegs,sizeof(Int4));
  iCnt = 0;
  ddp = (DenseDiagPtr) salp->segs;
  while (ddp) {
    iSegStart[iCnt] = ddp->starts[0];
    iSegLen[iCnt] = ddp->len;    
    iCnt++;
    ddp = ddp->next;
  }
  iDim = 1;
  salpHead = salp;
  while (salpHead) {
    iDim++;
    if (iDim > 2) {
      ddp = (DenseDiagPtr) salpHead->segs;
      iCnt = 0;
      while (ddp) {
        if (iCnt > iNumSegs || ddp->starts[0] != iSegStart[iCnt] ||
            ddp->len != iSegLen[iCnt]) {
          bIsOk = FALSE;
          break;
        }
        iCnt++;  
        ddp = ddp->next;
      }
      if (iCnt != iNumSegs) {
        bIsOk = FALSE;
        break;
      }
    }
    salpHead = salpHead->next;
  }
  
  if (!bIsOk) {
    printf(" CddMSLDenDiagToMulDenDiag Warning: Can't convert alignment!\n");
    return(salp);
  }
  salpNew = SeqAlignNew();
  salpNew->type = salp->type;
  salpNew->segtype = salp->segtype;
  salpNew->dim = iDim;

  sipNew = NULL;
  salpHead = salp;
  while (salpHead) {
    ddp = (DenseDiagPtr)salpHead->segs;
    if (!sipNew) {
      sipNew = SeqIdDup(ddp->id);
      sipNew->next = SeqIdDup(ddp->id->next);
      sipTail = sipNew->next;
    } else {
      sipTail->next = SeqIdDup(ddp->id->next);
      sipTail = sipTail->next;
    }
    salpHead = salpHead->next;
  }

  salpHead = salp;
  iCnt = 0;
  while (salpHead) {
    ddp = (DenseDiagPtr) salpHead->segs;
    while (ddp) {
      if (!ddpNew) {
        ddpNew = (DenseDiagPtr) DenseDiagNew();
        ddpNew->dim = iDim;
        ddpNew->id = sipNew;
        ddpNew->starts = calloc(iDim,sizeof(Int4));
        ddpNew->starts[0]=ddp->starts[0];
        ddpNew->starts[1]=ddp->starts[1];
        ddpNew->len=ddp->len;
        if (!ddpTail) {
          salpNew->segs = ddpNew;
          ddpTail = ddpNew;
        } else {
          ddpTail->next = ddpNew;
          ddpTail = ddpTail->next;
        }
      } else {
        ddpNew->starts[iCnt+1]=ddp->starts[1];
      }
      if (ddpTail->next) {
        ddpTail = ddpTail->next;
        ddpNew = ddpTail;
      } else {
        ddpNew = NULL;
      }
      ddp = ddp->next;
      if (!ddp) {
        ddpNew = (DenseDiagPtr) salpNew->segs;
        ddpTail = ddpNew;
      }
    }
    iCnt++;
    salpHead = salpHead->next;
  }
  free(iSegLen);
  free(iSegStart);
  return(salpNew);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Calculate per column information content for a SeqAlign                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Nlm_FloatHi LIBCALL SeqAlignConservation(SeqAlignPtr salp, Nlm_FloatHi fract,
                                         BioseqPtr bsp_master, Boolean bHasConsensus, Int4 offset)
{
  Int4               i, c, a;
  Int4               qlength, nover = 0;
  Int4               alphabetSize = 26;
  Int4               nColumns;
  Int4               *ntypes;
  Nlm_FloatHi        **typefreq, sumfreq;
  DenseDiagPtr       ddp;
  BioseqPtr          bsp;
  SeqIdPtr           sip;
  SeqPortPtr         spp;
  Uint1Ptr           buffer;
  
  if (!salp) return(0.0);
  if (!bsp_master) return(0.0);
  qlength = bsp_master->length;
  ntypes = (Int4 *) MemNew(qlength*sizeof(Int4));
  for (i=0;i<qlength;i++) ntypes[i] = 0;
  typefreq = (Nlm_FloatHi**) MemNew(qlength * sizeof(Nlm_FloatHi *));
  for (i=0;i<qlength;i++) {
    typefreq[i] = (Nlm_FloatHi *)MemNew(alphabetSize * sizeof(Nlm_FloatHi));
    for (a=0;a<alphabetSize;a++) typefreq[i][a] = 0.0;
  }
  if (!bHasConsensus) {   /* count residues in the master/representative too */
    ddp = salp->segs;
    sip = ddp->id;
    bsp = bsp_master;
    buffer = MemNew((bsp->length)*sizeof(Uint1));
    spp = SeqPortNew(bsp, 0, bsp->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
    for (i=0; i<bsp->length;i++) buffer[i] = SeqPortGetResidue(spp);
    spp = SeqPortFree(spp);
    while (ddp) {
      for (c=ddp->starts[0]-offset;c<ddp->starts[0]-offset+ddp->len;c++) {
        if (buffer[c] >= 0 && buffer[c] < (Uint1) alphabetSize)
          typefreq[c][buffer[c]] += 1.0;
      }
      ddp = ddp->next;
    }
    MemFree(buffer);
  }  
  while (salp) {
    ddp = salp->segs;
    sip = ddp->id->next;
    bsp = BioseqLockById(sip);
    buffer = MemNew((bsp->length)*sizeof(Uint1));
    spp = SeqPortNew(bsp, 0, bsp->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
    for (i=0; i<bsp->length;i++) buffer[i] = SeqPortGetResidue(spp);
    spp = SeqPortFree(spp);
    BioseqUnlock(bsp);
    while (ddp) {
      for (c=ddp->starts[1];c<ddp->starts[1]+ddp->len;c++) {
        i = ddp->starts[0]-offset+c-ddp->starts[1];
        if (buffer[c] >= 0 && buffer[c] < (Uint1) alphabetSize)
	  typefreq[i][buffer[c]] += 1.0;
      }
      ddp = ddp->next;
    }
    MemFree(buffer);
    salp = salp->next;
  }
  for (i=0;i<qlength;i++) {
    sumfreq = 0.0;
    for (a=0;a<alphabetSize;a++) {
      if (typefreq[i][a] > 0.0) {
        ntypes[i]++;
	sumfreq += typefreq[i][a];
      }
    }
    if (sumfreq > 0.0) for (a=0;a<alphabetSize;a++) {
      typefreq[i][a] /= sumfreq;
    }
    for (a=0;a<alphabetSize;a++) {
      if (typefreq[i][a] >= fract) {
        nover++; break;
      } 
    }
  }
  for (i=0;i<qlength;i++) MemFree(typefreq[i]);
  MemFree(typefreq);
  MemFree(ntypes);
  return((Nlm_FloatHi)nover/(Nlm_FloatHi)qlength);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Calculate per column information content for a SeqAlign                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Nlm_FloatHiPtr LIBCALL SeqAlignInform(SeqAlignPtr salp, BioseqPtr bsp_master,
                                      Boolean bHasConsensus, Int4 offset)
{
  BLAST_ScoreBlkPtr  sbp;
  BLAST_ResFreqPtr   stdrfp;
  Nlm_FloatHiPtr     standardProb;
  Nlm_FloatHiPtr     Informativeness;
  Int2               iStatus;
  Int4               i, c, a;
  Int4               qlength;
  Int4               alphabetSize = 26;
  Nlm_FloatHi        posEpsilon = 0.0001;
  Nlm_FloatHi        QoverPest;
  Int4               *ntypes;
  Nlm_FloatHi        **typefreq, sumfreq;
  DenseDiagPtr       ddp;
  BioseqPtr          bsp;
  SeqIdPtr           sip;
  SeqPortPtr         spp;
  Uint1Ptr           buffer;
  Char               matrix[32];
  
  if (!salp) return(NULL);
  if (!bsp_master) return(NULL);
  qlength = bsp_master->length;
  Informativeness = MemNew(sizeof(Nlm_FloatHi) * qlength);
  sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa,1);
  sbp->read_in_matrix = TRUE;
  sbp->protein_alphabet = TRUE;
  sbp->posMatrix = NULL;
  sbp->number_of_contexts = 1;
  StrCpy(matrix,"BLOSUM62");
  iStatus = BlastScoreBlkMatFill(sbp,matrix);
  stdrfp = BlastResFreqNew(sbp);
  BlastResFreqStdComp(sbp,stdrfp); 
  standardProb = MemNew(alphabetSize * sizeof(Nlm_FloatHi));
  for(a = 0; a < alphabetSize; a++) standardProb[a] = stdrfp->prob[a];
  stdrfp = BlastResFreqDestruct(stdrfp);
  sbp = (BLAST_ScoreBlkPtr) BLAST_ScoreBlkDestruct(sbp);
  ntypes = (Int4 *) MemNew(qlength*sizeof(Int4));
  for (i=0;i<qlength;i++) ntypes[i] = 0;
  typefreq = (Nlm_FloatHi**) MemNew(qlength * sizeof(Nlm_FloatHi *));
  for (i=0;i<qlength;i++) {
    typefreq[i] = (Nlm_FloatHi *)MemNew(alphabetSize * sizeof(Nlm_FloatHi));
    for (a=0;a<alphabetSize;a++) typefreq[i][a] = 0.0;
  }
  if (!bHasConsensus) {   /* count residues in the master/representative too */
    ddp = salp->segs;
    sip = ddp->id;
    bsp = bsp_master;
    buffer = MemNew((bsp->length)*sizeof(Uint1));
    spp = SeqPortNew(bsp, 0, bsp->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
    for (i=0; i<bsp->length;i++) buffer[i] = SeqPortGetResidue(spp);
    spp = SeqPortFree(spp);
    while (ddp) {
      for (c=ddp->starts[0]-offset;c<ddp->starts[0]-offset+ddp->len;c++) {
        if (buffer[c] < (Uint1) alphabetSize)
          typefreq[c][buffer[c]] += 1.0;
      }
      ddp = ddp->next;
    }
    MemFree(buffer);
  }  
  while (salp) {
    ddp = salp->segs;
    sip = ddp->id->next;
    bsp = BioseqLockById(sip);
    buffer = MemNew((bsp->length)*sizeof(Uint1));
    spp = SeqPortNew(bsp, 0, bsp->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
    for (i=0; i<bsp->length;i++) buffer[i] = SeqPortGetResidue(spp);
    spp = SeqPortFree(spp);
    BioseqUnlock(bsp);
    while (ddp) {
      for (c=ddp->starts[1];c<ddp->starts[1]+ddp->len;c++) {
        i = ddp->starts[0]-offset+c-ddp->starts[1];
        if (buffer[c] < (Uint1) alphabetSize)
	  typefreq[i][buffer[c]] += 1.0;
      }
      ddp = ddp->next;
    }
    MemFree(buffer);
    salp = salp->next;
  }
  for (i=0;i<qlength;i++) {
    sumfreq = 0.0;
    for (a=0;a<alphabetSize;a++) {
      if (typefreq[i][a] > 0.0) {
        ntypes[i]++;
	sumfreq += typefreq[i][a];
      }
    }
    if (sumfreq > 0.0) for (a=0;a<alphabetSize;a++) {
      typefreq[i][a] /= sumfreq; 
    }
  }

  for (c=0;c<qlength;c++) {
    Informativeness[c] = 0.0;
    for (a=0;a<alphabetSize;a++) {
      if (standardProb[a] > posEpsilon) {
        QoverPest = typefreq[c][a] / standardProb[a];
        if (QoverPest > posEpsilon) {
	  Informativeness[c] += typefreq[c][a] * log(QoverPest) / NCBIMATH_LN2;        
        }
      }
    }
  }
  for (i=0;i<qlength;i++) MemFree(typefreq[i]);
  MemFree(typefreq);
  MemFree(ntypes);
  MemFree(standardProb);
  return(Informativeness);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Calculate per column information content for a CDD from the seqalign      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int4 ** LIBCALL CddCountResTypes(CddPtr pcdd, Int4 *ncols)
{
  BioseqSetPtr       bssp;
  Boolean            bHasConsensus = CddHasConsensus(pcdd);
  Int4               offset, qlength, i, j, c;
  Int4             **ResFreqTable;
  SeqAlignPtr        salp;
  DenseDiagPtr       ddp;
  SeqIdPtr           sip;
  BioseqPtr          bsp;
  Uint1             *buffer;
  SeqPortPtr         spp;

  if (!pcdd) return(NULL);
  if (!pcdd->trunc_master) return(NULL);
  bssp = pcdd->sequences->data.ptrvalue;
  offset = pcdd->profile_range->from;
  qlength = pcdd->trunc_master->length;
  *ncols = qlength;  
  ResFreqTable = MemNew(qlength*sizeof(Int4 *));
  for (i=0;i<qlength;i++) {
    ResFreqTable[i] = MemNew(26*sizeof(Int4));
    for (j=0;j<26;j++) ResFreqTable[i][j] = 0;
  }
  salp = pcdd->seqannot->data;
  if (!bHasConsensus) {   /* count residues in the master/representative too */
    ddp = salp->segs;
    sip = ddp->id;
    bsp = CddRetrieveBioseqById(sip,bssp->seq_set);
    buffer = MemNew((bsp->length)*sizeof(Uint1));
    spp = SeqPortNew(bsp, 0, bsp->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
    for (i=0; i<bsp->length;i++) buffer[i] = SeqPortGetResidue(spp);
    spp = SeqPortFree(spp);
    while (ddp) {
      for (c=ddp->starts[0];c<ddp->starts[0]+ddp->len;c++) {
        ResFreqTable[c-offset][buffer[c]]++;
      }
      ddp = ddp->next;
    }
    MemFree(buffer);
  }  
  while (salp) {
    ddp = salp->segs;
    sip = ddp->id->next;
    bsp = CddRetrieveBioseqById(sip,bssp->seq_set);
    buffer = MemNew((bsp->length)*sizeof(Uint1));
    spp = SeqPortNew(bsp, 0, bsp->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
    for (i=0; i<bsp->length;i++) buffer[i] = SeqPortGetResidue(spp);
    spp = SeqPortFree(spp);
    while (ddp) {
      for (c=ddp->starts[1];c<ddp->starts[1]+ddp->len;c++) {
        i = ddp->starts[0]-offset+c-ddp->starts[1];
        ResFreqTable[i][buffer[c]]++;
      }
      ddp = ddp->next;
    }
    MemFree(buffer);
    salp = salp->next;
  }
  return(ResFreqTable);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Calculate per column information content for a CDD from the seqalign      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Nlm_FloatHiPtr LIBCALL CddAlignInform(CddPtr pcdd, Nlm_FloatHi * Niobs)
{
  Int4               offset;
  Boolean            bHasConsensus = CddHasConsensus(pcdd);
  BLAST_ScoreBlkPtr  sbp;
  BLAST_ResFreqPtr   stdrfp;
  Nlm_FloatHiPtr     standardProb;
  Nlm_FloatHiPtr     Informativeness;
  Int2               iStatus;
  Int4               i, c, a;
  Int4               qlength;
  Int4               alphabetSize = 26;
  Nlm_FloatHi        posEpsilon = 0.0001;
  Nlm_FloatHi        QoverPest;
  Int4               *ntypes;
  Nlm_FloatHi        **typefreq, sumfreq;
  SeqAlignPtr        salp;
  DenseDiagPtr       ddp;
  BioseqPtr          bsp;
  SeqIdPtr           sip;
  BioseqSetPtr       bssp;
  SeqPortPtr         spp;
  Uint1Ptr           buffer;
  
  bssp = pcdd->sequences->data.ptrvalue;
  offset = pcdd->profile_range->from;
  if (!pcdd) return(NULL);
  if (!pcdd->trunc_master) return(NULL);
  qlength = pcdd->trunc_master->length;
  Informativeness = MemNew(sizeof(Nlm_FloatHi) * qlength);
  sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa,1);
  sbp->read_in_matrix = TRUE;
  sbp->protein_alphabet = TRUE;
  sbp->posMatrix = NULL;
  sbp->number_of_contexts = 1;
  iStatus = BlastScoreBlkMatFill(sbp,"BLOSUM62");
  stdrfp = BlastResFreqNew(sbp);
  BlastResFreqStdComp(sbp,stdrfp); 
  standardProb = MemNew(alphabetSize * sizeof(Nlm_FloatHi));
  for(a = 0; a < alphabetSize; a++) standardProb[a] = stdrfp->prob[a];
  stdrfp = BlastResFreqDestruct(stdrfp);
  ntypes = (Int4 *) MemNew(qlength*sizeof(Int4));
  for (i=0;i<qlength;i++) ntypes[i] = 0;
  typefreq = (Nlm_FloatHi**) MemNew(qlength * sizeof(Nlm_FloatHi *));
  for (i=0;i<qlength;i++) {
    typefreq[i] = (Nlm_FloatHi *)MemNew(alphabetSize * sizeof(Nlm_FloatHi));
    for (a=0;a<alphabetSize;a++) typefreq[i][a] = 0.0;
  }
  salp = pcdd->seqannot->data;
  if (!bHasConsensus) {   /* count residues in the master/representative too */
    ddp = salp->segs;
    sip = ddp->id;
    bsp = CddRetrieveBioseqById(sip,bssp->seq_set);
    buffer = MemNew((bsp->length)*sizeof(Uint1));
    spp = SeqPortNew(bsp, 0, bsp->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
    for (i=0; i<bsp->length;i++) buffer[i] = SeqPortGetResidue(spp);
    spp = SeqPortFree(spp);
    while (ddp) {
      for (c=ddp->starts[0];c<ddp->starts[0]+ddp->len;c++) {
        typefreq[c-offset][buffer[c]] += 1.0;
      }
      ddp = ddp->next;
    }
    MemFree(buffer);
  }  
  while (salp) {
    ddp = salp->segs;
    sip = ddp->id->next;
    bsp = CddRetrieveBioseqById(sip,bssp->seq_set);
    buffer = MemNew((bsp->length)*sizeof(Uint1));
    spp = SeqPortNew(bsp, 0, bsp->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
    for (i=0; i<bsp->length;i++) buffer[i] = SeqPortGetResidue(spp);
    spp = SeqPortFree(spp);
    while (ddp) {
      for (c=ddp->starts[1];c<ddp->starts[1]+ddp->len;c++) {
        i = ddp->starts[0]-offset+c-ddp->starts[1];
        typefreq[i][buffer[c]] += 1.0;
      }
      ddp = ddp->next;
    }
    MemFree(buffer);
    salp = salp->next;
  }
  for (i=0;i<qlength;i++) {
    sumfreq = 0.0;
    for (a=0;a<alphabetSize;a++) {
      if (typefreq[i][a] > 0.0) {
        ntypes[i]++;
	sumfreq += typefreq[i][a];
      }
    }
    if (sumfreq > 0.0) for (a=0;a<alphabetSize;a++) {
      typefreq[i][a] /= sumfreq; 
    }
  }

  for (c=0;c<qlength;c++) {
    Informativeness[c] = 0.0;
    for (a=0;a<alphabetSize;a++) {
      if (standardProb[a] > posEpsilon) {
        QoverPest = typefreq[c][a] / standardProb[a];
        if (QoverPest > posEpsilon) {
	  Informativeness[c] += typefreq[c][a] * log(QoverPest) / NCBIMATH_LN2;        
        }
      }
    }
  }

  *Niobs = 0.0;
  for (i=0;i<qlength;i++) *Niobs += (Nlm_FloatHi) ntypes[i];
  *Niobs /= (Nlm_FloatHi) qlength;

  for (i=0;i<qlength;i++) MemFree(typefreq[i]);
  MemFree(typefreq);
  MemFree(ntypes);
  MemFree(standardProb);
  return(Informativeness);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Calculate per column information content for a CDD pssm                   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Nlm_FloatHiPtr LIBCALL CddPssmInform(CddPtr pcdd)
{
  BLAST_ScoreBlkPtr  sbp;
  BLAST_ResFreqPtr   stdrfp;
  Nlm_FloatHiPtr     standardProb;
  Nlm_FloatHiPtr     Informativeness;
  Nlm_FloatHi        thisposFreq;
  Int2               iStatus;
  Int4               c, a;
  Int4               qlength;
  Int4               alphabetSize = 26;
  MatrixPtr          posfreq;
  ValNodePtr         vnp;
  Nlm_FloatHi        posEpsilon = 0.0001;
  Nlm_FloatHi        QoverPest;

  if (!pcdd) return(NULL);
  if (!pcdd->trunc_master) return(NULL);
  qlength = pcdd->trunc_master->length;
  Informativeness = MemNew(sizeof(Nlm_FloatHi) * qlength);
  sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa,1);
  sbp->read_in_matrix = TRUE;
  sbp->protein_alphabet = TRUE;
  sbp->posMatrix = NULL;
  sbp->number_of_contexts = 1;
  iStatus = BlastScoreBlkMatFill(sbp,"BLOSUM62");
  stdrfp = BlastResFreqNew(sbp);
  BlastResFreqStdComp(sbp,stdrfp); 
  standardProb = MemNew(alphabetSize * sizeof(Nlm_FloatHi));
  for(a = 0; a < alphabetSize; a++) standardProb[a] = stdrfp->prob[a];
  stdrfp = BlastResFreqDestruct(stdrfp);
  posfreq = pcdd->posfreq;
  if (posfreq->nrows != alphabetSize || posfreq->ncolumns != qlength) {
    CddSevError("posfreq matrix size mismatch in CddPssmInform");  
  }
  vnp = posfreq->columns;
  for (c=0;c<qlength;c++) {
    Informativeness[c] = 0.0;
    for (a=0;a<alphabetSize;a++) {
      if (standardProb[a] > posEpsilon) {
        thisposFreq = (Nlm_FloatHi) vnp->data.intvalue / (Nlm_FloatHi) posfreq->scale_factor;
        QoverPest = thisposFreq / standardProb[a];
        if (QoverPest > posEpsilon) {
	  Informativeness[c] += thisposFreq * log(QoverPest) / NCBIMATH_LN2;        
        }
      }
      vnp = vnp->next;
    }
  }
  MemFree(standardProb);
  return(Informativeness);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Calculate per column information content for a frequency table            */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Nlm_FloatHiPtr LIBCALL CddPosFreqInform(Nlm_FloatHi **posFreq, Int4 ncol, Int4 nrow)
{
  BLAST_ScoreBlkPtr  sbp;
  BLAST_ResFreqPtr   stdrfp;
  Nlm_FloatHiPtr     standardProb;
  Nlm_FloatHiPtr     Informativeness;
  Nlm_FloatHi        thisposFreq;
  Int2               iStatus;
  Int4               c, a;
  Int4               qlength;
  Int4               alphabetSize = 26;
  Nlm_FloatHi        posEpsilon = 0.0001;
  Nlm_FloatHi        QoverPest;

  if (!posFreq) return(NULL);
  qlength = ncol;
  if (nrow != alphabetSize) return(NULL);
  Informativeness = MemNew(sizeof(Nlm_FloatHi) * qlength);
  sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa,1);
  sbp->read_in_matrix = TRUE;
  sbp->protein_alphabet = TRUE;
  sbp->posMatrix = NULL;
  sbp->number_of_contexts = 1;
  iStatus = BlastScoreBlkMatFill(sbp,"BLOSUM62");
  stdrfp = BlastResFreqNew(sbp);
  BlastResFreqStdComp(sbp,stdrfp); 
  standardProb = MemNew(alphabetSize * sizeof(Nlm_FloatHi));
  for(a = 0; a < alphabetSize; a++) standardProb[a] = stdrfp->prob[a];
  stdrfp = BlastResFreqDestruct(stdrfp);

  for (c=0;c<qlength;c++) {
    Informativeness[c] = 0.0;
    for (a=0;a<alphabetSize;a++) {
      if (standardProb[a] > posEpsilon) {
        thisposFreq = posFreq[c][a];
        QoverPest = thisposFreq / standardProb[a];
        if (QoverPest > posEpsilon) {
	  Informativeness[c] += thisposFreq * log(QoverPest) / NCBIMATH_LN2;        
        }
      }
    }
  }
  MemFree(standardProb);
  return(Informativeness);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Generation of position-specific scoring matrices for database scanning    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddDenDiagCposComputation(SeqAlignPtr listOfSeqAligns, BioseqPtr bsp,
                                       BioseqPtr bspF, CddPtr pcdd, Int4 pseudoCnt)
{
    Int4                numalign, numseq;    /*number of alignments and seqs */
    posSearchItems      *posSearch;          /*holds position-specific info  */
    compactSearchItems  *compactSearch = NULL;
    BLAST_ScoreBlkPtr   sbp;
    SeqLocPtr           private_slp = NULL;
    SeqPortPtr          spp = NULL;
    Uint1Ptr            query_seq, query_seq_start;
    Uint1               residue;
    Int4                index, a, KarlinReturn, array_size, iNew;
    Nlm_FloatHiPtr      lambda, K, H;
    Int4Ptr             open, extend;
    BLAST_ResFreqPtr    stdrfp;
    Int2                iStatus;
    ValNodePtr          error_return = NULL;
    ValNodePtr          ColumnHead;
    ValNodePtr          newRow, RowHead;
    ValNodePtr          newLetter, LetterHead;
    SeqCodeTablePtr     sctp;
    Char                ckptFileName[PATH_MAX];
    Char                cseqFileName[PATH_MAX];
    FILE                *fp;
    Boolean             bHasConsensus;
    CharPtr             cAccession;
    
    bHasConsensus = CddHasConsensus(pcdd);
    
    numalign = CddCountDenDiagSeqAligns(listOfSeqAligns, &numseq);
    posSearch = (posSearchItems *) MemNew(1*sizeof(posSearchItems));
    sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa,1);
    sbp->read_in_matrix = TRUE;
    sbp->protein_alphabet = TRUE;
    sbp->posMatrix = NULL;
    sbp->number_of_contexts = 1;
    iStatus = BlastScoreBlkMatFill(sbp,"BLOSUM62");
    compactSearch = compactSearchNew(compactSearch);
    compactSearch->qlength = bsp->length;
    compactSearch->alphabetSize = sbp->alphabet_size;
    compactSearch->matrix = sbp->matrix;
    compactSearch->gapped_calculation = TRUE;
    compactSearch->pseudoCountConst = pseudoCnt;
    compactSearch->ethresh = 0.001;

/*---------------------------------------------------------------------------*/
/* get the query sequence                                                    */
/*---------------------------------------------------------------------------*/
    private_slp = SeqLocIntNew(0, bspF->length-1 , Seq_strand_plus, SeqIdFindBest(bspF->id, SEQID_GI));
		spp = SeqPortNewByLoc(private_slp, Seq_code_ncbistdaa);
    SeqPortSet_do_virtual(spp, TRUE);
		query_seq_start = (Uint1Ptr) MemNew(((bspF->length)+2)*sizeof(Char));
		query_seq_start[0] = NULLB;
		query_seq = query_seq_start+1;
    index=0;
    while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
      if (IS_residue(residue)) {
				query_seq[index] = residue;
				index++;
			}
		}
		query_seq[index] = NULLB;
		spp = SeqPortFree(spp);
		private_slp = SeqLocFree(private_slp);
    compactSearch->query = query_seq;

    BlastScoreBlkFill(sbp,(CharPtr) compactSearch->query,compactSearch->qlength,0);

    sbp->kbp_gap_std[0] = BlastKarlinBlkCreate();
    KarlinReturn = BlastKarlinBlkGappedCalc(sbp->kbp_gap_std[0],11,1,sbp->name,&error_return);
    if (1 == KarlinReturn) {
      BlastErrorPrint(error_return);
    }
    sbp->kbp_gap_psi[0] = BlastKarlinBlkCreate();
    KarlinReturn = BlastKarlinBlkGappedCalc(sbp->kbp_gap_psi[0],11,1,sbp->name,&error_return);
    if (1 == KarlinReturn) {
      BlastErrorPrint(error_return);
    }

    array_size = BlastKarlinGetMatrixValues(sbp->name,&open,&extend,&lambda,&K,&H,NULL);
    if (array_size > 0) {
      for (index = 0; index < array_size; index++) {
        if (open[index] == INT2_MAX && extend[index] == INT2_MAX) {
          sbp->kbp_ideal = BlastKarlinBlkCreate();
          sbp->kbp_ideal->Lambda = lambda[index];
          sbp->kbp_ideal->K = K[index];
          sbp->kbp_ideal->H = H[index];
        }
      }
      MemFree(open);
      MemFree(extend);
      MemFree(lambda);
      MemFree(K);
      MemFree(H);
    }
    if (sbp->kbp_ideal == NULL) {
      sbp->kbp_ideal = BlastKarlinBlkStandardCalcEx(sbp);
    }
    compactSearch->lambda       = sbp->kbp_gap_std[0]->Lambda;
    compactSearch->kbp_std      = sbp->kbp_std;
    compactSearch->kbp_psi      = sbp->kbp_psi;
    compactSearch->kbp_gap_std  = sbp->kbp_gap_std;
    compactSearch->kbp_gap_psi  = sbp->kbp_gap_psi;
    compactSearch->lambda_ideal = sbp->kbp_ideal->Lambda;
    compactSearch->K_ideal      = sbp->kbp_ideal->K;

    stdrfp = BlastResFreqNew(sbp);
    BlastResFreqStdComp(sbp,stdrfp); 
    compactSearch->standardProb = MemNew(compactSearch->alphabetSize * sizeof(Nlm_FloatHi));
    for(a = 0; a < compactSearch->alphabetSize; a++)
      compactSearch->standardProb[a] = stdrfp->prob[a];
    stdrfp = BlastResFreqDestruct(stdrfp);
    posSearch->posInformation = NULL;
/*---------------------------------------------------------------------------*/
/* numseq is replaced with numalign (last argument) - each alignment is a    */
/* set of diags and represents an independent sequence/domain                */
/*---------------------------------------------------------------------------*/
    posAllocateMemory(posSearch, compactSearch->alphabetSize, compactSearch->qlength, numalign);
    CddfindDenseDiagThreshSequences(posSearch, listOfSeqAligns, numalign, numalign);
    sbp->kbp     = sbp->kbp_psi;
    sbp->kbp_gap = sbp->kbp_gap_psi;
    CddposInitializeInformation(posSearch,sbp,compactSearch);
    CddposDenseDiagDemographics(posSearch, compactSearch, listOfSeqAligns);
    posPurgeMatches(posSearch, compactSearch);
/*---------------------------------------------------------------------------*/
/* remove the consensus from the CposComputation if present in the CDD       */
/*---------------------------------------------------------------------------*/
    if (bHasConsensus) posCancel(posSearch,compactSearch,0,0,0,compactSearch->qlength);
    posComputeExtents(posSearch, compactSearch);
    posComputeSequenceWeights(posSearch, compactSearch, 1.0);
    posCheckWeights(posSearch, compactSearch);
    posSearch->posFreqs = (Nlm_FloatHi **) posComputePseudoFreqs(posSearch, compactSearch, TRUE);
    CddposFreqsToMatrix(posSearch,compactSearch);
    posScaling(posSearch, compactSearch);

    sctp = SeqCodeTableFind(Seq_code_ncbistdaa);
    LetterHead = NULL;
    for (a=0;a<compactSearch->alphabetSize;a++) {
      newLetter = ValNodeNew(NULL);
      newLetter->data.ptrvalue = MemNew(2*sizeof(Char));
      Nlm_StrNCpy(newLetter->data.ptrvalue,&(sctp->letters[a]),1);
      ValNodeLink(&(LetterHead),newLetter);
    }

    pcdd->posfreq = (MatrixPtr) MatrixNew();
    pcdd->posfreq->ncolumns = compactSearch->qlength;
    pcdd->posfreq->nrows = compactSearch->alphabetSize;
    pcdd->posfreq->scale_factor = POSFREQ_SCALE;
    pcdd->posfreq->row_labels = LetterHead;    
    ColumnHead = NULL;
    for (index = 0; index<compactSearch->qlength;index++) {
      RowHead = NULL;
      for (a=0;a<compactSearch->alphabetSize;a++) {
         newRow = ValNodeNew(NULL);
         iNew = (Int4) (0.5 + (Nlm_FloatHi) pcdd->posfreq->scale_factor * posSearch->posFreqs[index][a]);
         newRow->data.intvalue = iNew;
           ValNodeLink(&(ColumnHead),newRow);
      }
    }
    pcdd->posfreq->columns = ColumnHead;

    pcdd->scoremat = (MatrixPtr) MatrixNew();
    pcdd->scoremat->ncolumns = compactSearch->qlength;
    pcdd->scoremat->nrows = compactSearch->alphabetSize;
    pcdd->scoremat->scale_factor = 1;
    pcdd->scoremat->row_labels = LetterHead;    
    ColumnHead = NULL;
    for (index = 0; index<compactSearch->qlength;index++) {
      RowHead = NULL;
      for (a=0;a<compactSearch->alphabetSize;a++) {
         newRow = ValNodeNew(NULL);
         iNew = (Int4) posSearch->posMatrix[index][a];
         newRow->data.intvalue = iNew;
           ValNodeLink(&(ColumnHead),newRow);
      }
    }
    pcdd->scoremat->columns = ColumnHead;

/*---------------------------------------------------------------------------*/
/* Construct name for checkpoint file                                        */
/*---------------------------------------------------------------------------*/
    cAccession = CddGetAccession(pcdd);
    strcpy(ckptFileName,cAccession);
    strcat(ckptFileName,CKPTEXT);
    CddposTakeCheckpoint(posSearch, compactSearch, ckptFileName, &error_return);
    strcpy(cseqFileName,cAccession);
    strcat(cseqFileName,CSEQEXT);
    MemFree(cAccession);

    fp = FileOpen(cseqFileName, "w");
    if (NULL == fp) {
      BlastConstructErrorMessage("CddDenDiagCposComputation", "Could not open fasta file", 1, &error_return);
    }
    BioseqToFasta(bsp,fp,FALSE);
    FileClose(fp);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Generation of position-specific scoring matrices for database scanning    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddCposComputation(SeqAlignPtr listOfSeqAligns, BioseqPtr bsp,
                                CddPtr pcdd)
{
    Int4                numalign, numseq;    /*number of alignments and seqs */
    posSearchItems      *posSearch;          /*holds position-specific info  */
    compactSearchItems  *compactSearch = NULL;
    BLAST_ScoreBlkPtr   sbp;
    SeqLocPtr           private_slp = NULL;
    SeqPortPtr          spp = NULL;
    Uint1Ptr            query_seq, query_seq_start;
    Uint1               residue;
    Int4                index, a, KarlinReturn, array_size, iNew;
    Nlm_FloatHiPtr      lambda, K, H;
    Int4Ptr             open, extend;
    BLAST_ResFreqPtr    stdrfp;
    Int2                iStatus;
    ValNodePtr          error_return = NULL;
    ValNodePtr          ColumnHead;
    ValNodePtr          newRow, RowHead;
    ValNodePtr          newLetter, LetterHead;
    SeqCodeTablePtr     sctp;
    Char                ckptFileName[PATH_MAX];
    Char                cseqFileName[PATH_MAX];
    FILE                *fp;
    CharPtr             cAccession;

    numalign = CddCountSeqAligns(listOfSeqAligns, &numseq);
    posSearch = (posSearchItems *) MemNew(1*sizeof(posSearchItems));
    sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa,1);
    sbp->read_in_matrix = TRUE;
    sbp->protein_alphabet = TRUE;
    sbp->posMatrix = NULL;
    sbp->number_of_contexts = 1;
    iStatus = BlastScoreBlkMatFill(sbp,"BLOSUM62");
    compactSearch = compactSearchNew(compactSearch);
    compactSearch->qlength = bsp->length;
    compactSearch->alphabetSize = sbp->alphabet_size;
    compactSearch->matrix = sbp->matrix;
    compactSearch->gapped_calculation = TRUE;
    compactSearch->pseudoCountConst = 10;
    compactSearch->ethresh = 0.001;

/*---------------------------------------------------------------------------*/
/* get the query sequence                                                    */
/*---------------------------------------------------------------------------*/
    private_slp = SeqLocIntNew(0, bsp->length-1 , Seq_strand_plus, SeqIdFindBest(bsp->id, SEQID_GI));
		spp = SeqPortNewByLoc(private_slp, Seq_code_ncbistdaa);
    SeqPortSet_do_virtual(spp, TRUE);
		query_seq_start = (Uint1Ptr) MemNew(((bsp->length)+2)*sizeof(Char));
		query_seq_start[0] = NULLB;
		query_seq = query_seq_start+1;
    index=0;
    while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
      if (IS_residue(residue)) {
				query_seq[index] = residue;
				index++;
			}
		}
		query_seq[index] = NULLB;
		spp = SeqPortFree(spp);
		private_slp = SeqLocFree(private_slp);
    compactSearch->query = query_seq;

    BlastScoreBlkFill(sbp,(CharPtr) compactSearch->query,compactSearch->qlength,0);

    sbp->kbp_gap_std[0] = BlastKarlinBlkCreate();
    KarlinReturn = BlastKarlinBlkGappedCalc(sbp->kbp_gap_std[0],11,1,sbp->name,&error_return);
    if (1 == KarlinReturn) {
      BlastErrorPrint(error_return);
    }
    sbp->kbp_gap_psi[0] = BlastKarlinBlkCreate();
    KarlinReturn = BlastKarlinBlkGappedCalc(sbp->kbp_gap_psi[0],11,1,sbp->name,&error_return);
    if (1 == KarlinReturn) {
      BlastErrorPrint(error_return);
    }

    array_size = BlastKarlinGetMatrixValues(sbp->name,&open,&extend,&lambda,&K,&H,NULL);
    if (array_size > 0) {
      for (index = 0; index < array_size; index++) {
        if (open[index] == INT2_MAX && extend[index] == INT2_MAX) {
          sbp->kbp_ideal = BlastKarlinBlkCreate();
          sbp->kbp_ideal->Lambda = lambda[index];
          sbp->kbp_ideal->K = K[index];
          sbp->kbp_ideal->H = H[index];
        }
      }
      MemFree(open);
      MemFree(extend);
      MemFree(lambda);
      MemFree(K);
      MemFree(H);
    }
    if (sbp->kbp_ideal == NULL) {
      sbp->kbp_ideal = BlastKarlinBlkStandardCalcEx(sbp);
    }
    compactSearch->lambda       = sbp->kbp_gap_std[0]->Lambda;
    compactSearch->kbp_std      = sbp->kbp_std;
    compactSearch->kbp_psi      = sbp->kbp_psi;
    compactSearch->kbp_gap_std  = sbp->kbp_gap_std;
    compactSearch->kbp_gap_psi  = sbp->kbp_gap_psi;
    compactSearch->lambda_ideal = sbp->kbp_ideal->Lambda;
    compactSearch->K_ideal      = sbp->kbp_ideal->K;

    stdrfp = BlastResFreqNew(sbp);
    BlastResFreqStdComp(sbp,stdrfp); 
    compactSearch->standardProb = MemNew(compactSearch->alphabetSize * sizeof(Nlm_FloatHi));
    for(a = 0; a < compactSearch->alphabetSize; a++)
      compactSearch->standardProb[a] = stdrfp->prob[a];
    stdrfp = BlastResFreqDestruct(stdrfp);
    posSearch->posInformation = NULL;
/*---------------------------------------------------------------------------*/
/* numseq is replaced with numalign (last argument) - each alignment is a    */
/* single segment and represents a separate sequence                         */
/*---------------------------------------------------------------------------*/
    posAllocateMemory(posSearch, compactSearch->alphabetSize, compactSearch->qlength, numalign);
    CddfindThreshSequences(posSearch, listOfSeqAligns, numalign, numalign);
    sbp->kbp     = sbp->kbp_psi;
    sbp->kbp_gap = sbp->kbp_gap_psi;
    CddposInitializeInformation(posSearch,sbp,compactSearch);
    CddposDemographics(posSearch, compactSearch, listOfSeqAligns);
    posPurgeMatches(posSearch, compactSearch);
    posComputeExtents(posSearch, compactSearch);
    posComputeSequenceWeights(posSearch, compactSearch, 1.0);
    posCheckWeights(posSearch, compactSearch);
    posSearch->posFreqs = (Nlm_FloatHi **) posComputePseudoFreqs(posSearch, compactSearch, TRUE);
    CddposFreqsToMatrix(posSearch,compactSearch);
    posScaling(posSearch, compactSearch);

    sctp = SeqCodeTableFind(Seq_code_ncbistdaa);
    LetterHead = NULL;
    for (a=0;a<compactSearch->alphabetSize;a++) {
      newLetter = ValNodeNew(NULL);
      newLetter->data.ptrvalue = MemNew(2*sizeof(Char));
      Nlm_StrNCpy(newLetter->data.ptrvalue,&(sctp->letters[a]),1);
      ValNodeLink(&(LetterHead),newLetter);
    }

    pcdd->posfreq = (MatrixPtr) MatrixNew();
    pcdd->posfreq->ncolumns = compactSearch->qlength;
    pcdd->posfreq->nrows = compactSearch->alphabetSize;
    pcdd->posfreq->scale_factor = POSFREQ_SCALE;
    pcdd->posfreq->row_labels = LetterHead;    
    ColumnHead = NULL;
    for (index = 0; index<compactSearch->qlength;index++) {
      RowHead = NULL;
      for (a=0;a<compactSearch->alphabetSize;a++) {
         newRow = ValNodeNew(NULL);
         iNew = (Int4) (0.5 + (Nlm_FloatHi) pcdd->posfreq->scale_factor * posSearch->posFreqs[index][a]);
         newRow->data.intvalue = iNew;
           ValNodeLink(&(ColumnHead),newRow);
      }
    }
    pcdd->posfreq->columns = ColumnHead;

    pcdd->scoremat = (MatrixPtr) MatrixNew();
    pcdd->scoremat->ncolumns = compactSearch->qlength;
    pcdd->scoremat->nrows = compactSearch->alphabetSize;
    pcdd->scoremat->scale_factor = 1;
    pcdd->scoremat->row_labels = LetterHead;    
    ColumnHead = NULL;
    for (index = 0; index<compactSearch->qlength;index++) {
      RowHead = NULL;
      for (a=0;a<compactSearch->alphabetSize;a++) {
         newRow = ValNodeNew(NULL);
         iNew = (Int4) posSearch->posMatrix[index][a];
         newRow->data.intvalue = iNew;
           ValNodeLink(&(ColumnHead),newRow);
      }
    }
    pcdd->scoremat->columns = ColumnHead;

/*---------------------------------------------------------------------------*/
/* Construct name for checkpoint file                                        */
/*---------------------------------------------------------------------------*/
    cAccession = CddGetAccession(pcdd);
    strcpy(ckptFileName,cAccession);
    strcat(ckptFileName,CKPTEXT);

    CddposTakeCheckpoint(posSearch, compactSearch, ckptFileName, &error_return);
    strcpy(cseqFileName,cAccession);
    strcat(cseqFileName,CSEQEXT);
  
    MemFree(cAccession);
  
    fp = FileOpen(cseqFileName, "w");
    if (NULL == fp) {
      BlastConstructErrorMessage("CddCposComputation", "Could not open fasta file", 1, &error_return);
    }
    BioseqToFasta(bsp,fp,FALSE);
    FileClose(fp);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Count the number of seqAligns in a list (returned) and count the number   */
/* of distinct target sequences represented (passed back in numSequences);   */
/* Important assumption: All SeqAligns with  the same target sequence are    */
/* consecutive in the list                                                   */
/* ONLY works for lists of Seqaligns containing DenseSegs!!!                 */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int4 LIBCALL CddCountSeqAligns(SeqAlignPtr listOfSeqAligns, Int4 * numSequences)
{
  SeqAlignPtr curSeqAlign, prevSeqAlign;
  Int4        seqAlignCounter;
  DenseSegPtr curSegs;
  SeqIdPtr    curId, prevId;

  seqAlignCounter = 0;
  *numSequences = 0;
  curSeqAlign = listOfSeqAligns;
  prevSeqAlign = NULL;
  while (NULL != curSeqAlign) {
    curSegs = (DenseSegPtr) curSeqAlign->segs;
    if(curSegs->ids == NULL) break;
    curId = curSegs->ids->next; 
    seqAlignCounter++;
    if ((NULL == prevSeqAlign) || (!(SeqIdMatch(curId, prevId))))
      (*numSequences)++;
    prevSeqAlign = curSeqAlign;
    prevId = curId;
    curSeqAlign = curSeqAlign->next;
  }
  return(seqAlignCounter);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Count the number of seqAligns in a list (returned) and count the number   */
/* of distinct target sequences represented (passed back in numSequences);   */
/* Important assumption: All SeqAligns with  the same target sequence are    */
/* consecutive in the list                                                   */
/* ONLY works for lists of Seqaligns containing DenseDiags!!!                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int4 LIBCALL CddCountDenDiagSeqAligns(SeqAlignPtr listOfSeqAligns, Int4 * numSequences)
{
  SeqAlignPtr  curSeqAlign;
  Int4         seqAlignCounter;
  DenseDiagPtr curSegs;

  seqAlignCounter = 0;
  *numSequences = 0;
  curSeqAlign = listOfSeqAligns;
  while (NULL != curSeqAlign) {
    curSegs = (DenseDiagPtr) curSeqAlign->segs;
    if(curSegs->id == NULL) break;
    seqAlignCounter++;
    (*numSequences)++;
    curSeqAlign = curSeqAlign->next;
  }
  return(seqAlignCounter);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* assign the range of the master sequence involved in alignments            */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddAssignProfileRange(CddPtr pcdd, SeqIdPtr sip)
{
  SeqAlignPtr  salp;
  DenseDiagPtr ddp;
  SeqIntPtr    sintp;
  Int4         iMax = -10000000;
  Int4         iMin =  10000000;

  sintp = (SeqIntPtr) SeqIntNew();

  salp = (SeqAlignPtr) pcdd->seqannot->data;

  while (salp) {
    ddp = (DenseDiagPtr) salp->segs;
    while (ddp) {
      if (ddp->starts[0] < iMin) iMin = ddp->starts[0];
      if ((ddp->starts[0] + ddp->len - 1) > iMax) 
        iMax = ddp->starts[0] + ddp->len - 1;
      ddp = ddp->next;
    }
    salp = salp->next;
  }
  sintp->from = iMin;
  sintp->to = iMax;
  sintp->id = (SeqIdPtr) SeqIdDup(sip);
  if (iMax >= iMin) {
    pcdd->profile_range = (struct seqint PNTR) sintp;
  } else {
    printf(" CddAssignProfileRange: Warning: Can not assign alignment range!\n");
  }
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return a pointer to a specific bioseq from a seq-entry set                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
BioseqPtr LIBCALL CddFindBioseqInSeqEntry(SeqEntryPtr sep, SeqIdPtr sip)
{
  SeqEntryPtr  sepThis;
  BioseqPtr    bsp;
  BioseqSetPtr bssp;

  sepThis = sep;

  while (sepThis) {
    if (IS_Bioseq(sepThis)) {
      bsp = sepThis->data.ptrvalue;
      if (CddSameSip(bsp->id, sip)) return(bsp);
    } else if (IS_Bioseq_set(sepThis)) {
      bssp = sepThis->data.ptrvalue;
      bsp = CddFindBioseqInSeqEntry(bssp->seq_set,sip);
      if (CddSameSip(bsp->id, sip)) return(bsp);
    }
    sepThis = sepThis->next;
  }
  return(NULL);
}

/*---------------------------------------------------------------------------*/
/* rips out and returns a PDBSeqId from a SeqId                              */
/*---------------------------------------------------------------------------*/
PDBSeqIdPtr LIBCALL CddGetPdbSeqId(SeqIdPtr sip)
                    /* may need to be modified according to how bioseq id is */
{
  SeqIdPtr    seq_id = NULL;
  PDBSeqIdPtr pdb_seq_id = NULL;

  seq_id = sip;
  while(seq_id != NULL){
    if(seq_id->choice == SEQID_PDB) {
      pdb_seq_id = seq_id->data.ptrvalue;
      break;
    }
    seq_id = seq_id->next;
  }
  return(pdb_seq_id);
}


/*---------------------------------------------------------------------------*/
/* test whether two SeqIds are the same, only considers PDB-Ids and GIs      */
/* as well as local sequence-id's                                            */
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddSameSip(SeqIdPtr sip1, SeqIdPtr sip2)
{
  PDBSeqIdPtr psip1, psip2;
  SeqIdPtr    tsip1, tsip2;
  ObjectIdPtr oidp1, oidp2;

  if (sip1 == sip2) return(TRUE);
  tsip1 = sip1;
  while (tsip1) {
    tsip2 = sip2;
    while (tsip2) {
      if (tsip1->choice == tsip2->choice) {
        if (tsip1->choice == SEQID_PDB) {
          psip1 = (PDBSeqIdPtr) CddGetPdbSeqId(tsip1);
          psip2 = (PDBSeqIdPtr) CddGetPdbSeqId(tsip2);
          if (strcmp(psip1->mol,psip2->mol)==0 && psip1->chain==psip2->chain)
            return(TRUE);
        } else if (tsip1->choice == SEQID_LOCAL) {
          oidp1 = tsip1->data.ptrvalue;
          oidp2 = tsip2->data.ptrvalue;
          if (NULL != oidp1 && strcmp(oidp1->str,oidp2->str)==0) return(TRUE);
        } else if (tsip1->choice == SEQID_GI) {
	  if (tsip1->data.intvalue == tsip2->data.intvalue) return(TRUE);
	}
      }
      tsip2 = tsip2->next;
    }
    tsip1 = tsip1->next;
  }
  return(FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* reindex master of a MSL-DenSeg Alignment according to a n-terminal offset */
/* must allocate new arrays for "starts", these may just be pointers to DDG  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddReindexMSLDenSegMaster(SeqAlignPtr salp, Int4 offset)
{
  SeqAlignPtr salpThis;
  DenseSegPtr dsp;
  Int4        *newstarts, i;

  if (salp->type == SAT_PARTIAL && salp->segtype == SAS_DENSEG) {
    salpThis = salp;
    while (salpThis) {
      dsp = salpThis->segs;
      if (dsp) {
        newstarts = (Int4 *)MemNew(dsp->dim * sizeof(Int4));
        for (i=0;i<dsp->dim;i++) newstarts[i] = dsp->starts[i];
        newstarts[0] -= offset;
        dsp->starts = newstarts;
      }
      salpThis = salpThis->next;
    }
  }
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* reindex master of a MSL-DenDiag Alignment according to a n-terminal offset*/
/* must allocate new arrays for "starts", these may just be pointers to DDG  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddReindexMSLDenDiagMaster(SeqAlignPtr salp, Int4 offset)
{
  SeqAlignPtr  salpThis;
  DenseDiagPtr ddp;

  if (salp->type == SAT_PARTIAL && salp->segtype == SAS_DENDIAG) {
    salpThis = salp;
    while (salpThis) {
      ddp = salpThis->segs;
      while (ddp) {
        ddp->starts[0] -= offset;
        ddp = ddp->next;
      }
      salpThis = salpThis->next;
    }
  }
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* makes a copy of one master-slave dense-diag alignment - does NOT copy the */
/* identifiers. Should only be used for a reindexing the master for CposComp.*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAlignPtr LIBCALL CddCopyMSLDenDiag(SeqAlignPtr salp)
{
  SeqAlignPtr  salpNew = NULL, salpHead = NULL, salpTail = NULL;
  DenseDiagPtr ddp, ddpNew, ddpTail;
  Int4         i;
  
  if (salp->type == SAT_PARTIAL && salp->segtype == SAS_DENDIAG) {
    while (salp) {
      salpNew = SeqAlignNew();
      salpNew->type = salp->type;
      salpNew->segtype = salp->segtype;
      salpNew->dim = salp->dim;
      salpNew->segs = NULL;
      ddp = salp->segs;
      while (ddp) {
        ddpNew = DenseDiagNew();
        ddpNew->dim = ddp->dim;
        ddpNew->id = ddp->id;
        ddpNew->starts = MemNew(ddpNew->dim * sizeof(Int4));
        for (i =0;i<ddpNew->dim;i++) ddpNew->starts[i] = ddp->starts[i];
        ddpNew->len = ddp->len;
        if (!salpNew->segs) {
          salpNew->segs = ddpNew;
          ddpTail = ddpNew;
          ddpTail->next = NULL;
        } else {
          ddpTail->next = ddpNew;
          ddpTail = ddpNew;
          ddpTail->next = NULL;
        }
        ddp = ddp->next;
      }
      if (!salpHead) {
        salpHead = salpNew;
        salpTail = salpHead;
        salpTail->next = NULL;
      } else {
        salpTail->next = salpNew;
        salpTail = salpNew;
        salpTail->next = NULL;
      }
      salp = salp->next;
    }
  }
  return salpHead;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* allocate and free a new ExplicitAlignment object                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
CddExpAlignPtr CddExpAlignNew()
{
  CddExpAlignPtr pCDea;
  
  pCDea = MemNew(sizeof(CddExpAlign));
  if (!pCDea) return NULL;
  pCDea->length = 0;
  pCDea->ids = NULL;
  pCDea->adata = NULL;
  pCDea->starts = NULL;
  pCDea->bIdAlloc = FALSE;
  return(pCDea);
}

CddExpAlignPtr CddExpAlignFree(CddExpAlignPtr pCDea)
{
  if (!pCDea) return NULL;
  if (NULL != pCDea->adata) free(pCDea->adata);
  pCDea->ids = SeqIdSetFree(pCDea->ids);
  if (NULL != pCDea->starts) free(pCDea->starts);
  return MemFree(pCDea);
}

void CddExpAlignAlloc(CddExpAlignPtr pCDea, Int4 iLength)
{
  Int4    i;
  
  if (!pCDea || !iLength) return;
  pCDea->length = iLength;
  if (NULL != pCDea->adata) free(pCDea->adata);
  pCDea->adata = MemNew(sizeof(Int4)*pCDea->length);  
  for (i=0;i<pCDea->length;i++) pCDea->adata[i] = -1;
  if (NULL != pCDea->starts) free(pCDea->starts);
  pCDea->starts = MemNew(sizeof(Int4)*pCDea->length);  
  for (i=0;i<pCDea->length;i++) pCDea->starts[i] = 0;
}

/*---------------------------------------------------------------------------*/
/* Convert a SeqAlign (pairwise, DenseDiag) into an ExplicitAlignmentObject  */
/*---------------------------------------------------------------------------*/
CddExpAlignPtr LIBCALL SeqAlignToCddExpAlign(SeqAlignPtr salp, SeqEntryPtr sep)
{
  CddExpAlignPtr   pCDea;
  BioseqPtr        bsp1;
  DenseDiagPtr     ddp;
  Int4             i;
  SeqIdPtr         sip;
  
  pCDea = CddExpAlignNew();
  ddp = salp->segs;
  pCDea->ids = SeqIdDupList(ddp->id);
  sip = SeqIdDup(pCDea->ids);
  bsp1 = CddRetrieveBioseqById(sip,sep);
  SeqIdFree(sip);
  CddExpAlignAlloc(pCDea,bsp1->length);
  while (ddp) {
    for (i=0;i<ddp->len;i++) {
      if (ddp->starts[0]+i >= bsp1->length) {
        printf("Warning: Indexing error!\n");
      }
      pCDea->adata[ddp->starts[0]+i]=ddp->starts[1]+i;
    }
    pCDea->starts[ddp->starts[0]] = 1;
    ddp = ddp->next;
  }
  return(pCDea);
}

/*---------------------------------------------------------------------------*/
/* Invert an explicit alignment object (flip master-slave)                   */
/*---------------------------------------------------------------------------*/
CddExpAlignPtr InvertCddExpAlign(CddExpAlignPtr pCDea, SeqEntryPtr sep)
{
  BioseqPtr      bsp;
  CddExpAlignPtr pCDeaNew;
  Int4           i;
 
  if (!pCDea->ids || !pCDea->ids->next) CddSevError("No SeqId in InvertCddExpAlign");
  pCDeaNew = CddExpAlignNew();
  pCDeaNew->ids = SeqIdDup(pCDea->ids->next);
  pCDeaNew->ids->next = SeqIdDup(pCDea->ids);
  pCDeaNew->bIdAlloc = TRUE;
  bsp = CddRetrieveBioseqById(pCDea->ids->next, sep);
  CddExpAlignAlloc(pCDeaNew, bsp->length);
  for (i=0;i<pCDea->length;i++) {
    if (pCDea->adata[i] != -1) {
      pCDeaNew->adata[pCDea->adata[i]] = i;
    }
    if (pCDea->starts[i] > 0) {
      pCDeaNew->starts[pCDea->adata[i]] = 1;
    }
  }
  return(pCDeaNew);
}

/*---------------------------------------------------------------------------*/
/* Convert an Explicit Alignment Object (with proper IDs) to a SeqAlign      */
/*---------------------------------------------------------------------------*/
SeqAlignPtr CddExpAlignToSeqAlign(CddExpAlignPtr pCDea, Int4Ptr iBreakAfter)
{
  SeqAlignPtr   salp;
  DenseDiagPtr  ddp, ddplast = NULL;
  Int4          i;
  
  if (!pCDea->ids || !pCDea->ids->next)
   CddSevError("Missing Sequence ID in CddExpAlignToSeqAlign");
  salp = SeqAlignNew();
  salp->type = SAT_PARTIAL;
  salp->segtype = SAS_DENDIAG;
  salp->dim = 2;
  salp->segs = NULL;
  
  i= -1;
  while (++i < pCDea->length) {
    if (pCDea->adata[i] != -1) {
      ddp = DenseDiagNew();
      ddp->id = SeqIdDup(pCDea->ids);
      ddp->id->next = SeqIdDup(pCDea->ids->next);
      ddp->dim = 2;
      ddp->starts = MemNew(2*sizeof(Int4));
      ddp->starts[0] = i;
      ddp->starts[1] = pCDea->adata[i];
      ddp->len = 1;
      while (i<(pCDea->length-1) && pCDea->adata[i+1]==(pCDea->adata[i]+1) &&
             (NULL == iBreakAfter || iBreakAfter[i] == 0) &&
	     pCDea->starts[i+1] == 0) {
	ddp->len++;
	i++;
      }
      if (ddplast) {
        ddplast->next = ddp;
      } else {
        salp->segs = ddp;
      }
      ddplast = ddp;
    }
  }
  if (!salp->segs) salp = SeqAlignFree(salp);
  return(salp);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* reindex a m-s1, m-s2 alignment pair to a s1-s2 alignment pair             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
CddExpAlignPtr CddReindexExpAlign(CddExpAlignPtr pCDea1, Int4 newlength,
                                  CddExpAlignPtr pCDea2, Int4 iOuter, Int4 iInner)
{
  CddExpAlignPtr     pCDea = NULL;
  Int4               i, iP1, iP2;
  SeqIdPtr           sip1, sip2;
  
  if (!pCDea1 || !pCDea2) return pCDea;
  if (pCDea1->ids && pCDea1->ids->next) {
    sip1 = SeqIdDup(pCDea1->ids->next);  
  } else sip1 = NULL;
  if (pCDea2->ids && pCDea2->ids->next) {
    sip2 = SeqIdDup(pCDea2->ids->next);  
  } else sip2 = NULL;
  
  pCDea = CddExpAlignNew();
  CddExpAlignAlloc(pCDea,newlength);
  for (i=0;i<pCDea1->length && i<pCDea2->length;i++) {
    iP1 = pCDea1->adata[i]; iP2 = pCDea2->adata[i];
    if (iP1 > -1 && iP2 > -1){
      if (iP1 >= newlength) {
        printf("Warning: index error between sequences %d and %d\n",
	        iInner, iOuter);
        CddSevError("Error while reindexing explicit alignments!");
      } else  pCDea->adata[iP1] = iP2;
      if (pCDea1->starts[i] > 0) pCDea->starts[iP1] = 1;
    }
  }
  if (sip1 && sip2) {
    pCDea->ids = sip1;
    pCDea->ids->next = sip2;
    pCDea->bIdAlloc = TRUE;
  }
  return pCDea;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* returns a pairwise % id for two sequences in a CD (ordered by alignmt)    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
FloatHi CddGetPairId(TrianglePtr pTri, Int4 idx1, Int4 idx2)
{
  Int4 i,  index;
  float    findex;
  ScorePtr psc;
  
  if (idx1 == idx2) return 100.0;
  if (idx1 > idx2) { i = idx1; idx1 = idx2; idx2 = i; }
  findex=(float)idx1*((float)pTri->nelements-((float)idx1+1.0)/2.0-1.0)+(float)idx2-1.0;
  index = (Int4) findex;
  if (index < 0) CddSevError("Impossible index in CddGetPairId!");
  psc = (ScorePtr) pTri->scores; i=0;
  while (psc && (i<index)) {
    i++; psc = psc->next;
  }
  if (psc) return psc->value.realvalue;
  else CddSevError("Ran out of scores in CddGetPairId!");
  return((FloatHi) 0.0);
}

static Boolean HitYet(Int4Ptr retlist, Int4 index, Int4 i)
{
  Int4 j;
  for (j=0;j<index;j++) {
    if (retlist[j]==i) {
      return TRUE; 
    }
  }
  return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return a list of indices for the n most dissimilar sequences in a CD      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int4Ptr CddMostDiverse(TrianglePtr pTri, Int4 length, Int4 maxdiv)
{
  Int4Ptr     retlist;
  Int4        index, winner, i, j, ncomp;
  FloatHi     sumcomp, mincomp;
  FloatHi     **iMat;
  ScorePtr    psc;
  ValNodePtr  vnp;
  
  if (!pTri) return NULL;
  if (length >= pTri->nelements) return NULL;
  if (length > maxdiv) length = maxdiv;
  retlist = MemNew(length * sizeof(Int4));
  retlist[0] = 0;

  if (pTri->div_ranks) {
    vnp = pTri->div_ranks; i = 0;
    while (vnp) {
      retlist[i] = vnp->data.intvalue;
      i++; 
      if (i >= length) break;
      vnp = vnp->next;
    }
    return(retlist);
  }
  
  iMat = (FloatHi **) calloc(pTri->nelements,sizeof (FloatHi *));
  for (i=0;i<pTri->nelements;i++) {
    iMat[i] = (FloatHi *) calloc(pTri->nelements,sizeof(FloatHi));
    iMat[i][i] = 100;
  }
  psc = (ScorePtr) pTri->scores;
  for (i=0;i<pTri->nelements;i++) {
    for (j=i+1;j<pTri->nelements;j++) {
      iMat[i][j] = iMat[j][i] = psc->value.realvalue;
      psc = psc->next;
    }
  }
  
  for (index = 1; index < length; index++) {
    mincomp = 100.0, winner = -1;
    for (i=1;i<pTri->nelements;i++) {
      sumcomp = 0.0; ncomp = 0;
      if (!HitYet(retlist,index,i)) {
        for (j=0;j<pTri->nelements;j++) {
	  if (HitYet(retlist,index,j)) {
/*	    sumcomp += CddGetPairId(pTri, i, j); */
            sumcomp += iMat[i][j];
	    ncomp ++;
	  }
	}
	if (ncomp) sumcomp /= (FloatHi) ncomp;
	if (sumcomp < mincomp) {
	  mincomp = sumcomp; winner = i;
	}
      }
    }
    retlist[index] = winner;
  }
  for (i=0;i<pTri->nelements;i++) free(iMat[i]);
  free(iMat);
  return (retlist);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return a list of indices for the n most similar sequences for the query   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int4Ptr CddMostSimilarToQuery(ScorePtr psc, Int4 length)
{
  Int4Ptr    retlist;
  Int4       winner, index, i, nelements;
  FloatHi    sumcomp, mincomp;
  ScorePtr   thispsc;
  
  if (!psc) return NULL;
  nelements = 0; thispsc = psc;
  while (thispsc) { nelements++; thispsc = thispsc->next; }
  if (length >= nelements) return NULL;
  retlist = MemNew(length * sizeof(Int4));
  retlist[0] = 0;
  for (index=1;index<length;index++) {
    mincomp = 0.0; winner = -1;
    i = 0;
    thispsc = psc->next;
    while (thispsc) {
      i++;    
      if (!HitYet(retlist,index,i)) {
        sumcomp = thispsc->value.realvalue;
        if (sumcomp > mincomp) {
	  mincomp = sumcomp;
	  winner = i;
	}
      }
      thispsc = thispsc->next;
    }
    retlist[index] = winner;
  }  
  return(retlist);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* retrieve a sequence by trying to locate it in the CDD bioseq-set or other */
/* wise through the sequence/object-manager or ID1                           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
BioseqPtr CddRetrieveBioseqById(SeqIdPtr sip, SeqEntryPtr sep)
{
  BioseqPtr   bsp = NULL;
  Int4        uid = 0;
/*  SeqEntryPtr sepThis; */
  Int2        retcode = 1;

  bsp = (BioseqPtr) CddFindSip(sip, sep);
  if (!bsp) {
    bsp = BioseqLockById(sip);
  }
  /*
  if (!bsp) {
    uid = ID1FindSeqId(sip);
    if (uid > 0) {
      sepThis = (SeqEntryPtr) ID1SeqEntryGet(uid,retcode);
      if (sepThis) {
        bsp = CddExtractBioseq(sepThis, sip);
      }
    }
  }
  */
  return bsp;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Calculate pairwise percentages of identical residues for sequences in a CD*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
TrianglePtr CddCalculateTriangle(CddPtr pcdd)
{
   BioseqPtr      bsp1 = NULL, bsp2, bsp3;
   BioseqSetPtr   bssp = NULL;
   CddExpAlignPtr pCDea, pCDea1, pCDea2;
   DenseDiagPtr   ddp;
   SeqAlignPtr    salpOuter, salpInner, salpHead;
   SeqIdPtr       sip1, sip2, sip3;
   SeqPortPtr     spp1, spp2, spp3;
   ScorePtr       pscHead=NULL, psc, pscTail;
   TrianglePtr    pTri;
   Uint1Ptr       buffer1, buffer2, buffer3;
   Int4           i, alen, nid, iOuter, iInner;
   

   if (!pcdd->seqannot) return NULL;
   salpHead = pcdd->seqannot->data;
   pTri = TriangleNew();
   pTri->nelements = 1;

/*---------------------------------------------------------------------------*/
/* Loop around Outer pointer to get master-slave alignments                  */
/*---------------------------------------------------------------------------*/
   salpOuter = salpHead; bsp1 = NULL; sip1 = NULL;
   while (salpOuter) {
     pTri->nelements++;
     ddp = salpOuter->segs;
     if (!sip1) sip1 = SeqIdDup(ddp->id);
     sip2 = SeqIdDup(ddp->id->next);
     if (!bssp) bssp = pcdd->sequences->data.ptrvalue;
     if (!bsp1) {
       bsp1 = (BioseqPtr) CddRetrieveBioseqById(sip1, bssp->seq_set);
       buffer1 = MemNew((bsp1->length)*sizeof(Uint1));
       spp1 = SeqPortNew(bsp1, 0, bsp1->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
       for (i=0; i<bsp1->length;i++) buffer1[i] = SeqPortGetResidue(spp1);
       spp1 = SeqPortFree(spp1);
     }
     bsp2 = NULL;
     bsp2 = (BioseqPtr) CddRetrieveBioseqById(sip2, bssp->seq_set);
     buffer2 = MemNew((bsp2->length)*sizeof(Uint1));
     spp2 = SeqPortNew(bsp2, 0, bsp2->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
     for (i=0; i<bsp2->length;i++) buffer2[i] = SeqPortGetResidue(spp2);
     spp2 = SeqPortFree(spp2);
     pCDea = CddExpAlignNew();
     CddExpAlignAlloc(pCDea,bsp1->length);
     while (ddp) {
       for (i=0;i<ddp->len;i++) {
         pCDea->adata[ddp->starts[0]+i]=ddp->starts[1]+i;
       }
       ddp = ddp->next;
     }
     alen = 0; nid = 0;
     for (i=0;i<bsp1->length;i++) {
       if (pCDea->adata[i] > -1) {
         alen++; if (buffer1[i] == buffer2[pCDea->adata[i]]) nid++;
       }
     }
     psc = ScoreNew(); psc->id=ObjectIdNew(); psc->id->str=StringSave("%id");
     psc->choice = 2;
     if (alen) psc->value.realvalue = 100.0 * (FloatHi) nid / (FloatHi) alen;
     else psc->value.realvalue = 0.0;
     psc->next = NULL;
     if (!pscHead) {
       pscHead = psc; pscTail = psc;
     } else {
       pscTail->next = psc; pscTail = psc; psc = NULL;
     }
     pCDea = CddExpAlignFree(pCDea);
     MemFree(buffer2); SeqIdFree(sip2);
     salpOuter = salpOuter->next;
   }
   
/*---------------------------------------------------------------------------*/
/* Loop around Outer and Inner Pointer to get slave-slave similarities       */
/*---------------------------------------------------------------------------*/
   salpOuter = salpHead; iOuter = 1;
   while (salpOuter->next) {
     iOuter++;
     ddp = salpOuter->segs;
     sip2 = SeqIdDup(ddp->id->next); bsp2 = NULL;
     bsp2 = (BioseqPtr) CddRetrieveBioseqById(sip2, bssp->seq_set);
     buffer2 = MemNew((bsp2->length)*sizeof(Uint1));
     spp2 = SeqPortNew(bsp2, 0, bsp2->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
     for (i=0; i<bsp2->length;i++) buffer2[i] = SeqPortGetResidue(spp2);
     spp2 = SeqPortFree(spp2);
     pCDea1 = CddExpAlignNew();
     CddExpAlignAlloc(pCDea1,bsp1->length);
     while (ddp) {
       for (i=0;i<ddp->len;i++) pCDea1->adata[ddp->starts[0]+i]=ddp->starts[1]+i;
       ddp = ddp->next;
     }
     salpInner = salpOuter->next;
     iInner = iOuter;
     while (salpInner) {
       iInner++;
       ddp = salpInner->segs;
       sip3 = SeqIdDup(ddp->id->next); bsp3 = NULL;
       bsp3 = (BioseqPtr) CddRetrieveBioseqById(sip3,bssp->seq_set);
       buffer3  =MemNew((bsp3->length)*sizeof(Uint1));
       spp3 = SeqPortNew(bsp3, 0, bsp3->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
       for (i=0; i<bsp3->length;i++) buffer3[i] = SeqPortGetResidue(spp3);
       spp3 = SeqPortFree(spp3);
       pCDea2 = CddExpAlignNew();
       CddExpAlignAlloc(pCDea2,bsp1->length);
       while (ddp) {
         for (i=0;i<ddp->len;i++) pCDea2->adata[ddp->starts[0]+i]=ddp->starts[1]+i;
         ddp = ddp->next;
       }
       pCDea = CddReindexExpAlign(pCDea1, bsp2->length, pCDea2, iOuter, iInner);
       alen = 0; nid = 0;
       for (i=0;i<bsp2->length;i++) {
         if (pCDea->adata[i] > -1) {
           alen++; if (buffer2[i] == buffer3[pCDea->adata[i]]) nid++;
         }
       }
       psc = ScoreNew(); psc->id=ObjectIdNew(); psc->id->str=StringSave("%id");
       psc->choice = 2;
       if (alen) psc->value.realvalue = 100.0 * (FloatHi) nid / (FloatHi) alen;
       else psc->value.realvalue = 0.0;
       psc->next = NULL;
       pscTail->next = psc; pscTail = psc; psc = NULL;
       pCDea2 = CddExpAlignFree(pCDea2);
       pCDea = CddExpAlignFree(pCDea);
       MemFree(buffer3); SeqIdFree(sip3); 
       salpInner = salpInner->next;
     }
     pCDea1 = CddExpAlignFree(pCDea1);
     MemFree(buffer2); SeqIdFree(sip2);
     salpOuter = salpOuter->next;
   }
   if (sip1) SeqIdFree(sip1); if (bsp1) bsp1 = NULL;   
   if (buffer1) MemFree(buffer1);
   pTri->scores = (struct score PNTR) pscHead;
   return(pTri);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Calculate pairwise similarities for the query added to a cd alignment     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
ScorePtr CddCalculateQuerySim(CddPtr pcdd, SeqAlignPtr salp)
{
   BioseqPtr      bsp1, bsp2, bsp3;
   BioseqSetPtr   bssp = NULL;
   CddExpAlignPtr pCDea, pCDea1, pCDea2;
   DenseDiagPtr   ddp;
   SeqAlignPtr    salpOuter, salpInner, salpHead;
   SeqIdPtr       sip1, sip2, sip3;
   SeqPortPtr     spp1, spp2, spp3;
   ScorePtr       pscHead=NULL, psc, pscTail;
   Uint1Ptr       buffer1, buffer2, buffer3;
   Int4           i, alen, nid;
   

   if (!salp || !pcdd) return NULL;
   salpHead = salp;
/*---------------------------------------------------------------------------*/
/* master-query alignment                                                    */
/*---------------------------------------------------------------------------*/
   salpOuter = salpHead; bsp1 = NULL; sip1 = NULL;
   pCDea = CddExpAlignNew();
   ddp = salpOuter->segs;
   sip1 = SeqIdDup(ddp->id);
   sip2 = SeqIdDup(ddp->id->next);
   bssp = pcdd->sequences->data.ptrvalue;
   bsp1 = (BioseqPtr) CddRetrieveBioseqById(sip1, bssp->seq_set);
   buffer1 = MemNew((bsp1->length)*sizeof(Uint1));
   spp1 = SeqPortNew(bsp1, 0, bsp1->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
   for (i=0; i<bsp1->length;i++) buffer1[i] = SeqPortGetResidue(spp1);
   spp1 = SeqPortFree(spp1);
   bsp2 = (BioseqPtr) CddRetrieveBioseqById(sip2, bssp->seq_set);
   buffer2 = MemNew((bsp2->length)*sizeof(Uint1));
   spp2 = SeqPortNew(bsp2, 0, bsp2->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
   for (i=0; i<bsp2->length;i++) buffer2[i] = SeqPortGetResidue(spp2);
   spp2 = SeqPortFree(spp2);
   CddExpAlignAlloc(pCDea,bsp1->length);
   while (ddp) {
     for (i=0;i<ddp->len;i++) {
       pCDea->adata[ddp->starts[0]+i]=ddp->starts[1]+i;
     }
     ddp = ddp->next;
   }
   alen = 0; nid = 0;
   for (i=0;i<bsp1->length;i++) {
     if (pCDea->adata[i] > -1) {
       alen++; if (buffer1[i] == buffer2[pCDea->adata[i]]) nid++;
     }
   }
   pCDea = CddExpAlignFree(pCDea);
   psc = ScoreNew(); psc->id=ObjectIdNew(); psc->id->str=StringSave("%id");
   psc->choice = 2;
   if (alen) psc->value.realvalue = 100.0 * (FloatHi) nid / (FloatHi) alen;
   else psc->value.realvalue = 0.0;
   psc->next = NULL;
   pscHead = psc; pscTail = psc; psc = NULL;
   MemFree(buffer2); SeqIdFree(sip2);
/*---------------------------------------------------------------------------*/
/* Loop around Inner Pointer to get query-slave similarities                 */
/*---------------------------------------------------------------------------*/
   salpOuter = salpHead;
   pCDea1 = CddExpAlignNew();
   ddp = salpOuter->segs;
   sip2 = SeqIdDup(ddp->id->next);
   bsp2 = (BioseqPtr) CddRetrieveBioseqById(sip2, bssp->seq_set);
   buffer2 = MemNew((bsp2->length)*sizeof(Uint1));
   spp2 = SeqPortNew(bsp2, 0, bsp2->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
   for (i=0; i<bsp2->length;i++) buffer2[i] = SeqPortGetResidue(spp2);
   spp2 = SeqPortFree(spp2);
   CddExpAlignAlloc(pCDea1,bsp1->length);
   while (ddp) {
     for (i=0;i<ddp->len;i++) pCDea1->adata[ddp->starts[0]+i]=ddp->starts[1]+i;
     ddp = ddp->next;
   }
   salpInner = salpOuter->next;
   while (salpInner) {
     pCDea2 = CddExpAlignNew();
     ddp = salpInner->segs;
     sip3 = SeqIdDup(ddp->id->next);
     bsp3 = (BioseqPtr) CddRetrieveBioseqById(sip3,bssp->seq_set);
     buffer3  =MemNew((bsp3->length)*sizeof(Uint1));
     spp3 = SeqPortNew(bsp3, 0, bsp3->length-1, Seq_strand_unknown, Seq_code_ncbistdaa);
     for (i=0; i<bsp3->length;i++) buffer3[i] = SeqPortGetResidue(spp3);
     spp3 = SeqPortFree(spp3);
     CddExpAlignAlloc(pCDea2,bsp1->length);
     while (ddp) {
       for (i=0;i<ddp->len;i++) pCDea2->adata[ddp->starts[0]+i]=ddp->starts[1]+i;
       ddp = ddp->next;
     }
     pCDea = CddReindexExpAlign(pCDea1, bsp2->length, pCDea2,0,0);
     alen = 0; nid = 0;
     for (i=0;i<bsp2->length;i++) {
       if (pCDea->adata[i] > -1) {
         alen++; if (buffer2[i] == buffer3[pCDea->adata[i]]) nid++;
       }
     }
     pCDea = CddExpAlignFree(pCDea);
     psc = ScoreNew(); psc->id=ObjectIdNew(); psc->id->str=StringSave("%id");
     psc->choice = 2;
     if (alen) psc->value.realvalue = 100.0 * (FloatHi) nid / (FloatHi) alen;
     else psc->value.realvalue = 0.0;
     psc->next = NULL;
     pscTail->next = psc; pscTail = psc; psc = NULL;
     pCDea2 = CddExpAlignFree(pCDea2);
     MemFree(buffer3); SeqIdFree(sip3); 
     salpInner = salpInner->next;
   }
   pCDea1 = CddExpAlignFree(pCDea1);
   MemFree(buffer2); SeqIdFree(sip2);
   if (sip1) SeqIdFree(sip1); if (bsp1) bsp1 = NULL;   
   if (buffer1) MemFree(buffer1);
   return(pscHead);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* cloned from salpedit.c and salpacc.c                                      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAnnotPtr LIBCALL CddSeqAnnotForSeqAlign (SeqAlignPtr salp)
{
  SeqAnnotPtr sap;

  if (salp==NULL)
     return NULL;
  sap = SeqAnnotNew ();
  sap->type = 2;
  sap->data = (Pointer) salp;
  return sap;
}

SeqAlignPtr LIBCALL CddSeqAlignDup (SeqAlignPtr salp)
{
  SeqAnnotPtr sap, 
              sap2;
  SeqAlignPtr salp2 = NULL,
              next; 

  next = salp->next;
  salp->next = NULL;
  sap = CddSeqAnnotForSeqAlign (salp);
  sap2 = (SeqAnnotPtr) AsnIoMemCopy ((Pointer) sap, (AsnReadFunc) SeqAnnotAsnRead, (AsnWriteFunc) SeqAnnotAsnWrite);
  if (sap2!=NULL) {
     salp2 = (SeqAlignPtr)sap2->data;
     sap2->data = NULL;
     SeqAnnotFree (sap2);
  }
  if (next != NULL)
     salp->next = next;
  sap->data = NULL;
  SeqAnnotFree (sap);
  return salp2;
}



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Duplicate a whole Sequence Alignment (i.e. a linked list of Seqaligns     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/


SeqAlignPtr LIBCALL SeqAlignSetDup(SeqAlignPtr salp)
{
  SeqAlignPtr   salpHead = NULL;
  SeqAlignPtr   salpTail = NULL;
  SeqAlignPtr   salpThis;

  while (salp) {
    salpThis = (SeqAlignPtr) CddSeqAlignDup(salp);
    if (salpTail) {
      salpTail->next = salpThis;
      salpTail = salpThis;
      salpThis->next = NULL;
    } else {
      salpHead = salpThis;
      salpTail = salpThis;
      salpThis->next = NULL;
    }
    salp = salp->next;
  }
  return (salpHead);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* fill internal gaps in a sequence alignment to prepare it for PSSM calc.   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddDegapSeqAlign(SeqAlignPtr salp)
{
  DenseDiagPtr    ddp;
  Int4            iM, iS, iN, iC;
  Boolean         bFirst, bInit = TRUE;
  BioseqPtr       bsp;
  SeqIdPtr        sip;
  Int4            mLen, sLen;
  
  while (salp) {
    ddp = salp->segs;
    if (bInit) {
      sip = ddp->id;
      bsp = BioseqLockById(sip);
      mLen = bsp->length;
      BioseqUnlock(bsp);
      bInit = FALSE;
    }
    sip = ddp->id->next;
    bsp = BioseqLockById(sip);
    sLen = bsp->length;
    BioseqUnlock(bsp);
    bFirst = TRUE;
    while (ddp) {
      if (bFirst) { /* extend first aligned block to N-terminus */
        iM = ddp->starts[0];
	iS = ddp->starts[1];
	if (iM > iS) iM = iS;
	ddp->starts[0] -= iM;
	ddp->starts[1] -= iM;
	ddp->len += iM;
        bFirst = FALSE;
      }
      if (ddp->next) {
        iM = ddp->next->starts[0] - (ddp->starts[0]+ddp->len);
        iS = ddp->next->starts[1] - (ddp->starts[1]+ddp->len);
        if (iM > iS) iM = iS;
        if (iM > 0) {
          if (iM == 1) {
	    iN = 1; iC = 0;
          } else {
	    if ((iM % 2) == 0) {
	      iN = iC = iM / 2;
	    } else {
	      iC = iM / 2; iN = iC + 1;
	    }
          }
	  ASSERT(iN+iC == iM);
	  ddp->len += iN;
	  ddp->next->starts[0] -= iC;
	  ddp->next->starts[1] -= iC;
	  ddp->next->len += iC;
        }
      } else { /* extend last aligned block to C-terminus */
        iM = mLen - (ddp->starts[0]+ddp->len);      
        iS = sLen - (ddp->starts[1]+ddp->len);      
        if (iM > iS) iM = iS;
	ddp->len += iM;
      }
      ddp = ddp->next;
    }
    salp = salp->next;
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Duplicate a SeqId, pick the GI if encountering a linked list of several   */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqIdPtr LIBCALL CddSeqIdDupPDBGI(SeqIdPtr sipold)
{
  SeqIdPtr   sip, sipgi = NULL, sippdb = NULL;
  
  sip = sipold;
  while (sip) {
    if (sip->choice == SEQID_GI)   sipgi = sip;    
    if (sip->choice == SEQID_PDB) sippdb = sip;
    sip = sip->next;
  }
  if (sippdb) return(SeqIdDup(sippdb));
  if (sipgi) return(SeqIdDup(sipgi));
  return (SeqIdDup(sipold));
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Calculate a weighted 50/50 consensus sequence and make it new master      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAlignPtr LIBCALL CddConsensus(SeqAlignPtr salp,
                                 SeqEntryPtr sep,
				 BioseqPtr   bsp,
				 SeqIntPtr   range,
				 BioseqPtr   *bspCons,
				 SeqAlignPtr *salpCons)
{
  BioseqPtr           bspThis;
  CddAlignWeightPtr   pCAW;
  CddExpAlignPtr      pCDea;
  CddExtAlignCellPtr  *pCEACell;
  CddExtAlignColPtr   pCEACol;
  CharPtr             cbuffer, outptr;
  DenseDiagPtr        ddp;
  Int4Ptr             trunc_on_virtual;
  Int4Ptr             trunc_aligned;
  ObjectIdPtr         oidp;
  SeqAlignPtr         salpThis, salpNew, salpTail;
  SeqEntryPtr         sepNew;
  SeqIdPtr            sipThis, sipCopy;
  SeqMapTablePtr      smtp;
  SeqPortPtr          spp;
  Uint1Ptr            buffer;
  FloatHi             SumWeight;
  Int4                maxextlen, nalign, ConsensusLen;
  Int4                index, midpt, ContWeight = 0;
  Int4                offset, i, j , k, ipt, seqpos;
  Int4                m_last, m_frst, s_insert, m_end;
  Int4                s_last, s_frst, maxpos = 0;
  
  if (!salp) return (NULL);
  if (salp->type != SAT_PARTIAL || salp->segtype != SAS_DENDIAG) return(NULL);
  if (!range) return (NULL);
  offset = range->from;
/*---------------------------------------------------------------------------*/
/* trunc_on_virtual holds, for every residue of the trunc_master, its posi-  */
/* tion on the maximal expanded new master sequence                          */
/* bsp is supposed to be the "truncated master", i.e. a bioseq covering the  */
/* interval from the first to the last aligned residue on the master sequence*/
/*---------------------------------------------------------------------------*/
  trunc_on_virtual = MemNew(sizeof(Int4)*bsp->length);
  for (i=0;i<bsp->length;i++) trunc_on_virtual[i] = i;
  maxextlen = bsp->length-1;
  trunc_aligned = MemNew(sizeof(Int4)*bsp->length);
  for (i=0;i<bsp->length;i++) trunc_aligned[i] = 0;

/*---------------------------------------------------------------------------*/
/* expand values on trunc_on_virtual to consider insertions relativ to master*/
/*---------------------------------------------------------------------------*/
  nalign = 1;
  salpThis = salp;
  while (salpThis) {
    nalign++;
    ddp = salpThis->segs;
    if (ddp) for (j=ddp->starts[0]-offset;j<ddp->starts[0]+ddp->len-offset;j++) trunc_aligned[j]=1;
    while (ddp && ddp->next) {
      for (j=ddp->next->starts[0]-offset;j<ddp->next->starts[0]+ddp->next->len-offset;j++) trunc_aligned[j]=1;
      m_last = ddp->starts[0]+ddp->len - 1 - offset;
      m_frst = ddp->next->starts[0] - offset;
      m_end  = ddp->next->starts[0] + ddp->next->len - 1 - offset;
      ASSERT(m_frst >= 0 && m_frst < bsp->length);
      ASSERT(m_last >= 0 && m_last < bsp->length);
      ASSERT(m_end >= 0 && m_end < bsp->length);
/*---------------------------------------------------------------------------*/
/* shift block from the c- to n-terminus if necessary                        */
/*---------------------------------------------------------------------------*/
/*    for (j = m_end; j > m_frst; j--) {
        if (trunc_on_virtual[j-1] < trunc_on_virtual[j]-1) {
	  trunc_on_virtual[j-1] = trunc_on_virtual[j] - 1;
	}
      }
*/
      s_insert = ddp->next->starts[1] - ddp->starts[1] - ddp->len;
      if (s_insert > (m_frst-m_last- 1)) {
        if (s_insert > (trunc_on_virtual[m_frst]-trunc_on_virtual[m_last]-1)) {
/*---------------------------------------------------------------------------*/
/* need to insert additional space, find insertion point first               */
/*---------------------------------------------------------------------------*/
	  ipt = m_frst;
	  for (j=m_frst;j>m_last;j--) {
	    if (trunc_on_virtual[j-1] < trunc_on_virtual[j]-1) {
	      ipt = j; break;
	    }
	  }
/*---------------------------------------------------------------------------*/
/* do the shift starting at the insertion point and moving to the end        */
/*---------------------------------------------------------------------------*/
          s_insert -= trunc_on_virtual[m_frst]-trunc_on_virtual[m_last]-1;
	  ASSERT(s_insert > 0);
	  for (j=ipt;j<bsp->length;j++) trunc_on_virtual[j]+=s_insert;
	}
      }
      ddp = ddp->next;
    }
    salpThis = salpThis->next;
  }
/*---------------------------------------------------------------------------*/
/* take care of unaligned regions on the master - distribute evenly on both  */
/* sides of the gap. If uneven number, one more residue shifted to N-terminus*/
/*---------------------------------------------------------------------------*/
  i=-1;
  while (++i < bsp->length-1) {
    if (trunc_aligned[i]==0) {
      j=i; while(trunc_aligned[j]==0 && j < (bsp->length)) j++;
      j--;
      if (j > i && j < (bsp->length)) {
        midpt = i + (j-i)/2;
	for (k=j;k>midpt;k--) {
	  if (trunc_on_virtual[k]<trunc_on_virtual[k+1]-1) {
	    trunc_on_virtual[k] = trunc_on_virtual[k+1]-1;
	  }
	}
        i = j;  
      }
    }
  }
/*  for (j=0;j<bsp->length;j++) printf(" %d",trunc_on_virtual[j]);
  printf("\n"); */
  maxextlen = trunc_on_virtual[bsp->length-1]+1;
  for (i=1;i<bsp->length;i++) {
    if (trunc_on_virtual[i] <= trunc_on_virtual[i-1]) {
      CddSevError("Error determining alignment coordinate system");
    }
  }
/*---------------------------------------------------------------------------*/
/* allocate space for storing the extended alignment plus weights and columns*/
/*---------------------------------------------------------------------------*/
  pCAW = MemNew(nalign * sizeof(CddAlignWeight));
  pCEACol = MemNew(maxextlen * sizeof(CddExtAlignCol));
  pCEACell = MemNew(nalign * sizeof(CddExtAlignCellPtr));
  for (j=0;j<nalign;j++) {
    pCEACell[j] = MemNew(maxextlen * sizeof(CddExtAlignCell));
    for (k=0;k<maxextlen;k++) {
      pCEACell[j][k].seqpos = -1;
      pCEACell[j][k].aatype = 26;
    } 
  }
/*---------------------------------------------------------------------------*/
/* start to fill in the extended alignment data, row 0 is taken by old       */
/* master which is added as fully extended sequence (i.e. not truncated)     */
/*---------------------------------------------------------------------------*/
  i = 0;
  salpThis = salp;
  ddp = salpThis->segs;
  sipCopy = SeqIdDup(ddp->id); sipCopy->next = NULL;
  bspThis = CddRetrieveBioseqById(sipCopy,sep);
  SeqIdFree(sipCopy);
  pCAW[i].bsp = bspThis;
  spp = SeqPortNew(bspThis, 0, LAST_RESIDUE, Seq_strand_unknown,
                   Seq_code_ncbistdaa);
  buffer = MemNew((bspThis->length)*sizeof(Uint1));
  for (index=0; index<bspThis->length; index++)
    buffer[index] = SeqPortGetResidue(spp);
  spp = SeqPortFree(spp);
  for (j=0;j<bsp->length;j++) {
    k = trunc_on_virtual[j];
    seqpos = j + offset;
    ASSERT (k>=0 && k<maxextlen);
    pCEACell[i][k].seqpos = seqpos;
    pCEACell[i][k].aatype = buffer[seqpos];
    if (pCEACell[i][k].aatype > (Uint1) 26) {
      pCEACell[i][k].aatype = 26;
    }
  }
  MemFree(buffer);
/*---------------------------------------------------------------------------*/
/* continue to fill in alignment data for the remaining sequences            */
/*---------------------------------------------------------------------------*/
  salpThis = salp;
  while (salpThis) {
    i++;
    ddp = salpThis->segs;
    sipThis = ddp->id->next;
    bspThis = CddRetrieveBioseqById(sipThis,sep);
    pCAW[i].bsp = bspThis;
    spp = SeqPortNew(bspThis, 0, LAST_RESIDUE, Seq_strand_unknown,
                     Seq_code_ncbistdaa);
    buffer = MemNew((bspThis->length)*sizeof(Uint1));
    for (index=0; index<bspThis->length; index++)
      buffer[index] = SeqPortGetResidue(spp);
    spp = SeqPortFree(spp);
    while (ddp) {
      for (j = ddp->starts[0]; j<ddp->starts[0]+ddp->len;j++) {
        k = trunc_on_virtual[j - offset];
        ASSERT (k>=0 && k<maxextlen);
	pCEACell[i][k].seqpos = ddp->starts[1] + j - ddp->starts[0];
	pCEACell[i][k].aatype = buffer[pCEACell[i][k].seqpos];
        if (pCEACell[i][k].aatype > (Uint1) 26) {
          pCEACell[i][k].aatype = 26;
        }
      }
      if (ddp->next) {
        m_last = ddp->starts[0]+ddp->len-1-offset;
	m_frst = ddp->next->starts[0]-offset;
	s_last = ddp->starts[1]+ddp->len-1;
	s_frst = ddp->next->starts[1];
	if (s_frst - s_last > 1) {   /* have unaligned residues */
          midpt = s_last + (s_frst - s_last)/2;
	  for (j = s_last+1;j<=midpt;j++) {
	    k = trunc_on_virtual[m_last] + j - s_last;
            ASSERT (k>=0 && k<maxextlen);
	    pCEACell[i][k].seqpos = j;
	    pCEACell[i][k].aatype = buffer[j];
            if (pCEACell[i][k].aatype > (Uint1) 26) {
              pCEACell[i][k].aatype = 26;
            }
	  }
          for (j = s_frst-1; j>midpt; j--) {
	    k = trunc_on_virtual[m_frst] - (s_frst - j);
            ASSERT (k>=0 && k<maxextlen);
	    pCEACell[i][k].seqpos = j;
	    pCEACell[i][k].aatype = buffer[j];
            if (pCEACell[i][k].aatype > (Uint1) 26) {
              pCEACell[i][k].aatype = 26;
            }
	  }	
	}
      }
      ddp = ddp->next;
    }
    MemFree(buffer);
    salpThis = salpThis->next;
  }
  
/*---------------------------------------------------------------------------*/
/* now do statistics on the columns in this matrix to determine weights      */
/*---------------------------------------------------------------------------*/
  for (k=0;k<maxextlen;k++) {
    pCEACol[k].conpos = -1;
    pCEACol[k].occpos = 0;
    pCEACol[k].ntypes = 0;
    pCEACol[k].aatype = 0;
    pCEACol[k].w_occpos = 0.0;
    pCEACol[k].typecount = (Int4 *) MemNew(26*sizeof(Int4));
    pCEACol[k].wtypefreq = (FloatHi *) MemNew(26*sizeof(FloatHi));
    for (j=0;j<26;j++) {
      pCEACol[k].typecount[j] = 0;
      pCEACol[k].wtypefreq[j] = 0.0;
    }
    for (i=0;i<nalign;i++) {
      if (pCEACell[i][k].seqpos != -1 && pCEACell[i][k].aatype < 26) {
        pCEACol[k].typecount[(Int4)pCEACell[i][k].aatype]++;
	pCEACol[k].occpos++;
	if (pCEACol[k].occpos > maxpos) maxpos = pCEACol[k].occpos;
      }
    }
    for (j=0;j<26;j++) if (pCEACol[k].typecount[j]) pCEACol[k].ntypes++;
  }
/*---------------------------------------------------------------------------*/
/* use blocks with the highest position counts to determine weight           */
/*---------------------------------------------------------------------------*/
  for (i=0;i<nalign;i++) {
    pCAW[i].weight = 0.0;
    pCAW[i].nposaligned = 0;
  }
  for (k=0;k<maxextlen;k++) {
    if (pCEACol[k].occpos >= maxpos) {
      ContWeight++;
      for (i=0;i<nalign;i++) {
        if (pCEACell[i][k].seqpos != -1 && pCEACell[i][k].aatype < (Uint1) 26) {
          pCAW[i].nposaligned++;
	  if (pCEACol[k].typecount[pCEACell[i][k].aatype] > 0) {
  	    pCAW[i].weight += 1.0 / (pCEACol[k].ntypes * 
	                             pCEACol[k].typecount[pCEACell[i][k].aatype]);
	  }	
        }
      }    
    }
  }
/*---------------------------------------------------------------------------*/
/* if sequence not covered by weight calculation, give it the expected weight*/
/*---------------------------------------------------------------------------*/
  for (i=0;i<nalign;i++) {
    if (pCAW[i].weight <= 0.0) {
      pCAW[i].weight = (FloatHi) ContWeight * 1.0 / (FloatHi) nalign;
    }
  }
  SumWeight = 0.0;
  for(i=0;i<nalign;i++) {
    SumWeight += pCAW[i].weight;
  }
  for(i=0;i<nalign;i++) pCAW[i].weight /= SumWeight;
/*---------------------------------------------------------------------------*/
/* now determine columns with at least 50% representation and the consensus  */
/*---------------------------------------------------------------------------*/
  ConsensusLen = 0;
  for (k=0;k<maxextlen;k++) {
    for (i=0;i<nalign;i++) {
      if (pCEACell[i][k].seqpos != -1 && pCEACell[i][k].aatype < (Uint1) 26) {
        pCEACol[k].w_occpos += pCAW[i].weight;
	pCEACol[k].wtypefreq[pCEACell[i][k].aatype]+=pCAW[i].weight;
      }
    }
    if (pCEACol[k].w_occpos >= 0.5) {
      pCEACol[k].conpos = ConsensusLen;
      SumWeight = 0.0;
      ConsensusLen++;
      for (j=0;j<26;j++) {
        if (pCEACol[k].wtypefreq[j] > SumWeight) {
	  SumWeight = pCEACol[k].wtypefreq[j];
	  pCEACol[k].aatype = (Uint1) j;
	}
      }
    }
  }  
  buffer = (Uint1 *) MemNew((ConsensusLen+1) * sizeof(Uint1));
  cbuffer = (Char *) MemNew((ConsensusLen+1) * sizeof(Char));
  for (k=0;k<maxextlen;k++) {
    if (pCEACol[k].conpos != -1) {
      buffer[pCEACol[k].conpos] = pCEACol[k].aatype;
    }
  }
/*---------------------------------------------------------------------------*/
/* translate sequence into ascii/character and make new bioseq               */
/*---------------------------------------------------------------------------*/
  smtp = SeqMapTableFind(Seq_code_ncbieaa, Seq_code_ncbistdaa);
  for (i=0;i<ConsensusLen;i++) {
    cbuffer[i] = (Char) SeqMapTableConvert(smtp, buffer[i]);
  }
  sepNew = FastaToSeqBuffEx(cbuffer,&outptr,FALSE,NULL,FALSE);
  *bspCons = (BioseqPtr) sepNew->data.ptrvalue;
  sipThis = (*bspCons)->id;
  oidp = ObjectIdNew();
  oidp->str = MemNew(10*sizeof(Char));
  strcpy(oidp->str,"consensus");
  oidp->str[9]='\0';
  MemFree(sipThis->data.ptrvalue);
  sipThis->data.ptrvalue = oidp;
  sipThis->choice = SEQID_LOCAL;
  sipThis->next = NULL;
  
/*---------------------------------------------------------------------------*/
/* create the pairwise alignment old-master(not truncated) new-master        */
/*---------------------------------------------------------------------------*/
  bspThis = pCAW[0].bsp;
  pCDea = CddExpAlignNew();
  CddExpAlignAlloc(pCDea,bspThis->length);
  pCDea->ids = CddSeqIdDupPDBGI(bspThis->id);
  pCDea->ids->next = SeqIdDup((*bspCons)->id);
  pCDea->bIdAlloc = TRUE;
  for (k=0;k<maxextlen;k++) {
    if (pCEACol[k].conpos != -1) {
      pCDea->adata[pCEACell[0][k].seqpos]=pCEACol[k].conpos;
    }
  }
/*---------------------------------------------------------------------------*/
/* record block structure present in the input alignment to prevent merges   */
/*---------------------------------------------------------------------------*/
  ddp = salp->segs;
  while (ddp) {
    pCDea->starts[ddp->starts[0]] = 1;
    ddp = ddp->next;
  }
  *salpCons = CddExpAlignToSeqAlign(pCDea,NULL);
  pCDea = CddExpAlignFree(pCDea);

/*---------------------------------------------------------------------------*/
/* create a multiple alignment salpNew where the consensus is the new master */
/*---------------------------------------------------------------------------*/
  salpNew = NULL;
  for (i=0;i<nalign;i++) {
    pCDea = CddExpAlignNew();
    CddExpAlignAlloc(pCDea,ConsensusLen);
    pCDea->ids = SeqIdDup((*bspCons)->id);
    pCDea->ids->next = SeqIdDup(pCAW[i].bsp->id);
    pCDea->bIdAlloc = TRUE;
    for (k=0;k<maxextlen;k++) {
      if (pCEACol[k].conpos != -1) {
        pCDea->adata[pCEACol[k].conpos]=pCEACell[i][k].seqpos;
      }
    }
    salpThis = CddExpAlignToSeqAlign(pCDea,NULL);
    pCDea = CddExpAlignFree(pCDea);
    if (salpThis) {
      if (!salpNew) {
        salpNew = salpThis;
      } else {
        salpTail->next = salpThis;
      }
      salpTail = salpThis;
    }
  }

/*---------------------------------------------------------------------------*/
/* deallocate memory for this function                                       */
/*---------------------------------------------------------------------------*/
  MemFree(cbuffer);
  MemFree(buffer);
  for (j=0;j<nalign;j++) MemFree(pCEACell[j]);
  MemFree(pCEACell);
  for (j=0;j<maxextlen;j++) {
    MemFree(pCEACol[j].typecount);
    MemFree(pCEACol[j].wtypefreq);    
  }
  MemFree(pCEACol);
  MemFree(pCAW);
  MemFree(trunc_aligned);
  MemFree(trunc_on_virtual);
  return(salpNew);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* first part of CddCposComp()                                               */
/* basically returns compactSearch, LetterHead, and posSearch                */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddCposCompPart1(SeqAlignPtr listOfSeqAligns, BioseqPtr bsp,
                             compactSearchItems* compactSearch, ValNodePtr* LetterHead,
                             posSearchItems* posSearch) {
    Int4                numalign, numseq;    /*number of alignments and seqs */
    BLAST_ScoreBlkPtr   sbp;
    SeqLocPtr           private_slp = NULL;
    SeqPortPtr          spp = NULL;
    Uint1Ptr            query_seq, query_seq_start;
    Uint1               residue;
    Int4                index, a, KarlinReturn, array_size;
    Nlm_FloatHiPtr      lambda, K, H;
    Int4Ptr             open, extend;
    BLAST_ResFreqPtr    stdrfp;
    Int2                iStatus;
    ValNodePtr          error_return = NULL;
    ValNodePtr          newLetter;
    SeqCodeTablePtr     sctp;
    BioseqPtr           bspFake, temp_bsp;

    /* this is used for the DenseDiag calculations */
    bspFake  = (BioseqPtr) CddBioseqCopy(NULL,bsp,0,bsp->length-1,0,FALSE);

    if (listOfSeqAligns->segtype == SAS_DENDIAG) {
      numalign = CddCountDenDiagSeqAligns(listOfSeqAligns, &numseq);
    }
    else {
      numalign = CddCountSeqAligns(listOfSeqAligns, &numseq);
    }
    sbp = BLAST_ScoreBlkNew(Seq_code_ncbistdaa,1);
    sbp->read_in_matrix = TRUE;
    sbp->protein_alphabet = TRUE;
    sbp->posMatrix = NULL;
    sbp->number_of_contexts = 1;
    iStatus = BlastScoreBlkMatFill(sbp,"BLOSUM62");
    ASSERT(iStatus == 0);
    compactSearch->qlength = bsp->length;
    compactSearch->alphabetSize = sbp->alphabet_size;
    compactSearch->matrix = sbp->matrix;
    compactSearch->gapped_calculation = TRUE;
    compactSearch->pseudoCountConst = 10;
    compactSearch->ethresh = 0.001;

/*---------------------------------------------------------------------------*/
/* get the query sequence                                                    */
/*---------------------------------------------------------------------------*/
    temp_bsp = bsp;
    if (listOfSeqAligns->segtype == SAS_DENDIAG) {
      bsp = bspFake;  /* to make call compatible with CddDenDiagCposComputation */
    }
    private_slp = SeqLocIntNew(0, bsp->length-1 , Seq_strand_plus, SeqIdFindBest(bsp->id, SEQID_GI));
		spp = SeqPortNewByLoc(private_slp, Seq_code_ncbistdaa);
    SeqPortSet_do_virtual(spp, TRUE);
    query_seq_start = (Uint1Ptr) MemNew(((bsp->length)+2)*sizeof(Char));
    bsp = temp_bsp;
		query_seq_start[0] = NULLB;
    query_seq = query_seq_start+1;
    index=0;
    while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
      if (IS_residue(residue)) {
				query_seq[index] = residue;
				index++;
			}
		}
		query_seq[index] = NULLB;
		spp = SeqPortFree(spp);
		private_slp = SeqLocFree(private_slp);
    compactSearch->query = query_seq;

    BlastScoreBlkFill(sbp,(CharPtr) compactSearch->query,compactSearch->qlength,0);

    sbp->kbp_gap_std[0] = BlastKarlinBlkCreate();
    KarlinReturn = BlastKarlinBlkGappedCalc(sbp->kbp_gap_std[0],11,1,sbp->name,&error_return);
    if (1 == KarlinReturn) {
      BlastErrorPrint(error_return);
    }
    sbp->kbp_gap_psi[0] = BlastKarlinBlkCreate();
    KarlinReturn = BlastKarlinBlkGappedCalc(sbp->kbp_gap_psi[0],11,1,sbp->name,&error_return);
    if (1 == KarlinReturn) {
      BlastErrorPrint(error_return);
    }

    array_size = BlastKarlinGetMatrixValues(sbp->name,&open,&extend,&lambda,&K,&H,NULL);
    if (array_size > 0) {
      for (index = 0; index < array_size; index++) {
        if (open[index] == INT2_MAX && extend[index] == INT2_MAX) {
          sbp->kbp_ideal = BlastKarlinBlkCreate();
          sbp->kbp_ideal->Lambda = lambda[index];
          sbp->kbp_ideal->K = K[index];
          sbp->kbp_ideal->H = H[index];
        }
      }
      MemFree(open);
      MemFree(extend);
      MemFree(lambda);
      MemFree(K);
      MemFree(H);
    }
    if (sbp->kbp_ideal == NULL) {
      sbp->kbp_ideal = BlastKarlinBlkStandardCalcEx(sbp);
    }
    compactSearch->lambda       = sbp->kbp_gap_std[0]->Lambda;
    compactSearch->kbp_std      = sbp->kbp_std;
    compactSearch->kbp_psi      = sbp->kbp_psi;
    compactSearch->kbp_gap_std  = sbp->kbp_gap_std;
    compactSearch->kbp_gap_psi  = sbp->kbp_gap_psi;
    compactSearch->lambda_ideal = sbp->kbp_ideal->Lambda;
    compactSearch->K_ideal      = sbp->kbp_ideal->K;

    stdrfp = BlastResFreqNew(sbp);
    BlastResFreqStdComp(sbp,stdrfp); 
    compactSearch->standardProb = MemNew(compactSearch->alphabetSize * sizeof(Nlm_FloatHi));
    for(a = 0; a < compactSearch->alphabetSize; a++)
      compactSearch->standardProb[a] = stdrfp->prob[a];
    stdrfp = BlastResFreqDestruct(stdrfp);
    posSearch->posInformation = NULL;
/*---------------------------------------------------------------------------*/
/* numseq is replaced with numalign (last argument) - each alignment is a    */
/* single segment and represents a separate sequence                         */
/*---------------------------------------------------------------------------*/
    posAllocateMemory(posSearch, compactSearch->alphabetSize, compactSearch->qlength, numalign);
    if (listOfSeqAligns->segtype == SAS_DENDIAG) {
      CddfindDenseDiagThreshSequences(posSearch, listOfSeqAligns, numalign, numalign);
    }
    else {
      CddfindThreshSequences(posSearch, listOfSeqAligns, numalign, numalign);
    }
    sbp->kbp     = sbp->kbp_psi;
    sbp->kbp_gap = sbp->kbp_gap_psi;
    CddposInitializeInformation(posSearch,sbp,compactSearch);
    if (listOfSeqAligns->segtype == SAS_DENDIAG) {
      CddposDenseDiagDemographics(posSearch, compactSearch, listOfSeqAligns);
    }
    else {
      CddposDemographics(posSearch, compactSearch, listOfSeqAligns);
    }
    posPurgeMatches(posSearch, compactSearch);
    posComputeExtents(posSearch, compactSearch);
    posComputeSequenceWeights(posSearch, compactSearch, 1.0);
    posCheckWeights(posSearch, compactSearch);
    posSearch->posFreqs = (Nlm_FloatHi **) posComputePseudoFreqs(posSearch, compactSearch, TRUE);
    CddposFreqsToMatrix(posSearch,compactSearch);
    posScaling(posSearch, compactSearch);

    sctp = SeqCodeTableFind(Seq_code_ncbistdaa);
    for (a=0;a<compactSearch->alphabetSize;a++) {
      newLetter = ValNodeNew(NULL);
      newLetter->data.ptrvalue = MemNew(2*sizeof(Char));
      Nlm_StrNCpy(newLetter->data.ptrvalue,&(sctp->letters[a]),1);
      ValNodeLink(LetterHead,newLetter);
    }
}


Int4 LIBCALL CddGetNewIndexForThreading(char InChar, char* Output) {
/*---------------------------------------------------------------------------*/
/*  get the index in Output where InChar is found.                           */
/*  return -1 if char is not found in the Output array.                      */
/*---------------------------------------------------------------------------*/
    Int4  i, Index=0;

    /* look through Output for InChar */
    for (i=0; i<StrLen(Output); i++) {
      if (Output[i] == InChar) {
        return(i);
      }
    }
    return(-1);
}


Seq_Mtf* LIBCALL CddCalcPSSM(SeqAlignPtr listOfSeqAligns, BioseqPtr bsp,
                             double Weight, double ScaleFactor) {
/*---------------------------------------------------------------------------*/
/* Generation of position-specific scoring matrices.                         */
/* Put results in a Seq_Mtf structure in preparation for calling atd().      */
/* allocate space for 2-d arrays pssm->ww and pssm->freq here.               */
/* pssm->ww[i][j] is pssm, where i is sequence index, j is residue index.    */
/* pssm->freqs[i][j] is table of residue frequencies.                        */
/*---------------------------------------------------------------------------*/
    posSearchItems      posSearch;           /*holds position-specific info  */
    compactSearchItems  compactSearch;
    ValNodePtr          LetterHead = NULL;
    Int4                i, j, jj;
    char*               Input;     /* order of residue-types from CD's */
    char*               Output;    /* order of residue-types needed for threading */
    Boolean*            Coverage;  /* for making sure all columns are filled */
    Int4                OutLen;
    Seq_Mtf*            pssm;

    /* allocate arrays for strings that determine input and output order */
    Input = (char*) MemNew(sizeof(InputOrder));
    Output = (char*) MemNew(sizeof(OutputOrder));
    StrCpy(Input, InputOrder);
    StrCpy(Output, OutputOrder);
    
    OutLen = StrLen(Output);
    CddCposCompPart1(listOfSeqAligns, bsp, &compactSearch, &LetterHead, &posSearch);

    /* construct new pssm */
    pssm = NewSeqMtf(compactSearch.qlength, OutLen);
    Coverage = (Boolean*) MemNew(sizeof(Boolean) * OutLen);  /* set to 0 by MemNew */
    
    /* copy results to pssm */
    for (j=0; j<compactSearch.alphabetSize;j++) {
      jj = CddGetNewIndexForThreading(Input[j], Output);
      if (jj != -1) {
        for (i=0; i<compactSearch.qlength;i++) {
          pssm->ww[i][jj] = ThrdRound(posSearch.posMatrix[i][j]*ScaleFactor*Weight);
          pssm->freqs[i][jj] = ThrdRound(posSearch.posFreqs[i][j]*ScaleFactor);
          Coverage[jj] = TRUE;
        }
      }
    }

    /* make sure all columns are filled */
    for (i=0; i<OutLen; i++) {
      if (Coverage[i] == FALSE) {
        ASSERT(FALSE);  /* should never get here */
      }
    }

    /* clean up */
    MemFree(Input);
    MemFree(Output);
    MemFree(Coverage);
    CddposFreeMemory(&posSearch);

    return(pssm);
}


void LIBCALL CddCposComp(SeqAlignPtr listOfSeqAligns, BioseqPtr bsp, CddPtr pcdd) {
/*---------------------------------------------------------------------------*/
/* Generation of position-specific scoring matrices for database scanning    */
/*---------------------------------------------------------------------------*/
    posSearchItems      posSearch;           /*holds position-specific info  */
    compactSearchItems  compactSearch;
    Int4                index, a, iNew;
    ValNodePtr          error_return = NULL;
    ValNodePtr          ColumnHead;
    ValNodePtr          newRow, RowHead;
    ValNodePtr          LetterHead = NULL;
    Char                ckptFileName[PATH_MAX];
    Char                cseqFileName[PATH_MAX];
    FILE                *fp;
    CharPtr             cAccession;

    CddCposCompPart1(listOfSeqAligns, bsp, &compactSearch, &LetterHead, &posSearch);

    /* put results in proper format... */

    pcdd->posfreq = (MatrixPtr) MatrixNew();
    pcdd->posfreq->ncolumns = compactSearch.qlength;
    pcdd->posfreq->nrows = compactSearch.alphabetSize;
    pcdd->posfreq->scale_factor = POSFREQ_SCALE;
    pcdd->posfreq->row_labels = LetterHead;    
    ColumnHead = NULL;
    for (index = 0; index<compactSearch.qlength;index++) {
      RowHead = NULL;
      for (a=0;a<compactSearch.alphabetSize;a++) {
         newRow = ValNodeNew(NULL);
         iNew = (Int4) (0.5 + (Nlm_FloatHi) pcdd->posfreq->scale_factor * posSearch.posFreqs[index][a]);
         newRow->data.intvalue = iNew;
           ValNodeLink(&(ColumnHead),newRow);
      }
    }
    pcdd->posfreq->columns = ColumnHead;

    pcdd->scoremat = (MatrixPtr) MatrixNew();
    pcdd->scoremat->ncolumns = compactSearch.qlength;
    pcdd->scoremat->nrows = compactSearch.alphabetSize;
    pcdd->scoremat->scale_factor = 1;
    pcdd->scoremat->row_labels = LetterHead;    
    ColumnHead = NULL;
    for (index = 0; index<compactSearch.qlength;index++) {
      RowHead = NULL;
      for (a=0;a<compactSearch.alphabetSize;a++) {
         newRow = ValNodeNew(NULL);
         iNew = (Int4) posSearch.posMatrix[index][a];
         newRow->data.intvalue = iNew;
           ValNodeLink(&(ColumnHead),newRow);
      }
    }
    pcdd->scoremat->columns = ColumnHead;

/*---------------------------------------------------------------------------*/
/* Construct name for checkpoint file                                        */
/*---------------------------------------------------------------------------*/
    cAccession = CddGetAccession(pcdd);
    strcpy(ckptFileName,cAccession);
    strcat(ckptFileName,CKPTEXT);

    CddposTakeCheckpoint(&posSearch, &compactSearch, ckptFileName, &error_return);
    strcpy(cseqFileName,cAccession);
    strcat(cseqFileName,CSEQEXT);

    MemFree(cAccession);
  
    fp = FileOpen(cseqFileName, "w");
    if (NULL == fp) {
      BlastConstructErrorMessage("CddCposComp", "Could not open fasta file", 1, &error_return);
    }
    BioseqToFasta(bsp,fp,FALSE);
    FileClose(fp);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* modified after PGPReadBlastOptions from blastpgp.c, as of 12/27/00        */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static PGPBlastOptionsPtr CddReadBlastOptions(BioseqPtr bsp, Int4 iPseudo, CharPtr matrix_name)
{
    PGPBlastOptionsPtr  bop;
    BLAST_OptionsBlkPtr options;

    bop = MemNew(sizeof(PGPBlastOptions));
    bop->blast_database   = StringSave("nr");
    bop->number_of_descriptions = 500;
    bop->number_of_alignments = 500;
    bop->believe_query = FALSE;
    options = BLASTOptionNew("blastp",TRUE);                /* gapped Blast! */
    bop->options = options;
    bop->query_bsp = bsp; 
    BLASTOptionSetGapParams(options, matrix_name, 0, 0);
    /* bop->is_xml_output = FALSE; */
    bop->align_options = 0;
    bop->print_options = 0;
    options->required_start = 0;
    options->required_end = -1;
    options->window_size = 40;
    options->dropoff_2nd_pass  = 7.0;
    options->expect_value  = 10.0;
    options->hitlist_size = 500;
    options->two_pass_method  = FALSE;
    options->multiple_hits_only  = TRUE;
    if (strcmp(matrix_name,"BLOSUM62") == 0) {
      options->gap_open = 11;
      options->gap_extend = 1;
    }
    options->decline_align = INT2_MAX;
    options->gap_x_dropoff = 15;
    options->gap_x_dropoff_final = 25;
    options->gap_trigger = 22.0;
    options->filter_string = StringSave("F");
    options->number_of_cpus = 1;
    options->isPatternSearch = FALSE;
    options->ethresh = 0.0;                  /* zeroed out, will not be used */
    options->pseudoCountConst = iPseudo;
    options->maxNumPasses = 1;
    options->hsp_range_max  = 0;
    options->block_width  = 20; /* Default value - previously '-L' parameter */
    options->tweak_parameters = TRUE;
    options->smith_waterman = FALSE;
    if (bop->options->tweak_parameters) {
      /*allows for extra matches in first pass of screening,
        hitlist_size */
      bop->options->original_expect_value = bop->options->expect_value;
      bop->options->hitlist_size *= 2; 
    }

    options = BLASTOptionValidate(options, "blastp");
    if (options == NULL) return NULL;
    bop->fake_bsp = bop->query_bsp;
#if 0
      BioseqNew();
    bop->fake_bsp->descr = bop->query_bsp->descr;
    bop->fake_bsp->repr = bop->query_bsp->repr;
    bop->fake_bsp->mol = bop->query_bsp->mol;
    bop->fake_bsp->length = bop->query_bsp->length;
    bop->fake_bsp->seq_data_type = bop->query_bsp->seq_data_type;
    bop->fake_bsp->seq_data = bop->query_bsp->seq_data;
    bop->fake_bsp->id = bop->query_bsp->id;
#endif
    /* bop->fake_bsp->seq_data = BSDup(bop->query_bsp->seq_data); */
    /* bop->fake_bsp->id = SeqIdDup(bop->query_bsp->id); */
    /* bug fix -- Lewis, Dave and Aron */
    /* bop->query_bsp->id = SeqIdSetFree(bop->query_bsp->id); */
    /* BLASTUpdateSeqIdInSeqInt(options->query_lcase_mask, bop->fake_bsp->id); */
    return bop;
}



/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* write out a matrix-file (ASCII formatted) for use with copymat/formatdb   */
/* routines from makemat, 4/9/01                                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/

static void CddputMatrixKbp(FILE *checkFile, BLAST_KarlinBlkPtr kbp,
                            Boolean scaling, Nlm_FloatHi scalingDown)
{
  if (scaling)
    fprintf(checkFile,"%le\n",kbp->Lambda * scalingDown);
  else
    fprintf(checkFile,"%le\n",kbp->Lambda);
  fprintf(checkFile,"%le\n",kbp->K);
  fprintf(checkFile,"%le\n",kbp->logK);
  fprintf(checkFile,"%le\n",kbp->H);
}

static void CddputMatrixMatrix(FILE *checkFile, 
                               compactSearchItems *compactSearch,
			       posSearchItems *posSearch,
			       Boolean scaleScores)
{
  Int4 i, j;                                                  /*loop indices*/

  if (scaleScores) {
    for(i = 0; i < compactSearch->qlength; i++) {
      for(j = 0; j < compactSearch->alphabetSize; j++)
        fprintf(checkFile,"%ld  ", (long) posSearch->posPrivateMatrix[i][j]);
      fprintf(checkFile,"\n");
    }
  }
  else {
    for(i = 0; i < compactSearch->qlength; i++) {
      for(j = 0; j < compactSearch->alphabetSize; j++)
        fprintf(checkFile,"%ld  ", (long) posSearch->posMatrix[i][j]);
      fprintf(checkFile,"\n");
    }
  }
}

static Boolean CddtakeMatrixCheckpoint(compactSearchItems *compactSearch,
                                       posSearchItems *posSearch,
				       BLAST_ScoreBlkPtr sbp, 
                                       Char *fileName,ValNodePtr *error_return,
				       Boolean scaleScores,
				       Nlm_FloatHi scalingFactor)
{
  FILE * checkFile;                  /* file in which to take the checkpoint */
  Int4 length;              /* length of query sequence, and an index for it */
  Int4 i;                                /* indices to position and alphabet */
  Char localChar;                                     /* temporary character */

  checkFile = FileOpen(fileName, "w");
  if (NULL == checkFile) {
    BlastConstructErrorMessage("posTakeCheckpoint", "Could not open checkpoint file", 1, error_return);
    return(FALSE);
  }
  length = compactSearch->qlength;
  fprintf(checkFile,"%ld\n",(long) length);
  for(i = 0; i < length; i++) {
    localChar = getRes(compactSearch->query[i]);
    fprintf(checkFile,"%c",localChar);
    posSearch->posMatrix[i][Xchar] = Xscore;
    posSearch->posPrivateMatrix[i][Xchar] = Xscore * scalingFactor;
  }  
  fprintf(checkFile,"\n");
  CddputMatrixKbp(checkFile, compactSearch->kbp_gap_std[0], scaleScores, 1/scalingFactor);
  CddputMatrixKbp(checkFile, compactSearch->kbp_gap_psi[0], scaleScores, 1/scalingFactor);
  CddputMatrixKbp(checkFile, sbp->kbp_ideal, scaleScores, 1/scalingFactor);
  CddputMatrixMatrix(checkFile, compactSearch, posSearch, scaleScores);
  FileClose(checkFile);
  return(TRUE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void BlastKarlinBlkCopy(BLAST_KarlinBlkPtr kbp_in,BLAST_KarlinBlkPtr kbp_out)
{ 
  kbp_out->Lambda = kbp_in->Lambda;
  kbp_out->K = kbp_in->K;
  kbp_out->logK = kbp_in->logK;
  kbp_out->H = kbp_in->H;
/*  kbp_out->Lambda_real = kbp_in->Lambda_real;
  kbp_out->K_real = kbp_in->K_real;
  kbp_out->logK_real = kbp_in->logK_real;
  kbp_out->H_real = kbp_in->H_real;
  kbp_out->q_frame = kbp_in->q_frame;
  kbp_out->s_frame = kbp_in->s_frame; */
  kbp_out->paramC = kbp_in->paramC;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* version of the PSSM calculation routine which does not return Karlin-     */
/* Altschul parameters                                                       */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Seq_Mtf * LIBCALL CddDenDiagCposComp2(BioseqPtr bspFake, Int4 iPseudo,
                                      SeqAlignPtr salp, CddPtr pcdd,
				      BioseqPtr bspOut, double Weight,
				      double ScaleFactor, CharPtr matrix_name)
{
  return CddDenDiagCposComp2KBP(bspFake, iPseudo, salp, pcdd, bspOut,
                                Weight, ScaleFactor, matrix_name, NULL);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* updated version of the PSSM calculation routine, modified from blastpgp.c */
/* as of 12/27/2000                                                          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Seq_Mtf * LIBCALL CddDenDiagCposComp2KBP(BioseqPtr bspFake, Int4 iPseudo,
                                         SeqAlignPtr salp, CddPtr pcdd,
				         BioseqPtr bspOut, double Weight,
				         double ScaleFactor, CharPtr matrix_name,
				         BLAST_KarlinBlkPtr kbp)
{
  PGPBlastOptionsPtr      bop;
  BlastSearchBlkPtr       search;
  posSearchItems         *posSearch = NULL;
  compactSearchItems     *compactSearch = NULL;
  ValNodePtr              ColumnHead, LetterHead, newRow, RowHead, newLetter;
  SeqCodeTablePtr         sctp;
  Int4                    numSeqs, numASeqs, alignLength, index, a, iNew, i;
  Int4                    scale_factor, j, OutLen, jj;
  Char                    ckptFileName[PATH_MAX];
  Char                    cseqFileName[PATH_MAX];
  Char                    mtrxFileName[PATH_MAX];
  FILE                   *fp;
  Boolean                 bHasConsensus;
  Boolean                 bWriteOut = FALSE;
  Nlm_FloatHi            *AInf, SumAInf = 0.0;
  char*                   Input;    /* order of residue-types from CD's      */
  char*                   Output;   /*order of res-types needed for threading*/
  Boolean*                Coverage; /* for making sure all columns are filled*/
  Seq_Mtf*                pssm;
  CharPtr                 cAccession;
  Boolean                 scalingOkay = TRUE;

  cAccession = CddGetAccession(pcdd);
  if (pcdd) {
    bHasConsensus = CddHasConsensus(pcdd);
    bWriteOut = CddHasDescription(pcdd,"Write out matrix");
  } else {                                        /* determine from seqalign */
    bHasConsensus = SeqAlignHasConsensus(salp);
  }
  if (iPseudo <= 0) {                             /* need to determine first */
    AInf = SeqAlignInform(salp, bspFake, bHasConsensus, 0); 
    for (i=0;i<bspFake->length;i++) SumAInf += AInf[i];
    if      (SumAInf > 84  ) iPseudo = 10;             /* purely empirical   */
    else if (SumAInf > 55  ) iPseudo =  7;
    else if (SumAInf > 43  ) iPseudo =  5;
    else if (SumAInf > 41.5) iPseudo =  4;
    else if (SumAInf > 40  ) iPseudo =  3;
    else if (SumAInf > 39  ) iPseudo =  2;
    else                     iPseudo =  1;
    MemFree(AInf);
    if (pcdd && bWriteOut) printf("%s AInf:%6.1f PseudoCt: %d\n",cAccession,SumAInf, iPseudo);
  }
  if (NULL == matrix_name) {
    static CharPtr defaultMatrix = "BLOSUM62";
    matrix_name = defaultMatrix;
  }
  bop = (PGPBlastOptionsPtr) CddReadBlastOptions(bspFake, iPseudo, matrix_name);
  if (pcdd) {          /* set up search with nr database size for CD dumping */
    search = BLASTSetUpSearchWithReadDb(bop->fake_bsp, "blastp", 
                                        bop->query_bsp->length, 
                                        bop->blast_database,
                                        bop->options, NULL);
  } else {          /* or avoid using nr when calculating PSSM for threading */
    search = BLASTSetUpSearch(bop->fake_bsp, "blastp",
                              bop->query_bsp->length,
			      0, NULL, bop->options, NULL);
  }
  posSearch = NULL;
  compactSearch = NULL;
  posSearch = (posSearchItems *)MemNew(sizeof(posSearchItems));
  compactSearch = compactSearchNew(compactSearch);
  copySearchItems(compactSearch, search, bop->options->matrix);
  posInitializeInformation(posSearch,search);
  
/*---------------------------------------------------------------------------*/
/* section modelled after BposComputation, posit.c as of 12/28/2000          */
/*---------------------------------------------------------------------------*/
  search->posConverged = FALSE;
  numSeqs = CddCountDenDiagSeqAligns(salp, &numASeqs) + 1; 
  alignLength = bspFake->length;
  posAllocateMemory(posSearch, compactSearch->alphabetSize, 
                       compactSearch->qlength, numSeqs);
  CddposProcessAlignment(posSearch,compactSearch,salp,numSeqs,alignLength,bspFake);
  posSearch->posNumSequences = numSeqs;
  posPurgeMatches(posSearch, compactSearch);
  if (bHasConsensus) posCancel(posSearch,compactSearch,0,0,0,compactSearch->qlength);
  posComputeExtents(posSearch, compactSearch);
  posComputeSequenceWeights(posSearch, compactSearch, 1.0);
  posCheckWeights(posSearch, compactSearch);
/*  printf("Calling posComputePseudoFreqs with %s\n",compactSearch->standardMatrixName);
  fflush(stdout); */
  posSearch->posFreqs = posComputePseudoFreqs(posSearch,compactSearch,TRUE);
  if (NULL == search->sbp->posFreqs)
    search->sbp->posFreqs =  allocatePosFreqs(compactSearch->qlength, compactSearch->alphabetSize);
  copyPosFreqs(posSearch->posFreqs,search->sbp->posFreqs, compactSearch->qlength, compactSearch->alphabetSize);

/*---------------------------------------------------------------------------*/
/* Construct name for checkpoint file and write out (if in a CD context)     */
/*---------------------------------------------------------------------------*/
  if (pcdd && bWriteOut) {
    strcpy(ckptFileName,cAccession);
    strcat(ckptFileName,CKPTEXT);
    if (NULL != ckptFileName)
      posTakeCheckpoint(posSearch, compactSearch, ckptFileName, NULL);
  }

  posFreqsToMatrix(posSearch,compactSearch, NULL, 1);
/*---------------------------------------------------------------------------*/
/* kludge to set the matrix column for matches to * (stop codons) to the     */
/* most negative value used, so that matrix-file corresponds to what makemat */
/* produces.                                                                 */
/*---------------------------------------------------------------------------*/
  ASSERT(compactSearch->alphabetSize == 26);                 /* sanity check */
/*
  for (c=0;c<compactSearch->qlength;c++) {
    posSearch->posPrivateMatrix[c][compactSearch->alphabetSize-1] = BLAST_SCORE_MIN;
  }
*/
/*  posScaling(posSearch, compactSearch); */
  scalingOkay = impalaScaling(posSearch, compactSearch, 1.0, TRUE);
  if (!scalingOkay) goto cleanup;

  if (pcdd) {
/*---------------------------------------------------------------------------*/
/* also, write out the PSSM as an ASCII-formatted MATRIX file, for use with  */
/* copymat                                                                   */
/*---------------------------------------------------------------------------*/
/*    strcpy(mtrxFileName,cAccession);
    strcat(mtrxFileName,MTRXEXT);
    CddtakeMatrixCheckpoint(compactSearch,posSearch,search->sbp,mtrxFileName,
                            NULL,FALSE,1.0);
*/
/*---------------------------------------------------------------------------*/
/* in compliance with makemat, set the score for the X-column                */
/*---------------------------------------------------------------------------*/
    for (index = 0; index<compactSearch->qlength;index++) {
      posSearch->posMatrix[index][21] = Xscore;
    }
    sctp = SeqCodeTableFind(Seq_code_ncbistdaa);
    LetterHead = NULL;
    for (a=0;a<compactSearch->alphabetSize;a++) {
      newLetter = ValNodeNew(NULL);
      newLetter->data.ptrvalue = MemNew(2*sizeof(Char));
      Nlm_StrNCpy(newLetter->data.ptrvalue,&(sctp->letters[a]),1);
      ValNodeLink(&(LetterHead),newLetter);
    }

    pcdd->posfreq = (MatrixPtr) MatrixNew();
    pcdd->posfreq->ncolumns = compactSearch->qlength;
    pcdd->posfreq->nrows = compactSearch->alphabetSize;
    pcdd->posfreq->scale_factor = POSFREQ_SCALE;
    pcdd->posfreq->row_labels = LetterHead;    
    ColumnHead = NULL;
    for (index = 0; index<compactSearch->qlength;index++) {
      RowHead = NULL;
      for (a=0;a<compactSearch->alphabetSize;a++) {
         newRow = ValNodeNew(NULL);
         iNew = (Int4) (0.5 + (Nlm_FloatHi) pcdd->posfreq->scale_factor * posSearch->posFreqs[index][a]);
         newRow->data.intvalue = iNew;
           ValNodeLink(&(ColumnHead),newRow);
      }
    }
    pcdd->posfreq->columns = ColumnHead;
    pcdd->scoremat = (MatrixPtr) MatrixNew();
    pcdd->scoremat->ncolumns = compactSearch->qlength;
    pcdd->scoremat->nrows = compactSearch->alphabetSize;
    pcdd->scoremat->scale_factor = 1;
    pcdd->scoremat->row_labels = LetterHead;    
    ColumnHead = NULL;
    for (index = 0; index<compactSearch->qlength;index++) {
      RowHead = NULL;
      for (a=0;a<compactSearch->alphabetSize;a++) {
         newRow = ValNodeNew(NULL);
         iNew = (Int4) posSearch->posMatrix[index][a];
         newRow->data.intvalue = iNew;
           ValNodeLink(&(ColumnHead),newRow);
      }
    }
    pcdd->scoremat->columns = ColumnHead;

  } else  { /* if pcdd is empty, fill in the Seq_Mtf data structure */
    Input = (char*) MemNew(sizeof(InputOrder));
    Output = (char*) MemNew(sizeof(OutputOrder));
    StrCpy(Input, InputOrder);
    StrCpy(Output, OutputOrder);
    OutLen = StrLen(Output);
    pssm = NewSeqMtf(compactSearch->qlength, OutLen);
    Coverage = (Boolean*) MemNew(sizeof(Boolean) * OutLen);  /* set to 0 by MemNew */
    for (j=0; j<compactSearch->alphabetSize;j++) {
      jj = CddGetNewIndexForThreading(Input[j], Output);
      if (jj != -1) {
        for (i=0; i<compactSearch->qlength;i++) {
          pssm->ww[i][jj] = ThrdRound(posSearch->posMatrix[i][j]*ScaleFactor*Weight);
          pssm->freqs[i][jj] = ThrdRound(posSearch->posFreqs[i][j]*ScaleFactor);
          Coverage[jj] = TRUE;
        }
      }
    }
    for (i=0; i<OutLen; i++) {
      if (Coverage[i] == FALSE) {
        ASSERT(FALSE);  /* should never get here */
      }
    }
    MemFree(Input);
    MemFree(Output);
    MemFree(Coverage);
  }

  if (bspOut && pcdd && bWriteOut) {
    strcpy(cseqFileName,cAccession);
    strcat(cseqFileName,CSEQEXT);
    fp = FileOpen(cseqFileName, "w");
    if (NULL == fp) {
      BlastConstructErrorMessage("CddDenDiagCposComp2", "Could not open fasta file", 1, NULL);
    }
    BioseqToFasta(bspOut,fp,FALSE);
    FileClose(fp);
  }

/*---------------------------------------------------------------------------*/
/* clean up                                                                  */
/*---------------------------------------------------------------------------*/
cleanup:
  MemFree(cAccession);
  posSearch->NumSequences = posSearch->posNumSequences;
  posSearch->QuerySize = alignLength;
  CddposFreeMemory(posSearch);
  MemFree(posSearch);
  if (kbp) {
    BlastKarlinBlkCopy(*(compactSearch->kbp_gap_psi),kbp);
  }
  CddposFreeMemory2(compactSearch);
  MemFree(compactSearch);
  search = BlastSearchBlkDestruct(search);
  bop->options = BLASTOptionDelete(bop->options);
  MemFree(bop->blast_database);
  MemFree(bop);
  if (NULL == pcdd && scalingOkay) {  /* return pssm if from within threader */
    return(pssm);
  }
  if (NULL != pcdd && !scalingOkay) { /* fields are empty if scaling failed  */
    pcdd->posfreq = NULL;
    pcdd->scoremat = NULL;
  }
  return NULL;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Free a Cdd data structure - works on a single CD only (not linked lists)  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
CddPtr LIBCALL CddFreeCarefully(CddPtr pcdd)
{
  BioseqPtr    bsp;
  BioseqSetPtr bssp;
  ScorePtr     nxtscore, thisscore;
  SeqAnnotPtr  sap, sapThis;
  SeqEntryPtr  sep, sepThis;
  ValNodePtr   vnp, vnpThis;
  
  if(pcdd == NULL) return NULL;
  if (NULL != pcdd->name) pcdd->name = MemFree(pcdd->name);
  if (NULL != pcdd->id)   pcdd->id   = (CddIdSetPtr) CddIdSetFree(pcdd->id);

  sap = pcdd->seqannot;
  while (sap) {
    sapThis = sap->next;
    sap = SeqAnnotFree(sap);
    sap = sapThis;
  }

  vnp = pcdd->description;
  while (vnp) {
    vnpThis = vnp->next;
    switch (vnp->choice) {
      case CddDescr_othername:
        vnp->data.ptrvalue = MemFree(vnp->data.ptrvalue);
	break;
      case CddDescr_category:
        vnp->data.ptrvalue = MemFree(vnp->data.ptrvalue);
	break;
      case CddDescr_comment:
        vnp->data.ptrvalue = MemFree(vnp->data.ptrvalue);
	break;
      case CddDescr_reference:
        vnp->data.ptrvalue = PubFree(vnp->data.ptrvalue);
	break;
      case CddDescr_create_date:
        vnp->data.ptrvalue = DateFree(vnp->data.ptrvalue);
	break;
      case CddDescr_tax_source:
        vnp->data.ptrvalue = OrgRefFree(vnp->data.ptrvalue);
	break;
      case CddDescr_source:
        vnp->data.ptrvalue = MemFree(vnp->data.ptrvalue);
	break;
      default:
	break;
    }
    vnp = MemFree(vnp);
    vnp = vnpThis;
  }

  if (NULL != pcdd->features) {
    pcdd->features = BiostrucAnnotSetFree(pcdd->features);
  }

  if (NULL != pcdd->sequences) {
    bssp = pcdd->sequences->data.ptrvalue;
    if (NULL != bssp) {
      sep = bssp->seq_set;
      while (sep) {
        sepThis = sep->next;
	if (sep->choice == 1) {
          bsp = sep->data.ptrvalue;
	  if (bsp->id->choice != SEQID_LOCAL)BioseqUnlock(bsp);
	  BioseqFree(bsp);
        }
        MemFree(sep);
	sep = sepThis;
      }
      bssp = MemFree(bssp);
    }
    pcdd->sequences = ValNodeFree(pcdd->sequences);
  }

  if(NULL != pcdd->profile_range) {
    pcdd->profile_range = SeqIntFree(pcdd->profile_range);
  }
  if (NULL != pcdd->trunc_master) {
    pcdd->trunc_master = BioseqFree(pcdd->trunc_master);
  }
  if (NULL != pcdd->posfreq) {
    vnp = pcdd->posfreq->row_labels; if (NULL != vnp) ValNodeFree(vnp);
    vnp = pcdd->posfreq->columns;    if (NULL != vnp) ValNodeFree(vnp);
    pcdd->posfreq = MemFree(pcdd->posfreq);
  }
  if (NULL != pcdd->scoremat) {
    vnp = pcdd->scoremat->columns;    if (NULL != vnp) ValNodeFree(vnp);
    pcdd->scoremat = MemFree(pcdd->scoremat);
  }
  if (NULL != pcdd->distance) {
    nxtscore = pcdd->distance->scores;
    while (nxtscore) {
      if (NULL != nxtscore->id) ObjectIdFree(nxtscore->id);
      thisscore = nxtscore;
      nxtscore = nxtscore->next;
      thisscore = MemFree(thisscore);
    }
    pcdd->distance = MemFree(pcdd->distance);
  }
  return MemFree(pcdd);
}

/*---------------------------------------------------------------------------*/
/* Translate a SeqLoc into an Integer list of residue numbers                */
/*---------------------------------------------------------------------------*/
Int4Ptr LIBCALL CddGetFeatLocList(SeqLocPtr location, Int4 *nres)
{
  SeqLocPtr     slp;
  SeqIntPtr     sintp;
  PackSeqPntPtr pspp;
  SeqPntPtr     spp;
  Int4Ptr       presnum = NULL;
  Int4          i;
  Int4          nSubLoc;
  Int4Ptr       pSubLoc;

  presnum = MemNew(100 * sizeof(Int4));
  *nres = 0;
  if (!location) return NULL;
  switch (location->choice) {
    case SEQLOC_NULL:
    case SEQLOC_FEAT:
    case SEQLOC_BOND:
    case SEQLOC_EMPTY:
    case SEQLOC_WHOLE:
      break;
    case SEQLOC_INT:
      sintp = (SeqIntPtr) location->data.ptrvalue;
      for (i=sintp->from;i<=sintp->to;i++) {
        presnum[*nres] = i;
	(*nres)++; if (*nres >= 100) presnum = Nlm_MemMore(presnum,((*nres)+1)*sizeof(Int4));
      }
      break;
    case SEQLOC_PNT:
      spp = location->data.ptrvalue;
      presnum[*nres] = spp->point;
	(*nres)++; if (*nres >= 100) presnum = Nlm_MemMore(presnum,((*nres)+1)*sizeof(Int4));
      break;
    case SEQLOC_PACKED_PNT:
      pspp = location->data.ptrvalue;
      while (pspp) {
	for (i=0;i<pspp->used;i++) {
	  presnum[*nres] = pspp->pnts[i];
	  (*nres)++; if (*nres >= 100) presnum = Nlm_MemMore(presnum,((*nres)+1)*sizeof(Int4));
	}
        pspp = pspp->next;
      }
      break;
    case SEQLOC_PACKED_INT:
    case SEQLOC_MIX:
    case SEQLOC_EQUIV:
      slp = (SeqLocPtr)location->data.ptrvalue;
      while (slp) {
        pSubLoc = CddGetFeatLocList(slp, &nSubLoc);
        for (i=0;i<nSubLoc;i++) {
	  presnum[*nres] = pSubLoc[i];
	  (*nres)++; if (*nres >= 100) presnum = Nlm_MemMore(presnum,((*nres)+1)*sizeof(Int4));
	}	
        slp = slp->next;
      }      
    default:
      break;
  }
  return(presnum);
}

/*---------------------------------------------------------------------------*/
/* check for violations of an interval defined on the aligned master with    */
/* respect to aligned residues. Returns -1 if everything is OK, returns      */
/* the first location on the master (counting from 0) if a violation is      */
/* detected, returns INT4_MAX if negative interval numbers are encountered   */
/*---------------------------------------------------------------------------*/
static Int4 CddSeqLocInExpAlign(SeqLocPtr location, CddExpAlignPtr eap)
{
  SeqIntPtr     sintp;
  SeqPntPtr     spp;
  PackSeqPntPtr pspp;
  SeqLocPtr     slp;
  Int4          i;

  if (!location) return(-1);
  switch (location->choice) {
    case SEQLOC_NULL:
    case SEQLOC_FEAT:
    case SEQLOC_BOND:
    case SEQLOC_EMPTY:
    case SEQLOC_WHOLE:
      break;
    case SEQLOC_INT:
      sintp = (SeqIntPtr) location->data.ptrvalue;
      if (sintp->from < 0 || sintp->to < 0) return(INT4_MAX);
      for (i=sintp->from;i<=sintp->to;i++) {
        if (eap->adata[i] < 0) return(i);
      }
      break;
    case SEQLOC_PNT:
      spp = location->data.ptrvalue;
      if (spp->point < 0) return(INT4_MAX);
      if (eap->adata[spp->point] < 0) return(spp->point);
      break;
    case SEQLOC_PACKED_PNT:
      pspp = location->data.ptrvalue;
      while (pspp) {
	for (i=0;i<pspp->used;i++) {
          if (pspp->pnts[i] < 0) return (INT4_MAX);
          if (eap->adata[pspp->pnts[i]] < 0) return(pspp->pnts[i]);
	}
        pspp = pspp->next;
      }
      break;
    case SEQLOC_PACKED_INT:
    case SEQLOC_MIX:
    case SEQLOC_EQUIV:
      slp = (SeqLocPtr)location->data.ptrvalue;
      while (slp) {
        i = CddSeqLocInExpAlign(slp,eap);
        if (i >= 0) return(i);
        slp = slp->next;
      }      
    default:
      break;
  }
  return(-1);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
static void CddRelocateSeqLoc(SeqLocPtr location, SeqIdPtr sip, Int4 *ali)
{
  SeqIntPtr     sintp;
  SeqPntPtr     spp;
  PackSeqPntPtr pspp;
  SeqLocPtr     slp;
  Int4          i;
  
  if (!location) return;
  switch (location->choice) {
    case SEQLOC_NULL:
    case SEQLOC_FEAT:
    case SEQLOC_BOND:
      break;
    case SEQLOC_EMPTY:
    case SEQLOC_WHOLE:
      SeqIdFree(location->data.ptrvalue);
      location->data.ptrvalue = (SeqIdPtr) SeqIdDup(sip);
      break;
    case SEQLOC_INT:
      sintp = (SeqIntPtr) location->data.ptrvalue;
      SeqIdFree(sintp->id);
      sintp->id = SeqIdDup(sip);
      sintp->from = ali[sintp->from];
      sintp->to   = ali[sintp->to];
      if (sintp->from < 0 || sintp->to < 0) CddSevError("Invalid SeqLoc Interval!");
      break;
    case SEQLOC_PNT:
      spp = location->data.ptrvalue;
      SeqIdFree(spp->id);
      spp->id = (SeqIdPtr) SeqIdDup(sip);
      spp->point = ali[spp->point];
      if (spp->point < 0) CddSevError("Invalid SeqLoc Interval!");
      break;
    case SEQLOC_PACKED_PNT:
      pspp = location->data.ptrvalue;
      while (pspp) {
        SeqIdFree(pspp->id);
        pspp->id = (SeqIdPtr) SeqIdDup(sip);
	for (i=0;i<pspp->used;i++) {
          pspp->pnts[i] = ali[pspp->pnts[i]];	
          if (pspp->pnts[i] < 0) CddSevError("Invalid SeqLoc Interval!");
	}
        pspp = pspp->next;
      }
      break;
    case SEQLOC_PACKED_INT:
    case SEQLOC_MIX:
    case SEQLOC_EQUIV:
      slp = (SeqLocPtr)location->data.ptrvalue;
      while (slp) {
        CddRelocateSeqLoc(slp,sip,ali);
        slp = slp->next;
      }      
    default:
      break;
  }
}

static SeqIdPtr CddFindSeqIdInAlignAnnot(AlignAnnotPtr oldannot)
{
  SeqLocPtr location;
  location = oldannot->location;
  return SeqLocId(location);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* transfer alignment annotations from the current master to another         */
/* sequence (before reindexing the alignment to this new master)             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddTransferAlignAnnot(AlignAnnotPtr oldannot,
                                            SeqIdPtr newMaster,
					    SeqAlignPtr salp,
					    BioseqSetPtr bssp)
{
  CddExpAlignPtr    pCDeaO= NULL, pCDeaN = NULL, pCDeaON = NULL;
  SeqLocPtr         slp;
  SeqIdPtr          oldMaster, sip, sipTmp;
  Boolean           bOldIsTop = FALSE;
  Boolean           bNewIsTop = FALSE;
  DenseDiagPtr      ddp;
  SeqAlignPtr       salpThis;
  BioseqPtr         bsp;
  SeqEntryPtr       sep;
  AlignAnnotPtr     aap;
  
  slp = (SeqLocPtr) oldannot->location;
  oldMaster = CddFindSeqIdInAlignAnnot(oldannot);
  if (oldMaster->choice == SEQID_GI) {
    sep = bssp->seq_set; while (sep) {
      if (sep->choice == 1) {
        bsp = sep->data.ptrvalue;
	if (CddSameSip(oldMaster, bsp->id)) {
	  sipTmp = bsp->id;
	  while (sipTmp) {
            if (sipTmp->choice == SEQID_PDB) {
	      oldMaster = sipTmp;
	      break;	  
	    }
	    sipTmp = sipTmp->next;
	  }
	  break;
	}
      }
      sep = sep->next;
    }
  }
  ddp = salp->segs;
  sip = SeqIdDup(ddp->id);  SeqIdFree(sip->next);
  if (CddSameSip(oldMaster,sip)) bOldIsTop = TRUE;
  if (CddSameSip(newMaster,sip)) bNewIsTop = TRUE;
  SeqIdFree(sip);
  salpThis = salp; while (salpThis) {
    ddp = salpThis->segs;
    if (CddSameSip(oldMaster,ddp->id->next)) {
      if (bNewIsTop) {
        pCDeaON = SeqAlignToCddExpAlign(salpThis,bssp->seq_set);
	break;
      } else {
        pCDeaO = SeqAlignToCddExpAlign(salpThis,bssp->seq_set);
	if (pCDeaN) break;
      }
    }
    if (CddSameSip(newMaster,ddp->id->next)) {
      if (bOldIsTop) {
        pCDeaON = SeqAlignToCddExpAlign(salpThis,bssp->seq_set);
        break;
      } else {
        pCDeaN = SeqAlignToCddExpAlign(salpThis,bssp->seq_set);
	if (pCDeaO) break;
      }
    }
    salpThis = salpThis->next;
  }
  if (!pCDeaON) {                                     /* no direct relation */
    bsp = CddRetrieveBioseqById(oldMaster,bssp->seq_set);
    pCDeaON = CddReindexExpAlign(pCDeaO, bsp->length, pCDeaN, 0, 1);
    if (bNewIsTop) {
    
    }
  }
  if (bNewIsTop) {                             /* need to revert alignment  */
    pCDeaON = InvertCddExpAlign(pCDeaON,bssp->seq_set);
  }
  if (!pCDeaON) CddSevError("Error in CDDTransferAlignAnnot, can not locate alignment data!");
  aap = oldannot; while (aap) {
    slp = aap->location;
    while (slp) {
      CddRelocateSeqLoc(slp,newMaster,pCDeaON->adata);
      slp = slp->next;
    }
    aap = aap->next;
  }
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* reindex a SeqAlign (a linked list of pairwise Den-Diag Master-Slave Seq-  */
/* aligns) to a new Master, pointed out by it's SeqId. Will pick the first   */
/* instance of that SeqId in the alignment, and return a SeqAlign of the     */
/* same format as the input was                                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAlignPtr LIBCALL CddReindexSeqAlign(SeqAlignPtr salp, SeqIdPtr sipMaster,
                                       BioseqSetPtr bssp)
{
  SeqAlignPtr    salpThis, salpNew, salpHead, salpTail;
  DenseDiagPtr   ddp;
  CddExpAlignPtr pCDea, pCDea1, pCDeaR;
  Int4           iNotThis = -1, i = -1, mlength;
  BioseqPtr      bsp;

  bsp = CddRetrieveBioseqById(sipMaster, bssp->seq_set);
  if (!bsp) return NULL;
  mlength = bsp->length;
  salpThis = salp; while(salpThis) {
    iNotThis++;
    ddp = salpThis->segs;
    if (CddSameSip(sipMaster,ddp->id->next)) break;
    else salpThis = salpThis->next; 
  }
  pCDea1 = (CddExpAlignPtr) SeqAlignToCddExpAlign(salpThis,bssp->seq_set);
  pCDeaR = (CddExpAlignPtr) InvertCddExpAlign(pCDea1, bssp->seq_set);
  salpNew = (SeqAlignPtr) CddExpAlignToSeqAlign(pCDeaR,NULL);
  pCDeaR = CddExpAlignFree(pCDeaR);
  salpTail = salpNew;
  salpHead = salp;
  while (salpHead) {
    i++; 
    if (i!= iNotThis) {    
      pCDea = (CddExpAlignPtr) SeqAlignToCddExpAlign(salpHead, bssp->seq_set);
      pCDeaR = (CddExpAlignPtr) CddReindexExpAlign(pCDea1,mlength,pCDea,0,i);
      salpThis = (SeqAlignPtr) CddExpAlignToSeqAlign(pCDeaR,NULL);
      pCDeaR = CddExpAlignFree(pCDeaR);
      pCDea = CddExpAlignFree(pCDea);
      salpTail->next = salpThis;
      salpTail = salpThis;
    }
    salpHead = salpHead->next;
  }
  
  pCDea1 = CddExpAlignFree(pCDea1);
  return (salpNew);
}
/********************************************************************/
/* Remove all description/annotation info from the bioseq,	    */
/* except for tax and mmdb info.				    */
/********************************************************************/
void LIBCALL CddShrinkBioseq(BioseqPtr bsp)
{
  SeqDescrPtr next, sdp, sdp2              = NULL;
  SeqAnnotPtr sap, sap_next, thissap, sap2 = NULL;

  sdp=bsp->descr;
  while (sdp != NULL) {
    next = sdp->next;
    sdp->next = NULL;
    if(sdp->choice == Seq_descr_title ||
      sdp->choice == Seq_descr_org ||
      sdp->choice == Seq_descr_pdb ||
      sdp->choice == Seq_descr_source) {
        ValNodeLink(&sdp2, sdp);
      } else SeqDescFree(sdp);
    sdp = next;
  }
  bsp->descr = sdp2;

  sap = bsp->annot;
  while (sap != NULL) {
    sap_next = sap->next;
    if ((sap->type == 4 && ((SeqIdPtr)sap->data)->choice == SEQID_GENERAL
         && Nlm_StrCmp(((DbtagPtr)((SeqIdPtr)sap->data)->data.ptrvalue)->db,"mmdb") == 0)
	|| (sap->type == 1 && ((SeqFeatPtr)sap->data)->data.choice == SEQFEAT_PROT)) {
      if (!sap2) {
        sap2 = sap;
      } else {
        thissap = sap2; while (thissap->next) thissap = thissap->next;
        thissap->next = sap;
      }
      sap->next = NULL;
    } else SeqAnnotFree(sap);
    sap = sap_next;
  }
  bsp->annot = sap2;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Make a list of m-s seq-aligns proper, aligning only columns	             */
/* found in all pairs.						             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Int2 LIBCALL CddTrimSeqAligns(CddPtr pcdd)
{
    CddExpAlignPtr	ceap,tceap;
    SeqAlignPtr		salp,salp2,head,head2=NULL;
    SeqEntryPtr		sep;
    ValNodePtr		vnp,vhead=NULL;
    Int2		i;
    Int4Ptr             iBreakAfter;
    DenseDiagPtr        ddp;

    if(pcdd == NULL) return 1;
    sep = ((BioseqSetPtr)pcdd->sequences->data.ptrvalue)->seq_set;
    head = pcdd->seqannot->data;
    if(head == NULL /*|| head->next == NULL */) return 1;

    tceap = (CddExpAlignPtr)SeqAlignToCddExpAlign(head, sep);
    iBreakAfter = MemNew(tceap->length * sizeof(Int4));
/*---------------------------------------------------------------------------*/
/* record existing block structure to make sure adjacent blocks aren't merged*/
/*---------------------------------------------------------------------------*/
    for (i=0;i<tceap->length;i++) iBreakAfter[i] = 0;
    ddp = head->segs;
    while (ddp) {
      iBreakAfter[ddp->starts[0]+ddp->len-1] = 1;
      ddp = ddp->next;
    }    
    for(salp=head; salp; salp=salp->next) {
      ceap = (CddExpAlignPtr)SeqAlignToCddExpAlign(salp, sep);
      ValNodeAddPointer(&vhead, 0, ceap);
      if(SeqIdComp(tceap->ids,ceap->ids) != SIC_YES) {
	for(vnp=vhead; vnp; vnp=vnp->next) {
	  ceap = vnp->data.ptrvalue;
	  ceap->ids = SeqIdSetFree(ceap->ids);
	  CddExpAlignFree(ceap);
	}
	tceap->ids = SeqIdSetFree(tceap->ids);
	CddExpAlignFree(tceap);
	ErrPostEx(SEV_WARNING,0,0,"Not master/slave alignments");
	return 1;
      }
      for(i=0; i<tceap->length; i++) {
	if(ceap->adata[i] == -1) { 
	  tceap->adata[i] = -1;
	  if (i > 0) { 
	    if (ceap->adata[i-1] != -1)  iBreakAfter[i-1] = 1;
	    if (tceap->adata[i-1] != -1) iBreakAfter[i-1] = 1;
          }
	} else {
	  if (i > 0) {
	    if (tceap->adata[i-1] != -1 && (tceap->adata[i]-tceap->adata[i-1])>1)
	      iBreakAfter[i-1] = 1;
	    if (ceap->adata[i-1] != -1 && (ceap->adata[i]-ceap->adata[i-1])>1)
	      iBreakAfter[i-1] = 1;
	  }
	}
      }
    }
    for(vnp=vhead; vnp; vnp=vnp->next) {
      ceap = vnp->data.ptrvalue;
      for(i=0; i<tceap->length; i++)
	if(tceap->adata[i] == -1) ceap->adata[i] = -1;
      if(head2) {
	salp2->next = (SeqAlignPtr)CddExpAlignToSeqAlign(ceap,iBreakAfter);
	salp2 = salp2->next;
      } else
	head2 = salp2 = (SeqAlignPtr)CddExpAlignToSeqAlign(ceap,iBreakAfter);
      ceap->ids = SeqIdSetFree(ceap->ids);
      CddExpAlignFree(ceap);
    }

    tceap->ids = SeqIdSetFree(tceap->ids);
    MemFree(iBreakAfter);
    CddExpAlignFree(tceap);
    ValNodeFree(vhead);
    SeqAlignListFree(pcdd->seqannot->data);
    pcdd->seqannot->data = head2;

    return 0;
}

/********************************************************************/
/* Make a list of m-s seq-aligns proper, aligning only columns	    */
/* found in all pairs.	If !modify, don't change seq-aligns, only   */
/* read them to populate iBreakAfter.				    */
/********************************************************************/
Int2 LIBCALL CddGetProperBlocks(CddPtr pcdd, Boolean modify,
				Int4Ptr *iBreakAfter)
{
    CddExpAlignPtr	ceap,tceap;
    SeqAlignPtr		salp,salp2,head,head2=NULL;
    SeqEntryPtr		sep;
    ValNodePtr		vnp,vhead=NULL;
    Int2		i;
    Int4Ptr             iBrkAft;

    if(pcdd == NULL) return 1;
    sep = ((BioseqSetPtr)pcdd->sequences->data.ptrvalue)->seq_set;
    head = pcdd->seqannot->data;
    if(head == NULL || head->next == NULL) return 1;

    tceap = (CddExpAlignPtr)SeqAlignToCddExpAlign(head, sep);
    iBrkAft = MemNew(tceap->length * sizeof(Int4));
    for (i=0;i<tceap->length;i++) iBrkAft[i] = 0;
    for(salp=head; salp; salp=salp->next) {
      ceap = (CddExpAlignPtr)SeqAlignToCddExpAlign(salp, sep);
      ValNodeAddPointer(&vhead, 0, ceap);
      if(SeqIdComp(tceap->ids,ceap->ids) != SIC_YES) {
	for(vnp=vhead; vnp; vnp=vnp->next) {
	  ceap = vnp->data.ptrvalue;
	  ceap->ids = SeqIdSetFree(ceap->ids);
	  CddExpAlignFree(ceap);
	}
	tceap->ids = SeqIdSetFree(tceap->ids);
	CddExpAlignFree(tceap);
	MemFree(iBrkAft);
	ErrPostEx(SEV_WARNING,0,0,"Not master/slave alignments");
	return 1;
      }
      for(i=0; i<tceap->length; i++) {
	if(ceap->adata[i] == -1) { 
	  tceap->adata[i] = -1;
	  if (i > 0) { 
	    if (ceap->adata[i-1] != -1)  iBrkAft[i-1] = 1;
	    if (tceap->adata[i-1] != -1) iBrkAft[i-1] = 1;
          }
	} else {
	  if (i > 0) {
	    if (tceap->adata[i-1] != -1 && (tceap->adata[i]-tceap->adata[i-1])>1)
	      iBrkAft[i-1] = 1;
	    if (ceap->adata[i-1] != -1 && (ceap->adata[i]-ceap->adata[i-1])>1)
	      iBrkAft[i-1] = 1;
	  }
	}
      }
    }
    if(modify) {
	for(vnp=vhead; vnp; vnp=vnp->next) {
	    ceap = vnp->data.ptrvalue;
	    for(i=0; i<tceap->length; i++)
		if(tceap->adata[i] == -1) ceap->adata[i] = -1;
	    if(head2) {
		salp2->next = (SeqAlignPtr)CddExpAlignToSeqAlign(ceap,iBrkAft);
		salp2 = salp2->next;
	    } else
		head2 = salp2 = (SeqAlignPtr)CddExpAlignToSeqAlign(ceap,iBrkAft);
	    ceap->ids = SeqIdSetFree(ceap->ids);
	    CddExpAlignFree(ceap);
	}
	SeqAlignListFree(pcdd->seqannot->data);
	pcdd->seqannot->data = head2;
    }

    if(iBreakAfter) *iBreakAfter = iBrkAft;
    else            MemFree(iBrkAft);
    tceap->ids = SeqIdSetFree(tceap->ids);
    CddExpAlignFree(tceap);
    ValNodeFree(vhead);

    return 0;
}

/*---------------------------------------------------------------------------*/
/* fix character string for export into XML                                  */
/*---------------------------------------------------------------------------*/
static CharPtr CddFixForXML(CharPtr pc)
{
  CharPtr pcNew;
  Int4    i = 0, j = 0;
  
  pcNew = MemNew(sizeof(Char) * (Nlm_StrLen(pc) + 100));
  
  while (pc[i] != '\0') {
    if (pc[i] == '>') {
      pcNew[j] = '&';
      pcNew[j+1] = 'g';
      pcNew[j+2] = 't';
      pcNew[j+3] = ';';    
      j += 4;
    } else if (pc[i] == '<') {
      pcNew[j] = '&';
      pcNew[j+1] = 'l';
      pcNew[j+2] = 't';
      pcNew[j+3] = ';';    
      j += 4;
    } else if (pc[i] == '&') {
      pcNew[j] = '&';
      pcNew[j+1] = 'a';
      pcNew[j+2] = 'm';
      pcNew[j+3] = 'p';    
      pcNew[j+4] = ';';    
      j += 5;
    } else {
      pcNew[j] = pc[i];
      j++;    
    }
    i++;
  }
  MemFree(pc);
  pcNew[j] = '\0';
  pcNew = Nlm_Realloc(pcNew, sizeof(Char) * Nlm_StrLen(pcNew));
  return(pcNew);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* dump out pubmed links for a CD to be used in Entrez neighboring           */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddDumpPMLinks(CddPtr pcdd, FILE *FP)
{
  CddIdPtr     cid;
  Int4         gi = 0, pmid = 0;
  ValNodePtr   pub, pdescr;

  cid = (CddIdPtr) pcdd->id;
  while (cid) {
    switch (cid->choice) {
      case CddId_uid:
        gi = cid->data.intvalue;
        break;
      default:
        break;
    }
    cid = cid->next;
  }
  if (gi > 0) {
    pdescr = pcdd->description;
    while (pdescr) {
      switch (pdescr->choice) {
        case CddDescr_reference:
          pub = pdescr->data.ptrvalue;
	  if (pub->choice == PUB_PMid) {
            pmid = pub->data.intvalue;
	    if (pmid > 0) {
	      fprintf(FP,"%d\t%d\t0\n",gi,pmid);
	    }
	  }
          break;
        default:
          break;
      }
      pdescr = pdescr->next;
    }
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* dump out taxonomy links for a CD to be used in Entrez neighboring         */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddDumpTaxLinks(CddPtr pcdd, FILE *FP)
{
  CddIdPtr     cid;
  Int4         txid = 0, gi = 0;
  DbtagPtr     dbtp;
  ObjectIdPtr  oidp;
  ValNodePtr   pdescr, vnp;
  OrgRefPtr    pOrgRef;

  cid = (CddIdPtr) pcdd->id;
  while (cid) {
    switch (cid->choice) {
      case CddId_uid:
        gi = cid->data.intvalue;
        break;
      default:
        break;
    }
    cid = cid->next;
  }
  if (gi > 0) {
    pdescr = pcdd->description;
    while (pdescr) {
      switch (pdescr->choice) {
        case CddDescr_tax_source:
          pOrgRef = (OrgRefPtr) pdescr->data.ptrvalue;
          vnp = pOrgRef->db;
	  while (vnp) {
	    dbtp = (DbtagPtr) vnp->data.ptrvalue;
	    if (dbtp && Nlm_StrCmp(dbtp->db,"taxon") == 0) {
	      oidp = dbtp->tag;
	      if (oidp && oidp->id >= 0) {
                fprintf(FP,"%d\t%d\t0\n",gi,oidp->id);	      
	      } 
	    }
	    vnp = vnp->next;
	  }
          break;
        default:
          break;
      }
      pdescr = pdescr->next;
    }
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* dump out contents of a CD used for Entrez-indexing in pseudo-XML          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddDumpXML(CddPtr pcdd, FILE *FP)
{
  ValNodePtr    pdescr, cid;
  CharPtr       cShortName  = NULL, cAbstract = NULL, cPubDate  = NULL;
  CharPtr       cEntrezDate = NULL, cFilter   = NULL, cOrganism = NULL;
  CharPtr       cAccession  = NULL, cDatabase = NULL;
  Int4          gi = 0, txid = -1;
  Boolean       bHaveAbstract = FALSE, bAllocDatabase = FALSE;
  GlobalIdPtr   pGid;
  OrgRefPtr     pOrgRef;
  DatePtr       pDate;
  ObjectIdPtr   oidp;
  ValNodePtr    vnp;
  DbtagPtr      dbtp;

  cEntrezDate = MemNew(11 * sizeof(Char));
  cid = (CddIdPtr) pcdd->id;
  while (cid) {
    switch (cid->choice) {
      case CddId_uid:
        gi = cid->data.intvalue;
        break;
      case CddId_gid:
        pGid = (GlobalIdPtr) cid->data.ptrvalue;
	cAccession = pGid->accession;      
        break;
      default:
        break;
    }
    cid = cid->next;
  }
  pdescr = pcdd->description;
  while (pdescr) {
    switch (pdescr->choice) {
      case CddDescr_comment:
        if (!bHaveAbstract) {
	  cAbstract = (CharPtr) pdescr->data.ptrvalue;
	  bHaveAbstract = TRUE;
	}
        break;
      case CddDescr_source:
        cDatabase = (CharPtr) pdescr->data.ptrvalue;
      case CddDescr_tax_source:
        pOrgRef = (OrgRefPtr) pdescr->data.ptrvalue;
	cOrganism = pOrgRef->taxname; 
	vnp = pOrgRef->db;
	while (vnp) {
	  dbtp = (DbtagPtr) vnp->data.ptrvalue;
	  if (dbtp && Nlm_StrCmp(dbtp->db,"taxon") == 0) {
	    oidp = dbtp->tag;
	    if (oidp && oidp->id >= 0) {
              txid = oidp->id;	      
	    } 
	  }
	  vnp = vnp->next;
	}
        break;
      case CddDescr_create_date:
      case CddDescr_update_date:
        cPubDate = MemNew(11 * sizeof(Char));
        pDate = (DatePtr) pdescr->data.ptrvalue;
	if (pDate->data[2] > 0 && pDate->data[3] > 0)
	  sprintf(cPubDate,"%d/%d/%d",pDate->data[1]+1900,pDate->data[2],pDate->data[3]);
        if (pDate->data[2] > 0 && pDate->data[3] == 0)
          sprintf(cPubDate,"%d/%d",pDate->data[1]+1900,pDate->data[2]);
        if (pDate->data[2] == 0 && pDate->data[3] == 0)
          sprintf(cPubDate,"%d",pDate->data[1]+1900);
        break;
      default:
        break;
    }
    pdescr = pdescr->next;
  }
  cShortName = (CharPtr) pcdd->name;
  if (cAccession) {
    if (Nlm_StrNCmp(cAccession,"cd",2) == 0) {
      cDatabase = StringSave("Cdd");
      bAllocDatabase = TRUE;
    }
  }
  if (!cPubDate) {
    cPubDate = MemNew(11 * sizeof(Char));
    pDate = (DatePtr) DateCurr();
    if (pDate->data[2] > 0 && pDate->data[3] > 0)
      sprintf(cPubDate,"%d/%d/%d",pDate->data[1]+1900,pDate->data[2],pDate->data[3]);
    if (pDate->data[2] > 0 && pDate->data[3] == 0)
      sprintf(cPubDate,"%d/%d",pDate->data[1]+1900,pDate->data[2]);
    if (pDate->data[2] == 0 && pDate->data[3] == 0)
      sprintf(cPubDate,"%d",pDate->data[1]+1900);
  }
  pDate = (DatePtr) DateCurr();
  if (pDate->data[2] > 0 && pDate->data[3] > 0)
    sprintf(cEntrezDate,"%d/%d/%d",pDate->data[1]+1900,pDate->data[2],pDate->data[3]);
  if (pDate->data[2] > 0 && pDate->data[3] == 0)
    sprintf(cEntrezDate,"%d/%d",pDate->data[1]+1900,pDate->data[2]);
  if (pDate->data[2] == 0 && pDate->data[3] == 0)
    sprintf(cEntrezDate,"%d",pDate->data[1]+1900);

  cAbstract = CddFixForXML(cAbstract);
  cShortName = CddFixForXML(cShortName);

  fprintf(FP,"    <IdxDocument>\n");  
  if (gi > 0) 
    fprintf(FP,"        <IdxUid>%d</IdxUid>\n",gi);
  fprintf(FP,"        <IdxKeyFields>\n");
  if (cPubDate)
    fprintf(FP,"            <PubDate>%s</PubDate>\n",cPubDate);  
  if (cEntrezDate)
    fprintf(FP,"            <EntrezDate>%s</EntrezDate>\n",cEntrezDate);  
  fprintf(FP,"        </IdxKeyFields>\n");
  fprintf(FP,"        <IdxDisplayFields>\n");
  if (cAccession)
    fprintf(FP,"            <Accession>%s</Accession>\n",cAccession);
  if (cShortName)
    fprintf(FP,"            <Title>%s</Title>\n",cShortName);
  if (cAbstract) 
    fprintf(FP,"            <Abstract>%s</Abstract>\n",cAbstract);
  fprintf(FP,"        </IdxDisplayFields>\n");
  fprintf(FP,"        <IdxSearchFields>\n");
  if (gi > 0) 
    fprintf(FP,"            <Uid>%d</Uid>\n",gi);
  if (cFilter)
    fprintf(FP,"            <Filter>%s</Filter>\n",cFilter);
  if (cAccession)
    fprintf(FP,"            <Accession>%s</Accession>\n",cAccession);
  if (cDatabase) 
    fprintf(FP,"            <Database>%s</Database>\n",cDatabase);
  if (cShortName)
    fprintf(FP,"            <Title>%s</Title>\n",cShortName);
  if (cAbstract) 
    fprintf(FP,"            <Text>%s</Text>\n",cAbstract);
  if (txid >= 0) 
    fprintf(FP,"            <Organism>txid%d</Organism>\n",txid);
  if (cOrganism)
    fprintf(FP,"            <Organism>%s</Organism>\n",cOrganism);
  if (cPubDate)
    fprintf(FP,"            <PubDate>%s</PubDate>\n",cPubDate);  
  if (cEntrezDate)
    fprintf(FP,"            <EntrezDate>%s</EntrezDate>\n",cEntrezDate);  
  fprintf(FP,"        </IdxSearchFields>\n");
  fprintf(FP,"    </IdxDocument>\n");  

  if (bAllocDatabase) MemFree(cDatabase);
  MemFree(cPubDate);
  MemFree(cEntrezDate);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* create and link a CddIdxData structure                                    */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
CddIdxDataPtr LIBCALL CddIdxDataNew(CharPtr acc, Int4 uid)
{

  CddIdxDataPtr     cidp;

  cidp = MemNew(sizeof(CddIdxData));

  if (acc) {
    cidp->cCDDid  = acc;
    cidp->iPssmId = uid;
  }
  cidp->next = NULL;
  return(cidp);
}

CddIdxDataPtr LIBCALL CddIdxDataLink(CddIdxDataPtr PNTR head, CddIdxDataPtr cidp)
{
  CddIdxDataPtr  cidpThis;

  cidpThis = *head;
  if (cidpThis) {
    while (cidpThis->next != NULL) cidpThis = cidpThis->next;
    cidpThis->next = cidp;
  } else {
    *head = cidp;
  }
  return *head;
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* read in the cdd.idx file to return data associating accessions with uids  */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
CddIdxDataPtr LIBCALL CddReadIdx(CharPtr CDDidx)
{
  CddIdxDataPtr cidp, cidpHead = NULL;
  Char          pcBuf[2048];
  CharPtr       pcTest;
  CharPtr       acc;
  Int4          uid;
  FILE         *fp;
  
  fp = FileOpen(CDDidx,"r");
  if (!fp) CddSevError("could not open cdd.idx!");
  do {
    pcBuf[0]='\0';
    pcTest = fgets(pcBuf, (size_t)2048, fp);
    if (pcTest) if (pcTest[0] != ' ') {
      uid = (Int4) atoi(Nlm_StrTok(pcTest," "));
      acc = (CharPtr) StringSave(Nlm_StrTok(NULL," "));
      if (acc) {
        acc[Nlm_StrLen(acc) - 1] = '\0';
        cidp = CddIdxDataNew(acc,uid);
        CddIdxDataLink(&(cidpHead),cidp);
      }
    }
  } while (pcTest);
  FileClose(fp);
  return(cidpHead);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* retrieve a CDD accession via the PSSMid or vice versa                     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddAccFromPssmId(Int4 iPssmId, CharPtr cAcc, CharPtr CDDidx)
{
  CddIdxDataPtr cidp;

  cidp = CddReadIdx(CDDidx);
  if (!cidp) CddSevError("cdd.idx read failed!");
  while (cidp) {
    if (iPssmId == cidp->iPssmId) {
      Nlm_StrCpy(cAcc, cidp->cCDDid);
      break;
    }
    cidp = cidp->next;
  }
}

void LIBCALL CddPssmIdFromAcc(Int4 *iPssmId, CharPtr cAcc, CharPtr CDDidx)
{
  CddIdxDataPtr cidp;

  cidp = CddReadIdx(CDDidx);
  if (!cidp) CddSevError("cdd.idx read failed!");
  while (cidp) {
    if (Nlm_StrCmp(cAcc, cidp->cCDDid) == 0) {
      *iPssmId = cidp->iPssmId;
      break;
    }
    cidp = cidp->next;
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* truncate a string right at the position of the first puncutation mark     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
void LIBCALL CddTruncStringAtFirstPunct(CharPtr pChar)
{
  Int4   i;
  
  for (i=0;i<Nlm_StrLen(pChar);i++) {
    if (pChar[i] == ',' ||
	pChar[i] == ';' ||
	pChar[i] == '.') {
      pChar[i] = '\0'; break;	
    }
  }
}

void LIBCALL CddFillBlanksInString(CharPtr pChar) 
{
  Int4   i;
  
  for (i=0;i<Nlm_StrLen(pChar);i++) {
    if (pChar[i] == ' ') pChar[i] = '_';
  }
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Remove a Consensus from a CD                                              */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddRemoveConsensus(CddPtr pcdd)
{
  SeqIdPtr     sip;
  BioseqPtr    bsp;
  SeqAlignPtr  salp, salpnew;
  DenseDiagPtr ddp;
  BioseqSetPtr bssp;
  SeqEntryPtr  sepThis, sepHead = NULL, sepTail = NULL;

  if (!CddHasConsensus(pcdd)) return FALSE;
  salp = (SeqAlignPtr)  pcdd->seqannot->data;
  ddp  = (DenseDiagPtr) salp->segs;
  sip  = (SeqIdPtr)     ddp->id->next;
  bssp = (BioseqSetPtr) pcdd->sequences->data.ptrvalue;
  if (pcdd->alignannot) {
    CddTransferAlignAnnot(pcdd->alignannot, sip, salp, bssp);
  }
  salpnew = CddReindexSeqAlign(salp,sip,bssp);
  pcdd->seqannot->data = salpnew->next;
  sepThis = bssp->seq_set;
  while (sepThis) {
    bsp = (BioseqPtr) sepThis->data.ptrvalue;
    if (!SipIsConsensus(bsp->id)) {
      if (!sepHead) sepHead = sepThis;
      if (sepTail) sepTail->next = sepThis;
      sepTail = sepThis;
    }
    sepThis = sepThis->next;
  }
  sepTail->next = NULL;
  bssp->seq_set = sepHead;
  pcdd->sequences->data.ptrvalue = bssp;
  pcdd->profile_range = NULL;
  pcdd->trunc_master = NULL;
  pcdd->posfreq = NULL;
  pcdd->scoremat = NULL;
  pcdd->distance = NULL;
  return TRUE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Check if a CD has annotation that is inconsistent with the alignment      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddFeaturesAreConsistent(CddPtr pcdd, CharPtr errmsg)
{
  SeqAlignPtr    salp;
  CddExpAlignPtr eap;
  BioseqSetPtr   bssp;
  AlignAnnotPtr  aap;
  SeqLocPtr      slp;
  Int4           iWhere;
  
  if (!pcdd->alignannot) return TRUE;
  bssp = (BioseqSetPtr) pcdd->sequences->data.ptrvalue;
  salp = (SeqAlignPtr) pcdd->seqannot->data;
  if (!salp) return TRUE;
  eap = SeqAlignToCddExpAlign(salp,bssp->seq_set);
  aap = pcdd->alignannot;
  while (aap) {
    slp = aap->location;
    iWhere = CddSeqLocInExpAlign(slp,eap);
    if (iWhere >= 0) {
      if (iWhere == INT4_MAX) {
        sprintf(errmsg,"Feature: %s, illegal feature address",aap->description);
      } else {
        sprintf(errmsg,"Feature: %s, Location: %d",aap->description,iWhere+1);
      }    
      return FALSE;
    }
    aap = aap->next;
  }
  CddExpAlignFree(eap);
  return TRUE;
  
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* find a MMDB-Identifier in a Bioseq                                        */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
SeqAnnotPtr LIBCALL CddFindMMDBIdInBioseq(BioseqPtr bsp, Int4 *iMMDBid)
{
  SeqAnnotPtr annot, prevannot = NULL;
  SeqIdPtr    sip;
  DbtagPtr    dbtp;
  ObjectIdPtr oidp;

  *iMMDBid = 0;
  annot = bsp->annot;
  while(annot) {
    if (annot->type == 4) {
      sip = annot->data;
      while (sip) {
        if (sip->choice == SEQID_GENERAL) {
          dbtp = sip->data.ptrvalue;
	  if (Nlm_StrCmp(dbtp->db,"mmdb") == 0) {
            oidp = dbtp->tag;
	    *iMMDBid = oidp->id;
	    return(prevannot);
	  }
        }
        sip = sip->next;
      }
    }
    if (annot->next) prevannot = annot;
    annot = annot->next;
  }
  return (NULL);
}


/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Check if a CD has 3D superposition information for all aligned chains     */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean LIBCALL CddHas3DSuperpos(CddPtr pcdd)
{
  BioseqSetPtr          bssp;
  BiostrucAnnotSetPtr   basp        = NULL;
  SeqAlignPtr           salp;
  DenseDiagPtr          ddp;
  SeqIdPtr              sip1, sip2;
  Int4Ptr               pMMDBid;
  Int4                  i, nStruct  = 0;
  BioseqPtr             bsp;
  BiostrucFeatureSetPtr bfsp;
  BiostrucFeaturePtr    bfp;
  ValNodePtr            location, vnp;
  ChemGraphAlignmentPtr cgap;
  Int4                  thisslave;

  pMMDBid = MemNew(250 * sizeof(Int4));
  basp = pcdd->features;
  if (!basp) return FALSE;
  bssp = (BioseqSetPtr) pcdd->sequences->data.ptrvalue;
/*---------------------------------------------------------------------------*/
/* create a list of MMDB-Id pairs which need to be checked                   */
/*---------------------------------------------------------------------------*/
  salp = pcdd->seqannot->data;
  while (salp) {
    ddp = salp->segs;
    sip1 = ddp->id;
	  sip2 = ddp->id->next;
    if (sip1->choice == SEQID_PDB) {
		  if (nStruct == 0) {
        bsp = CddFindSip(sip1,bssp->seq_set);
        CddFindMMDBIdInBioseq(bsp,&pMMDBid[nStruct]);
        nStruct++;
      }
    }
    if (sip2->choice == SEQID_PDB) {
      bsp = CddFindSip(sip2,bssp->seq_set);
      CddFindMMDBIdInBioseq(bsp,&pMMDBid[nStruct]);
      nStruct++;
    }
    salp = salp->next;
  }
/*---------------------------------------------------------------------------*/
/* now walk down the biostruc annot set and remove all pairs encountered     */
/*---------------------------------------------------------------------------*/
  bfsp = basp->features;
  while (bfsp) {
    bfp = bfsp->features;
    while (bfp) {
      location = bfp->Location_location;
      if (location->choice == Location_location_alignment) {
        cgap = location->data.ptrvalue;
	vnp = cgap->biostruc_ids;
        thisslave = vnp->next->data.intvalue;
        for (i=1;i<nStruct;i++) {
	  if (thisslave == pMMDBid[i]) {
            pMMDBid[i] = 0; break;  
	  }
	}
      }
      bfp = bfp->next;    
    }
    bfsp = bfsp->next;  
  }

/*---------------------------------------------------------------------------*/
/* at this point check if any pair is left (i.e. any mmdb-id except master)  */
/*---------------------------------------------------------------------------*/
  for (i=1;i<nStruct;i++) {
    if (pMMDBid[i]) {
      MemFree(pMMDBid); return(FALSE);
    }
  }
  MemFree(pMMDBid); return TRUE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* CD has unfinished alignment business                                      */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean    LIBCALL CddHasPendingAlignments(CddPtr pcdd)
{
  if (pcdd->pending) return TRUE;
  return FALSE;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* check 
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
Boolean LIBCALL SeqHasTax(BioseqPtr bsp)
{
  ValNodePtr  descr;

  descr = bsp->descr;
  while (descr) {
    if (descr->choice == Seq_descr_source) {
      return(TRUE);
    }
    descr = descr->next;
  }
  return(FALSE);
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* Fetch bioseq using BLAST dB, removing ids redundant to query.             */
/* index is the NR index in the dB -- if you don't have it, use -1.          */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
NLM_EXTERN BlastDefLineSetPtr LIBCALL BlastDefLineSetFree PROTO ((BlastDefLineSetPtr ));

BioseqPtr LIBCALL CddReadDBGetBioseq(SeqIdPtr query, Int4 index,
                                     ReadDBFILEPtr rdfp)
{
  return CddReadDBGetBioseqEx(query,index,rdfp,TRUE);
}

BioseqPtr LIBCALL CddReadDBGetBioseqEx(SeqIdPtr query, Int4 index,
				       ReadDBFILEPtr rdfp, Boolean bUseObjMgr)
{
    BioseqPtr		bsp;
    BlastDefLinePtr	bdp,tbdp;
    RDBTaxNamesPtr	tnames;
    Char		buf[64];

    if(query == NULL) return NULL;
    if(index < 0 && (index = SeqId2OrdinalId(rdfp, query)) < 0)
	return NULL;
    if(!(bsp = readdb_get_bioseq_ex(rdfp, index, bUseObjMgr, FALSE))) return NULL;
    if(!(tbdp = FDGetDeflineAsnFromBioseq(bsp))) return NULL;

    for(bdp=tbdp; bdp; bdp=bdp->next) {
	if(SeqIdComp(bdp->seqid,query) == SIC_YES ||
	   SeqIdComp(bdp->seqid->next,query) == SIC_YES) {
	    tnames = RDBGetTaxNames(rdfp->taxinfo, bdp->taxid);
	    break;
	}
    }
    if(bdp == NULL) {
	SeqIdPrint(query,buf,PRINTID_FASTA_SHORT);
	ErrPostEx(SEV_WARNING,0,0,"Bad seqid %s",buf);
	return NULL;
    }
    bsp->id = SeqIdSetFree(bsp->id);
    bsp->id = SeqIdDup(bdp->seqid->next);
    bsp->id->next = SeqIdDup(bdp->seqid);

    /* Remove concatenated deflines. */
    SeqDescrFree(ValNodeExtractList(&bsp->descr, Seq_descr_title));
    /* Replace with single defline. */
    ValNodeCopyStr(&bsp->descr, Seq_descr_title, bdp->title);

    tbdp = BlastDefLineSetFree(tbdp);

    /* Remove all user data (tax info, asn1 defline). */
    SeqDescrFree(ValNodeExtractList(&bsp->descr, Seq_descr_user));

    if(tnames) {		/* Add taxid, taxnames. */
	BioSourcePtr	bsrp;
	DbtagPtr	dtp;

	bsrp = BioSourceNew();
	bsrp->subtype = NULL;
	bsrp->org = OrgRefNew();
	if(tnames->sci_name)
	    bsrp->org->taxname = StringSave(tnames->sci_name);
	if(tnames->common_name)
	    bsrp->org->common = StringSave(tnames->common_name);
	dtp = DbtagNew();
	dtp->db = StringSave("taxon");
	dtp->tag = ObjectIdNew();
	dtp->tag->id = tnames->tax_id;
	bsrp->org->db = NULL;
	ValNodeAddPointer(&bsrp->org->db, 0, dtp);
	ValNodeAddPointer(&bsp->descr, Seq_descr_source, bsrp);
    }
    RDBTaxNamesFree(tnames);

/*    if (!SeqHasTax(bsp)) {
      printf("Warning: Bioseq without taxonomy!");
      if (query->choice == SEQID_GI) {
        printf("GI: %d\n",query->data.intvalue);
      } else printf("\n");
    } */
    return bsp;
}

/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/* return a common style dictionary for Cn3D 4.0                             */
/*---------------------------------------------------------------------------*/
/*---------------------------------------------------------------------------*/
/*
Cn3dStyleDictionaryPtr LIBCALL CddSrvGetStyle2(Int4 *styles[], Int4 nstyles)
{
    Cn3dStyleDictionaryPtr    csdp;
    Cn3dStyleTableItemPtr     cstip,cstipTail,cstipHead=NULL;
    Int4			    i;

    if(nstyles < 1) return(NULL);
    csdp = Cn3dStyleDictionaryNew();
    csdp->global_style = CddSrvGetStyle2_Ex(styles[0]);
    for(i=1; i<nstyles; i++) {
	cstip = Cn3dStyleTableItemNew();
	cstip->id = i;
	cstip->style = CddSrvGetStyle2_Ex(styles[i]);
	cstip->next = NULL;
	if(!cstipHead) cstipHead = cstipTail = cstip;
	else {
	    cstipTail->next = cstip;
	    cstipTail = cstip;
	}
    }
    csdp->style_table = cstipHead;

    return(csdp);
}
static Cn3dStyleSettingsPtr CddSrvGetStyle2_Ex(Int4 style[])
{
  Cn3dStyleSettingsPtr      cssp;
  Cn3dBackboneStylePtr      cbsp;
  Cn3dGeneralStylePtr       cgsp;
  Cn3dBackboneLabelStylePtr cblsp;

    cssp = Cn3dStyleSettingsNew();
      cssp->next = NULL;
      cssp->name = NULL;
      cbsp = Cn3dBackboneStyleNew();
        cbsp->type = style[0];
        cbsp->style = style[1];
        cbsp->color_scheme = style[2];
        cbsp->user_color =
            MyCn3dColorInit(style[3],style[4],style[5],
                            style[6],style[7]);
      cssp->protein_backbone = cbsp;
      cbsp = Cn3dBackboneStyleNew();
        cbsp->type = style[8];
        cbsp->style = style[9];
        cbsp->color_scheme = style[10];
        cbsp->user_color =
            MyCn3dColorInit(style[11],style[12],style[13],
                            style[14],style[15]);
      cssp->nucleotide_backbone = cbsp;
        cgsp = Cn3dGeneralStyleNew();
        cgsp->is_on = style[16];
        cgsp->style = style[17];
        cgsp->color_scheme = style[18];
        cgsp->user_color =
            MyCn3dColorInit(style[19],style[20],style[21],
                            style[22],style[23]);
      cssp->protein_sidechains = cgsp;
        cgsp = Cn3dGeneralStyleNew();
        cgsp->is_on = style[24];
        cgsp->style = style[25];
        cgsp->color_scheme = style[26];
        cgsp->user_color =
            MyCn3dColorInit(style[27],style[28],style[29],
                            style[30],style[31]);
      cssp->nucleotide_sidechains = cgsp;
        cgsp = Cn3dGeneralStyleNew();
        cgsp->is_on = style[32];
        cgsp->style = style[33];
        cgsp->color_scheme = style[34];
        cgsp->user_color =
            MyCn3dColorInit(style[35],style[36],style[37],
                            style[38],style[39]);
       cssp->heterogens = cgsp;
        cgsp = Cn3dGeneralStyleNew();
        cgsp->is_on = style[40];
        cgsp->style = style[41];
        cgsp->color_scheme = style[42];
        cgsp->user_color =
            MyCn3dColorInit(style[43],style[44],style[45],
                            style[46],style[47]);
      cssp->solvents = cgsp;
        cgsp = Cn3dGeneralStyleNew();
        cgsp->is_on = style[48];
        cgsp->style = style[49];
        cgsp->color_scheme = style[50];
        cgsp->user_color =
            MyCn3dColorInit(style[51],style[52],style[53],
                            style[54],style[55]);
       cssp->connections = cgsp;
        cgsp = Cn3dGeneralStyleNew();
        cgsp->is_on = style[56];
        cgsp->style = style[57];
        cgsp->color_scheme = style[58];
        cgsp->user_color =
            MyCn3dColorInit(style[59],style[60],style[61],
                            style[62],style[63]);
      cssp->helix_objects = cgsp;
        cgsp = Cn3dGeneralStyleNew();
        cgsp->is_on = style[64];
        cgsp->style = style[65];
        cgsp->color_scheme = style[66];
        cgsp->user_color =
            MyCn3dColorInit(style[67],style[68],style[69],
                            style[70],style[71]);
      cssp->strand_objects = cgsp;
      cssp->virtual_disulfides_on = style[72];
      cssp->virtual_disulfide_color =
          MyCn3dColorInit(style[73],style[74],style[75],
                          style[76],style[77]);
      cssp->hydrogens_on = style[78];
      cssp->background_color =
          MyCn3dColorInit(style[79],style[80],style[81],
                          style[82],style[83]);
      cssp->scale_factor = style[84];
      cssp->space_fill_proportion = style[85];
      cssp->ball_radius = style[86];
      cssp->stick_radius = style[87];
      cssp->tube_radius = style[88];
      cssp->tube_worm_radius = style[89];
      cssp->helix_radius = style[90];
      cssp->strand_width = style[91];
      cssp->strand_thickness = style[92];
        cblsp = Cn3dBackboneLabelStyleNew();
        cblsp->spacing = style[93];
        cblsp->type = style[94];
        cblsp->number = style[95];
        cblsp->termini = style[96];
        cblsp->white = style[97];
      cssp->protein_labels = cblsp;
        cblsp = Cn3dBackboneLabelStyleNew();
        cblsp->spacing = style[98];
        cblsp->type = style[99];
        cblsp->number = style[100];
        cblsp->termini = style[101];
        cblsp->white = style[102];
      cssp->nucleotide_labels =cblsp;
      cssp->ion_labels = style[103];

  return(cssp);
}
static Cn3dColorPtr MyCn3dColorInit(Int4 scale_factor, Int4 red, Int4 green, Int4 blue,
			     Int4 alpha)
{
    Cn3dColorPtr	ccp;

    ccp = Cn3dColorNew();
    ccp->scale_factor = scale_factor;
    ccp->red = red;
    ccp->green = green;
    ccp->blue = blue;
    ccp->alpha = alpha;

    return ccp;
}
*/
