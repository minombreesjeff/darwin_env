/*  $Id: cn3dentr.c,v 6.11 2000/01/06 00:04:42 lewisg Exp $
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
* Author:  Christopher Hogue, Denis Vakatov
*
* Version Creation Date:   1/31/96
*
* File Description:  Entrez entry point for Cn3D
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: cn3dentr.c,v $
* Revision 6.11  2000/01/06 00:04:42  lewisg
* selection bug fixes, update message outbound, animation APIs moved to vibrant
*
* Revision 6.10  2000/01/04 15:55:50  lewisg
* don't hang on disconnected network and fix memory leak/hang at exit
*
* Revision 6.9  1999/10/29 14:15:27  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.8  1999/10/15 20:56:38  lewisg
* append DDV_ColorGlobal as userdata.  free memory when cn3d terminates.
*
* Revision 6.7  1999/10/05 23:18:19  lewisg
* add ddv and udv to cn3d with memory management
*
* Revision 6.6  1999/04/06 20:05:53  lewisg
* more opengl
*
* Revision 6.5  1999/01/20 18:21:18  ywang
* include salmedia.h due to the move around of MediaInfo from cn3dmsg.h to the new created salmedia.h
*
* Revision 6.4  1999/01/14 19:07:16  kans
* network availability is configurable
*
* Revision 6.3  1998/11/24 17:01:01  kans
* put render/label/view/model controls in separate window
*
* Revision 6.2  1998/05/29 22:12:59  kans
* needed to include cn3dmsg.h
*
* Revision 6.1  1998/05/28 22:05:59  ywang
* maintain highlight upon rendering switch when salsa is on
*
 * Revision 6.0  1997/08/25  18:13:28  madden
 * Revision changed to 6.0
 *
* Revision 1.2  1997/07/29 21:17:07  vakatov
* [WIN32,DLL]  Made Cn3D's stubbed functions be DLL-exportable
*
* Revision 1.1  1997/03/20 16:24:02  vakatov
* Initial revision
*
*
* ==========================================================================
*/

#include <cn3dmain.h>
#include <cn3dentr.h>
#include <cn3dmsg.h>
#include <salmedia.h>
#include <cn3dshim.h>
#include <accentr.h>


/* QUERY data and callbacks
 */

static IteM Cn3D_query;
static BeepHook Cn3D_query_callback;
static WindoW Cn3D_query_window;

static void Cn3D_ItemQueryCB(IteM i)
{
    if (Cn3D_query_callback)
        (*Cn3D_query_callback) ();
}


/* CLOSE callbacks
 */

extern void Cn3D_HideCtrl(WindoW w);
static void Cn3D_MyClose(WindoW www)
{
    if (Cn3D_query_window && !Visible(Cn3D_query_window)) {
        ClearStructures();
        ClearSequences();
        QuitProgram();
    } else {
#ifdef _OPENGL
        if (OGL_IsPlaying(Cn3D_ColorData.OGL_Data))
            OGL_StopPlaying(Cn3D_ColorData.OGL_Data);
#else
        if (IsPlaying3D())
            StopPlaying3D();
#endif
        ClearStructures();
        ClearSequences();
        Hide(www);
        Cn3D_HideCtrl(NULL);
    }
}

static void Cn3D_OnCloseCB(WindoW w)
{
    Cn3D_MyClose(w);
}

static void Cn3D_ItemCloseCB(IteM i)
{
    WindoW w = GetObjectExtra(i);
    Cn3D_MyClose(w);
}


/* QUIT callback
 */

static void Cn3D_ItemQuitCB(IteM i)
{
    ClearStructures();
    ClearSequences();
    QuitProgram();
}



/* Set Query callback and data
 */

NLM_EXTERN void LIBCALL Cn3D_SetQueryCallback(BeepHook queryFunc,
                                              VoidPtr queryWindow)
{
    Enable(Cn3D_query);
    Cn3D_query_callback = queryFunc;
    Cn3D_query_window = (WindoW) queryWindow;
}


/* Attach CN3D to Entrez
 */

NLM_EXTERN Handle LIBCALL Cn3DWin_Entrez(ItmActnProc netconfig,
                                         Boolean usingEntrez)
{
    MenU menu = NULL;
    WindoW www;

    if(!EntrezIsInited()) return NULL;
    
    Cn3D_ColorData.UseEntrez = usingEntrez;
    Cn3D_ColorData.EntrezOn = TRUE;  /* assume entrez is inited */
    www =
        Cn3DWin(Cn3D_OnCloseCB, &menu, netconfig, FALSE);

    SetObjectExtra(CommandItem(menu, "Close/C", Cn3D_ItemCloseCB), www,
                   NULL);

    SeparatorItem(menu);

    Cn3D_query =
        CommandItem(menu, "Entrez Query Window...", Cn3D_ItemQueryCB);
    Disable(Cn3D_query);

    CommandItem(menu, "Quit Entrez/Q", Cn3D_ItemQuitCB);

    return (Handle) www;
}
