//---------------------------------------------------
//
//	G-DOS
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

void *memccpy (void *restrict to, const void *restrict from, int c, size_t size) {
	void *ptr = memchr (from, c, size);
	if (ptr != NULL)
		return memcpy(to, from, ptr-from+1);
	memcpy(to, from, size);
	return NULL;
}
