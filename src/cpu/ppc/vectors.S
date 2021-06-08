
 	#include <entry.h>
	#include <asm.h>
	#include <linkage.h>
	#include <cpu.h>

#define LOCAL_ASM_PUTS(a)	\
	lis	r3,1f@h;	\
	ori	r3,r3,1f@l;	\
	bl      FUNC_NAME(puts);\
        b	_start;		\
1:     	.ascii  a;		\
	.byte	0x00

//---------------------------------------------------

//#define ILLEGAL_VECTOR(v)	.org __vectors + v ; vector__##v: LOCAL_ASM_PUTS("[!!!] Illegal Exception Fault:\n\r\tVector: ##v")
#define ILLEGAL_VECTOR(v)	.org __vectors + v ; vector__##v: b	vector__##v
#define VECTOR(v, dummystr)	.org __vectors + v ; vector__##v:

//---------------------------------------------------

.extern main
.extern media_start
.extern exit
.extern atexit
.extern __STACK

.section .vectors
.global __vectors
__vectors:

VECTOR(0x100, "SRE")
	b		_start
ILLEGAL_VECTOR(0x200)
ILLEGAL_VECTOR(0x300)
ILLEGAL_VECTOR(0x380)
ILLEGAL_VECTOR(0x400)
ILLEGAL_VECTOR(0x480)
ILLEGAL_VECTOR(0x500)
ILLEGAL_VECTOR(0x600)
VECTOR(0x700, "PROG")
	b	_start
VECTOR(0x800, "FPU")
	mtsprg1	r3
	mfsrr1	r3
	ori		r3,r3,0x2000
	mtsrr1	r3
	mfsprg1	r3
	rfi
ILLEGAL_VECTOR(0x900)
ILLEGAL_VECTOR(0xa00)
VECTOR(0xc00, "SYSCALL")
	b	__syscall_handler
ILLEGAL_VECTOR(0xd00)
ILLEGAL_VECTOR(0xe00)
ILLEGAL_VECTOR(0xf00)
ILLEGAL_VECTOR(0x1000)
ILLEGAL_VECTOR(0x1100)
ILLEGAL_VECTOR(0x1200)
ILLEGAL_VECTOR(0x1300)
ILLEGAL_VECTOR(0x1400)
ILLEGAL_VECTOR(0x1500)
ILLEGAL_VECTOR(0x1600)
ILLEGAL_VECTOR(0x1700)

.section .text
.global __vectors_end
__vectors_end:
.global _start
_start:
	mtspr	8,r0

	addis   r0,0,0x0000		// from reset, the BATs are in an unknown state on most PPCs.
							// Invalidate them all to avoid error states
	

	xor r1, r1, r1
	lis r1, __STACK@h
	addi r1, r1, __STACK@l

	bl __eabi

	#addis   r3,r0,main@h
	#ori     r3,r3,main@l
	#mtspr   0x01A,r3
	#addis   r4,0,0x0000

	#ori     r4,r4,0x3900	// enable the FPU & MC exceptions
	#mtspr   srr1,r4
	#ori     r4,r4,0x0040    // put exception prefix at 0xfff00000

	// set up the time base register
	#addis   r4,r0,0x0000
	#mtspr   285,r4
	#mtspr   284,r4

	#rfi


	/* exit into main */
	b		FUNC_NAME(main)

.global __eabi
__eabi:
	addis   r13,r0,_SDA_BASE_@h
	ori     r13,r13,_SDA_BASE_@l
	addis   r2,r0,_SDA2_BASE_@h
	ori     r2,r2,_SDA2_BASE_@l
	blr

.global __init
__init:
	blr
