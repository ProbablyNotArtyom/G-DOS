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

__weak_symbol int strncmp(const char *s1, const char *s2, size_t size){
	for (int i = 0; i < size; i++)
		while (*s1 == *s2++)
			if (*s1++ == '\0')
				return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)--s2);
}
