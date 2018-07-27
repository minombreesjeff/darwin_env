/*   vsm.h
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
* File Name:  vsm.h
*
* Author:  Jim Ostell
*
* Version Creation Date:   11-29-94
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

#ifndef _NCBI_VSM_
#define _NCBI_VSM_

#include <vibrant.h>
#include <vsmfile.h>


#ifdef __cplusplus
extern "C" {
#endif

/*****************************************************************************
*
*   Run VSeqMgr as the main program
*   	Initialize if not done already
*       title is the title of the main desktop window
*       aboutinfo is a string describing the application. Use ~ to separate
*       lines. Do not exceed 60 chars per line
*
*****************************************************************************/
void LIBCALL VSeqMgrRun PROTO((CharPtr title, CharPtr aboutinfo));

/*****************************************************************************
*
*   VSeqMgrInit(show)
*   	if (show) displays the window.
*   	OtherWise, just initializes.
*
*       WARNING: All datatypes and procs should be registered BEFORE calling
*           this function.
*
*****************************************************************************/
Boolean LIBCALL VSeqMgrInit PROTO((Boolean show));

/*****************************************************************************
*
*   VSeqMgrShow()
*   	display the VSeqMgr
*   	initializes if not already done
*
*****************************************************************************/
Boolean LIBCALL VSeqMgrShow PROTO((void));

/*****************************************************************************
*
*   VSeqMgrDelete()
*      removes the VSeqMgr from the screen
*      keeps settings for the main window
*
*****************************************************************************/
Boolean LIBCALL VSeqMgrDelete PROTO((void));

/*****************************************************************************
*
*   VSMAddToMenu(MenU m, Int2 menutype)
*      Adds a VSM style submenu to Menu
*      All included procs must have already been registered with the
*        objmgr.
*      types defined below function prototype
*
*****************************************************************************/
Boolean LIBCALL VSMAddToMenu PROTO((MenU m, Int2 menutype));

#define VSM_DESKTOP     0   /* adds whole NCBI desktop */
#define VSM_OPEN_MENU   1   /* adds cascading Open submenu */
#define VSM_SAVE_MENU   2   /* adds cascading Save As submenu */

/*****************************************************************************
*
*   VSMAddMenu(WindoW w, Int2 menutype)
*      Adds a VSM style menu to Window
*      All included procs must have already been registered with the
*        objmgr.
*      If no procs of this type are registered, does not add to window
*
*****************************************************************************/
Boolean LIBCALL VSMAddMenu PROTO((WindoW w, Int2 menutype));

#define VSM_FILTER_MENU 1   /* adds Filter menu */
#define VSM_ANALYZE_MENU 2   /* adds Analysis menu */
#define VSM_VIEW_MENU 3     /* viewers */
#define VSM_EDIT_MENU 4     /* editors */

#ifdef __cplusplus
}
#endif

#endif
