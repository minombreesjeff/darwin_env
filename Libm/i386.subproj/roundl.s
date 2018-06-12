/*
 *  roundl.s
 *  LibmV5
 *
 *  Created by Ian Ollmann on 8/19/05.
 *  Copyright 2005 Apple Computer. All rights reserved.
 *
 */

#include "machine/asm.h"

#define LOCAL_STACK_SIZE	12
#include "abi.h"


ENTRY(roundl)
    SUBP        $LOCAL_STACK_SIZE,     STACKP
    movl		$0x3f000000, 8(STACKP)             //0.5f
    movl		$0x5f000000, 4(STACKP)             //0x1.0p63 

//convert |f| to an int
    //check to see if f is already an int or zero:      |f| >= 2**63 is an int
    flds        8(STACKP)             //  {0.5}
    fldt        FIRST_ARG_OFFSET(STACKP)            //  {f, 0.5}
    fabs                            //  {|f|, 0.5}
    flds        4(STACKP)             //  {limit,     |f|, 0.5}
    fucomip     %ST(1), %ST         //  {|f|, 0.5}       0x1.0p63 <= |f| or NaN

    //if it is a large int, NaN, replace it with 0.5. This avoids spurious overflows, illegal, and inexact
    fld         %ST(0)              //  {|f|,   |f|,    0.5  }
    fcmovbe     %ST(2), %ST(0)      //  {.5or|f|, |f|,  0.5  }

    //since we have removed large integers, inf and NaN from being used, we can now round safely
    //add 0.5 (with same sign as f) to f, or to the 0.5 we just put there. 
    faddp       %ST(0), %ST(2)      //  {|f|, .5or|f|+.5 }
    fxch                            //  {.5or|f|+.5,    |f|} 

    //then convert to int with truncation to zero
    fisttpll    (STACKP)              //  {|f|}   ***USES SSE3***

    //generate NaN result
    fldz                            //  {0, |f|}
    faddp                           //  {|f|}   NaN silenced
    
    //load the integer result back in
    fildll      (STACKP)              //  {|intf|,  |f|}

    //if 2**63 <= |f| or NaN, use |f| instead
    fcmovbe      %ST(1), %ST(0)     //  {|intf| or |f|,   |f|}
    fstp         %ST(1)             //  {|intf| or |f|}
    
    //deal with the sign
    fldt        FIRST_ARG_OFFSET(STACKP)            //  { f, |intf| or |f|} 
    fldz                            //  { 0, f, |intf| or |f| }
    fucomip     %ST(1), %ST         //  { f, |intf| or |f| }           0 < f or f is NaN 
    fld         %ST(1)              //  { |intf| or |f|,  f, |intf| or |f| }  
    fchs                            //  { -(intf or f+0 or f), f, intf or f+0 or f }
    fcmovb      %ST(2), %ST(0)      //  { result, f, intf or f+0 or f}
    
    //return f, if f == 0
    fcmove      %ST(1), %ST(0)      //  { result, f, intf or f+0 or f}
    
    //clean up the stack
    fstp        %ST(2)              //  { intf or f+0 or f, result }
    fstp        %ST(0)              //  { result }

    ADDP        $LOCAL_STACK_SIZE,    STACKP
    ret


ENTRY(llroundl)
	SUBP		$LOCAL_STACK_SIZE, STACKP
    movl		$0x3f000000, 8(STACKP)             //0.5f

	//calculate floor(|x|)
	fldt		FIRST_ARG_OFFSET(STACKP)			//	{f}
	fld			%st(0)				//	{ f, f }
	fabs							//	{|f|, f}
	fld			%st(0)				//	{|f|, |f|, f}
	frndint							//	{rint(|f|), |f|, f}	
	fucomi		%st(1), %st(0)		//	{rint(|f|), |f|, f}
	fldz							//	{0, rint(|f|), |f|, f}			//select between 0 and 1 here to avoid inexact on large inputs
	fld1							//	{1, 0, rint(|f|), |f|, f}		
	fcmove		%st(1), %st(0)		//	{1 or 0, 0, rint(|f|), |f|, f}
	fstp		%st(1)				//	{1 or 0, rint( |f| ), |f|, f }
	fsubr		%st(1), %st(0)		//	{rint(|f|-1or0, rint(|f|), |f|, f }
	fxch							//	{rint(|f|, rint(|f|)-1or0, |f|, f }
	fcmovnbe	%st(1), %st(0)		//	{floor(|f|), rint(|f|)-1, |f|, f}
	fstp		%st(1)				//	{floor(|f|), |f|, f}
	
	//calculate the difference between floor(|f|) and |f|
	fsubr		%st(1), %st(0)		//	{ |f| - floor(|f|), |f|, f }

    //if( |f| - floor(|f|) >= 0.5 )	add 0.5, otherwise add |f| - floor(|f|) (which gives similar results to adding zero)
    flds        8(STACKP)				//  { 0.5, |f| - floor(|f|), |f|, f }
	fucomi		%st(1), %st(0)		//	{ 0.5, |f| - floor(|f|), |f|, f }
	fcmovnb		%st(1), %st(0)		//	{0.5 or |f| - floor(|f|), |f| - floor(|f|), |f|, f }
	fstp		%st(1)				//	{0.5 or |f| - floor(|f|), |f|, f }
	faddp							//	{ |f| + 0.5 or |f| - floor(|f|), f }

	//set the sign properly
	fldz							//	{ 0, a, f }
	fucomip		%st(2), %st(0)		//	{ a, f }
	fld			%st(0)				//	{ a, a, f }
	fchs							//	{ -a, a, f }
	fcmovb		%st(1), %st(0)		//	{ +-a, a, f }
	fstp		%st(2)				//	{ a, +-a }
	fstp		%st(0)				//	{ +-a }

    //convert to long long
    fisttpll    (STACKP)              //  {}

    //load into return registers
#if defined( __LP64__ ) 
    movq       (STACKP),     %rax
#else
    movl        (STACKP),     %eax    
    movl        4(STACKP),    %edx
#endif
    
    ADDP        $LOCAL_STACK_SIZE,    STACKP
    ret
    
ENTRY(lroundl)
	SUBP		$LOCAL_STACK_SIZE, STACKP
    movl		$0x3f000000, 8(STACKP)             //0.5f
    movl		$0x4f000000, 4(STACKP)             //0x1.0p31f

	//calculate floor(|x|)
	fldt		FIRST_ARG_OFFSET(STACKP)			//	{f}
	fld			%st(0)				//	{ f, f }
	fabs							//	{|f|, f}
	fld			%st(0)				//	{|f|, |f|, f}
	frndint							//	{rint(|f|), |f|, f}	
	fucomi		%st(1), %st(0)		//	{rint(|f|), |f|, f}
	fldz							//	{0, rint(|f|), |f|, f}			//select between 0 and 1 here to avoid inexact on large inputs
	fld1							//	{1, 0, rint(|f|), |f|, f}		
	fcmove		%st(1), %st(0)		//	{1 or 0, 0, rint(|f|), |f|, f}
	fstp		%st(1)				//	{1 or 0, rint( |f| ), |f|, f }
	fsubr		%st(1), %st(0)		//	{rint(|f|-1or0, rint(|f|), |f|, f }
	fxch							//	{rint(|f|, rint(|f|)-1or0, |f|, f }
	fcmovnbe	%st(1), %st(0)		//	{floor(|f|), rint(|f|)-1, |f|, f}
	fstp		%st(1)				//	{floor(|f|), |f|, f}
	
	//calculate the difference between floor(|f|) and |f|
	fsubr		%st(1), %st(0)		//	{ |f| - floor(|f|), |f|, f }

    //if( |f| - floor(|f|) >= 0.5 )	add 0.5, otherwise add |f| - floor(|f|) (which gives similar results to adding zero)
    flds        8(STACKP)				//  { 0.5, |f| - floor(|f|), |f|, f }
	fucomi		%st(1), %st(0)		//	{ 0.5, |f| - floor(|f|), |f|, f }
	fcmovnb		%st(1), %st(0)		//	{0.5 or |f| - floor(|f|), |f| - floor(|f|), |f|, f }
	fstp		%st(1)				//	{0.5 or |f| - floor(|f|), |f|, f }
	faddp							//	{ |f| + 0.5 or |f| - floor(|f|), f }

	//set the sign properly
	fldz							//	{ 0, a, f }
	fucomip		%st(2), %st(0)		//	{ a, f }
	fld			%st(0)				//	{ a, a, f }
	fchs							//	{ -a, a, f }
	fcmovb		%st(1), %st(0)		//	{ +-a, a, f }
	fstp		%st(2)				//	{ a, +-a }
	fstp		%st(0)				//	{ +-a }
	
	//check for overflow
	xorl		%edx,	%edx		// zero edz
	flds		4(STACKP)				//	{ 0x1.0p31, +-a }
	fxch							//  { +-a, 0x1.0p31 }
	fucomi		%st(1), %st(0)		//	{ +-a, 0x1.0p31 }
	setnb		%dl

#if defined( __LP64__ ) 
	negq		%rdx
    //convert to long long
    fisttpll    (STACKP)              //  {0x1.0p31}
	fstp		%st(0)

    //load into return registers
    movq       (STACKP),     %rax
	xorq		DX_P,		%rax
#else   
	negl		%edx
    //convert to long
    fisttpl     (STACKP)              //  {0x1.0p31}
	fstp		%st(0)

    //load into return registers
    movl        (STACKP),     %eax
	xorl		DX_P,		%eax
#endif

    ADDP        $LOCAL_STACK_SIZE,		STACKP
    ret
	
ENTRY(round)
//convert |f| to an int
    //check to see if f is already an int or zero:      |f| >= 2**63 is an int
#if defined( __LP64__ )
    SUBP        $24,     STACKP
	movl		$0x3f000000, 20(STACKP)
	movl		$0x5f000000, 16(STACKP)
    flds        20(STACKP)             //  {0.5}
	movsd		%xmm0, 8(STACKP)
    fldl        8(STACKP)				//  {f, 0.5}
    fabs                            //  {|f|, 0.5}
    flds        16(STACKP)             //  {limit,     |f|, 0.5}
#else
    pushl       $0x3f000000             //0.5f
    pushl       $0x5f000000             //0x1.0p63 
    SUBP        $4,     STACKP
    flds        8(STACKP)             //  {0.5}
    fldl        (FIRST_ARG_OFFSET)(STACKP)            //  {f, 0.5}
    fabs                            //  {|f|, 0.5}
    flds        4(STACKP)             //  {limit,     |f|, 0.5}
#endif

    fucomip     %ST(1), %ST         //  {|f|, 0.5}       0x1.0p63 <= |f| or NaN

    //if it is a large int, NaN, replace it with 0.5. This avoids spurious overflows, illegal, and inexact
    fld         %ST(0)              //  {|f|,   |f|,    0.5  }
    fcmovbe     %ST(2), %ST(0)      //  {.5or|f|, |f|,  0.5  }

    //since we have removed large integers, inf and NaN from being used, we can now round safely
    //add 0.5 (with same sign as f) to f, or to the 0.5 we just put there. 
    faddp       %ST(0), %ST(2)      //  {|f|, .5or|f|+.5 }
    fxch                            //  {.5or|f|+.5,    |f|} 

    //then convert to int with truncation to zero
    fisttpll    (STACKP)              //  {|f|}   ***USES SSE3***

    //generate NaN result
    fldz                            //  {0, |f|}
    faddp                           //  {|f|}   NaN silenced
    
    //load the integer result back in
    fildll      (STACKP)              //  {|intf|,  |f|}

    //if 2**63 <= |f| or NaN, use |f| instead
    fcmovbe      %ST(1), %ST(0)     //  {|intf| or |f|,   |f|}
    fstp         %ST(1)             //  {|intf| or |f|}
    
    //deal with the sign
#if defined( __LP64__ )
    fldl        8(STACKP)            //  { f, |intf| or |f|} 
#else
    fldl        FIRST_ARG_OFFSET(STACKP)            //  { f, |intf| or |f|} 
#endif
    fldz                            //  { 0, f, |intf| or |f| }
    fucomip     %ST(1), %ST         //  { f, |intf| or |f| }           0 < f or f is NaN 
    fld         %ST(1)              //  { |intf| or |f|,  f, |intf| or |f| }  
    fchs                            //  { -(intf or f+0 or f), f, intf or f+0 or f }
    fcmovb      %ST(2), %ST(0)      //  { result, f, intf or f+0 or f}
    
    //return f, if f == 0
    fcmove      %ST(1), %ST(0)      //  { result, f, intf or f+0 or f}
    
    //clean up the stack
    fstp        %ST(2)              //  { intf or f+0 or f, result }
    fstp        %ST(0)              //  { result }

#if defined( __LP64__ )
	fstpl		(STACKP)
	movsd		(STACKP), %xmm0
    ADDP        $24,    STACKP
#else
    ADDP        $12,    STACKP
#endif

    ret

	
ENTRY(roundf)
    SUBP        $LOCAL_STACK_SIZE,     STACKP
    movl		$0x3f000000, 8(STACKP)             //0.5f
    movl		$0x5f000000, 4(STACKP)             //0x1.0p63 

//convert |f| to an int
    //check to see if f is already an int or zero:      |f| >= 2**63 is an int
    flds        8(STACKP)             //  {0.5}

#if defined( __LP64__ )
	movss		%xmm0,  8(STACKP)
	flds		8(STACKP)
#else
    flds        FIRST_ARG_OFFSET(STACKP)            //  {f, 0.5}
#endif
    fabs                            //  {|f|, 0.5}
    flds        4(STACKP)             //  {limit,     |f|, 0.5}
    fucomip     %ST(1), %ST         //  {|f|, 0.5}       0x1.0p63 <= |f| or NaN

    //if it is a large int, NaN, replace it with 0.5. This avoids spurious overflows, illegal, and inexact
    fld         %ST(0)              //  {|f|,   |f|,    0.5  }
    fcmovbe     %ST(2), %ST(0)      //  {.5or|f|, |f|,  0.5  }

    //since we have removed large integers, inf and NaN from being used, we can now round safely
    //add 0.5 (with same sign as f) to f, or to the 0.5 we just put there. 
    faddp       %ST(0), %ST(2)      //  {|f|, .5or|f|+.5 }
    fxch                            //  {.5or|f|+.5,    |f|} 

    //then convert to int with truncation to zero
    fisttpll    (STACKP)              //  {|f|}   ***USES SSE3***

    //generate NaN result
    fldz                            //  {0, |f|}
    faddp                           //  {|f|}   NaN silenced
    
    //load the integer result back in
    fildll      (STACKP)              //  {|intf|,  |f|}

    //if 2**63 <= |f| or NaN, use |f| instead
    fcmovbe      %ST(1), %ST(0)     //  {|intf| or |f|,   |f|}
    fstp         %ST(1)             //  {|intf| or |f|}
    
    //deal with the sign
#if defined( __LP64__ )
	flds		8(STACKP)
#else
    flds        16(STACKP)            //  { f, |intf| or |f|} 
#endif
    fldz                            //  { 0, f, |intf| or |f| }
    fucomip     %ST(1), %ST         //  { f, |intf| or |f| }           0 < f or f is NaN 
    fld         %ST(1)              //  { |intf| or |f|,  f, |intf| or |f| }  
    fchs                            //  { -(intf or f+0 or f), f, intf or f+0 or f }
    fcmovb      %ST(2), %ST(0)      //  { result, f, intf or f+0 or f}
    
    //return f, if f == 0
    fcmove      %ST(1), %ST(0)      //  { result, f, intf or f+0 or f}
    
    //clean up the stack
    fstp        %ST(2)              //  { intf or f+0 or f, result }
    fstp        %ST(0)              //  { result }

#if defined( __LP64__ )
	fstps		(STACKP)
	movss		(STACKP), %xmm0
#endif

    ADDP        $LOCAL_STACK_SIZE,    STACKP
    ret

