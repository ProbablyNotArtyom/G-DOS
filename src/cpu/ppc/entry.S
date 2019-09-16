
	#include <entry.h>
	#include <asm.h>
	#include <linkage.h>
	#include <cpu.h>

/*
 * Set things up so the application will run. For historical reasons
 * this is called 'start'.  We set things up to provide '_start'
 * as with other systems, but also provide a weak alias called
 * 'start' for compatibility with existing linker scripts.
 */

	.global _start
 	.global __syscall_handler
	.global vectors_start
	.global vectors_end

//---------------------------------------------------

//-------------------- IRQ Stuff --------------------

__syscall_handler:
	rfi
