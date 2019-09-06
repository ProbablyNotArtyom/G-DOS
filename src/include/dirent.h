//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Filesystem control header

#ifndef _HEADER_DIRENT
#define _HEADER_DIRENT

	#include <fs.h>
	#include <sys/types.h>

struct dirent {
	ino_t  d_ino;
	char   d_name[];
};

#endif
