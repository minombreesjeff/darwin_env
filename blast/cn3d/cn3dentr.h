/*  $Id: cn3dentr.h,v 6.2 1999/10/29 14:15:27 thiessen Exp $
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
* Version Creation Date:   3/19/97
*
* File Description:  Entry point from Entrez to Cn3D
*                   
* Modifications:
* --------------------------------------------------------------------------
* $Log: cn3dentr.h,v $
* Revision 6.2  1999/10/29 14:15:27  thiessen
* ran all Cn3D source through GNU Indent to prettify
*
* Revision 6.1  1999/01/14 19:07:16  kans
* network availability is configurable
*
* Revision 6.0  1997/08/25 18:13:29  madden
* Revision changed to 6.0
*
* Revision 1.2  1997/07/29 21:17:08  vakatov
* [WIN32,DLL]  Made Cn3D's stubbed functions be DLL-exportable
*
* Revision 1.1  1997/03/20 16:24:17  vakatov
* Initial revision
*
*
* ==========================================================================
*/

#ifndef _CN3DENTR_
#define _CN3DENTR_

#include <ncbi.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif
/* Set Query callback and data
 */
NLM_EXTERN void LIBCALL Cn3D_SetQueryCallback(BeepHook, VoidPtr);

/* Create CN3D window with some Entrez specifics;  return the created WindoW
 */
NLM_EXTERN Handle LIBCALL Cn3DWin_Entrez(ItmActnProc netconfig,
                                         Boolean usingEntrez);

#ifdef __cplusplus
}
#endif
#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif
#endif                          /* _CN3DENTR_ */
