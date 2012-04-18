/* addone.s - Assembly routine to increment a passed in value */

/*
 * Copyright (c) 2004-2006, 2011 Wind River Systems, Inc.
 *
 * The right to copy, distribute or otherwise make use of this software
 * may be licensed only pursuant to the terms of an applicable Wind River
 * license agreement. No license to Wind River intellectual property rights
 * is granted herein. All rights not licensed by Wind River are reserved
 * by Wind River.
 */

/*
modification history
--------------------
01g,19jan11,pee  WIND00251395
01f,07jan11,mcm  Added support for X86_64
01e,25apr06,rec  added coldfire support (SPR-117923)
01d,01feb05,h_k  added SH and ARM support. (SPR #105984)
01c,15nov05,gvm  Fix for SPR102730: using C style comments for SIMSPARCSOLARIS
01b,08nov04,pes  added #include "asm.h"
01a,07oct04,f_b  added
*/

#define _ASMLANGUAGE

#include <vxWorks.h>
#ifndef _VX_CPU_FAMILY
#error build error: -DVX_CPU=yourCPU missing
#endif
#include "asm.h"

	.text
	.align 4

/*
* Increment the passed in parameter by one.
*/

#if (_VX_CPU_FAMILY == _VX_PPC)

	.globl	addone
addone:
	addi	r3,r3,1	# increment parameter...
	bclr	20,0	# go back


#elif (_VX_CPU_FAMILY == _VX_SIMNT || _VX_CPU_FAMILY == _VX_SIMLINUX || \
	_VX_CPU_FAMILY == _VX_SIMPENTIUM || _VX_CPU_FAMILY == _VX_I80X86 )
	
	.globl	addone
addone:
#if defined(_WRS_CONFIG_LP64)
	movl	%edi, %eax
	addl	$1, %eax
#else
	movl	4(%esp), %eax
	incl	%eax
#endif
	ret


#elif (_VX_CPU_FAMILY==_VX_SIMSPARCSUNOS || \
	_VX_CPU_FAMILY==_VX_SIMSPARCSOLARIS)

#include "arch/simsolaris/asmSimsolaris.h"

	.global GTEXT(addone)
FUNC_LABEL(addone)
	retl		/* deferred return, return value follows */
	add %o0,1,%o0	/* increment parameter...  */


#elif (_VX_CPU_FAMILY==_VX_MIPS)

	.globl	addone
	.ent	addone
addone:
	addi	v0, a0, 1	# increment parameter...
	jr	ra
	nop
	
	.end	addone


#elif (_VX_CPU_FAMILY==_VX_SH)

	.global	GTEXT(addone)

FUNC_LABEL(addone)
	mov	r4,r0
	rts
	add	#1,r0


#elif (_VX_CPU_FAMILY==_VX_ARM)

	.globl	FUNC(addone)
_ARM_FUNCTION_CALLED_FROM_C(addone)
	ADD	r0, r0, #1
#if (ARM_THUMB)
	BX	lr
#else
	MOV	pc, lr
#endif


#elif (_VX_CPU_FAMILY==_VX_COLDFIRE)
	/* externals */
	
	.text
	
	.globl	FUNC(addone)
FUNC_LABEL(addone)
	movel	a7@(SP_ARG1), d0	/* argument passed */
        addql   #1, d0                  /* increment */
        rts

#else
#error Unknown VX_CPU Family
#endif
