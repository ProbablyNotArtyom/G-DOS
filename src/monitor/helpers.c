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

	#include "mon.h"

//---------------------------------------------------

extern bool doExit;
extern char* parse;
extern char *current_addr;
extern char *end_addr;

//---------------------------------------------------

bool isRange(){
	char *tmpParse = parse;
	while (*tmpParse != '.' && *tmpParse != ',' && *tmpParse != '\0') tmpParse++;
	if (*tmpParse != '.' && *tmpParse != ',') return false;
	return true;
}

void setCurrents(){
	if (isAddr()){
		current_addr = strToHEX();
		skipBlank();
		if (!(*parse == '.' || *parse == ',')) end_addr = NULL;
	}
	if ((*parse == '.' || *parse == ',')){
		getRange(current_addr, end_addr);
		skipBlank();
	}
}

char* skipBlank(){
	while (*parse == ' ') parse++;
	return parse;
}

char* skipToken(){
	while (*parse != ' ' && *parse != '\0') parse++;
	return parse;
}

char* skipHex(){
	uint8_t i;
	while (true){
		for (i = 0; *parse != hexTable[i] && hexTable[i] != '\0'; i++);
		if (hexTable[i] == '\0') return parse;
		parse++;
	}
}

bool isAddr(){
	uint8_t i;
	for (i = 0; *parse != hexTable[i] && hexTable[i] != '\0'; i++);
	return (hexTable[i] != '\0');
}

bool funcCmp(const char *s1, const char *s2){
	while (*s1 == *s2){
		if (*s1 == ' ' || *s1 == '.') return true;
		s1++;
		*s2++;
	}
	if (*s1 == '\0' && (*s2 == ' ' || *s2 == '.')) return true;
	return false;
}

ADDRSIZE strToHEX(){
	uint8_t i;
	ADDRSIZE val = 0;
	while (*parse != ' ' && *parse != '\0'){
		for (i = 0; *parse != hexTable[i] && hexTable[i] != '\0'; i++);
		if (hexTable[i] == '\0') return val;
		val = (val << 4) | i;
		parse++;
	}
	return val;
}

enum errList throw(enum errList index){
	if (index == errNONE) {
		return errNONE;
	} else if (index == errDOEXIT) return errDOEXIT;
	puts(errors[index]);
	return errNONE;
}

void evalScript(){


}
