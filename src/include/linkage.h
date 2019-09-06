#ifndef _HEADER_LINKAGE
#define _HEADER_LINKAGE

	#include <asmlinkage.h>
	#include <sys/cdefs.h>

#define __VMLINUX_SYMBOL(x) _##x
#define __VMLINUX_SYMBOL_STR(x) "_" #x
#define VMLINUX_SYMBOL(x) __VMLINUX_SYMBOL(x)
#define VMLINUX_SYMBOL_STR(x) __VMLINUX_SYMBOL_STR(x)

#ifndef ASM_NL
#define ASM_NL		 ;
#endif

#ifdef __cplusplus
#define CPP_ASMLINKAGE extern "C"
#else
#define CPP_ASMLINKAGE
#endif

#ifndef asmlinkage
#define asmlinkage CPP_ASMLINKAGE
#endif

#ifndef cond_syscall
#define cond_syscall(x)	asm(					\
	".weak " VMLINUX_SYMBOL_STR(x) "\n\t"		\
	".set  " VMLINUX_SYMBOL_STR(x) ","			\
		 VMLINUX_SYMBOL_STR(sys_ni_syscall))
#endif

#ifndef SYSCALL_ALIAS
#define SYSCALL_ALIAS(alias, name) asm(			\
	".globl " VMLINUX_SYMBOL_STR(alias) "\n\t"	\
	".set   " VMLINUX_SYMBOL_STR(alias) ","		\
		  VMLINUX_SYMBOL_STR(name))
#endif

#ifndef __ASSEMBLER__
#ifndef asmlinkage_protect
# define asmlinkage_protect(n, ret, args...)	do { } while (0)
#endif
#endif

#ifndef __ALIGN
#define __ALIGN		.align 4,0x90
#define __ALIGN_STR	".align 4,0x90"
#endif

#ifdef __ASSEMBLER__

#ifndef LINKER_SCRIPT
#define ALIGN __ALIGN
#define ALIGN_STR __ALIGN_STR

#ifndef ENTRY
#define ENTRY(name) \
	.globl name ASM_NL \
	ALIGN ASM_NL \
	name:
#endif
#endif

#ifndef WEAK
#define WEAK(name)	   \
	.weak name ASM_NL   \
	name:
#endif

#endif


#endif
