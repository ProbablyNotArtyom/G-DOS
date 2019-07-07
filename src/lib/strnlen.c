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

size_t strnlen(const char *str, size_t size) {
	size_t len = 0;
	while (*str++ != '\0' && size-- > 0)
		len++;
	return len;
}
