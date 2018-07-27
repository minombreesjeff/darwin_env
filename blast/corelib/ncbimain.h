/*   ncbimain.h
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
* File Name:  ncbimain.h
*
* Author:  Gish, Kans, Ostell, Schuler, Vakatov
*
* Version Creation Date:   7/7/91
*
* $Revision: 6.3 $
*
* File Description:
*     NCBI Main() function and cmd.-line argument-handling routines
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ncbimain.h,v $
* Revision 6.3  2001/04/12 22:13:54  vakatov
* #define GetArgs only if it is not defined yet
*
* Revision 6.2  2000/06/15 20:51:41  vakatov
* Use "const" in Args code
*
* Revision 6.1  1997/10/27 21:58:13  vakatov
* Added Nlm_FreeArgs() to reset args earlier set by GetArgs[Silent]()
*
* Revision 5.5  1997/07/22 19:11:31  vakatov
* Separated Main() from GetArg[svc]() functions;  [WIN_MSWIN] converged
* console and GUI libraries; [for WIN32-DLL] encapsulated global variables
*
* Revision 5.4  1996/12/30 15:11:36  vakatov
* [WIN_MOTIF][WIN_MSWIN][WIN_MAC] +GetArgsSilent()
*
* Revision 5.3  1996/12/13  15:04:25  vakatov
* Declared Nlm_GetArgv() and Nlm_GetArgc() be DLL-importable
*
* Revision 5.2  1996/12/12  17:05:12  shavirin
* Added definitions for functions Nlm_GetArgv() and Nlm_GetArgc()
*
* Revision 5.1  1996/12/03  21:48:33  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
* 7/7/91   Kans        Multiple configuration files, get and set functions
* 04-15-93 Schuler     Changed _cdecl to LIBCALL
*
* ==========================================================================
*/

#ifndef _NCBIMAIN_
#define _NCBIMAIN_

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif


typedef struct mainargs {
  const char *prompt;            /* prompt for field */
  const char *defaultvalue;      /* default */
  char *from;              /* range or datalink type */
  char *to;
  Nlm_Boolean	optional;
  Nlm_Char	tag;       /* argument on command line */
  Nlm_Int1	type;      /* type of value */
  Nlm_FloatHi	floatvalue;
  Nlm_Int4	intvalue;
  CharPtr	strvalue;
} Nlm_Arg, * Nlm_ArgPtr;      /* (*) not PNTR for MS Windows */

#define Args   Nlm_Arg
#define ArgPtr Nlm_ArgPtr


#define ARG_INVALID  0
#define ARG_BOOLEAN  1
#define ARG_INT      2
#define ARG_FLOAT    3
#define ARG_STRING   4
#define ARG_FILE_IN  5
#define ARG_FILE_OUT 6
#define ARG_DATA_IN  7
#define ARG_DATA_OUT 8


extern Nlm_Int2 Nlm_Main(void);

NLM_EXTERN Nlm_Boolean Nlm_GetArgs(const char* progname, Nlm_Int2 numargs, Nlm_ArgPtr ap);

/* do not post the arg-query dialog if all non-optional arguments can be
 * resolved by default or using command line(even if command line is empty) */
NLM_EXTERN Nlm_Boolean Nlm_GetArgsSilent(const char* progname, Nlm_Int2 numargs, Nlm_ArgPtr ap);

/* Free all dynamically allocated data and reset all data in "ap" */
NLM_EXTERN Nlm_Boolean Nlm_FreeArgs(Nlm_Int2 numargs, Nlm_ArgPtr ap);


/*****************************************************************************
*
*   Nlm_GetArg[v,c]
*     these functions will return standard argc, argv of command-line
*     program
*
*****************************************************************************/

NLM_EXTERN Nlm_CharPtr PNTR Nlm_GetArgv(void);
NLM_EXTERN Nlm_Int4         Nlm_GetArgc(void);


#define Main           Nlm_Main
#if !defined(GetArgs)
#  define GetArgs      Nlm_GetArgs
#endif
#define FreeArgs       Nlm_FreeArgs
#define GetArgsSilent  Nlm_GetArgsSilent
#define GetArgv        Nlm_GetArgv
#define GetArgc        Nlm_GetArgc

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

