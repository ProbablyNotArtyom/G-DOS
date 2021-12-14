//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------


#ifndef _LIBC_INTERNAL_FORMATTER_H
#define _LIBC_INTERNAL_FORMATTER_H

#include <stddef.h>
#include <stdbool.h>
#include <stdarg.h>

// Each call to _format_next will produce a format_chunk, which can be written
// to the output stream or copied to the destination buffer as appropriate.
struct format_chunk
{
	const char *addr;
	size_t size;
};

// The formatter maintains its internal state in this record; don't alter it.
// We need a buffer large enough to contain any formatted number.
#define FORMAT_BUFFER_SIZE 32
struct format_state
{
// public
	// Pointer to the next unread format string character
	const char *fmt;
	// Current string chunk
	struct format_chunk current;
// private
	// Characters which have been produced by the current format specifier but
	// not yet returned from _format_next
	size_t leading_spaces;
	int prefix;
	size_t leading_zeros;
	struct format_chunk body;
	size_t trailing_zeros;
	bool trailing_point;
	size_t trailing_fraction;
	size_t trailing_spaces;

	// Temporary storage for specifiers other than %s
	char buffer[FORMAT_BUFFER_SIZE];
};

void _format_start(struct format_state*, const char *format_string);
bool _format_next(struct format_state*, va_list *arg);

#endif //_LIBC_INTERNAL_FORMATTER_H
