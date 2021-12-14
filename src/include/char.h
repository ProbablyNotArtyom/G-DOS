//---------------------------------------------------
//
//	G-DOS
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Device control header

#ifndef _HEADER_CHARACTER
#define _HEADER_CHARACTER

//---------------------Defines-----------------------

typedef enum {
	CH_OK = 0,		/* 0: Successful */
	CH_ERROR,		/* 1: R/W Error */
	CH_WRPRT,		/* 2: Write Protected */
	CH_NOTRDY,		/* 3: Not Ready */
	CH_PARERR		/* 4: Invalid Parameter */
} charResult;

enum charFuncs {
	DEV_WRITE,
	DEV_READ,
	DEV_INIT
};

#define NORMALIZE_DEV_WRITE		dev_write
#define NORMALIZE_DEV_READ		dev_read
#define NORMALIZE_DEV_INIT		dev_init

#define NORMALIZE(STR) NORMALIZE_ ## STR


//-------------------Type Protos---------------------

struct dev_tty {
	charResult (*init)();
	charResult (*write)(char out);
	char (*read)();
};

#endif
