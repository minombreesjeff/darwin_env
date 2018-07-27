/*   ncbistd.h
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
* File Name:  ncbistd.h
*
* Author:  Gish, Kans, Ostell, Schuler
*
* Version Creation Date:   1/1/91
*
* $Revision: 6.8 $
*
* File Description:
*  This system-independent header supposedly works "as is"
*  with the system-dependent header files available for these
*  system/compiler combinations:
*
*	SunOS
*	BSD UNIX
*	SGI IRIX
*	IBM AIX
*	Cray UNICOS
*	MS-DOS and Microsoft C compiler
*	Macintosh with Apple MPW C and Symantec THINK C
*
* Modifications:
* --------------------------------------------------------------------------
* $Log: ncbistd.h,v $
* Revision 6.8  2001/09/27 21:38:57  vakatov
* Vice-versa fix to R6.7:  define "NDEBUG" if "_DEBUG" is not defined
*
* Revision 6.7  2001/08/28 21:01:02  vakatov
* Define "_DEBUG" if "NDEBUG" is not defined
*
* Revision 6.6  2000/02/25 16:45:58  vakatov
* Redesigned to really share "ncbi_*.[ch]" etc. between the C and
* the C++ toolkits, and even to use them in a "standalone" fashion
*
* Revision 6.5  1999/10/14 18:44:35  vakatov
* For better consistency, include <ncbilcl.h> to <ncbistd.h>
*
* Revision 6.4  1999/04/14 20:04:47  vakatov
* For better consistency, moved <ncbiopt.h> from <ncbi.h> to <ncbistd.h>
*
* Revision 6.3  1999/03/17 15:37:24  vakatov
* Set the default(<ncbistd.h>) "Nlm_Int4" to "int" rather tnan "long";
* the same for "Nlm_Uint4";  modifyed the concerned "ncbilcl.*" files
* accordingly.
*
* Revision 6.2  1999/03/04 20:49:24  vakatov
* + "Nlm_STimeout" typedef
*
* Revision 6.1  1998/06/11 21:06:28  shavirin
* Fixed old style definition warning.
*
* Revision 4.2  1995/12/18  17:37:27  epstein
* add TIME_MAX
*
* Revision 4.1  1995/09/13  18:31:24  epstein
* Add BigScalar type for compatability with DEC Alpha
*
* 04-12-93 Schuler     Misc. Windows-isms needed for DLL support
* 12-16-93 Schuler     Added EXPORT to LIBCALL in _WINDLL case
* ==========================================================================
*/
#ifndef _NCBISTD_
#define _NCBISTD_

#if !defined(NDEBUG)  &&  !defined(_DEBUG)
#  define NDEBUG
#endif

#include <ncbilcl.h>
#include <ncbiopt.h>
#include <connect/ncbi_core.h>

#ifndef LITTLE_ENDIAN
#define LITTLE_ENDIAN	1234
#define BIG_ENDIAN	4321
#define OTHER_ENDIAN	0
#endif

#ifndef BYTE_ORDER
#ifdef IS_LITTLE_ENDIAN
#define BYTE_ORDER	LITTLE_ENDIAN
#else
#ifdef IS_BIG_ENDIAN
#define BYTE_ORDER	BIG_ENDIAN
#else
#define BYTE_ORDER	OTHER_ENDIAN
#endif
#endif
#endif

#ifndef INLINE
#ifdef __cplusplus
#define INLINE inline
#else
#define INLINE
#endif
#endif

/*----------------------------------------------------------------------*/
/*      Aliased Logicals, Datatypes                                     */
/*----------------------------------------------------------------------*/
#ifndef PNTR
#define PNTR	*
#endif
#ifndef HNDL
#define HNDL	*
#endif

#ifndef FnPtr
typedef int		(*Nlm_FnPtr)(void);
#define FnPtr		Nlm_FnPtr
#endif

#ifndef VoidPtr
typedef void PNTR	Nlm_VoidPtr;
#define VoidPtr		Nlm_VoidPtr
#endif
#ifndef Pointer
#define Pointer		Nlm_VoidPtr
#endif

#ifndef Handle
typedef void HNDL	Nlm_Handle;
#define Handle		Nlm_Handle
#endif

#ifndef Char
typedef char		Nlm_Char, PNTR Nlm_CharPtr;
#define Char		Nlm_Char
#define CharPtr		Nlm_CharPtr
#endif

#ifndef Uchar
typedef unsigned char	Nlm_Uchar, PNTR Nlm_UcharPtr;
#define Uchar		Nlm_Uchar
#define UcharPtr	Nlm_UcharPtr
#endif

#ifndef Boolean
typedef unsigned char	Nlm_Boolean, PNTR Nlm_BoolPtr;
#define Boolean		Nlm_Boolean
#define BoolPtr		Nlm_BoolPtr
#endif

#ifndef Byte
typedef unsigned char	Nlm_Byte, PNTR Nlm_BytePtr;
#define Byte		Nlm_Byte
#define BytePtr		Nlm_BytePtr
#define BYTE_MAX	UCHAR_MAX
#endif

#ifndef Int1
typedef signed char	Nlm_Int1, PNTR Nlm_Int1Ptr;
#define Int1		Nlm_Int1
#define Int1Ptr		Nlm_Int1Ptr
#define INT1_MIN	SCHAR_MIN
#define INT1_MAX	SCHAR_MAX
#endif

#ifndef Uint1
typedef unsigned char	Nlm_Uint1, PNTR Nlm_Uint1Ptr;
#define Uint1		Nlm_Uint1
#define Uint1Ptr	Nlm_Uint1Ptr
#define UINT1_MAX	UCHAR_MAX
#endif

#ifndef Int2
typedef short		Nlm_Int2, PNTR Nlm_Int2Ptr;
#define Int2		Nlm_Int2
#define Int2Ptr		Nlm_Int2Ptr
#define INT2_MIN	SHRT_MIN
#define INT2_MAX	SHRT_MAX
#endif

#ifndef Uint2
typedef unsigned short	Nlm_Uint2, PNTR Nlm_Uint2Ptr;
#define Uint2		Nlm_Uint2
#define Uint2Ptr	Nlm_Uint2Ptr
#define UINT2_MAX	USHRT_MAX
#endif

#ifndef Int4
typedef signed int  Nlm_Int4, PNTR Nlm_Int4Ptr;
#define Int4        Nlm_Int4
#define Int4Ptr     Nlm_Int4Ptr
#define INT4_MIN    (-2147483647-1)
#define INT4_MAX    2147483647
#endif

#ifndef Uint4
typedef unsigned int  Nlm_Uint4, PNTR Nlm_Uint4Ptr;
#define Uint4         Nlm_Uint4
#define Uint4Ptr      Nlm_Uint4Ptr
#define UINT4_MAX     4294967295U
#endif

#ifndef TIME_MAX
#define TIME_MAX	ULONG_MAX
#endif

#ifndef FloatLo
typedef float		Nlm_FloatLo, PNTR Nlm_FloatLoPtr;
#define FloatLo		Nlm_FloatLo
#define FloatLoPtr	Nlm_FloatLoPtr
#endif

#ifndef FloatHi
typedef double		Nlm_FloatHi, PNTR Nlm_FloatHiPtr;
#define FloatHi		Nlm_FloatHi
#define FloatHiPtr	Nlm_FloatHiPtr
#endif

#ifndef BigScalar
#define BigScalar long
#endif


/*----------------------------------------------------------------------*/
/*      Misc Common Macros                                              */
/*----------------------------------------------------------------------*/
#ifndef SIZE_MAX
#define SIZE_MAX	MAXALLOC
#endif

#ifndef PATH_MAX
#define PATH_MAX	FILENAME_MAX
#endif

#ifndef NULL
#define NULL ((void *)0)
#endif

#ifndef NULLB
#define NULLB '\0'
#endif

#ifndef TRUE
#define TRUE ((Nlm_Boolean)1)
#endif

#ifndef FALSE
#define FALSE ((Nlm_Boolean)0)
#endif

#ifndef MIN
#define MIN(a,b)	((a)>(b)?(b):(a))
#endif

#ifndef MAX
#define MAX(a,b)	((a)>=(b)?(a):(b))
#endif

#ifndef ABS
#define ABS(a)	((a)>=0?(a):-(a))
#endif

#ifndef SIGN
#define SIGN(a)	((a)>0?1:((a)<0?-1:0))
#endif

#ifndef ROUNDUP       /* Round A up to the nearest multiple of B */
#define ROUNDUP(A,B)	(((A)%(B)) != 0 ? (A)+(B)-((A)%(B)) : (A))
#endif

#ifndef DIM
#define DIM(A) (sizeof(A)/sizeof((A)[0]))
#endif

#ifndef LN2
#define LN2 (0.693147180559945)
#endif
#ifndef LN10
#define LN10 (2.302585092994046)
#endif

/*----------------------------------------------------------------------*/
/*      Misc. MS-DOS-isms                                               */
/*----------------------------------------------------------------------*/
#ifndef NEAR
#define NEAR
#endif
#ifndef FAR
#define FAR
#endif
#ifndef CDECL
#define CDECL
#endif
#ifndef PASCAL
#define PASCAL
#endif
#ifndef EXPORT
#define EXPORT
#endif

#ifndef LIBCALL
#ifdef _WINDLL
#define LIBCALL		FAR PASCAL EXPORT
#else
#define LIBCALL		FAR PASCAL
#endif
#endif

#ifndef LIBCALLBACK
#define LIBCALLBACK	FAR PASCAL
#endif

#endif
