
	#include <std.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <io.h>

//---------------------------------------------------

static const char const help_txt[] = {
	"Usage: serpinski [largest tri size]\r\n"
};

result_t main(char *argv[], int argc){
	uint8_t opts = 0x00;				// Create something to log what options are ticked
	uint8_t fnames;						// Track the index where file names begin
	uint32_t num;
	for (int i = 0; i < argc; i++){
		if (argv[i][0] == '-'){
			switch (argv[i][1]){
				case 'h':
					puts(help_txt);
					return RET_OK;
				default:
					return RET_ARGS;
			}
		} else {
			fnames = i;
			i = argc;
		}
	}
	num = strtoul(argv[fnames], NULL, 0);

	while (num > 0){
		uint8_t size = num;
		int x, y, i;
		puts("");
		for (int loop = 0; loop <= size; loop++){
			for (y = (1 << loop) - 1; y >= 0; y--, puts("")) {
				for (i = 0; i < y; i++) putc(' ');
				for (x = 0; x + y < (1 << loop); x++)
				printf((x & y) ? "  " : "* ");
			}
			for (y = 1; y <= (1 << loop); y++, puts("")) {
				for (i = 0; i < y; i++) putc(' ');
				for (x = 0; x + y < (1 << loop); x++)
				printf((x & y) ? "  " : "* ");
			}
		}
		for (int loop = size-1; loop >= 0; loop--){
			for (y = (1 << loop) - 1; y >= 0; y--, puts("")) {
				for (i = 0; i < y; i++) putc(' ');
				for (x = 0; x + y < (1 << loop); x++)
				printf((x & y) ? "  " : "* ");
			}
			for (y = 1; y <= (1 << loop); y++, puts("")) {
				for (i = 0; i < y; i++) putc(' ');
				for (x = 0; x + y < (1 << loop); x++)
				printf((x & y) ? "  " : "* ");
			}
		}
	}
	return RET_OK;
}
