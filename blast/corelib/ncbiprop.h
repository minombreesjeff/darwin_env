/*   ncbiprop.h
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
* File Name:  ncbiprop.h
*
* Author:  Schuler
*
* Version Creation Date:   06-04-93
*
* $Revision: 6.1 $
*
* File Description:
*   	Application Property functions.
*
* Modifications:
* --------------------------------------------------------------------------
* Date     Name        Description of modification
* -------  ----------  -----------------------------------------------------
* 06-14-94 Schuler     Added GetProgramName and SetProgramName macros
*
* $Log: ncbiprop.h,v $
* Revision 6.1  2001/04/12 22:42:42  vakatov
* Define #GetProgramName and #SetProgramName only if they have not
* been defined yet
*
* Revision 6.0  1997/08/25 18:16:59  madden
* Revision changed to 6.0
*
* Revision 5.2  1996/12/03 21:48:33  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
 * Revision 5.1  1996/07/16  19:58:29  vakatov
 * Added the Nlm_GetScratchBuffer() function prototype
 *
 * Revision 5.0  1996/05/28  13:18:57  ostell
 * Set to revision 5.0
 *
 * Revision 4.0  1995/07/26  13:46:50  ostell
 * force revision to 4.0
 *
 * Revision 2.3  1995/05/15  18:45:58  ostell
 * added Log line
 *
*
* ==========================================================================
*/
#ifndef _NCBIPROP_
#define _NCBIPROP_

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef int (LIBCALLBACK *Nlm_AppPropEnumProc) PROTO((const char *key, void *data));

NLM_EXTERN void  LIBCALL Nlm_InitAppContext PROTO((void));
NLM_EXTERN char* LIBCALL Nlm_GetScratchBuffer PROTO((size_t size));
NLM_EXTERN void  LIBCALL Nlm_ReleaseAppContext PROTO((void));
NLM_EXTERN void* LIBCALL Nlm_SetAppProperty PROTO((const char *key, void *data));
NLM_EXTERN void* LIBCALL Nlm_GetAppProperty PROTO((const char *key));
NLM_EXTERN void* LIBCALL Nlm_RemoveAppProperty PROTO((const char *key));
NLM_EXTERN int   LIBCALL Nlm_EnumAppProperties PROTO((Nlm_AppPropEnumProc));
NLM_EXTERN long  LIBCALL Nlm_GetAppProcessID PROTO((void));

#define AppPropEnumProc Nlm_AppPropEnumProc
#define GetScratchBuffer Nlm_GetScratchBuffer
#define InitAppContext Nlm_InitAppContext
#define ReleaseAppContext Nlm_ReleaseAppContext
#define SetAppProperty Nlm_SetAppProperty
#define GetAppProperty Nlm_GetAppProperty
#define RemoveAppProperty Nlm_RemoveAppProperty
#define EnumAppProperties Nlm_EnumAppProperties
#define GetAppProcessID Nlm_GetAppProcessID

#if !defined(GetProgramName)
#  define GetProgramName()   (const char*)GetAppProperty("ProgramName")
#endif

#if !defined(SetProgramName)
#  define SetProgramName(x)  MemFree(SetAppProperty("ProgramName",(void*)(x)))
#endif

#ifdef __cplusplus
}
#endif


#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif

