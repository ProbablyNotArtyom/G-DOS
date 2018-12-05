//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#define	BUFFLEN		0xFF
	#define NUMFUNCS	6

	#define	ADDRSIZE	uint32_t

//---------------------Includes----------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

//----------------------Protos-----------------------

	static enum errList {
		errNONE,
		errSYNTAX,
		errUNDEF,
		errNOARGS,
		errEND,
		errHEX
	};

	static const enum errList const (*funcTable[])(char*);
	static const char* const funcNames[];
	static const char* const errors[];
	static const char const hexTable[];
	static bool doExit = false;

	static char* skipBlank(char* ptr);
	static char* skipToken(char* ptr);
	static char* skipHex(char* ptr);
	static char* skipChar(char* ptr, char arg);
	static ADDRSIZE strToHEX(char* str);
	static void throw(enum errList index);
	static bool funcCmp(const char *s1, const char *s2);

//-----------------------Main------------------------

void monBegin(){
	static char inBuffer[BUFFLEN];
	static char* parse;
	puts("G'Mon Version 0.1");
	while (doExit == false){
		fputs("> ");
		parse = inBuffer;
		gets(inBuffer, BUFFLEN);
		parse = skipBlank(parse);
		uint8_t i;
		for (i = 0; funcCmp(parse, funcNames[i]) == false && i < NUMFUNCS; i++);
		if (i == NUMFUNCS) throw(errUNDEF);
		else {
			parse = skipToken(parse);
			throw((*funcTable[i])(parse));
		}
	}
	return;
}

static enum errList exit(char* args){
	doExit = true;
	return errNONE;
}

static enum errList deposit(char* args){
	if (*skipBlank(args) == '\0') return errNOARGS;
	args = skipBlank(args);
	char *ptr = strToHEX(args);
	args = skipBlank(skipHex(args));
	while (*args != '\0'){
		*ptr++ = strToHEX(args);
		args = skipBlank(skipHex(args));
	}
	return errNONE;
}

static enum errList read(char* args){
	if (*skipBlank(args) == '\0') return errNOARGS;
	args = skipBlank(args);
	char *ptr;
	char *end;
	while (*args != '-' && *args != '\0'){
		ptr = strToHEX(args);
		args = skipBlank(skipHex(args));
		puts("");
		printLong(ptr);
		fputs(" | ");
		printByte(*ptr++);
		putc(' ');
	} 
	if (*args == '-'){
		args++;
		args = skipBlank(args);
		uint8_t column = 1;
		end = strToHEX(args);
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
	puts("");
	return errNONE;
}

static enum errList copy(char* args){
	if (*skipBlank(args) == '\0') return errNOARGS;
	return errNONE;
}

static enum errList move(char* args){
	if (*skipBlank(args) == '\0') return errNOARGS;
	return errNONE;
}

static enum errList fill(char* args){
	if (*skipBlank(args) == '\0') return errNOARGS;
	return errNONE;
}

static char* skipBlank(char* ptr){
	while (*ptr == ' ') ptr++;
	return ptr;
}

static char* skipToken(char* ptr){
	while (*ptr != ' ' && *ptr != '\0') ptr++;
	return ptr;
}

static char* skipHex(char* ptr){
	uint8_t i;
	while (true){
		for (i = 0; *ptr != hexTable[i] && hexTable[i] != '\0'; i++);
		if (hexTable[i] == '\0') return ptr; 
		ptr++;
	}
}

static char* skipChar(char* ptr, char arg){
	while (*ptr != arg && *ptr != '\0') ptr++;
	if (*ptr == arg) return ++ptr;
	else return NULL;
}

static bool funcCmp(const char *s1, const char *s2){
	while (*s1 == *s2){
		if (*s1++ == ' ') return true;
		*s2++;
	}
	if (*s1 == '\0' && *s2 == ' ') return true;
	return false;
}

static ADDRSIZE strToHEX(char* str){
	uint8_t i;
	ADDRSIZE val = 0;
	while (*str != ' ' && *str != '\0'){
		for (i = 0; *str != hexTable[i] && hexTable[i] != '\0'; i++); 
		if (hexTable[i] == '\0') return val;	
		val = (val << 4) | i;
		str++;
	}
	return val;
}

static void throw(enum errList index){
	if (index == errNONE) return;
	puts(errors[index]);
}

//----------------------Tables-----------------------

static const enum errList const (*funcTable[])(char*) = {
	exit,
	deposit,
	read,
	copy,
	move,
	fill,
	NULL
};

static const char* const funcNames[] = {
	"exit ",
	"put ",
	"read ",
	"copy ",
	"move ",
	"fill ",
	"\0"
};

static const char* const errors[] = {
	"",
	"?Syntax error",
	"?Undefined function",
	"?Unexpected arguments",
	"!Unexpected end of input",
	"!Invalid hex"
};

static const char const hexTable[] = "0123456789ABCDEF";