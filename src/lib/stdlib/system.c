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

__weak__ int system(const char *command) {

	// We dont really support multiprocessing yet
	// So just return 0
	// If command isn't null, then return -1  because we are unable to execute it
	return command? -1: 0;
}
