/*   styleman.c
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
* File Name:  styleman.c
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
#define SM_DLG_UP  1
#define SM_DLG_OK  2
#define SM_DLG_DIS 4

/**************************************************************************/
/* TYPEDEFS */
/**************************************************************************/

/**************************************************************************/
/* STATIC VARIABLE */
/**************************************************************************/
static Int2    managerStatus = 0;
static WindoW  window;
static PrompT  currentPrompt;
static LisT    namesList;
static ButtoN  modifyButton;
static ButtoN  deleteButton;
static Char    promptVal[64];

static Int2    nsdStatus = 0;
static WindoW  nsdWindow = NULL;

static CharPtr winTitle[] = {
" Loading . . . .", ". Loading . . .", ". . Loading . .",
". . . Loading .", ". . . . Loading ", NULL
};
static Int2    curTitle = 0;

/**************************************************************************/
/* STATIC FUNCTION */
/**************************************************************************/

/**************************************************************************
* MakePromptVal()
**************************************************************************/
static CharPtr
/*FCN*/MakePromptVal (
 void
){
  StringCpy ( promptVal, "Current style: " );
  StringCat ( promptVal, Nlm_GetMuskStyleName(Nlm_GetMuskCurrentSt()) );
  promptVal[40] = 0;
  return &(promptVal[0]);
} /* End of MakePromptVal() */

/**************************************************************************
* ReloadNames()
**************************************************************************/
static void 
/*FCN*/ReloadNames (
  void
){
  Int2 i,total;

  Hide(namesList);
  Reset(namesList);
  total = Nlm_GetMuskTotalSt();
  for ( i=0; i<total; i++ ) {
    ListItem ( namesList, Nlm_GetMuskStyleName(i) );
  }
  SetValue ( namesList, Nlm_GetMuskCurrentSt()+1 );
  Show(namesList);
  if ( Nlm_GetMuskCurrentSt() == 0 ){
    Disable ( modifyButton );
    Disable( deleteButton );
  } else {
    Enable ( modifyButton );
    Enable ( deleteButton );
  }
} /* End of ReloadNames() */

/**************************************************************************
* OnSelectName()
**************************************************************************/
static void 
/*FCN*/OnSelectName (
  LisT l
){
  Int2 i;

  i = GetValue ( l );
  if ( i == 1 ){
    Disable ( modifyButton );
    Disable( deleteButton );
  } else {
    Enable ( modifyButton );
    Enable ( deleteButton );
  }
} /* End of OnSelectName() */

/**************************************************************************
* OnCloseButton()
**************************************************************************/
static void 
/*FCN*/OnCloseButton (
  ButtoN b
){
  if ( (managerStatus & SM_DLG_DIS) == 0 ){
    managerStatus = 0;
  }
} /* End of OnCloseButton() */

/**************************************************************************
* OnSetCurrentButton()
**************************************************************************/
static void 
/*FCN*/OnSetCurrentButton (
  ButtoN b
){
  if ( managerStatus & SM_DLG_DIS ) return;
  Nlm_SetMuskCurrentSt ( Nlm_GetMuskStyleName (GetValue (namesList)-1));
  SetTitle ( currentPrompt, MakePromptVal() );
} /* End of OnSetCurrentButton() */

/**************************************************************************
* OnOKButton()
**************************************************************************/
static void 
/*FCN*/OnOKButton (
  ButtoN b
){
  nsdStatus = SM_DLG_OK;
} /* End of OnOKButton() */

/**************************************************************************
* OnCancelButton()
**************************************************************************/
static void 
/*FCN*/OnCancelButton (
  ButtoN b
){
  nsdStatus = 0;
} /* End of OnCancelButton() */

/**************************************************************************
* NewStyleDialog()
**************************************************************************/
static Boolean
/*FCN*/NewStyleDialog (
  Int2 srcStyle
){
  GrouP g, tg;
  TexT  nsdDialogText;
  Char  newName[64];

  if ( nsdStatus & SM_DLG_UP ) return FALSE;
  nsdStatus = SM_DLG_UP;
  nsdWindow = FixedWindow (-50, -33, -10, -10, "Copy Style", NULL );
  g = HiddenGroup ( nsdWindow, 1, 2, NULL); 
  tg = HiddenGroup ( g, 2, 1, NULL); 
  StaticPrompt ( tg, "To:", 0, 0, systemFont, 'l');
  nsdDialogText = DialogText ( tg, "", 20, NULL );
  tg = HiddenGroup ( g, 2, 1, NULL);
  SetGroupMargins ( tg, 7*Nlm_stdCharWidth, 5 );
  PushButton ( tg, "OK", OnOKButton ),
  PushButton ( tg, "Cancel", OnCancelButton );
  RealizeWindow ( nsdWindow );
  Show ( nsdWindow );
  Select ( nsdWindow );
  Nlm_WaitForCondition ( nsdStatus & SM_DLG_UP);
  GetTitle ( nsdDialogText, newName, 64 );
  newName[63] = 0;
  Nlm_ProcessAnEvent ();
  Hide ( nsdWindow );
  Remove ( nsdWindow );
  Select ( window );
  if ( nsdStatus & SM_DLG_OK ){
    if ( Nlm_CopyMuskStyle ( newName, srcStyle ) != -1 ) 
      return TRUE;
    else Message ( MSG_ERROR, "Can not create style %s", newName );
  }
  return FALSE;
} /*End of NewStyleDialog() */

/**************************************************************************
* OnNewButton()
**************************************************************************/
static void 
/*FCN*/OnNewButton (
  ButtoN b
){
  if ( managerStatus & SM_DLG_DIS ) return;
  managerStatus |= SM_DLG_DIS;
  if ( NewStyleDialog ( GetValue (namesList)-1 )){
    ReloadNames();
  }
  managerStatus &= ~SM_DLG_DIS;
} /* End of OnNewButton() */

/**************************************************************************
* OnModifyButton()
**************************************************************************/
static void 
/*FCN*/OnModifyButton (
  ButtoN b
){
  Int2    styleNum;
  Boolean save = FALSE;

  if ( managerStatus & SM_DLG_DIS ) return;
  managerStatus |= SM_DLG_DIS;
  styleNum = GetValue (namesList)-1 ;
  Nlm_SetMuskManagetTitle ( FALSE );
  if ( Nlm_EditMuskStyle ( styleNum ) ){
    Nlm_SetMuskManagetTitle ( FALSE );
    if ( Nlm_MuskStyleDialog(Nlm_GetMuskStyleName(styleNum)) ) save = TRUE;
    Nlm_FreeMuskStyleEd ( save );
  }
  managerStatus &= ~SM_DLG_DIS;
} /* End of OnModifyButton() */

/**************************************************************************
* OnDeleteButton()
**************************************************************************/
static void 
/*FCN*/OnDeleteButton (
  ButtoN b
){
  Int2     styleNum;
  CharPtr  styleName;

  if ( managerStatus & SM_DLG_DIS ) return;
  managerStatus |= SM_DLG_DIS;
  styleNum = GetValue ( namesList ) - 1;
  styleName = Nlm_GetMuskStyleName(styleNum);
  if ( Message ( MSG_YN, "Delete style %s ?", styleName ) == ANS_YES ){
    if ( Nlm_DeleteMuskStyle (styleNum) ){
      ReloadNames();
    } else Message ( MSG_ERROR, "Can not delete current style" );
  }
  managerStatus &= ~SM_DLG_DIS;
} /* End of OnDeleteButton() */

/**************************************************************************/
/* GLOBAL FUNCTIONS */
/**************************************************************************/

/**************************************************************************
* Nlm_MuskStyleManager()
*   call manager
**************************************************************************/
void LIBCALL Nlm_MuskStyleManager (
  void
){
  GrouP tg,bg,lg,rg;

  if ( managerStatus & SM_DLG_UP ) {
    Hide(window);
    Show(window);
    return;
  }
  managerStatus = SM_DLG_UP;
  window = FixedWindow (-50, -33, -10, -10, "Style manager", NULL);
  tg = HiddenGroup (window, 2, 1, NULL); 
  lg = HiddenGroup (tg, 1, 2, NULL); 
  StaticPrompt (lg, "Styles:", 0, 0, systemFont, 'l');
  namesList = SingleList (lg, 15, 8, OnSelectName);
  rg = HiddenGroup (tg, 1, 6, NULL); 
  PushButton (rg, "Close", OnCloseButton );
  StaticPrompt (rg, "", 0, 0, systemFont, 'l');
  PushButton (rg, "Set", OnSetCurrentButton);
  PushButton (rg, "Copy...", OnNewButton);
  modifyButton = PushButton (rg, "Modify...", OnModifyButton);
  deleteButton = PushButton (rg, "Delete", OnDeleteButton);
  bg = HiddenGroup (tg, 1, 1, NULL); 
  currentPrompt = StaticPrompt (bg, MakePromptVal(), 0, 0, systemFont, 'l');
  ReloadNames();
  RealizeWindow (window);
  Show (window);
  Select (window);
  Nlm_WaitForCondition ( managerStatus & SM_DLG_UP);
  Nlm_ProcessAnEvent ();
  Hide (window);  
  Remove (window);
} /* End of Nlm_MuskStyleManager() */

void
/*FCN*/Nlm_SetMuskManagetTitle ( 
  Boolean resetTitle
){
  if ( resetTitle ){
    SetTitle ( (Handle)window, "Style manager" );
    curTitle = 0;
  } else {
    SetTitle ( (Handle)window, winTitle[curTitle++] );
    if ( winTitle[curTitle] == NULL ) curTitle = 0;
  }
}

#ifdef TEST_STYLE

static TexT dlg1, dlg2, dlg3, dlg4;

static void OnManager ( ButtoN b ) {
  Nlm_MuskStyleManager();
}

static void OnGetParam ( ButtoN b ) {
 Int2 cl, subcl, tp;
 Int4 param;
 Char buf[30];
 int  xxx;

 GetTitle ( dlg1, buf, 30 ); buf[29] = 0;
 sscanf ( buf, "%d", &xxx ); cl = (Int2)xxx;
 GetTitle ( dlg2, buf, 30 ); buf[29] = 0;
 sscanf ( buf, "%d", &xxx ); subcl = (Int2)xxx;
 GetTitle ( dlg3, buf, 30 ); buf[29] = 0;
 sscanf ( buf, "%d", &xxx ); tp = (Int2)xxx;

 param = GetMuskCParam ( cl, subcl, tp );

 sprintf ( buf, "%ld", (long)param );
 SetTitle ( dlg4, buf );
}

static void OnQuit ( ButtoN b ) {
  ExitMuskStyles();
  QuitProgram();
}

Int2 Main ()
{
  WindoW w;
  w = FixedWindow(-50,-50,-10,-10,"Test",NULL);
  dlg1 = DialogText ( w, "", 10, NULL );
  dlg2 = DialogText ( w, "", 10, NULL );
  dlg3 = DialogText ( w, "", 10, NULL );
  dlg4 = DialogText ( w, "", 10, NULL );
  PushButton( w, "Manager", OnManager);
  PushButton( w, "GetParam", OnGetParam);
  PushButton( w, "Quit", OnQuit);
  Show(w);
  ProcessEvents();
  return 0;
}

#endif

/* END */
