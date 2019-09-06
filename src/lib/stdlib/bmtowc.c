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

__weak__ int mbtowc(wchar_t *dest, const char *src, size_t max) {
	if (!src)
		return 0;

	char c = *src;
	if ('\0' == c)		// Input string is empty
		return 0;
	// 0xxxxxxx: 7 bits
	// 110xxxxx 10xxxxxx: 11 bits
	// 1110xxxx 10xxxxxx 10xxxxxx: 16 bits
	// 11110xxx 10xxxxxx 10xxxxxx 10xxxxxx: 21 bits
	// UTF-8 can encode 26 and 32-bit sequences, but those are outside the
	// Unicode range, so we don't accept them.
	switch ((c >> 4) & 0x0F) {
		case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7:
			// 1-byte ascii
			if (max < 1) goto fail;
			*dest = c;
			return 1;
		case 0xC: case 0xD:
			// 2-byte sequence
			if (max < 2) goto fail;
			*dest = ((c & 0x1F) << 6) |
					(src[1] & 0x3F);
			return 2;
		case 0xE:
			// 3-byte sequence
			if (max < 3) goto fail;
			*dest = ((c & 0x0F) << 12) |
					((src[1] & 0x3F) << 6) |
					(src[2] & 0x3F);
			return 3;
		case 0xF:
			// probably a 4-byte sequence, but let's check
			if (c & 0x08) goto fail;
			if (max < 4) goto fail;
			*dest = ((c & 0x07) << 18) |
					((src[1] & 0x3F) << 12) |
					((src[2] & 0x3F) << 6) |
					(src[3] & 0x3F);
			return 4;
		default:
			// must be a continuing character: not legal
			goto fail;
	}
fail:
	errno = EILSEQ;
	return -1;
}
