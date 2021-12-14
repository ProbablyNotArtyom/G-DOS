 //---------------------------------------------------
 //
 //	G-DOS
 //	NotArtyom
 //	02/11/18
 //
 //---------------------------------------------------
 // arduino parallel tty Driver

 #ifdef CONFIG_DEV_AVRPTOS
 #define CONFIG_DEV_AVRPTOS

	#include <std.h>
	#include <char.h>
	#include "avrPtoS.h"

//--------------------Functions----------------------

charResult avrptos_dev_write(char out){
	asm volatile ("move.b 	#0x00, 0x00090400\n\t"
		"move.l		#0x0000000F, %%d4\n\t"
		"A%=:\n\t"
		"subi.l  	#0x00000001, %%d4\n\t"
		"cmpi.l		#0x00000000, %%d4\n\t"
		"bne		A%=\n\t"
		"move.b 	%[x], 0x00090400\n\t"
		"move.l		#0x0000000F, %%d4\n\t"
		"B%=:\n\t"
		"subi.l  	#0x00000001, %%d4\n\t"
		"cmpi.l		#0x00000000, %%d4\n\t"
		"bne		B%="
		:
		:[x] "r" (out)
		: "d4" );
	return CH_OK;
}

char avrptos_dev_read(void){

	while(1);
}

charResult avrptos_dev_init(){
	avrptos_dev_write('\r');
	avrptos_dev_write('\n');
	return CH_OK;		// Dummy
}

#endif
