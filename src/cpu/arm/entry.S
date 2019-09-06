
	#include <entry.h>
	#include <asm.h>
	#include <linkage.h>

 .section .vectors, "x"

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

 	.extern _C_LABEL (_start)
 	.extern _C_LABEL (__swi_handler)
 	.extern _C_LABEL (_unvectored_start)

	.global _C_LABEL (vectors_start)
	.global _C_LABEL (vectors_end)

//---------------------------------------------------

vectors_start:
_C_LABEL (_start):
	ldr 	pc, _unvectored_start_addr
    ldr 	pc, _undef_vector_addr
    ldr 	pc, _swi_vector_addr
	ldr 	pc, _undef_vector_addr
	ldr 	pc, _undef_vector_addr
	b		.

_unvectored_start_addr: .word _unvectored_start
_undef_vector_addr: 	.word _unvectored_start
_swi_vector_addr: 		.word __swi_handler
vectors_end:

//-------------------- SWI Stuff --------------------

__swi_handler:
	adr		r8, syscall_table
	ldr		pc, [r8, r7, lsl #2]

syscall_table:
	.long	f_open
	.long	f_close
	.long	f_read
	.long	f_write
	.long	f_lseek
	.long	f_truncate
	.long	f_sync
	.long	f_opendir
	.long	f_closedir
	.long	f_readdir
	.long	f_mkdir
	.long	f_unlink
	.long	f_rename
	.long	f_stat
	.long	f_chdir
	.long	f_chdrive
	.long	f_getcwd
	.long	f_getfree
	.long	f_getlabel
	.long	f_setlabel
	.long	f_expand
	.long	f_mount
	.long	f_mkfs
	.long	f_putc
	.long	f_puts
	.long	f_printf
	.long	f_gets

	.long	pmalloc
	.long	malloc

	.long	read
	.long	putc
