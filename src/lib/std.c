//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// generic routines that dont require ASM

	#include <std.h>
	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>

//-----------------Variable Protos-------------------

extern const size_t _end;

static const char hexTable[16] = {								// Hex to ASCII lookup table
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'A', 'B', 'C', 'D', 'E', 'F'
};

//---------------------------------------------------

void putnl(void){
	putc('\n');
	putc('\r');
	return;
}

void putsl(char *str, int len){
	while (len-- > 0) putc(*str++);
	return;
}

void fputs(char *str){
	while(*str != NULL) putc(*str++);
	return;
}

void puts(char *str){
	while(*str != NULL) putc(*str++);
	putc('\n');
	putc('\r');
	return;
}

char *gets(char *buff, int len){
	register char *s = buff;
	register int index = len;
	while (true){
		*s = read();
		if (*s == 0x7F && index == len);
		else if (*s == 0x7F){
			fputs("\b \b");
			*s = NULL;
			index++;
			s--;
		} else {
			if (index >= 2){
				putc(*s);
				if (*s == '\r') break;
				s++;
				index--;
			} else if (*s == '\r') {
				putc(*s);
				break;
			}
		}
	}
	*s = NULL;
	return *buff;
}

char *sgets(char *buff, int len){
	register char *s = buff;
	register int index = len;
	while (index >= 2){
		*s = read();
		if (*s == 0x7F && index == len);
		else if (*s == 0x7F){
			fputs("\b \b");
			*s = NULL;
			index++;
			s--;
		} else {
			if (*s == '\r') break;
			s++;
			index--;
		}
	}
	*s = NULL;
	return *buff;
}
