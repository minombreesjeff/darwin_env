/*************************************************************************
*   utilpars.h:
*
* $Log: utilpars.h,v $
* Revision 6.1  2001/12/06 17:00:41  kans
* TextSave takes size_t, not Int2, otherwise titin protein tries to allocate negative number
*
* Revision 6.0  1997/08/25 18:08:18  madden
* Revision changed to 6.0
*
* Revision 5.1  1997/06/19 18:39:46  vakatov
* [WIN32,MSVC++]  Adopted for the "NCBIOBJ.LIB" DLL'ization
*
* Revision 5.0  1996/05/28 13:23:23  ostell
* Set to revision 5.0
*
 * Revision 4.1  1995/11/17  21:28:35  kans
 * asn2ff now uses gather (Tatiana)
 *
 * Revision 1.2  1995/05/15  21:46:05  ostell
 * added Log line
 *
*
*                                                              10-12-93
*************************************************************************/
#ifndef _UTILITY_
#define _UTILITY_

#include <stdio.h>
#include <ncbi.h>

#undef NLM_EXTERN
#ifdef NLM_IMPORT
#define NLM_EXTERN NLM_IMPORT
#else
#define NLM_EXTERN extern
#endif


#define  ParFlat_TOTAL_AA            26

/*************************************************************************/

NLM_EXTERN CharPtr TextSave PROTO((CharPtr text, size_t len));
/*  -- allocates a null terminated string and copies len characters into
       it starting at text.
    -- returns a pointer to the allocated string. If len is 0 returns NULL 
                                                                    3-26-93 */

NLM_EXTERN void DelTailBlank PROTO((CharPtr str));
/* -- return a string without tail blanks                          10-12-93 */

NLM_EXTERN Int2 MatchArrayStringIcase PROTO((CharPtr array_string[], Int2 totalstr,
                                                            CharPtr str));
/*  -- return array position of the "str" (ignored case) in the array_string,
      otherwise, return (-1) if no match                           10-12-93 */


NLM_EXTERN Uint1 ValidAminoAcid PROTO((CharPtr aa));
/*  -- return a sequential code for the amino acid if "aa" is a valid
       amino acid; otherwise return 255, unknown
    -- using NCBIstdaa, IUPAC3aa
                                                                  7-8-93  */

#undef NLM_EXTERN
#ifdef NLM_EXPORT
#define NLM_EXTERN NLM_EXPORT
#else
#define NLM_EXTERN
#endif

#endif
