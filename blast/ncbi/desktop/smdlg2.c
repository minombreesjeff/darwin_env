/*   smdlg2.c
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
* File Name:  smdlg2.c
*
* Author:  Alex Smirnov
*
* Version Creation Date:   8/10/95
*
* $Revision: 6.1 $
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

/**************************************************************************/
/* TYPEDEFS */
/**************************************************************************/
typedef struct a_group {
  struct a_group PNTR next;
  Char           GroupName[64];
  Uint1          total;
  Uint1          hide;
  Uint1          farray[100];
} Nlm_AGroup, PNTR Nlm_AGroupPtr;


/**************************************************************************/
/* EXTERN VARIABLE */
/**************************************************************************/
extern CharPtr mSM_allfeatureNames[];

/**************************************************************************/
/* STATIC VARIABLE */
/**************************************************************************/
static ButtoN groupDelButton;
static ButtoN groupHideButton;
static ButtoN groupCompBox;
static ButtoN newButton;
static TexT   groupDialog;
static LisT   groupList;
static LisT   featList;
static ButtoN featHideButton;
static GrouP  featGroup;
static PopuP  featPopup;

static Int2   curGroupInd;
static Int2   totalGroups;
static Int2   nHideGroups;
static Int2   curFeatureInd;
static Int2   totalFeatures;
static Int2   nHideFeatures;
static Int2   groupAr[10];
static Int2   featAr[100];

static Boolean wasModified = FALSE;
static CharPtr unknownName = "<unknown>";

/**************************************************************************/
/* STATIC FUNCTION */
/**************************************************************************/
static void
/*FCN*/LoadGroupArray(
  void
){
  Int2 i,j,k;

  curGroupInd = 0;
  totalGroups = (Int2)Nlm_GetMuskCParamEd(MSM_GROUPS,MSM_NOSUBCLASS,MSM_NUM);
  nHideGroups = 0;
  for ( i=1; i<=totalGroups; i++ ){
    j = (Int2)Nlm_GetMuskCParamEd(MSM_GROUPS,i,MSM_NUM );
    if ( j != 0 ) {
      groupAr[j-1] = i;
      nHideGroups++;
    }
  }
  k = nHideGroups;
  for ( i=1; i<=totalGroups; i++ ){
    j = (Int2)Nlm_GetMuskCParamEd(MSM_GROUPS,i,MSM_NUM );
    if ( j == 0 ) groupAr[k++] = i;
  }
}

static void
/*FCN*/SaveGroupArray (
  void
){
  Int2 i;

  for (i=0; i<nHideGroups; i++ ){
    Nlm_SetMuskCParamEd(MSM_GROUPS, groupAr[i], MSM_NUM, (BigScalar)(i+1) );
  }
  for (i=nHideGroups; i<totalGroups; i++ ){
    Nlm_SetMuskCParamEd(MSM_GROUPS, groupAr[i], MSM_NUM, 0 );
  }
  wasModified = TRUE;
}

static void
/*FCN*/LoadFeatureArray(
  void
){
  Int2 i,j,k;

  nHideFeatures = 0;
  for ( i=1; i<=MSM_TOTAL_POSITIVE; i++ ){
    j = (Int2)Nlm_GetMuskCParamEd(i,MSM_FGROUP,MSM_NUM );
    if ( j==groupAr[curGroupInd] ) {
      j = (Int2)Nlm_GetMuskCParamEd(i,MSM_FORDER,MSM_NUM );
      if ( j != 0 ){
        featAr[j-1] = i;
        nHideFeatures++;
      }
    }
  }
  k = nHideFeatures;
  for ( i=1; i<=MSM_TOTAL_POSITIVE; i++ ){
    j = (Int2)Nlm_GetMuskCParamEd(i,MSM_FGROUP,MSM_NUM );
    if ( j==groupAr[curGroupInd] ) {
      j = (Int2)Nlm_GetMuskCParamEd(i,MSM_FORDER,MSM_NUM );
      if ( j == 0 ) featAr[k++] = i;
    }
  }
  totalFeatures = k;
}

static void
/*FCN*/SaveFeatureArray(
  void
){
  Int2 i;

  for (i=0; i<nHideFeatures; i++ ){
    Nlm_SetMuskCParamEd(featAr[i],MSM_FORDER,MSM_NUM,(BigScalar)(i+1) );
  }
  for (i=nHideFeatures; i<totalFeatures; i++ ){
    Nlm_SetMuskCParamEd(featAr[i],MSM_FORDER,MSM_NUM,0 );
  }
  wasModified = TRUE;
}

static void
/*FCN*/UpdateDlgControls (
  void
){
  if ( totalFeatures == 0 ){
    Disable ( featGroup );
    Enable ( groupDelButton );
  } else {
    if ( curFeatureInd < nHideFeatures ) Enable ( featHideButton );
    else Disable ( featHideButton );
    Enable ( featGroup );
    Disable ( groupDelButton );
  }
  if ( totalGroups < 6 ){
    Enable ( newButton );
  } else {
    Disable ( newButton );
  }
  if ( curGroupInd < nHideGroups ) Enable (groupHideButton);
  else Disable ( groupHideButton );
  if ( (Int2)Nlm_GetMuskCParamEd(MSM_GROUPS,groupAr[curGroupInd],MSM_STYLE)
       == MSM_SPREAD ) SetStatus (groupCompBox,FALSE);
  else SetStatus (groupCompBox,TRUE);
}

static void
/*FCN*/FillGroupList (
  Boolean needHide
){
  Int2    i;
  CharPtr name;

  if ( needHide ){
    Hide( groupList );
    Reset ( groupList );
  }
  for ( i=0; i<nHideGroups; i++ ){
    name = (CharPtr)Nlm_GetMuskCParamEd(MSM_GROUPS, groupAr[i],MSM_STRING);
    if ( (name == NULL) || (*name == 0) ) ListItem ( groupList, unknownName );
    else ListItem ( groupList, name );
  }
  if ( nHideGroups != totalGroups ) {
    ListItem ( groupList, "-- Hidden groups: --" );
    for ( ; i<totalGroups; i++ ){
      name = (CharPtr)Nlm_GetMuskCParamEd(MSM_GROUPS, groupAr[i],MSM_STRING);
      if ( (name == NULL) || (*name == 0) ) ListItem ( groupList, unknownName );
      else ListItem ( groupList, name );
    }
  }
  if ( needHide ) {
    Show( groupList);
  }
}

static void
/*FCN*/FillFeatPopup (
  Boolean needHide
){
  Int2    i;
  CharPtr name;

  if ( needHide ){
    Hide( featPopup );
    Reset ( featPopup );
  }
  for ( i=1; i<=totalGroups; i++ ){
    if ( i== groupAr[curGroupInd] ) continue;
    name = (CharPtr)Nlm_GetMuskCParamEd(MSM_GROUPS, i, MSM_STRING);
    if ( (name == NULL) || (*name == 0) ) PopupItem ( featPopup,unknownName );
    else PopupItem ( featPopup, name );
  }
  SetValue ( featPopup, 1 );
  if ( needHide ) {
    Show ( featPopup );
  }
}

static void
/*FCN*/FillFeatList (
  Boolean needHide
){
  Int2    i;
  Int2    feat;
  CharPtr name;

  if ( needHide ){
    Hide( featList );
    Reset ( featList );
  }
  for ( i=0; i<nHideFeatures; i++ ){
    feat = featAr[i];
    name = mSM_allfeatureNames[feat];
    if ( (name==NULL) || (*name=='\0') ) ListItem ( featList, unknownName );
	else ListItem ( featList, name );
  }
  if ( nHideFeatures != totalFeatures ){
    ListItem ( featList, "-- Hidden features --" );
    for ( ; i<totalFeatures; i++ ){
      feat = featAr[i];
      name = mSM_allfeatureNames[feat];
      if ( (name==NULL) || (*name=='\0') ) ListItem ( featList, unknownName );
	  else ListItem ( featList, name );
    }
  }
  if ( needHide ) Show( featList);
}

static void
/*FCN*/SetGroupValue ( 
  Int2 val
){
  if ( val < nHideGroups ) val++;
  else val += 2;
  SetValue ( groupList, val );
}

static void
/*FCN*/SetFeatValue ( 
  Int2 val
){
  if ( val >= 0 ){
    if ( val < nHideFeatures ) val++;
    else val += 2;
    SetValue ( featList, val );
  }
}

static void
/*FCN*/OnGroupTop (
  ButtoN b
){
  Int2 i;

  if ( curGroupInd >= nHideGroups ) nHideGroups++;
  i = groupAr[curGroupInd];
  MemMove ( &(groupAr[1]), &(groupAr[0]), curGroupInd*sizeof(Int2) );
  groupAr[0] = i;
  curGroupInd = 0;
  FillGroupList(TRUE);
  SetGroupValue ( curGroupInd );
  SaveGroupArray();
  UpdateDlgControls();
}

static void
/*FCN*/OnGroupBottom (
  ButtoN b
){
  Int2 i;

  i = groupAr[curGroupInd];
  if ( curGroupInd >= nHideGroups ) {
    MemMove ( &(groupAr[nHideGroups+1]), 
              &(groupAr[nHideGroups]), 
              (curGroupInd-nHideGroups)*sizeof(Int2) );
    nHideGroups++;
  } else {
    MemMove ( &(groupAr[curGroupInd]), 
              &(groupAr[curGroupInd+1]), 
              (nHideGroups-curGroupInd-1)*sizeof(Int2) );
  }
  curGroupInd = nHideGroups-1;
  groupAr[curGroupInd] = i;
  FillGroupList(TRUE);
  SetGroupValue ( curGroupInd );
  SaveGroupArray();
  UpdateDlgControls();
}

static void
/*FCN*/OnGroupNew (
  ButtoN b
){
  Int2 i;
  Char newName[64];

  GetTitle ( groupDialog, &(newName[0]), 64 );
  if ( newName[0] == 0 ) return;
  newName[63] = 0;
  for ( i=1; i<=totalGroups; i++ ){
    if ( StringCmp ( (CharPtr)Nlm_GetMuskCParamEd(MSM_GROUPS,
                      i,MSM_STRING), &(newName[0]) ) == 0 ) return;
  }
  MemMove ( &(groupAr[1]), &(groupAr[0]), totalGroups*sizeof(Int2) );
  totalGroups++;
  nHideGroups++;
  groupAr[0] = totalGroups;
  Nlm_SetMuskCParamEd(MSM_GROUPS,MSM_NOSUBCLASS,MSM_NUM,(BigScalar)totalGroups);
  Nlm_SetMuskCParamEd(MSM_GROUPS,totalGroups,MSM_STRING,(BigScalar)&(newName[0]));
  curGroupInd = 0;
  FillGroupList(TRUE);
  SetGroupValue ( curGroupInd );
  FillFeatPopup(TRUE);
  LoadFeatureArray();
  FillFeatList(TRUE);
  if ( totalFeatures == 0 ){
    curFeatureInd = -1;
  } else {
    curFeatureInd = 0;
    SetFeatValue ( curFeatureInd );
  }
  UpdateDlgControls();
  SaveGroupArray();
}

static void
/*FCN*/OnGroupDelete (
  ButtoN b
){
  Int2      i,j;
  BigScalar k;
  CharPtr   name;

  name = (CharPtr)Nlm_GetMuskCParamEd(MSM_GROUPS,
         groupAr[curGroupInd],MSM_STRING);
  if ( (name == NULL) || (*name == 0) ) name = unknownName;
  if ( Message ( MSG_YN, "Delete group %s ?", name ) == ANS_YES ){
    if ( curGroupInd < nHideGroups ) nHideGroups--;
    i = groupAr[curGroupInd];
    for ( j=1; j<=MSM_TOTAL_POSITIVE; j++ ){
      k = Nlm_GetMuskCParamEd(j,MSM_FGROUP,MSM_NUM);
      if ( k > i ) {
        Nlm_SetMuskCParamEd(j,MSM_FGROUP,MSM_NUM,k-1);
      }
    }
    for ( j=i+1; j<=totalGroups; j++ ){
      k = Nlm_GetMuskCParamEd(MSM_GROUPS,j,MSM_TRUEFALSE);
      Nlm_SetMuskCParamEd(MSM_GROUPS,(Int2)(j-1),MSM_TRUEFALSE,k);
      k = Nlm_GetMuskCParamEd(MSM_GROUPS,j,MSM_STYLE);
      Nlm_SetMuskCParamEd(MSM_GROUPS,(Int2)(j-1),MSM_STYLE,k);
      k = Nlm_GetMuskCParamEd(MSM_GROUPS,j,MSM_STRING);
      Nlm_SetMuskCParamEd(MSM_GROUPS,(Int2)(j-1),MSM_STRING,k);
    }
    Nlm_SetMuskCParamEd(MSM_GROUPS,totalGroups,MSM_STRING,(BigScalar)NULL);
    for ( j=0; j<totalGroups; j++ ){
      if ( groupAr[j] > i )  groupAr[j] -= 1;
    }
    if ( curGroupInd != (totalGroups-1) ){
      MemMove ( &(groupAr[curGroupInd]), &(groupAr[curGroupInd+1]), 
                (totalGroups-curGroupInd-1)*sizeof(Int2) );
    } else curGroupInd--;
    totalGroups--;
	Nlm_SetMuskCParamEd(MSM_GROUPS,MSM_NOSUBCLASS,MSM_NUM,(BigScalar)totalGroups);
    FillGroupList(TRUE);
    SetGroupValue ( curGroupInd );
    FillFeatPopup(TRUE);
    LoadFeatureArray();
    FillFeatList(TRUE);
    if ( totalFeatures == 0 ){
      curFeatureInd = -1;
    } else {
      curFeatureInd = 0;
      SetFeatValue ( curFeatureInd );
    }
    SaveGroupArray();
    UpdateDlgControls();
  }
}

static void
/*FCN*/OnGroupHide (
  ButtoN b
){
  Int2 i;

  i = groupAr[curGroupInd];
  if ( curGroupInd >= nHideGroups ) return;
  MemMove ( &(groupAr[curGroupInd]), 
            &(groupAr[curGroupInd+1]), 
            (nHideGroups-curGroupInd-1)*sizeof(Int2) );
  nHideGroups--;
  groupAr[nHideGroups] = i;
  curGroupInd = nHideGroups;
  FillGroupList(TRUE);
  SetGroupValue ( curGroupInd );
  SaveGroupArray();
  UpdateDlgControls();
}

static void
/*FCN*/OnGroupCompress (
  ButtoN b
){
  if ( GetStatus(b) ){
    Nlm_SetMuskCParamEd(MSM_GROUPS,groupAr[curGroupInd],MSM_STYLE,
                        MSM_COMPRESS);
  } else {
    Nlm_SetMuskCParamEd(MSM_GROUPS,groupAr[curGroupInd],MSM_STYLE,MSM_SPREAD);
  }
}

static void
/*FCN*/OnGroupList (
  LisT l
){
  curGroupInd = GetValue ( groupList ) - 1;
  if ( curGroupInd == nHideGroups ) {
    SetValue ( (Handle)groupList, (Int2)(curGroupInd+2));
    curGroupInd = GetValue ( groupList ) - 1;
  }
  if ( curGroupInd > nHideGroups ) curGroupInd--;
  LoadFeatureArray();
  FillFeatList(TRUE);
  FillFeatPopup(TRUE);
  if ( totalFeatures == 0 ){
    curFeatureInd = -1;
  } else {
    curFeatureInd = 0;
    SetFeatValue ( curFeatureInd );
  }
  UpdateDlgControls();
}

static void
/*FCN*/OnFeatureList (
  LisT l
){
  curFeatureInd = GetValue ( featList ) - 1;
  if ( curFeatureInd == nHideFeatures ) {
    SetValue( (Handle)featList, (Int2)(curFeatureInd+2) );
    curFeatureInd = GetValue ( featList ) - 1;
  }
  if ( curFeatureInd > nHideFeatures ) curFeatureInd--;
  UpdateDlgControls();
}

static void
/*FCN*/OnFeatTop (
  ButtoN b
){
  Int2 i;

  if ( curFeatureInd >= nHideFeatures ) nHideFeatures++;
  i = featAr[curFeatureInd];
  MemMove ( &(featAr[1]), &(featAr[0]), curFeatureInd*sizeof(Int2) );
  featAr[0] = i;
  curFeatureInd = 0;
  FillFeatList(TRUE);
  SetFeatValue ( curFeatureInd );
  SaveFeatureArray();
  UpdateDlgControls();
}

static void
/*FCN*/OnFeatBottom (
  ButtoN b
){
  Int2 i;

  i = featAr[curFeatureInd];
  if ( curFeatureInd >= nHideFeatures ) {
    MemMove ( &(featAr[nHideFeatures+1]), 
              &(featAr[nHideFeatures]), 
              (curFeatureInd-nHideFeatures)*sizeof(Int2) );
    nHideFeatures++;
  } else {
    MemMove ( &(featAr[curFeatureInd]), 
              &(featAr[curFeatureInd+1]), 
              (nHideFeatures-curFeatureInd-1)*sizeof(Int2) );
  }
  curFeatureInd = nHideFeatures-1;
  featAr[curFeatureInd] = i;
  FillFeatList(TRUE);
  SetFeatValue ( curFeatureInd );
  SaveFeatureArray();
  UpdateDlgControls();
}

static void
/*FCN*/OnFeatMove (
  ButtoN b
){
  Int2 i,j,k;
  Int2 movFeat;

  movFeat = featAr[curFeatureInd];
  i = GetValue ( featPopup );
  if ( groupAr[curGroupInd] <= i ) i++;
  MemMove ( &(featAr[curFeatureInd]), &(featAr[curFeatureInd+1]),
            (totalFeatures - curFeatureInd - 1)*sizeof(Int2) );
  if ( curFeatureInd >= nHideFeatures ){
    Nlm_SetMuskCParamEd(movFeat,MSM_FGROUP,MSM_NUM,(BigScalar)i);
  } else {
    nHideFeatures--;
    for ( j=1; j<=MSM_TOTAL_POSITIVE; j++ ){
      if ( (Int2)Nlm_GetMuskCParamEd(j,MSM_FGROUP,MSM_NUM ) == i ){
        k = (Int2)Nlm_GetMuskCParamEd(j,MSM_FORDER,MSM_NUM );
        if ( k > 0 ){
          Nlm_SetMuskCParamEd(j,MSM_FORDER,MSM_NUM,(BigScalar)(k+1));
        }
      }
    }
    Nlm_SetMuskCParamEd(movFeat,MSM_FGROUP,MSM_NUM,(BigScalar)i);
    Nlm_SetMuskCParamEd(movFeat,MSM_FORDER,MSM_NUM,1);
  }
  totalFeatures--;
  if ( curFeatureInd == totalFeatures ) curFeatureInd--;
  FillFeatList(TRUE);
  SetFeatValue ( curFeatureInd );
  SaveFeatureArray();
  UpdateDlgControls();
}

static void
/*FCN*/OnFeatHide (
  ButtoN b
){
  Int2 i;

  i = featAr[curFeatureInd];
  if ( curFeatureInd >= nHideFeatures ) return;
  MemMove ( &(featAr[curFeatureInd]), 
            &(featAr[curFeatureInd+1]), 
            (nHideFeatures-curFeatureInd-1)*sizeof(Int2) );
  nHideFeatures--;
  featAr[nHideFeatures] = i;
  curFeatureInd = nHideFeatures;
  FillFeatList(TRUE);
  SetFeatValue ( curFeatureInd );
  SaveFeatureArray();
  UpdateDlgControls();
}

static void 
/*FCN*/OnText16 ( 
  TexT t 
){
  Char curVal[20];

  GetTitle ( t, &(curVal[0]), 20 );
  curVal[19] = 0;
  if ( StringLen ( &(curVal[0]) ) > 16 ){
    Nlm_Beep();
    curVal[17] = 0;
    SetTitle ( t, &(curVal[0]) );
  }
}


/***************************************************************************/
/* GLOBAL FUNCTIONS */
/**************************************************************************/
GrouP 
/*FCN*/Nlm_CreateDlgGroup ( 
  GrouP gTop
){
  GrouP    g, glr, g1, g2;
  Handle   toAlign[3];

  g = HiddenGroup ( gTop, 2, 1, NULL);
  SetGroupSpacing ( g, 1, 1 );
  glr = NormalGroup ( g, 1, 2, "Groups:", systemFont, NULL );
#ifdef WIN_MOTIF
  SetGroupSpacing ( glr, 12, 12 );
  SetGroupMargins ( glr, 8, 8 );
#else
  SetGroupSpacing ( glr, 6, 6 );
  SetGroupMargins ( glr, 6, 6 );
#endif
  g1 = HiddenGroup ( glr, 2, 1, NULL );
  g2 = HiddenGroup ( g1, 1, 0, NULL );
  PushButton ( g2, "Top", OnGroupTop );
  PushButton ( g2, "Bottom", OnGroupBottom );
  groupHideButton = PushButton ( g2, "Hide", OnGroupHide );
  groupCompBox = CheckBox ( g2, "Compress", OnGroupCompress );
  groupDelButton = PushButton ( g2, "Delete", OnGroupDelete );
  groupList = SingleList ( g1, 10, 8, OnGroupList );
  curGroupInd = 0;
  LoadGroupArray();
  g2 = HiddenGroup ( glr, 3, 0, NULL );
  toAlign[2] = (Handle)PushButton ( g2, "Create", OnGroupNew );
  toAlign[0] = (Handle)StaticPrompt ( g2, "Group", 0, 0, systemFont, 'l');
#ifdef WIN_MOTIF
  groupDialog = DialogText ( g2, "", 7, OnText16 );
#else
  groupDialog = DialogText ( g2, "", 9, OnText16 );
#endif
  toAlign[1] = (Handle)groupDialog;
  AlignObjects ( ALIGN_CENTER, (HANDLE)g1, (HANDLE)g2, NULL );

  featGroup = glr = NormalGroup ( g, 1, 2, "Features:", systemFont, NULL );
#ifdef WIN_MOTIF
  SetGroupSpacing ( glr, 12, 12 );
  SetGroupMargins ( glr, 8, 8 );
#else
  SetGroupSpacing ( glr, 6, 6 );
  SetGroupMargins ( glr, 6, 6 );
#endif
  g1 = HiddenGroup ( glr, 2, 1, NULL );
  featList = SingleList ( g1, 10, 8, OnFeatureList );
  LoadFeatureArray();
  FillFeatList( FALSE );
  if ( totalFeatures == 0 ){
    curFeatureInd = -1;
  } else {
    curFeatureInd = 0;
    SetValue ( featList, 1 );
  }
  g2 = HiddenGroup ( g1, 1, 0, NULL );
  PushButton ( g2, "Top", OnFeatTop );
  PushButton ( g2, " Bottom ", OnFeatBottom );
  featHideButton = PushButton ( g2, "Hide", OnFeatHide );
  g2 = HiddenGroup ( glr, 3, 0, NULL );
#ifdef WIN_MOTIF
  SetGroupMargins ( g2, 8, 8 );
#endif
  toAlign[0] = (Handle)PushButton ( g2, "Move", OnFeatMove );
  toAlign[1] = (Handle)StaticPrompt ( g2, "To", 0, 0, systemFont, 'l');
  featPopup = PopupList ( g2, FALSE, NULL );
  toAlign[2] = (Handle)featPopup;
  FillGroupList(FALSE);
  SetGroupValue (curGroupInd);
  FillFeatPopup(FALSE);
  UpdateDlgControls ();
  return g;
}



void
/*FCN*/Nlm_ToolDlgGroup ( 
  Int2 tools
){
}

void  Nlm_SetTDefGroup (
  void
){
  Nlm_ShowToolsSM (0);
}

Boolean
/*FCN*/Msk_OrderWasModified (
  void
){
  if ( wasModified == TRUE ){
    wasModified = FALSE;
    return TRUE;
  }
  return FALSE;
}

/* END */
