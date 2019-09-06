//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#include <stdio.h>
#include <string.h>
#include "formatter.h"

int vsnprintf(char *s, size_t max, const char *format, va_list arg) {
	int written = 0;
	struct format_state state;
	_format_start(&state, format);
	while (_format_next(&state, &arg)) {
		struct format_chunk blob = state.current;
		size_t copy = (max >= written + blob.size)? blob.size: (max - written);
		strncpy(&s[written], blob.addr, copy);
		written += copy;
		if (copy < blob.size) break;
	}
	return written;
}
