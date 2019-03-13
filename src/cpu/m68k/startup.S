/* Startup file for the BSVC simulator suite */

	#include <asm.h>
	#include <entry.h>

	.title <startup.S>
#define STACKSIZE	0x8000

#ifndef __STACK
#define __STACK		0x300000
#endif

/*
 * Define an empty environment.
 */

SYM (environ):
 		.align	2
		.text


/* These are defined in C code. */
	.extern SYM (main)
	.extern SYM (media_start)
	.extern SYM (exit)
	.extern SYM (atexit)

/*
 * These values are set in the linker script, so they must be
 * explicitly named here without SYM.
 */
	.extern __bss_start
	.extern _end

	.global SYM (sys_flash_program)
	.weak SYM (sys_flash_program)
	.set SYM (sys_flash_program),SYM(__program_flash)

	.global SYM (sys_disassemble)
	.weak SYM (sys_disassemble)
	.set SYM (sys_disassemble),SYM(__disassemble)
	.extern SYM (DCODE68K)
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

	or.w    #0x2700, sr 			/* disable interrupts */
	movel	#(__STACK), a0


	cmpl	#(0), a0
	jbeq    1f
	movel	a0, sp

1:
	/* set up initial stack frame */
	link	a6, #(-8)

	/* Clear BSS */
 	leal   _end, a0
 	movel  a0, d2
 	leal   __bss_start, a1
 	subl   a1, a0
 	movel  a0, d0
 	jbra   3f
2:	clrb   (a1)+
3:	dbra    d0, 2b

	lea.l   __data_end, a0
	lea.l   __data_start, a1    /* target in a1 */
	sub.l   a1, a0
	move.l  a0, d0             /* num bytes to copy in d0 */
	lea.l   __data_rom, a2      /* source in a2 */
	jbra    5f
4:
	move.b  (a2)+, (a1)+
5:
	dbra    d0, 4b

        pea     0
	PICPEA	SYM (environ),a0
        pea     sp@(4)
        pea     0
	pea	(a1)
	PICCALL	SYM (main) /* Here the main(..) function is called */
	addq.l	#4,sp
	unlk	a6
	rts

	.align 2

sys_disassemble:
	movea.l	sp@(8), a0
	movea.l	sp@(12), a1
	move.l	d2, -(sp)
	move.l	d3, -(sp)
	move.l	d4, -(sp)
	move.l	d5, -(sp)
	move.l	d6, -(sp)
	move.l	d7, -(sp)
	move.l	a2, -(sp)
	move.l	a3, -(sp)
	move.l	a4, -(sp)
	move.l	a5, -(sp)
	move.l	a6, -(sp)


	movea.l	a0, a4
	movea.l a1, a5
	movea.l	a4, a3
	move.l	(a3)+, d0
	move.l	(a3)+, d1
	move.l	(a3)+, d2

	bsr		disassemble

	move.l	a4, d0

	movea.l	(sp)+, a6
	movea.l	(sp)+, a5
	movea.l	(sp)+, a4
	movea.l	(sp)+, a3
	movea.l	(sp)+, a2
	move.l	(sp)+, d7
	move.l	(sp)+, d6
	move.l	(sp)+, d5
	move.l	(sp)+, d4
	move.l	(sp)+, d3
	move.l	(sp)+, d2

	rts
