
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
#ifdef CONFIG_RPI2
	// Shut off extra cores
	mrc p15, 0, r5, c0, c0, 5
	and r5, r5, #3
	cmp r5, #0
	bne 3f

	// Setup the stack.
	ldr r5, =_start
	mov sp, r5

	// Clear out bss.
	ldr r4, =__bss_start
	ldr r9, =_end
	mov r5, #0
	mov r6, #0
	mov r7, #0
	mov r8, #0
	b       2f

1:
	// store multiple at r4.
	stmia r4!, {r5-r8}

	// If we are still below bss_end, loop.
2:
	cmp r4, r9
	blo 1b
3:
#else
	ldr sp, =_stack_end
#endif
	BL	copy_vectors
	BL	main
	B	.
