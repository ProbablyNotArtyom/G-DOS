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

char * strncpy (char *restrict to, const char *restrict from, size_t size) {
	char *to_orig = to;
	while (*from != '\0' && size-- > 0)
		*to++ = *from++;
	*to = '\0';
	return to_orig;
}
