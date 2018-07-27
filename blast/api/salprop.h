/*   salprop.h
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
* File Name:  salprop.h
*
* Author:  Colombe Chappey
*
* Version Creation Date:   8/20/99
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

#ifndef _SALPROP_
#define _SALPROP_

#include <salsa.h>

#define first_GBFeat  15
#define number_GBFeat 75
static CharPtr GBFeat[number_GBFeat] = {
"allele", "attenuator", "C_region", "CAAT_signal", "CDS", 
"conflict", "D-loop", "D_segment", "enhancer",  "exon",  
"GC_signal", "gene",  "intron",  "J_segment",  "LTR",  
"mat_peptide", "misc_binding",   "misc_difference",  
"misc_feature", "misc_recomb",  "misc_RNA",   "misc_signal",   
"misc_structure", "modified_base",  "mutation", "N_region", 
"old_sequence", "polyA_signal",  "polyA_site", "precursor_RNA",   
"prim_transcript", "primer_bind",  "promoter",   "protein_bind",  "RBS",  
"repeat_region", "repeat_unit",  "rep_origin",  "S_region",  "satellite",  
"sig_peptide", "source",  "stem_loop",  "STS",   "TATA_signal", 
"terminator", "transit_peptide",  "unsure",   "V_region",   "V_segment",   
"variation", "virion",   "3'clip",   "3'UTR",   "5'clip",  "5'UTR",  
"-10_signal", "-35_signal", "site_ref","REGION", "COMMENT","BOND","SITE",
"RSITE","USER","TXINIT","NUM","PSEC_STR","NON_STD_RESIDUE","HET",
"BIOSRC","preprotein","mat_peptide_aa","sig_peptide_aa",
"transit_peptide_aa" };

NLM_EXTERN void PropagateFeatureBySeqLock (SeqAnnotPtr sap, Uint2 source_bspitemID, Uint2 target_entityID, SeqEntryPtr target_sep, ValNodePtr seqfeat, Uint1 gap_choice);

NLM_EXTERN void PropagateFeatureByApply (PropaStructPtr psp);

NLM_EXTERN PropaStructPtr CreatePropaStruc (SeqIdPtr target_id, 
                              SeqIdPtr source_id, SeqAlignPtr salp);

NLM_EXTERN Boolean MergeFunc (SeqIdPtr target_id, SeqIdPtr source_id, SeqIdPtr merge_id,
                              SeqAlignPtr salp, Int4 fromseq2, Int4 toseq2, 
                              ValNodePtr sqlocs, Boolean spliteditmode, Boolean keep_protID);

NLM_EXTERN Boolean FeatListToProp (EditAlignDataPtr adp, ValNodePtr vnpfeat);

#endif
