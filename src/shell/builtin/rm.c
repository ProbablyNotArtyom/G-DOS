// BUILTIN
// RM
//---------------------------------------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <gsh.h>

#define SH_RM_DO_RECURSIVE	0x01

//---------------------------------------------------

static const char const rm_help_txt[] = {
	"Usage: rm [-r] file\r\n"
	"       [-r allow-recursive]\r\n"
};

result_t shfunc_rm(char *argv[], int argc){
	uint8_t opts = 0x00;				// Create something to log what options are ticked
	uint8_t fnames;						// Track the index where file names begin
	for (int i = 0; i < argc; i++){
		if (argv[i][0] == '-'){
			switch (argv[i][1]){
				case 'h':
					puts(rm_help_txt);
					return RET_OK;
				case 'r':
					opts |= SH_RM_DO_RECURSIVE;
					break;
				default:
					return RET_ARGS;
			}
		} else {
			fnames = i;
			i = argc;
		}
	}

	f_error res;
	f_info info;
	while (fnames < argc){
		res = f_stat(argv[fnames], &info);
		if ((info.fattrib & AM_DIR) && !(opts & SH_RM_DO_RECURSIVE)){
			nprintf("ignoring %s, is a directory", argv[fnames]);
		} else {
			res = f_unlink(argv[fnames]);
			fs_putsError(res);
		}
		fnames++;
	}

	return RET_OK;
}
