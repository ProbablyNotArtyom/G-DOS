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

__weak_symbol size_t strnlen(const char *str, size_t size) {
	size_t len = 0;
	while (*str++ != '\0' && size-- > 0)
		len++;
	return len;
}
