//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

//---------------------Includes----------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <fs.h>

//-----------------Variable Protos-------------------

//-----------------------Main------------------------

int main(void){
	if (init() == -1){
		puts("Init sequence failed!\n\rHaulting execution...");
		while (1);
	}
	puts("NotArtyom 02/11/18");
	puts("G'DOS Shell");

	fsInit();
	romfsInit();
	loadELF(fsLoad("test.elf"), 0x00400000);

	monBegin();

	puts("End.");
	while(1);
}
