
	#include <stdarg.h>
	#include <stdlib.h>
	#include <stdbool.h>
	#include <stdio.h>
	#include <assert.h>
	#include <ctype.h>

//------------------------------------------------------------------------------
/* ##### Macros ##### */

// This implements _NARG(...) to test recursive preprocessor macros
// it expands to a number reflecting how many args it's been passed
#define _NARG(...)			_NARG_M1(_CCHECK(__VA_ARGS__),_CCHECK(_COMMA __VA_ARGS__ ()),_NARG_(__VA_ARGS__, _RSEQ_N()))
#define _CCHECK(...)		_NARG_(__VA_ARGS__, _CSEQ_N())
#define _NARG_(...)			_ARG_N(__VA_ARGS__)
#define _NARG_M1(a, b, N)	_NARG_M2(a, b, N)
#define _NARG_M2(a, b, N)	_NARG_M3_##a##b(N)
#define _NARG_M3_01(N)		0
#define _NARG_M3_00(N)		1
#define _NARG_M3_11(N)		N
#define _COMMA()			,
#define _CSEQ_N()			1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0
#define _RSEQ_N()			16,15,14,13,12,11,10,9,8,7,6,5,4,3,2,1,0
#define _ARG_N(				_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,		\
							_11,_12,_13,_14,_15,_16, N, ...)	N

// ANSI Color macros for use with printf/puts
#define C_BLACK		"\e[30m"
#define C_DGRAY		"\e[1;30m"
#define C_RED		"\e[31m"
#define C_LRED		"\e[1;31m"
#define C_GREEN		"\e[32m"
#define C_LGREEN	"\e[1;32m"
#define C_ORANGE	"\e[33m"
#define C_YELLOW	"\e[1;33m"
#define C_BLUE		"\e[34m"
#define C_LBLUE		"\e[1;34m"
#define C_INDIGO	"\e[35m"
#define C_PURPLE	"\e[1;35m"
#define C_TEAL		"\e[36m"
#define C_CYAN		"\e[1;36m"
#define C_LGRAY		"\e[37m"
#define C_WHITE		"\e[1;37m"
#define C_CLR		"\e[0m"

// These print pretty strings for each test case
#define START_TEST(n,c)	printf("[" C_LBLUE n C_CLR " expects " C_ORANGE c C_CLR "]\n -> ")
#define END_TEST(n)		printf(C_CLR "[" C_GREEN n C_CLR "]\n")

//------------------------------------------------------------------------------
int main(int argc, char *argv[]);
int test_loops(void);
int test_args(int argc, char *argv[]);
int test_preprocessor(int argc, char *argv[]);
//------------------------------------------------------------------------------
/* ##### Functions ##### */

int main(int argc, char *argv[]) {
	printf("This is a benchmark for bare-minimum functionality in C compilers.\n\n");
	test_args(argc, argv);
	test_loops();
	test_preprocessor(argc, argv);
	return 0;
}

//------------------------------------------------------------------------------
/* ##### Tests ##### */

int test_loops(void) {
	int iter;

	START_TEST("for loop", "numbers 1-10");
	printf(C_WHITE);
	for (iter = 1; iter <= 10; iter++) printf("%d ", iter);
	END_TEST("done");

	START_TEST("while loop", "numbers 1-10");
	printf(C_WHITE);
	iter = 0;
	while (iter++ < 10) printf("%d ", iter);
	END_TEST("done");

	return 0;
}

int test_args(int argc, char *argv[]) {
	int i;

	START_TEST("argc & argv", "all passed options shown");
	printf("argv = [");
	for (i = 0; i < argc; i++) {
		if (i > 0) printf(", ");
		printf("\"" C_PURPLE "%s" C_CLR "\"", argv[i]);
	}
	printf("] ");
	END_TEST("done");

	return 0;
}

int test_preprocessor(int argc, char *argv[]) {
	int i;

	START_TEST("recursive macro", "correct arg count for each macro");
	printf("_NARG() = " C_PURPLE "%d\n" C_CLR, _NARG());
	printf(" -> _NARG(1) = " C_PURPLE "%d\n" C_CLR, _NARG(0));
	printf(" -> _NARG(1, 2, 3, 4, 5, 6, 7, 8) = " C_PURPLE "%d\n" C_CLR, _NARG(0, 0, 0, 0, 0, 0, 0, 0));
	END_TEST("done");

	return 0;
}
