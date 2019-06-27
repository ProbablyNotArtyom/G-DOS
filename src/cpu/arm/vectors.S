 .section .vectors, "x"
 .global _reset
 
 .extern _C_LABEL (_start)
 
 _reset:
	B _start
	B .
	B .
	B .
	B .
	B .
	B .
	B .
	B .
	B .
	B .
	B .
	B .
	B .
	
