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

__weak__ void * memset (void *block, int c, size_t size) {
	unsigned char* s = block;
	uint32_t* ws = (uint32_t*)s;
	uint32_t wc = c & 0xFF;
	wc |= ((wc << 8) | (wc << 16) | (wc << 24));

	if (!size) return block;
	while (size) {
		*ws = wc;
		ws++;
		size--;
	}
	return block;
}
