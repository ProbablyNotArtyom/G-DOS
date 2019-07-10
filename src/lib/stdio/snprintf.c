//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#include <stdio.h>
#include <stdarg.h>

int snprintf(char *str, size_t max, const char *format, ...) {
	va_list arg;
	va_start(arg, format);
	int ret = vsnprintf(str, max, format, arg);
	va_end(arg);
	return ret;
}
