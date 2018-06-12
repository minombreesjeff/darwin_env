/*
 * Written by Ian Ollmann
 * Copyright © 2005, Apple Computer Inc. All rights reserved.
 */

#include <machine/asm.h>
#include "abi.h"


ENTRY( floorl )
	fldt		FIRST_ARG_OFFSET(STACKP)	//{ f }
	frndint									//{ rounded }
	fldt		FIRST_ARG_OFFSET(STACKP)	//{ f, rounded }
	fucomi		%ST(1), %ST					//  test for f >= rounded
	fldz									//{ 0, f, rounded } 
	fld1									//{ 1, 0, f, rounded }
	fcmovnb		%ST(1), %ST(0)				//{ 0 or 1, 0, f, rounded }
	fsubrp		%ST(0), %ST(3)				//{ 0, f, rounded - (0 or 1) }
	fucomip		%ST(1), %ST					//{ f, rounded - (0 or 1) }
	fcmovne		%ST(1), %ST(0)				//{ correct, rounded - (0 or 1)}
	fstp		%ST(1)
	ret


ENTRY( floorf )
#if defined( __LP64__ )
    SUBP    $4, STACKP
    movss   %xmm0, (STACKP)
    flds    (STACKP)
    frndint
    flds    (STACKP)
#else
	flds		4(STACKP)                   //{ f }
	frndint									//{ rounded }
	flds		4(STACKP)                   //{ f, rounded }
#endif
	fucomi		%ST(1), %ST					//  test for f >= rounded
	fldz									//{ 0, f, rounded } 
	fld1									//{ 1, 0, f, rounded }
	fcmovnb		%ST(1), %ST(0)				//{ 0 or 1, 0, f, rounded }
	fsubrp		%ST(0), %ST(3)				//{ 0, f, rounded - (0 or 1) }
	fucomip		%ST(1), %ST					//{ f, rounded - (0 or 1) }
	fcmovne		%ST(1), %ST(0)				//{ correct, rounded - (0 or 1)}
	fstp		%ST(1)
#if defined( __LP64__ )
    fstps        (STACKP)
    movss       (STACKP), %xmm0
    ADDP        $4, STACKP
#endif
	ret


ENTRY( floor )
#if defined( __LP64__ )
    SUBP    $8, STACKP
    movsd   %xmm0, (STACKP)
    fldl    (STACKP)
    frndint
    fldl    (STACKP)
#else
	fldl		4(STACKP)                   //{ f }
	frndint									//{ rounded }
	fldl		4(STACKP)                   //{ f, rounded }
#endif
	fucomi		%ST(1), %ST					//  test for f >= rounded
	fldz									//{ 0, f, rounded } 
	fld1									//{ 1, 0, f, rounded }
	fcmovnb		%ST(1), %ST(0)				//{ 0 or 1, 0, f, rounded }
	fsubrp		%ST(0), %ST(3)				//{ 0, f, rounded - (0 or 1) }
	fucomip		%ST(1), %ST					//{ f, rounded - (0 or 1) }
	fcmovne		%ST(1), %ST(0)				//{ correct, rounded - (0 or 1)}
	fstp		%ST(1)
#if defined( __LP64__ )
    fstpl        (STACKP)
    movsd       (STACKP), %xmm0
    ADDP        $8, STACKP
#endif
    ret
    

/*  
    //A xmm version that is 50% slower on Yonah. Might be faster some day...
    ENTRY( floorf )
    #if defined( __i386__ )
        movss		4(STACKP),		%xmm0
    #endif
        movl		$0x3f800000,	%eax			//1.0f
        movd		%eax,			%xmm7			// 1.0f
        movl		$0x4b000000,	%ecx			//0x1.0p23f
        movd		%ecx,			%xmm6			// 2**23
        movl		$0x80000000,	%edx			//-0.0f
        movd		%edx,			%xmm5			// -0.0

        //set aside the sign of x, flush NaNs to zero and take fabsf(x)
        movaps		%xmm0,			%xmm1			
        andps		%xmm5,			%xmm1			//sign of x
        movaps		%xmm0,			%xmm2	
        cmpunordps	%xmm2,			%xmm2	
        movaps		%xmm5,			%xmm4			//x
        andnps		%xmm0,			%xmm4			//fabsf(x)
        andnps		%xmm4,			%xmm2			//safe fabs(x)		(NaNs flushed to zero)
        
        //flush step to zero if x is already an integer
        movaps		%xmm2,			%xmm3
        cmpltss		%xmm6,			%xmm3
        andps		%xmm3,			%xmm6			//2**23 or zero if x is already an integer
        
        //check for non-zero
        cmpeqss		%xmm2,			%xmm5			// |safex| == 0
        
        //round to integer
        movaps		%xmm2,			%xmm3
        addss		%xmm6,			%xmm2
        subss		%xmm6,			%xmm2			//round_to_int( safeX)
        
        //restore the sign of round_to_int( safeX) and safeX
        orps		%xmm1,			%xmm2
        orps		%xmm1,			%xmm3
        
        //Fix up mistaken rounding for rounding modes other than round to -Inf
        cmpltss		%xmm2,			%xmm3
        andps		%xmm7,			%xmm3
        subss		%xmm3,			%xmm2
        
        //select between x and the result based on the |safex| > 0 test
        andps		%xmm5,			%xmm0
        andnps		%xmm2,			%xmm5
        orps		%xmm5,			%xmm0

    #if defined( __i386__ )
        SUBP		$4,              STACKP
        movss		%xmm0,			(STACKP)
        flds		(STACKP)
        ADDP		$4,              STACKP
    #endif
        ret

*/
