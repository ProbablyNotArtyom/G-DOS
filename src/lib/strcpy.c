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

char * strcpy (char *to, const char *from) {
	while (*from != '\0')
		*to++ = *from++;
	*to = '\0';
	return to;
}
