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
* File Name:  cblastutil.c
*
* Author:  Yanli Wang
*
* Initial Version Creation Date: 6/25/2002
*
* $Revision: 1.10 $
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cblastutil.c,v $
* Revision 1.10  2002/12/31 21:00:33  ywang
* read environment variables from config file
*
* Revision 1.9  2002/12/31 19:39:58  ywang
* add Log to record modification history
*
* ==========================================================================
*/

#include <ncbi.h>  /* the NCBI SDK header */
#include <asn.h>   /* the ASN.1 header */
#include <mmdbapi.h>   /* the MMDB-API header */
#include <sequtil.h>
#include <objsset.h>
#include <mmdbdata.h>
#include <mmdblocl.h>
#include <string.h>
#include <accentr.h>
#include <saledit.h>
#include <lsqfetch.h>
#include <objseq.h>
#include <asnseq.h>
#include <objmgr.h>
#include <sequtil.h>
#include <objmime.h>
#include <blast.h>
#include <www.h>
#include <accutils.h>
#include <netentr.h>
#include <cddutil.h>
#include <objcn3d.h>
#include <fdlobj.h>
/* #include <asnalign.h> */
#include <txalign.h>

#include "/netopt/structure/include/dbinfo.h"
#include "/netopt/structure/include/dart.h"
#include "/netopt/structure/include/dartutil.h"

#include <PubStructApi.h> 
#include <PubVastApi.h>
#include "cblast.h"
#include "cdd_seedutil.h"

static Int4  iNcolors = 13;
extern Int2 entry_page;
Int4 snp_color[2][3] = {255, 128, 0, 0, 255, 0};

#define MaxRelatedCD 15000 

/*----------*/
BioseqPtr CblastRDbGetBioseq(SeqIdPtr sip, Int4 oid, ReadDBFILEPtr rdfp)
{
    BioseqPtr   bsp;
    Uint4   header;
    SeqIdPtr    tmp_sip = NULL;
    Char    buf[64];

    bsp = readdb_get_bioseq(rdfp, oid);

    if(sip == NULL) return bsp;

    header = 0;
    while(readdb_get_header(rdfp,oid,&header,&tmp_sip,NULL)) {
    if(SeqIdComp(tmp_sip,sip) == SIC_YES ||
       SeqIdComp(tmp_sip->next,sip) == SIC_YES) break;
    tmp_sip = SeqIdSetFree(tmp_sip);
    }
    if(tmp_sip == NULL) {
    SeqIdPrint(sip,buf,PRINTID_FASTA_SHORT);
    ErrPostEx(SEV_WARNING,0,0,"Bad seqid %s",buf);
    return NULL;
    }
    bsp->id = SeqIdSetFree(bsp->id);
    bsp->id = tmp_sip->next;
    bsp->id->next = tmp_sip;
    bsp->id->next->next = NULL;

    return bsp;
}
/*--------------*/
BioseqPtr CblastGetBioseqWithGi(Int4 gi, ReadDBFILEPtr rdfp)
{
  Int4 oid = 0, start = 0;
  BioseqPtr bsp = NULL;
  SeqIdPtr sip = NULL;

  if((oid = readdb_gi2seq(rdfp,gi,&start)) != -1){
    sip = ValNodeNew(NULL);
    sip->choice = SEQID_GI;
    sip->data.intvalue = gi;
    bsp = (BioseqPtr) CblastRDbGetBioseq(sip, oid, rdfp);
  }

  return bsp;
}
/*-------------------*/
static CharPtr CblastGetDefline(Int4 gi, ReadDBFILEPtr rdfp)
{
  Int4 oid=0, start=0;
  CharPtr doc = NULL;
  BlastDefLinePtr bdlp_head=NULL, bdlp=NULL;
  SeqIdPtr sip=NULL;


  if((oid = readdb_gi2seq(rdfp,gi,&start)) != -1){
    bdlp_head = FDReadDeflineAsn(rdfp, oid);
    bdlp = bdlp_head;
    while(bdlp){
      sip = bdlp->seqid;
      while(sip){
        if(sip->choice == SEQID_GI){
          if(gi == sip->data.intvalue) doc=StringSave(bdlp->title);
        }
        sip=sip->next;
      }
      bdlp=bdlp->next;
    }
  }

  if(!doc) doc=StringSave("Protein with known structure");

  if(bdlp_head)bdlp_head=BlastDefLineSetFree(bdlp_head);

  return doc;
}
/*-------------------*/
static Boolean cblast_load_align_sum_for_DenseSeg(DenseSegPtr dsp, AlignSumPtr asp)
{
    SeqInt msi, tsi;
    SeqIntPtr sint;
    SeqLoc sl;
    Int2 i, k;
    Int2 dim;
    Int2 m_order, t_order;  /*order of the master and the target sequence*/
    Int4 index;
    Int4 j, val, t_val;
    Uint1 m_res, t_res, stdaa_res;
    SeqIdPtr sip;
    SeqPortPtr m_spp, t_spp;
    SeqMapTablePtr smtp;
   
   
    if(dsp == NULL || asp == NULL)
        return FALSE;
   
   if((smtp = SeqMapTableFindObj(Seq_code_ncbistdaa, Seq_code_ncbieaa)) == NULL) {
     return FALSE;
   }
   
    m_order = -1;
    t_order = -1;
    dim = 0;
    for(i = 0, sip = dsp->ids; sip != NULL; sip = sip->next, ++i) {
        if(SeqIdMatch(sip, asp->master_sip) && m_order == -1)
            m_order = i;
        else if(SeqIdMatch(sip, asp->target_sip) && t_order == -1)
            t_order = i;
        ++dim;
    }

    if(m_order == -1 || t_order == -1)
        return FALSE;

    msi.id = asp->master_sip;
    msi.from = -1;
    msi.to = -1;
    msi.strand = (dsp->strands == NULL) ? 0 : dsp->strands[m_order];
    tsi.id = asp->target_sip;
    tsi.from = -1;
    tsi.to = -1;
    tsi.strand = (dsp->strands == NULL) ? 0 : dsp->strands[t_order];

    for(i = 0; i<dsp->numseg; ++i) {
        for(k = 0; k<dim; ++k) {
            val = dsp->starts[i*dim + k];
            if(val != -1 && (k == m_order || k == t_order)) {
                sint = (k == m_order) ? (&msi) : (&tsi);
                if(sint->from == -1 || sint->from > val)
                    sint->from = val;
                if(sint->to == -1 || sint->to < (val + dsp->lens[i] -1))
                    sint->to = val + dsp->lens[i] -1;
            }
        }
    }

    asp->master_from = msi.from;
    asp->master_to = msi.to;
    asp->target_from = tsi.from;
    asp->target_to = tsi.to;

    if (asp->is_aa) {
        asp->m_strand = Seq_strand_unknown;
        asp->t_strand = Seq_strand_unknown;
    } else {
        asp->m_strand = dsp->strands[m_order];
        asp->t_strand = dsp->strands[t_order];
    }
    asp->m_frame_set = FALSE;
    asp->t_frame_set = FALSE;

    sl.choice = SEQLOC_INT;
    sl.data.ptrvalue = &msi;
    m_spp = SeqPortNewByLoc(&sl, (asp->is_aa) ? Seq_code_ncbieaa : Seq_code_iupacna);

    sl.choice = SEQLOC_INT;
    sl.data.ptrvalue = &tsi;
    t_spp = SeqPortNewByLoc(&sl, (asp->is_aa) ? Seq_code_ncbieaa : Seq_code_iupacna);

    for(i = 0; i<dsp->numseg; ++i) {
        val = dsp->starts[i*dim + m_order];
        t_val = dsp->starts[i*dim + t_order];
        if(val == -1 || t_val == -1) {
            asp->gaps += dsp->lens[i];
            if(val != -1) {
                index = dsp->lens[i];
                while (index > 0) {
                    index--;
                    m_res = SeqPortGetResidue(m_spp);
                }
            }
            if(t_val != -1) {
                index = dsp->lens[i];
                while (index > 0) {
                    index--;
                    t_res = SeqPortGetResidue(t_spp);
                }
            }
        } else {
            for(j = 0; j<dsp->lens[i]; ++j) {
                m_res = SeqPortGetResidue(m_spp);
                m_res = SeqMapTableConvert(smtp, m_res);
                  /* blast query seq is Seq_code_ncbieaa */
                  /* seq code for sequences in blast db is Seq_code_ncbistdaa */
                t_res = SeqPortGetResidue(t_spp);
                if(m_res == t_res)
                    ++(asp->identical);
                else if(asp->matrix != NULL && asp->is_aa) {
                    if (IS_residue(m_res) && IS_residue(t_res)) {

                        if(asp->posMatrix != NULL) {
                            stdaa_res = SeqMapTableConvert(smtp, t_res);
                            if(asp->posMatrix[val+j][stdaa_res] > 0)
                                ++(asp->positive);
                        } else {
                            if(asp->matrix[m_res][t_res] >0)
                                ++(asp->positive);
                        }
                    }
                }
            }

        }
        asp->totlen += dsp->lens[i];
    }
    SeqPortFree(m_spp);
    SeqPortFree(t_spp);
    return TRUE;
}

/*-------------------*/
ScorePtr cblast_find_score_in_align(SeqAlignPtr align, Uint2 chain,AlignSumPtr asp)
{
    DenseDiagPtr ddp;
    DenseSegPtr dsp;
    StdSegPtr ssp;
    Uint2 order = 0;
    SeqAlignPtr sap;
    ScorePtr    sp;

    if(align == NULL)
        return NULL;
   
    if(asp != NULL) {
        asp->totlen = 0;
        asp->positive = 0;
        asp->identical = 0;
        asp->gaps = 0;
    }
    dsp = (DenseSegPtr) align->segs;
    if(asp != NULL) cblast_load_align_sum_for_DenseSeg(dsp, asp);
    if (dsp->scores) return dsp->scores;
    else return align->score;

}
/*-------------------*/
Boolean MultiHSP(SeqAnnotPtr sap, Int4 rep_gi)
{
  SeqAlignPtr salp = NULL;
  DenseSegPtr dsp = NULL;
  SeqIdPtr sip = NULL;
  Int4 count = 0;
 
  salp = sap->data;
  while(salp){
    dsp = salp->segs;
    sip = dsp->ids;
    while(sip){
      if(sip->choice == SEQID_GI){
        if(sip->data.intvalue == rep_gi) count++;
      }
      sip = sip->next;
    }
    salp = salp->next;
  }

  if(count > 1) return TRUE;
  else return FALSE;

}
/*-------------------*/
SeqIdPtr CblastGetPDBIdFromBioseq(BioseqPtr bsp)
{
  SeqIdPtr pdb_sip = NULL, sip = NULL;

  sip = bsp->id;
  while(sip){
    if(sip->choice == SEQID_PDB){
      pdb_sip = SeqIdDup(sip);
      return pdb_sip;
    }
    sip = sip->next;
  } 

} 
/*-------------*/
SeqAnnotPtr ReplaceSeqIdForSeqAnnot(SeqAnnotPtr sap, SeqIdPtr sipThis)
{
  SeqAnnotPtr sapThis = NULL;
  SeqAlignPtr salp = NULL;
  DenseSegPtr dsp = NULL;
  SeqIdPtr sip = NULL;

  sapThis = AsnIoMemCopy(sap, (AsnReadFunc)SeqAnnotAsnRead, (AsnWriteFunc)SeqAnnotAsnWrite);
  salp = sapThis->data;
  while(salp){
    dsp = salp->segs;
    sip = dsp->ids; sip->next = SeqIdFree(sip->next);
    sip->next = SeqIdDup(sipThis);
    salp = salp->next;
  }

  return sapThis;
}
/*-------------------*/
SeqAnnotPtr CblastGetSapPdbPair(SeqAnnotPtr sap, BioseqPtr bsp)
{
  SeqIdPtr pdbid = NULL;
  SeqAnnotPtr sap_pdb_pair = NULL;

  if(!sap || !bsp) return NULL;
  pdbid = (SeqIdPtr) CblastGetPDBIdFromBioseq(bsp);
  if(pdbid) {
    sap_pdb_pair = ReplaceSeqIdForSeqAnnot(sap, pdbid);
    pdbid = SeqIdFree(pdbid);
  }
  else sap_pdb_pair = AsnIoMemCopy(sap, (AsnReadFunc)SeqAnnotAsnRead, (AsnWriteFunc)SeqAnnotAsnWrite);
   
  return sap_pdb_pair;
}
/*-------------------*/
CblastScoreDataPtr CblastScoreDataNew()
{
  CblastScoreDataPtr csdp = NULL;
  csdp = (CblastScoreDataPtr)MemNew(sizeof(CblastScoreData));
  return csdp;
}
/*-------------------*/
CblastScoreDataPtr CblastScoreDataFree(CblastScoreDataPtr csdp)
{
  csdp = MemFree(csdp);
  return csdp;
}
/*-------------------*/
CblastSeqInfoDataPtr CblastSeqInfoDataNew()
{
  CblastSeqInfoDataPtr csidp = NULL;
  csidp = (CblastSeqInfoDataPtr)MemNew(sizeof(CblastSeqInfoData));
  return csidp;
} 
/*-------------------*/
CblastSeqInfoDataPtr CblastSeqInfoDataFree(CblastSeqInfoDataPtr csidp)
{
  SeqIdFree(csidp->pdbid);
  MemFree(csidp->doc);
  MemFree(csidp);
}
/*-------------------*/
PDBIdInfoPtr PDBIdInfoNew()
{
  PDBIdInfoPtr piip = NULL;
  piip = (PDBIdInfoPtr)MemNew(sizeof(PDBIdInfo));
  return piip;
}
/*-------------------*/
PDBIdInfoPtr PDBIdInfoFree(PDBIdInfoPtr piip)
{
  if(!piip) return NULL;

  piip->pdbList = SeqIdSetFree(piip->pdbList);
  piip->PDBgiList = ValNodeFree(piip->PDBgiList);

  MemFree(piip);
}
/*-------------*/
SeqAnnotPtr CblastAlgTruncate(SeqAlignPtr salp, AlgValidationStatusPtr avsp)
{
  SeqAnnotPtr sap = NULL;
  SeqAlignPtr salp_head = NULL, salp_new = NULL, salp_tmp = NULL;


  if(salp == NULL) return NULL;

  while(salp){
    if(avsp->data.intvalue > 0 ){
      salp_tmp = salp;
      if(salp_head == NULL){
        salp_head = AsnIoMemCopy(salp_tmp, (AsnReadFunc)SeqAlignAsnRead, (AsnWriteFunc)SeqAlignAsnWrite);
        salp_new = salp_head;
      }
      else {
        salp_new->next = AsnIoMemCopy(salp_tmp, (AsnReadFunc)SeqAlignAsnRead, (AsnWriteFunc)SeqAlignAsnWrite);
        salp_new = salp_new->next;
      }
    }
    salp = salp->next;
    avsp = avsp->next;
  }

  if(salp_head != NULL){
    sap = SeqAnnotNew();
    sap->type = 2;
    sap->data = salp_head;

    return sap;
  }
  else return NULL;

}
/*-------------*/
SeqAnnotPtr CblastAlgTruncateAndExtend(SeqAlignPtr salp, ValNodePtr avsp)
{
  SeqAnnotPtr sap = NULL;
  SeqAlignPtr salp_head = NULL, salp_new = NULL, salp_tmp = NULL;
  Int2 i = 0;

  if(salp == NULL) return NULL;

  while(salp){
    if(avsp->data.intvalue > 0 ){
      salp_tmp = salp;
      for(i = 0; i < avsp->data.intvalue; i++){
        if(salp_head == NULL){
          salp_head = AsnIoMemCopy(salp_tmp, (AsnReadFunc)SeqAlignAsnRead, (AsnWriteFunc)SeqAlignAsnWrite);
          salp_new = salp_head;
        }
        else {
          salp_new->next = AsnIoMemCopy(salp_tmp, (AsnReadFunc)SeqAlignAsnRead, (AsnWriteFunc)SeqAlignAsnWrite);
          salp_new = salp_new->next;
        }
      }
    }
    salp = salp->next;
    avsp = avsp->next;
  }
 
  if(salp_head != NULL){
    sap = SeqAnnotNew();
    sap->type = 2;
    sap->data = salp_head;

    return sap;
  }
  else return NULL;

}
/*-------------*/
SeqEntryPtr CreatSingleEntry(BioseqPtr bsp)
{
  SeqEntryPtr sep=NULL;

  sep = SeqEntryNew();
  sep->choice = 1;
  sep->data.ptrvalue = AsnIoMemCopy(bsp, (AsnReadFunc)BioseqAsnRead, (AsnWriteFunc)BioseqAsnWrite);

  return sep;
}
/*-------------*/
SeqEntryPtr GetSeqEntry(BioseqPtr bsp1, BioseqPtr bsp2)
{
  SeqEntryPtr sep1=NULL, sep2 = NULL,sep_root = NULL;
  BioseqSetPtr bssp = NULL;

  sep1 = CreatSingleEntry(bsp1);
  sep2 = CreatSingleEntry(bsp2);
  sep1->next = sep2;

  bssp = BioseqSetNew();
  bssp->seq_set = sep1;
  sep_root = SeqEntryNew();
  sep_root->choice = 2;
  sep_root->data.ptrvalue = bssp;

  return sep_root;
}
/*-------------*/
SeqAnnotPtr GetAlgForSubjectPairPerHSP(SeqAnnotPtr sap, Int4 gi, Int2 hsp)
{
  AlgValidationStatusPtr avsp_head = NULL, avsp = NULL;
  SeqAlignPtr salp = NULL;
  DenseSegPtr dsp = NULL;
  SeqIdPtr sip = NULL;
  SeqAnnotPtr sapThis = NULL;
  Int2 hsp_num = 0;

  salp = sap->data;
  while(salp){
    avsp = ValNodeNew(NULL);
    avsp->data.intvalue = 0;
    dsp = salp->segs;
    sip = dsp->ids;
    sip = sip->next;
    if(sip->choice == SEQID_GI){
      if(sip->data.intvalue == gi){
        hsp_num++;
        if(hsp_num == hsp) {
          avsp->data.intvalue = 1;
        }
      }
    }
    ValNodeLink(&avsp_head, avsp);
    salp = salp->next;
  }

  sapThis = CblastAlgTruncate(sap->data, avsp_head);
  ValNodeFree(avsp_head);

  return sapThis;

}
/*-------------*/
SeqAnnotPtr GetAlgForSubjectPair(SeqAnnotPtr sap, Int4 gi)
{
  AlgValidationStatusPtr avsp_head = NULL, avsp = NULL;
  SeqAlignPtr salp = NULL;
  DenseSegPtr dsp = NULL;
  SeqIdPtr sip = NULL;
  SeqAnnotPtr sapThis = NULL;

  salp = sap->data;
  while(salp){
    avsp = ValNodeNew(NULL);
    avsp->data.intvalue = 0;
    dsp = salp->segs;
    sip = dsp->ids;
    sip = sip->next;
    if(sip->choice == SEQID_GI){
      if(sip->data.intvalue == gi){
        avsp->data.intvalue = 1;
      }
    }
    ValNodeLink(&avsp_head, avsp);
    salp = salp->next;
  }

  sapThis = CblastAlgTruncate(sap->data, avsp_head);
  ValNodeFree(avsp_head);

  return sapThis;
}
/*--------------*/
Int4 GetGiForRedundantPDB(Int4 query_gi, ReadDBFILEPtr rdfp)
{
    Uint4       header_index = 0;
    SeqIdPtr    sip = NULL, sip_tmp = NULL;
    Int4        ordinal_id = 0, start = 0, gi = 0;

    Boolean PdbFound = FALSE;

    if((ordinal_id = readdb_gi2seq(rdfp,query_gi,&start)) == -1) {
    ErrPostEx(SEV_WARNING,0,0,"ord_id not found for %d",query_gi);
    return NULL;
    }

    /* get seqid from database headers file */
    while(readdb_get_header(rdfp,ordinal_id,&header_index,&sip,NULL)) {
       gi = 0;
       sip_tmp = sip;
       while(sip_tmp){
          if(sip_tmp->choice == SEQID_GI) {
             gi = sip_tmp->data.intvalue;
          }
          else if(sip_tmp->choice == SEQID_PDB){
             sip = SeqIdSetFree(sip);
             return gi;
          }
          sip_tmp = sip_tmp->next;
       }
       sip = SeqIdSetFree(sip);
    }

    return NULL;
}
/*--------------*/
PDBIdInfoPtr GetAllRedundantPDBSeqIdsForGi(Int4 query_gi, ReadDBFILEPtr rdfp)
{   
    PDBIdInfoPtr piip = NULL;
    ValNodePtr PDBgiList= NULL;
    Uint4       header_index = 0;
    SeqIdPtr    sip = NULL, sip_tmp = NULL, pdbList = NULL, pdb_sip = NULL;
    Int4        ordinal_id = 0, start = 0, gi = 0;

    if((ordinal_id = readdb_gi2seq(rdfp,query_gi,&start)) == -1) {
    ErrPostEx(SEV_WARNING,0,0,"ord_id not found for %d",query_gi);
    return NULL;
    }

    /* get seqid from database headers file */
    while(readdb_get_header(rdfp,ordinal_id,&header_index,&sip,NULL)) {
       while(sip){
          if(sip->choice == SEQID_GI){
            gi = sip->data.intvalue;
          }
          if(sip->choice == SEQID_PDB){
             pdb_sip = SeqIdDup(sip);
             if(pdb_sip){
               ValNodeLink(&pdbList, pdb_sip); 
               ValNodeAddInt(&PDBgiList, 0, gi);
             }
             else ErrPostEx(SEV_WARNING,0,0,"SeqIdDup failed, probably running out of memory\n");
          }
          sip = sip->next;
       }
       sip = SeqIdSetFree(sip);
    }

    if(pdbList && PDBgiList){
      piip = PDBIdInfoNew();
      piip->pdbList = pdbList;
      piip->PDBgiList = PDBgiList;
    }

    return piip;
}
/*--------------*/
PDBIdInfoPtr GetFirstRedundantPDBSeqIdsForGi(Int4 query_gi, ReadDBFILEPtr rdfp)
{
    Uint4       header_index = 0;
    SeqIdPtr    sip = NULL, sip_tmp = NULL, pdb_sip = NULL;
    Int4        ordinal_id = 0, start = 0, gi = 0;
    PDBIdInfoPtr piip = NULL;
    ValNodePtr PDBgiList= NULL;

    Boolean PdbFound = FALSE;

    if((ordinal_id = readdb_gi2seq(rdfp,query_gi,&start)) == -1) {
    ErrPostEx(SEV_WARNING,0,0,"ord_id not found for %d",query_gi);
    return NULL;
    }

    /* get seqid from database headers file */
    while(readdb_get_header(rdfp,ordinal_id,&header_index,&sip,NULL)) {
       sip_tmp = sip;
       while(sip_tmp){
          if(sip_tmp->choice == SEQID_GI) {
             gi = sip_tmp->data.intvalue;
          }
          else if(sip_tmp->choice == SEQID_PDB){
             pdb_sip = SeqIdDup(sip_tmp);

             sip = SeqIdSetFree(sip);
             ValNodeAddInt(&PDBgiList, 0, gi);
             piip = PDBIdInfoNew();
             piip->pdbList = pdb_sip;
             piip->PDBgiList = PDBgiList;
             
             return piip;
          }
          sip_tmp = sip_tmp->next;
       }
       sip = SeqIdSetFree(sip);
    }

    return NULL;
}
/*-------------------*/
BlastPDBNbPtr BlastPDBNbNew()
{
  BlastPDBNbPtr bpnp = NULL;

  bpnp = (BlastPDBNbPtr)MemNew(sizeof(BlastPDBNb));
  return bpnp;
}
/*-------------------*/
BlastPDBNbPtr BlastPDBNbFree(BlastPDBNbPtr bpnp)
{
  ValNodePtr csip = NULL, next = NULL;
  CblastSeqInfoDataPtr csidp = NULL;

  if(!bpnp) return NULL;

  bpnp->sap = SeqAnnotFree(bpnp->sap);
  csip = bpnp->csip;

  while(csip){
    next = csip->next;
    csidp = csip->data.ptrvalue;
    csidp = CblastSeqInfoDataFree(csidp);
    csip = next;
  }
  ValNodeFree(bpnp->csip);

  MemFree(bpnp);

  return NULL;
}
/*-------------------*/
WWWQueryDataPtr WWWQueryDataNew()
{
  WWWQueryDataPtr wqdp = NULL;

  wqdp =(WWWQueryDataPtr)MemNew(sizeof(WWWQueryData)); 
  wqdp->name = NULL; wqdp->value = NULL;

  return wqdp;
}
/*-------------------*/
NeighborInfoDataPtr NeighborInfoDataNew()
{
  NeighborInfoDataPtr nidp = NULL;
  
  nidp = (NeighborInfoDataPtr)MemNew(sizeof(NeighborInfoData));

  nidp->gi = 0;
  nidp->docsum = NULL;
  
  return nidp;
}
/*-------------------*/
NeighborInfoDataPtr NeighborInfoDataFree(NeighborInfoDataPtr nidp)
{
  if(!nidp) return NULL;

  nidp->pdbid = SeqIdFree(nidp->pdbid);
  MemFree(nidp->docsum);
  MemFree(nidp);

  return NULL;
}
/*------------------------*/
NeighborInfoPtr  NeighborInfoFree(NeighborInfoPtr nip_head)
{
  NeighborInfoPtr next;
  NeighborInfoPtr nip = NULL;
  NeighborInfoDataPtr nidp = NULL;

  nip = nip_head;
  while(nip){
    next = nip->next;
    nidp = nip->data.ptrvalue;
    nidp = NeighborInfoDataFree(nidp);
    nip = next;
  }
  ValNodeFree(nip_head);

  return NULL;
}
/*------------------*/
NeighborInfoPtr GetRedundantPDBNeighborList(Int4 query_gi, ReadDBFILEPtr rdfp)
{
  NeighborInfoPtr nip_head = NULL, nip = NULL;
  NeighborInfoDataPtr nidp = NULL;

  Uint4       header_index = 0;
  SeqIdPtr    sip = NULL, sip_tmp = NULL, pdbid = NULL;
  Int4        ordinal_id = 0, start = 0, gi = 0;

  if((ordinal_id = readdb_gi2seq(rdfp,query_gi,&start)) == -1) {
    ErrPostEx(SEV_WARNING,0,0,"ord_id not found for %d",query_gi);
    return NULL;
  }

   /* get seqid from database headers file */
  while(readdb_get_header(rdfp,ordinal_id,&header_index,&sip_tmp,NULL)) {
    gi = 0;
    pdbid = NULL;
    sip = sip_tmp;
    while(sip){
      if(sip->choice == SEQID_GI) {
        gi = sip->data.intvalue;
      }
      else if(sip->choice == SEQID_PDB){
        pdbid = sip;
      }
      sip = sip->next;
    }
    if(pdbid){
      nidp = NeighborInfoDataNew();
      nidp->pdbid = SeqIdDup(pdbid);
      nidp->gi = gi;
      ValNodeAddPointer(&nip_head, 1000, nidp);
    }
    sip_tmp = SeqIdSetFree(sip_tmp);
  }

  return nip_head;
}
/*------------------*/
NeighborInfoPtr GetRedundantPDBNeighborInfo(Int4 gi, ReadDBFILEPtr rdfp)
{
  NeighborInfoPtr nip = NULL, nipThis = NULL;
  NeighborInfoDataPtr nidp = NULL;
  DocSumPtr dsp = NULL;
  BioseqPtr bsp = NULL;
  ValNodePtr descr = NULL;
  SeqIdPtr sip = NULL, pdbid = NULL;
  Int2 count = 0;

  nip = GetRedundantPDBNeighborList(gi, rdfp);

  count = 0;
  nipThis = nip;
  while(nipThis){
/*
    bsp = (BioseqPtr) CblastGetBioseqWithGi(nip_gi, rdfp); 
    if(bsp){
      if(bsp->descr) {
        descr = bsp->descr;
        while(descr){
          if(descr->choice == Seq_descr_title) {
            nip->docsum = StringSave(descr->data.ptrvalue);
          }
          descr = descr->next;
        }
      }
    }
    bsp = BioseqFree(bsp);
*/

    count;
    nidp = nipThis->data.ptrvalue;
    nidp->pagenum = (Int2) ((count - count % entry_page)/entry_page + 1);
    dsp = NetDocSum(TYP_AA, nidp->gi);
    if(dsp){
      nidp->docsum = StringSave(dsp->title);
      DocSumFree(dsp);
    }
    else nidp->docsum = StringSave("Protein with known structure");

    pdbid = nidp->pdbid;
    nidp->mol =StringSave(((PDBSeqIdPtr)pdbid->data.ptrvalue)->mol); 
    nidp->chain = ((PDBSeqIdPtr)pdbid->data.ptrvalue)->chain;

    count++;
    nipThis = nipThis->next;
  }

  return nip;

}
/*-----------------------*/
NeighborInfoDataPtr GetNIForSel(Int4 gi, NeighborInfoPtr nip)
{
  NeighborInfoPtr nipThis = NULL;
  NeighborInfoDataPtr nidp = NULL;

  nipThis = nip;
  while(nipThis){
    nidp = nipThis->data.ptrvalue;
    if(gi == nidp->gi) return nidp;
    nipThis = nipThis->next;
  }

  return nip->data.ptrvalue;
     /* otherwise use the first node */
}
/*-----------------------*/
static int LIBCALLBACK SortNeighborInfo(VoidPtr ptr1, VoidPtr ptr2)
{
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);

  if(vnp1->choice == vnp2->choice) return 0;
  else if(vnp1->choice > vnp2->choice) return 1;
  else return -1;
          /* sort as 1 2 3 4 ...*/
   
}
/*----- Get ranking info from querying database-----*/
NeighborInfoPtr GetRankedNeighborInfoFD(NeighborInfoPtr nip)
{
  NeighborInfoPtr nipThis = NULL;
  NeighborInfoDataPtr nidp = NULL;
  char domName[10];
  unsigned  gn, gr;
  SubSetID subId;
  Int4 n = 0;

  n = constructSdiCounts();
  subId = (SubSetID)NonIdenticalSeq;

  nipThis = nip;
  while(nipThis){
    nidp = nipThis->data.ptrvalue;
    sprintf(domName, "%s%c0",nidp->mol, nidp->chain);
    if(constructGrpIdRank(domName, subId, &gn, &gr) <=0){
      nidp->rank = n;
      nipThis->choice = n;
    }
    else {
      nidp->rank = gr;
      nipThis->choice = gr;
    }
    nipThis = nipThis->next;
  }

  nip = ValNodeSort(nip, SortNeighborInfo);

  return nip;

}
/*----- Get ranking info from text file nrpdb.mm/dd/yr-----*/
NeighborInfoPtr GetRankedNeighborInfoFF(NeighborInfoPtr nip, FILE *fp)
{
  NeighborInfoDataPtr nidp = NULL;

  char buf[1024], mol[12], chain;
  Int4 group = 0, rank = 0, groupThis = 0;
  NeighborInfoPtr nip_tmp = NULL;
  Int2 i = 0;

  nidp = nip->data.ptrvalue;

  while(fgets(buf, 1024, fp)){
    if(buf[0]!='#'){
      sscanf(buf, "%s", mol);
      chain = buf[5];
      sscanf(buf+53, "%d", &group);
      sscanf(buf+59, "%d", &rank); 
      if(strcmp(nidp->mol, mol)==0 && nidp->chain == chain) {
        groupThis = group;
        break;
      }
    }
  }

  rewind(fp);
  while(fgets(buf, 1024, fp)){
    if(buf[0]!='#'){
      sscanf(buf, "%s", mol);
      chain = buf[5];
      sscanf(buf+53, "%d", &group);
      sscanf(buf+59, "%d", &rank); 
      if(group == groupThis){
        nip_tmp = nip;
        while(nip_tmp){
          nidp = nip_tmp->data.ptrvalue;
          if(strcmp(nidp->mol, mol)==0 && nidp->chain == chain){
            nidp->rank = rank;
            nip_tmp->choice = rank;
            break;
          }
          nip_tmp = nip_tmp->next;
        }
      }
    }
  }

  nip = ValNodeSort(nip, SortNeighborInfo);

  return nip;
}
/*-----------------------------*/
SnpFeatureInfoPtr SnpFeatureInfoNew()
{
  SnpFeatureInfoPtr sfip = NULL;

  sfip = MemNew(sizeof(SnpFeatureInfo));
  return sfip;
}
/*-----------------------------*/
SnpFeatureInfoPtr SnpFeatureInfoFree(SnpFeatureInfoPtr sfipThis)
{
  if(sfipThis == NULL) return NULL;

  if(sfipThis->title) MemFree(sfipThis->title);
  if(sfipThis->descr) MemFree(sfipThis->descr);

}
/*-----------------------------*/
void CblastAddWWWQueryData(CharPtr name, CharPtr value, Int2 *query_num, WWWQueryPtr PNTR wqp_head) 
{
  WWWQueryDataPtr wqdp;
 
  wqdp = (WWWQueryDataPtr) WWWQueryDataNew();
  wqdp->name = StringSave(name);
  wqdp->value = StringSave(value);
  *query_num++;
  ValNodeAddPointer(wqp_head, *query_num, wqdp);
}
/*-----------------------------*/
CdInfoPtr CdInfoNew()
{
  CdInfoPtr cif = NULL;

  cif = (CdInfoPtr)MemNew(sizeof(CdInfo));
  return cif;
}
/*---------------------------------*/
MapRowInfoPtr MapRowInfoNew()
{
  MapRowInfoPtr mrip = NULL;
  
  mrip = (MapRowInfoPtr)MemNew(sizeof(MapRowInfo));
  return mrip;
}
/*-----------------------------*/
static int LIBCALLBACK SortMapInfoByAlgPos(VoidPtr ptr1, VoidPtr ptr2)
{
  ValNodePtr   vnp1;
  ValNodePtr   vnp2;
  MapRowInfoPtr mrip1 = NULL, mrip2 = NULL;

  if (ptr1 == NULL || ptr2 == NULL) return 0;
  vnp1 = *((ValNodePtr PNTR) ptr1);
  vnp2 = *((ValNodePtr PNTR) ptr2);

  mrip1 = vnp1->data.ptrvalue;
  mrip2 = vnp2->data.ptrvalue;

  if(mrip1->from == mrip2->from) return 0;
  else if(mrip1->from > mrip2->from) return 1;
  else return -1;
        /* sorting based on aligned start position */
}
/*-----------------------------*/
MapRowInfoPtr MapRowInfoFree(MapRowInfoPtr mrip)
{
  if(!mrip) return NULL;

  if(mrip->CdName) MemFree(mrip->CdName);
  if(mrip->cdaln) MemFree(mrip->cdaln);

  MemFree(mrip);
  return NULL;
}
/*-----------------------------*/
CdNbInfoPtr CdNbInfoFree(CdNbInfoPtr cnip)
{
  if(!cnip) return NULL;

  MemFree(cnip->CdName);
  MemFree(cnip); 

  return NULL;
}
/*-----------------------------*/
CdInfoPtr CdInfoFree(CdInfoPtr cip)
{
  Int2 icount = 0;
  CdNbInfoPtr *cnip;
  ValNodePtr mapinfo = NULL, mapinfo_data = NULL, next = NULL;
  MapRowInfoPtr mrip = NULL;

  if(!cip) return NULL;

  cnip = cip->cnip; 
  for(icount=0; icount<cip->cd_num; icount++){
    cnip[icount] = CdNbInfoFree(cnip[icount]);
  }
  MemFree(cnip);
  
  mapinfo = cip->mapinfo;
  while(mapinfo){
    mapinfo_data = mapinfo->data.ptrvalue;
    while(mapinfo_data){
      next = mapinfo_data->next;

      mrip = mapinfo_data->data.ptrvalue;
      mrip = MapRowInfoFree(mrip);
      mapinfo_data = next;
    }
    mapinfo->data.ptrvalue = ValNodeFree(mapinfo->data.ptrvalue);
    mapinfo = mapinfo->next;
  }
  ValNodeFree(cip->mapinfo);

  cip->sap = SeqAnnotFree(cip->sap);
  MemFree(cip);
      
  return NULL;
}
/*-----------------------------*/
CdInfoPtr FillCdInfo(CdInfoPtr cip, Dart_Connect *dartcon)
{
  SeqAnnotPtr sap = NULL;
  SeqAlignPtr salp = NULL;
  DenseSegPtr dsp = NULL;
  ValNodePtr maprow_head = NULL, maprow = NULL, Amaprow_head = NULL, Amaprow = NULL;
  MapRowInfoPtr mrip = NULL;
  Int4 from_dsp = 0, to_dsp = 0, from =0, to = 0;
  Int2 numseg = 0, cd_num = 0, maprow_num = 0;
  Int4Ptr     starts, lens;
  Boolean allocated = FALSE;
  CdNbInfoPtr *cnip = NULL;
  Int2 i=0, j=0, thisColor = 0;
  unsigned Gilist[MaxRelatedCD];
  Int4     ActualNumRows = 0, NumRows = 0, alinumseg = 0;
  Int2   index = 0;
  Char CdName[30];
  Char definition[PATH_MAX], cdalign[PATH_MAX];
  Char str[PATH_MAX];
  

  sap = cip->sap; 
  salp = sap->data;
  cd_num = 0;
  while(salp){
    cd_num++;
    salp = salp->next;
  }
  cip->cd_num = cd_num;
  cnip = (CdNbInfoPtr *)MemNew(cd_num * sizeof(CdNbInfoPtr));
  for(i = 0; i < cd_num; i++) {
    cnip[i] = (CdNbInfoPtr)MemNew(sizeof(CdNbInfo));
    cnip[i]->iClust = -1;
    cnip[i]->iColor = -1;
    cnip[i]->PssmId = 0;
    cnip[i]->bRep = 0;
    cnip[i]->bFinished = 0;
  }

  salp = sap->data;
  cd_num = 0;
  while(salp){
    dsp = salp->segs;
    numseg = dsp->numseg;
    starts = dsp->starts;
    lens = dsp->lens;
    cnip[cd_num]->from  = starts[0];
    cnip[cd_num]->to = starts[(numseg -1)*2] + lens[numseg - 1];
    
    cnip[cd_num]->PssmId = GetPSSMID(dsp);

    if(cnip[cd_num]->PssmId && cnip[cd_num]->PssmId < 10000000){
       if(Dart_CDGi2Acc(dartcon, cnip[cd_num]->PssmId,  CdName, 30)) cnip[cd_num]->CdName = StringSave(CdName);
       else cnip[cd_num]->CdName = StringSave("   ");
    }
    else {
      CharPtr strtmp;
      strtmp = GetCDDName(dsp);
      cnip[cd_num]->CdName = StringSave(strtmp);
    }
    
    cd_num++;
    salp = salp->next;
  }

  thisColor = 0;
  for(i=0; i<cd_num; i++){
    if(cnip[i]->iClust >= 0) continue;
    cnip[i]->iClust = i;
    cnip[i]->iColor = (thisColor++) % iNcolors;
    if(Dart_SameSim(dartcon, cnip[i]->PssmId, Gilist, MaxRelatedCD, &NumRows)) {
      Int2 k;
      if(NumRows <= MaxRelatedCD) ActualNumRows = NumRows;
      else ActualNumRows = MaxRelatedCD;
      for(j=0; j < NumRows; j++){
        for(k=i+1; k<cd_num; k++){
          if(cnip[k]->PssmId == Gilist[j]){
            cnip[k]->iClust = cnip[i]->iClust;
            cnip[k]->iColor = cnip[i]->iColor;
          }
        }
      }
    }
  }

  for(i=0; i<cd_num; i++){
    if(cnip[i]->bFinished) continue;
    cnip[i]->bRep = TRUE;
    cnip[i]->bFinished = TRUE;
/*
printf("i: %d CdName: %s iClust: %d from: %d to: %d\n", i, cnip[i]->CdName, cnip[i]->iClust, cnip[i]->from, cnip[i]->to);
*/
    for(j =i+1; j<cd_num; j++){
      if(cnip[j]->iClust != cnip[i]->iClust) continue;
      if(cnip[j]->bFinished) continue;
      if(cnip[j]->to <= cnip[i]->from || cnip[j]->from >= cnip[i]->to){
        cnip[j]->bFinished = FALSE;
      }
      else {
        cnip[j]->bRep = FALSE;
        cnip[j]->bFinished = TRUE;
      }
    }
  }

/* to confirm cd display info */
/*
  for(i=0; i<cd_num; i++){
    printf("i: %d CdName: %s iClust: %d bRep: %d bFinished: %d from: %d to: %d\n", i, cnip[i]->CdName, cnip[i]->iClust, cnip[i]->bRep, cnip[i]->bFinished, cnip[i]->from, cnip[i]->to);
  }
*/
             
  cd_num = 0;
  maprow_num = 0;
  salp = sap->data;
  while(salp){
    if(!cnip[cd_num]->bRep){
      salp = salp->next;
      cd_num++;
      continue;
    }

    allocated = FALSE;
    dsp = salp->segs;
    numseg = dsp->numseg;
    starts = dsp->starts;
    lens = dsp->lens;

    alinumseg = 0;
    cdalign[0] = '\0';
    for (j=0; j< numseg; j++) {
      index = 2*j;
      if (starts[index]!= -1 && starts[index+1] != -1) {
        sprintf(str, ",%d", starts[index+1]);
        StringCat(cdalign, str);
        sprintf(str, ",%d", starts[index]);
        StringCat(cdalign, str);
        sprintf(str, ",%d", lens[j]);
        StringCat(cdalign, str);
        alinumseg++;
      }
    }
    sprintf(str, "%d", alinumseg);
    StringCat(str, cdalign);

    from_dsp  = starts[0];
    to_dsp = starts[(numseg -1)*2] + lens[numseg - 1];
    maprow = maprow_head;
    while(maprow){
      Amaprow_head = maprow->data.ptrvalue;
      Amaprow = Amaprow_head;
      from = 0;
      while(Amaprow){
        mrip = Amaprow->data.ptrvalue;
        to = mrip->from;
        if(from_dsp > from && to_dsp < to){  
          mrip = MapRowInfoNew();
          mrip->from = from_dsp; mrip->to = to_dsp;
          mrip->CdName = StringSave(cnip[cd_num]->CdName);
          mrip->PssmId = cnip[cd_num]->PssmId;
          mrip->iColor = cnip[cd_num]->iColor;
          mrip->cdaln = StringSave(str);
          ValNodeAddPointer(&Amaprow_head, 0, mrip);
          Amaprow_head = ValNodeSort(Amaprow_head, SortMapInfoByAlgPos);
          maprow->data.ptrvalue = Amaprow_head;
          allocated = TRUE;
          cnip[cd_num]->iMaprow = maprow->choice;
          break;
        }
        from = mrip->to;
        Amaprow = Amaprow->next;
      } 
      if(!allocated && from_dsp > from && to_dsp < (cip->query_length+1)){
        mrip = MapRowInfoNew();
        mrip->from = from_dsp; mrip->to = to_dsp;
        mrip->CdName = StringSave(cnip[cd_num]->CdName);
        mrip->PssmId = cnip[cd_num]->PssmId;
        mrip->iColor = cnip[cd_num]->iColor;
        mrip->cdaln = StringSave(str);
        ValNodeAddPointer(&Amaprow_head, 0, mrip);
        Amaprow_head = ValNodeSort(Amaprow_head, SortMapInfoByAlgPos);
        maprow->data.ptrvalue = Amaprow_head;
        
        allocated = TRUE;
        cnip[cd_num]->iMaprow = maprow->choice;
      }

      if(allocated) break;
      maprow = maprow->next;
    }
    if(!allocated) {
      Amaprow_head = NULL;
         /* starting a new row, no need to do sorting  */
      mrip = MapRowInfoNew();
      mrip->from = from_dsp; mrip->to = to_dsp;
      mrip->CdName = StringSave(cnip[cd_num]->CdName);
      mrip->PssmId = cnip[cd_num]->PssmId;
      mrip->iColor = cnip[cd_num]->iColor;
      mrip->cdaln = StringSave(str);
      ValNodeAddPointer(&Amaprow_head, 0, mrip);
      maprow_num++;
      ValNodeAddPointer(&maprow_head, maprow_num, Amaprow_head); 
      allocated = TRUE;
      cnip[cd_num]->iMaprow = maprow_num;
    }

    salp = salp->next;
    cd_num++;
  }

  cip->maprow_num = maprow_num;
  cip->mapinfo = maprow_head;
  cip->cnip = cnip;
        
  return cip;
}
/*-----------------------------*/
/*-----------------------------*/
BlastPDBNbPtr BlastPDBNbInfoBuild(SeqAnnotPtr sap, Int4 gi_rep, Int4 gi_subject, SeqIdPtr sip, ReadDBFILEPtr rdfp)
{
  SeqAlignPtr salp = NULL;
  SeqIdPtr pdbid = NULL;
  ValNodePtr csip_head = NULL;
  CblastSeqInfoDataPtr csidp = NULL;
  Int2 icount = 0;

  BlastPDBNbPtr bpnp = NULL;
  DocSumPtr entrez_dsp = NULL;

  salp = sap->data;
  while(salp){
    csidp = CblastSeqInfoDataNew();
    pdbid = SeqIdDup(sip);

    csidp->pdbid = pdbid;
    csidp->gi = gi_rep;
    csidp->bRep = 1;
    csidp->PDBgi = gi_subject;
          /* by default, all blast hits will be shown */
 /*
    entrez_dsp = NetDocSum(TYP_AA, gi_subject);
    if(entrez_dsp){
      csidp->doc = StringSave(entrez_dsp->title);
      DocSumFree(entrez_dsp);
    }
    else csidp->doc = StringSave("   ");
*/
    csidp->doc=CblastGetDefline(csidp->PDBgi, rdfp);

    ValNodeAddPointer(&csip_head, icount, csidp); 
    salp = salp->next;
    icount++;
  }
     
  bpnp = BlastPDBNbNew();
  bpnp->sap = AsnIoMemCopy(sap, (AsnReadFunc)SeqAnnotAsnRead, (AsnWriteFunc)SeqAnnotAsnWrite);
  bpnp->csip = csip_head;

  return bpnp;
}
/*------------*/
PDBIdInfoPtr SortPDBIdInfoWithRank(PDBIdInfoPtr piip)
{ 
  PDBSeqIdPtr pdbid = NULL;
  SeqIdPtr pdbList = NULL, tmp = NULL;
  ValNodePtr pdb_giList;

  char domName[10];
  unsigned  gn, gr;
  SubSetID subId;
  Int4 n = 0;

  n = constructSdiCounts();
  subId = (SubSetID)NonIdenticalSeq;

  pdbList = piip->pdbList;
  pdb_giList = piip->PDBgiList;
  while(pdbList){
    pdbid = pdbList->data.ptrvalue;
    sprintf(domName, "%s%c0",pdbid->mol, pdbid->chain);
    if(constructGrpIdRank(domName, subId, &gn, &gr) <=0){
      pdbList->choice = n;
      pdb_giList->choice = n;
    }
    else {
      pdbList->choice = gr;
      pdb_giList->choice = gr;
    }
    pdbList = pdbList->next;
    pdb_giList = pdb_giList->next;
  }

  pdbList = (ValNodePtr)ValNodeSort((ValNodePtr)piip->pdbList, SortNeighborInfo);
  pdb_giList = (ValNodePtr)ValNodeSort(piip->PDBgiList, SortNeighborInfo);
  tmp = pdbList;
  while(tmp){
    tmp->choice = SEQID_PDB;
    tmp = tmp->next;
  }

  piip->pdbList = pdbList;
  piip->PDBgiList = pdb_giList;

  return piip;
}
/*---------to get non-identical pdb neighbor info ----------------*/
BlastPDBNbPtr GetBlastPDBNbInfo(SeqAnnotPtr sap, Int4 blast_rep_gi, Int4 subject_gi, ReadDBFILEPtr rdfp, CharPtr set)
{
  SeqAlignPtr salp = NULL;
  DenseSegPtr dsp = NULL;
  ValNodePtr sip = NULL, pdbList = NULL, pdbid = NULL;
  ValNodePtr avsp_head = NULL, avsp = NULL;
         /* avsp: AlgValidationStatusPtr defined as ValNodePtr */
  Int4 gi = 0, pdb_gi = 0;
  
  ValNodePtr csip_head = NULL;
  CblastSeqInfoDataPtr csidp = NULL;
  Int4 icount = 0;

  ValNodePtr pdb_gilist;
  PDBIdInfoPtr piip = NULL;

  SeqAnnotPtr sap_pdb = NULL; 
  BlastPDBNbPtr bpnp = NULL;
  DocSumPtr entrez_dsp = NULL;

  salp = sap->data;
  while(salp){
    dsp = salp->segs;
    sip = dsp->ids; sip = sip->next;
    gi = sip->data.intvalue;

    avsp = ValNodeNew(NULL);
    avsp->data.intvalue = 0;

    if(strcmp(set, "ALLOneGroup")==0 || strcmp(set, "OnePairMultiHSP")==0) {
      if(blast_rep_gi >0 && blast_rep_gi != gi){
        ValNodeLink(&avsp_head, avsp);
        salp = salp->next;
        continue;
      }

    }

    if(strcmp(set, "OneEachGroup")==0) piip = GetFirstRedundantPDBSeqIdsForGi(gi, rdfp);
    else piip = GetAllRedundantPDBSeqIdsForGi(gi, rdfp);

    if(piip){
      piip = SortPDBIdInfoWithRank(piip);
      pdbid = piip->pdbList;
      pdb_gilist = piip->PDBgiList;
      while(pdbid){
        if(strcmp(set, "OnePairMultiHSP")==0 && pdb_gilist->data.intvalue != subject_gi){
          pdbid = pdbid->next;
          pdb_gilist = pdb_gilist->next;
          continue;
        }

        csidp = CblastSeqInfoDataNew();
        csidp->pdbid = SeqIdDup(pdbid);
        csidp->gi = gi;
        csidp->bRep = 1;
        pdb_gi = pdb_gilist->data.intvalue;
        csidp->PDBgi = pdb_gi;
/*
        entrez_dsp = NetDocSum(TYP_AA, pdb_gi);
        if(entrez_dsp){
          csidp->doc = StringSave(entrez_dsp->title);
          DocSumFree(entrez_dsp);
        }
        else csidp->doc = StringSave("   ");
*/
        csidp->doc=CblastGetDefline(csidp->PDBgi, rdfp);

        ValNodeAddPointer(&csip_head, icount, csidp);
        icount++;

        avsp->data.intvalue++;
        pdbid = pdbid->next;
        pdb_gilist = pdb_gilist->next;
      }
      piip = PDBIdInfoFree(piip);
    }

    ValNodeLink(&avsp_head, avsp);
    salp = salp->next;
  }
      
  bpnp = BlastPDBNbNew();
  salp = sap->data;
  bpnp->sap = CblastAlgTruncateAndExtend(salp, avsp_head);
  bpnp->csip = csip_head;

  ValNodeFree(avsp_head);

  return bpnp;
} 
/*---------------------------------------------------------------------------*/
Boolean print_score_evalue (FloatHi evalue, FloatHi bit_score, CharPtr buf)
{
  Char eval_buff[101];
  Char bit_score_buff[101];

  buf[0] = '\0';
  eval_buff[0] = '\0';
  bit_score_buff[0] = '\0';
  if (evalue < 1.0e-180)     sprintf(eval_buff, "0.0");
  else if (evalue < 1.0e-99) sprintf(eval_buff, "%2.0e", evalue);
  else if (evalue < 0.0009)  sprintf(eval_buff, "%3.0e", evalue);
  else if (evalue < 0.1)     sprintf(eval_buff, "%4.3f", evalue);
  else if (evalue < 1.0)     sprintf(eval_buff, "%3.2f", evalue);
  else if (evalue < 10.0)    sprintf(eval_buff, "%2.1f", evalue);
  else                       sprintf(eval_buff, "%5.0f", evalue);
  if (bit_score > 9999)      sprintf(bit_score_buff, "%4.3e", bit_score);
  else if (bit_score > 99.9) sprintf(bit_score_buff, "%4.0ld", (long)bit_score);
  else                       sprintf(bit_score_buff, "%4.1f", bit_score);
  if (eval_buff[0] != '\0' && bit_score_buff[0] != '\0') {
    sprintf(buf, "S=%s E=%s", bit_score_buff, eval_buff);
    return TRUE;
  }
  if (eval_buff[0] != '\0') {
    sprintf(buf, "E=%s", eval_buff);
    return TRUE;
  }
  if (bit_score_buff[0] != '\0') {
    sprintf(buf, "S=%s", bit_score_buff);
    return TRUE;
  }
  return FALSE;
}
/*---------------------------------------------------------------------------*/
Boolean print_bitscore(FloatHi bit_score, CharPtr buf)
{
  Char bit_score_buff[101];

  buf[0] = '\0';
  bit_score_buff[0] = '\0';

  if (bit_score > 9999)      sprintf(bit_score_buff, "%4.3e", bit_score);
  else if (bit_score > 99.9) sprintf(bit_score_buff, "%4.0ld", (long)bit_score);
  else                       sprintf(bit_score_buff, "%4.0f", bit_score);

  if (bit_score_buff[0] != '\0') {
    sprintf(buf, "%s", bit_score_buff);
    return TRUE;
  }
  return FALSE;

}
/*---------------------------------------------------------------------------*/
Boolean print_evalue (FloatHi evalue, CharPtr buf)
{
  Char eval_buff[101];

  buf[0] = '\0';
  eval_buff[0] = '\0';
  if (evalue < 1.0e-180)     sprintf(eval_buff, "0.0");
  else if (evalue < 1.0e-99) sprintf(eval_buff, "%2.0e", evalue);
  else if (evalue < 0.0009)  sprintf(eval_buff, "%3.0e", evalue);
  else if (evalue < 0.1)     sprintf(eval_buff, "%4.3f", evalue);
  else if (evalue < 1.0)     sprintf(eval_buff, "%3.2f", evalue);
  else if (evalue < 10.0)    sprintf(eval_buff, "%2.1f", evalue);
  else                       sprintf(eval_buff, "%5.0f", evalue);

  if (eval_buff[0] != '\0') {
    sprintf(buf, "%s", eval_buff);
    return TRUE;
  }
  return FALSE;
}
/*---------------------------------------------------------------------------*/
void printscore(CblastScoreDataPtr csdp, FILE *File)
{
  char str_score[1024], percentage_symbol = '%';

  fprintf(File, "<table border=\"0\" cellpadding=\"1\" cellspacing=\"0\">\n");
  fprintf(File, "<tr><td>&nbsp</td></tr>\n");

  print_bitscore(csdp->bit_score, str_score);
  fprintf(File, "<tr><td>Score(bits) = %s,</td>", str_score);
  fprintf(File, "<td>&nbsp</td>");

  print_evalue(csdp->evalue, str_score);
  fprintf(File, "<td>E_value = %s</td></tr>\n", str_score);

  sprintf(str_score, "%d\n", csdp->length);
  fprintf(File, "<tr><td>Aligned Length = %s,</td>\n", str_score);
  fprintf(File, "<td>&nbsp</td>");

/*sprintf(str_score, "%4.2f\n", csdp->seq_identity); */
  sprintf(str_score, "%d\n", (int)(100*csdp->seq_identity));
  fprintf(File, "<td>Sequence Identity = %s%c</td></tr>\n", str_score, percentage_symbol);

  fprintf(File, "<tr><td>&nbsp</td></tr>\n");
  fprintf(File, "</table>\n");

}
/*------------*/
void print_query_args(FILE *File, WWWQueryPtr wqp_head, ValNodePtr redefine_query_head)
{
  WWWQueryPtr wqp = NULL;
  WWWQueryDataPtr wqdp = NULL;
  ValNodePtr query = NULL;
  Boolean ReDefine = FALSE;

  wqp = wqp_head;
  while(wqp){
    wqdp = wqp->data.ptrvalue;
    ReDefine = FALSE;
    query = redefine_query_head;
    while(query){
      if(strcmp(wqdp->name, query->data.ptrvalue)==0){
        ReDefine = TRUE;
        break;
      }
      query = query->next;
    }
    if(ReDefine){
      wqp = wqp->next;
      continue;
    }
    fprintf(File, "&%s=%s", wqdp->name, wqdp->value);
    wqp = wqp->next;
  }

}
/*------------*/
void SetMMDBSubsetRep(BlastPDBNbPtr bpnp, Int2 subset)
{

  Int4 n = 0, count = 0, i = 0;
  unsigned  gn, gr, *min_ranks, *group_num, *group_rank;
  char domName[10];
  SubSetID subId;

  ValNodePtr csip = NULL;
  CblastSeqInfoDataPtr csidp = NULL;
  ValNodePtr bRepList = NULL, pdbList = NULL;
  PDBSeqIdPtr pdb_sip = NULL;

  if(subset == 1) return;
  else if(subset == 2) subId = (SubSetID)NonIdenticalSeq;
  else if(subset == 3) subId = (SubSetID)NRBlast10e_80;
  else if(subset == 4) subId = (SubSetID)NRBlast10e_40;
  else if(subset == 5) subId = (SubSetID)NRBlast10e_7;

  n = constructSdiCounts();

  min_ranks = (unsigned *)MemNew(n*sizeof(unsigned));
  group_num = (unsigned *)MemNew(n*sizeof(unsigned));
  group_rank = (unsigned *)MemNew(n*sizeof(unsigned));
  
  for(i=0; i < n; i++){
    group_num[i] = 0;
    min_ranks[i] = n + 1;
    group_rank[i] = n + 1;
  }

  count = 0;
  csip = bpnp->csip;
  while(csip){
    csidp = csip->data.ptrvalue;
    pdb_sip = csidp->pdbid->data.ptrvalue;
    sprintf(domName, "%s%c0", pdb_sip->mol, pdb_sip->chain);
    if(constructGrpIdRank(domName, subId, &gn, &gr) <=0){
      group_num[count] = 0;
      group_rank[count] = 0;      
      csidp->bRep = 0;
    }
    group_num[count] = gn;
    group_rank[count] = gr;
    if(gr !=0 && gr < min_ranks[gn - 1]) min_ranks[gn - 1] = gr;

    count++;
    csip = csip->next;
  }

  count = 0;
  csip = bpnp->csip;
  while(csip){
    csidp = csip->data.ptrvalue;
    gn = group_num[count]; 
    if(group_rank[count] == min_ranks[gn - 1]) csidp->bRep = 1;
    else csidp->bRep = 0;

    count++;
    csip = csip->next;
  }

}
/*---------------------*/
Cn3dStyleTableItemPtr MakeCn3dStyleTableForSNP(ValNodePtr sfp)
{
  SnpFeatureInfoPtr sfipThis = NULL;
  Cn3dStyleTableItemPtr cstip_head = NULL, cstip = NULL;
  Cn3dStyleSettingsPtr      cssp = NULL;
  Cn3dGeneralStylePtr cgsp = NULL;
  Cn3dColorPtr ccp = NULL;
  Cn3dBackboneStylePtr cbsp = NULL;
  Cn3dBackboneLabelStylePtr cblsp = NULL;
  Int4 r = 0, g = 0, b = 0;
  

  while(sfp){
    sfipThis = sfp->data.ptrvalue;
    if(cstip_head == NULL){
      cstip_head = Cn3dStyleTableItemNew();
      cstip = cstip_head;
    }
    else {
      cstip->next = Cn3dStyleTableItemNew();
      cstip = cstip->next;
    }

    r =snp_color[sfipThis->color-1][0]; 
    g = snp_color[sfipThis->color-1][1]; 
    b = snp_color[sfipThis->color-1][2];
    cstip->id = sfipThis->feature_id;
    cssp = (Cn3dStyleSettingsPtr) GetDefaultCn3DStyleSettings();

    cbsp = cssp->protein_backbone;
    cbsp->type = Cn3d_backbone_type_trace;
    cbsp->style = Cn3d_drawing_style_tubes;
         /* setting mainchain drawing style */
    cbsp->color_scheme = Cn3d_color_scheme_user_select;
      ccp = cbsp->user_color;
      SetCn3dColor(ccp, 255, r, g, b, 255); 
        /* setting mainchain drawing color here */
 
    cgsp = cssp->protein_sidechains;
    cgsp->style = Cn3d_drawing_style_tubes;
         /* setting sidechain drawing style */
    cgsp->color_scheme = Cn3d_color_scheme_user_select;
      ccp = cgsp->user_color;
      SetCn3dColor(ccp, 255, r, g, b, 255); 
    /* setting sidechain drawing color here */

    if(sfipThis->id == 1){
      cblsp = cssp->protein_labels;
      cblsp->spacing = 1;
      cblsp->type=2;
      cblsp->number=1;
              /* Cn3d_backbone_label_style_number_sequential */
      cblsp->white=1;
    }

    cstip->style = cssp;
    sfp = sfp->next;
  }

  return cstip_head;
}
/*---------------------*/
Cn3dStyleDictionaryPtr MakeCn3dStyleDicForSNP(ValNodePtr sfp)
{
  Cn3dStyleDictionaryPtr csdp = NULL;

  csdp = Cn3dStyleDictionaryNew();
  csdp->global_style = (Cn3dStyleSettingsPtr) GetDefaultCn3DStyleSettings();
  csdp->style_table = (Cn3dStyleTableItemPtr) MakeCn3dStyleTableForSNP(sfp);

  return csdp;
}
/*---------------------*/
Cn3dUserAnnotationsPtr MakeUserAnnotForSNP(ValNodePtr sfp, Int4 mmdb_id, Int4 mol_id)
{
  SnpFeatureInfoPtr sfipThis = NULL;

  Cn3dUserAnnotationsPtr   cuasp = NULL;
  Cn3dUserAnnotationPtr    cuap = NULL, cuap_head = NULL;
  Cn3dObjectLocationPtr    colp = NULL;
  Cn3dMoleculeLocationPtr cmlp = NULL;
  Cn3dResidueRangePtr crrp = NULL;

  while(sfp){
    sfipThis = sfp->data.ptrvalue;
    if(!cuap_head){
      cuap_head = (Cn3dUserAnnotationPtr) Cn3dUserAnnotationNew();
      cuap = cuap_head;
    }
    else {
      cuap->next = (Cn3dUserAnnotationPtr) Cn3dUserAnnotationNew();
      cuap = cuap->next;
    }
    cuap->style_id = sfipThis->feature_id;
    cuap->is_on = TRUE;
    cuap->name = StringSave(sfipThis->title);
    cuap->description = StringSave(sfipThis->descr);

    crrp = Cn3dResidueRangeNew();
    crrp->from = sfipThis->pos;
    crrp->to = sfipThis->pos;
    crrp->next = NULL;

    cmlp = Cn3dMoleculeLocationNew();
    cmlp->residues = crrp;
    cmlp->molecule_id = mol_id;
      
    colp = Cn3dObjectLocationNew();
    colp->residues = cmlp;
    colp->structure_id = ValNodeNew(NULL); 
    colp->structure_id->choice = BiostrucId_mmdb_id;
    colp->structure_id->data.intvalue = mmdb_id;

    colp->next = NULL;
  
    cuap->residues = colp;    

    sfp = sfp->next;
  }

  cuasp = (Cn3dUserAnnotationsPtr) Cn3dUserAnnotationsNew();
  cuasp->annotations = cuap_head; 
  cuasp->view = NULL;

  return cuasp;
}
/*--------------------*/
void SapMSInvert(SeqAnnotPtr sap)
{
  SeqAlignPtr salp = NULL;
  DenseSegPtr dsp = NULL;
  SeqIdPtr sip1 = NULL, sip2 = NULL;
  Int4Ptr     starts, lens;
  Int4 i = 0, numseg = 0, algpos = 0;

  salp = sap->data;
  dsp = salp->segs;

  sip2 = dsp->ids->next;
  sip1 = dsp->ids;
  sip1->next = NULL;
  sip2->next = sip1;
  dsp->ids = sip2;

  numseg = dsp->numseg;
  starts = dsp->starts;
  lens = dsp->lens;

  for(i=0; i<numseg; i++){
    algpos = starts[2*i];  
    starts[2*i] = starts[2*i + 1];
    starts[2*i + 1] = algpos;
  }

}
/*----------------------*/
BlastPDBNbPtr CblastIndexHSP(BlastPDBNbPtr bpnp_head)
{
  ValNodePtr csip = NULL, csip1 = NULL, csip2 = NULL;
  CblastSeqInfoDataPtr csidp = NULL, csidp1 = NULL, csidp2 = NULL;
  
 
  ValNodePtr hspList_head = NULL, hspList1 = NULL, hspList2 = NULL;
  ValNodePtr giList1 = NULL, giList2 = NULL; 
  Int2 hsp_num = 0;
  Int4 gi1 = 0, gi2 = 0;

  if(!bpnp_head) return NULL;

  csip = bpnp_head->csip;
  while(csip){
    csidp = csip->data.ptrvalue;
    csidp->hsp = 0;
    csip = csip->next;
  }        /* initialize, may not be neccessary */
    
  csip1 = bpnp_head->csip;
  while(csip1){
    csidp1 = csip1->data.ptrvalue;
    if(csidp1->hsp > 0) {
       csip1 = csip1->next;
       continue;
    }

    if(csidp1->hsp == 0) {
      csidp1->hsp = 1;
      hsp_num = 1; 
    }
    gi1 = csidp1->PDBgi;

    csip2 = csip1->next;
    while(csip2){
      csidp2 = csip2->data.ptrvalue;
      gi2 = csidp2->PDBgi;
      if(gi2 == gi1) {
        hsp_num++;
        csidp2->hsp = hsp_num;
      }
      csip2 = csip2->next;
    }

    csip1 = csip1->next;
  }

  return bpnp_head;

}
/*----------------------*/
BlastPDBNbPtr CblastGetAlignScoreParam(BlastPDBNbPtr bpnp)
{
  CblastScoreDataPtr csdp = NULL;
  ValNodePtr csp_head = NULL;
  Int4 icount = 0;


  SeqAlignPtr salp = NULL;
  DenseSegPtr dsp = NULL;
  Int4Ptr starts, lens;
  Int2 iseg = 0;
  Int4 start_master = 0, start_slave = 0, alg_length = 0;

  FloatHi      evalue = 0.0;
  Int4         number = 0;
  FloatHi      bit_score = 0.0;
  Int4         score = 0;

  Int4 match = 0;
  AlignSum as;
  ScorePtr sp = NULL;

  if(bpnp->sap == NULL) return bpnp;
  salp = bpnp->sap->data;
  while(salp){
    GetScoreAndEvalue(salp, &score, &bit_score, &evalue, &number);
    csdp = CblastScoreDataNew();
    csdp->evalue = evalue;
    csdp->bit_score = bit_score; 
    csdp->score = score; 

    dsp = salp->segs;
    starts = dsp->starts; 
    lens = dsp->lens;
    alg_length = 0;
    for(iseg = 0; iseg < dsp->numseg; iseg++){
      start_master = *starts; starts++;
      start_slave = *starts; starts++;
      if(start_master == -1 || start_slave == -1){
        lens++; 
        continue;
      }
      else alg_length += *lens;
      lens++;
    }
    csdp->length = alg_length;

    MemSet(&as, 0, sizeof(AlignSum));
    as.master_sip =(SeqIdPtr) SeqIdPtrFromSeqAlign(salp) ;
    as.target_sip = (SeqIdPtr)((SeqIdPtr)SeqIdPtrFromSeqAlign(salp))->next;
    sp = cblast_find_score_in_align(salp, 1, &as);
    match=as.identical;
    csdp->seq_identity = (float)match/csdp->length;
/*
printf("match: %d total_len: %d percentage: %f\n", match, csdp->length, csdp->seq_identity);
*/
   
       /* get sequence identity */
    ValNodeAddPointer(&csp_head, 0, csdp);
    icount++;
    salp = salp->next;
  }

  bpnp->csp = csp_head;
  return bpnp; 

}
/*-------------------*/
SeqAlignPtr CblastSeqAlignSort(SeqAlignPtr list, FloatHi *value, int direction)
{
  SeqAlignPtr tmp, PNTR head;
  Int4 count = 0, i = 0, j = 0;
  FloatHi tmp_value = 0.0;

  if(list == NULL) {
    return NULL;
  }

  tmp = list;
  while(tmp){
    count++;
    tmp = tmp->next;
  }

  head = (SeqAlignPtr *) MemNew (((size_t) count + 1) * sizeof (SeqAlignPtr));
  for (tmp = list, i = 0; tmp != NULL && i < count; i++) {
    head [i] = tmp;
    tmp = tmp->next;
  }

  for(i = 0; i < count - 1; i++){
    for(j = i + 1; j < count; j++){
      if(direction == 1) {   /* sorting ascendly */
        if(value[i] > value[j]){

          tmp_value = value[i];
          value[i] = value[j];
          value[j] = tmp_value;

          tmp = head[i];
          head[i] = head[j];
          head[j] = tmp;
        }
      }
      else if(direction == 2){  /* sorting descendly */
        if(value[i] < value[j]){
          tmp_value = value[i];
          value[i] = value[j];
          value[j] = tmp_value;

          tmp = head[i];
          head[i] = head[j];
          head[j] = tmp;
        }
      }
    }
  }

  for(i = 0; i < count; i++) {
    tmp = head [i];
    tmp->next = head [i + 1];
  }
  list = head [0];
  MemFree (head);

  return list;
}

/*-------------------*/
ValNodePtr CblastValNodeSort(ValNodePtr list, FloatHi *value, int direction)
{
  ValNodePtr tmp, PNTR head;
  Int4 count = 0, i = 0, j = 0;
  FloatHi tmp_value = 0.0;

  if(list == NULL) {
    return NULL;
  }

  count = ValNodeLen(list);
  head = (ValNodePtr *) MemNew (((size_t) count + 1) * sizeof (ValNodePtr));
  for (tmp = list, i = 0; tmp != NULL && i < count; i++) {
    head [i] = tmp;
    tmp = tmp->next;
  }

  for(i = 0; i < count - 1; i++){
    for(j = i + 1; j < count; j++){
      if(direction == 1) {   /* sorting ascendly */
        if(value[i] > value[j]){

          tmp_value = value[i];
          value[i] = value[j];
          value[j] = tmp_value;

          tmp = head[i];
          head[i] = head[j];
          head[j] = tmp; 
        }
      }
      else if(direction == 2){  /* sorting descendly */
        if(value[i] < value[j]){
          tmp_value = value[i];
          value[i] = value[j];
          value[j] = tmp_value;

          tmp = head[i];
          head[i] = head[j];
          head[j] = tmp;
        }
      }
    }
  }

  for(i = 0; i < count; i++) {
    tmp = head [i];
    tmp->next = head [i + 1];
  }
  list = head [0];
  MemFree (head);

  return list;
}
/*-------------------*/
void value_copy(FloatHi *value_ori, FloatHi *value, Int4 count)
{
  Int4 i = 0;
  for(i = 0; i< count; i++){
    value[i] = value_ori[i];
  }

}
/*-------------------*/
BlastPDBNbPtr SortBlastPDBNb(BlastPDBNbPtr bpnp, Int2 sortpara)
{

  ValNodePtr csp = NULL;
  CblastScoreDataPtr csdp = NULL;

  Int4 count = 0;
  FloatHi *value_ori = NULL, *value = NULL;

  csp = bpnp->csp;
  count = ValNodeLen(csp);
  value_ori = (FloatHi *)MemNew(((size_t)count + 1) * sizeof(FloatHi));
  value = (FloatHi *)MemNew(((size_t)count + 1) * sizeof(FloatHi));
  count = 0;
  while(csp){
    csdp = csp->data.ptrvalue;
    if(sortpara == 1) value_ori[count] = csdp->evalue;
    else if(sortpara == 2) value_ori[count] = csdp->bit_score;
    else if(sortpara == 3) value_ori[count] = csdp->length;
    else if(sortpara == 4) value_ori[count] = csdp->seq_identity; 

    count++;
    csp = csp->next;
  }
  
  if(sortpara == 1){
    value_copy(value_ori, value, count);
    bpnp->sap->data = (SeqAlignPtr)CblastSeqAlignSort((SeqAlignPtr) bpnp->sap->data, value, 1);
    value_copy(value_ori, value, count);
    bpnp->csp = (ValNodePtr)CblastValNodeSort((ValNodePtr) bpnp->csp, value, 1);
    value_copy(value_ori, value, count);
    bpnp->csip = (ValNodePtr)CblastValNodeSort((ValNodePtr) bpnp->csip, value, 1);
  }
  else {
    value_copy(value_ori, value, count);
    bpnp->sap->data = (SeqAlignPtr)CblastSeqAlignSort((SeqAlignPtr) bpnp->sap->data, value, 2);
    value_copy(value_ori, value, count);
    bpnp->csp = (ValNodePtr)CblastValNodeSort((ValNodePtr) bpnp->csp, value, 2);
    value_copy(value_ori, value, count);
    bpnp->csip = (ValNodePtr)CblastValNodeSort((ValNodePtr) bpnp->csip, value, 2);
  }
 
  MemFree(value);
  MemFree(value_ori);

  return bpnp;
}
/*-------------*/
BlastPDBNbPtr CblastIndexPage(BlastPDBNbPtr bpnp)
{
  ValNodePtr csip = NULL;
  CblastSeqInfoDataPtr csidp = NULL;
  Int4 count = 0;
  Int2 numpages = 0;

  csip = bpnp->csip;
  while(csip){
     csidp = csip->data.ptrvalue;
     if(csidp->bRep){
       csidp->pagenum = ((count - count%MaxEntryPerPage)/MaxEntryPerPage) + 1;
       count++;
     }
     else csidp->pagenum = 0;
     csip = csip->next;
  }

  count = count-1;
  bpnp->numpages = ((count - count%MaxEntryPerPage)/MaxEntryPerPage) + 1; 

  return bpnp;
}
/*-----------------------*/
Int4 GetSelectedAlgSeqNum(BlastPDBNbPtr bpnp, Int2 pagenum)
{
  SeqAlignPtr salp = NULL;
  ValNodePtr csip = NULL;
  CblastSeqInfoDataPtr csidp = NULL;
  Int4 algseq = 0;

  salp = bpnp->sap->data;
  csip = bpnp->csip;
  algseq = 0;
  while(salp){
    csidp = csip->data.ptrvalue;
    if(csidp->pagenum == pagenum ) algseq++;
    salp = salp->next;
    csip = csip->next;
  }

  return algseq;
}
