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

#define putc_debug(chr)			if (__global_flags[GLOBAL_FLAG_DEBUG]) putchar(chr)
#define puts_debug(str)			if (__global_flags[GLOBAL_FLAG_DEBUG]) puts(str)
#define debug_printf(fmt, ...) if (__global_flags[GLOBAL_FLAG_DEBUG]) printf(fmt, __VA_ARGS__)

#endif
