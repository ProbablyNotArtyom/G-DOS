

	#include <linkage.h>
	#include <traps.h>
	#include <entry.h>

.globl addrerr, buserr, trap, syscall
.globl syscall_table

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
	SAVE_ALL_SYS

	RESTORE_ALL
