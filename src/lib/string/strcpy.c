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

__weak__ char * strcpy (char *to, const char *from) {
	while (*from != '\0')
		*to++ = *from++;
	*to = '\0';
	return to;
}
