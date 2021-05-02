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

__weak_symbol void *memmove (void *to, const void *from, size_t len) {
	unsigned char *dest = (unsigned char *)to;
	unsigned char *src = (unsigned char *)from;
	if (len == NULL)
		return to;
	if (dest > src) {
		memcpy(dest, src, len);
	} else {
		while (len > 0) {
			*(dest+len) = *(src+len);
			len--;
		}
	}
	return dest;
}
