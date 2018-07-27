/*   dlogutil.h
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
* File Name:  dlogutil.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   1/22/95
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

#ifndef _DLOGUTIL_
#define _DLOGUTIL_

#include <vibrant.h>
#include <sqnutils.h>
#include <prtutil.h>
#include <objall.h>
#include <objfeat.h>
#include <objfdef.h>
#include <sequtil.h>


#ifdef __cplusplus
extern "C" {
#endif

#define CHANGE_VIEW_NOTABS        0
#define CHANGE_VIEW_FOLDERTABS    1
#define CHANGE_VIEW_TEXTTABS      2
#define CHANGE_VIEW_RADIOBUTTONS  3
#define CHANGE_VIEW_POPUP         4

extern StdPrintOptionsPtr  spop;

extern Boolean SetupPrintOptions (void);
extern void FreePrintOptions (void);

extern EnumFieldAssoc  months_alist [];

extern void DatePtrToVibrant (DatePtr dp, PopuP dateMonth, TexT dateDay, TexT dateYear);
extern DatePtr VibrantToDatePtr (PopuP dateMonth, TexT dateDay, TexT dateYear);

extern ValNodePtr AddStringToValNodeChain (ValNodePtr head, CharPtr str, Uint1 choice);

#define DESCRIPTOR_FORM_BLOCK       \
  FORM_MESSAGE_BLOCK                \
  DialoG          data;


#define FEATURE_FORM_BLOCK          \
  DESCRIPTOR_FORM_BLOCK             \
  DialoG          commonRadio;      \
  GrouP           commonSubGrp [5]; \
  Int2            commonPage;       \
  ButtoN          partial;          \
  ButtoN          exception;        \
  ButtoN          pseudo;           \
  TexT            comment;          \
  TexT            title;            \
  TexT            exceptText;       \
  PopuP           evidence;         \
  Handle          gene;             \
  PopuP           genePopup;        \
  LisT            geneList;         \
  ValNodePtr      geneNames;        \
  GrouP           useGeneXref;      \
  GrouP           newGeneGrp;       \
  TexT            geneSymbol;       \
  TexT            geneDesc;         \
  ButtoN          editGeneBtn;      \
  TexT            protXrefName;     \
  DialoG          location;         \
  DialoG          product;          \
  DialoG          featcits;         \
  DialoG          dbxrefs;          \
  DialoG          gbquals;          \
  DialoG          usrobjext;

typedef struct descform {
  DESCRIPTOR_FORM_BLOCK
} DescriptorForm, PNTR DescriptorFormPtr;

typedef struct featform {
  FEATURE_FORM_BLOCK
} FeatureForm, PNTR FeatureFormPtr;

extern Boolean DescFormReplaceWithoutUpdateProc (ForM f);
extern void StdDescFormActnProc (ForM f);

extern void StdDescFormCleanupProc (GraphiC g, VoidPtr data);

extern Boolean FeatFormReplaceWithoutUpdateProc (ForM f);
extern void StdFeatFormActnProc (ForM f);

extern void StdSeqFeatPtrToFeatFormProc (ForM f, Pointer data);
extern void StdInitFeatFormProc (ForM f);
extern void StdFeatFormCleanupProc (GraphiC g, VoidPtr data);
extern void StdFeatFormAcceptButtonProc (ButtoN b);

extern void ExtendGeneFeatIfOnMRNA (Uint2 entityID, SeqEntryPtr sep);

extern OMUserDataPtr ItemAlreadyHasEditor (Uint2 entityID, Uint2 itemID, Uint2 itemtype, Uint2 procid);
extern Int2 LIBCALLBACK StdVibrantEditorMsgFunc (OMMsgStructPtr ommsp);

/*****************************************************************************
*
*   Various specific spreadsheet or editor dialogs.
*
*****************************************************************************/

extern DialoG CreateAuthorDialog (GrouP prnt, Uint2 rows, Int2 spacing);

extern DialoG CreateDateDialog (GrouP prnt, CharPtr title);

extern DialoG CreateAffilDialog (GrouP prnt, CharPtr title);
extern DialoG CreatePublisherAffilDialog (GrouP prnt, CharPtr title);
extern DialoG CreateProceedingsDialog (GrouP prnt, CharPtr title);

/* The Ext versions split the affil dialogs into two superimposed pages,
   both of which are initially hidden, that can go in separate folder tabs */

extern DialoG CreateExtAffilDialog (GrouP prnt, CharPtr title,
                                    GrouP PNTR grp1, GrouP PNTR grp2);
extern DialoG CreateExtPublisherAffilDialog (GrouP prnt, CharPtr title,
                                             GrouP PNTR grp1, GrouP PNTR grp2);
extern DialoG CreateExtProceedingsDialog (GrouP prnt, CharPtr title,
                                          GrouP PNTR grp1, GrouP PNTR grp2);

extern GrouP CreateCommonFeatureGroup (GrouP h, FeatureFormPtr ffp,
                                       SeqFeatPtr sfp, Boolean hasGeneControl,
                                       Boolean hasCitationTab);
extern GrouP CreateCommonFeatureGroupEx (GrouP h, FeatureFormPtr ffp,
                                         SeqFeatPtr sfp, Boolean hasGeneControl,
                                         Boolean hasCitationTab, Boolean hasGeneSuppress);
extern void PopulateGenePopup (FeatureFormPtr ffp);
extern void SeqFeatPtrToCommon (FeatureFormPtr ffp, SeqFeatPtr sfp);
extern void SetNewFeatureDefaultInterval (FeatureFormPtr ffp);
extern DialoG CreateImportFields (GrouP h, CharPtr name, SeqFeatPtr sfp, Boolean allowProductGBQual);

extern DialoG CreateIntervalEditorDialog (GrouP h, CharPtr title, Uint2 rows,
                                          Int2 spacing, SeqEntryPtr sep,
                                          Boolean nucsOK, Boolean protsOK);

typedef void (*IntEdPartialProc) (FeatureFormPtr ffp, Boolean partial5, Boolean partial3, Boolean order);

extern DialoG CreateIntervalEditorDialogEx (GrouP h, CharPtr title, Uint2 rows,
                                            Int2 spacing, SeqEntryPtr sep,
                                            Boolean nucsOK, Boolean protsOK,
                                            Boolean useBar, Boolean showPartials,
                                            Boolean allowGaps, FeatureFormPtr ffp,
                                            IntEdPartialProc proc);
extern void StdFeatIntEdPartialCallback (FeatureFormPtr ffp, Boolean partial5, Boolean partial3, Boolean order);

extern DialoG CreateVisibleStringDialog (GrouP h, Uint2 rows,
                                         Int2 spacing, Int2 width);

extern DialoG CreateDbtagDialog (GrouP h, Uint2 rows, Int2 spacing,
                                 Int2 width1, Int2 width2);

/*****************************************************************************
*
*   Miscellaneous functions.
*
*****************************************************************************/

/*
*  The TextViewProcsPtr may be registered with a call to SetAppProperty
*  e.g., SetAppProperty ("TextDisplayForm", &viewprocs), where viewprocs
*  is a persistent structure filled with callback function pointers specific
*  for a given application.
*/

typedef struct textviewprocs {
  WndActnProc      activateForm;
  WndActnProc      closeForm;
  WndActnProc      createMenus;

  FormMessageFunc  handleMessages;

  Int2             minPixelWidth;
  Int2             minPixelHeight;
  Boolean          useScrollText;

  FonT             displayFont;
} TextViewProcs, PNTR TextViewProcsPtr;

extern void LaunchGeneralTextViewer (CharPtr path, CharPtr title);

extern Boolean FileToScrollText (TexT t, CharPtr path);
extern void ScrollTextToFile (TexT t, CharPtr path);

extern void FileToClipboard (CharPtr path);

/*
*  The StdEditorProcsPtr may be registered with a call to SetAppProperty
*  e.g., SetAppProperty ("StdEditorForm", &viewprocs), where viewprocs
*  is a persistent structure filled with callback function pointers specific
*  for a given application.
*/

typedef struct stdeditorprocs {
  WndActnProc      activateForm;
  FormMessageFunc  handleMessages;

  Boolean          duplicateExisting;
} StdEditorProcs, PNTR StdEditorProcsPtr;

#ifndef WIN_MAC
extern void CreateStdEditorFormMenus (WindoW w);
#endif

/*
*  When duplicating instead of editing, the editors call the following
*  function to change the baseFormPtr itemID and itemtype to the closest
*  Bioseq or BioseqSet, thus adding the new item to that parent.
*/

extern Boolean SetClosestParentIfDuplicating (BaseFormPtr bfp);

/*
*  The HelpMessageFunc may be registered with a call to SetAppProperty
*  e.g., SetAppProperty ("HelpMessageProc", helpproc), where helpproc
*  is an application-supplied callback function that takes character
*  strings for the heading and section.
*/

typedef void (*HelpMessageFunc) (CharPtr, CharPtr);

/*****************************************************************************
*
*   Miscellaneous functions.
*
*****************************************************************************/

extern CharPtr SaveStringFromTextAndStripNewlines (TexT t);

/* GetRidOfEmptyFeatsDescStrings takes either an entityID or a SeqEntryPtr */

extern void GetRidOfEmptyFeatsDescStrings (Uint2 entityID, SeqEntryPtr sep);

/*****************************************************************************
*
*   LaunchEntrezURL constructs a web-Entrez URL query.  Parameter choices are:
*
*     Database      Report Formats
*
*      PubMed        DocSum, Brief, Abstract, Citation, MEDLINE, ASN.1, ExternalLink
*      Nucleotide    DocSum, Brief, GenBank, ASN.1, FASTA, ExternalLink
*      Protein       DocSum, Brief, GenPept, ASN.1, FASTA, ExternalLink
*      Genome        DocSum, Brief, ASN.1, ExternalLink
*      Popset        DocSum, Brief, ASN.1, FASTA, ExternalLink
*      Structure     DocSum, Brief
*      OMIM          DocSum, Text, Synopsis, Variants, MiniMIM, ASN.1, ExternalLink
*      Taxonomy      DocSum, Brief, TxInfo, TxTree, ExternalLink
*
*****************************************************************************/

NLM_EXTERN void LaunchEntrezURL (CharPtr database, Int4 uid, CharPtr format);


#ifdef __cplusplus
}
#endif

#endif /* ndef _DLOGUTIL_ */

