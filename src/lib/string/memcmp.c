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

__weak_symbol int memcmp(const void *p1, const void *p2, size_t n) {
    const unsigned char *l = p1, *r = p2;
    for (; n && *l == *r; n--, l++, r++);
    return n ? *l-*r : 0;
}
