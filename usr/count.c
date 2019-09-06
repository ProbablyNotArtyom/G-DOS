
	#include <std.h>
	#include <cpu/calling.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <cpu/io.h>
	#include <char.h>

//---------------------------------------------------

static const char const help_txt[] = {
	"Count counts to a number starting from one\r\n"
	"Usage: count [number to count to]\r\n"
};


result_t main(char *argv[], int argc){
	uint8_t opts = 0x00;				// Create something to log what options are ticked
	uint8_t numIndex;
	uint32_t num;

	if (argc == 0) {
		puts(help_txt);
		return RET_OK;
	}
	
	for (int i = 0; i < argc; i++){
		if (argv[i][0] == '-'){
			switch (argv[i][1]){
				default:
				case 'h':
					puts(help_txt);
					return RET_OK;
			}
		} else {
			numIndex = i;
			i = argc;
		}
	}

	num = strtoul(argv[numIndex], NULL, 10);

	for (uint32_t i = 1; i <= num; i++) {
		printf(">> %d\r\n", i);
	}
}
