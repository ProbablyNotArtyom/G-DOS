// BUILTIN
// FLASH
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

static const char const flash_help_txt[] = {
	"Usage: flash file\r\n"
};

result_t shfunc_flash(char *argv[], int argc){
	uint8_t opts = 0x00;				// Create something to log what options are ticked
	uint8_t fnames;						// Track the index where file names begin
	for (int i = 0; i < argc; i++){
		if (argv[i][0] == '-'){
			switch (argv[i][1]){
				case 'h':
					puts(flash_help_txt);
					return RET_OK;
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
	res = f_stat(argv[fnames], &info);
	fs_putsError(res);
	if (res != RES_OK) return RET_ARGS;
	f_file file;
	res = f_open(&file, argv[fnames], FA_READ);
	fs_putsError(res);

	uint8_t *ptr = (uint8_t *)pmalloc(512);
	uint8_t *ptr2 = (uint8_t *)pmalloc(524288);

	sys_flash_program(&file, info.fsize, ptr, ptr2);

	return RET_OK;
}
