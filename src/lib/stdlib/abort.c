//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <stdlib.h>
	#include <signal.h>
	#include <sys/cdefs.h>

//---------------------------------------------------

__weak_symbol void abort(void) {
	raise(SIGABRT);
	_exit(EXIT_FAILURE);
}
