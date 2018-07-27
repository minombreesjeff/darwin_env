
/*  $Id: ncbienv.h,v 6.4 2003/06/06 15:01:03 rsmith Exp $
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
* $Revision: 6.4 $
*
* File Description:
*   	protokeys for portable string routines
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ncbienv.h,v $
* Revision 6.4  2003/06/06 15:01:03  rsmith
* fixed ProgramPath for OS Darwin & Mac, Added IsApplicationPackage for Darwin. ProgramPath on Darwin now returns the seen application not the executable buried in the package.
*
* Revision 6.3  2003/01/29 19:37:14  kans
* added GetOpSysString to return allocated string describing operating system running the program
*
* Revision 6.2  1999/11/29 19:58:50  vakatov
* To parse command-line arguments:
* + ECmdLineQuote, Nlm_ParseCmdLineArguments(), Nlm_FreeCmdLineArguments()
*
* Revision 6.1  1999/02/12 16:01:41  vakatov
* Added a draft version of Nlm_GetEnvParamEx()
* Got rid of the old "PROTO" and "LIBCALL" prototype junk, etc.
*
* Revision 5.1  1996/12/03 21:48:33  vakatov
* Adopted for 32-bit MS-Windows DLLs
*
* Revision 4.1  1995/10/06  15:54:00  epstein
* add CacheAppParam() and FlushAppParam()
*
* 06-14-94  Schuler     Created this file.  These definitions previously
*                       resided in ncbimain.h
* 06-14-94  Schuler     Added some new functions:
*                       GetAppParamBoolean() , SetAppParamBoolean(),
*                       GetAppParamInt2(), SetAppParamInt2(),
*                       GetAppParamInt4(), SetAppParamInt4()
* 06-14-94  Schuler     Added LIBCALL to ProgramPath
* ==========================================================================
*/

#ifndef __NCBIenv_h__
#define __NCBIenv_h__

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#ifdef __cplusplus
extern "C" {
#endif

NLM_EXTERN Nlm_Boolean Nlm_FindPath(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, Nlm_Char* buf, Nlm_Int2 buflen);
NLM_EXTERN Nlm_Boolean Nlm_TransientSetAppParam(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* value);
NLM_EXTERN void Nlm_FreeConfigStruct(void);
NLM_EXTERN void Nlm_ProgramPath(Nlm_Char* buf, size_t maxsize);
#ifdef OS_UNIX_DARWIN
NLM_EXTERN Nlm_Boolean Nlm_IsApplicationPackage(char *filePath);
#endif

NLM_EXTERN void Nlm_FlushAppParam(void);
NLM_EXTERN Nlm_Boolean Nlm_CacheAppParam(Nlm_Boolean value);

NLM_EXTERN Nlm_Int2 Nlm_GetAppParam(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* dflt, Nlm_Char* buf, Nlm_Int2 buflen);
NLM_EXTERN Nlm_Boolean Nlm_SetAppParam(const Nlm_Char* file, const Nlm_Char* section, const Nlm_Char* type, const Nlm_Char* value);
NLM_EXTERN Nlm_Boolean GetAppParamBoolean(const Nlm_Char* filebase, const Nlm_Char* sect, const Nlm_Char* key, Nlm_Boolean dflt);
NLM_EXTERN Nlm_Boolean SetAppParamBoolean(const Nlm_Char* filebase, const Nlm_Char* sect, const Nlm_Char* key, Nlm_Boolean value);
NLM_EXTERN Nlm_Boolean SetAppParamLong(const Nlm_Char* filebase, const Nlm_Char* sect, const Nlm_Char* key, long value);
NLM_EXTERN long GetAppParamLong(const Nlm_Char* filebase, const Nlm_Char* sect, const Nlm_Char* key, long dflt);

/* 1) search for the specified key "conf_name" in the transient parameter list
 *    in pseudo-file "conf_file", pseudo-section "conf_section";
 * 2) if not found then look for the environment variable named "env_name";
 * 3) if not found then read configuration file "conf_file", section
 *    "conf_section" and search "conf_name" entry there.
 * If the parameter is found in (1),(2),(3) then put its value to "buf".
 * If nothing is found and "dflt" is not zero then put "dflt" to "buf".
 * Return the length of "buf" string(can be zero).
 * NOTE:  the resultant "buf" string is guaranteed to be '\0'-terminated
 *        and no bigger than "bufsize-1" long. 
 */
NLM_EXTERN size_t Nlm_GetEnvParamEx
(const Char* conf_file,     /* default:  "ncbi" */
 const Char* conf_section,  /* default:  "NCBI" */
 const Char* env_name,
 const Char* conf_name,
 Char*       buf,
 size_t      bufsize,
 const Char* dflt
 );

/* Call Nlm_GetEnvParamEx() with "env_name" == "conf_name" := "env_conf_name"
 */
NLM_EXTERN size_t Nlm_GetEnvParam
(const Char* conf_file, const Char* conf_section,
 const Char* env_conf_name, Char* buf, size_t bufsize, const Char* dflt);

/* Parse cmd.-line args and create (NULL-terminated) "argv" array of values.
 * Use "prog_name" to compose "argv[0]";  if "prog_name" is NULL then
 * use the value of app.property "ProgramName" instead.
 * The resultant argv must be deallocated by the caller using
 * Nlm_FreeCmdLineArguments().
 * On error, do nothing and return FALSE.
 */
typedef enum { /* specifies whether " and ' have special meaning */
  eProcessQuotes,  /* `aa "bb c"' -->  argv[]:  {`aa', `bb c'} */
  eIgnoreQuotes    /* `aa "bb c"' -->  argv[]:  {`aa', `"bb', `c"'} */
} ECmdLineQuote;

NLM_EXTERN Nlm_Boolean Nlm_ParseCmdLineArguments
(const char* prog_name, const char* cmd_line, int* argc_ptr, char*** argv_ptr,
 ECmdLineQuote quote_handling);

/* Deallocate all dynamic memory occupied by "argv".
 * NOTE:  the "argv" must be created by Nlm_ParseCmdLineArguments()
 */
NLM_EXTERN void Nlm_FreeCmdLineArguments(char** argv);

/* returns allocated string describing operating system running the program */

NLM_EXTERN Nlm_CharPtr Nlm_GetOpSysString (void);

#define GetAppParamInt(a,b,c,d)   (int)GetAppParamLong(a,b,c,(long)(d))
#define SetAppParamInt(a,b,c,d)   SetAppParamLong(a,b,c,(long)(d))
#define GetAppParamShort(a,b,c,d) (short)GetAppParamLong(a,b,c,(long)(d))
#define SetAppParamShort(a,b,c,d) SetAppParamLong(a,b,c,(long)(d))

#define FindPath Nlm_FindPath
#define FlushAppParam Nlm_FlushAppParam
#define CacheAppParam Nlm_CacheAppParam
#define GetAppParam Nlm_GetAppParam
#define SetAppParam Nlm_SetAppParam
#define TransientSetAppParam Nlm_TransientSetAppParam
#define FreeConfigStruct Nlm_FreeConfigStruct
#define ProgramPath Nlm_ProgramPath
#define GetAppParamInt2 GetAppParamShort
#define GetAppParamInt4 GetAppParamLong
#define GetEnvParamEx Nlm_GetEnvParamEx
#define GetEnvParam   Nlm_GetEnvParam
#define ParseCmdLineArguments Nlm_ParseCmdLineArguments
#define FreeCmdLineArguments  Nlm_FreeCmdLineArguments
#define GetOpSysString Nlm_GetOpSysString
#ifdef OS_UNIX_DARWIN
#define IsApplicationPackage Nlm_IsApplicationPackage
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

