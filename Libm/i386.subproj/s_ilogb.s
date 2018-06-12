/*
 * Written by Ian Ollmann
 *
 * Copyright © 2005, Apple Computer. All rights reserved.
 */

#include <machine/asm.h>

#if defined( __LP64__ )
	#error not 64-bit ready
#endif


ENTRY(ilogbl)
	pushl	$0x4f000000				// 0x1.0p31
	fld1							//	{ 1 }
	flds	(%esp)					//  { 1.0p31, 1 }
	fsub	%st(0), %st(1)			//	{ 0x1.0p31, 0x1.0p31 - 1 }
	fchs							//	{ -0x1.0p31, 0x1.0p31 - 1 }
	fldt	8(%esp)					//	{x, -0x1.0p31, 0x1.0p31 - 1 }
	fxtract							//	{ sig, exp, -0x1.0p31, 0x1.0p31 - 1 }
	fstp	%st(0)					//	{ exp, -0x1.0p31, 0x1.0p31 - 1 }
	fucomi  %st(2), %st(0)			//	{ exp, -0x1.0p31, 0x1.0p31 - 1 }
	fcmovnbe	%st(2), %st(0)		//	{ exp or 0x1.0p31 - 1, -0x1.0p31, 0x1.0p31 - 1 }
	fstp	%st(2)					//	{ -0x1.0p31, exp or 0x1.0p31 - 1 }
	fucomi	%st(1), %st(0)			//	{ -0x1.0p31, exp or 0x1.0p31 - 1 }
	fcmovb	%st(1), %st(0)			//	{ result, exp or 0x1.0p31 - 1 }
	fistpl	(%esp)					//	{exp or 0x1.0p31 - 1}
	fstp	%st(0)
	movl	(%esp), %eax
	addl	$4, %esp
	ret
