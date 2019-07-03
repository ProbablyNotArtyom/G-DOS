#ifndef __M68K_ENTRY_H
#define __M68K_ENTRY_H

/*
 * Stack layout in 'ret_from_exception':
 *
 *	This allows access to the syscall arguments in registers d1-d5
 *
 *	 0(sp) - d1
 *	 4(sp) - d2
 *	 8(sp) - d3
 *	 C(sp) - d4
 *	10(sp) - d5
 *	14(sp) - a0
 *	18(sp) - a1
 *	1C(sp) - a2
 *	20(sp) - d0
 *	24(sp) - orig_d0
 *	28(sp) - stack adjustment
 *	2C(sp) - [ sr              ] [ format & vector ]
 *	2E(sp) - [ pc-hiword       ] [ sr              ]
 *	30(sp) - [ pc-loword       ] [ pc-hiword       ]
 *	32(sp) - [ format & vector ] [ pc-loword       ]
 *		  ^^^^^^^^^^^^^^^^^   ^^^^^^^^^^^^^^^^^
 *			M68K		  COLDFIRE
 */

#ifdef __ASSEMBLER__
/*
 * This defines the normal kernel pt-regs layout.
 *
 * regs a3-a6 and d6-d7 are preserved by C code
 * the kernel doesn't mess with usp unless it needs to
 */
#define SWITCH_STACK_SIZE	(6*4+4)	/* includes return address */

/*
 * All other types of m68k parts (68000, 680x0, CPU32) have the same
 * entry and exit code.
 */

/*
 * a -1 in the orig_d0 field signifies
 * that the stack frame is NOT for syscall
 */
.macro SAVE_ALL_INT
	clrl	%sp@-			/* stk_adj */
	pea	-1:w			/* orig d0 */
	movel	%d0,%sp@-		/* d0 */
	moveml	%d1-%d5/%a0-%a2,%sp@-
.endm

.macro SAVE_ALL_SYS
	clrl	%sp@-			/* stk_adj */
	movel	%d0,%sp@-		/* orig d0 */
	movel	%d0,%sp@-		/* d0 */
	moveml	%d1-%d5/%a0-%a2,%sp@-
.endm

.macro RESTORE_ALL
	moveml	%sp@+,%a0-%a2/%d1-%d5
	movel	%sp@+,%d0
	addql	#4,%sp			/* orig d0 */
	addl	%sp@+,%sp		/* stk adj */
	rte
.endm


.macro SAVE_SWITCH_STACK
	moveml	%a3-%a6/%d6-%d7,%sp@-
.endm

.macro RESTORE_SWITCH_STACK
	moveml	%sp@+,%a3-%a6/%d6-%d7
.endm

#else /* C source */

#define STR(X) STR1(X)
#define STR1(X) #X

#define SAVE_ALL_INT				\
	"clrl	%%sp@-;"    /* stk_adj */	\
	"pea	-1:w;"	    /* orig d0 = -1 */	\
	"movel	%%d0,%%sp@-;" /* d0 */		\
	"moveml	%%d1-%%d5/%%a0-%%a2,%%sp@-"

#define GET_CURRENT(tmp) \
	"movel	%%sp,"#tmp"\n\t" \
	"andw	#-"STR(THREAD_SIZE)","#tmp"\n\t" \
	"movel	"#tmp",%%a2\n\t" \
	"movel	%%a2@,%%a2"

#endif

#endif /* __M68K_ENTRY_H */
