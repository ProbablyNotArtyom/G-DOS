//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <stdlib.h>
	#include <errno.h>
	#include <sys/cdefs.h>

//---------------------------------------------------

__weak_symbol int atoi(const char *str) {
	return (int)strtol(str, (char**)NULL, 10);
}
