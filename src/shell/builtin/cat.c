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

extern uint8_t vram_cursor_x;

#define SH_CAT_DO_NUMLINES	0x01
#define SH_CAT_DO_LESS		0x02

//---------------------------------------------------

static const char const cat_help_txt[] = {
	"Usage: cat [-n][-l] file\r\n"
	"       [-n number-lines][-l less]\r\n"
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
				case 'l':
					opts |= SH_CAT_DO_LESS;
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
			uint32_t nextLine = 25;
			uint8_t col = 0;
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
						if (buff[i] == '\n'){
							line++;
							if (opts & SH_CAT_DO_LESS){
								if (nextLine == line){
									char tmp = read();
									if (tmp == ' ') nextLine += 25;
									else nextLine += 1;
								}
							}
							printf("\r\n %d | ", line);
						} else {
							if (col < 80){
								if (buff[i] == '\t'){
									fputs("  ");
									col += 2;
								} else {
									putc(buff[i]);
									col++;
								}
							}
						}
					}
				} else {
					for (int i = 0; i < bytes; i++){
						if (buff[i] == '\n'){
							line++;
							if (opts & SH_CAT_DO_LESS){
								if (nextLine == line){
									char tmp = read();
									if (tmp == ' ') nextLine += 25;
									else nextLine += 1;
								}
							}
							puts("");
						} else {
							if (col < 80){
								if (buff[i] == '\t'){
									fputs("  ");
									col += 2;
								} else {
									putc(buff[i]);
									col++;
								}
							}
						}
					}
				}
			} while (bytes == 512);
			puts("");
		}
		fnames++;
	}

	return RET_OK;
}
