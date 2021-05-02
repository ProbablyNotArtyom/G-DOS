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

__weak_symbol void *memchr(const void *buf, int c, size_t size) {
	const unsigned char *src = buf;
	while (size--) {
		if (*src == (unsigned char)c) {
			return (void*)src;
		}
		src++;
	}
	return NULL;
}
