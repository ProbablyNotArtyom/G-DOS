//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#include <stdio.h>

int puts(const char *str) {
	/*
	int out = fputc(str, stdout);
	if (out < 0) return out;
	if (0 == putchar('\n')) return EOF;
	return out + 1;
	*/
	
	char *ptr = str;
	while(*ptr != NULL) putchar(*ptr++);
	putchar('\n');
	return NULL;
}
