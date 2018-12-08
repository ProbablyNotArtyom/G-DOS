//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#define	BUFFLEN		0xFF
	#define NUMFUNCS	7

	#define	ADDRSIZE	uint32_t

//---------------------Includes----------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	
	#include "mon.h"

//-----------------------Main------------------------

bool doExit;
char* parse;

void monBegin(){
	doExit = false;
	static char inBuffer[BUFFLEN];
	puts("G'Mon Version 0.1");
	while (doExit == false){
		fputs("> ");
		parse = inBuffer;
		gets(inBuffer, BUFFLEN);
		skipBlank();
		uint8_t i;
		for (i = 0; funcCmp(parse, funcNames[i]) == false && i < NUMFUNCS; i++);
		if (i == NUMFUNCS) throw(errUNDEF);
		else {
			skipToken();
			throw((*funcTable[i])());
		}
	}
	return;
}

static enum errList exit(){
	doExit = true;
	return errNONE;
}

static enum errList deposit(){
	if (*skipBlank() == '\0') return errNOARGS;
	char *ptr = strToHEX();
	skipBlank();
	while (*parse != '\0'){
		*ptr++ = strToHEX();
		skipBlank();
	}
	return errNONE;
}

static enum errList read(){
	if (*skipBlank() == '\0') return errNOARGS;
	char *ptr;
	char *end;
	while (*parse != '.' && *parse != '\0'){
		ptr = strToHEX();
		skipBlank();
		puts("");
		printLong(ptr);
		fputs(" | ");
		printByte(*ptr++);
		putc(' ');
	} 
	if (*parse == '.'){
		parse++;
		skipBlank();
		uint8_t column = 1;
		end = strToHEX();
		if (end == NULL) return errHEX;
		while (ptr != end){
			if (column == 16){
				puts("");
				column = 0;
				printLong(ptr);
				fputs(" | ");
			}
			printByte(*ptr);
			column++;
			putc(' ');
			ptr++;
		}
	}
	return errNONE;
}

static enum errList copy(){
	if (*skipBlank() == '\0') return errNOARGS;
	return errNONE;
}

static enum errList move(){
	if (*skipBlank() == '\0') return errNOARGS;
	return errNONE;
}

static enum errList fill(){
	if (*skipBlank() == '\0') return errNOARGS;
	return errNONE;
}

static enum errList execute(){
	if (*skipBlank() == '\0') return errNOARGS;
	void (*ptr)(void) = strToHEX();
	(*ptr)();
	return errNONE;
}

//----------------------Tables-----------------------

const enum errList const (*funcTable[])() = {
	exit,
	deposit,
	read,
	copy,
	move,
	fill,
	execute,
	NULL
};

const char* const funcNames[] = {
	"exit ",
	"put ",
	"read ",
	"copy ",
	"move ",
	"fill ",
	"go ",
	"\0"
};

const char* const errors[] = {
	"",
	"\n\r?Syntax error",
	"\n\r?Undefined function",
	"\n\r?Unexpected arguments",
	"\n\r!Unexpected end of input",
	"\n\r!Invalid hex"
};

const char const hexTable[] = "0123456789ABCDEF";