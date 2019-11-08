// BUILTIN
// EXPR
//---------------------------------------------------

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>

	#include <disk.h>
	#include <fs.h>
	#include <lib/sysexit.h>
	#include <gsh.h>

extern double tcl_interp_expr(const char *expression, int *error);
//---------------------------------------------------

result_t shfunc_expr(char *argv[], int argc) {
	if (argc == 0) return RET_ARGS;
	double expr_ret = tcl_interp_expr(argv[0], 0);

	if (expr_ret == (1.0/0.0)) printf("infinity\n");
	else printf("%d\n", (int)expr_ret);

	return RET_OK;
}
