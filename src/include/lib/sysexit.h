//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Standard exit codes for use with G'DOS shell

#ifndef _HEADER_SYSEXIT
#define _HEADER_SYSEXIT

#define result_t uint8_t

//---------------------------------------------------

enum {
	RET_OK = 0,
	RET_FORMAT,				// Formatting error
	RET_ARGS,				// Invalid arguments
	RET_FERR,				// File error
	RET_SYS,				// Internal DOS error
	RET_CONFIG,				// Configuration invalid
	RET_TEMP,				// ???
	RET_GENERIC,
	RET_BADSTR				// String with no end
};

typedef result_t (*shFunc_t)(char *argv[], int argc);

//---------------------------------------------------
#endif
