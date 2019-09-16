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
#include <types.h>
#endif

#undef SYS_ify
#define SYS_ify(syscall_name)        syscall_##syscall_name

/* On powerpc a system call basically clobbers the same registers like a
 * function call, with the exception of LR (which is needed for the
 * "sc; bnslr" sequence) and CR (where only CR0.SO is clobbered to signal
 * an error return status).
 */

#define __sc_asm_input_0 "0" (__sc_0)
#define __sc_asm_input_1 __sc_asm_input_0, "1" (__sc_3)
#define __sc_asm_input_2 __sc_asm_input_1, "2" (__sc_4)
#define __sc_asm_input_3 __sc_asm_input_2, "3" (__sc_5)
#define __sc_asm_input_4 __sc_asm_input_3, "4" (__sc_6)
#define __sc_asm_input_5 __sc_asm_input_4, "5" (__sc_7)

#define __sc_loadargs_0(name, dummy...)					\
	__sc_0 = syscall_##name
#define __sc_loadargs_1(name, arg1)					\
	__sc_loadargs_0(name);						\
	__sc_3 = (unsigned long) (arg1)
#define __sc_loadargs_2(name, arg1, arg2)				\
	__sc_loadargs_1(name, arg1);					\
	__sc_4 = (unsigned long) (arg2)
#define __sc_loadargs_3(name, arg1, arg2, arg3)				\
	__sc_loadargs_2(name, arg1, arg2);				\
	__sc_5 = (unsigned long) (arg3)
#define __sc_loadargs_4(name, arg1, arg2, arg3, arg4)			\
	__sc_loadargs_3(name, arg1, arg2, arg3);			\
	__sc_6 = (unsigned long) (arg4)
#define __sc_loadargs_5(name, arg1, arg2, arg3, arg4, arg5)		\
	__sc_loadargs_4(name, arg1, arg2, arg3, arg4);			\
	__sc_7 = (unsigned long) (arg5)

#define _syscall0(type,name)						\
type name(void)								\
{									\
	__syscall_nr(0, type, name);					\
}

#define _syscall1(type,name,type1,arg1)					\
type name(type1 arg1)							\
{									\
	__syscall_nr(1, type, name, arg1);				\
}

#define _syscall2(type,name,type1,arg1,type2,arg2)			\
type name(type1 arg1, type2 arg2)					\
{									\
	__syscall_nr(2, type, name, arg1, arg2);			\
}

#define _syscall3(type,name,type1,arg1,type2,arg2,type3,arg3)		\
type name(type1 arg1, type2 arg2, type3 arg3)				\
{									\
	__syscall_nr(3, type, name, arg1, arg2, arg3);			\
}

#define _syscall4(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4) \
type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4)		\
{									\
	__syscall_nr(4, type, name, arg1, arg2, arg3, arg4);		\
}

#define _syscall5(type,name,type1,arg1,type2,arg2,type3,arg3,type4,arg4,type5,arg5) \
type name(type1 arg1, type2 arg2, type3 arg3, type4 arg4, type5 arg5)	\
{									\
	__syscall_nr(5, type, name, arg1, arg2, arg3, arg4, arg5);	\
}

#define __syscall_nr(nr, type, name, args...)				\
	unsigned long __sc_ret, __sc_err;						\
	{								\
		register unsigned long __sc_0  __asm__ ("r0");		\
		register unsigned long __sc_3  __asm__ ("r3");		\
		register unsigned long __sc_4  __asm__ ("r4");		\
		register unsigned long __sc_5  __asm__ ("r5");		\
		register unsigned long __sc_6  __asm__ ("r6");		\
		register unsigned long __sc_7  __asm__ ("r7");		\
									\
		__sc_loadargs_##nr(name, args);				\
		__asm__ __volatile__					\
			("sc           \n\t"				\
			 "mfcr %0      "				\
			: "=&r" (__sc_0),				\
			  "=&r" (__sc_3),  "=&r" (__sc_4),		\
			  "=&r" (__sc_5),  "=&r" (__sc_6),		\
			  "=&r" (__sc_7)				\
			: __sc_asm_input_##nr				\
			: "cr0", "ctr", "memory",			\
			  "r8", "r9", "r10","r11", "r12");		\
		__sc_ret = __sc_3;					\
		__sc_err = __sc_0;					\
	}								\
	if (__sc_err & 0x10000000)					\
	{								\
		errno = __sc_ret;					\
		__sc_ret = -1;						\
	}								\
	return (type) __sc_ret

#endif
