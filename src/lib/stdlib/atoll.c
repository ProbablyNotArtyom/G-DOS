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

__weak__ long long atoll(const char *str) {
	return strtoll(str, (char**)NULL, 10);
}
