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

enum f_type {
	_ftype_directory,
	_ftype_binary,
	_ftype_hardlink,
	_ftype_symbolic,
	_ftype_blockdev,
	_ftype_chardev
};

typedef enum {
	ER_OK = 0,				/* Succeeded */
	ER_DISK_ERR,			/* A hard error occurred in the low level disk I/O layer */
	ER_INT_ERR,				/* Assertion failed */
	ER_NOT_READY,			/* The physical drive cannot work */
	ER_NO_FILE,				/* Could not find the file */
	ER_NO_PATH,				/* Could not find the path */
	ER_INVALID_NAME,		/* The path name format is invalid */
	ER_DENIED,				/* Access denied due to prohibited access or directory full */
	ER_WRITE_PROTECTED,		/* The physical drive is write protected */
	ER_INVALID_DRIVE,		/* The logical drive number is invalid */
	ER_NOT_ENABLED,			/* The volume has no work area */
	ER_NO_FILESYSTEM,		/* There is no valid filesystem */
	ER_TIMEOUT,				/* Could not get a grant to access the volume within defined period */
	ER_INVALID_PARAMETER	/* Given parameter is invalid */
} f_error;

struct f_file {
	struct f_instance *fs;		// Host filesystem
	struct f_dir *parent;		// Directory that contains the file
	uint8_t	flags;
	size_t	size;				// Size of file in bytes
	size_t	ptr;				// Access pointer
	size_t	node;				// Position on disk
	uint8_t	buff[MAX_SECSIZE];	// Data buffer for disk stuff
};

struct f_info {
	char		name[13];
	size_t		size;
	int16_t		lastTime;
	int16_t		lastDate;
	enum f_type	type;
};

struct f_dir {
	struct f_instance *fs;		// Host filesystem
	size_t		index;			// Cursor position for read/write
	size_t		dirStart;
	size_t		blockPtr;
	size_t		nodePtr;
	char		*fileNames;		// Filename lookup table pointer
};

struct f_instance {
	uint8_t		drive;			// Drive number
	uint16_t	blockSize;		// Bytes per block
	size_t		lastBlock;		// Newest block
	size_t		freeBlocks;		// Number of free blocks

	size_t		currentDir;		// Current directory
	size_t		blockNum;		// Total number of blocks
	size_t		nodeNum;		// Total number of nodes
	size_t		volume;			// Start of volume
	size_t		superblock;		// Start of superblock
	size_t		rootNode;		// Root directory
	size_t		dataStart;		// Start of data blocks
	size_t		buffSector;			// Sector in buffer
	uint8_t		buff[MAX_SECSIZE];	// Data buffer for table stuff
};

//-----------------Function Protos-------------------

void fsInit(void);
f_error f_seek(struct f_file *file, uint32_t bytes);
f_error f_read(struct f_file *file, void *buff, size_t bytes);
f_error f_write(struct f_file *file, const void *buff, size_t bytes, size_t offset);
f_error f_open(struct f_file *file, const char *path);
f_error f_close(struct f_file *file);
f_error f_openDir(struct f_dir *dir, const char *path);
f_error f_closeDir(struct f_dir *dir, const char *path);
f_error f_readDir(struct f_dir *dir, struct f_info info);
f_error f_getCWD(char *buff, uint16_t len);
f_error f_getLabel(const uint8_t disknum, char *label);
f_error f_chdir(const char* path);

#endif
