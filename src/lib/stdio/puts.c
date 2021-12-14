//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#define _DONT_ADD_STD_STREAMS

#include <std.h>
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
	putchar('\r');
	putchar('\n');
	return NULL;
}

int nputs(const char *str, size_t bytes) {
	/*
	int out = fputc(str, stdout);
	if (out < 0) return out;
	if (0 == putchar('\n')) return EOF;
	return out + 1;
	*/

	char *ptr = str;
	while(bytes > 0) {
		putchar(*ptr++);
		bytes--;
	}
	return NULL;
}
