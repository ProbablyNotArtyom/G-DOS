
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
 	.extern __syscall_handler
	.global vectors_start
	.global vectors_end

//---------------------------------------------------

//-------------------- IRQ Stuff --------------------

__syscall_handler:


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
syscall_table_end:
