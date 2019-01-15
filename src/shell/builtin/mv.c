// BUILTIN
// MV
//---------------------------------------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <gsh.h>

//---------------------------------------------------

result_t shfunc_mv(char *argv[], int argc){
	#ifdef debug
		puts("shFunc_call: mv");
	#endif
	return RET_OK;
}
