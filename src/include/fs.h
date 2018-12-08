//---------------------------------------------------
//
//	GBoot v0.0
//	NotArtyom
//	02/11/18
//
//---------------------------------------------------
// Filesystem control header

#ifndef _HEADER_FSTOOLS
#define _HEADER_FSTOOLS

//-------------------Type Protos---------------------

typedef struct fsFile(*FSquery)(char *fname);
typedef struct fsFile(*FSexists)(char *fname);
typedef struct fsFile(*FSload)(char *fname);
typedef int(*FSchdir)(char *fname);

enum fType {
	_ftype_directory,
	_ftype_binary,
	_ftype_hardlink,
	_ftype_symbolic,
	_ftype_blockdev,
	_ftype_chardev
};

//-----------------Variable Protos-------------------

struct fsDisk {
	char label[16];
	uint32_t size;
	FSquery query;
	FSexists exists;
	FSload load;
	FSchdir chdir;
};

struct fsFile {
	char name[16];
	uint32_t size;
	uint32_t type;
	uint32_t *data;
};

struct fsDisk diskTable[4];

uint8_t currentDisk;
uint8_t nextDisk;

//-----------------Function Protos-------------------

void fsInit(void);
struct fsFile fsQuery(uint32_t index);
struct fsFile fsLoad(char *fname);
struct fsFile fsExists(char *fname);
int fdChDisk(int num);

#endif