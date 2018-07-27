/*   udvseq.c
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
* File Name:  udvseq.c
*
* Author:  Patrick Durand
*
* Version Creation Date:   5/3/99
*
* $Revision: 6.23 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: udvseq.c,v $
* Revision 6.23  2001/10/03 00:15:47  vakatov
* Replaced some ASSERTs by VERIFYs
*
* Revision 6.22  2000/05/31 21:05:38  kans
* suppress all source features
*
* Revision 6.21  2000/05/09 17:00:18  kans
* suppresses full-length biosource features
*
* Revision 6.20  2000/05/08 19:24:22  lewisg
* fix UDV_CreateParaGList to stop at last character instead of one before
*
* Revision 6.19  2000/04/13 13:57:33  durand
* allowed udv to display reverse complement sequence
*
* Revision 6.18  2000/02/03 15:55:14  hurwitz
* added constructor and destructor for MsaTxtDisp
*
* Revision 6.17  1999/12/02 13:47:32  durand
* add new function for Entrez sequence viewer
*
* Revision 6.16  1999/11/03 13:46:15  durand
* add UDV_GetStrandinPGP, UDV_GetStrandinPGPList and UDV_GetBspRangeinPGPList for DDV
*
* Revision 6.15  1999/10/02 15:11:09  durand
* update the code to be used by wwwudv
*
* Revision 6.14  1999/09/16 18:51:52  durand
* move MsaTxtDisp struct from pgppop.h to udvseq.h
*
* Revision 6.13  1999/09/14 12:09:48  durand
* don't show publication features
*
* Revision 6.12  1999/09/07 19:39:18  durand
* don't display special features
*
* Revision 6.11  1999/08/07 17:04:00  sicotte
* fixed cast warning
*
* Revision 6.10  1999/07/31 14:28:07  durand
* take into account parent for seg-set
*
* Revision 6.9  1999/07/30 20:08:27  durand
* updates for the new Entrez graphical viewer
*
* Revision 6.8  1999/07/21 21:38:45  durand
* fix a bug for the display of spanning features on circular DNA
*
* Revision 6.7  1999/07/12 21:12:09  durand
* fix a bug in the cgi-bin release of UDV
*
* Revision 6.6  1999/06/16 13:07:51  durand
* update UDV functions to be used by DDV
*
* Revision 6.5  1999/06/15 15:24:40  durand
* update for DDV
*
* Revision 6.4  1999/06/07 15:10:19  durand
* add LOG line to keep track of the history
*
*
*
* ==========================================================================
*/

#include <udvseq.h>
#include <udvdef.h>
#include <subutil.h>
#include <jzmisc.h>

/*******************************************************************************

  Function : WhatBspName()
  
  Purpose : retrieve the name of the bsp. Currently call only BioseqGetTitle;
  			may be changed in the future
  
  Parameters : bsp; pointer to the Bioseq
  				szName; filled with the Bioseq's name
  
  Return value : none

*******************************************************************************/
static void  WhatBspName(BioseqPtr bsp,CharPtr szName)
{
	szName=BioseqGetTitle(bsp);
}

/*******************************************************************************

  Function : WhatBspID()
  
  Purpose : retrieve the identifier of the bsp. 
  
  Parameters : bsp; pointer to the Bioseq
  				szAccess; filled with the Bioseq's accession number
  
  Return value : none

*******************************************************************************/
static void  WhatBspID(BioseqPtr bsp,CharPtr szAccess)
{
	SeqIdWrite(bsp->id,szAccess,PRINTID_TEXTID_ACCESSION,20);
}


/*******************************************************************************

  Function : WhatBspDataType()
  
  Purpose : retrieve the coding type of the sequence.
  
  Parameters : bsp; pointer to the Bioseq
  				szDtype; filled with the Bioseq's coding type
  
  Return value : none

*******************************************************************************/
static void  WhatBspDataType(BioseqPtr bsp,CharPtr szDtype)
{
Uint1 i;
Char *datatype[]={"IUPACna","IUPACaa","NCBI2na","NCBI4na",
					"NCBI8na","NCBIpna","NCBI8aa","NCBIeaa",
					"NCBIpaa","iupacaa3","NCBIstdaa","Unknown"};
	
	i=bsp->seq_data_type;

	switch(i){
		case Seq_code_iupacna:
		case Seq_code_iupacaa:
		case Seq_code_ncbi2na:
		case Seq_code_ncbi4na:
		case Seq_code_ncbi8na:
		case Seq_code_ncbipna:
		case Seq_code_ncbi8aa:
		case Seq_code_ncbieaa:
		case Seq_code_ncbipaa:
		case Seq_code_iupacaa3:
		case Seq_code_ncbistdaa:
			StringCpy(szDtype,datatype[i-1]);
			break;
		default:
			StringCpy(szDtype,datatype[11]);
			break;
	}
}

/*******************************************************************************

  Function : WhatBspStrand()
  
  Purpose : retrieve the strand def. of the BioSeq
  
  Parameters : bsp; pointer to the Bioseq
  				szStrand; filled with the Bioseq's strand def.
  
  Return value : none

*******************************************************************************/
static void  WhatBspStrand(BioseqPtr bsp,CharPtr szStrand)
{
Uint1 i;
Char *strand[]={"Single","Double","Mixed","Unknown"};
	
	i=bsp->strand;
	switch(i){
		case 1:
		case 2:
		case 3:
			StringCpy(szStrand,strand[i-1]);
			break;
		default:
			StringCpy(szStrand,strand[3]);
			break;
	}
}



/*******************************************************************************

  Function : WhatBspTopo()
  
  Purpose : retrieve the topology of the Bioseq
  
  Parameters : bsp; pointer to the Bioseq
  				szTopo; filled with the Bioseq's topology.
  
  Return value : none

*******************************************************************************/
static void  WhatBspTopo(BioseqPtr bsp,CharPtr szTopo)
{
Uint1 i;
Char *topo[]={"Linear","Circular","Tandem","Unknown"};
	
	i=bsp->topology;
	switch(i){
		case 1:
		case 2:
		case 3:
			StringCpy(szTopo,topo[i-1]);
			break;
		default:
			StringCpy(szTopo,topo[3]);
			break;
	}
}

/*******************************************************************************

  Function : WhatBspMol()
  
  Purpose : what's the bioseq ?
  
  Parameters : bsp; pointer to the Bioseq
  				szMol; filled with the Bioseq's type.
  
  Return value : TRUE if Bioseq is nucleic acid

*******************************************************************************/
static Boolean  WhatBspMol(BioseqPtr bsp,CharPtr szMol)
{
Uint1 i;
Char *mol[]={"DNA","RNA","AA","NA","Unknown"};				
	
	i=bsp->mol;
	switch(i){
		case Seq_mol_dna:
		case Seq_mol_rna:
		case Seq_mol_aa:
		case Seq_mol_na:
			StringCpy(szMol,mol[i-1]);
			break;
		default:
			StringCpy(szMol,mol[4]);
			break;
	}
	
	return(ISA_na(i));	
}

/*******************************************************************************

  Function : WhatBspRepr()
  
  Purpose : What's the bioseq representation
  
  Parameters : bsp; pointer to the Bioseq
  				szRepr; filled with the Bioseq's representation.
  
  Return value : none

*******************************************************************************/
static void  WhatBspRepr(BioseqPtr bsp,CharPtr szRepr)
{
Uint1 i;
Char *repr[]={"Virtual","Raw","Segmented","Constructed",
				"Reference","Consensus","Map","Delta","Unknown"};

	i=Bioseq_repr(bsp);
	switch(i){
		case Seq_repr_virtual:
		case Seq_repr_raw:
		case Seq_repr_seg:
		case Seq_repr_const:
		case Seq_repr_ref:
		case Seq_repr_consen:
		case Seq_repr_map:
		case Seq_repr_delta:
			StringCpy(szRepr,repr[i-1]);
			break;
		default:
			StringCpy(szRepr,repr[8]);
			break;
	} 
}

/*******************************************************************************

  Function : UDV_ReadBspDataForViewer()
  
  Purpose : call all of the above functions; exported function
  
  Parameters : 
  
  Return value : 

*******************************************************************************/
NLM_EXTERN void  UDV_ReadBspDataForViewer(BspInfoPtr bsp_i)
{
	WhatBspName(bsp_i->bsp,bsp_i->bspName);
	WhatBspID(bsp_i->bsp,bsp_i->bspAccNum);
	WhatBspDataType(bsp_i->bsp,bsp_i->bspDataType);
	WhatBspStrand(bsp_i->bsp,bsp_i->bspStrand);
	WhatBspTopo(bsp_i->bsp,bsp_i->bspTopo);
	bsp_i->bspMolNuc=WhatBspMol(bsp_i->bsp,bsp_i->bspMol);
	WhatBspRepr(bsp_i->bsp,bsp_i->bspRepr);
	bsp_i->bspLength=BioseqGetLen(bsp_i->bsp);
}

/*******************************************************************************

  Function : UDV_FreeListParaG()
  
  Purpose : delete the ParaG population
  
  Parameters : head of the ParaG val_node list
  
  Return value : none

*******************************************************************************/
NLM_EXTERN void UDV_FreeListParaG(ValNodePtr PNTR vnp_head)
{
ValNodePtr vnp,vnp2;
ParaGPtr pgp;


	if ((*vnp_head)){
		for(vnp=(*vnp_head) ; vnp != NULL ; vnp=vnp->next){
			if (vnp->data.ptrvalue){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				if (pgp->pFeatList) ValNodeFree(pgp->pFeatList);
				if (pgp->ptxtList){
					for(vnp2=pgp->ptxtList;vnp2 != NULL ; vnp2=vnp2->next){
						if(vnp2->data.ptrvalue) MemFree(vnp2->data.ptrvalue);
					}
					ValNodeFree(pgp->ptxtList);
				}
				Free(pgp);
			}		
		}	

		ValNodeFree(*vnp_head);
		*vnp_head=NULL;
	}
}

/*******************************************************************************

Function: UDV_DecodeIdxFeat()

Purpose: decode a 32 bits integer containing val1 (lower 16 bits)
		and val2 (higher 16 bits)

Parameters: index_g; value to decode in 'val1' and 'val2'

*******************************************************************************/

NLM_EXTERN void  UDV_DecodeIdxFeat (Uint4 index_g, Uint2Ptr val1,
									   Uint2Ptr val2)
{
Uint2Ptr  index_g2;

	index_g2 = (Uint2Ptr) (&index_g);
	if (val1) *val1 = (Uint2) index_g2 [0];
	if (val2) *val2 = (Uint2) index_g2 [1];
}

/*******************************************************************************

Function: UDV_BigDecodeIdxFeat()

Purpose: decode a 64 bits integer into 16 bits integers

Parameters: index_g; value to decode in 'val1' and 'val2'

*******************************************************************************/

NLM_EXTERN void  UDV_BigDecodeIdxFeat (Uint8 index_g, Uint2Ptr val1, Uint2Ptr val2,
	Uint2Ptr val3, Uint2Ptr val4)
{
Uint2Ptr  index_g2;

	index_g2 = (Uint2Ptr) (&index_g);
	if (val1) *val1 = (Uint2) index_g2 [0];
	if (val2) *val2 = (Uint2) index_g2 [1];
	if (val3) *val3 = (Uint2) index_g2 [2];
	if (val4) *val4 = (Uint2) index_g2 [3];
}

/*******************************************************************************

Function: UDV_EncodeIdxFeat()

Purpose: encode a 32 bits integer with val1 (lower 16 bits)
		and val2 (higher 16 bits)

Parameters: values to decode -> 'val1' and 'val2'

Return : the 32 bits encoded value

*******************************************************************************/

NLM_EXTERN Uint4  UDV_EncodeIdxFeat (Uint2 val1,Uint2 val2)
{
Uint2 index_g[2];
	
	index_g[0]=val1;
	index_g[1]=val2;
	
	return *((Int4Ptr) index_g);
	
}

/*******************************************************************************

Function: UDV_BigEncodeIdxFeat()

Purpose: encode a 64 bits integer with 4 16 bits integers

Parameters: values to decode 

Return : the 64 bits encoded value

*******************************************************************************/

NLM_EXTERN Uint8 UDV_BigEncodeIdxFeat (Uint2 val1,Uint2 val2,Uint2 val3,Uint2 val4)
{
Uint2 index_g[4];
	
	index_g[0]=val1;
	index_g[1]=val2;
	index_g[2]=val3;
	index_g[3]=val4;
	
	return *((Uint8Ptr) index_g);
	
}

/*******************************************************************************

Function: UDV_ConvertFeatContext()

Purpose: convert the coordinates from master bioseq to a part sequence (seg-set)

*******************************************************************************/
NLM_EXTERN SeqMgrFeatContextPtr UDV_ConvertFeatContext(
		SeqMgrFeatContextPtr context,Int4 cumOffset,Int4 bsp_part_length)
{
SeqMgrFeatContextPtr newContext;
Int2 i,i2;

	newContext=(SeqMgrFeatContextPtr)MemNew(sizeof(SeqMgrFeatContext));
	
	if (!newContext) return(NULL);
	
	memcpy(newContext,context,sizeof(SeqMgrFeatContext));
	
	newContext->left=context->left-cumOffset;
	if (newContext->left<0 &&  
			newContext->bsp->topology!=TOPOLOGY_CIRCULAR) newContext->left=0;
	newContext->right=context->right-cumOffset;
	if (newContext->right<0 &&  
			newContext->bsp->topology!=TOPOLOGY_CIRCULAR) newContext->right=0;
	if (newContext->right>bsp_part_length){
		newContext->right=bsp_part_length-1;
	}
	
	i2=context->numivals*2;
	newContext->ivals=(Int4Ptr)MemNew((i2)*sizeof(Int4));
	for(i=0;i<i2;i+=2){
		newContext->ivals[i]=context->ivals[i]-cumOffset;
		/*if (newContext->ivals[i]<0 && 
			newContext->bsp->topology!=TOPOLOGY_CIRCULAR) newContext->ivals[i]=0;*/
		newContext->ivals[i+1]=context->ivals[i+1]-cumOffset;
		/*if (newContext->ivals[i+1]<0 &&  
			newContext->bsp->topology!=TOPOLOGY_CIRCULAR) newContext->ivals[i+1]=0;*/
	}

	return(newContext);
}

/*******************************************************************************

Function: UDV_ComputeBspCoordRangeinPGP()

Purpose: compute from and to in Bioseq coordinates given the line descriptor
         (pgp->ptxtList) of a ParaG invloved in a multiple sequence 
		 alignement (MSA).

Parameters: pgp; the ParaG to analyse
            from, to; return values

*******************************************************************************/
NLM_EXTERN void UDV_ComputeBspCoordRangeinPGP(ParaGPtr pgp,Int4Ptr from, Int4Ptr to)
{
ValNodePtr    vnp2;
MsaTxtDispPtr mtdp;

	*from=INT4_MAX;
	*to=INT4_MIN;

	if (!pgp->ptxtList) return;
	
	for(vnp2=pgp->ptxtList;vnp2!=NULL;vnp2=vnp2->next){
		mtdp=(MsaTxtDispPtr)(vnp2->data.ptrvalue);
		if (mtdp){
			if (mtdp->IsGap==FALSE) {
				*from=MIN(*from,mtdp->from);
				*to=MAX(*to,mtdp->to);
			}
		}
	}
}

/*******************************************************************************

Function: UDV_GetStrandinPGP()

Purpose: get the strand of the sequence in a ParaG.

Parameters: pgp; the ParaG to analyse
            strand; return value

*******************************************************************************/
NLM_EXTERN void UDV_GetStrandinPGP(ParaGPtr pgp,Uint1Ptr strand)
{
ValNodePtr    vnp2;
MsaTxtDispPtr mtdp;

	if (!pgp->ptxtList) return;
	*strand=Seq_strand_unknown;
		
	for(vnp2=pgp->ptxtList;vnp2!=NULL;vnp2=vnp2->next){
		mtdp=(MsaTxtDispPtr)(vnp2->data.ptrvalue);
		if (mtdp && mtdp->IsGap==FALSE) {
			*strand=mtdp->strand;
			break;
		}
	}
}

/*******************************************************************************

Function: UDV_GetStrandinPGPList()

Purpose: get the strand of the sequence in a list of ParaG (usually called
by DDV to scan a row).

Parameters: pgp_list; list of ParaG (row) to analyse
            strand; return value

*******************************************************************************/
NLM_EXTERN void UDV_GetStrandinPGPList(ValNodePtr pgp_list,Uint1Ptr strand)
{
ValNodePtr    vnp,vnp2;
MsaTxtDispPtr mtdp;
ParaGPtr   pgp;

	if (!pgp_list) return;
	*strand=Seq_strand_unknown;
	
	vnp=pgp_list;
	while(vnp){
		pgp=(ParaGPtr)vnp->data.ptrvalue;
		if (pgp){
			vnp2=pgp->ptxtList;
			while(vnp2){
				mtdp=(MsaTxtDispPtr)(vnp2->data.ptrvalue);
				if (mtdp && mtdp->IsGap==FALSE) {
					*strand=mtdp->strand;
					return;
				}
				vnp2=vnp2->next;
			}
		}
		vnp=vnp->next;
	}
}

/*******************************************************************************

Function: UDV_GetBspRangeinPGPList()

Purpose: get the start,stop (bsp coord)  of the sequence in a list of ParaG 
(usually called by DDV to scan a row).

Note : bsp_start is always less than bsp_stop (so, don't forget to swap 
the values if minus strand)

*******************************************************************************/
NLM_EXTERN void UDV_GetBspRangeinPGPList(ValNodePtr pgp_list,
	Int4Ptr bsp_start,Int4Ptr bsp_stop)
{
ValNodePtr vnp;
ParaGPtr   pgp;
Int4       from_bsp,to_bsp;
	
	*bsp_start=INT4_MAX;
	*bsp_stop=INT4_MIN;
	vnp=pgp_list;
	while(vnp){
		pgp=(ParaGPtr)vnp->data.ptrvalue;
		if (pgp){
			UDV_ComputeBspCoordRangeinPGP(pgp,&from_bsp,&to_bsp);
			/*skip full-gapped ParaG*/
			if (from_bsp!=INT4_MAX && to_bsp!=INT4_MIN){
				*bsp_start=MIN(*bsp_start,from_bsp);
				*bsp_stop=MAX(*bsp_stop,to_bsp);
			}
		}
		vnp=vnp->next;
	}
}

/*******************************************************************************

Function: UDV_IsTranslationNeeded()

Purpose: for a CDS in a ParaG, determine whether a translation is needed or not

Parameters: context ; feature data
			pgp ; ParaG structure

Return : TRUE is a translation is needed

*******************************************************************************/
NLM_EXTERN Boolean UDV_IsTranslationNeeded(SeqMgrFeatContextPtr context,ParaGPtr pgp)
{
Int2 i,numivals2,i_decal;	/*counters*/
Uint1 strand;
Int4 StartLetter,StopLetter;

	strand=context->strand;
	
	/*temporary situation; will be modified in the future*/
	if (strand>Seq_strand_minus ||
		strand==Seq_strand_unknown) strand=Seq_strand_plus;

	/*strand PLUS*/
	if (strand==Seq_strand_plus){
		numivals2=context->numivals*2;
		i=0;
		i_decal=2;
	}

	/*strand MINUS*/
	if (strand==Seq_strand_minus){
		numivals2=2*context->numivals-2;
		i=numivals2;
		i_decal=-2;
	}		
	
	/*are the ParaG from a MSA ?*/
	if (pgp->ptxtList){
		UDV_ComputeBspCoordRangeinPGP(pgp,&StartLetter, &StopLetter);
	}
	else{
		StartLetter=pgp->StartLetter;
		StopLetter=pgp->StopLetter;
	}
	
	while (TRUE){
		/*if ivals.stop<= start ParaG : not yet in the current ParaG*/
		if (context->ivals[i+1]<StartLetter) {
			if (strand==Seq_strand_plus){
				if (numivals2>2 && i+2<numivals2){
					/*stop ParaG < start next ivals -> inter-region: fill 
					the ParaG with a thin line; this is the case
					for coding region: draw thin line to delineate the introns*/		
					if (context->ivals[i+2]>StopLetter){
						return(FALSE);
					}
				}
			}
			if (strand==Seq_strand_minus){
				if (numivals2>2 && i-2>-1){
					/*stop ParaG < start next ivals -> inter-region: fill 
					the ParaG with a thin line; this is the case
					for coding region: draw thin line to delineate the introns*/		
					if (context->ivals[i-2]>StopLetter){
						return(FALSE);
					}
				}
			}
		}

		if (strand==Seq_strand_plus){
			i=i+i_decal;
			if (i>numivals2-2) break;
		}
		if (strand==Seq_strand_minus){
			i=i+i_decal;
			if (i<0) break;
		}
	}		
	return(TRUE);
}

/*******************************************************************************

Function: UDV_ParaGFTableFeatures()

Purpose: populate several ParaG with one feature at a time

Parameters: see explore.h

*******************************************************************************/
NLM_EXTERN Boolean LIBCALLBACK UDV_ParaGFTableFeatures (SeqFeatPtr sfp, 
			SeqMgrFeatContextPtr context2)

{
ParaGFeaturesInLocPtr pgfl;
ValNodePtr vnp,vnp2,vnp3,vnp4;
ParaGPtr pgp;
Int4 BeginLines=0,PosFeat,StartLetter,StopLetter;
Int2 nLines=0,i=1,j,nFeatLines,decal;
Boolean FeatInParaG=FALSE;
Boolean bFirst=TRUE,bTrouve=FALSE;
Boolean IsTransNeeded=FALSE;
SeqMgrFeatContextPtr context;
SeqAnnotPtr sap;

	if (!context2->sfp) return (TRUE);

/* for test only : print out the Feature list*/
/*{{
 Char  str [256];
 int i;

  if (FeatDefLabel (sfp, str, sizeof (str) - 1, OM_LABEL_BOTH)) {
    printf ( "   Feature item %d index %d (%d) (%d - %d) (%d) %s\n",
             (int) context2->itemID, (int) context2->index, 
                         (int) context2->numivals,(int) context2->left,
                         (int) context2->right,(int) context2->strand,str);
        if (context2->numivals>1){
                for (i=0;i<context2->numivals*2;i+=2){
                        printf ( "   Ivals: (%d - %d)\n", context2->ivals[i],
                                                context2->ivals[i+1]);
                }
        }
  }
}}
*/
	pgfl = (ParaGFeaturesInLocPtr) context2->userdata;	

	/*don't retrieve special annotations*/
	sap = context2->sap;
	if (sap && sap->desc!=NULL) return(TRUE);
	if (context2->seqfeattype==SEQFEAT_PUB) return(TRUE);
	if (context2->seqfeattype == SEQFEAT_BIOSRC) {
	  /*
	  if (context2->bsp != NULL) {
        MemSet ((Pointer) &vn, 0, sizeof (ValNode));
        vn.choice = SEQLOC_WHOLE;
        vn.data.ptrvalue = (Pointer) SeqIdFindBest (context2->bsp->id, 0);
        vn.next = NULL;
        if (SeqLocCompare (sfp->location, &vn) == SLC_A_EQ_B) return TRUE;
	  }
	  */
	  return TRUE; /* now decided to suppress all source features */
	}
	
	context=UDV_ConvertFeatContext(context2,pgfl->cumOffset,pgfl->bsp_part_length);
	if (!context) return(TRUE);

	vnp=pgfl->ParaG_head;
	pgp=(ParaGPtr)vnp->data.ptrvalue;

	BeginLines=pgp->StartLine;

	pgfl->nFeat++;

	/*HET feature correction for the ends*/
	if(context->featdeftype== FEATDEF_HET){
		context->right=context->ivals[2*context->numivals-1];
	}
	
	if(context->left<0){/*if circular DNA with feature overlapping the junction*/
		/*technically, just for the purpose of a display, a feature
		overlapping the ends of a circular DNA covers all the sequence; that is
		because I ALWAYS display from LEFT to RIGHT.*/
		context->left=0;
		context->right=context->bsp->length-1;
	}
	while(TRUE){		
		pgp=(ParaGPtr)vnp->data.ptrvalue;
		
		FeatInParaG=FALSE;
		if (pgp){
			/*are the ParaG from a MSA ?*/
			if (pgp->ptxtList){
				UDV_ComputeBspCoordRangeinPGP(pgp,&StartLetter, &StopLetter);
			}
			else{
				StartLetter=pgp->StartLetter;
				StopLetter=pgp->StopLetter;
			}
			
			if (StartLetter>context->right){
				pgfl->ParaG_last_head=vnp;/*uses to count last ParaG of the list
											where there is no feature*/
				break;
			}
			/*Feature in pgp ?*/
			/*if(StopLetter>=context->left && StopLetter<=context->right){
					FeatInParaG=TRUE;
			}
			else if(StartLetter>=context->left && 
				StartLetter<=context->right){
					FeatInParaG=TRUE;
			}
			else if(StartLetter<=context->left && 
				StopLetter>=context->right){
					FeatInParaG=TRUE;

			}*/
			if(StopLetter>=context->left && StartLetter<=context->right){
				FeatInParaG=TRUE;
			}
			if (FeatInParaG){
				nLines=1;decal=0;
				if (pgfl->DispType&DDV_DISP_VERT){/*UDV*/
					if (bFirst) {
						pgfl->ParaG_next_head=vnp;
						bFirst=FALSE;
					}
					if (context->sfp->data.choice==SEQFEAT_CDREGION) {
						IsTransNeeded=UDV_IsTranslationNeeded(context,pgp);
						if (IsTransNeeded) {
							nLines++;
							pgp->nTrans++;
							decal=1;
						}
					}
					if (pgfl->ShowFeatures) {
						if (context->left<=pgp->OccupyTo){/*new line of features*/
							pgp->nLines+=nLines;
							pgp->nFeatLines+=nLines;
						}
						else{/*same feature line, but CDS*/
							/*for the translation*/
							if (context->sfp->data.choice==SEQFEAT_CDREGION){
								pgp->nLines++;
								pgp->nFeatLines++;
							}
						}
						pgp->OccupyTo=context->right;
						/*this is mainly for wwwudv in order to display labels*/
						if ((pgfl->DispType&DDV_DISP_VERT) && 
							(pgfl->DispType&DDV_DISP_LABEL)){
							pgp->OccupyTo+=StringLen(context->label);
						}
					}
					PosFeat=pgp->nFeatLines;
				}
				else{/*DDV (MSA only)*/
					if (context->sfp->data.choice==SEQFEAT_CDREGION) {
						/*IsTransNeeded=UDV_IsTranslationNeeded(context,pgp);
						if (IsTransNeeded) {*/
							nLines++;
							pgp->nTrans++;
						/*}*/
					}
					decal=0;
					if (bFirst) {
				/*A*/	/*where to put the feature (line number); I compute this value
						when I populate the first ParaG with the current feature. To
						populate next ParaG with the same feature, see B./

						/PosFeat is used for every ParaG where the current feature
						is located. In that way, the feature always belongs to the
						same line from one ParaG to the next one*/
						PosFeat=0;/*always base 1 for a line number;*/
						j=0;
						while(j<pgp->nFeatLines){
							if (context->left>pgfl->OccupyTo[j]){
								if (context->sfp->data.choice==SEQFEAT_CDREGION){
									if (context->left>pgfl->OccupyTo[j+1]){
										PosFeat=j+1;break;
									}
								}
								else {
									PosFeat=j+1;break;
								}
							}
							j++;
						}
						if (PosFeat==0){/*if this case, means we need to create nLines
								new lines*/
							PosFeat=pgp->nFeatLines+1;
							pgp->nFeatLines+=nLines;
						}
						pgfl->OccupyTo[PosFeat-1]=context->right;
						if (context->sfp->data.choice==SEQFEAT_CDREGION)
							pgfl->OccupyTo[PosFeat]=context->right;
						pgfl->ParaG_next_head=vnp;
						nFeatLines=pgp->nFeatLines;
						bFirst=FALSE;
					}
					else{
				/*B*/	/*to pupulate other ParaG with the current feature, I check if 
						there is blank line before PosFeat line (use ONLY for a full
						vertical display)*/
/*						for(j=0;j<PosFeat;j++){
							if (pgfl->OccupyTo[j]<pgp->StartLetter){
								decal++;
							}
						}*/
						pgp->nFeatLines=nFeatLines/*-decal*/;
					}

					/*new number of lines in the ParaG*/
					if (pgfl->ShowFeatures)
						pgp->nLines=pgp->MinLine+pgp->nFeatLines;
					else
						pgp->nLines=pgp->MinLine;
				}

				/*total number of features in a ParaG*/
				pgp->nFeat++;
				
				/*(re)populate Features structure; if a previous structure already
				exists, use it; avoid a lot of MemNew/MemFree; see also the function
				which create/(re)populate ParaG structure (CreateParaGList)*/
				if (pgp->pFeatList==NULL) {
					pgp->pFeatList = ValNodeAddBigInt(&pgp->pFeatList,0,
							UDV_BigEncodeIdxFeat ((Uint2) context->itemID,
								(Uint2) context->index,(Uint2)PosFeat-decal,(Uint2)0));
					if (!pgp->pFeatList) return TRUE;
				}
				else {
					i=1;bTrouve=FALSE;
					for (vnp2=pgp->pFeatList ; vnp2!=NULL ; vnp2=vnp2->next){
						if (i==pgp->nFeat){
							bTrouve=TRUE;
							break;
						}
						i++;
						vnp3=vnp2;
					}
					/*mem alloc only if it's needed*/
					if (bTrouve) {
						vnp2->data.bigintvalue=UDV_BigEncodeIdxFeat(
								(Uint2) context->itemID,
								(Uint2) context->index,
								(Uint2)(PosFeat-decal),(Uint2)0);
					}
					else{
						vnp4=ValNodeAddBigInt(&vnp3,0,
							UDV_BigEncodeIdxFeat ((Uint2) context->itemID,
								(Uint2) context->index,
								(Uint2)PosFeat-decal,(Uint2)0));
						if (!vnp4)return TRUE;
					}
				}
			}

			pgp->StartLine=BeginLines;/*beginning of this ParaG*/

			BeginLines+=pgp->nLines;/*will be used for the next ParaG*/

			vnp=vnp->next;/*get the next ParaG*/

			if (vnp==NULL){
				pgfl->ParaG_next_head=NULL; /*end*/
				break;
			}
		}
	}

	pgfl->nTotLines_new=BeginLines;
	
	if (pgfl->ParaG_next_head)
		pgfl->ParaG_head=pgfl->ParaG_next_head;
	MemFree(context->ivals);
	MemFree(context);
	return TRUE;
}


/*******************************************************************************

Function: UDV_CreateOneFeatureIndex()

Purpose: Call Sequence Manager to create the Feature Index of a BioSeq.

Parameter: entityID_seq ; entityID of the BioSeq.

Return value: the index value to use with others Sequence Manager functions.
		(Ex.: SeqMgrExplore... functions)

*******************************************************************************/

NLM_EXTERN Uint2 UDV_CreateOneFeatureIndex(Uint2 entityID_seq, BioseqPtr bsp,
	Boolean bReverse)
{
Uint2 EntityID;

	EntityID=entityID_seq;
	/*Is a previous Index exists for this sequence*/
	if (SeqMgrFeaturesAreIndexed (entityID_seq) == 0) {
		/*create the index*/
		if (bsp)
			EntityID=SeqMgrIndexFeaturesEx (0, bsp, bReverse);
		else
			EntityID=SeqMgrIndexFeaturesEx (entityID_seq, NULL, bReverse);
	}
	
	if (EntityID == 0) {
		Message (MSG_ERROR, "SeqMgrIndexFeaturesEx failed.");
		EntityID=INDEX_CREATION_ERROR;
	}
	
	return(EntityID);
}

/*******************************************************************************

  Function : UDV_CreateParaGList()
  
  Purpose : Create and populate ParaG val_node_list without features
  
  Parameters : 	nCharByLine; # of char by line
  				bsp_length; length of the bsp
				ShowTop; show scale on top if TRUE
				ShowTick; show scale's ticks if TRUE
				ShowSequence; show sequence if TRUE
				ShowBlank; add blank line at the bottom of the ParaG
				nTotL; total number of single lines for the whole ParaG list
				ParaG_head; head of the ParaG list

  Note : use this function to create a simple representation of a sequence
  				without the features. If you want to show the features, first
				call this function, then UDV_CreateOneFeatureIndex() to create
				feature index, and finally 	PopulateParaGFeatures() to populate
				ParaG with the features.		

  Return value : a ValNodePtr which is the head of the ParaG list
			Indeed, in this function ,the value of ParaG_head (passed in here) is
			modified. This is done to speed up the population of ParaG.
			
			(see also nTotL)
			
*******************************************************************************/
NLM_EXTERN ValNodePtr UDV_CreateParaGList(Int2 nCharByLine,
			Int4 bsp_length,Int4 from,Int4 to,
			Boolean ShowTop,Boolean ShowTick,Boolean ShowSequence, 
			Boolean ShowBlank,Int4Ptr nTotL,ValNodePtr ParaG_head)
{
ParaGPtr 	pgp=NULL;				/*ParaG data*/
ValNodePtr 	local_head=NULL,		/*these are used to scan ParaG*/
			vnp1=NULL,
			vnp2=NULL;				
Boolean		isFailed=FALSE;			/*memory allocation failure*/
Boolean		bFirst=TRUE;			/*used to avoid non-neces. mem alloc*/
Int4		start=0,				/*letter start, zero based*/
			stop=0,					/*letter stop. zero based*/
			nTotLines=0;			/*Total lines in viewer*/
Int1		minLineByParaG=0;		/*min height of a paraG, nb. of lines*/			
Int4		nParaG=0;				/*ParaG counter*/
			
	/*Minimum ParaG height*/
	if (ShowTop) minLineByParaG++;
	if (ShowTick) minLineByParaG++;
	if (ShowSequence) minLineByParaG++;
	if (ShowBlank) minLineByParaG++;

	/*stop=MIN(nCharByLine,bsp_length);*/

	local_head=ParaG_head;
	vnp1=ParaG_head;
	start=from;
	/*to++;
	stop=MIN(nCharByLine,to);*/
	
	/*(re)populate ParaG structure; if a previous structure already
	exists, use it; avoid a lot of MemNew/MemFree*/
	while(!(start>to)){
		if (vnp1){
			pgp=(ParaGPtr)vnp1->data.ptrvalue;

			if (!pgp){
				isFailed=TRUE;
				break;
			}

			/*Features; note that pgp->FeatList is not deleted : see the
			function which populates this structure (ParaGFTableFeatures)*/
			pgp->nFeat=0;pgp->nTrans=0;
			if (bFirst) bFirst=FALSE;
			vnp2=vnp1;
			vnp1=vnp1->next;
		}
		else{
			/*allocate new ParaG*/
			pgp=(ParaGPtr)MemNew(sizeof(ParaG));
			if (!pgp){
				isFailed=TRUE;
				break;
			}
			MemSet(pgp,0,sizeof(ParaG));
			
			/*create a new node*/
			if (vnp2==NULL) vnp2=ValNodeAddPointer(NULL,0,pgp);
			else vnp2=ValNodeAddPointer(&vnp2,0,pgp);

			if (!vnp2){
				isFailed=TRUE;
				break;
			}
			
			if (bFirst){
				local_head=vnp2;
				bFirst=FALSE;
			}
		}

		/*Fill in pgp*/
		pgp->StartLine=nTotLines;
		pgp->nLines=minLineByParaG;
		pgp->StartLetter=start;
		pgp->NumOrder=nParaG+1;
		pgp->nFeatLines=0;
		
		/*modify values*/
		nTotLines+=minLineByParaG;
		start+=nCharByLine;
		stop=start-1;
		if (stop>to) stop=to;
		pgp->StopLetter=stop;

		pgp->OccupyTo=pgp->StopLetter;
		/*this element is used to keep track of the minimum number
		of lines in a ParaG; i.e. ruler+sequence, 1 to 3 lines. I need this
		value in the function UDV_ParaGFTableFeatures() to increase pgp->nLines
		during the population of the ParaG with the features*/
		pgp->MinLine=(Int2)minLineByParaG;

		nParaG++;
	}
	
	if (isFailed){
		UDV_FreeListParaG(&local_head);
		nTotLines=0;		
	}

	if (vnp1){
		ValNodeFree(vnp1);
		vnp2->next=NULL;
	}

	*nTotL=nTotLines;

	return(local_head);
}

/*******************************************************************************

  Function : UDV_PopulateParaGFeatures()
  
  Purpose : populate ParaG val_node_list with features
  
  Parameters : 	GrData; graphical data
  				bsp; BioseqPtr of the Bioseq
				ParaG_vnp; valnode list of the ParaG. Each node has a
					data.ptrvalue which is of type ParaGPtr.
				rcP; size and position of the UnD viewer's panel
				nTotL; total number of single lines for the whole ParaG list

  Note : this function, if called, MUST BE used after a call to	
  		UDV_CreateParaGList().	

  Return value : TRUE if features found; otherwise FALSE (see also nTotL)

*******************************************************************************/
NLM_EXTERN Boolean UDV_PopParaGFeaturesEx(BioseqPtr bsp,ValNodePtr ParaG_vnp,
			Boolean ShowFeatures,Int4Ptr nTotL,Uint4 DispType,Int2Ptr nFeatFound,
			BoolPtr FeatDefTable,Int4 from_bsp,Int4 to_bsp)
{
ParaGFeaturesInLoc pgfl;	/*used for the Explore features function*/
ValNodePtr vnp;				/*to scan ParaG list*/
ParaGPtr pgp;				/*to modify ParaG, when needed*/
Int4 n=0;					/*a little counter*/
Boolean nRet;
/*variables used to populate a bsp which is a segment of a "master" sequence (parent)*/
BioseqPtr parent;
SeqMgrSegmentContext context;
SeqLocPtr   slp;
Int4 from,to;
Int2 nFeat;

	if (ParaG_vnp == NULL) {*nTotL=0;return(FALSE);}

	/*prepare the Exploration of Features*/
	pgfl.ParaG_head=ParaG_vnp;
	pgfl.nTotLines_new=0;
	pgfl.ParaG_next_head=NULL;
	/*pgfl.LineH=GrData.udv_font.LineHeight;*/
	/*pgfl.rcP_top=rcP.top;*/
	pgfl.ParaG_last_head=NULL;
	pgfl.ShowFeatures=ShowFeatures;
	pgfl.DispType=DispType;
	
	MemSet(pgfl.OccupyTo,(Int4)-1,sizeof(pgfl.OccupyTo));

	/*the current bsp is just a segment ?*/
	parent=SeqMgrGetParentOfPart(bsp,&context);
	
	/*convert the from-to of the segment to the "master" bioseq
	coordinates system*/
	if (parent){
		from=context.cumOffset+from_bsp;
		/*to=from+context.to-context.from;*/
		to=context.cumOffset+to_bsp;
		slp=SeqLocIntNew(from, to, Seq_strand_plus, parent->id);
		pgfl.cumOffset=context.cumOffset;
		pgfl.bsp_part_length=bsp->length;
	}
	else{
		slp=SeqLocIntNew (from_bsp, to_bsp, Seq_strand_plus, bsp->id);;
		pgfl.cumOffset=0;
		pgfl.bsp_part_length=bsp->length;
	}

	/*Explore the features */
	nFeat=SeqMgrExploreFeatures ((parent!=NULL ? parent : bsp), (Pointer) &pgfl, 
				UDV_ParaGFTableFeatures, slp, NULL, FeatDefTable);

	if (nFeatFound){
		*nFeatFound=nFeat;
	}
	/*complete the list where no features have been found; actually, this
	code is used to locate (RecT) correctly the end of the ParaG list*/
	if (pgfl.ParaG_next_head && pgfl.ParaG_last_head){
		n=pgfl.nTotLines_new;
		for(vnp=pgfl.ParaG_last_head ; vnp != NULL ; vnp=vnp->next){
			if (vnp->data.ptrvalue){
				pgp=(ParaGPtr)vnp->data.ptrvalue;
				pgp->StartLine=n;
				n+=pgp->nLines;
			}		
		}
		pgfl.nTotLines_new=n;	
	}
	
	/*pgfl.nTotLines_new could be 0 if no features*/
	if (pgfl.nTotLines_new>0) {
		*nTotL=pgfl.nTotLines_new;
		nRet=TRUE;
	}
	else nRet=FALSE;

	MemFree(slp);
/*DEBUG purpose : check the feature content of each ParaG
{{
ValNodePtr vnp2;
Int4 i;
Uint2 eID,idx,line,not;

	vnp=ParaG_vnp;
	i=0;
	while(vnp){
		pgp=(ParaGPtr)vnp->data.ptrvalue;
		printf("ParaG [%6i]:[%7i..%7i]================\n",i,pgp->StartLetter+1,pgp->StopLetter+1);
		vnp2=pgp->pFeatList;
		while(vnp2){
			UDV_BigDecodeIdxFeat((Uint8)vnp2->data.bigintvalue, &eID, &idx, &line, &not);
			printf("   eID:%4i ; idx:%4i ; Line :%4i\n",eID,idx,line);
			vnp2=vnp2->next;
		}
		vnp=vnp->next;i++;
	}
}}*/

	return(nRet);
}
NLM_EXTERN Boolean UDV_PopulateParaGFeatures(BioseqPtr bsp,ValNodePtr ParaG_vnp,
			Boolean ShowFeatures,Int4Ptr nTotL,Uint4 DispType,Int2Ptr nFeatFound)
{
	return(UDV_PopParaGFeaturesEx(bsp,ParaG_vnp,ShowFeatures,nTotL,DispType,
		nFeatFound,NULL,0,bsp->length-1));
}

/*******************************************************************************

  Function : UDV_Read_SequenceEx()
  
  Purpose : read a sequence 
  
  Parameters : Warning, from must be < than to; always
  
  Return value : the sequence 

*******************************************************************************/
NLM_EXTERN CharPtr UDV_Read_SequenceEx (SeqIdPtr sip, Int4 from, Int4 to, 
		Boolean IsProt,Int2 len,Uint1 strand)
{
/*BioseqPtr        bsp;*/
SeqLocPtr        slp;
SeqPortPtr       spp;
CharPtr          str = NULL;
Uint1			residue;
Uint2			i=0;

	/*from always < than to*/
	slp = SeqLocIntNew (from, to, strand, sip);
	spp = SeqPortNewByLoc (slp, (Uint1)(IsProt==TRUE ? Seq_code_iupacaa 
			: Seq_code_iupacna));
	if (spp != NULL) {
		str = MemNew ((len+1) * sizeof(Char));
		if (!str) return(NULL);
		while ((residue = SeqPortGetResidue(spp)) != SEQPORT_EOF) {
			if (IS_residue(residue)) {
				str[i] = residue;
				i++;
			}
		}
		/*SeqPortRead(spp, (BytePtr)str, len);*/
		SeqPortFree (spp);
	}   

	SeqLocFree (slp);
	
	return str;
}

/*******************************************************************************

  Function : UDV_Read_Sequence()
  
  Purpose : read a sequence 
  
  Parameters : Warning, from must be < than to; always
  
  Return value : the sequence 

*******************************************************************************/
NLM_EXTERN CharPtr UDV_Read_Sequence (SeqIdPtr sip, Int4 from, Int4 to, 
		Boolean IsProt,Int2 len)
{
	return (UDV_Read_SequenceEx ( sip,  from,  to, 
		 IsProt, len,Seq_strand_plus));
}

/*******************************************************************************

  Function : UDV_RevertBioSeqCoord()
  
  Purpose : given a 5'->3' coord, get back the 3'->5' coord (nuc. seq only) 
  
  IMPORTANT NOTE : the return value is ONE-BASED
  
*******************************************************************************/
NLM_EXTERN Int4 UDV_RevertBioSeqCoord(Int4 bsp_coord,Int4 bsp_length)
{
	return(bsp_length-bsp_coord);
}

NLM_EXTERN MsaTxtDispPtr UDV_MsaTxtDispNew(MsaTxtDispPtr pTxtDisp) {
/*----------------------------------------------------------------------------
*  makes a copy of an MsaTxtDisp.
*  returns pointer to a new MsaTxtDisp.
*---------------------------------------------------------------------------*/
  MsaTxtDispPtr  pTxtDispCopy;

  pTxtDispCopy = MemNew(sizeof(MsaTxtDisp));
  ASSERT(pTxtDispCopy != NULL);
  MemCopy(pTxtDispCopy, pTxtDisp, sizeof(MsaTxtDisp));
  return(pTxtDispCopy);
}


NLM_EXTERN MsaTxtDispPtr UDV_MsaTxtDispFree(MsaTxtDispPtr pTxtDisp) {
/*******************************************************************************
*  frees memory used for MsaTxtDisp.
*  returns NULL for successful completion.
*******************************************************************************/
  ASSERT(pTxtDisp != NULL);
  VERIFY(MemFree(pTxtDisp) == NULL);
  return(NULL);
}

