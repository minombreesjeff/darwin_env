/*
 * Copyright (c) 2002 Apple Computer, Inc. All rights reserved.
 *
 * @APPLE_LICENSE_HEADER_START@
 * 
 * Copyright (c) 1999-2003 Apple Computer, Inc.  All Rights Reserved.
 * 
 * This file contains Original Code and/or Modifications of Original Code
 * as defined in and that are subject to the Apple Public Source License
 * Version 2.0 (the 'License'). You may not use this file except in
 * compliance with the License. Please obtain a copy of the License at
 * http://www.opensource.apple.com/apsl/ and read it before using this
 * file.
 * 
 * The Original Code and all software distributed under the License are
 * distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
 * EXPRESS OR IMPLIED, AND APPLE HEREBY DISCLAIMS ALL SUCH WARRANTIES,
 * INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR NON-INFRINGEMENT.
 * Please see the License for the specific language governing rights and
 * limitations under the License.
 * 
 * @APPLE_LICENSE_HEADER_END@
 */
/*******************************************************************************
*                                                                              *
*    tg.c                                                                      *
*                                                                              *
*    Double precision Tangent                                                  *
*                                                                              *
*    Copyright: © 1993-1997 by Apple Computer, Inc., all rights reserved       *
*                                                                              *
*    Written and ported by A. Sazegari, started on June 1997.                  *
*    Modified by Robert Murley, 2001                                           *
*                                                                              *
*    A MathLib v4 file.                                                        *
*                                                                              *
*    Based on the trigonometric functions from IBM/Taligent.                   *
*                                                                              *
*    Modification history:                                                     *
*    November  06 2001: commented out warning about Intel architectures.       *
*    July      20 2001: replaced __setflm with FEGETENVD/FESETENVD.            *
*                       replaced DblInHex typedef with hexdouble.              *
*    September 07 2001: added #ifdef __ppc__.                                  *
*    September 19 2001: added macros to detect PowerPC and correct compiler.   *
*    September 19 2001: added <CoreServices/CoreServices.h> to get to <fp.h>   *
*                       and <fenv.h>, removed denormal comments.               *
*    September 25 2001: removed more denormal comments.                        *
*    October   08 2001: removed <CoreServices/CoreServices.h>.                 *
*                       changed compiler errors to warnings.                   *
*                                                                              *
*    W A R N I N G:                                                            *
*    This routine requires a 64-bit double precision IEEE-754 model.           *
*    They are written for PowerPC only and are expecting the compiler          *
*    to generate the correct sequence of multiply-add fused instructions.      *
*                                                                              *
*    This routine is not intended for 32-bit Intel architectures.              *
*                                                                              *
*     08 Oct 01   ram   changed compiler errors to warnings.                   *
*                                                                              *
*      GCC compiler options:                                                   *
*            optimization level 3 (-O3)                                        *
*            -fschedule-insns -finline-functions -funroll-all-loops            *
*                                                                              *
*******************************************************************************/

#ifdef      __APPLE_CC__
#if         __APPLE_CC__ > 930

#endif      /* __APPLE_CC__ version */
#endif      /* __APPLE_CC__ */
