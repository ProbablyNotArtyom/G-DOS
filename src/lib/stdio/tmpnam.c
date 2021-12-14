//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

#include <stdio.h>
#include <errno.h>

char *tmpnam(char *s) {
	errno = EROFS;
	return NULL;
}
