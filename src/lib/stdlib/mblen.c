//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <stdlib.h>
	#include <sys/cdefs.h>

//---------------------------------------------------

__weak_symbol int mblen(const char *src, size_t max) {
	wchar_t dummy;
	return mbtowc(&dummy, src, max);
}
