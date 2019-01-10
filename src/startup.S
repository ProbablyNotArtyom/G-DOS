/* Startup file for the BSVC simulator suite */

#include <asm.h>

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

CACR_EI   = 1       /* Enable Instruction Cache      */
CACR_FI   = 1<<1    /* Freeze Instruction Cache      */
CACR_CEI  = 1<<2    /* Clear Entry in Instr. Cache   */
CACR_CI   = 1<<3    /* Clear Instruction Cache       */
CACR_IBE  = 1<<4    /* Instr. Cache Burst Enable     */

CACR_ED   = 1<<8    /* Enable Data Cache             */
CACR_FD   = 1<<9    /* Freeze Data Cache             */
CACR_CED  = 1<<10   /* Clear Entry in Data Cache     */
CACR_CD   = 1<<11   /* Clear Data Cache              */
CACR_DBE  = 1<<12   /* Data Cache Burst Enable       */
CACR_WA   = 1<<13   /* Write Allocate the Data Cache */
BOARD_CACR0	= (CACR_CI + CACR_EI + CACR_CD + CACR_ED)

/*
 * These values are set in the linker script, so they must be
 * explicitly named here without SYM.
 */
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

	or.w    #0x2700, sr 			/* disable interrupts */
	movel	#(__STACK), a0
	cmpl	#(0), a0
	jbeq    1f
	movel	a0, sp

#ifdef CONFIG_BLITZ
	clrl   	d2                 		/* set Vector Base Register */
	movecl 	d2, vbr
	movel  	#BOARD_CACR0, d2    	/* enable data & instruction caches */
	movecl 	d2, cacr
#endif
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
