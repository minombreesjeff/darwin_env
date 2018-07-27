/* $Id: cddsrv.h,v 1.17 2003/11/19 14:37:52 bauer Exp $
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
* File Name:  cddsrv.h
*
* Author:  Aron Marchler-Bauer
*
* Initial Version Creation Date: 9/20/1999
*
* $Revision: 1.17 $
*
* File Description:
*         header file for the CD-server CGI-bin
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cddsrv.h,v $
* Revision 1.17  2003/11/19 14:37:52  bauer
* more consistent use of PSSM-IDs
*
* Revision 1.16  2003/10/07 21:21:09  bauer
* initial changes to support drawing of hierarchies
*
* Revision 1.15  2002/12/05 19:15:12  bauer
* fixed lint warning
*
* Revision 1.14  2002/10/09 20:31:14  bauer
* increased max. number of CDART neighbors, and other additions
*
* Revision 1.13  2002/08/06 12:54:25  bauer
* fixes to accomodate COGs
*
* Revision 1.12  2002/07/05 21:10:54  bauer
* added CDtrack access functionality
*
* Revision 1.11  2002/06/12 15:22:09  bauer
* 6/11/02 update
*
* Revision 1.10  2002/04/25 14:31:22  bauer
* layout changes to the CDD server
*
* Revision 1.9  2002/03/07 19:12:14  bauer
* major revisions to cgi-bins and the CD-dumper
*
* Revision 1.8  2002/01/04 19:50:57  bauer
* initial changes to deal with PSSM-Ids
*
* Revision 1.7  2001/11/13 19:48:48  bauer
* add cCDDfname for cddumper
*
* Revision 1.6  2001/05/31 22:04:46  bauer
* changes to accomodate new type of Smart accessions
*
* Revision 1.5  2001/02/01 17:52:35  bauer
* changes for consensus CDDsrv
*
* Revision 1.4  2001/01/11 22:14:14  bauer
* added RealInd data structure
*
* Revision 1.3  2000/07/28 17:58:50  bauer
* added DomainComposition hotlinks in CD summary pages
*
* Revision 1.2  2000/07/19 19:26:55  bauer
* added modification logging
*
*
* ==========================================================================
*/

/*---------------------------------------------------------------------------*/
/* Options for iSeqStrMode in cddsrv Main                                    */
/*  SEQUONLY applies if no PDB-derived sequence is present in the CDD        */
/*  ONESTRUC applies if a single (master) PDB-derived sequence is present    */
/*  SEVSTRUC applies for more than one PDB-derived sequence + VAST data      */
/* prefix CDD indicates that only one (the final CDD) alignment is to be     */
/* reported (this mechanism is not used anymore)                             */
/*---------------------------------------------------------------------------*/
#define NOALIGN      0
#define SEQUONLY     1
#define ONESTRUC     2
#define SEVSTRUC     3
#define CDDSEQUONLY  4
#define CDDONESTRUC  5
#define CDDSEVSTRUC  6
#define CDDUPDATE    7  /* in cddumper: import a CD and recalculate tax. etc */
#define CDDASCDD     8  /* for use with Cn3D 4.0 - new mime types            */

/*---------------------------------------------------------------------------*/
/* Options for iMode in cddsrv. Determines whether a summary is produced or  */
/* whether alignment information is generated                                */
/*---------------------------------------------------------------------------*/
#define CDDSUMMARY      0
#define CDDALIGNMENT    1

/*---------------------------------------------------------------------------*/
/* switch between define/undef DEBUG to toggle verbose printout              */
/*---------------------------------------------------------------------------*/
#undef  DEBUG

#define CPUTIME_MAX       240
#define MAX_MMDBIDS      4096
#define CDD_MAX_DESCR    3072
#define CDD_GRAPH_HEIGHT   16
#define CDD_GRAPH_SPACER_X  5
#define CDD_GRAPH_SPACER_Y 25
#define CDD_GRAPH_WIDTH    60
#define DARTSIZELIMIT    1500
#define DARTFAMILYNUM    5000
#define MAXTREELEVELS      50
#define MAXDIV            100

#define MMDB_UNIXCOMPRESSED

#define MMDBCALL   "http://www.ncbi.nlm.nih.gov/Structure/mmdb/mmdbsrv.cgi?uid="
#define ENTREZCALL "http://www.ncbi.nlm.nih.gov/entrez/utils/qmap.cgi?db=Protein&form=6&Dopt=g&uid="
#define WRPSBCALL  "wrpsb.cgi?DATALIB=oasis_sap&INPUT_TYPE=access&GRAPH=2&FILTER=T&SEQUENCE="

/*---------------------------------------------------------------------------*/
/* Structures to hold summary and description information in linked lists    */
/*---------------------------------------------------------------------------*/
typedef struct cddsum {
  BiostrucAnnotSetPtr pbsaShort;
  Boolean             bIsPdb;
  Boolean             bIsMaster;
  Boolean             bIs3dRep;
  Char                cPdbId[5];
  Char                cChainId[2];
  Char                cPKBMDom[7];
  Char                cPKBDom[9];
  Char                cDefLine[256];
  CharPtr             cTaxName;
  Int4                iFsid;
  Int4                iFid;
  Int4                iMMDBId;
  Int4                uid;
  Int4                iCddIdx;
  Int4                iTaxId;
  SeqIdPtr            sip;
  SeqAlignPtr         salp;
  struct cddsum  PNTR next;
} CddSum, PNTR CddSumPtr;

typedef struct cdddesc {
  Int4                iPssmId;
  Char                cCddId[PATH_MAX];
  Char                cDescr[CDD_MAX_DESCR];
  Char                cSourc[PATH_MAX];
  Int4                iPssmLength;
  struct cdddesc PNTR next;
} CddDesc, PNTR CddDescPtr;

typedef struct realind {
  Int4                aid, mgi, sgi, mstart, mstop, sstart, sstop;
  SeqIdPtr            msip, ssip;
  Boolean             mIsPdb, sIsPdb;
  struct realind PNTR next;
} RealInd, PNTR RealIndPtr;

/*---------------------------------------------------------------------------*/
/* static global variables                                                   */
/*---------------------------------------------------------------------------*/
static Char    URLBase[PATH_MAX];
static Char    URLcgi[PATH_MAX];
static Char    CTBcgi[PATH_MAX];
static Char    PFAMcgiUS[PATH_MAX];
static Char    PFAMcgiUK[PATH_MAX];
static Char    SMARTcgi[PATH_MAX];
static Char    SMACCcgi[PATH_MAX];
static Char    COGcgi[PATH_MAX];
static Char    KOGcgi[PATH_MAX];
static Char    TAXcgi[PATH_MAX];
static Char    DATApath[PATH_MAX];
static Char    REFpath[PATH_MAX];
static Char    CDDpath[PATH_MAX];
static Char    ENTREZurl[PATH_MAX];
static Char    DOCSUMurl[PATH_MAX];
static Char    MAILto[PATH_MAX];
static FILE    *OutputFile = NULL;
static Char    OutputName[200];
static Char    gunzip[PATH_MAX];
static Char    MMDBpath[PATH_MAX];
static Char    CDDdpath[PATH_MAX];
static Char    CDDvpath[PATH_MAX];
static Char    CDDextens[PATH_MAX];
static Char    RAWextens[PATH_MAX];
static Int2    SortOn = 0;
static Int4    cnt_MMDBid;
static long    save_MMDBid[MAX_MMDBIDS];
static Boolean Chain;
static Char    SlaveChain[2];
static Char    MasterChain[2];
static Char    cCDDid[PATH_MAX];
static Char    cCDDfname[PATH_MAX];
static Char    CDDdescr[PATH_MAX];
static Char    database[PATH_MAX];
static Char    PUBcgi[PATH_MAX];
static Char    CDDPrefix[PATH_MAX];
static Char    CDDPost_O[PATH_MAX];
static Char    CDDPost_C[PATH_MAX];
static Char    CDDefault[PATH_MAX];
static Char    CDDdbtype[PATH_MAX];
static Char    ODBCINI[PATH_MAX];
static Char    DARTUSER[PATH_MAX];
static Char    DARTPASS[PATH_MAX];
static Char    CDTRKDBS[PATH_MAX];
static Char    CDDlocat[PATH_MAX];

/*---------------------------------------------------------------------------*/
/* static/local Function prototypes                                          */
/*---------------------------------------------------------------------------*/
static void                CddHtmlError(CharPtr cErrTxt);
static Boolean             CddGetParams();
static CddSumPtr           CddSumNew();
static CddSumPtr           CddSumFree(CddSumPtr pcds);
static CddSumPtr           CddSumLink(CddSumPtr PNTR head, CddSumPtr newnode);
static Boolean             UniqueUid(Int4 uid, Boolean pIsPdb, CddSumPtr pcds);
static Boolean             UseThisGi(Int4 Gi, ValNodePtr pvnGis);
static void                CddShowTracks(CddSumPtr pcds, CddDescPtr pcdd);
static void                CddDebugExamineSEP(SeqEntryPtr sep);
static CddDescPtr          CddDescNew();
static CddDescPtr          CddDescFree(CddDescPtr pcdd);
static CddDescPtr          CddDescLink(CddDescPtr PNTR head, CddDescPtr newnode);
static CddDescPtr          CddReadDescr();
static Int4                ConvertMMDBUID(CharPtr pcString);
static void                CddReadVASTInfo(CddSumPtr pcds);
static void                CddDetermineFsids(CddSumPtr pcds, PDNMS pModelStruc);
static void                CddDetermineFids(CddSumPtr pcds, PDNMS pModelStruc, CharPtr szName);
static BiostrucAnnotSetPtr LocalGetFeatureSet(Int4 mmdbid, Int4 feature_set_id, CharPtr JobID);
static BiostrucAnnotSetPtr CddLocalGetFeatureSet(Int4 mmdbid, Int4 feature_set_id);
static BiostrucAnnotSetPtr CddBiostrucAnnotSetGetByFid (BiostrucAnnotSetPtr basp, Int4 feature_id, Int4 feature_set_id);
static Boolean             CddIsVASTData(Int4 uid);
static BiostrucAnnotSetPtr CddVASTBsAnnotSetGet (Int4 uid);
static CddSumPtr           CddSumSort(CddSumPtr pcds);
static SeqAlignPtr         CddAlignSort(SeqAlignPtr salp, CddSumPtr pcds);
static void CDDSrvHead(FILE *table, CharPtr title);
static void CDDSrvFoot(FILE *table);




