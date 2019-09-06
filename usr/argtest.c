
	#include <std.h>
	#include <cpu/calling.h>
	#include <string.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <cpu/io.h>
	#include <char.h>

//---------------------------------------------------

result_t main(char *argv[], int argc){

	if (argc == 0) {
		puts("argtest: echoes back each argument it is passed.");
		puts("         this is used to verify that arg passing is working.\n");
		return RET_OK;
	}

	for (uint32_t i = 0; i < argc; i++) {
		if (isAlpha(argv[i]))
			printf("ARG%d: %d\n", i, argv[i]);
		else
			printf("ARG%d: %s\n", i, argv[i]);
	}
}
