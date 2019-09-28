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

static int bufread(FILE *stream, char *dest, size_t bytes);

//---------------------------------------------------

size_t fread(void *dest, size_t size, size_t count, FILE *stream) {
	size_t bytes = size * count;
	/* Dont do anything when 0 bytes are requested */
	if (bytes == 0) return 0;
	/* Avoid writing to output streams */
	if (stream->state & STREAM_WRITE) {
		stream->state |= STREAM_ERR;
		return 0;
	}

	stream->state |= STREAM_READ;
	int total = 0;
	if (stream->state & STREAM_UNGET) {
		*(unsigned char*)dest = stream->unget;
		dest++;
		stream->state &= ~STREAM_UNGET;
		++total;
		if (0 == --bytes) return total;
	}
	int ret = 0;
	if (stream->buf_size > 0) {
		ret = bufread(stream, dest, bytes);
	} else {
		fflush(0);
		ret = read(stream->id, dest, bytes);
	}
	if (ret < 0) {
		stream->state |= STREAM_ERR;
		return 0;
	} else {
		total += ret;
	}
	if (ret < bytes) {
		stream->state |= STREAM_EOF;
	}
	return total / size;
}

static int bufread(FILE *stream, char *dest, size_t bytes) {
	size_t total = 0;
	char *buf_end = stream->buf_addr + stream->buf_size;
	if (stream->buf_count > 0) {
		// There is data in this input buffer. Copy as much of it into the
		// destination buffer as we can fit.
		size_t space = bytes - total;
		size_t copy = (space > stream->buf_count)? stream->buf_count: space;
		memcpy(dest, buf_end - stream->buf_count, copy);
		stream->buf_count -= copy;
		total += copy;
		dest += copy;
	}
	size_t remaining = bytes - total;
	if (remaining >= stream->buf_size) {
		// We want to read more data than the stream buffer will hold. Read
		// directly from the socket into the destination buffer instead.
		size_t copy = remaining - (remaining % stream->buf_size);
		int ret = read(stream->id, dest, copy);
		if (ret < 0) return ret;
		total += ret;
		dest += ret;
		if (ret < copy) return total;
		remaining = bytes - total;
	}
	if (remaining > 0) {
		// The buffer is empty, but we still need more data. Refill the stream
		// buffer, then copy as much of it into the destination buffer as it
		// can still hold.
		int ret = read(stream->id, stream->buf_addr, stream->buf_size);
		if (ret < 0) return ret;
		stream->buf_count = ret;
		if (ret > 0 && ret < stream->buf_size) {
			memmove(buf_end - ret, stream->buf_addr, ret);
		}
		size_t copy = remaining > ret? ret: remaining;
		memcpy(dest, buf_end - stream->buf_count, copy);
		stream->buf_count -= copy;
		total += copy;
	}
	return total;
}
