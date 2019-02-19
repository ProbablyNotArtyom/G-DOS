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

#define SH_MKFS_DO_VOLNAME	0x01

//---------------------------------------------------

static const char const mkfs_help_txt[] = {
	"Usage: mkfs [-h][-n] disk\r\n"
	"       [-h show-help][-n volume-name]\r\n"
};

result_t shfunc_mkfs(char *argv[], int argc){
	uint8_t opts = 0x00;				// Create something to log what options are ticked
	for (int i = 0; i < argc; i++){
		if (argv[i][0] == '-'){
			switch (argv[i][1]){
				case 'h':
					puts(mkfs_help_txt);
					return RET_OK;
				case 'n':
					opts = i+1;
					break;
				default:
					return RET_ARGS;
			}
		}
	}

	f_file fil;
    f_error res;
	uint8_t work[512];
	res = f_mkfs(argv[argc-1], FM_ANY | FM_FAT, 0, work, sizeof(work));
	fs_putsError(res);
	if (res == FR_OK){
		if (opts != 0x00){
			FATFS tmpFS;

			memcpy(work, argv[argc-1], strlen(argv[argc-1]));
			work[strlen(argv[argc-1])] = ':';
			memcpy(work+strlen(argv[argc-1])+1, argv[opts], strlen(argv[opts]));
			f_mount(&tmpFS, argv[argc-1], 0);
			res = f_setlabel(work);
			f_mount(0, argv[argc-1], 0);			// Unmount
			fs_putsError(res);
			if (res != FR_OK)
				return RET_SYS;
		}
		return RET_OK;
	}
	return RET_SYS;
}
