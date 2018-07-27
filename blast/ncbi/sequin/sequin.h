/*   sequin.h
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
* File Name:  sequin.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/22/95
*
* $Revision: 6.289 $
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

#ifndef _SEQUIN_
#define _SEQUIN_

#ifdef INTERNAL_NCBI_SEQUIN
#ifndef EXTRA_SERVICES
#define EXTRA_SERVICES
#endif
#ifndef NETWORK_SAVVY_SEQUIN
#define NETWORK_SAVVY_SEQUIN
#endif
#ifndef USE_SPELL
#define USE_SPELL
#endif
#endif

#ifdef EXTRA_SERVICES
#define USE_DESKTOP
#define REPLACE_THIS
#define EDIT_LOCUS
#endif

#ifdef NETWORK_SAVVY_SEQUIN
#define USE_ENTREZ
#define USE_LOCAL
#define USE_BLAST
#define USE_MEDARCH
#define USE_TAXON
#define ALLOW_DOWNLOAD
#endif

#ifdef PUBLIC_NETWORK_SEQUIN
#define USE_DESKTOP
#define USE_ENTREZ
#define USE_LOCAL
#define USE_BLAST
#define USE_MEDARCH
#define USE_TAXON
#define ALLOW_DOWNLOAD
#endif

#include <dlogutil.h>
#include <bspview.h>
#include <objproj.h>
#include <urlquery.h>


#ifdef __cplusplus
extern "C" {
#endif


#define SEQ_PKG_SINGLE        1
#define SEQ_PKG_SEGMENTED     2
#define SEQ_PKG_GAPPED        3
#define SEQ_PKG_GENOMICCDNA   4
#define SEQ_PKG_POPULATION    5
#define SEQ_PKG_PHYLOGENETIC  6
#define SEQ_PKG_MUTATION      7
#define SEQ_PKG_ENVIRONMENT   8
#define SEQ_PKG_GENBANK       9
#define NUM_SEQ_PKG           9

#define SEQ_FMT_FASTA         1
#define SEQ_FMT_ALIGNMENT     2 
#define NUM_SEQ_FMT           2 
  
/*
#define SEQ_FMT_FASTAGAP      2
#define SEQ_FMT_PHYLIP        3
#define SEQ_FMT_NEXUS         4
#define SEQ_FMT_PAUP          5
*/

#define SEQ_ORIG_SUBMISSION   1
#define SEQ_TPA_SUBMISSION    2

typedef struct fmtblk {
  Int2         seqPackage;
  Int2         seqFormat;
  Int2         numSeqs;
  Int2         submType;
} FormatBlock, PNTR FormatBlockPtr;

typedef struct sqnblk {
  AuthorPtr    contactperson;
  AuthListPtr  citsubauthors;
  AffilPtr     citsubaffil;
  CharPtr      citsubtitle;
  DatePtr      releasedate;
  Boolean      holduntilpublished;
} SequinBlock, PNTR SequinBlockPtr;

extern CharPtr SEQUIN_APPLICATION;
extern CharPtr SEQUIN_SERVICES;
extern CharPtr SEQUIN_VERSION;

extern ForM  helpForm;

extern Boolean  useDesktop;
extern Boolean  useEntrez;
extern Boolean  useLocal;
extern Boolean  useBlast;
extern Boolean  useMedarch;
extern Boolean  useTaxon;
extern Boolean  allowDownload;
extern Boolean  extraServices;
extern Boolean  indexerVersion;
extern CharPtr  genomeCenter;

extern Boolean  leaveAsOldAsn;
extern Boolean  newAlignReader;

#ifdef WIN_MAC
extern Boolean  termListUp;
extern Boolean  docSumUp;
extern Boolean  bioseqViewUp;
#endif

extern void SwapQualifiers (IteM i);
extern void PrefixAuthorityWithOrganism (IteM i);
extern void UpdateFastaSet (IteM i);
extern void ExtendFastaSet (IteM i);
extern void ExtendAllSequencesInSet (IteM i);
extern void SeqLocAdjustByOffset (SeqLocPtr slp, Int4 offset);
extern void SplitSegmentedFeatsMenuItem (IteM i);
extern SeqLocPtr SeqLocWholeNew (BioseqPtr bsp);
extern SeqFeatPtr SeqFeatCopy (SeqFeatPtr sfp);
extern SeqLocPtr SeqLocReplaceLocalID (SeqLocPtr slp,
				       SeqIdPtr  new_sip);
extern SequinBlockPtr SequinBlockFree (SequinBlockPtr sbp);

extern ForM CreateStartupForm (Int2 left, Int2 top, CharPtr title,
                               BtnActnProc startFa2htgs,
                               BtnActnProc startPhrap,
                               BtnActnProc buildContig,
                               BtnActnProc startNew,
                               BtnActnProc readExisting,
                               BtnActnProc fetchFromNet,
                               BtnActnProc showHelp,
                               BtnActnProc quitProgram,
                               WndActnProc activateForm);

extern void DrawAbout (PaneL p);
extern Int2 AboutBoxWidth (void);
extern Int2 AboutBoxHeight (void);

extern ForM CreateFormatForm (Int2 left, Int2 top, CharPtr title,
                              BtnActnProc goToNext,
                              BtnActnProc goBack,
                              WndActnProc activateForm);

extern ForM CreateInitSubmitterForm (Int2 left, Int2 top, CharPtr title,
                                     BtnActnProc goToNext,
                                     BtnActnProc goBack,
                                     WndActnProc activateForm);

extern DialoG CreateFastaDialog (GrouP h, CharPtr title, Boolean is_na, Boolean is_mrna,
                                 CharPtr text, Boolean parseSeqId, Boolean single, Int2Ptr seqPackagePtr);

extern ForM CreateInitOrgNucProtForm (Int2 left, Int2 top, CharPtr title,
                                      FormatBlockPtr format,
                                      BtnActnProc goToNext,
                                      BtnActnProc goBack,
                                      WndActnProc activateForm);

extern ForM CreateGenomeCenterForm (Int2 left, Int2 top, CharPtr title,
                                    BtnActnProc finish,
                                    BtnActnProc cancel,
                                    Boolean readPhrap,
                                    Boolean buildContig,
                                    WndActnProc activateForm);

extern SeqEntryPtr ImportOneGappedSequence (FILE *fp);

extern Boolean HasZeroLengthSequence (ForM newForm);
extern Boolean SequencesFormHasProteins (ForM f);
extern Boolean SequencesFormHasTooManyNucleotides (ForM f);

extern void AppendOrReplaceString (
  CharPtr PNTR string_loc,
  CharPtr new_value,
  Boolean PNTR asked_question,
  Boolean PNTR do_replace,
  Boolean PNTR use_semicolon
);

extern void ConsolidateOrganismNotes (IteM i);
extern void ConsolidateLikeModifiersWithSemicolons (IteM i);
extern void ConsolidateLikeModifiersWithoutSemicolons (IteM i);

extern void CountryLookup (IteM i);
extern void ExtractProteinFeaturesFromNote (IteM i);
extern void ConvertPseudoCDSToMiscFeat (IteM i);
extern void ProcessPseudoMiscFeat (IteM i);
extern void ParseInfluenzaAVirusNames (IteM i);
extern void AddStrainAndSerotypeToInfluenzaAVirusNames (IteM i);
extern void EditPubs (IteM i);
extern void RemovePubConsortiums (IteM i);

extern void ExtendPartialFeatures (IteM i);
extern void TrimOrganismName (IteM i);
extern void SUCSubmitterProc (IteM i);

extern void ConfirmSequencesFormParsing (ForM f, FormActnFunc putItAllTogether);

extern ForM CreateHelpForm (Int2 left, Int2 top, CharPtr title,
                            CharPtr file, BtnActnProc closeForm,
                            WndActnProc activateForm);

extern void SendHelpScrollMessage (ForM f, CharPtr heading, CharPtr section);

/* The next pointer in NewObject is not used in freeing the list.  Each
block is attached individually as extra data to the appropriate menu item.
The linked list is used solely to enable and disable new feature menu items
by the target bsp->mol, or to enable and disable analysis menu items by the
ability to produce FASTA (bioseq viewer or docsum window). */

typedef struct urlparamdata {
  Uint1          type;     /* 1 = text, 2 = checkbox, 3 = popup, 4 = radio, 5 = list */
  CharPtr        param;
  CharPtr        prompt;   /* if no prompt, use param */
  CharPtr        dfault;
  CharPtr        choices;  /* choices if param is popup */
  CharPtr        group;    /* used for grouping related controls */
  CharPtr        descr;
  CharPtr        help;
} UrlParamData, PNTR UrlParamPtr;

typedef struct newobjectdata {
  Int2             kind;   /* 1 = feature creation, 2 = analysis */
  ObjMgrProcPtr    ompp;
  BaseFormPtr      bfp;
  IteM             item;
  Uint1            molgroup;
  Uint2            descsubtype;
  Boolean          bspOK;
  Boolean          dsmOK;
  Boolean          fastaNucOK;
  Boolean          fastaProtOK;
  Boolean          onlyBspTarget;
  /* the next eight fields are for the analysis menu only, for remote URLs */
  CharPtr          host_machine;
  Uint2            host_port;
  CharPtr          host_path;
  CharPtr          query;
  Uint4            timeoutsec;
  Int2             format;     /* 1 = FASTA, 2 = ASN.1 */
  Boolean          demomode;
  QueryResultProc  resultproc;
  ValNodePtr       paramlist; /* data.ptrvalue points to UrlParamData block */
  CharPtr          prefix;
  CharPtr          suffix;
  CharPtr          homepage;
  CharPtr          credits;
  CharPtr          authors;
  CharPtr          disclaimer;
  CharPtr          reference;
  Uint4            pmid;
  CharPtr          blurb;
  struct newobjectdata PNTR next;
} NewObjectData, PNTR NewObjectPtr;

#ifdef WIN_MAC
extern VoidPtr macUserDataPtr;
#endif

extern void SetupSpecialMenu (MenU m, BaseFormPtr bfp);
extern void SetupNewFeaturesMenu (MenU m, BaseFormPtr bfp);
extern void SetupNewDescriptorsMenu (MenU m, BaseFormPtr bfp);
extern void SetupNewPublicationsMenu (MenU m, BaseFormPtr bfp);
extern void SetupBatchApplyMenu (MenU s, BaseFormPtr bfp);
extern void SetupBatchEditMenu (MenU s, BaseFormPtr bfp);
extern MenU CreateAnalysisMenu (WindoW w, BaseFormPtr bfp, Boolean bspviewOK, Boolean docsumOK);
extern void SetupSequinFilters (void);
extern void SetupBioseqPageList (void);

extern Boolean LIBCALLBACK SequinOpenMimeFile (CharPtr filename);
extern Boolean LIBCALLBACK SequinOpenResultFile (CharPtr filename);
extern Boolean LIBCALLBACK SequinHandleNetResults (CharPtr filename);

extern void SequinCheckSocketsProc (void);

extern Int4 MySeqEntryToAsn3 (SeqEntryPtr sep, Boolean strip, Boolean correct, Boolean force);
extern void ValSeqEntryForm (ForM f);

extern void InitSequinExtras (void);
extern void FiniSequinExtras (void);

/* This function destroys the SequinBlockPtr */

extern Uint2 PackageFormResults (SequinBlockPtr sbp, SeqEntryPtr sep,
                                 Boolean makePubAndDefLine);

extern void EnableFeaturesPerTarget (BaseFormPtr bfp);
extern void EnableAnalysisItems (BaseFormPtr bfp, Boolean isDocSum);

extern void ExtendSeqLocToEnd (SeqLocPtr slp, BioseqPtr bsp, Boolean end5);

#define REGISTER_BIOSEQ_SEG_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Bioseq Seg","BioseqSegEditor",OBJ_BIOSEQ_SEG,0,OBJ_BIOSEQ_SEG,0,NULL,BioseqSegEditFunc,PROC_PRIORITY_DEFAULT)
extern Int2 LIBCALLBACK BioseqSegEditFunc (Pointer data);

#define REGISTER_BIOSEQ_SET_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Bioseq Set","BioseqSetEditor",OBJ_BIOSEQSET,0,OBJ_BIOSEQSET,0,NULL,BioseqSetEditFunc,PROC_PRIORITY_DEFAULT)
extern Int2 LIBCALLBACK BioseqSetEditFunc (Pointer data);

extern void LaunchOrfViewer (BioseqPtr bsp, Uint2 entityID, Uint2 itemID, Boolean standAlone);

#define ADD_TITLE 1
#define ADD_RRNA  2
#define ADD_CDS   3
#define ADD_IMP   4
#define ADD_GENE  5

extern Int2 ApplyAnnotationToAll (Int2 type, SeqEntryPtr sep,
                                  ButtoN partialLft, ButtoN partialRgt,
                                  TexT geneName, TexT protName, 
                                  TexT protDesc, TexT rnaName,
                                  TexT featcomment, TexT defline);

extern SeqFeatPtr FindBestCds (Uint2 entityID, SeqLocPtr loc, SeqLocPtr prod, SeqEntryPtr scope);

NLM_EXTERN SeqEntryPtr SequinFastaToSeqEntryEx 
  (
    FILE *fp, Boolean is_na, CharPtr PNTR errormsg,
    Boolean parseSeqId, CharPtr special_symbol
  );

/* Many miscellaneous extern functions within sequin source files */

extern Boolean WriteSequinAppParam (CharPtr section, CharPtr type, CharPtr value);

extern Boolean PropagateFromGenBankBioseqSet (SeqEntryPtr sep, Boolean ask);

extern Uint2 SmartAttachSeqAnnotToSeqEntry (Uint2 entityID, SeqAnnotPtr sap);
extern void HandleProjectAsn (ProjectPtr proj, Uint2 entityID);

extern CharPtr CompressSpaces (CharPtr str);
extern CharPtr SearchForString (CharPtr str, CharPtr sub, Boolean case_counts, Boolean whole_word);
extern void AddAboutAndHelpMenuItems (MenU m);
extern void NetConfigureProc (IteM i);
extern void EntrezQueryProc (IteM i);
extern void Entrez2QueryProc (IteM i);
extern void SetupEditSecondary (MenU m, BaseFormPtr bfp);
extern void SimpleCDDBlastProc (IteM i);
extern void SimpleCDDSearchFeatProc (IteM i);
extern void SimpleCDDSearchAlignProc (IteM i);
extern void ForceCleanupEntityID (Uint2 entityID);
extern void ForceTaxonFixupBtn (IteM i, ButtoN b);
extern void CommonAddOrgOrModsToDefLines (IteM i, Int2 orgmod, Int2 subsource, ButtoN b);
extern void PrefixDefLines (IteM i);
extern void MRnaFromCdsProc (Uint2 entityID);
extern void BioseqViewFormToolBar (GrouP h);
extern Boolean DoBuildContig (void);
extern void SetGenome (PopuP p);
extern PopuP ReplaceBioSourceGenomePopup (DialoG d, PopuP genome);
extern PopuP ReplaceBioSourceGencodePopup (DialoG d, PopuP gencode);
extern CharPtr NameStdPtrToAuthorSpreadsheetString (NameStdPtr nsp);
extern NameStdPtr AuthorSpreadsheetStringToNameStdPtr (CharPtr txt);
extern Boolean ExtendGene (GeneRefPtr grp, SeqEntryPtr nsep, SeqLocPtr slp);
extern void CommonAddOrgOrModsToDefLines (IteM i, Int2 orgmod, Int2 subsource, ButtoN b);
extern void CorrectGenCodes (SeqEntryPtr sep, Uint2 entityID);
extern void PrepareToConvertToCDS (SeqEntryPtr sep, Uint2 entityID,
                                   Uint2 subtype, CharPtr findthis);
extern void EditGenbankElements (Handle i);
extern void EditSequenceHistory (IteM i);
extern void InsertGeneLocusTagPrefix (IteM i);
extern void ReplaceRepeatRegionLocusTagWithDbxref (IteM i);
extern void FindGeneAndProtForCDS (Uint2 entityID, SeqFeatPtr cds,
                                   SeqFeatPtr PNTR gene, SeqFeatPtr PNTR prot);
extern SeqFeatPtr FindBestProtein (Uint2 entityID, SeqLocPtr product);
extern void ExportAlignmentInterleave (IteM i);
extern void ExportAlignmentContiguous (IteM i);
extern void FixFeatureIntervals (IteM i);
extern void ConvertInnerCDSsToProteinFeatures (IteM i);

extern void NewDescriptorMenuFunc (ObjMgrProcPtr ompp, BaseFormPtr bfp, Uint2 descsubtype);
extern Boolean PropagateFromGenBankBioseqSet (SeqEntryPtr sep, Boolean ask);
extern CharPtr MergeValNodeStrings (ValNodePtr list, Boolean useReturn);
extern int LIBCALLBACK SortByVnpChoice (VoidPtr ptr1, VoidPtr ptr2);
extern void PrepareToConvertToCDS (SeqEntryPtr sep, Uint2 entityID,
                                   Uint2 subtype, CharPtr findthis);
extern void ConvertToLocalProcOnlyNucs (IteM i);
extern void ConvertToLocalProcOnlyProts (IteM i);
extern void ConvertToLocalProcAll (IteM i);

extern void PromoteToBestIDProc (IteM i);
extern void PromoteToWorstIDProc (IteM i);
extern void RemoveGBIDsFromBioseqs (IteM i);
extern void RemoveGBIDsFromProteins (IteM i);
extern void RemoveGIsFromBioseqs (IteM i);
extern CharPtr MergeValNodeStrings (ValNodePtr list, Boolean useReturn);
extern CharPtr JustSaveStringFromText (TexT t);

extern SeqLocPtr RemoveGapsFromDeltaLocation (SeqLocPtr slp, BioseqPtr bsp);
extern void CommonApplyToAllProc (BaseFormPtr bfp, Int2 type);
extern void ApplyTitle (IteM i);
extern void ApplyCDS (IteM i);
extern void ApplyRRNA (IteM i);
extern void ApplyImpFeat (IteM i);
extern void LoadTPAAccessionNumbersFromFile (IteM i);
extern void LoadSecondaryAccessionNumbersFromFile (IteM i);
extern void LoadHistoryAccessionNumbersFromFile (IteM i);
extern void LoadOrganismModifierTable (IteM i);
extern void ExportOrganismTable (IteM i);
extern void LoadFeatureQualifierTable (IteM i);
extern Boolean ImportOrganismModifiersForSubmit (SeqEntryPtr seq_list);
extern Boolean ImportOrganismModifiersForSourceAssistant
(Int4 num_sequences,
 CharPtr PNTR id_list,
 CharPtr PNTR defline_list);
extern CharPtr GetSelectedModifierPopupName (PopuP p);
extern CharPtr GetModifierPopupPositionName (Int2 val);
extern Int4 GetNumDeflineModifiers (void);

extern void AddCodonListTotRNA (tRNAPtr trna, ValNodePtr codons);

extern void RemoveRedundantProproteinMiscFeats (IteM i);
extern void AddTypeStrainCommentsToAll (IteM i);
extern void AddTypeStrainCommentsWithConstraint (IteM i);
extern void RemoveSequencesFromAlignment (IteM i);
extern void RemoveSequencesFromRecord (IteM i);

extern void ParseFileToSource (IteM i);
extern void AddModToOrg (IteM i);

extern void ParseInMoreProteins (IteM i);
extern void ParseInNucUpdates (IteM i);
extern void ParseInOligoPrimers (IteM i);
extern void ParseInMoreMRNAs (IteM i);
extern void ParseInProteinsInOrder (IteM i);

extern void RecomputeSuggestEx (Uint2 entityID, Boolean fix_genes, Boolean recompute_all);
extern void RecomputeSuggest (IteM i);
extern void RecomputeSuggestFixGenes (IteM i);
extern void RetranslateCdRegionsEx (
  Uint2   entityID,
  Boolean include_stop,
  Boolean no_stop_at_end_of_complete_cds );
extern void RetranslateCdRegionsNoStop (IteM i);
extern void RetranslateCdRegionsDoStop (IteM i);
extern void RetranslateCdRegionsNoStopExceptEndCompleteCDS (IteM i);
extern void AddGlobalCodeBreak (IteM i);
extern void ParseCodonQualToCodeBreak (IteM i);
extern void CorrectCDSGenCodes (IteM i);
/* extern void CorrectCDSStartCodon (IteM i); */
/* extern Boolean RetranslateOneCDS (SeqFeatPtr sfp, Uint2 entityID, Boolean include_stop); */
extern void UpdateProteinsFromCDS (IteM i);

extern void testAutoDef (IteM i);
extern void testAutoDefWithOptions (IteM i);
extern void AutoDefWithoutModifiers (IteM i);
extern void AutoDefBaseFormCommon (BaseFormPtr bfp, Boolean use_form, Boolean use_modifiers);
extern void AutoDefToolBtn (ButtoN b);
extern void AutoDefOptionsToolBtn (ButtoN b);

extern void RemoveDefLinesToolBtn (ButtoN b);
extern void FindStringProcToolBtn (ButtoN b);
extern void ResolveExistingLocalIDsToolBtn (ButtoN b);
extern void GroupExplodeToolBtn (ButtoN b);

extern void MakeGroupsOf200 (IteM i);

extern void SetBestFrame (SeqFeatPtr sfp);

extern void ViewAlignmentSummary (IteM i);

extern void SetupEditSecondary (MenU m, BaseFormPtr bfp);
extern void EditLocusProc (IteM i);

extern ValNodePtr BuildDescriptorValNodeList (void);

extern void RemoveDescriptor (IteM i);

extern void SelectDescriptor (IteM i);
extern void SelectBioseq (IteM i);
extern void SelectPubs (IteM i);

extern void FuseFeature (IteM i);

extern void MakeExonsFromCDSIntervals (IteM i);
extern void MakeExonsFromMRNAIntervals (IteM i);

extern Int2 LIBCALLBACK CreateDeleteByTextWindow (Pointer data);
extern Int2 LIBCALLBACK CreateSegregateByTextWindow (Pointer data);
extern Int2 LIBCALLBACK CreateSegregateByFeatureWindow (Pointer data);
extern Int2 LIBCALLBACK CreateSegregateByDescriptorWindow (Pointer data);
extern Int2 LIBCALLBACK CreateSegregateByMoleculeTypeWindow (Pointer data);
extern Int2 LIBCALLBACK RemoveExtraneousSets (Pointer data);
extern void ReverseComplementBioseqAndFeats (BioseqPtr bsp, Uint2 entityID);
extern void RemoveOrphanProteins (Uint2 entityID, SeqEntryPtr sep);
extern void ParseAsnOrFlatfileToAnywhere (IteM i);
extern void ParseCommentToAnywhere (IteM i);
extern void RemoveTextInsideString (IteM i);
extern void RemoveTextOutsideString (IteM i);

extern void BioseqViewFormToolBar (GrouP h);

extern void FindStringProc (IteM i);
extern void FindFlatfileProc (IteM i);
extern void FindGeneProc (IteM i);
extern void FindProtProc (IteM i);
extern void FindPosProc (IteM i);

extern Boolean MeetsStringConstraint (SeqFeatPtr sfp, CharPtr str, Boolean case_insensitive);

extern Boolean SaveSeqSubmitProc (BaseFormPtr bfp, Boolean saveAs);

extern void ExciseString (CharPtr str, CharPtr from, CharPtr to);
extern void MakeSearchStringFromAlist (CharPtr str, CharPtr name);
extern void AddToSubSource (BioSourcePtr biop, CharPtr title, CharPtr label, Uint1 subtype);
extern void AddToOrgMod (BioSourcePtr biop, CharPtr title, CharPtr label, Uint1 subtype);
extern Boolean AutomaticProteinProcess (SeqEntryPtr esep, SeqEntryPtr psep,
                                        Int2 code, Boolean makeMRNA, BioseqPtr forceTarget);

extern CharPtr repackageMsg;
extern BioseqPtr  updateTargetBspKludge;
extern SeqEntryPtr     globalsep;
extern Uint2           globalEntityID;
extern Char            globalPath [PATH_MAX];
extern ForM  startupForm;
extern SeqViewProcs        seqviewprocs;

extern void CommonFetchFromNet (BtnActnProc actn, BtnActnProc cancel);
extern void FetchFromNet (ButtoN b);
extern Boolean SequinEntrezInit (CharPtr appl_id, Boolean no_warnings, BoolPtr is_network);
extern void JustRegisterSeqEntry (BaseFormPtr bfp, Boolean freeit);
extern void JustRegisterSeqEntryBtn (ButtoN b);
extern void AddSubmitBlockToSeqEntry (ForM f);

extern void SqnReadAlignView (BaseFormPtr bfp, BioseqPtr target_bsp, SeqEntryPtr source_sep, Boolean do_update);
extern void DownloadAndUpdateProc (ButtoN b);
extern void DownloadAndExtendProc (ButtoN b);
extern void UpdateSeqAfterDownload (BaseFormPtr bfp, BioseqPtr oldbsp, BioseqPtr newbsp);
extern void ExtendSeqAfterDownload (BaseFormPtr bfp, BioseqPtr oldbsp, BioseqPtr newbsp);
extern void NewUpdateSequence (IteM i);
extern void NewExtendSequence (IteM i);

extern void FastaNucDirectToSeqEdProc (IteM i);

extern void ParseCodonsFromtRNAComment (IteM i);
extern void ParseAntiCodonsFromtRNAComment (IteM i);

extern void RemoveAlignment (IteM i);
extern void RemoveGraph (IteM i);
extern void RemoveProteins (IteM i);
extern void RemoveProteinsAndRenormalize (IteM i);

extern void GlobalAddTranslExcept (IteM i);
extern void AddTranslExceptWithComment (IteM i);

extern void ReadAlignment (IteM i);
extern SeqEntryPtr SeqEntryFromAlignmentFile (FILE *fp, CharPtr missing, CharPtr match,
                                              CharPtr beginning_gap, CharPtr middle_gap,
                                              CharPtr end_gap, CharPtr alphabet, Uint1 moltype,
                                              CharPtr no_org_err_msg);

extern SeqAlignPtr Sqn_GlobalAlignTwoSeq (BioseqPtr bsp1, BioseqPtr bsp2, BoolPtr revcomp);

extern void SqnNewAlign (BioseqPtr bsp1, BioseqPtr bsp2, SeqAlignPtr PNTR salp);

extern void ProduceAlignmentNotes (TAlignmentFilePtr afp, TErrorInfoPtr error_list);

extern Boolean CreateUpdateCitSubFromBestTemplate (SeqEntryPtr top_sep, SeqEntryPtr upd_sep);

#ifndef WIN_MAC
extern void CreateSqnInitialFormMenus (WindoW w);
#endif

#define NUM_PAGES  8

typedef struct sequencesform {
  FORM_MESSAGE_BLOCK
  GrouP           pages [NUM_PAGES];
  Int2            currentPage;
  Int2            tagFromPage [NUM_PAGES];
  Int2            numPages;
  DialoG          tbs;

  Uint1           dnamolfrommolinfo;
  EnumFieldAssoc  PNTR moltypeAlist;
  ButtoN          partial5;
  ButtoN          partial3;
  GrouP           singleIdGrp;
  TexT            singleSeqID;
  ButtoN          makeAlign;
  DialoG          dnaseq;

  Int2            seqPackage;
  Int2            seqFormat;
  Int2            numSeqs;
  Int2            submType;

  ButtoN          protTechBoth;
  ButtoN          partialN;
  ButtoN          partialC;
  Boolean         makeMRNA;
  DialoG          protseq;

  DialoG          mrnaseq;
  ButtoN          partialmRNA5;
  ButtoN          partialmRNA3;

  GrouP           annotType;
  GrouP           annotGrp;
  ButtoN          partialLft;
  ButtoN          partialRgt;
  TexT            geneName;
  PrompT          protOrRnaPpt;
  TexT            protOrRnaName;
  PrompT          protDescPpt;
  TexT            protDesc;
  TexT            featcomment;
  TexT            defline;
  ButtoN          orgPrefix;

  ButtoN          nextBtn;
  ButtoN          prevBtn;
  BtnActnProc     goToNext;
  BtnActnProc     goToPrev;

  SeqEntryPtr     topSeqForConfirm;
  SeqEntryPtr     currConfirmSeq;
  FormActnFunc    putItAllTogether;
  Int2            currConfirmCount;
  
  ButtoN          use_id_from_fasta_defline;
      
  /* These are added to add modifiers on the source tab */  
  ButtoN          import_mod_btn;
  ButtoN          source_assist_btn;
  ButtoN          specify_orgs_btn;
  ButtoN          specify_locs_btn;
  ButtoN          specify_gcode_btn;
  ButtoN          specify_mgcode_btn;
  ButtoN          clear_mods_btn;
  DoC             org_doc;
  DoC             summary_doc;
  
  /* These allow the user to specify topology and molecule */
  ButtoN          topology_btn;
  ButtoN          molecule_btn;

} SequencesForm, PNTR SequencesFormPtr;

typedef int (LIBCALLBACK *CompareFunc) (Nlm_VoidPtr, Nlm_VoidPtr);

extern int LIBCALLBACK CompareImpFeatEnumFieldAssoc (VoidPtr ptr1, VoidPtr ptr2);
extern int LIBCALLBACK CompareFeatureValNodeStrings (VoidPtr ptr1, VoidPtr ptr2);

extern ValNodePtr InsertMostUsedFeatureValNodes (ValNodePtr old_list);

extern void SortEnumFieldAssocPtrArray (EnumFieldAssocPtr alist, CompareFunc compar);

extern ValNodePtr FindExactStringInStrings ( ValNodePtr strings, CharPtr value);

extern EnumFieldAssocPtr InsertMostUsedFeatureEnumFieldAssoc (
  EnumFieldAssocPtr alist
);

extern ValNodePtr BuildFeatureValNodeList (
  Boolean prefer_most_used,
  CharPtr wild_card_name,
  Int4 wild_card_value,
  Boolean skip_unusual,
  Boolean skip_import
);

extern void SetTaxNameAndRemoveTaxRef (OrgRefPtr orp, CharPtr taxname);

extern void MergeFeatureIntervalsToParts (SeqFeatPtr sfp, Boolean ordered);
extern void MergeToPartsJoin (IteM i);
extern void MergeToPartsOrdered (IteM i);

extern void ConvertInnerCDSsToMatPeptidesCallback (BioseqPtr bsp, Pointer userdata);
extern void MergeCDS (IteM i);
extern Boolean RetranslateOneCDS 
( SeqFeatPtr sfp,
  Uint2 entityID,
  Boolean include_stop,
  Boolean no_stop_at_end_of_complete_cds);

extern void InitValNodePopup (ValNodePtr list, PopuP p);
extern Int2 GetValNodePopup (PopuP p, ValNodePtr list);
extern void SetValNodePopupValue (ValNodePtr list, PopuP p, CharPtr val);

typedef struct listpair
{
  ValNodePtr selected_names_list;
  ValNodePtr selected_values_list;
} ListPairData, PNTR ListPairPtr;

extern DialoG CreateModifierTagList (GrouP g, ListPairPtr lpp);
extern ListPairPtr GetModifierList (DialoG d);
extern Uint1 FindTypeForModNameText (CharPtr cp);

typedef struct featureswithtextdata 
{
  Uint1             seqFeatChoice;
  Uint1             featDefChoice;
  CharPtr           search_text;
  Boolean           case_insensitive;
  Boolean           whole_word;
  Boolean           no_text;
  Boolean           act_when_string_not_present;
  VisitFeaturesFunc callback;
  Pointer           userdata;
} FeaturesWithTextData, PNTR FeaturesWithTextPtr;

typedef struct descriptorswithtextdata 
{
  CharPtr           search_text;
  Boolean           case_insensitive;
  Boolean           whole_word;
  Boolean           no_text;
  Boolean           act_when_string_not_present;
  VisitDescriptorsFunc callback;
  Pointer           userdata;
} DescriptorsWithTextData, PNTR DescriptorsWithTextPtr;


extern void OperateOnBioseqFeaturesWithText 
(BioseqPtr         bsp,
 Pointer           userdata);

extern void OperateOnSeqEntryFeaturesWithText (SeqEntryPtr sep, FeaturesWithTextPtr fdp);
extern void OperateOnSeqEntryDescriptorsWithText (SeqEntryPtr sep, DescriptorsWithTextPtr ddp);

extern LisT 
MakeSequenceListControl 
(GrouP g,
 SeqEntryPtr sep,
 Nlm_LstActnProc actn, 
 Pointer userdata, 
 Boolean show_nucs, 
 Boolean show_prots);
extern ValNodePtr GetSelectedSequenceList (LisT l);
extern void SelectAllSequencesInListCtrl (LisT l);
extern void UnSelectAllSequencesInListCtrl (LisT l);
extern void OffsetLocation (SeqLocPtr loc, Int4 offset, SeqIdPtr sip);
extern void AddCitSubToUpdatedSequence (BioseqPtr upd_bsp, Uint2 input_entityID);
extern Boolean AlistMessage (EnumFieldAssocPtr al, UIEnumPtr val, UIEnum dflt, CharPtr mssg);

typedef struct loginfo 
{
  FILE *fp;
  Boolean data_in_log;
  CharPtr display_title;
  Char path[PATH_MAX];  
} LogInfoData, PNTR LogInfoPtr;

extern LogInfoPtr OpenLog (CharPtr display_title);
extern void CloseLog (LogInfoPtr lip);
extern LogInfoPtr FreeLog (LogInfoPtr lip);

typedef struct sourcequaldesc
{
  CharPtr       name;
  Boolean       isOrgMod;
  Uint1         subtype;
} SourceQualDescData, PNTR SourceQualDescPtr;

extern CharPtr SourceQualValNodeName (ValNodePtr vnp);
extern ValNodePtr SourceQualValNodeDataCopy (ValNodePtr vnp);
extern Boolean SourceQualValNodeMatch (ValNodePtr vnp1, ValNodePtr vnp2);

extern ValNodePtr GetSourceQualDescList (Boolean get_subsrc, Boolean get_orgmod);
extern Boolean IsNonTextModifier (CharPtr mod_name);

extern void FeatureRemove (IteM i);
extern void ConvertFeatures (IteM i);
extern void SelectFeatures (IteM i);
extern void ParseDefLineToSourceQual (IteM i);
extern void ParseFlatfileToSourceQual (IteM i);
extern void ParseLocalIDToSourceQual (ButtoN b);
extern void FeatureEvidenceEditor (IteM i);
extern void FeatureExceptionEditor (IteM i);
extern void FeaturePartialEditor (IteM i);
extern void FeatureStrandEditor (IteM i);
extern void FeatureCitationEditor (IteM i);
extern void ApplySourceQual (IteM i);
extern void PublicApplySourceQual (IteM i);
extern void EditSourceQual (IteM i);
extern void PublicEditSourceQual (IteM i);
extern void ConvertSourceQual (IteM i);
extern void SwapSourceQual (IteM i);
extern void RemoveSourceQual (IteM i);
extern void ApplyCDSGeneProt (IteM i);
extern void PublicApplyCDSGeneProt (IteM i);
extern void EditCDSGeneProt (IteM i);
extern void PublicEditCDSGeneProt (IteM i);
extern void ConvertCDSGeneProt (IteM i);
extern void SwapCDSGeneProt (IteM i);
extern void RemoveCDSGeneProt (IteM i);
extern void ApplyRNAQual (IteM i);
extern void PublicApplyRNAQual (IteM i);
extern void EditRNAQual (IteM i);
extern void PublicEditRNAQual (IteM i);
extern void ConvertRNAQual (IteM i);
extern void SwapRNAQual (IteM i);
extern void RemoveRNAQual (IteM i);
extern void ApplyGBQual (IteM i);
extern void PublicApplyGBQual (IteM i);
extern void EditGBQual (IteM i);
extern void PublicEditGBQual (IteM i);
extern void ConvertGBQual (IteM i);
extern void SwapGBQual (IteM i);
extern void RemoveGBQual (IteM i);
extern void ConvertLocusTagToOldLocusTag (IteM i);
extern void ExportLastLineage (IteM i);

typedef struct existingtext
{
  Int4    existing_text_choice;
} ExistingTextData, PNTR ExistingTextPtr;

#define EXISTING_TEXT_CHOICE_REPLACE_OLD    1
#define EXISTING_TEXT_CHOICE_APPEND_SEMI    2
#define EXISTING_TEXT_CHOICE_APPEND_SPACE   3
#define EXISTING_TEXT_CHOICE_APPEND_NONE    4
#define EXISTING_TEXT_CHOICE_PREFIX_SEMI    5
#define EXISTING_TEXT_CHOICE_PREFIX_SPACE   6
#define EXISTING_TEXT_CHOICE_PREFIX_NONE    7
#define EXISTING_TEXT_CHOICE_LEAVE_OLD      8
#define EXISTING_TEXT_CHOICE_CANCEL         9
extern CharPtr 
HandleExistingText 
(CharPtr existing_text,
 CharPtr new_text, 
 ExistingTextPtr etp);

typedef struct applyvalue 
{
  ValNodePtr        field_list;
  ExistingTextPtr   etp;
  CharPtr           new_text;
  CharPtr           text_to_replace;
} ApplyValueData, PNTR ApplyValuePtr;

/* constraint values */
#define LOCATION_CONSTRAINT_WHOLE_INTERVAL  1
#define LOCATION_CONSTRAINT_START_ENDPOINT  2
#define LOCATION_CONSTRAINT_STOP_ENDPOINT   3

#define LOCATION_CONSTRAINT_ANY        1
#define LOCATION_CONSTRAINT_UPSTREAM   2
#define LOCATION_CONSTRAINT_DOWNSTREAM 3
#define LOCATION_CONSTRAINT_CONTAINED  4
#define LOCATION_CONSTRAINT_NOT_IN     5
#define LOCATION_CONSTRAINT_OVERLAP    6
#define LOCATION_CONSTRAINT_EQUAL      7

#define LOCATION_CONSTRAINT_ANY_STRAND   1
#define LOCATION_CONSTRAINT_PLUS_STRAND  2
#define LOCATION_CONSTRAINT_MINUS_STRAND 3

#define LOCATION_CONSTRAINT_ANY_SEQ      1
#define LOCATION_CONSTRAINT_NUC_SEQ      2
#define LOCATION_CONSTRAINT_PROT_SEQ     3

typedef struct locationconstraint
{
  Int4      left;
  Int4      right;
  Int4      interval_end_choice;
  Int4      match_choice;
  Int4      strand;
  Int4      sequence_type;
} LocationConstraintData, PNTR LocationConstraintPtr;

#define STRING_CONSTRAINT_CONTAINS 1
#define STRING_CONSTRAINT_STARTS   2
#define STRING_CONSTRAINT_ENDS     3

typedef struct stringconstraint
{
  CharPtr match_text;
  Int4    match_location;
  Boolean insensitive;
  Boolean whole_word;
  Boolean not_present;
} StringConstraintData, PNTR StringConstraintPtr;

#define CHOICE_CONSTRAINT_ANY          1
#define CHOICE_CONSTRAINT_QUAL_PRESENT 3
#define CHOICE_CONSTRAINT_STRING       5

typedef struct choiceconstraint
{
  Int4                constraint_type;
  ValNodePtr          qual_choice;
  StringConstraintPtr string_constraint;
  FreeValNodeProc     free_vn_proc;
  CopyValNodeDataProc copy_vn_proc;
} ChoiceConstraintData, PNTR ChoiceConstraintPtr;

typedef struct filterset 
{
  StringConstraintPtr   scp;
  ChoiceConstraintPtr   ccp;
  LocationConstraintPtr lcp;
  ChoiceConstraintPtr   cgp;
} FilterSetData, PNTR FilterSetPtr;

extern Boolean DoesStringMatchConstraint (CharPtr pchSource, StringConstraintPtr scp);

typedef CharPtr (*GetFeatureFieldString) PROTO ((SeqFeatPtr, ValNodePtr, FilterSetPtr));
typedef void (*SetFeatureFieldString) PROTO ((SeqFeatPtr, Pointer, FilterSetPtr));
typedef void (*RemoveFeatureFieldString) PROTO ((SeqFeatPtr, Pointer, FilterSetPtr));
typedef CharPtr (*GetDescriptorFieldString) PROTO ((SeqDescrPtr, ValNodePtr, FilterSetPtr));
typedef void (*SetDescriptorFieldString) PROTO ((SeqDescrPtr, Pointer, FilterSetPtr));
typedef void (*RemoveDescriptorFieldString) PROTO ((SeqDescrPtr, Pointer, FilterSetPtr));
typedef void (*FeatureActionProc) PROTO ((SeqFeatPtr, Pointer, FilterSetPtr));
typedef void (*DescriptorActionProc) PROTO ((SeqDescrPtr, Pointer, FilterSetPtr));

extern CharPtr HandleApplyValue (CharPtr orig_text, ApplyValuePtr avp);
extern ValNodePtr 
ApplyValueToValNodeStringList 
(ValNodePtr list, Int2 choice, ApplyValuePtr avp);

typedef  Boolean (*Nlm_AcceptActnProc) PROTO((Pointer));
typedef  void  (*Nlm_CancelActnProc) PROTO ((Pointer));
typedef  void  (*Nlm_ClearActnProc) PROTO ((Pointer));
typedef  void  (*Nlm_ClearTextActnProc) PROTO ((Pointer));

extern ValNodePtr ValNodeFuncFree (ValNodePtr vnp, FreeValNodeProc free_vn_proc);

#define CONVERT_TYPE_MOVE 0
#define CONVERT_TYPE_COPY 1
#define CONVERT_TYPE_SWAP 2

typedef struct convertfield
{
  ValNodePtr                  src_field_list;
  ValNodePtr                  dst_field_list;
  ExistingTextPtr             etp;
  Int2                        convert_type;
  GetFeatureFieldString       get_str_func;
  SetFeatureFieldString       set_str_func;
  RemoveFeatureFieldString    remove_str_func;
  GetDescriptorFieldString    get_d_str_func;
  SetDescriptorFieldString    set_d_str_func;
  RemoveDescriptorFieldString remove_d_str_func;
  NameFromValNodeProc         name_field_func;
  FilterSetPtr                fsp;
} ConvertFieldData, PNTR ConvertFieldPtr;

extern DialoG StringConstraintDialog (GrouP h, CharPtr label, Boolean clear_btn);
extern DialoG LocationConstraintDialog (GrouP h, Boolean show_interval_controls, Boolean clear_btn);

enum pub_field_nums 
{
  PUB_FIELD_ANY = 0,
  PUB_FIELD_TITLE,
  PUB_FIELD_FIRST_NAME,
  PUB_FIELD_MIDDLE_INITIAL,
  PUB_FIELD_LAST_NAME,
  PUB_FIELD_SUFFIX,
  PUB_FIELD_CONSORTIUM,
  PUB_FIELD_INSTITUTION,
  PUB_FIELD_DEPARTMENT,
  PUB_FIELD_ADDRESS,
  PUB_FIELD_CITY,
  PUB_FIELD_STATE,
  PUB_FIELD_COUNTRY,
  PUB_FIELD_ZIP,
  PUB_FIELD_EMAIL,
  PUB_FIELD_PHONE,
  PUB_FIELD_FAX
};

enum pub_status
{
  PUB_STAT_ANY = 0,
  PUB_STAT_PUBLISHED,
  PUB_STAT_UNPUBLISHED,
  PUB_STAT_INPRESS,
  PUB_STAT_PUBLISHED_SUBMISSION
};

typedef struct pubconstraint
{
  CharPtr find_str;
  Int4    field_for_find;
  Boolean insensitive_to_case;
  Int4    pub_status;
} PubConstraintData, PNTR PubConstraintPtr;

extern PubConstraintPtr PubConstraintFree (PubConstraintPtr pcp);
extern DialoG PubConstraintDialog (GrouP h);


extern DialoG SelectionDialog 
(GrouP h,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata,
 Boolean                  allow_multi,
 CharPtr                  err_msg,
 ValNodePtr               choice_list,
 Int2                     list_height);

extern DialoG ValNodeSelectionDialog
(GrouP h,
 ValNodePtr               choice_list,
 Int2                     list_height,
 NameFromValNodeProc      name_proc,
 FreeValNodeProc          free_vn_proc,
 CopyValNodeDataProc      copy_vn_proc,
 MatchValNodeProc         match_vn_proc,
 CharPtr                  err_name,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata,
 Boolean                  allow_multi);

extern DialoG SourceQualTypeSelectionDialog 
(GrouP h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);

extern DialoG SequenceSelectionDialog 
(GrouP h,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata,
 Boolean                  allow_multi,
 Boolean                  show_nucs,
 Boolean                  show_prots,
 Uint2                    entityID);

extern DialoG 
FeatureFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Int4                     num_fields,
 CharPtr PNTR             field_names,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);

extern DialoG 
GeneFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);
extern CharPtr GetGeneFieldString (SeqFeatPtr sfp, ValNodePtr gene_field, FilterSetPtr fsp);
extern void RemoveGeneFieldString (SeqFeatPtr sfp, ValNodePtr gene_field);

extern DialoG 
MRNAFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);
extern CharPtr GetmRNAFieldString (SeqFeatPtr sfp, ValNodePtr mrna_field, FilterSetPtr fsp);
extern void RemovemRNAFieldString (SeqFeatPtr sfp, ValNodePtr mrna_field);
extern CharPtr GetProteinFieldString (SeqFeatPtr sfp, ValNodePtr protein_field, FilterSetPtr fsp);

extern DialoG 
ProteinFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);

extern DialoG 
CDSGeneProtFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);  
extern CharPtr GetCDSGeneProtField (SeqFeatPtr sfp, ValNodePtr vnp, FilterSetPtr fsp);

extern DialoG
RNAAddFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);
 
extern DialoG
RNARemoveFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);

extern DialoG
RNAFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);
 
extern CharPtr GetRNAFieldString (SeqFeatPtr sfp, ValNodePtr vnp, FilterSetPtr fsp);

extern DialoG 
ExonFieldSelectionDialog
(GrouP                    h,
 Boolean                  allow_none,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);
extern CharPtr GetExonFieldString (SeqFeatPtr sfp, ValNodePtr exon_field);
extern void RemoveExonFieldString (SeqFeatPtr sfp, ValNodePtr exon_field);

typedef  DialoG  (*FeatureFieldSelectionProc) PROTO((GrouP, Boolean, Nlm_ChangeNotifyProc, Pointer));

extern DialoG FeatureFieldChoiceDialog 
(GrouP h,
 FeatureFieldSelectionProc make_fieldlist_dlg,
 Boolean                   offer_to_remove,
 Nlm_ChangeNotifyProc      change_notify,
 Pointer                   change_userdata);

extern DialoG BioSourceStringDialog 
(GrouP                    h,
 Boolean                  allow_multi,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);

typedef struct textportion
{
  Int4    start_choice;
  CharPtr start_text;
  Int4    end_choice;
  CharPtr end_text;
  Boolean insensitive;
  Boolean whole_word;
} TextPortionData, PNTR TextPortionPtr;

extern TextPortionPtr TextPortionFree (TextPortionPtr tp);
extern void 
FindTextPortionInString 
(CharPtr        str, 
 TextPortionPtr tp, 
 CharPtr PNTR   ploc, 
 Int4Ptr        plen);

extern DialoG TextPortionDialog (GrouP h);

extern DialoG 
ConstraintChoiceDialog 
(GrouP                     h, 
 FeatureFieldSelectionProc present_func,
 FeatureFieldSelectionProc string_func,
 FreeValNodeProc           free_vn_proc,
 CopyValNodeDataProc       copy_vn_proc,
 CharPtr                   present_name,
 CharPtr                   text_name,
 Boolean                   clear_btn);
extern DialoG SourceConstraintDialog (GrouP h, Boolean clear_btn);
extern DialoG CDSGeneProtConstraintDialog (GrouP h, Boolean clear_btn);
extern DialoG 
FilterGroup 
(GrouP h,
 Boolean has_string_constraint,
 Boolean has_source_constraint,
 Boolean has_location_constraint,
 Boolean has_cds_gene_prot_constraint,
 CharPtr string_constraint_label);
extern DialoG ParseFieldDestDialog 
(GrouP                    h, 
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);
extern DialoG ParseFieldSourceDialog
(GrouP                    h,
 Nlm_ChangeNotifyProc     change_notify,
 Pointer                  change_userdata);

extern DialoG SampleDialog (GrouP h);

extern CharPtr 
CreateListMessage 
(CharPtr    msg_before,
 CharPtr    msg_after, 
 ValNodePtr id_list);

extern void StringToLower (CharPtr str);

extern Boolean ExportSubmitterBlockTemplate (SeqEntryPtr sep, SeqDescrPtr sdp);
extern DialoG OrganismSelectionDialog (GrouP parent, CharPtr org_name);


#ifdef __cplusplus
}
#endif

#endif /* ndef _SEQUIN_ */

