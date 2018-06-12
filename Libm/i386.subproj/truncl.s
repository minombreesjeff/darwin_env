/*
 *  truncl.s
 *  LibmV5
 *
 *  Created by Ian Ollmann on 9/1/05.
 *  Copyright 2005 Apple Computer. All rights reserved.
 *
 */

#include "machine/asm.h"

#define LOCAL_STACK_SIZE 12
#include "abi.h"

ENTRY(truncl)
    SUBP        $LOCAL_STACK_SIZE,     STACKP
    movl		$0x5f000000, 8(STACKP)             //0x1.0p63 

//convert |f| to an int
    //check to see if f is already an int or zero:      |f| >= 2**63 is an int
	fldz							//	{0}
    fldt        FIRST_ARG_OFFSET(STACKP)            //  {f, 0}
	fld			%st(0)				//	{f, f, 0 }
    fabs                            //  {|f|, f 0}
    flds        8(STACKP)             //  {limit,     |f|, f, 0}
    fucomip     %ST(1), %ST         //  {|f|, f, 0}       0x1.0p63 <= |f| or NaN
	fld			%st(0)				//	{|f|, |f|, f, 0 }

    //if it is a large int, NaN, replace it with 0. This avoids spurious overflows, illegal, and inexact
    fcmovbe     %ST(3), %ST(0)      //  { 0 or |f|, |f|, f, 0  }

    //then convert to int with truncation to zero
    fisttpll    (STACKP)              //  {|f|, f, 0}   ***USES SSE3***

    //generate NaN result
    fadd		%st(2)				//  {|f|+0, f, 0}   NaN silenced
    
    //load the integer result back in
    fildll      (STACKP)              //  {|intf|,  |f|+0, f, 0}

    //if 2**63 <= |f| or NaN, use f+0 instead
    fcmovbe      %ST(1), %ST(0)     //  {|intf| or |f|+0, |f|+0, f, 0 }
	fstp		%ST(1)				//	{|intf| or |f|+0, f, 0 }
	fld			%st(0)				//	{|intf| or |f|+0, |intf| or |f|+0, f, 0 }
	fchs							//  {-(|intf| or |f|+0), |intf| or |f|+0, f, 0 }
        
    //return f, if f == 0
	fxch		%ST(3)				//	{ 0, |intf| or |f|+0, f, -(|intf| or |f|+0) }
    fucomip     %ST(2), %ST         //  { |intf| or |f|+0, f, -(|intf| or |f|+0) }           0 < f or f is NaN 
    fcmovnbe    %ST(2), %ST(0)		//  { +-(|intf| or |f|+0), f, -(|intf| or |f|+0) }
    fcmove		%st(1), %st(0)		//	{ result, f, -(|intf| or |f|+0) }
	
    //clean up the stack
    fstp        %ST(2)              //  { f, result }
	fstp		%st(0)				//	{ result }

    ADDP        $LOCAL_STACK_SIZE,    STACKP
    ret

/*
//This runs about 10 cycles slower on Yonah than the more complicated one above
ENTRY( truncl )
    fldt        FRAME_SIZE(STACKP)          // { x }
    fld         %st(0)                      // { x, x }
    frndint                                 // { round(x), x }
    fucomip     %st(1), %st(0)              // { x }    // check to see if we already were an int or NaN
    jz          1f                          // if so, exit

    //rounding occurred so that means we must be non-zero, non-NaN and |x| < 2**23
    //In that case, we can round to int here with the right rounding mode without worring
    //about overflow
    SUBP        $8,     STACKP
    fisttpll    (STACKP)
    fildll      (STACKP)
    ADDP        $8,     STACKP
1:  ret
*/

    
ENTRY( truncf )
#if defined( __i386__ )
    flds        FRAME_SIZE(STACKP)          // { x }
    fld         %st(0)                      // { x, x }
    frndint                                 // { round(x), x }
    fucomip     %st(1), %st(0)              // { x }    // check to see if we already were an int or NaN
    jz          1f                          // if so, exit

    //rounding occurred so that means we must be non-zero, non-NaN and |x| < 2**23
    //In that case, we can round to int here with the right rounding mode without worring
    //about overflow
    SUBP        $4,     STACKP
    fisttpl     (STACKP)
    fildl       (STACKP)
    ADDP        $4,     STACKP
1:  ret

#else
    SUBP        $4,     STACKP
    movss       %xmm0,  (STACKP)
    flds        (STACKP)                    // { x }
    ADDP        $4,     STACKP
    fld         %st(0)                      // { x, x }
    frndint                                 // { round(x), x }
    fucomip     %st(1), %st(0)              // { x }    // check to see if we already were an int or NaN
	fstp		%st(0)
    jz          1f                          // if so, exit

    //rounding occurred so that means we must be non-zero, non-NaN and |x| < 2**23
    //In that case, we can round to int here with the right rounding mode without worring
    //about overflow
    cvttss2si   %xmm0, %eax
    cvtsi2ss    %eax, %xmm0
1:	ret
	
#endif

ENTRY( trunc )
#if defined( __i386__ )
    fldl        FRAME_SIZE(STACKP)          // { x }
    fld         %st(0)                      // { x, x }
    frndint                                 // { round(x), x }
    fucomip     %st(1), %st(0)              // { x }    // check to see if we already were an int or NaN
    jz          1f                          // if so, exit

    //rounding occurred so that means we must be non-zero, non-NaN and |x| < 2**23
    //In that case, we can round to int here with the right rounding mode without worring
    //about overflow
    SUBP        $8,     STACKP
    fisttpll    (STACKP)
    fildll      (STACKP)
    ADDP        $8,     STACKP
1:  ret

#else
    SUBP        $8,     STACKP
    movsd       %xmm0,  (STACKP)
    fldl        (STACKP)                    // { x }
    ADDP        $8,     STACKP
    fld         %st(0)                      // { x, x }
    frndint                                 // { round(x), x }
    fucomip     %st(1), %st(0)              // { x }    // check to see if we already were an int or NaN
	fstp		%st(0)
    jz          1f                          // if so, exit

    //rounding occurred so that means we must be non-zero, non-NaN and |x| < 2**23
    //In that case, we can round to int here with the right rounding mode without worring
    //about overflow
    cvttsd2siq  %xmm0, %rax
    cvtsi2sdq   %rax, %xmm0
1:	ret	
#endif

