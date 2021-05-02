//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <stdlib.h>
	#include <sys/cdefs.h>

//---------------------------------------------------

__weak_symbol long atol(const char *str) {
	return strtol(str, (char**)NULL, 10);
}
