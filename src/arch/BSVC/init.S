/* Startup file for the BSVC simulator suite */

#include "asm.h"

	.title <init.S>
#define STACKSIZE	0x8000
#define __stack		0x300000

/*
 * Define an empty environment.
 */
        .data
        .align 2
SYM (environ):
        .long 0

 		.align	2
		.text

/*
 * These symbols are defined in C code, so they need to always be
 * named with SYM because of the difference between object file formats.
 */

/* These are defined in C code. */
	.extern SYM (main)
	.extern SYM (media_start)
	.extern SYM (exit)
	.extern SYM (atexit)

/*
 * These values are set in the linker script, so they must be
 * explicitly named here without SYM.
 */
	.extern __stack
	.extern __bss_start
	.extern _end

/*
 * Set things up so the application will run. For historical reasons
 * this is called 'start'.  We set things up to provide '_start'
 * as with other systems, but also provide a weak alias called
 * 'start' for compatibility with existing linker scripts.
 */
	.global SYM (start)
	.weak SYM (start)
	.set SYM (start),SYM(_start)

	.global SYM (_start)
SYM (_start):

	/* See if user supplied their own stack (__stack != 0).  If not, then
	 * default to using the value of %sp as set by the ROM monitor.
	 */
	movel	#(__stack), a0
	cmpl	#(0), a0
	jbeq    1f
	movel	a0, sp
1:
	/* set up initial stack frame */
	link	a6, #(-8)

/*
 * zero out the bss section.
 */
	movel	#(__bss_start), d1
	movel	#(_end), d0
	cmpl	d0, d1
	jbeq	5f
	movl	d1, a0
	subl	d1, d0
	subql	#(1), d0
2:
	clrb	(a0)+
	subql	#(1), d0
	jbpl	2b

5:
	move.l	#0x00EFFC01, a1
	move.b	#0b00010000, 0x00EFFC05
	move.b	#0b00100011, 0x00EFFC01
	move.b	#0b00010111, 0x00EFFC01
	move.b	#0b11001011, 0x00EFFC03
	move.b	#0b00000101, 0x00EFFC05

        pea     0
	PICPEA	SYM (environ),a0
        pea     sp@(4)
        pea     0 
	pea	(a1)
	PICCALL	SYM (main) /* Here the main(..) function is called */
	addq.l	#4,sp
	unlk	a6
	rts
