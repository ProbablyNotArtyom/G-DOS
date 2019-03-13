

	#include <linkage.h>
	#include <traps.h>
	#include <entry.h>
	#include <syscall.h>

.globl addrerr, buserr, trap, syscall
.globl syscall_table
.globl cpu_int_enable, cpu_int_disable, cpu_cache_enable, cpu_cache_disable
.globl cpu_cache_flush

CACR_EI   = 1       /* Enable Instruction Cache      */
CACR_FI   = 1<<1    /* Freeze Instruction Cache      */
CACR_CEI  = 1<<2    /* Clear Entry in Instr. Cache   */
CACR_CI   = 1<<3    /* Clear Instruction Cache       */
CACR_IBE  = 1<<4    /* Instr. Cache Burst Enable     */

CACR_ED   = 1<<8    /* Enable Data Cache             */
CACR_FD   = 1<<9    /* Freeze Data Cache             */
CACR_CED  = 1<<10   /* Clear Entry in Data Cache     */
CACR_CD   = 1<<11   /* Clear Data Cache              */
CACR_DBE  = 1<<12   /* Data Cache Burst Enable       */
CACR_WA   = 1<<13   /* Write Allocate the Data Cache */
BOARD_CACR0	= (CACR_CI + CACR_EI + CACR_CD + CACR_ED)

.text
ENTRY(trap)
	SAVE_ALL_INT
	move.l	%sp,%sp@-
	jbsr	trap_c
	addq.l	#4,%sp
	RESTORE_ALL

ENTRY(addrerr)
	SAVE_ALL_INT
	move.l	%sp,%sp@-
	jbsr	addrerr_c
	addq.l	#4,%sp
	RESTORE_ALL

ENTRY(buserr)
	SAVE_ALL_INT
	move.l	%sp,%sp@-
	jbsr	buserr_c
	addq.l	#4,%sp
	RESTORE_ALL

ENTRY(syscall)
	movem.l	%d1-%d5/%a0,%sp@-
	lea.l	syscall_table,%a0
	mulu.w	#4,%d0
	adda.l	%d0,%a0
	movea.l	(%a0),%a0
	jsr		(%a0)
	movem.l	%sp@+,%a0/%d1-%d5
	rte

ENTRY(cpu_cache_disable)
#ifdef CONFIG_CPU_68030
	move.l  #(CACR_CI+CACR_CD),%d0
    movec %d0,%cacr
#endif
	rts
ENTRY(cpu_cache_enable)
#ifdef CONFIG_CPU_68030
	move.l  #BOARD_CACR0,%d0
    movec %d0,%cacr
#endif
	rts
ENTRY(cpu_cache_flush)
#ifdef CONFIG_CPU_68030
	movec %cacr,%d0
    or.w    #CACR_CD,%d0
    movec %d0,%cacr
#endif
    rts
ENTRY(cpu_int_disable)
	or.w    #0x2700,%sr
	rts
ENTRY(cpu_int_enable)
	and.w   #~0x0700,%sr
	rts
