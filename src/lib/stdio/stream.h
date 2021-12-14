//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#ifndef _INTERNAL_HEADER_STREAM
#define _INTERNAL_HEADER_STREAM

#include <stdio.h>

#define STREAM_EOF 0x01 		// C standard feof() indicator
#define STREAM_ERR 0x02 		// C standard ferror() indicator
#define STREAM_ALLOC 0x04		// we own the buffer and must free() it on close
#define STREAM_LINESYNC 0x08 	// must flush buffer after each newline
#define STREAM_READ 0x10 		// currently in read mode
#define STREAM_WRITE 0x20 		// currently in write mode
#define STREAM_UNGET 0x40 		// has a character in the unget buffer

struct _stream {
	struct _stream *next;	// all the active streams
	struct _stream **back;
	unsigned state;
	int id;
	char *buf_addr;
	size_t buf_size;
	size_t buf_count;		// number of data bytes contained in the buffer
	int unget;				// unget character, if present
};

extern struct _stream *_stream_list;
void _init_stream(struct _stream*);
void _exit_stream(struct _stream*);

#endif
