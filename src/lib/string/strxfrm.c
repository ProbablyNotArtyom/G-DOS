//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <string.h>
	#include <sys/cdefs.h>

//---------------------------------------------------

__weak_symbol size_t strxfrm(char *dest, const char *src, size_t n) {
	size_t out = 0;
	while (n--) {
		if ((*dest++ = *src++))
			++out;
		else
			return out;
	}
	while (*src) {
		++src;
		++out;
	}
	return out;
}
