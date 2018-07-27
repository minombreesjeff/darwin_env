/*   tsprintf.h
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
* File Name:  tsprintf.h
*
* Author:  Denis Vakatov
*
* Version Creation Date:   07/10/96
*
* $Id: tsprintf.h,v 6.0 1997/08/25 18:17:42 madden Exp $
*
* File Description:
*   	Memory- and MT-safe "sprintf()"
*
* ==========================================================================
*/


#ifndef TSPRINTF_H
#define TSPRINTF_H

#include <ncbilcl.h>
#include <ncbistd.h>
#ifdef VAR_ARGS
#include <varargs.h>
#else
#include <stdarg.h>
#endif


#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 *  Return pointer to a temporary (based on the thread's scratch
 *  buffer) memory area containing the resulting formatted output
 *  The resulting string is '\0'-terminated
 *  Return NULL on error
 */
NLM_EXTERN const Nlm_Char PNTR Nlm_TSPrintf      VPROTO (
 (const Nlm_Char PNTR fmt,
  ...)
);
#define TSPrintf Nlm_TSPrintf


NLM_EXTERN const Nlm_Char PNTR Nlm_TSPrintfArgs  VPROTO (
 (const Nlm_Char PNTR fmt,
  va_list args)
);
#define TSPrintfArgs Nlm_TSPrintfArgs


/*
 *  Attention!!!  the resulting pointer points to the scratch buffer area,
 *  so it must be used immediately and should not be explicitly deallocated
 */

#ifdef VAR_ARGS
#define TSPRINTF(const_char_ptr, fmt) \
{{ \
  va_list args; \
  va_start(args); \
  const_char_ptr = TSPrintfArgs(fmt, args); \
  va_end( args ); \
}}
#else
#define TSPRINTF(const_char_ptr, fmt) \
{{ \
  va_list args; \
  va_start(args, fmt); \
  const_char_ptr = TSPrintfArgs(fmt, args); \
  va_end( args ); \
}}
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

#endif  /* TSPRINTF_H */

