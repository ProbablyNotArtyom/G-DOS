
	#include <entry.h>
	#include <asm.h>
	#include <linkage.h>
	#include <cpu.h>

/* Startup file for the BSVC simulator suite */

	.title <startup.S>

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

/*
 * Set things up so the application will run. For historical reasons
 * this is called 'start'.  We set things up to provide '_start'
 * as with other systems, but also provide a weak alias called
 * 'start' for compatibility with existing linker scripts.
 */
	.globl _start

_start:
	bl	FUNC_NAME(main)