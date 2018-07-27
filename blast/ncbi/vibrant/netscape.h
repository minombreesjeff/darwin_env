/* $Id: netscape.h,v 6.1 1998/01/21 21:22:30 vakatov Exp $
 * Copyright (c) 1996 Netscape Communications Corporation, all rights reserved.
* ===========================================================================
*
*                            PUBLIC DOMAIN NOTICE
*               National Center for Biotechnology Information
*
*  This software/database is a "United States Government Work" under the
*  terms of the United States Copyright Act.  It was written as part of
*  the author's official duties as a United States Government employee and
*  thus cannot be copyrighted.  This software/database is freely available
*  to the public for use. The National Library of Medicine and the U.S.
*  Government have not placed any restriction on its use or reproduction.
*
*  Although all reasonable efforts have been taken to ensure the accuracy
*  and reliability of the software and data, the NLM and the U.S.
*  Government do not and cannot warrant the performance or results that
*  may be obtained by using this software or data. The NLM and the U.S.
*  Government disclaim all warranties, express or implied, including
*  warranties of performance, merchantability or fitness for any particular
*  purpose.
*
*  Please cite the author in any work or product based on this material.
*
* ===========================================================================
*
* File Name:  $RCSfile: netscape.h,v $
*
* Author:  Sergei Shavirin
*
* Initial Version Creation Date: 11/08/1997
*
* $Revision: 6.1 $
*
* File Description:
*        Main definitions file for Vibrant -> Netscape Index Program
*        NOTE: this is actually a UNIX/X11 code only; on other platforms
*              all API functions do nothing and return FALSE
*
*---------------------------------------------------------------------------
* $Log: netscape.h,v $
* Revision 6.1  1998/01/21 21:22:30  vakatov
* Initial revision
*
*
* ==========================================================================
*/

/* Complete explanation of the interface is given at:
   http://home.netscape.com/newsref/std/x-remote.html */

#ifndef __NETSCAPE_H__
#define __NETSCAPE_H__

#include <ncbistd.h>


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

/* Type definitions */

struct NS_WindowTag;
typedef struct NS_WindowTag PNTR NS_Window;
typedef NS_Window PNTR NS_WindowPtr;

/* External functions */

/* ------------------------  NS_SendCommand  -----------------------
   Purpose:     Send command to the Netscape
   Parameters:  window - handle of Netscape Window. Will be initialized
                         inside function itself
                cmd - command to sent to the Netscape window
                url - if not NULL command will be sent to window
                      identified by this URL
                wintag - _logical_ name of window in Netscape

   Returns:     TRUE on success...
   NOTE:        If Netscape is not loaded - it will be loaded first
                no command will be sent
                See list of commands in the end of this file
  ------------------------------------------------------------------*/
extern Nlm_Boolean NS_SendCommand(NS_WindowPtr window,
                                  const Nlm_Char *cmd, 
                                  const Nlm_Char *url,
                                  const Nlm_Char *wintag,
                                  Nlm_Boolean raise);

/* ------------------------  NS_OpenURL  -----------------------
   Purpose:     Loads a specific URL to the Netscape window
   Parameters:  window - ptr to handle of Netscape Window. Will be initialized
                         inside function itself
                url - URL to load to the Netscape
                wintag - _logical_ name of window in Netscape

   Returns:     TRUE on success...
   NOTE:        If Netscape is not loaded - it will be loaded first
                no command will be sent
                Program in first call will try to find Netscape window
                with specified URL -if nothing present it will peak up
                first available and will use only this window afterwards
  ------------------------------------------------------------------*/
extern Nlm_Boolean NS_OpenURL(NS_WindowPtr window, const Nlm_Char *url, 
                              const Nlm_Char *wintag, Nlm_Boolean raise);


/* ------------------------  NS_ResetWindow  -----------------------
   Purpose:     Force next command to reinitialize Netscape window
   Parameters:  window - ptr to handle of Netscape Window.
   Returns:     None
  ------------------------------------------------------------------*/
extern void NS_ResetWindow(NS_Window window);


/* ------------------------  NS_ResetWindow  -----------------------
   Purpose:     Will free Netscape window handle 
   Parameters:  window - handle of Netscape Window.
   Returns:     None
  ------------------------------------------------------------------*/
extern void NS_WindowFree(NS_Window window);


/*-------------------------------------------------------
     openURL ( )
          Prompts for a URL with a dialog box.
     openURL (URL)
          Opens the specified document without prompting.
     openURL (URL, new-window)
          Create a new window displaying the the specified document.
     openFile ( )
          Prompts for a file with a dialog box.
     openFile (File)
          Opens the specified file without prompting.
     saveAs ( )
          Prompts for a file with a dialog box (like the menu item).
     saveAs (Output-File)
          Writes HTML to the specified file without prompting.
     saveAs (Output-File, Type)
          Writes to the specified file with the type specified - the type
          may be HTML, Text, or PostScript.
     mailto ( )
          pops up the mail dialog with the To: field empty.
     mailto (a, b, c)
          Puts the addresses "a, b, c" in the default To: field.
     addBookmark ( )
          Adds the current document to the bookmark list.
     addBookmark (URL)
          Adds the given document to the bookmark list.
     addBookmark (URL, Title)
          Adds the given document to the bookmark list, with the given
          title.
----------------------------------------------------------*/

#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* __NETSCAPE_H__ */
