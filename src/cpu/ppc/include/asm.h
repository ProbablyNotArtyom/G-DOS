
#ifndef _ASM_POWERPC_PPC_ASM_H
#define _ASM_POWERPC_PPC_ASM_H

#include <stringify.h>
#include <cpu.h>


.macro asm_puts str
.section .rodata
1:	.ascii	str
.section .text
.extern	puts
	lis	r3,1b@ha
	la	r3,1b@l
	bl	puts
	blr
.endm

#ifdef __ASSEMBLY__

/*
 * LOAD_REG_IMMEDIATE(rn, expr)
 *   Loads the value of the constant expression 'expr' into register 'rn'
 *   using immediate instructions only.  Use this when it's important not
 *   to reference other data (i.e. on ppc64 when the TOC pointer is not
 *   valid) and when 'expr' is a constant or absolute address.
 *
 * LOAD_REG_ADDR(rn, name)
 *   Loads the address of label 'name' into register 'rn'.  Use this when
 *   you don't particularly need immediate instructions only, but you need
 *   the whole address in one register (e.g. it's a structure address and
 *   you want to access various offsets within it).  On ppc32 this is
 *   identical to LOAD_REG_IMMEDIATE.
 *
 * LOAD_REG_ADDR_PIC(rn, name)
 *   Loads the address of label 'name' into register 'run'. Use this when
 *   the kernel doesn't run at the linked or relocated address. Please
 *   note that this macro will clobber the lr register.
 *
 * LOAD_REG_ADDRBASE(rn, name)
 * ADDROFF(name)
 *   LOAD_REG_ADDRBASE loads part of the address of label 'name' into
 *   register 'rn'.  ADDROFF(name) returns the remainder of the address as
 *   a constant expression.  ADDROFF(name) is a signed expression < 16 bits
 *   in size, so is suitable for use directly as an offset in load and store
 *   instructions.  Use this when loading/storing a single word or less as:
 *      LOAD_REG_ADDRBASE(rX, name)
 *      ld	rY,ADDROFF(name)(rX)
 */

/* Be careful, this will clobber the lr register. */
#ifndef LOAD_REG_ADDR_PIC
#define LOAD_REG_ADDR_PIC(reg, name)		\
	bl	0f;				\
0:	mflr	reg;				\
	addis	reg,reg,(name - 0b)@ha;		\
	addi	reg,reg,(name - 0b)@l;
#endif

#ifndef LOAD_REG_IMMEDIATE
#define LOAD_REG_IMMEDIATE(reg,expr)		\
	lis	reg,(expr)@ha;		\
	addi	reg,reg,(expr)@l;
#endif

#ifndef LOAD_REG_ADDR
#define LOAD_REG_ADDR(reg,name)		LOAD_REG_IMMEDIATE(reg, name)
#endif

#ifndef LOAD_REG_ADDRBASE
#define LOAD_REG_ADDRBASE(reg, name)	lis	reg,name@ha
#endif

#ifndef LOAD_REG_FUNC
#define LOAD_REG_FUNC(D, x) \
    LOAD_REG_IMMEDIATE((D), (x))
#endif

#ifndef ADDROFF
#define ADDROFF(name)			name@l
#endif

#define SZL			(BITS_PER_LONG/8)

/*
 * Macros for storing registers into and loading registers from
 * exception frames.
 */

#define SAVE_GPR(n, base)	stw	n,GPR0+4*(n)(base)
#define REST_GPR(n, base)	lwz	n,GPR0+4*(n)(base)
#define SAVE_NVGPRS(base)	SAVE_GPR(13, base); SAVE_8GPRS(14, base); \
				SAVE_10GPRS(22, base)
#define REST_NVGPRS(base)	REST_GPR(13, base); REST_8GPRS(14, base); \
				REST_10GPRS(22, base)

#define SAVE_2GPRS(n, base)	SAVE_GPR(n, base); SAVE_GPR(n+1, base)
#define SAVE_4GPRS(n, base)	SAVE_2GPRS(n, base); SAVE_2GPRS(n+2, base)
#define SAVE_8GPRS(n, base)	SAVE_4GPRS(n, base); SAVE_4GPRS(n+4, base)
#define SAVE_10GPRS(n, base)	SAVE_8GPRS(n, base); SAVE_2GPRS(n+8, base)
#define REST_2GPRS(n, base)	REST_GPR(n, base); REST_GPR(n+1, base)
#define REST_4GPRS(n, base)	REST_2GPRS(n, base); REST_2GPRS(n+2, base)
#define REST_8GPRS(n, base)	REST_4GPRS(n, base); REST_4GPRS(n+4, base)
#define REST_10GPRS(n, base)	REST_8GPRS(n, base); REST_2GPRS(n+8, base)

#define SAVE_FPR(n, base)	stfd	n,8*TS_FPRWIDTH*(n)(base)
#define SAVE_2FPRS(n, base)	SAVE_FPR(n, base); SAVE_FPR(n+1, base)
#define SAVE_4FPRS(n, base)	SAVE_2FPRS(n, base); SAVE_2FPRS(n+2, base)
#define SAVE_8FPRS(n, base)	SAVE_4FPRS(n, base); SAVE_4FPRS(n+4, base)
#define SAVE_16FPRS(n, base)	SAVE_8FPRS(n, base); SAVE_8FPRS(n+8, base)
#define SAVE_32FPRS(n, base)	SAVE_16FPRS(n, base); SAVE_16FPRS(n+16, base)
#define REST_FPR(n, base)	lfd	n,8*TS_FPRWIDTH*(n)(base)
#define REST_2FPRS(n, base)	REST_FPR(n, base); REST_FPR(n+1, base)
#define REST_4FPRS(n, base)	REST_2FPRS(n, base); REST_2FPRS(n+2, base)
#define REST_8FPRS(n, base)	REST_4FPRS(n, base); REST_4FPRS(n+4, base)
#define REST_16FPRS(n, base)	REST_8FPRS(n, base); REST_8FPRS(n+8, base)
#define REST_32FPRS(n, base)	REST_16FPRS(n, base); REST_16FPRS(n+16, base)

#define SAVE_VR(n,b,base)	li b,16*(n);  stvx n,base,b
#define SAVE_2VRS(n,b,base)	SAVE_VR(n,b,base); SAVE_VR(n+1,b,base)
#define SAVE_4VRS(n,b,base)	SAVE_2VRS(n,b,base); SAVE_2VRS(n+2,b,base)
#define SAVE_8VRS(n,b,base)	SAVE_4VRS(n,b,base); SAVE_4VRS(n+4,b,base)
#define SAVE_16VRS(n,b,base)	SAVE_8VRS(n,b,base); SAVE_8VRS(n+8,b,base)
#define SAVE_32VRS(n,b,base)	SAVE_16VRS(n,b,base); SAVE_16VRS(n+16,b,base)
#define REST_VR(n,b,base)	li b,16*(n); lvx n,base,b
#define REST_2VRS(n,b,base)	REST_VR(n,b,base); REST_VR(n+1,b,base)
#define REST_4VRS(n,b,base)	REST_2VRS(n,b,base); REST_2VRS(n+2,b,base)
#define REST_8VRS(n,b,base)	REST_4VRS(n,b,base); REST_4VRS(n+4,b,base)
#define REST_16VRS(n,b,base)	REST_8VRS(n,b,base); REST_8VRS(n+8,b,base)
#define REST_32VRS(n,b,base)	REST_16VRS(n,b,base); REST_16VRS(n+16,b,base)

#ifdef __BIG_ENDIAN__
#define STXVD2X_ROT(n,b,base)		STXVD2X(n,b,base)
#define LXVD2X_ROT(n,b,base)		LXVD2X(n,b,base)
#else
#define STXVD2X_ROT(n,b,base)		XXSWAPD(n,n);		\
					STXVD2X(n,b,base);	\
					XXSWAPD(n,n)

#define LXVD2X_ROT(n,b,base)		LXVD2X(n,b,base);	\
					XXSWAPD(n,n)
#endif
/* Save the lower 32 VSRs in the thread VSR region */
#define SAVE_VSR(n,b,base)	li b,16*(n);  STXVD2X_ROT(n,R##base,R##b)
#define SAVE_2VSRS(n,b,base)	SAVE_VSR(n,b,base); SAVE_VSR(n+1,b,base)
#define SAVE_4VSRS(n,b,base)	SAVE_2VSRS(n,b,base); SAVE_2VSRS(n+2,b,base)
#define SAVE_8VSRS(n,b,base)	SAVE_4VSRS(n,b,base); SAVE_4VSRS(n+4,b,base)
#define SAVE_16VSRS(n,b,base)	SAVE_8VSRS(n,b,base); SAVE_8VSRS(n+8,b,base)
#define SAVE_32VSRS(n,b,base)	SAVE_16VSRS(n,b,base); SAVE_16VSRS(n+16,b,base)
#define REST_VSR(n,b,base)	li b,16*(n); LXVD2X_ROT(n,R##base,R##b)
#define REST_2VSRS(n,b,base)	REST_VSR(n,b,base); REST_VSR(n+1,b,base)
#define REST_4VSRS(n,b,base)	REST_2VSRS(n,b,base); REST_2VSRS(n+2,b,base)
#define REST_8VSRS(n,b,base)	REST_4VSRS(n,b,base); REST_4VSRS(n+4,b,base)
#define REST_16VSRS(n,b,base)	REST_8VSRS(n,b,base); REST_8VSRS(n+8,b,base)
#define REST_32VSRS(n,b,base)	REST_16VSRS(n,b,base); REST_16VSRS(n+16,b,base)

/*
 * b = base register for addressing, o = base offset from register of 1st EVR
 * n = first EVR, s = scratch
 */
#define SAVE_EVR(n,s,b,o)	evmergehi s,s,n; stw s,o+4*(n)(b)
#define SAVE_2EVRS(n,s,b,o)	SAVE_EVR(n,s,b,o); SAVE_EVR(n+1,s,b,o)
#define SAVE_4EVRS(n,s,b,o)	SAVE_2EVRS(n,s,b,o); SAVE_2EVRS(n+2,s,b,o)
#define SAVE_8EVRS(n,s,b,o)	SAVE_4EVRS(n,s,b,o); SAVE_4EVRS(n+4,s,b,o)
#define SAVE_16EVRS(n,s,b,o)	SAVE_8EVRS(n,s,b,o); SAVE_8EVRS(n+8,s,b,o)
#define SAVE_32EVRS(n,s,b,o)	SAVE_16EVRS(n,s,b,o); SAVE_16EVRS(n+16,s,b,o)
#define REST_EVR(n,s,b,o)	lwz s,o+4*(n)(b); evmergelo n,s,n
#define REST_2EVRS(n,s,b,o)	REST_EVR(n,s,b,o); REST_EVR(n+1,s,b,o)
#define REST_4EVRS(n,s,b,o)	REST_2EVRS(n,s,b,o); REST_2EVRS(n+2,s,b,o)
#define REST_8EVRS(n,s,b,o)	REST_4EVRS(n,s,b,o); REST_4EVRS(n+4,s,b,o)
#define REST_16EVRS(n,s,b,o)	REST_8EVRS(n,s,b,o); REST_8EVRS(n+8,s,b,o)
#define REST_32EVRS(n,s,b,o)	REST_16EVRS(n,s,b,o); REST_16EVRS(n+16,s,b,o)

/* Macros to adjust thread priority for hardware multithreading */
#define HMT_VERY_LOW	or	31,31,31	# very low priority
#define HMT_LOW		or	1,1,1
#define HMT_MEDIUM_LOW  or	6,6,6		# medium low priority
#define HMT_MEDIUM	or	2,2,2
#define HMT_MEDIUM_HIGH or	5,5,5		# medium high priority
#define HMT_HIGH	or	3,3,3
#define HMT_EXTRA_HIGH	or	7,7,7		# power7 only


#define ULONG_SIZE	4
#define __VCPU_GPR(n)	(VCPU_GPRS + (n * ULONG_SIZE))
#define VCPU_GPR(n)	__VCPU_GPR(__REG_##n)


#define _ENTRY(n)	\
	.globl n;	\
n:

#define _GLOBAL(n)	\
	.text;		\
	.stabs __stringify(n:F-1),N_FUN,0,0,n;\
	.globl n;	\
n:

#define _GLOBAL_TOC(name) _GLOBAL(name)

#define _KPROBE(n)	\
	.section ".kprobes.text","a";	\
	.globl	n;	\
n:

/* offsets for stack frame layout */
#define LRSAVE	4


/* various errata or part fixups */
#ifdef CONFIG_PPC601_SYNC_FIX
#define SYNC				\
BEGIN_FTR_SECTION			\
	sync;				\
	isync;				\
END_FTR_SECTION_IFSET(CPU_FTR_601)
#define SYNC_601			\
BEGIN_FTR_SECTION			\
	sync;				\
END_FTR_SECTION_IFSET(CPU_FTR_601)
#define ISYNC_601			\
BEGIN_FTR_SECTION			\
	isync;				\
END_FTR_SECTION_IFSET(CPU_FTR_601)
#else
#define	SYNC
#define SYNC_601
#define ISYNC_601
#endif

#define MFTB(dest)			mfspr dest, SPRN_TBRL

#ifndef CONFIG_SMP
#define TLBSYNC
#else /* CONFIG_SMP */
/* tlbsync is not implemented on 601 */
#define TLBSYNC				\
BEGIN_FTR_SECTION			\
	tlbsync;			\
	sync;				\
END_FTR_SECTION_IFCLR(CPU_FTR_601)
#endif

#define PPC440EP_ERR42

/* The following stops all load and store data streams associated with stream
 * ID (ie. streams created explicitly).  The embedded and server mnemonics for
 * dcbt are different so we use machine "power4" here explicitly.
 */
#define DCBT_STOP_ALL_STREAM_IDS(scratch)	\
.machine push ;					\
.machine "power4" ;				\
       lis     scratch,0x60000000@h;		\
       dcbt    r0,scratch,0b01010;		\
.machine pop

/*
 * On APUS (Amiga PowerPC cpu upgrade board), we don't know the
 * physical base address of RAM at compile time.
 */
#define toreal(rd)	tophys(rd,rd)
#define fromreal(rd)	tovirt(rd,rd)

#define tophys(rd,rs)				\
0:	addis	rd,rs,-PAGE_OFFSET@h;		\
	.section ".vtop_fixup","aw";		\
	.align  1;				\
	.long   0b;				\
	.previous

#define tovirt(rd,rs)				\
0:	addis	rd,rs,PAGE_OFFSET@h;		\
	.section ".ptov_fixup","aw";		\
	.align  1;				\
	.long   0b;				\
	.previous


/* The boring bits... */

/* Condition Register Bit Fields */

#define	cr0	0
#define	cr1	1
#define	cr2	2
#define	cr3	3
#define	cr4	4
#define	cr5	5
#define	cr6	6
#define	cr7	7


/*
 * General Purpose Registers (GPRs)
 *
 * The lower case r0-r31 should be used in preference to the upper
 * case R0-R31 as they provide more error checking in the assembler.
 * Use R0-31 only when really nessesary.
 */

#define	r0	%r0
#define	r1	%r1
#define	r2	%r2
#define	r3	%r3
#define	r4	%r4
#define	r5	%r5
#define	r6	%r6
#define	r7	%r7
#define	r8	%r8
#define	r9	%r9
#define	r10	%r10
#define	r11	%r11
#define	r12	%r12
#define	r13	%r13
#define	r14	%r14
#define	r15	%r15
#define	r16	%r16
#define	r17	%r17
#define	r18	%r18
#define	r19	%r19
#define	r20	%r20
#define	r21	%r21
#define	r22	%r22
#define	r23	%r23
#define	r24	%r24
#define	r25	%r25
#define	r26	%r26
#define	r27	%r27
#define	r28	%r28
#define	r29	%r29
#define	r30	%r30
#define	r31	%r31


/* Floating Point Registers (FPRs) */

#define	fr0	0
#define	fr1	1
#define	fr2	2
#define	fr3	3
#define	fr4	4
#define	fr5	5
#define	fr6	6
#define	fr7	7
#define	fr8	8
#define	fr9	9
#define	fr10	10
#define	fr11	11
#define	fr12	12
#define	fr13	13
#define	fr14	14
#define	fr15	15
#define	fr16	16
#define	fr17	17
#define	fr18	18
#define	fr19	19
#define	fr20	20
#define	fr21	21
#define	fr22	22
#define	fr23	23
#define	fr24	24
#define	fr25	25
#define	fr26	26
#define	fr27	27
#define	fr28	28
#define	fr29	29
#define	fr30	30
#define	fr31	31

/* AltiVec Registers (VPRs) */

#define	v0	0
#define	v1	1
#define	v2	2
#define	v3	3
#define	v4	4
#define	v5	5
#define	v6	6
#define	v7	7
#define	v8	8
#define	v9	9
#define	v10	10
#define	v11	11
#define	v12	12
#define	v13	13
#define	v14	14
#define	v15	15
#define	v16	16
#define	v17	17
#define	v18	18
#define	v19	19
#define	v20	20
#define	v21	21
#define	v22	22
#define	v23	23
#define	v24	24
#define	v25	25
#define	v26	26
#define	v27	27
#define	v28	28
#define	v29	29
#define	v30	30
#define	v31	31

/* VSX Registers (VSRs) */

#define	vs0	0
#define	vs1	1
#define	vs2	2
#define	vs3	3
#define	vs4	4
#define	vs5	5
#define	vs6	6
#define	vs7	7
#define	vs8	8
#define	vs9	9
#define	vs10	10
#define	vs11	11
#define	vs12	12
#define	vs13	13
#define	vs14	14
#define	vs15	15
#define	vs16	16
#define	vs17	17
#define	vs18	18
#define	vs19	19
#define	vs20	20
#define	vs21	21
#define	vs22	22
#define	vs23	23
#define	vs24	24
#define	vs25	25
#define	vs26	26
#define	vs27	27
#define	vs28	28
#define	vs29	29
#define	vs30	30
#define	vs31	31
#define	vs32	32
#define	vs33	33
#define	vs34	34
#define	vs35	35
#define	vs36	36
#define	vs37	37
#define	vs38	38
#define	vs39	39
#define	vs40	40
#define	vs41	41
#define	vs42	42
#define	vs43	43
#define	vs44	44
#define	vs45	45
#define	vs46	46
#define	vs47	47
#define	vs48	48
#define	vs49	49
#define	vs50	50
#define	vs51	51
#define	vs52	52
#define	vs53	53
#define	vs54	54
#define	vs55	55
#define	vs56	56
#define	vs57	57
#define	vs58	58
#define	vs59	59
#define	vs60	60
#define	vs61	61
#define	vs62	62
#define	vs63	63

/* SPE Registers (EVPRs) */

#define	evr0	0
#define	evr1	1
#define	evr2	2
#define	evr3	3
#define	evr4	4
#define	evr5	5
#define	evr6	6
#define	evr7	7
#define	evr8	8
#define	evr9	9
#define	evr10	10
#define	evr11	11
#define	evr12	12
#define	evr13	13
#define	evr14	14
#define	evr15	15
#define	evr16	16
#define	evr17	17
#define	evr18	18
#define	evr19	19
#define	evr20	20
#define	evr21	21
#define	evr22	22
#define	evr23	23
#define	evr24	24
#define	evr25	25
#define	evr26	26
#define	evr27	27
#define	evr28	28
#define	evr29	29
#define	evr30	30
#define	evr31	31

/* some stab codes */
#define N_FUN	36
#define N_RSYM	64
#define N_SLINE	68
#define N_SO	100

/*
 * Create an endian fixup trampoline
 *
 * This starts with a "tdi 0,0,0x48" instruction which is
 * essentially a "trap never", and thus akin to a nop.
 *
 * The opcode for this instruction read with the wrong endian
 * however results in a b . + 8
 *
 * So essentially we use that trick to execute the following
 * trampoline in "reverse endian" if we are running with the
 * MSR_LE bit set the "wrong" way for whatever endianness the
 * kernel is built for.
 */

#define FIXUP_ENDIAN						   \
	tdi   0,0,0x48;	  /* Reverse endian of b . + 8		*/ \
	b     $+36;	  /* Skip trampoline if endian is good	*/ \
	.long 0x05009f42; /* bcl 20,31,$+4			*/ \
	.long 0xa602487d; /* mflr r10				*/ \
	.long 0x1c004a39; /* addi r10,r10,28			*/ \
	.long 0xa600607d; /* mfmsr r11				*/ \
	.long 0x01006b69; /* xori r11,r11,1			*/ \
	.long 0xa6035a7d; /* mtsrr0 r10				*/ \
	.long 0xa6037b7d; /* mtsrr1 r11				*/ \
	.long 0x2400004c  /* rfid				*/

#endif /*  __ASSEMBLY__ */

/*
 * Macros to glue together two tokens.
 */

#ifdef __STDC__
#define XGLUE(a,b) a##b
#else
#define XGLUE(a,b) a/**/b
#endif

#define GLUE(a,b) XGLUE(a,b)

/*
 * Macros to begin and end a function written in assembler.  If -mcall-aixdesc
 * or -mcall-nt, create a function descriptor with the given name, and create
 * the real function with one or two leading periods respectively.
 */

#define FUNC_NAME(name) GLUE(__USER_LABEL_PREFIX__,name)
#if defined __PIC__ || defined __pic__
#define JUMP_TARGET(name) FUNC_NAME(name@plt)
#else
#define JUMP_TARGET(name) FUNC_NAME(name)
#endif
#define FUNC_START(name) \
	.type FUNC_NAME(name),@function; \
	.globl FUNC_NAME(name); \
FUNC_NAME(name):

#define HIDDEN_FUNC(name) \
  FUNC_START(name) \
  .hidden FUNC_NAME(name);

#define FUNC_END(name) \
GLUE(.L,name): \
	.size FUNC_NAME(name),GLUE(.L,name)-FUNC_NAME(name)

#endif
