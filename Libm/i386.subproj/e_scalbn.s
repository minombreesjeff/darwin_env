/*
 *  e_scalbn.s
 *
 *	by Ian Ollmann
 *
 *	Copyright © 2005 Apple Computer. All Rights Reserved.
 */
 
#include <machine/asm.h>
#include "abi.h"

	
#if defined( __LP64__ )

ENTRY( scalbnl )
	pushq	%rdi
	fildl	(STACKP)						//{ scale }
	popq	%rdi
	fldt	FIRST_ARG_OFFSET(STACKP )		//{ f, scale }
	fscale
	fstp	%st(1)
	ret

#else

ENTRY( scalbnl )
	fildl	SECOND_ARG_OFFSET(STACKP )		//{ scale }
	fldt	FIRST_ARG_OFFSET(STACKP )		//{ f, scale }
	fscale
	fstp	%st(1)
	ret

#endif	
