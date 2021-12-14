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

__weak_symbol char * strcasestr (const char *haystack, const char *needle) {
	if (*needle == '\0') {
		return haystack;
	} else {
		char *needle_orig = needle;
		while (*haystack != '\0') {
			int i = 0;
			while (*haystack != '\0' && toLower(*haystack) != toLower(*needle))
				haystack++;		// try and find the first letter of the substring
			while (*(haystack+i) != '\0' && toLower(*(haystack+i)) == toLower(*(needle+i)))
				i++;			// continue to see if the full substring is present
			if (*needle == '\0')
				return haystack;
			else
				needle = needle_orig;
		}
	}
	return NULL;
}
