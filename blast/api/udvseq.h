/*   udvseq.h
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
* File Name:  udvseq.h
*
* Author:  Patrick Durand
*
* Version Creation Date:   5/3/99
*
* $Revision: 6.20 $
*
* File Description: 
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: udvseq.h,v $
* Revision 6.20  2000/07/08 20:43:55  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* Revision 6.19  2000/04/13 13:57:33  durand
* allowed udv to display reverse complement sequence
*
* Revision 6.18  2000/02/17 15:54:36  durand
* use ~ for an unaligned gap and - for an aligned gap
*
* Revision 6.17  2000/02/03 15:55:14  hurwitz
* added constructor and destructor for MsaTxtDisp
*
* Revision 6.16  2000/01/24 19:37:18  durand
* add field szEditSeq in ParaG
*
* Revision 6.15  1999/12/02 13:47:32  durand
* add new function for Entrez sequence viewer
*
* Revision 6.14  1999/11/03 13:46:15  durand
* add UDV_GetStrandinPGP, UDV_GetStrandinPGPList and UDV_GetBspRangeinPGPList for DDV
*
* Revision 6.13  1999/10/29 14:14:53  durand
* add text styles for DDV
*
* Revision 6.12  1999/10/20 13:13:54  durand
* add new fields in data structure for DDV
*
* Revision 6.11  1999/10/04 19:52:56  durand
* add new text MSA_TXT_STYLE_2
*
* Revision 6.9  1999/09/16 18:51:53  durand
* move MsaTxtDisp struct from pgppop.h to udvseq.h
*
* Revision 6.8  1999/09/07 19:39:18  durand
* don't display special features
*
* Revision 6.7  1999/07/30 20:08:28  durand
* updates for the new Entrez graphical viewer
*
* Revision 6.6  1999/06/08 21:21:16  durand
* update ParaG structure
*
* Revision 6.5  1999/06/08 17:02:54  durand
* add bspGeneticCode to bspinfo data structure
*
* Revision 6.4  1999/06/07 15:10:18  durand
* add LOG line to keep track of the history
*
*
*
* ==========================================================================
*/

#ifndef _UDVSEQ_
#define _UDVSEQ_

#include <explore.h>
#include <ncbi.h>
#include <objfdef.h>
#include <objloc.h>
#include <objseq.h>
#include <seqport.h>
#include <sequtil.h>

#ifdef __cplusplus
extern "C" {
#endif

/*******************************************************************************

  The major element of this module is: paragraph (or ParaG)

  This module is Vibrant free and can be used by non-vibrant software.
  
*******************************************************************************/

	/*****************************************************************************

	Structure of a ParaG
	
              10         20        30   Line 0
	      .    |     .    |    .    |   Line 1
	1 GFDEDJSHHF DHGCJFDHCGNJFGJDHFGJ   Line 3
        |===>......===>.....===>|       Line 4
		/\/\/\/\    >------<  ^         Line n
		
		Line 0: numerical scale
		Line 1: ticks (minor: . ; major: |)
		Line 3: left scale (if needed) + the sequence
		Line 4: features
		Line n: features
	*****************************************************************************/


/*******************************************************************************

	DEFINES

*******************************************************************************/

/*Error value; if Feature Index failed*/
#define INDEX_CREATION_ERROR FALSE

/*****************************************************************************
	text styles
*****************************************************************************/
#define MSA_TXT_STYLE_SEQ		    1	/*sequence type                 */
#define MSA_TXT_STYLE_GAP		    2	/*gap type                      */
#define MSA_TXT_STYLE_NONE		    3	/*nothing                       */
#define MSA_TXT_STYLE_1             4   /*style 1 = 5 "space" char.     */
#define MSA_TXT_STYLE_REG_ALIGN		5	/*aligned region                */
#define MSA_TXT_STYLE_REG_UNALIGN	6	/*unaligned region              */
#define MSA_TXT_STYLE_2             7   /*for a disc. ali, display unaligned seq.*/
#define MSA_TXT_STYLE_UAGAP		    8	/*gap type, unaligned regions   */

#define SPACER_TXT_BLANK            5  /*use with MSA_TXT_STYLE_1*/

/*text justification for UnAligned region; used only by DDV */
#define DISP_JUST_LEFT    ((Uint1)1)
#define DISP_JUST_RIGHT   ((Uint1)2)
#define DISP_JUST_CENTER  ((Uint1)3)
#define DISP_JUST_SPLIT   ((Uint1)4)

/*******************************************************************************

	STRUCTURES

*******************************************************************************/

/*****************************************************************************
	text to display in a MSA_ParaG; line descriptor
*****************************************************************************/
	typedef struct msatxtdisp {
		Int4       	from;		/*bioseq coord - zero-based          */
		Int4       	to;			/*or used to compute if gap size     */
		Int4		SegID;		/*identifiers of the Indexed SeqAlign*/
		Int4		BspID;
		Boolean		IsGap;		/*display a gap                      */
		Boolean     IsUnAligned;/*unaligned region                   */
		Uint1		TextStyle;	/*seqence/gap_text/gap/...           */
		Uint1		strand;		/*plus, minus, etc.*/
	} MsaTxtDisp, PNTR MsaTxtDispPtr;

typedef struct parag {/*Paragraph information*/
	Int4 NumOrder;
	/*ParaG graphical values*/
	Int4 StartLine;				/*this ParaG starts at this line*/
	Int4 nLines;				/*and contains nLines (scale+seq+feat)*/
	Int4 StartLetter;			/*first letter of the bioseq to show*/
	Int4 StopLetter;			/*last letter of the bioseq to show*/
	ValNodePtr pFeatList;		/*list of itemID,index (Feature Index
								values)*/
	ValNodePtr 	ptxtList;/*text to draw - MSA*/
	SeqIdPtr	sip;/*identification of the bioseq - MSA*/
	Char *szEditSeq; /*used by DDV only for editi*/
	Int4 OccupyTo;				/*used to populate features; vertical disp*/
	Int2 MinLine;				/*used to populate features;horz display*/
	Int2 nFeatLines;			/*number of lines with features*/
	Int2 nFeat;					/*number of features*/
	Int2 nTrans;				/*number of translation*/
	
	Uint1		ScaleStyle;/*scale type (top/left/...)-MSA*/
	} ParaG, PNTR ParaGPtr;

/*structure used to initialize ParaG with features*/
typedef struct paragfeaturesinloc{
	ValNodePtr  ParaG_head;
	ValNodePtr	ParaG_next_head;
	ValNodePtr	ParaG_last_head;
	Int4        OccupyTo[500];
	Int4		nTotLines_new;
	/*Int2		rcP_top;*/
	Int4		nFeat;
	Int2		LineH;
	Boolean 	ShowFeatures;
	Uint1		DispType;
	Int4        cumOffset;
	Int4        bsp_part_length;
	} ParaGFeaturesInLoc,PNTR ParaGFeaturesInLocPtr;

typedef struct bspinfo {
		/*general data*/
	Uint2			bsp_entityID;
	Uint2			bsp_itemID;
	Uint2			bsp_itemType;
	BioseqPtr 		bsp;  /* set to NULL if uninitialized */
	Char			bspName[41];
	Char			bspAccNum[21];
	Char			bspRepr[21];
	Char			bspMol[21];
	Boolean         bspMolNuc;
	Char			bspTopo[21];
	Char 			bspStrand[21];
	Char 			bspDataType[21];
	CharPtr			bspGeneticCode;/* set to NULL if uninitialized */
	Int4			bspLength;
    SeqPortPtr      spp;  /* set to NULL if uninitialized */
    SeqIdPtr        sip;  /* set to NULL if uninitialized */
    
        /*SAM specific data*/
    Int4            BspID;

		/*Sequence Buffer*/
	Int4 			StartBuf;	/*buffer start here in the sequence*/
	Int4 			StopBuf;	/*buffer stop here... (0 based values)*/
	Int2 			LengthBuf;	/*buffer size*/
	CharPtr 		SeqBuf;		/*buffer sequence*/
	ValNodePtr 		PgpStartBuf;/*Pgp where start buffer is located*/

    struct bspinfo  *next;
	}BspInfo, PNTR BspInfoPtr;

/*******************************************************************************

	FUNCTIONS DECLARATION
	(see .c module for a complete description)

*******************************************************************************/

NLM_EXTERN MsaTxtDispPtr UDV_MsaTxtDispNew(MsaTxtDispPtr pTxtDisp);
NLM_EXTERN MsaTxtDispPtr UDV_MsaTxtDispFree(MsaTxtDispPtr pTxtDisp);
/*BSP information*/
NLM_EXTERN void  UDV_ReadBspDataForViewer(BspInfoPtr bsp_i);
/*Feature management*/
NLM_EXTERN void UDV_DecodeIdxFeat (Uint4 index_g, Uint2Ptr val1,
		Uint2Ptr val2);
NLM_EXTERN void  UDV_BigDecodeIdxFeat (Uint8 index_g, Uint2Ptr val1, Uint2Ptr val2,
	Uint2Ptr val3, Uint2Ptr val4);
NLM_EXTERN Uint4  UDV_EncodeIdxFeat (Uint2 val1,Uint2 val2);
NLM_EXTERN Uint8 UDV_BigEncodeIdxFeat (Uint2 val1,Uint2 val2,Uint2 val3,Uint2 val4);
NLM_EXTERN SeqMgrFeatContextPtr UDV_ConvertFeatContext(
		SeqMgrFeatContextPtr context,Int4 cumOffset,Int4 bsp_part_length);
NLM_EXTERN Boolean UDV_IsTranslationNeeded(SeqMgrFeatContextPtr context,
		ParaGPtr pgp);
NLM_EXTERN Boolean LIBCALLBACK UDV_ParaGFTableFeatures (SeqFeatPtr sfp, 
		SeqMgrFeatContextPtr context);
NLM_EXTERN Uint2 UDV_CreateOneFeatureIndex(Uint2 entityID_seq, 
		BioseqPtr bsp,Boolean bReverse);
NLM_EXTERN void UDV_FreeListParaG(ValNodePtr PNTR vnp_head);
NLM_EXTERN ValNodePtr UDV_CreateParaGList(Int2 nCharByLine,
		Int4 bsp_length,Int4 from,Int4 to,
		Boolean ShowTop,Boolean ShowTick,Boolean ShowSequence, 
		Boolean ShowBlank,Int4Ptr nTotL,ValNodePtr ParaG_head);
NLM_EXTERN Boolean UDV_PopulateParaGFeatures(BioseqPtr bsp,
		ValNodePtr ParaG_vnp,Boolean ShowFeatures,Int4Ptr nTotL,
		Uint4 DispType,Int2Ptr nFeatFound);
NLM_EXTERN Boolean UDV_PopParaGFeaturesEx(BioseqPtr bsp,ValNodePtr ParaG_vnp,
			Boolean ShowFeatures,Int4Ptr nTotL,Uint4 DispType,Int2Ptr nFeatFound,
			BoolPtr FeatDefTable,Int4 from_bsp,Int4 to_bsp);
/*Sequence reader*/
NLM_EXTERN CharPtr UDV_Read_SequenceEx (SeqIdPtr sip, Int4 from, Int4 to, 
		Boolean IsProt,Int2 len,Uint1 strand);
NLM_EXTERN CharPtr UDV_Read_Sequence (SeqIdPtr sip, Int4 from, Int4 to, 
		Boolean IsProt,Int2 len);
NLM_EXTERN void UDV_ComputeBspCoordRangeinPGP(ParaGPtr pgp,Int4Ptr from, 
		Int4Ptr to);
NLM_EXTERN void UDV_GetStrandinPGP(ParaGPtr pgp,Uint1Ptr strand);
NLM_EXTERN void UDV_GetStrandinPGPList(ValNodePtr pgp_list,Uint1Ptr strand);
NLM_EXTERN void UDV_GetBspRangeinPGPList(ValNodePtr pgp_list,
	Int4Ptr bsp_start,Int4Ptr bsp_stop);
NLM_EXTERN Int4 UDV_RevertBioSeqCoord(Int4 bsp_coord,Int4 bsp_length);



#ifdef __cplusplus
}
#endif

#endif /* ndef _UDVSEQ_ */

