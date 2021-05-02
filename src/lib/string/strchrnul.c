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

__weak_symbol char * strchrnul (const void *block, int c) {
	unsigned int i;
	for (i = 0; (unsigned char *)(block + i) != '\0'; i++) {
		if (*(unsigned char *)(block + i) == (unsigned char)c)
			return (unsigned char *)(block + i);
	}
	return (unsigned char *)(block + i);
}
