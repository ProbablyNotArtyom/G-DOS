// BUILTIN
// CD
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

result_t shfunc_cd(int argc, char *argv[]) {
	if (argc == 0) return RET_ARGS;

	f_error res;
	res = f_chdir(argv[0]);
	fs_putsError(res);

	return RET_OK;
}
