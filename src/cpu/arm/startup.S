
	#include <entry.h>
	#include <asm.h>

	.title <startup.S>
#define STACKSIZE	0x8000

/*
 * Define an empty environment.
 */

_C_LABEL (environ):
 		.align	2
		.text


/* These are defined in C code. */
	.extern _C_LABEL (main)
	.extern _C_LABEL (media_start)
	.extern _C_LABEL (exit)
	.extern _C_LABEL (atexit)

/*
 * These values are set in the linker script, so they must be
 * explicitly named here without _C_LABEL.
 */
	.extern __bss_start
	.extern _end

	.global _C_LABEL (sys_flash_program)
	.weak _C_LABEL (sys_flash_program)
	.set _C_LABEL (sys_flash_program),_C_LABEL(__program_flash)

	.global _C_LABEL (sys_disassemble)
	.weak _C_LABEL (sys_disassemble)
	.set _C_LABEL (sys_disassemble),_C_LABEL(__disassemble)
	.extern _C_LABEL (DCODE68K)
/*
 * Set things up so the application will run. For historical reasons
 * this is called 'start'.  We set things up to provide '_start'
 * as with other systems, but also provide a weak alias called
 * 'start' for compatibility with existing linker scripts.
 */
	.global _C_LABEL (start)
	.weak _C_LABEL (start)
	.set _C_LABEL (start),_C_LABEL(_start)

	.global _C_LABEL (_start)
_C_LABEL (_start):
	LDR sp, =_stack_end
	BL main
	B .
