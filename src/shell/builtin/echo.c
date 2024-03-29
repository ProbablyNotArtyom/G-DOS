// BUILTIN
// ECHO
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

result_t shfunc_echo(int argc, char *argv[]) {
	if (argc == 0) return RET_ARGS;
	for (int i = 0; i < argc; i++) {
		printf(argv[i]);
		putchar(' ');
	}
	puts("");
	return RET_OK;
}
