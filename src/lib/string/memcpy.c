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

__weak_symbol void *memcpy (void *str1, const void *str2, size_t n){
	char *d = str1;
	const char *s = str2;
	while (n--){
		*d++ = *s++;

	}
	return str1;
}
