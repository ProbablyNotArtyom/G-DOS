
	#include <entry.h>
	#include <asm.h>
	#include <asm.h>
	#include <linkage.h>
	#include <traps.h>
	#include <entry.h>
	#include <syscall.h>

.globl addrerr, buserr, trap, _unvectored_start

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

	.extern _C_LABEL (copy_vectors)

_unvectored_start:
	LDR sp, =_stack_end
	BL	copy_vectors
	BL	main
	B	.
