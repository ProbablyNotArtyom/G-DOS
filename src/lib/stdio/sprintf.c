//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>

int sprintf(char *str, const char *format, ...) {
	va_list arg;
	va_start(arg, format);
	int ret = vsnprintf(str, SIZE_MAX, format, arg);
	va_end(arg);
	return ret;
}
