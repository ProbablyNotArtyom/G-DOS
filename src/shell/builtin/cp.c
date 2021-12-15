// BUILTIN
// CP
//---------------------------------------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <gsh.h>

#define SH_CP_DO_RECURSIVE	0x01

//---------------------------------------------------

static const char const cp_help_txt[] = {
	"Usage: cp [-r] source dest\r\n"
	"       [-r allow-recursive]\r\n"
};

result_t shfunc_cp(int argc, char *argv[]) {
	uint8_t opts = 0x00;				// Create something to log what options are ticked
	uint8_t fnames;						// Track the index where file names begin
	for (int i = 0; i < argc; i++) {
		if (argv[i][0] == '-') {
			switch (argv[i][1]) {
				case 'h':
					puts(cp_help_txt);
					return RET_OK;
				case 'r':
					opts |= SH_CP_DO_RECURSIVE;
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
	while (fnames < argc) {
		res = f_stat(argv[fnames], &info);
		if (info.fattrib & AM_DIR) {
			nprintf("ignoring %s, is a directory", argv[fnames]);
		} else {

			f_file source, dest;
			res = f_open(&source, argv[fnames], FA_READ);
			fs_putsError(res);
			res = f_open(&dest, argv[fnames+1], FA_CREATE_ALWAYS | FA_WRITE);
			fs_putsError(res);

			res = f_expand(&dest, info.fsize, 1);	// Create a new file of the same size
			fs_putsError(res);
			f_lseek(&source, 0);
			f_lseek(&dest, 0);

			uint8_t buff[512];
			uint32_t bytes, bytes_wr;
			for (int i = 0; i < info.fsize; i += 512) {
				res = f_read(&source, buff, sizeof(buff), &bytes);
				fs_putsError(res);
				res = f_write(&dest, buff, bytes, &bytes_wr);
				fs_putsError(res);
			}
			f_close(&dest);
			f_close(&source);
		}
		fnames += 2;
	}

	return RET_OK;
}
