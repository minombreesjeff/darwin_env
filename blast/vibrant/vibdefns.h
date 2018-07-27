/*   vibdefns.h
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
* File Name:  vibdefns.h
*
* Author:  Jonathan Kans
*
* Version Creation Date:   7/1/91
*
* $Revision: 6.8 $
*
* File Description: 
*       Vibrant alias definitions
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: vibdefns.h,v $
* Revision 6.8  2001/09/10 17:34:21  bazhin
* Added function Nlm_SetTextCursorBlinkRate(Nlm_TexT t, Nlm_Int2 msec).
*
* Revision 6.7  1999/04/06 14:23:25  lewisg
* add opengl replacement for viewer3d
*
* Revision 6.6  1998/06/01 17:20:13  vakatov
* Added code to draw/fill 90-degree arc/pie (quadrants)
*
* Revision 6.5  1998/03/22 03:00:57  kans
* changed names to RegisterServiceProc and RegisterResultProc
*
* Revision 6.4  1998/03/22 02:33:15  kans
* added request proc, result proc, message handlers to support, and send open doc event, launch app now work with file names or signatures
*
* Revision 6.3  1997/11/12 20:56:06  kans
* added SetMouseMoveCallback and SetMouseMoveRegion, implemented first on Mac
*
* Revision 6.2  1997/10/18 23:30:24  kans
* implemented Nlm_GetTextCursorPos (DV)
*
* Revision 6.1  1997/09/16 20:25:48  vakatov
* +Nlm_FitStringWidth
*
* Revision 5.18  1997/08/04 14:15:09  vakatov
* Added Nlm_SetTextCursorPos() function
*
* Revision 5.17  1997/08/01 16:47:26  vakatov
* [WIN_MOTIF,WIN_MSWIN]  Added IconifyWindow() and IconicWindow()
*
* Revision 5.16  1997/07/23 19:38:21  vakatov
* +#def Nlm_PaintStringEx
*
* Revision 5.15  1997/07/16 19:52:38  vakatov
* + Nlm_StrngPrintable() #define
*
* Revision 5.14  1997/07/10 21:49:28  vakatov
* [WIN_X]  Now able to manage windows having different depths(and
* different visuals and GC).
*
* Revision 5.13  1997/06/23 21:18:44  vakatov
* Added Nlm_SetTextEditable() function to allow/prohibit text editing
*
* Revision 5.12  1997/05/27 21:50:53  vakatov
* Added Nlm_PopupParentWindow() to popup window w/o switching input focus
*
* Revision 5.11  1997/04/25 16:08:01  vakatov
* +SetSlatePolicy
*
 * Revision 5.10  1997/03/19  15:55:20  vakatov
 * Added:  #define QuittingProgram Nlm_QuittingProgram
 *
 * Revision 5.9  1996/12/12  23:11:10  kans
 * added Nlm_ClipPrintingRect (DV)
 *
 * Revision 5.8  1996/09/30  19:58:57  vakatov
 * + #define SetPenDash Nlm_SetPenDash
 *
 * Revision 5.7  1996/09/26  00:29:23  kans
 * added SetWindowTimer
 *
 * Revision 5.6  1996/09/09  00:15:01  kans
 * AutonomousPanel4 uses ScrollBar4 to have > 32K positions
 *
 * Revision 5.5  1996/08/27  20:43:40  vakatov
 * Added defs of new scrollbar functions
 *
 * Revision 5.4  1996/07/25  18:49:54  vakatov
 * Added "#define GetBarMax Nlm_GetBarMax"
 *
 * Revision 5.3  1996/06/19  20:31:53  vakatov
 * #define GetListItem Nlm_GetListItem
 *
 * Revision 5.1  1996/05/31  20:12:39  vakatov
 * Added Nlm_ExtendedList() function to set the list selection policy
 * to EXTENDED_SELECTION -- as it was for Nlm_MultiList() since Revision 4.4.
 *
 * Revision 4.3  1996/03/11  20:39:09  epstein
 * add support for Drag & Drop
 *
 * Revision 4.2  1995/11/08  23:30:31  kans
 * removed edit block fields, which belong in the application
 *
 * Revision 2.48  1995/07/14  17:48:26  kans
 * new CopyPixmap (AS)
*
* ==========================================================================
*/

#ifndef _VIBDEFNS_
#define _VIBDEFNS_

#ifdef __cplusplus
extern "C" {
#endif

/* Interface object types */

#define GraphiC Nlm_GraphiC

#define BaR Nlm_BaR
#define ButtoN Nlm_ButtoN
#define ChoicE Nlm_ChoicE
#define DisplaY Nlm_DisplaY
#define GrouP Nlm_GrouP
#define IcoN Nlm_IcoN
#define IteM Nlm_IteM
#define LisT Nlm_LisT
#define MenU Nlm_MenU
#define PaneL Nlm_PaneL
#define PopuP Nlm_PopuP
#define PrompT Nlm_PrompT
#define RepeaT Nlm_RepeaT
#define SlatE Nlm_SlatE
#define SwitcH Nlm_SwitcH
#define TexT Nlm_TexT
#define WindoW Nlm_WindoW

/* Callback procedure types */

#define VoidProc Nlm_VoidProc
#define KeyProc Nlm_KeyProc
#define ActnProc Nlm_ActnProc
#define ClckProc Nlm_ClckProc
#define CharProc Nlm_CharProc
#define ScrlProc4 Nlm_ScrlProc4
#define ScrlProc Nlm_ScrlProc
#define ChngProc Nlm_ChngProc
#define FreeProc Nlm_FreeProc
#define DropProc Nlm_DropProc
#define ServiceProc Nlm_ServiceProc
#define ResultProc Nlm_ResultProc

#define BtnActnProc Nlm_BtnActnProc
#define ChsActnProc Nlm_ChsActnProc
#define GrpActnProc Nlm_GrpActnProc
#define IcnActnProc Nlm_IcnActnProc
#define ItmActnProc Nlm_ItmActnProc
#define LstActnProc Nlm_LstActnProc
#define MnuActnProc Nlm_MnuActnProc
#define PnlActnProc Nlm_PnlActnProc
#define PupActnProc Nlm_PupActnProc
#define RptActnProc Nlm_RptActnProc
#define TxtActnProc Nlm_TxtActnProc
#define WndActnProc Nlm_WndActnProc
#define WndFreeProc Nlm_WndFreeProc

#define PnlClckProc Nlm_PnlClckProc
#define RptClckProc Nlm_RptClckProc
#define IcnClckProc Nlm_IcnClckProc

#define BarScrlProc4 Nlm_BarScrlProc4
#define BarScrlProc Nlm_BarScrlProc
#define SltScrlProc4 Nlm_SltScrlProc4
#define SltScrlProc Nlm_SltScrlProc

#define IcnChngProc Nlm_IcnChngProc
#define SwtChngProc Nlm_SwtChngProc

#define SltCharProc Nlm_SltCharProc

/* Global variables */

#define fileDone Nlm_fileDone
#define fileError Nlm_fileError
#define termCH Nlm_termCH

#define currentKey Nlm_currentKey

#define cmmdKey Nlm_cmmdKey
#define ctrlKey Nlm_ctrlKey
#define optKey Nlm_optKey
#define shftKey Nlm_shftKey
#define dblClick Nlm_dblClick

#define screenRect Nlm_screenRect

#define hScrollBarHeight Nlm_hScrollBarHeight
#define vScrollBarWidth Nlm_vScrollBarWidth

#define dialogTextHeight Nlm_dialogTextHeight
#define popupMenuHeight Nlm_popupMenuHeight

/* Interface object manipulation procedures */

#define RestrictMotifColorsTo Nlm_RestrictMotifColorsTo

#define SetColorCell          Nlm_SetColorCell

#define DocumentWindow Nlm_DocumentWindow
#define FixedWindow Nlm_FixedWindow
#define FrozenWindow Nlm_FrozenWindow
#define RoundWindow Nlm_RoundWindow
#define AlertWindow Nlm_AlertWindow
#define ModalWindow Nlm_ModalWindow
#define FloatingWindow Nlm_FloatingWindow
#define ShadowWindow Nlm_ShadowWindow
#define PlainWindow Nlm_PlainWindow

/* esl++ */
#define MovableModalWindow Nlm_MovableModalWindow
#define SetModalWindowOwner Nlm_SetModalWindowOwner
#define IsWindowModal Nlm_IsWindowModal

#define SetClose Nlm_SetClose
#define SetActivate Nlm_SetActivate
#define SetDeactivate Nlm_SetDeactivate
#define SetResize Nlm_SetResize
#define SetColorMap Nlm_SetColorMap
#define SetWindowExtra Nlm_SetWindowExtra
#define GetWindowExtra Nlm_GetWindowExtra

#define NormalGroup Nlm_NormalGroup
#define HiddenGroup Nlm_HiddenGroup
#define SetGroupMargins Nlm_SetGroupMargins
#define SetGroupSpacing Nlm_SetGroupSpacing

#define PushButton Nlm_PushButton
#define DefaultButton Nlm_DefaultButton
#define CheckBox Nlm_CheckBox
#define RadioButton Nlm_RadioButton

#define SingleList Nlm_SingleList
#define MultiList Nlm_MultiList
#define ExtendedList Nlm_ExtendedList
#define ListItem Nlm_ListItem
#define GetListItem Nlm_GetListItem

#define PulldownMenu Nlm_PulldownMenu
#define AppleMenu Nlm_AppleMenu
#define PopupMenu Nlm_PopupMenu
#define SubMenu Nlm_SubMenu
#define CommandItem Nlm_CommandItem
#define StatusItem Nlm_StatusItem
#define ChoiceGroup Nlm_ChoiceGroup
#define ChoiceItem Nlm_ChoiceItem
#define PopupList Nlm_PopupList
#define PopupItem Nlm_PopupItem
#define PopupItems Nlm_PopupItems
#define DeskAccGroup Nlm_DeskAccGroup
#define FontGroup Nlm_FontGroup
#define SeparatorItem Nlm_SeparatorItem

#define ScrollBar4 Nlm_ScrollBar4
#define ScrollBar Nlm_ScrollBar
#define SetBarValue Nlm_SetBarValue
#define CorrectBarValue Nlm_CorrectBarValue
#define GetBarValue Nlm_GetBarValue
#define SetBarMax Nlm_SetBarMax
#define CorrectBarMax Nlm_CorrectBarMax
#define GetBarMax Nlm_GetBarMax
#define CorrectBarPage Nlm_CorrectBarPage

#define RepeatButton Nlm_RepeatButton

#define IconButton Nlm_IconButton

#define UpDownSwitch Nlm_UpDownSwitch
#define LeftRightSwitch Nlm_LeftRightSwitch
#define SetSwitchMax Nlm_SetSwitchMax
#define GetSwitchMax Nlm_GetSwitchMax
#define SetSwitchParams Nlm_SetSwitchParams

#define DialogText Nlm_DialogText
#define HiddenText Nlm_HiddenText
#define SpecialText Nlm_SpecialText
#define PasswordText Nlm_PasswordText
#define ScrollText Nlm_ScrollText
#define SetTextSelect Nlm_SetTextSelect
#define CurrentText Nlm_CurrentText
#define TextSelectionRange Nlm_TextSelectionRange
#define CutText Nlm_CutText
#define CopyText Nlm_CopyText
#define PasteText Nlm_PasteText
#define ClearText Nlm_ClearText
#define TextLength Nlm_TextLength
#define SelectText Nlm_SelectText
#define SetTextEditable Nlm_SetTextEditable
#define SetTextCursorPos Nlm_SetTextCursorPos
#define GetTextCursorPos Nlm_GetTextCursorPos
#define SetTextCursorBlinkRate Nlm_SetTextCursorBlinkRate

#define StaticPrompt Nlm_StaticPrompt

#define NormalDisplay Nlm_NormalDisplay
#define ScrollDisplay Nlm_ScrollDisplay

#define SimplePanel Nlm_SimplePanel
#define AutonomousPanel4 Nlm_AutonomousPanel4
#define AutonomousPanel Nlm_AutonomousPanel
#ifdef _OPENGL
#define Autonomous3DPanel Nlm_Autonomous3DPanel
#endif /* _OPENGL */
#define SetPanelClick Nlm_SetPanelClick
#define SetPanelExtra Nlm_SetPanelExtra
#define GetPanelExtra Nlm_GetPanelExtra

#define ScrollSlate Nlm_ScrollSlate
#define NormalSlate Nlm_NormalSlate
#define HiddenSlate Nlm_HiddenSlate
#define GeneralSlate Nlm_GeneralSlate
#define VirtualSlate Nlm_VirtualSlate
#define RegisterRow Nlm_RegisterRow
#define RegisterColumn Nlm_RegisterColumn
#define GetSlateVScrollBar Nlm_GetSlateVScrollBar
#define GetSlateHScrollBar Nlm_GetSlateHScrollBar

#define CustomPanel Nlm_CustomPanel
#define GeneralPanel Nlm_GeneralPanel
#define RegisterRect Nlm_RegisterRect

#define SetSlateChar Nlm_SetSlateChar
#define CaptureSlateFocus Nlm_CaptureSlateFocus
#define SetSlatePolicy Nlm_SetSlatePolicy

#define KeyboardView Nlm_KeyboardView

#define StartPrinting Nlm_StartPrinting
#define EndPrinting Nlm_EndPrinting
#define StartPage Nlm_StartPage
#define EndPage Nlm_EndPage
#define PrintingRect Nlm_PrintingRect
#define ClipPrintingRect Nlm_ClipPrintingRect

#define StartPicture Nlm_StartPicture
#define EndPicture Nlm_EndPicture

#define StringToClipboard Nlm_StringToClipboard
#define ClipboardToString Nlm_ClipboardToString
#define ClipboardHasString Nlm_ClipboardHasString

#define GetInputFileName Nlm_GetInputFileName
#define GetOutputFileName Nlm_GetOutputFileName

#define Metronome Nlm_Metronome
#define SetWindowTimer Nlm_SetWindowTimer

#define SetMouseMoveCallback Nlm_SetMouseMoveCallback
#define SetMouseMoveRegion Nlm_SetMouseMoveRegion

#define ProcessEvents Nlm_ProcessEvents
#define QuitProgram Nlm_QuitProgram
#define QuittingProgram Nlm_QuittingProgram
#define RegisterDropProc Nlm_RegisterDropProc
#define RegisterServiceProc Nlm_RegisterServiceProc
#define RegisterResultProc Nlm_RegisterResultProc

#define SetAction Nlm_SetAction

#define Advance Nlm_Advance
#define Break Nlm_Break
#define SetNextPosition Nlm_SetNextPosition
#define GetNextPosition Nlm_GetNextPosition

#define CountItems Nlm_CountItems

#define Enabled Nlm_Enabled
#define Visible Nlm_Visible
#define ObjectRect Nlm_ObjectRect
#define InvalObject Nlm_InvalObject

#define Show Nlm_Show
#define Hide Nlm_Hide
#define Enable Nlm_Enable
#define Disable Nlm_Disable
#define Select Nlm_Select

#define SetTitle Nlm_SetTitle
#define GetTitle Nlm_GetTitle
#define SetValue Nlm_SetValue
#define GetValue Nlm_GetValue
#define SetStatus Nlm_SetStatus
#define GetStatus Nlm_GetStatus
#define SetOffset Nlm_SetOffset
#define GetOffset Nlm_GetOffset
#define SetPosition Nlm_SetPosition
#define GetPosition Nlm_GetPosition
#define SetRange Nlm_SetRange
#define AdjustPrnt Nlm_AdjustPrnt

#define SetItemTitle Nlm_SetItemTitle
#define GetItemTitle Nlm_GetItemTitle
#define SetItemStatus Nlm_SetItemStatus
#define GetItemStatus Nlm_GetItemStatus

#define GetNextItem Nlm_GetNextItem

#define Reset Nlm_Reset

#define Remove Nlm_Remove

#define Parent Nlm_Parent

#define RealizeWindow Nlm_RealizeWindow
#define IconifyWindow Nlm_IconifyWindow
#define IconicWindow  Nlm_IconicWindow
#define WhichWindow Nlm_WhichWindow
#define InWindow Nlm_InWindow
#define FrontWindow Nlm_FrontWindow
#define InFront Nlm_InFront
#define UseWindow Nlm_UseWindow
#define CurrentWindow Nlm_CurrentWindow
#define UsingWindow Nlm_UsingWindow
#define ActiveWindow Nlm_ActiveWindow
#define EraseWindow Nlm_EraseWindow
#define ParentWindow Nlm_ParentWindow
#define SavePort Nlm_SavePort
#define RestorePort Nlm_RestorePort
#define Update Nlm_Update
#define EventAvail Nlm_EventAvail
#define FlushEvents Nlm_FlushEvents
#define ProcessAnEvent Nlm_ProcessAnEvent
#define ProcessEventOrIdle Nlm_ProcessEventOrIdle
#define ProcessExternalEvent Nlm_ProcessExternalEvent
#define AllParentsEnabled Nlm_AllParentsEnabled
#define AllParentsVisible Nlm_AllParentsVisible
#define UnloadSegment Nlm_UnloadSegment

#define SetObjectExtra Nlm_SetObjectExtra
#define GetObjectExtra Nlm_GetObjectExtra
#define ProcessUpdatesFirst Nlm_ProcessUpdatesFirst
#define PopupParentWindow Nlm_PopupParentWindow

#define HANDLE Nlm_HANDLE
#define AlignObjects Nlm_AlignObjects

#define CurrentVisibleText Nlm_CurrentVisibleText
#define StdCutTextProc Nlm_StdCutTextProc
#define StdCopyTextProc Nlm_StdCopyTextProc
#define StdPasteTextProc Nlm_StdPasteTextProc
#define StdDeleteTextProc Nlm_StdDeleteTextProc

#define ReadText Nlm_ReadText
#define WriteText Nlm_WriteText
#define WriteLog Nlm_WriteLog

#define ReadChar Nlm_ReadChar
#define ReadString Nlm_ReadString
#define ReadField Nlm_ReadField
#define ReadLine Nlm_ReadLine
#define ReadCard Nlm_ReadCard
#define ReadInt Nlm_ReadInt
#define ReadLong Nlm_ReadLong
#define ReadReal Nlm_ReadReal
#define ReadDouble Nlm_ReadDouble
#define WriteChar Nlm_WriteChar
#define WriteLn Nlm_WriteLn
#define WriteString Nlm_WriteString
#define WriteCard Nlm_WriteCard
#define WriteInt Nlm_WriteInt
#define WriteLong Nlm_WriteLong
#define WriteReal Nlm_WriteReal
#define WriteDouble Nlm_WriteDouble

#define StrngPrintable Nlm_StrngPrintable
#define StrngLen Nlm_StrngLen
#define StrngCat Nlm_StrngCat
#define StrngCpy Nlm_StrngCpy
#define StrngPos Nlm_StrngPos
#define StrngSeg Nlm_StrngSeg
#define StrngRep Nlm_StrngRep
#define StrngEql Nlm_StrngEql
#define StrngCmp Nlm_StrngCmp
#define SymblCmp Nlm_SymblCmp

#define StrToCard Nlm_StrToCard
#define StrToInt Nlm_StrToInt
#define StrToLong Nlm_StrToLong
#define StrToPtr Nlm_StrToPtr
#define StrToReal Nlm_StrToReal
#define StrToDouble Nlm_StrToDouble
#define CardToStr Nlm_CardToStr
#define IntToStr Nlm_IntToStr
#define LongToStr Nlm_LongToStr
#define PtrToStr Nlm_PtrToStr
#define RealToStr Nlm_RealToStr
#define DoubleToStr Nlm_DoubleToStr

#define SetString Nlm_SetString
#define GetString Nlm_GetString

#define ArrowCursor Nlm_ArrowCursor
#define CrossCursor Nlm_CrossCursor
#define IBeamCursor Nlm_IBeamCursor
#define PlusCursor Nlm_PlusCursor
#define WatchCursor Nlm_WatchCursor

#define Version Nlm_Version
#define MousePosition Nlm_MousePosition
#define MouseButton Nlm_MouseButton
#define ComputerTime Nlm_ComputerTime

#define ChooseColorDialog Nlm_ChooseColorDialog

#define PoinT Nlm_PoinT
#define PointPtr Nlm_PointPtr
#define RecT Nlm_RecT
#define RectPtr Nlm_RectPtr
#define RegioN Nlm_RegioN
#define FonT Nlm_FonT

#define updateRgn Nlm_updateRgn
#define updateRect Nlm_updateRect

#define systemFont Nlm_systemFont
#define programFont Nlm_programFont

#define stdAscent Nlm_stdAscent
#define stdDescent Nlm_stdDescent
#define stdLeading Nlm_stdLeading
#define stdFontHeight Nlm_stdFontHeight
#define stdLineHeight Nlm_stdLineHeight
#define stdCharWidth Nlm_stdCharWidth

#ifndef WIN_X
#define SetPort Nlm_SetPort
#endif

#define SetUpDrawingTools Nlm_SetUpDrawingTools
#define CleanUpDrawingTools Nlm_CleanUpDrawingTools

#define ResetDrawingTools Nlm_ResetDrawingTools

#define CopyMode Nlm_CopyMode
#define MergeMode Nlm_MergeMode
#define InvertMode Nlm_InvertMode
#define EraseMode Nlm_EraseMode

#define Black Nlm_Black
#define Red Nlm_Red
#define Green Nlm_Green
#define Blue Nlm_Blue
#define Cyan Nlm_Cyan
#define Magenta Nlm_Magenta
#define Yellow Nlm_Yellow
#define White Nlm_White
#define Gray Nlm_Gray
#define LtGray Nlm_LtGray
#define DkGray Nlm_DkGray
#define SelectColor Nlm_SelectColor
#define GetColorRGB Nlm_GetColorRGB
#define GetColor Nlm_GetColor
#define SetColor Nlm_SetColor
#define InvertColors Nlm_InvertColors
#define DecodeColor Nlm_DecodeColor

#define Solid Nlm_Solid
#define Dark Nlm_Dark
#define Medium Nlm_Medium
#define Light Nlm_Light
#define Empty Nlm_Empty
#define SetPenPattern Nlm_SetPenPattern
#define Dotted Nlm_Dotted
#define Dashed Nlm_Dashed
#define WidePen Nlm_WidePen
#define SetPenDash Nlm_SetPenDash

#define GetFont Nlm_GetFont
#define ParseFont Nlm_ParseFont
#define SelectFont Nlm_SelectFont
#define AssignPrinterFont Nlm_AssignPrinterFont

#define FontSpec Nlm_FontSpec
#define FontSpecPtr Nlm_FontSpecPtr

#define CreateFont Nlm_CreateFont
#define GetResidentFont Nlm_GetResidentFont
#define CopyFont Nlm_CopyFont
#define DeleteFont Nlm_DeleteFont
#define FindNextResidentFont Nlm_FindNextResidentFont
#define GetFontSpec Nlm_GetFontSpec
#define EqualFontSpec Nlm_EqualFontSpec

#define ChooseFont Nlm_ChooseFont
#define StrToFontSpec Nlm_StrToFontSpec
#define FontSpecToStr Nlm_FontSpecToStr

#define Helvetica Nlm_Helvetica
#define Times Nlm_Times
#define Courier Nlm_Courier
#define Symbol Nlm_Symbol

#define CharWidth Nlm_CharWidth
#define StringWidth Nlm_StringWidth
#define TextWidth Nlm_TextWidth
#define Ascent Nlm_Ascent
#define Descent Nlm_Descent
#define Leading Nlm_Leading
#define FontHeight Nlm_FontHeight
#define LineHeight Nlm_LineHeight
#define MaxCharWidth Nlm_MaxCharWidth
#define FitStringWidth Nlm_FitStringWidth

#define SetPen Nlm_SetPen
#define GetPen Nlm_GetPen

#define PaintChar Nlm_PaintChar
#define PaintString Nlm_PaintString
#define PaintStringEx Nlm_PaintStringEx
#define PaintText Nlm_PaintText
#define DrawString Nlm_DrawString
#define DrawText Nlm_DrawText

#define MoveTo Nlm_MoveTo
#define LineTo Nlm_LineTo
#define DrawLine Nlm_DrawLine

#define LoadPt Nlm_LoadPt
#define AddPt Nlm_AddPt
#define SubPt Nlm_SubPt
#define EqualPt Nlm_EqualPt
#define PtInRect Nlm_PtInRect
#define PtInRgn Nlm_PtInRgn

#define LoadRect Nlm_LoadRect
#define UpsetRect Nlm_UpsetRect
#define OffsetRect Nlm_OffsetRect
#define InsetRect Nlm_InsetRect
#define SectRect Nlm_SectRect
#define UnionRect Nlm_UnionRect
#define EqualRect Nlm_EqualRect
#define EmptyRect Nlm_EmptyRect
#define RectInRect Nlm_RectInRect
#define RectInRgn Nlm_RectInRgn

#define EraseRect Nlm_EraseRect
#define FrameRect Nlm_FrameRect
#define PaintRect Nlm_PaintRect
#define InvertRect Nlm_InvertRect
#define ScrollRect Nlm_ScrollRect

#define EraseOval Nlm_EraseOval
#define FrameOval Nlm_FrameOval
#define PaintOval Nlm_PaintOval
#define InvertOval Nlm_InvertOval

#define EraseRoundRect Nlm_EraseRoundRect
#define FrameRoundRect Nlm_FrameRoundRect
#define PaintRoundRect Nlm_PaintRoundRect
#define InvertRoundRect Nlm_InvertRoundRect

#define EraseArc Nlm_EraseArc
#define FrameArc Nlm_FrameArc
#define PaintArc Nlm_PaintArc
#define InvertArc Nlm_InvertArc

#define EraseQuadrant Nlm_EraseQuadrant
#define FrameQuadrant Nlm_FrameQuadrant
#define PaintQuadrant Nlm_PaintQuadrant
#define InvertQuadrant Nlm_InvertQuadrant

#define ErasePoly Nlm_ErasePoly
#define FramePoly Nlm_FramePoly
#define PaintPoly Nlm_PaintPoly
#define InvertPoly Nlm_InvertPoly

#define CreateRgn Nlm_CreateRgn
#define DestroyRgn Nlm_DestroyRgn
#define ClearRgn Nlm_ClearRgn
#define LoadRectRgn Nlm_LoadRectRgn
#define SectRgn Nlm_SectRgn
#define UnionRgn Nlm_UnionRgn
#define DiffRgn Nlm_DiffRgn
#define XorRgn Nlm_XorRgn
#define EqualRgn Nlm_EqualRgn
#define EmptyRgn Nlm_EmptyRgn

#define EraseRgn Nlm_EraseRgn
#define FrameRgn Nlm_FrameRgn
#define PaintRgn Nlm_PaintRgn
#define InvertRgn Nlm_InvertRgn

#define ClipRect Nlm_ClipRect
#define ClipRgn Nlm_ClipRgn
#define ResetClip Nlm_ResetClip

#define ValidRect Nlm_ValidRect
#define InvalRect Nlm_InvalRect
#define ValidRgn Nlm_ValidRgn
#define InvalRgn Nlm_InvalRgn
#define CopyBits Nlm_CopyBits
#define CopyPixmap Nlm_CopyPixmap
#define RGBColoR Nlm_RGBColoR
#define RGBColoRPtr Nlm_RGBColoRPtr

#ifdef __cplusplus
}
#endif

#endif


