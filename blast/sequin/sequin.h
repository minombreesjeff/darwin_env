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
* $Revision: 6.71 $
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
#define SEQ_PKG_GENOMICCDNA   3
#define SEQ_PKG_POPULATION    4
#define SEQ_PKG_PHYLOGENETIC  5
#define SEQ_PKG_MUTATION      6
#define SEQ_PKG_ENVIRONMENT   7
#define SEQ_PKG_GENBANK       8
#define NUM_SEQ_PKG           9

#define SEQ_FMT_FASTA         1
#define SEQ_FMT_CONTIGUOUS    2 
#define SEQ_FMT_INTERLEAVE    3 
#define NUM_SEQ_FMT           3 

/*
#define SEQ_FMT_FASTAGAP      2
#define SEQ_FMT_PHYLIP        3
#define SEQ_FMT_NEXUS         4
#define SEQ_FMT_PAUP          5
*/

typedef struct fmtblk {
  Int2         seqPackage;
  Int2         seqFormat;
  Int2         numSeqs;
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
                                 CharPtr text, Boolean parseSeqId, Boolean single);

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

extern Boolean SequencesFormHasProteins (ForM f);
extern Boolean SequencesFormHasTooManyNucleotides (ForM f);

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

#define REGISTER_BIOSEQ_SEG_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Bioseq Seg","BioseqSegEditor",OBJ_BIOSEQ_SEG,0,OBJ_BIOSEQ_SEG,0,NULL,BioseqSegEditFunc,PROC_PRIORITY_DEFAULT)
extern Int2 LIBCALLBACK BioseqSegEditFunc (Pointer data);

#define REGISTER_BIOSEQ_SET_EDIT ObjMgrProcLoad(OMPROC_EDIT,"Edit Bioseq Set","BioseqSetEditor",OBJ_BIOSEQSET,0,OBJ_BIOSEQSET,0,NULL,BioseqSetEditFunc,PROC_PRIORITY_DEFAULT)
extern Int2 LIBCALLBACK BioseqSetEditFunc (Pointer data);

extern void LaunchOrfViewer (BioseqPtr bsp, Uint2 entityID, Uint2 itemID, Boolean standAlone);

#define ADD_TITLE 1
#define ADD_RRNA  2
#define ADD_CDS   3
#define ADD_IMP   4

extern Int2 ApplyAnnotationToAll (Int2 type, SeqEntryPtr sep,
                                  ButtoN partialLft, ButtoN partialRgt,
                                  TexT geneName, TexT protName, TexT rnaName,
                                  TexT featcomment, TexT defline);

extern SeqFeatPtr FindBestCds (Uint2 entityID, SeqLocPtr loc, SeqLocPtr prod, SeqEntryPtr scope);

NLM_EXTERN SeqEntryPtr SequinFastaToSeqEntryEx 
  (
    FILE *fp, Boolean is_na, CharPtr PNTR errormsg,
    Boolean parseSeqId, CharPtr special_symbol
  );

/* Many miscellaneous extern functions within sequin source files */

extern Boolean PropagateFromGenBankBioseqSet (SeqEntryPtr sep, Boolean ask);

extern Uint2 SmartAttachSeqAnnotToSeqEntry (Uint2 entityID, SeqAnnotPtr sap);
extern void HandleProjectAsn (ProjectPtr proj, Uint2 entityID);

extern CharPtr CompressSpaces (CharPtr str);
extern CharPtr SearchForString (CharPtr str, CharPtr sub, Boolean case_counts, Boolean whole_word);
extern void AddAboutAndHelpMenuItems (MenU m);
extern void NetConfigureProc (IteM i);
extern void EntrezQueryProc (IteM i);
extern void SetupEditSecondary (MenU m, BaseFormPtr bfp);
extern void SimplePowerBlastProc (IteM i);
extern void SimpleCDDBlastProc (IteM i);
extern void VectorScreenProc (IteM i);
extern void GenerateAutomaticDefLinesCommon (IteM i, Boolean addMods, Boolean smartMods, ButtoN b);
extern void ForceTaxonFixupBtn (IteM i, ButtoN b);
extern void CommonAddOrgOrModsToDefLines (IteM i, Int2 orgmod, Int2 subsource, ButtoN b);
extern void MRnaFromCdsProc (Uint2 entityID);
extern void BioseqViewFormToolBar (GrouP h);
extern Boolean DoBuildContig (void);
extern void SetGenome (PopuP p);
extern void ReplaceBioSourceGenomePopup (DialoG d, PopuP genome);
extern CharPtr NameStdPtrToAuthorSpreadsheetString (NameStdPtr nsp);
extern NameStdPtr AuthorSpreadsheetStringToNameStdPtr (CharPtr txt);
extern Boolean ExtendGene (GeneRefPtr grp, SeqEntryPtr nsep, SeqLocPtr slp);
extern void CommonAddOrgOrModsToDefLines (IteM i, Int2 orgmod, Int2 subsource, ButtoN b);
extern void CorrectGenCodes (SeqEntryPtr sep, Uint2 entityID);
extern void PrepareToConvertToCDS (SeqEntryPtr sep, Uint2 entityID,
                                   Uint2 subtype, CharPtr findthis);
extern Int2 CountSeqEntryComponents (SeqEntryPtr sep);
extern void EditGenbankElements (Handle i);
extern void EditSequenceHistory (IteM i);
extern void FindGeneAndProtForCDS (Uint2 entityID, SeqFeatPtr cds,
                                   SeqFeatPtr PNTR gene, SeqFeatPtr PNTR prot);
extern SeqFeatPtr FindBestProtein (Uint2 entityID, SeqLocPtr product);
extern void NewDescriptorMenuFunc (ObjMgrProcPtr ompp, BaseFormPtr bfp, Uint2 descsubtype);
extern Boolean PropagateFromGenBankBioseqSet (SeqEntryPtr sep, Boolean ask);
extern CharPtr MergeValNodeStrings (ValNodePtr list, Boolean useReturn);
extern int LIBCALLBACK SortByVnpChoice (VoidPtr ptr1, VoidPtr ptr2);
extern void PrepareToConvertToCDS (SeqEntryPtr sep, Uint2 entityID,
                                   Uint2 subtype, CharPtr findthis);
extern void ConvertToLocalProc (IteM i);
extern void PromoteToBestIDProc (IteM i);
extern void RemoveIDsFromBioseqs (IteM i);
extern void VectorScreenProc (IteM i);
extern void SimplePowerBlastProc (IteM i);
extern CharPtr MergeValNodeStrings (ValNodePtr list, Boolean useReturn);
extern CharPtr JustSaveStringFromText (TexT t);

extern void ApplyTitle (IteM i);
extern void ApplyCDS (IteM i);
extern void ApplyRRNA (IteM i);
extern void ApplyImpFeat (IteM i);

extern void RemoveRNA (IteM i);
extern void ConvertRNA (IteM i);
extern void AddRNA (IteM i);
extern void EditRNA (IteM i);

extern void ParseDefToSource (IteM i);
extern void ParseLocalIDToSource (IteM i);
extern void ParseFileToSource (IteM i);
extern void AddStrainToOrg (IteM i);
extern void AddCloneToOrg (IteM i);
extern void AddSubspeciesToOrg (IteM i);

extern void ParseInMoreProteins (IteM i);
extern void ParseInNucUpdates (IteM i);
extern void ParseInOligoPrimers (IteM i);
extern void ParseInMoreMRNAs (IteM i);
extern void ParseInProteinsInOrder (IteM i);

extern void AutoParseFeatureTableProc (IteM i);

extern void RecomputeSuggest (IteM i);
extern void RetranslateCdRegionsNoStop (IteM i);
extern void RetranslateCdRegionsDoStop (IteM i);
extern void CorrectCDSGenCodes (IteM i);
/* extern void CorrectCDSStartCodon (IteM i); */
/* extern Boolean RetranslateOneCDS (SeqFeatPtr sfp, Uint2 entityID, Boolean include_stop); */

extern void GenerateAutoDefLinesNoMods (IteM i);
extern void GenerateAutoDefLinesWithMods (IteM i);
extern void GenerateAutoDefLinesSmartMods (IteM i);

extern void EditEvidenceFlag (IteM i);
extern void EditExceptionFlag (IteM i);
extern void EditFeaturePartials (IteM i);
extern void MakeGroupsOf200 (IteM i);

extern void ViewAlignmentSummary (IteM i);

extern void SetupEditSecondary (MenU m, BaseFormPtr bfp);
extern void EditLocusProc (IteM i);
extern void ConvertToLocalProc (IteM i);

extern void RemoveFeature (IteM i);
extern void RemoveDescriptor (IteM i);

extern void SelectFeature (IteM i);
extern void SelectDescriptor (IteM i);
extern void SelectBioseq (IteM i);

extern void FuseFeature (IteM i);

extern void ParseAsnOrFlatfileToAnywhere (IteM i);
extern void RemoveTextInsideString (IteM i);
extern void GeneralTextConversion (IteM i);

extern void BioseqViewFormToolBar (GrouP h);

extern void AddCDSet (IteM i);
extern void RemoveCDSet (IteM i);
extern void EditCDSet (IteM i);
extern void ConvertCDSet (IteM i);

extern void FindStringProc (IteM i);
extern void FindFlatfileProc (IteM i);
extern void FindGeneProc (IteM i);
extern void FindProtProc (IteM i);
extern void FindPosProc (IteM i);

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
extern void SqnReadAlignView (BioseqPtr target_bsp, SeqEntryPtr source_sep);
extern void DownloadAndUpdateProc (ButtoN b);

extern void FastaNucDirectToSeqEdProc (IteM i);

extern void RemoveAlignment (IteM i);
extern void RemoveGraph (IteM i);
extern void RemoveProteins (IteM i);

extern SeqAlignPtr Sqn_GlobalAlignTwoSeq (BioseqPtr bsp1, BioseqPtr bsp2, BoolPtr revcomp);
extern SeqAlignPtr Sequin_GlobalAlignTwoSeq (BioseqPtr bsp1, BioseqPtr bsp2, Int4Ptr endsfixed);


#ifndef WIN_MAC
extern void CreateSqnInitialFormMenus (WindoW w);
#endif


#ifdef __cplusplus
}
#endif

#endif /* ndef _SEQUIN_ */

