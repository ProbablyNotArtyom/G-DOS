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

void * rawmemchr (const void *block, int c) {
	unsigned int i = 0;
	while (i > 0x00) {
		if (*(unsigned char *)(block + i) == (unsigned char)c)
			return (unsigned char *)(block + i);
		i++;
	}
	return NULL;		// It shouldn't be possible to get here but just in case
}
