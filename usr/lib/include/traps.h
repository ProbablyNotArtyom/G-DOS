/*
 *  linux/include/asm/traps.h
 *
 *  Copyright (C) 1993        Hamish Macdonald
 *
 * This file is subject to the terms and conditions of the GNU General Public
 * License.  See the file COPYING in the main directory of this archive
 * for more details.
 */

#ifndef _M68K_TRAPS_H
#define _M68K_TRAPS_H

#ifndef __ASSEMBLER__
#include <linkage.h>
#include <cpu.h>
#endif

#define VEC_RESETSP (0)
#define VEC_RESETPC (1)
#define VEC_BUSERR  (2)
#define VEC_ADDRERR (3)
#define VEC_ILLEGAL (4)
#define VEC_ZERODIV (5)
#define VEC_CHK     (6)
#define VEC_TRAP    (7)
#define VEC_PRIV    (8)
#define VEC_TRACE   (9)
#define VEC_LINE10  (10)
#define VEC_LINE11  (11)
#define VEC_RESV12  (12)
#define VEC_COPROC  (13)
#define VEC_FORMAT  (14)
#define VEC_UNINT   (15)
#define VEC_RESV16  (16)
#define VEC_RESV17  (17)
#define VEC_RESV18  (18)
#define VEC_RESV19  (19)
#define VEC_RESV20  (20)
#define VEC_RESV21  (21)
#define VEC_RESV22  (22)
#define VEC_RESV23  (23)
#define VEC_SPUR    (24)
#define VEC_INT1    (25)
#define VEC_INT2    (26)
#define VEC_INT3    (27)
#define VEC_INT4    (28)
#define VEC_INT5    (29)
#define VEC_INT6    (30)
#define VEC_INT7    (31)
#define VEC_SYS     (32)
#define VEC_TRAP1   (33)
#define VEC_TRAP2   (34)
#define VEC_TRAP3   (35)
#define VEC_TRAP4   (36)
#define VEC_TRAP5   (37)
#define VEC_TRAP6   (38)
#define VEC_TRAP7   (39)
#define VEC_TRAP8   (40)
#define VEC_TRAP9   (41)
#define VEC_TRAP10  (42)
#define VEC_TRAP11  (43)
#define VEC_TRAP12  (44)
#define VEC_TRAP13  (45)
#define VEC_TRAP14  (46)
#define VEC_TRAP15  (47)
#define VEC_FPBRUC  (48)
#define VEC_FPIR    (49)
#define VEC_FPDIVZ  (50)
#define VEC_FPUNDER (51)
#define VEC_FPOE    (52)
#define VEC_FPOVER  (53)
#define VEC_FPNAN   (54)
#define VEC_FPUNSUP (55)
#define VEC_MMUCFG  (56)
#define VEC_MMUILL  (57)
#define VEC_MMUACC  (58)
#define VEC_RESV59  (59)
#define	VEC_UNIMPEA (60)
#define	VEC_UNIMPII (61)
#define VEC_RESV62  (62)
#define VEC_RESV63  (63)
#define VEC_USER    (64)

#define VECOFF(vec) ((vec)<<2)

#ifndef __ASSEMBLER__

/* Status register bits */
#define PS_T  (0x8000)
#define PS_S  (0x2000)
#define PS_M  (0x1000)
#define PS_C  (0x0001)

/* bits for 68020/68030 special status word */

#define FC    (0x8000)
#define FB    (0x4000)
#define RC    (0x2000)
#define RB    (0x1000)
#define DF    (0x0100)
#define RM    (0x0080)
#define RW    (0x0040)
#define SZ    (0x0030)
#define DFC   (0x0007)

/* bits for 68030 MMU status register (mmusr,psr) */

#define MMU_B	     (0x8000)    /* bus error */
#define MMU_L	     (0x4000)    /* limit violation */
#define MMU_S	     (0x2000)    /* supervisor violation */
#define MMU_WP	     (0x0800)    /* write-protected */
#define MMU_I	     (0x0400)    /* invalid descriptor */
#define MMU_M	     (0x0200)    /* ATC entry modified */
#define MMU_T	     (0x0040)    /* transparent translation */
#define MMU_NUM      (0x0007)    /* number of levels traversed */


/* bus access size codes */
#define BA_SIZE_BYTE    (0x20)
#define BA_SIZE_WORD    (0x40)
#define BA_SIZE_LONG    (0x00)
#define BA_SIZE_LINE    (0x60)

/* bus access transfer type codes */
#define BA_TT_MOVE16    (0x08)

/* structure for stack frames */
struct frame {
    struct pt_regs ptregs;
    union {
	    struct {
		    uint32_t  iaddr;    /* instruction address */
	    } fmt2;
	    struct {
		    uint32_t  effaddr;  /* effective address */
	    } fmt3;
	    struct {
		    uint32_t  effaddr;  /* effective address */
		    uint32_t  pc;	     /* pc of faulted instr */
	    } fmt4;
	    struct {
		    uint32_t  effaddr;  /* effective address */
		    uint16_t ssw;      /* special status word */
		    uint16_t wb3s;     /* write back 3 status */
		    uint16_t wb2s;     /* write back 2 status */
		    uint16_t wb1s;     /* write back 1 status */
		    uint32_t  faddr;    /* fault address */
		    uint32_t  wb3a;     /* write back 3 address */
		    uint32_t  wb3d;     /* write back 3 data */
		    uint32_t  wb2a;     /* write back 2 address */
		    uint32_t  wb2d;     /* write back 2 data */
		    uint32_t  wb1a;     /* write back 1 address */
		    uint32_t  wb1dpd0;  /* write back 1 data/push data 0*/
		    uint32_t  pd1;      /* push data 1*/
		    uint32_t  pd2;      /* push data 2*/
		    uint32_t  pd3;      /* push data 3*/
	    } fmt7;
	    struct {
		    uint32_t  iaddr;    /* instruction address */
		    uint16_t int1[4];  /* internal registers */
	    } fmt9;
	    struct {
		    uint16_t int1;
		    uint16_t ssw;      /* special status word */
		    uint16_t isc;      /* instruction stage c */
		    uint16_t isb;      /* instruction stage b */
		    uint32_t  daddr;    /* data cycle fault address */
		    uint16_t int2[2];
		    uint32_t  dobuf;    /* data cycle output buffer */
		    uint16_t int3[2];
	    } fmta;
	    struct {
		    uint16_t int1;
		    uint16_t ssw;     /* special status word */
		    uint16_t isc;     /* instruction stage c */
		    uint16_t isb;     /* instruction stage b */
		    uint32_t  daddr;   /* data cycle fault address */
		    uint16_t int2[2];
		    uint32_t  dobuf;   /* data cycle output buffer */
		    uint16_t int3[4];
		    uint32_t  baddr;   /* stage B address */
		    uint16_t int4[2];
		    uint32_t  dibuf;   /* data cycle input buffer */
		    uint16_t int5[3];
		    unsigned	   ver : 4; /* stack frame version # */
		    unsigned	   int6:12;
		    uint16_t int7[18];
	    } fmtb;
    } un;
};

#endif /* __ASSEMBLY__ */

#endif /* _M68K_TRAPS_H */
