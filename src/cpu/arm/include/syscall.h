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

#define INTERNAL_SYSCALL_NCS(name, nr, args...)	\
  ({ uint32_t _sys_result;					\
     {										\
       LOAD_ARGS_##nr (args)				\
       LOAD_REGS_##nr						\
       register uint32_t _d0 asm ("%r7") = name;	\
       asm volatile ("svc		0x0"				\
		     : "=r" (_d0)					\
		     : "0" (_d0) ASM_ARGS_##nr		\
		     : "memory");					\
       _sys_result = _d0;					\
     }										\
     (uint32_t) _sys_result; })

#define INTERNAL_SYSCALL(name, nr, args...)	\
	INTERNAL_SYSCALL_NCS(name, nr, ##args)

#define LOAD_ARGS_0()
#define LOAD_REGS_0
#define ASM_ARGS_0
#define LOAD_ARGS_1(a1)				\
  LOAD_ARGS_0 ()				\
  uint32_t __arg1 = (int) (a1);
#define LOAD_REGS_1				\
  register uint32_t _d1 asm ("r0") = __arg1;		\
  LOAD_REGS_0
#define ASM_ARGS_1	ASM_ARGS_0, "r" (_d1)
#define LOAD_ARGS_2(a1, a2)			\
  LOAD_ARGS_1 (a1)				\
  uint32_t __arg2 = (int) (a2);
#define LOAD_REGS_2				\
  register uint32_t _d2 asm ("r1") = __arg2;		\
  LOAD_REGS_1
#define ASM_ARGS_2	ASM_ARGS_1, "r" (_d2)
#define LOAD_ARGS_3(a1, a2, a3)			\
  LOAD_ARGS_2 (a1, a2)				\
  uint32_t __arg3 = (int) (a3);
#define LOAD_REGS_3				\
  register uint32_t _d3 asm ("r2") = __arg3;		\
  LOAD_REGS_2
#define ASM_ARGS_3	ASM_ARGS_2, "r" (_d3)
#define LOAD_ARGS_4(a1, a2, a3, a4)		\
  LOAD_ARGS_3 (a1, a2, a3)			\
  uint32_t __arg4 = (int) (a4);
#define LOAD_REGS_4				\
  register uint32_t _d4 asm ("r3") = __arg4;		\
  LOAD_REGS_3
#define ASM_ARGS_4	ASM_ARGS_3, "r" (_d4)
#define LOAD_ARGS_5(a1, a2, a3, a4, a5)		\
  LOAD_ARGS_4 (a1, a2, a3, a4)			\
  uint32_t __arg5 = (int) (a5);
#define LOAD_REGS_5				\
  register uint32_t _d5 asm ("r4") = __arg5;		\
  LOAD_REGS_4
#define ASM_ARGS_5	ASM_ARGS_4, "r" (_d5)
#define LOAD_ARGS_6(a1, a2, a3, a4, a5, a6)	\
  LOAD_ARGS_5 (a1, a2, a3, a4, a5)		\
  uint32_t __arg6 = (int) (a6);
#define LOAD_REGS_6				\
  register uint32_t _a0 asm ("r5") = __arg6;		\
  LOAD_REGS_5
#define ASM_ARGS_6	ASM_ARGS_5, "r" (_a0)

#endif

#define	DOARGS_0	/* No arguments to frob.  */
#define	UNDOARGS_0	/* No arguments to unfrob.  */
#define	_DOARGS_0(n)	/* No arguments to frob.  */
#define	DOARGS_1	_DOARGS_1 (4)
#define	_DOARGS_1(n)	move.l n(%sp), %d1; _DOARGS_0 (n)
#define	UNDOARGS_1	UNDOARGS_0
#define	DOARGS_2	_DOARGS_2 (8)
#define	_DOARGS_2(n)	move.l %d2, %a0; move.l n(%sp), %d2; _DOARGS_1 (n-4)
#define	UNDOARGS_2	UNDOARGS_1; move.l %a0, %d2
#define DOARGS_3	_DOARGS_3 (12)
#define _DOARGS_3(n)	move.l %d3, %a1; move.l n(%sp), %d3; _DOARGS_2 (n-4)
#define UNDOARGS_3	UNDOARGS_2; move.l %a1, %d3
#define DOARGS_4	_DOARGS_4 (16)
#define _DOARGS_4(n)	move.l %d4, -(%sp); move.l n+4(%sp), %d4; _DOARGS_3 (n)
#define UNDOARGS_4	UNDOARGS_3; move.l (%sp)+, %d4
#define DOARGS_5	_DOARGS_5 (20)
#define _DOARGS_5(n)	move.l %d5, -(%sp); move.l n+4(%sp), %d5; _DOARGS_4 (n)
#define UNDOARGS_5	UNDOARGS_4; move.l (%sp)+, %d5

#endif
