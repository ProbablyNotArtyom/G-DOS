//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// lowlevel drivers for the BSVC serial monitor

void putc(char out){
	int i;
	char* port = 0x00180000;
	port = 0x00;
	for (i = 0; i <= 0x2000; i++) asm("");
	port = out;
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
