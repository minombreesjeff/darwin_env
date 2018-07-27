/*   stylemui.c
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
* File Name:  stylemui.c
*
* Author:  Alex Smirnov
*
* Version Creation Date:   8/10/95
*
* $Revision: 6.0 $
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

/**************************************************************************/
/* INCLUDE */
/**************************************************************************/
#include <vibrant.h>
#include <picture.h> 
#include <fstyle.h> 
#include <fstylep.h> 

/**************************************************************************/
/* DEFINES */
/**************************************************************************/
#define SM_DLG_UP     1
#define SM_DLG_OK     2
#define SM_DLG_DIS    4

#define FOLDER_NUMBER 6

/**************************************************************************/
/* TYPEDEFS */
/**************************************************************************/
typedef GrouP (*Nlm_CreateDlg)    (GrouP g);
typedef void  (*Nlm_SetToolsProc) (void);
typedef struct {
  Nlm_CreateDlg    crt;
  Nlm_SmToolsProc  toolProc;
  Nlm_SetToolsProc setDef;
  GrouP            g;
} Nlm_DlgFolder, PNTR DlgFolderPtr;

/**************************************************************************/
/* STATIC VARIABLE */
/**************************************************************************/
static Int2   wStatus = 0;
static DialoG dFolderTabs;
static CharPtr folderNames[FOLDER_NUMBER+1] = {
"Common","Groups","Seq","Features", "Extra", "Cycle", NULL
};
static Nlm_DlgFolder folderGroups[FOLDER_NUMBER] = {
{ Nlm_CreateDlgCommon, Nlm_ToolDlgCommon, Nlm_SetTDefCommon, 0},
{ Nlm_CreateDlgGroup, Nlm_ToolDlgGroup, Nlm_SetTDefGroup, 0}, 
{ Nlm_CreateDlgSeq, Nlm_ToolDlgSeq, Nlm_SetTDefSeq, 0 }, 
{ Nlm_CreateDlgFeatures, Nlm_ToolDlgFeatures, Nlm_SetTDefFeatures, 0 }, 
{ Nlm_CreateDlgExtra, Nlm_ToolDlgExtra, Nlm_SetTDefExtra, 0}, 
{ Nlm_CreateDlgCColor, Nlm_ToolDlgCColor, Nlm_SetTDefCColor, 0}
};
static Int2   curPage;
static GrouP  toolsGroup;
static GrouP  buttonGroup;

/**************************************************************************/
/* STATIC FUNCTION */
/**************************************************************************/

static void 
/*FCN*/OnOKButton (
  ButtoN b
){
  if ( wStatus & SM_DLG_DIS ) return;
  wStatus = SM_DLG_OK;
} /* End of OnOKButton() */

static void 
/*FCN*/OnCancelButton (
  ButtoN b
){
  if ( wStatus & SM_DLG_DIS ) return;
  wStatus = 0;
} /* End of OnCancelButton() */

static void 
/*FCN*/OnChangePage ( 
  VoidPtr data, 
  Int2 newVal, 
  Int2 oldVal 
){
  folderGroups[newVal].setDef();
  SafeHide(folderGroups[oldVal].g);
  SafeShow(folderGroups[newVal].g);
  curPage = newVal;
  Nlm_SetToolsCallbackSM ( folderGroups[newVal].toolProc );
} /* End of OnChangePage () */

/**************************************************************************/
/* GLOBAL FUNCTIONS */
/**************************************************************************/

/**************************************************************************
* Nlm_MuskStyleManager()
*   call manager
**************************************************************************/
Boolean Nlm_MuskStyleDialog (
  CharPtr styleName
){
  WindoW window;
  GrouP  g1, g2, g3;
  Int2   i;
  Char   wName[64];

  if ( wStatus & SM_DLG_UP ) return FALSE;
  Nlm_SetMuskManagetTitle (FALSE);
  wStatus = SM_DLG_UP;
  StringCpy ( &(wName[0]), "Edit style: " );
  StringCat ( &(wName[0]), styleName );
  window = FixedWindow (-33, -33,  -10, -10, &(wName[0]), NULL );
  g1 = HiddenGroup ( window, 1, 0, NULL );
  dFolderTabs = CreateFolderTabs ( g1, folderNames, 0, 0, 0,
                                   SYSTEM_FOLDER_TAB, OnChangePage, NULL );
  curPage = 0;
  Nlm_SetMuskManagetTitle (FALSE);
  g2 = HiddenGroup ( window, 0, 0, NULL );
  for ( i=1 ; i<FOLDER_NUMBER; i++ ){
    folderGroups[i].g = folderGroups[i].crt ( g2 );
    Hide ( folderGroups[i].g );
    Nlm_SetMuskManagetTitle (FALSE);
  }
  folderGroups[0].g = folderGroups[0].crt ( g2 );
  Nlm_SetMuskManagetTitle (FALSE);
  toolsGroup = Nlm_CtreateToolsSM ( window );
  buttonGroup = g3 = HiddenGroup ( window, -2, 1, NULL );
  PushButton ( g3, "OK", OnOKButton );
  PushButton ( g3, "Cancel", OnCancelButton );
#ifndef WIN_MOTIF
  AlignObjects ( ALIGN_CENTER, (HANDLE)g1, (HANDLE)g2,
                (HANDLE)dFolderTabs, 
                (HANDLE)g3, 
                (HANDLE)folderGroups[0].g,
                (HANDLE)folderGroups[1].g,
                (HANDLE)folderGroups[2].g,
                (HANDLE)folderGroups[3].g,
                (HANDLE)folderGroups[4].g,
                (HANDLE)folderGroups[5].g,
                NULL );
#else
  AlignObjects ( ALIGN_CENTER, (HANDLE)g1, (HANDLE)g2,
                (HANDLE)dFolderTabs, 
                (HANDLE)g3, 
                (HANDLE)folderGroups[0].g,
                (HANDLE)folderGroups[2].g,
                (HANDLE)folderGroups[5].g,
                NULL );
#endif
  folderGroups[0].setDef();
  Nlm_SetToolsCallbackSM ( folderGroups[0].toolProc );
  RealizeWindow (window);
  Nlm_SetMuskManagetTitle (TRUE);
  Show (window);
  Select (window);
  Nlm_WaitForCondition ( wStatus & SM_DLG_UP);
  Nlm_ProcessAnEvent ();
  Hide (window);  
  Remove (window);
  if (  wStatus & SM_DLG_OK ) return TRUE;
  return FALSE;
} /* End of Nlm_MuskStyleDialog() */

void
/*FCN*/Nlm_EnableSM ( 
  void
){
  wStatus &= ~SM_DLG_DIS;
#ifdef WIN_MOTIF
  Enable ( (Handle)dFolderTabs);
  Enable ( (Handle)folderGroups[curPage].g );
  Enable ( (Handle)toolsGroup );
  Enable ( (Handle)buttonGroup );
#endif
}  /* End of EnableSM() */

void
/*FCN*/Nlm_DisableSM ( 
  void
){
  wStatus |= SM_DLG_DIS;
#ifdef WIN_MOTIF
  Disable ( (Handle)dFolderTabs);
  Disable ( (Handle)folderGroups[curPage].g );
  Disable ( (Handle)toolsGroup );
  Disable ( (Handle)buttonGroup );
#endif
}  /* End of DisableSM() */

/* END */
