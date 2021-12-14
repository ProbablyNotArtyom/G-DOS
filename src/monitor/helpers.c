//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include "mon.h"

//---------------------------------------------------

extern bool doExit;
extern char* parse;
extern char *current_addr;
extern char *end_addr;

extern uint32_t mon_vars[];

uint32_t mon_vars[6];
bool isCurrentVar;

//---------------------------------------------------

bool isRange() {
	char *tmpParse = parse;
	while (*tmpParse != '.' && *tmpParse != ',' && *tmpParse != '\0') tmpParse++;
	if (*tmpParse != '.' && *tmpParse != ',') return false;
	return true;
}

bool isVar() {
	if (isLower(*parse) && (*(parse+1) == ' ' || !isAlnum(*(parse+1)))) return true;
	return false;
}

bool isArg() {
	return (isVar() || isAddr());
}

bool setCurrents() {
	skipBlank();
	if (isVar()) isCurrentVar = true;
	else if (isAddr()) isCurrentVar = false;

	return getRange(&current_addr, &end_addr);
}

char* skipBlank() {
	while (*parse == ' ') parse++;
	return parse;
}

char* skipToken() {
	while (*parse != ' ' && *parse != '\0') parse++;
	return parse;
}

char* skipHex() {
	uint8_t i;
	while (true) {
		for (i = 0; *parse != hexTable[i] && hexTable[i] != '\0'; i++);
		if (hexTable[i] == '\0') return parse;
		parse++;
	}
}

bool isAddr() {
	uint8_t i;
	for (i = 0; *parse != hexTable[i] && hexTable[i] != '\0'; i++);
	return (hexTable[i] != '\0');
}

bool funcCmp(const char *s1, const char *s2) {
	while (*s1 == *s2) {
		if (*s1 == ' ' || *s1 == '.') return true;
		s1++;
		*s2++;
	}
	if (*s1 == '\0' && (*s2 == ' ' || *s2 == '.')) return true;
	return false;
}

ADDRSIZE strToHEX() {
	uint8_t i;
	ADDRSIZE val = 0;
	while (*parse != ' ' && *parse != '\0') {
		for (i = 0; *parse != hexTable[i] && hexTable[i] != '\0'; i++);
		if (hexTable[i] == '\0') return val;
		val = (val << 4) | i;
		parse++;
	}
	return val;
}

enum errList throw(enum errList index) {
	if (index == errNONE) {
		return errNONE;
	} else if (index == errDOEXIT) return errDOEXIT;
	puts(errors[index]);
	return errNONE;
}

void evalScript() {


}

uint32_t *getMonVar(char var) {
	if (isLower(var)) {
		return &mon_vars[ (var - 0x60) ];
	} else {
		return 0x00000000;
	}
}

void setMonVar(char var, uint32_t val) {
	if (isAscii(var) && isLower(var)) {
		mon_vars[ var & 0x00011111 ] = val;
	} else {
		return;
	}
}

bool getRange(void **lower, void **upper) {
	skipBlank();
	if (isAddr()) {
		*lower = strToHEX();
	} else if (isVar()) {
		*lower = getMonVar(*parse);
	} else if (*parse == '.' || *parse == ',') {
		*lower = current_addr;
	} else {
		return false;
	}
	if (isCurrentVar == false && (*parse == '.' || *parse == ',')) {
		char tmp = *parse;
		parse++;
		skipBlank();
		ifEOI(errNOARGS);
		if (!isAddr()) return errSYNTAX;
		if (tmp == '.')
			*upper = strToHEX();
		else
			*upper = (strToHEX() + (uint32_t)*lower);
	} else {
		if (*parse != ' ' || *parse != '\0') {
			skipToken();
		}
		*upper = NULL;
	}
	return true;
}

void printHex(char num) {
	putchar(hexTable[(num & 0x0F)]);
	return;
}

void printByte(char num) {
	putchar(hexTable[(num & 0xF0) >> 4]);
	putchar(hexTable[(num & 0x0F)]);
	return;
}

void printWord(uint16_t num) {
	putchar(hexTable[(num & 0xF000) >> 12]);
	putchar(hexTable[(num & 0x0F00) >> 8]);
	putchar(hexTable[(num & 0x00F0) >> 4]);
	putchar(hexTable[(num & 0x000F)]);
	return;
}

void printLong(uint32_t num) {
	printWord((num & 0xFFFF0000) >> 16);
	printWord(num & 0x0000FFFF);
	return;
}
