//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------

	#include <stdlib.h>
	#include <signal.h>
	#include <sys/cdefs.h>

//---------------------------------------------------

static int func_count = 0;
typedef void (*exitfunc)(void);
static exitfunc exit_funcs[32];

//---------------------------------------------------

__weak_symbol int atexit(exitfunc func) {
	if (func_count >= 32)
		return 1;
	exit_funcs[func_count++] = func;
}

__weak_symbol void exit(int status) {
	while (func_count > 0)
		exit_funcs[--func_count]();
	_exit(status);
}
