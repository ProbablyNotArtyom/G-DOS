// BUILTIN
// CAT
//---------------------------------------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <gsh.h>

#define SH_CAT_DO_NUMLINES	0x01

//---------------------------------------------------

static const char const cat_help_txt[] = {
	"Usage: cat [-n] file\r\n"
	"       [-n number-lines]\r\n"
};

result_t shfunc_cat(char *argv[], int argc){
	uint8_t opts = 0x00;				// Create something to log what options are ticked
	uint8_t fnames;						// Track the index where file names begin
	for (int i = 0; i < argc; i++){
		if (argv[i][0] == '-'){
			switch (argv[i][1]){
				case 'h':
					puts(cat_help_txt);
					return RET_OK;
				case 'n':
					opts |= SH_CAT_DO_NUMLINES;
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
		if (info.fattrib & AM_DIR){
			nprintf("ignoring %s, is a directory", argv[fnames]);
		} else {
			char buff[512];
			uint32_t line = 0;
			f_file file;
			uint32_t bytes;
			res = f_open(&file, argv[fnames], FA_OPEN_ALWAYS | FA_READ);
			fs_putsError(res);
			f_lseek(&file, 0);
			do {
				res = f_read(&file, buff, sizeof(buff), &bytes);
				fs_putsError(res);
				if (opts & SH_CAT_DO_NUMLINES){
					printf(" %d | ", line);
					for (int i = 0; i < bytes; i++){
						putc(buff[i]);
						if (buff[i] == '\0'){
							line++;
							printf("\r\n %d | ", line);
						}
					}
				} else {
					putsl(buff, bytes);
				}
			} while (bytes == 512);
			puts("");
		}
		fnames++;
	}

	return RET_OK;
}
