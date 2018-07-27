/*   salstruc.h
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
* File Name:  salstruc.h
*
* Author:  Colombe Chappey
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.4 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: salstruc.h,v $
* Revision 6.4  1999/11/24 21:24:29  vakatov
* Fixed for the C++ and/or MSVC DLL compilation
*
* ==========================================================================
*/

#ifndef _SALSTRUC_
#define _SALSTRUC_

#include <salsa.h>
#include <txalign.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif


/**********************************************************************/
NLM_EXTERN SelStructPtr     BufferFree (SelStructPtr ssp);
NLM_EXTERN EditAlignDataPtr SetupDataBuffer (EditAlignDataPtr adp);
NLM_EXTERN EditAlignDataPtr SetupDataPanel (EditAlignDataPtr adp);

NLM_EXTERN CharPtr      next_notemptyline (ValNodePtr anp_list, ValNodePtr linebuff, Int2 numberalignline, Int2 *index, Int4 start, Int4 *drw_width, TextAlignBufPtr *tdp, AlignNodePtr *anp);

NLM_EXTERN SelEdStructPtr is_feature_to_buffer (ValNodePtr vnphead, Uint2 bspitemID, Uint2 entityID, Int4 from, Int4 drw_width, SeqAlignPtr salp, Uint2 seqedit, ValNodePtr sqloc_list);
NLM_EXTERN ByteStorePtr cds_to_pept (SeqLocPtr slp, Uint1 frame, Int2 gencode, Boolean include_stop);
NLM_EXTERN void         data_collect_arrange (EditAlignDataPtr adp, Boolean recollect);
 
NLM_EXTERN CharPtr      get_master (ValNodePtr linebuff, Uint2 entityID, Uint2 itemID, Uint2 itemtype);

NLM_EXTERN Boolean      read_buffer_fromalignnode (EditAlignDataPtr adp, ValNodePtr *linebuff, Int4 bufferstart, Int4 minbufferlength, Int2 *numberalignline);

/**********************************
***
*** BioseqTrimN 
***   truncates the nnn's at the extremities of a bioseq bsp
***   providing TopSeqEntry sep allows to modify the SeqAlign if any
***
***********************************/
NLM_EXTERN Boolean BioseqTrimN (BioseqPtr bsp, SeqEntryPtr sep);

/*******************************************************************
*** AddFeatFunc
******************************************************************/
NLM_EXTERN ValNodePtr AddFeatFunc (SelEdStructPtr feat, ValNodePtr *featlist, Uint2 itemsubtype);
/******************************************************************
*
***    CollectFeatureForEditor
***      slp: the target Seq-loc
***      anp: the AlignNode belong to the target Seq-loc
***      csop: the option for gathering the features
******************************************************************/
NLM_EXTERN ValNodePtr CollectFeatureForEditor (SeqLocPtr slp, ValNodePtr seqfeat, Uint2 seq_entityID, Uint2 bsp_itemID, Uint1 *featOrder, Boolean all_feat);

/**********************************************************************
***  Display alignment in several formats
*** 
**********************************************************************/
NLM_EXTERN void ShowAlignmentText (FILE *fout, EditAlignDataPtr adp, SelStructPtr ssp, Int2 leftmargin, Int4 printfrom, Int4 printto, Boolean html_option);

NLM_EXTERN void showfastagap_fromalign (SeqAlignPtr salp, Int4 line, FILE *f);


#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
