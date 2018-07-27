/*   asn2ff3.c
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
* File Name:  asn2ff3.c
*
* Author:  Karl Sirotkin, Tom Madden, Tatiana Tatusov
*
* Version Creation Date:   7/15/95
*
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: asn2ff3.c,v $
* Revision 6.104  2001/11/29 18:29:38  kans
* added FANTOM_DB to list of legal db_xrefs, incremented DBNUM
*
* Revision 6.103  2001/11/12 19:32:38  kans
* updated mRNAEvidenceComment
*
* Revision 6.102  2001/10/25 12:45:45  kans
* Get3LetterSymbol was using table->num instead of table_3aa->num
*
* Revision 6.101  2001/10/15 17:08:44  kans
* updated legal db_xref list to collaboration + RefSeq
*
* Revision 6.100  2001/10/15 13:57:22  kans
* added BDGP_INS and SoyBase as legal db_xrefs
*
* Revision 6.99  2001/10/02 17:39:50  yaschenk
* Removing memory leaks
*
* Revision 6.98  2001/09/06 20:31:24  yaschenk
* removing memory leak - seqid returned by GetSeqIdForGI() needs to be freed
*
* Revision 6.97  2001/09/05 23:37:42  tatiana
* ribosomal slippage added to /note
*
* Revision 6.96  2001/09/05 23:32:39  tatiana
* supressed comparison of note to gene->synonym
*
* Revision 6.95  2001/08/22 22:35:07  kans
* added ProductIsLocal for /translation
*
* Revision 6.94  2001/08/07 16:49:41  kans
* use NUM_SEQID, added third party annotation SeqIDs to one more place
*
* Revision 6.93  2001/08/03 20:36:16  kans
* implemented ASN2GNBK_PRINT_UNKNOWN_ORG test to suppress unwanted mode diffs for asn2gnbk QA
*
* Revision 6.92  2001/07/12 17:12:49  kans
* biop->genome range checks in AddBioSourceToGBQual to prevent crashes
*
* Revision 6.91  2001/07/08 21:18:50  kans
* if ssp->subtype is 0, use ? as tag in note
*
* Revision 6.90  2001/06/26 19:50:07  kans
* call AddPID with is_NC as an option for showing /protein_id with the gi
*
* Revision 6.89  2001/06/25 22:22:17  kans
* ProteinFromCdRegion and GetProductFromCDS only if sfp->product and ! ajp->genome_view, should eliminate unwanted fetches to get far delta components
*
* Revision 6.88  2001/05/31 17:42:18  kans
* NC and NG RefSeq records allow remote fetching for /protein_id and /transcript_id, show gi if fetching not enabled
*
* Revision 6.87  2001/03/17 00:51:30  tatiana
* GeneID added to dbxref array
*
* Revision 6.86  2001/02/13 23:31:58  kans
* allow trans splicing exception, do not change sfp_in->excpt
*
* Revision 6.85  2001/01/30 16:25:54  kans
* precursor_RNA now has /product as legal qualifier
*
* Revision 6.84  2001/01/26 19:26:36  kans
* added niaEST, increased DBNUM
*
* Revision 6.83  2001/01/26 19:21:45  kans
* extrachromosomal into source note, removed macronuclear, extrachrom, plasmid from organism line
*
* Revision 6.82  2001/01/18 23:57:01  kans
* add GO (gene ontology) to list of legal dbxrefs
*
* Revision 6.81  2001/01/02 19:56:48  kans
* Get3LetterSymbol protects against empty string
*
* Revision 6.80  2000/12/07 19:03:53  tatiana
* transcript_id shows for NT only
*
* Revision 6.79  2000/12/06 22:00:46  tatiana
* ifdef removed
*
* Revision 6.78  2000/12/06 20:56:24  tatiana
* AceView link added
*
* Revision 6.76  2000/12/04 22:23:47  tatiana
* contig comments added
*
* Revision 6.75  2000/11/22 16:48:18  tatiana
* remove debugging error printing
*
* Revision 6.74  2000/11/10 00:37:13  tatiana
* changes in AddPID
*
* Revision 6.73  2000/10/25 15:57:57  kans
* sfp_in->excpt set to FALSE, not NULL, UNIX compiler does not know the difference, but Mac and PC compilers do
*
* Revision 6.72  2000/10/24 20:35:35  tatiana
* CDS without protein sequence is accepted for  not forgbrel mode
*
* Revision 6.70  2000/10/19 18:52:32  kans
* added another NULL entry to organelleQual for endogenous virus to suppress as organelle qualifier
*
* Revision 6.69  2000/10/16 19:10:17  kans
* added UniSTS and InterimID to legal dbxrefs
*
* Revision 6.68  2000/10/10 15:06:02  kans
* added SUBSRC_endogenous_virus_name
*
* Revision 6.67  2000/08/28 22:17:18  kans
* added CDD to list of legal dbxrefs
*
* Revision 6.66  2000/07/14 20:24:26  kans
* added RGD as dbxref with web link
*
* Revision 6.65  2000/07/12 22:45:15  kans
* added ORGMOD_old_lineage
*
* Revision 6.64  2000/06/20 17:31:34  kans
* added authority through breed as orgmod.subtypes
*
* Revision 6.63  2000/06/15 16:45:40  kans
* added segment to biosource note print
*
* Revision 6.62  2000/06/05 17:52:11  tatiana
* increase size of feature arrays to Int4
*
* Revision 6.61  2000/05/15 15:52:50  bazhin
* Removed memory leak in "PrintSourceFeat()".
*
* Revision 6.60  2000/03/30 20:37:29  kans
* added tilde to newline code in PrintImpFeatEx (thanks to Sergei B)
*
* Revision 6.59  2000/03/01 19:09:53  tatiana
* for SYN records with multiple source features there is no subtraction
*
* Revision 6.58  2000/02/17 21:59:18  kans
* /organelle not under ajp->forgbrel for this release now
*
* Revision 6.57  2000/02/15 22:53:56  kans
* added dbSNP and RATMAP as legal dbxrefs, put /organelle under ajp->forgrel control
*
* Revision 6.56  2000/02/09 01:12:51  tatiana
* remove space in organelle qualifier
*
* Revision 6.55  2000/01/21 20:48:45  kans
* changes to merge several source qualifiers under new organelle qualifier
*
* Revision 6.54  2000/01/11 22:49:37  tatiana
* protein accession is not required in DUMP_MODE
*
* Revision 6.53  2000/01/03 23:16:17  kans
* CDS note components from GetProtRefComment are separated by semicolons - to be consistent with upcoming asn2gnbk style
*
* Revision 6.52  1999/10/18 20:13:34  kans
* removed erroneous cast in sprintf
*
* Revision 6.51  1999/10/06 22:18:29  kans
* calls ComposeCodonsRecognizedString
*
* Revision 6.50  1999/10/06 20:23:48  bazhin
* Removed memory leaks.
*
* Revision 6.49  1999/08/03 20:48:23  tatiana
* UMR error fixed in PrintImpFeat
*
* Revision 6.47  1999/04/26 18:53:00  tatiana
*  added pseuod from sfp in ConvertToNAImpFeat()
*
* Revision 6.46  1999/04/06 22:37:45  tatiana
* protein_id hot link added
*
* Revision 6.45  1999/04/06 15:00:16  tatiana
* www_featkey is not called for slp view
*
* Revision 6.44  1999/03/30 22:23:33  kans
* pseudo can be on grp or sfp
*
* Revision 6.43  1999/03/30 19:18:19  tatiana
* changes for SEQID_OTHER
*
* Revision 6.42  1999/03/22 23:09:26  tatiana
* AddPID() changes
*
* Revision 6.41  1998/10/19 15:57:35  tatiana
* UniGene added to dbtag array
*
* Revision 6.40  1998/09/24 17:45:57  kans
* fixed GetDBXrefFromGene problem (TT)
*
* Revision 6.39  1998/09/01 19:25:21  kans
* context parameter in get best protein, get cds/rna given product
*
* Revision 6.38  1998/08/19 18:40:38  tatiana
* RiceGenes added to dbtag array
*
* Revision 6.37  1998/07/21 15:14:50  kans
* GetProtRefComments modified for indexes because continue statement avoided get next feature, got stuck
*
* Revision 6.36  1998/07/15 22:07:19  kans
* implemented sequence manager indexes for non-segmented nucleotides
*
* Revision 6.35  1998/07/13 14:52:24  tatiana
* subtypes added to source feature /note
*
* Revision 6.34  1998/06/15 14:57:22  tatiana
* UNIX compiler warnings  and extra HTML characters in notes fixed
*
* Revision 6.33  1998/05/20 20:05:40  tatiana
* SEQFEAT_REGION added to get_prot_feats()
*
* Revision 6.32  1998/05/18 14:41:53  tatiana
* GI added to dbtag array
*
* Revision 6.31  1998/05/08 21:56:56  tatiana
* added new PARTIAL_MODE
*
* Revision 6.30  1998/04/30 21:42:36  tatiana
* *** empty log message ***
*
* Revision 6.29  1998/04/27 18:31:51  tatiana
* added /focus in PrintSourceFeat()
*
* Revision 6.28  1998/04/24 15:10:08  tatiana
* GetProtRefComment() fixed: only main Prot-Ref feature adds comment to CDS
*
* Revision 6.27  1998/04/15 21:38:32  kans
* rearrange dbtag array so PID set is at start, allow unknown database on all but release_mode (Tatiana)
*
* Revision 6.24  1998/04/06 14:59:08  tatiana
* PutTranslationLast has been moved
*
* Revision 6.23  1998/04/03 22:38:36  tatiana
* selenocysteine added tp /note in ComposeCodeBreakQuals()
*
* Revision 6.22  1998/04/02 21:42:53  tatiana
* ignore old_name in OrgMod
*
* Revision 6.21  1998/04/02 17:21:23  tatiana
* a bug fixed in AddBioSourceToGBQual()
*
* Revision 6.20  1998/03/30 20:38:56  tatiana
* nat_host changed to specific_host
*
* Revision 6.19  1998/03/27 23:01:54  tatiana
* AddBioSourceToGBQual: added all OrgMod.subtypes as /notes on the source feature
*
* Revision 6.18  1998/03/24 19:47:45  tatiana
* added check for sfp->except_text
*
* Revision 6.17  1998/03/04 18:38:48  tatiana
*  illegal feature will be dropped in ConvertToAAImpFeat
*
* Revision 6.16  1998/02/19 21:28:52  tatiana
* dbtags array updated
*
* Revision 6.15  1998/01/26 21:16:16  tatiana
* biovar and country added to source feature /note
*
* Revision 6.14  1998/01/20 22:45:11  tatiana
* show both product and descr in Genpept
*
* Revision 6.13  1998/01/13 16:27:38  tatiana
* fixed a bug in dbtag check in PrintSourceFeat
*
* Revision 6.12  1997/12/23 21:57:16  tatiana
* focus and specimen_voucher
*
* Revision 6.11  1997/12/15 15:48:33  tatiana
* features processing has been changed
*
* Revision 6.10  1997/12/02 18:15:02  tatiana
* fix use of printf
*
* Revision 6.9  1997/10/23 16:57:42  tatiana
* *** empty log message ***
*
* Revision 6.6  1997/09/16 15:48:07  kans
* removed automatically generated diff lines
*
* Revision 6.5  1997/09/16 15:42:52  kans
* show non-gbff source qualifiers in note with labels (TT)
*
* Revision 6.4  1997/09/12 20:20:18  tatiana
* fixed typo
*
* Revision 6.3  1997/09/12 20:03:53  tatiana
* added source feature in genome_view
*
* Revision 6.2  1997/09/04 01:16:48  kans
* fixed typo
*
* Revision 6.1  1997/09/03 21:49:37  tatiana
* GatherItemWithLock() added for ProtRef features
*
* Revision 6.0  1997/08/25 18:04:51  madden
* Revision changed to 6.0
*
* Revision 5.59  1997/08/21 19:03:17  tatiana
* map, syn, description eliminated from features other than gene
*
* Revision 5.58  1997/08/05 20:09:08  kans
* added check for po->sfp null in PrintSourceFeat
*
* Revision 5.57  1997/07/29 14:55:51  kans
* make sure features on protein are SEQFEAT_PROT
*
* Revision 5.56  1997/07/16 21:08:28  tatiana
* Use gene synonym for /gene qualifier
*
* Revision 5.55  1997/06/19 18:37:02  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.54  1997/06/12 16:56:37  kans
* fixed typo that resulted in lost note (TT)
*
* Revision 5.53  1997/06/10 15:27:12  tatiana
* fix a typo in COnvertToNa... that leaded to the lost /note
*
 * Revision 5.47  1997/03/14  21:21:33  tatiana
 * exp_evidence fix
 *
 * Revision 5.46  1997/03/05  22:12:33  tatiana
 * print 'pseudo' in /note for orphan genes
 *
 * Revision 5.45  1997/03/04  23:45:14  tatiana
 * check for 'pseudo' gene added in ConvertToNAImpFeat()
 *
 * Revision 5.44  1997/02/25  23:47:21  tatiana
 * new error message added for dropped feature
 *
 * Revision 5.42  1997/01/29  15:49:11  tatiana
 * fix the entityID in GatherProductGeneInfo()
 *
 * Revision 5.40  1997/01/15  17:23:38  tatiana
 * a bug fixed (purify reported) in PrintNAFeatByNumber()
 *
 * Revision 5.39  1997/01/07  23:27:13  tatiana
 * check for NULLs added in CompareTranslation
 *
 * Revision 5.38  1997/01/07  22:32:41  tatiana
 * added SEQFEAT_SITE to get_prot_feats callback
 *
 * Revision 5.37  1997/01/02  22:49:55  tatiana
 * gather SEQFEAT_BOND
 *
 * Revision 5.36  1996/12/10  17:45:41  tatiana
 * a bug fixed in ComposeNoteFromNoteStruct()
 *
 * Revision 5.35  1996/12/09  19:12:33  tatiana
 * SPTREMBL added to legal db_xref database names
 *
 * Revision 5.34  1996/12/04  16:52:16  tatiana
 * a typo fixed in Add_dbxref
 *
 * Revision 5.33  1996/12/03  15:49:57  tatiana
 * 'CK' added to array of legal databases in db_xref
 *
 * Revision 5.32  1996/10/30  16:52:36  tatiana
 * SeqIdFindBest added in PrintSourceFeat
 *
 * Revision 5.31  1996/10/25  22:11:19  tatiana
 * NoteCmp changed
 *
 * Revision 5.30  1996/10/24  20:40:12  tatiana
 * a bug fixed in AddDBXref()
 *
 * Revision 5.29  1996/10/18  21:37:22  tatiana
 * a bug fixed in NoteCmp
 *
 * Revision 5.28  1996/10/09  15:15:00  tatiana
 * Take the main protein ONLY (not sig_peptide mat_peptide)
 * to make CDS comments
 *
 * Revision 5.27  1996/09/25  18:05:45  tatiana
 * SEQFEAT_COMMENT becomes misc_feature
 *
 * Revision 5.26  1996/09/18  20:41:26  kans
 * changed uninitialized variable names to correct names, removed unused
 * variable
 *
 * Revision 5.25  1996/09/18  20:21:27  tatiana
 * NoteCmp added to ComposeNoteFromNoteStruct to check for identical notes
 *
 * Revision 5.24  1996/09/17  14:59:04  tatiana
 * virion and transl_except added
 *
 * Revision 5.23  1996/09/12  17:52:28  tatiana
 * a bug fixed in PrintSourceFeat
 *
 * Revision 5.22  1996/09/06  14:58:00  tatiana
 * clean sfp_out at the end of PrintSourceFeat and PrintNAFeatByNumber
 *
 * Revision 5.21  1996/09/04  13:40:17  tatiana
 * a  bug fixed in GetDotTRNA
 *
 * Revision 5.19  1996/09/03  19:51:30  tatiana
 * extra_loc added
 *
 * Revision 5.18  1996/08/16  20:32:23  tatiana
 * for ifp->key StringSave is used not StringCpy
 *
 * Revision 5.17  1996/08/12  16:36:40  tatiana
 * ErrPostEx changed to ErrPostStr
 *
 * Revision 5.16  1996/08/06  20:30:46  kans
 * SeqIdFindBest called to handle local IDs and genbank IDs coexisting
 *
 * Revision 5.15  1996/08/02  21:41:23  tatiana
 * turned off metho conceptual transl by author
 *
 * Revision 5.14  1996/07/30  17:28:07  kans
 * ParFlat_... arrays now external in header file
 *
 * Revision 5.13  1996/07/30  16:34:09  tatiana
 * minor change in PrintSourcefeat
 *
 * Revision 5.12  1996/07/29  19:46:14  tatiana
 * GBQual_names changed to use a structureGBQual_names changed to use a structure
 *
 * Revision 5.11  1996/07/23  22:33:40  tatiana
 * prot feats in genpept (piptides)
 *
 * Revision 5.10  1996/07/22  22:07:21  tatiana
 * a bug fixed in DoTRNAQual
 *
 * Revision 5.9  1996/07/15  18:07:10  tatiana
 * minor changes in PrintSourceFeat to show 'unknown' in debug mode
 *
 * Revision 5.8  1996/07/12  20:38:22  tatiana
 * concept_transl_a supressed
 *
 * Revision 5.7  1996/07/12  20:11:49  tatiana
 * DotRNAQuals() changed
 *
 * Revision 5.6  1996/07/11  14:58:27  tatiana
 * product in tRNA
 *
 * Revision 5.5  1996/07/09  16:31:34  tatiana
 * a bug fixed in PrintNAFeatByNumber
 *
 * Revision 5.4  1996/07/02  18:09:17  tatiana
 * don't print duplicated features (PrintNAFeatByNumber)
 *
 * Revision 5.3  1996/06/14  18:03:56  tatiana
 * GetNAFeatKey change
 *
 * Revision 5.2  1996/06/11  15:35:04  tatiana
 * make GetGeneticCode static and get_prot_feats non-static
 *
 * Revision 5.1  1996/05/31  18:01:24  tatiana
 * check for /pseudo in CdRegion added
 *
 * Revision 4.35  1996/05/21  21:02:03  tatiana
 * a bug fixed in location[] size in PrintSourceFeat()
 *
 * Revision 4.34  1996/05/16  20:58:09  tatiana
 * GetCdregionGeneXrefInfo changed to Boolean
 *
 * Revision 4.33  1996/04/25  14:55:33  kans
 * protect against biosource subsource subtype of 255 (other) or bad values
 *
 * Revision 4.32  1996/04/15  14:36:49  tatiana
 * memory leaks cleaning
 *
 * Revision 4.31  1996/04/08  21:53:56  tatiana
 * change in www_featloc
 *
 * Revision 4.30  1996/04/05  17:43:36  ostell
 * added quickie patch for overrun of buf[30] when called by
 * www_featloc()
 *
 * Revision 4.29  1996/03/25  15:20:19  tatiana
 * add html symbols
 *
 * Revision 4.28  1996/03/19  23:58:27  tatiana
 * print activity in CDS
 *
 * Revision 4.27  1996/03/12  21:36:32  tatiana
 * 'serotype' added to orgmod_subtype array
 *
 * Revision 4.26  1996/02/28  04:53:06  ostell
 * changes to support segmented master seeuquences
 *
 * Revision 4.25  1996/02/26  00:46:18  ostell
 * removed unused local variables and integer size mismatch fusses
 *
 * Revision 4.24  1996/02/18  21:16:48  tatiana
 * memory leaks cleaned up
 *
 * Revision 4.23  1996/02/16  16:22:32  tatiana
 * a bug fixed in ConvertToNAImpFeat
 *
 * Revision 4.22  1996/02/15  15:52:18  tatiana
 * Gather for temp loaded items and sortin features within entity addded
 *
 * Revision 4.21  1996/01/29  22:34:42  tatiana
 * mainly PID changes
 *
 * Revision 4.20  1995/12/20  22:38:02  tatiana
 * gene xrefs to db_xref
 *
 * Revision 4.19  1995/12/15  02:47:01  ostell
 * added protection so that GatherProductGeneInfo does not gather if protein
 * bioseq not already in memory
 *
 * Revision 4.18  1995/12/13  16:31:36  tatiana
 * anticodon added to new tRNA slot
 *
 * Revision 4.17  1995/12/04  23:01:16  tatiana
 * take starin from OrgRef.mod in PrintSourceFeat()
 *
 * Revision 4.16  1995/11/28  15:19:46  tatiana
 * GetPID fixed
 *
 * Revision 4.15  1995/11/22  18:59:42  tatiana
 * a bug fixed in orphan genes printing
 *
 * Revision 4.14  1995/11/17  21:49:19  tatiana
 * hot link to genetic code added
 *
 * Revision 4.13  1995/11/17  21:28:35  kans
 * asn2ff now uses gather (Tatiana)
 *
 * Revision 4.4  1995/08/18  22:18:31  tatiana
 * a bug fix
 *
 * Revision 4.1  1995/08/01  14:51:29  tatiana
 * change SeqIdPrint to SeqIdWrite
 *
 * Revision 1.65  1995/07/17  19:33:20  kans
 * parameters combined into Asn2ffJobPtr structure
 *
 * Revision 1.61  1995/06/19  21:40:02  kans
 * Tatiana's first major reorganization, moving printing, adding HTML
 *
 * Revision 1.60  1995/05/19  21:25:06  kans
 * no longer fetches CDS protein product causing Entrez disc swap
 *
 * Revision 1.59  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
**************************************/
#include <asn2ffp.h>
#include <a2ferrdf.h>
#include <a2ferr.h>
#include <utilpub.h>
#include <ffprint.h>
#include <parsegb.h>
#include <sequtil.h>
#include <edutil.h>
#include <gather.h>
#include <explore.h>
#include <sqnutils.h>

#define METHOD_concept_transl_a 6

NLM_EXTERN CharPtr mRNAEvidenceComment PROTO ((UserObjectPtr obj, Boolean add));
NLM_EXTERN Int2 ConvertToNAImpFeat PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, SeqFeatPtr sfp_in, SeqFeatPtr PNTR sfp_out, SortStructPtr p));
NLM_EXTERN Int2 ConvertToAAImpFeat PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, SeqFeatPtr sfp_in, SeqFeatPtr PNTR sfp_out, SortStructPtr p));
NLM_EXTERN Int2 ValidateAAImpFeat PROTO ((SeqFeatPtr sfp, Boolean use_product));
NLM_EXTERN Int2 ValidateNAImpFeat PROTO ((SeqFeatPtr sfp));
NLM_EXTERN void AddProteinQuals PROTO ((SeqFeatPtr sfp, SeqFeatPtr sfp_out, NoteStructPtr nsp));
static void GetGeneticCode PROTO ((CharPtr ptr, SeqFeatPtr sfp));
NLM_EXTERN void ComposeGBQuals PROTO((Asn2ffJobPtr ajp, SeqFeatPtr sfp_out, GBEntryPtr gbp, SortStructPtr p, Boolean note_pseudo));
NLM_EXTERN CharPtr ComposeNoteFromNoteStruct PROTO ((NoteStructPtr nsp, GeneStructPtr gsp));
NLM_EXTERN void AddPID PROTO ((Asn2ffJobPtr ajp, SeqFeatPtr sfp_out, Boolean is_NTorNG));
NLM_EXTERN void Add_trid PROTO ((Asn2ffJobPtr ajp, SeqFeatPtr sfp_out));
NLM_EXTERN Int2 MakeGBSelectNote PROTO ((CharPtr ptr, SeqFeatPtr sfp));
static void GetProtRefComment PROTO ((SeqFeatPtr sfp, BioseqPtr bsp, Asn2ffJobPtr ajp, OrganizeProtPtr opp, NoteStructPtr nsp, Uint1 method));
NLM_EXTERN Int2 MiscFeatOrphanGenes PROTO ((Asn2ffJobPtr ajp, GBEntryPtr gbp, SeqFeatPtr sfp, Int2 index));
Int2 CheckForQual PROTO ((GBQualPtr gbqual, CharPtr string_q, CharPtr string_v));
NLM_EXTERN GBQualPtr AddModifsToGBQual PROTO ((GBEntryPtr gbp, GBQualPtr gbqual));
NLM_EXTERN GBQualPtr AddOrgRefModToGBQual PROTO ((OrgRefPtr orp, GBQualPtr gbqual));
NLM_EXTERN Int2 CheckForEqualSign PROTO ((CharPtr qual));
NLM_EXTERN CharPtr GetProductFromCDS PROTO ((ValNodePtr product, ValNodePtr location, Int4 length));
NLM_EXTERN void PrepareSourceFeatQuals PROTO ((SeqFeatPtr sfp_in, SeqFeatPtr sfp_out, GBEntryPtr gbp, Boolean add_modif));
static Int2 CheckForExtraChars PROTO ((CharPtr note));
NLM_EXTERN GBQualPtr AddBioSourceToGBQual PROTO((Asn2ffJobPtr ajp, NoteStructPtr nsp, BioSourcePtr biosp, GBQualPtr gbqual, Boolean new_release));
NLM_EXTERN Boolean delete_qual PROTO((GBQualPtr PNTR qlist, CharPtr qual));

typedef struct {
	CharPtr name;
	Uint1   num;
} ORGMOD;

#define num_subtype 25
CharPtr subtype[num_subtype] = {
"chromosome", "map", "clone", "sub_clone", "haplotype", "genotype", "sex",
"cell_line", "cell_type", "tissue_type", "clone_lib", "dev_stage", 
"frequency", "germline", "rearranged", "lab_host", "pop_variant",
"tissue_lib", "plasmid", "transposon", "insertion_seq", "plastid", "country",
"segment", "endogenous_virus"};

#define num_genome 15
static CharPtr genome[num_genome] = {"unknown", "genomic", "chloroplast", "chromoplast", "kinetoplast", "mitochondrion", "plastid", "macronuclear",
"extrachrom", "plasmid", "transposon", "insertion_seq", "cyanelle", "proviral", "virion"};

/*______________________________________________________________________
**
**	This code is not currently used.
**	I do not remove this piece of code, just comment it out.
**	-- Dmitri Lukyanov
*/
#if 0

#define num_biomol 7
static CharPtr biomol[num_biomol] = {"genomic", "RNA", "mRNA", "rRNA",
"tRNA", "snRNA", "scRNA"};

#endif
/*______________________________________________________________________
*/

ORGMOD orgmod_subtype[34] = {
	{ "strain", 2 }, {"sub_strain", 3}, {"type", 4}, {"subtype", 5},
	{"variety", 6},	{"serotype",7}, {"serogroup",8}, {"serovar", 9}, 
	{"cultivar", 10}, {"pathovar", 11}, {"chemovar", 12}, {"biovar", 13},
	{"biotype", 14}, {"group", 15}, {"subgroup", 16}, {"isolate", 17},
	{"common", 18}, {"acronym", 19}, {"dosage", 20}, {"nat_host", 21},
	{"sub_species", 22}, {"specimen_voucher", 23}, {"authority", 24},
	{"forma", 25}, {"forma_specialis", 26}, {"ecotype", 27},
	{"synonym", 28}, {"anamorph", 29}, {"teleomorph", 30}, {"breed", 31},
	{"old_lineage", 253}, {"old_name", 254}, {"note", 255}, { NULL, 0 }
};

/*
CharPtr dbtag[DBNUM] = {
  "PIDe", "PIDd", "PIDg", "PID", "FLYBASE",
  "GDB", "MIM", "SGD", "SWISS-PROT", "CK",
  "SPTREMBL", "ATCC", "ATCC (inhost)", "ATCC (dna)", "taxon",
  "BDGP_EST", "dbEST", "dbSTS", "MGD", "PIR",
  "GI", "RiceGenes", "UniGene", "LocusID", "dbSNP",
  "RATMAP", "RGD", "CDD", "UniSTS", "InterimID", "COG", "GO", "niaEST",
  "GeneID", "BDGP_INS", "SoyBase",
  };
*/

CharPtr dbtag[DBNUM] = {
  "PIDe", "PIDd", "PIDg", "PID",
  "ATCC",
  "ATCC(in host)",
  "ATCC(dna)",
  "BDGP_EST",
  "BDGP_INS",
  "CDD",
  "CK",
  "COG",
  "dbEST",
  "dbSNP",
  "dbSTS",
  "ENSEMBL",
  "ESTLIB",
  "FANTOM_DB",
  "FLYBASE",
  "GDB",
  "GeneID",
  "GI",
  "GO",
  "InterimID",
  "LocusID",
  "MaizeDB",
  "MGD",
  "MGI",
  "MIM",
  "niaEST",
  "PIR",
  "PSEUDO",
  "RATMAP",
  "RiceGenes",
  "REMTREMBL",
  "RGD",
  "RZPD",
  "SGD",
  "SoyBase",
  "SPTREMBL",
  "SWISS-PROT",
  "taxon",
  "UniGene",
  "UniSTS",
  };


/*************************************************************************
*	sfp_out: synthetic SeqFeatPtr of type ImpFeat for use in printing.
*	This function puts the dbxref qualifier on every SeqFeatPtr.
*************************************************************************/
static void Add_dbxref (Asn2ffJobPtr ajp, SeqFeatPtr sfp_out, SeqFeatPtr sfp)
{
	Int4 id = -1;
	Int2 i;
	ValNodePtr vnp;
	DbtagPtr db = NULL;
	CharPtr val;

	if (sfp == NULL || sfp->dbxref == NULL) {
		return;
	}
	for (vnp=sfp->dbxref; vnp; vnp=vnp->next) {
		id = -1;
		db = vnp->data.ptrvalue;
		if (db && db->db) {
			for (i =0; i < DBNUM; i++) {
				if (StringCmp(db->db, dbtag[i]) == 0) {
					id = i;
					break;
				}
			}
			if (ajp->mode == RELEASE_MODE && id == -1) {
				continue;  /* drop unknown dbtag */
			}
		}
		if (sfp->data.choice == SEQFEAT_CDREGION) {
			/*
			if (sfp->product != NULL && id > 4) {
				continue;
			}
			*/
		} else {
			if (id == -1 && ajp->mode != RELEASE_MODE) {
			} else
			if (id < 4) {
				continue;  /* PID is illegal on non-CDS features */
			}
		}
		if (db == NULL) {
			return;
		}
		if (db->tag && db->tag->str) {
			val = MemNew(StringLen(db->db)+StringLen(db->tag->str)+2);
			sprintf(val, "%s:%s", db->db, db->tag->str);
		} else if (db->tag) {
			val = MemNew(StringLen(db->db)+16);
			if (StringNCmp(db->db, "PIDe", 4) == 0) {
					sprintf(val, "PID:e%ld", (long) db->tag->id);
			} else if (StringNCmp(db->db, "PIDd", 4) == 0) {
					sprintf(val, "PID:d%ld", (long) db->tag->id);
			} else if (StringNCmp(db->db, "PIDg", 4) == 0) {
					sprintf(val, "PID:g%ld", (long) db->tag->id);
			} else {
				sprintf(val, "%s:%ld", db->db, (long) db->tag->id);
			}
		}
		if (val[0] != '\0') {
			sfp_out->qual = AddGBQual(sfp_out->qual, "db_xref", val);
			MemFree(val);
		}
	}
	return;
}	/* Add_dbxref */

static Boolean CheckSeqIdChoice(SeqIdPtr sip)
{
	Uint1 ch;
	SeqIdPtr si;
	
	for (si = sip; si; si=si->next) {
		ch = si->choice;
		if (ch == SEQID_GI || ch == SEQID_GENBANK || ch == SEQID_EMBL || ch == SEQID_DDBJ ||
			ch == SEQID_TPG || ch == SEQID_TPE || ch == SEQID_TPD) {
			return TRUE;
		}
	}
	return FALSE;
}

static SeqIdPtr GetSeqIdChoice(SeqIdPtr sip)
{
	Uint1 ch;
	SeqIdPtr si;
	
	for (si = sip; si; si=si->next) {
		ch = si->choice;
		if (ch == SEQID_GI || ch == SEQID_GENBANK || ch == SEQID_EMBL || ch == SEQID_DDBJ ||
			ch == SEQID_OTHER || ch == SEQID_TPG || ch == SEQID_TPE || ch == SEQID_TPD) {
			return si;
		}
	}
	return NULL;
}

static Boolean CheckSeqIdAccVer(SeqIdPtr sip)
{
	Uint1 ch;
	SeqIdPtr si;
	TextSeqIdPtr tsip;
	
	for (si = sip; si; si=si->next) {
		ch = si->choice;
		if (ch == SEQID_GENBANK || ch == SEQID_EMBL || ch == SEQID_DDBJ || ch == SEQID_OTHER ||
			ch == SEQID_TPG || ch == SEQID_TPE || ch == SEQID_TPD) {
			tsip = si->data.ptrvalue;
			if (tsip->accession != NULL && tsip->version >= 1) {
				return TRUE;
			}
		}
	}
	return FALSE;
}

static void GetNonGeneQuals (Int2 mode, SeqFeatPtr sfp_in, SeqFeatPtr sfp_out, NoteStructPtr nsp)
{
	GBQualPtr gbqp;
	Boolean evidence_present;
	Int2 i;
	
	for (gbqp=sfp_in->qual; gbqp; gbqp=gbqp->next) {
		if (StringCmp(gbqp->qual, "gene") == 0) {
			;
		} else if (StringCmp(gbqp->qual, "product") == 0) {
			;
		} else if (StringCmp(gbqp->qual, "standard_name") == 0) {
			;
		} else if (StringCmp(gbqp->qual, "map") == 0) {
			;
		} else if (StringCmp(gbqp->qual, "EC_number") == 0) {
			;
		} else if (StringCmp(gbqp->qual, "anticodon") == 0) {
			;	/* This is done by DotRNAQuals */
		} else if (StringCmp(gbqp->qual, "note") == 0) {
			CpNoteToCharPtrStack(nsp, NULL, gbqp->val);
		} else if (StringCmp(gbqp->qual, "transl_table") == 0) {
			sfp_out->qual = 
				AddGBQual(sfp_out->qual, gbqp->qual, gbqp->val);
			/* This is captured by GetGeneticCode */
		} else if (StringCmp(gbqp->qual, "db_xref") == 0) {
			for (i =0; i < DBNUM; i++) {
				if (StringNCmp(gbqp->val, dbtag[i], StringLen(dbtag[i])) == 0) {
					break;
				}
			}
			if (mode == RELEASE_MODE && i == DBNUM) {
				continue;  /* drop unknown dbtag */
			}			
			sfp_out->qual = 
				AddGBQual(sfp_out->qual, gbqp->qual, gbqp->val);
		} else {
			sfp_out->qual = 
				AddGBQual(sfp_out->qual, gbqp->qual, gbqp->val);
		}
	}

	evidence_present = GBQualPresent("evidence", sfp_out->qual);
	if (sfp_out->exp_ev) {
		if (evidence_present == FALSE) {
			if (sfp_out->exp_ev == 1)
				sfp_out->qual = 
				AddGBQual(sfp_out->qual, "evidence", "experimental");
			if (sfp_out->exp_ev == 2)
				sfp_out->qual = 
				AddGBQual(sfp_out->qual, "evidence", "not_experimental");
		} else {
			for (gbqp=sfp_out->qual; gbqp; gbqp=gbqp->next)
				if (StringCmp(gbqp->qual, "evidence") == 0) {
					gbqp->val = MemFree(gbqp->val);
					if (sfp_out->exp_ev == 1)
						gbqp->val = StringSave("experimental");
					if (sfp_out->exp_ev == 2)
						gbqp->val = StringSave("not_experimental");
					break;
				}
		}
	} else if (evidence_present == TRUE) {
		for (gbqp=sfp_out->qual; gbqp; gbqp=gbqp->next)
			if (StringCmp(gbqp->qual, "evidence") == 0) {
				if (StringCmp(gbqp->val, "EXPERIMENTAL") == 0) {
					StringCpy(gbqp->val, "experimental");
				} else if (StringCmp(gbqp->val, "NOT_EXPERIMENTAL") == 0) {
					StringCpy(gbqp->val, "not_experimental");
				}
				break;
			}
	}
	return;
}	/* GetNonGeneQuals */

/*****************************************************************************
*LookForPartialImpFeat
*
*	This function first looks for the sfp->qual of type "partial".
*	If found the qual is deleted and the variable "partial" is
*	set equal to TRUE.  If "partial" is TRUE or if sfp->partial
*	is TRUE, FlatAnnotPartial is called (modified version of Karl Sirotkin's
*	program) to see if sfp->partial should really be TRUE.
*	WARNING: sfp should be an ImpFeatPtr
*
*	written by Tom Madden (12/7/93)
*****************************************************************************/
static void LookForPartialImpFeat(SeqFeatPtr sfp, Boolean use_product)

{
	Boolean partial=FALSE;
	GBQualPtr curq, gbqual, lastq=NULL, tmpqual;

	gbqual = sfp->qual;

	while (gbqual && (StringCmp(gbqual->qual, "partial")==0))
	{
		partial = TRUE;
		tmpqual = gbqual->next;
		gbqual->next = NULL;
		gbqual = GBQualFree(gbqual);
		gbqual = tmpqual;
	}

	if (gbqual)
	{
		for (lastq=gbqual, curq=gbqual->next; curq; curq=curq->next)
		{
			if (StringCmp(curq->qual, "partial") == 0)
			{
				partial = TRUE;
				lastq->next = curq->next;
				curq->next = NULL;
				curq = GBQualFree(curq);
				curq = lastq;
			}
			else
				lastq = curq;
		}
	}

	sfp->qual = gbqual;

	if (partial == TRUE || sfp->partial == TRUE)
		sfp->partial = FlatAnnotPartial(sfp, use_product);
}	/* LookForPartialImpFeat */

static CharPtr SeqCodeNameGet (SeqCodeTablePtr table, Uint1 residue, Boolean error_msgs)
{
	int index=residue - table -> start_at;
	static CharPtr oops = "?";

	if (index >= 0 && index < (int) table -> num){
		return (table -> names) [index];
	}else {
		if (error_msgs == TRUE) 
			ErrPostEx(SEV_WARNING, CTX_NCBI2GB, 1,
			"asn2ff: %c(%d) > max in SeqCode table=%d",
			(char) residue, (int) residue, (int) table -> num);
		return oops;
	}
}

/***************************************************************************
*CharPtr Get3LetterSymbol (Uint1 seq_code, SeqCodeTablePtr table, Uint1 residue, Boolean error_msgs)
*
*	if (ASN2FF_IUPACAA_ONLY == TRUE) then
*	Check if the residue is legal in iupacaa; if not, return 'X', if so,
*	return the three letter code from iupacaa3.
*
*	if (ASN2FF_IUPACAA_ONLY != TRUE) then
*	Then do a translation, if necessary, then get th three letter code
*	from iupacaa3.
*
***************************************************************************/

static CharPtr Get3LetterSymbol (Uint1 seq_code, SeqCodeTablePtr table, Uint1 residue, Boolean error_msgs)
{
	static CharPtr bad_symbol= "OTHER";
	CharPtr ptr, retval=NULL;
	Int2 index;
	SeqCodeTablePtr table_3aa;
	SeqMapTablePtr smtp;
	Uint1 code, new_residue;

	if (residue == 42) {  /* stop codon in NCBIeaa */
		retval = "TERM";
		return retval;
	}
	if (ASN2FF_IUPACAA_ONLY == TRUE)
		code = Seq_code_iupacaa;
	else
		code = Seq_code_ncbieaa;

	if (code != seq_code)
	{/* if code and seq_code are identical, then smtp is NULL?? */
		smtp = SeqMapTableFind(seq_code, code);
		new_residue = SeqMapTableConvert(smtp, residue);
	}
	else
		new_residue = residue;

/* The following looks for non-symbols (255) and "Undetermined" (88) */
	if ((int) new_residue == 255 || (int) new_residue == 88)
		retval = bad_symbol;
	else
	{
		ptr = SeqCodeNameGet(table, residue, error_msgs);
		
		table_3aa=SeqCodeTableFind (Seq_code_iupacaa3);
		if (ptr != NULL && *ptr != '\0' && table_3aa != NULL)
		{
			for (index=0; index < (int) table_3aa->num; index++)
			{
				if (StringCmp(ptr, (table_3aa->names) [index]) == 0)
				{
					retval = (table_3aa->symbols) [index];
					break;
				}
			}
		}
	}
	
	return retval;

}	/* Get3LetterSymbol */

static CharPtr GetNameFromOrgName(OrgNamePtr orgname)
{
	BinomialOrgNamePtr bi;
	CharPtr name = NULL, virus, newname;
	Int2 len=0;
	Boolean first;
	OrgNamePtr org;
	
	switch(orgname->choice)
	{
		case 1:			/*binomial*/
			bi = (BinomialOrgNamePtr) orgname->data;
			len = StringLen(bi->genus);
			if (bi->species) {
				len += StringLen(bi->species);
			}
			name = MemNew(len + 2);
			StringCpy(name, bi->genus);
			if (bi->species) {
				name = StringCat(name, " ");
				name = StringCat(name, bi->species);
			} else {
				name = StringCat(name, " sp.");
			}
		break;
		case 2:			/*virus*/
			virus = (CharPtr) orgname->data;
			name = MemNew(StringLen(virus));
			StringCpy(name, virus);
		break;
		case 3:			/*hybrid*/
			first = TRUE;
			for (org = (OrgNamePtr) orgname->data; org; org=org->next) {
				newname = GetNameFromOrgName(org);
				len += StringLen(newname) + 3;
			}
			name = MemNew(len + 1);
			for (org = (OrgNamePtr) orgname->data; org; org=org->next) {
				newname = GetNameFromOrgName(org);
				if (first == TRUE) {
					name = StringCat(name, newname);
					first = FALSE;
				} else {
					name = StringCat(name, " x ");
					name = StringCat(name, newname);
				}
			}			
		break;
		case 4:			/*namedhybrid*/
			bi = (BinomialOrgNamePtr) orgname->data;
			len = StringLen(bi->genus);
			if (bi->species) {
				len += StringLen(bi->species);
			}
			name = MemNew(len + 4);
			StringCpy(name, bi->genus);
			if (bi->species) {
				name = StringCat(name, " x ");
				name = StringCat(name, bi->species);
			}
		break;
		case 5:			/*partial*/
	/* not implemented yet */
			ErrPostStr(SEV_WARNING, 0, 0, "Partial name in OrgName.name");
		break;
		default:
		break;
	}
	return name;
}

NLM_EXTERN void PrintSourceFeat(Asn2ffJobPtr ajp, GBEntryPtr gbp)

{
	BioseqPtr bsp;
	Char location[40];
	ImpFeatPtr ifp;
	Int2  status = -1, /* mol = -1, -- UNUSED */ i, bsize=0;
	NoteStructPtr nsp = NULL;
	OrgRefPtr orp=NULL;
	SeqFeatPtr sfp_in, sfp_out=NULL, sfp;
	SeqIntPtr sip;
	SeqLocPtr slp, keep_loc;
	ValNodePtr vnp=NULL;
	BioSourcePtr biosp = NULL;
	OrgModPtr omp;
	SortStructPtr pss, ps=NULL, bs = NULL, po=NULL;
	DescrStructPtr ds;
	CharPtr name;
	
	if (gbp == NULL) {
		return;
	}
	if (gbp->feat) {
		nsp=gbp->feat->source_notes;
		po = gbp->feat->Orglist;
		ps = gbp->feat->Sourcelist;
		bs = gbp->feat->Biosrclist;
		bsize = gbp->feat->biosrcsize;
	}
	ds = gbp->source_info;
	bsp = gbp->bsp;
	if (ajp->slp) {
		return;
	}
	sprintf(location, "1..%ld", (long) (bsp->length));
    sfp_out = ajp->sfp_out;
	ifp = sfp_out->data.value.ptrvalue;
	ifp->key = StringSave("source");
	if (ajp->slp) {
		slp = AsnIoMemCopy(ajp->slp,
					(AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);
	} else {
		slp = (SeqLocPtr) ValNodeNew(NULL);
		slp->choice = SEQLOC_INT;
		sip = SeqIntNew();
		sip->from = 0;
		sip->to = (bsp->length)-1;
		sip->id = SeqIdDup(SeqIdFindBest (bsp->id, 0));
		slp->data.ptrvalue = sip;
	}
	sfp_out->location = slp;
	if (ds != NULL) {
		vnp = ds->vnp;
		keep_loc = AsnIoMemCopy(slp,
					(AsnReadFunc) SeqLocAsnRead, (AsnWriteFunc) SeqLocAsnWrite);					
		if (vnp && vnp->choice == Seq_descr_source) {
			biosp = vnp->data.ptrvalue;
			if (biosp->is_focus == TRUE) {
				sfp_out->qual = AddGBQual(sfp_out->qual, 
										"focus", NULL);
				if (StringNCmp(gbp->div, "SYN", 3) != 0) {
					for (pss=bs, i= 0; pss && i < bsize; i++, pss++) {
						if (pss->sfp == NULL)
							continue;
						sfp_out->location = 
							SeqLocSubtract(sfp_out->location,
												 pss->sfp->location);
					}
				}
			}
		}
		if (sfp_out->location == NULL) {
			sfp_out->location = keep_loc;
		}
		else
			SeqLocFree(keep_loc);
	}
	flat2asn_install_feature_user_string("source", ifp->loc);
	if (gbp->feat && gbp->feat->sfpSourcesize != 0) {
		if ((sfp_in = ps->sfp) == NULL) {
			GatherItemWithLock(ps->entityID, ps->itemID, ps->itemtype, 
									&sfp_in, find_item);
		}
		if (sfp_out->qual != NULL)
			sfp_out->qual = GBQualFree(sfp_out->qual);
		NoteStructReset(nsp);
		PrepareSourceFeatQuals(sfp_in, sfp_out, gbp, FALSE);
		Add_dbxref(ajp, sfp_out, sfp_in); 
		status = ValidateNAImpFeat(sfp_out);
		if (status < 0) { 
/* source feat is probably missing organism name, add
		and try again.  Don't delete old quals! */
			if (ds != NULL) {
				vnp = ds->vnp;
				if (vnp->choice == Seq_descr_source) {
					biosp = vnp->data.ptrvalue;
					orp = (OrgRefPtr) biosp->org;
				} else if (vnp->choice == Seq_descr_org) {
					orp = (OrgRefPtr) vnp->data.ptrvalue;
				}
			} else if (gbp->feat && gbp->feat->sfpOrgsize != 0) {
				if ((sfp = po->sfp) == NULL) {
					GatherItemWithLock(po->entityID, po->itemID, po->itemtype, 
									&sfp, find_item);
				}
				if (sfp != NULL) {
					orp = (OrgRefPtr) sfp->data.value.ptrvalue;
				}
			}
			if (orp) {
				if (ajp->orgname && orp->orgname) {
					name = GetNameFromOrgName(orp->orgname);
					sfp_out->qual = AddGBQual(sfp_out->qual, 
										"organism", name);
					MemFree(name);
				} else if (orp->taxname) {
					sfp_out->qual = AddGBQual(sfp_out->qual, 
										"organism", orp->taxname);
					if (orp->common && sfp_in->comment != NULL)
						CpNoteToCharPtrStack(nsp, NULL, orp->common);
				} else if (orp->common) {
					if (StrStr(orp->common, "virus") ||
					    StrStr(orp->common, "Virus") ||
					    StrStr(orp->common, "phage") ||
					    StrStr(orp->common, "Phage") ||
					    StrStr(orp->common, "viroid") ||
					    StrStr(orp->common, "Viroid")) {
						sfp_out->qual = AddGBQual(sfp_out->qual, 
											"organism", orp->common);
					}
				}
			}
			status = ValidateNAImpFeat(sfp_out);
		}
	}
	if (status < 0) {
		if (ds != NULL) {
			if ((vnp = ds->vnp) != NULL) {
				if (vnp->choice == Seq_descr_source) {
					biosp = vnp->data.ptrvalue;
					orp = (OrgRefPtr) biosp->org;
				} else if (vnp->choice == Seq_descr_org) {
					orp = (OrgRefPtr) vnp->data.ptrvalue;
				}
			}
		} else if (gbp->feat && gbp->feat->sfpOrgsize != 0 && po->sfp != NULL) {
				orp = (OrgRefPtr) (po->sfp)->data.value.ptrvalue;
		} else {
			orp = NULL;
		}
		if (orp) {
			if (nsp) {
				NoteStructReset(nsp);
			}
			if (sfp_out->qual != NULL)
				sfp_out->qual = GBQualFree(sfp_out->qual);
			if (ajp->orgname && orp->orgname) {
				name = GetNameFromOrgName(orp->orgname);
				sfp_out->qual = AddGBQual(sfp_out->qual, 
									"organism", name);
				MemFree(name);
			} else if (orp->taxname) {
				sfp_out->qual = AddGBQual(sfp_out->qual, 
					"organism", orp->taxname);
			} else if (orp->common) {
				if (StrStr(orp->common, "virus") ||
				    StrStr(orp->common, "Virus") ||
				    StrStr(orp->common, "phage") ||
				    StrStr(orp->common, "Phage") ||
				    StrStr(orp->common, "viroid") ||
				    StrStr(orp->common, "Viroid")) {
					sfp_out->qual = AddGBQual(sfp_out->qual, "organism",
															 orp->common);
				}
			}
			if (orp->orgname && orp->orgname->mod) {
				omp = orp->orgname->mod;
				if (omp->subtype == 0 && omp->subname != NULL) {
					CpNoteToCharPtrStack(nsp, NULL, omp->subname);
				}
			}
			sfp_out->qual = AddBioSourceToGBQual(ajp, nsp, biosp, sfp_out->qual, TRUE);
			sfp_out->qual = AddOrgRefModToGBQual(orp, sfp_out->qual);
		}
		if ((vnp=BioseqGetSeqDescr(gbp->bsp, Seq_descr_molinfo, NULL)) != NULL){
			/*
		 	mfp = vnp->data.ptrvalue;
			if (mfp) {
				mol = mfp->biomol;
			}
			-- NO EFFECT */
		}
		PrepareSourceFeatQuals(NULL, sfp_out, gbp, TRUE);
		status = ValidateNAImpFeat(sfp_out);
	}
/* ----------Organism not found -------------*/
	if (status < 0) {
		if (sfp_out->qual)
			sfp_out->qual = GBQualFree(sfp_out->qual);
		sfp_out->qual = AddGBQual(sfp_out->qual, "organism", "unknown");
		NoteStructReset(nsp);
		if (orp && orp->common)
			CpNoteToCharPtrStack(nsp, NULL, orp->common);
/*try new first */
		if (biosp) {
			sfp_out->qual = AddBioSourceToGBQual(ajp, nsp, biosp, sfp_out->qual, TRUE);
			if (orp)
				sfp_out->qual = AddOrgRefModToGBQual(orp, sfp_out->qual);
		}
/* try old  then */
		sfp_out->qual = AddOrgRefModToGBQual(orp, sfp_out->qual);
		PrepareSourceFeatQuals(NULL, sfp_out, gbp, TRUE);
		status = ValidateNAImpFeat(sfp_out);
	}	
	flat2asn_delete_feature_user_string();

	if (status >= 0 || ASN2FF_VALIDATE_FEATURES == FALSE) {
		PrintImpFeat(ajp, gbp->bsp, sfp_out);
	}
	sfp_out->comment = NULL;
	sfp_out->location = SeqLocFree(sfp_out->location);
	sfp_out->location = NULL;
	sfp_out->product = NULL;
	sfp_out->exp_ev = FALSE;
	sfp_out->partial = FALSE;
	sfp_out->excpt = FALSE;
	ifp = sfp_out->data.value.ptrvalue;
	if (ifp->key) {
		ifp->key = MemFree(ifp->key);
	}
	if (ifp->loc) {
		ifp->loc = MemFree(ifp->loc);
	}
	if (sfp_out->qual)
		sfp_out->qual = GBQualFree(sfp_out->qual);
	return;
}	/* PrintSourceFeat */

/*****************************************************************************
*
*	Add the quals of the form "/transl_except=(pos: ,aa: )" to the
*	SeqFeatPtr sfp_out.  
*	
*****************************************************************************/	

static void ComposeCodeBreakQuals (Asn2ffJobPtr ajp, BioseqPtr bsp, SeqFeatPtr sfp_in, SeqFeatPtr sfp_out, SeqLocPtr PNTR extra_loc, Int2 extra_loc_cnt, NoteStructPtr nsp)

{
	CdRegionPtr crp;
	CharPtr buffer, ptr, pos;
	Choice aa;
	CodeBreakPtr cbp;
	SeqCodeTablePtr table;
	SeqLocPtr slp;
	Uint1 seq_code=0, the_residue;
	Int2 i, buflen;

	if ((sfp_in == NULL) || (sfp_in->data.choice != 3)) {
		return;
	}

	crp = (CdRegionPtr) sfp_in->data.value.ptrvalue;

	if (crp->code_break != NULL) {
		cbp = crp->code_break;
		while (cbp != NULL) {
			aa = cbp->aa;
			switch (aa.choice) {
				case 1:
					seq_code = 8;
					break;
				case 2:
					seq_code = 7;
					break;
				case 3:
					seq_code = 11;
					break;
			}
			table = NULL;
			if (seq_code != 0)
				table=SeqCodeTableFind (seq_code);
			if (table == NULL) {
				continue;
			}
			if (extra_loc_cnt > 0) {  /* was converted to new coordinates*/
				for (i=0; i < extra_loc_cnt; i++) {
					if (extra_loc[i] == NULL) {
						continue;
					}
					slp = extra_loc[i];
					pos = FlatLoc(bsp, slp);
					if (pos) {
						the_residue = (Uint1) cbp->aa.value.intvalue;
						if (the_residue == 'U') {
					CpNoteToCharPtrStack(nsp, NULL, "selenocysteine");
						}
						ptr = Get3LetterSymbol(seq_code, table, 
									    the_residue, ajp->error_msgs);
						buflen = StringLen(pos) + StringLen(ptr) + 11;
						buffer = MemNew(buflen);
						sprintf(buffer, "(pos:%s,aa:%s)", pos, ptr);
				    	sfp_out->qual = AddGBQual(sfp_out->qual, 
										    "transl_except", buffer); 
						MemFree(buffer);
						MemFree(pos);
			    	} else if (ajp->error_msgs) {
				    	ErrPostEx(SEV_WARNING, ERR_FEATURE_CodeBreakLoc, 
								"Invalid Code-break.location: %s", pos);
			    	}
				}    
			} else {
				slp = NULL;
				while ((slp = SeqLocFindNext(cbp->loc, slp)) != NULL) {
					pos = FlatLoc(bsp, slp);
			    	if (pos) {
				    	the_residue = (Uint1) cbp->aa.value.intvalue;
						if (the_residue == 'U') {
					CpNoteToCharPtrStack(nsp, NULL, "selenocysteine");
						}
				    	ptr = Get3LetterSymbol(seq_code, table, 
									    the_residue, ajp->error_msgs);
						buflen = StringLen(pos) + StringLen(ptr) + 11;
						buffer = MemNew(buflen);
				    	sprintf(buffer, "(pos:%s,aa:%s)", pos, ptr);
				    	sfp_out->qual = AddGBQual(sfp_out->qual, 
										    "transl_except", buffer);
						MemFree(buffer);
						MemFree(pos);
			    	} else if (ajp->error_msgs) {
				    	ErrPostEx(SEV_WARNING, ERR_FEATURE_CodeBreakLoc, 
								"Invalid Code-break.location: %s", pos);
					}
			    }
			}
			cbp = cbp->next;
		}
	}

	return;

}	/* ComposeCodeBreakQuals */

/***********************************************************************
*void GetGeneticCode(CharPtr ptr, SeqFeatPtr sfp)
*
*	returns ONLY non-standard (i.e., id not 0 or 1)
*	genetic codes.
***********************************************************************/

static void GetGeneticCode(CharPtr ptr, SeqFeatPtr sfp)

{
	Boolean code_is_one=FALSE;
	CdRegionPtr cdr;
	GBQualPtr qual;
	ValNodePtr gcp, var;

	cdr = sfp->data.value.ptrvalue;
	gcp = cdr->genetic_code;

	if (gcp != NULL)
	{
		for (var=gcp->data.ptrvalue; var != NULL; var=var->next)
		{
			if (var->choice == 2)
			{
				if (var->data.intvalue != 0 )
				{
					if (var->data.intvalue == 1)
						code_is_one = TRUE;
					else
						sprintf(ptr, "%ld", (long) (var->data.intvalue));
				}
				break;	
			}
		}
		if (*ptr != '\0')
		{
			for (qual=sfp->qual; qual; qual=qual->next)
			{
				if (StringCmp("transl_table", qual->qual) == 0 &&
				      	  StringCmp(ptr, qual->val) != 0)
				{
				      	  ErrPostStr(SEV_WARNING, 
				 		ERR_FEATURE_GcodeAndTTableClash, "");
				          break;
				}
			}
		}	
		else if (code_is_one == TRUE)
		{
			for (qual=sfp->qual; qual; qual=qual->next)
			{
				if (StringCmp("transl_table", qual->qual) == 0 &&
				      	  StringCmp("1", qual->val) != 0)
				{
				      	  ErrPostStr(SEV_WARNING, 
				 		ERR_FEATURE_GcodeAndTTableClash, "");
				          break;
				}
			}
		}
	}
	else
	{
		for (qual=sfp->qual; qual; qual=qual->next)
			if (StringCmp("transl_table", qual->qual) == 0)
			{
				StringCpy(ptr, qual->val);
				break;
			}
	}

	return;
}	/* GetGeneticCode */

static SeqFeatPtr cleanup_sfp(SeqFeatPtr sfp_out)
{
	ImpFeatPtr ifp;
	
	if (sfp_out == NULL) {
		return NULL;
	}
	sfp_out->comment = NULL;
	sfp_out->location = NULL;
	sfp_out->product = NULL;
	sfp_out->exp_ev = FALSE;
	sfp_out->partial = FALSE;
	sfp_out->excpt = FALSE;
	ifp = sfp_out->data.value.ptrvalue;
	if (ifp->key) {
		ifp->key = MemFree(ifp->key);
	}
	if (ifp->loc) {
		ifp->loc = MemFree(ifp->loc);
	}
	if (sfp_out->qual)
		sfp_out->qual = GBQualFree(sfp_out->qual);
		
	return sfp_out;
}

static GBQualPtr remove_qual(GBQualPtr head, GBQualPtr x)
{
	GBQualPtr	v, p;
	
	if (head == NULL) {
		return NULL;
	}
	if (x == head) {
		head = x->next;
		x->next = NULL;
		return head;
	}
	for (v = head; v != NULL && v != x; v = v->next) {
		p = v;
	}
	if (v != NULL) {
		p->next = x->next;
		x->next = NULL;
	}
	return head;
}

static void PutGeneFirst(SeqFeatPtr sfp)

{
	Boolean still_looking=TRUE;
	GBQualPtr gbqual, qual, qual_temp=NULL, qual_gene=NULL;
	ImpFeatPtr ifp=NULL;

	if ((sfp == NULL) || (sfp->data.choice != 8))
		return;
	if (sfp->qual == NULL)
		return;

	ifp = sfp->data.value.ptrvalue;
	if (StringCmp(ifp->key, "gene") == 0)
	{
		gbqual = sfp->qual;
		for (qual=gbqual; qual; qual=qual->next) {
			if (StringCmp("gene", qual->qual) == 0) {
				qual_gene = qual;
				break;
			}
		}
		if (qual_gene == NULL) {
			return;
		}
		gbqual = remove_qual(gbqual, qual_gene);
		qual_gene->next = gbqual;
		sfp->qual = qual_gene;
	}
	return;
}	/* PutGeneFirst */

static void PutTranslationLast(SeqFeatPtr sfp)

{
	Boolean still_looking=TRUE;
	GBQualPtr gbqual, qual, qual_temp=NULL, qual_last;
	ImpFeatPtr ifp=NULL;

	if ((sfp == NULL) || (sfp->data.choice != 8))
		return;
	if (sfp->qual == NULL)
		return;

	ifp = sfp->data.value.ptrvalue;
	if (StringCmp(ifp->key, "CDS") == 0)
	{
		gbqual = sfp->qual;
		qual_last = NULL;
		for (qual=gbqual; qual->next; qual=qual->next)
		{ /* We need to go to the end of the linked list */
			if (still_looking == TRUE &&
				StringCmp("translation", qual->qual) == 0)
			{
				still_looking = FALSE;
				if (qual->next != NULL)
				{ /* if it's not the last qual anyway */
					if (qual_last == NULL) /*first*/
						gbqual = qual->next;
					else
						qual_last->next = qual->next;
					qual_temp = qual;
					qual=qual->next;
					qual_temp->next = NULL;
				}
			}
			qual_last = qual;
			if (qual->next == NULL)
				break;
		}
		qual->next = qual_temp;
		sfp->qual = gbqual;
	}
	return;
}	/* PutTranslationLast */

static CharPtr mrnaevtext1 = "Derived by automated computational analysis";
static CharPtr mrnaevtext2 = "using gene prediction method:";
static CharPtr mrnaevtext3 = "Supporting evidence includes similarity to:";

NLM_EXTERN CharPtr mRNAEvidenceComment(UserObjectPtr uop, Boolean add)
{
    ObjectIdPtr		oip;
	UserFieldPtr	ufp, u, uu;
	CharPtr			method = NULL, ptr, ne_name;
	static Char		temp[20];
	Int2			ptrlen=0, np=0, nd=0, nm=0, ne=0;
	Boolean			is_evidence = FALSE;
	Int4			Locus_id = 0;

	if (uop == NULL) return NULL;
	if ((oip = uop->type) == NULL) return NULL;
	if (StringCmp(oip->str, "ModelEvidence") != 0) return NULL;
	for (ufp=uop->data; ufp; ufp=ufp->next) {
		oip = ufp->label;
		if (StringCmp(oip->str, "Method") == 0) {
			if (ufp->data.ptrvalue) {
				method = StringSave((CharPtr) ufp->data.ptrvalue);
			}
		}
		if (StringCmp(oip->str, "mRNA")==0) {
			is_evidence = TRUE;
			for (u = (UserFieldPtr) ufp->data.ptrvalue;u; u=u->next) {
				for (uu = (UserFieldPtr) u->data.ptrvalue; uu; uu=uu->next) {
				oip = uu->label;
				if (StringCmp(oip->str, "accession") == 0) {
					nm++;
				}
				}
			}
		}
		if (StringCmp(oip->str, "EST")==0) {
			is_evidence = TRUE;
			for (u = (UserFieldPtr) ufp->data.ptrvalue;u; u=u->next) {
				for (uu = (UserFieldPtr) u->data.ptrvalue;uu; uu=uu->next) {
					oip = uu->label;
					if (StringCmp(oip->str, "count") == 0) {
						ne = uu->data.intvalue;
					}
					if (StringCmp(oip->str, "organism") == 0) {
						ne_name = StringSave(( CharPtr) uu->data.ptrvalue);
					}
				}
			}
		}
	}
	ptrlen = StringLen (mrnaevtext1) + StringLen (mrnaevtext2) + StringLen (mrnaevtext3) + StringLen (method) + 25;
	if (np > 0) {
		ptrlen += StringLen("proteins") + 5;
	}
	if (nd > 0) {
		ptrlen += StringLen("domains") + 5;
	}
	if (nm > 0) {
		ptrlen += StringLen("mRNAs") + 5;
	}
	if (ne > 0) {
		ptrlen += StringLen("ESTs") + StringLen(ne_name) + 10;
	}
	ptr = (CharPtr) MemNew(ptrlen) + 1;
	if (add) {
		if (method != NULL) {
			sprintf (ptr, "%s %s %s.", mrnaevtext1, mrnaevtext1, method);
		} else {
			sprintf (ptr, "%s.", mrnaevtext1);
		}
	}
	if (is_evidence) {
		if (add)  StringCat(ptr, " ");
	 StringCat(ptr, "Supporting evidence includes similarity to:");
	} 
	if (np > 0) {
	 sprintf(temp, " %d proteins", np);
	 StringCat(ptr, temp);
	}
	if (nd > 0) {
		if (np > 0)
	 		StringCat(ptr, ",");
	 sprintf(temp, " %d domains", np);
	 StringCat(ptr, temp);
	}
	if (nm > 0) {
		if (np > 0 || nd > 0)
	 	StringCat(ptr, ",");
	 if (nm > 1) {
		 sprintf(temp, " %d mRNAs", nm);
	 } else {
		 sprintf(temp, " %d mRNA", nm);
	 }
	 StringCat(ptr, temp);
	}
	if (ne > 0) {
	if ( np > 0 || nm > 0 || nd > 0)
	 	StringCat(ptr, ",");
	 sprintf(temp, " %d %s ESTs", ne, ne_name);
	 StringCat(ptr, temp);
	}
	return ptr;
}

static CharPtr mRNAFeatEvidenceComment(SeqFeatPtr sfp_in)
{
	RnaRefPtr		rfp;
	UserObjectPtr	uop, obj;
    ObjectIdPtr		oip;
	UserFieldPtr	uf;
	
	rfp = (RnaRefPtr) sfp_in->data.value.ptrvalue;
	if (rfp->type != 2) { /* mRNA */
		return NULL;
	}
	if ((uop = sfp_in->ext) == NULL)
		return NULL;
	if ((oip = uop->type) == NULL) return NULL;
	if (StringCmp(oip->str, "CombinedFeatureUserObjects") != 0) return NULL;
	for (uf=uop->data; uf; uf=uf->next) {
		obj = (UserObjectPtr) uf->data.ptrvalue;
		return( mRNAEvidenceComment(obj, TRUE));
	}
	return NULL;
}

NLM_EXTERN void PrintNAFeatByNumber (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	
	Boolean loc_ok;
	Char genetic_code[3];
	CharPtr ptr=NULL, sptr;
	ImpFeatPtr ifp;
	SeqFeatPtr sfp_in, sfp_out=NULL;
	Int4 status, total_feats, feat_index;
	SortStructPtr p;

	if (gbp == NULL || gbp->feat == NULL) {
		return;
	}
	feat_index = ajp->pap_index;
	total_feats=gbp->feat->sfpListsize;
	if (total_feats == 0) {
		return;
	}
	sfp_out=ajp->sfp_out;
	if (sfp_out->qual)
		sfp_out->qual = GBQualFree(sfp_out->qual);
	ifp = sfp_out->data.value.ptrvalue;
	if (ifp->loc)
		ifp->loc = MemFree(ifp->loc);
	if (feat_index < total_feats) {
		p = gbp->feat->List + feat_index;
		if (p == NULL)
			return;
		if (p->tempload == TRUE) {
			GatherItemWithLock(p->entityID, p->itemID, p->itemtype, 
									&sfp_in, find_item);
		} else {
			sfp_in = p->sfp;
		}
		if (sfp_in == NULL) {
			return;
		}
		if (ajp->mode == PARTIAL_MODE &&
					sfp_in->data.choice != SEQFEAT_CDREGION) {
			sfp_out = cleanup_sfp(sfp_out);
			return;
		}
		status = ConvertToNAImpFeat(ajp, gbp, sfp_in, &sfp_out, p);
		if (status < 1) {
			sfp_out = cleanup_sfp(sfp_out);
			return;
		}
		if (p->slp != NULL) {
			sfp_out->location = p->slp;
		}
		ifp = sfp_out->data.value.ptrvalue;
		flat2asn_install_feature_user_string(ifp->key, NULL);
		loc_ok=CheckAndGetNAFeatLoc(gbp->bsp, &ptr, sfp_out, TRUE);
		if (loc_ok == TRUE || ASN2FF_VALIDATE_FEATURES == FALSE) {
			ifp->loc = ptr;
		} else {
			flat2asn_delete_feature_user_string();
			flat2asn_install_feature_user_string(ifp->key, ptr);
			MemFree(ptr);
			if (ASN2FF_SHOW_ERROR_MSG == TRUE) {
				ErrPostEx(SEV_WARNING, ERR_FEATURE_Dropped, "Unparsable location");
			}
			sfp_out = cleanup_sfp(sfp_out);
			flat2asn_delete_feature_user_string();
			return;
		}
		flat2asn_delete_feature_user_string();
		flat2asn_install_feature_user_string(ifp->key, ptr);
		if (p->dup == TRUE) {
			if (ASN2FF_SHOW_ERROR_MSG == TRUE) {
				ErrPostEx(SEV_WARNING, ERR_FEATURE_Duplicated, 
					"Duplicated feature dropped");
			}
			sfp_out = cleanup_sfp(sfp_out);
			flat2asn_delete_feature_user_string();
			return;
		}
		if (sfp_in->data.choice == SEQFEAT_CDREGION) {
			ComposeCodeBreakQuals(ajp, gbp->bsp, sfp_in, sfp_out, 
							p->extra_loc, p->extra_loc_cnt, p->nsp);
			genetic_code[0]='\0';
			if (ASN2FF_TRANSL_TABLE == TRUE) {
				GetGeneticCode(genetic_code, sfp_in);
				if (genetic_code[0] != '\0') {
					sfp_out->qual = AddGBQual(sfp_out->qual, 
							"transl_table", genetic_code);
				}
			}
		}
		if (sfp_in->data.choice == SEQFEAT_GENE) {
			if (ajp->show_gene == FALSE) {
				sfp_out = cleanup_sfp(sfp_out);
				flat2asn_delete_feature_user_string();
				return;
			}
		}
		GetNonGeneQuals(ajp->mode, sfp_in, sfp_out, p->nsp);
		LookForPartialImpFeat(sfp_out, FALSE);
		ComposeGBQuals(ajp, sfp_out, gbp, p, FALSE);
		status = ValidateNAImpFeat(sfp_out);
		if (sfp_in->data.choice == SEQFEAT_CDREGION) {
			PutTranslationLast(sfp_out);
		} else if (sfp_in->data.choice == SEQFEAT_GENE) {
			PutGeneFirst(sfp_out);
		} else if (sfp_in->data.choice == SEQFEAT_RNA) {
			if ((sptr = mRNAFeatEvidenceComment(sfp_in)) != NULL) {
				sfp_out->qual =
				     AddGBQual(sfp_out->qual, "note", sptr);
			}
		}
		if (status >= 0 || ASN2FF_VALIDATE_FEATURES == FALSE) {
			PrintImpFeatEx(ajp, gbp->bsp, sfp_out, gbp->gi, p->entityID, p->itemID);
		}
		flat2asn_delete_feature_user_string();
	}
	sfp_out = cleanup_sfp(sfp_out);
	return;
}	/* PrintNAFeatByNumber */

/***************************************************************************
*PrintAAFeatByNumber
*
*	This function prints out the genpept SeqFeats.
*
**************************************************************************/

NLM_EXTERN void PrintAAFeatByNumber (Asn2ffJobPtr ajp, GBEntryPtr gbp)
{
	CharPtr ptr=NULL;
	Char genetic_code[3];
	ImpFeatPtr ifp;
	Int2 status;
	Int4 feat_index, total_feats;
	NoteStructPtr nsp;
	SeqFeatPtr sfp_in, sfp_out=NULL;
	SortStructPtr p;

	if (gbp == NULL || gbp->feat == NULL) {
		return;
	}
	feat_index = ajp->pap_index;
	total_feats=gbp->feat->sfpListsize;
	if (total_feats == 0) {
		return;
	}
	sfp_out=ajp->sfp_out;
	if (sfp_out->qual) {
		sfp_out->qual = GBQualFree(sfp_out->qual);
	}
	ifp = sfp_out->data.value.ptrvalue;
	if (ifp->loc) {
		ifp->loc = MemFree(ifp->loc);
	}
	if (feat_index < total_feats) {
		p = gbp->feat->List + feat_index;
		if (p == NULL || p->dup == TRUE) {
			return;
		}
		if ((sfp_in = p->sfp) == NULL) {
			GatherItemWithLock(p->entityID, p->itemID, p->itemtype, 
									&sfp_in, find_item);
		}
		if (sfp_in == NULL) {
			return;
		}
		nsp = p->nsp;
		switch (sfp_in->data.choice) {
/* Note: the functions that CheckAndGetFeatLoc use for
		checking fails on protein locations sometimes. */
			case SEQFEAT_CDREGION:
				GetNonGeneQuals(ajp->mode, sfp_in, sfp_out, nsp);
				status = ConvertToAAImpFeat(ajp, gbp, sfp_in, &sfp_out, p);
				if (status < 0)
					break;
				ComposeGBQuals(ajp, sfp_out, gbp, p, FALSE);
				GetAAFeatLoc(gbp->bsp, &ptr, sfp_in, TRUE);
				ifp->loc = ptr;
				ptr = FlatLoc(gbp->bsp, sfp_in->location);
				sfp_out->qual =
				     AddGBQual(sfp_out->qual, "coded_by", ptr);
				ptr = MemFree(ptr);
				genetic_code[0]='\0';
				if (ASN2FF_TRANSL_TABLE == TRUE) {
					GetGeneticCode(genetic_code, sfp_in);
					if (genetic_code[0] != '\0')
						sfp_out->qual =
							AddGBQual(sfp_out->qual, "transl_table", genetic_code);
				}
				status = ValidateAAImpFeat(sfp_out, TRUE);
				if (status >= 0)
					PrintImpFeat(ajp, gbp->bsp, sfp_out);
				break;
			case SEQFEAT_PROT:
				GetNonGeneQuals(ajp->mode, sfp_in, sfp_out, nsp);
				AddProteinQuals(sfp_in, sfp_out, nsp);
				status = ConvertToAAImpFeat(ajp, gbp, sfp_in, &sfp_out, p);
				if (status < 0)
					break;
				ComposeGBQuals(ajp, sfp_out, gbp, p, FALSE);
				GetAAFeatLoc(gbp->bsp, &ptr, sfp_out, FALSE);
				ifp->loc = ptr;
				status = ValidateAAImpFeat(sfp_out, FALSE);
				if (status >= 0)
					PrintImpFeat(ajp, gbp->bsp, sfp_out);
				break;
			case SEQFEAT_SEQ:
			case SEQFEAT_IMP:
			case SEQFEAT_REGION:
			case SEQFEAT_COMMENT:
			case SEQFEAT_BOND:
			case SEQFEAT_SITE:
			case SEQFEAT_PSEC_STR:
			case SEQFEAT_NON_STD_RESIDUE:
			case SEQFEAT_HET:
				GetNonGeneQuals(ajp->mode, sfp_in, sfp_out, nsp);
				status = ConvertToAAImpFeat(ajp, gbp, sfp_in, &sfp_out, p);
				if (status < 0)
					break;
				ComposeGBQuals(ajp, sfp_out, gbp, p, FALSE);
				GetAAFeatLoc(gbp->bsp, &ptr, sfp_out, FALSE);
				ifp->loc = ptr;
				status = ValidateAAImpFeat(sfp_out, FALSE);
				if (status >= 0)
					PrintImpFeat(ajp, gbp->bsp, sfp_out);
				break;
			case SEQFEAT_GENE:
				if (ajp->show_gene == FALSE) {
					break;
				}
				GetNonGeneQuals(ajp->mode, sfp_in, sfp_out, nsp);
				status = ConvertToAAImpFeat(ajp, gbp, sfp_in, &sfp_out, p);
				if (status < 0)
					break;
				ComposeGBQuals(ajp, sfp_out, gbp, p, FALSE);
				GetAAFeatLoc(gbp->bsp, &ptr, sfp_out, FALSE);
				ifp->loc = ptr;
				status = ValidateAAImpFeat(sfp_out, FALSE);
				if (status >= 0)
					PrintImpFeat(ajp, gbp->bsp, sfp_out);
				break;
			default:
				break;
		}
	}
	sfp_out = cleanup_sfp(sfp_out);
}	/* PrintAAFeatByNumber */


/************************************************************************
*GetProductFromCDS(ValNodePtr product, ValNodePtr location, Int4 length)
*
*	Gets the CDS product, using SeqPortNewByLoc
*	The bsp is that of the protein, and comes from the location.  The bsp
*	is found in the calling program anyway, as it's used to get
*	the EC_NUM.
*	The protein sequence comes back in allocated memory.  The user
*	is responsible for deallocating that.
*
*  A check is made (BioseqFind()) that the protein Bioseq is in memory.
*  This guarantees that a fetch is NOT made if it is not memory, to accomodate
*  the splitting of DNA and protein in Entrez. In this case, it's just
*  translated.
*
*************************************************************************/

NLM_EXTERN CharPtr GetProductFromCDS(ValNodePtr product, ValNodePtr location, Int4 bsp_length)

{
	Boolean at_end=FALSE;
	CharPtr protein_seq=NULL, start_ptr=NULL;
	Int4 length;
	SeqPortPtr spp;
	Uint1 residue, code;
	BioseqPtr bsp;
	SeqIdPtr sip;

	if (ASN2FF_IUPACAA_ONLY == TRUE)
		code = Seq_code_iupacaa;
	else
		code = Seq_code_ncbieaa;

	if (product) {
		sip = SeqLocId(product);
		bsp = BioseqFindCore(sip);
		if (bsp != NULL)    /* Bioseq is (or has been) in memory */ {
			length = SeqLocLen(product);
			if (length > 0) {
				if (SeqLocStart(location) == 0 ||
					SeqLocStop(location) == bsp_length-1)
					at_end = TRUE;
				start_ptr = protein_seq = 
					(CharPtr) MemNew((size_t) (length*sizeof(CharPtr)));
				spp = SeqPortNewByLoc(product, code);
				spp->do_virtual = TRUE;
				while ((residue=SeqPortGetResidue(spp)) != SEQPORT_EOF) {
					if ( !IS_residue(residue) && residue != INVALID_RESIDUE )
						continue;
					if (residue == INVALID_RESIDUE)
						residue = (Uint1) 'X';
					*protein_seq = residue;
					protein_seq++;
				}
				SeqPortFree(spp);
				if (at_end) {
					if (StringLen(start_ptr) < GENPEPT_MIN)
						start_ptr = MemFree(start_ptr);
				}
			}
		}
	}
	return start_ptr;
}

/**************************************************************************
*CharPtr GettRNAaa (tRNAPtr trna, Boolean error_messages)
*
*	Return a pointer containing the amino acid type.
**************************************************************************/

static CharPtr GettRNAaa (tRNAPtr trna, Boolean error_msgs)

{
	CharPtr ptr=NULL;
	SeqCodeTablePtr table;
	Uint1 seq_code;
/*
  The choice values used in the tRNA structure do NOT corresond to
  the choice(==ENUMs) of Seq-code_type, and the latter are used
  by all the utility functions, so we map them...
*/
	if ( trna && trna -> aatype) {
		switch (trna -> aatype) {
			case 1:
				seq_code = 2;
				break;
			case 2:
				seq_code = 8;
				break;
			case 3:
				seq_code = 7;
				break;
			case 4:
				seq_code = 11;
				break;
		}

		if ((table=SeqCodeTableFind (seq_code)) != NULL)
		  ptr = Get3LetterSymbol(seq_code, table, trna->aa, error_msgs);
	}
	
	return ptr;
}	/* GettRNAaa */

/*************************************************************************
*ComposetRNANote (Asn2ffJobPtr ajp, NoteStructPtr nsp, tRNAPtr trna, )
*
*	Add info from Trna-ext to Note stack in the GeneStructPtr.
**************************************************************************/

static void ComposetRNANote(Asn2ffJobPtr ajp, NoteStructPtr nsp, tRNAPtr trna)
{
	/*
	Char buffer[25];
	CharPtr ptr = &(buffer[0]);
	Int2 index;
	Uint1 codon[4];

	if (! trna) 
		return;

	if ((trna->codon)[0] != 255)
	{
		codon[3] = '\0';
		for (index=0; index<6; index++)
		{
			if ((trna->codon)[index] == 255)
				break;
			if (CodonForIndex((trna->codon)[index], Seq_code_iupacna, codon))
			{
				StringCpy(ptr, (CharPtr) codon);
				ptr += 3;
			}
			else
			{
				*ptr = '?';	ptr++;
			}
			if (index<5 && (trna->codon)[index+1] != 255)
			{
				*ptr = ',';	ptr++;
				*ptr = ' ';	ptr++;
			}
		}
		if ((trna->codon)[1] == 255)
		{
			ptr = &buffer[0];
			SaveNoteToCharPtrStack(nsp, "codon recognized:", ptr);
		}
		else
		{
			ptr = &buffer[0];
			SaveNoteToCharPtrStack(nsp, "codons recognized:", ptr);
		}
	}	
	return;
	*/

	Char  buffer [25];
	Int2  num;

	num = ComposeCodonsRecognizedString (trna, buffer, sizeof (buffer));
	if (num < 1 || StringHasNoText (buffer)) return;
	if (num == 1) {
			SaveNoteToCharPtrStack(nsp, "codon recognized:", buffer);
	} else {
			SaveNoteToCharPtrStack(nsp, "codons recognized:", buffer);
	}

}	/* ComposetRNANote */


/************************************************************************
*	Make the anticodon qualifier and (possible) note to the tRNA
*	with the following paradigm:
*	0.) First look at the new anticodon slot on tRNAPtr
*	if not found do the rest:
**	1.) Look at SeqFeat.ext for a UserObject using the fct. QualLocWrite,
*	if result is not NULL, use this location in anticodon qualifier;
*
*	2.) Look for an anticodon qualifier, use if no QualLocWrite's 
*	result was not zero;
*	3.) Make note if neither 1.) or 2.) was true, or there are
*	multiple codons.
*****************************
*	NEW ALGORITHM 07-15-96
*****************************
*	1) aa present?
*	      print /product = tRNA-aa
*	2) codon recognized present?
*	      print /note="codon recognized: codon"
*	3) anticodon and aa present?
*	      print /anticodon=...
*************************************************************************/
static void DotRNAQuals (Asn2ffJobPtr ajp, GBEntryPtr gbp, SeqFeatPtr sfp_in, SeqFeatPtr sfp_out, NoteStructPtr nsp, SeqLocPtr PNTR extra_loc, 
Int2 extra_loc_cnt)
{
	Boolean found_anticodon=FALSE /*, found_qual=FALSE -- UNUSED */;
	Char buffer[40];
	CharPtr aa_ptr, newptr=NULL, ptr = &(buffer[0]), tmp=NULL;
	GBQualPtr curq;
	RnaRefPtr rrp;
	tRNAPtr trna=NULL;
	SeqLocPtr slp=NULL;
	SeqIntPtr sip;

	if (sfp_in == NULL) {
		return;
	}
	if (sfp_in->data.choice != SEQFEAT_RNA) {
		return;
	}
	rrp = sfp_in->data.value.ptrvalue;
/* Look for anticodon struct */
	if (rrp->ext.choice == 2) {
		newptr = MemNew(50*sizeof(Char));
		trna = rrp->ext.value.ptrvalue;
		if ((aa_ptr = GettRNAaa(trna, ajp->error_msgs)) != NULL) {
			if (GBQualPresent("product", sfp_out->qual) == FALSE) {
				sprintf(newptr, "tRNA-%s", aa_ptr);
				sfp_out->qual = AddGBQual(sfp_out->qual, "product", newptr);
			}
		}
		if (trna && (slp = trna->anticodon) != NULL && aa_ptr) {
			if (extra_loc_cnt > 0) {
				slp = extra_loc[0];
			}
			if (slp && slp->choice == SEQLOC_INT) {
				sip = slp->data.ptrvalue;
				sprintf(ptr, "%ld..%ld", (long) sip->from+1, (long) sip->to+1);
				sprintf(newptr, "(pos:%s,aa:%s)", ptr, aa_ptr);
				sfp_out->qual = AddGBQual(sfp_out->qual, "anticodon", newptr); 
				found_anticodon=TRUE;
			}
		}
	}
	if (! found_anticodon) {
		if (sfp_in->ext) {	/* Look for UserObject */
			tmp = QualLocWrite(sfp_in->ext, ptr);
			if (tmp) {
				newptr = MemNew(50*sizeof(Char));
				rrp = sfp_in->data.value.ptrvalue;
				trna = rrp->ext.value.ptrvalue;
				aa_ptr = GettRNAaa(trna, ajp->error_msgs);
				if (aa_ptr) {
					sprintf(newptr, "(pos:%s,aa:%s)", ptr, aa_ptr);
					sfp_out->qual = 
							AddGBQual(sfp_out->qual, "anticodon", newptr); 
					found_anticodon=TRUE;
				}
			}
		}
	}
	if (! found_anticodon) {
		/* Look for anticodon qual if no UserObject found */
		for (curq=sfp_in->qual; curq; curq=curq->next)
			if (StringCmp("anticodon", curq->qual) == 0) {
			    sfp_out->qual = 
			       AddGBQual(sfp_out->qual, "anticodon", curq->val); 
			    /* found_qual=TRUE; -- NO EFFECT */
			    break;
			}
	}

/* make note "codon recognized*/
	ComposetRNANote(ajp, nsp, trna);
	MemFree(newptr);
	
}	/* DotRNAQuals */

/**************************************************************************
*ConvertToAAImpFeat
*
*	This code copies a SeqFeat into an ImpFeat format for use in
*	producing GenBank format.  Two SeqFeatPtr's should be passed
*	in as arguments (sfp_in, sfp_out).  On the first call, of a
*	number of calls, sfp_out should be NULL so that memory for 
*	ImpFeat can be allocated.  On subsequent calls, sfp_out->data.choice
*	should be "8" (for ImpFeats).
*
*	Written by Tom Madden 
*
**************************************************************************/

NLM_EXTERN Int2 ConvertToAAImpFeat (Asn2ffJobPtr ajp, GBEntryPtr gbp, SeqFeatPtr sfp_in, SeqFeatPtr PNTR sfpp_out, SortStructPtr p)
{
	BioseqPtr bsp=NULL;
	Char printbuf[41], temp[65];
	CharPtr ptr;
	ImpFeatPtr ifp, ifp_in;
	Int2 retval=1;
	NoteStructPtr nsp;
	GeneStructPtr gsp;
	ProtRefPtr prot;
	SeqFeatPtr sfp_out;
	SeqIdPtr sip=NULL, xid;
	ValNodePtr vnp, vnp1;

	sfp_out = *sfpp_out;

	if (sfp_out->data.choice != SEQFEAT_IMP)
		return -1;

	ifp = (ImpFeatPtr) sfp_out->data.value.ptrvalue;

	sfp_out->partial = sfp_in->partial;
	sfp_out->comment = sfp_in->comment;
	sfp_out->exp_ev = sfp_in->exp_ev;
	sfp_out->location = sfp_in->location;

	nsp = p->nsp;
	gsp = p->gsp;
	if (sfp_out->comment) {
		CpNoteToCharPtrStack(nsp, NULL, (CharPtr) sfp_out->comment);
	}

	switch (sfp_in->data.choice) {
	case SEQFEAT_CDREGION:
		ifp->key = StringSave("CDS");
		break;
	case SEQFEAT_PROT:
		prot = sfp_in->data.value.ptrvalue;
		if (prot->processed == 0 || prot->processed == 1) {
			GetProtRefInfo(ajp->format, gsp, nsp, prot);
			ifp->key = StringSave("Protein");
		} else if (prot->processed == 2) {
			ifp->key = StringSave("mat_peptide");
		} else if (prot->processed == 3) {
			ifp->key = StringSave("sig_peptide");
		} else if (prot->processed == 4) {
			ifp->key = StringSave("transit_peptide");
		}
		if (sfp_in->location) {
			sip = SeqLocId(sfp_in->location);
			if (sip)
				bsp = BioseqFind(sip);
			if (bsp) {
				vnp = bsp->descr;
				for (vnp = bsp->descr; vnp; vnp = vnp->next) {
					if (vnp->choice != Seq_descr_modif) {
						continue;
					}
					for (vnp1 = vnp->data.ptrvalue; vnp1; vnp1=vnp1->next) {
						if (vnp1->data.intvalue == 1) {
							sfp_out->partial = TRUE;
							break;
					    }
					}
				}
			}
		}
		break;
	case SEQFEAT_SEQ:	
		ifp->key = StringSave("misc_feature");
		if ((xid=CheckXrefFeat(gbp->bsp, sfp_in)) != NULL)
		{
			SeqIdWrite(xid, printbuf, PRINTID_FASTA_SHORT, 40);
			ptr = &(temp[0]);
			sprintf(ptr, "Cross-reference: %s", printbuf);
			SaveNoteToCharPtrStack(nsp, NULL, ptr);
		}
		else
			retval = 0;
		break;
	case SEQFEAT_IMP:
		ifp_in = (ImpFeatPtr) sfp_in->data.value.ptrvalue;
		ifp->key = StringSave(ifp_in->key);
		break;
	case SEQFEAT_REGION:
		sfp_out->qual = 
		AddGBQual(sfp_out->qual, "region_name", sfp_in->data.value.ptrvalue);
		ifp->key = StringSave("Region");
		break;
	case SEQFEAT_COMMENT:
		ifp->key = StringSave("misc_feature");
		break;
	case SEQFEAT_BOND:
		ptr = AsnEnumStr("SeqFeatData.bond", 
							(Int2) (sfp_in->data.value.intvalue));
		sfp_out->qual = AddGBQual(sfp_in->qual, "bond_type", ptr);
		ifp->key = StringSave("Bond");
		break;
	case SEQFEAT_SITE:
		ptr = AsnEnumStr("SeqFeatData.site", 
							(Int2) (sfp_in->data.value.intvalue));
		sfp_out->qual = AddGBQual(sfp_out->qual, "site_type", ptr);
		ifp->key = StringSave("Site");
		break;
	case SEQFEAT_PSEC_STR:
		ptr = AsnEnumStr("SeqFeatData.psec-str", 
								(Int2) (sfp_in->data.value.intvalue));
		sfp_out->qual = AddGBQual(sfp_out->qual, "sec_str_type", ptr);
		ifp->key = StringSave("SecStr");
		break;
	case SEQFEAT_NON_STD_RESIDUE:
		sfp_out->qual = 
			AddGBQual(sfp_out->qual, "non-std-residue", 
											sfp_in->data.value.ptrvalue);
		ifp->key = StringSave("NonStdResidue");
		break;
	case SEQFEAT_HET:
		sfp_out->qual = 
		     AddGBQual(sfp_out->qual, "heterogen", sfp_in->data.value.ptrvalue);
		ifp->key = StringSave("Het");
		break;
	default:
		if (ajp->error_msgs == TRUE)
			ErrPostStr(SEV_WARNING, ERR_FEATURE_UnknownFeatureKey, 
				"Unimplemented type of feat in ConvertToAAImpFeat");
		retval = 1;
		break;
	}

	return retval;

}	/* ConvertToAAImpFeat */

/*****************************************************************************
*  CompareTranslation:
*  -- if bsp != translation's value return FALSE
*****************************************************************************/
static Boolean CompareTranslation(ByteStorePtr bsp, CharPtr qval)
{
	CharPtr		 		ptr;
	Int2			 residue, residue1, residue2;
	Int4			 len, blen;
	Boolean		 done;

	if (qval == NULL || bsp == NULL) {
		return FALSE;  /* no comparison */
	}
	len = StringLen(qval);
	BSSeek(bsp, 0, SEEK_SET);

	blen = BSLen(bsp);
	done = FALSE;
	while ((! done) && (len)) {
		  residue1 = qval[(len-1)];
		  if (residue1 == 'X')	/* remove terminal X */
				len--;
		  else
				done = TRUE;
	 }
	 done = FALSE;
	 while ((! done) && (blen)) {
		  BSSeek(bsp, (blen-1), SEEK_SET);
		  residue2 = BSGetByte(bsp);
		  if (residue2 == 'X')
				blen--;
		  else
				done = TRUE;
	 }
		BSSeek(bsp, 0, SEEK_SET);
		if (blen != len) {
			return FALSE;
		} else {
			for (ptr = qval; *ptr != '\0' && 
								(residue = BSGetByte(bsp)) != EOF; ptr++) {

				 if (residue != *ptr) {
					return FALSE;
				 }

			 } /* for */

		 } /* compare two sequences */
			return TRUE;
} /* check */

static void  GatherProductGeneInfo (Asn2ffJobPtr ajp, SeqFeatPtr sfp_in, GBEntryPtr gbp, SortStructPtr gp, Uint1 method)
{
	BioseqPtr p_bsp;
	GatherScope gs;
	GeneStructPtr gsp;
	NoteStructPtr nsp;
	Int2 index;
	Int4 length, longest_length=0;
	ProtRefPtr prot=NULL;
	SeqFeatPtr sfp=NULL;
	SeqIdPtr sip;
	ValNodePtr product=NULL;
	OrganizeProtPtr opp;
	SortStructPtr p;
	Uint2 entityID;
	
	if (sfp_in->product)
		product = sfp_in->product;
	else 
		return;
	if (gp == NULL)
		return;
	gsp = gp->gsp;
	nsp = gp->nsp;
	sip = SeqLocId(product);
	p_bsp = BioseqFindCore(sip);
	if (p_bsp == NULL)    /* Bioseq is (or has been) in memory */
		return;
	if (ajp->useSeqMgrIndexes) {
		sfp = SeqMgrGetBestProteinFeature (p_bsp, NULL);
		if (sfp != NULL && sfp->data.choice == SEQFEAT_PROT) {
			prot = (ProtRefPtr) sfp->data.value.ptrvalue;
			if (prot != NULL) {
				GetProtRefInfo(ajp->format, gsp, nsp, prot);
				GetProtRefComment(sfp_in, p_bsp, ajp, NULL, nsp, method);
 				return;
 			}
 		}
	}
	entityID = ObjMgrGetEntityIDForPointer(p_bsp);
	opp = (OrganizeProtPtr) MemNew(sizeof(OrganizeProt));
	opp->size = 0;
	  MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
	MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
	gs.ignore[OBJ_SEQANNOT] = FALSE;
	gs.ignore[OBJ_SEQFEAT] = FALSE;
	gs.get_feats_location = TRUE;
	gs.target = product;
	gs.seglevels = 1;
	GatherEntity(entityID, opp, get_prot_feats, &gs);
	if (opp->size > 0)	{
		prot = NULL;
		p = opp->list;
		for (index=0; index < opp->size; index++, p++) {
			if ((sfp = p->sfp) == NULL) {
				GatherItemWithLock(p->entityID, p->itemID, p->itemtype, 
								&sfp, find_item);
			}
			if (sfp == NULL) {
				continue;
			}
			if (sfp->data.choice != SEQFEAT_PROT) {
				continue;
			}
			if ((length=SeqLocLen(sfp->location)) == -1)
				continue;
			if (length > longest_length) {
				prot = sfp->data.value.ptrvalue;
				longest_length = length;
			}
		}
		GetProtRefInfo(ajp->format, gsp, nsp, prot);
	}
	GetProtRefComment(sfp_in, p_bsp, ajp, opp, nsp, method);
	p = opp->list;
	for (index=0; index < opp->size; index++, p++) {
		if (p && p->gsp)
			GeneStructFree(p->gsp);
		if (p && p->nsp)
			NoteStructFree(p->nsp);
	}
	MemFree(opp->list);
	MemFree(opp);
	
 	return;
}

/**************************************************************************
*ConvertToNAImpFeat
*
*	This code copies a SeqFeat into an ImpFeat format for use in
*	producing GenBank format.  Two SeqFeatPtr's should be passed
*	in as arguments (sfp_in, sfp_out).  
*	return status:
*		1: conversion successful
*		0: no conversion, also no error (data in ASN.1 is lost or put out
*			otherwise
*		-1 an error
**************************************************************************/

static Boolean ProductIsLocal (Uint2 entityID, SeqLocPtr product)

{
  BioseqPtr    bsp;
  SeqEntryPtr  sep, oldscope;
  SeqIdPtr     sip = NULL;
  SeqLocPtr    slp;

  slp = SeqLocFindNext (product, NULL);
  while (slp != NULL && sip == NULL) {
    sip = SeqLocId (slp);
    slp = SeqLocFindNext (product, slp);
  }
  if (sip == NULL) return FALSE;
  sep = GetTopSeqEntryForEntityID (entityID);
  if (sep == NULL) return FALSE;
  oldscope = SeqEntrySetScope (sep);
  bsp = BioseqFind (sip);
  SeqEntrySetScope (oldscope);
  if (bsp != NULL) return TRUE;
  return FALSE;
}

NLM_EXTERN Int2 ConvertToNAImpFeat (Asn2ffJobPtr ajp, GBEntryPtr gbp, SeqFeatPtr sfp_in, SeqFeatPtr PNTR sfpp_out, SortStructPtr gp)
{
	BioseqPtr bsp=gbp->bsp, pbsp=NULL;
	Boolean found_key, non_pseudo = FALSE;
	CdRegionPtr cdr;
	Char buffer[2], printbuf[41], temp[65];
	CharPtr buf_ptr = &(buffer[0]), protein_seq=NULL, ptr = &(temp[0]);
	NoteStructPtr nsp;
	ImpFeatPtr ifp, ifp_in;
	Int2 retval=1;
	Int4 length=0;
	SeqFeatPtr sfp_out;
	SeqIdPtr xid;
	ValNodePtr product;
	ValNodePtr mod, syn;
	BioSourcePtr biosp;
	OrgRefPtr orp;
	RnaRefPtr rrp;
	ByteStorePtr byte_sp;
	/* Int4 len_cds, len_prot; -- UNUSED */
	Uint1 method = 0;
	GeneRefPtr grp;
	Boolean was_gene = FALSE;
	CharPtr key=NULL, tmp;
	GeneStructPtr gsp;
	CharPtr except_msg="No explanation supplied", loc;
	
	sfp_out = *sfpp_out;

	if (sfp_out->data.choice != SEQFEAT_IMP)
		return -1;

	ifp = (ImpFeatPtr) sfp_out->data.value.ptrvalue;

	sfp_out->partial = sfp_in->partial;
	sfp_out->comment = sfp_in->comment;
	sfp_out->exp_ev = sfp_in->exp_ev;
	sfp_out->location = sfp_in->location;
	sfp_out->product = sfp_in->product;
	sfp_out->pseudo = sfp_in->pseudo;
	
	found_key = GetNAFeatKey(ajp->show_gene, &(key), sfp_in, sfp_out);
	if (!found_key)
		return -1;
	ifp->key = key;
	nsp = gp->nsp;
	gsp=gp->gsp;
	switch (sfp_in->data.choice)
	{
	case SEQFEAT_BIOSRC:
		biosp = sfp_in->data.value.ptrvalue;
		orp = (OrgRefPtr) biosp->org;	
		if (orp) {
			if (orp->taxname) {
				sfp_out->qual = AddGBQual(sfp_out->qual, 
									"organism", orp->taxname);
			} else if (orp->common) {
				if (StrStr(orp->common, "virus") ||
				    StrStr(orp->common, "Virus") ||
				    StrStr(orp->common, "phage") ||
				    StrStr(orp->common, "Phage") ||
				    StrStr(orp->common, "viroid") ||
				    StrStr(orp->common, "Viroid"))
				{
					sfp_out->qual = AddGBQual(sfp_out->qual, "organism",
															 orp->common);
				}
			}
/* added from OrgRef.mod 03.20.96 */
			for (mod = orp->mod; mod; mod = mod->next) {
				CpNoteToCharPtrStack(nsp, NULL, (CharPtr) mod->data.ptrvalue);
			}
		} else {
			sfp_out->qual = AddGBQual(sfp_out->qual, "organism",
															 "unknown");
		}
		sfp_out->qual = AddBioSourceToGBQual(ajp, nsp, biosp, sfp_out->qual,
		 	TRUE);
		break;	
	case SEQFEAT_CDREGION:
		product = sfp_in->product;
		if (ajp->mode == RELEASE_MODE) {
			if (GBQualPresent("pseudo", sfp_in->qual) == FALSE &&
						gsp->pseudo == FALSE && sfp_in->pseudo == FALSE) {
				non_pseudo = TRUE;
			}
		  if (non_pseudo) {
			if (product == NULL) {
				if (ajp->error_msgs == TRUE) {
					loc = SeqLocPrint(sfp_in->location);
					ErrPostEx(SEV_ERROR, ERR_FEATURE_Dropped, 
						"Dropping CDS due to missing product: %s", loc);
					MemFree(loc);
				}
				return -1;
			}
			if (ajp->forgbrel && CheckSeqIdChoice(SeqLocId(product)) == FALSE) {
				if (ajp->error_msgs == TRUE) {
					loc = SeqLocPrint(sfp_in->location);
					ErrPostEx(SEV_ERROR, ERR_FEATURE_Dropped, 
						"Dropping CDS due to missing EMBL/DDBJ/GB protein accession: %s", loc);
					MemFree(loc);
				}
				return -1;
			}
			if (ajp->forgbrel && (pbsp = BioseqFindCore(SeqLocId(product))) == NULL) {
				if (ajp->error_msgs == TRUE) {
					loc = SeqLocPrint(sfp_in->location);
					ErrPostEx(SEV_ERROR, ERR_FEATURE_Dropped, 
						"Dropping CDS due to missing protein: %s", loc);
					MemFree(loc);
				}
				return -1;
			}
			if (pbsp != NULL) {
				if (ajp->forgbrel && CheckSeqIdChoice(pbsp->id) == FALSE) {
						if (ajp->error_msgs == TRUE) {
							loc = SeqLocPrint(sfp_in->location);
							ErrPostEx(SEV_ERROR, ERR_FEATURE_Dropped, 
				    		"Dropping CDS due to missing EMBL/DDBJ/GB protein accession: %s", loc);
							MemFree(loc);
						}
						return -1;
				}
				if (ajp->show_version == TRUE) {
					if (CheckSeqIdAccVer(pbsp->id) == FALSE) {
						if (ajp->error_msgs == TRUE) {
							loc = SeqLocPrint(sfp_in->location);
							ErrPostEx(SEV_ERROR, ERR_FEATURE_Dropped, 
								"Dropping CDS due to missing protein accession.version: %s", loc);
							MemFree(loc);
						}
						return -1;
					}
				}
			}
		  }
		}
		cdr = (CdRegionPtr) sfp_in->data.value.ptrvalue;
		if ((GBQualPresent("codon_start", sfp_in->qual)) == FALSE)
		{ /* Above checks if codon_start is already present. */
			if (cdr->frame)
				sprintf(buf_ptr, "%ld", (long) (cdr->frame)); 
			else 
				sprintf(buf_ptr, "1"); 
			sfp_out->qual = AddGBQual(sfp_out->qual, "codon_start", buf_ptr);
		}
		if (product && (! ajp->genome_view) && (ProductIsLocal (ajp->entityID, product))) {
			byte_sp = ProteinFromCdRegion(sfp_in, FALSE);

			if (product) {
				length = bsp->length;
				protein_seq = GetProductFromCDS(product, sfp_in->location, length);
/* check conflict flag and fix it */
				if (cdr->conflict == TRUE) {
					if (CompareTranslation(byte_sp, protein_seq)) {
						cdr->conflict = FALSE;
					} else {
						method = METHOD_concept_transl_a;
					}
				}
				if (protein_seq) {
					if ((GBQualPresent("pseudo", sfp_in->qual)) == FALSE &&
						 gsp->pseudo == FALSE && sfp_in->pseudo == FALSE) {
						sfp_out->qual = AddGBQual(sfp_out->qual, 
										"translation", protein_seq);
					}
					MemFree(protein_seq);
				}
			}
			BSFree(byte_sp);
		}
		if (sfp_in->pseudo) {
			sfp_out->qual = AddGBQual(sfp_out->qual, "pseudo", NULL);
		}
		if (sfp_in->excpt) {
			if (StringCmp("ribosomal slippage", sfp_in->except_text) == 0 ||
				StringCmp("ribosome slippage", sfp_in->except_text) == 0) {
				sfp_out->qual = AddGBQual(sfp_out->qual, 
									"note", sfp_in->except_text);
				sfp_out->excpt = FALSE;
			} else if (StringCmp("trans splicing", sfp_in->except_text) == 0 ||
						StringCmp("trans-splicing", sfp_in->except_text) == 0) {
				sfp_out->excpt = FALSE;
			} else if (sfp_in->except_text) {
				sfp_out->qual = AddGBQual(sfp_out->qual, 
									"exception", sfp_in->except_text);
			} else if (GBQualPresent("exception", sfp_in->qual) == TRUE) {
				sfp_out->qual = AddGBQual(sfp_out->qual, 
									"exception", sfp_in->qual->val);
			} else if (sfp_out->comment != NULL) {
				sfp_out->qual = AddGBQual(sfp_out->qual, 
									"exception", sfp_in->comment);
				sfp_out->comment = NULL;			
			} else {
				sfp_out->qual = AddGBQual(sfp_out->qual, 
									"exception", except_msg);
			}
		} else {
			if (GBQualPresent("exception", sfp_in->qual) == TRUE) {
				sfp_out->qual = AddGBQual(sfp_out->qual, 
									"exception", sfp_in->qual->val);
			}
		}
		
		GatherProductGeneInfo(ajp, sfp_in, gbp, gp, method);

/******************************************************************************
- asn2ff shouldn't generate a de-novo /translation for any
  cdregion that lacks a product, regardless of mode or -V setting  2/15/99
******************************************************************************
		if (protein_seq == NULL && ajp->mode != RELEASE_MODE) {
			protein_seq = BSMerge(byte_sp, NULL);
			if ( protein_seq && protein_seq[0] != '-') {
				len_prot = StringLen(protein_seq);
				SeqLocLen(sfp_in->location) - (cdr->frame - 1);
				if (len_prot >= 6) {
					if ((GBQualPresent("pseudo", sfp_in->qual)) == FALSE &&
						 gsp->pseudo == FALSE) {
						sfp_out->qual = 
							AddGBQual(sfp_out->qual, 
								"translation", protein_seq);
					}
				}
			}
			MemFree(protein_seq);
		}
		BSFree(byte_sp);
*/
		break;
	case SEQFEAT_RNA:
		rrp = sfp_in->data.value.ptrvalue;
		/* the following code was taken (almost) directly
			from Karl Sirotkin's code.					*/
		switch ( rrp -> type){ /* order of case n: matches tests in
                                is_RNA_type() of genasn.c in
                                GenBankConversion directory */
			case 2:
				break;
			case 255:
				break;
			case 3:
				if (rrp->ext.choice == 1) {
					if ((GBQualPresent("product", sfp_in->qual)) == FALSE) {
						sfp_out->qual = AddGBQual(sfp_out->qual, 
								"product", (CharPtr) rrp->ext.value.ptrvalue);
					}
				} else if (rrp->ext.choice == 0 ||
					rrp->ext.choice == 2) {
					DotRNAQuals(ajp, gbp, sfp_in, sfp_out,
							gp->nsp, gp->extra_loc, gp->extra_loc_cnt);
				}
				break;
			case 4:
				break;
			case 1:
				if (rrp->ext.choice == 1) {
					if ((GBQualPresent("product", sfp_in->qual)) == FALSE) {
						sfp_out->qual = AddGBQual(sfp_out->qual, 
								"product", (CharPtr) rrp->ext.value.ptrvalue);
					}
				} else if (rrp->ext.choice == 0 ||
					rrp->ext.choice == 2) {
					DotRNAQuals(ajp, gbp, sfp_in, sfp_out,
							gp->nsp, gp->extra_loc, gp->extra_loc_cnt);
				}
				break;
			case 5:
				break;
			case 6:
				break;
		}

		if (rrp && rrp->pseudo == TRUE) {
			if ((GBQualPresent("pseudo", sfp_in->qual)) == FALSE)
				sfp_out->qual = AddGBQual(sfp_out->qual, "pseudo", NULL);
		}

			
		break;
	case SEQFEAT_SEQ:	
		if ((xid=CheckXrefFeat(bsp, sfp_in)) != NULL) {
			ptr = &(temp[0]);
			SeqIdWrite(xid, printbuf, PRINTID_FASTA_SHORT, 40);
			sprintf(ptr, "Cross-reference: %s", printbuf);
			SaveNoteToCharPtrStack(nsp, NULL, ptr);
		}
		else
			retval = 0;
		break;
	case SEQFEAT_IMP:
		ifp_in = (ImpFeatPtr) sfp_in->data.value.ptrvalue;
		if (ifp_in->loc != NULL)
			ifp->loc = ifp_in->loc;
		if (StringCmp(ifp_in->key, "CDS") == 0) {
			if ((GBQualPresent("pseudo", sfp_in->qual)) == FALSE &&
				ajp->error_msgs == TRUE)
				ErrPostStr(SEV_INFO, ERR_FEATURE_non_pseudo, 
				    "ConvertToNAImpFeat: Non-pseudo ImpFeat CDS found");
			if ((GBQualPresent("translation", sfp_in->qual)) == TRUE &&
												ajp->mode == RELEASE_MODE) {
				if (ajp->error_msgs == TRUE) {
					ErrPostStr(SEV_ERROR, ERR_FEATURE_Dropped, 
				    "ImpFeat CDS with /translation found");
				}
				retval = -1;
			}
		}
		break;
	case SEQFEAT_REGION:
		tmp = MemNew(StringLen(sfp_in->data.value.ptrvalue) + 9);
		sprintf(tmp, "Region: %s", (CharPtr ) sfp_in->data.value.ptrvalue);
		sfp_out->qual = AddGBQual(sfp_out->qual, "note", tmp);
		tmp = MemFree(tmp);
		break;
	case SEQFEAT_SITE:
		AddSiteNoteQual(sfp_in, sfp_out);
		break;
	case SEQFEAT_RSITE:
		break;
	case SEQFEAT_COMMENT:
		if(ifp->key != NULL)
			MemFree(ifp->key);
		ifp->key = StringSave("misc_feature");
		break;
	case SEQFEAT_GENE:
		grp = (GeneRefPtr) sfp_in->data.value.ptrvalue;
		if (grp == NULL)
			break;
		syn=grp->syn;
		if (grp->locus ) {
			sfp_out->qual = AddGBQual(sfp_out->qual, "gene", grp->locus);
			was_gene = TRUE;
		} else if (syn != NULL) {
			sfp_out->qual = AddGBQual(sfp_out->qual, "gene", 
												syn->data.ptrvalue);
			syn=syn->next;
			was_gene = TRUE;
		}
		if (grp->desc ) {
			if (was_gene) {
				CpNoteToCharPtrStack(nsp, NULL, grp->desc);
			} else {
			/*	s = MemNew(StringLen(grp->desc) + 15);
				sprintf(s, "Description: %s", grp->desc);
				sfp_out->qual = AddGBQual(sfp_out->qual, "gene", s);*/
				sfp_out->qual = AddGBQual(sfp_out->qual, "gene", grp->desc);
			}
		}
		if (grp->allele ) {
			if ((GBQualPresent("allele", sfp_in->qual)) == FALSE)
				sfp_out->qual = AddGBQual(sfp_out->qual, "allele", grp->allele);
		}
		if (grp->maploc ) {
			if ((GBQualPresent("map", sfp_in->qual)) == FALSE)
				sfp_out->qual = AddGBQual(sfp_out->qual, "map", grp->maploc);
		}
		for (; syn; syn=syn->next) {
			CpNoteToCharPtrStack(nsp, NULL, syn->data.ptrvalue);
		}
		if (grp->pseudo == TRUE || sfp_in->pseudo) {
			if ((GBQualPresent("pseudo", sfp_in->qual)) == FALSE)
				sfp_out->qual = AddGBQual(sfp_out->qual, "pseudo", NULL);
		}
		GetDBXrefFromGene(grp, sfp_out);
		break;
	default:
		if (ajp->error_msgs == TRUE)
			ErrPostStr(SEV_WARNING, ERR_FEATURE_UnknownFeatureKey, 
				"Unimplemented type of gbqual in ConvertToNAImpFeat");
		retval = 0;
		break;
	}
	if (gsp->grp) {
		GetDBXrefFromGene(gsp->grp, sfp_out);
	}
	if (sfp_in->pseudo) {
		sfp_out->qual = AddGBQual(sfp_out->qual, "pseudo", NULL);
	}
	if (sfp_out->comment) {
		CpNoteToCharPtrStack(nsp, NULL, (CharPtr) sfp_out->comment);
		sfp_out->comment = NULL;
	}
	return retval;

}	/* ConvertToNAImpFeat */

/*****************************************************************************
*ValidateNAImpFeat
*
*	This code validates an ImpFeat using some functions from
*	the flat2asn parser.
*
*	If a feat is bad and can't be corrected, -1 is returned.
*
*****************************************************************************/

NLM_EXTERN Int2 ValidateNAImpFeat (SeqFeatPtr sfp)

{
	CharPtr key;
	ImpFeatPtr ifp;
	Int2 index, retval=0, status=0;

	if (sfp->data.choice != SEQFEAT_IMP) {
		return -1;
	} else {

		ifp = sfp->data.value.ptrvalue;
		key = StringSave(ifp->key);
		index = GBFeatKeyNameValid(&key, ASN2FF_SHOW_ERROR_MSG);
		if (StringCmp(key, ifp->key) != 0) {
			ifp->key = key;
		} else {
			MemFree(key);
		}
	
		if (index == -1) {
			retval = -2;
		} else {
			status = GBFeatKeyQualValid(sfp->cit, index, &sfp->qual, 
						ASN2FF_SHOW_ERROR_MSG, ASN2FF_VALIDATE_FEATURES);
#ifdef ASN2GNBK_PRINT_UNKNOWN_ORG
			if (index == 46 && status == GB_FEAT_ERR_NONE) {
				status = GBFeatKeyQualValid(sfp->cit, index, &sfp->qual, 
							ASN2FF_SHOW_ERROR_MSG, TRUE);
			}
#endif
			if (status == GB_FEAT_ERR_NONE) {
				retval = 1;
			} else if (status == GB_FEAT_ERR_REPAIRABLE) {
				retval = 0;
			} else if (status == GB_FEAT_ERR_DROP) {
				retval = -1;
			}
		}

	}

	return retval;
}	/* ValidateNAImpFeat */

/*****************************************************************************
*ValidateAAImpFeat
*
*	This code will validate an ImpFeat using some functions from
*	the flat2asn parser.  Right now it just checks to see that the
*	sfp is an ImpFeat and checks for a partial qualifier.
*
*	If a feat is bad and can't be corrected, -1 is returned.
*
*****************************************************************************/

NLM_EXTERN Int2 ValidateAAImpFeat (SeqFeatPtr sfp, Boolean use_product)

{

	if (sfp->data.choice != SEQFEAT_IMP)
		return -1;

	LookForPartialImpFeat(sfp, use_product);

	return 0;
}	/* ValidateAAImpFeat */


/*****************************************************************************
*void PrepareSourceFeatQuals(SeqFeatPtr sfp_in, SeqFeatPtr sfp_out, GBEntryPtr gbp, Boolean add_modifs)
*
*Normally called from PrintSourceFeat, collects all notes etc. together.
*Note: sfp_out may already have quals when it comes here, they should not
*be deleted!
* 	add_modifs: allows the addition of modifs to be specified, don't add
*		modifs if the source feature is a ImpFeat.
*
*For many cases there is no sfp_in, so that must be checked for.
*****************************************************************************/

NLM_EXTERN void PrepareSourceFeatQuals(SeqFeatPtr sfp_in, SeqFeatPtr sfp_out, GBEntryPtr gbp, Boolean add_modif)

{
	CharPtr note=NULL;
	GBQualPtr qual1;
	NoteStructPtr nsp=NULL;

	if (gbp->feat) {
		nsp=gbp->feat->source_notes;
	}
	if (sfp_in) {
		for (qual1=sfp_in->qual; qual1; qual1=qual1->next) {
			if (StringCmp(qual1->qual, "note") == 0)
				CpNoteToCharPtrStack(nsp, NULL, qual1->val);
			else 	
				sfp_out->qual = 
					AddGBQual(sfp_out->qual, qual1->qual, qual1->val);
		}
	}
/* not used in new style  */
	if (add_modif == TRUE)
		sfp_out->qual = AddModifsToGBQual(gbp, sfp_out->qual);
/*---------------------    tatiana */
	if (sfp_in && sfp_in->comment) {
		CpNoteToCharPtrStack(nsp, NULL, (CharPtr) sfp_in->comment);
	}

	if (nsp && nsp->note[0]) {
		note = ComposeNoteFromNoteStruct(nsp, NULL);
		if (note) {
			sfp_out->qual = AddGBQual(sfp_out->qual, "note", note);
			note = MemFree(note);
		}
	}
	if (sfp_in && sfp_in->cit) {
		if (ASN2FF_SHOW_ERROR_MSG)
			ErrPostStr(SEV_WARNING, 0, 0, 
				"Unwanted /citation on 'source' feature will be dropped");
	}

	return;
}


/*************************************************************************
*AddProteinQuals
*
*************************************************************************/

NLM_EXTERN void AddProteinQuals (SeqFeatPtr sfp, SeqFeatPtr sfp_out, NoteStructPtr nsp)

{
	ProtRefPtr prp=sfp->data.value.ptrvalue;
	ValNodePtr vnp;

	if (prp->name != NULL) {
		for (vnp=prp->name; vnp; vnp=vnp->next)
			if (GBQualPresent("product", sfp_out->qual) == FALSE)
				sfp_out->qual = 
				     AddGBQual(sfp_out->qual, "product", vnp->data.ptrvalue);
			else 
				CpNoteToCharPtrStack(nsp, NULL, vnp->data.ptrvalue);
	}
	if (prp->desc) {
		sfp_out->qual = 
		     AddGBQual(sfp_out->qual, "name", prp->desc);
	}

	for (vnp=prp->ec; vnp; vnp=vnp->next)
		if ((CheckForQual(sfp_out->qual, "EC_number", vnp->data.ptrvalue)) == 0)
			sfp_out->qual = 
		    		AddGBQual(sfp_out->qual, "EC_number", vnp->data.ptrvalue);

	return;
}

/*______________________________________________________________________
**
**	This code is not currently used.
**	I do not remove this piece of code, just comment it out.
**	-- Dmitri Lukyanov
*/
#if 0

static GBQualPtr RemoveQual(GBQualPtr head, GBQualPtr x)
{
	GBQualPtr	v, p;
	
	if (head == NULL) {
		return NULL;
	}
	if (x == head) {
		head = x->next;
		x->next = NULL;
		GBQualFree(x);
		return head;
	}
	for (v = head; v != NULL && v != x; v = v->next) {
		p = v;
	}
	if (v != NULL) {
		p->next = x->next;
		x->next = NULL;
		GBQualFree(x);
	}
	return head;
}

#endif
/*______________________________________________________________________
*/

static void Add_gene_id (GeneStructPtr gsp, SeqFeatPtr sfp_out)
{
	ImpFeatPtr ifp;
	GeneRefPtr grp;
	ValNodePtr vnp;
	Char val[40];
	
	if ((grp = gsp->grp) == NULL)
		return;
	ifp = sfp_out->data.value.ptrvalue;
	if (StringCmp(ifp->key, "CDS") != 0) {
		return;
	}
	if ((vnp = grp->syn) == NULL)  /* no synonyms */
		return;
	sprintf(val, "GeneID:%s", vnp->data.ptrvalue);
	sfp_out->qual = AddGBQual(sfp_out->qual, "db_xref", val);
}

/****************************************************************************
*	Composes the GBQuals for sfp_out using the information in the
*	GeneStructPtr (gsp), and then the quals already on sfp_out.
*
* 	use only info from GeneStruct throw away the quals gene and map if they
*	different /tatiana  07.11.95/
*	do not add /map to the features other than 'gene' /08-29-97/
*	sfp_out: SEQFEAT_IMP
* 	map /citation added by Tatiana
**************************************************************************/
NLM_EXTERN void ComposeGBQuals (Asn2ffJobPtr ajp, SeqFeatPtr sfp_out, GBEntryPtr gbp, SortStructPtr p, Boolean note_pseudo)
{
	Char temp[65];
	Char buffer[10];
	CharPtr ascii, start, note=NULL, ptr=NULL, tmp;
	GBQualPtr gbqp=NULL, qual1, qnext;
	GeneStructPtr gsp;
	Int2 int_index, status;
	NoteStructPtr nsp;
	PubStructPtr psp;
	SeqFeatPtr sfp = NULL;
	Int2 ascii_len, l;
	ValNodePtr vnp, vnp1;
	ValNodePtr pub, pubq, pubset;
	ImpFeatPtr ifp;
	BioseqPtr bsp;
	Boolean is_contig = FALSE, is_NC = FALSE, is_NG = FALSE;
	SeqIdPtr sid;
	TextSeqIdPtr tsip;

	if (gbp == NULL || gbp->feat == NULL || p == NULL) {
		return;
	}
	bsp = gbp->bsp;
	for (sid=bsp->id; sid; sid=sid->next) {
		if (sid->choice == SEQID_OTHER) {
			tsip = (TextSeqIdPtr) sid->data.ptrvalue;
			if (StringNCmp(tsip->accession, "NT", 2) == 0) {
				is_contig = TRUE;
			}
			if (StringNCmp(tsip->accession, "NC", 2) == 0 
					|| StringNCmp(tsip->accession, "NP", 2) == 0) {
				is_NC = TRUE;
			}
			if (StringNCmp(tsip->accession, "NG", 2) == 0) {
				is_NG = TRUE;
			}
		}
	}
	gsp=p->gsp;
	nsp = p->nsp;
	if ((sfp=p->sfp) == NULL) {
		GatherItemWithLock(p->entityID, p->itemID, p->itemtype, 
								&sfp, find_item);
	}
	if (gsp) {
		if (gsp->gene) {
		/*	delete_qual(&(sfp_out->qual), "gene"); */
			for (vnp=gsp->gene; vnp; vnp=vnp->next)
			{
				ascii_len = Sgml2AsciiLen(vnp->data.ptrvalue);
				start = ascii = MemNew((size_t) (10+ascii_len));
				ascii = Sgml2Ascii(vnp->data.ptrvalue, ascii, ascii_len+1);
				if ((GBQualPresent("gene", gbqp)) == FALSE) {
					if ((GBQualPresent("gene", sfp_out->qual)) == FALSE) {
						gbqp=AddGBQual(gbqp, "gene", start);
					} 
				}
				start = MemFree(start);
			}
		}
		if (gsp->product) {
			for (vnp=gsp->product; vnp; vnp=vnp->next)
			{
				if (GBQualPresent("product", gbqp) == FALSE &&
					GBQualPresent("product", sfp_out->qual) == FALSE)
						sfp_out->qual = AddGBQual(sfp_out->qual, "product",
														 vnp->data.ptrvalue);
				else 
					CpNoteToCharPtrStack(nsp, NULL, vnp->data.ptrvalue);
			}
		}
		if (gsp->standard_name) {
			for (vnp=gsp->standard_name; vnp; vnp=vnp->next)
			{
				if ((CheckForQual(sfp_out->qual, "standard_name",
											 vnp->data.ptrvalue)) == 0) {
					gbqp=AddGBQual(gbqp, "standard_name", vnp->data.ptrvalue);
				}
			}
		}
		if (ajp->show_gene == TRUE) {
			ifp = sfp_out->data.value.ptrvalue;
			if (StringCmp(ifp->key, "gene") == 0) {
				if (gsp->map[0]) {
					gbqp = AddGBQual(gbqp, "map", gsp->map[0]);
				}
			}
		} else {
			if (gsp->map[0]) {
				gbqp = AddGBQual(gbqp, "map", gsp->map[0]);
			}
		}
		for (vnp=gsp->ECNum; vnp; vnp=vnp->next) {
			if ((CheckForQual(sfp_out->qual, "EC_number",
									vnp->data.ptrvalue)) == 0) {
				gbqp=AddGBQual(gbqp, "EC_number", vnp->data.ptrvalue);
			}
		}
		for (vnp=gsp->activity; vnp; vnp=vnp->next) {
			if ((CheckForQual(sfp_out->qual, "function",
									vnp->data.ptrvalue)) == 0) {
				gbqp=AddGBQual(gbqp, "function", vnp->data.ptrvalue);
			}
		}
		if (gsp->pseudo == TRUE) {
			if (note_pseudo == TRUE) {
					CpNoteToCharPtrStack(nsp, NULL, "pseudogene");
			} else if (GBQualPresent("pseudo", gbqp) == FALSE &&
					GBQualPresent("pseudo", sfp_out->qual) == FALSE) {
				gbqp = AddGBQual(gbqp, "pseudo", NULL);
			}
		}
	}
/* Add Experimental note */
	if (sfp != NULL && sfp->data.choice == SEQFEAT_CDREGION)
	{
		ptr = &(temp[0]);
		status = MakeGBSelectNote(ptr, sfp);
		if (status > 0)
			SaveNoteToCharPtrStack(nsp, NULL, ptr);
		ptr=NULL;
/* gene synonym appears as db-xref
		if (is_NC) {
			Add_gene_id(gsp, sfp_out); 
		}
*/
	}
	if (nsp && nsp->note[0])
	{
		note = ComposeNoteFromNoteStruct(nsp, gsp);
		if (note)
		{
			gbqp = AddGBQual(gbqp, "note", note);
			note = MemFree(note);
		}
	}
	if (ajp->mode != DIRSUB_MODE) {
		AddPID(ajp, sfp_out, (Boolean) (is_contig || is_NG || is_NC));
	}
	if (is_contig || is_NG) {
		if (sfp != NULL && sfp->data.choice == SEQFEAT_RNA) {
			Add_trid(ajp, sfp_out); 
		}
	}
	Add_dbxref(ajp, sfp_out, sfp); 
	vnp = gbp->Pub;
	if (sfp && sfp->cit) {
		buffer[0] = '\0';
		pubset = sfp->cit;
		for (pubq = pubset->data.ptrvalue; pubq; pubq = pubq->next) {
			if (pubq->choice == PUB_Equiv) {
				pub = pubq->data.ptrvalue;
				for (; pub != NULL; pub = pub->next) {
					for (vnp1=vnp; vnp1; vnp1=vnp1->next) {
						psp = vnp1->data.ptrvalue;
						if (PubLabelMatch(psp->pub, pub) == 0) {
							sprintf(buffer, "[%ld]", (long) (psp->number));
							 gbqp = AddGBQual(gbqp, "citation", buffer);
							break;
						}
					}
				}
			} else {
				pub = pubq;
				for (vnp1=vnp; vnp1; vnp1=vnp1->next) {
					psp = vnp1->data.ptrvalue;
					if (PubLabelMatch(psp->pub, pub) == 0) {
						sprintf(buffer, "[%ld]", (long) (psp->number));
						 gbqp = AddGBQual(gbqp, "citation", buffer);
						break;
					}
				}
			}
		}
/************** old algorithm for pub matching ****************/
		if (buffer[0] == '\0') {
			for (vnp1=vnp; vnp1; vnp1=vnp1->next)
			{
				psp = vnp1->data.ptrvalue;
				for (int_index=0; int_index<psp->pubcount; int_index++)
					if (sfp == psp->pubfeat[int_index])
					{
						sprintf(buffer, "[%ld]", (long) (psp->number));
						gbqp = AddGBQual(gbqp, "citation", buffer);
					}
			}
		}
	}
	if (gbqp)	/* any gene or note related quals added above? */
	{
		for (qual1=gbqp; qual1->next; qual1=qual1->next)
			;
		qual1->next = sfp_out->qual;
		sfp_out->qual = gbqp;
	}
/* check for the qual gdb_xref */
	for (qual1 = sfp_out->qual; qual1; qual1 = qnext) {
		qnext = qual1->next;
		if (StringCmp(qual1->qual, "gdb_xref") == 0) {
			qual1->qual = StringSave("db_xref");
			l = StringLen(qual1->val);
			tmp = MemNew(l + 5);
			sprintf(tmp, "GDB:%s", qual1->val);
			qual1->val = StringSave(tmp);
			MemFree(tmp);
		}
		if (ajp->show_gene == FALSE) {
/* change qual 'replace' to the old style location operator */
/* changed December 1996 release 100.0 */
		/*	if (StringCmp(qual1->qual, "replace") == 0) {
				ifp = sfp_out->data.value.ptrvalue;
				loc = FlatLoc(gbp->bsp, sfp->location);
				l = StringLen(qual1->val) + StringLen(loc);
				tmp = MemNew(l + 15);
				sprintf(tmp, "replace(%s,\"%s\")", loc, qual1->val);
				MemFree(loc);
				ifp->loc = tmp;
				sfp_out->qual = RemoveQual(sfp_out->qual, qual1);
			}
				*/
		}
	}
	return;
}	/* ComposeGBQuals */

static CharPtr  tmp_save(CharPtr str) 
/* deletes spaces from the begining and the end and returns Nlm_StringSave */		           {
	CharPtr s, ss;

	if (str == NULL) {
		return NULL;
	}
	for (; isspace(*str) || *str == ','; str++) continue;
	for (s = str; *s != '\0'; s++) {
		if (*s == '\n') {
			for (ss = s+1; isspace(*ss); ss++) continue;
			*s = ' ';
			strcpy(s+1, ss);
		}
	}
	for (s=str+StringLen(str)-1; s >= str && (*s == ' ' || *s == ';' ||
		 *s == ',' || *s == '.' || *s == '\"' || *s == '\t'); s--) {
		*s = '\0';
	}  

	if (*str == '\0') { 
	    return NULL;
	} else {
	    return Nlm_StringSave(str);
	}
}
static Int2 NoteCmp(CharPtr n1, CharPtr n2)
{
	CharPtr s1, s2;
	Int2 ret = 1;
	
	if (n1 == NULL || n2 == NULL)
		return ret;
	s1 = tmp_save(n1);
	s2 = tmp_save(n2);
	if (StringStr(s1, s2) != NULL) 
		ret = 0;  /*duplicated */
	MemFree(s1);
	MemFree(s2);
	
	return ret;
}

/****************************************************************************
* CharPtr ComposeNoteFromNoteStruct (NoteStructPtr nsp, GeneStrunctPtr gsp)
*
*	This function composes a "/note" for a SeqFeatPtr from the information
*	in the GeneStructPtr (gsp).
*	The first "for" loop initializes the first CharPtr and a check 
*	is done that the information in gsp->note is *not* redundant.  If 
*	it is not, first gsp->note_annot is copied onto a CharPtr (this 
*	field contains words describing the origin of the info in note, i.e., 
*	"Description"); then the actual note is copied onto the CharPtr.  
*	The second "for" loop does the same checking as the first and the
*	concatenation of more "note" strings is performed.
*
*n.b.: the caller is responsible for deallocating the final returned "note".
***************************************************************************/
NLM_EXTERN CharPtr ComposeNoteFromNoteStruct (NoteStructPtr nsp, GeneStructPtr gsp)

{
	Boolean status;
	CharPtr note1=NULL, note2, note3;
	Int2 index, index1, index2, len;

	for (index=0; index<nsp->note_index; index++) {
		if (gsp) {
			if (CompareStringWithGsp(gsp, nsp->note[index]) != 0) {
				if (nsp->note_annot[index])
					note1 = Cat2Strings(nsp->note_annot[index], nsp->note[index], " ", 0);
				else
					note1 = StringSave(nsp->note[index]);
				len = CheckForExtraChars(note1);
				if (len == 0)
					note1 = MemFree(note1);
				else
					break;
			}
		} else {
			if (nsp->note_annot[index])
				note1 = Cat2Strings(nsp->note_annot[index], nsp->note[index], " ", 0);
			else
				note1 = StringSave(nsp->note[index]);
			len = CheckForExtraChars(note1);
			if (len == 0)
				note1 = MemFree(note1);
			else
				break;
		}
	}
	index++;

	for (index1=index; index1<nsp->note_index; index1++)
	{
		status = TRUE;
		note2 = nsp->note[index1];
		if (gsp && CompareStringWithGsp(gsp, note2) == 0)
			continue;

		for (index2=0; index2<index1; index2++) {
			if (gsp)
				if (GeneStringCmp(note2, nsp->note[index2]) == 0)
					status = FALSE;
		}
		if (status == TRUE) {
			if (nsp->note_annot[index1])
				note2 = Cat2Strings(nsp->note_annot[index1], nsp->note[index1], " ", 0);
			else /* rewrite to not always allocate note2 if no annot?????*/
				note2 = StringSave(nsp->note[index1]);
			len = CheckForExtraChars(note1);
			if (NoteCmp(note1, note2) == 0) {
				len = 0;
			}
			if (len > 0) {
				if (note1[len-1] == '.') {
			        	note3 = Cat2Strings(note1, note2, "  ", -1);
				} else {
					note3 = Cat2Strings(note1, note2, "; ", -1);
				}
				note1 = MemFree(note1);
				note2 = MemFree(note2);
				note1 = note3;
				note3 = NULL;
			} else {
				if (note2) {
					note2 = MemFree(note2);
				}
			}
		}
	}

	return note1;
}	/* ComposeNoteFromNoteStruct */

/*************************************************************************
*static Int2 CheckForExtraChars(CharPtr note)
*
*	Check for spaces or semi-colons on the ends of notes.
************************************************************************/

static Int2 CheckForExtraChars(CharPtr note)

{
	Int2 len=0;

	if (note != NULL)
	{
		len = StringLen(note);
		while (len > 0)
		{
			if (note[len-1] == ' ' || note[len-1] == ';')
				note[len-1] = '\0';
			else
				break;
			len--;
		}
	}

	return len;

}	/* CheckForExtraChars */

NLM_EXTERN void Add_trid (Asn2ffJobPtr ajp, SeqFeatPtr sfp_out)

{
	ImpFeatPtr ifp;
	Int4 gi = -1;
	SeqIdPtr sip, newid=NULL;
	ValNodePtr product;
	Char buf[MAX_ACCESSION_LEN+5];
	
	ifp = sfp_out->data.value.ptrvalue;
	if (StringCmp(ifp->key, "mRNA") != 0) {
		return;
	}
	product = sfp_out->product; 
	if (product == NULL) {
		return;
	}
	sip = GetProductSeqId(product);
	if (sip->choice == SEQID_GI) {
		if ((newid = GetSeqIdForGI(sip->data.intvalue)) != NULL) {
			SeqIdWrite(newid, buf, PRINTID_TEXTID_ACC_VER, MAX_ACCESSION_LEN+1);
		} else {
			sprintf(buf, "%ld", sip->data.intvalue);
		}
	} else {	
		SeqIdWrite(sip, buf, PRINTID_TEXTID_ACC_VER, MAX_ACCESSION_LEN+1);
	}
	sfp_out->qual = AddGBQual(sfp_out->qual, "transcript_id", buf);
}

/*************************************************************************
*	sfp_out: synthetic SeqFeatPtr of type ImpFeat for use in printing.
*
*	This function puts the GI number on a SeqFeatPtr /db_xref of type CDS.
*	Checking is first done to see if this sfp is indeed a CDS, then
*	the PID number is gotten from the product SeqId
*****************************************************************************/

NLM_EXTERN void AddPID (Asn2ffJobPtr ajp, SeqFeatPtr sfp_out, Boolean is_NTorNG)

{
	ImpFeatPtr ifp;
	Int4 gi = -1;
	SeqIdPtr sip, new_id=NULL;
	ValNodePtr product, vnp;
	BioseqPtr p_bsp = NULL;
	DbtagPtr db;
	Char val[20];
	Char buf[MAX_ACCESSION_LEN+1];
	
	ifp = sfp_out->data.value.ptrvalue;
	if (StringCmp(ifp->key, "CDS") != 0) {
		return;
	}
	product = sfp_out->product; 
	if (product == NULL) {
		return;
	}
	sip = GetProductSeqId(product);
	if (sip) {	/* Get protein bsp	*/
		if (sip->choice == SEQID_GI && is_NTorNG) {
			if ((new_id = GetSeqIdForGI(sip->data.intvalue)) != NULL) {
				SeqIdWrite(new_id, buf, PRINTID_TEXTID_ACC_VER, MAX_ACCESSION_LEN+1);
				SeqIdFree(new_id); /*** need to free it !!! (EY) ***/
			} else {
				sprintf(buf, "%ld", sip->data.intvalue);
			}
			sfp_out->qual = AddGBQual(sfp_out->qual, "protein_id", buf);
		} else if ((p_bsp = BioseqFind(sip)) != NULL) {
			new_id = GetSeqIdChoice(p_bsp->id);
			if (ajp->forgbrel && new_id == NULL) {
				ErrPostStr(SEV_ERROR, ERR_ACCESSION_NoAccessNum, "");
			} else if (new_id) {
				SeqIdWrite(new_id, buf, PRINTID_TEXTID_ACC_VER,
														MAX_ACCESSION_LEN+1);
				sfp_out->qual = AddGBQual(sfp_out->qual, "protein_id", buf);
			}
		}
	}
	if (p_bsp == NULL) {
		gi = GetGINumFromSip(sip);
		if (gi != -1) {
			if (ajp->show_gi) {
				val[0] = '\0';
				sprintf(val, "PID:g%ld", (long) gi);
				if (val[0] != '\0') {
					sfp_out->qual = AddGBQual(sfp_out->qual, "db_xref", val);
				}
			}
			if (ajp->show_version) {
				val[0] = '\0';
				sprintf(val, "GI:%ld", (long) gi);
				sfp_out->qual = AddGBQual(sfp_out->qual, "db_xref", val);
			}
		}
		return;
	}
	for (vnp=p_bsp->id; vnp; vnp=vnp->next) {
		if (vnp->choice == SEQID_GENERAL) {
			db = vnp->data.ptrvalue;
			if (db == NULL) {
				continue;
			}
			val[0] = '\0';
			if (StringNCmp(db->db, "PIDe", 4) == 0) {
					sprintf(val, "PID:e%ld", (long) db->tag->id);
					gi = db->tag->id;
			} else if (StringNCmp(db->db, "PIDd", 4) == 0) {
					sprintf(val, "PID:d%ld", (long) db->tag->id);
					gi = db->tag->id;
			} else if (StringNCmp(db->db, "PID", 3) == 0) {
				if (db->tag && db->tag->str) {
					sprintf(val, "%s:%s", db->db, db->tag->str);
					gi = atoi((db->tag->str)+1);
				}
			}
			if (ajp->show_gi && val[0] != '\0') {
				sfp_out->qual = AddGBQual(sfp_out->qual, "db_xref", val);
			}
			/*if (ajp->show_version) {
				val[0] = '\0';
				sprintf(val, "GI:%ld", (long) gi);
				sfp_out->qual = AddGBQual(sfp_out->qual, "db_xref", val);
			}*/
		}
		if (vnp->choice == SEQID_GI) {
			if (ajp->show_gi) {
				val[0] = '\0';
				sprintf(val, "PID:g%ld", (long) vnp->data.intvalue);
				sfp_out->qual = AddGBQual(sfp_out->qual, "db_xref", val);
			}
			if (ajp->show_version) {
				val[0] = '\0';
				sprintf(val, "GI:%ld", (long) vnp->data.intvalue);
				sfp_out->qual = AddGBQual(sfp_out->qual, "db_xref", val);
			}
		}
	}
	return;
}	/* AddPID */

/***************************************************************************
*Int2 MakeGBSelectNote (CharPtr ptr, SeqFeatPtr sfp)
*
*Adds note to CDS GenBankSelect 
***************************************************************************/
NLM_EXTERN Int2 MakeGBSelectNote (CharPtr ptr, SeqFeatPtr sfp)

{
	Boolean found_select=FALSE, found_match=FALSE;
	CharPtr acc=NULL;
	Int2 number = -1;
	ObjectIdPtr oip=NULL, type;
	UserFieldPtr ufp;
	UserObjectPtr uop=NULL;

	if (sfp && (uop=sfp->ext) != NULL)
	{
		if (uop->_class && (type=uop->type) != NULL)
		{
			if (StringCmp(uop->_class, "GB-Select") == 0)
				found_select = TRUE;
			if (type->str) 
				if (StringCmp(type->str, "SPmatch") == 0)
					found_match = TRUE;
			if (found_match && found_select)
			{
				for (ufp=uop->data; ufp; ufp=ufp->next)
				{
					oip = ufp->label;
					if (oip->id == 2)
					{
						if (ufp->choice == 1)
							acc = ufp->data.ptrvalue;
					}
					else if (oip->id == 3)
					{
						if (ufp->choice == 2)
						{
							number = (Int2) (ufp->data.intvalue);
						}
					}
						
				}
				if (number == 1)
					sprintf(ptr, 
						"Identical to Swiss-Prot Accession Number %s", acc);
				else if (number == 2 || number == 3)
					sprintf(ptr, 
						"Similar to Swiss-Prot Accession Number %s", acc);
			}
		}
	}
	return number;
}

NLM_EXTERN Boolean get_prot_feats (GatherContextPtr gcp)
{
	BioseqPtr	bsp;
	OrganizeProtPtr opp;
	SeqFeatPtr sfp;
	Boolean temp = FALSE;

	opp = gcp->userdata;

	switch (gcp->thistype)
	{
		case OBJ_SEQFEAT:
			sfp = (SeqFeatPtr) (gcp->thisitem);
			if (sfp->data.choice == SEQFEAT_PROT || 
					sfp->data.choice == SEQFEAT_REGION || 
					sfp->data.choice == SEQFEAT_BOND || 
						sfp->data.choice == SEQFEAT_SITE) {
				bsp = BioseqFindCore(SeqLocId(sfp->location));
		    	if (gcp->tempload == TRUE) {
		    		temp = TRUE;
		    	}
		    	opp->list = EnlargeSortList(opp->list, opp->size);
				opp->size = StoreFeatTemp(opp->list, sfp, opp->size, bsp, NULL,
					gcp->entityID, gcp->itemID, gcp->thistype,
						gcp->new_loc, NULL, 0, temp);
			}
			break;
		default:
			break;
	}
	return TRUE;
}

/********************************************************************
*	Int2 CompareStringWithGsp (GeneStructPtr gsp, CharPtr string)
*
*	gsp: GeneStructPtr containing the gene information,
*	gene->synonym in is store in gsp->gene with choice 1 (GetGeneRefInfo)
*	it is not compared to note string 
*
*	string: a CharPtr with (possibly) relevant gene information
*		(i.e., gene name, allele, product etc.).
*
*	A comparison is made between string and the information already
*	stored in the gsp.  Following the convention for StringCmp,
*	"0" is returned if a match is found, otherwise "1" is returned.
*	At present (2/7/94) GeneStringCmp is a #define for StringCmp.
************************************************************************/

NLM_EXTERN Int2 CompareStringWithGsp (GeneStructPtr gsp, CharPtr string)

{
	CharPtr ascii, start;
	Int2 ascii_len;
	ValNodePtr vnp;	

	for (vnp=gsp->gene; vnp; vnp=vnp->next)
	{
		if (vnp->choice == 1) {
			continue;
		}
		ascii_len = Sgml2AsciiLen(vnp->data.ptrvalue);
		start = ascii = MemNew((size_t) (10+ascii_len));
		ascii = Sgml2Ascii(vnp->data.ptrvalue, ascii, ascii_len+1);
		if (GeneStringCmp(start, string) == 0)
		{
			start = MemFree(start);
			return 0;
		}
		start = MemFree(start);
	}
	vnp=gsp->product;
	if (vnp != NULL)
	{
		if (GeneStringCmp(vnp->data.ptrvalue, string) == 0)
			return 0;
	}
	for (vnp=gsp->standard_name; vnp; vnp=vnp->next)
	{
		if (GeneStringCmp(vnp->data.ptrvalue, string) == 0)
			return 0;
	}
	if (gsp->map[0] && GeneStringCmp(gsp->map[0], string) == 0)
		return 0;
	if (gsp->ECNum)
	for (vnp=gsp->ECNum; vnp; vnp=vnp->next)
	{
		if (GeneStringCmp(vnp->data.ptrvalue, string) == 0)
			return 0;
	}

	return 1;
}	/* CompareStringWithGsp */

NLM_EXTERN void GetDBXrefFromGene (GeneRefPtr grp, SeqFeatPtr sfp)

{
	CharPtr dbase;
	DbtagPtr dbtp;
	ValNodePtr tmp;
	Char buffer[50];
	
	if (grp == NULL) {
		return;
	}
	for (tmp = grp->db; tmp != NULL; tmp=tmp->next) {
	    dbtp = tmp->data.ptrvalue;
	    if (dbtp && dbtp->db && dbtp->tag) {
			dbase = MemNew(StringLen(dbtp->db) + 3);
			sprintf(dbase, "%s:", dbtp->db);
			if (dbtp->tag->str) {
				sprintf(buffer, "%s%s", dbase, dbtp->tag->str);
				sfp->qual = AddGBQual(sfp->qual, "db_xref", buffer);
			} else if (dbtp->tag->id) {
				sprintf(buffer, "%s%ld", dbase, (long) dbtp->tag->id);
				sfp->qual = AddGBQual(sfp->qual, "db_xref", buffer);
			}
			MemFree(dbase);
	    }
	}

	return;
}

/****************************************************************************
*	void GetProtRefInfo (GeneStructPtr gsp, NoteStructPtr nsp, ProtRefPtr prp)
*
*	gsp: GeneStructPtr containing gene information
*	prp: ProtRefPtr from a sfp of type protein or a sfp xref.
*
*	If fields are empty on the gsp, and the relevant information
*	is given by the prp, that field is filled on the gsp
****************************************************************************/
NLM_EXTERN void GetProtRefInfo (Uint1 format, GeneStructPtr gsp, NoteStructPtr nsp, ProtRefPtr prp)
{
	ValNodePtr tmp, vnp;

	if (prp == NULL) {
		return;
	}
	for (vnp=prp->name; vnp; vnp=vnp->next) { 
		tmp = ValNodeNew(NULL);
		tmp->data.ptrvalue = StringSave(vnp->data.ptrvalue);
		gsp->product = tie_next(gsp->product, tmp);
	}
	for (vnp=prp->ec; vnp; vnp=vnp->next) {
		tmp = ValNodeNew(NULL);
		tmp->data.ptrvalue = StringSave(vnp->data.ptrvalue);
		gsp->ECNum = tie_next(gsp->ECNum, tmp);
	}
	for (vnp=prp->activity; vnp; vnp=vnp->next) {
		tmp = ValNodeNew(NULL);
		tmp->data.ptrvalue = StringSave(vnp->data.ptrvalue);
		gsp->activity = tie_next(gsp->activity, tmp);
	}
	if (format != GENPEPT_FMT) {
		if (prp->desc) {
			SaveNoteToCharPtrStack(nsp, NULL, prp->desc);
		}
	}
	return;
}

/****************************************************************************
*
*	sfp: SeqFeatPtr for CDS
*	nsp: NoteStructPtr 
*
* Used to get comments from the Protein for use in a CDS /note.
*
* Take the main protein ONLY (not sig_peptide mat_peptide)
*
* Will find the Protein Pubs, as they are needed and (presumably) haven't
* been found yet, so as to save "upfront" time when the formatter is 
* running in Entrez.
****************************************************************************/
static void GetProtRefComment (SeqFeatPtr sfp, BioseqPtr bsp, Asn2ffJobPtr ajp, OrganizeProtPtr opp, NoteStructPtr nsp, Uint1 method)
{
	Boolean first_done=FALSE, protein=FALSE;
	CharPtr ptr = NULL, string=NULL, string1=NULL, newstring=NULL, temp, s;
	CharPtr conflict_msg_no_protein="Coding region translates with internal stops";
/*	CharPtr except_msg_no_protein="Coding region translates with internal stops for reasons explained in citation. "; -- except_msg_no_protein UNUSED */
	CharPtr conflict_msg="Protein sequence is in conflict with the conceptual translation";
/*	CharPtr except_msg="Protein sequence differs from the conceptual translation for reasons explained in citation. "; -- except_msg UNUSED */
	CdRegionPtr cdr=NULL;
	Int2 total=0, i;
	PubdescPtr pdp;
	PubStructPtr psp;
	SeqFeatPtr sfp_local=NULL;
	ValNodePtr descr, vnp, vnp1, vnp1next, product;
	MolInfoPtr mfp;
	GatherScope gs;
	SeqLocPtr slp;
	ProtRefPtr prot_local;
	SeqMgrFeatContext fcontext;
	SeqMgrDescContext dcontext;
	GatherContext gc;
	SeqFeatPtr psfp;
	ValNodePtr psdp;
	ObjMgrDataPtr omdp;
	SeqSubmitPtr ssp;
	SubmitBlockPtr sbp;
	CharPtr prefix = "";

	if (ajp->useSeqMgrIndexes) {
		sfp_local = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_PROT, 0, &fcontext);
		while (sfp_local != NULL) {
			prot_local = sfp_local->data.value.ptrvalue;
			if (prot_local->processed <= 1) {
				if (first_done) {
					if (StringLen(sfp_local->comment)) {
						string1 = CheckEndPunctuation(sfp_local->comment, '\0');
						if (StringCmp(string, string1) != 0) {
							newstring = Cat2Strings(string, string1, "; ", 0);
							string = MemFree(string);
							string = newstring;
						}
						string1 = MemFree(string1);
					}
				} else {
					if (StringLen(sfp_local->comment)) {
						string = CheckEndPunctuation(sfp_local->comment, '\0');
						first_done = TRUE;
					}
				}
			}
			sfp_local = SeqMgrGetNextFeature (bsp, sfp_local, SEQFEAT_PROT, 0, &fcontext);
		}
	} else if (opp != NULL) {
		for (i = 0; i < opp->size; i++) {
			if ((sfp_local = opp->list[i].sfp) == NULL) {
				continue;
			}
			if (sfp_local->data.choice != SEQFEAT_PROT) {
				continue;
			}
			prot_local = sfp_local->data.value.ptrvalue;
			if (prot_local->processed > 1) {
				continue;
			}
			if (first_done) {
				if (StringLen(sfp_local->comment)) {
					string1 = CheckEndPunctuation(sfp_local->comment, '\0');
					if (StringCmp(string, string1) != 0) {
						newstring = Cat2Strings(string, string1, "; ", 0);
						string = MemFree(string);
						string = newstring;
					}
					string1 = MemFree(string1);
				}
			} else {
				if (StringLen(sfp_local->comment)) {
					string = CheckEndPunctuation(sfp_local->comment, '\0');
					first_done = TRUE;
				}
			}
		}
	}

	if (bsp && (descr=bsp->descr) != NULL) {
		for (vnp=descr; vnp; vnp=vnp->next) {
			if (vnp->choice == Seq_descr_comment) {
				if (first_done) {
					if (StringLen(vnp->data.ptrvalue)) {
						string1 = CheckEndPunctuation(vnp->data.ptrvalue, '\0');
						if (StringCmp(string, string1) != 0) {
							newstring = Cat2Strings(string, string1, "; ", 0);
							string = MemFree(string);
							string = newstring;
						}
						string1 = MemFree(string1);
					}
				} else {
					if (StringLen(vnp->data.ptrvalue)) {
						string = CheckEndPunctuation(vnp->data.ptrvalue, '\0');
						first_done = TRUE;
					}
				}
			} else if (vnp->choice == Seq_descr_molinfo) {
				mfp = vnp->data.ptrvalue;
				if (mfp && mfp->tech > 1 && mfp->tech != 8) {
					if (mfp->tech == MI_TECH_concept_trans_a) {
				/*		s = StringForSeqMethod(method); */
						s = NULL;
					} else {
						s = StringForSeqTech(mfp->tech);
					}
					if (s!= NULL && *s != '\0') {
						ptr = MemNew(StringLen(s) + 10);
						sprintf(ptr, "Method: %s", s); 
					}
					if (first_done) {
						newstring = Cat2Strings(string, ptr, "; ", 0);
						string = MemFree(string);
						string = newstring;
					} else {
						string = StringSave(ptr);
						first_done = TRUE;
					}
					MemFree(ptr);
				}
			} else if (vnp->choice == Seq_descr_method) {
				if (vnp->data.intvalue > 1) {
					if (method == METHOD_concept_transl_a) {
					/*	s = StringForSeqMethod(method);*/
						s = NULL;
					} else {
						s = StringForSeqMethod((Uint1)(vnp->data.intvalue));
					}
					if (s!= NULL && *s != '\0') {
						ptr = MemNew(StringLen(s) + 10);
						sprintf(ptr, "Method: %s", s); 
					}
		
					if (first_done) {
						newstring = Cat2Strings(string, ptr, "; ", 0);
						string = MemFree(string);
						string = newstring;
					} else {
						string = StringSave(ptr);
						first_done = TRUE;
					}
					MemFree(ptr);
				}
			}
		}
	}
/* gather pubs on protein bioseq do not do checking or sorting*/
	vnp = NULL;
	if (ajp->useSeqMgrIndexes) {
		/* finess calls to get_pubs */
		MemSet ((Pointer) (&gc), 0, sizeof (GatherContext));
		gc.userdata = (Pointer) (&vnp);
		gc.entityID = ajp->entityID;
		psdp = SeqMgrGetNextDescriptor (bsp, NULL, Seq_descr_pub, &dcontext);
		while (psdp != NULL) {
			gc.thistype = OBJ_SEQDESC;
			gc.itemID = dcontext.itemID;
			gc.thisitem = (Pointer) psdp;
			omdp = dcontext.omdp;
			if (omdp != NULL) {
				gc.parenttype = omdp->datatype;
				gc.parentitem = omdp->dataptr;
			} else {
				gc.parenttype = 0;
				gc.parentitem = NULL;
			}
			get_pubs (&gc);
			psdp = SeqMgrGetNextDescriptor (bsp, psdp, Seq_descr_pub, &dcontext);
		}
		psfp = SeqMgrGetNextFeature (bsp, NULL, SEQFEAT_PUB, 0, &fcontext);
		while (psfp != NULL) {
			gc.thistype = OBJ_SEQFEAT;
			gc.itemID = dcontext.itemID;
			gc.thisitem = (Pointer) psfp;
			get_pubs (&gc);
			psfp = SeqMgrGetNextFeature (bsp, psfp, SEQFEAT_PUB, 0, &fcontext);
		}
		omdp = ObjMgrGetData (ajp->entityID);
		if (omdp != NULL && omdp->datatype == OBJ_SEQSUB) {
			ssp = (SeqSubmitPtr) omdp->dataptr;
			if (ssp != NULL) {
				sbp = ssp->sub;
				if (sbp != NULL) {
					gc.thistype = OBJ_SUBMIT_BLOCK;
					gc.itemID = 1;
					gc.thisitem = (Pointer) sbp;
					get_pubs (&gc);
				}
			}
		}
		/* also submit block */
	} else {
  		MemSet ((Pointer) (&gs), 0, sizeof (GatherScope));
/*	MemSet ((Pointer) (gs.ignore), (int)(TRUE), (size_t) (OBJ_MAX * sizeof(Boolean)));
		gs.ignore[OBJ_SEQENTRY] = FALSE;
		gs.ignore[OBJ_BIOSEQ] = FALSE;
		gs.ignore[OBJ_SEQDESC] = FALSE;*/
		gs.ignore[OBJ_SEQSUB] = TRUE;
		gs.ignore[OBJ_SEQSUB_CIT] = TRUE;
		slp = ValNodeNew(NULL);
		slp->choice = SEQLOC_WHOLE;
		slp->data.ptrvalue = (SeqIdPtr) SeqIdDup (SeqIdFindBest (bsp->id, 0));
		gs.target = slp;
		gs.seglevels = 4;

		GatherEntity(ajp->entityID, &vnp, get_pubs, &gs);
		if (slp)
			SeqLocFree(slp);
	}
/*	if ((status = CheckPubs(ajp, bsp, &vnp)) < 0) {
			ValNodeFree(vnp);
			vnp = NULL;
	}
	vnp = OrganizePubList(vnp); */
	for (vnp1=vnp; vnp1; vnp1=vnp1->next) {
		psp = vnp1->data.ptrvalue;
		if ((pdp=psp->descr) != NULL) {
			if (pdp->fig) {
				total += 32;
				total += StringLen(pdp->fig);
			}
			if (pdp->maploc) {
				total += 22;
				total += StringLen(pdp->maploc);
			}
		}
	}

	if (sfp) {
		cdr = (CdRegionPtr) sfp->data.value.ptrvalue;
		product = sfp->product;
		if (product && SeqLocLen(product)) 
			protein = TRUE;
		if (sfp->excpt)
			total += 112;
		if (cdr && cdr->conflict && (protein || ! sfp->excpt))
			total += 112;
	}

	string1 = (CharPtr) MemNew(total*sizeof(Char));

	for (vnp1=vnp; vnp1; vnp1=vnp1->next) {
		psp = vnp1->data.ptrvalue;
		if ((pdp=psp->descr) != NULL) {
			if (pdp->fig) {
			
				temp = CheckEndPunctuation(pdp->fig, '\0');
				total = StringLen(string1);

				sprintf(string1+total, "This sequence comes from %s", temp);
				prefix = "; ";
				temp = MemFree(temp);
			}
			if (pdp->maploc) {
				total = StringLen(string1);
				sprintf(string1+total, "%sMap location %s", prefix, pdp->maploc);
				prefix = "; ";
			}
		}
	}

	if (sfp) {
		if (cdr && cdr->conflict && (protein || ! sfp->excpt)) {
			total = StringLen(string1);
			sprintf(string1+total, "%s%s", prefix,
					protein?conflict_msg:conflict_msg_no_protein);
		}
	}
	if (string && string1) {
		newstring = Cat2Strings(string, string1, "; ", 0);
		string = MemFree(string);
		string1 = MemFree(string1);
	} else if (string) {
		newstring = string;
	} else if (string1) {
		newstring = string1;
	}

	if (newstring) {
		SaveNoteToCharPtrStack(nsp, NULL, newstring);
		newstring = MemFree(newstring);
	}
	for (vnp1=vnp; vnp1; vnp1=vnp1next) {
		vnp1next = vnp1->next;
		psp = vnp1->data.ptrvalue;
		FreePubStruct(psp);
		MemFree(vnp1);
	}
	return;
}	/* GetProtRefComment */

NLM_EXTERN GBQualPtr AddModifsToGBQual (GBEntryPtr gbp, GBQualPtr gbqual)
{
	CharPtr ptr;
	ValNodePtr descr, man;

	descr=BioseqGetSeqDescr(gbp->bsp, Seq_descr_modif, NULL); 
	if (descr) {
		for (man = (ValNodePtr) descr-> data.ptrvalue; man != NULL; man = man -> next){
			switch (man -> data.intvalue){
			case 3: case 14: 
				ptr = AsnEnumStr("GIBB-mod", 
		  		   (Int2) man->data.intvalue);
				if (GBQualPresent(ptr, gbqual) == FALSE)
					gbqual = AddGBQual(gbqual, ptr, " ");
				break;
			case 4:
				if (GBQualPresent("mitochondrion", gbqual) == FALSE)
					gbqual = AddGBQual(gbqual, "mitochondrion", NULL);
				break;
			case 15:
				if (GBQualPresent("insertion_seq", gbqual) == FALSE)
					gbqual = AddGBQual(gbqual, "insertion_seq", " ");
				break;
			case 5: case 6: case 7: case 18: case 19:
				ptr = AsnEnumStr("GIBB-mod", 
		  		   (Int2) man->data.intvalue);
				if (GBQualPresent(ptr, gbqual) == FALSE)
					gbqual = AddGBQual(gbqual, ptr, NULL);
				break;
			default:
				break;
			}
		}
	}
	return gbqual;
}	/* AddModifsToGBQual */

/*************************************************************************
*GBQualPtr AddOrgRefModToGBQual (OrgRefPtr orp, GBQualPtr gbqual);
*
*Add the OrgRef.mod to a source feat.  Note: a few of the quals added
*may be illegal for a source feature, but the validator will catch them 
*in the end.
***************************************************************************/
NLM_EXTERN GBQualPtr AddOrgRefModToGBQual (OrgRefPtr orp, GBQualPtr gbqual)

{
	CharPtr mod, ptr, temp_ptr;
	Char temp[ASN2FF_STD_BUF]; /* ASN2FF_STD_BUF (now 35) is longer than 
any qual. */
	Int2 index;
	ValNodePtr vnp;

	if (orp && orp->mod)
	{
		for (vnp=orp->mod; vnp; vnp=vnp->next)
		{
			mod = vnp->data.ptrvalue;
			if (StringNCmp(mod, "citation", 8) == 0)
				continue;
			index=0;
			for (ptr=mod; *ptr != '\0'; ptr++)
			{
				index++;
				if (*ptr == ' ' || *ptr == '=')
				{
					ptr++;
					index--;
					break;
				}
			}
			if (index > ASN2FF_STD_BUF-1)
				continue;

			temp_ptr = &(temp[0]);
			StringNCpy(temp_ptr, mod, index);
			temp[index] = '\0';
			if ((GBQualNameValid(temp_ptr)) == -1)
				continue;
			if (ptr)
				gbqual = AddGBQual(gbqual, temp_ptr, ptr);
			else
				gbqual = AddGBQual(gbqual, temp_ptr, NULL);
		}
	}
	return gbqual;
}	/* AddOrgRefModToGBQual */

/*************************************************************************
*GBQualPtr AddBioSourceToGBQual (BioSourcePtr biosp, GBQualPtr gbqual);
*
*Add the OrgMod.subtypes and SubSource.subtypes to a source feat.
*Add BioSource.genome to a source feat.  
*Note: a few of the quals added may be illegal for a source feature, 
*but the validator will catch them in the end.
***************************************************************************/

static CharPtr organelleQual [] = {
  NULL,
  NULL,
  "plastid:chloroplast",
  "plastid:chromoplast",
  "mitochondrion:kinetoplast",
  "mitochondrion",
  "plastid",
  NULL,
  NULL,
  NULL, 
  NULL,
  NULL,
  "plastid:cyanelle",
  NULL,
  NULL,
  "nucleomorph",
  "plastid:apicoplast",
  "plastid:leucoplast",
  "plastid:proplastid",
  NULL
};

NLM_EXTERN GBQualPtr AddBioSourceToGBQual (Asn2ffJobPtr ajp, NoteStructPtr nsp, BioSourcePtr biosp, GBQualPtr gbqual, Boolean new_release)
{
	CharPtr qual, val = NULL;
	OrgModPtr omp;
	OrgNamePtr onp;
	SubSourcePtr ssp;
	Int2 i;
	Int4 id = -1;
	DbtagPtr db = NULL;
	OrgRefPtr org;
	ValNodePtr vnp;
	CharPtr s;

	if (biosp == NULL)
		return gbqual;
	if (biosp->genome) {
		i = biosp->genome;
		if (i > 1 && i < 20) {
			val = organelleQual [i];
			if (val != NULL) {
				gbqual = AddGBQual (gbqual, "organelle", val);
			} else if (i < num_genome) {
				qual = genome[i];
				if (qual && (GBQualNameValid(qual)) != -1) {
					if (i == 8) {  /*extrachrom*/
						gbqual = AddGBQual(gbqual, "note", "extrachromosomal");
					} else {
						gbqual = AddGBQual(gbqual, qual, val);
					}
				} else if (qual && i == 8) {
					gbqual = AddGBQual(gbqual, "note", "extrachromosomal");
				}
			}
		}
	}
	org = (OrgRefPtr) biosp->org;
	if (org) {
		if ((onp = (OrgNamePtr) org->orgname) != NULL) {
			for (omp=onp->mod; omp != NULL; omp=omp->next) {
				for (i=0; orgmod_subtype[i].name != NULL; i++) {
					if (omp->subtype == orgmod_subtype[i].num)
						break;
				}
				if (orgmod_subtype[i].name == NULL) {
					continue;
				}
				if (orgmod_subtype[i].num == 253) { /* old_lineage */
					continue;
				}
				if (orgmod_subtype[i].num == 254) { /* old_name */
					continue;
				}
				qual = orgmod_subtype[i].name;
				if (orgmod_subtype[i].num == 21) {   /* nat_hos */
					qual = "specific_host";
				}
				if ((val = omp->subname) == NULL)
					val = "";
				if ((GBQualNameValid(qual)) != -1) {
					gbqual = AddGBQual(gbqual, qual, val);
				} else {
					s = MemNew(StringLen(val) + 
							StringLen(qual) + 3);
					sprintf(s, "%s: %s", qual, val);
					CpNoteToCharPtrStack(nsp, NULL, s);
				}
			}
		}
/* add db_xref */
		val = NULL;
		for (vnp=org->db; vnp; vnp=vnp->next) {
			id = -1;
			db = (DbtagPtr) vnp->data.ptrvalue;
			if (db && db->db) {
				for (i =0; i < DBNUM; i++) {
					if (StringCmp(db->db, dbtag[i]) == 0) {
						id = i;
						break;
					}
				}
				if (id == -1) {
					continue;  /* unknown dbtag */
				}
			}
			if (db->tag && db->tag->str) {
				val = MemNew(StringLen(db->db)+StringLen(db->tag->str)+2);
				sprintf(val, "%s:%s", db->db, db->tag->str);
			} else if (db->tag) {
				val = MemNew(StringLen(db->db)+16);
				sprintf(val, "%s:%ld", db->db, (long) db->tag->id);
			}
			if (val[0] != '\0') {
				gbqual = AddGBQual(gbqual, "db_xref", val);
				MemFree(val);
			}
		}
	}
	for (ssp = biosp->subtype; ssp != NULL; ssp=ssp->next) {
		qual = NULL;
		if (ssp->subtype == 255) {
			qual = "note";
		} else if (ssp->subtype > num_subtype) {
			qual = NULL;
		} else if (ssp->subtype > 0) {
			qual = subtype[ssp->subtype - 1];
		} else {
			qual = "?";
		}
		val = ssp->name;
		if (ssp->subtype != 14 && ssp->subtype != 15) {
			if (val == NULL)
				val = "";
			}
		if ((GBQualNameValid(qual)) == -1) {
			if (qual == NULL) {
				qual = "?";
			}
			s = MemNew(StringLen(val) + StringLen(qual) + 3);
			sprintf(s, "%s: %s", qual, val);
			CpNoteToCharPtrStack(nsp, NULL, s);
		} else {
			gbqual = AddGBQual(gbqual, qual, val);
		}
	}
	if (biosp->is_focus == TRUE) {
		gbqual = AddGBQual(gbqual, "focus", NULL);
	}
	return gbqual;
}	/* AddBioSourceToGBQual */

/****************************************************************************
*PrintImpFeatEx
*
*	This code prints out an ImpFeat in GenBank and HTML format.
*
****************************************************************************/
NLM_EXTERN Int2 PrintImpFeatEx (Asn2ffJobPtr ajp, BioseqPtr bsp, SeqFeatPtr sfp, Int4 gi, Int2 entityID, Int2 itemID)
{
	CharPtr flatloc_ptr, key, loc;
	GBQualPtr gbqp;
	ImpFeatPtr ifp;
	Uint1 class_qual, format=ajp->format;
	Int2 class_equal, gbqual_index;
	static CharPtr buf = NULL;
	Uint2 retval;
	ValNodePtr seqid;
	CharPtr p, q;

	if (sfp == NULL)
		return -1;
	if (sfp->data.choice != SEQFEAT_IMP)
		return -1;
	ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
	key = ifp->key;
	loc = ifp->loc;

	for (seqid = ajp->id_print; seqid; seqid=seqid->next) {
		if (seqid->choice == SEQID_GI) {
		}
	}
	if (format == EMBL_FMT || format == PSEUDOEMBL_FMT || 
						format == EMBLPEPT_FMT)
		ff_StartPrint(5, 21, ASN2FF_EMBL_MAX, "FT");
	else
		ff_StartPrint(5, 21, ASN2FF_GB_MAX, NULL);

	if (ajp->slp) {
		ff_AddString(key);
	} else {
		www_featkey(key, gi, entityID, itemID);
	}
	TabToColumn(22);
	if (loc == NULL) {
		flatloc_ptr = FlatLoc(bsp, sfp->location);
		if (get_www()) {
			buf = www_featloc(flatloc_ptr);
			ff_AddString(buf);
			MemFree(buf);
		} else {
			ff_AddString(flatloc_ptr);
		}
		MemFree(flatloc_ptr);
	} else {
		if (get_www()) {
			buf = www_featloc(loc);
			ff_AddString(buf);
			MemFree(buf);
		} else {
			ff_AddString(loc);
		}
	}
	if (sfp->partial == TRUE) {
		retval = SeqLocPartialCheck(sfp->location);
		if (retval == SLP_COMPLETE || retval > SLP_OTHER) {
			NewContLine();
			ff_AddString("/partial");
		}
	}
	for (gbqp=sfp->qual; gbqp; gbqp=gbqp->next) {
		gbqual_index = GBQualNameValid(gbqp->qual);
		if (gbqual_index != -1) {
			NewContLine();
			ff_AddChar( '/');
			ff_AddString(gbqp->qual);
			class_qual = ParFlat_GBQual_names[gbqual_index].gbclass;
			if (class_qual == Class_none) {
				class_equal=CheckForEqualSign(gbqp->qual);
				if (class_equal == 1)
					continue;
			}
			ff_AddChar('=');
			if (class_qual == Class_text && 
				StringCmp(gbqp->val, "\"\"") == 0) { 
				ff_AddString(gbqp->val);
				continue;
			}
			if (get_www() && (class_qual == Class_text
							|| class_qual == Class_note)) {
				buf = www_featloc(gbqp->val);
			} else {
				buf = StringSave(gbqp->val);
			}
			if (class_qual == Class_text || class_qual == Class_none
				|| class_qual == Class_ecnum || class_qual == Class_note)
				ff_AddString("\"");
			if (class_qual == Class_note) {
				/* start of process tildes */
				if (StringCmp (gbqp->qual, "note") == 0) {
					for (p = buf, q = buf; *p != '\0'; *q++ = *p++) {
						if (*p != '~')
							continue;
						if (p [1] != '~')
							*p = '\n';
						else
							p++;
					}
					*q = '\0';
				}
				/* end of process tildes */
				www_note_gi(buf);
			} else if (class_qual != Class_none) {
				if (StringCmp(gbqp->qual, "transl_table") == 0) {
					www_gcode(buf);
				} else if (StringCmp(gbqp->qual, "db_xref") == 0) {
					www_db_xref(buf);
				} else if (StringCmp(gbqp->qual, "protein_id") == 0 ||
					StringCmp(gbqp->qual, "transcript_id") == 0) {
					www_protein_id(buf);
				} else {
					ff_AddString(buf);
				}
			}
			if (class_qual == Class_text || class_qual == Class_none
				|| class_qual == Class_ecnum || class_qual == Class_note)
				ff_AddString("\"");
			if (buf) {
				MemFree(buf);
			}
		} else if (format == GENPEPT_FMT) {
			if (StringCmp(gbqp->qual, "site_type") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "bond_type") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "region_name") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "sec_str_type") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "non-std-residue") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "heterogen") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "name") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "coded_by") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			}
		} else if (ASN2FF_VALIDATE_FEATURES == FALSE) {
			NewContLine();
			ff_AddChar('/');
			ff_AddString(gbqp->qual);
			if (gbqp->val != NULL && StringLen(gbqp->val) != 0) {
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			}
		}
	}

	ff_EndPrint();

	return 1;
} /*PrintImpFeatEx */
		
static GBQualPtr extract_qual(GBQualPtr PNTR head, GBQualPtr x)
{
	GBQualPtr	v, p;
	
	if (*head == NULL) {
		return NULL;
	}
	if (x == *head) {
		*head = x->next;
		x->next = NULL;
		return x;
	}
	for (v = *head; v != NULL && v != x; v = v->next) {
		p = v;
	}
	if (v == NULL) {
		return NULL;
	}
	p->next = x->next;
	x->next = NULL;
	return x;
}
static GBQualPtr tie_next_qual(GBQualPtr head, GBQualPtr next)
{
	GBQualPtr v;

	if (head == NULL) {
		return next;
	}
	for (v = head; v->next != NULL; v = v->next) {
		v = v;
	}
	v->next = next;
	return head;
}

/****************************************************************************
*PrintImpFeat
*
*	This code prints out an ImpFeat in GenBank and HTML format.
*
****************************************************************************/
NLM_EXTERN Int2 PrintImpFeat (Asn2ffJobPtr ajp, BioseqPtr bsp, SeqFeatPtr sfp)
{
	CharPtr flatloc_ptr, key, loc;
	GBQualPtr gbqp;
	ImpFeatPtr ifp;
	Uint1 class_qual, format=ajp->format;
	Int2 class_equal, gbqual_index;
	static CharPtr buf = NULL;
	Uint2 retval;
	Boolean first=TRUE;
	GBQualPtr tmp, gbqpnext, head=NULL;

	if (sfp == NULL)
		return -1;
	if (sfp->data.choice != SEQFEAT_IMP)
		return -1;
	ifp = (ImpFeatPtr) sfp->data.value.ptrvalue;
	key = ifp->key;
	loc = ifp->loc;

	if (format == EMBL_FMT || format == PSEUDOEMBL_FMT || 
						format == EMBLPEPT_FMT)
		ff_StartPrint(5, 21, ASN2FF_EMBL_MAX, "FT");
	else
		ff_StartPrint(5, 21, ASN2FF_GB_MAX, NULL);
	ff_AddString(key);
	TabToColumn(22);
	if (loc == NULL) {
		flatloc_ptr = FlatLoc(bsp, sfp->location);
		if (get_www()) {
			buf = www_featloc(flatloc_ptr);
			ff_AddString(buf);
			MemFree(buf);
		} else {
			ff_AddString(flatloc_ptr);
		}
		MemFree(flatloc_ptr);
	} else {
		if (get_www()) {
			buf = www_featloc(loc);
			ff_AddString(buf);
			MemFree(buf);
		} else {
			ff_AddString(loc);
		}
	}
	if (sfp->partial == TRUE) {
		retval = SeqLocPartialCheck(sfp->location);
		if (retval == SLP_COMPLETE || retval > SLP_OTHER) {
			NewContLine();
			ff_AddString("/partial");
		}
	}
/* put all /note last */
	for (gbqp=sfp->qual; gbqp; gbqp=gbqpnext) {
		gbqpnext=gbqp->next;
		if (StringCmp(gbqp->qual, "note") == 0) {
			tmp = extract_qual(&(sfp->qual), gbqp);
			head = tie_next_qual(head, tmp);
		}
	}
	if (head) {
		sfp->qual = tie_next_qual(sfp->qual, head);
	}
	for (gbqp=sfp->qual; gbqp; gbqp=gbqp->next) {
		gbqual_index = GBQualNameValid(gbqp->qual);
		if (gbqual_index != -1) {
			NewContLine();
			if (first) {
				ff_AddChar( '/');
				ff_AddString(gbqp->qual);
			}
			class_qual = ParFlat_GBQual_names[gbqual_index].gbclass;
			if (class_qual == Class_none) {
				class_equal=CheckForEqualSign(gbqp->qual);
				if (class_equal == 1)
					continue;
			}
			if (first) {
				ff_AddChar('=');
			}
			if (class_qual == Class_text && 
				StringCmp(gbqp->val, "\"\"") == 0) { 
			/* an empty string is considered legal */ 
				ff_AddString(gbqp->val);
				continue;
			}
			if (get_www() && (class_qual == Class_text
							|| class_qual == Class_note)) {
				buf = www_featloc(gbqp->val);
			} else {
				buf = StringSave(gbqp->val);
			}
			if (class_qual == Class_text || class_qual == Class_none
				|| class_qual == Class_ecnum)
				ff_AddString("\"");
			if (first && class_qual == Class_note)
					ff_AddString("\"");					
			if (class_qual == Class_note) {
				www_note_gi(buf);
			} else if (class_qual != Class_none) {
				if (StringCmp(gbqp->qual, "transl_table") == 0) {
					www_gcode(buf);
				} else if (StringCmp(gbqp->qual, "db_xref") == 0) {
					www_db_xref(buf);
				} else {
					ff_AddString(buf);
				}
			}
			if (class_qual == Class_text || class_qual == Class_none
				|| class_qual == Class_ecnum)
				ff_AddString("\"");
			if (gbqp->next == NULL && class_qual == Class_note)
				ff_AddString("\"");
			if (buf) {
				MemFree(buf);
			}
			if (class_qual == Class_note) {
				if (first == TRUE)
					first = FALSE;
			}
		} else if (format == GENPEPT_FMT) {
			if (StringCmp(gbqp->qual, "site_type") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "bond_type") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "region_name") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "sec_str_type") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "non-std-residue") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "heterogen") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "name") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			} else if (StringCmp(gbqp->qual, "coded_by") == 0) {
				NewContLine();
				ff_AddChar('/');
				ff_AddString(gbqp->qual);
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			}
		} else if (ASN2FF_VALIDATE_FEATURES == FALSE) {
			NewContLine();
			ff_AddChar('/');
			ff_AddString(gbqp->qual);
			if (gbqp->val != NULL && StringLen(gbqp->val) != 0) {
				ff_AddChar('=');
				ff_AddString("\"");
				ff_AddString(gbqp->val);
				ff_AddString("\"");
			}
		}
	}

	ff_EndPrint();

	return 1;
} /*PrintImpFeat */
		
#define NOEQUALTOTAL 13
NLM_EXTERN Int2 CheckForEqualSign(CharPtr qual)
				/* this have to be changed. Tatiana 02.28.95 */
{
	Int2 i;
	static CharPtr NoEqualSign[NOEQUALTOTAL] = {
	"chloroplast",
	"chromoplast",
	"cyanelle", 
	"germline",
	"kinetoplast",
	"macronuclear", 
	"mitochondrion",
	"partial",
	"proviral",
	"pseudo",
	"rearranged",
	"virion",
	"focus"
	};

	if (qual == NULL)
		return -1;

	for (i=0; i < NOEQUALTOTAL; i++)
		if (StringICmp(qual, NoEqualSign[i]) == 0)
			return 1;

	return 0;

}

/*-------------------------- delete_qual() ----------------------------*/
/*************************************************************************
*   delete_qual:
*   -- return TRUE if found the "qual" in the "qlist", also remove
*      the "qual" from list
*                                                                7-8-93
**************************************************************************/
NLM_EXTERN Boolean delete_qual(GBQualPtr PNTR qlist, CharPtr qual)
{
   GBQualPtr curq, preq;

   for (preq = NULL, curq = *qlist; curq != NULL; curq = curq->next) {
       if (StringCmp(curq->qual, qual) == 0) {
          if (preq == NULL)
             preq = *qlist = curq->next;
          else
             preq->next = curq->next;

          curq->next = NULL;
          GBQualFree(curq);
          curq = NULL;

          return (TRUE);
       }

       preq = curq;
   }

   return (FALSE);

}
