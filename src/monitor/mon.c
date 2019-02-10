//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	#include <io.h>

	#include "mon.h"

//---------------------------------------------------

bool gmon_do_cmd(uint8_t num);

static enum errList help();
static enum errList exit();
static enum errList deposit();
static enum errList view();
static enum errList copy();
static enum errList move();
static enum errList fill();
static enum errList execute();
static enum errList sirpinski();
static enum errList memmod();
static enum errList echo();
static enum errList echo_fmt();
static enum errList delay_arb();
static enum errList brot();

//----------------------Tables-----------------------

enum errList const (* const funcTable[])() = {
	help,
	exit,
	deposit,
	view,
	copy,
	move,
	fill,
	execute,
	sirpinski,
	memmod,
	echo,
	echo_fmt,
	delay_arb,
	brot,
	NULL
};

const char* const funcNames[] = {
	"help ",
	"exit ",
	"put ",
	"read ",
	"copy ",
	"move ",
	"fill ",
	"go ",
	"demo ",
	"@.",
	"echo ",
	"printf ",
	"delay ",
	"brot ",
	"\0"
};

const char* const errors[] = {
	"",
	"\n\r?Syntax error",
	"\n\r?Undefined function",
	"\n\r?Unexpected arguments",
	"\n\r!Unexpected end of input",
	"\n\r!Invalid hex",
	"\n\r?Invalid range"
};

const char const hexTable[] = "0123456789ABCDEF";

const char const helpText[] =
	"\r\n\r\nG'Mon Version " GMON_VERSION
	"\r\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-"
	"\r\n  read [range] ...          put <byte> ..."
	"\r\n  copy [range], <dest>      move [range], <dest>"
	"\r\n  fill [range], <byte>      go [address] ..."
	"\r\n  help";

//-----------------------Main------------------------

static bool doExit;
char *parse;
char *current_addr;
char *end_addr;
char *cmdStart;

void monBegin(){

	static uint8_t numCMDs;
	static uint32_t numLoops;
	static char inBuffer[BUFFLEN];					// Our input buffer
	puts("G'Mon Version " GMON_VERSION);
	doExit = false;
	current_addr = 0x00;
	end_addr = 0x00;
	while (doExit == false){
		if (end_addr == 0x00) printf("\r\n(%08X)\r\n", (size_t)current_addr);	// Print the prompt
		else printf("\r\n(%08X.%08X)\r\n", (size_t)current_addr, end_addr);	// Print the prompt
		fputs("   ~ ");
		parse = inBuffer;							// Set the parse pointer to the beginning of the buffer
		gets(inBuffer, BUFFLEN);					// Get user input
		skipBlank();								// Skip and leading spaces
		if (!isEOI()){
			puts("");
			numLoops = 1;
			numCMDs = 0x01;
			skipBlank();
			cmdStart = parse;
			if (*cmdStart == '{'){
				while (*cmdStart != '}') cmdStart++;
				numLoops = strtoul(parse+1, NULL, 10);
				cmdStart++;
			}
			parse = cmdStart;
			for (char *tmp = parse; *tmp != '\0'; tmp++) if (*tmp == ';') numCMDs += 1;
			while (numLoops > 0){
				gmon_do_cmd(numCMDs);
				numLoops--;
				parse = cmdStart;
			}
		} else {
			puts("");
		}											// Call it from a throw so we can print out any errors after returning
	}
	return;
}

bool gmon_do_cmd(uint8_t num){
	uint8_t i;
	for (char *tmp = parse; *tmp != '\0'; tmp++){
		if (*tmp == ';'){
			*tmp = '\0';
		}
	}
	while(num > 0){
		skipBlank();
		for (i = 0; funcCmp(parse, funcNames[i]) == false && i < ARRAY_SIZE(funcNames)-1; i++);	// Identify what function it is
		if (i == ARRAY_SIZE(funcNames)-1) {
			if (setCurrents() == false){
				throw(errUNDEF);			// If none matches, complain
				return false;
			}
		} else {
			skipToken();							// Skip over the function name itself
			if (throw((*funcTable[i])()) != errNONE) return false;
		}
		while (*parse != '\0') parse++;
		parse++;
		num--;
	}
}

/* Exits the monitor */
static enum errList exit(){
	doExit = true;
	return errNONE;
}

/* Writes bytes to memory */
static enum errList deposit(){
	char *ptr = current_addr;
	skipBlank();
	if (isCurrentVar){
		outl(strToHEX(), ptr);
	} else {
		while (*parse != '\0'){							// Keep reading in arguments until we hit the end of input
			outb(strToHEX(), ptr++);					// Take those arguments and store them to memory in succesion
			skipBlank();
		}
	}
	return errNONE;										// Return error free
}

static void read_range(char *ptr,char *end, char size){
	if (end != NULL){									// If we hit a range identifier...
		uint8_t column;									// Create something to track how many columns have been printed so far
		char *addrBuff;
		while (ptr <= end){								// Continue until we've reached the end of the range
			int i;
			if (ptr <= end){
				fputs("\r\n ");							// Then set up a new line
				column = 0;								// And print out the location header
				printLong(ptr);
				fputs(" | ");
			}
			while (column < 16 && ptr <= end){
				printByte(inb(ptr++));							// Print data byte at this address
				putc(' ');									// Space between bytes
				column++;									// Increase our column number
			}
			for (i = column; i < 16; i++) fputs("   ");
			fputs("| ");
			addrBuff = ptr - column;
			for (i = 0; i < column; addrBuff++){
				if (*addrBuff >= 0x20 && *addrBuff < 0x7F) putc(*addrBuff);
				else putc('.');
				i++;
			}
		}
	} else {
		fputs("\r\n   ");									// Then set up a new line
		printLong(ptr);
		fputs(" | ");
		if (size == 'l') printLong(inl(ptr));
		else printByte(inb(ptr));
	}
}

/* Handles viewing of memory */
static enum errList view(){
	char *ptr, *end;									// Create start and end pointers
	if (!isEOI()){
		while(*parse != '\0'){
			skipBlank();
			if (isVar()){
				read_range(getMonVar(*parse), 0x00000000, 'l');
				parse++;
			} else {
				if (!getRange(&ptr, &end)) return errSYNTAX;
				read_range(ptr, end, 'b');
			}
		}
	} else {
		if (isCurrentVar) read_range(current_addr, end_addr, 'l');
		else read_range(current_addr, end_addr, 'b');
	}

	return errNONE;										// Return error free
}

/* Copies a range of data */
static enum errList copy(){
	char *ptr, *end, *dest;								// Create pointers for start, end, and destination of block

	if (isRange()){
		if (!getRange(&ptr, &end)) return errSYNTAX;
	} else {
		ptr = current_addr;
		end = end_addr;
	}
	getArg(dest);

	if (dest <= ptr) {									// If the destination is below the source in memory,
		while (ptr <= end) outb(*ptr++, dest++);		// then copy it starting at the beginning
	} else {
		dest += end - ptr;								// If the destination is above the start of the source,
		while (end >= ptr) outb(*end--, dest--);		// then copy starting at the end of the source
	}													// This is done to avoid overwriting the source before we can copy it

	return errNONE;										// Return error free
}

/* Moves a block of data, filling the old space with 00 */
static enum errList move(){
	char *ptr, *end, *dest;								// Create pointers for start, end, and destination of block

	if (isRange()){
		if (!getRange(&ptr, &end)) return errSYNTAX;
	} else {
		ptr = current_addr;
		end = end_addr;
	}
	getArg(dest);

	if (dest <= ptr) {									// If the destination is below the source in memory,
		while (ptr <= end){
			outb(*ptr, dest++);						// then copy it starting at the beginning
			outb(NULL, ptr++);
		}
	} else {
		dest += end - ptr;								// If the destination is above the start of the source,
		while (end >= ptr){
			outb(*end, dest--);						// then copy starting at the end of the source
			outb(NULL, end--);
		}
	}													// This is done to avoid overwriting the source before we can copy it

	return errNONE;										// Return error free
}

/* Fills a range with a pattern byte */
static enum errList fill(){
	char *ptr, *end;									// Create pointers for the start and end of the section
	uint8_t val;										// The fill pattern itself

	if (isRange()){
		if (!getRange(&ptr, &end)) return errSYNTAX;
	} else {
		ptr = current_addr;
		end = end_addr;
	}
	getArg(val);

	while (ptr <= end) outb(val, ptr++);				// Set every byte from *ptr to *end to the pattern in val
	return errNONE;										// Return error free
}

/* Starts executing code from a place in memory */
static enum errList execute(){
	void (*ptr)(void);
	if (!isEOI()){
		getArg(ptr);
	} else {
		ptr = current_addr;
	}
	(*ptr)();											// Call that function
	return errNONE;										// Return error free, assuming that whatever we call actually returns (good chance it wont)
}

static enum errList memmod(){
	void *ptr;
	char size;
	parse--;
	if (*parse == 'l') size = 'l';
	else if (*parse == 'w') size = 'w';
	else size = 'b';
	parse++;
	skipBlank();

	if (isArg()){
		getArg(ptr);
	} else {
		ptr = current_addr;
	}

	skipBlank();
	switch (size){
		case 'b':
			if (*parse == '-'){
				*(uint8_t *)ptr = *(uint8_t *)ptr - strtoul(++parse, NULL, 10);
			} else if (*parse == '+'){
				*(uint8_t *)ptr = *(uint8_t *)ptr + strtoul(++parse, NULL, 10);
			} else if (*parse == '/'){
				*(uint8_t *)ptr = *(uint8_t *)ptr / strtoul(++parse, NULL, 10);
			} else if (*parse == '*'){
				*(uint8_t *)ptr = *(uint8_t *)ptr * strtoul(++parse, NULL, 10);
			} else return errSYNTAX;
			break;
		case 'w':
			if (*parse == '-'){
				*(uint16_t *)ptr = *(uint16_t *)ptr - strtoul(++parse, NULL, 10);
			} else if (*parse == '+'){
				*(uint16_t *)ptr = *(uint16_t *)ptr + strtoul(++parse, NULL, 10);
			} else if (*parse == '/'){
				*(uint16_t *)ptr = *(uint16_t *)ptr / strtoul(++parse, NULL, 10);
			} else if (*parse == '*'){
				*(uint16_t *)ptr = *(uint16_t *)ptr * strtoul(++parse, NULL, 10);
			} else return errSYNTAX;
			break;
		case 'l':
			if (*parse == '-'){
				*(uint32_t *)ptr = *(uint32_t *)ptr - strtoul(++parse, NULL, 10);
			} else if (*parse == '+'){
				*(uint32_t *)ptr = *(uint32_t *)ptr + strtoul(++parse, NULL, 10);
			} else if (*parse == '/'){
				*(uint32_t *)ptr = *(uint32_t *)ptr / strtoul(++parse, NULL, 10);
			} else if (*parse == '*'){
				*(uint32_t *)ptr = *(uint32_t *)ptr * strtoul(++parse, NULL, 10);
			} else return errSYNTAX;
			break;
		default:
			break;
	}

	return errNONE;
}

static enum errList echo(){
	skipBlank();
	if (isEOI()) return;

	puts(parse);

	return errNONE;
}

static enum errList echo_fmt(){
	char *ptr = parse;
	char *end;
	uint32_t *val;
	puts("");
	while (*ptr != '\0' && *ptr != '\"') ptr++;
	if (*ptr == '\0') return errSYNTAX;
	ptr++;
	end = ptr;
	while (*end != '\0' && *end != '\"') end++;
	if (*end == '\0') return errSYNTAX;
	*end = '\0';

	parse = (end+1);

	if (isEOI()) return errSYNTAX;

	getArg(val);
	printf(ptr, *val);
	*end = '\"';
	return errNONE;
}

static enum errList sirpinski(){
	uint8_t size;
	getArg(size);
	int x, y, i;
	puts("");
	for (int loop = 0; loop <= size; loop++){
		for (y = (1 << loop) - 1; y >= 0; y--, puts("")) {
			for (i = 0; i < y; i++) putc(' ');
			for (x = 0; x + y < (1 << loop); x++)
			printf((x & y) ? "  " : "* ");
		}
		for (y = 1; y <= (1 << loop); y++, puts("")) {
			for (i = 0; i < y; i++) putc(' ');
			for (x = 0; x + y < (1 << loop); x++)
			printf((x & y) ? "  " : "* ");
		}
	}
	for (int loop = size-1; loop >= 0; loop--){
		for (y = (1 << loop) - 1; y >= 0; y--, puts("")) {
			for (i = 0; i < y; i++) putc(' ');
			for (x = 0; x + y < (1 << loop); x++)
			printf((x & y) ? "  " : "* ");
		}
		for (y = 1; y <= (1 << loop); y++, puts("")) {
			for (i = 0; i < y; i++) putc(' ');
			for (x = 0; x + y < (1 << loop); x++)
			printf((x & y) ? "  " : "* ");
		}
	}
	return errNONE;
}

static enum errList help(){
	puts(helpText);
	return errNONE;
}

static enum errList delay_arb(){
	uint32_t arg;
	if (isEOI()){
		arg = 0xFFFF;
	} else {
		getArg(arg);
	}
	delay(arg);
	return errNONE;
}

static enum errList brot(){
	int n = 0;
	float r,i,R,I,b;
	for(i=-1;i<1;i+=.06,puts(""))for(r=-2;I=i,(R=r)<1;
	r+=.040,putc(n+31))for(n=0;b=I*I,26>n++&&R*R+b<4;I=2*R*I+i,R=R*R-b+r);
	return errNONE;
}
