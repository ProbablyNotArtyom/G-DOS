//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Debugging funcions and protos

#ifndef _HEADER_DEBUG_UTILS
#define _HEADER_DEBUG_UTILS

	#include <char.h>
	#include <std.h>

//---------------------Defines-----------------------

#define putc_debug(chr)			if (__global_flags[0] == true) putc(chr)
#define puts_debug(str)			if (__global_flags[0] == true) puts(str)
#define debug_printf(fmt, ...) if (__global_flags[0] == true) printf(fmt, __VA_ARGS__)

extern const char* const __global_flag_names[];
extern const size_t __num_global_flags;

#endif
