#ifndef __ASM_LINKAGE_H
#define __ASM_LINKAGE_H

#ifdef CONFIG_PPC64
#if !defined(_CALL_ELF) || _CALL_ELF != 2
#define cond_syscall(x) \
	asm ("\t.weak " #x "\n\t.set " #x ", sys_ni_syscall\n"		\
	     "\t.weak ." #x "\n\t.set ." #x ", .sys_ni_syscall\n")
#define SYSCALL_ALIAS(alias, name)					\
	asm ("\t.globl " #alias "\n\t.set " #alias ", " #name "\n"	\
	     "\t.globl ." #alias "\n\t.set ." #alias ", ." #name)
#endif
#endif

#endif
