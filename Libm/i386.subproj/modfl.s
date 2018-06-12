/*
 *  modfl.s
 *  LibmV5
 *
 *  Created by Ian Ollmann on 9/6/05.
 *  Copyright 2005 Apple Computer. All rights reserved.
 *
 */

#include <machine/asm.h>
#define LOCAL_STACK_SIZE	12
#include "abi.h"

#if ! defined( __SSE3__ )
	#error	modfl function requires SSE3
#endif

#if defined( __LP64__ )
	#define DEST_P			%rdi
	#define LOAD_DEST_P
#else
	#define DEST_P			%eax
	#define LOAD_DEST_P		mov			SECOND_ARG_OFFSET(STACKP), DEST_P
#endif

// I tried branch free code here. Alas there were so many special cases, that 2/3 of the code was patchup after the fidll instruction.
// I've moved some special cases { 0, +-inf, NaN} out, which simplifies things quite a bit on any path you care to follow.

ENTRY( modfl )
	SUBP		$LOCAL_STACK_SIZE, STACKP
	movl		$0x7f800000, 8(STACKP)				//inf
	movl		$0x5f000000, 4(STACKP)				//1.0p63f
	
	fldt		FIRST_ARG_OFFSET(STACKP)				// {x}
	fld			%st(0)					// {x}
	fabs								// {|x|, x}
	LOAD_DEST_P								//				load *iptr
	flds		4(STACKP)					// {1.0p63, |x|, x}
	fucomip		%st(1), %st(0)			// {|x|, x}			1.0p63 > |x| 
	fldz								// { 0, |x|, x }
	fcmovnbe	%st(2), %st(0)			// { 0 or x, |x|, x }
	fisttpll	(STACKP)					// { |x|, x }					***uses sse3***

	//patch up x is finite integer case
	fildll		(STACKP)					// { i or 0, |x|, x}
	fcmovbe		%st(2), %st(0)			// { i, |x|, x }					//copy back x for all large integers, Inf and NaN

	//get zero and NaN out of the main path
	fldz
	fucomip		%st(1), %st(0)
	je			modfl_zero
		
	//deal with infinity
	flds		8(STACKP)					// { inf, i, |x|, x }
	fucomip		%st(2), %st(0)			// { i, |x|, x }
	fstp		%st(1)					// { i, x }
	je			modfl_inf
	
	//find the fraction
	fsubr		%st(0), %st(1)			// {i, f }

	//deal with the sign of f == 0
	fld			%st(1)					// { f, i, f}
	fchs								// { -f, i, f }
	fucomi		%st(2), %st(0)			// { -f, i, f }
	fcmovne		%st(2), %st(0)			// { +-f, i, f }
	fucomi		%st(1), %st(0)			
	fcmovb		%st(2), %st(0)
	fstp		%st(2)

	//return result
	fstpt		(DEST_P)				// { i, f }
	
	ADDP		$LOCAL_STACK_SIZE, STACKP
	ret
	
modfl_inf:
	fstp		%st(0)					// { x }
	fldz								// { 0, x }
	fchs								// { -0, x }
	fldz								// { 0, -0, x }
	fucomi		%st(2), %st(0)			// { 0, -0, x }
	fcmovnb		%st(1), %st(0)			// {+-0, -0, x }
	fxch		%st(2)					// { x, -0, +-0 }
	fstpt		(DEST_P)				// { -0, +-0 }
	fstp		%st(0)
	ADDP		$LOCAL_STACK_SIZE, STACKP
	ret
	
modfl_zero:								// { i, |x|, x }		handles 0 and NaN
	//set i to have the same sign as x
	//while we could do this for most cases by giving a signed x to fisttpll, the zero case always comes back +0
	//so we save a fabs operation by throwing away the sign early
	fstp		%st(1)					// { i, x }
	fabs								// { |i|, x }
	fucomi		%st(1), %st(0)			// { |i|, x }
	fld			%st(0)					// { |i|, |i|, x }
	fchs								// { -|i|, |i|, x }
	fcmovb		%st(1), %st(0)			// { +-|i|, |i|, x }		//Handle x > 0
	fcmove		%st(2), %st(0)			// { i, |i|,  x }			//Handle x == 0 and i is NaN
	fstp		%st(1)					// { i, x }
	fstpt		(DEST_P)				// { x }
	ADDP		$LOCAL_STACK_SIZE, STACKP
	ret
	
	
	