/*
 * Written by Ian Ollmann.
 * Copyright © 2005 Apple Computer Inc.
 */

#include <machine/asm.h>

#include "abi.h"


ENTRY( copysignf )
#if defined( __i386__ )
    movss   FIRST_ARG_OFFSET(STACKP), %xmm0
    movss   (FIRST_ARG_OFFSET+4)(STACKP), %xmm1
#endif
    pcmpeqw     %xmm2,  %xmm2           //0xffffffff
    pslld       $31,    %xmm2           //0x80000000
    andps       %xmm2,  %xmm1
    andnps      %xmm0,  %xmm2
    orps        %xmm2,  %xmm1
#if defined( __i386__ )
    SUBP        $4,     STACKP
    movss       %xmm1,  (STACKP)
    flds        (STACKP)
    ADDP        $4,     STACKP
#else
    movaps      %xmm1,  %xmm0
#endif
    ret

#if defined( __x86_64__ )    
ENTRY( copysign )
    pcmpeqw     %xmm2,  %xmm2           //0xffffffff
    psrlq       $1,     %xmm2           //0x80000000
    andpd       %xmm2,  %xmm0
    andnpd      %xmm1,  %xmm2
    orpd        %xmm2,  %xmm0
    ret
#endif

ENTRY( fabsf )
#if defined( __i386__ )
    flds        FIRST_ARG_OFFSET(STACKP)
    fabs
    ret
#else
    pcmpeqw     %xmm1,  %xmm1           //0xffffffff
    psrld       $1,     %xmm1           //0x7fffffff
    andps       %xmm1,  %xmm0
    ret
#endif

ENTRY( fabs )
#if defined( __i386__ )
    fldl        FIRST_ARG_OFFSET(STACKP)
    fabs
    ret
#else
    pcmpeqw     %xmm1,  %xmm1           //0xffffffffffffffff
    psrlq       $1,     %xmm1           //0x7fffffffffffffff
    andpd       %xmm1,  %xmm0
    ret
#endif

ENTRY( fabsl )
    fldt    FIRST_ARG_OFFSET(STACKP)
    fabs
    ret
    
