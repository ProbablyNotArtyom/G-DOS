//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#include "stream.h"
#include <errno.h>

static int flushwrite(FILE *stream) {
	/* Write any unwritten data in teh buffer if stream is an output */
	int ret = 0;
	if (stream->buf_count > 0) {
		ret = write(stream->id, stream->buf_addr, stream->buf_count);
		if (ret < stream->buf_count) {
			stream->state |= STREAM_ERR;
			ret = EOF;
		}
		stream->buf_count = 0;
	}
	stream->state &= ~STREAM_WRITE;
	return ret;
}

static int flushread(FILE *stream) {
	/* Clear the stream state */
	stream->state &= ~(STREAM_READ | STREAM_UNGET);
	return 0;
}

int fflush(FILE *stream) {
	/* Flush the specified output stream */
	/* If stream == NULL, then flush all streams */
	if (stream) {
		if (stream->state & STREAM_WRITE) return flushwrite(stream);
		if (stream->state & STREAM_READ) return flushread(stream);
		stream->buf_count = 0;
	} else for (stream = _stream_list; stream; stream = stream->next) {
		if (stream->state & STREAM_WRITE) {
			int ret = flushwrite(stream);
			if (ret != 0) return ret;
		}
	}
	return 0;
}
