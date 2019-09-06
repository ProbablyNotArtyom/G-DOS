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

__weak__ char *strcat(char *dest, const char *src, size_t n){
    char *ptr = dest + strlen(dest);
	while (*src != '\0')
		*ptr++ = *src++;
	*ptr = '\0';
	return ptr;
}
