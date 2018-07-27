/*  ncbilang.h
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
* File Name:  ncbilang.h
*
* Author:  T. Koike, DDBJ
*   
* Version Creation Date: 5/27/97
*
* $Revision: 6.0 $
*
* File Description:
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: ncbilang.h,v $
* Revision 6.0  1997/08/25 18:15:37  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/07/16 21:59:01  vakatov
* Adopted for 32-bit MS-Win DLLs
*
* ==========================================================================
*/

#ifndef _NCBILANG_H_
#define _NCBILANG_H_

#include <ncbistd.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

enum _Nlm_langEnum {
  LangUninit = -1,
  LangSystem,
  LangEnglish,
  LangJapanese,
  LangFrench,
  LangGerman,
  LangItalian,
  LangUnknown
};
typedef enum _Nlm_langEnum Nlm_LangEnum;

/* what language is used on this system ? */
NLM_EXTERN Nlm_LangEnum Nlm_GetLanguage PROTO((void));

#define IsSystemLang()  (LangSystem   == Nlm_GetLanguage() ? TRUE : FALSE)
#define IsEnglish()     (LangEnglish  == Nlm_GetLanguage() ? TRUE : FALSE)
#define IsJapanese()    (LangJapanese == Nlm_GetLanguage() ? TRUE : FALSE)
#define IsFrench()      (LangFrench   == Nlm_GetLanguage() ? TRUE : FALSE)
#define IsGerman()      (LangGerman   == Nlm_GetLanguage() ? TRUE : FALSE)
#define IsItalian()     (LangItalian  == Nlm_GetLanguage() ? TRUE : FALSE)

/* how much bytes for the next letter ? */
/* NULL or "" -> return 0 */
/* illegal code -> return 1 to avoid infinite loop. */
NLM_EXTERN int Nlm_LetterByte PROTO((const char* str));

/* is the next letter muti-byte one ? */
#define IsMBLetter(str)	(1 < Nlm_LetterByte(str) ? TRUE : FALSE)

#ifdef __cplusplus
}
#endif

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif /*_NCBILANG_H_*/
