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

void * memmem (const void *haystack, size_t haystack_len, const void *needle, size_t needle_len) {
	char *needle_orig = needle;
	char *haystack_end = haystack + haystack_len-1;
	while (haystack <= haystack_end) {
		int i = 0;
		while (haystack <= haystack_end && *(unsigned char *)haystack != *(unsigned char *)needle)
			haystack++;		// try and find the first letter of the substring
		while (haystack <= haystack_end && *(unsigned char *)(haystack+i) == *(unsigned char *)(needle+i))
			i++;			// continue to see if the full substring is present
		if (needle == needle + needle_len)
			return haystack;
		else
			needle = needle_orig;
	}
	return NULL;
}
