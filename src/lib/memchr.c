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

void * memchr (const void *block, int c, size_t size) {
	for (unsigned int i = 0; i < size; i++) {
		if (*(unsigned char *)(block + i) == (unsigned char)c)
			return (unsigned char *)(block + i);
	}
	return NULL;
}
