//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <stdlib.h>
	#include <errno.h>
	#include <sys/cdefs.h>

//---------------------------------------------------

__weak__ size_t wcstombs(char* dest, const wchar_t* src, size_t max) {
	size_t total = 0;
	if (dest) {
		while (*src && (total + 4) < max) {
			int len = wctomb(&dest[total], *src++);
			if (len < 0) return (size_t)-1;
			total += len;
		}
		while (*src && total < max) {
			char scratch[4];
			int len = wctomb(scratch, *src++);
			if (len < 0 || total + len > max) return (size_t)-1;
			if (len >= 0) dest[total++] = scratch[0];
			if (len >= 1) dest[total++] = scratch[1];
			if (len >= 2) dest[total++] = scratch[2];
			if (len >= 3) dest[total++] = scratch[3];
		}
		if (total < max) {
			*dest = '\0';
		}
	} else {
		// Compute the number of bytes which would be written if we had a
		// destination buffer; we ignore the 'max' value.
		while (*src) {
			char scratch[4];
			if (wctomb(scratch, *src++) < 0) return (size_t)-1;
			total++;
		}
	}
	return total;
}
