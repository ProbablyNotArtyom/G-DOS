// BUILTIN
// MON
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

result_t shfunc_mon(char *argv[], int argc){
	monBegin();
	return RET_OK;
}
