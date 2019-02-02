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

	#include <disk.h>
	#include <fs.h>
	#include "gdos.h"

//-----------------------Main------------------------

void keyboardTest(){
	char lastChar;
	puts("**KEYBOARD TEST**");
	fputs("> ");
	register char s;
	lastChar = NULL;
	while(true){
		while(true){
			s = read();
			if (s != NULL && s != lastChar) break;
		}
		lastChar = s;
		if (s == 0x7F){
			fputs("\b \b");
		} else {
			putc(s);
			if (s == '\r') break;
		}
	}
	return;
}

void gdos(void){

	

	shellBegin();

	puts("End.");
	while(1);
}
