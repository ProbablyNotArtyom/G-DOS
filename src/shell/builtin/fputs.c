// BUILTIN
// FPUTS
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


result_t shfunc_fputs(int argc, char *argv[]) {
	if (argc != 2) return RET_ARGS;

	FIL file;
	FRESULT res;
	uint32_t bw;

	res = f_open(&file, argv[1], FA_CREATE_ALWAYS | FA_WRITE);
	fs_putsError(res);
	f_write(&file, argv[0], strlen(argv[0]), &bw);
	f_close(&file);

	return RET_OK;
}
