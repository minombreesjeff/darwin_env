/*   fstylep.h
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
* File Name:  fstylep.h
*
* Author:  Alex Smirnov
*
* Version Creation Date:   9/23/94
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
* ==========================================================================
*/

#ifndef _FSTYLEP_
#define _FSTYLEP_

/**************************************************************************/
/* INCLUDE */
/**************************************************************************/

/**************************************************************************/
/* DEFINES */
/**************************************************************************/
#define MSM_TOTAL_POSITIVE   89
#define MSM_TOTAL_NOTPOS     8
#define MSM_TOTAL_CLASSES    (MSM_TOTAL_POSITIVE+MSM_TOTAL_NOTPOS)
#define MSM_TOTAL_POSITIVE_R (MSM_TOTAL_POSITIVE+1)
#define MSM_TOTAL_CLASSES_R  (MSM_TOTAL_POSITIVE_R+MSM_TOTAL_NOTPOS)
#define MSM_SUBCLASSES_NUM   7
#define SM_TOOLS_TOTAL       4
#define SM_TOOLS_COLOR       0
#define SM_TOOLS_SHADDING    1
#define SM_TOOLS_LTYPE       2
#define SM_TOOLS_FONT        3

#define SM_SHTOOLS_COLOR     0x1
#define SM_SHTOOLS_SHADDING  0x2
#define SM_SHTOOLS_LTYPE     0x4
#define SM_SHTOOLS_FONT      0x8

/**************************************************************************/
/* TYPEDEFS */
/**************************************************************************/
typedef void (*Nlm_SmToolsProc) (Int2 tool);

/**************************************************************************/
/* FUNCTION PROTOTYPES */
/**************************************************************************/
extern Boolean   Nlm_MuskStyleDialog     (CharPtr styleName);
extern void      Nlm_SetMuskManagetTitle ( Boolean resetTitle );

extern void      Nlm_EnableSM         ( void );
extern void      Nlm_DisableSM        ( void );

extern GrouP     Nlm_CtreateToolsSM     ( WindoW w );
extern void      Nlm_SetToolsCallbackSM ( Nlm_SmToolsProc proc );
extern void      Nlm_SetToolDefaultSM   ( Int2 tool, BigScalar val );
extern BigScalar Nlm_GetToolValueSM     ( Int2 tool );
extern void      Nlm_ShowToolsSM        ( Int2 tools );

extern GrouP   Nlm_CreateDlgCommon    ( GrouP g );
extern void    Nlm_ToolDlgCommon      ( Int2 tool );
extern void    Nlm_SetTDefCommon      ( void );
extern GrouP   Nlm_CreateDlgGroup     ( GrouP g );
extern void    Nlm_ToolDlgGroup       ( Int2 tool );
extern void    Nlm_SetTDefGroup       ( void );
extern GrouP   Nlm_CreateDlgFeatures  ( GrouP g );
extern void    Nlm_ToolDlgFeatures    ( Int2 tool );
extern void    Nlm_SetTDefFeatures    ( void );
extern GrouP   Nlm_CreateDlgExtra     ( GrouP g );
extern void    Nlm_ToolDlgExtra       ( Int2 tool );
extern void    Nlm_SetTDefExtra       ( void );
extern GrouP   Nlm_CreateDlgSeq       ( GrouP g );
extern void    Nlm_ToolDlgSeq         ( Int2 tool );
extern void    Nlm_SetTDefSeq         ( void );
extern GrouP   Nlm_CreateDlgCColor    ( GrouP g );
extern void    Nlm_ToolDlgCColor      ( Int2 tool );
extern void    Nlm_SetTDefCColor      ( void );
extern Boolean Msk_OrderWasModified ( void );

#endif /* _FSTYLEP_ */

/*END*/
