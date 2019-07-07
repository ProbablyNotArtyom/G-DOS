//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <std.h>
	#include <string.h>
	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>

//---------------------------------------------------

char *strncat(char *dest, const char *src, size_t n){
    char *ret = dest;
    while (*dest) dest++;
    while (n--) if (!(*dest++ = *src++)) return ret;
    *dest = 0;
    return ret;
}
