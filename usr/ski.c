
	#include <std.h>
	#include <cpu/calling.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <cpu/io.h>
	#include <char.h>

//---------------------------------------------------

static const char const brot_help_txt[] = {
	"Renders a serpinski triangle of a specific size to the terminal\r\n"
	"Usage: serpinski [-d] [-p] [size] \r\n"
	"\r\n"
	"    -d           generates a serpinski diamond instead of a serpinski triangle\r\n"
	"    -p           progressive mode; a serpinski will be generated for every size \r\n"
	"                     until the input size\r\n"
};

result_t main(char *argv[], int argc){
	uint8_t opts = 0x00;				// Create something to log what options are ticked
	uint8_t size = 4;
	bool diamond = false, progressive = false;
	
	if (argc == 0) {
		puts(brot_help_txt);
		return RET_OK;
	}
	for (int i = 0; i < argc; i++){
		if (argv[i][0] == '-'){
			switch (argv[i][1]){
				default:
				case 'h':
					puts(brot_help_txt);
					return RET_OK;
				case 'd':
					diamond = true;
					break;
				case 'p':
					progressive = true;
					break;
			}
		} else {
			size = strtoul(argv[i], NULL, 10);
		}
	}

	puts("");
	int x, y, i;
	uint16_t loop_count = 1;
	if (!progressive) loop_count = size;
	while (loop_count <= size) {
		for (y = (1 << loop_count) - 1; y >= 0; y--, puts("")) {
			for (i = 0; i < y; i++) putc(' ');
			for (x = 0; x + y < (1 << loop_count); x++)
			printf((x & y) ? "  " : "* ");
		}
		if (diamond) {
			for (y = 1; y <= (1 << loop_count); y++, puts("")) {
				for (i = 0; i < y; i++) putc(' ');
				for (x = 0; x + y < (1 << loop_count); x++)
				printf((x & y) ? "  " : "* ");
			}
		}
		loop_count++;
	}
}
