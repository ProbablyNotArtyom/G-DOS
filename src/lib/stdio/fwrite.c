//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#include "stream.h"
#include <errno.h>
#include <string.h>

static int bufwrite(FILE *stream, const char *src, size_t bytes);
static int linewrite(FILE *stream, const char *src, size_t bytes);

//---------------------------------------------------

size_t fwrite(const void *src, size_t size, size_t count, FILE *stream) {
	size_t bytes = size * count;
	/* Dont do anything when 0 bytes are requested */
	if (bytes == 0) return 0;
	/* Avoid writing to input streams */
	if (stream->state & STREAM_READ) {
		stream->state |= STREAM_ERR;
		return 0;
	}

	stream->state |= STREAM_WRITE;
	int retval = 0;
	if (stream->buf_size > 0) {
		if (stream->state & STREAM_LINESYNC) {
			retval = linewrite(stream, src, bytes);
		} else {
			retval = bufwrite(stream, src, bytes);
		}
	} else {
		retval = write(stream->id, src, bytes);
	}
	if (retval < bytes) {
		stream->state |= STREAM_ERR;
		return 0;
	}
	return (retval / size);
}

static int bufwrite(FILE *stream, const char *src, size_t bytes) {
	size_t total = 0;
	if (stream->buf_count > 0 && stream->buf_size > stream->buf_count) {
		// There's data in the stream buffer, so we need to write it out before
		// any of the new stuff we're adding, but there's still space in the
		// buffer. Copy in as much of the source data as will fit.
		size_t remain = bytes - total;
		size_t avail = stream->buf_size - stream->buf_count;
		size_t copy = (avail > remain)? remain: avail;
		memcpy(stream->buf_addr + stream->buf_count, src, copy);
		stream->buf_count += copy;
		src += copy;
		total += copy;
	}
	if (stream->buf_count == stream->buf_size) {
		// The stream buffer is full, so we have to flush it before we can
		// queue up any more outgoing data.
		int expect = stream->buf_size;
		stream->buf_count = 0;
		int ret = write(stream->id, stream->buf_addr, expect);
		if (ret < 0) return ret;
		if (ret < expect) return total;
	}
	if (bytes - total >= stream->buf_size) {
		// The data remaining to write is bigger than the stream buffer, so
		// we might as well copy it directly out from the source buffer rather
		// than wasting time copying it into the stream buffer first.
		size_t remaining = bytes - total;
		size_t copy = remaining - (remaining % stream->buf_size);
		int ret = write(stream->id, src, copy);
		if (ret < 0) return ret;
		total += ret;
		src += ret;
		if (ret < copy) return total;
	}
	if (bytes > total) {
		// We have some bytes left, not enough to fill the stream buffer. Copy
		// them in and leave them to write out next time.
		size_t avail = stream->buf_size - stream->buf_count;
		size_t remain = bytes - total;
		size_t copy = (avail > remain)? remain: avail;
		memcpy(stream->buf_addr + stream->buf_count, src, copy);
		stream->buf_count += copy;
		src += copy;
		total += copy;
	}
	return bytes;
}

static int linewrite(FILE *stream, const char *src, size_t bytes) {
	size_t total = 0;
	char *dest = stream->buf_addr + stream->buf_count;
	while (total < bytes) {
		if (stream->buf_count < stream->buf_size) {
			char c = *src++;
			*dest++ = c;
			stream->buf_count++;
			total++;
			if (c != '\n') continue;
		}
		int ret = write(stream->id, stream->buf_addr, stream->buf_count);
		if (ret < 0) return ret;
		if (ret < stream->buf_count) break;
		stream->buf_count = 0;
		dest = stream->buf_addr;
	}
	return total;
}
