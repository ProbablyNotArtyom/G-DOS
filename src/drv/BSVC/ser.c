//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// lowlevel drivers for the BSVC serial monitor

void putc(char out){
	asm("%=:\n\t"
		"btst	#0x02, 0x00EFFC03\n\t"
		"beq.s 	%=b\n\t"
		"move.b	%[x], 0x00EFFC07"
		:
		:[x] "r" (out) );
	return;
}

char read(void){
	char read;
	asm("%=:\n\t"
		"btst	#0x00, 0x00EFFC03\n\t"
		"beq.s	%=b\n\t"
		"move.b 0x00EFFC07, %[x]"
		:[x] "=r" (read));

	return read;
}

char query(void){
	char read;
	asm("btst	#0x00, 0x00EFFC03\n\t"
		"beq.s	1f\n\t"
		"move.b 0x00EFFC07, %[x]\n\t"
		"bra.b	2f\n\t"
		"1:\n\t"
		"move.b #0xFF, %[x]\n\t"
		"2:"
		:[x] "=r" (read));

	return read;
}

