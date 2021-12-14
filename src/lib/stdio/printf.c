//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#include <stdio.h>
#include <stdarg.h>
#include <std.h>

int printf(const char *format, ...) {
	va_list va;
    va_start(va, format);
	int ret = _bandaid_vprintf(format, va);
	va_end(va);
	return ret;
}

int nprintf(size_t max, const char *format, ...) {
	va_list va;
    va_start(va, format);
	int ret = _bandaid_vnprintf(max, format, va);
	va_end(va);
	return ret;
}
