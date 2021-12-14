//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#include <stdio.h>
#include <stdint.h>

int vsprintf(char *s, const char *format, va_list arg) {
	return vsnprintf(s, SIZE_MAX, format, arg);
}
