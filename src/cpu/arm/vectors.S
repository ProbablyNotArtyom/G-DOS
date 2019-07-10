 .section .vectors, "x"
 .global _reset

 .extern _C_LABEL (_start)
 .extern _C_LABEL (__swi_handler)

 _reset:
	B _start
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
	B _start
	B __swi_handler
