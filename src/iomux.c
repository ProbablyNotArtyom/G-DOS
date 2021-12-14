//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Standard output/input support

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	#include <fs.h>

//---------------------------------------------------

size_t write(int fd, const void *src, size_t bytes) {
	size_t retval;

	if (fd == 1) {
		/* STDOUT */
		nputs(src, bytes);
		retval = bytes;
	} else if (fd == 2) {
		/* STDERR */
		nputs(src, bytes);
		retval = bytes;
	} else {
		/* Acting on a file */
		retval = -1;
	}
	return retval;
}

size_t read(int fd, void *dest, size_t bytes) {
	size_t retval;

	if (fd == 0) {
		/* STDIN */
		gets(dest, bytes);
		retval = bytes;
	} else if (fd == 2) {
		/* STDERR */
		retval = -1;
	} else {
		/* Acting on a file */
		retval = -1;
	}
	return retval;
}
