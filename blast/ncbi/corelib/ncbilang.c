/*  ncbilang.c
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
* File Name:  ncbilang.c
*
* Author:  T. Koike, DDBJ
*   
* Version Creation Date: 5/27/97
*
* $Revision: 6.3 $
*
* File Description:
*
* Modifications:  
* --------------------------------------------------------------------------
* $Log: ncbilang.c,v $
* Revision 6.3  2002/06/13 16:14:07  kans
* fix includes for OS_UNIX_DARWIN with WIN_MAC (EN)
*
* Revision 6.2  1999/12/21 17:52:40  kans
* removed MPW/THINKC conditional code, starting upgrade to Carbon compatibility - Churchill
*
* Revision 6.1  1997/11/26 21:26:16  vakatov
* Fixed errors and warnings issued by C and C++ (GNU and Sun) compilers
*
* Revision 6.0  1997/08/25 18:15:35  madden
* Revision changed to 6.0
*
* Revision 5.3  1997/07/16 21:59:00  vakatov
* Adopted for 32-bit MS-Win DLLs
*
* ==========================================================================
*/

#include <ncbi.h>	/* includes ncbilcl.h for OS macros. */
#include <ncbilang.h>
#ifdef OS_MAC
#	include <Script.h>
#endif
#ifdef OS_UNIX
#	include <locale.h>
#endif
#ifdef OS_VMS
#endif
#ifdef OS_WINNT
#	include <locale.h>
#endif

static Nlm_LangEnum CheckLanguage(void)
{
	Nlm_LangEnum ret = LangUninit;

#if defined(OS_MAC) && !defined(OS_UNIX_DARWIN)
	long script = GetScriptManagerVariable(smSysScript);
	long lang = GetScriptVariable((short)script, smScriptLang);
	switch (lang) {
		case langEnglish :		ret = LangEnglish;	break;
		case langJapanese :		ret = LangJapanese;	break;
/*		case langFrench :		ret = LangFrench;	break;	*/
/*		case langGerman :		ret = LangGerman;	break;	*/
/*		case langItalian :		ret = LangItalian;	break;	*/
/*		default:				ret = LangUnknown;	break;	*/
		default:				ret = LangEnglish;	break;
	}
#endif /*OS_MAC && !OS_UNIX_DARWIN*/

#ifdef WIN16
	Nlm_Char lang [64];
	ret = LangEnglish;
	if (Nlm_GetAppParam ("VIBRANT", "GENERAL", "LANGUAGE", "", lang, sizeof (lang))) {
		if (Nlm_StringICmp (lang, "JAPANESE") == 0) {
			ret = LangJapanese;
		}
	}
#endif /*WIN16*/

#ifdef OS_WINNT
	const char* locale = setlocale(LC_ALL, "");
	if		(Nlm_StringICmp(locale, "C") == 0)					ret = LangSystem;
/*	else if (Nlm_StringICmp(locale, "chinese") == 0)			ret = LangChinese;	*/
/*	else if (Nlm_StringICmp(locale, "chinese-simplified") == 0)	ret = LangChinese;	*/
/*	else if (Nlm_StringICmp(locale, "chs") == 0)				ret = LangChinese;	*/
/*	else if (Nlm_StringICmp(locale, "chinese-traditional") == 0)	ret = LangChinese;	*/
/*	else if (Nlm_StringICmp(locale, "cht") == 0)				ret = LangChinese;	*/
/*	else if (Nlm_StringICmp(locale, "australian") == 0)			ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "ena") == 0)				ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "english-aus") == 0)		ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "canadian") == 0)			ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "enc") == 0)				ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "english-can") == 0)		ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "english") == 0)			ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "english-nz") == 0)			ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "enz") == 0)				ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "eng") == 0)				ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "english-uk") == 0)			ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "uk") == 0)					ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "american") == 0)			ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "american english") == 0)	ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "american-english") == 0)	ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "english-american") == 0)	ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "english-us") == 0)			ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "english-usa") == 0)		ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "enu") == 0)				ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "us") == 0)					ret = LangEnglish;	*/
/*	else if (Nlm_StringICmp(locale, "usa") == 0)				ret = LangEnglish;	*/
	else if (Nlm_StringICmp(locale, "japanese") == 0)			ret = LangJapanese;
	else if (Nlm_StringICmp(locale, "jpn") == 0)				ret = LangJapanese;
/*	else if (Nlm_StringICmp(locale, "korean") == 0)				ret = LangKorean;	*/
/*	else if (Nlm_StringICmp(locale, "kor") == 0)				ret = LangKorean;	*/
/*	else														ret = LangUnknown;	*/
	else														ret = LangEnglish;
#endif /*OS_WINNT*/

#ifdef OS_UNIX
	const char* locale = setlocale(LC_ALL, "");
	if		(Nlm_StringICmp(locale, "C") == 0)			ret = LangSystem;
	else if	(Nlm_StringICmp(locale, "iso_8859_1") == 0)	ret = LangEnglish;	/* ISO Latin-1 */
	else if	(Nlm_StringICmp(locale, "iso_8859_2") == 0)	ret = LangEnglish;	/* ISO Latin-2 */
	else if	(Nlm_StringICmp(locale, "iso_8859_3") == 0)	ret = LangEnglish;	/* ISO Latin-3 */
	else if	(Nlm_StringICmp(locale, "iso_8859_4") == 0)	ret = LangEnglish;	/* ISO Latin-4 */

#ifdef NOT_IMPLEMENTED_LANG
	else if	(Nlm_StringICmp(locale, "iso_8859_5") == 0)	ret = LangCyrillic;	/* ISO Cyrillic */
	else if	(Nlm_StringICmp(locale, "iso_8859_6") == 0)	ret = LangArabic;	/* ISO Arabic */
	else if	(Nlm_StringICmp(locale, "iso_8859_7") == 0)	ret = LangGreek;	/* ISO Greek */
	else if	(Nlm_StringICmp(locale, "iso_8859_8") == 0)	ret = LangHebrew;	/* ISO Hebrew */
	else if	(Nlm_StringICmp(locale, "iso_8859_9") == 0)	ret = LangEnglish;	/* ISO Latin-5 */
	else if	(Nlm_StringICmp(locale, "en") == 0)			ret = LangEnglish;
	else if	(Nlm_StringICmp(locale, "en_UK") == 0)		ret = LangEnglish;
	else if	(Nlm_StringICmp(locale, "en_US") == 0)		ret = LangEnglish;
	else if	(Nlm_StringICmp(locale, "fr") == 0)			ret = LangFrench;
	else if	(Nlm_StringICmp(locale, "de") == 0)			ret = LangGerman;
	else if	(Nlm_StringICmp(locale, "it") == 0)			ret = LangItalian;
	else if	(Nlm_StringICmp(locale, "es") == 0)			ret = LangSpanish;
#endif /* NOT_IMPLEMENTED_LANG */

	else if	(Nlm_StringICmp(locale, "ja") == 0)			ret = LangJapanese;
	else if	(Nlm_StringICmp(locale, "ja_JP") == 0)		ret = LangJapanese;
	else if	(Nlm_StringICmp(locale, "ja_JP.EUC") == 0)	ret = LangJapanese;
	else if	(Nlm_StringICmp(locale, "ja_JP.SJIS") == 0)	ret = LangJapanese;
	else if	(Nlm_StringICmp(locale, "japanese") == 0)	ret = LangJapanese;

#ifdef NOT_IMPLEMENTED_LANG
	else if	(Nlm_StringICmp(locale, "ko") == 0)			ret = LangKorean;
	else if	(Nlm_StringICmp(locale, "ko_KR") == 0)		ret = LangKorean;
	else if	(Nlm_StringICmp(locale, "ko_KR.EUC") == 0)	ret = LangKorean;
	else if	(Nlm_StringICmp(locale, "korean") == 0)		ret = LangKorean;
	else if	(Nlm_StringICmp(locale, "zh") == 0)			ret = LangChinese;
	else if	(Nlm_StringICmp(locale, "chinese") == 0)	ret = LangChinese;
	else if	(Nlm_StringICmp(locale, "zh_TW") == 0)		ret = LangChinese;	/* Taipei */
	else if	(Nlm_StringICmp(locale, "tchinese") == 0)	ret = LangChinese;	/* Taipei */
	else												ret = LangUnknown;
#endif /* NOT_IMPLEMENTED_LANG */

	else												ret = LangEnglish;
#endif /*OS_UNIX*/

#ifdef OS_VMS
#endif /*OS_VMS*/

	ASSERT(ret != LangUninit);
	return ret;
}

NLM_EXTERN Nlm_LangEnum Nlm_GetLanguage(void)
{
	static Nlm_LangEnum lang = LangUninit;

	if (lang == LangUninit) {
		lang = CheckLanguage();
	}

	return lang;
}

#ifdef WIN16
static int SJIS1stByte(unsigned char ch)
{
  return (((ch >= 0x81) && (ch <= 0x9F)) || ((ch >= 0xE0) && (ch <= 0xFC)));
}

static int SJIS2ndByte(unsigned char ch)
{
  return (((ch >= 0x40) && (ch <= 0x7E)) || ((ch >= 0x80) && (ch <= 0xFC)));
}
#endif

/* Nlm_LetterByte
 *
 *  Pass in a pointer to a string and we return the number of bytes
 *  per character that the underlying OS is using...
 */

NLM_EXTERN int Nlm_LetterByte(const char* str)
{
	int byte = 0x0F0F;	/* magic number to assert. */

#ifdef OS_MAC
    /* This should work regardless of the language/script system in use...
     * if more languages are supported in the future, this function shouldn't need
     * to be changed. pjc 12/99
     */
	if ((str == NULL) || (*str == '\0')) {
		byte = 0;
	}
	else {
    	long script = GetScriptManagerVariable(smSysScript);
    	long lang = GetScriptVariable((short)script, smScriptLang);

   	    byte = (CharacterType((char*)str, 0, lang) & smChar2byte) ? 2:1;
    }
#endif /*OS_MAC*/

#ifdef WIN16
	if ((str == NULL) || (*str == '\0')) {
		byte = 0;
	}
	else {
		byte = ((IsJapanese() && SJIS1stByte(*str) && SJIS2ndByte(*(str + 1))) ? 2 : 1);
	}
#endif /*WIN16*/

#ifdef OS_WINNT
	byte = mblen(str, MB_CUR_MAX);
	byte = (byte < 0 ? 1 : byte);
#endif /*OS_WINNT*/

#ifdef OS_UNIX
	byte = mblen(str, MB_CUR_MAX);
	byte = (byte < 0 ? 1 : byte);
#endif /*OS_UNIX*/

	ASSERT(byte != 0x0F0F);
	return byte;
}



