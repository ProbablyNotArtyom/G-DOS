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

__weak_symbol void * strrchr (const void *block, int c) {
	for (int i = strlen(block); i >= 0; i--) {
		if (*(unsigned char *)(block + i) == (unsigned char)c)
			return (unsigned char *)(block + i);
	}
	return NULL;
}
