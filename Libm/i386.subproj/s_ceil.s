/*
 * Written by Ian Ollmann
 *
 * Copyright © 2005, Apple Computer.  All Rights Reserved.
 */

#include <machine/asm.h>
#include "abi.h"

//We play a few games with the sign here to get the sign of ceil( -1 < x < 0 ) to come out right

ENTRY(ceill)
	fldt		FIRST_ARG_OFFSET(STACKP)	//{ f }
	fld			%ST(0)						//{ f, f }
	frndint									//{ rounded, f }
	fucomi		%ST(1), %ST					//  test for rounded > f
	fldz									//{ 0, rounded, f } 
	fld1									//{ 1, 0, rounded, f }
	fchs									//{ -1, 0, rounded, f }
	fcmovnb		%ST(1), %ST(0)				//{ 0 or -1, 0, rounded, f }
	fsubp		%ST(0), %ST(2)				//{ 0, (0 or -1) - rounded, f }
	fucomip		%ST(2), %ST					//{ (0 or -1) - rounded, f }
	fchs									//{ -((0 or -1) - rounded), f }
	fxch									//{ f, rounded - (0 or 1) }
	fcmovne		%ST(1), %ST(0)				//{ correct, rounded - (0 or 1)}
	fstp		%ST(1)
	ret


ENTRY(ceilf)
#if defined( __LP64__ )
    SUBP        $4, STACKP
    movss       %xmm0, (STACKP)
    flds        (STACKP)
#else
	flds		FIRST_ARG_OFFSET(STACKP)	//{ f }
#endif
	fld			%ST(0)						//{ f, f }
	frndint									//{ rounded, f }
	fucomi		%ST(1), %ST					//  test for rounded > f
	fldz									//{ 0, rounded, f } 
	fld1									//{ 1, 0, rounded, f }
	fchs									//{ -1, 0, rounded, f }
	fcmovnb		%ST(1), %ST(0)				//{ 0 or -1, 0, rounded, f }
	fsubp		%ST(0), %ST(2)				//{ 0, (0 or -1) - rounded, f }
	fucomip		%ST(2), %ST					//{ (0 or -1) - rounded, f }
	fchs									//{ -((0 or -1) - rounded), f }
	fxch									//{ f, rounded - (0 or 1) }
	fcmovne		%ST(1), %ST(0)				//{ correct, rounded - (0 or 1)}
	fstp		%ST(1)
#if defined( __LP64__ )
    fstps        (STACKP)
    movss       (STACKP), %xmm0
    ADDP        $4, STACKP
#endif
	ret

ENTRY(ceil)
#if defined( __LP64__ )
    SUBP        $8, STACKP
    movsd       %xmm0, (STACKP)
    fldl        (STACKP)
#else
	fldl		FIRST_ARG_OFFSET(STACKP)	//{ f }
#endif
	fld			%ST(0)						//{ f, f }
	frndint									//{ rounded, f }
	fucomi		%ST(1), %ST					//  test for rounded > f
	fldz									//{ 0, rounded, f } 
	fld1									//{ 1, 0, rounded, f }
	fchs									//{ -1, 0, rounded, f }
	fcmovnb		%ST(1), %ST(0)				//{ 0 or -1, 0, rounded, f }
	fsubp		%ST(0), %ST(2)				//{ 0, (0 or -1) - rounded, f }
	fucomip		%ST(2), %ST					//{ (0 or -1) - rounded, f }
	fchs									//{ -((0 or -1) - rounded), f }
	fxch									//{ f, rounded - (0 or 1) }
	fcmovne		%ST(1), %ST(0)				//{ correct, rounded - (0 or 1)}
	fstp		%ST(1)
#if defined( __LP64__ )
    fstpl        (STACKP)
    movsd       (STACKP), %xmm0
    ADDP        $8, STACKP
#endif
	ret


