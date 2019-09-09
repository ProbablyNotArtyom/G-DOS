#ifndef _HEADER_SYSCALL_DEFS
#define _HEADER_SYSCALL_DEFS

#define syscall_f_open		0
#define syscall_f_close		1
#define syscall_f_read		2
#define syscall_f_write		3
#define syscall_f_lseek		4
#define syscall_f_truncate	5
#define syscall_f_sync		6
#define syscall_f_opendir	7
#define syscall_f_closedir	8
#define syscall_f_readdir	9
#define syscall_f_mkdir		10
#define syscall_f_unlink	11
#define syscall_f_rename	12
#define syscall_f_stat		13
#define syscall_f_chdir		14
#define syscall_f_chdrive	15
#define syscall_f_getcwd	16
#define syscall_f_getfree	17
#define syscall_f_getlabel	18
#define syscall_f_setlabel	19
#define syscall_f_expand	20
#define syscall_f_mount		21
#define syscall_f_mkfs		22
#define syscall_f_putc		23
#define syscall_f_puts		24
#define syscall_f_printf	25
#define syscall_f_gets		26

#define syscall_pmalloc		27
#define syscall_malloc		28

#define syscall_read		29
#define syscall_putc		30

#ifndef __ASSEMBLER__
#include <errno.h>
#endif

#undef SYS_ify
#define SYS_ify(syscall_name)        syscall_##syscall_name

#define INLINE_SYSCALL(name, nr, args...)	4

#endif
