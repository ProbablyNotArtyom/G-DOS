 //---------------------------------------------------
 //
 //	GBoot v0.0
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // BSVC Serial Driver

 #ifdef CONFIG_DEV_BSVCSERIAL
 #define CONFIG_DEV_BSVCSERIAL

	#include <std.h>
	#include <stdbool.h>
	#include <char.h>
	#include <mod/init.h>
	#include "BSVC.h"

	#include <tui.h>

//--------------------Functions----------------------

char bsvc_dev_peek(){
	char read = '\0';
	asm("btst	#0x00, 0x00EFFC03\n\t"
		"beq.s	%=f\n\t"
		"move.b 0x00EFFC07, %[x]\n\t"
		"%=:"
		:[x] "=r" (read));
	return read;
}

char bsvc_dev_read(){
	char read;
	asm("%=:\n\t"
		"btst	#0x00, 0x00EFFC03\n\t"
		"beq.s	%=b\n\t"
		"move.b 0x00EFFC07, %[x]"
		:[x] "=r" (read));
	return read;
}

charResult bsvc_dev_write(char out){
	if (out == '\n')	bsvc_dev_write('\r');
	asm("%=:\n\t"
		"btst	#0x02, 0x00EFFC03\n\t"
		"beq.s 	%=b\n\t"
		"move.b	%[x], 0x00EFFC07"
		:
		:[x] "r" (out) );
	return;
}

charResult bsvc_dev_init(){
	asm("move.b	#0b00010000, 0x00EFFC05\n\t"
		"move.b	#0b00100011, 0x00EFFC01\n\t"
		"move.b	#0b00010111, 0x00EFFC01\n\t"
		"move.b	#0b11001011, 0x00EFFC03\n\t"
		"move.b	#0b00000101, 0x00EFFC05"
	);
	tui_cls();
	puts("BSVC Serial driver / NotArtyom / 27-6-19");
}

early_initcall(bsvc_dev_init);

 #endif
