//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	14/12/21
//
//---------------------------------------------------
// Global flag support & I/O wrappers

	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	#include <string.h>
	#include <linkage.h>
	#include <syscall.h>
	#include <char.h>

	#include <mod/init.h>
	#include <flags.h>

//---------------------------------------------------

	bool __global_flags[] = {
		false		// Global debug flag. changes the behavior of various parts of GDos in order to be more verbose
	};

	const char* const __global_flag_names[] = {
		"debug"
	};

	const size_t __num_global_flags = ARRAY_SIZE(__global_flag_names);

//---------------------------------------------------

int putchar(int out) {
	asm("");						// Prevent the parent function symbol from being optimized away
	return putc_def(out);
}

int getchar(void) {
	asm("");						// Prevent the parent function symbol from being optimized away
	return (int)read_def();
}

char peek() {
	asm("");						// Prevent the parent function symbol from being optimized away
	return peek_def();
}

charResult tty_init() {
	asm("");						// Prevent the parent function symbol from being optimized away
	return tty_init_def();
}

//---------------------------------------------------
