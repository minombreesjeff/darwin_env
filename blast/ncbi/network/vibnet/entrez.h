/*   entrez.h
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
* File Name:  entrez.h
*
* Author:  Jonathan Kans, Jonathan Epstein
*
* Version Creation Date:   8/5/96
*
* $Revision: 6.12 $
*
* File Description: 
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: entrez.h,v $
* Revision 6.12  2000/07/08 20:44:08  vakatov
* Get all "#include" out of the 'extern "C" { }' scope;  other cleanup...
*
* ==========================================================================
*/

#ifndef _ENTREZ_
#define _ENTREZ_

#include <vibrant.h>
#include <objacces.h>
#include <objentr.h>


#ifdef __cplusplus
extern "C" {
#endif

/*
*  The EntrezGlobalsPtr may be registered with a call to SetAppProperty
*  e.g., SetAppProperty ("EntrezGlobals", &entrezglobals), where entrezglobals
*  is a persistent structure filled with Vibrant objects or callback
*  function pointers specific for a given application.
*/

/*
*  The following callback types help process requests between forms.  The
*  form passed in belongs to the calling form, not the target form.  It is
*  up to the application to pass the request to the appropriate form.  This
*  way, there can be multiple term list and docsum forms, and the window
*  handles are not global to the libraries.
*/

typedef void (*EntrezRetrieveDocsProc) (ForM f, Int2 num, Int2 parents, Int4Ptr uids, Int2 db);
typedef void (*EntrezProjectToFormProc) (ForM f, Pointer proj);
typedef void (*EntrezSimpleSeqProc) (ForM f, ValNodePtr simpleSeqs);
typedef void (*EntrezNamedUidListProc) (ForM f, CharPtr term, Int4 num, Int4Ptr uids, Int2 db);
typedef void (*EntrezLaunchViewerProc) (ForM f, Int4 uid, Int2 numAlign, Int4Ptr alignuids, Int2 db);

typedef struct entrezglobals {
  IteM                     parentsPersist;
  IteM                     alignWithChecked;
  ChoicE                   seqComplex;
  ChoicE                   strucComplex;
  ChoicE                   strucModels;
  FonT                     docsumFont;

  Boolean                  persistDefault;
  Boolean                  alignDefault;
  Boolean                  popdownBehavior;
  Boolean                  lookupDirect;
  Boolean                  showAsn;
  Boolean                  sortFields;
  CharPtr                  initDatabase;
  CharPtr                  initField;
  CharPtr                  initMode;

  WndActnProc              createTrmLstMenus;
  WndActnProc              createDocSumMenus;

  EntrezRetrieveDocsProc   retrieveDocsProc;
  EntrezProjectToFormProc  retrieveProjectProc;
  EntrezSimpleSeqProc      retrieveSimpleProc;
  EntrezNamedUidListProc   loadNamedUidProc;
  EntrezLaunchViewerProc   launchViewerProc;
} EntrezGlobals, PNTR EntrezGlobalsPtr;

extern ForM CreateTermListForm (Int2 left, Int2 top, CharPtr title,
                                WndActnProc activate, FormMessageFunc messages);

extern ForM CreateDocSumForm (Int2 left, Int2 top, CharPtr title,
                              WndActnProc activate, FormMessageFunc messages);

/* The following functions are called by the application in response to request */

extern void RetrieveDocuments (ForM f, Int2 num, Int2 parents, Int4Ptr uids, Int2 db);

extern void RetrieveSimpleSeqs (ForM f, ValNodePtr simpleSeqs);

extern void LoadNamedUidList (ForM f, CharPtr term, Int4 num, Int4Ptr uids, Int2 db);

extern void LaunchRecordViewer (Int4 uid, Int2 numAlign, Int4Ptr alignuids, Int2 db);

/* The following functions are used to toggle between query styles */

extern ChoicE CreateQueryTypeChoice (MenU m, BaseFormPtr bfp);
extern IteM CreateClearUnusedItem (MenU m, BaseFormPtr bfp);
extern Boolean UsingTextQuery (ForM f);

/* The following functions are used to toggle between immediate/delayed neighbor styles */

extern ChoicE CreateNeighborDelayChoice (MenU m, BaseFormPtr bfp);
extern Boolean UsingDelayedNeighbor (ForM f);
extern void UseDelayedNeighbor (ForM f, Boolean delayMode);

/* The following functions are used when creating windows */

extern GrouP MakeViewerLinkControls (GrouP prnt, BaseFormPtr bfp, Int2 doctype, Int4 uid, Boolean blast);
extern void UpdateViewerLinkTarget (GrouP g);
extern void LoadDocsumOptionsMenu (MenU m);
extern void DocSumFontChangeProc (IteM i);
extern void DisplayFontChangeProc (IteM i);

extern void DSLoadUidListProc (IteM i);
extern void DSSaveUidListProc (IteM i);

extern void TLLoadUidListProc (IteM i);
extern void TLSaveUidListProc (IteM i);

extern Boolean DocSumCanSaveFasta (ForM f, Boolean nucs, Boolean prots);
extern Boolean ExportDocSumFasta (ForM f, CharPtr filename, Boolean nucs, Boolean prots);

/* The following dialog function is used within a preferences form */

typedef struct entrezprefs {
  Boolean          persistDefault;
  Boolean          alignDefault;
  Boolean          lookupDirect;
  Boolean          showAsn;
  CharPtr          initDatabase;
  CharPtr          initField;
  CharPtr          initMode;
  CharPtr          initMedLabel;
  CharPtr          initNucLabel;
  CharPtr          initProtLabel;
  CharPtr          initGenomeLabel;
  Int2             minPixelWidth;
  Int2             minPixelHeight;
} EntrezPrefs, PNTR EntrezPrefsPtr;

extern DialoG CreateEntrezPrefsDialog (GrouP prnt, CharPtr title);
extern EntrezPrefsPtr EntrezPrefsNew (void);
extern EntrezPrefsPtr EntrezPrefsFree (EntrezPrefsPtr epp);

/* The following functions are normally for internal use */

#define ALIST_BLASTN    201
#define ALIST_BLASTP    202
#define ALIST_BLASTX    203
#define ALIST_TBLASTN   204
#define ALIST_TBLASTX	205

extern EnumFieldAssocPtr MakeDatabaseAlist (EntrezTypeInfo *type_info, short type_count, Boolean blast);
extern EnumFieldAssocPtr MakeFieldAlist (EntrezTypeData *types, EntrezFieldInfo *field_info,
                                         short field_count, Int2 db, Boolean allFields);

extern Int2 DatabaseFromName (CharPtr name);
extern Int2 FieldFromTag (CharPtr tag);

#ifdef __cplusplus
}
#endif

#endif /* ndef _ENTREZ_ */
