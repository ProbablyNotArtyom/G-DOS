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

__weak_symbol size_t mbstowcs(wchar_t *dest, const char *src, size_t max)
{
	size_t total = 0;
	size_t read = 0;
	while(1) {
		int bytes = mbtowc(&dest[total], &src[read], max - read);

		if (bytes < 0) return (size_t)-1;	// Make sure the decoder didnt fucky wucky the data
		if (bytes == 0) {					// Check validi string
			dest[total] = 0;
			break;
		}
		++total;
		read += bytes;
	};
	return total;
}
