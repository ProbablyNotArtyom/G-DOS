//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#include <stdio.h>
#include <errno.h>

FILE *tmpfile(void) {
	errno = EROFS;
	return NULL;
}
