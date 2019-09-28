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

//---------------------------------------------------

char *gets(char *buff, int len){
	register char *s = buff;
	register int index = len;
	while (true){
		*s = getchar();
		if ((*s == 0x7F || *s == 0x08) && index == len);
		else if (*s == 0x7F || *s == 0x08){
			printf("\b \b");
			*s = NULL;
			index++;
			s--;
		} else {
			if (index >= 2){
				putchar(*s);
				if (*s == '\r') break;
				s++;
				index--;
			} else if (*s == '\r') {
				putchar(*s);
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
		*s = getchar();
		if ((*s == 0x7F || *s == 0x08) && index == len);
		else if (*s == 0x7F || *s == 0x08){
			printf("\b \b");
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
