
	#include <stddef.h>
	#include <stdarg.h>
	#include <stdbool.h>
	#include <std.h>
	#include <linkage.h>
	#include <syscall.h>

	#include <mod/init.h>

//---------------------------------------------------

	bool __global_flags[] = {
		false		// Global debug flag. changes the behavior of various parts of GDos in order to be more verbose
	};

	const char* const __global_flag_names[] = {
		"debug"
	};

	const size_t __num_global_flags = ARRAY_SIZE(__global_flag_names);

//---------------------------------------------------
