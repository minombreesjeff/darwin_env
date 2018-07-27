/*===========================================================================
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
* File Name:  cblast.h
*
* Author:  Yanli Wang
*
* Initial Version Creation Date: 6/25/2002
*
* File Description:
*         header file for the CBLAST server CGI-bin
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cblast.h,v $
* Revision 1.6  2002/12/31 21:00:33  ywang
* read environment variables from config file
*
* Revision 1.5  2002/12/12 15:59:45  ywang
* wording improvement
*
*
*
* ==========================================================================
*/

#define MMDBCALL "http://www.ncbi.nlm.nih.gov/Structure/mmdb/mmdbsrv.cgi?uid="
#define ENTREZCALL "http://www.ncbi.nlm.nih.gov/entrez/utils/qmap.cgi?db=Protein&form=6&Dopt=g&uid="
#define WRPSBCALL "wrpsb.cgi?DATALIB=oasis_sap&INPUT_TYPE=access&GRAPH=2&FILTER=T&SEQUENCE="

#define MaxEntryPerPage 50
/*---------------------------------------------------------------------------*/
/* static global variables                                                   */
/*---------------------------------------------------------------------------*/
static Char    URLBase[PATH_MAX];
static Char    URLcgi[PATH_MAX];

       Char    CGIurl[PATH_MAX];
       Char    MMDBcgi[PATH_MAX];
       Char    CDDhome[PATH_MAX];
       Char    CGIName[PATH_MAX];
       Char    CDSRVurl[PATH_MAX];
static Char    ENTREZurl[PATH_MAX];
static Char    TAXcgi[PATH_MAX];
static Char    MMDBurl[PATH_MAX];
static Char    DARTcgi[PATH_MAX];
static Char    DATApath[PATH_MAX];
static Char    REFpath[PATH_MAX];
static Char    DOCSUMurl[PATH_MAX];
static Char    MAILto[PATH_MAX];
static Char    LOGpath[PATH_MAX];
static Char    NRPDB[PATH_MAX];
static Char    ODBCINI[PATH_MAX];
static Char    DARTUSER[PATH_MAX];
static Char    DARTPASS[PATH_MAX];
static Char    DARTLIB[PATH_MAX];
static Char    LDLIBRARYPATH[PATH_MAX];
static Char    SYBASE[PATH_MAX];
static Char    BLASTDB[PATH_MAX];
static Char    NCBI[PATH_MAX];

static Char    MMDBpath[PATH_MAX];

Int4        maxseqlen;

typedef struct cblastscoreinfodata{
  float seq_identity;
  Int2  length;
  FloatHi      evalue;
  FloatHi      bit_score;
  Int4         score;
} CblastScoreData, PNTR CblastScoreDataPtr;

typedef struct cblastseqinfodata{
  Int4 gi, PDBgi, hsp;
  /* gi-blast nr representative gi; PDBgi-gi of pdb sequence; hsp-HSP index */
  SeqIdPtr pdbid;
  CharPtr doc;   /* entrez docsum */
  Boolean bRep;
  Int2 pagenum;
}CblastSeqInfoData, PNTR CblastSeqInfoDataPtr;

typedef struct blastpdbnb {
  ValNodePtr csp; /* vnp->data.ptrvalue is CblastScoreDataPtr */
  ValNodePtr csip; /* vnp->data.ptrvalue is CblastSeqInfoDataPtr */
  SeqAnnotPtr sap;
  Int4 query_length;
  CharPtr query_title;
  Int2 numpages;
} BlastPDBNb, PNTR BlastPDBNbPtr;

typedef struct pdbidinfo {
  SeqIdPtr pdbList;
  ValNodePtr PDBgiList;
} PDBIdInfo, PNTR PDBIdInfoPtr;

typedef ValNode NeighborInfo, FAR *NeighborInfoPtr;

typedef struct neighborinfodata {
  Int4                gi;
  Int4                rank;
  SeqIdPtr            pdbid;
                        /* for PdbSeqIdPtr */
  CharPtr             docsum;
  Int4                MMDBid;
  CharPtr             mol;
  Uint1               chain;
  Int4                seqlen;
  Boolean             selected;
  Int2                pagenum;
  struct neighborinfodata  PNTR next;
} NeighborInfoData, PNTR NeighborInfoDataPtr;

typedef struct rankinfo {
  CharPtr             mol;
  char               chain;
  Int4                rank;
} RankInfo, PNTR RankInfoPtr;

typedef ValNode WWWQuery, FAR *WWWQueryPtr;

typedef struct wwwquerydata {
  CharPtr name;
  CharPtr value;
} WWWQueryData, PNTR WWWQueryDataPtr;

typedef struct snp_feature_info {
Int4 gi, pos, feature_id;
CharPtr title, descr;
Int4 color, id;
} SnpFeatureInfo, PNTR SnpFeatureInfoPtr;

typedef struct cdnb_info{
CharPtr CdName;
unsigned PssmId;
Int2 iClust, iMaprow, iColor;
Int4 from, to;
Boolean bRep, bFinished;
}CdNbInfo, PNTR CdNbInfoPtr;

typedef struct maprow_info{
Int4 from, to;
Int2 iColor;
CharPtr CdName;
CharPtr cdaln;
unsigned PssmId;
}MapRowInfo, PNTR MapRowInfoPtr;

typedef struct cd_info {
Int4 query_length;
SeqAnnotPtr sap;
Int2 cd_num, maprow_num;
ValNodePtr mapinfo;
      /* (MapRowInfoPtr) (mapinfo->data.ptrvalue)->data.ptrvalue */
CdNbInfoPtr *cnip;
}CdInfo, PNTR CdInfoPtr;
