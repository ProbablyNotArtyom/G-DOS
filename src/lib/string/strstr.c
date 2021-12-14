//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <string.h>
	#include <sys/cdefs.h>

//---------------------------------------------------

__weak_symbol char * strstr (const char *haystack, const char *needle) {
	if (*needle == '\0') {
		return haystack;
	} else {
		char *needle_orig = needle;
		while (*haystack != '\0') {
			int i = 0;
			while (*haystack != '\0' && *haystack != *needle)
				haystack++;		// try and find the first letter of the substring
			while (*(haystack+i) != '\0' && *(haystack+i) == *(needle+i))
				i++;			// continue to see if the full substring is present
			if (*needle == '\0')
				return haystack;
			else
				needle = needle_orig;
		}
	}
	return NULL;
}
