/*   netentcf.c
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
 * RCS $Id: netentcf.c,v 6.1 1998/12/08 16:52:42 kans Exp $
 *
 * Author:  Kans
 *
 * Version Creation Date:   9/10/96
 *
 * File Description:
 *       Network Entrez configuration
 *
 * Modifications:
 * --------------------------------------------------------------------------
 * Date     Name        Description of modification
 * -------  ----------  -----------------------------------------------------
 */

#include <vibrant.h>
#include <netcnfg.h>

static void     StandaloneFormMessage (ForM f, Int2 mssg)
{
  BaseFormPtr     bfp;

  bfp = (BaseFormPtr) GetObjectExtra (f);
  if (bfp != NULL) {
    switch (mssg) {
      case VIB_MSG_QUIT:
        QuitProgram ();
        break;
      default:
        break;
    }
  }
}

#ifdef WIN_MAC
static IteM     cutItem = NULL;
static IteM     copyItem = NULL;
static IteM     pasteItem = NULL;
static IteM     deleteItem = NULL;

static void     SetupMacMenus (void)
{
  MenU            m;

  m = AppleMenu (NULL);
  DeskAccGroup (m);

  m = PulldownMenu (NULL, "File");
  FormCommandItem (m, "Quit/Q", NULL, VIB_MSG_QUIT);

  m = PulldownMenu (NULL, "Edit");
  cutItem = FormCommandItem (m, CUT_MENU_ITEM, NULL, VIB_MSG_CUT);
  copyItem = FormCommandItem (m, COPY_MENU_ITEM, NULL, VIB_MSG_COPY);
  pasteItem = FormCommandItem (m, PASTE_MENU_ITEM, NULL, VIB_MSG_PASTE);
  deleteItem = FormCommandItem (m, CLEAR_MENU_ITEM, NULL, VIB_MSG_DELETE);
}

static void     StandaloneFormActivated (WindoW w)
{
  currentFormDataPtr = (VoidPtr) GetObjectExtra (w);
  RepeatProcOnHandles (Enable,
                   (HANDLE) cutItem, (HANDLE) copyItem,
                   (HANDLE) pasteItem, (HANDLE) deleteItem, NULL);
}

static void     MacDeactProc (WindoW w)
{
  currentFormDataPtr = NULL;
  RepeatProcOnHandles (Disable,
                   (HANDLE) cutItem, (HANDLE) copyItem,
                   (HANDLE) pasteItem, (HANDLE) deleteItem, NULL);
}

#else
#define StandaloneFormActivated NULL
#endif

static void     ConfigAccepted (void)
{
  QuitProgram ();
}

static void     ConfigCancelled (void)
{
  QuitProgram ();
}

Int2 Main (void)
{
  ProcessUpdatesFirst (FALSE);

#ifdef WIN_MAC
  SetDeactivate (NULL, MacDeactProc);
  SetupMacMenus ();
#endif

  ShowNetConfigForm (StandaloneFormActivated, StandaloneFormMessage,
                     ConfigAccepted, ConfigCancelled, NULL, FALSE);
  ProcessEvents ();
  return 0;
}
