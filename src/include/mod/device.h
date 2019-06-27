//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Device information header

#ifndef _HEADER_DEVICE
#define _HEADER_DEVICE

	#include <std.h>
	#include <disk.h>
	#include <char.h>

//---------------------Defines-----------------------

typedef enum {
	DEVTYPE_CHAR,
	DEVTYPE_BLOCK
} devtype_t;

//-------------------Type Protos---------------------

struct device_info {
	char			*name;
	char			*vendor;
	devtype_t		type;
	union {
		struct dev_disk	*driver_disk;
		struct dev_tty	*driver_char;
	};
};

#endif
