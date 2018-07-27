/*   salfiles.h
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
* File Name:  salfiles.h
*
* Author:  Colombe Chappey
*
* Version Creation Date:   1/27/96
*
* $Revision: 6.16 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
*
*
* ==========================================================================
*/

#ifndef _SALFILES_
#define _SALFILES_

#include <saledit.h>
#include <salsa.h>
#include <vibrant.h>
#include <txalign.h>

#ifdef __cplusplus
extern "C" {
#endif

#define REGISTER_IDTOUID ObjMgrProcLoad(OMPROC_FILTER,"IDtoUID","IDtoUID",0,0,0,0,NULL,SeqIdToUid,PROC_PRIORITY_DEFAULT)

#define REGISTER_DataToSeqAlign ObjMgrProcLoad(OMPROC_FILTER,"DataToSeqAlign","DataToSeqAlign",0,0,0,0,NULL,DataToSeqAlign,PROC_PRIORITY_DEFAULT)

/****************
*** Format Sequence
******************/
extern SeqEntryPtr FastaRead (CharPtr path, Uint2 mol_type);
extern SeqEntryPtr ReadInterleaveAlign (CharPtr path, Uint1 mol_type);
extern SeqEntryPtr ReadContiguouseAlign (CharPtr path, Uint1 mol_type);
extern SeqEntryPtr ReadAnyAlignment (Boolean is_prot, CharPtr path);
extern SeqEntryPtr ReadLocalAlignment (Uint1 format, CharPtr path);
extern SeqAlignPtr ImportFromFile (EditAlignDataPtr adp);
extern void CCFetchFromNet (EditAlignDataPtr adp, WindoW editor_window);
extern void EditBioseqToFasta (BioseqPtr bsp, FILE *fout, Int4 from, Int4 to);
extern Int2 seqannot_write (SeqAnnotPtr sap, CharPtr path);
extern void seqalign_write (SeqAlignPtr salp, CharPtr path);
extern Boolean sesp_to_pept (SelEdStructPtr cds, SeqAlignPtr salp, ValNodePtr sqlocs, Boolean partial);
extern void CdRgnToProtProc (PaneL pnl,  EditAlignDataPtr adp);
extern void UntranslateFunc (PaneL pnl,  EditAlignDataPtr adp);
extern Boolean ShowFeatureFunc (EditAlignDataPtr adp);
extern Boolean HideFeatureFunc (EditAlignDataPtr adp);
extern Boolean ResetFeatureFunc (EditAlignDataPtr adp);
extern ValNodePtr update_featpept (EditAlignDataPtr adp, ValNodePtr feathead, RecT *rp, SelStructPtr ssp, Int4 changevalue, Uint2 itemsubtype);
extern void ShowFeatureProc (PaneL pnl, Boolean invalidate);
extern void SaveFeatProc (PaneL pnl);
extern void SaveAllFeatProc (PaneL pnl);
extern void MakeFeatProc (PaneL pnl, Uint2 itemsubtype, Uint1 strand);
extern void TranslateAllBioseq (PaneL pnl,  EditAlignDataPtr adp);

extern ValNodePtr CCReadAnythingLoop (CharPtr filename, SelEdStructPtr seq_info);
extern SeqEntryPtr AsnReadForSalsa (CharPtr path);
extern SeqEntryPtr seqentry_read (CharPtr path);

#ifdef __cplusplus
}
#endif

#endif
