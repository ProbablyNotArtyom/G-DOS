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

#define syscall_getchar		29
#define syscall_putchar		30

#ifndef __ASSEMBLER__
#include <errno.h>
#endif

#undef SYS_ify
#define SYS_ify(syscall_name)        syscall_##syscall_name

#ifndef __ASSEMBLER__

#define LOAD_ARGS_0()
#define ASM_ARGS_0
#define LOAD_ARGS_1(a1)                                \
  int _a1tmp = (int) (a1);                        \
  LOAD_ARGS_0 ()                                \
  __a1 = _a1tmp;
#define ASM_ARGS_1        ASM_ARGS_0, "r" (__a1)
#define LOAD_ARGS_2(a1, a2)                        \
  int _a2tmp = (int) (a2);                        \
  LOAD_ARGS_1 (a1)                                \
  register int _a2 asm ("a2") = _a2tmp;
#define ASM_ARGS_2        ASM_ARGS_1, "r" (_a2)
#define LOAD_ARGS_3(a1, a2, a3)                        \
  int _a3tmp = (int) (a3);                        \
  LOAD_ARGS_2 (a1, a2)                                \
  register int _a3 asm ("a3") = _a3tmp;
#define ASM_ARGS_3        ASM_ARGS_2, "r" (_a3)
#define LOAD_ARGS_4(a1, a2, a3, a4)                \
  int _a4tmp = (int) (a4);                        \
  LOAD_ARGS_3 (a1, a2, a3)                        \
  register int _a4 asm ("a4") = _a4tmp;
#define ASM_ARGS_4        ASM_ARGS_3, "r" (_a4)
#define LOAD_ARGS_5(a1, a2, a3, a4, a5)                \
  int _v1tmp = (int) (a5);                        \
  LOAD_ARGS_4 (a1, a2, a3, a4)                        \
  register int _v1 asm ("v1") = _v1tmp;
#define ASM_ARGS_5        ASM_ARGS_4, "r" (_v1)
#define LOAD_ARGS_6(a1, a2, a3, a4, a5, a6)        \
  int _v2tmp = (int) (a6);                        \
  LOAD_ARGS_5 (a1, a2, a3, a4, a5)                \
  register int _v2 asm ("v2") = _v2tmp;
#define ASM_ARGS_6        ASM_ARGS_5, "r" (_v2)
#ifndef __thumb__
# define LOAD_ARGS_7(a1, a2, a3, a4, a5, a6, a7)        \
  int _v3tmp = (int) (a7);                                \
  LOAD_ARGS_6 (a1, a2, a3, a4, a5, a6)                        \
  register int _v3 asm ("v3") = _v3tmp;
# define ASM_ARGS_7        ASM_ARGS_6, "r" (_v3)
#endif
/* Define a macro which expands into the inline wrapper code for a system
   call.  */
#undef INLINE_SYSCALL
#define INLINE_SYSCALL(name, nr, args...)					\
  ({ unsigned int _inline_sys_result = INTERNAL_SYSCALL (name, , nr, args);	\
     if (__builtin_expect (INTERNAL_SYSCALL_ERROR_P (_inline_sys_result, ), 0))	\
       {									\
	 __set_errno (INTERNAL_SYSCALL_ERRNO (_inline_sys_result, ));		\
	 _inline_sys_result = (unsigned int) -1;				\
       }									\
     (int) _inline_sys_result; })

#undef INTERNAL_SYSCALL_DECL
#define INTERNAL_SYSCALL_DECL(err) do { } while (0)

#undef INTERNAL_SYSCALL_RAW
#if defined(__thumb__)
/* Hide the use of r7 from the compiler, this would be a lot
 * easier but for the fact that the syscalls can exceed 255.
 * For the moment the LOAD_ARG_7 is sacrificed.
 * We can't use push/pop inside the asm because that breaks
 * unwinding (ie. thread cancellation).
 */
#define INTERNAL_SYSCALL_RAW(name, err, nr, args...)		\
  ({ unsigned int _internal_sys_result;				\
    {								\
      int _sys_buf[2];						\
      register int __a1 __asm__ ("a1");				\
      register int *_v3 __asm__ ("v3") = _sys_buf;		\
      LOAD_ARGS_##nr (args)					\
      *_v3 = (int) (name);					\
      __asm__ __volatile__ ("str	r7, [v3, #4]\n"		\
                    "\tldr      r7, [v3]\n"			\
                    "\tswi      0       @ syscall " #name "\n"	\
                    "\tldr      r7, [v3, #4]"			\
                   : "=r" (__a1)				\
                    : "r" (_v3) ASM_ARGS_##nr			\
                    : "memory");				\
      _internal_sys_result = __a1;				\
    }								\
    (int) _internal_sys_result; })
#elif defined(__ARM_EABI__)
#define INTERNAL_SYSCALL_RAW(name, err, nr, args...)		\
  ({unsigned int _internal_sys_result;				\
     {								\
       register int __a1 __asm__ ("r0"), _nr __asm__ ("r7");	\
       LOAD_ARGS_##nr (args)					\
       _nr = name;						\
       __asm__ __volatile__ ("swi	0x0 @ syscall " #name	\
		     : "=r" (__a1)				\
		     : "r" (_nr) ASM_ARGS_##nr			\
		     : "memory");				\
       _internal_sys_result = __a1;				\
     }								\
     (int) _internal_sys_result; })
#else /* !defined(__ARM_EABI__) */
#define INTERNAL_SYSCALL_RAW(name, err, nr, args...)		\
  ({ unsigned int _internal_sys_result;				\
     {								\
       register int __a1 __asm__ ("a1");			\
       LOAD_ARGS_##nr (args)					\
       __asm__ __volatile__ ("swi	%1 @ syscall " #name	\
		     : "=r" (__a1)				\
		     : "i" (name) ASM_ARGS_##nr			\
		     : "memory");				\
       _internal_sys_result = __a1;				\
     }								\
     (int) _internal_sys_result; })
#endif

#undef INTERNAL_SYSCALL
#define INTERNAL_SYSCALL(name, err, nr, args...)		\
	INTERNAL_SYSCALL_RAW(SYS_ify(name), err, nr, args)

#undef INTERNAL_SYSCALL_ARM
#define INTERNAL_SYSCALL_ARM(name, err, nr, args...)		\
	INTERNAL_SYSCALL_RAW(__ARM_NR_##name, err, nr, args)

#undef INTERNAL_SYSCALL_ERROR_P
#define INTERNAL_SYSCALL_ERROR_P(val, err) \
  ((unsigned int) (val) >= 0xfffff001u)

#undef INTERNAL_SYSCALL_ERRNO
#define INTERNAL_SYSCALL_ERRNO(val, err)	(-(val))

#if defined(__ARM_EABI__)
#undef INTERNAL_SYSCALL_NCS
#define INTERNAL_SYSCALL_NCS(number, err, nr, args...)		\
	INTERNAL_SYSCALL_RAW(number, err, nr, args)
#else
/* We can't implement non-constant syscalls directly since the syscall
   number is normally encoded in the instruction.  So use SYS_syscall.  */
#undef INTERNAL_SYSCALL_NCS
#define INTERNAL_SYSCALL_NCS(number, err, nr, args...)		\
	INTERNAL_SYSCALL_NCS_##nr (number, err, args)

#define INTERNAL_SYSCALL_NCS_0(number, err, args...)		\
	INTERNAL_SYSCALL (syscall, err, 1, number, args)
#define INTERNAL_SYSCALL_NCS_1(number, err, args...)		\
	INTERNAL_SYSCALL (syscall, err, 2, number, args)
#define INTERNAL_SYSCALL_NCS_2(number, err, args...)		\
	INTERNAL_SYSCALL (syscall, err, 3, number, args)
#define INTERNAL_SYSCALL_NCS_3(number, err, args...)		\
	INTERNAL_SYSCALL (syscall, err, 4, number, args)
#define INTERNAL_SYSCALL_NCS_4(number, err, args...)		\
	INTERNAL_SYSCALL (syscall, err, 5, number, args)
#define INTERNAL_SYSCALL_NCS_5(number, err, args...)		\
	INTERNAL_SYSCALL (syscall, err, 6, number, args)
#endif

#endif


/* The ARM EABI user interface passes the syscall number in r7, instead
   of in the swi.  This is more efficient, because the kernel does not need
   to fetch the swi from memory to find out the number; which can be painful
   with separate I-cache and D-cache.  Make sure to use 0 for the SWI
   argument; otherwise the (optional) compatibility code for APCS binaries
   may be invoked.  */
/* Linux takes system call args in registers:
        arg 1                r0
        arg 2                r1
        arg 3                r2
        arg 4                r3
        arg 5                r4        (this is different from the APCS convention)
        arg 6                r5
        arg 7                r6
   The compiler is going to form a call by coming here, through PSEUDO, with
   arguments
        syscall number        in the DO_CALL macro
        arg 1                r0
        arg 2                r1
        arg 3                r2
        arg 4                r3
        arg 5                [sp]
        arg 6                [sp+4]
        arg 7                [sp+8]
   We need to shuffle values between R4..R6 and the stack so that the
   caller's v1..v3 and stack frame are not corrupted, and the kernel
   sees the right arguments.
*/

/* We must save and restore r7 (call-saved) for the syscall number.
   We never make function calls from inside here (only potentially
   signal handlers), so we do not bother with doubleword alignment.
   Just like the APCS syscall convention, the EABI syscall convention uses
   r0 through r6 for up to seven syscall arguments.  None are ever passed to
   the kernel on the stack, although incoming arguments are on the stack for
   syscalls with five or more arguments.
   The assembler will convert the literal pool load to a move for most
   syscalls.  */
#undef        DO_CALL
#define DO_CALL(syscall_name, args)                        \
        DOARGS_##args;                                        \
        ldr        r7, =SYS_ify (syscall_name);                \
        swi        0x0;                                        \
        UNDOARGS_##args
#undef  DOARGS_0
#define DOARGS_0                                        \
        .fnstart;                                        \
        push        { r7 };                                        \
        cfi_adjust_cfa_offset (4);                        \
        cfi_rel_offset (r7, 0);                                \
        .save        { r7 }
#undef  DOARGS_1
#define DOARGS_1 DOARGS_0
#undef  DOARGS_2
#define DOARGS_2 DOARGS_0
#undef  DOARGS_3
#define DOARGS_3 DOARGS_0
#undef  DOARGS_4
#define DOARGS_4 DOARGS_0
#undef  DOARGS_5
#define DOARGS_5                                        \
        .fnstart;                                        \
        push        {r4, r7};                                \
        cfi_adjust_cfa_offset (8);                        \
        cfi_rel_offset (r4, 0);                                \
        cfi_rel_offset (r7, 4);                                \
        .save        { r4, r7 };                                \
        ldr        r4, [sp, #8]
#undef  DOARGS_6
#define DOARGS_6                                        \
        .fnstart;                                        \
        mov        ip, sp;                                        \
        push        {r4, r5, r7};                                \
        cfi_adjust_cfa_offset (12);                        \
        cfi_rel_offset (r4, 0);                                \
        cfi_rel_offset (r5, 4);                                \
        cfi_rel_offset (r7, 8);                                \
        .save        { r4, r5, r7 };                                \
        ldmia        ip, {r4, r5}
#undef  DOARGS_7
#define DOARGS_7                                        \
        .fnstart;                                        \
        mov        ip, sp;                                        \
        push        {r4, r5, r6, r7};                        \
        cfi_adjust_cfa_offset (16);                        \
        cfi_rel_offset (r4, 0);                                \
        cfi_rel_offset (r5, 4);                                \
        cfi_rel_offset (r6, 8);                                \
        cfi_rel_offset (r7, 12);                        \
        .save        { r4, r5, r6, r7 };                        \
        ldmia        ip, {r4, r5, r6}
#undef  UNDOARGS_0
#define UNDOARGS_0                                        \
        pop        {r7};                                        \
        cfi_adjust_cfa_offset (-4);                        \
        cfi_restore (r7);                                \
        .fnend
#undef  UNDOARGS_1
#define UNDOARGS_1 UNDOARGS_0
#undef  UNDOARGS_2
#define UNDOARGS_2 UNDOARGS_0
#undef  UNDOARGS_3
#define UNDOARGS_3 UNDOARGS_0
#undef  UNDOARGS_4
#define UNDOARGS_4 UNDOARGS_0
#undef  UNDOARGS_5
#define UNDOARGS_5                                        \
        pop        {r4, r7};                                \
        cfi_adjust_cfa_offset (-8);                        \
        cfi_restore (r4);                                \
        cfi_restore (r7);                                \
        .fnend
#undef  UNDOARGS_6
#define UNDOARGS_6                                        \
        pop        {r4, r5, r7};                                \
        cfi_adjust_cfa_offset (-12);                        \
        cfi_restore (r4);                                \
        cfi_restore (r5);                                \
        cfi_restore (r7);                                \
        .fnend
#undef  UNDOARGS_7
#define UNDOARGS_7                                        \
        pop        {r4, r5, r6, r7};                        \
        cfi_adjust_cfa_offset (-16);                        \
        cfi_restore (r4);                                \
        cfi_restore (r5);                                \
        cfi_restore (r6);                                \
        cfi_restore (r7);                                \
        .fnend

#endif
