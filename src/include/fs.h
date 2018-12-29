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

#define	MAX_SECSIZE 512

//-------------------Type Protos---------------------

typedef enum {
	SEEK_TOP = 0,
	SEEK_CUR,
	SEEK_END
} SEEKTYPE;

typedef enum {
	DSK_OK = 0,
	DSK_ERR,
	DSK_PROTECT,
	DSK_NORDY,
	DSK_ARGS
} diskStatus;

enum f_type {
	_ftype_directory,
	_ftype_binary,
	_ftype_hardlink,
	_ftype_symbolic,
	_ftype_blockdev,
	_ftype_chardev
};

typedef enum {
	ER_OK = 0,
	ER_DISK_ERR,
	ER_NOT_READY,
	ER_NO_FILE,
	ER_INVALID_NAME,
	ER_DENIED,
	ER_WRITE_PROTECTED,
	ER_INVALID_DRIVE,
	ER_INVALID_PARAMETER,
	ER_INVALID_FILE
} f_error;

typedef struct {
	struct f_instance *fs;		// Host filesystem
	struct f_dir *parent;		// Directory that contains the file
	uint8_t	flags;
	size_t	size;				// Size of file in bytes
	size_t	ptr;				// Access pointer
	size_t	node;				// Position on disk
	uint8_t	buff[MAX_SECSIZE];	// Data buffer for disk stuff
} f_file;

typedef struct {
	char		name[13];
	size_t		size;
	int16_t		lastTime;
	int16_t		lastDate;
	enum f_type	type;
} f_info;

typedef struct {
	struct f_instance *fs;		// Host filesystem
	size_t		index;			// Cursor position for read/write
	size_t		dirStart;
	size_t		blockPtr;
	size_t		nodePtr;
	char		*fileNames;		// Filename lookup table pointer
} f_dir;

typedef struct {
	uint8_t		drive;				// Drive number
	uint16_t	blockSize;			// Bytes per block
	size_t		lastBlock;			// Newest block
	size_t		freeBlocks;			// Number of free blocks

	size_t		currentDir;			// Current directory
	size_t		blockNum;			// Total number of blocks
	size_t		nodeNum;			// Total number of nodes
	size_t		volume;				// Start of volume
	size_t		superblock;			// Start of superblock
	size_t		rootNode;			// Root directory
	size_t		dataStart;			// Start of data blocks
	size_t		buffSector;			// Sector in buffer
	uint8_t		buff[MAX_SECSIZE];	// Data buffer for table stuff
} f_instance;

struct f_handler {
	f_error (*fs_seek)();
	f_error (*fs_read)();
	f_error (*fs_write)();
	f_error (*fs_openDir)();
	f_error (*fs_closeDir)();
	f_error (*fs_readDir)();
};

//-----------------Function Protos-------------------

void fsInit(void);
f_error f_seek(f_file *file, uint32_t bytes);
f_error f_read(f_file *file, void *buff, size_t bytes);
f_error f_write(f_file *file, const void *buff, size_t bytes, size_t offset);
f_error f_open(f_file *file, const char *path);
f_error f_close(f_file *file);
f_error f_openDir(struct f_dir *dir, const char *path);
f_error f_closeDir(struct f_dir *dir, const char *path);
f_error f_readDir(struct f_dir *dir, f_info info);
f_error f_getCWD(char *buff, uint16_t len);
f_error f_getLabel(const uint8_t disknum, char *label);
f_error f_chdir(const char* path);

#endif
