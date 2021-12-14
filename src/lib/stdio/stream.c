//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#include "stream.h"

struct _stream *_stream_list;

void _init_stream(struct _stream *s) {
	s->next = _stream_list;
	if (s->next) {
		s->next->back = &s->next;
	}
	s->back = &_stream_list;
	_stream_list = s;
}

void _exit_stream(struct _stream *s) {
	*s->back = s->next;
	s->back = &s->next;
	s->next = 0;
}
