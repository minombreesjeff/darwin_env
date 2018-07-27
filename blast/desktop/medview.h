/*   medview.h
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
* File Name:  medview.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   4/30/95
*
* $Revision: 6.2 $
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

#ifndef _MEDVIEW_
#define _MEDVIEW_

#include <dlogutil.h>
#include <document.h>
#include <objacces.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
*  The MedlineViewProcsPtr may be registered with a call to SetAppProperty
*  e.g., SetAppProperty ("MedlineDisplayForm", &viewprocs), where viewprocs
*  is a persistent structure filled with parameters and callback function
*  pointers specific for a given application.
*/

typedef GrouP (*MedViewControlsProc) (GrouP prnt, BaseFormPtr bfp, Int2 doctype, Int4 uid);

typedef struct medlineviewprocs {
  Boolean          cleanupObjectPtr;
  WndActnProc      activateForm;
  WndActnProc      closeForm;
  WndActnProc      createMenus;
  Boolean          showAsnPage;

  FormMessageFunc  handleMessages;

  Int2             minPixelWidth;
  Int2             minPixelHeight;
  Int2             initPage;
  CharPtr          initMedLabel;
  Int2             useFolderTabs;
  Boolean          useScrollText;

  FonT             jourfnt;
  FonT             volfnt;
  FonT             pagesfnt;
  FonT             titlefnt;
  FonT             authorsfnt;
  FonT             affilfnt;
  FonT             abstractfnt;
  FonT             meshfnt;
  FonT             displayFont;

  MedViewControlsProc   makeControls;
} MedlineViewProcs, PNTR MedlineViewProcsPtr;

#define REGISTER_MEDLINE_VIEW ObjMgrProcLoad(OMPROC_VIEW,"View Medline","MEDLINE Report",OBJ_MEDLINE_ENTRY,0,OBJ_MEDLINE_ENTRY,0,NULL,MedlineViewGenFunc,PROC_PRIORITY_DEFAULT)

#define ABSTRACT_PAGE  0
#define CITATION_PAGE  1
#define MEDLINE_PAGE   2
#define MEDASN1_PAGE   3
#define MEDXML_PAGE    4

extern ForM LIBCALL CreateMedlineViewForm (Int2 left, Int2 top, CharPtr title,
                                   MedlineEntryPtr mep,
                                   MedlineViewProcsPtr mvpp);

extern Int2 LIBCALLBACK MedlineViewGenFunc (Pointer data);

extern void LIBCALL CopyMedlineViewFormToClipboard (Pointer formDataPtr);
extern void LIBCALL PrintMedlineViewForm (Pointer formDataPtr);
extern void LIBCALL ExportMedlineViewFormToFile (Pointer formDataPtr, CharPtr filename);


#ifdef __cplusplus
}
#endif

#endif /* ndef _MEDVIEW_ */

