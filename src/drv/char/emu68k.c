 //---------------------------------------------------
 //
 //	G-DOS
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // emu68k Serial Driver

 #ifdef CONFIG_DEV_EMU68KSERIAL
 #define CONFIG_DEV_EMU68KSERIAL

	#include <std.h>
	#include <stdbool.h>
	#include <char.h>
	#include <mod/init.h>
	#include "emu68k.h"

	#include <tui.h>

//--------------------Functions----------------------

char emu68k_dev_peek(){
	char read = '\0';
	asm("btst	#0x00, 0x00EFFC03\n\t"
		"beq.s	%=f\n\t"
		"move.b 0x00EFFC07, %[x]\n\t"
		"%=:"
		:[x] "=r" (read));
	return read;
}

char emu68k_dev_read(){
	char read;
	asm("%=:\n\t"
		"btst	#0x00, 0x00EFFC03\n\t"
		"beq.s	%=b\n\t"
		"move.b 0x00EFFC07, %[x]"
		:[x] "=r" (read));
	return read;
}

charResult emu68k_dev_write(char out){
	if (out == '\n') emu68k_dev_write('\r');
	asm("%=:\n\t"
		"btst	#0x02, 0x00EFFC03\n\t"
		"beq.s 	%=b\n\t"
		"move.b	%[x], 0x00EFFC07"
		:
		:[x] "r" (out) );
	return;
}

charResult emu68k_dev_init(){
	tui_cls();
	puts("emu68k Serial driver / NotArtyom / 27-6-19");
}

early_initcall(emu68k_dev_init);

 #endif
