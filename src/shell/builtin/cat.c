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
#define SH_CAT_WIDTH		0x04
#define SH_CAT_HEIGHT		0x08

#define CAT_WIDTH_DEFAULT	80
#define CAT_HEIGHT_DEFAULT	15

//---------------------------------------------------

static const char const cat_help_txt[] = {
	"Usage: cat [-n][-l][-xy] file\r\n"
	"       [-n number-lines][-l less][-x width][-y height]\r\n"
};

result_t shfunc_cat(char *argv[], int argc){
	uint8_t opts = 0x00;					// Create something to log what options are ticked
	uint8_t fnames;							// Track the index where file names begin

	uint32_t width = CAT_WIDTH_DEFAULT;		// Defaults for less mode
	uint32_t height = CAT_HEIGHT_DEFAULT;

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
				case 'x':
					opts |= SH_CAT_WIDTH;
					width = argv[i+1];
					i++;
					break;
				case 'y':
					opts |= SH_CAT_HEIGHT;
					height = argv[i+1];
					i++;
					break;
				default:
					return RET_ARGS;
			}
		} else {
			fnames = i;
			i = argc;
		}
	}

	if (opts & SH_CAT_WIDTH)
		width = strtoul(width, NULL, 10);
	if (opts & SH_CAT_HEIGHT)
		height = strtoul(height, NULL, 10);

	f_error res;
	f_info info;
	while (fnames < argc){
		res = f_stat(argv[fnames], &info);
		if (info.fattrib & AM_DIR){
			nprintf("ignoring %s, is a directory", argv[fnames]);
		} else {
			char buff[512];
			uint32_t line = 0;
			uint32_t nextLine = height;
			uint8_t col = 0;
			f_file file;
			uint32_t bytes = 1;
			res = f_open(&file, argv[fnames], FA_OPEN_ALWAYS | FA_READ);
			fs_putsError(res);
			f_lseek(&file, 0);

			if (opts & SH_CAT_DO_NUMLINES) {
				printf(" %04d | ", line);
				col = 8;
			}
			while (bytes != 0) {
				res = f_read(&file, buff, sizeof(buff), &bytes);
				fs_putsError(res);
				for (int i = 0; i < bytes; i++){
					if (buff[i] == '\n' || col >= width){
						printf("\n");
						line++;
						if (opts & SH_CAT_DO_NUMLINES) {
							printf(" %04d | ", line); col = 8;
						} else {
							col = 0;
						}
						if (opts & SH_CAT_DO_LESS && line >= nextLine){
							char tmp = read();
							if (tmp == ' ') nextLine += height;
							else nextLine += 1;
						}
					} else {
						col++;
						if (col <= width - 2 && buff[i] == '\t') {
							printf("  ");
							col++;
						} else {
							putc(buff[i]);
						}
					}
				}
			}
		}
		fnames++;
	}
	printf("\n");
	return RET_OK;
}
