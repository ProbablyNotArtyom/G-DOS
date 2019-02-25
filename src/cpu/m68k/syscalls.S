
	#include <linkage.h>

.section .rodata
ALIGN
ENTRY(syscall_table)
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
