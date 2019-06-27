

	#include <linkage.h>
	#include <traps.h>
	#include <entry.h>
	#include <syscall.h>

.globl addrerr, buserr, trap, syscall
.globl syscall_table

.text
ENTRY(trap)
	B .

ENTRY(addrerr)
	B .

ENTRY(buserr)
	B .

ENTRY(syscall)
	B .

