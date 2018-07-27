#ifndef _NCBIOPT_
#define _NCBIOPT_

/*  $Id: ncbiopt.h,v 6.9 2001/08/09 19:21:29 juran Exp $
* ==========================================================================
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
* ==========================================================================
*
* Authors:  Denis Vakatov, Hugues Sicotte
*
* File Description:
*  Miscellaneous platform-dependent types etc. that are probably not
*  available or not exactly in-sync for different platforms
*
* --------------------------------------------------------------------------
* $Log: ncbiopt.h,v $
* Revision 6.9  2001/08/09 19:21:29  juran
* Wrap the inclusion of <stdint.h> in #ifdef __MWERKS__.
*
* Revision 6.8  2001/08/09 18:55:28  juran
* Include <stdint.h> to avoid macro redefinition errors.
*
* Revision 6.7  2001/03/15 21:23:12  vakatov
* [OS_UNIX_AIX] #define Int8 long long  (was just "long").
* Fix by the Ruth Isserlin & Christopher Hogue, chogue@mdsproteomics.com
*
* Revision 6.6  2001/03/14 21:11:27  vakatov
* Int8 is "long long" for OS_UNIX::PROC_HPPA (M.Dumontier, micheld@mshri.on.ca)
*
* Revision 6.5  2000/12/21 15:52:22  vakatov
* [__GNUC__, OS_LINUX]  Force Int8 be "long long"
*
* Revision 6.4  2000/12/20 19:02:49  vakatov
* [__GNUC__]  Use "long long", do not rely on LONG_BIT (RedHat Linux 7.0)
*
* Revision 6.3  2000/03/20 16:04:38  vakatov
* [__GNUC__]  Fixed for absent #LONG_LONG_***
*
* Revision 6.2  1999/08/25 18:21:36  vakatov
* Int8 for OS_UNIX_AIX
*
* Revision 6.1  1998/08/11 22:09:08  vakatov
* Initial revision
*
* ==========================================================================
*/


/********************************************************
 *
 *  Int8, Uint8, INT8_MIN, INT8_MAX, UINT8_MAX
 *
 *  NOTE:  define it as Int4 for the platforms with no 8-byte integers
 *
 *  TESTED WITH:
 *    SunOS peony 5.6 Generic_105181-04 sun4u sparc SUNW,Ultra-1
 *      cc: WorkShop Compilers 4.2 30 Oct 1996 C 4.2
 *      CC: WorkShop Compilers 4.2 18 Sep 1997 C++ 4.2 patch 104631-04
 *      gcc, g++: gcc version 2.7.2.3
 *
 *    SunOS oldies 4.1.3_U1 22 sun4m
 *      CC: SC3.0.1 07 Jul 1995
 *      gcc, g++: gcc version 2.7.2.3
 *
 *    IRIX64 muncher 6.4 02121744 IP27
 *      cc: ???
 *
 *    IRIX leo 5.3 08031226 IP22 mips
 *      cc: ???
 *
 *    OSF1 annie V4.0 564 alpha
 *      gcc, g++: gcc version 2.8.0
 *      cc: ???
 *
 *    16-bit MS-Windows applications built with:
 *      Visual C++ 1.52 (Int8 is not supported, so gets substituted by Int4)
 *
 *    32-bit MS-Windows applications built with:
 *      Visual  C++ 4.2
 *      Visual  C++ 5.0
 *      Borland C++ 4.5 (Int8 is not supported, so gets substituted by Int4)
 */

/*
// Avoid errors when stdint.h tries to 'redefine' standard macros that we've corrupted,
// by including it first.
*/
#ifdef __MWERKS__
#include <stdint.h>
#endif

#if !defined(Int8) || !defined(Uint8) || !defined(Int8Ptr) || !defined(Uint8Ptr)

#  if defined(Int8) || defined(Uint8) || defined(Int8Ptr) || defined(Uint8Ptr)
     /* aut Caesar aut nihil... */
     ;;;!!!PARTIALLY DEFINED 8-BYTE INTEGER TYPES!!!;;;
#  endif

   /* on 64-bit operating systems, the long data type is already 8 bytes */
#  if LONG_BIT==64  &&  !defined(OS_UNIX_LINUX)
#    define Int8  long
#    define Uint8 unsigned long

   /* GNU compiler defines "long long" data type */
#  elif defined(__GNUC__)
#    define Int8  long long
#    define Uint8 unsigned long long

   /* use the compiler-dependent internal data types */
#  elif defined(OS_NT)  &&  _INTEGRAL_MAX_BITS >= 64
#      define Int8  __int64
#      define Uint8 unsigned __int64

#  elif defined(OS_UNIX)
     /* (signed) */
#    if defined(OS_UNIX_LINUX) || defined(OS_UNIX_SOL) || defined(OS_UNIX_IRIX) || defined(PROC_HPPA) || defined(OS_UNIX_AIX)
#      define Int8 long long
#    elif defined(PROC_ALPHA)
#      define Int8 long
#    endif

     /* (unsigned) */
#    if defined(Int8) 
#      define Uint8 unsigned Int8
#    endif

#  elif defined(OS_MAC) 
#    if defined(PROC_PPC)
#      define Int8  long long
#      define Uint8 unsigned long long
#    elif defined(PROC_MC680X0)
#      define Int8  long long
#      define Uint8 unsigned long long
#    endif

#  elif defined(PROC_ALPHA) 
#    define Uint8 unsigned long
#    define Int8  long

#  endif  /* #definitions of Int8,Uint8 for known platforms and compilers */



#  if  defined(Int8)  ||  defined(Uint8)

#    if !defined(Int8)  ||  !defined(Uint8)
       ;;;!!!PARTIALLY DEFINED 8-BYTE INTEGER TYPES!!!;;;
#    endif

     typedef Int8   Nlm_Int8;
     typedef Int8*  Nlm_Int8Ptr;
     typedef Uint8  Nlm_Uint8;
     typedef Uint8* Nlm_Uint8Ptr;

/* Have to undefine Int8 limits if already defined, as on some platforms
 * INT8_MAX, etc. exist but refer to 8-bit(not 8-byte!) integers
 */
#    if defined(INT8_MIN) || defined(INT8_MAX) || defined(UINT8_MAX)
#      if !defined(INT8_MIN) || !defined(INT8_MAX) || !defined(UINT8_MAX)
         ;;;!!!PARTIALLY DEFINED 8-BYTE INTEGER LIMITS!!!;;;
#      endif
#      undef INT8_MIN
#      undef INT8_MAX
#      undef UINT8_MAX
#    endif

#    if LONG_BIT==64
#      define INT8_MIN  LONG_MIN
#      define INT8_MAX  LONG_MAX
#      define UINT8_MAX ULONG_MAX

#    elif defined(__GNUC__)  &&  defined(LONG_LONG_MIN)
#      define INT8_MIN  LONG_LONG_MIN
#      define INT8_MAX  LONG_LONG_MAX
#      define UINT8_MAX ULONG_LONG_MAX

#    elif defined(OS_NT)
#      define INT8_MIN  _I64_MIN
#      define INT8_MAX  _I64_MAX
#      define UINT8_MAX _UI64_MAX

#    else /* def GNUC, LONG_BIT==64 */
#      ifdef LLONG_MIN
#        define INT8_MIN LLONG_MIN
#      else
#        define INT8_MIN (-9223372036854775807LL-1LL)
#      endif

#      ifdef LLONG_MAX
#        define INT8_MAX LLONG_MAX
#      else
#        define INT8_MAX 9223372036854775807LL
#      endif

#      ifdef ULLONG_MAX
#        define UINT8_MAX ULLONG_MAX
#      else
#        define UINT8_MAX 18446744073709551615ULL
#      endif
#    endif /* else def GNUC, LONG_BIT==64 */

     /* will be (re)defined after this #if */
#    undef  Int8
#    undef  Uint8

#  else
     /* Until we migrate to C++, and we can overload operations and
      * specify type conversions, we cannot simply define the Int8 datatype
      * as a structure...
      * Type the Int8 as Int4 so we can compile the tools.
      */
#    define Nlm_Int8     Nlm_Int4
#    define Nlm_Int8Ptr  Nlm_Int4Ptr
#    define Nlm_Uint8    Nlm_Uint4
#    define Nlm_Uint8Ptr Nlm_Uint4Ptr

#    define INT8_MIN  INT4_MIN
#    define INT8_MAX  INT4_MAX
#    define UINT8_MAX UINT4_MAX

#  endif /* else def Int8,Uint8 */


/* The final type definitions
 */
#  define Int8     Nlm_Int8
#  define Uint8    Nlm_Uint8
#  define Int8Ptr  Nlm_Int8Ptr
#  define Uint8Ptr Nlm_Uint8Ptr

#endif /* ndef Int8, ndef Uint8, ndef Int8Ptr, ndef Uint8Ptr */


#endif  /* _NCBIOPT_ */
